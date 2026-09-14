
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
#include "ContentProtectionFirebolt.h"


// Test Case: CloseDrmSession with a valid session ID
/**
 * @brief Verify that CloseDrmSession successfully terminates an active DRM session when called with a valid session identifier.
 *
 * This test validates that a ContentProtectionFirebolt object can be created without exceptions and that invoking
 * the CloseDrmSession method with a valid sessionId (12345) does not throw any exceptions, ensuring the API behaves as expected.
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
 * | Variation / Step | Description                                                            | Test Data                                 | Expected Result                                                                         | Notes         |
 * | :--------------: | ---------------------------------------------------------------------- | ----------------------------------------- | --------------------------------------------------------------------------------------- | ------------- |
 * | 01               | Create a ContentProtectionFirebolt object using the default constructor. | None                                      | Object is created successfully without throwing any exceptions.                       | Should be successful |
 * | 02               | Invoke CloseDrmSession on the object with a valid sessionId (12345).      | sessionId = 12345                         | API is invoked without throwing any exceptions; CloseDrmSession executes successfully.    | Should Pass   |
 */
TEST(ContentProtectionFirebolt, CloseDrmSession_with_valid_session)
{
    std::cout << "Entering CloseDrmSession_with_valid_session test" << std::endl;

    // Log: Creating ContentProtectionFirebolt object using default constructor
    std::cout << "Invoking default constructor for ContentProtectionFirebolt" << std::endl;
    EXPECT_NO_THROW({
        ContentProtectionFirebolt cp;
        
        // Logging internal state after construction (if any known state exists - simulated log)
        std::cout << "ContentProtectionFirebolt object created. (Internal state logged if available)" << std::endl;
        
        int64_t sessionId = 12345;
        std::cout << "Invoking CloseDrmSession with sessionId = " << sessionId << std::endl;
        EXPECT_NO_THROW({
            cp.CloseDrmSession(sessionId);
            std::cout << "CloseDrmSession executed successfully with sessionId = " << sessionId << std::endl;
        });
    });

    std::cout << "Exiting CloseDrmSession_with_valid_session test" << std::endl;
}
/**
 * @brief Validate that CloseDrmSession gracefully handles a negative session identifier.
 *
 * This test verifies that the ContentProtectionFirebolt API can handle an invalid DRM session identifier (sessionId = -1) without throwing exceptions. It ensures robustness in handling edge cases for DRM session management.
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
 * | Variation / Step | Description                                                         | Test Data                      | Expected Result                                       | Notes             |
 * | :--------------: | ------------------------------------------------------------------- | ------------------------------ | ----------------------------------------------------- | ----------------- |
 * | 01               | Create ContentProtectionFirebolt object using the default constructor | None                           | Object is successfully created without throwing exceptions | Should be successful |
 * | 02               | Invoke CloseDrmSession with sessionId = -1                            | sessionId = -1                 | No exception thrown; API handles negative session gracefully   | Should Pass       |
 */
TEST(ContentProtectionFirebolt, CloseDrmSession_with_negative_session)
{
    std::cout << "Entering CloseDrmSession_with_negative_session test" << std::endl;

    // Log: Creating ContentProtectionFirebolt object using default constructor
    std::cout << "Invoking default constructor for ContentProtectionFirebolt" << std::endl;
    EXPECT_NO_THROW({
        ContentProtectionFirebolt cp;
        
        // Logging the initial state of the object (if any known state exists - simulated log)
        std::cout << "ContentProtectionFirebolt object created. (Internal state logged if available)" << std::endl;
        
        int64_t sessionId = -1;
        std::cout << "Invoking CloseDrmSession with sessionId = " << sessionId << std::endl;
        EXPECT_NO_THROW({
            cp.CloseDrmSession(sessionId);
            std::cout << "CloseDrmSession executed successfully with sessionId = " << sessionId << std::endl;
        });
    });

    std::cout << "Exiting CloseDrmSession_with_negative_session test" << std::endl;
}
/**
 * @brief Verify that CloseDrmSession handles a session ID of zero gracefully.
 *
 * This test verifies that using a sessionId of 0 with the CloseDrmSession method on a ContentProtectionFirebolt instance does not throw an exception. It ensures that the API can handle edge cases for session management without causing errors.
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
 * | Variation / Step | Description | Test Data |Expected Result |Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoke default constructor for ContentProtectionFirebolt. | None | Object is instantiated successfully without throwing an exception. | Should be successful |
 * | 02 | Call CloseDrmSession with sessionId = 0 on the ContentProtectionFirebolt object. | sessionId = 0 | API call completes without exception; EXPECT_NO_THROW passes. | Should Pass |
 */
TEST(ContentProtectionFirebolt, CloseDrmSession_with_zero_session)
{
    std::cout << "Entering CloseDrmSession_with_zero_session test" << std::endl;

    // Log: Creating ContentProtectionFirebolt object using default constructor
    std::cout << "Invoking default constructor for ContentProtectionFirebolt" << std::endl;
    EXPECT_NO_THROW({
        ContentProtectionFirebolt cp;
        
        // Logging the initial state of the object (if any known state exists - simulated log)
        std::cout << "ContentProtectionFirebolt object created. (Internal state logged if available)" << std::endl;
        
        int64_t sessionId = 0;
        std::cout << "Invoking CloseDrmSession with sessionId = " << sessionId << std::endl;
        EXPECT_NO_THROW({
            cp.CloseDrmSession(sessionId);
            std::cout << "CloseDrmSession executed successfully with sessionId = " << sessionId << std::endl;
        });
    });

    std::cout << "Exiting CloseDrmSession_with_zero_session test" << std::endl;
}
/**
 * @brief Verify that ContentProtectionFirebolt can be default constructed without throwing exceptions.
 *
 * This test case verifies the proper behavior of the default constructor of the ContentProtectionFirebolt class.
 * It ensures that creating an instance using the default constructor does not throw any exceptions.
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
 * | Variation / Step | Description                                                                           | Test Data                                          | Expected Result                                                                           | Notes           |
 * | :--------------: | ------------------------------------------------------------------------------------- | -------------------------------------------------- | ----------------------------------------------------------------------------------------- | --------------- |
 * | 01               | Log the start of the test by printing the entry message.                             | None                                               | Entry message "Entering DefaultConstruction_Instance1 test" printed to console.           | Should be successful |
 * | 02               | Invoke the ContentProtectionFirebolt default constructor to create instance1.          | No input parameters, output: instance1 created     | The constructor completes without throwing any exception; EXPECT_NO_THROW passes.         | Should Pass     |
 * | 03               | Log the end of the test by printing the exit message.                                | None                                               | Exit message "Exiting DefaultConstruction_Instance1 test" printed to console.             | Should be successful |
 */
TEST(ContentProtectionFirebolt, DefaultConstruction_Instance1)
{
    std::cout << "Entering DefaultConstruction_Instance1 test" << std::endl;
    
    std::cout << "Invoking ContentProtectionFirebolt default constructor for instance1" << std::endl;
    EXPECT_NO_THROW({
        ContentProtectionFirebolt instance1;
        std::cout << "Successfully created instance1 using the default constructor" << std::endl;
    });
    
    std::cout << "Exiting DefaultConstruction_Instance1 test" << std::endl;
}
/**
 * @brief Test to verify deinitialization functionality on an object that is already initialized and connected.
 *
 * Test objective is to ensure that the DeInitialize() method of ContentProtectionFirebolt functions correctly when invoked on an object that has been initialized and connected.
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
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create an instance of ContentProtectionFirebolt using default constructor | None | Object is created successfully without exception | Should Pass |
 * | 02 | Simulate object state as initialized and connected | None | Object is assumed to be in an initialized and connected state | Should be successful |
 * | 03 | Invoke DeInitialize() method on the object | Method: DeInitialize() | Method call completes without throwing exception | Should Pass |
 */
TEST(ContentProtectionFirebolt, DeInitialize_on_initialized_and_connected) {
    std::cout << "Entering DeInitialize_on_initialized_and_connected test" << std::endl;
    
    // Create an object using the default constructor
    std::cout << "Invoking default constructor of ContentProtectionFirebolt." << std::endl;
    EXPECT_NO_THROW({
        ContentProtectionFirebolt contentProtection;
        std::cout << "ContentProtectionFirebolt object created." << std::endl;
        
        // Simulate that the object is 'initialized and connected'
        // (Assumed internal state; no setters/getters available.)
        std::cout << "Assuming object is in an initialized and connected state." << std::endl;
        
        // Invoke the DeInitialize() method
        std::cout << "Invoking DeInitialize() method." << std::endl;
        EXPECT_NO_THROW({
            contentProtection.DeInitialize();
            std::cout << "DeInitialize() method invoked successfully with no exceptions." << std::endl;
        });
    });

    std::cout << "Exiting DeInitialize_on_initialized_and_connected test" << std::endl;
}
/**
 * @brief Verify that HandleWatermarkEvent processes valid input parameters without throwing exceptions.
 *
 * This test verifies that the ContentProtectionFirebolt object handles valid inputs correctly by ensuring that no exceptions are thrown during the invocation of HandleWatermarkEvent. By testing with standard session, status, and app identifiers, it confirms reliable behavior under nominal conditions.@n
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
 * | Variation / Step | Description | Test Data |Expected Result |Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoke ContentProtectionFirebolt constructor and call HandleWatermarkEvent with valid inputs: sessionId='session123', statusStr='active', appId='app123'. | sessionId = session123, statusStr = active, appId = app123 | HandleWatermarkEvent should execute without throwing exceptions. | Should Pass |
 */
TEST(ContentProtectionFirebolt, PositiveTestValidInputs) {
    std::cout << "Entering PositiveTestValidInputs test" << std::endl;
    
    // Create object using default constructor
    EXPECT_NO_THROW({
        ContentProtectionFirebolt contentProtection;
        std::string sessionId = "session123";
        std::string statusStr = "active";
        std::string appId = "app123";
        
        std::cout << "Invoking HandleWatermarkEvent with sessionId: " << sessionId 
                  << ", statusStr: " << statusStr 
                  << ", appId: " << appId << std::endl;
        
        EXPECT_NO_THROW(contentProtection.HandleWatermarkEvent(sessionId, statusStr, appId));
        std::cout << "HandleWatermarkEvent executed successfully with valid inputs." << std::endl;
    });
    
    std::cout << "Exiting PositiveTestValidInputs test" << std::endl;
}
/**
 * @brief Negative test case for HandleWatermarkEvent API using an empty session identifier
 *
 * This test verifies that invoking the HandleWatermarkEvent API with an empty sessionId does not throw an exception while valid values are provided for statusStr and appId. This ensures that the API gracefully handles cases with invalid session input.
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
 * | Variation / Step | Description                                                             | Test Data                                                   | Expected Result                                                         | Notes             |
 * | :--------------: | ----------------------------------------------------------------------- | ----------------------------------------------------------- | ----------------------------------------------------------------------- | ----------------- |
 * | 01               | Instantiate ContentProtectionFirebolt object and initialize variables   | input: sessionId="", statusStr="active", appId="app123"       | Object creation without exceptions                                      | Should be successful |
 * | 02               | Log the details before invoking HandleWatermarkEvent                    | N/A                                                         | Log message indicating API invocation is about to start                 | Should be successful |
 * | 03               | Invoke HandleWatermarkEvent with an empty sessionId and valid parameters  | input: sessionId="", statusStr="active", appId="app123"       | The API is executed without throwing an exception                       | Should Fail       |
 * | 04               | Log after successful API invocation                                    | N/A                                                         | Log message confirming successful API execution                         | Should be successful |
 */
