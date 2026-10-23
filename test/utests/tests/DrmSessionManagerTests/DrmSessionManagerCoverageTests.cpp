/*
 * Coverage-focused tests for drm/DrmSessionManager.cpp.
 * Uses injected DrmSessionCreator and mocked DrmHelper/DrmSession objects.
 * The ContentSecurityManagerSession fake never reports a valid session, so
 * these tests cover the invalid-session (no CSM forwarding) branches.
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>
#include <mutex>
#include <string>
#include <vector>
#include "DrmSessionManager.h"
#include "DrmSession.h"
#include "DrmUtils.h"
#include "MockDrmHelper.h"
#include "MockDrmSession.h"

using ::testing::_;
using ::testing::Invoke;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::ReturnRef;

// Defined in drm/DrmSessionManager.cpp; not declared in any header.
std::string _extractSubstring(std::string parentStr, std::string startStr, std::string endStr);

namespace {

const std::string kWidevine = "com.widevine.alpha";
const std::string kClearKey = "org.w3.clearkey";
const std::vector<uint8_t> kKeyA = {0x01, 0x02, 0x03, 0x04};
const std::vector<uint8_t> kKeyB = {0x0A, 0x0B, 0x0C, 0x0D};

class CovSessionManager : public DrmSessionManager
{
public:
    CovSessionManager(int maxSessions, DrmSessionCreator creator)
        : DrmSessionManager(maxSessions, nullptr, nullptr, std::move(creator)) {}

    using DrmSessionManager::cachedKeyIDs;
    using DrmSessionManager::cachedKeyMutex;
};

std::shared_ptr<NiceMock<MockDrmHelper>> makeHelper(const std::string &systemId, const std::vector<uint8_t> &key)
{
    auto helper = std::make_shared<NiceMock<MockDrmHelper>>();
    ON_CALL(*helper, ocdmSystemId()).WillByDefault(ReturnRef(systemId));
    ON_CALL(*helper, getKey(_)).WillByDefault(Invoke([key](std::vector<uint8_t> &k) { k = key; }));
    return helper;
}

DrmCallbacks *sentinelInstance()
{
    static char sentinel = 0;
    return reinterpret_cast<DrmCallbacks *>(&sentinel);
}

class CovDrmSessionManagerTest : public ::testing::Test
{
protected:
    static constexpr int kMaxSessions = 3;

    int creatorCalls = 0;
    NiceMock<MockDrmSession> *createdSession = nullptr;
    KeyState createdFirstState = KEY_INIT;
    KeyState createdLaterState = KEY_INIT;
    int acquireCalls = 0;
    int acquiredSlot = -1;
    std::unique_ptr<CovSessionManager> mgr;

    void SetUp() override
    {
        mgr = std::make_unique<CovSessionManager>(kMaxSessions, makeCreator());
        mgr->setSessionMgrState(eSESSIONMGR_ACTIVE);
        mgr->RegisterProfilingUpdateCb([]() {});
        mgr->RegisterHandleContentProtectionCb(
            [](DrmHelperPtr, int, const std::vector<uint8_t> &, int) { return std::string("cdm-data"); });
    }

    void TearDown() override
    {
        mgr.reset();
    }

    DrmSessionCreator makeCreator()
    {
        return [this](DrmHelperPtr, DrmCallbacks *) -> std::unique_ptr<DrmSession>
        {
            ++creatorCalls;
            auto *session = new NiceMock<MockDrmSession>(kWidevine);
            EXPECT_CALL(*session, getState())
                .WillOnce(Return(createdFirstState))
                .WillRepeatedly(Return(createdLaterState));
            createdSession = session;
            return std::unique_ptr<DrmSession>(session);
        };
    }

    NiceMock<MockDrmSession> *makeSession(const std::string &keySystem, KeyState state)
    {
        auto *session = new NiceMock<MockDrmSession>(keySystem);
        ON_CALL(*session, getState()).WillByDefault(Return(state));
        return session;
    }

    // Manager takes ownership of session.
    void install(int slot, NiceMock<MockDrmSession> *session, const std::vector<uint8_t> &keyId)
    {
        mgr->drmSessionContexts[slot].drmSession = session;
        mgr->drmSessionContexts[slot].data = keyId;
    }

    void cacheKey(int slot, const std::vector<uint8_t> &keyId, bool failed = false)
    {
        std::lock_guard<std::mutex> guard(mgr->cachedKeyMutex);
        KeyIdEntry entry;
        entry.keyId = keyId;
        entry.isFailedKeyId = failed;
        mgr->cachedKeyIDs[slot].data.push_back(entry);
    }

    void setAcquireResult(KeyState result)
    {
        mgr->RegisterLicenseDataCb(
            [this, result](int &, DrmHelperPtr, int slot, int &, GstMediaType, void *, bool)
            {
                ++acquireCalls;
                acquiredSlot = slot;
                return result;
            });
    }
};

constexpr int CovDrmSessionManagerTest::kMaxSessions;

} // namespace

/* ---------------- Session and cache cleanup ---------------- */

