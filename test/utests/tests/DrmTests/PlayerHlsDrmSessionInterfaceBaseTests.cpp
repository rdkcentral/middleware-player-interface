
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
* @file test_l1_PlayerHlsDrmSessionInterfaceBase.cpp
* @page PlayerHlsDrmSessionInterfaceBase Level 1 Tests
*
* ## Module's Role
* This module includes Level 1 functional tests (success and failure scenarios).
* This is to ensure that the PlayerHlsDrmSessionInterfaceBase methods meet the requirements.
*
* **Pre-Conditions:**  None @n
* **Dependencies:** None @n
*
*/

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <stdio.h>
#include "PlayerHlsDrmSessionInterfaceBase.h"


class PlayerHlsDrmSessionInterfaceBaseTests : public ::testing::Test
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
// Test Case: Register a valid GetHlsDrmSession callback
/**
 * @brief Test to register a valid lambda callback for GetHlsDrmSession and verify its proper invocation.
 *
 * This test validates that a valid lambda callback is correctly registered using RegisterGetHlsDrmSessionCb and, when invoked with test parameters, updates the callbackInvoked flag. This confirms that the callback registration and execution mechanism in the PlayerHlsDrmSessionInterfaceBase class is working as expected.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoke RegisterGetHlsDrmSessionCb with a valid lambda callback, then trigger the callback with test parameters. | callback = validCallback, dummyBridge = nullptr, dummyDrmHelper = nullptr, dummySession = nullptr, streamType = 42 | Callback should be registered and, when invoked, update callbackInvoked to true and satisfy EXPECT_TRUE(callbackInvoked) | Should Pass |
 */
TEST_F(PlayerHlsDrmSessionInterfaceBaseTests, RegisterValidGetHlsDrmSessionCbTest) 
{
    std::cout << "Entering RegisterValidGetHlsDrmSessionCbTest test" << std::endl;

    PlayerHlsDrmSessionInterfaceBase obj;
    
    bool callbackInvoked = false;
    // Define a lambda callback that updates callbackInvoked flag when called.
    auto validCallback = [&callbackInvoked](std::shared_ptr<HlsDrmBase>& bridge, 
                                              std::shared_ptr<DrmHelper>& drmHelper, 
                                              DrmSession*& session, 
                                              int streamType) {
        std::cout << "Lambda callback invoked with streamType: " << streamType << std::endl;
        // Simulate internal state update using a fixed size char array assignment via strncpy.
        char internalState[64];
        const char* stateMsg = "Callback executed";
        std::memset(internalState, 0, sizeof(internalState));
        std::strncpy(internalState, stateMsg, sizeof(internalState)-1);
        std::cout << "Internal state updated in callback: " << internalState << std::endl;
        callbackInvoked = true;
    };
    
    std::cout << "Invoking RegisterGetHlsDrmSessionCb with valid lambda callback" << std::endl;
    obj.RegisterGetHlsDrmSessionCb(validCallback);
    std::cout << "Callback registered. Internal state 'GetHlsDrmSessionCb' updated with provided lambda." << std::endl;
    
    // Verify that the callback has been registered (is callable)
    if (obj.GetHlsDrmSessionCb) {
        std::cout << "Retrieved callback is valid. Invoking the callback with test parameters." << std::endl;
        std::shared_ptr<HlsDrmBase> dummyBridge(nullptr);
        std::shared_ptr<DrmHelper> dummyDrmHelper(nullptr);
        DrmSession* dummySession = nullptr;
        int streamType = 42;
        obj.GetHlsDrmSessionCb(dummyBridge, dummyDrmHelper, dummySession, streamType);
        std::cout << "Callback was invoked with streamType value: " << streamType << std::endl;
    } else {
        std::cout << "Error: Callback not registered properly." << std::endl;
    }
    
    EXPECT_TRUE(callbackInvoked);
    std::cout << "Exiting RegisterValidGetHlsDrmSessionCbTest test" << std::endl;
}
/**
 * @brief Verify that registering an empty (default-constructed) callback results in no callback action.
 *
 * This test verifies that when an empty callback is registered using RegisterGetHlsDrmSessionCb on a PlayerHlsDrmSessionInterfaceBase object,
 * the internal state of GetHlsDrmSessionCb remains empty. This ensures the system behaves correctly when no callback logic is provided.
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
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Instantiate PlayerHlsDrmSessionInterfaceBase object | None | Object instantiated successfully | Should be successful |
 * | 02 | Register empty callback using RegisterGetHlsDrmSessionCb | emptyCallback = default constructed | Callback internal state remains empty | Should Pass |
 * | 03 | Verify internal state of GetHlsDrmSessionCb via conditional log | None | Log reflects callback is empty as expected | Should Pass |
 * | 04 | Assert that the callback is false using EXPECT_FALSE | None | EXPECT_FALSE assertion passes confirming callback is empty | Should Pass |
 */
