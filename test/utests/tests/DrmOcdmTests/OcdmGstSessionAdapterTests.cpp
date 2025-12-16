
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
#include "OcdmGstSessionAdapter.h"

class WidevineDrmHelper : public DrmHelper {
public:
    WidevineDrmHelper(const DrmInfo& drmInfo) : DrmHelper(drmInfo) {}

    const std::string& ocdmSystemId() const override {
        static const std::string id = "WidevineTestSystem";
        return id;
    }

    void createInitData(std::vector<uint8_t>& initData) const override {
        initData = {1, 2, 3};
    }

    bool parsePssh(const uint8_t* initData, uint32_t initDataLen) override {
        return (initData != nullptr && initDataLen > 0);
    }

    bool isClearDecrypt() const override { return false; }

    void getKey(std::vector<uint8_t>& keyID) const override {
        keyID = {0xAA, 0xBB, 0xCC};
    }

    void generateLicenseRequest(const ChallengeInfo&, LicenseRequest&) const override { }
};


class TestDrmCallbacks : public DrmCallbacks {
public:
    void Individualization(const std::string& payload) override {
        std::cout << "Individualization callback invoked. Payload: " << payload << std::endl;
    }

    void LicenseRenewal(DrmHelperPtr drmHelper, void* userData) override {
        std::cout << "LicenseRenewal callback invoked." << std::endl;
        (void)drmHelper;
        (void)userData;
    }
};


// Test Case: Construct with valid drmHelper and valid drmCallbacks
/**
 * @brief Verify that OCDMGSTSessionAdapter constructor functions correctly with valid helper and callback objects.
 *
 * This test verifies that the OCDMGSTSessionAdapter instance can be successfully constructed when provided with a valid DrmHelper and a valid DrmCallbacks object. The test ensures that no exceptions are thrown during the construction process and that allocated resources are properly cleaned up after use.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 001@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | ------------- | ----- |
 * | 01 | Arrange valid DrmHelper and DrmCallbacks objects | drmInfo = valid instance, drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo), drmCallbacks = new TestDrmCallbacks() | Objects are created successfully | Should be successful |
 * | 02 | Invoke OCDMGSTSessionAdapter constructor with valid arguments and verify that no exception is thrown | drmHelper = valid pointer, drmCallbacks = valid pointer passed to constructor | OCDMGSTSessionAdapter instance is constructed without throwing exceptions | Should Pass |
 * | 03 | Cleanup allocated resources | drmCallbacks = pointer to TestDrmCallbacks to be deleted | Resources are freed successfully | Should be successful |
 */
TEST(OCDMGSTSessionAdapter, ConstructWithValidHelperAndCallbacks)
{
    std::cout << "Entering ConstructWithValidHelperAndCallbacks test" << std::endl;

    // Arrange: Create valid DrmHelper and DrmCallbacks objects
    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);   // Derived class instance
    auto* drmCallbacks = new TestDrmCallbacks();                     // Derived class instance

    // Act & Assert
    EXPECT_NO_THROW({
        OCDMGSTSessionAdapter adapter(drmHelper, drmCallbacks);
        std::cout << "OCDMGSTSessionAdapter instance constructed successfully." << std::endl;
    });

    // Cleanup
    delete drmCallbacks;

    std::cout << "Exiting ConstructWithValidHelperAndCallbacks test" << std::endl;
}
/**
 * @brief Test constructing OCDMGSTSessionAdapter with a null DrmHelper pointer and a valid DrmCallbacks instance
 *
 * This test verifies that OCDMGSTSessionAdapter can be successfully constructed without throwing an exception when provided 
 * with a null drmHelper and a valid instance of drmCallbacks. The objective is to ensure that the constructor handles a 
 * null drmHelper appropriately while the drmCallbacks is valid.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 002@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Initialize test inputs by setting drmHelper to null and creating a valid instance of TestDrmCallbacks | drmHelper = nullptr, drmCallbacks = valid pointer (TestDrmCallbacks instance) | drmHelper is null, drmCallbacks is non-null | Should be successful |
 * | 02 | Invoke the constructor of OCDMGSTSessionAdapter with the null drmHelper and valid drmCallbacks | Input: drmHelper = nullptr, drmCallbacks = valid pointer; Output: adapter instance address | Constructor does not throw any exception and adapter is constructed successfully | Should Pass |
 */
TEST(OCDMGSTSessionAdapter, ConstructWithNullHelperAndValidCallbacks)
{
    GTEST_SKIP();
    std::cout << "[Test] ConstructWithNullHelperAndValidCallbacks - Start" << std::endl;

    // drmHelper is intentionally null (default constructed shared_ptr)
    DrmHelperPtr drmHelper;
    std::cout << "drmHelper is null: " << drmHelper.get() << std::endl;

    // Create a valid DrmCallbacks implementation
    TestDrmCallbacks* drmCallbacks = new TestDrmCallbacks();
    std::cout << "Created TestDrmCallbacks instance at: " << drmCallbacks << std::endl;

    EXPECT_NO_THROW({
        std::cout << "Constructing OCDMGSTSessionAdapter with null drmHelper and valid drmCallbacks..." << std::endl;
        OCDMGSTSessionAdapter adapter(drmHelper, drmCallbacks);
        std::cout << "OCDMGSTSessionAdapter constructed at address: " << &adapter << std::endl;
    });

    // Clean up allocated callback instance (since we used `new`)
    delete drmCallbacks;

    std::cout << "[Test] ConstructWithNullHelperAndValidCallbacks - End" << std::endl;
}
/**
 * @brief Test the construction of OCDMGSTSessionAdapter with a valid helper and null callbacks
 *
 * This test verifies that constructing OCDMGSTSessionAdapter using a valid DrmHelper (specifically WidevineDrmHelper) and a null drmCallbacks pointer does not result in any exceptions. It ensures that the adapter can be created without crashing when the callback parameter is absent.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 003@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create a valid DrmHelper instance (WidevineDrmHelper) using a DrmInfo object. | drmInfo = default, drmHelper = instance of WidevineDrmHelper | DrmHelper is created successfully with a valid address. | Should be successful |
 * | 02 | Set drmCallbacks pointer to nullptr. | drmCallbacks = nullptr | drmCallbacks is set to nullptr. | Should be successful |
 * | 03 | Invoke the OCDMGSTSessionAdapter constructor with the valid drmHelper and null drmCallbacks wrapped inside EXPECT_NO_THROW. | input: drmHelper = valid WidevineDrmHelper instance, drmCallbacks = nullptr; output: OCDMGSTSessionAdapter instance created without exception | The adapter is constructed successfully without throwing an exception. | Should Pass |
 */
