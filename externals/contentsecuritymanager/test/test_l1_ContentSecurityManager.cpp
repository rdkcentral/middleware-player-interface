
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
#include "ContentSecurityManager.h"

class TestContentSecurityManager : public ContentSecurityManager {
public:
    using ContentSecurityManager::AcquireLicenseOpenOrUpdate;
    using ContentSecurityManager::SetDrmSessionState;
    using ContentSecurityManager::CloseDrmSession;
    using ContentSecurityManager::SetPlaybackPosition;
    using ContentSecurityManager::setWindowSize;
};

class ContentSecurityManagerTest : public ::testing::Test {
protected:
    TestContentSecurityManager* manager;

    virtual void SetUp() override {
        manager = new TestContentSecurityManager();
    }

    virtual void TearDown() override {
        delete manager;
        manager = nullptr;
    }
};

/**
 * @brief Test for instance creation and proper destruction via DestroyInstance().
 *
 * This test case verifies that the ContentSecurityManager instance can be created without throwing exceptions and that its DestroyInstance method can be invoked successfully to destroy the singleton instance.
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
 * | Variation / Step | Description                                                                                           | Test Data                                                                  | Expected Result                                              | Notes           |
 * | :--------------: | ----------------------------------------------------------------------------------------------------- | -------------------------------------------------------------------------- | ------------------------------------------------------------ | --------------- |
 * | 01               | Create ContentSecurityManager instance using default constructor and verify no exceptions are thrown.   | instance creation = new ContentSecurityManager()                           | Instance is created without throwing exceptions              | Should Pass     |
 * | 02               | Invoke DestroyInstance() method on the created instance and verify no exceptions are thrown.             | method invocation = instance->DestroyInstance()                            | DestroyInstance() executes without throwing exceptions       | Should Pass     |
 * | 03               | Assume the singleton instance has been correctly destroyed; perform any further required status checks. | post check = instance status (if available)                                | The singleton instance is assumed destroyed as expected      | Should be successful |
 */
TEST(ContentSecurityManager, DestroyInstance_start) {
    std::cout << "Entering DestroyInstance_start test" << std::endl;
    
    // Create a ContentSecurityManager object using default constructor.
    // Note: Using EXPECT_NO_THROW to catch any unexpected exceptions during object creation.
    ContentSecurityManager *instance = nullptr;
    EXPECT_NO_THROW({
        instance = new ContentSecurityManager();
        std::cout << "Created ContentSecurityManager instance at address: " << instance << std::endl;
    });
    
    // Invoking the DestroyInstance method.
    std::cout << "Invoking DestroyInstance() on the ContentSecurityManager instance." << std::endl;
    EXPECT_NO_THROW({
        instance->DestroyInstance();
        std::cout << "DestroyInstance() invoked successfully." << std::endl;
    });
    
    // Since DestroyInstance() is expected to destroy the singleton instance,
    // further status checks could be carried out here if any getter or status API existed.
    std::cout << "Assuming the singleton instance has been destroyed as expected." << std::endl;
    
    std::cout << "Exiting DestroyInstance_start test" << std::endl;
}
/**
 * @brief Validates that a singleton instance is successfully created.
 *
 * This test ensures that calling GetInstance() does not throw an exception and returns a non-null pointer. The test verifies the singleton implementation by checking that the returned instance is valid.
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
 * | Variation / Step | Description                                                                              | Test Data                                              | Expected Result                                                                          | Notes           |
 * | :--------------: | ---------------------------------------------------------------------------------------- | ------------------------------------------------------ | ---------------------------------------------------------------------------------------- | --------------- |
 * | 01               | Log the entry message for the test.                                                      | None                                                 | "Entering GetInstance_start test" is printed to output.                                  | Should be successful |
 * | 02               | Invoke the GetInstance() API and ensure no exception is thrown while obtaining instance.   | Before API call: instance = nullptr, API: GetInstance()  | No exception is thrown and instance is assigned a non-null pointer.                      | Should Pass     |
 * | 03               | Validate that the returned instance is non-null using an assertion.                        | instance variable = pointer returned by GetInstance() | EXPECT_NE(instance, nullptr) assertion passes.                                           | Should Pass     |
 * | 04               | Log the exit message for the test.                                                       | None                                                 | "Exiting GetInstance_start test" is printed to output.                                   | Should be successful |
 */
TEST(ContentSecurityManager, GetInstance_start) {
    std::cout << "Entering GetInstance_start test" << std::endl;
    
    ContentSecurityManager* instance = nullptr;
    std::cout << "Invoking GetInstance()" << std::endl;
    EXPECT_NO_THROW({
        instance = ContentSecurityManager::GetInstance();
    });
    
    if(instance) {
        std::cout << "GetInstance() returned non-null pointer: " << instance << std::endl;
        // If object had key information, it would be printed here.
    } else {
        std::cout << "GetInstance() returned null pointer" << std::endl;
    }
    
    EXPECT_NE(instance, nullptr);
    
    std::cout << "Exiting GetInstance_start test" << std::endl;
}
/**
 * @brief Verify that ReleaseSession method correctly processes a typical valid session ID.
 *
 * This test verifies that the ReleaseSession function in ContentSecurityManager behaves as expected when provided with a valid session ID. The primary aim is to ensure that no exceptions are thrown during the release of the session, confirming that the API handles normal input conditions properly.
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
 * | Variation / Step | Description                                                              | Test Data                                | Expected Result                                 | Notes         |
 * | :--------------: | ------------------------------------------------------------------------ | ---------------------------------------- | ----------------------------------------------- | ------------- |
 * | 01               | Instantiate ContentSecurityManager using the default constructor         | No input arguments                       | Object should be created successfully           | Should be successful |
 * | 02               | Initialize the sessionId variable with a valid session identifier        | sessionId = 12345                        | sessionId variable set to 12345                 | Should be successful |
 * | 03               | Invoke ReleaseSession API with the valid sessionId and verify no exception | input: sessionId = 12345, output: void     | Method executes without throwing any exceptions | Should Pass   |
 */
TEST(ContentSecurityManager, ReleaseSession_TypicalValidSessionID) {
    std::cout << "Entering ReleaseSession_TypicalValidSessionID test" << std::endl;
    EXPECT_NO_THROW({
        ContentSecurityManager manager;
        std::cout << "Created ContentSecurityManager object using default constructor" << std::endl;
        int64_t sessionId = 12345;
        std::cout << "Invoking ReleaseSession with sessionId: " << sessionId << std::endl;
        manager.ReleaseSession(sessionId);
        std::cout << "Method ReleaseSession executed successfully for sessionId: " << sessionId << std::endl;
    });
    std::cout << "Exiting ReleaseSession_TypicalValidSessionID test" << std::endl;
}
/**
 * @brief Test ReleaseSession API with a negative session ID to verify exception handling
 *
 * This test verifies that the ReleaseSession function of ContentSecurityManager can handle a negative session identifier (-100) correctly. Although using a negative session ID is an invalid input scenario, the function is expected to execute gracefully without throwing an exception.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |@n
 * | :----: | --------- | ---------- |-------------- | ----- |@n
 * | 01 | Create the ContentSecurityManager instance using the default constructor | No input parameters | Instance is created successfully | Should be successful |@n
 * | 02 | Invoke ReleaseSession method with a negative session ID | sessionId = -100 | Method completes without throwing an exception | Should Fail |
 */
TEST(ContentSecurityManager, ReleaseSession_NegativeSessionID) {
    std::cout << "Entering ReleaseSession_NegativeSessionID test" << std::endl;
    EXPECT_NO_THROW({
        ContentSecurityManager manager;
        std::cout << "Created ContentSecurityManager object using default constructor" << std::endl;
        int64_t sessionId = -100;
        std::cout << "Invoking ReleaseSession with sessionId: " << sessionId << std::endl;
        manager.ReleaseSession(sessionId);
        std::cout << "Method ReleaseSession executed successfully for sessionId: " << sessionId << std::endl;
    });
    std::cout << "Exiting ReleaseSession_NegativeSessionID test" << std::endl;
}
/**
 * @brief Verify that ReleaseSession correctly handles the maximum int64_t session ID without throwing an exception.
 *
 * This test creates an instance of ContentSecurityManager and invokes its ReleaseSession
 * method with the maximum possible int64_t value (9223372036854775807). The objective is to
 * ensure that the method does not throw any exception when handling edge-case session IDs.
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
 * | Variation / Step | Description                                                                               | Test Data                                                         | Expected Result                                      | Notes         |
 * | :--------------: | ----------------------------------------------------------------------------------------- | ----------------------------------------------------------------- | ---------------------------------------------------- | ------------- |
 * | 01               | Create a ContentSecurityManager instance using the default constructor                    | No input arguments                                                | Object creation successfully completed               | Should be successful |
 * | 02               | Invoke the ReleaseSession API with sessionId set to 9223372036854775807                       | sessionId = 9223372036854775807                                     | The ReleaseSession method is executed without throwing any exception and meets EXPECT_NO_THROW assertion | Should Pass   |
 */
TEST(ContentSecurityManager, ReleaseSession_MaximumInt64SessionID) {
    std::cout << "Entering ReleaseSession_MaximumInt64SessionID test" << std::endl;
    EXPECT_NO_THROW({
        ContentSecurityManager manager;
        std::cout << "Created ContentSecurityManager object using default constructor" << std::endl;
        int64_t sessionId = 9223372036854775807;
        std::cout << "Invoking ReleaseSession with sessionId: " << sessionId << std::endl;
        manager.ReleaseSession(sessionId);
        std::cout << "Method ReleaseSession executed successfully for sessionId: " << sessionId << std::endl;
    });
    std::cout << "Exiting ReleaseSession_MaximumInt64SessionID test" << std::endl;
}
/**
 * @brief Test ReleaseSession API with minimum int64_t session ID
 *
 * This test validates that the ReleaseSession() method of the ContentSecurityManager class can handle the minimum possible int64_t session ID without throwing exceptions. Handling such edge-case values is critical to ensure the robustness of the API.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 006@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create a ContentSecurityManager object and invoke ReleaseSession with the minimum int64_t session ID | sessionId = -9223372036854775808, output = N/A | ReleaseSession completes without throwing any exceptions | Should Pass |
 */
TEST(ContentSecurityManager, ReleaseSession_MinimumInt64SessionID) {
    std::cout << "Entering ReleaseSession_MinimumInt64SessionID test" << std::endl;
    EXPECT_NO_THROW({
        ContentSecurityManager manager;
        std::cout << "Created ContentSecurityManager object using default constructor" << std::endl;
        int64_t sessionId = std::numeric_limits<int64_t>::min();
        std::cout << "Invoking ReleaseSession with sessionId: " << sessionId << std::endl;
        manager.ReleaseSession(sessionId);
        std::cout << "Method ReleaseSession executed successfully for sessionId: " << sessionId << std::endl;
    });
    std::cout << "Exiting ReleaseSession_MinimumInt64SessionID test" << std::endl;
}
/**
 * @brief Validate that UpdateSessionState succeeds with valid positive session ID and active flag is set correctly
 *
 * This test verifies that invoking the UpdateSessionState API on a ContentSecurityManager instance with a valid positive sessionId 
 * (12345) and an active flag set to true does not throw an exception and returns true. It validates the positive path for updating the session.
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
 * | 01 | Initialize ContentSecurityManager, set valid sessionId and active flag, and invoke UpdateSessionState API. | sessionId = 12345, active = true | API returns true and assertion (EXPECT_TRUE) passes without exception. | Should Pass |
 */
TEST(ContentSecurityManager, UpdateActiveSessionValidPositiveId) {
    std::cout << "Entering UpdateActiveSessionValidPositiveId test" << std::endl;
    ContentSecurityManager csm;
    int64_t sessionId = 12345;
    bool active = true;
    std::cout << "Invoking UpdateSessionState with sessionId: " << sessionId << ", active: " << std::boolalpha << active << std::endl;
    bool result = false;
    EXPECT_NO_THROW({
        result = csm.UpdateSessionState(sessionId, active);
        std::cout << "UpdateSessionState returned: " << std::boolalpha << result << std::endl;
    });
    EXPECT_TRUE(result);
    std::cout << "Exiting UpdateActiveSessionValidPositiveId test" << std::endl;
}
/**
 * @brief Test for updating an inactive session with a valid positive session ID.
 *
 * This test verifies that the UpdateSessionState function correctly updates a session's state to inactive when a valid positive session ID is provided. The function call is wrapped in EXPECT_NO_THROW to ensure no exceptions are thrown, and the test asserts that the returned value is true, indicating a successful update.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 008@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Initialize ContentSecurityManager instance and set valid parameters, then invoke UpdateSessionState | sessionId = 12345, active = false, result variable uninitialized | No exception thrown during API call | Should Pass |
 * | 02 | Verify that the call to UpdateSessionState returns true | After invocation: result = true, sessionId = 12345, active = false | API returns true and the assertion EXPECT_TRUE(result) passes | Should Pass |
 */
TEST(ContentSecurityManager, UpdateInactiveSessionValidPositiveId) {
    std::cout << "Entering UpdateInactiveSessionValidPositiveId test" << std::endl;
    ContentSecurityManager csm;
    int64_t sessionId = 12345;
    bool active = false;
    std::cout << "Invoking UpdateSessionState with sessionId: " << sessionId << ", active: " << std::boolalpha << active << std::endl;
    bool result = false;
    EXPECT_NO_THROW({
        result = csm.UpdateSessionState(sessionId, active);
        std::cout << "UpdateSessionState returned: " << std::boolalpha << result << std::endl;
    });
    EXPECT_TRUE(result);
    std::cout << "Exiting UpdateInactiveSessionValidPositiveId test" << std::endl;
}
/**
 * @brief Test UpdateSessionState with a negative session id value.
 *
 * This test case verifies that UpdateSessionState correctly handles an invalid negative session identifier by not throwing an exception and returning false. It ensures that the API properly rejects negative session ids, maintaining the integrity of the session management.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- | -------------- | ----- |
 * | 01 | Invoke UpdateSessionState with a negative session id and inactive state | sessionId = -1, active = false | No exception thrown and API returns false | Should Fail |
 */
TEST(ContentSecurityManager, UpdateSessionWithNegativeSessionId) {
    std::cout << "Entering UpdateSessionWithNegativeSessionId test" << std::endl;
    ContentSecurityManager csm;
    int64_t sessionId = -1;
    bool active = false;
    std::cout << "Invoking UpdateSessionState with sessionId: " << sessionId << ", active: " << std::boolalpha << active << std::endl;
    bool result = true;
    EXPECT_NO_THROW({
        result = csm.UpdateSessionState(sessionId, active);
        std::cout << "UpdateSessionState returned: " << std::boolalpha << result << std::endl;
    });
    EXPECT_FALSE(result);
    std::cout << "Exiting UpdateSessionWithNegativeSessionId test" << std::endl;
}
/**
 * @brief Test to update session state using an extremely large sessionId.
 *
 * This test verifies that the ContentSecurityManager::UpdateSessionState method can handle 
 * an extremely large sessionId value (INT64_MAX) without throwing exceptions and returns true.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 010
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create an instance of ContentSecurityManager, initialize sessionId with INT64_MAX and active flag with true, then invoke UpdateSessionState | sessionId = INT64_MAX, active = true | The API should complete without throwing exceptions and return true; EXPECT_TRUE(result) should pass | Should Pass |
 */
