
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
#include "VanillaDrmHelper.h"


// Test Case: Successful default construction initializes internal state correctly and does not throw exceptions during instantiation
/**
 * @brief Validate successful default construction of VanillaDrmHelper object.
 *
 * This test verifies that the default constructor of VanillaDrmHelper can be invoked without throwing any exceptions,
 * ensuring that object initialization works as expected.
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
 * | Variation / Step | Description                                                        | Test Data                                    | Expected Result                                                 | Notes       |
 * | :----:           | ------------------------------------------------------------------ | -------------------------------------------- | --------------------------------------------------------------- | ----------- |
 * | 01               | Invoke VanillaDrmHelper default constructor and verify no exception is thrown | constructor = default, no input parameters    | Default constructor completes without throwing an exception      | Should Pass |
 */
TEST(VanillaDrmHelper, SuccessfulDefaultConstruction) {
    std::cout << "Entering SuccessfulDefaultConstruction test" << std::endl;
    
    // Invoke the default constructor and ensure no exceptions are thrown
    EXPECT_NO_THROW({
        VanillaDrmHelper helper;
        std::cout << "Invoked VanillaDrmHelper default constructor." << std::endl;

    });
    
    std::cout << "Exiting SuccessfulDefaultConstruction test" << std::endl;
}
/**
 * @brief Validate that createInitData does not modify an empty vector
 *
 * This test verifies that invoking the createInitData method on a VanillaDrmHelper object with an empty vector as input does not cause any modifications or exceptions. The test creates a VanillaDrmHelper object, prepares an empty vector, calls createInitData, and then asserts that the vector remains empty.
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
 * | Variation / Step | Description                                                | Test Data                                                       | Expected Result                                                      | Notes           |
 * | :--------------: | ---------------------------------------------------------- | --------------------------------------------------------------- | -------------------------------------------------------------------- | --------------- |
 * | 01               | Invoke VanillaDrmHelper default constructor                | N/A                                                             | Object created without exception                                     | Should be successful |
 * | 02               | Prepare initData as an empty vector                        | initData = {} (empty vector)                                      | initData vector is empty                                               | Should be successful |
 * | 03               | Invoke createInitData with the empty vector                | helper object, initData = {} (empty vector)                       | No exception thrown; initData vector remains empty                     | Should Pass     |
 */
TEST(VanillaDrmHelper, ValidateCreateInitDataEmptyVector) {
    std::cout << "Entering ValidateCreateInitDataEmptyVector test" << std::endl;

    // Create object using default constructor and log creation
    std::cout << "Invoking VanillaDrmHelper default constructor" << std::endl;
    EXPECT_NO_THROW({
        VanillaDrmHelper helper;
        std::cout << "VanillaDrmHelper object created" << std::endl;

        // Prepare initData as empty vector and log its initial state
        std::vector<uint8_t> initData;
        std::cout << "Initial initData vector size: " << initData.size() << std::endl;

        // Invoke createInitData method and log invocation
        std::cout << "Invoking createInitData with empty vector" << std::endl;
        EXPECT_NO_THROW(helper.createInitData(initData));

        // Log final state of initData vector
        std::cout << "After createInitData call, initData vector size: " << initData.size() << std::endl;
        if (initData.empty()) {
            std::cout << "initData vector remains empty as expected" << std::endl;
        } else {
            std::cout << "initData vector unexpectedly modified" << std::endl;
        }
        EXPECT_TRUE(initData.empty());
    });
    
    std::cout << "Exiting ValidateCreateInitDataEmptyVector test" << std::endl;
}
/**
 * @brief Validate that createInitData handles a non-empty vector appropriately
 *
 * This test verifies the behavior of the VanillaDrmHelper::createInitData API when provided with a non-empty vector.
 * It checks that the method call does not alter the vector's state and that no exceptions are thrown during execution.
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
 * | Variation / Step | Description                                                                               | Test Data                                | Expected Result                                                      | Notes            |
 * | :--------------: | ----------------------------------------------------------------------------------------- | ---------------------------------------- | -------------------------------------------------------------------- | ---------------- |
 * | 01               | Invoke default constructor of VanillaDrmHelper and log the object creation.               | None                                     | Object is constructed without exceptions.                          | Should be successful |
 * | 02               | Prepare a non-empty vector (10, 20, 30) and log its initial state.                         | initData = {10, 20, 30}                    | Vector contains initial values; ready for API call.                  | Should be successful |
 * | 03               | Call createInitData with the non-empty vector and log the invocation.                       | input: initData = {10, 20, 30}             | API executes without throwing exceptions and does not alter the vector.| Should Pass       |
 * | 04               | Verify that the vector remains unchanged after the API call.                              | Expected vector = {10, 20, 30}             | The vector equals the expected values.                               | Should be successful |
 */
TEST(VanillaDrmHelper, ValidateCreateInitDataNonEmptyVector) {
    std::cout << "Entering ValidateCreateInitDataNonEmptyVector test" << std::endl;

    // Create object using default constructor and log creation
    std::cout << "Invoking VanillaDrmHelper default constructor" << std::endl;
    EXPECT_NO_THROW({
        VanillaDrmHelper helper;
        std::cout << "VanillaDrmHelper object created" << std::endl;

        // Prepare initData with initial values and log its state
        std::vector<uint8_t> initData = {10, 20, 30};
        std::cout << "Initial initData vector values: ";
        for (const auto &val : initData) {
            std::cout << static_cast<int>(val) << " ";
        }
        std::cout << std::endl;

        // Invoke createInitData method and log invocation
        std::cout << "Invoking createInitData with non-empty vector" << std::endl;
        EXPECT_NO_THROW(helper.createInitData(initData));

        // Log final state of initData vector
        std::cout << "After createInitData call, initData vector values: ";
        for (const auto &val : initData) {
            std::cout << static_cast<int>(val) << " ";
        }
        std::cout << std::endl;

        // Check that initData remains unchanged
        std::vector<uint8_t> expected = {10, 20, 30};
        EXPECT_EQ(initData, expected);
        std::cout << "initData vector remains unchanged as expected" << std::endl;
    });

    std::cout << "Exiting ValidateCreateInitDataNonEmptyVector test" << std::endl;
}
/**
 * @brief Verify that VanillaDrmHelper handles empty initialization data correctly.
 *
 * This test verifies that when an empty vector is provided to the createInitData method of the VanillaDrmHelper, the method processes the input without throwing exceptions and the vector remains unchanged, ensuring proper handling of empty inputs.
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
 * | Variation / Step | Description                                                                                        | Test Data                         | Expected Result                                                  | Notes      |
 * | :--------------: | -------------------------------------------------------------------------------------------------- | --------------------------------- | ---------------------------------------------------------------- | ---------- |
 * | 01               | Call VanillaDrmHelper default constructor, initialize an empty vector, invoke createInitData, and verify that the vector remains empty | input: initData = []                | No exceptions thrown; initData remains empty (vector size = 0)      | Should Pass |
 */
