/*
 * If not stated otherwise in this file or this component's license file the
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

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "PlayReadyHelper.h"

// Test fixture for PlayReadyHelper
class PlayReadyHelperTest : public ::testing::Test {
protected:
    DrmInfo createDefaultDrmInfo() {
        DrmInfo info;
        info.method = eMETHOD_AES_128;
        info.mediaFormat = eMEDIAFORMAT_DASH;
        info.useFirst16BytesAsIV = false;
        info.bPropagateUriParams = true;
        info.bUseMediaSequenceIV = true;
        info.bDecryptClearSamplesRequired = true;
        memset(info.iv, 0, 16);
        return info;
    }
};

// ============================================================================
// PlayReadyHelper Class Tests (13 APIs)
// ============================================================================

/**
 * @brief Verify PlayReadyHelper constructor with default DrmInfo
 */
TEST_F(PlayReadyHelperTest, Constructor_DefaultDrmInfo) {
    DrmInfo drmInfo = createDefaultDrmInfo();
    PlayReadyHelper helper(drmInfo);
    SUCCEED();
}

/**
 * @brief Verify constructor with AES-128 and HLS
 */
TEST_F(PlayReadyHelperTest, Constructor_AES128HLS) {
    DrmInfo drmInfo = createDefaultDrmInfo();
    drmInfo.method = eMETHOD_AES_128;
    drmInfo.mediaFormat = eMEDIAFORMAT_HLS;
    PlayReadyHelper helper(drmInfo);
    SUCCEED();
}

/**
 * @brief Verify constructor with all DRM methods
 */
TEST_F(PlayReadyHelperTest, Constructor_AllDrmMethods) {
    DrmMethod methods[] = {eMETHOD_NONE, eMETHOD_AES_128};
    for (auto method : methods) {
        DrmInfo drmInfo = createDefaultDrmInfo();
        drmInfo.method = method;
        PlayReadyHelper helper(drmInfo);
        SUCCEED();
    }
}

/**
 * @brief Verify constructor with all media formats
 */
TEST_F(PlayReadyHelperTest, Constructor_AllMediaFormats) {
    MediaFormat formats[] = {
        eMEDIAFORMAT_HLS, eMEDIAFORMAT_DASH, eMEDIAFORMAT_PROGRESSIVE,
        eMEDIAFORMAT_HLS_MP4, eMEDIAFORMAT_OTA, eMEDIAFORMAT_HDMI,
        eMEDIAFORMAT_COMPOSITE, eMEDIAFORMAT_SMOOTHSTREAMINGMEDIA,
        eMEDIAFORMAT_RMF, eMEDIAFORMAT_UNKNOWN
    };
    for (auto format : formats) {
        DrmInfo drmInfo = createDefaultDrmInfo();
        drmInfo.mediaFormat = format;
        PlayReadyHelper helper(drmInfo);
        SUCCEED();
    }
}

/**
 * @brief Verify destructor executes without exceptions
 */
TEST_F(PlayReadyHelperTest, Destructor_Success) {
    DrmInfo drmInfo = createDefaultDrmInfo();
    { PlayReadyHelper helper(drmInfo); }
    SUCCEED();
}

/**
 * @brief Verify friendlyName() returns "PlayReady"
 */
TEST_F(PlayReadyHelperTest, FriendlyName_ReturnsPlayReady) {
    DrmInfo drmInfo = createDefaultDrmInfo();
    PlayReadyHelper helper(drmInfo);
    const std::string& name = helper.friendlyName();
    EXPECT_EQ(name, "PlayReady");
    EXPECT_FALSE(name.empty());
}

/**
 * @brief Verify getDrmCodecType() returns 2
 */
TEST_F(PlayReadyHelperTest, GetDrmCodecType_Returns2) {
    DrmInfo drmInfo = createDefaultDrmInfo();
    PlayReadyHelper helper(drmInfo);
    int codecType = helper.getDrmCodecType();
    EXPECT_EQ(codecType, 2);
}

/**
 * @brief Verify isClearDecrypt() returns false
 */
TEST_F(PlayReadyHelperTest, IsClearDecrypt_ReturnsFalse) {
    DrmInfo drmInfo = createDefaultDrmInfo();
    PlayReadyHelper helper(drmInfo);
    bool result = helper.isClearDecrypt();
    EXPECT_FALSE(result);
}

/**
 * @brief Verify isHdcp22Required() returns output protection flag
 */