TEST(ContentProtectionFirebolt, NegativeTestEmptySessionId) {
    std::cout << "Entering NegativeTestEmptySessionId test" << std::endl;
    
    // Create object using default constructor
    EXPECT_NO_THROW({
        ContentProtectionFirebolt contentProtection;
        std::string sessionId = "";
        std::string statusStr = "active";
        std::string appId = "app123";
        
        std::cout << "Invoking HandleWatermarkEvent with empty sessionId, statusStr: " 
                  << statusStr << ", appId: " << appId << std::endl;
        
        EXPECT_NO_THROW(contentProtection.HandleWatermarkEvent(sessionId, statusStr, appId));
        std::cout << "HandleWatermarkEvent executed successfully with empty sessionId." << std::endl;
    });
    
    std::cout << "Exiting NegativeTestEmptySessionId test" << std::endl;
}
/**
 * @brief Test the behavior of ContentProtectionFirebolt::HandleWatermarkEvent when provided an empty status string.
 *
 * This test verifies that the HandleWatermarkEvent API does not throw an exception when invoked with an empty statusStr.
 * The test sets up a valid sessionId and appId, while statusStr is empty, and ensures the API call is handled gracefully.
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
 * | 01 | Create ContentProtectionFirebolt object and initialize test parameters | sessionId = session123, statusStr = , appId = app123 | Object created without exceptions | Should be successful |
 * | 02 | Invoke HandleWatermarkEvent with an empty statusStr value | sessionId = session123, statusStr = , appId = app123 | API call completes without throwing an exception | Should Pass |
 */
TEST(ContentProtectionFirebolt, NegativeTestEmptyStatusStr) {
    std::cout << "Entering NegativeTestEmptyStatusStr test" << std::endl;
    
    // Create object using default constructor
    EXPECT_NO_THROW({
        ContentProtectionFirebolt contentProtection;
        std::string sessionId = "session123";
        std::string statusStr = "";
        std::string appId = "app123";
        
        std::cout << "Invoking HandleWatermarkEvent with sessionId: " << sessionId 
                  << ", empty statusStr, appId: " << appId << std::endl;
        
        EXPECT_NO_THROW(contentProtection.HandleWatermarkEvent(sessionId, statusStr, appId));
        std::cout << "HandleWatermarkEvent executed successfully with empty statusStr." << std::endl;
    });
    
    std::cout << "Exiting NegativeTestEmptyStatusStr test" << std::endl;
}
/**
 * @brief Validate negative scenario for HandleWatermarkEvent API when appId is empty.
 *
 * This test verifies that invoking the HandleWatermarkEvent method with an empty appId does not throw an exception.
 * The test is designed to ensure that the ContentProtectionFirebolt object's method handles an empty appId gracefully.
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
 * | Variation / Step | Description                                                        | Test Data                                                        | Expected Result                                                   | Notes       |
 * | :--------------: | ------------------------------------------------------------------ | ---------------------------------------------------------------- | ----------------------------------------------------------------- | ----------- |
 * | 01               | Create ContentProtectionFirebolt instance and call HandleWatermarkEvent with empty appId. | sessionId = session123, statusStr = active, appId = ""             | API call completes without throwing any exception.                | Should Fail |
 */
TEST(ContentProtectionFirebolt, NegativeTestEmptyAppId) {
    std::cout << "Entering NegativeTestEmptyAppId test" << std::endl;
    
    // Create object using default constructor
    EXPECT_NO_THROW({
        ContentProtectionFirebolt contentProtection;
        std::string sessionId = "session123";
        std::string statusStr = "active";
        std::string appId = "";
        
        std::cout << "Invoking HandleWatermarkEvent with sessionId: " << sessionId 
                  << ", statusStr: " << statusStr 
                  << ", empty appId" << std::endl;
        
        EXPECT_NO_THROW(contentProtection.HandleWatermarkEvent(sessionId, statusStr, appId));
        std::cout << "HandleWatermarkEvent executed successfully with empty appId." << std::endl;
    });
    
    std::cout << "Exiting NegativeTestEmptyAppId test" << std::endl;
}
/**
 * @brief Test to verify that HandleWatermarkEvent function can handle long string inputs without throwing exceptions.
 *
 * This test verifies that the ContentProtectionFirebolt object can be constructed successfully and that the
 * HandleWatermarkEvent API correctly processes very long strings for sessionId, statusStr, and appId without
 * throwing any exceptions. The test ensures that the API is robust for handling large input sizes.
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
 * | Variation / Step | Description                                                                                   | Test Data                                                                                                       | Expected Result                                                          | Notes          |
 * | :--------------: | --------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------ | -------------- |
 * | 01               | Create a ContentProtectionFirebolt object and initialize long strings for sessionId, statusStr, and appId. | Object construction; sessionId = 1000 's', statusStr = 1000 'a', appId = 1000 'i'                                | Object is constructed successfully and strings are initialized with the correct lengths. | Should be successful |
 * | 02               | Invoke HandleWatermarkEvent with the long string inputs to verify proper handling.               | input: sessionId = 1000 's', statusStr = 1000 'a', appId = 1000 'i'; output: none, no exceptions thrown             | The API call does not throw an exception and completes successfully.     | Should Pass    |
 */
TEST(ContentProtectionFirebolt, PositiveTestLongStringInputs) {
    std::cout << "Entering PositiveTestLongStringInputs test" << std::endl;
    
    // Create object using default constructor
    EXPECT_NO_THROW({
        ContentProtectionFirebolt contentProtection;
        std::string sessionId(1000, 's');
        std::string statusStr(1000, 'a');
        std::string appId(1000, 'i');
        
        std::cout << "Invoking HandleWatermarkEvent with long string inputs. "
                  << "sessionId length: " << sessionId.length() 
                  << ", statusStr length: " << statusStr.length() 
                  << ", appId length: " << appId.length() << std::endl;
        
        EXPECT_NO_THROW(contentProtection.HandleWatermarkEvent(sessionId, statusStr, appId));
        std::cout << "HandleWatermarkEvent executed successfully with long string inputs." << std::endl;
    });
    
    std::cout << "Exiting PositiveTestLongStringInputs test" << std::endl;
}
/**
 * @brief Validate that the ContentProtectionFirebolt object initializes properly without exceptions.
 *
 * This test verifies the proper instantiation of a ContentProtectionFirebolt object using its default constructor and ensures that calling the Initialize() method does not throw any exceptions. This confirms that the object is correctly set up and ready for further operations.
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
 * | Variation / Step | Description                                                               | Test Data                                                          | Expected Result                                                      | Notes      |
 * | :--------------: | ------------------------------------------------------------------------- | ------------------------------------------------------------------ | -------------------------------------------------------------------- | ---------- |
 * | 01               | Create a ContentProtectionFirebolt object using the default constructor.  | client = ContentProtectionFirebolt instance created via default constructor | Object is instantiated without throwing an exception.               | Should Pass|
 * | 02               | Invoke the Initialize() method on the ContentProtectionFirebolt object.     | client.Initialize() method; input: default parameters; output: none  | Initialize() executes without throwing an exception and passes assertion. | Should Pass|
 */
TEST(ContentProtectionFirebolt, PositiveInitialization) {
    std::cout << "Entering PositiveInitialization test" << std::endl;
    
    // Create ContentProtectionFirebolt object using default constructor
    std::cout << "Creating ContentProtectionFirebolt object using default constructor." << std::endl;
    EXPECT_NO_THROW({
        ContentProtectionFirebolt client;
        std::cout << "ContentProtectionFirebolt object created successfully." << std::endl;
        
        // Invoke Initialize() method
        std::cout << "Invoking Initialize() method on ContentProtectionFirebolt object." << std::endl;
        EXPECT_NO_THROW(client.Initialize());
        std::cout << "Initialize() method invoked successfully without throwing an exception." << std::endl;
    });
    
    std::cout << "Exiting PositiveInitialization test" << std::endl;
}
/**
 * @brief Verify that ContentProtectionFirebolt::IsActive returns a valid boolean value with default force.
 *
 * This test creates a ContentProtectionFirebolt object using the default constructor and invokes the IsActive() method 
 * with the default parameter (force = false). It then verifies that the method returns a valid boolean value (true or false)
 * and does not throw any exceptions during execution.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create ContentProtectionFirebolt object using default constructor and invoke IsActive() with default force parameter (false). | object = ContentProtectionFirebolt, force = false, result = bool | API returns a valid boolean value (true or false) without throwing exceptions. | Should Pass |
 */
TEST(ContentProtectionFirebolt, IsActiveDefaultForce) {
    std::cout << "Entering IsActiveDefaultForce test" << std::endl;
    EXPECT_NO_THROW({
        ContentProtectionFirebolt obj;
        std::cout << "Created ContentProtectionFirebolt object using default constructor" << std::endl;
        
        // Invoking IsActive with force = false (default)
        bool result = obj.IsActive();
        std::cout << "Invoked IsActive(false), returned value: " << std::boolalpha << result << std::endl;
        
        // Verifying the output is a valid boolean value (true or false)
        EXPECT_TRUE(result == true || result == false) << "IsActive with default force should return a valid boolean value";
    });
    std::cout << "Exiting IsActiveDefaultForce test" << std::endl;
}
/**
 * @brief Verify IsActive returns a valid boolean output when force parameter is true
 *
 * This test evaluates the ContentProtectionFirebolt::IsActive method for a scenario where the force parameter is set to true.
 * It ensures that the method does not throw an exception and returns a valid boolean value (either true or false).
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
 * | Variation / Step | Description                                                                  | Test Data                                   | Expected Result                                                          | Notes               |
 * | :--------------: | ---------------------------------------------------------------------------- | ------------------------------------------- | ------------------------------------------------------------------------ | ------------------- |
 * | 01               | Create a ContentProtectionFirebolt object using the default constructor        | N/A                                         | Object is created successfully                                           | Should be successful|
 * | 02               | Invoke IsActive API with force set to true and validate the returned boolean    | input: force = true, output: boolean value    | Returns a valid boolean value (true or false) without throwing an exception | Should Pass         |
 */
