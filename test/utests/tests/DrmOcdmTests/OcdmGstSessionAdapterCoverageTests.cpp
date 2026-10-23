/*
 * If not stated otherwise in this file or this component's LICENSE file the
 * following copyright and licenses apply:
 *
 * Copyright 2025 RDK Management
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * @file OcdmGstSessionAdapterCoverageTests.cpp
 * @brief Coverage-focused unit tests for OCDMGSTSessionAdapter.
 *
 * The existing OcdmGstSessionAdapterTests.cpp never attaches an OpenCDM session, so
 * the decrypt paths return early and the SEI extraction is never fed a valid buffer.
 * These tests attach a session through a test subclass, drive the OpenCDM mock, and
 * feed SEI time code payloads through the GStreamer map mock.
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <algorithm>
#include <cstdint>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include "OcdmGstSessionAdapter.h"
#include "MockOpenCdm.h"

extern "C" {
/* Hooks provided by test/utests/drm/mocks/gstMocks.c */
extern const guint8 *g_mockGstMapData;
extern gsize g_mockGstMapSize;
extern guint g_mockTimeCodeMetaCount;
extern guint g_mockTimeCodeHours;
extern guint g_mockTimeCodeMinutes;
extern guint g_mockTimeCodeSeconds;
extern guint g_mockTimeCodeFrames;
}

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Return;

namespace {

/**
 * @brief DrmCallbacks implementation local to this translation unit.
 */
class CovDrmCallbacks : public DrmCallbacks {
public:
    void Individualization(const std::string& payload) override { (void)payload; }
    void LicenseRenewal(DrmHelperPtr drmHelper, void* userData) override {
        (void)drmHelper;
        (void)userData;
    }
};

/**
 * @brief DRM helper whose clear-sample policy can be toggled per test.
 */
class CovDrmHelper : public DrmHelper {
public:
    explicit CovDrmHelper(const DrmInfo& drmInfo) : DrmHelper(drmInfo) {}

    bool decryptClearSamples = true;

    bool isDecryptClearSamplesRequired() const override { return decryptClearSamples; }

    const std::string& ocdmSystemId() const override {
        static const std::string id = "com.widevine.alpha";
        return id;
    }

    void createInitData(std::vector<uint8_t>& initData) const override {
        initData = {0x01, 0x02, 0x03, 0x04};
    }

    bool parsePssh(const uint8_t* initData, uint32_t initDataLen) override {
        return (initData != nullptr && initDataLen > 0);
    }

    bool isClearDecrypt() const override { return false; }

    void getKey(std::vector<uint8_t>& keyID) const override {
        keyID = {0xAA, 0xBB, 0xCC, 0xDD};
    }

    void generateLicenseRequest(const ChallengeInfo&, LicenseRequest&) const override { }
};

/**
 * @brief Adapter subclass that exposes the protected session pointer for injection.
 */
class CovGstAdapter : public OCDMGSTSessionAdapter {
public:
    CovGstAdapter(DrmHelperPtr drmHelper, DrmCallbacks* drmCallbacks)
        : OCDMGSTSessionAdapter(std::move(drmHelper), drmCallbacks) {}

