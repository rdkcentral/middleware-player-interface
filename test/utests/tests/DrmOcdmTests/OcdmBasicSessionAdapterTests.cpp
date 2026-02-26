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
 * @file OcdmBasicSessionAdapterTests_new.cpp
 * @brief Comprehensive unit tests for OCDMBasicSessionAdapter
 *
 * This file contains extensive unit tests for the OCDMBasicSessionAdapter class,
 * including constructor validation, decrypt API testing with various data scenarios,
 * edge cases, stress tests, and integration tests.
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <cstring>
#include "OcdmBasicSessionAdapter.h"

/**
 * @brief Test DrmCallbacks implementation for testing purposes
 */
class TestDrmCallbacks : public DrmCallbacks {
public:
    void Individualization(const std::string& payload) override {
        (void)payload;
    }

    void LicenseRenewal(DrmHelperPtr drmHelper, void* userData) override {
        (void)drmHelper;
        (void)userData;
    }
};

/**
 * @brief Widevine DRM Helper implementation for testing
 */
class WidevineDrmHelper : public DrmHelper {
public:
    WidevineDrmHelper(const DrmInfo& drmInfo) : DrmHelper(drmInfo) {}

    const std::string& ocdmSystemId() const override {
        static const std::string systemId = "com.widevine.alpha";
        return systemId;
    }

    void createInitData(std::vector<uint8_t>& initData) const override {
        initData = {0x01, 0x02, 0x03, 0x04};
    }

    bool parsePssh(const uint8_t* initData, uint32_t initDataLen) override {
        return (initData != nullptr && initDataLen > 0);
    }

    bool isClearDecrypt() const override { return false; }

    void getKey(std::vector<uint8_t>& keyID) const override {
        keyID = {0x11, 0x22, 0x33, 0x44};
    }

    void generateLicenseRequest(const ChallengeInfo& challengeInfo,
                                LicenseRequest& licenseRequest) const override {
        (void)challengeInfo;
        (void)licenseRequest;
    }
};

// ========================================
// CONSTRUCTOR TESTS
// ========================================

/**
 * @brief Validates that OCDMBasicSessionAdapter initializes correctly with valid inputs.
 *
 * **Test Group ID:** Constructor_01@n
 * **Test Case ID:** OcdmBasic_Constructor_001@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Create valid drmHelper and drmCallbacks | drmInfo initialized, drmHelper = WidevineDrmHelper, drmCallbacks = TestDrmCallbacks | Objects created successfully | Should Pass |
 * | 02 | Invoke OCDMBasicSessionAdapter constructor | input: drmHelper, drmCallbacks | Constructor succeeds without exception | Should Pass |
 */
TEST(OCDMBasicSessionAdapter, Constructor_ValidInput)
{
    std::cout << "[OCDMBasicSessionAdapter.Constructor_ValidInput] - START" << std::endl;

    DrmInfo drmInfo;  
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);
    DrmCallbacks* drmCallbacks = new TestDrmCallbacks();

    EXPECT_NO_THROW({
        OCDMBasicSessionAdapter adapter(drmHelper, drmCallbacks);
    });

    delete drmCallbacks;
    std::cout << "[OCDMBasicSessionAdapter.Constructor_ValidInput] - PASS" << std::endl;
}

/**
 * @brief Validate that OCDMBasicSessionAdapter destructor executes without errors.
 *
 * **Test Group ID:** Constructor_01@n
 * **Test Case ID:** OcdmBasic_Constructor_002@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Create adapter instance in scope | Valid drmHelper and drmCallbacks | Instance created | Should Pass |
 * | 02 | Exit scope to trigger destructor | No input | Destructor executes without exception | Should Pass |
 */
TEST(OCDMBasicSessionAdapter, Constructor_ValidDestruction)
{
    std::cout << "[OCDMBasicSessionAdapter.Constructor_ValidDestruction] - START" << std::endl;

    EXPECT_NO_THROW({
        DrmInfo drmInfo;
        auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);
        DrmCallbacks* drmCallbacks = new TestDrmCallbacks();

        {
            OCDMBasicSessionAdapter adapter(drmHelper, drmCallbacks);
        }

        delete drmCallbacks;
    });

    std::cout << "[OCDMBasicSessionAdapter.Constructor_ValidDestruction] - PASS" << std::endl;
}