TEST_F(CovDrmSessionManagerTest, ClearSessionData_ReleasesSessionsAndCachedKeys)
{
    install(0, makeSession(kWidevine, KEY_READY), kKeyA);
    cacheKey(0, kKeyA);

    mgr->clearSessionData();

    EXPECT_EQ(mgr->drmSessionContexts[0].drmSession, nullptr);
    EXPECT_TRUE(mgr->cachedKeyIDs[0].data.empty());
}

TEST_F(CovDrmSessionManagerTest, ClearDrmSession_ForceClearsAllPopulatedSlots)
{
    install(0, makeSession(kWidevine, KEY_READY), kKeyA);
    install(1, makeSession(kWidevine, KEY_READY), kKeyB);

    mgr->clearDrmSession(true);

    EXPECT_EQ(mgr->drmSessionContexts[0].drmSession, nullptr);
    EXPECT_EQ(mgr->drmSessionContexts[1].drmSession, nullptr);
}

TEST_F(CovDrmSessionManagerTest, ClearDrmSession_ClearsOnlyFailedSlotWhenNotForced)
{
    NiceMock<MockDrmSession> *healthy = makeSession(kWidevine, KEY_READY);
    install(0, healthy, kKeyA);
    install(1, makeSession(kWidevine, KEY_READY), kKeyB);
    mgr->cachedKeyIDs[1].isFailedKeyEntries = true;

    mgr->clearDrmSession(false);

    EXPECT_EQ(mgr->drmSessionContexts[0].drmSession, healthy);
    EXPECT_EQ(mgr->drmSessionContexts[1].drmSession, nullptr);
}

TEST_F(CovDrmSessionManagerTest, ClearFailedKeyIds_ResetsFailedAndPrimaryEntries)
{
    cacheKey(0, kKeyA);
    mgr->cachedKeyIDs[0].isFailedKeyEntries = true;
    mgr->cachedKeyIDs[0].creationTime = 99;
    cacheKey(1, kKeyB);
    mgr->cachedKeyIDs[1].isPrimaryKeyId = true;
    mgr->cachedKeyIDs[1].creationTime = 55;

    mgr->clearFailedKeyIds();

    EXPECT_TRUE(mgr->cachedKeyIDs[0].data.empty());
    EXPECT_FALSE(mgr->cachedKeyIDs[0].isFailedKeyEntries);
    EXPECT_EQ(mgr->cachedKeyIDs[0].creationTime, 0);
    EXPECT_FALSE(mgr->cachedKeyIDs[1].isPrimaryKeyId);
    EXPECT_EQ(mgr->cachedKeyIDs[1].data.size(), 1u);
    EXPECT_EQ(mgr->cachedKeyIDs[1].creationTime, 55);
}