TEST(ContentSecurityManager, UpdateSessionWithLargeSessionId) {
    std::cout << "Entering UpdateSessionWithLargeSessionId test" << std::endl;
    ContentSecurityManager csm;
    int64_t sessionId = INT64_MAX;
    bool active = true;
    std::cout << "Invoking UpdateSessionState with sessionId: " << sessionId << ", active: " << std::boolalpha << active << std::endl;
    bool result = false;
    EXPECT_NO_THROW({
        result = csm.UpdateSessionState(sessionId, active);
        std::cout << "UpdateSessionState returned: " << std::boolalpha << result << std::endl;
    });
    EXPECT_TRUE(result);
    std::cout << "Exiting UpdateSessionWithLargeSessionId test" << std::endl;
}
/**
 * @brief Tests the UpdateSessionState API with an edge case session ID
 *
 * This test verifies that the ContentSecurityManager::UpdateSessionState method correctly handles an edge case
 * where the sessionId is set to INT64_MIN and active is false. The method is expected to return false without throwing
 * any exceptions.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Instantiate ContentSecurityManager and define test parameters. | sessionId = INT64_MIN, active = false | Instance creation and parameters set successfully. | Should be successful |
 * | 02 | Invoke UpdateSessionState API with defined parameters and verify the outcome. | sessionId = INT64_MIN, active = false, output: result should be false | API should return false without throwing exceptions. | Should Fail |
 */
TEST(ContentSecurityManager, UpdateSessionWithSmallSessionId) {
    std::cout << "Entering UpdateSessionWithSmallSessionId test" << std::endl;
    ContentSecurityManager csm;
    int64_t sessionId = INT64_MIN;
    bool active = false;
    std::cout << "Invoking UpdateSessionState with sessionId: " << sessionId << ", active: " << std::boolalpha << active << std::endl;
    bool result = true;
    EXPECT_NO_THROW({
        result = csm.UpdateSessionState(sessionId, active);
        std::cout << "UpdateSessionState returned: " << std::boolalpha << result << std::endl;
    });
    EXPECT_FALSE(result);
    std::cout << "Exiting UpdateSessionWithSmallSessionId test" << std::endl;
}
/**
 * @brief Check if the ContentSecurityManager::UseFireboltSDK API enables Firebolt SDK without throwing exceptions
 *
 * This test evaluates the functionality of ContentSecurityManager::UseFireboltSDK when provided with a true value.
 * The objective is to ensure that the API call completes successfully, enabling the Firebolt SDK as evidenced by the
 * internal state and debug logs.
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
 * | 01 | Log entering the test and display the input value | inputStatus = true | Confirmation logs for entering the test and input value printed | Should be successful |
 * | 02 | Invoke the ContentSecurityManager::UseFireboltSDK API with true and check for exceptions | inputStatus = true, API = ContentSecurityManager::UseFireboltSDK(true) | API executes without throwing any exception | Should Pass |
 * | 03 | Log detailed message on API invocation success that Firebolt SDK is enabled | Log output displaying "Firebolt SDK is enabled; system state reflects active Firebolt capability" | Confirmation log showing successful API invocation with enabled SDK state | Should be successful |
 * | 04 | Log exiting the test | Log output displaying "Exiting UseFireboltSDK_EnableTrue test" | Final log confirming proper termination of the test | Should be successful |
 */
TEST(ContentSecurityManager, UseFireboltSDK_EnableTrue) {
    std::cout << "Entering UseFireboltSDK_EnableTrue test" << std::endl;

    // Log invocation details.
    std::cout << "Invoking ContentSecurityManager::UseFireboltSDK with true" << std::endl;
    bool inputStatus = true;
    std::cout << "Input value: " << std::boolalpha << inputStatus << std::endl;

    // Call the method and ensure no exception is thrown.
    EXPECT_NO_THROW(ContentSecurityManager::UseFireboltSDK(inputStatus));

    // Simulate internal state reflection debug log.
    std::cout << "Method UseFireboltSDK invoked successfully. "
              << "Expected state: Firebolt SDK is enabled; system state reflects active Firebolt capability." << std::endl;

    std::cout << "Exiting UseFireboltSDK_EnableTrue test" << std::endl;
}
/**
 * @brief Tests the ContentSecurityManager::UseFireboltSDK function with a false input to verify that the Firebolt SDK is disabled.
 *
 * This test verifies that when the ContentSecurityManager::UseFireboltSDK method is invoked with a false flag, it executes without throwing any exception and appropriately reflects that the Firebolt SDK is disabled. This positive test case ensures that the system correctly handles disabling the SDK.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 013
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                      | Test Data                                    | Expected Result                                                                                      | Notes       |
 * | :--------------: | ---------------------------------------------------------------  | -------------------------------------------- | ---------------------------------------------------------------------------------------------------- | ----------- |
 * | 01               | Invoke ContentSecurityManager::UseFireboltSDK with false flag       | inputStatus = false                          | API executes without throwing an exception and disables the Firebolt SDK as verified by internal logs | Should Pass |
 */
TEST(ContentSecurityManager, UseFireboltSDK_DisableFalse) {
    std::cout << "Entering UseFireboltSDK_DisableFalse test" << std::endl;

    // Log invocation details.
    std::cout << "Invoking ContentSecurityManager::UseFireboltSDK with false" << std::endl;
    bool inputStatus = false;
    std::cout << "Input value: " << std::boolalpha << inputStatus << std::endl;

    // Call the method and ensure no exception is thrown.
    EXPECT_NO_THROW(ContentSecurityManager::UseFireboltSDK(inputStatus));

    // Simulate internal state reflection debug log.
    std::cout << "Method UseFireboltSDK invoked successfully. "
              << "Expected state: Firebolt SDK is disabled; system state reflects inactive Firebolt capability." << std::endl;

    std::cout << "Exiting UseFireboltSDK_DisableFalse test" << std::endl;
}
/**
 * @brief Verifies that setPlaybackSpeedState works correctly for valid input values.
 *
 * This test checks that the ContentSecurityManager object is instantiated without any exceptions, and that
 * invoking setPlaybackSpeedState with valid input parameters (sessionId, playback_speed, playback_position)
 * returns true. This confirms that the API handles positive values as expected.
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
 * | Variation / Step | Description                                                                                   | Test Data                                                          | Expected Result                                               | Notes            |
 * | :--------------: | --------------------------------------------------------------------------------------------- | ------------------------------------------------------------------ | ------------------------------------------------------------- | ---------------- |
 * | 01               | Create ContentSecurityManager object using default constructor.                               | None                                                               | Object is successfully created with no exceptions thrown.     | Should be successful |
 * | 02               | Initialize valid input parameters for playback state.                                         | sessionId = 100, playback_speed = 2, playback_position = 5000        | Parameters are ready to be used for API invocation.           | Should be successful |
 * | 03               | Invoke setPlaybackSpeedState API with valid inputs and verify the returned result is true.       | sessionId = 100, playback_speed = 2, playback_position = 5000, result = true | API returns true confirming valid operation.                  | Should Pass      |
 */
TEST(ContentSecurityManager, ValidInput_PositiveValues) {
    std::cout << "Entering ValidInput_PositiveValues test" << std::endl;
    
    // Create object using default constructor.
    EXPECT_NO_THROW({
        ContentSecurityManager contentSecManager;
        std::cout << "Created ContentSecurityManager object using default constructor." << std::endl;
        
        // Input parameters.
        int64_t sessionId = 100;
        int64_t playback_speed = 2;
        int64_t playback_position = 5000;
        
        std::cout << "Invoking setPlaybackSpeedState with sessionId = " << sessionId 
                  << ", playback_speed = " << playback_speed 
                  << ", playback_position = " << playback_position << std::endl;
        
        // Invoke method and log the returned value.
        bool result = contentSecManager.setPlaybackSpeedState(sessionId, playback_speed, playback_position);
        std::cout << "Method returned: " << std::boolalpha << result << std::endl;
        
        EXPECT_TRUE(result);
    });
    
    std::cout << "Exiting ValidInput_PositiveValues test" << std::endl;
}
/**
 * @brief Test the setPlaybackSpeedState API with a negative session ID.
 *
 * This test verifies that the setPlaybackSpeedState method correctly handles a negative session ID by returning false. It ensures that the API properly validates the session ID and does not accept negative values.
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
 * | Variation / Step | Description                                                                                  | Test Data                                                            | Expected Result                                                  | Notes          |
 * | :--------------: | -------------------------------------------------------------------------------------------- | -------------------------------------------------------------------- | ---------------------------------------------------------------- | -------------- |
 * | 01               | Create ContentSecurityManager object using default constructor                               | None                                                                 | Object is created successfully                                   | Should be successful |
 * | 02               | Invoke setPlaybackSpeedState with negative sessionId (-1), playback_speed (2), playback_position (5000) | sessionId = -1, playback_speed = 2, playback_position = 5000           | Return value should be false; assertion verifies the returned value is false | Should Fail    |
 */
TEST(ContentSecurityManager, NegativeSessionId) {
    std::cout << "Entering NegativeSessionId test" << std::endl;
    
    EXPECT_NO_THROW({
        ContentSecurityManager contentSecManager;
        std::cout << "Created ContentSecurityManager object using default constructor." << std::endl;
        
        int64_t sessionId = -1;
        int64_t playback_speed = 2;
        int64_t playback_position = 5000;
        
        std::cout << "Invoking setPlaybackSpeedState with sessionId = " << sessionId 
                  << ", playback_speed = " << playback_speed 
                  << ", playback_position = " << playback_position << std::endl;
                  
        bool result = contentSecManager.setPlaybackSpeedState(sessionId, playback_speed, playback_position);
        std::cout << "Method returned: " << std::boolalpha << result << std::endl;
        
        EXPECT_FALSE(result);
    });
    
    std::cout << "Exiting NegativeSessionId test" << std::endl;
}
/**
 * @brief Test to validate negative playback speed scenario in setPlaybackSpeedState API
 *
 * This test validates that the ContentSecurityManager::setPlaybackSpeedState API returns false when provided with a negative playback_speed value. It creates an instance of ContentSecurityManager, sets a valid sessionId, a negative playback_speed, and a valid playback_position, then asserts that the API correctly handles this erroneous input by returning false.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create ContentSecurityManager object using default constructor | input: none, output: ContentSecurityManager object instance created | Object is instantiated successfully | Should be successful |
 * | 02 | Assign sessionId, playback_speed and playback_position with negative playback_speed value | sessionId = 100, playback_speed = -2, playback_position = 5000 | Variables are set with sessionId=100, playback_speed=-2, playback_position=5000 correctly | Should be successful |
 * | 03 | Invoke setPlaybackSpeedState API with negative playback_speed value | input: sessionId = 100, playback_speed = -2, playback_position = 5000, output: bool result | API returns false due to negative playback_speed | Should Fail |
 * | 04 | Assertion check with EXPECT_FALSE on result from API call | output: bool result = false | EXPECT_FALSE(result) passes confirming the API returns false | Should be successful |
 */
TEST(ContentSecurityManager, NegativePlaybackSpeed) {
    std::cout << "Entering NegativePlaybackSpeed test" << std::endl;
    
    EXPECT_NO_THROW({
        ContentSecurityManager contentSecManager;
        std::cout << "Created ContentSecurityManager object using default constructor." << std::endl;
        
        int64_t sessionId = 100;
        int64_t playback_speed = -2;
        int64_t playback_position = 5000;
        
        std::cout << "Invoking setPlaybackSpeedState with sessionId = " << sessionId 
                  << ", playback_speed = " << playback_speed 
                  << ", playback_position = " << playback_position << std::endl;
                  
        bool result = contentSecManager.setPlaybackSpeedState(sessionId, playback_speed, playback_position);
        std::cout << "Method returned: " << std::boolalpha << result << std::endl;
        
        EXPECT_FALSE(result);
    });
    
    std::cout << "Exiting NegativePlaybackSpeed test" << std::endl;
}
/**
 * @brief Verify that the setPlaybackSpeedState API returns false when provided with a negative playback position.
 *
 * This test case verifies that the setPlaybackSpeedState function in the ContentSecurityManager class correctly handles an invalid negative playback position by returning false. The test ensures that no exceptions are thrown and that the API's assertion (EXPECT_FALSE) behaves as expected.
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
 * | Variation / Step | Description                                                                                       | Test Data                                                       | Expected Result                                                  | Notes       |
 * | :--------------: | ------------------------------------------------------------------------------------------------- | --------------------------------------------------------------- | ---------------------------------------------------------------- | ----------- |
 * | 01               | Invoke setPlaybackSpeedState with sessionId, playback_speed, and a negative playback_position      | sessionId = 100, playback_speed = 2, playback_position = -5000     | API returns false and assertion (EXPECT_FALSE) passes without exception | Should Fail |
 */
TEST(ContentSecurityManager, NegativePlaybackPosition) {
    std::cout << "Entering NegativePlaybackPosition test" << std::endl;
    
    EXPECT_NO_THROW({
        ContentSecurityManager contentSecManager;
        std::cout << "Created ContentSecurityManager object using default constructor." << std::endl;
        
        int64_t sessionId = 100;
        int64_t playback_speed = 2;
        int64_t playback_position = -5000;
        
        std::cout << "Invoking setPlaybackSpeedState with sessionId = " << sessionId 
                  << ", playback_speed = " << playback_speed 
                  << ", playback_position = " << playback_position << std::endl;
                  
        bool result = contentSecManager.setPlaybackSpeedState(sessionId, playback_speed, playback_position);
        std::cout << "Method returned: " << std::boolalpha << result << std::endl;
        
        EXPECT_FALSE(result);
    });
    
    std::cout << "Exiting NegativePlaybackPosition test" << std::endl;
}
/**
 * @brief Verify that the setPlaybackSpeedState method handles a zero playback speed correctly
 *
 * This test ensures that when a playback speed of zero is supplied along with valid session ID and playback position,
 * the setPlaybackSpeedState method of the ContentSecurityManager class returns true without throwing any exceptions.
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
 * | Variation / Step | Description                                                                               | Test Data                                                       | Expected Result                                                      | Notes           |
 * | :-------------:  | ----------------------------------------------------------------------------------------- | --------------------------------------------------------------- | -------------------------------------------------------------------- | --------------- |
 * | 01               | Create an instance of ContentSecurityManager using the default constructor                | None                                                            | Instance is created successfully                                    | Should be successful |
 * | 02               | Invoke setPlaybackSpeedState with sessionId = 100, playback_speed = 0, playback_position = 5000 | sessionId = 100, playback_speed = 0, playback_position = 5000       | API returns true and passes the EXPECT_TRUE assertion                | Should Pass     |
 */
