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
#include "VanillaDrmHelper.h"

// ============================================================================
// VanillaDrmHelper Class Tests (11 APIs)
// ============================================================================

/**
 * @brief Verify VanillaDrmHelper constructor
 */
TEST(VanillaDrmHelper, Constructor_Success) {
    VanillaDrmHelper helper;
    SUCCEED();
}

/**
 * @brief Verify friendlyName() returns "Vanilla_AES"
 */
TEST(VanillaDrmHelper, FriendlyName_ReturnsVanillaAES) {
    VanillaDrmHelper helper;
    const std::string& name = helper.friendlyName();
    EXPECT_EQ(name, "Vanilla_AES");
    EXPECT_FALSE(name.empty());
}

/**
 * @brief Verify getDrmCodecType() returns 3
 */
TEST(VanillaDrmHelper, GetDrmCodecType_Returns3) {
    VanillaDrmHelper helper;
    int codecType = helper.getDrmCodecType();
    EXPECT_EQ(codecType, 3);
}

/**
 * @brief Verify isClearDecrypt() returns true
 */
TEST(VanillaDrmHelper, IsClearDecrypt_ReturnsTrue) {
    VanillaDrmHelper helper;
    bool result = helper.isClearDecrypt();
    EXPECT_TRUE(result);
}

/**
 * @brief Verify isExternalLicense() returns true
 */
TEST(VanillaDrmHelper, IsExternalLicense_ReturnsTrue) {
    VanillaDrmHelper helper;
    bool result = helper.isExternalLicense();
    EXPECT_TRUE(result);
}

/**
 * @brief Verify ocdmSystemId() returns empty string
 */
TEST(VanillaDrmHelper, OcdmSystemId_ReturnsEmpty) {
    VanillaDrmHelper helper;
    const std::string& systemId = helper.ocdmSystemId();
    EXPECT_TRUE(systemId.empty());
}

/**
 * @brief Verify createInitData() with empty vector
 */
TEST(VanillaDrmHelper, CreateInitData_EmptyVector) {
    VanillaDrmHelper helper;
    std::vector<uint8_t> initData;
    helper.createInitData(initData);
    // Vanilla implementation does nothing - vector stays empty
    EXPECT_TRUE(initData.empty());
}

/**
 * @brief Verify createInitData() with pre-populated vector
 */
TEST(VanillaDrmHelper, CreateInitData_PrePopulated) {
    VanillaDrmHelper helper;
    std::vector<uint8_t> initData = {0x01, 0x02, 0x03};
    size_t originalSize = initData.size();
    helper.createInitData(initData);
    // Vanilla implementation does nothing
    EXPECT_EQ(initData.size(), originalSize);
}

/**
 * @brief Verify parsePssh() returns false for valid data
 */
TEST(VanillaDrmHelper, ParsePssh_ValidData) {
    VanillaDrmHelper helper;
    uint8_t data[] = {0x01, 0x02, 0x03, 0x04};
    bool result = helper.parsePssh(data, sizeof(data));
    EXPECT_FALSE(result);
}

/**
 * @brief Verify parsePssh() with null pointer
 */
TEST(VanillaDrmHelper, ParsePssh_NullPointer) {
    VanillaDrmHelper helper;
    bool result = helper.parsePssh(nullptr, 10);
    EXPECT_FALSE(result);
}

/**
 * @brief Verify parsePssh() with zero length
 */
TEST(VanillaDrmHelper, ParsePssh_ZeroLength) {
    VanillaDrmHelper helper;
    uint8_t data[] = {0x01, 0x02, 0x03};
    bool result = helper.parsePssh(data, 0);
    EXPECT_FALSE(result);
}

/**
 * @brief Verify parsePssh() with large data set
 */
TEST(VanillaDrmHelper, ParsePssh_LargeData) {
    VanillaDrmHelper helper;
    uint8_t data[1024];
    memset(data, 0xFF, sizeof(data));
    bool result = helper.parsePssh(data, sizeof(data));
    EXPECT_FALSE(result);
}