TEST_F(PlayerHlsDrmSessionInterfaceBaseTests, RegisterEmptyGetHlsDrmSessionCbTest) {
    std::cout << "Entering RegisterEmptyGetHlsDrmSessionCbTest test" << std::endl;

    PlayerHlsDrmSessionInterfaceBase obj;
    
    std::function<void(std::shared_ptr<HlsDrmBase>&, std::shared_ptr<DrmHelper>&, DrmSession*&, int)> emptyCallback;
    std::cout << "Registering an empty (default-constructed) callback using RegisterGetHlsDrmSessionCb" << std::endl;
    obj.RegisterGetHlsDrmSessionCb(emptyCallback);
    std::cout << "Empty callback registered. Verifying the internal state of GetHlsDrmSessionCb." << std::endl;
    
    if (!obj.GetHlsDrmSessionCb) {
        std::cout << "Callback is empty as expected. No callback logic is present." << std::endl;
    } else {
        std::cout << "Unexpected: Callback is not empty." << std::endl;
    }
    
    EXPECT_FALSE(static_cast<bool>(obj.GetHlsDrmSessionCb));
    std::cout << "Exiting RegisterEmptyGetHlsDrmSessionCbTest test" << std::endl;
}
/**
 * @brief Test valid creation of HLS DRM session with valid DRM info and stream type
 *
 * This test verifies that a valid DRM session is successfully created by invoking the createSession API 
 * with valid DRM information and a valid stream type. It ensures that the returned session pointer is not null.
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
 * | Variation / Step | Description                                                                                           | Test Data                                                  | Expected Result                                                  | Notes          |
 * | :--------------: | ----------------------------------------------------------------------------------------------------- | ---------------------------------------------------------- | ---------------------------------------------------------------- | -------------- |
 * | 01               | Initialize stream type, DRM info, and create an instance of PlayerHlsDrmSessionInterfaceBase           | drmInfo = valid DRM info, streamType = 0                     | Instance created successfully                                    | Should be successful |
 * | 02               | Invoke createSession API using valid DRM info and stream type                                           | input: drmInfo = valid DRM info, input: streamType = 0         | Returned session pointer is non-null as per EXPECT_NE check        | Should Pass    |
 * | 03               | Log the returned session pointer to verify the output                                                | output: session pointer obtained from createSession call     | Console outputs a valid session pointer address                   | Should be successful |
 */