    void attachSession(OpenCDMSession* session) { m_pOpenCDMSession = session; }
};

/**
 * @brief Render an unsigned value as a string of '0'/'1' characters, MSB first.
 */
std::string bitsOf(uint32_t value, int width)
{
    std::string s;
    for (int i = width - 1; i >= 0; --i) {
        s += ((value >> i) & 1U) ? '1' : '0';
    }
    return s;
}

/**
 * @brief Build the bit string for one clock_time_stamp entry (H.264/HEVC pic timing SEI).
 *
 * @param clockFlag      clock_time_stamp_flag
 * @param fullTimestamp  full_timestamp_flag (writes seconds=3, minutes=2, hours=1)
 * @param secondsFlag    seconds_flag (partial timestamp path, writes seconds=3)
 * @param minutesFlag    minutes_flag (partial timestamp path, writes minutes=2)
 * @param hoursFlag      hours_flag (partial timestamp path, writes hours=1)
 *
 * Frame count is always 5.
 */
std::string clockTimestampBits(bool clockFlag, bool fullTimestamp = false,
                               bool secondsFlag = false, bool minutesFlag = false,
                               bool hoursFlag = false)
{
    std::string b = bitsOf(clockFlag ? 1U : 0U, 1);
    if (!clockFlag) {
        return b;
    }
    b += bitsOf(0, 1);                              // nuit_field_based_flag
    b += bitsOf(0, 5);                              // counting_type
    b += bitsOf(fullTimestamp ? 1U : 0U, 1);        // full_timestamp_flag
    b += bitsOf(0, 1);                              // discontinuity_flag
    b += bitsOf(0, 1);                              // cnt_dropped_flag
    b += bitsOf(5, 9);                              // n_frames

    if (fullTimestamp) {
        b += bitsOf(3, 6);                          // seconds_value
        b += bitsOf(2, 6);                          // minutes_value
        b += bitsOf(1, 5);                          // hours_value
        return b;
    }

    b += bitsOf(secondsFlag ? 1U : 0U, 1);
    if (secondsFlag) {
        b += bitsOf(3, 6);
        b += bitsOf(minutesFlag ? 1U : 0U, 1);
        if (minutesFlag) {
            b += bitsOf(2, 6);
            b += bitsOf(hoursFlag ? 1U : 0U, 1);
            if (hoursFlag) {
                b += bitsOf(1, 5);
            }
        }
    }
    return b;
}

/**
 * @brief Build a byte buffer holding an Annex-B start code followed by an SEI NAL.
 *
 * The parser looks for 00 00 00 0b, then reads the NAL header and SEI message header
 * fields in order, followed by the supplied clock timestamp bits.
 */
std::vector<uint8_t> buildSeiBuffer(uint32_t nalType, uint32_t payloadType,
                                    uint32_t numClockTs, const std::string& clockBits)
{
    std::string bits = bitsOf(0, 1)                 // forbidden_zero_bit
                     + bitsOf(nalType, 6)           // nal_unit_type
                     + bitsOf(0, 6)                 // nuh_layer_id
                     + bitsOf(1, 3)                 // nuh_temporal_id_plus1
                     + bitsOf(payloadType, 8)       // payload_type
                     + bitsOf(8, 8)                 // payload_size (not used by parser)
                     + bitsOf(numClockTs, 2)        // num_clock_ts
                     + clockBits;

    std::vector<uint8_t> out = {0x00, 0x00, 0x00, 0x0b};
    for (size_t i = 0; i < bits.size(); i += 8) {
        uint8_t byte = 0;
        for (size_t k = 0; k < 8; ++k) {
            byte = static_cast<uint8_t>(byte << 1);
            if (i + k < bits.size() && bits[i + k] == '1') {
                byte |= 1U;
            }
        }
        out.push_back(byte);
    }
    out.insert(out.end(), 8, 0x00);
    return out;
}

} // namespace

/**
 * @brief Fixture that installs an OpenCDM mock and resets the GStreamer map/meta hooks.
 */
class OcdmGstCoverageTest : public ::testing::Test {
protected:
    NiceMock<MockOpenCdm> m_ocdm;
    std::shared_ptr<CovDrmHelper> m_helper;
    CovDrmCallbacks m_callbacks;

    /* Any non-null address works as an opaque session handle. */
    static uint8_t s_sessionTag;
    OpenCDMSession* m_session = reinterpret_cast<OpenCDMSession*>(&s_sessionTag);

    void SetUp() override
    {
        g_mockopencdm = &m_ocdm;
        g_mockGstMapData = nullptr;
        g_mockGstMapSize = 0;
        g_mockTimeCodeMetaCount = 0;
        g_mockTimeCodeHours = 0;
        g_mockTimeCodeMinutes = 0;
        g_mockTimeCodeSeconds = 0;
        g_mockTimeCodeFrames = 0;

        DrmInfo drmInfo;
        m_helper = std::make_shared<CovDrmHelper>(drmInfo);
    }

