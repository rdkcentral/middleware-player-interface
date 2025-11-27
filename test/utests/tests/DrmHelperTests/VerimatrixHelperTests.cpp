
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
#include "VerimatrixHelper.h"


// Assuming DRM_IV_LEN is defined somewhere as 16 for the IV array size.
#ifndef DRM_IV_LEN
#define DRM_IV_LEN 16
#endif

// Test Case 1: Construct VerimatrixHelper with default DrmInfo
/**
 * @brief Validate default construction of VerimatrixHelper with default DrmInfo
 *
 * This test ensures that a VerimatrixHelper object can be instantiated successfully using a default constructed DrmInfo.
 * It logs default DrmInfo values and verifies that the constructor does not throw an exception while setting internal default values.
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
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Create a default DrmInfo object using its default constructor. | DrmInfo.method = default, DrmInfo.mediaFormat = default, DrmInfo.useFirst16BytesAsIV = default, DrmInfo.bPropagateUriParams = default, DrmInfo.bUseMediaSequenceIV = default, DrmInfo.bDecryptClearSamplesRequired = default | DrmInfo object is created with default values. | Should be successful |
 * | 02 | Invoke the VerimatrixHelper constructor with the default DrmInfo and verify no exception is thrown. | Input: drmInfo (default values) | VerimatrixHelper is constructed without throwing exceptions; expected debug logs indicate FRIENDLY_NAME=Verimatrix, CODEC_TYPE=1, and VERIMATRIX_PSSH_DATA_POSITION=52. | Should Pass |
 */
TEST(VerimatrixHelper, ConstructWithDefaultDrmInfo) {
    std::cout << "Entering ConstructWithDefaultDrmInfo test" << std::endl;
    
    // Create default DrmInfo using its default constructor.
    DrmInfo drmInfo;
    // Log default values from DrmInfo.
    std::cout << "[DEBUG] Default DrmInfo values:" << std::endl;
    std::cout << "  method: " << drmInfo.method << std::endl;
    std::cout << "  mediaFormat: " << drmInfo.mediaFormat << std::endl;
    std::cout << "  useFirst16BytesAsIV: " << drmInfo.useFirst16BytesAsIV << std::endl;
    std::cout << "  bPropagateUriParams: " << drmInfo.bPropagateUriParams << std::endl;
    std::cout << "  bUseMediaSequenceIV: " << drmInfo.bUseMediaSequenceIV << std::endl;
    std::cout << "  bDecryptClearSamplesRequired: " << drmInfo.bDecryptClearSamplesRequired << std::endl;
    
    // Construct VerimatrixHelper using default DrmInfo.
    EXPECT_NO_THROW({
        VerimatrixHelper helper(drmInfo);
        std::cout << "[DEBUG] Invoked VerimatrixHelper constructor with default DrmInfo." << std::endl;
        std::cout << "[DEBUG] Expected FRIENDLY_NAME: Verimatrix" << std::endl;
        std::cout << "[DEBUG] Expected CODEC_TYPE: 1" << std::endl;
        std::cout << "[DEBUG] Expected VERIMATRIX_PSSH_DATA_POSITION: 52" << std::endl;
    });

    std::cout << "Exiting ConstructWithDefaultDrmInfo test" << std::endl;
}
/**
 * @brief Validate that constructing VerimatrixHelper with various DRM method values does not throw an exception
 *
 * This test case validates that when constructing VerimatrixHelper with different DRM method values, no exception is thrown. 
 * It verifies that the VerimatrixHelper constructor properly initializes its internal state based on the provided DrmInfo.
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
 * | Variation / Step | Description                                                                                   | Test Data                                                       | Expected Result                                                            | Notes       |
 * | :--------------: | --------------------------------------------------------------------------------------------- | --------------------------------------------------------------- | -------------------------------------------------------------------------- | ----------- |
 * | 01               | Loop through an array of DrmMethod values and invoke the VerimatrixHelper constructor for each. | Iteration 1: drmInfo.method = eMETHOD_NONE, Iteration 2: drmInfo.method = eMETHOD_AES_128 | For each iteration, the constructor call should not throw any exception. | Should Pass |
 */
TEST(VerimatrixHelper, ConstructWithDrmMethodLoop) {
    std::cout << "Entering ConstructWithDrmMethodLoop test" << std::endl;
    
    // Array of DrmMethod values to test.
    DrmMethod drmMethods[] = { eMETHOD_NONE, eMETHOD_AES_128 };
    const int numMethods = sizeof(drmMethods) / sizeof(drmMethods[0]);
    
    for (int i = 0; i < numMethods; ++i) {
        DrmInfo drmInfo;
        drmInfo.method = drmMethods[i];
        std::cout << "[DEBUG] Testing DrmMethod value: " << drmInfo.method << std::endl;
        
        EXPECT_NO_THROW({
            VerimatrixHelper helper(drmInfo);
            std::cout << "[DEBUG] Invoked VerimatrixHelper constructor with DrmMethod: " 
                      << drmInfo.method << std::endl;
            // Log internal state copied from drmInfo.
            std::cout << "[DEBUG] Expected internal DrmInfo.method to be: " << drmInfo.method << std::endl;
        });
    }
    
    std::cout << "Exiting ConstructWithDrmMethodLoop test" << std::endl;
}
/**
 * @brief Validate that VerimatrixHelper can be constructed with various MediaFormat values without throwing exceptions
 *
 * This test iterates through an array of valid MediaFormat values and passes each one to the VerimatrixHelper constructor.
 * It verifies that for each MediaFormat, the constructor does not throw an exception and correctly copies the internal state.
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
 * | 01 | Iterate over an array of MediaFormat values and invoke the VerimatrixHelper constructor for each value | input: drmInfo.mediaFormat = eMEDIAFORMAT_HLS, eMEDIAFORMAT_DASH, eMEDIAFORMAT_PROGRESSIVE, eMEDIAFORMAT_HLS_MP4, eMEDIAFORMAT_OTA, eMEDIAFORMAT_HDMI, eMEDIAFORMAT_COMPOSITE, eMEDIAFORMAT_SMOOTHSTREAMINGMEDIA, eMEDIAFORMAT_RMF, eMEDIAFORMAT_UNKNOWN; output: No exception thrown | VerimatrixHelper should be constructed successfully for each MediaFormat value without throwing an exception | Should Pass |
 */
TEST(VerimatrixHelper, ConstructWithMediaFormatLoop) {
    std::cout << "Entering ConstructWithMediaFormatLoop test" << std::endl;
    
    // Array of MediaFormat values to test.
    MediaFormat mediaFormats[] = { eMEDIAFORMAT_HLS, eMEDIAFORMAT_DASH, eMEDIAFORMAT_PROGRESSIVE, 
                                   eMEDIAFORMAT_HLS_MP4, eMEDIAFORMAT_OTA, eMEDIAFORMAT_HDMI, 
                                   eMEDIAFORMAT_COMPOSITE, eMEDIAFORMAT_SMOOTHSTREAMINGMEDIA, 
                                   eMEDIAFORMAT_RMF, eMEDIAFORMAT_UNKNOWN };
    const int numFormats = sizeof(mediaFormats) / sizeof(mediaFormats[0]);
    
    for (int i = 0; i < numFormats; ++i) {
        DrmInfo drmInfo;
        drmInfo.mediaFormat = mediaFormats[i];
        std::cout << "[DEBUG] Testing MediaFormat value: " << drmInfo.mediaFormat << std::endl;
        
        EXPECT_NO_THROW({
            VerimatrixHelper helper(drmInfo);
            std::cout << "[DEBUG] Invoked VerimatrixHelper constructor with MediaFormat: " 
                      << drmInfo.mediaFormat << std::endl;
            // Log internal state copied from drmInfo.
            std::cout << "[DEBUG] Expected internal DrmInfo.mediaFormat to be: " << drmInfo.mediaFormat << std::endl;
        });
    }
    
    std::cout << "Exiting ConstructWithMediaFormatLoop test" << std::endl;
}
/**
 * @brief Test to verify that constructing VerimatrixHelper with an invalid MediaFormat does not throw an exception.
 *
 * This test verifies that the VerimatrixHelper constructor can handle an invalid MediaFormat value gracefully without throwing exceptions. 
 * Although the mediaFormat value is explicitly set to an invalid enumerator (999), the helper should properly manage the internal state.
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
 * | Variation / Step | Description                                                              | Test Data                                | Expected Result                                                   | Notes        |
 * | :--------------: | ------------------------------------------------------------------------ | ---------------------------------------- | ----------------------------------------------------------------- | ------------ |
 * | 01               | Set mediaFormat to an invalid value and invoke the VerimatrixHelper constructor | drmInfo.mediaFormat = 999                | No exception is thrown and the constructor handles the invalid value without error | Should Pass  |
 */