TEST_F(CovDrmSessionManagerTest, GetFailedKeyIdStatus_ReturnsSlotFlagAndRejectsOutOfRange)
{
    mgr->cachedKeyIDs[1].isFailedKeyEntries = true;

    EXPECT_FALSE(mgr->getFailedKeyIdStatus(0));
    EXPECT_TRUE(mgr->getFailedKeyIdStatus(1));
    EXPECT_FALSE(mgr->getFailedKeyIdStatus(-1));
    EXPECT_FALSE(mgr->getFailedKeyIdStatus(kMaxSessions));
}

TEST_F(CovDrmSessionManagerTest, IsKeyIdProcessed_CachedValidKey_ReturnsTrueWithStatusTrue)
{
    cacheKey(2, kKeyA, false);
    bool status = false;

    EXPECT_TRUE(mgr->IsKeyIdProcessed(kKeyA, status));
    EXPECT_TRUE(status);
}

TEST_F(CovDrmSessionManagerTest, IsKeyIdProcessed_CachedFailedKey_ReturnsTrueWithStatusFalse)
{
    cacheKey(0, kKeyA, true);
    bool status = true;

    EXPECT_TRUE(mgr->IsKeyIdProcessed(kKeyA, status));
    EXPECT_FALSE(status);
}

TEST_F(CovDrmSessionManagerTest, IsKeyIdProcessed_UnknownKey_ReturnsFalseAndLeavesStatus)
{
    cacheKey(0, kKeyA);
    bool status = true;

    EXPECT_FALSE(mgr->IsKeyIdProcessed(kKeyB, status));
    EXPECT_TRUE(status);
}

TEST_F(CovDrmSessionManagerTest, GetSlotIdForSession_ReturnsOwningSlotOrMinusOne)
{
    NiceMock<MockDrmSession> *owned = makeSession(kWidevine, KEY_READY);
    install(1, owned, kKeyA);
    NiceMock<MockDrmSession> stranger(kWidevine);

    EXPECT_EQ(mgr->getSlotIdForSession(owned), 1);
    EXPECT_EQ(mgr->getSlotIdForSession(&stranger), -1);
}

TEST_F(CovDrmSessionManagerTest, ValidateMultiKeySlot_InvalidSlotOrNoSession_ReturnsFalse)
{
    EXPECT_FALSE(mgr->ValidateMultiKeySlot(kKeyA, -1));
    EXPECT_FALSE(mgr->ValidateMultiKeySlot(kKeyA, kMaxSessions));
    EXPECT_FALSE(mgr->ValidateMultiKeySlot(kKeyA, 0));
}

/* ---------------- Session manager state and configuration ---------------- */

TEST_F(CovDrmSessionManagerTest, SetSessionMgrState_RoundTrips)
{
    mgr->setSessionMgrState(eSESSIONMGR_INACTIVE);
    EXPECT_EQ(mgr->getSessionMgrState(), eSESSIONMGR_INACTIVE);

    mgr->setSessionMgrState(eSESSIONMGR_ACTIVE);
    EXPECT_EQ(mgr->getSessionMgrState(), eSESSIONMGR_ACTIVE);
}

TEST_F(CovDrmSessionManagerTest, UpdateDRMConfig_StoresAllFlags)
{
    mgr->UpdateDRMConfig(true, false, true, false, true);

    EXPECT_TRUE(mgr->m_drmConfigParam->mUseSecManager);
    EXPECT_FALSE(mgr->m_drmConfigParam->mEnablePROutputProtection);
    EXPECT_TRUE(mgr->m_drmConfigParam->mPropagateURIParam);
    EXPECT_FALSE(mgr->m_drmConfigParam->mIsFakeTune);
    EXPECT_TRUE(mgr->m_drmConfigParam->mIsWVKIDWorkaround);
}

TEST_F(CovDrmSessionManagerTest, UpdateMaxDRMSessions_SameSize_KeepsSessions)
{
    NiceMock<MockDrmSession> *session = makeSession(kWidevine, KEY_READY);
    install(0, session, kKeyA);

    mgr->UpdateMaxDRMSessions(kMaxSessions);

    EXPECT_EQ(mgr->drmSessionContexts[0].drmSession, session);
}