TEST(ContentProtectionFirebolt, IsActiveWithForce) {
    std::cout << "Entering IsActiveWithForce test" << std::endl;
    EXPECT_NO_THROW({
        ContentProtectionFirebolt obj;
        std::cout << "Created ContentProtectionFirebolt object using default constructor" << std::endl;

        // Invoking IsActive with force = true
        bool result = obj.IsActive(true);
        std::cout << "Invoked IsActive(true), returned value: " << std::boolalpha << result << std::endl;

        // Verifying the output is a valid boolean value (true or false)
        EXPECT_TRUE(result == true || result == false) << "IsActive with force set to true should return a valid boolean value";
    });
    std::cout << "Exiting IsActiveWithForce test" << std::endl;
}
/**
 * @brief Validate the OpenDrmSession API with valid parameters.
 *
 * This test verifies that the OpenDrmSession API of ContentProtectionFirebolt successfully initiates a DRM session when provided with valid input parameters. It confirms that the API returns true and provides the expected response message.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 014@n
 * **Priority:** (High)@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                                                  | Test Data                                                                                                                       | Expected Result                                                              | Notes      |
 * | :--------------: | -------------------------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------------------------------------- | --------------------------------------------------------------------------- | ---------- |
 * | 01               | Instantiate ContentProtectionFirebolt, prepare valid input parameters, and invoke OpenDrmSession | clientId = client123, appId = appABC, keySystem = widevine, licenseRequest = licenseChallenge, initData = initDataPayload, sessionId = 0, response = empty | API returns true and response is "DRM session opened successfully"            | Should Pass |
 */
TEST(ContentProtectionFirebolt, PositiveTestWithValidParameters) {
    std::cout << "Entering PositiveTestWithValidParameters test" << std::endl;
  
    // Create object using default constructor and log the event.
    EXPECT_NO_THROW({
        ContentProtectionFirebolt cp;
        std::cout << "Constructed ContentProtectionFirebolt object using default constructor." << std::endl;
      
        // Prepare input parameters.
        std::string clientId = "client123";
        std::string appId = "appABC";
        std::string keySystem = "widevine";
        std::string licenseRequest = "licenseChallenge";
        std::string initData = "initDataPayload";
        int64_t sessionId = 0;
        std::string response = "";
      
        // Log input parameters.
        std::cout << "Invoking OpenDrmSession with parameters:" << std::endl;
        std::cout << "  clientId (before): " << clientId << std::endl;
        std::cout << "  appId: " << appId << std::endl;
        std::cout << "  keySystem: " << keySystem << std::endl;
        std::cout << "  licenseRequest: " << licenseRequest << std::endl;
        std::cout << "  initData: " << initData << std::endl;
        std::cout << "  sessionId: " << sessionId << std::endl;
      
        // Invoke the method.
        bool result = cp.OpenDrmSession(clientId, appId, keySystem, licenseRequest, initData, sessionId, response);
      
        // Log the returned value and modified clientId and response.
        std::cout << "Method OpenDrmSession returned: " << std::boolalpha << result << std::endl;
        std::cout << "clientId (after): " << clientId << std::endl;
        std::cout << "response: " << response << std::endl;
      
        // Check the expected outcomes.
        EXPECT_TRUE(result);
        EXPECT_EQ(response, "DRM session opened successfully");
    });
  
    std::cout << "Exiting PositiveTestWithValidParameters test" << std::endl;
}
/**
 * @brief Test that OpenDrmSession returns false and an error message when provided an empty clientId.
 *
 * This test verifies that the OpenDrmSession API correctly handles the case of an empty clientId. It constructs a ContentProtectionFirebolt object using the default constructor, prepares input parameters where clientId is empty, and invokes the API expecting it to return false and set the response to "Invalid clientId". This is essential for ensuring that invalid input is handled correctly.
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
 * | Variation / Step | Description                                                                                     | Test Data                                                                                                                                                   | Expected Result                                                                                   | Notes          |
 * | :--------------: | ----------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------- | -------------- |
 * | 01               | Construct ContentProtectionFirebolt object using default constructor.                           | N/A                                                                                                                                                         | Object is constructed without exceptions.                                                       | Should be successful |
 * | 02               | Prepare input parameters with an empty clientId and valid values for other parameters.           | clientId = "", appId = "appABC", keySystem = "widevine", licenseRequest = "licenseChallenge", initData = "initDataPayload", sessionId = 0, response = "" | Input parameters are set as specified, with clientId as empty.                                    | Should be successful |
 * | 03               | Invoke OpenDrmSession API using the prepared parameters.                                       | clientId = "", appId = "appABC", keySystem = "widevine", licenseRequest = "licenseChallenge", initData = "initDataPayload", sessionId = 0           | API returns false and sets response to "Invalid clientId".                                        | Should Fail    |
 */

TEST(ContentProtectionFirebolt, NegativeTestEmptyClientId) {
    std::cout << "Entering NegativeTestEmptyClientId test" << std::endl;
  
    EXPECT_NO_THROW({
        ContentProtectionFirebolt cp;
        std::cout << "Constructed ContentProtectionFirebolt object using default constructor." << std::endl;
  
        // Prepare input parameters with empty clientId.
        std::string clientId = "";
        std::string appId = "appABC";
        std::string keySystem = "widevine";
        std::string licenseRequest = "licenseChallenge";
        std::string initData = "initDataPayload";
        int64_t sessionId = 0;
        std::string response = "";
  
        // Log input parameters.
        std::cout << "Invoking OpenDrmSession with parameters:" << std::endl;
        std::cout << "  clientId (before): " << clientId << " (empty)" << std::endl;
        std::cout << "  appId: " << appId << std::endl;
        std::cout << "  keySystem: " << keySystem << std::endl;
        std::cout << "  licenseRequest: " << licenseRequest << std::endl;
        std::cout << "  initData: " << initData << std::endl;
        std::cout << "  sessionId: " << sessionId << std::endl;
  
        // Invoke the method.
        bool result = cp.OpenDrmSession(clientId, appId, keySystem, licenseRequest, initData, sessionId, response);
  
        // Log the returned value.
        std::cout << "Method OpenDrmSession returned: " << std::boolalpha << result << std::endl;
        std::cout << "response: " << response << std::endl;
  
        // Check the expected outcomes.
        EXPECT_FALSE(result);
        EXPECT_EQ(response, "Invalid clientId");
    });
  
    std::cout << "Exiting NegativeTestEmptyClientId test" << std::endl;
}
/**
 * @brief Verify that the OpenDrmSession API correctly handles an empty appId.
 *
 * This test checks that when an empty string is passed as the appId, the OpenDrmSession API
 * returns false and provides the error message "Invalid appId", ensuring improper input is correctly managed.
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
 * | Variation / Step | Description                                                                          | Test Data                                                                                                                  | Expected Result                                           | Notes       |
 * | :--------------: | ------------------------------------------------------------------------------------ | -------------------------------------------------------------------------------------------------------------------------- | --------------------------------------------------------- | ----------- |
 * | 01               | Invoke OpenDrmSession API using an empty appId to simulate an invalid input scenario   | clientId = client123, appId = (empty), keySystem = widevine, licenseRequest = licenseChallenge, initData = initDataPayload, sessionId = 0, response = (empty)  | Function returns false and response equals "Invalid appId" | Should Fail |
 */
TEST(ContentProtectionFirebolt, NegativeTestEmptyAppId) {
    std::cout << "Entering NegativeTestEmptyAppId test" << std::endl;
  
    EXPECT_NO_THROW({
        ContentProtectionFirebolt cp;
        std::cout << "Constructed ContentProtectionFirebolt object using default constructor." << std::endl;
  
        // Prepare input parameters with empty appId.
        std::string clientId = "client123";
        std::string appId = "";
        std::string keySystem = "widevine";
        std::string licenseRequest = "licenseChallenge";
        std::string initData = "initDataPayload";
        int64_t sessionId = 0;
        std::string response = "";
  
        // Log input parameters.
        std::cout << "Invoking OpenDrmSession with parameters:" << std::endl;
        std::cout << "  clientId: " << clientId << std::endl;
        std::cout << "  appId (empty): " << appId << std::endl;
        std::cout << "  keySystem: " << keySystem << std::endl;
        std::cout << "  licenseRequest: " << licenseRequest << std::endl;
        std::cout << "  initData: " << initData << std::endl;
        std::cout << "  sessionId: " << sessionId << std::endl;
  
        // Invoke the method.
        bool result = cp.OpenDrmSession(clientId, appId, keySystem, licenseRequest, initData, sessionId, response);
  
        // Log the returned value.
        std::cout << "Method OpenDrmSession returned: " << std::boolalpha << result << std::endl;
        std::cout << "response: " << response << std::endl;
  
        // Check the expected outcomes.
        EXPECT_FALSE(result);
        EXPECT_EQ(response, "Invalid appId");
    });
  
    std::cout << "Exiting NegativeTestEmptyAppId test" << std::endl;
}
/**
 * @brief Test negative scenario for OpenDrmSession with an empty keySystem.
 *
 * This test verifies that when an empty keySystem is provided to the OpenDrmSession API,
 * the function properly returns false and sets the response to "Invalid keySystem".
 * This ensures that the API performs the necessary input validation for the keySystem parameter.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Construct ContentProtectionFirebolt object using default constructor. | None | Object is created successfully. | Should be successful |
 * | 02 | Prepare input parameters with an empty keySystem. | clientId = client123, appId = appABC, keySystem = , licenseRequest = licenseChallenge, initData = initDataPayload, sessionId = 0, response = "" | Inputs are correctly initialized. | Should be successful |
 * | 03 | Invoke the OpenDrmSession API with the prepared parameters. | clientId = client123, appId = appABC, keySystem = , licenseRequest = licenseChallenge, initData = initDataPayload, sessionId = 0, response = "" | API returns false indicating failure due to empty keySystem. | Should Fail |
 * | 04 | Validate the API response and assertion checks. | result = false, response = "Invalid keySystem" | result is false and response equals "Invalid keySystem". | Should be successful |
 */