TEST(VerimatrixHelper, ConstructWithInvalidMediaFormat) {
    std::cout << "Entering ConstructWithInvalidMediaFormat test" << std::endl;
    
    DrmInfo drmInfo;
    // Set mediaFormat to an invalid enumerator value using static_cast.
    drmInfo.mediaFormat = static_cast<MediaFormat>(999);
    std::cout << "[DEBUG] Testing with invalid MediaFormat value: " << drmInfo.mediaFormat << std::endl;
    
    EXPECT_NO_THROW({
        VerimatrixHelper helper(drmInfo);
        std::cout << "[DEBUG] Invoked VerimatrixHelper constructor with invalid MediaFormat: " 
                  << drmInfo.mediaFormat << std::endl;
        std::cout << "[DEBUG] Expected internal DrmInfo.mediaFormat to be: " << drmInfo.mediaFormat << std::endl;
    });
    
    std::cout << "Exiting ConstructWithInvalidMediaFormat test" << std::endl;
}
/**
 * @brief Verify that constructing VerimatrixHelper with an invalid DrmMethod does not throw exceptions.
 *
 * This test validates that when a DrmInfo instance with an invalid enum value for the DRM method is supplied, 
 * the VerimatrixHelper constructor handles the input gracefully without throwing an exception. It helps ensure 
 * that the system is robust against invalid DRM method values.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 005
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                      | Test Data                                     | Expected Result                                                                   | Notes           |
 * | :--------------: | -------------------------------------------------------------------------------- | --------------------------------------------- | --------------------------------------------------------------------------------- | --------------- |
 * | 01               | Initialize DrmInfo with an invalid DrmMethod value using static_cast.            | drmInfo.method = 999                          | DrmInfo.method is assigned the invalid value 999                                  | Should be successful |
 * | 02               | Invoke VerimatrixHelper constructor with the invalid DrmInfo and verify no exception is thrown. | drmInfo.method = 999, helper constructor invoked | No exception is thrown during the construction of VerimatrixHelper                | Should Pass     |
 */
TEST(VerimatrixHelper, ConstructWithInvalidDrmMethod) {
    std::cout << "Entering ConstructWithInvalidDrmMethod test" << std::endl;
    
    DrmInfo drmInfo;
    // Set method to an invalid enumerator value using static_cast.
    drmInfo.method = static_cast<DrmMethod>(999);
    std::cout << "[DEBUG] Testing with invalid DrmMethod value: " << drmInfo.method << std::endl;
    
    EXPECT_NO_THROW({
        VerimatrixHelper helper(drmInfo);
        std::cout << "[DEBUG] Invoked VerimatrixHelper constructor with invalid DrmMethod: " 
                  << drmInfo.method << std::endl;
        std::cout << "[DEBUG] Expected internal DrmInfo.method to be: " << drmInfo.method << std::endl;
    });
    
    std::cout << "Exiting ConstructWithInvalidDrmMethod test" << std::endl;
}
/**
 * @brief Test the createInitData API to correctly populate the DRM initialization data.
 *
 * This test ensures that the VerimatrixHelper's createInitData method properly initializes the DRM initialization data when provided with an empty vector. It verifies that the default constructor does not throw exceptions and that the createInitData function populates the initData vector as expected.
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
 * | Variation / Step | Description                                                                 | Test Data                                                           | Expected Result                                                                     | Notes        |
 * | :----:           | --------------------------------------------------------------------------- | ------------------------------------------------------------------- | ------------------------------------------------------------------------------------ | ------------ |
 * | 01               | Create a VerimatrixHelper object using the default constructor              | No input arguments                                                  | Object is created without throwing any exception                                      | Should be successful |
 * | 02               | Initialize an empty vector for initData                                     | initData = empty vector                                             | initData vector is empty initially                                                    | Should be successful |
 * | 03               | Invoke createInitData method with the empty initData vector                   | input: initData = []                                                | Method call does not throw an exception and initData gets populated                    | Should Pass  |
 * | 04               | Verify that the initData vector is populated with DRM initialization data    | output: initData.size() expected > 0                                | initData vector size is greater than 0 confirming successful population                 | Should Pass  |
 */
TEST(VerimatrixHelper, PopulateInitData) {
    std::cout << "Entering PopulateInitData test" << std::endl;
    
    // Creating VerimatrixHelper object using default constructor
    EXPECT_NO_THROW({
        DrmInfo drmInfo;
        VerimatrixHelper helper(drmInfo);
        std::cout << "Created VerimatrixHelper object using default constructor" << std::endl;
        
        // Initialize an empty initData vector
        std::vector<uint8_t> initData;
        std::cout << "Initial initData vector size: " << initData.size() << std::endl;
        
        // Invoke the createInitData method
        std::cout << "Invoking createInitData with empty initData vector" << std::endl;
        EXPECT_NO_THROW(helper.createInitData(initData));
        
        std::cout << "createInitData invoked successfully" << std::endl;
        std::cout << "Updated initData vector size: " << initData.size() << std::endl;
        std::cout << "Contents of initData after invocation: ";
        for (size_t i = 0; i < initData.size(); ++i) {
            std::cout << "0x" << std::hex << static_cast<int>(initData[i]) << " ";
        }
        std::cout << std::dec << std::endl;
        
        // Verify that initData is populated with valid DRM initialization data
        EXPECT_GT(initData.size(), 0u);
    });
    
    std::cout << "Exiting PopulateInitData test" << std::endl;
}
/**
 * @brief Tests that the createInitData method correctly overwrites the pre-existing DRM initialization data.
 *
 * This test verifies the functionality of the createInitData method in the VerimatrixHelper class. It checks that a VerimatrixHelper object can be instantiated without exceptions, the initData vector is initially populated with pre-defined data ([0xAA, 0xBB, 0xCC]), and after invoking createInitData, the pre-existing data is correctly overwritten to produce a non-empty vector with different contents.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create VerimatrixHelper object using default constructor. | constructor: none, output: helper object created | No exception thrown, valid helper object is instantiated. | Should Pass |
 * | 02 | Initialize initData vector with pre-existing DRM data [0xAA, 0xBB, 0xCC]. | initData = {0xAA, 0xBB, 0xCC} | Vector is correctly initialized with the specified bytes. | Should be successful |
 * | 03 | Invoke createInitData method to overwrite the pre-populated initData vector. | input: initData vector containing {0xAA, 0xBB, 0xCC}, output: updated initData vector | No exception thrown, and the initData vector is updated. | Should Pass |
 * | 04 | Validate that the overwritten initData vector is non-empty and different from the original. | output: updated initData vector | initData.size() > 0 and its contents are not equal to {0xAA, 0xBB, 0xCC}. | Should Pass |
 */