TEST(VanillaDrmHelper, EmptyInitData)
{
    std::cout << "Entering EmptyInitData test" << std::endl;

    // Construct VanillaDrmHelper object using default constructor
    EXPECT_NO_THROW({
        VanillaDrmHelper drmHelper;
        std::cout << "VanillaDrmHelper default constructor invoked." << std::endl;

        // Prepare an empty vector for initialization data.
        std::vector<uint8_t> initData;
        std::cout << "Initial initData vector size: " << initData.size() << " (Expected: 0)" << std::endl;

        // Invoke the createInitData method.
        std::cout << "Invoking createInitData with empty vector." << std::endl;
        drmHelper.createInitData(initData);
        std::cout << "createInitData invoked." << std::endl;

        // Debug: Print vector contents after method invocation.
        std::cout << "After createInitData, initData vector size: " << initData.size() << " (Expected: 0)" << std::endl;
        EXPECT_EQ(initData.size(), 0u);
    });

    std::cout << "Exiting EmptyInitData test" << std::endl;
}
/**
 * @brief Verify that createInitData properly handles a non-empty initData vector without modification.
 *
 * This test verifies that when a non-empty vector containing initial values is provided to the createInitData method of VanillaDrmHelper, the method executes without throwing any exceptions and the vector remains unchanged after the call. This ensures the correctness of the API in managing input data without altering valid input.
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
 * | Variation / Step | Description                                                            | Test Data                                                | Expected Result                                            | Notes             |
 * | :----:           | ---------------------------------------------------------------------- | -------------------------------------------------------- | ---------------------------------------------------------- | ----------------- |
 * | 01               | Construct VanillaDrmHelper object using default constructor.          | N/A                                                    | Object created without exceptions.                       | Should be successful |
 * | 02               | Prepare a non-empty vector with initial values [1, 2, 3, 4].           | initData = {1, 2, 3, 4}                                  | Vector holds the values [1, 2, 3, 4].                      | Should be successful |
 * | 03               | Invoke createInitData with the non-empty vector.                       | input: initData = {1, 2, 3, 4}                             | Method executes without exceptions.                      | Should Pass       |
 * | 04               | Validate that the vector remains unchanged after invocation.           | initData = {1, 2, 3, 4}, expectedInitData = {1, 2, 3, 4}   | Vector equality assertion passes.                        | Should Pass       |
 */
TEST(VanillaDrmHelper, NonEmptyInitData)
{
    std::cout << "Entering NonEmptyInitData test" << std::endl;

    // Construct VanillaDrmHelper object using default constructor
    EXPECT_NO_THROW({
        VanillaDrmHelper drmHelper;
        std::cout << "VanillaDrmHelper default constructor invoked." << std::endl;
        
        // Prepare a non-empty vector with initial values [1, 2, 3, 4].
        std::vector<uint8_t> initData = {1, 2, 3, 4};
        std::cout << "Initial initData vector values: ";
        for(auto val : initData)
        {
            std::cout << static_cast<int>(val) << " ";
        }
        std::cout << " (Expected: 1 2 3 4)" << std::endl;

        // Invoke the createInitData method.
        std::cout << "Invoking createInitData with non-empty vector." << std::endl;
        drmHelper.createInitData(initData);
        std::cout << "createInitData invoked." << std::endl;

        // Debug: Print vector contents after method invocation.
        std::cout << "After createInitData, initData vector values: ";
        for(auto val : initData)
        {
            std::cout << static_cast<int>(val) << " ";
        }
        std::cout << " (Expected: 1 2 3 4)" << std::endl;

        // Validate that the vector remains unchanged.
        std::vector<uint8_t> expectedInitData = {1, 2, 3, 4};
        EXPECT_EQ(initData, expectedInitData);
    });

    std::cout << "Exiting NonEmptyInitData test" << std::endl;
}
/**
 * @brief Verify the DRM system friendly name is non-empty using VanillaDrmHelper
 *
 * This test creates an instance of VanillaDrmHelper using its non-parameterized constructor and invokes the friendlyName() method. It then checks that the returned DRM system name is non-empty. This ensures that the helper properly returns a valid DRM identifier.
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
 * | Variation / Step | Description                                                                  | Test Data                                                         | Expected Result                                                     | Notes         |
 * | :---------------: | ---------------------------------------------------------------------------- | ----------------------------------------------------------------- | ------------------------------------------------------------------- | ------------- |
 * | 01                | Create VanillaDrmHelper object using non-parameterized constructor.            | No input, output: helper object created                             | Object is created successfully with no exception thrown.            | Should be successful |
 * | 02                | Invoke friendlyName() method to retrieve the DRM system name.                | Input: helper; Output: drmName = value returned from friendlyName()  | Returns a non-empty string indicating the DRM system name.          | Should Pass   |
 * | 03                | Validate the returned DRM system name is non-empty using assertion check.     | Input: drmName; Assertion: EXPECT_FALSE(drmName.empty())            | Assertion passes confirming that the returned DRM system name is non-empty. | Should Pass   |
 */