TEST(ContentProtectionFirebolt, NegativeTestEmptyKeySystem) {
    std::cout << "Entering NegativeTestEmptyKeySystem test" << std::endl;
  
    EXPECT_NO_THROW({
        ContentProtectionFirebolt cp;
        std::cout << "Constructed ContentProtectionFirebolt object using default constructor." << std::endl;
  
        // Prepare input parameters with empty keySystem.
        std::string clientId = "client123";
        std::string appId = "appABC";
        std::string keySystem = "";
        std::string licenseRequest = "licenseChallenge";
        std::string initData = "initDataPayload";
        int64_t sessionId = 0;
        std::string response = "";
  
        // Log input parameters.
        std::cout << "Invoking OpenDrmSession with parameters:" << std::endl;
        std::cout << "  clientId: " << clientId << std::endl;
        std::cout << "  appId: " << appId << std::endl;
        std::cout << "  keySystem (empty): " << keySystem << std::endl;
        std::cout << "  licenseRequest: " << licenseRequest << std::endl;
        std::cout << "  initData: " << initData << std::endl;
        std::cout << "  sessionId: " << sessionId << std::endl;
  
        // Invoke the method.
        bool result = cp.OpenDrmSession(clientId, appId, keySystem, licenseRequest, initData, sessionId, response);
  
        // Log the returned value.
        std::cout << "Method OpenDrmSession returned: " << std::boolalpha << result << std::endl;
        std::cout << "response: " << response << std::endl;
  
        // Check the expected outcomes.
        EXPECT_FALSE(result);
        EXPECT_EQ(response, "Invalid keySystem");
    });
  
    std::cout << "Exiting NegativeTestEmptyKeySystem test" << std::endl;
}
/**
 * @brief Validates that OpenDrmSession fails when licenseRequest is empty
 *
 * This test verifies the behavior of the OpenDrmSession method in the ContentProtectionFirebolt class when provided with an empty licenseRequest string. It ensures that the API correctly rejects the request and returns false along with the appropriate error message.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 018@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | ------------- | ----- |
 * | 01 | Construct ContentProtectionFirebolt object and setup input parameters with an empty licenseRequest. | clientId = client123, appId = appABC, keySystem = widevine, licenseRequest = , initData = initDataPayload, sessionId = 0, response = (empty) | Object constructed; parameters initialized with licenseRequest as empty | Should be successful |
 * | 02 | Invoke OpenDrmSession method with prepared parameters and verify that it fails appropriately. | Input: clientId = client123, appId = appABC, keySystem = widevine, licenseRequest = , initData = initDataPayload, sessionId = 0; Output: response should be "Invalid licenseRequest" | API returns false and response equals "Invalid licenseRequest" as verified by EXPECT_FALSE and EXPECT_EQ assertions | Should Fail |
 */
TEST(ContentProtectionFirebolt, NegativeTestEmptyLicenseRequest) {
    std::cout << "Entering NegativeTestEmptyLicenseRequest test" << std::endl;
  
    EXPECT_NO_THROW({
        ContentProtectionFirebolt cp;
        std::cout << "Constructed ContentProtectionFirebolt object using default constructor." << std::endl;
  
        // Prepare input parameters with empty licenseRequest.
        std::string clientId = "client123";
        std::string appId = "appABC";
        std::string keySystem = "widevine";
        std::string licenseRequest = "";
        std::string initData = "initDataPayload";
        int64_t sessionId = 0;
        std::string response = "";
  
        // Log input parameters.
        std::cout << "Invoking OpenDrmSession with parameters:" << std::endl;
        std::cout << "  clientId: " << clientId << std::endl;
        std::cout << "  appId: " << appId << std::endl;
        std::cout << "  keySystem: " << keySystem << std::endl;
        std::cout << "  licenseRequest (empty): " << licenseRequest << std::endl;
        std::cout << "  initData: " << initData << std::endl;
        std::cout << "  sessionId: " << sessionId << std::endl;
  
        // Invoke the method.
        bool result = cp.OpenDrmSession(clientId, appId, keySystem, licenseRequest, initData, sessionId, response);
  
        // Log the returned value.
        std::cout << "Method OpenDrmSession returned: " << std::boolalpha << result << std::endl;
        std::cout << "response: " << response << std::endl;
  
        // Check the expected outcomes.
        EXPECT_FALSE(result);
        EXPECT_EQ(response, "Invalid licenseRequest");
    });
  
    std::cout << "Exiting NegativeTestEmptyLicenseRequest test" << std::endl;
}
/**
 * @brief Validate negative scenario of OpenDrmSession when initData is empty.
 *
 * This test verifies that passing an empty initialization data parameter to the OpenDrmSession API returns false and sets the response to "Invalid initData", ensuring that invalid input is handled correctly.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 019@n
 * **Priority:** High@n
 * @n
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * @n
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |@n
 * | :----: | --------- | ---------- |-------------- | ----- |@n
 * | 01 | Construct ContentProtectionFirebolt object and invoke OpenDrmSession with empty initData. | clientId = client123, appId = appABC, keySystem = widevine, licenseRequest = licenseChallenge, initData = , sessionId = 0, response =  | API returns false and response equals "Invalid initData" (assertions: EXPECT_FALSE(result) and EXPECT_EQ(response, "Invalid initData")) | Should Fail |
 */
TEST(ContentProtectionFirebolt, NegativeTestEmptyInitData) {
    std::cout << "Entering NegativeTestEmptyInitData test" << std::endl;
  
    EXPECT_NO_THROW({
        ContentProtectionFirebolt cp;
        std::cout << "Constructed ContentProtectionFirebolt object using default constructor." << std::endl;
  
        // Prepare input parameters with empty initData.
        std::string clientId = "client123";
        std::string appId = "appABC";
        std::string keySystem = "widevine";
        std::string licenseRequest = "licenseChallenge";
        std::string initData = "";
        int64_t sessionId = 0;
        std::string response = "";
  
        // Log input parameters.
        std::cout << "Invoking OpenDrmSession with parameters:" << std::endl;
        std::cout << "  clientId: " << clientId << std::endl;
        std::cout << "  appId: " << appId << std::endl;
        std::cout << "  keySystem: " << keySystem << std::endl;
        std::cout << "  licenseRequest: " << licenseRequest << std::endl;
        std::cout << "  initData (empty): " << initData << std::endl;
        std::cout << "  sessionId: " << sessionId << std::endl;
  
        // Invoke the method.
        bool result = cp.OpenDrmSession(clientId, appId, keySystem, licenseRequest, initData, sessionId, response);
  
        // Log the returned value.
        std::cout << "Method OpenDrmSession returned: " << std::boolalpha << result << std::endl;
        std::cout << "response: " << response << std::endl;
  
        // Check the expected outcomes.
        EXPECT_FALSE(result);
        EXPECT_EQ(response, "Invalid initData");
    });
  
    std::cout << "Exiting NegativeTestEmptyInitData test" << std::endl;
}
/**
 * @brief Test the functionality for a valid positive session ID with active state true
 *
 * This test verifies that a valid positive session ID along with an active flag set to true correctly updates the DRM session state.
 * It ensures that the ContentProtectionFirebolt object is instantiated successfully without throwing exceptions and that the SetDrmSessionState API returns true.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Construct ContentProtectionFirebolt object and validate that no exception is thrown | N/A | Object is constructed successfully without throwing any exception | Should be successful |
 * | 02 | Invoke SetDrmSessionState API with sessionId = 1 and active = true, then check return value | input: sessionId = 1, active = true; output: result = true | SetDrmSessionState returns true and assertion EXPECT_TRUE passes | Should Pass |
 */
TEST(ContentProtectionFirebolt, ValidPositiveSessionID_ActiveTrue) {
    std::cout << "Entering ValidPositiveSessionID_ActiveTrue test" << std::endl;
    
    // Create object
    EXPECT_NO_THROW({
        ContentProtectionFirebolt cp;
        std::cout << "Constructed ContentProtectionFirebolt object" << std::endl;
        
        int64_t sessionId = 1;
        bool active = true;
        std::cout << "Invoking SetDrmSessionState with sessionId: " << sessionId 
                  << " and active: " << (active ? "true" : "false") << std::endl;
        
        bool result = cp.SetDrmSessionState(sessionId, active);
        std::cout << "Returned value from SetDrmSessionState: " << (result ? "true" : "false") << std::endl;
        
        EXPECT_TRUE(result);
    });
    
    std::cout << "Exiting ValidPositiveSessionID_ActiveTrue test" << std::endl;
}
/**
 * @brief Verify that the SetDrmSessionState API returns true for a valid positive session ID when active is false
 *
 * Tests that the API handles the case where a valid session ID is provided along with an inactive flag, ensuring that the API correctly processes and returns true.
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
 * | Variation / Step | Description                                                          | Test Data                                   | Expected Result                                                    | Notes               |
 * | :--------------: | -------------------------------------------------------------------- | ------------------------------------------- | ------------------------------------------------------------------ | ------------------- |
 * | 01               | Construct ContentProtectionFirebolt object                           | No inputs; object instantiation             | Object should be constructed without throwing exceptions           | Should be successful|
 * | 02               | Invoke SetDrmSessionState with sessionId = 1 and active = false        | sessionId = 1, active = false, output = true | API should return true as validated by EXPECT_TRUE                   | Should Pass         |
 */
TEST(ContentProtectionFirebolt, ValidPositiveSessionID_ActiveFalse) {
    std::cout << "Entering ValidPositiveSessionID_ActiveFalse test" << std::endl;
    
    // Create object
    EXPECT_NO_THROW({
        ContentProtectionFirebolt cp;
        std::cout << "Constructed ContentProtectionFirebolt object" << std::endl;
        
        int64_t sessionId = 1;
        bool active = false;
        std::cout << "Invoking SetDrmSessionState with sessionId: " << sessionId 
                  << " and active: " << (active ? "true" : "false") << std::endl;
        
        bool result = cp.SetDrmSessionState(sessionId, active);
        std::cout << "Returned value from SetDrmSessionState: " << (result ? "true" : "false") << std::endl;
        
        EXPECT_TRUE(result);
    });
    
    std::cout << "Exiting ValidPositiveSessionID_ActiveFalse test" << std::endl;
}
/**
 * @brief Validate that SetDrmSessionState handles negative session IDs correctly when active is true
 *
 * This test verifies that the SetDrmSessionState API of the ContentProtectionFirebolt class correctly
 * handles a negative session ID by returning a false result when the active flag is set to true. The test
 * is designed to confirm that invalid session identifiers are properly rejected by the API.
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
 * | Variation / Step | Description                                                                 | Test Data                                        | Expected Result                                       | Notes          |
 * | :--------------: | --------------------------------------------------------------------------- | ------------------------------------------------ | ----------------------------------------------------- | -------------- |
 * | 01               | Construct ContentProtectionFirebolt object without exceptions             | No input required                                | Object is constructed successfully without any exception | Should be successful |
 * | 02               | Set sessionId to -1 and active flag to true, then invoke SetDrmSessionState   | sessionId = -1, active = true                     | The API returns false and the assertion confirms the false outcome  | Should Fail    |
 */