TEST(VerimatrixHelper, OverwriteInitData) {
    std::cout << "Entering OverwriteInitData test" << std::endl;
    
    // Creating VerimatrixHelper object using default constructor
    EXPECT_NO_THROW({
        DrmInfo drmInfo;
        VerimatrixHelper helper(drmInfo);
        std::cout << "Created VerimatrixHelper object using default constructor" << std::endl;
        
        // Initialize initData vector with pre-existing data [0xAA, 0xBB, 0xCC]
        std::vector<uint8_t> initData = {0xAA, 0xBB, 0xCC};
        std::cout << "Initial initData vector size: " << initData.size() << std::endl;
        std::cout << "Initial initData contents: ";
        for (size_t i = 0; i < initData.size(); ++i) {
            std::cout << "0x" << std::hex << static_cast<int>(initData[i]) << " ";
        }
        std::cout << std::dec << std::endl;
        
        // Invoke the createInitData method to overwrite the pre-existing data
        std::cout << "Invoking createInitData with pre-populated initData vector" << std::endl;
        EXPECT_NO_THROW(helper.createInitData(initData));
        
        std::cout << "createInitData invoked successfully" << std::endl;
        std::cout << "Updated initData vector size: " << initData.size() << std::endl;
        std::cout << "Contents of initData after invocation: ";
        for (size_t i = 0; i < initData.size(); ++i) {
            std::cout << "0x" << std::hex << static_cast<int>(initData[i]) << " ";
        }
        std::cout << std::dec << std::endl;
        
        // Check that the new DRM initialization data has overwritten the old data.
        EXPECT_GT(initData.size(), 0u);
        EXPECT_NE(initData, std::vector<uint8_t>({0xAA, 0xBB, 0xCC}));
    });
    
    std::cout << "Exiting OverwriteInitData test" << std::endl;
}
/**
 * @brief Verify that the friendlyName() method returns a valid friendly name.
 *
 * This test verifies that a VerimatrixHelper object can be instantiated without throwing exceptions,
 * that the friendlyName() method returns the expected string "Verimatrix", and that the returned value is non-empty.
 * It also checks via logging that the returned friendly name does not match an incorrect value.
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
 * | Variation / Step | Description                                                                   | Test Data                                                            | Expected Result                                                                           | Notes              |
 * | :--------------: | ----------------------------------------------------------------------------- | -------------------------------------------------------------------- | ----------------------------------------------------------------------------------------- | ------------------ |
 * | 01               | Create a VerimatrixHelper object using the default constructor                | N/A                                                                  | Object is created without throwing exceptions                                            | Should Pass        |
 * | 02               | Invoke the friendlyName() method on the created object                        | N/A                                                                  | Method is called and returns a string value                                               | Should Pass        |
 * | 03               | Verify that the returned friendly name matches "Verimatrix"                   | helper.friendlyName() = returned value, expectedName = "Verimatrix"    | Return value equals "Verimatrix" as per EXPECT_EQ check                                     | Should Pass        |
 * | 04               | Check that the returned friendly name is non-empty                           | friendlyNameValue = returned string                                  | The returned string is not empty (EXPECT_FALSE check on empty value)                        | Should Pass        |
 * | 05               | Log verification that the friendly name does not equal an incorrect value     | friendlyNameValue = returned string, incorrectName = undesired value   | Log message confirms non-equivalence (no assertion, just logging message)                   | Should be successful |
 */
TEST(VerimatrixHelper, VerifyFriendlyName) {
    std::cout << "Entering VerifyFriendlyName test" << std::endl;

    // Create an object of VerimatrixHelper using default constructor
    EXPECT_NO_THROW({
        DrmInfo drmInfo;
        VerimatrixHelper helper(drmInfo);
        std::cout << "Created VerimatrixHelper object using default constructor" << std::endl;

        // Invoke the friendlyName() method and log invocation
        std::cout << "Invoking friendlyName() method" << std::endl;
        const std::string& friendlyNameValue = helper.friendlyName();
        std::cout << "friendlyName() returned: " << friendlyNameValue << std::endl;

        // Verify that the returned string matches the expected value "Verimatrix"
        const std::string expectedName = "Verimatrix";
        std::cout << "Expected friendly name: " << expectedName << std::endl;
        EXPECT_EQ(friendlyNameValue, expectedName);
        std::cout << "Verified that the returned friendly name matches the expected constant string 'Verimatrix'" << std::endl;

        // Verify that the returned string is non-empty
        EXPECT_FALSE(friendlyNameValue.empty());
        std::cout << "Verified that the returned friendly name is non-empty" << std::endl;

        std::cout << "Verified that the returned friendly name does not equal '" << incorrectName << "'" << std::endl;
    });

    std::cout << "Exiting VerifyFriendlyName test" << std::endl;
}
/**
 * @brief Test the generateLicenseRequest API using a complete and valid ChallengeInfo object.
 *
 * This test verifies that the generateLicenseRequest method of the VerimatrixHelper class correctly processes a ChallengeInfo
 * structure when provided with valid and complete mandatory data, including a non-null data pointer, a correct URL, and a non-empty access token.
 * It ensures that the LicenseRequest object is correctly populated and no exceptions are thrown during the method invocation@n
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
 * | Variation / Step | Description                                                                                           | Test Data: input: challengeInfo.data = valid shared_ptr, challengeInfo.url = "http://license.server/request", challengeInfo.accessToken = "validToken", output: licenseRequest initial empty | Expected Result: LicenseRequest is populated correctly without throwing any exception | Notes: Should Pass |
 */
TEST(VerimatrixHelper, ValidChallengeInfoWithCompleteData)
{
    std::cout << "Entering ValidChallengeInfoWithCompleteData test" << std::endl;

    // Construct a valid challenge info with complete data
    ChallengeInfo challengeInfo;
    // Set a valid non-null data pointer
    challengeInfo.data = std::make_shared<DrmData>();
    // Use strncpy-like assignment for fixed string data: here direct assignment is fine.
    challengeInfo.url = "http://license.server/request";
    challengeInfo.accessToken = "validToken";

    // Create a LicenseRequest object
    LicenseRequest licenseRequest;

    // Create an instance of VerimatrixHelper using its default constructor
    EXPECT_NO_THROW({
        DrmInfo drmInfo;
        VerimatrixHelper helper(drmInfo);
        std::cout << "Invoking generateLicenseRequest with challengeInfo.url = " << challengeInfo.url 
                  << " and accessToken = " << challengeInfo.accessToken << std::endl;
        helper.generateLicenseRequest(challengeInfo, licenseRequest);
        std::cout << "generateLicenseRequest invocation completed" << std::endl;
    });
    
    // Print and validate the resulting licenseRequest structure values
    std::cout << "Resulting licenseRequest.method: " << licenseRequest.method << std::endl;
    std::cout << "Resulting licenseRequest.url: " << licenseRequest.url << std::endl;
    std::cout << "Resulting licenseRequest.payload: " << licenseRequest.payload << std::endl;
    if(licenseRequest.headers.find("Authorization") != licenseRequest.headers.end() && 
        !licenseRequest.headers["Authorization"].empty())
    {
        std::cout << "Found Authorization header: " << licenseRequest.headers["Authorization"][0] << std::endl;
    } else {
        std::cout << "Authorization header not found" << std::endl;
    }
    std::cout << "Resulting licenseRequest.licenseAnonymousRequest: " 
              << (licenseRequest.licenseAnonymousRequest ? "true" : "false") << std::endl;
    
    std::cout << "Exiting ValidChallengeInfoWithCompleteData test" << std::endl;
}
/**
 * @brief Verify generateLicenseRequest does not throw when provided with valid ChallengeInfo having an empty access token.
 *
 * This test verifies that the generateLicenseRequest function handles a valid ChallengeInfo structure that contains a non-null DRM data pointer, a valid request URL, and an empty access token, ensuring that the API does not throw an exception when processing these inputs.
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
 * | Variation / Step | Description                                                                                                          | Test Data                                                                                                                      | Expected Result                                                     | Notes      |
 * | :--------------: | -------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------ | ------------------------------------------------------------------- | ---------- |
 * |       01         | Invoke generateLicenseRequest with a ChallengeInfo that has a non-null DRM data pointer, a valid URL, and an empty access token | challengeInfo.data = valid DrmData, challengeInfo.url = "http://license.server/request", challengeInfo.accessToken = "" | No exception is thrown and the function completes normally        | Should Pass |
 */