TEST(VanillaDrmHelper, VerifyFriendlyName)
{
    std::cout << "Entering VerifyFriendlyName test" << std::endl;
    
    // Create VanillaDrmHelper object using non-parameterized constructor and ensure no exception is thrown.
    VanillaDrmHelper helper;
    std::cout << "VanillaDrmHelper object created successfully." << std::endl;

    // Invoke the friendlyName() method and log the invocation
    std::cout << "Invoking friendlyName() method." << std::endl;
    const std::string& drmName = helper.friendlyName();
    std::cout << "Method friendlyName() returned value: " << drmName << std::endl;
    
    // Check that the returned string is non-empty.
    EXPECT_FALSE(drmName.empty());
    std::cout << "Verified that the returned DRM system name is non-empty." << std::endl;
    
    std::cout << "Exiting VerifyFriendlyName test" << std::endl;
}
/**
 * @brief Test valid DRM challenge data with all non-empty fields
 *
 * This test verifies that when the DRM challenge data contains all required non-empty fields—including a valid shared DrmData pointer, URL, and access token—the generateLicenseRequest method of VanillaDrmHelper executes without throwing exceptions. This ensures that valid input data is properly handled.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 007@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Print entry log for test start | None | Log message "Entering ValidDRMChallengeDataWithAllNonEmptyFields test" printed | Should be successful |
 * | 02 | Create and populate ChallengeInfo with a valid DrmData pointer | challengeInfo.data = valid DrmData pointer | challengeInfo.data is set to a valid pointer | Should be successful |
 * | 03 | Set URL and access token in ChallengeInfo | challengeInfo.url = "https://drm.example.com/license", challengeInfo.accessToken = "validAccessToken" | challengeInfo.url and challengeInfo.accessToken are set correctly | Should be successful |
 * | 04 | Create a default LicenseRequest object | licenseRequest with default values (method = DRM_RETRIEVE, url = "", payload = "", licenseAnonymousRequest = false, headers size = 0) | LicenseRequest is initialized with default values | Should be successful |
 * | 05 | Instantiate VanillaDrmHelper | VanillaDrmHelper instance creation | Instance is created without exceptions | Should Pass |
 * | 06 | Invoke generateLicenseRequest with challengeInfo and LicenseRequest | challengeInfo (data, url, accessToken), licenseRequest (default values) | Method completes without throwing exceptions | Should Pass |
 * | 07 | Print completion and test exit logs | None | Appropriate exit messages are logged | Should be successful |
 */
TEST(VanillaDrmHelper, ValidDRMChallengeDataWithAllNonEmptyFields) {
    std::cout << "Entering ValidDRMChallengeDataWithAllNonEmptyFields test" << std::endl;
    
    // Create and populate a ChallengeInfo with valid non-empty fields.
    ChallengeInfo challengeInfo;
    {
        // Create a valid shared pointer to DrmData.
        // Assuming DrmData has a default constructor available.
        std::shared_ptr<DrmData> drmDataPtr = std::make_shared<DrmData>();
        challengeInfo.data = drmDataPtr;
        std::cout << "Set challengeInfo.data to a valid DrmData pointer" << std::endl;
    }
    
    // Set URL and access token.
    const std::string testUrl = "https://drm.example.com/license";
    const std::string testAccessToken = "validAccessToken";
    challengeInfo.url = testUrl;
    challengeInfo.accessToken = testAccessToken;
    std::cout << "Set challengeInfo.url to \"" << challengeInfo.url << "\"" << std::endl;
    std::cout << "Set challengeInfo.accessToken to \"" << challengeInfo.accessToken << "\"" << std::endl;
    
    // Create a default LicenseRequest object.
    LicenseRequest licenseRequest;
    std::cout << "Created default LicenseRequest:" << std::endl;
    std::cout << "  licenseRequest.method (default) assumed to be DRM_RETRIEVE" << std::endl;
    std::cout << "  licenseRequest.url: \"" << licenseRequest.url << "\"" << std::endl;
    std::cout << "  licenseRequest.payload: \"" << licenseRequest.payload << "\"" << std::endl;
    std::cout << "  licenseRequest.licenseAnonymousRequest: " 
              << (licenseRequest.licenseAnonymousRequest ? "true" : "false") << std::endl;
    std::cout << "  licenseRequest.headers size: " << licenseRequest.headers.size() << std::endl;
    
    // Create a VanillaDrmHelper object using its default constructor.
    EXPECT_NO_THROW(VanillaDrmHelper vanillaDrmHelper;);
    VanillaDrmHelper vanillaDrmHelper;
    std::cout << "Created VanillaDrmHelper instance using default constructor" << std::endl;
    
    // Invoke the generateLicenseRequest method.
    std::cout << "Invoking VanillaDrmHelper::generateLicenseRequest with provided challengeInfo" << std::endl;
    EXPECT_NO_THROW(vanillaDrmHelper.generateLicenseRequest(challengeInfo, licenseRequest));
    std::cout << "Method invocation completed" << std::endl;
    std::cout << "Exiting ValidDRMChallengeDataWithAllNonEmptyFields test" << std::endl;
}
/**
 * @brief Validate if getDrmCodecType method returns the expected codec type.
 *
 * This test verifies that the VanillaDrmHelper class instantiates properly using the default constructor and that its getDrmCodecType method returns the expected codec type. The constant for the expected codec type is assumed to be 1, and the test confirms that no exceptions are thrown during object creation and method invocation. 
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
 * | Variation / Step | Description                                                                 | Test Data                                                  | Expected Result                                               | Notes        |
 * | :----:           | :-------------------------------------------------------------------------- | :--------------------------------------------------------- | :------------------------------------------------------------ | :----------- |
 * | 01               | Create a VanillaDrmHelper instance using the default constructor.           | No input; output: instance of VanillaDrmHelper             | Instance is created without throwing an exception.            | Should Pass  |
 * | 02               | Invoke the getDrmCodecType method on the helper instance.                     | Input: helper instance; Invocation: helper.getDrmCodecType() | Method returns an integer representing the codec type.       | Should Pass  |
 * | 03               | Validate the returned codec type against the expected constant value (1).     | Input: returned codec type, expectedCodecType = 1            | The returned codec type equals 1 as verified by EXPECT_EQ.      | Should Pass  |
 */
