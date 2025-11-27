
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
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <stdio.h>
#include "OcdmBasicSessionAdapter.h"

class TestDrmCallbacks : public DrmCallbacks {
public:
    void Individualization(const std::string& payload) override {
        std::cout << "Individualization callback invoked with payload: " << payload << std::endl;
    }

    void LicenseRenewal(DrmHelperPtr drmHelper, void* userData) override {
        std::cout << "LicenseRenewal callback invoked." << std::endl;
        (void)drmHelper;
        (void)userData;
    }
};


class WidevineDrmHelper : public DrmHelper {
public:
    WidevineDrmHelper(const DrmInfo& drmInfo) : DrmHelper(drmInfo) {}

    // Implementing required pure virtual methods with dummy logic
    const std::string& ocdmSystemId() const override {
        static const std::string systemId = "WidevineTestSystem";
        return systemId;
    }

    void createInitData(std::vector<uint8_t>& initData) const override {
        initData = {1, 2, 3};
    }

    bool parsePssh(const uint8_t* initData, uint32_t initDataLen) override {
        return (initData != nullptr && initDataLen > 0);
    }

    bool isClearDecrypt() const override { return false; }

    void getKey(std::vector<uint8_t>& keyID) const override {
        keyID = {0x11, 0x22, 0x33};
    }

    void generateLicenseRequest(const ChallengeInfo& challengeInfo,
                                LicenseRequest& licenseRequest) const override {
        (void)challengeInfo;
        (void)licenseRequest;
    }
};


/**
 * @brief Validates that OCDMBasicSessionAdapter initializes correctly with valid inputs.
 *
 * This test confirms that when valid drmHelper and drmCallbacks objects are provided, the OCDMBasicSessionAdapter constructor
 * does not throw any exceptions and initializes its internal members properly. Ensuring robust initialization under valid conditions
 * is crucial for correct session management.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 001
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                             | Test Data                                                                                           | Expected Result                                                           | Notes      |
 * | :--------------: | ----------------------------------------------------------------------- | --------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------- | ---------- |
 * | 01               | Create valid drmHelper (derived object from WidevineDrmHelper) and drmCallbacks on heap | drmInfo = default, drmHelper = instance of WidevineDrmHelper, drmCallbacks = instance of TestDrmCallbacks | Objects are created successfully with valid addresses                     | Should be successful |
 * | 02               | Invoke OCDMBasicSessionAdapter constructor with valid drmHelper and drmCallbacks  | input: drmHelper (valid pointer), drmCallbacks (valid pointer)                                      | Constructor does not throw; internal m_drmHelper and m_drmCallbacks are properly initialized | Should Pass |
 */
TEST(OCDMBasicSessionAdapter, ValidInput)
{
    std::cout << "Entering ValidInput test" << std::endl;

    // Arrange
    DrmInfo drmInfo;  
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);  // Using derived class
    std::cout << "Created drmHelper of type WidevineDrmHelper at address: " << drmHelper.get() << std::endl;

    // Create a valid callback object on heap
    TestDrmCallbacks* drmCallbacks = new TestDrmCallbacks();
    std::cout << "Created drmCallbacks at address: " << drmCallbacks << std::endl;

    // Act & Assert
    EXPECT_NO_THROW({
        std::cout << "Invoking OCDMBasicSessionAdapter constructor with valid drmHelper and drmCallbacks" << std::endl;
        OCDMBasicSessionAdapter adapter(drmHelper, drmCallbacks);
        std::cout << "OCDMBasicSessionAdapter created successfully." << std::endl;
    });

    std::cout << "Expected: constructor should initialize internal m_drmHelper and m_drmCallbacks properly." << std::endl;

    delete drmCallbacks; // Clean up allocated callback object
    std::cout << "Exiting ValidInput test" << std::endl;
}
/**
 * @brief Validate exception thrown when constructing OCDMBasicSessionAdapter with null drmHelper.
 *
 * Test verifies that the OCDMBasicSessionAdapter constructor throws an exception when provided with a null drmHelper and a valid drmCallbacks, ensuring proper error handling for invalid input.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 002
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | ------------- | ----- |
 * | 01 | Invoke OCDMBasicSessionAdapter constructor with null drmHelper and valid drmCallbacks | drmHelper = nullptr, drmCallbacks = valid non-null pointer | Constructor throws an exception as verified by EXPECT_ANY_THROW | Should Pass |
 */