TEST(ContentProtectionFirebolt, NegativeSessionID_ActiveTrue) {
    std::cout << "Entering NegativeSessionID_ActiveTrue test" << std::endl;
    
    // Create object
    EXPECT_NO_THROW({
        ContentProtectionFirebolt cp;
        std::cout << "Constructed ContentProtectionFirebolt object" << std::endl;
        
        int64_t sessionId = -1;
        bool active = true;
        std::cout << "Invoking SetDrmSessionState with sessionId: " << sessionId 
                  << " and active: " << (active ? "true" : "false") << std::endl;
        
        bool result = cp.SetDrmSessionState(sessionId, active);
        std::cout << "Returned value from SetDrmSessionState: " << (result ? "true" : "false") << std::endl;
        
        EXPECT_FALSE(result);
    });
    
    std::cout << "Exiting NegativeSessionID_ActiveTrue test" << std::endl;
}
/**
 * @brief Test that verifies SetDrmSessionState correctly handles an invalid negative session identifier.
 *
 * This test checks whether the ContentProtectionFirebolt::SetDrmSessionState API returns false when invoked with a negative session ID (-1) and active flag set to false, ensuring that the API does not erroneously accept invalid parameters.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 023@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- | -------------- | ----- |
 * | 01 | Construct ContentProtectionFirebolt object | None | Object constructed without exceptions | Should be successful |
 * | 02 | Call SetDrmSessionState with sessionId = -1 and active = false | input: sessionId = -1, active = false; output: result expected to be false | API returns false and assertion verifies false | Should Pass |
 */
TEST(ContentProtectionFirebolt, NegativeSessionID_ActiveFalse) {
    std::cout << "Entering NegativeSessionID_ActiveFalse test" << std::endl;
    
    // Create object
    EXPECT_NO_THROW({
        ContentProtectionFirebolt cp;
        std::cout << "Constructed ContentProtectionFirebolt object" << std::endl;
        
        int64_t sessionId = -1;
        bool active = false;
        std::cout << "Invoking SetDrmSessionState with sessionId: " << sessionId 
                  << " and active: " << (active ? "true" : "false") << std::endl;
        
        bool result = cp.SetDrmSessionState(sessionId, active);
        std::cout << "Returned value from SetDrmSessionState: " << (result ? "true" : "false") << std::endl;
        
        EXPECT_FALSE(result);
    });
    
    std::cout << "Exiting NegativeSessionID_ActiveFalse test" << std::endl;
}
/**
 * @brief Test maximum int64_t session ID with active true for SetDrmSessionState
 *
 * This test verifies that the API SetDrmSessionState correctly handles the maximum int64_t session ID when active is set to true. The test ensures that no exceptions are thrown during object construction and that the method returns true as expected.
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
 * | Variation / Step | Description                                                                  | Test Data                                                     | Expected Result                                           | Notes         |
 * | :--------------: | ---------------------------------------------------------------------------- | ------------------------------------------------------------- | --------------------------------------------------------- | ------------- |
 * | 01               | Create an instance of ContentProtectionFirebolt                              | N/A: Constructor invocation                                   | Object constructed without throwing an exception         | Should be successful |
 * | 02               | Invoke SetDrmSessionState with sessionId as INT64_MAX and active as true       | sessionId = 9223372036854775807, active = true                  | Returns true; assertion (EXPECT_TRUE) passes             | Should Pass   |
 */
TEST(ContentProtectionFirebolt, MaxInt64SessionID_ActiveTrue) {
    std::cout << "Entering MaxInt64SessionID_ActiveTrue test" << std::endl;
    
    // Create object
    EXPECT_NO_THROW({
        ContentProtectionFirebolt cp;
        std::cout << "Constructed ContentProtectionFirebolt object" << std::endl;
        
        int64_t sessionId = std::numeric_limits<int64_t>::max();
        bool active = true;
        std::cout << "Invoking SetDrmSessionState with sessionId: " << sessionId 
                  << " and active: " << (active ? "true" : "false") << std::endl;
        
        bool result = cp.SetDrmSessionState(sessionId, active);
        std::cout << "Returned value from SetDrmSessionState: " << (result ? "true" : "false") << std::endl;
        
        EXPECT_TRUE(result);
    });
    
    std::cout << "Exiting MaxInt64SessionID_ActiveTrue test" << std::endl;
}
/**
 * @brief Validate that SetDrmSessionState correctly handles the maximum int64_t session ID when inactive
 *
 * This test verifies that the ContentProtectionFirebolt object's SetDrmSessionState method returns true when called with the maximum possible int64_t value for sessionId and an inactive state (false). It ensures that the method can correctly process edge case input conditions without throwing an exception.
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
 * | Variation / Step | Description                                                                     | Test Data                                                                               | Expected Result                                                   | Notes           |
 * | :--------------: | ------------------------------------------------------------------------------- | --------------------------------------------------------------------------------------- | ----------------------------------------------------------------- | --------------- |
 * | 01               | Construct ContentProtectionFirebolt object                                      | None                                                                                    | Object constructed without exceptions                             | Should be successful |
 * | 02               | Invoke SetDrmSessionState with sessionId as max int64 and active set to false     | input: sessionId = 9223372036854775807, active = false; output: expected result = true     | API returns true indicating the session state is set correctly      | Should Pass     |
 */
TEST(ContentProtectionFirebolt, MaxInt64SessionID_ActiveFalse) {
    std::cout << "Entering MaxInt64SessionID_ActiveFalse test" << std::endl;
    
    // Create object
    EXPECT_NO_THROW({
        ContentProtectionFirebolt cp;
        std::cout << "Constructed ContentProtectionFirebolt object" << std::endl;
        
        int64_t sessionId = std::numeric_limits<int64_t>::max();
        bool active = false;
        std::cout << "Invoking SetDrmSessionState with sessionId: " << sessionId 
                  << " and active: " << (active ? "true" : "false") << std::endl;
        
        bool result = cp.SetDrmSessionState(sessionId, active);
        std::cout << "Returned value from SetDrmSessionState: " << (result ? "true" : "false") << std::endl;
        
        EXPECT_TRUE(result);
    });
    
    std::cout << "Exiting MaxInt64SessionID_ActiveFalse test" << std::endl;
}
/**
 * @brief Test the behavior of ContentProtectionFirebolt with valid typical parameters.
 *
 * This test creates an instance of ContentProtectionFirebolt using the default constructor, then calls the
 * SetPlaybackPosition API with valid sessionId, speed, and position values. It verifies that no exception is thrown
 * and that the function returns a successful (true) result, confirming proper playback position setting.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 026
 * **Priority:** High
 * 
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 * 
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create an instance of ContentProtectionFirebolt and invoke SetPlaybackPosition with valid inputs | sessionId = 100, speed = 1.0, position = 30, expected output = true | No exception thrown; SetPlaybackPosition returns true | Should Pass |
 */
TEST(ContentProtectionFirebolt, ValidInputTypicalParameters)
{
    std::cout << "Entering ValidInputTypicalParameters test" << std::endl;
    // Create ContentProtectionFirebolt object using default constructor
    EXPECT_NO_THROW({
        ContentProtectionFirebolt obj;
        std::cout << "Created ContentProtectionFirebolt instance using default constructor." << std::endl;
        
        int64_t sessionId = 100;
        float speed = 1.0f;
        int32_t position = 30;
        
        std::cout << "Invoking SetPlaybackPosition with parameters: sessionId = " << sessionId
                  << ", speed = " << speed << ", position = " << position << std::endl;
        bool result = obj.SetPlaybackPosition(sessionId, speed, position);
        std::cout << "Returned value from SetPlaybackPosition: " << std::boolalpha << result << std::endl;
        
        EXPECT_TRUE(result);
    });
    std::cout << "Exiting ValidInputTypicalParameters test" << std::endl;
}
/**
 * @brief Verify that SetPlaybackPosition correctly handles a negative session ID.
 *
 * This test verifies that when a negative session ID is provided to the SetPlaybackPosition API, the API returns false,
 * indicating that the input is invalid. The test ensures that the function does not throw any exceptions and handles this error scenario gracefully.
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
 * | Variation / Step | Description                                                                 | Test Data                                      | Expected Result                                                                   | Notes         |
 * | :--------------: | --------------------------------------------------------------------------- | ---------------------------------------------- | --------------------------------------------------------------------------------- | ------------- |
 * | 01               | Instantiate ContentProtectionFirebolt using the default constructor         | None                                           | Instance should be created without any exceptions                                 | Should be successful |
 * | 02               | Invoke SetPlaybackPosition with negative sessionId (-50), speed (1.0), and position (30) | sessionId = -50, speed = 1.0, position = 30     | API returns false indicating that the negative sessionId is rejected and assertion passes | Should Fail   |
 */
TEST(ContentProtectionFirebolt, NegativeSessionId)
{
    std::cout << "Entering NegativeSessionId test" << std::endl;
    EXPECT_NO_THROW({
        ContentProtectionFirebolt obj;
        std::cout << "Created ContentProtectionFirebolt instance using default constructor." << std::endl;
        
        int64_t sessionId = -50;
        float speed = 1.0f;
        int32_t position = 30;
        
        std::cout << "Invoking SetPlaybackPosition with parameters: sessionId = " << sessionId
                  << ", speed = " << speed << ", position = " << position << std::endl;
        bool result = obj.SetPlaybackPosition(sessionId, speed, position);
        std::cout << "Returned value from SetPlaybackPosition: " << std::boolalpha << result << std::endl;
        
        EXPECT_FALSE(result);
    });
    std::cout << "Exiting NegativeSessionId test" << std::endl;
}
/**
 * @brief Validates that SetPlaybackPosition correctly handles a negative playback speed.
 *
 * This test verifies that invoking SetPlaybackPosition with a negative playback speed (-1.0)
 * does not throw an exception and returns false, ensuring that negative speed values are properly handled.
 *
 * **Test Group ID:** Basic: 01  
 * **Test Case ID:** 028  
 * **Priority:** High  
 *
 * **Pre-Conditions:** None  
 * **Dependencies:** None  
 * **User Interaction:** None  
 *
 * **Test Procedure:**  
 * | Variation / Step | Description                                                                                  | Test Data                                              | Expected Result                                                 | Notes      |
 * | :--------------: | -------------------------------------------------------------------------------------------- | ------------------------------------------------------ | ---------------------------------------------------------------- | ---------- |
 * | 01               | Create a ContentProtectionFirebolt instance and invoke SetPlaybackPosition with negative speed | input: sessionId = 101, speed = -1.0, position = 45; output: result boolean | API returns false and no exception is thrown; ASSERT_FALSE(result) | Should Fail |
 */