TEST(VanillaDrmHelper, GetDrmCodecType_ReturnsExpected) {
    std::cout << "Entering GetDrmCodecType_ReturnsExpected test" << std::endl;
    
    // The following block ensures no exception is thrown during construction and method invocation.
    EXPECT_NO_THROW({
        // Create a properly configured VanillaDrmHelper object using the default constructor.
        VanillaDrmHelper helper;
        std::cout << "Constructed VanillaDrmHelper instance using default constructor." << std::endl;

        // Invoke the getDrmCodecType() method.
        std::cout << "Invoking getDrmCodecType method." << std::endl;
        int codecType = helper.getDrmCodecType();
        std::cout << "Retrieved codec type: " << codecType << std::endl;

        // Expected constant value for CODEC_TYPE.
        // For this test, we assume the internally defined CODEC_TYPE constant equals 1.
        const int expectedCodecType = 1;
        std::cout << "Expected codec type: " << expectedCodecType << std::endl;

        // Verify that the returned codec type is as expected.
        EXPECT_EQ(codecType, expectedCodecType);
    });
    
    std::cout << "Exiting GetDrmCodecType_ReturnsExpected test" << std::endl;
}
/**
 * @brief Verifies that a non-empty DRM key vector is cleared after invoking getKey.
 *
 * This test verifies that after invoking the getKey method on a VanillaDrmHelper instance,
 * a non-empty DRM key vector, initially containing DRM keys, is cleared. This ensures that
 * the getKey method performs the expected functionality of clearing the provided DRM key vector.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 009@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                        | Test Data                                                  | Expected Result                                         | Notes        |
 * | :--------------: | ------------------------------------------------------------------ | ---------------------------------------------------------- | ------------------------------------------------------- | ------------ |
 * | 01               | Create an instance of VanillaDrmHelper using the default constructor | Constructor = default                                      | Instance is created without exceptions                | Should Pass  |
 * | 02               | Prepare a non-empty DRM key vector with values 0x01, 0x02, 0x03       | keyID = {0x01, 0x02, 0x03}                                  | Vector initialized with three elements                | Should be successful |
 * | 03               | Invoke getKey method with the non-empty DRM key vector               | Input: keyID (non-empty vector), helper instance           | getKey is called and returns successfully             | Should Pass  |
 * | 04               | Verify that the DRM key vector is cleared post getKey invocation      | Output: keyID size, expected keyID size = 0                | Assertion passes: keyID size is zero                  | Should Pass  |
 */
TEST(VanillaDrmHelper, ClearingNonEmptyDRMKeyVector) {
    std::cout << "Entering ClearingNonEmptyDRMKeyVector test" << std::endl;

    // Create an instance of VanillaDrmHelper using default constructor.
    EXPECT_NO_THROW({
        VanillaDrmHelper helper;
        std::cout << "VanillaDrmHelper object created successfully." << std::endl;
        
        // Prepare a non-empty DRM key vector with values 0x01, 0x02, 0x03.
        std::vector<uint8_t> keyID = {0x01, 0x02, 0x03};
        std::cout << "Initial keyID vector contents: ";
        for (auto byte : keyID) {
            std::cout << "0x" << std::hex << static_cast<int>(byte) << " ";
        }
        std::cout << std::dec << std::endl;
        std::cout << "keyID size before getKey: " << keyID.size() << std::endl;

        // Invoke the getKey method.
        std::cout << "Invoking getKey with a non-empty keyID vector." << std::endl;
        helper.getKey(keyID);
        std::cout << "Returned from getKey invocation." << std::endl;

        // After invocation, keyID should be cleared.
        std::cout << "keyID size after getKey: " << keyID.size() << std::endl;
        EXPECT_EQ(keyID.size(), 0);
    });
    
    std::cout << "Exiting ClearingNonEmptyDRMKeyVector test" << std::endl;
}
/**
 * @brief Tests the behavior of VanillaDrmHelper::getKey when operating on an empty DRM key vector.
 *
 * This test verifies that invoking getKey on an initially empty DRM key vector does not modify the vector.
 * It ensures that no exceptions are thrown during the operation and that the DRM key vector remains empty.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 010@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- | -------------- | ----- |
 * | 01 | Create instance of VanillaDrmHelper using its default constructor | No input arguments | Object is created successfully without throwing any exception | Should Pass |
 * | 02 | Verify that the keyID vector is empty prior to calling getKey | keyID vector: size = 0 | keyID.size() returns 0 | Should be successful |
 * | 03 | Invoke the getKey method with the empty keyID vector | input: keyID = empty vector | Method completes execution without throwing exceptions | Should Pass |
 * | 04 | Confirm that keyID vector remains empty after the getKey call | After getKey: keyID vector should still be empty (size = 0) | keyID.size() still equals 0 | Should Pass |
 */
TEST(VanillaDrmHelper, OperatingOnEmptyDRMKeyVector) {
    std::cout << "Entering OperatingOnEmptyDRMKeyVector test" << std::endl;

    // Create an instance of VanillaDrmHelper using default constructor.
    EXPECT_NO_THROW({
        VanillaDrmHelper helper;
        std::cout << "VanillaDrmHelper object created successfully." << std::endl;
        
        // Prepare an already empty DRM key vector.
        std::vector<uint8_t> keyID;
        std::cout << "Initial keyID vector is empty." << std::endl;
        std::cout << "keyID size before getKey: " << keyID.size() << std::endl;

        // Invoke the getKey method.
        std::cout << "Invoking getKey with an empty keyID vector." << std::endl;
        helper.getKey(keyID);
        std::cout << "Returned from getKey invocation." << std::endl;

        // After invocation, keyID should remain empty.
        std::cout << "keyID size after getKey: " << keyID.size() << std::endl;
        EXPECT_EQ(keyID.size(), 0);
    });
    
    std::cout << "Exiting OperatingOnEmptyDRMKeyVector test" << std::endl;
}
/**
 * @brief Verify that the getKey method handles an empty vector without throwing exceptions.
 *
 * This test verifies the behavior of VanillaDrmHelper::getKey when provided with an empty vector.
 * The test ensures that:
 * - Creating a VanillaDrmHelper object does not throw any exceptions.
 * - Invoking getKey with an empty keyID vector does not throw any exceptions.
 * - The keyID vector remains empty after calling getKey.
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
 * | Variation / Step | Description                                                       | Test Data                                                          | Expected Result                                                                                   | Notes        |
 * | :--------------: | ----------------------------------------------------------------- | ------------------------------------------------------------------ | ------------------------------------------------------------------------------------------------- | ------------ |
 * | 01               | Create a VanillaDrmHelper object using the default constructor.     | (No input parameters)                                              | VanillaDrmHelper object is created without any exception.                                        | Should Pass  |
 * | 02               | Prepare an empty vector for keyID.                                  | keyID = []                                                         | keyID vector is empty.                                                                            | Should be successful |
 * | 03               | Invoke the getKey method using the empty keyID vector.              | input: keyID = []                                                  | getKey is invoked without exceptions.                                                             | Should Pass  |
 * | 04               | Validate that the keyID vector remains empty after the method call.  | output: keyID, expected size = 0                                    | keyID vector remains empty and the assertion EXPECT_TRUE(keyID.empty()) passes.                    | Should be successful |
 */