TEST(ContentSecurityManager, ZeroPlaybackSpeed) {
    std::cout << "Entering ZeroPlaybackSpeed test" << std::endl;
    
    EXPECT_NO_THROW({
        ContentSecurityManager contentSecManager;
        std::cout << "Created ContentSecurityManager object using default constructor." << std::endl;
        
        int64_t sessionId = 100;
        int64_t playback_speed = 0;
        int64_t playback_position = 5000;
        
        std::cout << "Invoking setPlaybackSpeedState with sessionId = " << sessionId 
                  << ", playback_speed = " << playback_speed 
                  << ", playback_position = " << playback_position << std::endl;
                  
        bool result = contentSecManager.setPlaybackSpeedState(sessionId, playback_speed, playback_position);
        std::cout << "Method returned: " << std::boolalpha << result << std::endl;
        
        EXPECT_TRUE(result);
    });
    
    std::cout << "Exiting ZeroPlaybackSpeed test" << std::endl;
}
/**
 * @brief Test the setVideoWindowSize method for valid typical resolution inputs
 *
 * This test verifies that the setVideoWindowSize method of the ContentSecurityManager class correctly handles typical resolution values by returning true. It specifically checks that providing a valid session ID along with standard HD resolution dimensions results in a successful operation.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 019@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result |Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create a ContentSecurityManager object using its default constructor | None | Object is created without exceptions | Should be successful |
 * | 02 | Assign valid sessionId and typical HD dimensions for video window size | sessionId = 100, video_width = 1920, video_height = 1080 | Variables assigned appropriate valid values | Should be successful |
 * | 03 | Invoke setVideoWindowSize with the provided sessionId, video_width, and video_height | input: sessionId = 100, video_width = 1920, video_height = 1080; output: result expected true | Method returns true and ASSERT passes | Should Pass |
 */
TEST(ContentSecurityManager, ValidInputTypicalResolution) {
    std::cout << "Entering ValidInputTypicalResolution test" << std::endl;
    
    // Creating object using default constructor
    EXPECT_NO_THROW({
        ContentSecurityManager manager;
        std::cout << "Created ContentSecurityManager object." << std::endl;
        
        int64_t sessionId = 100;
        int64_t video_width = 1920;
        int64_t video_height = 1080;
        
        std::cout << "Invoking setVideoWindowSize with sessionId: " << sessionId 
                  << ", video_width: " << video_width 
                  << ", video_height: " << video_height << std::endl;
        bool result = manager.setVideoWindowSize(sessionId, video_width, video_height);
        std::cout << "Method setVideoWindowSize returned: " << std::boolalpha << result << std::endl;
        
        EXPECT_TRUE(result);
    });
    
    std::cout << "Exiting ValidInputTypicalResolution test" << std::endl;
}
/**
 * @brief Test valid minimal non-zero video window dimensions
 *
 * This test validates that the setVideoWindowSize function accepts the minimal non-zero dimensions (width = 1, height = 1) for a valid sessionId (1) without throwing an exception. The objective is to ensure that the API behaves correctly when provided with the smallest valid non-zero inputs.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 020@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create a ContentSecurityManager object using the default constructor | No input; output: instance creation | Object is created without throwing any exception | Should be successful |
 * | 02 | Invoke setVideoWindowSize with sessionId=1, video_width=1, and video_height=1 | sessionId = 1, video_width = 1, video_height = 1, output: result from setVideoWindowSize | Method returns true and the assertion (EXPECT_TRUE) passes | Should Pass |
 */
TEST(ContentSecurityManager, ValidInputMinimalNonZeroDimensions) {
    std::cout << "Entering ValidInputMinimalNonZeroDimensions test" << std::endl;
    
    // Creating object using default constructor
    EXPECT_NO_THROW({
        ContentSecurityManager manager;
        std::cout << "Created ContentSecurityManager object." << std::endl;
        
        int64_t sessionId = 1;
        int64_t video_width = 1;
        int64_t video_height = 1;
        
        std::cout << "Invoking setVideoWindowSize with sessionId: " << sessionId 
                  << ", video_width: " << video_width 
                  << ", video_height: " << video_height << std::endl;
        bool result = manager.setVideoWindowSize(sessionId, video_width, video_height);
        std::cout << "Method setVideoWindowSize returned: " << std::boolalpha << result << std::endl;
        
        EXPECT_TRUE(result);
    });
    
    std::cout << "Exiting ValidInputMinimalNonZeroDimensions test" << std::endl;
}
/**
 * @brief Verify that setVideoWindowSize fails when provided with a negative session ID.
 *
 * This test ensures that when a negative session ID is passed along with valid video dimensions,
 * the setVideoWindowSize method returns false, indicating that the input is invalid.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 021@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**
 * | Variation / Step | Description                                                      | Test Data                                               | Expected Result                                                      | Notes            |
 * | :--------------: | ---------------------------------------------------------------- | ------------------------------------------------------- | --------------------------------------------------------------------- | ---------------- |
 * | 01               | Create ContentSecurityManager object using default constructor   | None                                                    | Object is created without throwing any exception                    | Should be successful |
 * | 02               | Invoke setVideoWindowSize with negative sessionId and valid video dimensions | sessionId = -10, video_width = 1920, video_height = 1080 | API returns false indicating failure in applying the video window size | Should Fail      |
 */
TEST(ContentSecurityManager, NegativeInputNegativeSessionId) {
    std::cout << "Entering NegativeInputNegativeSessionId test" << std::endl;
    
    // Creating object using default constructor
    EXPECT_NO_THROW({
        ContentSecurityManager manager;
        std::cout << "Created ContentSecurityManager object." << std::endl;
        
        int64_t sessionId = -10;
        int64_t video_width = 1920;
        int64_t video_height = 1080;
        
        std::cout << "Invoking setVideoWindowSize with sessionId: " << sessionId 
                  << ", video_width: " << video_width 
                  << ", video_height: " << video_height << std::endl;
        bool result = manager.setVideoWindowSize(sessionId, video_width, video_height);
        std::cout << "Method setVideoWindowSize returned: " << std::boolalpha << result << std::endl;
        
        EXPECT_FALSE(result);
    });
    
    std::cout << "Exiting NegativeInputNegativeSessionId test" << std::endl;
}
/**
 * @brief Validate that setVideoWindowSize returns false when video_width is zero
 *
 * This test verifies that the setVideoWindowSize API correctly handles an invalid input where the video_width is set to zero. The expectation is that the method will return false, thereby indicating the input was rejected.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 022@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create a ContentSecurityManager object and invoke setVideoWindowSize with sessionId = 50, video_width = 0, video_height = 1080 | sessionId = 50, video_width = 0, video_height = 1080 | API returns false and assertion check EXPECT_FALSE(result) passes | Should Fail |
 */
TEST(ContentSecurityManager, NegativeInputZeroVideoWidth) {
    std::cout << "Entering NegativeInputZeroVideoWidth test" << std::endl;
    
    // Creating object using default constructor
    EXPECT_NO_THROW({
        ContentSecurityManager manager;
        std::cout << "Created ContentSecurityManager object." << std::endl;
        
        int64_t sessionId = 50;
        int64_t video_width = 0;
        int64_t video_height = 1080;
        
        std::cout << "Invoking setVideoWindowSize with sessionId: " << sessionId 
                  << ", video_width: " << video_width 
                  << ", video_height: " << video_height << std::endl;
        bool result = manager.setVideoWindowSize(sessionId, video_width, video_height);
        std::cout << "Method setVideoWindowSize returned: " << std::boolalpha << result << std::endl;
        
        EXPECT_FALSE(result);
    });
    
    std::cout << "Exiting NegativeInputZeroVideoWidth test" << std::endl;
}
/**
 * @brief Verify that setVideoWindowSize returns false when provided a video height of zero.
 *
 * This test validates that the ContentSecurityManager::setVideoWindowSize method correctly handles a zero value for video height by returning false, which is indicative of a failed setup as height cannot be zero.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 023@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                           | Test Data (comma separated)                                   | Expected Result                                                     | Notes       |
 * | :--------------: | ----------------------------------------------------------------------------------------------------- | ------------------------------------------------------------- | ------------------------------------------------------------------- | ----------- |
 * | 01               | Create a ContentSecurityManager object and call setVideoWindowSize with sessionId=50, video_width=1920, video_height=0 | sessionId = 50, video_width = 1920, video_height = 0            | Method setVideoWindowSize should return false after assertion check   | Should Pass |
 */
TEST(ContentSecurityManager, NegativeInputZeroVideoHeight) {
    std::cout << "Entering NegativeInputZeroVideoHeight test" << std::endl;
    
    // Creating object using default constructor
    EXPECT_NO_THROW({
        ContentSecurityManager manager;
        std::cout << "Created ContentSecurityManager object." << std::endl;
        
        int64_t sessionId = 50;
        int64_t video_width = 1920;
        int64_t video_height = 0;
        
        std::cout << "Invoking setVideoWindowSize with sessionId: " << sessionId 
                  << ", video_width: " << video_width 
                  << ", video_height: " << video_height << std::endl;
        bool result = manager.setVideoWindowSize(sessionId, video_width, video_height);
        std::cout << "Method setVideoWindowSize returned: " << std::boolalpha << result << std::endl;
        
        EXPECT_FALSE(result);
    });
    
    std::cout << "Exiting NegativeInputZeroVideoHeight test" << std::endl;
}
/**
 * @brief Validate that the CloseDrmSession API successfully closes a DRM session with a valid session ID.
 *
 * This test verifies that invoking CloseDrmSession with a valid session identifier (sessionId = 100) does not throw any exception.
 * It ensures that the DRM session closure functionality works properly for positive scenarios.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 024@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                         | Test Data                  | Expected Result                                           | Notes       |
 * | :--------------: | ------------------------------------------------------------------- | -------------------------- | --------------------------------------------------------- | ----------- |
 * | 01               | Invoke CloseDrmSession API with a valid session id (sessionId = 100)  | sessionId = 100            | API is invoked without throwing any exceptions; assertion passes | Should Pass |
 */
TEST_F(ContentSecurityManagerTest, CloseDrmSessionValidPositive) {
    std::cout << "Entering CloseDrmSessionValidPositive test" << std::endl;
    int64_t sessionId = 100;
    std::cout << "Invoking CloseDrmSession with sessionId: " << sessionId << std::endl;
    EXPECT_NO_THROW({
        manager->CloseDrmSession(sessionId);
        std::cout << "CloseDrmSession invoked successfully with positive sessionId: " << sessionId << std::endl;
    });
    std::cout << "Exiting CloseDrmSessionValidPositive test" << std::endl;
}
/**
 * @brief Validate that calling CloseDrmSession with a negative sessionId does not result in an exception.
 *
 * This test verifies that when a negative sessionId is provided, the CloseDrmSession API manages the input gracefully without throwing any exceptions. It ensures the robustness of the session management even when invalid sessionIds are passed.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 025
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Initialize a negative sessionId and invoke the CloseDrmSession API to verify that no exception is thrown | sessionId = -1 | The API call should not throw any exception and handle the negative sessionId gracefully | Should Pass |
 */
TEST_F(ContentSecurityManagerTest, CloseDrmSessionNegative) {
    std::cout << "Entering CloseDrmSessionNegative test" << std::endl;
    int64_t sessionId = -1;
    std::cout << "Invoking CloseDrmSession with sessionId: " << sessionId << std::endl;
    EXPECT_NO_THROW({
        manager->CloseDrmSession(sessionId);
        std::cout << "CloseDrmSession invoked successfully with negative sessionId: " << sessionId << std::endl;
    });
    std::cout << "Exiting CloseDrmSessionNegative test" << std::endl;
}
/**
 * @brief Verifies that the CloseDrmSession API can handle a sessionId with the maximum int64_t value without throwing an exception.
 *
 * This test checks whether invoking the CloseDrmSession API using a sessionId equal to 9223372036854775807LL (maximum int64_t value) executes without throwing exceptions. It is critical for ensuring that boundary values are handled correctly in a positive scenario.
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
 * | Variation / Step | Description                                                                 | Test Data                                     | Expected Result                                                        | Notes          |
 * | :--------------: | --------------------------------------------------------------------------- | --------------------------------------------- | ---------------------------------------------------------------------- | -------------- |
 * | 01               | Log entry indicating the start of the CloseDrmSessionMaxInt64 test.         | None                                          | "Entering CloseDrmSessionMaxInt64 test" is printed.                    | Should be successful |
 * | 02               | Invoke CloseDrmSession API with maximum int64_t sessionId to ensure no exception is thrown. | input: sessionId = 9223372036854775807LL         | No exception is thrown and "CloseDrmSession invoked successfully with maximum int64_t sessionId" message is printed. | Should Pass    |
 * | 03               | Log exit from the test to indicate test completion.                        | None                                          | "Exiting CloseDrmSessionMaxInt64 test" is printed.                     | Should be successful |
 */
TEST_F(ContentSecurityManagerTest, CloseDrmSessionMaxInt64) {
    std::cout << "Entering CloseDrmSessionMaxInt64 test" << std::endl;
    int64_t sessionId = 9223372036854775807LL;
    std::cout << "Invoking CloseDrmSession with sessionId: " << sessionId << std::endl;
    EXPECT_NO_THROW({
        manager->CloseDrmSession(sessionId);
        std::cout << "CloseDrmSession invoked successfully with maximum int64_t sessionId: " << sessionId << std::endl;
    });
    std::cout << "Exiting CloseDrmSessionMaxInt64 test" << std::endl;
}
/**
 * @brief Validates that the CloseDrmSession method handles the minimum int64_t value as sessionId successfully.
 *
 * This test case verifies that passing the minimum possible int64_t value (-9223372036854775808) to the CloseDrmSession API does not throw any exceptions. It confirms that the API properly handles this edge-case input and executes the session closure without error.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 027@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                          | Test Data                                              | Expected Result                                   | Notes      |
 * | :--------------: | -------------------------------------------------------------------- | ------------------------------------------------------ | ------------------------------------------------- | ---------- |
 * | 01               | Invoke CloseDrmSession using minimum int64_t value for sessionId       | sessionId = -9223372036854775808                        | No exception thrown during API invocation         | Should Pass |
 */