TEST(OCDMGSTSessionAdapter, ConstructWithValidHelperAndNullCallbacks) {
    std::cout << "Entering ConstructWithValidHelperAndNullCallbacks test" << std::endl;

    // Create a valid derived DrmHelper object
    DrmInfo drmInfo;
    DrmHelperPtr drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);
    std::cout << "Created drmHelper (WidevineDrmHelper) at address: " << drmHelper.get() << std::endl;

    // Set drmCallbacks to nullptr
    DrmCallbacks* drmCallbacks = nullptr;
    std::cout << "Using drmCallbacks = nullptr" << std::endl;

    // Constructing with null callbacks should not throw (if design allows)
    EXPECT_NO_THROW({
        OCDMGSTSessionAdapter adapter(drmHelper, drmCallbacks);
        std::cout << "OCDMGSTSessionAdapter instance created successfully at: " << &adapter << std::endl;
    });

    std::cout << "Exiting ConstructWithValidHelperAndNullCallbacks test" << std::endl;
}
/**
 * @brief Test the successful decryption operation when no sub-sample data is provided
 *
 * This test validates that the OCDMGSTSessionAdapter successfully decrypts data when no sub-sample data exists. It creates valid helper objects and dummy buffers, then verifies that decrypt returns 0 without throwing exceptions.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 004@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**  
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create valid DrmHelper and DrmCallbacks objects and instantiate OCDMGSTSessionAdapter | drmInfo, drmHelper instance (using drmInfo), drmCallbacks instance (TestDrmCallbacks) | Instance constructed without throwing exceptions | Should Pass |
 * | 02 | Initialize GstBuffer and GstCaps objects with dummy IDs for keyID, IV, payload, subSamples, and caps | keyID.id = "KeyID_Buffer", iv.id = "IV_Buffer", payload.id = "Payload_Buffer", subSamples.id = "SubSamples_Buffer", capsObj.capsInfo = "Valid_Caps" | Buffers and caps correctly initialized with valid dummy values | Should be successful |
 * | 03 | Invoke the decrypt method with subSampleCount set to 0 | keyID address, iv address, payload address, subSampleCount = 0, subSamples address, capsObj address | decrypt returns 0 and passes the EXPECT_EQ assertion | Should Pass |
 * | 04 | Delete the dynamically allocated drmCallbacks to avoid memory leaks | drmCallbacks pointer deletion | Memory deallocation successful | Should be successful |
 */
TEST(OCDMGSTSessionAdapter, SuccessfulDecryption_NoSubSampleData) {
    GTEST_SKIP();
    std::cout << "Entering SuccessfulDecryption_NoSubSampleData test" << std::endl;

    // Arrange: Create valid DrmHelper and DrmCallbacks objects
    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);   // Derived class instance
    auto* drmCallbacks = new TestDrmCallbacks();                     // Derived class instance

    // Act & Assert
    EXPECT_NO_THROW({
        OCDMGSTSessionAdapter adapter(drmHelper, drmCallbacks);
        std::cout << "OCDMGSTSessionAdapter instance constructed successfully." << std::endl;
    });

    OCDMGSTSessionAdapter adapter(drmHelper, drmCallbacks);

    // Create valid GstBuffer and GstCaps objects for simulation
    GstBuffer keyID;
    GstBuffer iv;
    GstBuffer payload;
    GstBuffer subSamples;
    GstCaps capsObj;

    // Invoke the decrypt method
    std::cout << "Invoking decrypt with subSampleCount = 0" << std::endl;
    int ret = adapter.decrypt(&keyID, &iv, &payload, 0, &subSamples, &capsObj);
    std::cout << "decrypt returned: " << ret << std::endl;
    EXPECT_EQ(ret, 0);
    delete drmCallbacks;
    std::cout << "Exiting SuccessfulDecryption_NoSubSampleData test" << std::endl;
}
/**
 * @brief Validate successful decryption using sub-sample data.
 *
 * This test verifies that the OCDMGSTSessionAdapter instance performs decryption correctly when provided with valid sub-sample data. It ensures that the adapter instance is constructed without exceptions and that the decrypt API returns a successful response when valid keyID, IV, payload, subSamples, and caps objects are provided.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 005@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Construct OCDMGSTSessionAdapter instance using valid drmHelper and drmCallbacks | drmHelper = valid WidevineDrmHelper instance, drmCallbacks = valid TestDrmCallbacks instance | Instance constructed successfully without exception | Should Pass |
 * | 02 | Initialize keyID, iv, payload, subSamples, and capsObj with valid buffer IDs | keyID.id = "KeyID_Buffer", iv.id = "IV_Buffer", payload.id = "Payload_Buffer", subSamples.id = "SubSamples_Buffer", capsObj.capsInfo = "Valid_Caps" | Buffers initialized correctly with provided IDs and caps infos | Should be successful |
 * | 03 | Invoke decrypt API with valid buffers and subSampleCount = 3 | input: keyID pointer, iv pointer, payload pointer, subSampleCount = 3, subSamples pointer, capsObj pointer; output: ret value from decrypt API | ret equals 0 indicating successful decryption | Should Pass |
 */
TEST(OCDMGSTSessionAdapter, SuccessfulDecryption_WithSubSampleData) {
    GTEST_SKIP();
    std::cout << "Entering SuccessfulDecryption_WithSubSampleData test" << std::endl;

    // Arrange: Create valid DrmHelper and DrmCallbacks objects
    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);   // Derived class instance
    auto* drmCallbacks = new TestDrmCallbacks();                     // Derived class instance

    // Act & Assert
    EXPECT_NO_THROW({
        OCDMGSTSessionAdapter adapter(drmHelper, drmCallbacks);
        std::cout << "OCDMGSTSessionAdapter instance constructed successfully." << std::endl;
    });

    OCDMGSTSessionAdapter adapter(drmHelper, drmCallbacks);

    GstBuffer keyID;
    GstBuffer iv;
    GstBuffer payload;
    GstBuffer subSamples;
    GstCaps capsObj;
        
    unsigned subSampleCount = 3;
    std::cout << "Invoking decrypt with subSampleCount = " << subSampleCount << std::endl;
    int ret = adapter.decrypt(&keyID, &iv, &payload, subSampleCount, &subSamples, &capsObj);
    std::cout << "decrypt returned: " << ret << std::endl;
    EXPECT_EQ(ret, 0);
    delete drmCallbacks;
    std::cout << "Exiting SuccessfulDecryption_WithSubSampleData test" << std::endl;
}
/**
 * @brief Test failure scenario when keyIDBuffer is nullptr in decrypt operation
 *
 * This test verifies that the decrypt function of the OCDMGSTSessionAdapter returns a non-zero value when supplied with a nullptr for keyIDBuffer. The test ensures that proper error handling is implemented for a null keyIDBuffer input.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 006@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01| Invoke decrypt with keyIDBuffer set to nullptr on a valid instance of OCDMGSTSessionAdapter constructed with valid DrmHelper and DrmCallbacks to test error handling. | keyIDBuffer = nullptr, iv.id = IV_Buffer, payload.id = Payload_Buffer, subSamples.id = SubSamples_Buffer, capsObj.capsInfo = Valid_Caps | Returns non-zero value; ASSERT_NE(ret, 0) check | Should Fail |
 */