TEST(VanillaDrmHelper, getKeyEmptyVector) {
    std::cout << "Entering getKeyEmptyVector test" << std::endl;

    // Creating VanillaDrmHelper object using default constructor.
    std::cout << "Creating VanillaDrmHelper object using default constructor." << std::endl;
    EXPECT_NO_THROW(VanillaDrmHelper drmHelper);

    VanillaDrmHelper drmHelper;
    
    // Prepare an empty vector.
    std::vector<uint8_t> keyID;
    std::cout << "Initial keyID vector is empty with size: " << keyID.size() << std::endl;
    
    // Invocation of getKey method.
    std::cout << "Invoking getKey with an empty vector." << std::endl;
    EXPECT_NO_THROW(drmHelper.getKey(keyID));
    
    // Debug log: print state of keyID after invoking getKey.
    std::cout << "After getKey invocation, keyID vector size: " << keyID.size() << std::endl;
    if (keyID.empty()) {
        std::cout << "keyID remains empty as expected." << std::endl;
    } else {
        std::cout << "keyID is not empty - unexpected result." << std::endl;
    }
    
    EXPECT_TRUE(keyID.empty());
    
    std::cout << "Exiting getKeyEmptyVector test" << std::endl;
}
/**
 * @brief Verify that the getKey method does not alter a pre-populated vector
 *
 * This test verifies that invoking the getKey method of a VanillaDrmHelper object using a pre-populated key vector does not modify the vector's contents. It ensures that the method behaves as expected when called with a vector already containing key values.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 012@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |@n
 * | :----: | --------- | ---------- |-------------- | ----- |@n
 * | 01 | Create VanillaDrmHelper object using default constructor | output1 = instance creation | No exception thrown, instance created successfully | Should Pass |@n
 * | 02 | Prepare vector with elements {0x01, 0x02, 0x03} | keyID = {0x01, 0x02, 0x03} | Vector pre-populated with provided values | Should be successful |@n
 * | 03 | Invoke getKey method with pre-populated vector | drmHelper instance, keyID = {0x01, 0x02, 0x03} | Method executes without throwing an exception | Should Pass |@n
 * | 04 | Validate that keyID remains unchanged after invocation | keyID input = {0x01, 0x02, 0x03}, expected = {0x01, 0x02, 0x03} | Vector remains unchanged (EXPECT_EQ passes) | Should Pass |
 */
TEST(VanillaDrmHelper, getKeyPrePopulatedVector) {
    std::cout << "Entering getKeyPrePopulatedVector test" << std::endl;

    // Creating VanillaDrmHelper object using default constructor.
    std::cout << "Creating VanillaDrmHelper object using default constructor." << std::endl;
    EXPECT_NO_THROW(VanillaDrmHelper drmHelper);
    
    VanillaDrmHelper drmHelper;

    // Prepare a vector pre-populated with elements {0x01, 0x02, 0x03}.
    std::vector<uint8_t> keyID = {0x01, 0x02, 0x03};
    std::cout << "Initial keyID vector elements: ";
    for (const auto &value : keyID) {
        std::cout << "0x" << std::hex << static_cast<int>(value) << " ";
    }
    std::cout << std::dec << " with size: " << keyID.size() << std::endl;
    
    // Invocation of getKey method.
    std::cout << "Invoking getKey with a pre-populated vector." << std::endl;
    EXPECT_NO_THROW(drmHelper.getKey(keyID));
    
    // Debug log: print state of keyID after invoking getKey.
    std::cout << "After getKey invocation, keyID vector elements: ";
    for (const auto &value : keyID) {
        std::cout << "0x" << std::hex << static_cast<int>(value) << " ";
    }
    std::cout << std::dec << " with size: " << keyID.size() << std::endl;
    
    // Validate that the keyID vector remains unchanged.
    std::vector<uint8_t> expected = {0x01, 0x02, 0x03};
    EXPECT_EQ(keyID, expected);
    
    std::cout << "keyID remains unchanged as expected." << std::endl;
    
    std::cout << "Exiting getKeyPrePopulatedVector test" << std::endl;
}
/**
 * @brief Validate that VanillaDrmHelper is instantiated and its isClearDecrypt method returns true.
 *
 * This test verifies that a VanillaDrmHelper object can be instantiated using its default constructor and that calling the isClearDecrypt() method on this object returns a boolean value true. The test is designed to ensure that the basic functionality for clear decryption is working as expected.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 013@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                          | Test Data                                                         | Expected Result                                                        | Notes              |
 * | :--------------: | ---------------------------------------------------------------------| ------------------------------------------------------------------| ---------------------------------------------------------------------- | ------------------ |
 * | 01               | Instantiate VanillaDrmHelper using the default constructor.          | No input parameters; output: drmHelper object created             | drHelper object should be successfully instantiated.                   | Should be successful|
 * | 02               | Invoke isClearDecrypt() method on the instantiated object.           | Method call: result = drmHelper.isClearDecrypt()                    | isClearDecrypt() should return true; assertion EXPECT_TRUE passes.       | Should Pass         |
 */