/**
 * @brief Verify getKey() clears the vector
 */
TEST(VanillaDrmHelper, GetKey_ClearsVector) {
    VanillaDrmHelper helper;
    std::vector<uint8_t> keyID = {0x01, 0x02, 0x03};
    helper.getKey(keyID);
    EXPECT_TRUE(keyID.empty());
}

/**
 * @brief Verify getKey() with empty vector
 */
TEST(VanillaDrmHelper, GetKey_EmptyVector) {
    VanillaDrmHelper helper;
    std::vector<uint8_t> keyID;
    helper.getKey(keyID);
    EXPECT_TRUE(keyID.empty());
}

/**
 * @brief Verify getKey() with large pre-populated vector
 */
TEST(VanillaDrmHelper, GetKey_LargeVector) {
    VanillaDrmHelper helper;
    std::vector<uint8_t> keyID(1000, 0xAA);
    helper.getKey(keyID);
    EXPECT_TRUE(keyID.empty());
}

/**
 * @brief Verify setDrmMetaData() with valid string
 */
TEST(VanillaDrmHelper, SetDrmMetaData_ValidString) {
    VanillaDrmHelper helper;
    std::string metadata = "test-metadata";
    helper.setDrmMetaData(metadata);
    SUCCEED();
}

/**
 * @brief Verify setDrmMetaData() with empty string
 */
TEST(VanillaDrmHelper, SetDrmMetaData_EmptyString) {
    VanillaDrmHelper helper;
    helper.setDrmMetaData("");
    SUCCEED();
}

/**
 * @brief Verify generateLicenseRequest() with valid challenge
 */
TEST(VanillaDrmHelper, GenerateLicenseRequest_ValidChallenge) {
    VanillaDrmHelper helper;
    ChallengeInfo challengeInfo;
    challengeInfo.url = "https://test.com/license";
    challengeInfo.accessToken = "token";
    LicenseRequest licenseRequest;
    helper.generateLicenseRequest(challengeInfo, licenseRequest);
    SUCCEED();
}

/**
 * @brief Verify generateLicenseRequest() with empty challenge
 */
TEST(VanillaDrmHelper, GenerateLicenseRequest_EmptyChallenge) {
    VanillaDrmHelper helper;
    ChallengeInfo challengeInfo;
    LicenseRequest licenseRequest;
    helper.generateLicenseRequest(challengeInfo, licenseRequest);
    SUCCEED();
}

/**
 * @brief Verify getKeys() with empty map (inherited from base, no-op)
 */
TEST(VanillaDrmHelper, GetKeys_EmptyMap) {
    VanillaDrmHelper helper;
    std::map<int, std::vector<uint8_t>> keyIDs;
    helper.getKeys(keyIDs);
    EXPECT_TRUE(keyIDs.empty());
}

/**
 * @brief Verify setDefaultKeyID() does nothing (inherited from base, no-op)
 */
TEST(VanillaDrmHelper, SetDefaultKeyID_NoOp) {
    VanillaDrmHelper helper;
    helper.setDefaultKeyID("test-data");
    SUCCEED();
}

/**
 * @brief Verify isHdcp22Required() returns false (inherited from base)
 */
TEST(VanillaDrmHelper, IsHdcp22Required_ReturnsFalse) {
    VanillaDrmHelper helper;
    bool result = helper.isHdcp22Required();
    EXPECT_FALSE(result);
}

/**
 * @brief Verify getDrmMetaData() returns empty (inherited from base)
 */
TEST(VanillaDrmHelper, GetDrmMetaData_ReturnsEmpty) {
    VanillaDrmHelper helper;
    const std::string& metadata = helper.getDrmMetaData();
    EXPECT_EQ(metadata, "");
}

// Note: VanillaDrmHelper has no Factory class - it's instantiated directly