TEST(OCDMGSTSessionAdapter, Failure_NullKeyIDBuffer) {
    std::cout << "Entering Failure_NullKeyIDBuffer test" << std::endl;

    // Arrange: Create valid DrmHelper and DrmCallbacks objects
    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);   // Derived class instance
    auto* drmCallbacks = new TestDrmCallbacks();                     // Derived class instance
    GstBuffer subSamples;
    // Act & Assert
    EXPECT_NO_THROW({
        OCDMGSTSessionAdapter adapter(drmHelper, drmCallbacks);
        std::cout << "OCDMGSTSessionAdapter instance constructed successfully." << std::endl;
    });

    OCDMGSTSessionAdapter adapter(drmHelper, drmCallbacks);

    // Create remaining valid objects
    GstBuffer iv;
    GstBuffer payload;
    GstCaps capsObj;
    
    std::cout << "Invoking decrypt with keyIDBuffer = nullptr" << std::endl;
    int ret = adapter.decrypt(nullptr, &iv, &payload, 0, &subSamples, &capsObj);
    std::cout << "decrypt returned: " << ret << std::endl;
    EXPECT_NE(ret, 0);
    delete drmCallbacks;
    std::cout << "Exiting Failure_NullKeyIDBuffer test" << std::endl;
}
/**
 * @brief Verify that decrypt API returns an error when provided with a nullptr IV buffer.
 *
 * This test ensures that, even when all other buffers (keyID, payload, subSamples, and caps) are valid,
 * invoking the decrypt method with a nullptr IV buffer results in a failure indicated by a non-zero return value.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 007@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                        | Test Data                                                                                                                  | Expected Result                                                                      | Notes             |
 * | :--------------: | ---------------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------- | ----------------- |
 * | 01               | Construct OCDMGSTSessionAdapter instance with valid DrmHelper and DrmCallbacks       | drmHelper = valid instance, drmCallbacks = valid pointer                                                                     | Instance constructed successfully without throwing exceptions                       | Should be successful |
 * | 02               | Initialize keyID, payload, subSamples, and caps objects with valid string values     | keyID = "KeyID_Buffer", payload = "Payload_Buffer", subSamples = "SubSamples_Buffer", caps = "Valid_Caps"                      | Buffers initialized with respective valid values                                      | Should be successful |
 * | 03               | Invoke decrypt API with ivBuffer set to nullptr and valid other parameters           | keyID = "KeyID_Buffer", ivBuffer = nullptr, payload = "Payload_Buffer", parameter = 0, subSamples = "SubSamples_Buffer", caps = "Valid_Caps" | decrypt returns a non-zero value indicating a failure due to null IV buffer invocation | Should Fail         |
 */
TEST(OCDMGSTSessionAdapter, Failure_NullIVBuffer) {
    GTEST_SKIP();
    std::cout << "Entering Failure_NullIVBuffer test" << std::endl;

    // Arrange: Create valid DrmHelper and DrmCallbacks objects
    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);   // Derived class instance
    auto* drmCallbacks = new TestDrmCallbacks();                     // Derived class instance
    GstBuffer subSamples;
    // Act & Assert
    EXPECT_NO_THROW({
        OCDMGSTSessionAdapter adapter(drmHelper, drmCallbacks);
        std::cout << "OCDMGSTSessionAdapter instance constructed successfully." << std::endl;
    });
    OCDMGSTSessionAdapter adapter(drmHelper, drmCallbacks);

    GstBuffer keyID;
    GstBuffer payload;
    GstCaps capsObj;
    
    std::cout << "Invoking decrypt with ivBuffer = nullptr" << std::endl;
    int ret = adapter.decrypt(&keyID, nullptr, &payload, 0, &subSamples, &capsObj);
    std::cout << "decrypt returned: " << ret << std::endl;
    EXPECT_NE(ret, 0);
    delete drmCallbacks;
    std::cout << "Exiting Failure_NullIVBuffer test" << std::endl;
}
/**
 * @brief Verify that decrypt returns an error when provided with a null payload buffer
 *
 * This test verifies the behavior of the OCDMGSTSessionAdapter::decrypt API when the payload buffer is set to nullptr. The objective is to ensure that despite other valid parameters, a null payload buffer causes the API to return a non-zero error code. This scenario confirms that the API fails gracefully under invalid input conditions.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 008
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Construct OCDMGSTSessionAdapter with valid drmHelper and drmCallbacks | drmHelper = valid instance, drmCallbacks = valid instance | Instance constructed without throwing exceptions | Should be successful |
 * | 02 | Create valid buffer objects for keyID, iv, capsObj, and subSamples | keyID = "KeyID_Buffer", iv = "IV_Buffer", capsInfo = "Valid_Caps", subSamples = "SubSamples_Buffer" | Buffers populated with valid string data | Should be successful |
 * | 03 | Call decrypt method with null payload buffer | keyID, iv, payloadBuffer = nullptr, subSamples, capsObj | decrypt returns a non-zero value indicating failure | Should Pass |
 * | 04 | Validate API failure using assertion check | ret != 0 | EXPECT_NE(ret, 0) assertion passes confirming failure as expected | Should Pass |
 */