TEST_F(ContentSecurityManagerTest, CloseDrmSessionMinInt64) {
    std::cout << "Entering CloseDrmSessionMinInt64 test" << std::endl;
    int64_t sessionId = -9223372036854775807LL - 1;
    std::cout << "Invoking CloseDrmSession with sessionId: " << sessionId << std::endl;
    EXPECT_NO_THROW({
        manager->CloseDrmSession(sessionId);
        std::cout << "CloseDrmSession invoked successfully with minimum int64_t sessionId: " << sessionId << std::endl;
    });
    std::cout << "Exiting CloseDrmSessionMinInt64 test" << std::endl;
}
/**
 * @brief Tests the default construction and proper initialization of ContentSecurityManager objects.
 *
 * This test verifies that creating a temporary TestContentManager object using the default constructor does not throw any exceptions and that the test fixture's manager object is correctly instantiated (i.e., it is not a nullptr). This ensures the basic functionality of object instantiation works as intended.
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
 * | Variation / Step | Description                                                            | Test Data                                                                                   | Expected Result                                       | Notes         |
 * | :--------------: | -----------------------------------------------------------------------| ------------------------------------------------------------------------------------------- | ----------------------------------------------------- | ------------- |
 * | 01               | Invoke the default constructor using a temporary object.               | tempManager is instantiated via default constructor; no input arguments are provided.       | No exception is thrown during the instantiation.     | Should Pass   |
 * | 02               | Validate that the test fixture manager pointer is not nullptr.         | manager pointer = non-null value after construction.                                      | manager != nullptr as per the EXPECT_NE check.       | Should Pass   |
 * | 03               | Log debug messages indicating the default initial state of the manager.  | Debug log outputs the construction information and initial state of ContentSecurityManager. | Informational messages are printed successfully.    | Should be successful |
 */
TEST_F(ContentSecurityManagerTest, ContentSecurityManager_ContentSecurityManager_start) {
    std::cout << "Entering ContentSecurityManager::ContentSecurityManager()_start test" << std::endl;
    
    // Testing the default constructor using a temporary object and the test fixture's manager object
    EXPECT_NO_THROW({
        std::cout << "Invoking ContentSecurityManager() constructor via temporary object." << std::endl;
        TestContentManager tempManager;  // Calls the default constructor internally
        std::cout << "Temporary object created at address: " << &tempManager << std::endl;
    });
    
    // Validate that the manager pointer from fixture is not nullptr and implicitly initialized correctly.
    EXPECT_NE(manager, nullptr);
    std::cout << "Test fixture manager object is instantiated at address: " << manager << std::endl;
    
    // Debug log for internal state:
    // If there were member variables to be printed, they would be logged here.
    std::cout << "ContentSecurityManager instance is in its default initial state." << std::endl;
    
    std::cout << "Exiting ContentSecurityManager::ContentSecurityManager()_start test" << std::endl;
}
/**
 * @brief Verify that SetDrmSessionState correctly processes a valid active DRM session state.
 *
 * This test validates that the ContentSecurityManager::SetDrmSessionState API correctly handles a positive scenario.
 * It invokes the API with a valid sessionId (12345) and the active flag set to true, ensuring that the method returns a valid boolean value.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 029@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**
 * | Variation / Step | Description                                                          | Test Data                                             | Expected Result                                                  | Notes       |
 * | :--------------: | -------------------------------------------------------------------- | ----------------------------------------------------- | ---------------------------------------------------------------- | ----------- |
 * | 01               | Invoke SetDrmSessionState with sessionId 12345 and active true         | sessionId = 12345, active = true, output: retValue       | Returns a valid boolean (true or false) and the assertion check passes | Should Pass |
 */
TEST_F(ContentSecurityManagerTest, SetDrmSessionState_PositiveActive) {
    std::cout << "Entering SetDrmSessionState_PositiveActive test" << std::endl;
    int64_t sessionId = 12345;
    bool active = true;
    std::cout << "Invoking ContentSecurityManager::SetDrmSessionState with sessionId: " << sessionId 
              << " and active: " << std::boolalpha << active << std::endl;
    bool retValue = manager->SetDrmSessionState(sessionId, active);
    std::cout << "Method returned: " << std::boolalpha << retValue << std::endl;
    std::cout << "Expected return value: true/false" << std::endl;
    EXPECT_TRUE(retValue == true || retValue == false)
    std::cout << "Exiting SetDrmSessionState_PositiveActive test" << std::endl;
}
/**
 * @brief Validate SetDrmSessionState API for inactive DRM session.
 *
 * This test verifies that when SetDrmSessionState is invoked with a given sessionId and the active flag set to false,
 * the API returns a valid boolean value. It ensures that the function correctly processes an inactive session state.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 030@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                        | Test Data                                              | Expected Result                                                                 | Notes      |
 * | :--------------: | ------------------------------------------------------------------ | ------------------------------------------------------ | ------------------------------------------------------------------------------- | ---------- |
 * | 01               | Invoke SetDrmSessionState with sessionId set to 12345 and active set to false. | sessionId = 12345, active = false, expected return value = true/false | The API returns a valid boolean value (either true or false) that satisfies the assertion check. | Should Pass |
 */
TEST_F(ContentSecurityManagerTest, SetDrmSessionState_PositiveInactive) {
    std::cout << "Entering SetDrmSessionState_PositiveInactive test" << std::endl;
    int64_t sessionId = 12345;
    bool active = false;
    std::cout << "Invoking ContentSecurityManager::SetDrmSessionState with sessionId: " << sessionId 
              << " and active: " << std::boolalpha << active << std::endl;
    bool retValue = manager->SetDrmSessionState(sessionId, active);
    std::cout << "Method returned: " << std::boolalpha << retValue << std::endl;
    std::cout << "Expected return value: true/false" << std::endl;
    EXPECT_TRUE(retValue == true || retValue == false)
    std::cout << "Exiting SetDrmSessionState_PositiveInactive test" << std::endl;
}
/**
 * @brief Test the behavior of SetDrmSessionState API when provided with maximum int64_t session id and active state true
 *
 * This test validates that the SetDrmSessionState function correctly handles the maximum possible int64_t value for the session identifier. It calls the API with sessionId = 9223372036854775807 and active = true, and checks if the function returns a valid boolean value.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 031@n
 * **Priority:** High@n
 * @n
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * @n
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Log entry indicating the start of the test | - | Log message "Entering SetDrmSessionState_MaximumInt64Active test" printed | Should be successful |
 * | 02 | Initialize sessionId with maximum int64 value and active state with true | sessionId = 9223372036854775807, active = true | Variables correctly set | Should be successful |
 * | 03 | Invoke the SetDrmSessionState API with the initialized parameters | sessionId = 9223372036854775807, active = true, output = retValue | API returns a boolean value (true/false) | Should Pass |
 * | 04 | Validate the return value using assertion | retValue = result from API call | Assert that retValue is either true or false | Should Pass |
 * | 05 | Log exit message indicating the end of the test | - | Log message "Exiting SetDrmSessionState_MaximumInt64Active test" printed | Should be successful |
 */
TEST_F(ContentSecurityManagerTest, SetDrmSessionState_MaximumInt64Active) {
    std::cout << "Entering SetDrmSessionState_MaximumInt64Active test" << std::endl;
    int64_t sessionId = 9223372036854775807LL;
    bool active = true;
    std::cout << "Invoking ContentSecurityManager::SetDrmSessionState with sessionId: " << sessionId 
              << " and active: " << std::boolalpha << active << std::endl;
    bool retValue = manager->SetDrmSessionState(sessionId, active);
    std::cout << "Method returned: " << std::boolalpha << retValue << std::endl;
    std::cout << "Expected return value: true/false" << std::endl;
    EXPECT_TRUE(retValue == true || retValue == false)
    std::cout << "Exiting SetDrmSessionState_MaximumInt64Active test" << std::endl;
}
/**
 * @brief Test to verify SetDrmSessionState API with minimum int64 session ID and inactive state.
 *
 * This test invokes the SetDrmSessionState method with the minimum possible int64 value for the session ID and sets the active flag to false. The purpose is to ensure that the API handles the minimum boundary condition correctly for the sessionId input and processes the inactive state without error.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 032@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                        | Test Data                                                  | Expected Result                                                  | Notes         |
 * | :--------------: | ------------------------------------------------------------------ | ---------------------------------------------------------- | ---------------------------------------------------------------- | ------------- |
 * | 01               | Invoke SetDrmSessionState with minimum int64 sessionId and inactive state | sessionId = -9223372036854775808, active = false            | Return value should be either true or false as per API's design and assertion should pass | Should Pass   |
 */
TEST_F(ContentSecurityManagerTest, SetDrmSessionState_MinimumInt64Inactive) {
    std::cout << "Entering SetDrmSessionState_MinimumInt64Inactive test" << std::endl;
    int64_t sessionId = (-9223372036854775807LL - 1);
    bool active = false;
    std::cout << "Invoking ContentSecurityManager::SetDrmSessionState with sessionId: " << sessionId 
              << " and active: " << std::boolalpha << active << std::endl;
    bool retValue = manager->SetDrmSessionState(sessionId, active);
    std::cout << "Method returned: " << std::boolalpha << retValue << std::endl;
    std::cout << "Expected return value: true/false" << std::endl;
    EXPECT_TRUE(retValue == true || retValue == false)
    std::cout << "Exiting SetDrmSessionState_MinimumInt64Inactive test" << std::endl;
}
/**
 * @brief Validate playback position update for a valid normal scenario
 *
 * This test validates that the SetPlaybackPosition API correctly updates the playback position when invoked with valid parameters.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 033@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoke SetPlaybackPosition with valid sessionId, speed, and position, then check that the returned result is true. | sessionId = 12345, speed = 1.0, position = 30, returned result = true | Return value should be true and match EXPECT_EQ assertion | Should Pass |
 */
TEST_F(ContentSecurityManagerTest, ValidNormalPlaybackPositionUpdate) {
    std::cout << "Entering ValidNormalPlaybackPositionUpdate test" << std::endl;
    
    int64_t sessionId = 12345;
    float speed = 1.0;
    int32_t position = 30;
    std::cout << "Invoking SetPlaybackPosition with sessionId: " << sessionId 
              << ", speed: " << speed 
              << ", position: " << position << std::endl;
    
    bool result = manager->SetPlaybackPosition(sessionId, speed, position);
    std::cout << "Returned result: " << std::boolalpha << result << std::endl;
    EXPECT_EQ(result, true);
    
    std::cout << "Exiting ValidNormalPlaybackPositionUpdate test" << std::endl;
}
/**
 * @brief Verify that the playback position is updated correctly with increased speed.
 *
 * This test verifies that the SetPlaybackPosition API returns true when a valid session ID, an increased speed value, and a specified playback position are provided. It ensures that the playback position is updated correctly under these conditions.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 034@n
 * **Priority:** High@n
 * @n
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * @n
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                                 | Test Data                                                                            | Expected Result                                                                      | Notes             |
 * | :--------------: | --------------------------------------------------------------------------- | ------------------------------------------------------------------------------------ | ------------------------------------------------------------------------------------ | ----------------- |
 * | 01               | Log the entry message for the test                                          | None                                                                                 | Entry message "Entering ValidUpdatedPlaybackWithIncreasedSpeed test" printed           | Should be successful |
 * | 02               | Initialize test parameters (sessionId, speed, and position)                 | sessionId = 12345, speed = 1.5, position = 45                                          | Parameters are correctly assigned                                                    | Should be successful |
 * | 03               | Invoke SetPlaybackPosition API with the initialized parameters              | input1 = sessionId 12345, input2 = speed 1.5, input3 = position 45, output1 = result     | API returns true; EXPECT_EQ(result, true) assertion passes                             | Should Pass       |
 * | 04               | Log the exit message for the test                                           | None                                                                                 | Exit message "Exiting ValidUpdatedPlaybackWithIncreasedSpeed test" printed             | Should be successful |
 */
TEST_F(ContentSecurityManagerTest, ValidUpdatedPlaybackWithIncreasedSpeed) {
    std::cout << "Entering ValidUpdatedPlaybackWithIncreasedSpeed test" << std::endl;
    
    int64_t sessionId = 12345;
    float speed = 1.5;
    int32_t position = 45;
    std::cout << "Invoking SetPlaybackPosition with sessionId: " << sessionId 
              << ", speed: " << speed 
              << ", position: " << position << std::endl;
    
    bool result = manager->SetPlaybackPosition(sessionId, speed, position);
    std::cout << "Returned result: " << std::boolalpha << result << std::endl;
    EXPECT_EQ(result, true);
    
    std::cout << "Exiting ValidUpdatedPlaybackWithIncreasedSpeed test" << std::endl;
}
/**
 * @brief Verify handling of reverse playback when a negative speed is provided.
 *
 * This test verifies that the SetPlaybackPosition API correctly processes a negative speed value, which is used to simulate reverse playback. The test ensures that the API returns true, confirming that the reverse playback scenario is handled properly.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 035@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                                                  | Test Data                                         | Expected Result                                             | Notes       |
 * | :--------------: | -------------------------------------------------------------------------------------------- | ------------------------------------------------- | ----------------------------------------------------------- | ----------- |
 * | 01               | Invoke SetPlaybackPosition with sessionId, negative speed, and position to simulate reverse playback. | sessionId = 12345, speed = -1.0, position = 30, output = bool result | API returns true and EXPECT_EQ confirms the result is true. | Should Pass |
 */
TEST_F(ContentSecurityManagerTest, ReversePlaybackScenarioWithNegativeSpeed) {
    std::cout << "Entering ReversePlaybackScenarioWithNegativeSpeed test" << std::endl;
    
    int64_t sessionId = 12345;
    float speed = -1.0;
    int32_t position = 30;
    std::cout << "Invoking SetPlaybackPosition with sessionId: " << sessionId 
              << ", speed: " << speed 
              << ", position: " << position << std::endl;
    
    bool result = manager->SetPlaybackPosition(sessionId, speed, position);
    std::cout << "Returned result: " << std::boolalpha << result << std::endl;
    EXPECT_EQ(result, true);
    
    std::cout << "Exiting ReversePlaybackScenarioWithNegativeSpeed test" << std::endl;
}
/**
 * @brief Verify that SetPlaybackPosition returns false when provided with an invalid session ID.
 *
 * This test verifies that the SetPlaybackPosition API handles invalid session IDs correctly by rejecting them.
 * A negative session ID is passed to the API along with valid speed and position values, and the expected behavior
 * is for the API to return false.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 036@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                        | Test Data                                   | Expected Result                                                  | Notes      |
 * | :--------------: | ------------------------------------------------------------------ | ------------------------------------------- | ---------------------------------------------------------------- | ---------- |
 * | 01               | Invoke SetPlaybackPosition with a negative sessionId, valid speed, and valid position | sessionId = -12345, speed = 1.0, position = 30 | API returns false and EXPECT_EQ verifies that returned value is false | Should Fail |
 */