TEST(OCDMBasicSessionAdapter, NullDrmHelper)
{
    std::cout << "Entering NullDrmHelper test" << std::endl;

    // Arrange
    std::shared_ptr<DrmHelper> nullDrmHelper = nullptr;
    std::cout << "Using nullDrmHelper (address: " << nullDrmHelper.get() << ")" << std::endl;

    // Create valid callbacks derived object
    TestDrmCallbacks* drmCallbacks = new TestDrmCallbacks();
    std::cout << "Created drmCallbacks at address: " << drmCallbacks << std::endl;

    // Act & Assert
    EXPECT_ANY_THROW({
        std::cout << "Invoking OCDMBasicSessionAdapter constructor with null drmHelper and valid drmCallbacks" << std::endl;
        OCDMBasicSessionAdapter adapter(nullDrmHelper, drmCallbacks);
        std::cout << "OCDMBasicSessionAdapter constructed with null drmHelper (unexpected)" << std::endl;
    });

    std::cout << "Exiting NullDrmHelper test" << std::endl;

    delete drmCallbacks;
}
/**
 * @brief Tests that OCDMBasicSessionAdapter throws an exception when initialized with null DRM callbacks.
 *
 * This test verifies that the OCDMBasicSessionAdapter constructor properly handles a null drmCallbacks pointer while receiving a valid drmHelper. By attempting to construct the adapter with these parameters, the test ensures that an exception is thrown as expected, which confirms the robustness of error handling in the DRM management.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 003@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Arrange valid drmHelper and set drmCallbacks to null | drmHelper = instance of WidevineDrmHelper, drmCallbacks = nullptr | drmHelper is instantiated; drmCallbacks is null | Should be successful |
 * | 02 | Invoke OCDMBasicSessionAdapter constructor with valid drmHelper and null drmCallbacks | input: drmHelper (valid instance address), drmCallbacks (nullptr); output: exception thrown | Exception is thrown during construction (assertion passes) | Should Pass |
 */
TEST(OCDMBasicSessionAdapter, NullDrmCallbacks)
{
    std::cout << "Entering NullDrmCallbacks test" << std::endl;

    // Arrange: Create valid drmHelper using derived class
    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);
    std::cout << "Created drmHelper of type WidevineDrmHelper at address: " << drmHelper.get() << std::endl;

    // drmCallbacks is null
    DrmCallbacks* nullDrmCallbacks = nullptr;
    std::cout << "Using nullDrmCallbacks (address: " << nullDrmCallbacks << ")" << std::endl;

    // Act & Assert
    EXPECT_ANY_THROW({
        std::cout << "Invoking OCDMBasicSessionAdapter constructor with valid drmHelper and null drmCallbacks" << std::endl;
        OCDMBasicSessionAdapter adapter(drmHelper, nullDrmCallbacks);
        std::cout << "OCDMBasicSessionAdapter constructed with null drmCallbacks (unexpected)" << std::endl;
    });

    std::cout << "Exiting NullDrmCallbacks test" << std::endl;
}
/**
 * @brief Validate decryption function using valid IV and payload data
 *
 * This test verifies that the decrypt() method of OCDMBasicSessionAdapter returns a successful result (0) when invoked with valid IV, payload data, and a valid output pointer. It also checks that the opaque data is handled and logged properly if available.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 004@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                           | Test Data                                                                                   | Expected Result                                                           | Notes      |
 * | :--------------: | --------------------------------------------------------------------- | ------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------- | ---------- |
 * | 01               | Invoke decrypt API with valid IV, payload and output pointer          | iv = {0x01,0x02,0x03,0x04}, ivSize = 4, payload = {0xAA,0xBB,0xCC}, payloadSize = 3, ppOpaqueData = pointer to opaqueData (initially nullptr) | Return value is 0 and assertion EXPECT_EQ(ret, 0) passes                  | Should Pass |
 */