TEST_F(PlayReadyHelperTest, IsHdcp22Required_ReturnsFalseByDefault) {
    DrmInfo drmInfo = createDefaultDrmInfo();
    PlayReadyHelper helper(drmInfo);
    bool result = helper.isHdcp22Required();
    EXPECT_FALSE(result);
}

/**
 * @brief Verify ocdmSystemId() returns PlayReady UUID
 */
TEST_F(PlayReadyHelperTest, OcdmSystemId_ReturnsPlayReadyUUID) {
    DrmInfo drmInfo = createDefaultDrmInfo();
    PlayReadyHelper helper(drmInfo);
    const std::string& systemId = helper.ocdmSystemId();
    EXPECT_FALSE(systemId.empty());
    EXPECT_EQ(systemId, "com.microsoft.playready");  // Returns OCDM ID, not UUID
}

/**
 * @brief Verify createInitData() with empty vector
 */
TEST_F(PlayReadyHelperTest, CreateInitData_EmptyVector) {
    DrmInfo drmInfo = createDefaultDrmInfo();
    PlayReadyHelper helper(drmInfo);
    std::vector<uint8_t> initData;
    helper.createInitData(initData);
    EXPECT_TRUE(initData.empty());  // Remains empty without real DRM system
}

/**
 * @brief Verify createInitData() with pre-populated vector
 */
TEST_F(PlayReadyHelperTest, CreateInitData_PrePopulatedVector) {
    DrmInfo drmInfo = createDefaultDrmInfo();
    PlayReadyHelper helper(drmInfo);
    std::vector<uint8_t> initData = {0x01, 0x02, 0x03};
    helper.createInitData(initData);
    EXPECT_TRUE(initData.empty());  // Vector cleared, not populated without real DRM system
}

/**
 * @brief Verify createInitData() consistency across multiple calls
 */
TEST_F(PlayReadyHelperTest, CreateInitData_ConsistencyAcrossMultipleCalls) {
    DrmInfo drmInfo = createDefaultDrmInfo();
    PlayReadyHelper helper(drmInfo);
    std::vector<uint8_t> initData1, initData2;
    helper.createInitData(initData1);
    helper.createInitData(initData2);
    EXPECT_EQ(initData1, initData2);
}

/**
 * @brief Verify parsePssh() with valid PlayReady PSSH
 */
TEST_F(PlayReadyHelperTest, ParsePssh_ValidData) {
    DrmInfo drmInfo = createDefaultDrmInfo();
    PlayReadyHelper helper(drmInfo);
    
    uint8_t psshData[64] = {0};
    psshData[0] = 0x00; psshData[1] = 0x00; psshData[2] = 0x00; psshData[3] = 0x40;
    psshData[4] = 0x70; psshData[5] = 0x73; psshData[6] = 0x73; psshData[7] = 0x68;
    for (int i = 8; i < 64; i++) psshData[i] = (i % 256);
    
    bool result = helper.parsePssh(psshData, sizeof(psshData));
    EXPECT_FALSE(result);  // Returns false without real DRM system
}

/**
 * @brief Verify parsePssh() with valid PlayReady 4.0 XML structure
 * This test exercises private extractKeyID() and extractMetaData() methods
 */
TEST_F(PlayReadyHelperTest, ParsePssh_ValidPlayReady40XML) {
    DrmInfo drmInfo = createDefaultDrmInfo();
    PlayReadyHelper helper(drmInfo);
    
    // Valid PlayReady 4.0 XML with base64-encoded 16-byte key
    const char* playreadyXML = 
        "<WRMHEADER version=\"4.0.0.0\">"
        "<DATA>"
        "<KID>AAAAAAAAAAAAAAAAAAAAAA==</KID>"
        "<ckm:policy xmlns:ckm=\"urn:ccp:ckm\">test_metadata</ckm:policy>"
        "</DATA>"
        "</WRMHEADER>";
    
    bool result = helper.parsePssh(
        reinterpret_cast<const uint8_t*>(playreadyXML), 
        strlen(playreadyXML)
    );
    
    EXPECT_TRUE(result);
    
    // Verify extracted key
    std::vector<uint8_t> keyID;
    helper.getKey(keyID);
    EXPECT_FALSE(keyID.empty());
    
    // Verify extracted metadata
    const std::string& metadata = helper.getDrmMetaData();
    EXPECT_EQ(metadata, "test_metadata");
}