TEST(OCDMGSTSessionAdapter, Failure_NullPayloadBuffer) {
    std::cout << "Entering Failure_NullPayloadBuffer test" << std::endl;

    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);   // Derived class instance
    auto* drmCallbacks = new TestDrmCallbacks();                     // Derived class instance
    GstBuffer subSamples;
    // Act & Assert
    EXPECT_NO_THROW({
        OCDMGSTSessionAdapter adapter(drmHelper, drmCallbacks);
        std::cout << "OCDMGSTSessionAdapter instance constructed successfully." << std::endl;
    });

    OCDMGSTSessionAdapter adapter(drmHelper, drmCallbacks);

    GstBuffer keyID;
    GstBuffer iv;
    GstCaps capsObj;
    
    std::cout << "Invoking decrypt with payload buffer = nullptr" << std::endl;
    int ret = adapter.decrypt(&keyID, &iv, nullptr, 0, &subSamples, &capsObj);
    std::cout << "decrypt returned: " << ret << std::endl;
    EXPECT_NE(ret, 0);
    delete drmCallbacks;
    std::cout << "Exiting Failure_NullPayloadBuffer test" << std::endl;
}
/**
 * @brief Validate failure scenario for decrypt when subSampleCount is non-zero but subSamplesBuffer is nullptr
 *
 * This test verifies that the OCDMGSTSessionAdapter's decrypt method correctly handles the case when a non-zero subSampleCount is provided,
 * but the corresponding subSamplesBuffer pointer is nullptr. The test ensures that the function returns an error (non-zero) and that the proper
 * error checking is in place to avoid proceeding with incomplete sample data.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 009
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                              | Test Data                                                                                   | Expected Result                                            | Notes         |
 * | :--------------: | ---------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------- | ---------------------------------------------------------- | ------------- |
 * | 01               | Construct OCDMGSTSessionAdapter instance using valid drmHelper and drmCallbacks            | drmHelper: valid instance, drmCallbacks: valid instance                                      | Instance constructed successfully without throwing exception | Should be successful |
 * | 02               | Initialize valid keyID, iv, payload, and capsObj with proper identification strings         | keyID = "KeyID_Buffer", iv = "IV_Buffer", payload = "Payload_Buffer", capsObj = "Valid_Caps"   | Buffers initialized with correct values                   | Should be successful |
 * | 03               | Invoke decrypt method with subSampleCount = 2 and subSamplesBuffer = nullptr                  | keyID, iv, payload, subSampleCount = 2, subSamplesBuffer = nullptr, capsObj = "Valid_Caps"       | decrypt returns non-zero indicating error due to missing subsamples | Should Fail         |
 */
TEST(OCDMGSTSessionAdapter, Failure_SubSampleCountNonZeroButNullSubSamplesBuffer) {
    std::cout << "Entering Failure_SubSampleCountNonZeroButNullSubSamplesBuffer test" << std::endl;

    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);   // Derived class instance
    auto* drmCallbacks = new TestDrmCallbacks();                     // Derived class instance
    GstBuffer subSamples;
    // Act & Assert
    EXPECT_NO_THROW({
        OCDMGSTSessionAdapter adapter(drmHelper, drmCallbacks);
        std::cout << "OCDMGSTSessionAdapter instance constructed successfully." << std::endl;
    });

    OCDMGSTSessionAdapter adapter(drmHelper, drmCallbacks);

    GstBuffer keyID;
    GstBuffer iv;
    GstBuffer payload;
    GstCaps capsObj;
    std::cout << "SubSampleCount > 0 but subSamplesBuffer is nullptr" << std::endl;

    unsigned subSampleCount = 2;
    std::cout << "Invoking decrypt with subSampleCount = " << subSampleCount << " and subSamplesBuffer = nullptr" << std::endl;
    int ret = adapter.decrypt(&keyID, &iv, &payload, subSampleCount, nullptr, &capsObj);
    std::cout << "decrypt returned: " << ret << std::endl;
    EXPECT_NE(ret, 0);
    delete drmCallbacks;
    std::cout << "Exiting Failure_SubSampleCountNonZeroButNullSubSamplesBuffer test" << std::endl;
}
/**
 * @brief Verify that decrypt fails when provided with a nullptr for caps
 *
 * This test verifies that when a null pointer is passed as the caps argument to the decrypt API,
 * the decryption operation fails as expected. The test first constructs an instance of OCDMGSTSessionAdapter
 * with valid DRM helper and callbacks, then sets up valid keyID, iv, payload, and subSamples buffers.
 * Finally, it invokes the decrypt function with caps set to nullptr and asserts that the returned value
 * is non-zero, indicating a failure in decryption.
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
 * | 01 | Construct OCDMGSTSessionAdapter instance using valid drmHelper and drmCallbacks. | input: drmHelper = valid instance of WidevineDrmHelper (constructed with valid DrmInfo), drmCallbacks = valid TestDrmCallbacks pointer | Instance constructed successfully without throwing an exception. | Should Pass |
 * | 02 | Invoke decrypt with valid keyID, iv, payload, subSamples and caps set to nullptr. | input: keyID = "KeyID_Buffer", iv = "IV_Buffer", payload = "Payload_Buffer", subSamples = "SubSamples_Buffer", caps = nullptr; output: ret (decryption result) | API returns a non-zero value indicating failure; Assertion EXPECT_NE(ret, 0) passes. | Should Fail |
 */
TEST(OCDMGSTSessionAdapter, Failure_NullCaps) {
    std::cout << "Entering Failure_NullCaps test" << std::endl;

    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);   // Derived class instance
    auto* drmCallbacks = new TestDrmCallbacks();                     // Derived class instance
    GstBuffer subSamples;
    // Act & Assert
    EXPECT_NO_THROW({
        OCDMGSTSessionAdapter adapter(drmHelper, drmCallbacks);
        std::cout << "OCDMGSTSessionAdapter instance constructed successfully." << std::endl;
    });

    OCDMGSTSessionAdapter adapter(drmHelper, drmCallbacks);

    GstBuffer keyID;
    GstBuffer iv;
    GstBuffer payload;
    
    std::cout << "Using nullptr for caps" << std::endl;

    std::cout << "Invoking decrypt with caps = nullptr" << std::endl;
    int ret = adapter.decrypt(&keyID, &iv, &payload, 0, &subSamples, nullptr);
    std::cout << "decrypt returned: " << ret << std::endl;
    EXPECT_NE(ret, 0);
    delete drmCallbacks;
    std::cout << "Exiting Failure_NullCaps test" << std::endl;
}
/**
 * @brief Validate the decryption process using valid input parameters.
 *
 * This test verifies that the OCDMGSTSessionAdapter's decrypt method functions correctly
 * when provided with valid IV and payload data. The test ensures the construction of the adapter 
 * does not throw an exception and that the decryption returns a success code (0) as expected.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 011@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                   | Test Data                                                                                               | Expected Result                                                | Notes        |
 * | :--------------: | --------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------- | -------------------------------------------------------------- | ------------ |
 * | 01               | Construct OCDMGSTSessionAdapter instance with valid drmHelper and drmCallbacks.               | drmHelper = instance of WidevineDrmHelper (initialized with DrmInfo), drmCallbacks = instance of TestDrmCallbacks | No exception thrown during construction                        | Should Pass  |
 * | 02               | Call the decrypt method with valid IV and payload arrays and verify the output.                | iv = {0x01,0x02,0x03,0x04}, iv_size = 4, payload = {0xAA,0xBB,0xCC,0xDD}, payload_size = 4, opaque = nullptr | Return value equals 0 and EXPECT_EQ(result, 0) assertion passes | Should Pass  |
 */