TEST(OCDMBasicSessionAdapter, ValidDecryption)
{
    std::cout << "Entering ValidDecryption test" << std::endl;
    
    DrmInfo drmInfo;  
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);  // Using derived class
    std::cout << "Created drmHelper of type WidevineDrmHelper at address: " << drmHelper.get() << std::endl;

    // Create a valid callback object on heap
    TestDrmCallbacks* drmCallbacks = new TestDrmCallbacks();
    std::cout << "Created drmCallbacks at address: " << drmCallbacks << std::endl;
    OCDMBasicSessionAdapter adapter(drmHelper, drmCallbacks);
    
    // Prepare IV: {0x01, 0x02, 0x03, 0x04}
    uint8_t iv[4] = {0};
    char iv_init[5] = "\x01\x02\x03\x04";
    strncpy((char*)iv, iv_init, 4);
    std::cout << "IV set to: ";
    for(uint32_t i = 0; i < 4; i++){
        std::cout << "0x" << std::hex << (int)iv[i] << " ";
    }
    std::cout << std::dec << std::endl;
    
    uint32_t ivSize = 4;
    
    // Prepare payloadData: {0xAA, 0xBB, 0xCC}
    uint8_t payload[3] = {0};
    char payload_init[4] = "\xAA\xBB\xCC";
    strncpy((char*)payload, payload_init, 3);
    std::cout << "Payload data set to: ";
    for(uint32_t i = 0; i < 3; i++){
        std::cout << "0x" << std::hex << (int)payload[i] << " ";
    }
    std::cout << std::dec << std::endl;
    
    uint32_t payloadSize = 3;
    
    // Prepare output pointer for opaque data
    uint8_t* opaqueData = nullptr;
    uint8_t** ppOpaqueData = &opaqueData;
    
    std::cout << "Invoking decrypt with valid IV, payload and output pointer." << std::endl;
    int ret = adapter.decrypt(iv, ivSize, payload, payloadSize, ppOpaqueData);
    std::cout << "decrypt returned: " << ret << std::endl;
    
    // Expected: ret should be 0 for success
    EXPECT_EQ(ret, 0);
    
    // Print key information if opaqueData is not nullptr
    if (opaqueData != nullptr) {
         // Simulated log of decrypted opaque key information (assuming first few bytes represent key info)
         std::cout << "Decrypted opaque data key info: ";
         for(uint32_t i = 0; i < payloadSize; i++){
             std::cout << "0x" << std::hex << (int)opaqueData[i] << " ";
         }
         std::cout << std::dec << std::endl;
    } else {
         std::cout << "No opaque data returned." << std::endl;
    }
    
    std::cout << "Exiting ValidDecryption test" << std::endl;
}
/**
 * @brief Verify that decrypt fails when provided with a NULL IV pointer.
 *
 * This test validates that the OCDMBasicSessionAdapter::decrypt API correctly handles a scenario where the IV pointer is NULL. The adapter is instantiated with null parameters, and valid payload data is prepared. When invoking decrypt with a NULL IV pointer, the API is expected to return a non-zero error code indicating failure.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 005@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Instantiate OCDMBasicSessionAdapter with null pointers to ensure no exception is thrown during object creation. | input1 = nullptr, input2 = nullptr | Adapter instance is created successfully with no exception. | Should Pass |
 * | 02 | Prepare test inputs by setting the IV pointer to nullptr (ivSize = 4), initializing valid payload data {0xAA, 0xBB, 0xCC} (payloadSize = 3), and setting the opaqueData pointer to nullptr. | iv = nullptr, ivSize = 4, payload = 0xAA, 0xBB, 0xCC, payloadSize = 3, opaqueData = nullptr | Test inputs are correctly set for invoking the API. | Should be successful |
 * | 03 | Invoke the decrypt API using the NULL IV pointer and verify that the return value is non-zero, indicating failure due to invalid IV pointer. | iv = nullptr, ivSize = 4, payload = 0xAA, 0xBB, 0xCC, payloadSize = 3, ppOpaqueData = &opaqueData | Return value is non-zero; assertion EXPECT_NE(ret, 0) passes. | Should Fail |
 */