TEST(VanillaDrmHelper, BasicFunctionalityIsClearDecrypt) {
    std::cout << "Entering BasicFunctionalityIsClearDecrypt test" << std::endl;
    
    EXPECT_NO_THROW({
        // Instantiating VanillaDrmHelper using default constructor.
        VanillaDrmHelper drmHelper;
        std::cout << "VanillaDrmHelper object instantiated using default constructor." << std::endl;
        
        // Invoking isClearDecrypt() method.
        std::cout << "Calling isClearDecrypt() method on VanillaDrmHelper object." << std::endl;
        bool result = drmHelper.isClearDecrypt();
        std::cout << "isClearDecrypt() returned: " << std::boolalpha << result << std::endl;
        
        // Verifying the returned value.
        EXPECT_TRUE(result) << "Expected isClearDecrypt() to return true.";
        std::cout << "Verified that isClearDecrypt() returned expected value true." << std::endl;
    });
    
    std::cout << "Exiting BasicFunctionalityIsClearDecrypt test" << std::endl;
}
/**
 * @brief Verify that the isClearDecrypt() method returns true.
 *
 * This test case instantiates an object of VanillaDrmHelper using the default constructor and verifies that calling isClearDecrypt() returns true. This confirms that the helper correctly identifies the clear decryption mode.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 014@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                              | Test Data                                             | Expected Result                                          | Notes      |
 * | :--------------: | ------------------------------------------------------------------------ | ----------------------------------------------------- | -------------------------------------------------------- | ---------- |
 * | 01               | Instantiate VanillaDrmHelper using the default constructor and invoke isClearDecrypt() method. | Constructor: no parameters; Method Call: isClearDecrypt() returns result = true | The method returns true and EXPECT_EQ(result, true) passes. | Should Pass|
 */
TEST(VanillaDrmHelper, VerifyIsClearDecryptReturnsTrue) {
    std::cout << "Entering VerifyIsClearDecryptReturnsTrue test" << std::endl;
    
    // Create instance of VanillaDrmHelper using the default constructor.
    EXPECT_NO_THROW({
        VanillaDrmHelper drmHelper;
        std::cout << "VanillaDrmHelper object instantiated successfully using default constructor." << std::endl;
    });
    
    VanillaDrmHelper drmHelper;
    std::cout << "Calling isClearDecrypt() on the VanillaDrmHelper instance." << std::endl;
    
    // Call the method and log the returned value.
    bool result = drmHelper.isClearDecrypt();
    std::cout << "Method isClearDecrypt() returned: " << std::boolalpha << result << std::endl;
    
    // Verify that isClearDecrypt() returns true as specified.
    EXPECT_EQ(result, true);
    
    std::cout << "Exiting VerifyIsClearDecryptReturnsTrue test" << std::endl;
}
/**
 * @brief Verify that the isExternalLicense() method returns true.
 *
 * This test validates that when a VanillaDrmHelper object is created using the default constructor,
 * the isExternalLicense() method correctly returns true, indicating the expected behavior of the API.
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
 * | Variation / Step | Description                                                                  | Test Data                                           | Expected Result                                           | Notes         |
 * | :--------------: | ---------------------------------------------------------------------------- | --------------------------------------------------- | --------------------------------------------------------- | ------------- |
 * | 01               | Create a VanillaDrmHelper object using the default constructor.              | constructor = VanillaDrmHelper()                    | Object is created without throwing an exception.         | Should be successful |
 * | 02               | Invoke the isExternalLicense() method and validate that it returns true.       | method call = isExternalLicense(), output = result  | Returns true as expected and passes the assertion check.   | Should Pass   |
 */
TEST(VanillaDrmHelper, isExternalLicense_ReturnsTrue) {
    std::cout << "Entering isExternalLicense_ReturnsTrue test" << std::endl;
    
    // Create an object using the default constructor and log the creation.
    std::cout << "Invoking VanillaDrmHelper default constructor." << std::endl;
    EXPECT_NO_THROW({
        VanillaDrmHelper drmHelper;
        std::cout << "VanillaDrmHelper object created successfully." << std::endl;
        
        // Invoke the method isExternalLicense() and log the method invocation.
        std::cout << "Invoking isExternalLicense() method." << std::endl;
        bool result = drmHelper.isExternalLicense();
        std::cout << "Method isExternalLicense() returned: " << result << std::endl;
        
        // Validate that the method returns true.
        EXPECT_EQ(result, true);
    });
    
    std::cout << "Exiting isExternalLicense_ReturnsTrue test" << std::endl;
}
/**
 * @brief Verify that VanillaDrmHelper::ocdmSystemId returns an empty string.
 *
 * This test ensures that creating a VanillaDrmHelper object using the default constructor does not throw an exception and that the ocdmSystemId() method returns an empty string. This validates the default state of the DRM system ID.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 016@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                         | Test Data                                              | Expected Result                                                             | Notes       |@n
 * | :--------------: | ------------------------------------------------------------------- | ------------------------------------------------------ | --------------------------------------------------------------------------- | ----------- |@n
 * | 01               | Invoke default constructor of VanillaDrmHelper                      | constructor = default                                  | Object is created successfully without throwing any exceptions            | Should Pass |@n
 * | 02               | Invoke ocdmSystemId() on the VanillaDrmHelper instance                | helper.ocdmSystemId() = called                         | Returns an empty string as verified by EXPECT_EQ("", drmSystemId)             | Should Pass |
 */