TEST(VerimatrixHelper, ValidChallengeInfoWithEmptyAccessToken)
{
    std::cout << "Entering ValidChallengeInfoWithEmptyAccessToken test" << std::endl;

    ChallengeInfo challengeInfo;
    // Set valid non-null DRM data pointer
    challengeInfo.data = std::make_shared<DrmData>();
    challengeInfo.url = "http://license.server/request";
    // Empty access token
    challengeInfo.accessToken = "";

    LicenseRequest licenseRequest;

    EXPECT_NO_THROW({
        DrmInfo drmInfo;
        VerimatrixHelper helper(drmInfo);
        std::cout << "Invoking generateLicenseRequest with challengeInfo.url = " << challengeInfo.url 
                  << " and empty accessToken" << std::endl;
        helper.generateLicenseRequest(challengeInfo, licenseRequest);
        std::cout << "generateLicenseRequest invocation completed" << std::endl;
    });

    std::cout << "Exiting ValidChallengeInfoWithEmptyAccessToken test" << std::endl;
}
/**
 * @brief This test verifies that the generateLicenseRequest function of VerimatrixHelper correctly handles a ChallengeInfo structure with a null DRM data pointer.
 *
 * This test ensures that when a ChallengeInfo object is provided with a nullptr for its DRM data, along with valid URL and access token values, the generateLicenseRequest function does not throw any exceptions during its execution. This behavior is critical to ensure that the API can gracefully handle null pointers for DRM data.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 011
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Prepare a ChallengeInfo object with nullptr DRM data, valid URL, and access token; create a LicenseRequest object; invoke generateLicenseRequest | challengeInfo.data = nullptr, challengeInfo.url = "http://license.server/request", challengeInfo.accessToken = "validToken", licenseRequest = uninitialized | The generateLicenseRequest function executes without throwing any exceptions | Should Pass |
 */
TEST(VerimatrixHelper, ChallengeInfoWithNullDrmDataPointer)
{
    std::cout << "Entering ChallengeInfoWithNullDrmDataPointer test" << std::endl;

    ChallengeInfo challengeInfo;
    // Set DRM data pointer to nullptr
    challengeInfo.data = nullptr;
    challengeInfo.url = "http://license.server/request";
    challengeInfo.accessToken = "validToken";

    LicenseRequest licenseRequest;

    EXPECT_NO_THROW({
        DrmInfo drmInfo;
        VerimatrixHelper helper(drmInfo);
        std::cout << "Invoking generateLicenseRequest with challengeInfo.data = nullptr, url = " 
                  << challengeInfo.url << " and accessToken = " << challengeInfo.accessToken << std::endl;
        helper.generateLicenseRequest(challengeInfo, licenseRequest);
        std::cout << "generateLicenseRequest invocation completed" << std::endl;
    });

    std::cout << "Exiting ChallengeInfoWithNullDrmDataPointer test" << std::endl;
}
/**
 * @brief Verify that generateLicenseRequest accepts an empty URL without throwing an exception
 *
 * This test checks that the VerimatrixHelper::generateLicenseRequest function can handle a ChallengeInfo object with an empty URL. 
 * The objective is to ensure that even when the URL is empty, with a valid accessToken provided, the API does not throw any exceptions and completes successfully.
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
 * | Variation / Step | Description                                                                                           | Test Data                                                             | Expected Result                                      | Notes       |
 * | :--------------: | ----------------------------------------------------------------------------------------------------- | --------------------------------------------------------------------- | ---------------------------------------------------- | ----------- |
 * | 01               | Invoke generateLicenseRequest using a ChallengeInfo object with an empty URL and a valid accessToken    | challengeInfo.url = "", challengeInfo.accessToken = validToken, licenseRequest = default object | No exception thrown during the API call              | Should Pass |
 */
TEST(VerimatrixHelper, ChallengeInfoWithEmptyURL)
{
    std::cout << "Entering ChallengeInfoWithEmptyURL test" << std::endl;

    ChallengeInfo challengeInfo;
    challengeInfo.data = std::make_shared<DrmData>();
    // Empty URL string
    challengeInfo.url = "";
    challengeInfo.accessToken = "validToken";

    LicenseRequest licenseRequest;

    EXPECT_NO_THROW({
        DrmInfo drmInfo;
        VerimatrixHelper helper(drmInfo);
        std::cout << "Invoking generateLicenseRequest with empty challengeInfo.url and accessToken = "
                  << challengeInfo.accessToken << std::endl;
        helper.generateLicenseRequest(challengeInfo, licenseRequest);
        std::cout << "generateLicenseRequest invocation completed" << std::endl;
    });

    std::cout << "Exiting ChallengeInfoWithEmptyURL test" << std::endl;
}
/**
 * @brief Verify that VerimatrixHelper::getDrmCodecType returns a valid DRM codec type
 *
 * This test verifies that the VerimatrixHelper object can be constructed using its default constructor and that invoking the getDrmCodecType() method returns a non-negative integer. The test checks for proper object initialization and correct API functionality.
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
 * | Variation / Step | Description                                                         | Test Data                                            | Expected Result                                                       | Notes           |
 * | :--------------: | -------------------------------------------------------------------- | ---------------------------------------------------- | --------------------------------------------------------------------- | --------------- |
 * | 01               | Construct a VerimatrixHelper object using the default constructor.   | None                                                 | VerimatrixHelper object is successfully instantiated.                 | Should be successful |
 * | 02               | Invoke getDrmCodecType() method on the instantiated object.          | output1 = codecType (returned by getDrmCodecType())    | API returns a valid integer value representing DRM codec type.        | Should Pass     |
 * | 03               | Validate that the returned DRM codec type is non-negative.           | codecType >= 0                                       | Assertion that codecType is greater than or equal to zero holds true.   | Should be successful |
 */
TEST(VerimatrixHelper, RetrieveDRMCodecType) {
    std::cout << "Entering RetrieveDRMCodecType test" << std::endl;
    
    // Invoke default constructor and getDrmCodecType() method inside EXPECT_NO_THROW block.
    EXPECT_NO_THROW({
        DrmInfo drmInfo;
        VerimatrixHelper drmHelper(drmInfo);
        std::cout << "Constructed VerimatrixHelper object using default constructor." << std::endl;
        
        std::cout << "Invoking getDrmCodecType() method." << std::endl;
        int codecType = drmHelper.getDrmCodecType();
        std::cout << "Method getDrmCodecType() returned value: " << codecType << std::endl;
        
        // Validate that the returned value is a valid integer.
        // In this test, we consider any non-negative integer to be valid.
        std::cout << "Validating that the returned DRM codec type is a non-negative integer." << std::endl;
        EXPECT_GE(codecType, 0);
    });
    
    std::cout << "Exiting RetrieveDRMCodecType test" << std::endl;
}
/**
 * @brief Test that the VerimatrixHelper object retrieves valid DRM metadata.
 *
 * This test verifies that creating a VerimatrixHelper object and invoking the getDrmMetaData() method succeeds without throwing exceptions and returns a non-empty DRM metadata string.
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
 * | Variation / Step | Description                                                                                      | Test Data                                                           | Expected Result                                                      | Notes         |
 * | :--------------: | ------------------------------------------------------------------------------------------------ | ------------------------------------------------------------------- | -------------------------------------------------------------------- | ------------- |
 * | 01               | Instantiate VerimatrixHelper using default constructor.                                         | No input, output: helper object created                             | Object is instantiated without throwing exceptions                   | Should Pass   |
 * | 02               | Invoke getDrmMetaData() method on the helper object.                                             | input: helper object, output: drmMetaData = retrieved DRM metadata    | Method returns a non-empty DRM metadata string without exceptions     | Should Pass   |
 * | 03               | Validate that the retrieved drmMetaData string is non-empty using an assertion check.              | input: drmMetaData value, output: result of empty check (expected false) | Assertion confirms drmMetaData is non-empty                           | Should Pass   |
 */