TEST(OCDMBasicSessionAdapter, NullIVPointer)
{
    std::cout << "Entering NullIVPointer test" << std::endl;
    
    // Arrange
    DrmInfo drmInfo;  
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);  // Using derived class
    std::cout << "Created drmHelper of type WidevineDrmHelper at address: " << drmHelper.get() << std::endl;
        
    // Create a valid callback object on heap
    TestDrmCallbacks* drmCallbacks = new TestDrmCallbacks();
    std::cout << "Created drmCallbacks at address: " << drmCallbacks << std::endl;

    OCDMBasicSessionAdapter adapter(drmHelper, drmCallbacks);
    
    // IV pointer is NULL
    const uint8_t* iv = nullptr;
    uint32_t ivSize = 4;
    
    // Prepare valid payloadData: {0xAA, 0xBB, 0xCC}
    uint8_t payload[3] = {0};
    char payload_init[4] = "\xAA\xBB\xCC";
    strncpy((char*)payload, payload_init, 3);
    std::cout << "Payload data set to: ";
    for(uint32_t i = 0; i < 3; i++){
         std::cout << "0x" << std::hex << (int)payload[i] << " ";
    }
    std::cout << std::dec << std::endl;
    
    uint32_t payloadSize = 3;
    
    // Prepare output pointer for opaque data
    uint8_t* opaqueData = nullptr;
    uint8_t** ppOpaqueData = &opaqueData;
    
    std::cout << "Invoking decrypt with NULL IV pointer." << std::endl;
    int ret = adapter.decrypt(iv, ivSize, payload, payloadSize, ppOpaqueData);
    std::cout << "decrypt returned: " << ret << std::endl;
    
    // Expected: ret should be non-zero indicating failure due to invalid IV pointer
    EXPECT_NE(ret, 0);
    
    std::cout << "Exiting NullIVPointer test" << std::endl;
}
/**
 * @brief Verify that decryption fails when IV size is zero
 *
 * This test verifies that the decrypt API of OCDMBasicSessionAdapter returns an error when provided with an IV size of zero. The test ensures that the function does not accept an invalid IV size, thereby maintaining the integrity of the decryption process.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 006
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoke the decrypt API with IV size set to zero while providing valid IV and payload data. | iv = {0x01, 0x02, 0x03, 0x04}, ivSize = 0, payload = {0xAA, 0xBB, 0xCC}, payloadSize = 3, opaqueData = nullptr | The API should return a non-zero error value indicating failure; EXPECT_NE(ret, 0) assertion should pass. | Should Fail |
 */