    void TearDown() override
    {
        g_mockGstMapData = nullptr;
        g_mockGstMapSize = 0;
        g_mockopencdm = nullptr;
    }

    /* Point the GStreamer map mock at a buffer that must outlive the decrypt call. */
    static void mapBuffer(const std::vector<uint8_t>& data)
    {
        g_mockGstMapData = data.data();
        g_mockGstMapSize = data.size();
    }
};

uint8_t OcdmGstCoverageTest::s_sessionTag = 0;

// ========================================
// SEI EXTRACTION (exercised through decrypt(GstBuffer*...))
// ========================================

/**
 * @brief A buffer with no SEI start code produces no time code metadata.
 */
TEST_F(OcdmGstCoverageTest, ExtractSei_NoStartCode_AddsNoTimeCode)
{
    CovGstAdapter adapter(m_helper, &m_callbacks);
    adapter.attachSession(m_session);

    GstBuffer buffer = {};
    int ret = adapter.decrypt(nullptr, nullptr, &buffer, 0, nullptr, nullptr);

    EXPECT_EQ(ret, 0);
    EXPECT_EQ(g_mockTimeCodeMetaCount, 0U);
}

/**
 * @brief A NULL buffer is tolerated by the SEI path and the clear-sample skip still applies.
 */
TEST_F(OcdmGstCoverageTest, ExtractSei_NullBuffer_IsTolerated)
{
    CovGstAdapter adapter(m_helper, &m_callbacks);
    adapter.attachSession(m_session);

    int ret = adapter.decrypt(nullptr, nullptr, nullptr, 0, nullptr, nullptr);

    EXPECT_EQ(ret, 0);
    EXPECT_EQ(g_mockTimeCodeMetaCount, 0U);
}

/**
 * @brief A full timestamp SEI yields one time code with the encoded hours, minutes, seconds and frames.
 */
TEST_F(OcdmGstCoverageTest, ExtractSei_FullTimestamp_AddsTimeCode)
{
    CovGstAdapter adapter(m_helper, &m_callbacks);
    adapter.attachSession(m_session);

    auto sei = buildSeiBuffer(39, 136, 1, clockTimestampBits(true, true));
    mapBuffer(sei);
    GstBuffer buffer = {};
    int ret = adapter.decrypt(nullptr, nullptr, &buffer, 0, nullptr, nullptr);

    EXPECT_EQ(ret, 0);
    EXPECT_EQ(g_mockTimeCodeMetaCount, 1U);
    EXPECT_EQ(g_mockTimeCodeHours, 1U);
    EXPECT_EQ(g_mockTimeCodeMinutes, 2U);
    EXPECT_EQ(g_mockTimeCodeSeconds, 3U);
    EXPECT_EQ(g_mockTimeCodeFrames, 5U);
}

/**
 * @brief A partial timestamp with seconds, minutes and hours flags set decodes all three fields.
 */
TEST_F(OcdmGstCoverageTest, ExtractSei_PartialTimestampAllFlags_AddsTimeCode)
{
    CovGstAdapter adapter(m_helper, &m_callbacks);
    adapter.attachSession(m_session);

    auto sei = buildSeiBuffer(39, 136, 1, clockTimestampBits(true, false, true, true, true));
    mapBuffer(sei);
    GstBuffer buffer = {};
    adapter.decrypt(nullptr, nullptr, &buffer, 0, nullptr, nullptr);

    EXPECT_EQ(g_mockTimeCodeMetaCount, 1U);
    EXPECT_EQ(g_mockTimeCodeHours, 1U);
    EXPECT_EQ(g_mockTimeCodeMinutes, 2U);
    EXPECT_EQ(g_mockTimeCodeSeconds, 3U);
}

/**
 * @brief A partial timestamp with only the seconds flag set leaves minutes and hours at zero.
 */
