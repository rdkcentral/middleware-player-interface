
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
* @file test_l1_HlsDrmSessionManager.cpp
* @page HlsDrmSessionManager Level 1 Tests
*
* ## Module's Role
* This module includes Level 1 functional tests (success and failure scenarios).
* This is to ensure that the HlsDrmSessionManager methods meet the requirements.
*
* **Pre-Conditions:**  None @n
* **Dependencies:** None @n
*
*/

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <stdio.h>
#include "HlsDrmSessionManager.h"

class HlsDrmSessionManagerTests : public ::testing::Test
{
protected:
    void SetUp() override
    {

    }   
    void TearDown() override
    {

    }
public:
};
/**
 * @brief Verifies that a DRM session is successfully created using a positive test scenario.
 *
 * This test validates that when valid arguments (a default DrmInfo object and streamType = 1) are provided to the createSession API of HlsDrmSessionManager, a non-null pointer is returned, indicating successful creation of a DRM session.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 001@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data |Expected Result |Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Instantiate DrmInfo and HlsDrmSessionManager objects | drmInfo = default, drmManager = default constructor | Objects are instantiated | Should be successful |
 * | 02 | Set streamType value | streamType = 1 | streamType is assigned value 1 | Should be successful |
 * | 03 | Invoke createSession with drmInfo and streamType, and capture the returned session pointer | drmInfo = default, streamType = 1, output: session pointer | Returns a valid non-null session pointer | Should Pass |
 * | 04 | Validate the returned session pointer using EXPECT_NE | session pointer != nullptr | Assertion passes confirming successful session creation | Should Pass |
 */
TEST_F(HlsDrmSessionManagerTests, PositiveDRMSessionCreation) {
    std::cout << "Entering PositiveDRMSessionCreation test" << std::endl;
    
    // Create DrmInfo
    DrmInfo drmInfo;
    // Create object for HlsDrmSessionManager using its default constructor
    HlsDrmSessionManager drmManager;
    int streamType = 1;
    std::cout << "Invoking createSession with streamType: " << streamType << std::endl;
    
    // Invoke createSession and capture the return value
    std::shared_ptr<HlsDrmBase> session = drmManager.createSession(drmInfo, streamType);
    std::cout << "Returned session pointer: " << session.get() << std::endl;
    
    // Validate that the returned session is non-null indicating successful DRM session creation
    EXPECT_NE(session, nullptr);
    
    std::cout << "Exiting PositiveDRMSessionCreation test" << std::endl;
}
/**
 * @brief Validate that HlsDrmSessionManager::createSession fails when an invalid stream type is provided
 *
 * This test verifies that the createSession API of HlsDrmSessionManager returns a null pointer when an invalid stream type is passed. It ensures that the API handles erroneous input correctly by not creating a DRM session.
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
 * | Variation / Step | Description                                                              | Test Data                                              | Expected Result                                               | Notes            |
 * | :----:           | ------------------------------------------------------------------------ | ------------------------------------------------------ | ------------------------------------------------------------- | ---------------- |
 * | 01               | Create a valid DrmInfo object and instantiate HlsDrmSessionManager         | drmInfo = valid, drmManager created                    | Successfully created objects                                  | Should be successful |
 * | 02               | Set an invalid stream type                                               | streamType = -1                                        | Invalid stream type set                                        | Should be successful |
 * | 03               | Invoke createSession with the valid DrmInfo and invalid streamType         | drmInfo = valid, streamType = -1                       | Returns null pointer indicating failure                        | Should Pass      |
 * | 04               | Validate that the returned session pointer is null using EXPECT_EQ         | session pointer from createSession                     | EXPECT_EQ confirms that session pointer is null                 | Should Pass      |
 */