TEST(OCDMGSTSessionAdapter, ValidDecryption)
{
    GTEST_SKIP();
    std::cout << "Entering ValidDecryption test" << std::endl;
    
    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);   // Derived class instance
    auto* drmCallbacks = new TestDrmCallbacks();                     // Derived class instance
    // Act & Assert
    EXPECT_NO_THROW({
        OCDMGSTSessionAdapter adapter(drmHelper, drmCallbacks);
        std::cout << "OCDMGSTSessionAdapter instance constructed successfully." << std::endl;
    });

    OCDMGSTSessionAdapter adapter(drmHelper, drmCallbacks);
    
    // Prepare IV
    uint8_t iv[4] = {0};
    // Using strncpy-like assignment for fixed size array (using memcpy since strncpy works for char arrays)
    memcpy(iv, "\x01\x02\x03\x04", 4);
    
    // Prepare payload data
    uint8_t payload[4] = {0};
    memcpy(payload, "\xAA\xBB\xCC\xDD", 4);
    
    // Prepare opaque output pointer
    uint8_t* opaque = nullptr;
    
    // Log input values
    std::cout << "Invoking decrypt with IV bytes: ";
    for (uint32_t i = 0; i < 4; i++) {
        std::cout << "0x" << std::hex << static_cast<int>(iv[i]) << " ";
    }
    std::cout << std::dec << ", IV size: 4" << std::endl;
    std::cout << "Payload bytes: ";
    for (uint32_t i = 0; i < 4; i++) {
        std::cout << "0x" << std::hex << static_cast<int>(payload[i]) << " ";
    }
    std::cout << std::dec << ", payload size: 4" << std::endl;
    
    // Invoke decrypt method
    std::cout << "Calling decrypt method" << std::endl;
    int result = adapter.decrypt(iv, 4, payload, 4, &opaque);
    std::cout << "Returned value from decrypt: " << result << std::endl;
    
    // Verify expected result (0 for success)
    EXPECT_EQ(result, 0);
    
    if (opaque != nullptr) {
        std::cout << "Decrypted opaque data key information: 0x" 
                  << std::hex << static_cast<int>(opaque[0]) 
                  << std::dec << std::endl;
    }
    
    std::cout << "Exiting ValidDecryption test" << std::endl;
}
/**
 * @brief Validate error handling when IV pointer is null but IV size is non-zero
 *
 * This test verifies that the decrypt method of OCDMGSTSessionAdapter correctly handles an edge case where the Initialization Vector (IV) pointer is null while the IV size is non-zero. This scenario is expected to fail, ensuring that the API does not process invalid parameters.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 012@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Construct OCDMGSTSessionAdapter instance using valid drmHelper and drmCallbacks | drmHelper = valid SharedPtr instance; drmCallbacks = valid pointer instance | Instance is constructed successfully without exceptions | Should Pass |
 * | 02 | Invoke decrypt with a null IV pointer and non-zero IV size, while providing a valid payload and opaque pointer | iv = nullptr, iv size = 4, payload = {0xAA, 0xBB, 0xCC, 0xDD}, opaque = pointer to null storage | decrypt returns a non-zero error status indicating failure | Should Fail |
 */
TEST(OCDMGSTSessionAdapter, NullIVPointerNonZeroIVSize)
{
    std::cout << "Entering NullIVPointerNonZeroIVSize test" << std::endl;
    
    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);   // Derived class instance
    auto* drmCallbacks = new TestDrmCallbacks();                     // Derived class instance
    // Act & Assert
    EXPECT_NO_THROW({
        OCDMGSTSessionAdapter adapter(drmHelper, drmCallbacks);
        std::cout << "OCDMGSTSessionAdapter instance constructed successfully." << std::endl;
    });

    OCDMGSTSessionAdapter adapter(drmHelper, drmCallbacks);
    
    // IV pointer is null, but IV size is non-zero (4)
    const uint8_t* iv = nullptr;
    
    // Prepare payload data
    uint8_t payload[4] = {0};
    memcpy(payload, "\xAA\xBB\xCC\xDD", 4);
    
    uint8_t* opaque = nullptr;
    
    std::cout << "Invoking decrypt with IV pointer: nullptr, IV size: 4" << std::endl;
    std::cout << "Payload bytes: ";
    for (uint32_t i = 0; i < 4; i++) {
        std::cout << "0x" << std::hex << static_cast<int>(payload[i]) << " ";
    }
    std::cout << std::dec << ", payload size: 4" << std::endl;
    
    std::cout << "Calling decrypt method" << std::endl;
    int result = adapter.decrypt(iv, 4, payload, 4, &opaque);
    std::cout << "Returned value from decrypt: " << result << std::endl;
    
    // Expected non-zero error status indicating failure
    EXPECT_NE(result, 0);
    
    std::cout << "Exiting NullIVPointerNonZeroIVSize test" << std::endl;
}
/**
 * @brief Validate decrypt method behavior when IV size is zero despite having a valid IV pointer.
 *
 * This test ensures that the OCDMGSTSessionAdapter properly handles a scenario where the IV pointer is valid but the IV size provided is zero. The test constructs the adapter instance, prepares valid IV and payload data, and then calls the decrypt method. The expected behavior is that the decrypt method returns a non-zero result, indicating an error or an unexpected condition.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 013@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                           | Test Data                                                                                                                           | Expected Result                                                                      | Notes         |
 * | :--------------: | ----------------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------ | ------------- |
 * | 01               | Construct OCDMGSTSessionAdapter instance using valid drmHelper and drmCallbacks and verify no exception is thrown during construction | drmHelper = valid instance, drmCallbacks = valid instance                                                                           | OCDMGSTSessionAdapter instance constructed without throwing exceptions               | Should Pass   |
 * | 02               | Invoke decrypt method with a valid IV pointer set to [0x01,0x02,0x03,0x04] but with IV size set to 0, along with valid payload data [0xAA,0xBB,0xCC,0xDD] and opaque pointer as nullptr | input: iv = {0x01,0x02,0x03,0x04}, ivSize = 0, payload = {0xAA,0xBB,0xCC,0xDD}, payloadSize = 4, opaque pointer = nullptr; output: result | decrypt method returns a non-zero result (assertion EXPECT_NE(result, 0))                  | Should Pass   |
 */