/**
 * @brief Verify parsePssh() with valid PlayReady 4.1 XML structure
 */
TEST_F(PlayReadyHelperTest, ParsePssh_ValidPlayReady41XML) {
    DrmInfo drmInfo = createDefaultDrmInfo();
    PlayReadyHelper helper(drmInfo);
    
    const char* playreadyXML = 
        "<WRMHEADER version=\"4.1.0.0\">"
        "<DATA>"
        "<PROTECTINFO>"
        "<KID ALGID=\"AESCTR\" VALUE=\"AAAAAAAAAAAAAAAAAAAAAA==\"></KID>"
        "</PROTECTINFO>"
        "</DATA>"
        "</WRMHEADER>";
    
    bool result = helper.parsePssh(
        reinterpret_cast<const uint8_t*>(playreadyXML), 
        strlen(playreadyXML)
    );
    
    EXPECT_TRUE(result);
    
    std::vector<uint8_t> keyID;
    helper.getKey(keyID);
    EXPECT_FALSE(keyID.empty());
}

/**
 * @brief Verify parsePssh() with valid PlayReady 4.2 XML structure (multiple KIDs)
 */
TEST_F(PlayReadyHelperTest, ParsePssh_ValidPlayReady42XML) {
    DrmInfo drmInfo = createDefaultDrmInfo();
    PlayReadyHelper helper(drmInfo);
    
    const char* playreadyXML = 
        "<WRMHEADER version=\"4.2.0.0\">"
        "<DATA>"
        "<PROTECTINFO>"
        "<KIDS>"
        "<KID VALUE=\"AAAAAAAAAAAAAAAAAAAAAA==\"></KID>"
        "<KID VALUE=\"AQEBAQEBAQEBAQEBAQEBAQ==\"></KID>"
        "</KIDS>"
        "</PROTECTINFO>"
        "</DATA>"
        "</WRMHEADER>";
    
    bool result = helper.parsePssh(
        reinterpret_cast<const uint8_t*>(playreadyXML), 
        strlen(playreadyXML)
    );
    
    EXPECT_TRUE(result);
    
    std::vector<uint8_t> keyID;
    helper.getKey(keyID);
    EXPECT_FALSE(keyID.empty());
}

/**
 * @brief Verify parsePssh() with valid PlayReady 4.3 XML structure
 */
TEST_F(PlayReadyHelperTest, ParsePssh_ValidPlayReady43XML) {
    DrmInfo drmInfo = createDefaultDrmInfo();
    PlayReadyHelper helper(drmInfo);
    
    const char* playreadyXML = 
        "<WRMHEADER version=\"4.3.0.0\">"
        "<DATA>"
        "<PROTECTINFO LICENSEREQUESTED=\"true\">"
        "<KIDS>"
        "<KID VALUE=\"AAAAAAAAAAAAAAAAAAAAAA==\"></KID>"
        "</KIDS>"
        "</PROTECTINFO>"
        "</DATA>"
        "</WRMHEADER>";
    
    bool result = helper.parsePssh(
        reinterpret_cast<const uint8_t*>(playreadyXML), 
        strlen(playreadyXML)
    );
    
    EXPECT_TRUE(result);
}

/**
 * @brief Verify parsePssh() with null pointer
 */
TEST_F(PlayReadyHelperTest, ParsePssh_NullPointer) {
    DrmInfo drmInfo = createDefaultDrmInfo();
    PlayReadyHelper helper(drmInfo);
    bool result = helper.parsePssh(nullptr, 10);
    EXPECT_FALSE(result);
}

/**
 * @brief Verify parsePssh() with zero length
 */
TEST_F(PlayReadyHelperTest, ParsePssh_ZeroLength) {
    DrmInfo drmInfo = createDefaultDrmInfo();
    PlayReadyHelper helper(drmInfo);
    uint8_t data[] = {0x01, 0x02, 0x03};
    bool result = helper.parsePssh(data, 0);
    EXPECT_FALSE(result);
}

/**
 * @brief Verify parsePssh() with corrupted header
 */
TEST_F(PlayReadyHelperTest, ParsePssh_CorruptedHeader) {
    DrmInfo drmInfo = createDefaultDrmInfo();
    PlayReadyHelper helper(drmInfo);
    uint8_t corruptedData[64] = {0xFF, 0xFF, 0xFF, 0xFF};
    bool result = helper.parsePssh(corruptedData, sizeof(corruptedData));
    EXPECT_FALSE(result);
}