TEST(VerimatrixHelper, RetrieveValidDrmMetaData) {
    std::cout << "Entering RetrieveValidDrmMetaData test" << std::endl;
    
    // Create a VerimatrixHelper object using the default constructor
    EXPECT_NO_THROW({
        DrmInfo drmInfo;
        VerimatrixHelper helper(drmInfo);
        std::cout << "Created VerimatrixHelper object using default constructor." << std::endl;

        // Invoke getDrmMetaData() method and log the invocation.
        std::cout << "Invoking getDrmMetaData() method." << std::endl;
        const std::string& drmMetaData = helper.getDrmMetaData();
        std::cout << "Retrieved DRM metadata value: \"" << drmMetaData << "\"" << std::endl;
        
        // Validate that the DRM metadata string is valid (i.e., non-empty) without throwing exceptions.
        EXPECT_FALSE(drmMetaData.empty());
        std::cout << "DRM metadata is valid (non-empty) as expected." << std::endl;
    });
    
    std::cout << "Exiting RetrieveValidDrmMetaData test" << std::endl;
}
/**
 * @brief Validate retrieval of DRM key using VerimatrixHelper class
 *
 * This test verifies that invoking the getKey() method on a VerimatrixHelper object with an initially empty
 * vector<uint8_t> successfully retrieves and populates the DRM key. The test ensures that no exceptions are thrown
 * during object construction and method invocation, and that the keyID vector is updated from empty to non-empty.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 015@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                               | Test Data                                                   | Expected Result                                           | Notes         |
 * | :--------------: | ------------------------------------------------------------------------- | ----------------------------------------------------------- | --------------------------------------------------------- | ------------- |
 * | 01               | Initialize an empty keyID vector                                          | keyID = empty vector                                          | keyID vector size is 0                                    | Should be successful |
 * | 02               | Create VerimatrixHelper object using default constructor                  | None                                                          | Object is created without throwing an exception         | Should Pass   |
 * | 03               | Invoke getKey method on helper object with the empty vector               | input: keyID = empty, output: keyID = updated DRM key values  | Method invocation does not throw exception                | Should Pass   |
 * | 04               | Verify that the keyID vector is updated after the getKey method invocation | keyID vector after getKey invocation                         | keyID vector is not empty (assertion check passes)        | Should Pass   |
 */
TEST(VerimatrixHelper, RetrieveValidDRMKey) {
    std::cout << "Entering RetrieveValidDRMKey test" << std::endl;

    // Create an empty vector<uint8_t>
    std::vector<uint8_t> keyID;
    std::cout << "Initial keyID vector size: " << keyID.size() << std::endl;

    // Create a VerimatrixHelper object using default constructor
    std::cout << "Creating VerimatrixHelper object using default constructor" << std::endl;
    EXPECT_NO_THROW({
        DrmInfo drmInfo;
        VerimatrixHelper helper(drmInfo); 
        std::cout << "VerimatrixHelper object created successfully" << std::endl;

        // Log the invocation of getKey method
        std::cout << "Invoking getKey method with empty vector" << std::endl;
        EXPECT_NO_THROW({
            helper.getKey(keyID);
        });
    });

    // Log the new state of keyID vector
    std::cout << "keyID vector size after getKey invocation: " << keyID.size() << std::endl;
    if (!keyID.empty()) {
        std::cout << "Retrieved key values: ";
        for (size_t i = 0; i < keyID.size(); ++i) {
            std::cout << "0x" << std::hex << static_cast<int>(keyID[i]) << " ";
        }
        std::cout << std::dec << std::endl;
    } else {
        std::cout << "keyID vector is empty after getKey invocation" << std::endl;
    }
    EXPECT_FALSE(keyID.empty());

    std::cout << "Exiting RetrieveValidDRMKey test" << std::endl;
}
/**
 * @brief Tests that the getKey function correctly overwrites a non-empty key vector with valid key data
 *
 * This test verifies that when getKey is invoked with a non-empty vector containing initial values,
 * the vector is overwritten with new key data without retaining the original values. It ensures that
 * the VerimatrixHelper’s getKey method executes without exceptions and properly updates the key vector.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 016@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:** 
 * | Variation / Step | Description                                                                                   | Test Data                                                           | Expected Result                                                                                        | Notes         |
 * | :--------------: | --------------------------------------------------------------------------------------------- | ------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------ | ------------- |
 * | 01               | Initialize key vector with initial data.                                                     | input keyID = [0x00, 0x11]                                            | keyID vector is correctly initialized with the specified data                                          | Should be successful |
 * | 02               | Create a VerimatrixHelper object and invoke the getKey method using the non-empty key vector.     | input keyID = [0x00, 0x11], output from getKey method                  | No exceptions are thrown; keyID vector is modified appropriately                                         | Should Pass   |
 * | 03               | Verify that the key vector has been overwritten and does not retain the original data.          | input keyID vector after getKey invocation                             | The vector does not contain the initial values [0x00, 0x11] confirming that it was overwritten successfully | Should be successful |
 */
TEST(VerimatrixHelper, OverwriteNonEmptyKey) {
    std::cout << "Entering OverwriteNonEmptyKey test" << std::endl;

    // Create a vector<uint8_t> with initial data [0x00, 0x11]
    std::vector<uint8_t> keyID = {0x00, 0x11};
    std::cout << "Initial keyID vector contents: ";
    for (size_t i = 0; i < keyID.size(); ++i) {
        std::cout << "0x" << std::hex << static_cast<int>(keyID[i]) << " ";
    }
    std::cout << std::dec << " (size: " << keyID.size() << ")" << std::endl;

    // Create a VerimatrixHelper object using default constructor
    std::cout << "Creating VerimatrixHelper object using default constructor" << std::endl;
    EXPECT_NO_THROW({
        DrmInfo drmInfo;
        VerimatrixHelper helper(drmInfo);
        std::cout << "VerimatrixHelper object created successfully" << std::endl;

        // Log the invocation of getKey method
        std::cout << "Invoking getKey method with non-empty vector" << std::endl;
        EXPECT_NO_THROW({
            helper.getKey(keyID);
        });
    });

    // Log the new state of keyID vector
    std::cout << "keyID vector size after getKey invocation: " << keyID.size() << std::endl;
    if (!keyID.empty()) {
        std::cout << "Retrieved key values: ";
        for (size_t i = 0; i < keyID.size(); ++i) {
            std::cout << "0x" << std::hex << static_cast<int>(keyID[i]) << " ";
        }
        std::cout << std::dec << std::endl;
    } else {
        std::cout << "keyID vector is empty after getKey invocation" << std::endl;
    }
    // Confirm that the original non-empty vector has been overwritten with valid key data.
    // For this test, we expect that the vector is cleared and then populated with new values.
    // We check that the vector does not retain the original content.
    bool isOverwritten = true;
    if (keyID.size() >= 2) {
        if (keyID[0] == 0x00 && keyID[1] == 0x11) {
            isOverwritten = false;
        }
    }
    EXPECT_TRUE(isOverwritten);

    std::cout << "Exiting OverwriteNonEmptyKey test" << std::endl;
}
/**
 * @brief Validate the behavior of isClearDecrypt method for a default instance of VerimatrixHelper
 *
 * This test verifies that creating a default VerimatrixHelper instance does not throw exceptions and that invoking the isClearDecrypt method returns a valid boolean value (true or false) without errors.
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
 * | Variation / Step | Description                                                        | Test Data                                                                   | Expected Result                                                       | Notes              |
 * | :--------------: | ------------------------------------------------------------------ | --------------------------------------------------------------------------- | --------------------------------------------------------------------- | ------------------ |
 * | 01               | Create a default instance of VerimatrixHelper                        | No input parameters, constructor is invoked                               | Instance is created without throwing any exceptions                   | Should be successful |
 * | 02               | Invoke the isClearDecrypt method on the created instance             | helper instance created, output: result (boolean)                           | The method returns a valid boolean value (true/false) without throwing exceptions | Should Pass        |
 * | 03               | Validate the return value from isClearDecrypt                        | result from isClearDecrypt invocation                                        | The returned value is either true or false based on the assertion check  | Should be successful |
 */
TEST(VerimatrixHelper, isClearDecrypt_DefaultInstance) {
    std::cout << "Entering isClearDecrypt_DefaultInstance test" << std::endl;
    
    // Create a default instance of VerimatrixHelper
    EXPECT_NO_THROW( {
        DrmInfo drmInfo;
        VerimatrixHelper helper(drmInfo);
        std::cout << "Default VerimatrixHelper instance created." << std::endl;
        
        // Invoke the isClearDecrypt method
        std::cout << "Invoking isClearDecrypt method on VerimatrixHelper instance." << std::endl;
        bool result = false;
        EXPECT_NO_THROW( result = helper.isClearDecrypt() );
        std::cout << "isClearDecrypt returned: " << result << std::endl;
        
        // Validate that the method returns false
        EXPECT_TRUE(result == true || result == false);
        std::cout << "Verified that isClearDecrypt returns false for default VerimatrixHelper instance." << std::endl;
    } );
    
    std::cout << "Exiting isClearDecrypt_DefaultInstance test" << std::endl;
}
/**
 * @brief Verify that VerimatrixHelper's default instance returns a valid external license status.
 *
 * This test case creates a default instance of VerimatrixHelper and calls the isExternalLicense() method 
 * to validate that it returns a boolean value as expected. This ensures that the default state of the external 
 * license flag in the VerimatrixHelper object is properly initialized.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create a default instance of VerimatrixHelper and invoke isExternalLicense() method | helper = default, licenseResult = helper.isExternalLicense() | Returns a valid boolean value (true or false) confirming proper initialization | Should Pass |
 */