TEST(OCDMBasicSessionAdapter, ZeroIVSize)
{
    std::cout << "Entering ZeroIVSize test" << std::endl;
    
    // Arrange
    DrmInfo drmInfo;  
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);  // Using derived class
    std::cout << "Created drmHelper of type WidevineDrmHelper at address: " << drmHelper.get() << std::endl;
        
    // Create a valid callback object on heap
    TestDrmCallbacks* drmCallbacks = new TestDrmCallbacks();
    std::cout << "Created drmCallbacks at address: " << drmCallbacks << std::endl;

    OCDMBasicSessionAdapter adapter(drmHelper, drmCallbacks);
    
    // Prepare IV: {0x01, 0x02, 0x03, 0x04}
    uint8_t iv[4] = {0};
    char iv_init[5] = "\x01\x02\x03\x04";
    strncpy((char*)iv, iv_init, 4);
    std::cout << "IV set to: ";
    for(uint32_t i = 0; i < 4; i++){
         std::cout << "0x" << std::hex << (int)iv[i] << " ";
    }
    std::cout << std::dec << std::endl;
    
    // IV size is zero
    uint32_t ivSize = 0;
    
    // Prepare valid payloadData: {0xAA, 0xBB, 0xCC}
    uint8_t payload[3] = {0};
    char payload_init[4] = "\xAA\xBB\xCC";
    strncpy((char*)payload, payload_init, 3);
    std::cout << "Payload data set to: ";
    for(uint32_t i = 0; i < 3; i++){
         std::cout << "0x" << std::hex << (int)payload[i] << " ";
    }
    std::cout << std::dec << std::endl;
    
    uint32_t payloadSize = 3;
    
    uint8_t* opaqueData = nullptr;
    uint8_t** ppOpaqueData = &opaqueData;
    
    std::cout << "Invoking decrypt with IV size zero." << std::endl;
    int ret = adapter.decrypt(iv, ivSize, payload, payloadSize, ppOpaqueData);
    std::cout << "decrypt returned: " << ret << std::endl;
    
    // Expected: ret should be non-zero due to IV size being zero
    EXPECT_NE(ret, 0);
    
    std::cout << "Exiting ZeroIVSize test" << std::endl;
}
/**
 * @brief Verify that decrypt fails when provided with a NULL payloadData pointer.
 *
 * This test verifies that the decrypt function of the OCDMBasicSessionAdapter correctly handles a NULL payloadData pointer by returning a non-zero value, indicating failure. It ensures that the adapter is constructed without throwing exceptions and that the API correctly handles invalid input.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 007
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                 | Test Data                                                                                     | Expected Result                                                                                           | Notes        |
 * | :--------------: | --------------------------------------------------------------------------- | --------------------------------------------------------------------------------------------- | --------------------------------------------------------------------------------------------------------- | ------------ |
 * | 01               | Construct OCDMBasicSessionAdapter with NULL pointers to ensure no exception. | Constructor arguments: arg1 = nullptr, arg2 = nullptr                                         | No exception is thrown during construction.                                                               | Should Pass  |
 * | 02               | Set up the IV for decryption by initializing a 4-byte IV.                    | IV: iv = {0x01, 0x02, 0x03, 0x04}, ivSize = 4                                                  | IV is correctly set and printed.                                                                          | Should be successful  |
 * | 03               | Invoke the decrypt method with a NULL payloadData pointer and verify failure. | Decrypt inputs: iv = {0x01, 0x02, 0x03, 0x04}, ivSize = 4, payload = nullptr, payloadSize = 3, ppOpaqueData = pointer to NULL  | decrypt returns a non-zero value indicating failure due to an invalid (NULL) payloadData pointer.           | Should Pass  |
 */
TEST(OCDMBasicSessionAdapter, NullPayloadDataPointer)
{
    std::cout << "Entering NullPayloadDataPointer test" << std::endl;
    
    // Arrange
    DrmInfo drmInfo;  
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);  // Using derived class
    std::cout << "Created drmHelper of type WidevineDrmHelper at address: " << drmHelper.get() << std::endl;
        
    // Create a valid callback object on heap
    TestDrmCallbacks* drmCallbacks = new TestDrmCallbacks();
    std::cout << "Created drmCallbacks at address: " << drmCallbacks << std::endl;
    OCDMBasicSessionAdapter adapter(drmHelper, drmCallbacks);
    
    // Prepare IV: {0x01, 0x02, 0x03, 0x04}
    uint8_t iv[4] = {0};
    char iv_init[5] = "\x01\x02\x03\x04";
    strncpy((char*)iv, iv_init, 4);
    std::cout << "IV set to: ";
    for(uint32_t i = 0; i < 4; i++){
         std::cout << "0x" << std::hex << (int)iv[i] << " ";
    }
    std::cout << std::dec << std::endl;
    
    uint32_t ivSize = 4;
    
    // payloadData is NULL
    const uint8_t* payload = nullptr;
    uint32_t payloadSize = 3;
    
    uint8_t* opaqueData = nullptr;
    uint8_t** ppOpaqueData = &opaqueData;
    
    std::cout << "Invoking decrypt with NULL payloadData pointer." << std::endl;
    int ret = adapter.decrypt(iv, ivSize, payload, payloadSize, ppOpaqueData);
    std::cout << "decrypt returned: " << ret << std::endl;
    
    // Expected: ret should be non-zero indicating failure due to invalid payloadData pointer
    EXPECT_NE(ret, 0);
    
    std::cout << "Exiting NullPayloadDataPointer test" << std::endl;
}
/**
 * @brief Verify that decrypt returns an error when payload size is zero
 *
 * This test verifies that the decrypt method in the OCDMBasicSessionAdapter class returns a non-zero error code when provided with a payload data size of zero. The test ensures proper handling of invalid payload sizes by invoking the method and asserting the expected failure.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 008@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                         | Test Data                                                                                                          | Expected Result                                         | Notes         |
 * | :--------------: | ------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------ | ------------------------------------------------------- | ------------- |
 * | 01               | Instantiate OCDMBasicSessionAdapter with nullptr parameters         | input1: adapter constructor arguments = (nullptr, nullptr)                                                         | Object created without throwing an exception            | Should be successful |
 * | 02               | Prepare Initialization Vector (IV) for decryption                   | input1: iv = "0x01,0x02,0x03,0x04", ivSize = 4                                                                      | IV array populated with values {0x01, 0x02, 0x03, 0x04}  | Should be successful |
 * | 03               | Prepare valid payload data but set payloadSize to zero               | input1: payload = "0xAA,0xBB,0xCC", payloadSize = 0                                                                  | Payload array is set but payloadSize is zero             | Should be successful |
 * | 04               | Invoke decrypt API with zero payloadSize and validate error response   | input1: iv pointer = (iv), ivSize = 4, input2: payload pointer = (payload), payloadSize = 0, output1: opaqueData pointer = (ppOpaqueData) | Return value is non-zero indicating error due to zero payloadSize | Should Fail   |
 */