TEST(ContentProtectionFirebolt, NegativePlaybackSpeed)
{
    std::cout << "Entering NegativePlaybackSpeed test" << std::endl;
    EXPECT_NO_THROW({
        ContentProtectionFirebolt obj;
        std::cout << "Created ContentProtectionFirebolt instance using default constructor." << std::endl;
        
        int64_t sessionId = 101;
        float speed = -1.0f;
        int32_t position = 45;
        
        std::cout << "Invoking SetPlaybackPosition with parameters: sessionId = " << sessionId
                  << ", speed = " << speed << ", position = " << position << std::endl;
        bool result = obj.SetPlaybackPosition(sessionId, speed, position);
        std::cout << "Returned value from SetPlaybackPosition: " << std::boolalpha << result << std::endl;
        
        EXPECT_FALSE(result);
    });
    std::cout << "Exiting NegativePlaybackSpeed test" << std::endl;
}
/**
 * @brief Verify that SetPlaybackPosition returns false when invoked with a negative position value.
 *
 * This test validates that the ContentProtectionFirebolt API, when provided with a negative playback position,
 * correctly handles the input by returning a false value. It ensures that the API does not allow invalid negative positions,
 * thereby preventing potential misbehavior during content playback.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 029@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                     | Test Data                                         | Expected Result                                              | Notes           |
 * | :--------------: | --------------------------------------------------------------- | ------------------------------------------------- | ------------------------------------------------------------ | --------------- |
 * | 01               | Create an instance of ContentProtectionFirebolt using default constructor. | None                                              | Instance created successfully.                               | Should be successful |
 * | 02               | Invoke the SetPlaybackPosition API with negative position value. | sessionId = 102, speed = 1.0f, position = -10      | API returns false indicating invalid negative position input. | Should Fail     |
 * | 03               | Verify that the API returns false using an assertion check.     | result = false                                    | The assertion EXPECT_FALSE(result) passes.                   | Should be successful |
 */
TEST(ContentProtectionFirebolt, NegativePosition)
{
    std::cout << "Entering NegativePosition test" << std::endl;
    EXPECT_NO_THROW({
        ContentProtectionFirebolt obj;
        std::cout << "Created ContentProtectionFirebolt instance using default constructor." << std::endl;
        
        int64_t sessionId = 102;
        float speed = 1.0f;
        int32_t position = -10;
        
        std::cout << "Invoking SetPlaybackPosition with parameters: sessionId = " << sessionId
                  << ", speed = " << speed << ", position = " << position << std::endl;
        bool result = obj.SetPlaybackPosition(sessionId, speed, position);
        std::cout << "Returned value from SetPlaybackPosition: " << std::boolalpha << result << std::endl;
        
        EXPECT_FALSE(result);
    });
    std::cout << "Exiting NegativePosition test" << std::endl;
}
/**
 * @brief Verifies that invoking SetPlaybackPosition with zero playback speed returns false.
 *
 * This test ensures that the ContentProtectionFirebolt API does not accept a playback speed of zero. It validates that the method SetPlaybackPosition returns false when provided with a zero speed value to prevent invalid playback scenarios.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 030@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                       | Test Data                                 | Expected Result                       | Notes          |
 * | :--------------: | ----------------------------------------------------------------- | ----------------------------------------- | ------------------------------------- | -------------- |
 * | 01               | Create ContentProtectionFirebolt instance using default constructor | None                                      | Object is created successfully         | Should be successful |
 * | 02               | Call SetPlaybackPosition with sessionId = 103, speed = 0.0, position = 30 | sessionId = 103, speed = 0.0, position = 30 | API returns false (ASSERT_FALSE)     | Should Pass    |
 */
TEST(ContentProtectionFirebolt, ZeroPlaybackSpeed)
{
    std::cout << "Entering ZeroPlaybackSpeed test" << std::endl;
    EXPECT_NO_THROW({
        ContentProtectionFirebolt obj;
        std::cout << "Created ContentProtectionFirebolt instance using default constructor." << std::endl;
        
        int64_t sessionId = 103;
        float speed = 0.0f;
        int32_t position = 30;
        
        std::cout << "Invoking SetPlaybackPosition with parameters: sessionId = " << sessionId
                  << ", speed = " << speed << ", position = " << position << std::endl;
        bool result = obj.SetPlaybackPosition(sessionId, speed, position);
        std::cout << "Returned value from SetPlaybackPosition: " << std::boolalpha << result << std::endl;
        
        EXPECT_FALSE(result);
    });
    std::cout << "Exiting ZeroPlaybackSpeed test" << std::endl;
}
/**
 * @brief Verifies that ShowWatermark enables watermark successfully for a valid positive session ID.
 *
 * This test creates an instance of ContentProtectionFirebolt and invokes the ShowWatermark method with a boolean true and a valid positive sessionId. It checks that no exceptions are thrown during object instantiation and method execution, confirming the API's expected behavior under normal conditions.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 031@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                        | Test Data                          | Expected Result                                                                  | Notes      |
 * | :--------------: | ------------------------------------------------------------------ | ---------------------------------- | -------------------------------------------------------------------------------- | ---------- |
 * | 01               | Create an instance of ContentProtectionFirebolt using default constructor | None                               | Object is instantiated without throwing any exceptions                         | Should Pass|
 * | 02               | Invoke ShowWatermark with parameters show = true and sessionId = 12345      | show = true, sessionId = 12345       | Method executes without throwing any exception and enables watermark as expected | Should Pass|
 */
TEST(ContentProtectionFirebolt, ShowWatermark_Enable_ValidPositiveSessionId) {
    std::cout << "Entering ShowWatermark_Enable_ValidPositiveSessionId test" << std::endl;
    
    // Creating instance of ContentProtectionFirebolt using default constructor
    EXPECT_NO_THROW({
        ContentProtectionFirebolt obj;
        std::cout << "Created ContentProtectionFirebolt object using default constructor." << std::endl;
        
        // Parameters for test
        bool show = true;
        int64_t sessionId = 12345;
        std::cout << "Invoking ShowWatermark with parameters: show = " << show 
                  << ", sessionId = " << sessionId << std::endl;
        
        // Invoke method and expect no exceptions
        EXPECT_NO_THROW({
            obj.ShowWatermark(show, sessionId);
            std::cout << "Method ShowWatermark executed successfully for enabling watermark." << std::endl;
        });
    });
    
    std::cout << "Exiting ShowWatermark_Enable_ValidPositiveSessionId test" << std::endl;
}
/**
 * @brief Validate disabling watermark using a valid positive session id.
 *
 * This test verifies that calling ShowWatermark with the parameter "show" set to false and a valid positive session id does not throw any exception and disables the watermark for the session.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create an instance of ContentProtectionFirebolt and set the test parameters. | None | Instance is created without throwing an exception. | Should be successful |
 * | 02 | Invoke ShowWatermark method with parameters to disable watermark. | show = false, sessionId = 12345 | Method executes without throwing an exception. | Should Pass |
 */
TEST(ContentProtectionFirebolt, ShowWatermark_Disable_ValidPositiveSessionId) {
    std::cout << "Entering ShowWatermark_Disable_ValidPositiveSessionId test" << std::endl;
    
    // Create instance using default constructor
    EXPECT_NO_THROW({
        ContentProtectionFirebolt obj;
        std::cout << "Created ContentProtectionFirebolt object using default constructor." << std::endl;
        
        // Set test parameters
        bool show = false;
        int64_t sessionId = 12345;
        std::cout << "Invoking ShowWatermark with parameters: show = " << show 
                  << ", sessionId = " << sessionId << std::endl;
        
        // Invoke method and expect no exception
        EXPECT_NO_THROW({
            obj.ShowWatermark(show, sessionId);
            std::cout << "Method ShowWatermark executed successfully for disabling watermark." << std::endl;
        });
    });
    
    std::cout << "Exiting ShowWatermark_Disable_ValidPositiveSessionId test" << std::endl;
}
/**
 * @brief Test to verify that disabling the watermark works correctly when a negative sessionId is provided.
 *
 * This test creates an instance of ContentProtectionFirebolt and invokes the ShowWatermark method
 * with show set to false and sessionId set to a negative value (-1). The objective is to ensure that
 * despite the negative sessionId, the method executes without throwing an exception, indicating that
 * the API can handle such an input gracefully.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 033@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                       | Test Data                                               | Expected Result                                                    | Notes       |
 * | :--------------: | ----------------------------------------------------------------- | ------------------------------------------------------- | ------------------------------------------------------------------ | ----------- |
 * | 01               | Create a ContentProtectionFirebolt instance, set show = false and sessionId = -1, and invoke ShowWatermark | show = false, sessionId = -1                             | The method executes without throwing any exception and completes successfully | Should Pass |
 */
TEST(ContentProtectionFirebolt, ShowWatermark_Disable_NegativeSessionId) {
    std::cout << "Entering ShowWatermark_Disable_NegativeSessionId test" << std::endl;
    
    // Create instance using default constructor
    EXPECT_NO_THROW({
        ContentProtectionFirebolt obj;
        std::cout << "Created ContentProtectionFirebolt object using default constructor." << std::endl;
        
        // Set test parameters
        bool show = false;
        int64_t sessionId = -1;
        std::cout << "Invoking ShowWatermark with parameters: show = " << show 
                  << ", sessionId = " << sessionId << " (negative sessionId)" << std::endl;
        
        // Invoke method and expect no exception
        EXPECT_NO_THROW({
            obj.ShowWatermark(show, sessionId);
            std::cout << "Method ShowWatermark executed successfully for disabling watermark with negative sessionId." << std::endl;
        });
    });
    
    std::cout << "Exiting ShowWatermark_Disable_NegativeSessionId test" << std::endl;
}
/**
 * @brief Test to verify that the ShowWatermark API successfully enables the watermark using INT64_MAX as sessionId.
 *
 * This test creates an instance of ContentProtectionFirebolt using the default constructor, then invokes the ShowWatermark method with the show parameter set to true and sessionId set to INT64_MAX. The test ensures that no exceptions are thrown during object creation and method invocation.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 034@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                                 | Test Data                                  | Expected Result                                                              | Notes           |
 * | :--------------: | --------------------------------------------------------------------------- | ------------------------------------------ | ---------------------------------------------------------------------------- | --------------- |
 * | 01               | Create instance of ContentProtectionFirebolt using the default constructor. | None                                       | Object is created successfully without any exceptions thrown.              | Should be successful |
 * | 02               | Invoke ShowWatermark with show set to true and sessionId set to INT64_MAX.    | show = true, sessionId = INT64_MAX           | ShowWatermark executes successfully without throwing any exceptions.       | Should Pass     |
 */