TEST_F(OcdmGstCoverageTest, ExtractSei_PartialTimestampSecondsOnly_AddsTimeCode)
{
    CovGstAdapter adapter(m_helper, &m_callbacks);
    adapter.attachSession(m_session);

    auto sei = buildSeiBuffer(39, 136, 1, clockTimestampBits(true, false, true, false, false));
    mapBuffer(sei);
    GstBuffer buffer = {};
    adapter.decrypt(nullptr, nullptr, &buffer, 0, nullptr, nullptr);

    EXPECT_EQ(g_mockTimeCodeMetaCount, 1U);
    EXPECT_EQ(g_mockTimeCodeHours, 0U);
    EXPECT_EQ(g_mockTimeCodeMinutes, 0U);
    EXPECT_EQ(g_mockTimeCodeSeconds, 3U);
}

/**
 * @brief A partial timestamp with no seconds flag leaves all time fields at zero but still emits a time code.
 */
TEST_F(OcdmGstCoverageTest, ExtractSei_PartialTimestampNoFlags_AddsZeroTimeCode)
{
    CovGstAdapter adapter(m_helper, &m_callbacks);
    adapter.attachSession(m_session);

    auto sei = buildSeiBuffer(39, 136, 1, clockTimestampBits(true, false, false, false, false));
    mapBuffer(sei);
    GstBuffer buffer = {};
    adapter.decrypt(nullptr, nullptr, &buffer, 0, nullptr, nullptr);

    EXPECT_EQ(g_mockTimeCodeMetaCount, 1U);
    EXPECT_EQ(g_mockTimeCodeHours, 0U);
    EXPECT_EQ(g_mockTimeCodeMinutes, 0U);
    EXPECT_EQ(g_mockTimeCodeSeconds, 0U);
    EXPECT_EQ(g_mockTimeCodeFrames, 5U);
}

/**
 * @brief When the first clock entry is disabled, the parser continues to the second entry.
 */
TEST_F(OcdmGstCoverageTest, ExtractSei_FirstClockDisabled_UsesSecondClock)
{
    CovGstAdapter adapter(m_helper, &m_callbacks);
    adapter.attachSession(m_session);

    auto sei = buildSeiBuffer(39, 136, 2,
                              clockTimestampBits(false) + clockTimestampBits(true, true));
    mapBuffer(sei);
    GstBuffer buffer = {};
    adapter.decrypt(nullptr, nullptr, &buffer, 0, nullptr, nullptr);

    EXPECT_EQ(g_mockTimeCodeMetaCount, 1U);
    EXPECT_EQ(g_mockTimeCodeHours, 1U);
}

/**
 * @brief An SEI whose only clock entry is disabled emits no time code.
 */
TEST_F(OcdmGstCoverageTest, ExtractSei_ClockDisabled_AddsNoTimeCode)
{
    CovGstAdapter adapter(m_helper, &m_callbacks);
    adapter.attachSession(m_session);

    auto sei = buildSeiBuffer(39, 136, 1, clockTimestampBits(false));
    mapBuffer(sei);
    GstBuffer buffer = {};
    adapter.decrypt(nullptr, nullptr, &buffer, 0, nullptr, nullptr);

    EXPECT_EQ(g_mockTimeCodeMetaCount, 0U);
}

/**
 * @brief An SEI with a payload type other than 136 (time code) is ignored.
 */
TEST_F(OcdmGstCoverageTest, ExtractSei_WrongPayloadType_AddsNoTimeCode)
{
    CovGstAdapter adapter(m_helper, &m_callbacks);
    adapter.attachSession(m_session);

    auto sei = buildSeiBuffer(39, 5, 1, clockTimestampBits(true, true));
    mapBuffer(sei);
    GstBuffer buffer = {};
    adapter.decrypt(nullptr, nullptr, &buffer, 0, nullptr, nullptr);

    EXPECT_EQ(g_mockTimeCodeMetaCount, 0U);
}