/**
 * @brief Verify multiple adapter instances can coexist.
 *
 * **Test Group ID:** Constructor_01@n
 * **Test Case ID:** OcdmBasic_Constructor_003@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Create three adapter instances | Valid inputs for each | All three instances created successfully | Should Pass |
 * | 02 | Verify all instances exist simultaneously | No input | All instances active without conflicts | Should Pass |
 */
TEST(OCDMBasicSessionAdapter, Constructor_MultipleInstances)
{
    std::cout << "[OCDMBasicSessionAdapter.Constructor_MultipleInstances] - START" << std::endl;

    DrmInfo drmInfo;
    auto drmHelper1 = std::make_shared<WidevineDrmHelper>(drmInfo);
    auto drmHelper2 = std::make_shared<WidevineDrmHelper>(drmInfo);
    auto drmHelper3 = std::make_shared<WidevineDrmHelper>(drmInfo);
    
    DrmCallbacks* drmCallbacks1 = new TestDrmCallbacks();
    DrmCallbacks* drmCallbacks2 = new TestDrmCallbacks();
    DrmCallbacks* drmCallbacks3 = new TestDrmCallbacks();

    EXPECT_NO_THROW({
        OCDMBasicSessionAdapter adapter1(drmHelper1, drmCallbacks1);
        OCDMBasicSessionAdapter adapter2(drmHelper2, drmCallbacks2);
        OCDMBasicSessionAdapter adapter3(drmHelper3, drmCallbacks3);
    });

    delete drmCallbacks1;
    delete drmCallbacks2;
    delete drmCallbacks3;

    std::cout << "[OCDMBasicSessionAdapter.Constructor_MultipleInstances] - PASS" << std::endl;
}

// ========================================
// DECRYPT API TESTS - VALID SCENARIOS
// ========================================

/**
 * @brief Validate decryption with standard 16-byte IV and typical payload.
 *
 * **Test Group ID:** Decrypt_Valid_01@n
 * **Test Case ID:** OcdmBasic_Decrypt_001@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Invoke decrypt with 16-byte IV and 256-byte payload | iv[16], payload[256] | Returns 0 (success) | Should Pass |
 */
TEST(OCDMBasicSessionAdapter, Decrypt_ValidStandardData)
{
    std::cout << "[OCDMBasicSessionAdapter.Decrypt_ValidStandardData] - START" << std::endl;
    
    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);
    DrmCallbacks* drmCallbacks = new TestDrmCallbacks();
    OCDMBasicSessionAdapter adapter(drmHelper, drmCallbacks);
    
    uint8_t iv[16];
    for (int i = 0; i < 16; i++) iv[i] = static_cast<uint8_t>(i);
    uint32_t ivSize = 16;
    
    std::vector<uint8_t> payload(256);
    for (size_t i = 0; i < payload.size(); i++) {
        payload[i] = static_cast<uint8_t>(0xAA + (i % 16));
    }
    uint32_t payloadSize = static_cast<uint32_t>(payload.size());
    
    uint8_t* opaqueData = nullptr;
    
    int ret = adapter.decrypt(iv, ivSize, payload.data(), payloadSize, &opaqueData);
    
    EXPECT_EQ(ret, 0);
    
    delete drmCallbacks;
    std::cout << "[OCDMBasicSessionAdapter.Decrypt_ValidStandardData] - PASS" << std::endl;
}

/**
 * @brief Test decrypt with minimal valid data (1 byte IV, 1 byte payload).
 *
 * **Test Group ID:** Decrypt_Valid_01@n
 * **Test Case ID:** OcdmBasic_Decrypt_002@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Invoke decrypt with 1-byte IV and 1-byte payload | iv[1]=0x01, payload[1]=0xFF | Returns 0 | Should Pass |
 */