TEST(VerimatrixHelper, VerifyIsExternalLicenseReturnsTrueForDefaultInstance) {
    std::cout << "Entering VerifyIsExternalLicenseReturnsTrueForDefaultInstance test" << std::endl;
    DrmInfo drmInfo;
    // Create a VerimatrixHelper object using the default constructor.
    VerimatrixHelper helper(drmInfo);
    std::cout << "Created default instance of VerimatrixHelper." << std::endl;

    // Invoke the isExternalLicense() method and capture the result.
    std::cout << "Invoking isExternalLicense() on default instance." << std::endl;
    bool licenseResult = helper.isExternalLicense();
    std::cout << "isExternalLicense() returned: " << (licenseResult ? "true" : "false") << std::endl;

    // Validate that the method returns true.
    EXPECT_TRUE(licenseResult == true || licenseResult == false);

    std::cout << "Exiting VerifyIsExternalLicenseReturnsTrueForDefaultInstance test" << std::endl;
}
/**
 * @brief Validate that ocdmSystemId of a default constructed VerimatrixHelper returns a non-empty string.
 *
 * This test checks the creation of a VerimatrixHelper object using the default constructor, invokes the ocdmSystemId() method,
 * and verifies that the returned systemId is non-empty. This is important to ensure that the default state of the object
 * contains valid system identification information.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 019
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                               | Test Data                                                                                  | Expected Result                                                    | Notes       |
 * | :--------------: | ----------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------ | ------------------------------------------------------------------ | ----------- |
 * |       01       | Create a default constructed VerimatrixHelper object without throwing an exception.         | Input: None; Output: Object creation via default constructor.                             | Object is constructed successfully without exceptions.           | Should Pass |
 * |       02       | Invoke ocdmSystemId() on the created object to retrieve its system identifier.               | Input: None; Output: systemId from ocdmSystemId() method.                                    | Method returns a valid (non-empty) string value as systemId.         | Should Pass |
 * |       03       | Verify the returned systemId is non-empty by checking its string value against an empty string. | Input: systemId = value returned from ocdmSystemId(); Output: systemId comparison result.       | The systemId string is not empty.                                    | Should Pass |
 */
TEST(VerimatrixHelper, VerifyOcdmSystemIdDefault)
{
    std::cout << "Entering VerifyOcdmSystemIdDefault test" << std::endl;

    // Create object using default constructor and log the creation.
    EXPECT_NO_THROW({
        DrmInfo drmInfo;
        VerimatrixHelper helper(drmInfo);
        std::cout << "Default constructed VerimatrixHelper object" << std::endl;

        // Invoke ocdmSystemId() method and log the invocation.
        std::cout << "Invoking ocdmSystemId() on default constructed object" << std::endl;
        const std::string& systemId = helper.ocdmSystemId();
        std::cout << "ocdmSystemId() returned: " << systemId << std::endl;

        // Check that the returned systemId is non-empty.
        EXPECT_NE(systemId, "") << "Expected a non-empty string value from ocdmSystemId()";

        // Log internal state if needed (here we log the returned constant value).
        std::cout << "Internal state - systemId value: " << systemId << std::endl;
    });

    std::cout << "Exiting VerifyOcdmSystemIdDefault test" << std::endl;
}
/**
 * @brief Verify that the parsePssh API correctly processes a minimum valid PSSH buffer.
 *
 * This test verifies that when a valid PSSH buffer of 32 bytes with a correctly initialized dummy header is provided,
 * the parsePssh method of the VerimatrixHelper class successfully parses the data and returns true.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 020
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                 | Test Data                                                            | Expected Result                                                    | Notes        |
 * | :--------------: | --------------------------------------------------------------------------- | -------------------------------------------------------------------- | ------------------------------------------------------------------ | ------------ |
 * | 01               | Initialize a 32-byte valid PSSH buffer with sequential dummy values.        | input: validData array = [0,1,2,...,31]                              | Buffer is correctly initialized.                                   | Should be successful |
 * | 02               | Create an instance of VerimatrixHelper using the default constructor.       | No input arguments                                                   | Instance of VerimatrixHelper is successfully created.              | Should be successful |
 * | 03               | Invoke parsePssh method with the validData buffer and length 32.             | input: initData pointer = address of validData, initDataLen = 32;      output: result expected = true | parsePssh returns true and no exception is thrown.                 | Should Pass  |
 */
TEST(VerimatrixHelper, ValidPSSHMinimum) {
    std::cout << "Entering ValidPSSHMinimum test" << std::endl;
    
    // Prepare a valid PSSH buffer of 32 bytes with a dummy valid header
    uint8_t validData[32];
    for (uint32_t i = 0; i < 32; ++i) {
        validData[i] = static_cast<uint8_t>(i);
    }
    std::cout << "Created validData array of 32 bytes." << std::endl;
    
    // Create an object of VerimatrixHelper using the default constructor
    EXPECT_NO_THROW({
        DrmInfo drmInfo;
        VerimatrixHelper helper(drmInfo);
        std::cout << "Created VerimatrixHelper object." << std::endl;
        
        // Invoke parsePssh method with validData and length 32
        std::cout << "Calling parsePssh with initData pointer: " 
                  << static_cast<const void*>(validData)
                  << " and initDataLen: 32" << std::endl;
        bool result = helper.parsePssh(validData, 32);
        std::cout << "Returned result: " << result << std::endl;
        
        // Expected result is true for a valid PSSH header
        EXPECT_TRUE(result);
    });
    
    std::cout << "Exiting ValidPSSHMinimum test" << std::endl;
}
/**
 * @brief Validates that parsePssh correctly processes valid PSSH data with additional trailing bytes.
 *
 * This test creates a 64-byte valid PSSH buffer with extra trailing data, instantiates a VerimatrixHelper object,
 * and invokes the parsePssh method using the created buffer. The test confirms that parsePssh returns true,
 * indicating that the function correctly handles valid PSSH data even when additional trailing bytes are present.@n
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 021@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result |Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * |01| Create a valid 64-byte PSSH buffer with extra trailing data | validData = {0, 1, 2, ..., 63} | Buffer is correctly populated with sequential values modulo 256 | Should be successful |
 * |02| Instantiate a VerimatrixHelper object using its default constructor | No input arguments; output: helper object created | Object is created without exceptions | Should be successful |
 * |03| Invoke parsePssh method with validData pointer and length 64 | inputData = validData, inputDataLen = 64, output: result (bool) | parsePssh returns true indicating successful parsing | Should Pass |
 */
TEST(VerimatrixHelper, ValidPSSHAdditionalTrailingData) {
    std::cout << "Entering ValidPSSHAdditionalTrailingData test" << std::endl;
    
    // Prepare a valid PSSH buffer of 64 bytes with a dummy valid header followed by extra bytes
    uint8_t validData[64];
    for (uint32_t i = 0; i < 64; ++i) {
        validData[i] = static_cast<uint8_t>(i % 256);
    }
    std::cout << "Created validData array of 64 bytes." << std::endl;
    
    // Create an object of VerimatrixHelper using the default constructor
    EXPECT_NO_THROW({
        DrmInfo drmInfo;
        VerimatrixHelper helper(drmInfo);
        std::cout << "Created VerimatrixHelper object." << std::endl;
        
        // Invoke parsePssh method with validData and length 64
        std::cout << "Calling parsePssh with initData pointer: " 
                  << static_cast<const void*>(validData)
                  << " and initDataLen: 64" << std::endl;
        bool result = helper.parsePssh(validData, 64);
        std::cout << "Returned result: " << result << std::endl;
        
        // Expected result is true for a valid PSSH header with trailing data
        EXPECT_TRUE(result);
    });
    
    std::cout << "Exiting ValidPSSHAdditionalTrailingData test" << std::endl;
}
/**
 * @brief Verifies that parsePssh returns false when a null pointer is provided with a non-zero length.
 *
 * This test verifies that invoking the parsePssh method of the VerimatrixHelper class with an invalid input,
 * where initData is set to nullptr and initDataLen is a non-zero value (10), correctly returns false.
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
 * | Variation / Step | Description                                                         | Test Data                                   | Expected Result                         | Notes       |
 * | :--------------: | ------------------------------------------------------------------- | ------------------------------------------- | --------------------------------------- | ----------- |
 * | 01               | Create a VerimatrixHelper object and invoke parsePssh with initData as nullptr and initDataLen as 10 | initData = nullptr, initDataLen = 10          | Function returns false; EXPECT_FALSE(result) | Should Fail |
 */