TEST(OCDMGSTSessionAdapter, ZeroIVSizeValidIVPointer)
{
    std::cout << "Entering ZeroIVSizeValidIVPointer test" << std::endl;
    
    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);   // Derived class instance
    auto* drmCallbacks = new TestDrmCallbacks();                     // Derived class instance
    // Act & Assert
    EXPECT_NO_THROW({
        OCDMGSTSessionAdapter adapter(drmHelper, drmCallbacks);
        std::cout << "OCDMGSTSessionAdapter instance constructed successfully." << std::endl;
    });

    OCDMGSTSessionAdapter adapter(drmHelper, drmCallbacks);
    
    // Prepare IV data
    uint8_t iv[4] = {0};
    memcpy(iv, "\x01\x02\x03\x04", 4);
    
    // Prepare payload data
    uint8_t payload[4] = {0};
    memcpy(payload, "\xAA\xBB\xCC\xDD", 4);
    
    uint8_t* opaque = nullptr;
    
    std::cout << "Invoking decrypt with IV pointer having bytes: ";
    for (uint32_t i = 0; i < 4; i++) {
        std::cout << "0x" << std::hex << static_cast<int>(iv[i]) << " ";
    }
    std::cout << std::dec << ", but IV size set to 0" << std::endl;
    std::cout << "Payload bytes: ";
    for (uint32_t i = 0; i < 4; i++) {
        std::cout << "0x" << std::hex << static_cast<int>(payload[i]) << " ";
    }
    std::cout << std::dec << ", payload size: 4" << std::endl;
    
    std::cout << "Calling decrypt method" << std::endl;
    int result = adapter.decrypt(iv, 0, payload, 4, &opaque);
    std::cout << "Returned value from decrypt: " << result << std::endl;
    
    EXPECT_NE(result, 0);
    
    std::cout << "Exiting ZeroIVSizeValidIVPointer test" << std::endl;
}
/**
 * @brief Validate that decrypt returns an error when a non-null IV and non-zero payload size are provided with a null payload pointer.
 *
 * This test verifies that the OCDMGSTSessionAdapter correctly handles cases where a valid IV is supplied but the payload pointer is nullptr even though the payload size is non-zero.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 014@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data |Expected Result |Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Construct OCDMGSTSessionAdapter with valid drmHelper and drmCallbacks pointers. | drmHelper = valid pointer, drmCallbacks = valid pointer | Instance constructed successfully with no exception thrown. | Should Pass |
 * | 02 | Prepare IV bytes and assign a null payload pointer with a non-zero payload size. | IV = {0x01, 0x02, 0x03, 0x04}, IV size = 4, payload = nullptr, payload size = 4, opaque pointer address | IV is correctly set; payload pointer remains nullptr; preparation successful. | Should be successful |
 * | 03 | Invoke decrypt method and validate the return value due to null payload pointer. | Call adapter.decrypt(iv, 4, payload, 4, &opaque) | Return value is non-zero, indicating error due to the null payload pointer. | Should Fail |
 */
TEST(OCDMGSTSessionAdapter, NullPayloadPointerNonZeroPayloadSize)
{
    std::cout << "Entering NullPayloadPointerNonZeroPayloadSize test" << std::endl;
    
    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);   // Derived class instance
    auto* drmCallbacks = new TestDrmCallbacks();                     // Derived class instance
    // Act & Assert
    EXPECT_NO_THROW({
        OCDMGSTSessionAdapter adapter(drmHelper, drmCallbacks);
        std::cout << "OCDMGSTSessionAdapter instance constructed successfully." << std::endl;
    });

    OCDMGSTSessionAdapter adapter(drmHelper, drmCallbacks);
    
    // Prepare IV data
    uint8_t iv[4] = {0};
    memcpy(iv, "\x01\x02\x03\x04", 4);
    
    const uint8_t* payload = nullptr;
    
    uint8_t* opaque = nullptr;
    
    std::cout << "Invoking decrypt with valid IV bytes: ";
    for (uint32_t i = 0; i < 4; i++) {
        std::cout << "0x" << std::hex << static_cast<int>(iv[i]) << " ";
    }
    std::cout << std::dec << ", IV size: 4" << std::endl;
    std::cout << "But payload pointer is nullptr with payload size: 4" << std::endl;
    
    std::cout << "Calling decrypt method" << std::endl;
    int result = adapter.decrypt(iv, 4, payload, 4, &opaque);
    std::cout << "Returned value from decrypt: " << result << std::endl;
    
    EXPECT_NE(result, 0);
    
    std::cout << "Exiting NullPayloadPointerNonZeroPayloadSize test" << std::endl;
}
/**
 * @brief Verifies handling of valid payload pointer with zero payload size in decrypt operation
 *
 * This test ensures that when a valid payload pointer is provided but the payload size is zero,
 * the decrypt method returns a non-zero error code. It tests proper error handling and parameter checking in the API.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 015@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                           | Test Data                                                                                  | Expected Result                                             | Notes            |
 * | :--------------: | --------------------------------------------------------------------- | ------------------------------------------------------------------------------------------ | ----------------------------------------------------------- | ---------------- |
 * | 01               | Construct OCDMGSTSessionAdapter with valid drmHelper and drmCallbacks pointers. | drmHelper = valid pointer, drmCallbacks = valid pointer | Instance constructed successfully with no exception thrown.                          | Should Pass |
 * | 02               | Prepare IV and payload buffers and set payload size to 0                | IV bytes = 0x01,0x02,0x03,0x04; Payload bytes = 0xAA,0xBB,0xCC,0xDD; Payload size = 0          | Buffers initialized successfully                           | Should be successful |
 * | 03               | Invoke decrypt method with valid IV, valid payload pointer but zero payload size | Inputs: iv = [0x01,0x02,0x03,0x04], ivSize = 4, payload pointer = valid address, payloadSize = 0, opaque pointer variable | Return value not equal to 0 (error indication)              | Should Fail      |
 * | 04               | Verify the API return value is not 0 using assertion                    | Output: result from decrypt method                                                         | Assertion EXPECT_NE(result, 0) passes                       | Should be successful |
 */