TEST_F(CovDrmSessionManagerTest, UpdateMaxDRMSessions_DifferentSize_ClearsAndResizes)
{
    install(0, makeSession(kWidevine, KEY_READY), kKeyA);
    cacheKey(0, kKeyA);

    mgr->UpdateMaxDRMSessions(5);

    EXPECT_EQ(mgr->drmSessionContexts[0].drmSession, nullptr);
    EXPECT_EQ(mgr->drmSessionContexts[4].drmSession, nullptr);
    bool status = false;
    EXPECT_FALSE(mgr->IsKeyIdProcessed(kKeyA, status));
}

/* ---------------- Video and watermark state ---------------- */

TEST_F(CovDrmSessionManagerTest, SetVideoWindowSize_InvalidSession_DoesNotForward)
{
    EXPECT_FALSE(mgr->mContentSecurityManagerSession.isSessionValid());

    mgr->setVideoWindowSize(1920, 1080);

    EXPECT_FALSE(mgr->mContentSecurityManagerSession.isSessionValid());
}

TEST_F(CovDrmSessionManagerTest, SetVideoMute_StoresMuteStateWithoutSession)
{
    mgr->setVideoMute(false, 0.0, false, 0.0, true, 0.0);
    EXPECT_TRUE(mgr->mIsVideoOnMute.load());

    mgr->mCurrentSpeed.store(2);
    mgr->setVideoMute(false, 0.0, false, 0.0, false, 500.0);
    EXPECT_FALSE(mgr->mIsVideoOnMute.load());
}

TEST_F(CovDrmSessionManagerTest, HideWatermarkOnDetach_ClearsFirstFrameLatch)
{
    mgr->mFirstFrameSeen.store(true);

    mgr->hideWatermarkOnDetach();

    EXPECT_FALSE(mgr->mFirstFrameSeen.load());
}

TEST_F(CovDrmSessionManagerTest, SetPlaybackSpeedState_LatchesFirstFrameAndRecordsSpeed)
{
    mgr->setPlaybackSpeedState(false, 0.0, false, 0.0, 2, 1000.0, true);
    EXPECT_TRUE(mgr->mFirstFrameSeen.load());
    EXPECT_EQ(mgr->mCurrentSpeed.load(), 2);

    // Latch stays set when firstFrameSeen is false on later calls.
    mgr->setPlaybackSpeedState(true, 150.0, true, 0.0, 1, 0.0, false);
    EXPECT_TRUE(mgr->mFirstFrameSeen.load());
    EXPECT_EQ(mgr->mCurrentSpeed.load(), 1);
}

TEST_F(CovDrmSessionManagerTest, SetPlaybackSpeedState_NoLatchWithoutFirstFrame_MutedPath)
{
    mgr->setVideoMute(false, 0.0, false, 0.0, true, 0.0);

    mgr->setPlaybackSpeedState(false, 0.0, false, 0.0, 0, 0.0, false);

    EXPECT_FALSE(mgr->mFirstFrameSeen.load());
    EXPECT_EQ(mgr->mCurrentSpeed.load(), 0);
}

TEST_F(CovDrmSessionManagerTest, NotifyCleanup_WithoutValidSession_LeavesStateUntouched)
{
    mgr->mCurrentSpeed.store(3);
    mgr->mFirstFrameSeen.store(true);

    mgr->notifyCleanup();

    EXPECT_EQ(mgr->mCurrentSpeed.load(), 3);
    EXPECT_TRUE(mgr->mFirstFrameSeen.load());
}

/* ---------------- String helper ---------------- */

TEST(DrmSessionManagerCoverageStringTest, ExtractSubstring_ReturnsTextBetweenDelimiters)
{
    EXPECT_EQ(_extractSubstring("abc<X>def", "<", ">"), "X");
}

TEST(DrmSessionManagerCoverageStringTest, ExtractSubstring_MissingDelimitersReturnsEmpty)
{
    EXPECT_EQ(_extractSubstring("abcdef", "<", ">"), "");
    EXPECT_EQ(_extractSubstring("<abc", "<", ">"), "");
}