TEST(OCDMBasicSessionAdapter, ZeroPayloadDataSize)
{
    std::cout << "Entering ZeroPayloadDataSize test" << std::endl;
    
    // Arrange
    DrmInfo drmInfo;  
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);  // Using derived class
    std::cout << "Created drmHelper of type WidevineDrmHelper at address: " << drmHelper.get() << std::endl;
        
    // Create a valid callback object on heap
    TestDrmCallbacks* drmCallbacks = new TestDrmCallbacks();
    std::cout << "Created drmCallbacks at address: " << drmCallbacks << std::endl;
    OCDMBasicSessionAdapter adapter(drmHelper, drmCallbacks);
    
    // Prepare IV: {0x01, 0x02, 0x03, 0x04}
    uint8_t iv[4] = {0};
    char iv_init[5] = "\x01\x02\x03\x04";
    strncpy((char*)iv, iv_init, 4);
    std::cout << "IV set to: ";
    for(uint32_t i = 0; i < 4; i++){
         std::cout << "0x" << std::hex << (int)iv[i] << " ";
    }
    std::cout << std::dec << std::endl;
    
    uint32_t ivSize = 4;
    
    // Prepare valid payloadData: {0xAA, 0xBB, 0xCC}
    uint8_t payload[3] = {0};
    char payload_init[4] = "\xAA\xBB\xCC";
    strncpy((char*)payload, payload_init, 3);
    std::cout << "Payload data set to: ";
    for(uint32_t i = 0; i < 3; i++){
         std::cout << "0x" << std::hex << (int)payload[i] << " ";
    }
    std::cout << std::dec << std::endl;
    
    // payloadDataSize is zero
    uint32_t payloadSize = 0;
    
    uint8_t* opaqueData = nullptr;
    uint8_t** ppOpaqueData = &opaqueData;
    
    std::cout << "Invoking decrypt with payloadDataSize zero." << std::endl;
    int ret = adapter.decrypt(iv, ivSize, payload, payloadSize, ppOpaqueData);
    std::cout << "decrypt returned: " << ret << std::endl;
    
    // Expected: ret should be non-zero due to payloadDataSize being zero
    EXPECT_NE(ret, 0);
    
    std::cout << "Exiting ZeroPayloadDataSize test" << std::endl;
}
/**
 * @brief Test the decrypt API with a null opaque output pointer to ensure proper error handling.
 *
 * This test case verifies that the decrypt method of OCDMBasicSessionAdapter returns a non-zero error code when
 * invoked with a NULL pointer for the opaque output data. It ensures that invalid parameters are correctly handled,
 * and that the API does not proceed with decryption when a necessary output pointer is missing.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 009@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Initialize OCDMBasicSessionAdapter with null opaque output, prepare IV and payload, and invoke decrypt with an invalid (NULL) opaque output pointer. | iv = {0x01, 0x02, 0x03, 0x04}, ivSize = 4, payload = {0xAA, 0xBB, 0xCC}, payloadSize = 3, ppOpaqueData = nullptr, adapter initialized with (nullptr, nullptr) | Return value from decrypt should be non-zero indicating error due to invalid output pointer. | Should Fail |
 */