TEST_F(PlayerHlsDrmSessionInterfaceBaseTests, createSessionValid)
{
    std::cout << "Entering createSessionValid test" << std::endl;
    int streamType = 0;
    DrmInfo drmInfo;
    // Create instance object and call createSession.
    PlayerHlsDrmSessionInterfaceBase playerHlsDrmSessionInterface;
    std::shared_ptr<HlsDrmBase> session = playerHlsDrmSessionInterface.createSession(drmInfo, streamType);

    // Log the returned session pointer.
    std::cout << "Returned session pointer: " << session.get() << std::endl;

    // Expected Output:
    EXPECT_NE(session, nullptr);
    
    std::cout << "Exiting createSessionValid test" << std::endl;
}
/**
 * @brief Verify that createSession returns a null session pointer when an invalid negative stream type is provided.
 *
 * This test case validates that the createSession method of PlayerHlsDrmSessionInterfaceBase properly handles an invalid input by returning a null session pointer. This negative test ensures the robustness of the API in handling erroneous input data.
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
 * | Variation / Step | Description                                                         | Test Data                                               | Expected Result                                                                      | Notes            |
 * | :---------------:| -------------------------------------------------------------------- | ------------------------------------------------------- | -------------------------------------------------------------------------------------| ---------------- |
 * | 01               | Initialize test parameters, creating a default DrmInfo object and setting streamType as -1. | drmInfo = default, streamType = -1                       | Test parameters are correctly initialized.                                           | Should be successful |
 * | 02               | Invoke createSession API with a negative stream type and verify that the session pointer returned is null. | drmInfo = default, streamType = -1, expected session pointer = nullptr | API returns a null session pointer and the assertion EXPECT_EQ(session, nullptr) passes. | Should Fail      |
 */
TEST_F(PlayerHlsDrmSessionInterfaceBaseTests, NegativeStreamType)
{
    std::cout << "Entering NegativeStreamType test" << std::endl;
    
    // Create a default DrmInfo object
    DrmInfo drmInfo;    
    int streamType = -1;
       
   // Create instance object and call createSession.
    PlayerHlsDrmSessionInterfaceBase playerHlsDrmSessionInterface;
    std::shared_ptr<HlsDrmBase> session = playerHlsDrmSessionInterface.createSession(drmInfo, streamType);
    
    // Log the returned session pointer.
    std::cout << "Returned session pointer: " << session.get() << std::endl;
    
    // Expected Output: null session pointer due to invalid stream type.
    EXPECT_EQ(session, nullptr) << "Expected null session for negative stream type";
    
    std::cout << "Exiting NegativeStreamType test" << std::endl;
}
/**
 * @brief Verify default DRM information handling
 *
 * This test checks the behavior of the isDrmSupported API when provided with a default constructed DrmInfo object and a PlayerHlsDrmSessionInterfaceBase instance created using its default constructor. The test ensures that the API call returns the expected true value under normal conditions.
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
 * | Variation / Step | Description                                                         | Test Data                                                                  | Expected Result                                                             | Notes         |
 * | :--------------: | ------------------------------------------------------------------- | -------------------------------------------------------------------------- | --------------------------------------------------------------------------- | ------------- |
 * | 01               | Initialize a default DrmInfo object and a default PlayerHlsDrmSessionInterfaceBase object | drmInfo = default, drmSessionObject = default                              | Objects are created successfully                                           | Should be successful |
 * | 02               | Invoke isDrmSupported using the default DrmInfo and validate the output                    | drmInfo = default, output: isSupported = true                               | isDrmSupported returns true and the assertion passes                        | Should Pass   |
 */
TEST_F(PlayerHlsDrmSessionInterfaceBaseTests, DefaultDrmInfo) {
    std::cout << "Entering DefaultDrmInfo test" << std::endl;

    // Create a default DrmInfo object
    DrmInfo drmInfo;
    
    // Create an object of PlayerHlsDrmSessionInterfaceBase using the default constructor.
    PlayerHlsDrmSessionInterfaceBase drmSessionObject;

    // Log the invocation of isDrmSupported and the values passed.
    std::cout << "Invoking isDrmSupported with default DrmInfo" << std::endl;
    bool isSupported = drmSessionObject.isDrmSupported(drmInfo);
    std::cout << "isDrmSupported returned: " << isSupported << std::endl;

    // Validate the expected output (which is false)
    EXPECT_TRUE(isSupported);

    std::cout << "Exiting DefaultDrmInfo test" << std::endl;
}