TEST(VanillaDrmHelper, VerifyOcdmSystemIdReturnsEmpty) {
    std::cout << "Entering VerifyOcdmSystemIdReturnsEmpty test" << std::endl;

    // Create VanillaDrmHelper object using default constructor.
    // Using EXPECT_NO_THROW for non-parameterized constructor.
    EXPECT_NO_THROW({
        VanillaDrmHelper helper;
        std::cout << "VanillaDrmHelper object created successfully using default constructor." << std::endl;

        // Log method invocation
        std::cout << "Invoking ocdmSystemId() method on VanillaDrmHelper object." << std::endl;
        
        const std::string& drmSystemId = helper.ocdmSystemId();

        // Debug: log the value retrieved from ocdmSystemId()
        std::cout << "Retrieved DRM system ID value: \"" << drmSystemId << "\"" << std::endl;

        // Verify that the returned DRM system ID is an empty string.
        EXPECT_EQ("", drmSystemId);
    });

    std::cout << "Exiting VerifyOcdmSystemIdReturnsEmpty test" << std::endl;
}
/**
 * @brief Verify that VanillaDrmHelper::parsePssh returns false for non-empty initialization data.
 *
 * This test case creates a VanillaDrmHelper object using its default constructor, then prepares a non-empty initialization data array {0x10, 0x20, 0x30} by copying values into it. Afterwards, it invokes the parsePssh method with the prepared data and checks that the method returns false.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 017@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Instantiate VanillaDrmHelper using default constructor | No input arguments, output: object instantiation | Object is instantiated without throwing an exception | Should be successful |
 * | 02 | Prepare non-empty initialization data {0x10, 0x20, 0x30} | input: initData = {0x10, 0x20, 0x30}, initDataLen = 3 | Initialization data is correctly set | Should be successful |
 * | 03 | Invoke parsePssh using the non-empty initialization data | input: initData = {0x10, 0x20, 0x30}, initDataLen = 3, output: result | The method returns false and the EXPECT_FALSE assertion passes | Should Pass |
 */
TEST(VanillaDrmHelper, ValidNonEmptyInitData)
{
    std::cout << "Entering ValidNonEmptyInitData test" << std::endl;

    // Create VanillaDrmHelper object using default constructor
    EXPECT_NO_THROW({
        VanillaDrmHelper drmHelper;
        std::cout << "VanillaDrmHelper object created using default constructor." << std::endl;

        // Prepare non-empty initialization data: {0x10, 0x20, 0x30}
        uint8_t initData[3];
        // Use strncpy to assign values to fixed size array (simulate by copying from a source buffer)
        const char srcData[3] = { static_cast<char>(0x10), static_cast<char>(0x20), static_cast<char>(0x30) };
        strncpy(reinterpret_cast<char*>(initData), srcData, 3);
        std::cout << "Invoking parsePssh with initData: ";
        for (int i = 0; i < 3; i++) {
            std::cout << "0x" << std::hex << static_cast<int>(initData[i]) << " ";
        }
        std::cout << std::dec << "and initDataLen: 3" << std::endl;

        bool result = drmHelper.parsePssh(initData, 3);
        std::cout << "parsePssh returned: " << std::boolalpha << result << std::endl;
        EXPECT_FALSE(result);
    });

    std::cout << "Exiting ValidNonEmptyInitData test" << std::endl;
}
/**
 * @brief Test VanillaDrmHelper::parsePssh with empty initialization data.
 *
 * This test verifies that the parsePssh method returns false when provided with an empty initialization data pointer and a length of 0.
 * It confirms that no exception is thrown during the execution and that the function correctly handles empty input data.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 018
 * **Priority:** High
 * 
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 * 
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Instantiate VanillaDrmHelper, prepare empty initialization data, and invoke parsePssh with pointer to empty data and length 0. | initData = pointer to an empty string buffer, initDataLen = 0, output = result from parsePssh | parsePssh returns false without throwing any exception and the EXPECT_FALSE assertion passes. | Should Pass |
 */
TEST(VanillaDrmHelper, ValidEmptyInitData)
{
    std::cout << "Entering ValidEmptyInitData test" << std::endl;

    EXPECT_NO_THROW({
        VanillaDrmHelper drmHelper;
        std::cout << "VanillaDrmHelper object created using default constructor." << std::endl;

        // Prepare empty initialization data
        uint8_t initData[1]; // array allocated but will use 0 length.
        // Initialize to zero for safety using strncpy (simulate a fixed-size buffer assignment)
        const char emptyStr[1] = { '\0' };
        strncpy(reinterpret_cast<char*>(initData), emptyStr, 1);
        std::cout << "Invoking parsePssh with empty initData pointer and initDataLen: 0" << std::endl;

        bool result = drmHelper.parsePssh(initData, 0);
        std::cout << "parsePssh returned: " << std::boolalpha << result << std::endl;
        EXPECT_FALSE(result);
    });

    std::cout << "Exiting ValidEmptyInitData test" << std::endl;
}
/**
 * @brief Validate that parsePssh correctly handles a nullptr input with a positive length.
 *
 * This test verifies the behavior of the parsePssh method in VanillaDrmHelper when a null pointer is provided for the initialization data along with a positive length. The test checks that the API does not throw an exception and returns false, ensuring that the API gracefully handles invalid input conditions.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 019@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Instantiate VanillaDrmHelper and invoke parsePssh with a nullptr for initData and positive initDataLen | initData = nullptr, initDataLen = 5, result = boolean | API returns false and assertion confirms false outcome | Should Pass |
 */
TEST(VanillaDrmHelper, NullPointerPositiveLength)
{
    std::cout << "Entering NullPointerPositiveLength test" << std::endl;

    EXPECT_NO_THROW({
        VanillaDrmHelper drmHelper;
        std::cout << "VanillaDrmHelper object created using default constructor." << std::endl;

        // Use nullptr for initData and a positive length 5.
        const uint8_t* initData = nullptr;
        uint32_t initDataLen = 5;
        std::cout << "Invoking parsePssh with initData: nullptr and initDataLen: " << initDataLen << std::endl;

        bool result = drmHelper.parsePssh(initData, initDataLen);
        std::cout << "parsePssh returned: " << std::boolalpha << result << std::endl;
        EXPECT_FALSE(result);
    });

    std::cout << "Exiting NullPointerPositiveLength test" << std::endl;
}
/**
 * @brief Verify that parsePssh properly handles a non-empty initialization data with an extremely large length.
 *
 * This test checks if the VanillaDrmHelper correctly processes non-empty initialization data when provided with an extremely large length (4294967295). The test expects that the parsePssh API does not throw any exceptions and returns false, indicating that the helper safely handles the extreme input.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 020@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- | -------------- | ----- |
 * | 01 | Instantiate VanillaDrmHelper, prepare non-empty initData, set an extremely large initDataLen, and invoke parsePssh. | input: initData = [0xAA, 0xBB, 0xCC], initDataLen = 4294967295; output: expected return value = false | The API should not throw any exceptions and should return false as verified by the assertion. | Should Pass |
 */