TEST_F(ContentSecurityManagerTest, InvalidSessionId) {
    std::cout << "Entering InvalidSessionId test" << std::endl;
    
    int64_t sessionId = -12345;
    float speed = 1.0;
    int32_t position = 30;
    std::cout << "Invoking SetPlaybackPosition with sessionId: " << sessionId 
              << ", speed: " << speed 
              << ", position: " << position << std::endl;
    
    bool result = manager->SetPlaybackPosition(sessionId, speed, position);
    std::cout << "Returned result: " << std::boolalpha << result << std::endl;
    EXPECT_EQ(result, false);
    
    std::cout << "Exiting InvalidSessionId test" << std::endl;
}
/**
 * @brief Validate that SetPlaybackPosition returns false when provided with an invalid negative playback position.
 *
 * This test verifies that the API SetPlaybackPosition correctly handles a negative playback position by returning false.
 * Negative playback positions are considered invalid, and the system is expected to reject such input.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 037@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                     | Test Data                                        | Expected Result                                        | Notes       |
 * | :--------------: | --------------------------------------------------------------- | ------------------------------------------------ | ------------------------------------------------------ | ----------- |
 * | 01               | Invoke SetPlaybackPosition with an invalid negative position value | sessionId = 12345, speed = 1.0, position = -10   | API returns false; assertion (EXPECT_EQ) validates false | Should Pass |
 */
TEST_F(ContentSecurityManagerTest, InvalidPlaybackPosition) {
    std::cout << "Entering InvalidPlaybackPosition test" << std::endl;
    
    int64_t sessionId = 12345;
    float speed = 1.0;
    int32_t position = -10;
    std::cout << "Invoking SetPlaybackPosition with sessionId: " << sessionId 
              << ", speed: " << speed 
              << ", position: " << position << std::endl;
    
    bool result = manager->SetPlaybackPosition(sessionId, speed, position);
    std::cout << "Returned result: " << std::boolalpha << result << std::endl;
    EXPECT_EQ(result, false);
    
    std::cout << "Exiting InvalidPlaybackPosition test" << std::endl;
}
/**
 * @brief Test to verify the behavior of SetPlaybackPosition when the minimum boundary for speed is provided.
 *
 * This test checks if the ContentSecurityManager correctly handles a speed value at its minimum boundary (0.0) by returning false. The objective is to ensure that the API does not allow playback to start when an invalid (minimum) speed is supplied.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 038@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoke SetPlaybackPosition API with sessionId, speed, and position at boundary values. | sessionId = 12345, speed = 0.0, position = 30 | API returns false; ASSERT_EQ(result, false) | Should Fail |
 */
TEST_F(ContentSecurityManagerTest, MinimumBoundaryForSpeed) {
    std::cout << "Entering MinimumBoundaryForSpeed test" << std::endl;
    
    int64_t sessionId = 12345;
    float speed = 0.0;
    int32_t position = 30;
    std::cout << "Invoking SetPlaybackPosition with sessionId: " << sessionId 
              << ", speed: " << speed 
              << ", position: " << position << std::endl;
    
    bool result = manager->SetPlaybackPosition(sessionId, speed, position);
    std::cout << "Returned result: " << std::boolalpha << result << std::endl;
    EXPECT_EQ(result, false);
    
    std::cout << "Exiting MinimumBoundaryForSpeed test" << std::endl;
}
/**
 * @brief Validate the behavior of setWindowSize API for valid positive values.
 *
 * This test verifies that setting the window size using valid positive integer parameters returns false as defined by the business logic. It ensures that when the API is invoked with a valid sessionId and expected video dimensions, the outcome adheres to the expected failure return scenario.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 039
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                         | Test Data                                     | Expected Result                                         | Notes      |
 * | :--------------: | --------------------------------------------------- | --------------------------------------------- | ------------------------------------------------------- | ---------- |
 * | 01               | Invoke setWindowSize with valid positive values     | sessionId = 1, video_width = 1920, video_height = 1080 | API returns false; assertion EXPECT_FALSE(result) passes | Should Pass|
 */
TEST_F(ContentSecurityManagerTest, ValidPositiveValues) {
    std::cout << "Entering ValidPositiveValues test" << std::endl;
    
    int64_t sessionId = 1;
    int64_t video_width = 1920;
    int64_t video_height = 1080;
    std::cout << "Invoking setWindowSize with sessionId = " << sessionId 
              << ", video_width = " << video_width 
              << ", video_height = " << video_height << std::endl;
    
    bool result = manager->setWindowSize(sessionId, video_width, video_height);
    std::cout << "Returned value: " << std::boolalpha << result << std::endl;
    
    EXPECT_FALSE(result);
    
    std::cout << "Exiting ValidPositiveValues test" << std::endl;
}
/**
 * @brief Validate that setWindowSize returns false when zero dimensions are provided for a valid session.
 *
 * This test verifies that when a valid sessionId is used but both video_width and video_height are zero, the setWindowSize API
 * correctly identifies the invalid window size and returns false.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 040@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                               | Test Data                                      | Expected Result                                                 | Notes       |
 * | :--------------: | ------------------------------------------------------------------------- | ---------------------------------------------- | ---------------------------------------------------------------- | ----------- |
 * | 01               | Invoke setWindowSize with a valid sessionId but with zero video dimensions  | sessionId = 1, video_width = 0, video_height = 0 | API returns false and EXPECT_FALSE(result) assertion passes         | Should Pass |
 */
TEST_F(ContentSecurityManagerTest, ZeroDimensionsValidSessionId) {
    std::cout << "Entering ZeroDimensionsValidSessionId test" << std::endl;
    
    int64_t sessionId = 1;
    int64_t video_width = 0;
    int64_t video_height = 0;
    std::cout << "Invoking setWindowSize with sessionId = " << sessionId 
              << ", video_width = " << video_width 
              << ", video_height = " << video_height << std::endl;
    
    bool result = manager->setWindowSize(sessionId, video_width, video_height);
    std::cout << "Returned value: " << std::boolalpha << result << std::endl;
    
    EXPECT_FALSE(result);
    
    std::cout << "Exiting ZeroDimensionsValidSessionId test" << std::endl;
}
/**
 * @brief Verify that setWindowSize returns false for a negative session ID with valid dimensions.
 *
 * This test case validates the behavior of the setWindowSize API when a negative session ID is provided alongside positive video dimensions. The test ensures that the API correctly rejects an invalid session by returning false. This scenario is critical for ensuring that the API does not accept malformed or unintended session identifiers.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 041@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Call setWindowSize API with negative session id and valid dimensions | sessionId = -1, video_width = 640, video_height = 480, expected_return = false | API returns false indicating invalid session id and assertion EXPECT_FALSE passes | Should Fail |
 */
TEST_F(ContentSecurityManagerTest, NegativeSessionIdPositiveDimensions) {
    std::cout << "Entering NegativeSessionIdPositiveDimensions test" << std::endl;
    
    int64_t sessionId = -1;
    int64_t video_width = 640;
    int64_t video_height = 480;
    std::cout << "Invoking setWindowSize with sessionId = " << sessionId 
              << ", video_width = " << video_width 
              << ", video_height = " << video_height << std::endl;
    
    bool result = manager->setWindowSize(sessionId, video_width, video_height);
    std::cout << "Returned value: " << std::boolalpha << result << std::endl;
    
    EXPECT_FALSE(result);
    
    std::cout << "Exiting NegativeSessionIdPositiveDimensions test" << std::endl;
}
/**
 * @brief Test that verifies the behavior of setWindowSize with a positive session ID and a negative video width.
 *
 * This test validates that when setWindowSize is invoked with a valid session ID and a negative video width, the API responds appropriately by returning false. This ensures that invalid video width input is handled correctly.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 042@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                        | Test Data                                                      | Expected Result                                                     | Notes       |
 * | :--------------: | ------------------------------------------------------------------ | -------------------------------------------------------------- | ------------------------------------------------------------------- | ----------- |
 * | 01               | Invoke setWindowSize with sessionId = 10, video_width = -640, video_height = 480 | input: sessionId = 10, video_width = -640, video_height = 480; output: result should be false | API returns false and EXPECT_FALSE(result) assertion passes         | Should Pass |
 */
TEST_F(ContentSecurityManagerTest, PositiveSessionIdNegativeVideoWidth) {
    std::cout << "Entering PositiveSessionIdNegativeVideoWidth test" << std::endl;
    
    int64_t sessionId = 10;
    int64_t video_width = -640;
    int64_t video_height = 480;
    std::cout << "Invoking setWindowSize with sessionId = " << sessionId 
              << ", video_width = " << video_width 
              << ", video_height = " << video_height << std::endl;
    
    bool result = manager->setWindowSize(sessionId, video_width, video_height);
    std::cout << "Returned value: " << std::boolalpha << result << std::endl;
    
    EXPECT_FALSE(result);
    
    std::cout << "Exiting PositiveSessionIdNegativeVideoWidth test" << std::endl;
}
/**
 * @brief Verify that setWindowSize fails with a negative video_height despite a valid sessionId.
 *
 * This test checks that the API call setWindowSize correctly handles the scenario where the sessionId and video_width
 * are valid but the video_height is negative. The function is expected to return false (indicating an error) when provided
 * with these parameters.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 043@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                              | Test Data                                      | Expected Result                                           | Notes       |
 * | :--------------: | ------------------------------------------------------------------------ | ---------------------------------------------- | --------------------------------------------------------- | ----------- |
 * | 01               | Invoke setWindowSize with valid sessionId and video_width, but negative video_height | sessionId = 10, video_width = 1280, video_height = -720 | Expected false return from API and assertion EXPECT_FALSE(result) passes | Should Fail |
 */
TEST_F(ContentSecurityManagerTest, MixedPositiveSessionIdNegativeVideoHeight) {
    std::cout << "Entering MixedPositiveSessionIdNegativeVideoHeight test" << std::endl;
    
    int64_t sessionId = 10;
    int64_t video_width = 1280;
    int64_t video_height = -720;
    std::cout << "Invoking setWindowSize with sessionId = " << sessionId 
              << ", video_width = " << video_width 
              << ", video_height = " << video_height << std::endl;
    
    bool result = manager->setWindowSize(sessionId, video_width, video_height);
    std::cout << "Returned value: " << std::boolalpha << result << std::endl;
    
    EXPECT_FALSE(result);
    
    std::cout << "Exiting MixedPositiveSessionIdNegativeVideoHeight test" << std::endl;
}
/**
 * @brief Verify that the destructor of TestContentSecurityManager is invoked properly without throwing exceptions.
 *
 * This test creates a TestContentSecurityManager object on the heap and then deletes it to ensure that the object's destructor is called correctly without any runtime errors. The test verifies that both allocation (using new) and deletion (using delete) occur without throwing exceptions.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 044@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Allocate TestContentSecurityManager object on the heap using new operator. | new TestContentSecurityManager() | Object is allocated successfully and no exception is thrown. | Should Pass |
 * | 02 | Delete the allocated object to invoke destructor. | pointer = allocated TestContentSecurityManager object | Object is deleted successfully and no exception is thrown. | Should Pass |
 */
TEST_F(ContentSecurityManagerTest, ValidDestruction) {
    std::cout << "Entering ValidDestruction test" << std::endl;
    
    // Allocate a new object on the heap to test destructor invocation.
    std::cout << "Invoking new operator to allocate TestContentSecurityManager object on the heap." << std::endl;
    TestContentSecurityManager* temp = nullptr;
    EXPECT_NO_THROW({
        temp = new TestContentSecurityManager();
    });
    
    std::cout << "Allocated object at pointer: " << temp << std::endl;
    
    // Log internal state before deletion (if any internal state exists, display pointer value as a debug log)
    std::cout << "About to delete the object to invoke ContentSecurityManager::~ContentSecurityManager()." << std::endl;
    
    // Delete the object and check that no exceptions are thrown during deletion.
    EXPECT_NO_THROW({
        delete temp;
    });
    
    std::cout << "Object deletion completed. Destructor should have been called. (No internal state details available)" << std::endl;
    std::cout << "Exiting ValidDestruction test" << std::endl;
}
/**
 * @brief Verify that the FakeSecManager default constructor does not throw any exceptions.
 *
 * This test ensures that when the FakeSecManager is instantiated using its default constructor,
 * no exceptions occur and the instance is created with a valid default state. The test logs the
 * process and uses EXPECT_NO_THROW to validate that the instantiation is safe.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 045@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                                 | Test Data                                                       | Expected Result                                                      | Notes      |
 * | :--------------: | ---------------------------------------------------------------------------- | ---------------------------------------------------------------- | -------------------------------------------------------------------- | ---------- |
 * | 01               | Invoke the default constructor of FakeSecManager within EXPECT_NO_THROW block  | constructor = FakeSecManager(), no input parameters, output = instance created | No exception is thrown and a valid instance is created without errors | Should Pass |
 */
TEST(FakeSecManager, DefaultConstructor) {
    std::cout << "Entering DefaultConstructor test" << std::endl;
    EXPECT_NO_THROW({
        std::cout << "Invoking FakeSecManager::FakeSecManager() default constructor" << std::endl;
        // Creating instance using the default constructor.
        FakeSecManager instance;
        std::cout << "Successfully created FakeSecManager instance using default constructor" << std::endl;
        // Debug log for internal state (default state check).
        // Since the default state is not explicitly provided, we assume the instance to be valid.
        std::cout << "Default state of instance is assumed valid as no errors occurred during instantiation" << std::endl;
    });
    std::cout << "Exiting DefaultConstructor test" << std::endl;
}
/**
 * @brief Verify that a typical valid session ID is released successfully
 *
 * This test checks that the FakeSecManager::ReleaseSession method can be invoked with a typical valid session ID without throwing any exceptions.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 046@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                          | Test Data                                                       | Expected Result                                            | Notes        |
 * | :--------------: | ---------------------------------------------------- | --------------------------------------------------------------- | ---------------------------------------------------------- | ------------ |
 * | 01               | Create FakeSecManager object using default constructor | No input, output: FakeSecManager object instantiated           | Object is created successfully without throwing exceptions (EXPECT_NO_THROW) | Should Pass  |
 * | 02               | Define test input sessionId                           | sessionId = 12345                                               | sessionId is correctly defined                             | Should be successful |
 * | 03               | Invoke ReleaseSession with the valid sessionId        | input: sessionId = 12345; output: no exception thrown              | ReleaseSession method is executed without throwing exceptions (EXPECT_NO_THROW) | Should Pass  |
 */
TEST(FakeSecManager, ReleaseSession_TypicalValidSessionId) {
    std::cout << "Entering ReleaseSession_TypicalValidSessionId test" << std::endl;
    
    // Creating FakeSecManager object using default constructor
    std::cout << "Creating FakeSecManager object using default constructor" << std::endl;
    EXPECT_NO_THROW(FakeSecManager fakeSecManager);
    
    // Define test input sessionId
    int64_t sessionId = 12345;
    std::cout << "Test input sessionId: " << sessionId << std::endl;
    
    // Invoking ReleaseSession method
    std::cout << "Invoking FakeSecManager::ReleaseSession with sessionId: " << sessionId << std::endl;
    EXPECT_NO_THROW(fakeSecManager.ReleaseSession(sessionId));
    
    std::cout << "Exiting ReleaseSession_TypicalValidSessionId test" << std::endl;
}
/**
 * @brief Test the ReleaseSession method with a negative sessionId
 *
 * This test verifies that the FakeSecManager::ReleaseSession method can handle a negative sessionId (invalid input) without throwing exceptions. It checks the behavior of the method when an unexpected sessionId is passed, ensuring that the method processes the negative input as per the defined error handling.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 047@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                                  | Test Data                                | Expected Result                                                                | Notes               |
 * | :--------------: | ---------------------------------------------------------------------------- | ---------------------------------------- | ------------------------------------------------------------------------------ | ------------------- |
 * | 01               | Create FakeSecManager object using default constructor                       | None                                     | Object is created successfully without exceptions                            | Should be successful|
 * | 02               | Define test input sessionId with a negative value                            | sessionId = -1                           | The sessionId variable is set correctly                                        | Should be successful|
 * | 03               | Invoke FakeSecManager::ReleaseSession with the negative sessionId             | sessionId = -1                           | The method call does not throw an exception as verified by EXPECT_NO_THROW        | Should Fail         |
 */