/* ---------------- initializeDrmSession error mapping ---------------- */

TEST_F(CovDrmSessionManagerTest, InitializeDrmSession_Success_ReturnsInitAndKeepsError)
{
    install(0, makeSession(kWidevine, KEY_INIT), kKeyA);
    auto helper = makeHelper(kWidevine, kKeyA);
    int err = 0;

    EXPECT_EQ(mgr->initializeDrmSession(helper, 0, err), KEY_INIT);
    EXPECT_EQ(err, 0);
}

TEST_F(CovDrmSessionManagerTest, InitializeDrmSession_EmptySessionId_MapsToSessionIdEmpty)
{
    install(0, makeSession(kWidevine, KEY_ERROR_EMPTY_SESSION_ID), kKeyA);
    auto helper = makeHelper(kWidevine, kKeyA);
    int err = 0;

    EXPECT_EQ(mgr->initializeDrmSession(helper, 0, err), KEY_ERROR_EMPTY_SESSION_ID);
    EXPECT_EQ(err, MW_DRM_SESSIONID_EMPTY);
}

TEST_F(CovDrmSessionManagerTest, InitializeDrmSession_GenericError_MapsToDataBindFailed)
{
    install(0, makeSession(kWidevine, KEY_ERROR), kKeyA);
    auto helper = makeHelper(kWidevine, kKeyA);
    int err = 0;

    EXPECT_EQ(mgr->initializeDrmSession(helper, 0, err), KEY_ERROR);
    EXPECT_EQ(err, MW_DRM_DATA_BIND_FAILED);
}

/* ---------------- getDrmSession slot selection ---------------- */

TEST_F(CovDrmSessionManagerTest, GetDrmSession_EmptyKeyId_ReturnsKeyErrorWithoutCreating)
{
    auto helper = makeHelper(kWidevine, {});
    int err = 0;
    int slot = -1;

    EXPECT_EQ(mgr->getDrmSession(err, helper, slot, nullptr), KEY_ERROR);
    EXPECT_EQ(err, MW_FAILED_TO_GET_KEYID);
    EXPECT_EQ(creatorCalls, 0);
}

TEST_F(CovDrmSessionManagerTest, GetDrmSession_CachedFailedSlot_ReturnsKeyErrorWithoutCreating)
{
    cacheKey(0, kKeyA);
    mgr->cachedKeyIDs[0].isFailedKeyEntries = true;
    auto helper = makeHelper(kWidevine, kKeyA);
    int err = 0;
    int slot = -1;

    EXPECT_EQ(mgr->getDrmSession(err, helper, slot, nullptr), KEY_ERROR);
    EXPECT_EQ(creatorCalls, 0);
}

TEST_F(CovDrmSessionManagerTest, GetDrmSession_AllSlotsPrimary_ReturnsKeyErrorWithoutCreating)
{
    for (int i = 0; i < kMaxSessions; i++)
    {
        mgr->cachedKeyIDs[i].isPrimaryKeyId = true;
    }
    auto helper = makeHelper(kWidevine, kKeyA);
    int err = 0;
    int slot = -1;

    EXPECT_EQ(mgr->getDrmSession(err, helper, slot, nullptr), KEY_ERROR);
    EXPECT_EQ(creatorCalls, 0);
}

TEST_F(CovDrmSessionManagerTest, GetDrmSession_ReadySessionSameKey_ReusesWithoutCreating)
{
    install(0, makeSession(kWidevine, KEY_READY), kKeyA);
    cacheKey(0, kKeyA);
    auto helper = makeHelper(kWidevine, kKeyA);
    int err = 0;
    int slot = -1;

    EXPECT_EQ(mgr->getDrmSession(err, helper, slot, nullptr), KEY_READY);
    EXPECT_EQ(slot, 0);
    EXPECT_EQ(creatorCalls, 0);
}