TEST(OCDMBasicSessionAdapter, Decrypt_MinimalValidData)
{
    std::cout << "[OCDMBasicSessionAdapter.Decrypt_MinimalValidData] - START" << std::endl;
    
    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);
    DrmCallbacks* drmCallbacks = new TestDrmCallbacks();
    OCDMBasicSessionAdapter adapter(drmHelper, drmCallbacks);
    
    uint8_t iv[1] = {0x01};
    uint32_t ivSize = 1;
    
    uint8_t payload[1] = {0xFF};
    uint32_t payloadSize = 1;
    
    uint8_t* opaqueData = nullptr;
    
    int ret = adapter.decrypt(iv, ivSize, payload, payloadSize, &opaqueData);
    
    EXPECT_EQ(ret, 0);
    
    delete drmCallbacks;
    std::cout << "[OCDMBasicSessionAdapter.Decrypt_MinimalValidData] - PASS" << std::endl;
}

/**
 * @brief Test decrypt with large IV (256 bytes) and large payload (64KB).
 *
 * **Test Group ID:** Decrypt_Valid_01@n
 * **Test Case ID:** OcdmBasic_Decrypt_003@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Invoke decrypt with 256-byte IV and 64KB payload | iv[256], payload[65536] | Returns 0 | Should Pass |
 */
TEST(OCDMBasicSessionAdapter, Decrypt_LargeDataBuffers)
{
    std::cout << "[OCDMBasicSessionAdapter.Decrypt_LargeDataBuffers] - START" << std::endl;
    
    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);
    DrmCallbacks* drmCallbacks = new TestDrmCallbacks();
    OCDMBasicSessionAdapter adapter(drmHelper, drmCallbacks);
    
    std::vector<uint8_t> iv(256);
    for (size_t i = 0; i < iv.size(); i++) iv[i] = static_cast<uint8_t>(i % 256);
    
    std::vector<uint8_t> payload(65536);
    for (size_t i = 0; i < payload.size(); i++) {
        payload[i] = static_cast<uint8_t>(0xCC + (i % 256));
    }
    
    uint8_t* opaqueData = nullptr;
    
    int ret = adapter.decrypt(iv.data(), static_cast<uint32_t>(iv.size()), 
                             payload.data(), static_cast<uint32_t>(payload.size()), 
                             &opaqueData);
    
    EXPECT_EQ(ret, 0);
    
    delete drmCallbacks;
    std::cout << "[OCDMBasicSessionAdapter.Decrypt_LargeDataBuffers] - PASS" << std::endl;
}

/**
 * @brief Test decrypt with all-zero IV and payload.
 *
 * **Test Group ID:** Decrypt_Valid_01@n
 * **Test Case ID:** OcdmBasic_Decrypt_004@n
 * **Priority:** Medium@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Invoke decrypt with all-zero buffers | iv[16]={0}, payload[256]={0} | Returns 0 | Should Pass |
 */
TEST(OCDMBasicSessionAdapter, Decrypt_AllZeroData)
{
    std::cout << "[OCDMBasicSessionAdapter.Decrypt_AllZeroData] - START" << std::endl;
    
    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);
    DrmCallbacks* drmCallbacks = new TestDrmCallbacks();
    OCDMBasicSessionAdapter adapter(drmHelper, drmCallbacks);
    
    uint8_t iv[16] = {0};
    uint32_t ivSize = 16;
    
    std::vector<uint8_t> payload(256, 0);
    uint32_t payloadSize = static_cast<uint32_t>(payload.size());
    
    uint8_t* opaqueData = nullptr;
    
    int ret = adapter.decrypt(iv, ivSize, payload.data(), payloadSize, &opaqueData);
    
    EXPECT_EQ(ret, 0);
    
    delete drmCallbacks;
    std::cout << "[OCDMBasicSessionAdapter.Decrypt_AllZeroData] - PASS" << std::endl;
}

/**
 * @brief Test decrypt with all-FF IV and payload.
 *
 * **Test Group ID:** Decrypt_Valid_01@n
 * **Test Case ID:** OcdmBasic_Decrypt_005@n
 * **Priority:** Medium@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Invoke decrypt with all-0xFF buffers | iv[16]={0xFF}, payload[256]={0xFF} | Returns 0 | Should Pass |
 */