/**
 * @brief A NAL unit that is not an SEI prefix (type 39) is ignored.
 */
TEST_F(OcdmGstCoverageTest, ExtractSei_NonSeiNalType_AddsNoTimeCode)
{
    CovGstAdapter adapter(m_helper, &m_callbacks);
    adapter.attachSession(m_session);

    auto sei = buildSeiBuffer(1, 136, 1, clockTimestampBits(true, true));
    mapBuffer(sei);
    GstBuffer buffer = {};
    adapter.decrypt(nullptr, nullptr, &buffer, 0, nullptr, nullptr);

    EXPECT_EQ(g_mockTimeCodeMetaCount, 0U);
}

/**
 * @brief SEI data beyond the 2048-byte scan window is not examined.
 */
TEST_F(OcdmGstCoverageTest, ExtractSei_BeyondScanWindow_AddsNoTimeCode)
{
    CovGstAdapter adapter(m_helper, &m_callbacks);
    adapter.attachSession(m_session);

    auto sei = buildSeiBuffer(39, 136, 1, clockTimestampBits(true, true));
    std::vector<uint8_t> data(4096, 0x00);
    std::copy(sei.begin(), sei.end(), data.begin() + 3000);
    mapBuffer(data);
    GstBuffer buffer = {};
    adapter.decrypt(nullptr, nullptr, &buffer, 0, nullptr, nullptr);

    EXPECT_EQ(g_mockTimeCodeMetaCount, 0U);
}

// ========================================
// DECRYPT(GstBuffer*...) WITH AN ATTACHED SESSION
// ========================================

/**
 * @brief Clear samples with no key ID are skipped when the helper does not require decryption of clear samples.
 */
TEST_F(OcdmGstCoverageTest, DecryptGst_ClearSampleSkipped_ReturnsZero)
{
    m_helper->decryptClearSamples = false;
    CovGstAdapter adapter(m_helper, &m_callbacks);
    adapter.attachSession(m_session);

    GstBuffer buffer = {};
    int ret = adapter.decrypt(nullptr, nullptr, &buffer, 0, nullptr, nullptr);

    EXPECT_EQ(ret, 0);
}

/**
 * @brief Clear samples are passed to the GStreamer decrypt entry point when the helper requires it.
 */
TEST_F(OcdmGstCoverageTest, DecryptGst_ClearSampleDecryptRequired_ReturnsZero)
{
    m_helper->decryptClearSamples = true;
    CovGstAdapter adapter(m_helper, &m_callbacks);
    adapter.attachSession(m_session);

    GstBuffer buffer = {};
    int ret = adapter.decrypt(nullptr, nullptr, &buffer, 0, nullptr, nullptr);

    EXPECT_EQ(ret, 0);
}

/**
 * @brief An encrypted sample with sub-samples and an SEI payload is decrypted and its time code recorded.
 */
TEST_F(OcdmGstCoverageTest, DecryptGst_EncryptedSubSamples_ReturnsZeroAndExtractsSei)
{
    CovGstAdapter adapter(m_helper, &m_callbacks);
    adapter.attachSession(m_session);

    auto sei = buildSeiBuffer(39, 136, 1, clockTimestampBits(true, true));
    mapBuffer(sei);

    GstBuffer keyId = {};
    GstBuffer iv = {};
    GstBuffer buffer = {};
    GstBuffer subSamples = {};
    int ret = adapter.decrypt(&keyId, &iv, &buffer, 2, &subSamples, nullptr);

    EXPECT_EQ(ret, 0);
    EXPECT_EQ(g_mockTimeCodeMetaCount, 1U);
}

/**
 * @brief A NULL sample buffer with a key ID still goes through the decrypt path without crashing.
 */
TEST_F(OcdmGstCoverageTest, DecryptGst_NullBufferWithKeyId_ReturnsZero)
{
    CovGstAdapter adapter(m_helper, &m_callbacks);
    adapter.attachSession(m_session);

    GstBuffer keyId = {};
    GstBuffer iv = {};
    GstBuffer subSamples = {};
    int ret = adapter.decrypt(&keyId, &iv, nullptr, 0, &subSamples, nullptr);

    EXPECT_EQ(ret, 0);
}