TEST(OCDMBasicSessionAdapter, NullOpaqueOutputPointer)
{
    std::cout << "Entering NullOpaqueOutputPointer test" << std::endl;
    
    // Arrange
    DrmInfo drmInfo;  
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);  // Using derived class
    std::cout << "Created drmHelper of type WidevineDrmHelper at address: " << drmHelper.get() << std::endl;
        
    // Create a valid callback object on heap
    TestDrmCallbacks* drmCallbacks = new TestDrmCallbacks();
    std::cout << "Created drmCallbacks at address: " << drmCallbacks << std::endl;
    OCDMBasicSessionAdapter adapter(drmHelper, drmCallbacks);
    
    // Prepare IV: {0x01, 0x02, 0x03, 0x04}
    uint8_t iv[4] = {0};
    char iv_init[5] = "\x01\x02\x03\x04";
    strncpy((char*)iv, iv_init, 4);
    std::cout << "IV set to: ";
    for(uint32_t i = 0; i < 4; i++){
         std::cout << "0x" << std::hex << (int)iv[i] << " ";
    }
    std::cout << std::dec << std::endl;
    
    uint32_t ivSize = 4;
    
    // Prepare payloadData: {0xAA, 0xBB, 0xCC}
    uint8_t payload[3] = {0};
    char payload_init[4] = "\xAA\xBB\xCC";
    strncpy((char*)payload, payload_init, 3);
    std::cout << "Payload data set to: ";
    for(uint32_t i = 0; i < 3; i++){
         std::cout << "0x" << std::hex << (int)payload[i] << " ";
    }
    std::cout << std::dec << std::endl;
    
    uint32_t payloadSize = 3;
    
    // ppOpaqueData is NULL
    uint8_t** ppOpaqueData = nullptr;
    
    std::cout << "Invoking decrypt with NULL output pointer for opaque data." << std::endl;
    int ret = adapter.decrypt(iv, ivSize, payload, payloadSize, ppOpaqueData);
    std::cout << "decrypt returned: " << ret << std::endl;
    
    // Expected: ret should be non-zero due to invalid output pointer
    EXPECT_NE(ret, 0);
    
    std::cout << "Exiting NullOpaqueOutputPointer test" << std::endl;
}
/**
 * @brief Validate decrypt API with large IV and payload data
 *
 * This test verifies that the decrypt API correctly processes a 16-byte IV and a 1024-byte payload without throwing exceptions. It ensures that the API can handle large input sizes and returns the expected status code.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 010@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * |01| Instantiate OCDMBasicSessionAdapter with null parameters and verify no exception is thrown. | Input: adapter = OCDMBasicSessionAdapter(nullptr, nullptr) | API constructor does not throw an exception. | Should Pass |
 * |02| Prepare a large IV of 16 bytes and print the IV. | Input: iv = "ABCDEFGHIJKLMNOP", ivSize = 16 | IV is correctly set and printed. | Should be successful |
 * |03| Create a large payload of 1024 bytes filled with repeating 'A' characters and print confirmation. | Input: payloadData = 1024 bytes of 'A', payloadSize = 1024 | Payload data is correctly prepared and confirmed with print output. | Should be successful |
 * |04| Call the decrypt API using the prepared IV and payload, and store the result in opaqueData. | Input: iv, ivSize, payload, payloadSize, ppOpaqueData | Returns 0 from decrypt API and prints decrypted opaque data if available. | Should Pass |
 * |05| Free the allocated payload memory. | Input: delete[] payload | Memory is freed without error. | Should be successful |
 */