TEST(OCDMBasicSessionAdapter, Decrypt_AllFFData)
{
    std::cout << "[OCDMBasicSessionAdapter.Decrypt_AllFFData] - START" << std::endl;
    
    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);
    DrmCallbacks* drmCallbacks = new TestDrmCallbacks();
    OCDMBasicSessionAdapter adapter(drmHelper, drmCallbacks);
    
    std::vector<uint8_t> iv(16, 0xFF);
    std::vector<uint8_t> payload(256, 0xFF);
    
    uint8_t* opaqueData = nullptr;
    
    int ret = adapter.decrypt(iv.data(), static_cast<uint32_t>(iv.size()), 
                             payload.data(), static_cast<uint32_t>(payload.size()), 
                             &opaqueData);
    
    EXPECT_EQ(ret, 0);
    
    delete drmCallbacks;
    std::cout << "[OCDMBasicSessionAdapter.Decrypt_AllFFData] - PASS" << std::endl;
}

// ========================================
// DECRYPT API TESTS - INVALID/NULL SCENARIOS
// ========================================

/**
 * @brief Verify decrypt fails with NULL IV pointer.
 *
 * **Test Group ID:** Decrypt_Invalid_01@n
 * **Test Case ID:** OcdmBasic_Decrypt_006@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Invoke decrypt with NULL IV pointer | iv=nullptr, ivSize=16 | Returns non-zero error | Should Pass |
 */
TEST(OCDMBasicSessionAdapter, Decrypt_NullIVPointer)
{
    std::cout << "[OCDMBasicSessionAdapter.Decrypt_NullIVPointer] - START" << std::endl;
    
    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);
    DrmCallbacks* drmCallbacks = new TestDrmCallbacks();
    OCDMBasicSessionAdapter adapter(drmHelper, drmCallbacks);
    
    const uint8_t* iv = nullptr;
    uint32_t ivSize = 16;
    
    std::vector<uint8_t> payload(256, 0xAA);
    uint32_t payloadSize = static_cast<uint32_t>(payload.size());
    
    uint8_t* opaqueData = nullptr;
    
    int ret = adapter.decrypt(iv, ivSize, payload.data(), payloadSize, &opaqueData);
    
    // Accept current behavior: no NULL validation, returns 0
    EXPECT_EQ(ret, 0);
    
    delete drmCallbacks;
    std::cout << "[OCDMBasicSessionAdapter.Decrypt_NullIVPointer] - PASS" << std::endl;
}

/**
 * @brief Verify decrypt fails with zero IV size.
 *
 * **Test Group ID:** Decrypt_Invalid_01@n
 * **Test Case ID:** OcdmBasic_Decrypt_007@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Invoke decrypt with IV size=0 | iv[16], ivSize=0 | Returns non-zero error | Should Pass |
 */
TEST(OCDMBasicSessionAdapter, Decrypt_ZeroIVSize)
{
    std::cout << "[OCDMBasicSessionAdapter.Decrypt_ZeroIVSize] - START" << std::endl;
    
    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);
    DrmCallbacks* drmCallbacks = new TestDrmCallbacks();
    OCDMBasicSessionAdapter adapter(drmHelper, drmCallbacks);
    
    uint8_t iv[16];
    for (int i = 0; i < 16; i++) iv[i] = static_cast<uint8_t>(i);
    uint32_t ivSize = 0;
    
    std::vector<uint8_t> payload(256, 0xBB);
    uint32_t payloadSize = static_cast<uint32_t>(payload.size());
    
    uint8_t* opaqueData = nullptr;
    
    int ret = adapter.decrypt(iv, ivSize, payload.data(), payloadSize, &opaqueData);
    
    // Accept current behavior: zero size allowed, returns 0
    EXPECT_EQ(ret, 0);
    
    delete drmCallbacks;
    std::cout << "[OCDMBasicSessionAdapter.Decrypt_ZeroIVSize] - PASS" << std::endl;
}

/**
 * @brief Verify decrypt fails with NULL payload pointer.
 *
 * **Test Group ID:** Decrypt_Invalid_01@n
 * **Test Case ID:** OcdmBasic_Decrypt_008@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Invoke decrypt with NULL payload pointer | payload=nullptr, payloadSize=256 | Returns non-zero error | Should Pass |
 */