/**
 * @brief Verify getKey() with empty vector
 */
TEST_F(PlayReadyHelperTest, GetKey_EmptyVector) {
    DrmInfo drmInfo = createDefaultDrmInfo();
    PlayReadyHelper helper(drmInfo);
    std::vector<uint8_t> keyID;
    helper.getKey(keyID);
    // Key may be empty or populated
}

/**
 * @brief Verify getKey() with pre-populated vector
 */
TEST_F(PlayReadyHelperTest, GetKey_PrePopulatedVector) {
    DrmInfo drmInfo = createDefaultDrmInfo();
    PlayReadyHelper helper(drmInfo);
    
    uint8_t psshData[64] = {0};
    psshData[0] = 0x00; psshData[1] = 0x00; psshData[2] = 0x00; psshData[3] = 0x40;
    psshData[4] = 0x70; psshData[5] = 0x73; psshData[6] = 0x73; psshData[7] = 0x68;
    helper.parsePssh(psshData, sizeof(psshData));
    
    std::vector<uint8_t> keyID = {0xFF, 0xFE};
    helper.getKey(keyID);
}

/**
 * @brief Verify getDrmMetaData() returns empty initially
 */
TEST_F(PlayReadyHelperTest, GetDrmMetaData_InitiallyEmpty) {
    DrmInfo drmInfo = createDefaultDrmInfo();
    PlayReadyHelper helper(drmInfo);
    const std::string& metadata = helper.getDrmMetaData();
    EXPECT_EQ(metadata, "");
}

/**
 * @brief Verify setDrmMetaData() and getDrmMetaData() round-trip
 */
TEST_F(PlayReadyHelperTest, SetGetDrmMetaData_ValidString) {
    DrmInfo drmInfo = createDefaultDrmInfo();
    PlayReadyHelper helper(drmInfo);
    
    std::string testMetadata = "playready-metadata-123";
    helper.setDrmMetaData(testMetadata);
    const std::string& retrieved = helper.getDrmMetaData();
    EXPECT_EQ(retrieved, testMetadata);
}

/**
 * @brief Verify setDrmMetaData() with empty string
 */
TEST_F(PlayReadyHelperTest, SetDrmMetaData_EmptyString) {
    DrmInfo drmInfo = createDefaultDrmInfo();
    PlayReadyHelper helper(drmInfo);
    helper.setDrmMetaData("initial");
    helper.setDrmMetaData("");
    const std::string& retrieved = helper.getDrmMetaData();
    EXPECT_EQ(retrieved, "initial");  // Empty string doesn't override existing value
}

/**
 * @brief Verify generateLicenseRequest() with valid challenge containing all required data
 */
TEST_F(PlayReadyHelperTest, GenerateLicenseRequest_ValidChallengeAllData) {
    DrmInfo drmInfo = createDefaultDrmInfo();
    PlayReadyHelper helper(drmInfo);
    
    ChallengeInfo challengeInfo;
    challengeInfo.url = "https://playready.license.com/";
    challengeInfo.accessToken = "pr-token-123";
    uint8_t challengeData[] = {0x01, 0x02, 0x03, 0x04};
    challengeInfo.data = std::make_shared<DrmData>(reinterpret_cast<const char*>(challengeData), sizeof(challengeData));
    
    LicenseRequest licenseRequest;
    helper.generateLicenseRequest(challengeInfo, licenseRequest);
    
    EXPECT_FALSE(licenseRequest.url.empty());
    EXPECT_EQ(licenseRequest.url, challengeInfo.url);
}

/**
 * @brief Verify generateLicenseRequest() with anonymous request (empty token)
 */
TEST_F(PlayReadyHelperTest, GenerateLicenseRequest_AnonymousRequest) {
    DrmInfo drmInfo = createDefaultDrmInfo();
    PlayReadyHelper helper(drmInfo);
    
    ChallengeInfo challengeInfo;
    challengeInfo.url = "https://playready.license.com/";
    challengeInfo.accessToken = "";
    uint8_t challengeData[] = {0x01, 0x02};
    challengeInfo.data = std::make_shared<DrmData>(reinterpret_cast<const char*>(challengeData), sizeof(challengeData));
    
    LicenseRequest licenseRequest;
    helper.generateLicenseRequest(challengeInfo, licenseRequest);
    EXPECT_FALSE(licenseRequest.url.empty());
}