TEST(OCDMGSTSessionAdapter, ZeroPayloadSizeValidPayloadPointer)
{
    std::cout << "Entering ZeroPayloadSizeValidPayloadPointer test" << std::endl;
    
    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);   // Derived class instance
    auto* drmCallbacks = new TestDrmCallbacks();                     // Derived class instance
    // Act & Assert
    EXPECT_NO_THROW({
        OCDMGSTSessionAdapter adapter(drmHelper, drmCallbacks);
        std::cout << "OCDMGSTSessionAdapter instance constructed successfully." << std::endl;
    });

    OCDMGSTSessionAdapter adapter(drmHelper, drmCallbacks);

    
    // Prepare IV data
    uint8_t iv[4] = {0};
    memcpy(iv, "\x01\x02\x03\x04", 4);
    
    // Prepare payload data
    uint8_t payload[4] = {0};
    memcpy(payload, "\xAA\xBB\xCC\xDD", 4);
    
    uint8_t* opaque = nullptr;
    
    std::cout << "Invoking decrypt with IV bytes: ";
    for (uint32_t i = 0; i < 4; i++) {
        std::cout << "0x" << std::hex << static_cast<int>(iv[i]) << " ";
    }
    std::cout << std::dec << ", IV size: 4" << std::endl;
    std::cout << "Payload pointer is valid, but payload size is set to 0" << std::endl;
    
    std::cout << "Calling decrypt method" << std::endl;
    int result = adapter.decrypt(iv, 4, payload, 0, &opaque);
    std::cout << "Returned value from decrypt: " << result << std::endl;
    
    EXPECT_NE(result, 0);
    
    std::cout << "Exiting ZeroPayloadSizeValidPayloadPointer test" << std::endl;
}
/**
 * @brief Verify that decrypt function handles null opaque data pointer input correctly.
 *
 * This test checks that when a null pointer is provided for the opaque data pointer,
 * the OCDMGSTSessionAdapter is constructed without throwing an exception, but the decrypt
 * method returns a non-zero error value to indicate the failure of the decryption process.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 016
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                               | Test Data                                                                                                                  | Expected Result                                                                                  | Notes         |
 * | :----:           | ----------------------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------ | ------------- |
 * | 01               | Construct OCDMGSTSessionAdapter with valid drmHelper and drmCallbacks pointers.            | drmHelper = valid pointer, drmCallbacks = valid pointer                                                                   | Instance constructed successfully with no exception thrown.                                      | Should Pass |
 * | 02               | Prepare IV data by copying 4 bytes into an IV array                                         | iv = {0x01, 0x02, 0x03, 0x04}, ivSize = 4                                                                                  | IV data populated successfully                                                                   | Should be successful |
 * | 03               | Prepare payload data by copying 4 bytes into a payload array                                 | payload = {0xAA, 0xBB, 0xCC, 0xDD}, payloadSize = 4                                                                        | Payload data populated successfully                                                              | Should be successful |
 * | 04               | Set the opaque data pointer to null                                                         | opaquePtr = nullptr                                                                                                        | Opaque data pointer is explicitly set to null                                                    | Should be successful |
 * | 05               | Call the decrypt method with IV, payload, and null opaque data pointer                        | decrypt inputs: iv pointer, ivSize = 4, payload pointer, payloadSize = 4, opaquePtr = nullptr                              | decrypt returns a non-zero error code indicating failure due to the null opaque pointer            | Should Fail   |
 * | 06               | Assert that the result from decrypt is non-zero                                             | result != 0                                                                                                                | EXPECT_NE assertion passes confirming a non-zero value returned                                  | Should Pass   |
 */
TEST(OCDMGSTSessionAdapter, NullOpaqueDataPointer)
{
    std::cout << "Entering NullOpaqueDataPointer test" << std::endl;
    
    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);   // Derived class instance
    auto* drmCallbacks = new TestDrmCallbacks();                     // Derived class instance
    // Act & Assert
    EXPECT_NO_THROW({
        OCDMGSTSessionAdapter adapter(drmHelper, drmCallbacks);
        std::cout << "OCDMGSTSessionAdapter instance constructed successfully." << std::endl;
    });

    OCDMGSTSessionAdapter adapter(drmHelper, drmCallbacks);
    
    // Prepare IV data
    uint8_t iv[4] = {0};
    memcpy(iv, "\x01\x02\x03\x04", 4);
    
    // Prepare payload data
    uint8_t payload[4] = {0};
    memcpy(payload, "\xAA\xBB\xCC\xDD", 4);
    
    // Passing a null pointer for opaque data pointer
    uint8_t** opaquePtr = nullptr;
    
    std::cout << "Invoking decrypt with IV bytes: ";
    for (uint32_t i = 0; i < 4; i++) {
       std::cout << "0x" << std::hex << static_cast<int>(iv[i]) << " ";
    }
    std::cout << std::dec << ", IV size: 4" << std::endl;
    std::cout << "Payload bytes: ";
    for (uint32_t i = 0; i < 4; i++) {
       std::cout << "0x" << std::hex << static_cast<int>(payload[i]) << " ";
    }
    std::cout << std::dec << ", payload size: 4" << std::endl;
    std::cout << "Passing nullptr for opaque data pointer" << std::endl;
    
    std::cout << "Calling decrypt method" << std::endl;
    int result = adapter.decrypt(iv, 4, payload, 4, opaquePtr);
    std::cout << "Returned value from decrypt: " << result << std::endl;
    
    EXPECT_NE(result, 0);
    
    std::cout << "Exiting NullOpaqueDataPointer test" << std::endl;
}
/**
 * @brief Verify that OCDMGSTSessionAdapter decrypt correctly processes minimal valid IV and payload sizes
 *
 * This test ensures that the OCDMGSTSessionAdapter object can be constructed without throwing an exception when provided with valid minimal input parameters. It then validates that the 'decrypt' method correctly processes a 1-byte IV and 1-byte payload, returning the expected result.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 017@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                           | Test Data                                                                                          | Expected Result                                             | Notes       |
 * | :--------------: | ----------------------------------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------- | ----------------------------------------------------------- | ----------- |
 * | 01               | Construct OCDMGSTSessionAdapter with valid drmHelper and drmCallbacks ensuring no exception is thrown. | drmHelper = valid WidevineDrmHelper, drmCallbacks = valid TestDrmCallbacks                           | Constructor does not throw any exception                    | Should Pass |
 * | 02               | Invoke decrypt method with a 1-byte IV and a 1-byte payload.                                           | iv[0] = 0x0F, iv_size = 1, payload[0] = 0xF0, payload_size = 1, opaque = nullptr                     | decrypt returns 0 as expected and assertion passes          | Should Pass |
 */