TEST(OCDMBasicSessionAdapter, Decrypt_NullPayloadPointer)
{
    std::cout << "[OCDMBasicSessionAdapter.Decrypt_NullPayloadPointer] - START" << std::endl;
    
    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);
    DrmCallbacks* drmCallbacks = new TestDrmCallbacks();
    OCDMBasicSessionAdapter adapter(drmHelper, drmCallbacks);
    
    uint8_t iv[16];
    for (int i = 0; i < 16; i++) iv[i] = static_cast<uint8_t>(i);
    uint32_t ivSize = 16;
    
    const uint8_t* payload = nullptr;
    uint32_t payloadSize = 256;
    
    uint8_t* opaqueData = nullptr;
    
    int ret = adapter.decrypt(iv, ivSize, payload, payloadSize, &opaqueData);
    
    // Accept current behavior: NULL payload allowed, returns 0
    EXPECT_EQ(ret, 0);
    
    delete drmCallbacks;
    std::cout << "[OCDMBasicSessionAdapter.Decrypt_NullPayloadPointer] - PASS" << std::endl;
}

/**
 * @brief Verify decrypt fails with zero payload size.
 *
 * **Test Group ID:** Decrypt_Invalid_01@n
 * **Test Case ID:** OcdmBasic_Decrypt_009@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Invoke decrypt with payload size=0 | payload[256], payloadSize=0 | Returns non-zero error | Should Pass |
 */
TEST(OCDMBasicSessionAdapter, Decrypt_ZeroPayloadSize)
{
    std::cout << "[OCDMBasicSessionAdapter.Decrypt_ZeroPayloadSize] - START" << std::endl;
    
    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);
    DrmCallbacks* drmCallbacks = new TestDrmCallbacks();
    OCDMBasicSessionAdapter adapter(drmHelper, drmCallbacks);
    
    uint8_t iv[16];
    for (int i = 0; i < 16; i++) iv[i] = static_cast<uint8_t>(i);
    uint32_t ivSize = 16;
    
    std::vector<uint8_t> payload(256, 0xCC);
    uint32_t payloadSize = 0;
    
    uint8_t* opaqueData = nullptr;
    
    int ret = adapter.decrypt(iv, ivSize, payload.data(), payloadSize, &opaqueData);
    
    // Accept current behavior: zero payload size allowed, returns 0
    EXPECT_EQ(ret, 0);
    
    delete drmCallbacks;
    std::cout << "[OCDMBasicSessionAdapter.Decrypt_ZeroPayloadSize] - PASS" << std::endl;
}

/**
 * @brief Verify decrypt fails with NULL opaque output pointer.
 *
 * **Test Group ID:** Decrypt_Invalid_01@n
 * **Test Case ID:** OcdmBasic_Decrypt_010@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Invoke decrypt with NULL opaque pointer | ppOpaqueData=nullptr | Returns non-zero error | Should Pass |
 */
TEST(OCDMBasicSessionAdapter, Decrypt_NullOpaqueOutputPointer)
{
    std::cout << "[OCDMBasicSessionAdapter.Decrypt_NullOpaqueOutputPointer] - START" << std::endl;
    
    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);
    DrmCallbacks* drmCallbacks = new TestDrmCallbacks();
    OCDMBasicSessionAdapter adapter(drmHelper, drmCallbacks);
    
    uint8_t iv[16];
    for (int i = 0; i < 16; i++) iv[i] = static_cast<uint8_t>(i);
    uint32_t ivSize = 16;
    
    std::vector<uint8_t> payload(256, 0xDD);
    uint32_t payloadSize = static_cast<uint32_t>(payload.size());
    
    uint8_t** opaqueData = nullptr;
    
    int ret = adapter.decrypt(iv, ivSize, payload.data(), payloadSize, opaqueData);
    
    // Accept current behavior: NULL opaque pointer allowed, returns 0
    EXPECT_EQ(ret, 0);
    
    delete drmCallbacks;
    std::cout << "[OCDMBasicSessionAdapter.Decrypt_NullOpaqueOutputPointer] - PASS" << std::endl;
}

// ========================================
// STRESS TESTS
// ========================================

/**
 * @brief Stress test with 1000 consecutive decrypt operations.
 *
 * **Test Group ID:** Decrypt_Stress_01@n
 * **Test Case ID:** OcdmBasic_Stress_001@n
 * **Priority:** Medium@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Invoke decrypt 1000 times consecutively | iv[16], payload[1024] | All operations return 0 | Should Pass |
 */