TEST_F(HlsDrmSessionManagerTests, NegativeDRMSessionCreation) {
    std::cout << "Entering NegativeDRMSessionCreation test" << std::endl;
    
    // Create and populate DrmInfo with valid fields but with an invalid stream type for this test
    DrmInfo drmInfo;
    // Create object for HlsDrmSessionManager using its default constructor
    HlsDrmSessionManager drmManager;
    int streamType = -1;
    std::cout << "Invoking createSession with streamType: " << streamType << std::endl;
    
    // Invoke createSession and capture the return value
    std::shared_ptr<HlsDrmBase> session = drmManager.createSession(drmInfo, streamType);
    std::cout << "Returned session pointer: " << session.get() << std::endl;
    
    // Validate that the returned session is null indicating failure due to an invalid stream type
    EXPECT_EQ(session, nullptr);
    
    std::cout << "Exiting NegativeDRMSessionCreation test" << std::endl;
}
/**
 * @brief Test HlsDrmSessionManager::getInstance() to verify that a valid singleton instance is returned.
 *
 * This test verifies that the getInstance() method of HlsDrmSessionManager returns a valid (non-null) singleton instance. It ensures that the singleton implementation is correct and does not throw exceptions during invocation.
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
 * | 01 | Invoke HlsDrmSessionManager::getInstance() to obtain the singleton instance and verify its validity | input: none, output: instance address != nullptr | The API should return a valid, non-null instance and pass the ASSERT_NE check | Should Pass |
 */
TEST_F(HlsDrmSessionManagerTests, getInstance_start) {
    std::cout << "Entering getInstance_start test" << std::endl;
    try {
        std::cout << "Invoking HlsDrmSessionManager::getInstance()" << std::endl;
        HlsDrmSessionManager &instance = HlsDrmSessionManager::getInstance();
        std::cout << "Received instance address: " << &instance << std::endl;
        // Verify the returned instance is valid (non-null)
        ASSERT_NE(&instance, nullptr);
        std::cout << "HlsDrmSessionManager instance is valid and non-null." << std::endl;
    } catch(const std::exception &e) {
        std::cout << "Exception caught during getInstance() invocation: " << e.what() << std::endl;
        FAIL() << "HlsDrmSessionManager::getInstance() threw an exception: " << e.what();
    } catch(...) {
        std::cout << "Unknown exception caught during getInstance() invocation." << std::endl;
        FAIL() << "HlsDrmSessionManager::getInstance() threw an unknown exception.";
    }
    std::cout << "Exiting getInstance_start test" << std::endl;
}
/**
 * @brief This test verifies that HlsDrmSessionManager returns true when provided with a valid DrmInfo object.
 *
 * This test is designed to ensure that the isDrmSupported method in HlsDrmSessionManager correctly identifies support for DRM when valid DRM information is provided. It creates an instance using the default constructor, prepares a valid DrmInfo object, invokes the method, and asserts that the result is true.
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
 * | Variation / Step | Description                                                                                                | Test Data                                             | Expected Result                                    | Notes         |
 * | :--------------: | ---------------------------------------------------------------------------------------------------------- | ----------------------------------------------------- | -------------------------------------------------- | ------------- |
 * | 01               | Create HlsDrmSessionManager object using the default constructor                                             | drmSessionManager = default                           | Object is instantiated successfully                | Should be successful |
 * | 02               | Prepare a valid DrmInfo object for HLS media format                                                          | drmInfo = valid                                       | DrmInfo object is prepared correctly               | Should be successful |
 * | 03               | Invoke the isDrmSupported method on the drmSessionManager object using the valid drmInfo object                | input: drmInfo = valid, output: result = expected true| Method returns true                                | Should Pass   |
 * | 04               | Validate that the result returned by isDrmSupported is true using ASSERT_TRUE                                | result = true                                         | Assertion confirms the method returns true         | Should Pass   |
 */
TEST_F(HlsDrmSessionManagerTests, PositiveTestForDRMSupport) {
    std::cout << "Entering PositiveTestForDRMSupport test" << std::endl;

    // Create HlsDrmSessionManager object using the default constructor.
    std::cout << "Creating HlsDrmSessionManager object using default constructor" << std::endl;
    HlsDrmSessionManager drmSessionManager;

    // Prepare DrmInfo object with valid DRM method and HLS media format.
    DrmInfo drmInfo;

    // Invoke the method under test.
    bool result = drmSessionManager.isDrmSupported(drmInfo);
    std::cout << "Method isDrmSupported returned: " << std::boolalpha << result << std::endl;

    // Check that the method returns true for valid DRM support.
    ASSERT_TRUE(result);

    std::cout << "Exiting PositiveTestForDRMSupport test" << std::endl;
}