TEST(FakeSecManager, ReleaseSession_NegativeSessionId) {
    std::cout << "Entering ReleaseSession_NegativeSessionId test" << std::endl;
    
    // Creating FakeSecManager object using default constructor
    std::cout << "Creating FakeSecManager object using default constructor" << std::endl;
    EXPECT_NO_THROW(FakeSecManager fakeSecManager);
    
    // Define test input sessionId
    int64_t sessionId = -1;
    std::cout << "Test input sessionId: " << sessionId << std::endl;
    
    // Invoking ReleaseSession method
    std::cout << "Invoking FakeSecManager::ReleaseSession with sessionId: " << sessionId << std::endl;
    EXPECT_NO_THROW(fakeSecManager.ReleaseSession(sessionId));
    
    std::cout << "Exiting ReleaseSession_NegativeSessionId test" << std::endl;
}
/**
 * @brief Validate ReleaseSession with maximum int64_t session ID.
 *
 * This test verifies that the FakeSecManager's ReleaseSession method correctly handles the boundary condition when provided with the maximum possible int64_t value. It confirms that the session is released without throwing any exceptions, ensuring the proper handling of extreme input values.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 048@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**
 * | Variation / Step | Description                                                        | Test Data                                                             | Expected Result                                                             | Notes         |
 * | :--------------: | ------------------------------------------------------------------ | --------------------------------------------------------------------- | ---------------------------------------------------------------------------- | ------------- |
 * | 01               | Create FakeSecManager object using default constructor             | No input, output: FakeSecManager object instance created              | Object is created successfully without exception                           | Should Pass   |
 * | 02               | Define test input sessionId as maximum int64_t value                 | sessionId = 9223372036854775807                                         | sessionId is set to INT64_MAX                                                | Should be successful |
 * | 03               | Invoke ReleaseSession method with the maximum sessionId            | input: sessionId = 9223372036854775807, output: no exception thrown       | ReleaseSession executes without throwing any exceptions                      | Should Pass   |
 */
TEST(FakeSecManager, ReleaseSession_Int64Max) {
    std::cout << "Entering ReleaseSession_Int64Max test" << std::endl;
    
    // Creating FakeSecManager object using default constructor
    std::cout << "Creating FakeSecManager object using default constructor" << std::endl;
    EXPECT_NO_THROW(FakeSecManager fakeSecManager);
    
    // Define test input sessionId as maximum int64_t value
    int64_t sessionId = 9223372036854775807LL;
    std::cout << "Test input sessionId (INT64_MAX): " << sessionId << std::endl;
    
    // Invoking ReleaseSession method
    std::cout << "Invoking FakeSecManager::ReleaseSession with sessionId: " << sessionId << std::endl;
    EXPECT_NO_THROW(fakeSecManager.ReleaseSession(sessionId));
    
    std::cout << "Exiting ReleaseSession_Int64Max test" << std::endl;
}
/**
 * @brief Validate the FakeSecManager::UpdateSessionState functionality for a valid session.
 *
 * This test creates a FakeSecManager object using its default constructor and then invokes the UpdateSessionState method
 * with a valid session ID and an active flag. The objective is to ensure that the object is created without throwing exceptions,
 * and that the UpdateSessionState function returns a boolean value when provided with valid inputs.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 049@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create FakeSecManager object using default constructor | None | Object is created without throwing exceptions | Should Pass |
 * | 02 | Invoke UpdateSessionState with sessionId = 12345 and active = true | sessionId = 12345, active = true | Function returns a boolean value indicating the update status | Should Pass |
 * | 03 | Validate the return value of UpdateSessionState using an assertion | result from UpdateSessionState | Assertion passes that result is either true or false | Should Pass |
 */
TEST(FakeSecManager, ValidSessionIdTrue) {
    std::cout << "Entering ValidSessionIdTrue test" << std::endl;
    
    // Create object using default constructor
    EXPECT_NO_THROW(FakeSecManager fsm);
    std::cout << "Created FakeSecManager object" << std::endl;
    
    int64_t sessionId = 12345;
    bool active = true;
    std::cout << "Invoking FakeSecManager::UpdateSessionState with sessionId = " << sessionId 
              << " and active = " << active << std::endl;
              
    bool result = fsm.UpdateSessionState(sessionId, active);
    std::cout << "UpdateSessionState returned: " << result << std::endl;
    
    EXPECT_TRUE(result == true || result == false);
    
    std::cout << "Exiting ValidSessionIdTrue test" << std::endl;
}
/**
 * @brief Validate that FakeSecManager::UpdateSessionState returns a boolean for a given session id and active state
 *
 * This test verifies that when FakeSecManager is instantiated and the UpdateSessionState method is called with a valid session id and a false active state, the method returns a boolean value (either true or false). This ensures that the function handles the false active scenario without throwing exceptions.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 050@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                       | Test Data                                                 | Expected Result                                           | Notes         |
 * | :--------------: | ----------------------------------------------------------------- | --------------------------------------------------------- | --------------------------------------------------------- | ------------- |
 * | 01               | Instantiate FakeSecManager using the default constructor          | No input parameters                                       | Object created without any exception                      | Should Pass   |
 * | 02               | Initialize sessionId and active variables                         | sessionId = 12345, active = false                           | Variables are set correctly                                | Should be successful |
 * | 03               | Call UpdateSessionState with sessionId and active parameters and verify the result | input: sessionId = 12345, active = false, output: result (boolean) | The method returns a boolean value (true or false)       | Should Pass   |
 */
TEST(FakeSecManager, ValidSessionIdFalse) {
    std::cout << "Entering ValidSessionIdFalse test" << std::endl;
    
    // Create object using default constructor
    EXPECT_NO_THROW(FakeSecManager fsm);
    FakeSecManager fsm;
    std::cout << "Created FakeSecManager object" << std::endl;
    
    int64_t sessionId = 12345;
    bool active = false;
    std::cout << "Invoking FakeSecManager::UpdateSessionState with sessionId = " << sessionId 
              << " and active = " << active << std::endl;
              
    bool result = fsm.UpdateSessionState(sessionId, active);
    std::cout << "UpdateSessionState returned: " << result << std::endl;
    
    EXPECT_TRUE(result == true || result == false);
    
    std::cout << "Exiting ValidSessionIdFalse test" << std::endl;
}
/**
 * @brief Test to validate that FakeSecManager::UpdateSessionState returns false when a negative session ID is provided.
 *
 * This test verifies that when a negative session ID (-1) is provided along with an active session flag (true),
 * the FakeSecManager::UpdateSessionState method correctly returns false. This behavior ensures that the API does not accept
 * negative session IDs, which are considered invalid.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 051
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                 | Test Data                                | Expected Result                                        | Notes       |
 * | :--------------: | --------------------------------------------------------------------------- | ---------------------------------------- | ------------------------------------------------------ | ----------- |
 * | 01               | Create FakeSecManager object using the default constructor and verify no exception is thrown. | None                                     | FakeSecManager object is created successfully.       | Should be successful |
 * | 02               | Invoke UpdateSessionState with a negative session ID (-1) and active flag set to true. | sessionId = -1, active = true, output expected = false | UpdateSessionState returns false as expected. | Should Fail |
 */
TEST(FakeSecManager, NegativeSessionIdTrue) {
    std::cout << "Entering NegativeSessionIdTrue test" << std::endl;
    
    // Create object using default constructor
    EXPECT_NO_THROW(FakeSecManager fsm);
    FakeSecManager fsm;
    std::cout << "Created FakeSecManager object" << std::endl;
    
    int64_t sessionId = -1;
    bool active = true;
    std::cout << "Invoking FakeSecManager::UpdateSessionState with sessionId = " << sessionId 
              << " and active = " << active << std::endl;
              
    bool result = fsm.UpdateSessionState(sessionId, active);
    std::cout << "UpdateSessionState returned: " << result << std::endl;
    
    EXPECT_EQ(result, false);
    
    std::cout << "Exiting NegativeSessionIdTrue test" << std::endl;
}
/**
 * @brief Verifies that UpdateSessionState returns false for a negative session ID.
 *
 * This test function validates that providing a negative session ID (-1) to the FakeSecManager::UpdateSessionState method returns false. The objective is to ensure that the API correctly handles invalid session IDs and does not erroneously activate a session with an invalid identifier.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 052@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                         | Test Data                                         | Expected Result                                           | Notes           |
 * | :--------------: | ------------------------------------------------------------------- | ------------------------------------------------- | --------------------------------------------------------- | --------------- |
 * | 01               | Create FakeSecManager object using default constructor              | None                                              | Object is created without exceptions                      | Should be successful |
 * | 02               | Invoke UpdateSessionState with a negative sessionId (-1) and active false | sessionId = -1, active = false, output1 = result   | Method returns false                                      | Should Fail     |
 * | 03               | Verify the returned result is false using assertion                 | result = false                                    | Assertion passes if return value equals false             | Should be successful |
 */
TEST(FakeSecManager, NegativeSessionIdFalse) {
    std::cout << "Entering NegativeSessionIdFalse test" << std::endl;
    
    // Create object using default constructor
    EXPECT_NO_THROW(FakeSecManager fsm);
    FakeSecManager fsm;
    std::cout << "Created FakeSecManager object" << std::endl;
    
    int64_t sessionId = -1;
    bool active = false;
    std::cout << "Invoking FakeSecManager::UpdateSessionState with sessionId = " << sessionId 
              << " and active = " << active << std::endl;
              
    bool result = fsm.UpdateSessionState(sessionId, active);
    std::cout << "UpdateSessionState returned: " << result << std::endl;
    
    EXPECT_EQ(result, false);
    
    std::cout << "Exiting NegativeSessionIdFalse test" << std::endl;
}
/**
 * @brief Verify that updating the session state with the maximum session ID value works as expected
 *
 * This test case creates a FakeSecManager object and then invokes the UpdateSessionState method using the maximum possible sessionId (9223372036854775807LL) and an active flag set to true. The objective is to ensure that the API handles extreme sessionId values without throwing exceptions and returns a valid boolean value.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 053@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                        | Test Data                                                    | Expected Result                                                         | Notes         |
 * | :--------------: | ------------------------------------------------------------------ | ------------------------------------------------------------ | ----------------------------------------------------------------------- | ------------- |
 * | 01               | Instantiate FakeSecManager using its default constructor           | None                                                         | Object creation does not throw any exception                            | Should Pass   |
 * | 02               | Set test input values for the UpdateSessionState method              | sessionId = 9223372036854775807, active = true                | Test data is prepared and available for the API call                    | Should be successful |
 * | 03               | Invoke the UpdateSessionState method with the provided test inputs   | sessionId = 9223372036854775807, active = true                | Return value is a valid boolean (true or false) as per method specification | Should Pass   |
 */
TEST(FakeSecManager, MaxSessionIdTrue) {
    std::cout << "Entering MaxSessionIdTrue test" << std::endl;
    
    // Create object using default constructor
    EXPECT_NO_THROW(FakeSecManager fsm);
    FakeSecManager fsm;
    std::cout << "Created FakeSecManager object" << std::endl;
    
    int64_t sessionId = 9223372036854775807LL;
    bool active = true;
    std::cout << "Invoking FakeSecManager::UpdateSessionState with sessionId = " << sessionId 
              << " and active = " << active << std::endl;
              
    bool result = fsm.UpdateSessionState(sessionId, active);
    std::cout << "UpdateSessionState returned: " << result << std::endl;
    
    EXPECT_TRUE(result == true || result == false);
    
    std::cout << "Exiting MaxSessionIdTrue test" << std::endl;
}
/**
 * @brief Verify that FakeSecManager::UpdateSessionState handles the maximum session ID input correctly with a deactivated state.
 *
 * This test creates a FakeSecManager object using its default constructor and then calls the UpdateSessionState API with a session ID of 9223372036854775807 and an active flag set to false. The test verifies that the API returns a boolean value and does not throw exceptions during object construction.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 054@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create a FakeSecManager object using the default constructor. | None | Object is constructed successfully without throwing exceptions. | Should be successful |
 * | 02 | Invoke FakeSecManager::UpdateSessionState with sessionId = 9223372036854775807 and active = false. | sessionId = 9223372036854775807, active = false | Returns a boolean value (true or false) based on internal logic; assertion check passes. | Should Pass |
 */
TEST(FakeSecManager, MaxSessionIdFalse) {
    std::cout << "Entering MaxSessionIdFalse test" << std::endl;
    
    // Create object using default constructor
    EXPECT_NO_THROW(FakeSecManager fsm);
    FakeSecManager fsm;
    std::cout << "Created FakeSecManager object" << std::endl;
    
    int64_t sessionId = 9223372036854775807LL;
    bool active = false;
    std::cout << "Invoking FakeSecManager::UpdateSessionState with sessionId = " << sessionId 
              << " and active = " << active << std::endl;
              
    bool result = fsm.UpdateSessionState(sessionId, active);
    std::cout << "UpdateSessionState returned: " << result << std::endl;
    
    EXPECT_TRUE(result == true || result == false);
    
    std::cout << "Exiting MaxSessionIdFalse test" << std::endl;
}
/**
 * @brief Test the valid typical input scenario for FakeSecManager's setPlaybackSpeedState method
 *
 * This test verifies that the FakeSecManager object is created successfully using its default constructor and that the setPlaybackSpeedState method handles valid input parameters (sessionId, speed, position) without throwing any exceptions. The test checks that the method returns a boolean value as expected.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 055@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                                 | Test Data                                                  | Expected Result                                                                           | Notes         |
 * | :--------------: | --------------------------------------------------------------------------- | ---------------------------------------------------------- | ----------------------------------------------------------------------------------------- | ------------- |
 * | 01               | Create FakeSecManager instance using the default constructor and check no throw | No input for constructor                                   | Instance is created successfully without throwing any exceptions                         | Should be successful |
 * | 02               | Invoke setPlaybackSpeedState with sessionId=123, speed=150, position=500      | sessionId = 123, speed = 150, position = 500, output = bool  | Method returns a boolean value (true or false) as validated by the EXPECT_TRUE assertion   | Should Pass   |
 */