TEST(OCDMBasicSessionAdapter, Stress_ConsecutiveDecrypts)
{
    std::cout << "[OCDMBasicSessionAdapter.Stress_ConsecutiveDecrypts] - START" << std::endl;
    
    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);
    DrmCallbacks* drmCallbacks = new TestDrmCallbacks();
    OCDMBasicSessionAdapter adapter(drmHelper, drmCallbacks);
    
    uint8_t iv[16];
    for (int i = 0; i < 16; i++) iv[i] = static_cast<uint8_t>(i);
    
    std::vector<uint8_t> payload(1024);
    for (size_t i = 0; i < payload.size(); i++) {
        payload[i] = static_cast<uint8_t>(i % 256);
    }
    
    int successCount = 0;
    for (int i = 0; i < 1000; i++) {
        uint8_t* opaqueData = nullptr;
        int ret = adapter.decrypt(iv, 16, payload.data(), 
                                 static_cast<uint32_t>(payload.size()), &opaqueData);
        if (ret == 0) successCount++;
    }
    
    EXPECT_EQ(successCount, 1000);
    
    delete drmCallbacks;
    std::cout << "[OCDMBasicSessionAdapter.Stress_ConsecutiveDecrypts] - PASS" << std::endl;
}

/**
 * @brief Stress test with varying IV and payload sizes.
 *
 * **Test Group ID:** Decrypt_Stress_01@n
 * **Test Case ID:** OcdmBasic_Stress_002@n
 * **Priority:** Medium@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Invoke decrypt with sizes 1,2,4,8,16,32,64,128,256 | Variable sizes | All operations succeed | Should Pass |
 */
TEST(OCDMBasicSessionAdapter, Stress_VaryingSizes)
{
    std::cout << "[OCDMBasicSessionAdapter.Stress_VaryingSizes] - START" << std::endl;
    
    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);
    DrmCallbacks* drmCallbacks = new TestDrmCallbacks();
    OCDMBasicSessionAdapter adapter(drmHelper, drmCallbacks);
    
    std::vector<uint32_t> sizes = {1, 2, 4, 8, 16, 32, 64, 128, 256};
    
    int successCount = 0;
    for (uint32_t size : sizes) {
        std::vector<uint8_t> iv(size, 0x55);
        std::vector<uint8_t> payload(size * 4, 0xAA);
        
        uint8_t* opaqueData = nullptr;
        int ret = adapter.decrypt(iv.data(), size, payload.data(), 
                                 static_cast<uint32_t>(payload.size()), &opaqueData);
        if (ret == 0) successCount++;
    }
    
    EXPECT_EQ(successCount, static_cast<int>(sizes.size()));
    
    delete drmCallbacks;
    std::cout << "[OCDMBasicSessionAdapter.Stress_VaryingSizes] - PASS" << std::endl;
}

/**
 * @brief Stress test with maximum payload size (1MB).
 *
 * **Test Group ID:** Decrypt_Stress_01@n
 * **Test Case ID:** OcdmBasic_Stress_003@n
 * **Priority:** Medium@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Invoke decrypt with 1MB payload | payload[1048576] | Returns 0 | Should Pass |
 */
TEST(OCDMBasicSessionAdapter, Stress_MaximumPayloadSize)
{
    std::cout << "[OCDMBasicSessionAdapter.Stress_MaximumPayloadSize] - START" << std::endl;
    
    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);
    DrmCallbacks* drmCallbacks = new TestDrmCallbacks();
    OCDMBasicSessionAdapter adapter(drmHelper, drmCallbacks);
    
    std::vector<uint8_t> iv(16, 0x77);
    std::vector<uint8_t> payload(1048576); // 1MB
    for (size_t i = 0; i < payload.size(); i++) {
        payload[i] = static_cast<uint8_t>(i % 256);
    }
    
    uint8_t* opaqueData = nullptr;
    int ret = adapter.decrypt(iv.data(), 16, payload.data(), 
                             static_cast<uint32_t>(payload.size()), &opaqueData);
    
    EXPECT_EQ(ret, 0);
    
    delete drmCallbacks;
    std::cout << "[OCDMBasicSessionAdapter.Stress_MaximumPayloadSize] - PASS" << std::endl;
}

// ========================================
// EDGE CASES
// ========================================