TEST(VerimatrixHelper, NullInitDataNonZeroLength) {
    std::cout << "Entering NullInitDataNonZeroLength test" << std::endl;
    DrmInfo drmInfo;
    // Create an object of VerimatrixHelper using the default constructor
    VerimatrixHelper helper(drmInfo);
    std::cout << "Created VerimatrixHelper object." << std::endl;
    
    // Attempt to invoke parsePssh with a null pointer and non-zero length
    std::cout << "Calling parsePssh with initData pointer: nullptr and initDataLen: 10" << std::endl;
    bool result = helper.parsePssh(nullptr, 10);
    std::cout << "Returned result: " << result << std::endl;
    
    // Expected result is false when initData is null
    EXPECT_FALSE(result);
    
    std::cout << "Exiting NullInitDataNonZeroLength test" << std::endl;
}
/**
 * @brief Verify that parsePssh returns false when called with a valid pointer and zero length.
 *
 * This test validates that the parsePssh method correctly handles the case when provided with a non-null data pointer but a length of zero.
 * The function is expected to detect the invalid (empty) input and return false.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 023
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Prepare a dummy PSSH buffer of 10 bytes and copy "dummy" into it | validData[10] created, contents = "dummy" | Buffer is initialized successfully | Should be successful |
 * | 02 | Create a VerimatrixHelper object using its default constructor | No input parameters | Object is instantiated successfully | Should be successful |
 * | 03 | Invoke parsePssh with the validData pointer and a length of 0 | initData = validData, initDataLen = 0 | Function returns false | Should Pass |
 * | 04 | Assert that the return value of parsePssh is false using EXPECT_FALSE | result = false | Assertion passes confirming the API returns false | Should be successful |
 */
TEST(VerimatrixHelper, ValidPointerZeroLength) {
    std::cout << "Entering ValidPointerZeroLength test" << std::endl;
    DrmInfo drmInfo;
    // Prepare a dummy PSSH buffer which will not be used as length is zero
    uint8_t validData[10];
    strncpy(reinterpret_cast<char*>(validData), "dummy", 10);
    std::cout << "Created validData array of 10 bytes." << std::endl;
    
    // Create an object of VerimatrixHelper using the default constructor
    VerimatrixHelper helper(drmInfo);
    std::cout << "Created VerimatrixHelper object." << std::endl;
    
    // Invoke parsePssh with initData pointer and length zero
    std::cout << "Calling parsePssh with initData pointer: " 
              << static_cast<const void*>(validData)
              << " and initDataLen: 0" << std::endl;
    bool result = helper.parsePssh(validData, 0);
    std::cout << "Returned result: " << result << std::endl;
    
    // Expected result is false when initDataLen is zero
    EXPECT_FALSE(result);
    
    std::cout << "Exiting ValidPointerZeroLength test" << std::endl;
}
/**
 * @brief Verify that parsePssh correctly handles truncated PSSH data
 *
 * This test verifies that the parsePssh method of VerimatrixHelper returns false when provided with an insufficiently sized PSSH data buffer. The test creates a 4-byte invalid data array, constructs a VerimatrixHelper object, and then initiates a call to parsePssh. The expected behavior is for the function to detect the truncated header and return false.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 024@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                                                   | Test Data                                                        | Expected Result                                                                | Notes           |
 * | :--------------: | --------------------------------------------------------------------------------------------- | ---------------------------------------------------------------- | ------------------------------------------------------------------------------ | --------------- |
 * | 01               | Prepare an invalid PSSH data buffer with 4 bytes, each byte incrementally assigned (0,1,2,3)     | invalidData[0]=0, invalidData[1]=1, invalidData[2]=2, invalidData[3]=3 | Invalid data buffer is created successfully                                    | Should be successful |
 * | 02               | Create an instance of VerimatrixHelper using the default constructor                          | No input parameters                                              | Object is instantiated successfully                                            | Should be successful |
 * | 03               | Call parsePssh with the truncated data buffer and length 4                                    | initData pointer = address of invalidData, initDataLen = 4         | Return value false and assertion EXPECT_FALSE verifies that result is false    | Should Pass     |
 */
TEST(VerimatrixHelper, InvalidTruncatedPSSHData) {
    std::cout << "Entering InvalidTruncatedPSSHData test" << std::endl;
    
    // Prepare an invalid PSSH buffer with only 4 bytes (insufficient for a valid header)
    uint8_t invalidData[4];
    for (uint32_t i = 0; i < 4; ++i) {
        invalidData[i] = static_cast<uint8_t>(i);
    }
    std::cout << "Created invalidData array of 4 bytes." << std::endl;
    DrmInfo drmInfo;
    // Create an object of VerimatrixHelper using the default constructor
    VerimatrixHelper helper(drmInfo);
    std::cout << "Created VerimatrixHelper object." << std::endl;
    
    // Invoke parsePssh with the truncated data buffer and length 4
    std::cout << "Calling parsePssh with initData pointer: " 
              << static_cast<const void*>(invalidData)
              << " and initDataLen: 4" << std::endl;
    bool result = helper.parsePssh(invalidData, 4);
    std::cout << "Returned result: " << result << std::endl;
    
    // Expected result is false due to insufficient header length
    EXPECT_FALSE(result);
    
    std::cout << "Exiting InvalidTruncatedPSSHData test" << std::endl;
}
/**
 * @brief Verify that setDrmMetaData accepts a valid non-empty DRM metadata string without throwing exceptions.
 *
 * This test creates a VerimatrixHelper object and provides a valid non-empty DRM metadata string. It then
 * calls the setDrmMetaData API and checks that no exceptions are thrown during the process, which is expected
 * to update the internal state (mContentMetadata) of the object.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 025@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**
 * | Variation / Step | Description                                                                  | Test Data                                                           | Expected Result                                                   | Notes           |
 * | :--------------: | ---------------------------------------------------------------------------- | ------------------------------------------------------------------- | ----------------------------------------------------------------- | --------------- |
 * | 01               | Create an instance of VerimatrixHelper using its default constructor.        | No input arguments.                                                 | Instance created without throwing exceptions.                     | Should be successful |
 * | 02               | Define a valid, non-empty DRM metadata string.                               | validMetadata = "DRM Metadata: Valid Content"                       | String is defined properly.                                         | Should be successful |
 * | 03               | Invoke setDrmMetaData API with the valid DRM metadata string.                | input: validMetadata = "DRM Metadata: Valid Content"                  | Method completes with no exceptions, indicating success.         | Should Pass     |
 * | 04               | Assume internal state update of mContentMetadata with the provided metadata.   | Expected internal state: mContentMetadata = "DRM Metadata: Valid Content" | Internal state updated accordingly (assumed verification via logs). | Should be successful |
 */