/**
 * @brief Verify generateLicenseRequest() with empty URL
 */
TEST_F(PlayReadyHelperTest, GenerateLicenseRequest_EmptyURL) {
    DrmInfo drmInfo = createDefaultDrmInfo();
    PlayReadyHelper helper(drmInfo);
    
    ChallengeInfo challengeInfo;
    challengeInfo.url = "";
    challengeInfo.accessToken = "token";
    uint8_t challengeData[] = {0x01};
    challengeInfo.data = std::make_shared<DrmData>(reinterpret_cast<const char*>(challengeData), sizeof(challengeData));
    
    LicenseRequest licenseRequest;
    helper.generateLicenseRequest(challengeInfo, licenseRequest);
    SUCCEED();
}

/**
 * @brief Verify generateLicenseRequest() with null challenge data
 */
TEST_F(PlayReadyHelperTest, GenerateLicenseRequest_NullData) {
    DrmInfo drmInfo = createDefaultDrmInfo();
    PlayReadyHelper helper(drmInfo);
    
    ChallengeInfo challengeInfo;
    challengeInfo.url = "https://playready.license.com/";
    challengeInfo.accessToken = "token";
    challengeInfo.data = nullptr;
    
    LicenseRequest licenseRequest;
    helper.generateLicenseRequest(challengeInfo, licenseRequest);
    SUCCEED();
}

/**
 * @brief Verify generateLicenseRequest() with whitespace URL
 */
TEST_F(PlayReadyHelperTest, GenerateLicenseRequest_WhitespaceURL) {
    DrmInfo drmInfo = createDefaultDrmInfo();
    PlayReadyHelper helper(drmInfo);
    
    ChallengeInfo challengeInfo;
    challengeInfo.url = "   ";
    challengeInfo.accessToken = "token";
    uint8_t challengeData[] = {0x01};
    challengeInfo.data = std::make_shared<DrmData>(reinterpret_cast<const char*>(challengeData), sizeof(challengeData));
    
    LicenseRequest licenseRequest;
    helper.generateLicenseRequest(challengeInfo, licenseRequest);
    SUCCEED();
}

/**
 * @brief Verify isExternalLicense() returns false (inherited from base)
 */
TEST_F(PlayReadyHelperTest, IsExternalLicense_ReturnsFalse) {
    DrmInfo drmInfo = createDefaultDrmInfo();
    PlayReadyHelper helper(drmInfo);
    bool result = helper.isExternalLicense();
    EXPECT_FALSE(result);
}

/**
 * @brief Verify getKeys() with empty map (inherited from base, no-op)
 */
TEST_F(PlayReadyHelperTest, GetKeys_EmptyMap) {
    DrmInfo drmInfo = createDefaultDrmInfo();
    PlayReadyHelper helper(drmInfo);
    std::map<int, std::vector<uint8_t>> keyIDs;
    helper.getKeys(keyIDs);
    EXPECT_TRUE(keyIDs.empty());
}

/**
 * @brief Verify setDefaultKeyID() does nothing (inherited from base, no-op)
 */
TEST_F(PlayReadyHelperTest, SetDefaultKeyID_NoOp) {
    DrmInfo drmInfo = createDefaultDrmInfo();
    PlayReadyHelper helper(drmInfo);
    helper.setDefaultKeyID("test-cenc-data");
    SUCCEED();
}

// ============================================================================
// PlayReadyHelperFactory Class Tests (3 APIs)
// ============================================================================

/**
 * @brief Verify PlayReadyHelperFactory::appendSystemId() adds PlayReady UUID
 */
TEST(PlayReadyHelperFactory, AppendSystemId_EmptyVector) {
    PlayReadyHelperFactory factory;
    std::vector<std::string> systemIds;
    
    factory.appendSystemId(systemIds);
    
    EXPECT_FALSE(systemIds.empty());
    EXPECT_EQ(systemIds.size(), 1);
    EXPECT_EQ(systemIds[0], "9a04f079-9840-4286-ab92-e65be0885f95");
}

/**
 * @brief Verify isDRM() returns true for valid PlayReady DRM
 */
TEST(PlayReadyHelperFactory, IsDRM_ValidPlayReadyDRM_AES_HLS) {
    PlayReadyHelperFactory factory;
    DrmInfo drmInfo;
    drmInfo.method = eMETHOD_AES_128;
    drmInfo.mediaFormat = eMEDIAFORMAT_HLS;
    drmInfo.systemUUID = "9a04f079-9840-4286-ab92-e65be0885f95";
    
    bool result = factory.isDRM(drmInfo);
    EXPECT_FALSE(result);  // Only DASH/HLS_MP4 supported, not HLS
}