TEST_F(CovDrmSessionManagerTest, GetDrmSession_PendingSessionSameKey_WaitsAndReturnsReady)
{
    install(0, makeSession(kWidevine, KEY_PENDING), kKeyA);
    cacheKey(0, kKeyA);
    auto helper = makeHelper(kWidevine, kKeyA);
    int err = 0;
    int slot = -1;

    EXPECT_EQ(mgr->getDrmSession(err, helper, slot, nullptr), KEY_READY);
    EXPECT_EQ(creatorCalls, 0);
}

TEST_F(CovDrmSessionManagerTest, GetDrmSession_ErrorSessionSameKey_MarksSlotFailed)
{
    install(0, makeSession(kWidevine, KEY_ERROR), kKeyA);
    cacheKey(0, kKeyA);
    auto helper = makeHelper(kWidevine, kKeyA);
    int err = 0;
    int slot = -1;

    EXPECT_EQ(mgr->getDrmSession(err, helper, slot, nullptr), KEY_ERROR);
    EXPECT_TRUE(mgr->getFailedKeyIdStatus(0));
    EXPECT_EQ(creatorCalls, 0);
}

TEST_F(CovDrmSessionManagerTest, GetDrmSession_DifferentKeySystem_ReplacesSessionWithCreator)
{
    // The old session is deleted by the manager, so its address must not be compared afterwards.
    install(0, makeSession(kClearKey, KEY_READY), kKeyA);
    cacheKey(0, kKeyA);
    auto helper = makeHelper(kWidevine, kKeyA);
    int err = 0;
    int slot = -1;

    EXPECT_EQ(mgr->getDrmSession(err, helper, slot, nullptr), KEY_INIT);
    EXPECT_EQ(slot, 0);
    EXPECT_EQ(creatorCalls, 1);
    EXPECT_EQ(mgr->drmSessionContexts[0].drmSession, createdSession);
}

TEST_F(CovDrmSessionManagerTest, GetDrmSession_NewKey_CreatesSessionAndCachesKey)
{
    createdFirstState = KEY_INIT;
    auto helper = makeHelper(kWidevine, kKeyA);
    int err = 0;
    int slot = -1;

    EXPECT_EQ(mgr->getDrmSession(err, helper, slot, nullptr), KEY_INIT);
    EXPECT_EQ(creatorCalls, 1);
    EXPECT_EQ(mgr->drmSessionContexts[slot].drmSession, createdSession);
    EXPECT_EQ(mgr->drmSessionContexts[slot].data, kKeyA);
    EXPECT_FALSE(mgr->getFailedKeyIdStatus(slot));
    bool status = false;
    EXPECT_TRUE(mgr->IsKeyIdProcessed(kKeyA, status));
    EXPECT_TRUE(status);
}

/* ---------------- createDrmSession paths ---------------- */

TEST_F(CovDrmSessionManagerTest, CreateDrmSession_InactiveState_ReturnsNull)
{
    mgr->setSessionMgrState(eSESSIONMGR_INACTIVE);
    auto helper = makeHelper(kWidevine, kKeyA);
    int responseCode = 0;
    int err = 0;

    EXPECT_EQ(mgr->createDrmSession(responseCode, err, helper, sentinelInstance(), 0, nullptr), nullptr);
    EXPECT_EQ(creatorCalls, 0);
}

TEST_F(CovDrmSessionManagerTest, CreateDrmSession_NullHelperOrCallbacks_ReturnsNull)
{
    auto helper = makeHelper(kWidevine, kKeyA);
    int responseCode = 0;
    int err = 0;

    EXPECT_EQ(mgr->createDrmSession(responseCode, err, nullptr, sentinelInstance(), 0, nullptr), nullptr);
    EXPECT_EQ(mgr->createDrmSession(responseCode, err, helper, nullptr, 0, nullptr), nullptr);
    EXPECT_EQ(creatorCalls, 0);
}