TEST(VanillaDrmHelper, ValidNonEmptyInitDataExtremelyLargeLength)
{
    std::cout << "Entering ValidNonEmptyInitDataExtremelyLargeLength test" << std::endl;

    EXPECT_NO_THROW({
        VanillaDrmHelper drmHelper;
        std::cout << "VanillaDrmHelper object created using default constructor." << std::endl;

        // Prepare non-empty initialization data
        uint8_t initData[3];
        const char srcData[3] = { static_cast<char>(0xAA), static_cast<char>(0xBB), static_cast<char>(0xCC) };
        strncpy(reinterpret_cast<char*>(initData), srcData, 3);
        uint32_t initDataLen = 4294967295;  // extremely large length

        std::cout << "Invoking parsePssh with initData: ";
        for (int i = 0; i < 3; i++) {
            std::cout << "0x" << std::hex << static_cast<int>(initData[i]) << " ";
        }
        std::cout << std::dec << "and initDataLen: " << initDataLen << std::endl;

        bool result = drmHelper.parsePssh(initData, initDataLen);
        std::cout << "parsePssh returned: " << std::boolalpha << result << std::endl;
        EXPECT_FALSE(result);
    });

    std::cout << "Exiting ValidNonEmptyInitDataExtremelyLargeLength test" << std::endl;
}
/**
 * @brief Positive test to verify that the DRM metadata can be set without errors.
 *
 * This test verifies that a VanillaDrmHelper object is successfully created using its default constructor and that the setDrmMetaData method correctly accepts a non-empty DRM metadata string without throwing any exceptions. It also confirms that the internal state remains unchanged, ensuring that the API works as expected.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 021
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                         | Test Data                                             | Expected Result                                     | Notes                |
 * | :--------------: | ------------------------------------------------------------------- | ----------------------------------------------------- | --------------------------------------------------- | -------------------- |
 * | 01               | Create a VanillaDrmHelper object using the default constructor      | (none)                                                | Object is created without throwing any exception    | Should Pass          |
 * | 02               | Define and log a DRM metadata string                                  | drmMetaData = Simple DRM metadata                     | DRM metadata string is defined                      | Should be successful |
 * | 03               | Invoke setDrmMetaData method with the defined DRM metadata string       | input: drmMetaData = Simple DRM metadata              | Method executes without throwing an exception      | Should Pass          |
 * | 04               | Confirm that no internal state change occurs after invoking the API    | (none)                                                | Internal state remains unchanged                     | Should be successful |
 */
TEST(VanillaDrmHelper, PositiveNormalDRMMetadata) {
    std::cout << "Entering PositiveNormalDRMMetadata test" << std::endl;
    
    // Creating VanillaDrmHelper object using default constructor
    EXPECT_NO_THROW({
        VanillaDrmHelper helper;
        std::cout << "VanillaDrmHelper object created using default constructor." << std::endl;
        
        // Define and log the input DRM metadata string.
        std::string drmMetaData = "Simple DRM metadata";
        std::cout << "Invoking setDrmMetaData with value: " << drmMetaData << std::endl;
        
        // Invoke the method and ensure execution without throwing exceptions
        EXPECT_NO_THROW(helper.setDrmMetaData(drmMetaData));
        std::cout << "Method setDrmMetaData executed without error." << std::endl;
        
        // Since VanillaDrmHelper does not change its state for DRM metadata,
        // log the expectation that internal state remains unchanged.
        std::cout << "No internal state change expected for VanillaDrmHelper object." << std::endl;
    });
    
    std::cout << "Exiting PositiveNormalDRMMetadata test" << std::endl;
}
/**
 * @brief Validate that VanillaDrmHelper::setDrmMetaData correctly handles an empty DRM metadata string without errors.
 *
 * This test verifies that when an empty DRM metadata string is provided to VanillaDrmHelper::setDrmMetaData, the method does not throw any exceptions and the internal state of the VanillaDrmHelper object remains unchanged. The objective is to ensure the robustness of the API in handling edge cases.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 022@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                          | Test Data                                | Expected Result (Return value/Assertion)                            | Notes            |
 * | :--------------: | ---------------------------------------------------------------------| ---------------------------------------- | ---------------------------------------------------------------------| ---------------- |
 * | 01               | Create VanillaDrmHelper object using the default constructor.         | None                                     | Object is created successfully without throwing any exception.      | Should be successful |
 * | 02               | Invoke setDrmMetaData with an empty DRM metadata string.              | drmMetaData = ""                         | Method executes without throwing any exceptions.                    | Should Pass         |
 * | 03               | Verify that no internal state change occurred post invocation.         | None                                     | Internal state of VanillaDrmHelper remains unchanged.                 | Should be successful |
 */
TEST(VanillaDrmHelper, PositiveEmptyDRMMetadata) {
    std::cout << "Entering PositiveEmptyDRMMetadata test" << std::endl;
    
    // Creating VanillaDrmHelper object using default constructor
    EXPECT_NO_THROW({
        VanillaDrmHelper helper;
        std::cout << "VanillaDrmHelper object created using default constructor." << std::endl;
        
        // Define and log the empty DRM metadata string.
        std::string drmMetaData = "";
        std::cout << "Invoking setDrmMetaData with value: \"" << drmMetaData << "\"" << std::endl;
        
        // Invoke the method and ensure execution without throwing exceptions
        EXPECT_NO_THROW(helper.setDrmMetaData(drmMetaData));
        std::cout << "Method setDrmMetaData executed without error." << std::endl;
        
        // Since VanillaDrmHelper does not change its state for DRM metadata,
        // log the expectation that internal state remains unchanged.
        std::cout << "No internal state change expected for VanillaDrmHelper object." << std::endl;
    });
    
    std::cout << "Exiting PositiveEmptyDRMMetadata test" << std::endl;
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}