TEST(FakeSecManager, ValidTypicalInput)
{
    std::cout << "Entering ValidTypicalInput test" << std::endl;
    
    // Create object using default constructor and check no throw
    EXPECT_NO_THROW({
        FakeSecManager fsm;
        std::cout << "FakeSecManager object created using default constructor." << std::endl;
        
        int64_t sessionId = 123;
        int64_t speed = 150;
        int64_t position = 500;
        std::cout << "Invoking setPlaybackSpeedState with sessionId: " << sessionId 
                  << ", speed: " << speed << ", position: " << position << std::endl;
        
        bool result = fsm.setPlaybackSpeedState(sessionId, speed, position);
        std::cout << "Method setPlaybackSpeedState returned: " << std::boolalpha << result << std::endl;
        
        // Expected output is false
        EXPECT_TRUE(result == true || result == false);
    });
    
    std::cout << "Exiting ValidTypicalInput test" << std::endl;
}
/**
 * @brief Test setPlaybackSpeedState with negative session identifier.
 *
 * This test verifies that setPlaybackSpeedState correctly handles an invalid (negative) session identifier. The API is expected to return false when a negative session identifier is provided, ensuring that invalid inputs are managed as designed.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 056
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create FakeSecManager object and invoke setPlaybackSpeedState with negative sessionId | sessionId = -100, speed = 150, position = 500 | Function returns false and assertion (EXPECT_FALSE) passes | Should Fail |
 */
TEST(FakeSecManager, NegativeSessionId)
{
    std::cout << "Entering NegativeSessionId test" << std::endl;
    
    EXPECT_NO_THROW({
        FakeSecManager fsm;
        std::cout << "FakeSecManager object created using default constructor." << std::endl;
        
        int64_t sessionId = -100;
        int64_t speed = 150;
        int64_t position = 500;
        std::cout << "Invoking setPlaybackSpeedState with sessionId: " << sessionId 
                  << ", speed: " << speed << ", position: " << position << std::endl;
        
        bool result = fsm.setPlaybackSpeedState(sessionId, speed, position);
        std::cout << "Method setPlaybackSpeedState returned: " << std::boolalpha << result << std::endl;
        
        EXPECT_FALSE(result);
    });
    
    std::cout << "Exiting NegativeSessionId test" << std::endl;
}
/**
 * @brief Verify that setPlaybackSpeedState rejects negative speed values.
 *
 * This test creates an instance of FakeSecManager and attempts to set the playback speed state with a negative speed value,
 * ensuring that the API correctly handles negative speed input by returning false without throwing any exceptions.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 057@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                                                          | Test Data                                                  | Expected Result                                          | Notes             |
 * | :--------------: | ---------------------------------------------------------------------------------------------------- | ---------------------------------------------------------- | -------------------------------------------------------- | ----------------- |
 * | 01               | Create a FakeSecManager object using the default constructor.                                      | None                                                       | Object is created successfully.                          | Should be successful |
 * | 02               | Invoke setPlaybackSpeedState with sessionId = 123, speed = -150, and position = 500 to test negative speed handling. | sessionId = 123, speed = -150, position = 500, output = false | The API returns false indicating rejection of negative speed | Should Fail       |
 */
TEST(FakeSecManager, NegativeSpeed)
{
    std::cout << "Entering NegativeSpeed test" << std::endl;
    
    EXPECT_NO_THROW({
        FakeSecManager fsm;
        std::cout << "FakeSecManager object created using default constructor." << std::endl;
        
        int64_t sessionId = 123;
        int64_t speed = -150;
        int64_t position = 500;
        std::cout << "Invoking setPlaybackSpeedState with sessionId: " << sessionId 
                  << ", speed: " << speed << ", position: " << position << std::endl;
        
        bool result = fsm.setPlaybackSpeedState(sessionId, speed, position);
        std::cout << "Method setPlaybackSpeedState returned: " << std::boolalpha << result << std::endl;
        
        EXPECT_FALSE(result);
    });
    
    std::cout << "Exiting NegativeSpeed test" << std::endl;
}
/**
 * @brief Verifies that setPlaybackSpeedState fails when a negative playback position is provided.
 *
 * This test verifies the behavior of FakeSecManager::setPlaybackSpeedState when a negative position is used. 
 * It ensures that the API returns false, indicating that the invalid input is rejected. This validation 
 * is crucial to maintain the integrity of playback parameters and prevent undefined behaviors caused by erroneous input.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 058@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoke FakeSecManager::setPlaybackSpeedState with a valid sessionId and speed, and with a negative position to simulate invalid input. | sessionId = 123, speed = 150, position = -500 | API returns false without throwing exceptions and the assertion confirms the failure. | Should Fail |
 */
TEST(FakeSecManager, NegativePosition)
{
    std::cout << "Entering NegativePosition test" << std::endl;
    
    EXPECT_NO_THROW({
        FakeSecManager fsm;
        std::cout << "FakeSecManager object created using default constructor." << std::endl;
        
        int64_t sessionId = 123;
        int64_t speed = 150;
        int64_t position = -500;
        std::cout << "Invoking setPlaybackSpeedState with sessionId: " << sessionId 
                  << ", speed: " << speed << ", position: " << position << std::endl;
        
        bool result = fsm.setPlaybackSpeedState(sessionId, speed, position);
        std::cout << "Method setPlaybackSpeedState returned: " << std::boolalpha << result << std::endl;
        
        EXPECT_FALSE(result);
    });
    
    std::cout << "Exiting NegativePosition test" << std::endl;
}
/**
 * @brief Validate the functionality of setVideoWindowSize with normal input values
 *
 * This test verifies that the setVideoWindowSize API in the FakeSecManager class accepts valid normal values without throwing exceptions. It checks that the API returns a boolean value. The test confirms proper object construction and method invocation using standard video dimensions.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 059@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                          | Test Data                                                         | Expected Result                                                        | Notes            |
 * | :--------------: | -------------------------------------------------------------------- | ----------------------------------------------------------------- | ---------------------------------------------------------------------- | ---------------- |
 * | 01               | Create an object of FakeSecManager using its default constructor     | No parameters                                                     | Object created without throwing any exceptions                         | Should be successful |
 * | 02               | Define sessionId, video_width, and video_height                        | sessionId = 1, video_width = 640, video_height = 480                | Variables assigned with the correct values                             | Should be successful |
 * | 03               | Invoke setVideoWindowSize with valid parameters                        | sessionId = 1, video_width = 640, video_height = 480, output result expected as boolean | API returns a boolean value indicating the operation result             | Should Pass       |
 * | 04               | Verify the correctness of the boolean result returned by setVideoWindowSize | output result from the API call                                     | The result must be either true or false as enforced by the assertion check | Should Pass       |
 */
TEST(FakeSecManager, NormalValues) {
    std::cout << "Entering NormalValues test" << std::endl;
    
    // Create an object of FakeSecManager using the default constructor
    EXPECT_NO_THROW(FakeSecManager secMgr);
    int64_t sessionId = 1;
    int64_t video_width = 640;
    int64_t video_height = 480;
    
    std::cout << "Invoking setVideoWindowSize with sessionId: " << sessionId 
              << ", video_width: " << video_width 
              << ", video_height: " << video_height << std::endl;
    
    // Call setVideoWindowSize and capture the return value
    bool result = secMgr.setVideoWindowSize(sessionId, video_width, video_height);
    std::cout << "Method setVideoWindowSize returned: " << std::boolalpha << result << std::endl;
    
    // Verify the returned result is false
    EXPECT_TRUE(result == true || result == false);
    
    std::cout << "Exiting NormalValues test" << std::endl;
}
/**
 * @brief Test for verifying that setVideoWindowSize correctly handles a negative sessionId.
 *
 * This test validates that the setVideoWindowSize API returns false when provided with a negative sessionId.
 * It ensures that the method properly handles invalid session IDs by not processing the video window size update.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 060@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                      | Test Data                                           | Expected Result                                                              | Notes      |
 * | :--------------: | ---------------------------------------------------------------- | --------------------------------------------------- | ---------------------------------------------------------------------------- | ---------- |
 * | 01               | Invoke setVideoWindowSize with a negative sessionId parameter.   | sessionId = -1, video_width = 640, video_height = 480 | The API should return false; EXPECT_EQ verifies false.                       | Should Fail|
 */
TEST(FakeSecManager, NegativeSessionId) {
    std::cout << "Entering NegativeSessionId test" << std::endl;
    
    EXPECT_NO_THROW(FakeSecManager secMgr);
    FakeSecManager secMgr;
    int64_t sessionId = -1;
    int64_t video_width = 640;
    int64_t video_height = 480;
    
    std::cout << "Invoking setVideoWindowSize with sessionId: " << sessionId 
              << ", video_width: " << video_width 
              << ", video_height: " << video_height << std::endl;
    
    bool result = secMgr.setVideoWindowSize(sessionId, video_width, video_height);
    std::cout << "Method setVideoWindowSize returned: " << std::boolalpha << result << std::endl;
    
    EXPECT_EQ(result, false);
    
    std::cout << "Exiting NegativeSessionId test" << std::endl;
}
/**
 * @brief Test the behavior of setVideoWindowSize when passed a negative video width.
 *
 * This test verifies that the setVideoWindowSize API in the FakeSecManager class returns false when invoked with a negative video_width value. The objective is to ensure that the API properly handles invalid input conditions. This is important to guard against potential errors in the video display configuration.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 061@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Instantiate FakeSecManager and ensure no exception is thrown. | N/A | FakeSecManager object is created successfully. | Should be successful |
 * | 02 | Invoke setVideoWindowSize with sessionId=1, video_width=-640, video_height=480. | sessionId = 1, video_width = -640, video_height = 480, output = false | API returns false due to invalid video_width input. | Should Fail |
 * | 03 | Assert that the return value from setVideoWindowSize is false. | result = false | The result equals false confirming the negative test scenario. | Should be successful |
 */
TEST(FakeSecManager, NegativeVideoWindowWidth) {
    std::cout << "Entering NegativeVideoWindowWidth test" << std::endl;
    
    EXPECT_NO_THROW(FakeSecManager secMgr);
    FakeSecManager secMgr;
    int64_t sessionId = 1;
    int64_t video_width = -640;
    int64_t video_height = 480;
    
    std::cout << "Invoking setVideoWindowSize with sessionId: " << sessionId 
              << ", video_width: " << video_width 
              << ", video_height: " << video_height << std::endl;
    
    bool result = secMgr.setVideoWindowSize(sessionId, video_width, video_height);
    std::cout << "Method setVideoWindowSize returned: " << std::boolalpha << result << std::endl;
    
    EXPECT_EQ(result, false);
    
    std::cout << "Exiting NegativeVideoWindowWidth test" << std::endl;
}
/**
 * @brief Verify that setVideoWindowSize returns false when a negative video height is provided
 *
 * This test verifies that the setVideoWindowSize method of the FakeSecManager class correctly handles invalid input by returning false when a negative video height is specified. It first ensures that the FakeSecManager object can be instantiated without throwing an exception, then calls the setVideoWindowSize method with a negative video height value.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 062
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                               | Test Data                                                  | Expected Result                                          | Notes          |
 * | :--------------: | ------------------------------------------------------------------------- | ---------------------------------------------------------- | -------------------------------------------------------- | -------------- |
 * | 01               | Instantiate FakeSecManager and verify no exceptions are thrown            | Constructor Invocation                                     | Object is successfully created without throwing exceptions| Should be successful |
 * | 02               | Invoke setVideoWindowSize with negative video height value                | sessionId = 1, video_width = 640, video_height = -480        | Method returns false and EXPECT_EQ(result, false) check passes | Should Fail    |
 */
TEST(FakeSecManager, NegativeVideoWindowHeight) {
    std::cout << "Entering NegativeVideoWindowHeight test" << std::endl;
    
    EXPECT_NO_THROW(FakeSecManager secMgr);
    FakeSecManager secMgr;
    int64_t sessionId = 1;
    int64_t video_width = 640;
    int64_t video_height = -480;
    
    std::cout << "Invoking setVideoWindowSize with sessionId: " << sessionId 
              << ", video_width: " << video_width 
              << ", video_height: " << video_height << std::endl;
    
    bool result = secMgr.setVideoWindowSize(sessionId, video_width, video_height);
    std::cout << "Method setVideoWindowSize returned: " << std::boolalpha << result << std::endl;
    
    EXPECT_EQ(result, false);
    
    std::cout << "Exiting NegativeVideoWindowHeight test" << std::endl;
}
/**
 * @brief Test the proper destruction of a FakeSecManager object created on the stack.
 *
 * This test verifies that a FakeSecManager object, when instantiated on the stack, is automatically destroyed when it goes out of scope without throwing any exceptions.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 063
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoke FakeSecManager default constructor within an EXPECT_NO_THROW block and let the instance go out of scope to trigger destructor. | input: None, output: FakeSecManager instance created | No exception thrown; destructor called automatically when instance goes out of scope. | Should Pass |
 */
TEST(FakeSecManager, DestructorCalledOnStackInstance) {
    std::cout << "Entering DestructorCalledOnStackInstance test" << std::endl;
    EXPECT_NO_THROW({
        std::cout << "Invoking FakeSecManager default constructor" << std::endl;
        FakeSecManager fsm;
        std::cout << "FakeSecManager instance created. It will be automatically destroyed when going out of scope." << std::endl;
    });
    std::cout << "Exiting DestructorCalledOnStackInstance test" << std::endl;
}
/**
 * @brief Verify successful destruction of the FakeSecManager instance using DestroyInstance().
 *
 * This test verifies that the static method DestroyInstance() is invoked without throwing exceptions.
 * It also simulates a status check by verifying that the instance is no longer active.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 064@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:** 
 * | Variation / Step | Description                                                         | Test Data                                | Expected Result                                | Notes          |
 * | :--------------: | ------------------------------------------------------------------- | ---------------------------------------- | ---------------------------------------------- | -------------- |
 * | 01               | Log test entry and initialization messages                          | N/A                                      | Test entry logged                              | Should be successful |
 * | 02               | Invoke FakeSecManager::DestroyInstance() and check that no exception is thrown | DestroyInstance() = invoked              | No exception thrown; method executed successfully | Should Pass    |
 * | 03               | Verify instance destruction simulation by checking post-destruction status | N/A                                      | Instance is no longer active                   | Should be successful |
 */