// ========================================
// DECRYPT(IV, PAYLOAD, ...) WITH AN ATTACHED SESSION
// ========================================

/**
 * @brief A successful OpenCDM decrypt returns zero.
 */
TEST_F(OcdmGstCoverageTest, DecryptBasic_Success_ReturnsZero)
{
    CovGstAdapter adapter(m_helper, &m_callbacks);
    adapter.attachSession(m_session);

    EXPECT_CALL(m_ocdm, opencdm_session_decrypt(_, _, _, _, _, _, _, _, _, _))
        .WillOnce(Return(ERROR_NONE));

    uint8_t iv[16] = {};
    uint8_t payload[16] = {};
    int ret = adapter.decrypt(iv, sizeof(iv), payload, sizeof(payload), nullptr);

    EXPECT_EQ(ret, 0);
}

/**
 * @brief A failed decrypt with an OutputRestricted key status maps to the HDCP output protection error.
 */
TEST_F(OcdmGstCoverageTest, DecryptBasic_OutputRestricted_ReturnsHdcpOutputProtectionFailure)
{
    CovGstAdapter adapter(m_helper, &m_callbacks);
    adapter.attachSession(m_session);

    EXPECT_CALL(m_ocdm, opencdm_session_decrypt(_, _, _, _, _, _, _, _, _, _))
        .WillOnce(Return(ERROR_UNKNOWN));
    EXPECT_CALL(m_ocdm, opencdm_session_status(_, _, _))
        .WillOnce(Return(KeyStatus::OutputRestricted));

    uint8_t iv[16] = {};
    uint8_t payload[16] = {};
    int ret = adapter.decrypt(iv, sizeof(iv), payload, sizeof(payload), nullptr);

    EXPECT_EQ(ret, HDCP_OUTPUT_PROTECTION_FAILURE);
}

/**
 * @brief A failed decrypt with an OutputRestrictedHDCP22 key status maps to the HDCP compliance error.
 */
TEST_F(OcdmGstCoverageTest, DecryptBasic_OutputRestrictedHdcp22_ReturnsHdcpComplianceFailure)
{
    CovGstAdapter adapter(m_helper, &m_callbacks);
    adapter.attachSession(m_session);

    EXPECT_CALL(m_ocdm, opencdm_session_decrypt(_, _, _, _, _, _, _, _, _, _))
        .WillOnce(Return(ERROR_UNKNOWN));
    EXPECT_CALL(m_ocdm, opencdm_session_status(_, _, _))
        .WillOnce(Return(KeyStatus::OutputRestrictedHDCP22));

    uint8_t iv[16] = {};
    uint8_t payload[16] = {};
    int ret = adapter.decrypt(iv, sizeof(iv), payload, sizeof(payload), nullptr);

    EXPECT_EQ(ret, HDCP_COMPLIANCE_CHECK_FAILURE);
}

/**
 * @brief A failed decrypt with any other key status returns the raw OpenCDM error.
 */
TEST_F(OcdmGstCoverageTest, DecryptBasic_OtherKeyStatus_ReturnsRawError)
{
    CovGstAdapter adapter(m_helper, &m_callbacks);
    adapter.attachSession(m_session);

    EXPECT_CALL(m_ocdm, opencdm_session_decrypt(_, _, _, _, _, _, _, _, _, _))
        .WillOnce(Return(ERROR_UNKNOWN));
    EXPECT_CALL(m_ocdm, opencdm_session_status(_, _, _))
        .WillOnce(Return(KeyStatus::Usable));

    uint8_t iv[16] = {};
    uint8_t payload[16] = {};
    int ret = adapter.decrypt(iv, sizeof(iv), payload, sizeof(payload), nullptr);

    EXPECT_EQ(ret, static_cast<int>(ERROR_UNKNOWN));
}