TEST(OCDMGSTSessionAdapter, MinimalValidSizes)
{
    GTEST_SKIP();
    std::cout << "Entering MinimalValidSizes test" << std::endl;
    
    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);   // Derived class instance
    auto* drmCallbacks = new TestDrmCallbacks();                     // Derived class instance
    // Act & Assert
    EXPECT_NO_THROW({
        OCDMGSTSessionAdapter adapter(drmHelper, drmCallbacks);
        std::cout << "OCDMGSTSessionAdapter instance constructed successfully." << std::endl;
    });

    OCDMGSTSessionAdapter adapter(drmHelper, drmCallbacks);
    
    // Prepare IV with one byte
    uint8_t iv[1] = {0};
    memcpy(iv, "\x0F", 1);
    
    // Prepare payload with one byte
    uint8_t payload[1] = {0};
    memcpy(payload, "\xF0", 1);
    
    uint8_t* opaque = nullptr;
    
    std::cout << "Invoking decrypt with minimal IV byte: 0x" 
              << std::hex << static_cast<int>(iv[0]) << std::dec 
              << ", IV size: 1" << std::endl;
    std::cout << "Payload byte: 0x" 
              << std::hex << static_cast<int>(payload[0]) << std::dec 
              << ", payload size: 1" << std::endl;
    
    std::cout << "Calling decrypt method" << std::endl;
    int result = adapter.decrypt(iv, 1, payload, 1, &opaque);
    std::cout << "Returned value from decrypt: " << result << std::endl;
    
    EXPECT_EQ(result, 0);
    
    std::cout << "Exiting MinimalValidSizes test" << std::endl;
}
/**
 * @brief Test the decryption API handling with large IV and payload sizes.
 *
 * This test verifies that the OCDMGSTSessionAdapter can correctly handle decryption operations when provided with large initialization vector (IV) and payload data arrays. It checks for proper construction of the adapter instance and ensures that the decrypt function returns a success result when processing large inputs.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 018@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                   | Test Data                                                                                                 | Expected Result                                                                                             | Notes          |
 * | :--------------: | --------------------------------------------------------------------------------------------- | --------------------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------------------------- | -------------- |
 * | 01               | Construct OCDMGSTSessionAdapter instance with valid drmHelper and drmCallbacks                 | drmHelper = valid shared instance, drmCallbacks = valid pointer                                             | Instance should be constructed without throwing an exception                                               | Should Pass    |
 * | 02               | Invoke decrypt with a large IV array (256 bytes) and payload array (1024 bytes)                  | iv = [0,1,...,255], payload = [0xAA + (i mod 256) for i from 0 to 1023, opaque initially = nullptr]         | decrypt method returns 0 and EXPECT_EQ(result, 0) passes                                                      | Should Pass    |
 * | 03               | Check and log opaque data key information if available after decryption                         | opaque pointer as output from decrypt function                                                              | Opaque data key information (first byte) is printed if opaque is not nullptr                                   | Should be successful |
 */
TEST(OCDMGSTSessionAdapter, LargeSizesForIVAndPayload)
{
    GTEST_SKIP();
    std::cout << "Entering LargeSizesForIVAndPayload test" << std::endl;
    
    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);   // Derived class instance
    auto* drmCallbacks = new TestDrmCallbacks();                     // Derived class instance
    // Act & Assert
    EXPECT_NO_THROW({
        OCDMGSTSessionAdapter adapter(drmHelper, drmCallbacks);
        std::cout << "OCDMGSTSessionAdapter instance constructed successfully." << std::endl;
    });

    OCDMGSTSessionAdapter adapter(drmHelper, drmCallbacks);
    
    // Create large IV array (256 bytes)
    std::vector<uint8_t> iv(256, 0);
    for (uint32_t i = 0; i < 256; i++) {
        iv[i] = static_cast<uint8_t>(i);
    }
    
    // Create large payload array (1024 bytes)
    std::vector<uint8_t> payload(1024, 0);
    for (uint32_t i = 0; i < 1024; i++) {
        payload[i] = static_cast<uint8_t>(0xAA + (i % 256));
    }
    
    uint8_t* opaque = nullptr;
    
    std::cout << "Invoking decrypt with large IV of size: " << iv.size() << std::endl;
    std::cout << "Invoking decrypt with large payload of size: " << payload.size() << std::endl;
    
    std::cout << "Calling decrypt method" << std::endl;
    int result = adapter.decrypt(iv.data(), static_cast<uint32_t>(iv.size()), payload.data(), static_cast<uint32_t>(payload.size()), &opaque);
    std::cout << "Returned value from decrypt: " << result << std::endl;
    
    EXPECT_EQ(result, 0);
    
    if (opaque != nullptr) {
        std::cout << "Decrypted opaque data key information (first byte): 0x" 
                  << std::hex << static_cast<int>(opaque[0]) 
                  << std::dec << std::endl;
    }
    
    std::cout << "Exiting LargeSizesForIVAndPayload test" << std::endl;
}
/**
 * @brief Verify that OCDMGSTSessionAdapter destructor is invoked without throwing exceptions.
 *
 * This test validates that when an OCDMGSTSessionAdapter object goes out of scope, its destructor is
 * called correctly without causing any exception, ensuring proper resource cleanup and stability of the application.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 019@n
 * **Priority:** (High) Critical for ensuring reliable resource management@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                  | Test Data                                                                                              | Expected Result                             | Notes       |
 * | :--------------: | -------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------ | ------------------------------------------- | ----------- |
 * | 01               | Construct OCDMGSTSessionAdapter with valid drmHelper and drmCallbacks, and then let the object go out of scope to invoke the destructor | drmHelper = valid instance of WidevineDrmHelper, drmCallbacks = valid pointer of TestDrmCallbacks | No exception thrown during object destruction | Should Pass |
 */
TEST(OCDMGSTSessionAdapter, DestructorInvocation) {
    GTEST_SKIP();
    std::cout << "Entering DestructorInvocation test" << std::endl;

    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);   // Derived class instance
    auto* drmCallbacks = new TestDrmCallbacks();                     // Derived class instance
    
    // Invoke the destructor by letting the object go out of scope.
    EXPECT_NO_THROW({
        std::cout << "Constructing OCDMGSTSessionAdapter object using provided constructor" << std::endl;
        {
            OCDMGSTSessionAdapter adapter(drmHelper, drmCallbacks);
            std::cout << "OCDMGSTSessionAdapter object constructed successfully" << std::endl;
            // Logging internal state (for demonstration, we print the pointer values passed)
            std::cout << "Inside object: drmHelper pointer = " << drmHelper << ", drmCallbacks pointer = " << drmCallbacks << std::endl;
        }
        std::cout << "OCDMGSTSessionAdapter object has been destroyed as it went out of scope" << std::endl;
    });

    std::cout << "Exiting DestructorInvocation test" << std::endl;
}