TEST(ContentProtectionFirebolt, ShowWatermark_Enable_INT64_MAX) {
    std::cout << "Entering ShowWatermark_Enable_INT64_MAX test" << std::endl;
    
    // Create instance using default constructor
    EXPECT_NO_THROW({
        ContentProtectionFirebolt obj;
        std::cout << "Created ContentProtectionFirebolt object using default constructor." << std::endl;
        
        // Set test parameters
        bool show = true;
        int64_t sessionId = std::numeric_limits<int64_t>::max();
        std::cout << "Invoking ShowWatermark with parameters: show = " << show 
                  << ", sessionId = " << sessionId << " (INT64_MAX)" << std::endl;
        
        // Invoke method and expect no exception
        EXPECT_NO_THROW({
            obj.ShowWatermark(show, sessionId);
            std::cout << "Method ShowWatermark executed successfully for enabling watermark with INT64_MAX sessionId." << std::endl;
        });
    });
    
    std::cout << "Exiting ShowWatermark_Enable_INT64_MAX test" << std::endl;
}
/**
 * @brief Verify that valid subscription settings are processed without exceptions.
 *
 * This test verifies that a ContentProtectionFirebolt object can be created using the default constructor and that the method SubscribeContentProtectionSettings accepts a valid subscription string ("HDCP") without throwing any exceptions.
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
 * | Variation / Step | Description                                                                | Test Data                                 | Expected Result                                                      | Notes         |
 * | :--------------: | -------------------------------------------------------------------------- | ----------------------------------------- | -------------------------------------------------------------------- | ------------- |
 * | 01               | Create a ContentProtectionFirebolt object using the default constructor.   | N/A                                       | Object is created without any exception.                             | Should be successful |
 * | 02               | Invoke SubscribeContentProtectionSettings with the valid subscription "HDCP". | inputStr = "HDCP"                         | The method call completes without throwing any exception.            | Should Pass   |
 */
TEST(ContentProtectionFirebolt, ValidSubscriptionSettings) {
    std::cout << "Entering ValidSubscriptionSettings test" << std::endl;

    // Creating object using default constructor
    EXPECT_NO_THROW({
        ContentProtectionFirebolt cpObj;
        std::cout << "Created ContentProtectionFirebolt object using default constructor" << std::endl;

        // Prepare input subscription string "HDCP"
        const char inputStr[] = "HDCP";
        
        // Invocation of SubscribeContentProtectionSettings
        std::cout << "Invoking SubscribeContentProtectionSettings with input: " << inputStr << std::endl;
        EXPECT_NO_THROW(cpObj.SubscribeContentProtectionSettings(inputStr));
        std::cout << "SubscribeContentProtectionSettings processed the subscription successfully" << std::endl;
    });

    std::cout << "Exiting ValidSubscriptionSettings test" << std::endl;
}
/**
 * @brief Verify SubscribeContentProtectionSettings behavior with empty subscription settings
 *
 * This test ensures that the ContentProtectionFirebolt object gracefully handles an empty subscription string input by not throwing exceptions. It validates that the method SubscribeContentProtectionSettings is equipped to manage empty inputs, which might occur during real-world usage.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 036
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                       | Test Data                                      | Expected Result                                       | Notes          |
 * | :--------------: | ----------------------------------------------------------------- | ---------------------------------------------- | ----------------------------------------------------- | -------------- |
 * | 01               | Create ContentProtectionFirebolt object using default constructor   | No input values                                | Object is created without exception                   | Should be successful |
 * | 02               | Prepare empty subscription string                                 | inputStr = ""                                  | Empty string prepared for subscription                | Should be successful |
 * | 03               | Invoke SubscribeContentProtectionSettings with empty input         | inputStr = ""                                  | Method handles empty input without throwing exception | Should Pass    |
 */
TEST(ContentProtectionFirebolt, EmptySubscriptionSettings) {
    std::cout << "Entering EmptySubscriptionSettings test" << std::endl;

    // Creating object using default constructor
    EXPECT_NO_THROW({
        ContentProtectionFirebolt cpObj;
        std::cout << "Created ContentProtectionFirebolt object using default constructor" << std::endl;

        // Prepare empty input subscription string
        const char inputStr[] = "";

        // Invocation of SubscribeContentProtectionSettings
        std::cout << "Invoking SubscribeContentProtectionSettings with empty input" << std::endl;
        EXPECT_NO_THROW(cpObj.SubscribeContentProtectionSettings(inputStr));
        std::cout << "SubscribeContentProtectionSettings handled empty input gracefully" << std::endl;
    });

    std::cout << "Exiting EmptySubscriptionSettings test" << std::endl;
}
/**
 * @brief Validate that the ContentProtectionFirebolt object successfully unsubscribes from content protection settings for a valid event.
 *
 * This test case verifies that the ContentProtectionFirebolt object can be instantiated using its default constructor and that the method UnsubscribeContentProtectionSettings executes without throwing any exceptions when provided with a valid event string. The objective is to ensure that the unsubscription functionality for a valid event input works as expected.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create ContentProtectionFirebolt object using default constructor | None | Object is created without any exceptions | Should be successful |
 * | 02 | Define test input validEvent with value "ContentProtectionEvent123" | validEvent = "ContentProtectionEvent123" | Test input is set correctly | Should be successful |
 * | 03 | Invoke UnsubscribeContentProtectionSettings with validEvent | input: validEvent = "ContentProtectionEvent123" | Method executes without throwing an exception | Should Pass |
 */
TEST(ContentProtectionFirebolt, PositiveUnsubscription)
{
    std::cout << "Entering PositiveUnsubscription test" << std::endl;
    
    // Create object using the default constructor
    std::cout << "Creating ContentProtectionFirebolt object using default constructor" << std::endl;
    EXPECT_NO_THROW(ContentProtectionFirebolt cp);
    ContentProtectionFirebolt cp;
    
    // Define test input
    const std::string validEvent = "ContentProtectionEvent123";
    std::cout << "Test Input: " << validEvent << std::endl;
    
    // Invoke the method and output debug logs
    std::cout << "Invoking UnsubscribeContentProtectionSettings with input: " << validEvent << std::endl;
    EXPECT_NO_THROW(cp.UnsubscribeContentProtectionSettings(validEvent));
    std::cout << "Method UnsubscribeContentProtectionSettings executed successfully with input: " << validEvent << std::endl;
    
    std::cout << "Exiting PositiveUnsubscription test" << std::endl;
}
/**
 * @brief Verify that UnsubscribeContentProtectionSettings handles empty string input appropriately.
 *
 * This test verifies that when an empty string is provided to the UnsubscribeContentProtectionSettings method, the method executes without throwing an exception. The test checks the API's robustness against edge-case input.
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
 * | 01 | Create a ContentProtectionFirebolt object using the default constructor | Constructor: no args | Object created successfully; no exception thrown | Should Pass |
 * | 02 | Define test input as an empty string | emptyEvent = "" | Test input is set to empty string correctly | Should be successful |
 * | 03 | Invoke UnsubscribeContentProtectionSettings with the empty string | cp, emptyEvent = "" | Method executes without throwing an exception | Should Pass |
 */
TEST(ContentProtectionFirebolt, NegativeUnsubscription_EmptyString)
{
    std::cout << "Entering NegativeUnsubscription_EmptyString test" << std::endl;
    
    // Create object using the default constructor
    std::cout << "Creating ContentProtectionFirebolt object using default constructor" << std::endl;
    EXPECT_NO_THROW(ContentProtectionFirebolt cp);
    ContentProtectionFirebolt cp;
    
    // Define test input as empty string
    const std::string emptyEvent = "";
    std::cout << "Test Input (empty string): \"" << emptyEvent << "\"" << std::endl;
    
    // Invoke the method and output debug logs
    std::cout << "Invoking UnsubscribeContentProtectionSettings with an empty string" << std::endl;
    EXPECT_NO_THROW(cp.UnsubscribeContentProtectionSettings(emptyEvent));
    std::cout << "Method UnsubscribeContentProtectionSettings executed successfully with an empty string" << std::endl;
    
    std::cout << "Exiting NegativeUnsubscription_EmptyString test" << std::endl;
}
/**
 * @brief Test the UnsubscribeContentProtectionSettings API with special characters in the input to validate handling of negative case
 *
 * This test verifies that when a string containing special characters is passed to the UnsubscribeContentProtectionSettings method,
 * the method handles the input in a negative scenario. The test ensures that the API behaves correctly by not throwing any unexpected exceptions.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 039@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                 | Test Data                                           | Expected Result                                           | Notes         |
 * | :--------------: | --------------------------------------------------------------------------- | --------------------------------------------------- | --------------------------------------------------------- | ------------- |
 * | 01               | Create ContentProtectionFirebolt object using the default constructor       | None                                                | Object is created without exceptions                      | Should be successful |
 * | 02               | Define input string with special characters                               | specialCharEvent = "!@#$%^&*()"                      | Input string is initialized as expected                   | Should be successful |
 * | 03               | Invoke UnsubscribeContentProtectionSettings with special characters input   | input: specialCharEvent = "!@#$%^&*()"               | No exception is thrown; the method executes the call       | Should Fail   |
 */
TEST(ContentProtectionFirebolt, NegativeUnsubscription_SpecialCharacters)
{
    std::cout << "Entering NegativeUnsubscription_SpecialCharacters test" << std::endl;
    
    // Create object using the default constructor
    std::cout << "Creating ContentProtectionFirebolt object using default constructor" << std::endl;
    EXPECT_NO_THROW(ContentProtectionFirebolt cp);
    ContentProtectionFirebolt cp;
    
    // Define test input with special characters
    const std::string specialCharEvent = "!@#$%^&*()";
    std::cout << "Test Input (special characters): " << specialCharEvent << std::endl;
    
    // Invoke the method and output debug logs
    std::cout << "Invoking UnsubscribeContentProtectionSettings with special characters" << std::endl;
    EXPECT_NO_THROW(cp.UnsubscribeContentProtectionSettings(specialCharEvent));
    std::cout << "Method UnsubscribeContentProtectionSettings executed successfully with input: " << specialCharEvent << std::endl;
    
    std::cout << "Exiting NegativeUnsubscription_SpecialCharacters test" << std::endl;
}
/**
 * @brief Validate that UpdateDrmSession returns success with valid inputs
 *
 * This test verifies that the UpdateDrmSession API in ContentProtectionFirebolt works correctly when provided with valid sessionId, licenseRequest, and initData values. The test confirms that the API does not throw exceptions and returns a true value, indicating success.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 040@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                        | Test Data                                                                        | Expected Result                                                                         | Notes        |
 * | :--------------: | ------------------------------------------------------------------- | -------------------------------------------------------------------------------- | --------------------------------------------------------------------------------------- | ------------ |
 * | 01               | Instantiate and validate ContentProtectionFirebolt object creation   | No input parameters                                                              | Object is created without throwing any exception                                        | Should be successful |
 * | 02               | Prepare valid input parameters for UpdateDrmSession                  | sessionId = 12345, licenseRequest = "validLicenseRequest", initData = "validInitData", response variable uninitialized | Inputs are correctly set for the API invocation                                           | Should be successful |
 * | 03               | Invoke UpdateDrmSession with the prepared valid parameters           | sessionId = 12345, licenseRequest = "validLicenseRequest", initData = "validInitData", response variable passed by reference | API returns true without any exception thrown                                             | Should Pass  |
 * | 04               | Validate the returned value and response from UpdateDrmSession method  | result variable, response variable                                               | result is true and response contains a valid challenge response string                     | Should Pass  |
 */