TEST(OCDMBasicSessionAdapter, LargeIVAndPayloadData)
{
    std::cout << "Entering LargeIVAndPayloadData test" << std::endl;
    
    // Arrange
    DrmInfo drmInfo;  
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);  // Using derived class
    std::cout << "Created drmHelper of type WidevineDrmHelper at address: " << drmHelper.get() << std::endl;
        
    // Create a valid callback object on heap
    TestDrmCallbacks* drmCallbacks = new TestDrmCallbacks();
    std::cout << "Created drmCallbacks at address: " << drmCallbacks << std::endl;
    OCDMBasicSessionAdapter adapter(drmHelper, drmCallbacks);

    // Prepare large IV of 16 bytes
    uint8_t iv[16] = {0};
    char iv_init[17] = "ABCDEFGHIJKLMNOP";  // Example 16 characters
    // Use strncpy to copy 16 bytes into iv (note: treating iv as char*)
    strncpy((char*)iv, iv_init, 16);
    std::cout << "Large IV set to: ";
    for(uint32_t i = 0; i < 16; i++){
         std::cout << (char)iv[i];
    }
    std::cout << std::endl;
    
    uint32_t ivSize = 16;
    
    // Prepare large payloadData of 1024 bytes
    uint8_t* payload = new uint8_t[1024];
    char pattern[1025];
    // Fill pattern with a repeating pattern (for example, 'A')
    memset(pattern, 'A', 1024);
    pattern[1024] = '\0';
    strncpy((char*)payload, pattern, 1024);
    std::cout << "Large payload data prepared of size 1024 bytes." << std::endl;
    
    uint32_t payloadSize = 1024;
    
    uint8_t* opaqueData = nullptr;
    uint8_t** ppOpaqueData = &opaqueData;
    
    std::cout << "Invoking decrypt with large IV and payload data." << std::endl;
    int ret = adapter.decrypt(iv, ivSize, payload, payloadSize, ppOpaqueData);
    std::cout << "decrypt returned: " << ret << std::endl;
    EXPECT_EQ(ret, 0);
    
    if (opaqueData != nullptr) {
         std::cout << "Decrypted opaque data key info (first 16 bytes): ";
         for(uint32_t i = 0; i < 16 && i < payloadSize; i++){
             std::cout << "0x" << std::hex << (int)opaqueData[i] << " ";
         }
         std::cout << std::dec << std::endl;
    } else {
         std::cout << "No opaque data returned." << std::endl;
    }
    
    delete[] payload;
    
    std::cout << "Exiting LargeIVAndPayloadData test" << std::endl;
}
/**
 * @brief ValidDestruction test to verify that OCDMBasicSessionAdapter is destroyed without exceptions.
 *
 * This test validates that an OCDMBasicSessionAdapter instance, created with valid WidevineDrmHelper and TestDrmCallbacks, is destructed properly when it goes out of scope, thereby ensuring that the destructor does not throw any exceptions. The test also ensures that manual cleanup (deletion) of drmCallbacks is handled correctly.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 011@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Arrange valid objects for safe construction. | drmInfo = default, drmHelper = valid shared_ptr, drmCallbacks = pointer to TestDrmCallbacks | Objects are created successfully. | Should be successful |
 * | 02 | Create OCDMBasicSessionAdapter instance within inner scope using valid drmHelper and drmCallbacks. | Input: drmHelper = valid, drmCallbacks = valid | Adapter instance is created without throwing exception. | Should Pass |
 * | 03 | Trigger destruction by exiting inner scope and perform manual cleanup of drmCallbacks. | No additional input arguments | Destructor is called without exception and memory cleanup is successful. | Should be successful |
 */
TEST(OCDMBasicSessionAdapter, ValidDestruction)
{
    std::cout << "Entering ValidDestruction test" << std::endl;

    EXPECT_NO_THROW({
        // Arrange – Create valid objects for a safe construction
        DrmInfo drmInfo;
        auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);
        TestDrmCallbacks* drmCallbacks = new TestDrmCallbacks();

        {
            std::cout << "Creating OCDMBasicSessionAdapter with valid drmHelper and drmCallbacks" << std::endl;
            OCDMBasicSessionAdapter adapter(drmHelper, drmCallbacks);
            std::cout << "OCDMBasicSessionAdapter instance created successfully." << std::endl;

            // Adapter will be destroyed at the end of this scope
            std::cout << "Leaving inner scope to trigger OCDMBasicSessionAdapter destructor" << std::endl;
        }

        delete drmCallbacks; // manual cleanup
    });

    std::cout << "OCDMBasicSessionAdapter destructed successfully without exception" << std::endl;
    std::cout << "Exiting ValidDestruction test" << std::endl;
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
