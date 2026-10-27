/*
 * @file FakeContentProtectionFirebolt.cpp
 * @brief Fake/mock implementation of ContentProtectionFirebolt for unit testing.
 */

#include "externals/contentsecuritymanager/IFirebolt/ContentProtectionFirebolt.h"
#include "externals/contentsecuritymanager/ContentSecurityManagerSession.h"

#include <string>
#include <mutex>

uint64_t ContentProtectionFirebolt::mSubscriptionId = 0;

ContentProtectionFirebolt:: ContentProtectionFirebolt() : mInitialized(false), mSpeedStateMutex(), mContentProtectionMutex(), mFireboltInitMutex()
{
    // No real initialization in fake
}

ContentProtectionFirebolt::~ContentProtectionFirebolt()
{
    // No real deinitialization in fake
}

void ContentProtectionFirebolt::SubscribeEvents() {}
void ContentProtectionFirebolt::UnSubscribeEvents() {}
void ContentProtectionFirebolt::HandleWatermarkEvent(const std::string&, const std::string&, const std::string&) {}
void ContentProtectionFirebolt::Initialize() { mInitialized = true; }
void ContentProtectionFirebolt::DeInitialize() { mInitialized = false; }
bool ContentProtectionFirebolt::IsActive(bool) { return mInitialized; }

bool ContentProtectionFirebolt::AcquireLicenseOpenOrUpdate(
    std::string, std::string, const char*, const char*[][2], const char*[][2],
    const char*, size_t, const char*, size_t, const char*, const char*, const char*, size_t,
    ContentSecurityManagerSession& session, char** licenseResponse, size_t* licenseResponseLength,
    int32_t* statusCode, int32_t* reasonCode, int32_t* businessStatus, bool, int)
{
    // Fake: always succeed, fill dummy license
    if (licenseResponse && licenseResponseLength) {
        static const char dummyLicense[] = "FAKE_LICENSE";
        *licenseResponse = (char*)malloc(sizeof(dummyLicense));
        memcpy(*licenseResponse, dummyLicense, sizeof(dummyLicense));
        *licenseResponseLength = sizeof(dummyLicense);
    }
    if (statusCode) *statusCode = 0;
    if (reasonCode) *reasonCode = 0;
    if (businessStatus) *businessStatus = 0;
    session = ContentSecurityManagerSession(1, 0); // fake valid session
    return true;
}

void ContentProtectionFirebolt::CloseDrmSession(int64_t) {}
bool ContentProtectionFirebolt::SetDrmSessionState(int64_t, bool) { return true; }
bool ContentProtectionFirebolt::SetPlaybackPosition(int64_t, float, int32_t) { return true; }
void ContentProtectionFirebolt::ShowWatermark(bool, int64_t) {}
bool ContentProtectionFirebolt::OpenDrmSession(std::string& clientId, std::string appId, std::string keySystem, std::string licenseRequest, std::string initData, int64_t &sessionId, int32_t &errorCode, std::string &response)
{
    response = "{\"license\":\"RkFLRV9MSUNFTlNF\"}"; // base64 for "FAKE_LICENSE"
    return true;
}
bool ContentProtectionFirebolt::UpdateDrmSession(int64_t sessionId, int32_t &errorCode, std::string licenseRequest, std::string initData, std::string &response)
{
    response = "{\"license\":\"RkFLRV9MSUNFTlNF\"}";
    return true;
}