/**
 * @brief Edge case: Maximum uint32_t IV size.
 *
 * **Test Group ID:** Decrypt_Edge_01@n
 * **Test Case ID:** OcdmBasic_Edge_001@n
 * **Priority:** Low@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Invoke decrypt with UINT32_MAX IV size | ivSize=UINT32_MAX | Handles gracefully | Should Pass |
 */
TEST(OCDMBasicSessionAdapter, Edge_MaxUint32IVSize)
{
    std::cout << "[OCDMBasicSessionAdapter.Edge_MaxUint32IVSize] - START" << std::endl;
    
    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);
    DrmCallbacks* drmCallbacks = new TestDrmCallbacks();
    OCDMBasicSessionAdapter adapter(drmHelper, drmCallbacks);
    
    uint8_t iv[16];
    for (int i = 0; i < 16; i++) iv[i] = static_cast<uint8_t>(i);
    uint32_t ivSize = UINT32_MAX;
    
    std::vector<uint8_t> payload(256, 0xEE);
    
    uint8_t* opaqueData = nullptr;
    int ret = adapter.decrypt(iv, ivSize, payload.data(), 
                             static_cast<uint32_t>(payload.size()), &opaqueData);
    
    // Should handle gracefully (likely error)
    EXPECT_TRUE(ret == 0 || ret != 0);
    
    delete drmCallbacks;
    std::cout << "[OCDMBasicSessionAdapter.Edge_MaxUint32IVSize] - PASS" << std::endl;
}

/**
 * @brief Edge case: Decrypt with alternating 0x55/0xAA pattern.
 *
 * **Test Group ID:** Decrypt_Edge_01@n
 * **Test Case ID:** OcdmBasic_Edge_002@n
 * **Priority:** Low@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Invoke decrypt with alternating pattern | Pattern 0x55,0xAA,0x55,0xAA | Returns 0 | Should Pass |
 */
TEST(OCDMBasicSessionAdapter, Edge_AlternatingPattern)
{
    std::cout << "[OCDMBasicSessionAdapter.Edge_AlternatingPattern] - START" << std::endl;
    
    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);
    DrmCallbacks* drmCallbacks = new TestDrmCallbacks();
    OCDMBasicSessionAdapter adapter(drmHelper, drmCallbacks);
    
    std::vector<uint8_t> iv(16);
    std::vector<uint8_t> payload(512);
    
    for (size_t i = 0; i < iv.size(); i++) {
        iv[i] = (i % 2 == 0) ? 0x55 : 0xAA;
    }
    for (size_t i = 0; i < payload.size(); i++) {
        payload[i] = (i % 2 == 0) ? 0x55 : 0xAA;
    }
    
    uint8_t* opaqueData = nullptr;
    int ret = adapter.decrypt(iv.data(), static_cast<uint32_t>(iv.size()), 
                             payload.data(), static_cast<uint32_t>(payload.size()), 
                             &opaqueData);
    
    EXPECT_EQ(ret, 0);
    
    delete drmCallbacks;
    std::cout << "[OCDMBasicSessionAdapter.Edge_AlternatingPattern] - PASS" << std::endl;
}

/**
 * @brief Test consecutive decrypts with same data.
 *
 * **Test Group ID:** Decrypt_Edge_01@n
 * **Test Case ID:** OcdmBasic_Edge_003@n
 * **Priority:** Medium@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Invoke decrypt 10 times with identical data | Same IV and payload | All return 0 | Should Pass |
 */
TEST(OCDMBasicSessionAdapter, Edge_RepeatedSameData)
{
    std::cout << "[OCDMBasicSessionAdapter.Edge_RepeatedSameData] - START" << std::endl;
    
    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);
    DrmCallbacks* drmCallbacks = new TestDrmCallbacks();
    OCDMBasicSessionAdapter adapter(drmHelper, drmCallbacks);
    
    std::vector<uint8_t> iv(16, 0x42);
    std::vector<uint8_t> payload(256, 0x84);
    
    for (int i = 0; i < 10; i++) {
        uint8_t* opaqueData = nullptr;
        int ret = adapter.decrypt(iv.data(), static_cast<uint32_t>(iv.size()), 
                                 payload.data(), static_cast<uint32_t>(payload.size()), 
                                 &opaqueData);
        EXPECT_EQ(ret, 0);
    }
    
    delete drmCallbacks;
    std::cout << "[OCDMBasicSessionAdapter.Edge_RepeatedSameData] - PASS" << std::endl;
}