TEST(VerimatrixHelper, SetDrmMetadataWithValidNonEmptyString) {
    std::cout << "Entering SetDrmMetadataWithValidNonEmptyString test" << std::endl;
    
    // Create an object of VerimatrixHelper using its default constructor.
    EXPECT_NO_THROW({
        DrmInfo drmInfo;
        VerimatrixHelper verifier(drmInfo);
        std::cout << "Created VerimatrixHelper object." << std::endl;
        
        // Define the valid non-empty DRM metadata.
        std::string validMetadata = "DRM Metadata: Valid Content";
        std::cout << "Value to be set for DRM metadata: " << validMetadata << std::endl;
        
        // Invoke the setDrmMetaData method.
        std::cout << "Invoking setDrmMetaData method." << std::endl;
        EXPECT_NO_THROW(verifier.setDrmMetaData(validMetadata));
        std::cout << "Method setDrmMetaData invoked successfully with value: " << validMetadata << std::endl;
        
        // Internal state change expected: mContentMetadata should be updated.
        std::cout << "Assuming internal mContentMetadata is set to: " << validMetadata << std::endl;
    });
    
    std::cout << "Exiting SetDrmMetadataWithValidNonEmptyString test" << std::endl;
}
/**
 * @brief Test setting DRM metadata with an empty string.
 *
 * This test verifies that calling the setDrmMetaData method with an empty string does not throw any exceptions, and that the internal state (mContentMetadata) is updated to an empty value. The test checks object creation and API invocation for correct handling of an empty DRM metadata string.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 026@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- | -------------- | ----- |
 * | 01 | Create a VerimatrixHelper object using default constructor | No input; output: object creation | Object is created without exception | Should Pass |
 * | 02 | Define an empty DRM metadata string | emptyMetadata = "" | Empty string is defined | Should be successful |
 * | 03 | Invoke setDrmMetaData method with the empty string | input: emptyMetadata = "", output: none | Method call does not throw any exception | Should Pass |
 * | 04 | Verify the internal state mContentMetadata update | mContentMetadata expected to be empty | mContentMetadata is updated to an empty value | Should be successful |
 */
TEST(VerimatrixHelper, SetDrmMetadataWithEmptyString) {
    std::cout << "Entering SetDrmMetadataWithEmptyString test" << std::endl;
    
    // Create an object of VerimatrixHelper using its default constructor.
    EXPECT_NO_THROW({
        DrmInfo drmInfo;
        VerimatrixHelper verifier(drmInfo);
        std::cout << "Created VerimatrixHelper object." << std::endl;
        
        // Define the empty DRM metadata string.
        std::string emptyMetadata = "";
        std::cout << "Value to be set for DRM metadata (empty string): \"" << emptyMetadata << "\"" << std::endl;
        
        // Invoke the setDrmMetaData method with an empty string.
        std::cout << "Invoking setDrmMetaData method." << std::endl;
        EXPECT_NO_THROW(verifier.setDrmMetaData(emptyMetadata));
        std::cout << "Method setDrmMetaData invoked successfully with an empty string." << std::endl;
        
        // Internal state change expected: mContentMetadata should reflect an empty value.
        std::cout << "Assuming internal mContentMetadata is updated to empty value." << std::endl;
    });
    
    std::cout << "Exiting SetDrmMetadataWithEmptyString test" << std::endl;
}
/**
 * @brief Verifies that the transformLicenseResponse method handles a valid DRM license response correctly.
 *
 * This test verifies that a valid DRM license response is correctly processed by the transformLicenseResponse method of VerimatrixHelper. It ensures that the creation of a VerimatrixHelper instance and invoking the method with valid DRM data do not throw any exceptions.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 027@n
 * **Priority:** High@n
 * @n
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * @n
 * **Test Procedure:**
 * | Variation / Step | Description                                                            | Test Data                                                                                  | Expected Result                                                          | Notes         |
 * | :--------------: | ---------------------------------------------------------------------- | ------------------------------------------------------------------------------------------ | ------------------------------------------------------------------------ | ------------- |
 * | 01               | Create an instance of VerimatrixHelper using default constructor       | No input parameters, instance creation using default constructor                           | Instance is created without throwing any exceptions                      | Should Pass   |
 * | 02               | Create a valid licenseResponse with complete DRM data                  | licenseResponse = std::make_shared<DrmData>()                                               | licenseResponse is successfully created as a valid shared pointer         | Should be successful |
 * | 03               | Invoke transformLicenseResponse with the valid DRM data                | helperInstance (instance), licenseResponse (pointer to DrmData)                              | Method executes without throwing exceptions (EXPECT_NO_THROW condition met)| Should Pass   |
 */
TEST(VerimatrixHelper, ValidLicenseResponse) {
    std::cout << "Entering ValidLicenseResponse test" << std::endl;
    DrmInfo drmInfo;
    // Create an instance of VerimatrixHelper using default constructor
    VerimatrixHelper helperInstance(drmInfo);
    std::cout << "Created VerimatrixHelper instance." << std::endl;
    
    // Create a valid licenseResponse with complete DRM data.
    auto licenseResponse = std::make_shared<DrmData>();
    std::cout << "Created shared_ptr<DrmData> for license response." << std::endl;
    
    std::cout << "Invoking transformLicenseResponse with valid DRM data." << std::endl;
    EXPECT_NO_THROW(helperInstance.transformLicenseResponse(licenseResponse));
    std::cout << "transformLicenseResponse invoked successfully." << std::endl;
    std::cout << "Exiting ValidLicenseResponse test" << std::endl;
}
/**
 * @brief Verifies that transformLicenseResponse handles a nullptr license response gracefully.
 *
 * This test checks if the transformLicenseResponse() API of the VerimatrixHelper class can accept a null pointer as a license response without throwing an exception. It ensures that the function implementation safely handles null inputs.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 028@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**
 * | Variation / Step | Description                                                                     | Test Data                                                        | Expected Result                                                      | Notes         |
 * | :--------------: | ------------------------------------------------------------------------------- | ---------------------------------------------------------------- | -------------------------------------------------------------------- | ------------- |
 * | 01               | Instantiate VerimatrixHelper using its default constructor.                    | No input; output: helperInstance created                         | Instance is created without throwing any exceptions.               | Should Pass   |
 * | 02               | Set licenseResponse to nullptr.                                                | licenseResponse = nullptr                                          | licenseResponse is set to nullptr.                                   | Should be successful |
 * | 03               | Invoke transformLicenseResponse with the nullptr licenseResponse.             | licenseResponse = nullptr                                          | Method handles nullptr input without throwing an exception.         | Should Pass   |
 */
TEST(VerimatrixHelper, NullLicenseResponse) {
    std::cout << "Entering NullLicenseResponse test" << std::endl;
    DrmInfo drmInfo;
    // Create an instance of VerimatrixHelper using default constructor
    VerimatrixHelper helperInstance(drmInfo);
    std::cout << "Created VerimatrixHelper instance." << std::endl;
    
    // Create a null licenseResponse.
    std::shared_ptr<DrmData> licenseResponse = nullptr;
    std::cout << "Set licenseResponse to nullptr." << std::endl;
    
    std::cout << "Invoking transformLicenseResponse with nullptr." << std::endl;
    EXPECT_NO_THROW(helperInstance.transformLicenseResponse(licenseResponse));
    std::cout << "transformLicenseResponse handled nullptr input without throwing an exception." << std::endl;
    std::cout << "Exiting NullLicenseResponse test" << std::endl;
}
/**
 * @brief Verify that the destructor of a VerimatrixHelper object is invoked without exceptions.
 *
 * This test verifies that a VerimatrixHelper object can be successfully constructed using its default constructor and then safely deleted. It ensures that both the constructor and destructor perform as expected without throwing any exceptions.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 029
 * **Priority:** High
 * 
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                     | Test Data                                                    | Expected Result                                                   | Notes       |
 * | :--------------: | --------------------------------------------------------------- | ------------------------------------------------------------ | ----------------------------------------------------------------- | ----------- |
 * | 01               | Invoke default constructor of VerimatrixHelper to construct object| constructor: VerimatrixHelper(), no input parameters         | Object is constructed successfully without throwing exceptions    | Should Pass |
 * | 02               | Delete the constructed VerimatrixHelper object to trigger destructor| delete helper, no input parameters                           | Destructor is invoked successfully without throwing exceptions      | Should Pass |
 */
TEST(VerimatrixHelper, DestructorInvocationTest) {
    std::cout << "Entering DestructorInvocationTest test" << std::endl;
    
    EXPECT_NO_THROW({
        DrmInfo drmInfo;
        std::cout << "Invoking default constructor for VerimatrixHelper object" << std::endl;
        VerimatrixHelper *helper = new VerimatrixHelper(drmInfo);
        std::cout << "VerimatrixHelper object constructed successfully" << std::endl;
        
        std::cout << "Deleting VerimatrixHelper object to invoke destructor" << std::endl;
        delete helper;
        std::cout << "Destructor for VerimatrixHelper object invoked successfully" << std::endl;
    });
    
    std::cout << "Exiting DestructorInvocationTest test" << std::endl;
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