TEST_F(CovDrmSessionManagerTest, CreateDrmSession_ReadySessionReused_ReturnsExistingAndNotifiesContent)
{
    NiceMock<MockDrmSession> *existing = makeSession(kWidevine, KEY_READY);
    install(0, existing, kKeyA);
    cacheKey(0, kKeyA);
    auto helper = makeHelper(kWidevine, kKeyA);
    int responseCode = 0;
    int err = 0;

    EXPECT_EQ(mgr->createDrmSession(responseCode, err, helper, sentinelInstance(), 0, nullptr), existing);
    EXPECT_EQ(mgr->mCustomData, "cdm-data");
    EXPECT_EQ(creatorCalls, 0);
}

TEST_F(CovDrmSessionManagerTest, CreateDrmSession_NewSession_AcquiresLicenseAndReturnsSession)
{
    createdFirstState = KEY_INIT;
    createdLaterState = KEY_INIT;
    setAcquireResult(KEY_READY);
    auto helper = makeHelper(kWidevine, kKeyA);
    int responseCode = 0;
    int err = 0;

    DrmSession *result = mgr->createDrmSession(responseCode, err, helper, sentinelInstance(), 0, nullptr);

    EXPECT_EQ(result, createdSession);
    EXPECT_EQ(creatorCalls, 1);
    EXPECT_EQ(acquireCalls, 1);
    EXPECT_EQ(acquiredSlot, 0);
    EXPECT_FALSE(mgr->getFailedKeyIdStatus(0));
}

TEST_F(CovDrmSessionManagerTest, CreateDrmSession_LicenseAcquireFails_MarksSlotFailed)
{
    createdFirstState = KEY_INIT;
    setAcquireResult(KEY_ERROR);
    auto helper = makeHelper(kWidevine, kKeyA);
    int responseCode = 0;
    int err = 0;

    EXPECT_EQ(mgr->createDrmSession(responseCode, err, helper, sentinelInstance(), 0, nullptr), nullptr);
    EXPECT_EQ(acquireCalls, 1);
    EXPECT_TRUE(mgr->getFailedKeyIdStatus(0));
}

TEST_F(CovDrmSessionManagerTest, CreateDrmSession_NoLicenseCallback_ReportsInitFailed)
{
    createdFirstState = KEY_INIT;
    auto helper = makeHelper(kWidevine, kKeyA);
    int responseCode = 0;
    int err = 0;

    EXPECT_EQ(mgr->createDrmSession(responseCode, err, helper, sentinelInstance(), 0, nullptr), nullptr);
    EXPECT_EQ(err, MW_DRM_INIT_FAILED);
    EXPECT_TRUE(mgr->getFailedKeyIdStatus(0));
}

TEST_F(CovDrmSessionManagerTest, CreateDrmSession_FakeTune_SkipsLicenseAndMarksFailed)
{
    createdFirstState = KEY_INIT;
    setAcquireResult(KEY_READY);
    mgr->UpdateDRMConfig(false, false, false, true, false);
    auto helper = makeHelper(kWidevine, kKeyA);
    int responseCode = 0;
    int err = 0;

    EXPECT_EQ(mgr->createDrmSession(responseCode, err, helper, sentinelInstance(), 0, nullptr), nullptr);
    EXPECT_EQ(acquireCalls, 0);
    EXPECT_TRUE(mgr->getFailedKeyIdStatus(0));
}

TEST_F(CovDrmSessionManagerTest, CreateDrmSession_InitFailsAfterCreate_MarksSlotFailedWithMappedError)
{
    createdFirstState = KEY_INIT;
    createdLaterState = KEY_ERROR_SESSION_CREATE_FAILED;
    setAcquireResult(KEY_READY);
    auto helper = makeHelper(kWidevine, kKeyA);
    int responseCode = 0;
    int err = 0;

    EXPECT_EQ(mgr->createDrmSession(responseCode, err, helper, sentinelInstance(), 0, nullptr), nullptr);
    EXPECT_EQ(err, MW_DRM_SESSION_CREATE_FAILED);
    EXPECT_EQ(acquireCalls, 0);
    EXPECT_TRUE(mgr->getFailedKeyIdStatus(0));
}