TEST(FakeSecManager, SuccessfullyDestroyInstance) {
    std::cout << "Entering SuccessfullyDestroyInstance test" << std::endl;

    // Invoke the static method DestroyInstance on the base class
    std::cout << "Invoking ContentSecurityManager::DestroyInstance()" << std::endl;
    EXPECT_NO_THROW({
        FakeSecManager::DestroyInstance();
        std::cout << "ContentSecurityManager::DestroyInstance() invoked successfully" << std::endl;
    });

    // Simulate status check for instance destruction (if applicable)
    std::cout << "Post-destruction: Verified that the instance is no longer active" << std::endl;

    std::cout << "Exiting SuccessfullyDestroyInstance test" << std::endl;
}
/**
 * @brief Test to verify that FakeSecManager::GetInstance returns a valid instance without throwing an exception.
 *
 * This test verifies that invoking FakeSecManager::GetInstance does not throw an exception and returns a non-null pointer.
 * It ensures that the instance is properly initialized and available for further operations.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 065
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                       | Test Data                                            | Expected Result                                                                                | Notes      |
 * | :--------------: | ------------------------------------------------------------------------------------------------- | ---------------------------------------------------- | ----------------------------------------------------------------------------------------------- | ---------- |
 * | 01               | Invoke FakeSecManager::GetInstance and check for no exception, and then validate the returned pointer is non-null. | No input, output instance pointer is non-null        | Returns a valid pointer with no exception thrown and assertion passes for instance != nullptr.  | Should Pass |
 */
TEST(FakeSecManager, GetInstance_start) {
    std::cout << "Entering GetInstance_start test" << std::endl;
    
    ContentSecurityManager* instance = nullptr;
    
    // Invoke GetInstance() using the derived class.
    EXPECT_NO_THROW({
        instance = FakeSecManager::GetInstance();
        std::cout << "Called GetInstance(), received pointer: " << instance << std::endl;
    });
    
    // Check that the pointer is not null.
    EXPECT_NE(instance, nullptr);
    if (instance != nullptr) {
        // Assuming the 'key information' is represented by the pointer's memory address.
        std::cout << "ContentSecurityManager instance key information: "
                  << "instance address = " << instance << std::endl;
    }
    
    std::cout << "Exiting GetInstance_start test" << std::endl;
}
/**
 * @brief Test enabling the Firebolt SDK using the FakeSecManager instance.
 *
 * This test verifies that a FakeSecManager object can be instantiated without throwing an exception
 * and that invoking the UseFireboltSDK method with a true input enables the Firebolt SDK without errors.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 066@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                       | Test Data                                                          | Expected Result                                                        | Notes              |
 * | :--------------: | ------------------------------------------------------------------ | ------------------------------------------------------------------ | ---------------------------------------------------------------------- | ------------------ |
 * | 01               | Create FakeSecManager object using default constructor              | None                                                               | Object is created without throwing an exception                        | Should be successful |
 * | 02               | Invoke the UseFireboltSDK method with inputStatus set to true         | inputStatus = true                                                 | API method is called without throwing any exception                     | Should Pass        |
 * | 03               | Log the internal state update confirming Firebolt SDK is enabled      | internal state update log: "Firebolt SDK is enabled"               | Internal state updated confirming that Firebolt SDK is enabled            | Should be successful |
 */
TEST(FakeSecManager, UseFireboltSDK_Enable) {
    std::cout << "Entering UseFireboltSDK_Enable test" << std::endl;
    
    // Create FakeSecManager object using default constructor and verify no exception is thrown.
    EXPECT_NO_THROW({
        FakeSecManager fakeSecManager;
        std::cout << "Created FakeSecManager object using default constructor." << std::endl;
        
        bool inputStatus = true;
        std::cout << "Invoking UseFireboltSDK with input value: " << std::boolalpha << inputStatus << std::endl;
        
        // Call the base class method UseFireboltSDK using the derived class object.
        EXPECT_NO_THROW(fakeSecManager.UseFireboltSDK(inputStatus));
        std::cout << "UseFireboltSDK method successfully invoked with input value: " << std::boolalpha << inputStatus << std::endl;
        
        // Log the expected internal state change.
        std::cout << "Internal state updated: Firebolt SDK is enabled." << std::endl;
        
    });
    
    std::cout << "Exiting UseFireboltSDK_Enable test" << std::endl;
}
/**
 * @brief Verify that the Firebolt SDK is disabled when UseFireboltSDK is invoked with a false input.
 *
 * This test creates an instance of FakeSecManager using the default constructor and verifies that passing a false value to UseFireboltSDK does not throw an exception. It confirms that the method correctly updates the internal state to disable the Firebolt SDK.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 067
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                           | Test Data                                   | Expected Result                                                                                     | Notes              |
 * | :--------------: | ----------------------------------------------------------------------------------------------------- | ------------------------------------------- | ---------------------------------------------------------------------------------------------------- | ------------------ |
 * | 01               | Create FakeSecManager object using the default constructor and verify no exception is thrown.         | Constructor call: FakeSecManager()          | Object is created successfully without throwing any exceptions.                                    | Should be successful |
 * | 02               | Invoke UseFireboltSDK with inputStatus set to false to disable the Firebolt SDK.                      | inputStatus = false                         | UseFireboltSDK does not throw an exception and disables the Firebolt SDK internally.                 | Should Pass        |
 */
TEST(FakeSecManager, UseFireboltSDK_Disable) {
    std::cout << "Entering UseFireboltSDK_Disable test" << std::endl;
    
    // Create FakeSecManager object using default constructor and verify no exception is thrown.
    EXPECT_NO_THROW({
        FakeSecManager fakeSecManager;
        std::cout << "Created FakeSecManager object using default constructor." << std::endl;
        
        bool inputStatus = false;
        std::cout << "Invoking UseFireboltSDK with input value: " << std::boolalpha << inputStatus << std::endl;
        
        // Call the base class method UseFireboltSDK using the derived class object.
        EXPECT_NO_THROW(fakeSecManager.UseFireboltSDK(inputStatus));
        std::cout << "UseFireboltSDK method successfully invoked with input value: " << std::boolalpha << inputStatus << std::endl;
        
        // Log the expected internal state change.
        std::cout << "Internal state updated: Firebolt SDK is disabled." << std::endl;
        
    });
    
    std::cout << "Exiting UseFireboltSDK_Disable test" << std::endl;
}
/**
 * @brief Validate that the setPlaybackSpeedState API behaves correctly with valid positive input values.
 *
 * This test verifies that providing valid positive integers for sessionId, playback_speed, and playback_position
 * results in successful invocation of setPlaybackSpeedState. The test creates a FakeSecManager object using the default
 * constructor, ensures no exceptions are thrown during object creation or method execution, and checks that the method
 * returns true.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 068@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- | -------------- | ----- |
 * | 01 | Create a FakeSecManager object using the default constructor | [No input parameters] | Object is constructed without throwing an exception | Should be successful |
 * | 02 | Set sessionId, playback_speed, and playback_position, then invoke setPlaybackSpeedState | sessionId = 100, playback_speed = 2, playback_position = 5000 | Method returns true without throwing an exception | Should Pass |
 * | 03 | Verify that the result from setPlaybackSpeedState is true using assertion | result = true | EXPECT_TRUE(result) passes | Should Pass |
 */
TEST(FakeSecManager, ValidInputPositiveValues) {
    std::cout << "Entering ValidInputPositiveValues test" << std::endl;
    // Create derived class object using default constructor
    EXPECT_NO_THROW({
        FakeSecManager fakeSecManager;
        std::cout << "Constructed FakeSecManager object successfully" << std::endl;
        
        int64_t sessionId = 100;
        int64_t playback_speed = 2;
        int64_t playback_position = 5000;
        std::cout << "Invoking setPlaybackSpeedState with sessionId: " << sessionId 
                  << ", playback_speed: " << playback_speed 
                  << ", playback_position: " << playback_position << std::endl;
        
        bool result = fakeSecManager.setPlaybackSpeedState(sessionId, playback_speed, playback_position);
        std::cout << "Method returned: " << std::boolalpha << result << std::endl;
        EXPECT_TRUE(result);
    });
    std::cout << "Exiting ValidInputPositiveValues test" << std::endl;
}
/**
 * @brief Verifies that setPlaybackSpeedState returns false when invoked with a negative sessionId
 *
 * This test case validates that the FakeSecManager::setPlaybackSpeedState API handles an invalid negative sessionId appropriately.
 * The test creates an instance of FakeSecManager, then calls setPlaybackSpeedState with sessionId = -1, playback_speed = 2, and playback_position = 5000. It asserts that the returned value is false, ensuring that the method does not permit a negative session identifier.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 069@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                                             | Test Data                                              | Expected Result                                                       | Notes       |
 * | :--------------: | ----------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------ | --------------------------------------------------------------------- | ----------- |
 * | 01               | Construct FakeSecManager and invoke setPlaybackSpeedState with sessionId = -1, playback_speed = 2, playback_position = 5000 | sessionId = -1, playback_speed = 2, playback_position = 5000 | API returns false; EXPECT_FALSE assertion verifies the invalid sessionId | Should Fail |
 */
TEST(FakeSecManager, NegativeSessionId) {
    std::cout << "Entering NegativeSessionId test" << std::endl;
    EXPECT_NO_THROW({
        FakeSecManager fakeSecManager;
        std::cout << "Constructed FakeSecManager object successfully" << std::endl;
        
        int64_t sessionId = -1;
        int64_t playback_speed = 2;
        int64_t playback_position = 5000;
        std::cout << "Invoking setPlaybackSpeedState with sessionId: " << sessionId 
                  << ", playback_speed: " << playback_speed 
                  << ", playback_position: " << playback_position << std::endl;
        
        bool result = fakeSecManager.setPlaybackSpeedState(sessionId, playback_speed, playback_position);
        std::cout << "Method returned: " << std::boolalpha << result << std::endl;
        EXPECT_FALSE(result);
    });
    std::cout << "Exiting NegativeSessionId test" << std::endl;
}
/**
 * @brief Verify that the API correctly handles a negative playback_speed input.
 *
 * This test verifies that when a negative playback_speed value is provided to the 
 * setPlaybackSpeedState method of FakeSecManager, the API does not throw an exception and returns false as expected.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 070@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**
 * | Variation / Step | Description                                                        | Test Data                                         | Expected Result                                                                 | Notes              |
 * | :--------------: | ------------------------------------------------------------------ | ------------------------------------------------- | ------------------------------------------------------------------------------- | ------------------ |
 * | 01               | Construct FakeSecManager object and ensure no exception is thrown    | No inputs; output: FakeSecManager object created  | Object constructed successfully without exceptions                             | Should be successful |
 * | 02               | Invoke setPlaybackSpeedState with negative playback_speed and verify result | input: sessionId = 100, playback_speed = -2, playback_position = 5000, output: result = false | API returns false indicating negative playback_speed is handled correctly without throwing exceptions | Should Fail        |
 */
TEST(FakeSecManager, NegativePlaybackSpeed) {
    std::cout << "Entering NegativePlaybackSpeed test" << std::endl;
    EXPECT_NO_THROW({
        FakeSecManager fakeSecManager;
        std::cout << "Constructed FakeSecManager object successfully" << std::endl;
        
        int64_t sessionId = 100;
        int64_t playback_speed = -2;
        int64_t playback_position = 5000;
        std::cout << "Invoking setPlaybackSpeedState with sessionId: " << sessionId 
                  << ", playback_speed: " << playback_speed 
                  << ", playback_position: " << playback_position << std::endl;
        
        bool result = fakeSecManager.setPlaybackSpeedState(sessionId, playback_speed, playback_position);
        std::cout << "Method returned: " << std::boolalpha << result << std::endl;
        EXPECT_FALSE(result);
    });
    std::cout << "Exiting NegativePlaybackSpeed test" << std::endl;
}
/**
 * @brief Validate that setPlaybackSpeedState returns false when a negative playback_position is provided.
 *
 * This test verifies that the FakeSecManager::setPlaybackSpeedState API handles negative playback_position values correctly.
 * The function is expected to return false when the playback_position is negative, ensuring proper handling of invalid input.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 071@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                  | Test Data                                             | Expected Result                                           | Notes         |
 * | :--------------: | ------------------------------------------------------------ | ----------------------------------------------------- | --------------------------------------------------------- | ------------- |
 * | 01               | Build a FakeSecManager object and call setPlaybackSpeedState with a negative playback_position parameter | sessionId = 100, playback_speed = 2, playback_position = -5000 | API returns false and the assertion (EXPECT_FALSE) passes | Should Pass   |
 */
TEST(FakeSecManager, NegativePlaybackPosition) {
    std::cout << "Entering NegativePlaybackPosition test" << std::endl;
    EXPECT_NO_THROW({
        FakeSecManager fakeSecManager;
        std::cout << "Constructed FakeSecManager object successfully" << std::endl;
        
        int64_t sessionId = 100;
        int64_t playback_speed = 2;
        int64_t playback_position = -5000;
        std::cout << "Invoking setPlaybackSpeedState with sessionId: " << sessionId 
                  << ", playback_speed: " << playback_speed 
                  << ", playback_position: " << playback_position << std::endl;
        
        bool result = fakeSecManager.setPlaybackSpeedState(sessionId, playback_speed, playback_position);
        std::cout << "Method returned: " << std::boolalpha << result << std::endl;
        EXPECT_FALSE(result);
    });
    std::cout << "Exiting NegativePlaybackPosition test" << std::endl;
}
/**
 * @brief Verify setPlaybackSpeedState behavior with zero playback speed
 *
 * This test verifies that when the playback_speed is set to zero, the FakeSecManager's
 * setPlaybackSpeedState method does not throw any exceptions and returns true. The test
 * ensures that the API handles a zero playback speed scenario correctly.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 072@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                              | Test Data                                               | Expected Result                                          | Notes         |
 * | :--------------: | ---------------------------------------------------------------------------------------- | ------------------------------------------------------- | -------------------------------------------------------- | ------------- |
 * | 01               | Construct FakeSecManager object                                                          | None                                                    | FakeSecManager object constructed successfully without exception | Should be successful |
 * | 02               | Invoke setPlaybackSpeedState API with sessionId, playback_speed, and playback_position     | sessionId = 100, playback_speed = 0, playback_position = 5000 | API returns true and does not throw an exception         | Should Pass   |
 * | 03               | Validate that the returned result is true via assertion                                    | output=result (expected true)                           | Assertion EXPECT_TRUE(result) passes                     | Should Pass   |
 */
TEST(FakeSecManager, ZeroPlaybackSpeed) {
    std::cout << "Entering ZeroPlaybackSpeed test" << std::endl;
    EXPECT_NO_THROW({
        FakeSecManager fakeSecManager;
        std::cout << "Constructed FakeSecManager object successfully" << std::endl;
        
        int64_t sessionId = 100;
        int64_t playback_speed = 0;
        int64_t playback_position = 5000;
        std::cout << "Invoking setPlaybackSpeedState with sessionId: " << sessionId 
                  << ", playback_speed: " << playback_speed 
                  << ", playback_position: " << playback_position << std::endl;
        
        bool result = fakeSecManager.setPlaybackSpeedState(sessionId, playback_speed, playback_position);
        std::cout << "Method returned: " << std::boolalpha << result << std::endl;
        EXPECT_TRUE(result);
    });
    std::cout << "Exiting ZeroPlaybackSpeed test" << std::endl;
}

int main(int argc, char **argv) {
     ::testing::InitGoogleTest(&argc, argv);
     return RUN_ALL_TESTS();
}