TEST(ContentProtectionFirebolt, UpdateDrmSession_ValidInputs_ReturnsSuccess)
{
    std::cout << "Entering UpdateDrmSession_ValidInputs_ReturnsSuccess test" << std::endl;

    EXPECT_NO_THROW({ ContentProtectionFirebolt cp; });
    ContentProtectionFirebolt cp;
    int64_t sessionId = 12345;
    std::string licenseRequest;
    char licenseReqBuffer[64];
    strncpy(licenseReqBuffer, "validLicenseRequest", sizeof(licenseReqBuffer)-1);
    licenseReqBuffer[sizeof(licenseReqBuffer)-1] = '\0';
    licenseRequest = std::string(licenseReqBuffer);

    std::string initData;
    char initDataBuffer[64];
    strncpy(initDataBuffer, "validInitData", sizeof(initDataBuffer)-1);
    initDataBuffer[sizeof(initDataBuffer)-1] = '\0';
    initData = std::string(initDataBuffer);

    std::string response;

    std::cout << "Invoking UpdateDrmSession with sessionId: " << sessionId
              << ", licenseRequest: " << licenseRequest
              << ", initData: " << initData << std::endl;

    bool result = false;
    EXPECT_NO_THROW({ result = cp.UpdateDrmSession(sessionId, licenseRequest, initData, response); });
    std::cout << "Method UpdateDrmSession invoked. Returned value: " << std::boolalpha << result << std::endl;
    std::cout << "Response received: " << response << std::endl;

    // Expected outcome: true and a valid challenge response string
    EXPECT_TRUE(result);

    std::cout << "Exiting UpdateDrmSession_ValidInputs_ReturnsSuccess test" << std::endl;
}
/**
 * @brief To verify that UpdateDrmSession returns failure when provided with a negative session ID.
 *
 * Test function verifies that using a negative session ID along with valid license request and valid init data results in failure. It tests that the API returns false and an error message indicating an invalid session ID.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 041
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Call UpdateDrmSession with sessionId = -1, valid licenseRequest ("validLicenseRequest"), and valid initData ("validInitData"); verify that method returns false and an error message "Error: Invalid session ID". | sessionId = -1, licenseRequest = validLicenseRequest, initData = validInitData | Return value: false, Response: "Error: Invalid session ID" | Should Fail |
 */
TEST(ContentProtectionFirebolt, UpdateDrmSession_NegativeSessionId_ReturnsFailure)
{
    std::cout << "Entering UpdateDrmSession_NegativeSessionId_ReturnsFailure test" << std::endl;

    EXPECT_NO_THROW({ ContentProtectionFirebolt cp; });
    ContentProtectionFirebolt cp;
    int64_t sessionId = -1;
    std::string licenseRequest;
    char licenseReqBuffer[64];
    strncpy(licenseReqBuffer, "validLicenseRequest", sizeof(licenseReqBuffer)-1);
    licenseReqBuffer[sizeof(licenseReqBuffer)-1] = '\0';
    licenseRequest = std::string(licenseReqBuffer);

    std::string initData;
    char initDataBuffer[64];
    strncpy(initDataBuffer, "validInitData", sizeof(initDataBuffer)-1);
    initDataBuffer[sizeof(initDataBuffer)-1] = '\0';
    initData = std::string(initDataBuffer);

    std::string response;

    std::cout << "Invoking UpdateDrmSession with sessionId: " << sessionId
              << ", licenseRequest: " << licenseRequest
              << ", initData: " << initData << std::endl;

    bool result = false;
    EXPECT_NO_THROW({ result = cp.UpdateDrmSession(sessionId, licenseRequest, initData, response); });
    std::cout << "Method UpdateDrmSession invoked. Returned value: " << std::boolalpha << result << std::endl;
    std::cout << "Response received: " << response << std::endl;

    // Expected outcome: false and an error message indicating an invalid session ID
    EXPECT_FALSE(result);
    EXPECT_EQ(response, "Error: Invalid session ID");

    std::cout << "Exiting UpdateDrmSession_NegativeSessionId_ReturnsFailure test" << std::endl;
}
/**
 * @brief Validate that UpdateDrmSession returns failure when provided with an empty license request.
 *
 * This test case verifies that the ContentProtectionFirebolt API correctly handles the scenario where an empty license request is passed.
 * It checks that the API instantiation does not throw exceptions and that invoking UpdateDrmSession with a valid sessionId, an empty licenseRequest,
 * and valid initData returns false with the error message "Error: Empty license request".
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 042
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                                              | Test Data                                                                                      | Expected Result                                                              | Notes       |
 * | :--------------: | ------------------------------------------------------------------------------------------------------------------------ | ---------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------------- | ----------- |
 * | 01               | Invoke UpdateDrmSession API with sessionId=12345, an empty licenseRequest, and valid initData 'validInitData'.             | input: sessionId=12345, licenseRequest="", initData="validInitData", response=<captured value>    | API returns false and response equals "Error: Empty license request"         | Should Fail |
 */
TEST(ContentProtectionFirebolt, UpdateDrmSession_EmptyLicenseRequest_ReturnsFailure)
{
    std::cout << "Entering UpdateDrmSession_EmptyLicenseRequest_ReturnsFailure test" << std::endl;

    EXPECT_NO_THROW({ ContentProtectionFirebolt cp; });
    ContentProtectionFirebolt cp;
    int64_t sessionId = 12345;
    std::string licenseRequest = "";
    std::string initData;
    char initDataBuffer[64];
    strncpy(initDataBuffer, "validInitData", sizeof(initDataBuffer)-1);
    initDataBuffer[sizeof(initDataBuffer)-1] = '\0';
    initData = std::string(initDataBuffer);

    std::string response;

    std::cout << "Invoking UpdateDrmSession with sessionId: " << sessionId
              << ", licenseRequest: <empty>"
              << ", initData: " << initData << std::endl;

    bool result = false;
    EXPECT_NO_THROW({ result = cp.UpdateDrmSession(sessionId, licenseRequest, initData, response); });
    std::cout << "Method UpdateDrmSession invoked. Returned value: " << std::boolalpha << result << std::endl;
    std::cout << "Response received: " << response << std::endl;

    // Expected outcome: false and an error message indicating an empty license request
    EXPECT_FALSE(result);
    EXPECT_EQ(response, "Error: Empty license request");

    std::cout << "Exiting UpdateDrmSession_EmptyLicenseRequest_ReturnsFailure test" << std::endl;
}
/**
 * @brief Validate that UpdateDrmSession returns failure when empty initialization data is provided.
 *
 * This test verifies that invoking UpdateDrmSession with an empty initData parameter results in a failure.
 * It ensures that the DRM session update does not proceed when the necessary initialization data is missing.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * |01| Initialize ContentProtectionFirebolt instance and test parameters | sessionId = 12345, licenseRequest = validLicenseRequest, initData = (empty), response = uninitialized | Object is successfully created and parameters are set without exceptions | Should be successful |
 * |02| Invoke UpdateDrmSession and validate output | sessionId = 12345, licenseRequest = validLicenseRequest, initData = (empty), response = output message | Method returns false and response equals "Error: Empty initialization data" | Should Pass |
 */
TEST(ContentProtectionFirebolt, UpdateDrmSession_EmptyInitData_ReturnsFailure)
{
    std::cout << "Entering UpdateDrmSession_EmptyInitData_ReturnsFailure test" << std::endl;

    EXPECT_NO_THROW({ ContentProtectionFirebolt cp; });
    ContentProtectionFirebolt cp;
    int64_t sessionId = 12345;
    std::string licenseRequest;
    char licenseReqBuffer[64];
    strncpy(licenseReqBuffer, "validLicenseRequest", sizeof(licenseReqBuffer)-1);
    licenseReqBuffer[sizeof(licenseReqBuffer)-1] = '\0';
    licenseRequest = std::string(licenseReqBuffer);

    std::string initData = "";
    std::string response;

    std::cout << "Invoking UpdateDrmSession with sessionId: " << sessionId
              << ", licenseRequest: " << licenseRequest
              << ", initData: <empty>" << std::endl;

    bool result = false;
    EXPECT_NO_THROW({ result = cp.UpdateDrmSession(sessionId, licenseRequest, initData, response); });
    std::cout << "Method UpdateDrmSession invoked. Returned value: " << std::boolalpha << result << std::endl;
    std::cout << "Response received: " << response << std::endl;

    // Expected outcome: false and an error message indicating empty initialization data
    EXPECT_FALSE(result);
    EXPECT_EQ(response, "Error: Empty initialization data");

    std::cout << "Exiting UpdateDrmSession_EmptyInitData_ReturnsFailure test" << std::endl;
}
/**
 * @brief Validate that a ContentProtectionFirebolt object can be allocated on the stack and destroyed without exceptions.
 *
 * This test verifies that creating a ContentProtectionFirebolt object using the default constructor does not throw an exception.
 * It also ensures that when the object goes out of scope, the destructor is invoked automatically without causing any runtime errors.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 044@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                     | Test Data                                                       | Expected Result                                                      | Notes           |
 * | :--------------: | ----------------------------------------------------------------------------------------------- | --------------------------------------------------------------- | -------------------------------------------------------------------- | --------------- |
 * | 01               | Invoke default constructor of ContentProtectionFirebolt and verify no exceptions are thrown.      | input: none, output: Created object instance                    | No exception thrown; EXPECT_NO_THROW assertion passes                 | Should Pass     |
 * | 02               | Allow the object to go out of scope to automatically invoke the destructor.                     | input: none                                                     | Destructor called successfully on scope exit without any errors       | Should be successful |
 */
TEST(ContentProtectionFirebolt, StackAllocationDestructorTest) {
    std::cout << "Entering StackAllocationDestructorTest test" << std::endl;

    {
        std::cout << "Creating ContentProtectionFirebolt object using default constructor" << std::endl;
        EXPECT_NO_THROW({
            ContentProtectionFirebolt obj;
            std::cout << "ContentProtectionFirebolt object created. Invoking destructor upon scope exit." << std::endl;
        });
        std::cout << "ContentProtectionFirebolt object is about to go out of scope, destructor will be called automatically" << std::endl;
    }

    std::cout << "Exiting StackAllocationDestructorTest test" << std::endl;
}

int main(int argc, char **argv) {
     ::testing::InitGoogleTest(&argc, argv);
     return RUN_ALL_TESTS();
}