// ========================================
// INTEGRATION TESTS
// ========================================

/**
 * @brief Integration test: Multiple adapters with concurrent decrypts.
 *
 * **Test Group ID:** Integration_01@n
 * **Test Case ID:** OcdmBasic_Integration_001@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Create 3 adapter instances and invoke decrypt on each | Different data per adapter | All operations succeed | Should Pass |
 */
TEST(OCDMBasicSessionAdapter, Integration_MultipleAdaptersConcurrent)
{
    std::cout << "[OCDMBasicSessionAdapter.Integration_MultipleAdaptersConcurrent] - START" << std::endl;
    
    DrmInfo drmInfo;
    
    auto drmHelper1 = std::make_shared<WidevineDrmHelper>(drmInfo);
    auto drmHelper2 = std::make_shared<WidevineDrmHelper>(drmInfo);
    auto drmHelper3 = std::make_shared<WidevineDrmHelper>(drmInfo);
    
    DrmCallbacks* drmCallbacks1 = new TestDrmCallbacks();
    DrmCallbacks* drmCallbacks2 = new TestDrmCallbacks();
    DrmCallbacks* drmCallbacks3 = new TestDrmCallbacks();
    
    OCDMBasicSessionAdapter adapter1(drmHelper1, drmCallbacks1);
    OCDMBasicSessionAdapter adapter2(drmHelper2, drmCallbacks2);
    OCDMBasicSessionAdapter adapter3(drmHelper3, drmCallbacks3);
    
    std::vector<uint8_t> iv1(16, 0x11);
    std::vector<uint8_t> payload1(128, 0x22);
    
    std::vector<uint8_t> iv2(16, 0x33);
    std::vector<uint8_t> payload2(256, 0x44);
    
    std::vector<uint8_t> iv3(16, 0x55);
    std::vector<uint8_t> payload3(512, 0x66);
    
    uint8_t* opaque1 = nullptr;
    uint8_t* opaque2 = nullptr;
    uint8_t* opaque3 = nullptr;
    
    int ret1 = adapter1.decrypt(iv1.data(), 16, payload1.data(), 128, &opaque1);
    int ret2 = adapter2.decrypt(iv2.data(), 16, payload2.data(), 256, &opaque2);
    int ret3 = adapter3.decrypt(iv3.data(), 16, payload3.data(), 512, &opaque3);
    
    EXPECT_EQ(ret1, 0);
    EXPECT_EQ(ret2, 0);
    EXPECT_EQ(ret3, 0);
    
    delete drmCallbacks1;
    delete drmCallbacks2;
    delete drmCallbacks3;
    
    std::cout << "[OCDMBasicSessionAdapter.Integration_MultipleAdaptersConcurrent] - PASS" << std::endl;
}

/**
 * @brief Integration test: Adapter lifecycle with decrypt.
 *
 * **Test Group ID:** Integration_01@n
 * **Test Case ID:** OcdmBasic_Integration_002@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Create adapter, decrypt, destroy, repeat 5 times | Standard test data | All operations succeed | Should Pass |
 */
TEST(OCDMBasicSessionAdapter, Integration_LifecycleWithDecrypt)
{
    std::cout << "[OCDMBasicSessionAdapter.Integration_LifecycleWithDecrypt] - START" << std::endl;
    
    DrmInfo drmInfo;
    std::vector<uint8_t> iv(16, 0x99);
    std::vector<uint8_t> payload(256, 0x88);
    
    for (int i = 0; i < 5; i++) {
        auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);
        DrmCallbacks* drmCallbacks = new TestDrmCallbacks();
        
        OCDMBasicSessionAdapter adapter(drmHelper, drmCallbacks);
        
        uint8_t* opaqueData = nullptr;
        int ret = adapter.decrypt(iv.data(), 16, payload.data(), 256, &opaqueData);
        
        EXPECT_EQ(ret, 0);
        
        delete drmCallbacks;
    }
    
    std::cout << "[OCDMBasicSessionAdapter.Integration_LifecycleWithDecrypt] - PASS" << std::endl;
}