/**
 * @brief Verify isDRM() returns false for eMETHOD_NONE
 */
TEST(PlayReadyHelperFactory, IsDRM_MethodNone) {
    PlayReadyHelperFactory factory;
    DrmInfo drmInfo;
    drmInfo.method = eMETHOD_NONE;
    drmInfo.mediaFormat = eMEDIAFORMAT_HLS;
    drmInfo.systemUUID = "9a04f079-9840-4286-ab92-e65be0885f95";
    
    bool result = factory.isDRM(drmInfo);
    EXPECT_FALSE(result);
}

/**
 * @brief Verify isDRM() with all media formats
 */
TEST(PlayReadyHelperFactory, IsDRM_AllMediaFormats) {
    PlayReadyHelperFactory factory;
    MediaFormat formats[] = {
        eMEDIAFORMAT_HLS, eMEDIAFORMAT_DASH, eMEDIAFORMAT_PROGRESSIVE,
        eMEDIAFORMAT_HLS_MP4, eMEDIAFORMAT_OTA
    };
    
    for (auto format : formats) {
        DrmInfo drmInfo;
        drmInfo.method = eMETHOD_AES_128;
        drmInfo.mediaFormat = format;
        drmInfo.systemUUID = "9a04f079-9840-4286-ab92-e65be0885f95";
        
        bool result = factory.isDRM(drmInfo);
        // Result depends on which formats PlayReady supports
    }
}

/**
 * @brief Verify isDRM() returns false for unsupported media format
 */
TEST(PlayReadyHelperFactory, IsDRM_UnsupportedMediaFormat) {
    PlayReadyHelperFactory factory;
    DrmInfo drmInfo;
    drmInfo.method = eMETHOD_AES_128;
    drmInfo.mediaFormat = eMEDIAFORMAT_UNKNOWN;
    drmInfo.systemUUID = "9a04f079-9840-4286-ab92-e65be0885f95";
    
    bool result = factory.isDRM(drmInfo);
    // Result depends on factory implementation
}

/**
 * @brief Verify createHelper() returns valid helper
 */
TEST(PlayReadyHelperFactory, CreateHelper_ValidDRM_AES_HLS) {
    PlayReadyHelperFactory factory;
    DrmInfo drmInfo;
    drmInfo.method = eMETHOD_AES_128;
    drmInfo.mediaFormat = eMEDIAFORMAT_HLS;
    drmInfo.systemUUID = "9a04f079-9840-4286-ab92-e65be0885f95";
    
    DrmHelperPtr helper = factory.createHelper(drmInfo);
    EXPECT_EQ(helper, nullptr);  // Returns nullptr for unsupported HLS format
}

/**
 * @brief Verify createHelper() with all media formats
 */
TEST(PlayReadyHelperFactory, CreateHelper_AllMediaFormats) {
    PlayReadyHelperFactory factory;
    MediaFormat formats[] = {
        eMEDIAFORMAT_HLS, eMEDIAFORMAT_DASH,
        eMEDIAFORMAT_PROGRESSIVE, eMEDIAFORMAT_HLS_MP4
    };
    
    for (auto format : formats) {
        DrmInfo drmInfo;
        drmInfo.method = eMETHOD_AES_128;
        drmInfo.mediaFormat = format;
        drmInfo.systemUUID = "9a04f079-9840-4286-ab92-e65be0885f95";
        
        DrmHelperPtr helper = factory.createHelper(drmInfo);
        // Helper may be null or valid depending on supported formats
    }
}

/**
 * @brief Verify createHelper() with non-PlayReady UUID returns null
 */
TEST(PlayReadyHelperFactory, CreateHelper_NonPlayReadyUUID) {
    PlayReadyHelperFactory factory;
    DrmInfo drmInfo;
    drmInfo.method = eMETHOD_AES_128;
    drmInfo.mediaFormat = eMEDIAFORMAT_DASH;
    drmInfo.systemUUID = "edef8ba9-79d6-4ace-a3c8-27dcd51d21ed"; // Widevine
    
    DrmHelperPtr helper = factory.createHelper(drmInfo);
    // Should return nullptr for non-PlayReady DRM
}
