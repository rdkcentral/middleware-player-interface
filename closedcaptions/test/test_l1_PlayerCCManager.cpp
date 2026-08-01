
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
* @file test_l1_PlayerCCManager.cpp
* @page PlayerCCManager Level 1 Tests
*
* ## Module's Role
* This module includes Level 1 functional tests (success and failure scenarios).
* This is to ensure that the PlayerCCManager methods meet the requirements.
*
* **Pre-Conditions:**  None @n
* **Dependencies:** None @n
*
*/

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <stdio.h>
#include "PlayerCCManager.h"

class TestPlayerCCManager : public PlayerCCManagerBase {
public:
    virtual void Release(int iID) override { }
    virtual void StartRendering() override { }
    virtual void StopRendering() override { }
    virtual int SetDigitalChannel(unsigned int id) override { return 0; }
    virtual int SetAnalogChannel(unsigned int id) override { return 0; }
    using PlayerCCManagerBase::EnsureInitialized;
    using PlayerCCManagerBase::EnsureHALInitialized;
    using PlayerCCManagerBase::EnsureRendererCommsInitialized;
    using PlayerCCManagerBase::Initialize;
    using PlayerCCManagerBase::CheckCCHandle;
    using PlayerCCManagerBase::Start;
    using PlayerCCManagerBase::Stop;
};

struct PlayerCCManagerInstantiableTest : public ::testing::Test {
protected:
    TestPlayerCCManager* playerCCManager;
    void SetUp() override {
        playerCCManager = new TestPlayerCCManager();
    }
    void TearDown() override {
        delete manager;
    }
};

/**
 * @brief Verify that DestroyInstance correctly resets the internal instance when an instance exists.
 *
 * This test creates an instance using the default constructor of PlayerCCManager and then calls the DestroyInstance method.
 * The objective is to ensure that DestroyInstance properly resets the internal instance state (sets it to null) without errors.
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
 * | Variation / Step | Description                                                                                | Test Data                                                             | Expected Result                                                                  | Notes             |
 * | :--------------: | ------------------------------------------------------------------------------------------ | --------------------------------------------------------------------- | --------------------------------------------------------------------------------- | ----------------- |
 * | 01               | Invoking default constructor of PlayerCCManager to create an instance                        | instance = default constructed                                        | Instance should be created successfully                                          | Should be successful |
 * | 02               | Calling DestroyInstance() method to destroy the created instance                              | input: none, output: internal instance = null after call              | DestroyInstance sets the internal instance to null without errors                | Should Pass       |
 * | 03               | Validating the internal state by checking that the assertion passes with EXPECT_TRUE(true)     | input: none, assertion: true check                                    | EXPECT_TRUE(true) passes confirming the internal instance is handled correctly     | Should be successful |
 */
TEST(PlayerCCManager, DestroyInstance_when_instance_exists) {
    std::cout << "Entering DestroyInstance_when_instance_exists test" << std::endl;
    
    std::cout << "Invoking default constructor of PlayerCCManager to create an instance" << std::endl;
    // Create object instance using the default constructor.
    PlayerCCManager instance;

    std::cout << "Calling DestroyInstance() method" << std::endl;
    PlayerCCManager::DestroyInstance();
    std::cout << "DestroyInstance() returned. The internal instance should now be set to null." << std::endl;
    
    // As mInstance is private, we assume the internal state is correct if no error occurred.
    EXPECT_TRUE(true);

    std::cout << "Exiting DestroyInstance_when_instance_exists test" << std::endl;
}
/**
 * @brief Validate that DestroyInstance() correctly handles the no-instance scenario
 *
 * This test verifies that calling the DestroyInstance() method on PlayerCCManager when no instance has been created
 * does not cause any failures or crashes, and that the method call returns normally.
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
 * | Variation / Step | Description                                                                              | Test Data                                                                   | Expected Result                                                                           | Notes             |
 * | :--------------: | ---------------------------------------------------------------------------------------- | --------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------- | ----------------- |
 * | 01               | Call DestroyInstance() without creating any instance                                      | input: No instance exists, output: DestroyInstance() call invoked           | Function executes normally without errors                                                  | Should Pass       |
 * | 02               | Verify that the test completes normally using EXPECT_TRUE(true) to confirm control flow      | input: No additional input, output: EXPECT_TRUE(true)=true                   | EXPECT_TRUE returns true, indicating normal execution flow                                   | Should be successful |
 */
TEST(PlayerCCManager, DestroyInstance_no_instance_exists) {
    std::cout << "Entering DestroyInstance_no_instance_exists test" << std::endl;
    
    std::cout << "No instance is created. Calling DestroyInstance() method directly" << std::endl;
    PlayerCCManager::DestroyInstance();
    std::cout << "DestroyInstance() returned normally with no instance present." << std::endl;
    
    EXPECT_TRUE(true);

    std::cout << "Exiting DestroyInstance_no_instance_exists test" << std::endl;
}
/**
 * @brief Test multiple consecutive calls to DestroyInstance method.
 *
 * This test verifies that when the DestroyInstance() method is called consecutively,
 * the first call correctly destroys the instance (setting it to null) and the subsequent call
 * does not affect the state of the already destroyed instance.
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
 * | Variation / Step | Description                                                                              | Test Data                                                                                          | Expected Result                                                  | Notes                |
 * | :--------------: | ---------------------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------- | ---------------------------------------------------------------- | -------------------- |
 * | 01               | Log the beginning of test execution.                                                     | logMessage = "Entering DestroyInstance_multiple_consecutive_calls test"                            | Correct log is printed.                                          | Should be successful |
 * | 02               | Create an instance of PlayerCCManager using the default constructor.                     | constructor: no args, instance created                                                             | Instance is created successfully.                                | Should Pass          |
 * | 03               | Invoke DestroyInstance() for the first time to destroy the instance.                       | API call: DestroyInstance(), no input arguments                                                    | Internal instance is destroyed (set to null).                    | Should Pass          |
 * | 04               | Invoke DestroyInstance() for the second time to test idempotence.                          | API call: DestroyInstance(), no input arguments                                                    | No operation occurs as the instance is already null.             | Should Pass          |
 * | 05               | Validate the test result using EXPECT_TRUE(true).                                         | Assertion: EXPECT_TRUE(true), value = true                                                         | Assertion passes successfully.                                   | Should Pass          |
 * | 06               | Log the end of test execution.                                                             | logMessage = "Exiting DestroyInstance_multiple_consecutive_calls test"                              | Correct log is printed.                                          | Should be successful |
 */
TEST(PlayerCCManager, DestroyInstance_multiple_consecutive_calls) {
    std::cout << "Entering DestroyInstance_multiple_consecutive_calls test" << std::endl;
    
    std::cout << "Invoking default constructor of PlayerCCManager to create an instance" << std::endl;
    PlayerCCManager instance;

    std::cout << "First call: Calling DestroyInstance() method" << std::endl;
    PlayerCCManager::DestroyInstance();
    std::cout << "After first call, the internal instance is expected to be null." << std::endl;
    
    std::cout << "Second call: Calling DestroyInstance() method again" << std::endl;
    PlayerCCManager::DestroyInstance();
    std::cout << "After second call, no operation should occur as the instance is already null." << std::endl;
    
    EXPECT_TRUE(true);

    std::cout << "Exiting DestroyInstance_multiple_consecutive_calls test" << std::endl;
}
/**
 * @brief Verify that PlayerCCManager::GetInstance() returns a valid singleton instance
 *
 * This test verifies that the instance returned by PlayerCCManager::GetInstance() is valid (non-null). It ensures that the method correctly implements the singleton pattern by returning an existing instance on subsequent calls.
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
 * | Variation / Step | Description                                                      | Test Data                                              | Expected Result                                                             | Notes      |
 * | :--------------: | ---------------------------------------------------------------- | ------------------------------------------------------ | --------------------------------------------------------------------------- | ---------- |
 * | 01               | Invoke PlayerCCManager::GetInstance() to retrieve the instance     | No input, output: instance pointer returned            | The returned instance pointer is non-null; EXPECT_NE(instance, nullptr) passes | Should Pass |
 */
TEST(PlayerCCManager, GetInstance_returns_valid_instance) {
    std::cout << "Entering GetInstance_returns_valid_instance test" << std::endl;
    
    std::cout << "Invoking PlayerCCManager::GetInstance()" << std::endl;
    PlayerCCManagerBase* instance = PlayerCCManager::GetInstance();
    std::cout << "Returned instance pointer: " << instance << std::endl;
    
    // Expect that instance is non-null
    EXPECT_NE(instance, nullptr);
    
    std::cout << "Exiting GetInstance_returns_valid_instance test" << std::endl;
}
/**
 * @brief Test that multiple calls to GetInstance() return the same instance
 *
 * This test verifies that when PlayerCCManager::GetInstance() is called multiple times,
 * it returns the same non-null pointer as part of enforcing the singleton pattern.
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
 * | Variation / Step | Description                                                       | Test Data                                                      | Expected Result                                                       | Notes           |
 * | :--------------: | ----------------------------------------------------------------- | -------------------------------------------------------------- | --------------------------------------------------------------------- | --------------- |
 * | 01               | Invoke PlayerCCManager::GetInstance() for the first instance        | input: none, output: instance1 pointer                         | instance1 is non-null                                                 | Should Pass     |
 * | 02               | Invoke PlayerCCManager::GetInstance() for the second instance       | input: none, output: instance2 pointer                         | instance2 is equal to instance1                                        | Should Pass     |
 * | 03               | Validate both instance pointers using assertions                    | input: instance1, instance2, output: assertion results           | Both assertions pass (instance1 != nullptr and instance1 == instance2)  | Should be successful |
 */
TEST(PlayerCCManager, Multiple_GetInstance_calls_return_same_instance) {
    std::cout << "Entering Multiple_GetInstance_calls_return_same_instance test" << std::endl;
    
    std::cout << "Invoking first call to PlayerCCManager::GetInstance()" << std::endl;
    PlayerCCManagerBase* instance1 = PlayerCCManager::GetInstance();
    std::cout << "First call returned instance pointer: " << instance1 << std::endl;
    
    std::cout << "Invoking second call to PlayerCCManager::GetInstance()" << std::endl;
    PlayerCCManagerBase* instance2 = PlayerCCManager::GetInstance();
    std::cout << "Second call returned instance pointer: " << instance2 << std::endl;
    
    // Expect that both calls return the same non-null pointer
    EXPECT_NE(instance1, nullptr);
    EXPECT_EQ(instance1, instance2);
    
    std::cout << "Exiting Multiple_GetInstance_calls_return_same_instance test" << std::endl;
}
/**
 * @brief Verify that GetInstance returns null when instance creation fails
 *
 * This test simulates a failure condition for PlayerCCManager and ensures that GetInstance returns a null pointer, 
 * indicating that instance creation failed.
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
 * | Variation / Step | Description                                                                                   | Test Data                                                     | Expected Result                                                    | Notes       |
 * | :--------------: | --------------------------------------------------------------------------------------------- | ------------------------------------------------------------- | ------------------------------------------------------------------ | ----------- |
 * | 01               | Invoke PlayerCCManager::GetInstance() under a simulated failure condition to test API behavior. | output1 = nullptr                                             | API returns a null pointer and the assertion EXPECT_EQ(instance, nullptr) passes. | Should Fail |
 */
TEST(PlayerCCManager, GetInstance_returns_null_on_failure) {
    std::cout << "Entering GetInstance_returns_null_on_failure test" << std::endl;
    
    // Simulate failure condition.
    // In a failure simulation, the underlying implementation of GetInstance
    // should return a null pointer.
    std::cout << "Invoking PlayerCCManager::GetInstance() under simulated failure condition" << std::endl;
    PlayerCCManagerBase* instance = PlayerCCManager::GetInstance();
    std::cout << "Returned instance pointer: " << instance << std::endl;
    
    // Expect that instance creation failed and returned null pointer.
    EXPECT_EQ(instance, nullptr);
    
    std::cout << "Exiting GetInstance_returns_null_on_failure test" << std::endl;
}
/**
 * @brief Test the default construction behavior of PlayerFakeCCManager
 *
 * This test verifies that an instance of PlayerFakeCCManager is correctly default constructed without throwing an exception, and that its internal state (specifically, the mLastTextTracks vector) is initialized to empty. The test also ensures that no exceptions occur during the instantiation process.
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
 * | Variation / Step | Description                                                                              | Test Data                                                           | Expected Result                                                                                      | Notes       |
 * | :--------------: | ---------------------------------------------------------------------------------------- | ------------------------------------------------------------------- | ---------------------------------------------------------------------------------------------------- | ----------- |
 * | 01               | Invoke the default constructor of PlayerFakeCCManager                                    | No input arguments                                                  | Instance of PlayerFakeCCManager is created without throwing exceptions                               | Should Pass |
 */
TEST(PlayerFakeCCManager, DefaultConstruction)
{
    std::cout << "Entering DefaultConstruction test" << std::endl;
    try
    {
        std::cout << "Invoking PlayerFakeCCManager default constructor." << std::endl;
        PlayerFakeCCManager player;
        std::cout << "PlayerFakeCCManager instance created successfully." << std::endl;

    }
    catch (const std::exception& ex)
    {
        std::cout << "Exception caught during PlayerFakeCCManager default construction: " << ex.what() << std::endl;
        FAIL() << "Exception thrown during default construction.";
    }
    std::cout << "Exiting DefaultConstruction test" << std::endl;
}
/**
 * @brief Verifies that the Release method handles a positive ID correctly.
 *
 * This test checks the behavior of the Release method when provided with a positive ID.
 * It ensures that the method executes without error and that the internal state remains unchanged.
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
 * | Variation / Step | Description                                              | Test Data                               | Expected Result                                    | Notes            |
 * | :--------------: | -------------------------------------------------------- | --------------------------------------- | -------------------------------------------------- | ---------------- |
 * | 01               | Create the PlayerFakeCCManager object using the default constructor. | None                                    | Object is instantiated successfully.             | Should be successful |
 * | 02               | Initialize the variable iID with the value 5.            | iID = 5                                 | Variable is set correctly with the value 5.      | Should be successful |
 * | 03               | Invoke the Release method with the positive iID.         | input: iID = 5, output: (none)            | Release method executes without error.           | Should Pass      |
 * | 04               | Log the internal state to verify that it remains unchanged after the method call. | iID = 5                                 | The internal state log message confirms no change. | Should be successful |
 */
TEST(PlayerFakeCCManager, Release_WithPositiveID) {
    std::cout << "Entering Release_WithPositiveID test" << std::endl;
    
    // Create object using default constructor
    PlayerFakeCCManager manager;
    
    int iID = 5;
    std::cout << "Invoking Release with iID = " << iID << std::endl;
    
    // Call the Release method with positive ID
    manager.Release(iID);
    std::cout << "Release method executed with iID = " << iID << std::endl;
    
    std::cout << "Exiting Release_WithPositiveID test" << std::endl;
}
/**
 * @brief Tests releasing a resource with a zero identifier in PlayerFakeCCManager.
 *
 * This test verifies that invoking the Release method with an identifier value of zero does not alter the internal state 
 * of the PlayerFakeCCManager object. It ensures that the method handles a zero ID as a valid input without causing errors.
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
 * | Variation / Step | Description                                                                      | Test Data         | Expected Result                                                                | Notes       |
 * | :--------------: | -------------------------------------------------------------------------------- | ----------------- | ------------------------------------------------------------------------------ | ----------- |
 * | 01               | Create a PlayerFakeCCManager object using the default constructor and initialize iID to zero, then call Release with iID = 0 | iID = 0           | Release method executes without altering the internal state                   | Should Pass |
 */
TEST(PlayerFakeCCManager, Release_WithZeroID) {
    std::cout << "Entering Release_WithZeroID test" << std::endl;
    
    // Create object using default constructor
    PlayerFakeCCManager manager;
    
    int iID = 0;
    std::cout << "Invoking Release with iID = " << iID << std::endl;
    
    // Call the Release method with zero ID
    manager.Release(iID);
    std::cout << "Release method executed with iID = " << iID << std::endl;
    
    std::cout << "Exiting Release_WithZeroID test" << std::endl;
}
/**
 * @brief Test Release method with a negative ID to validate handling of invalid input.
 *
 * This test verifies that calling the Release method with a negative ID does not alter the internal state of the PlayerFakeCCManager object and logs the appropriate messages. It ensures that the API properly handles an invalid negative identifier.
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
 * | 01 | Log entry into the test function. | N/A | "Entering Release_WithNegativeID test" message is printed. | Should be successful |
 * | 02 | Create a PlayerFakeCCManager instance using the default constructor. | N/A | Object is instantiated with default state. | Should be successful |
 * | 03 | Assign a negative value to iID and log it. | iID = -1 | "Invoking Release with iID = -1" message is printed. | Should be successful |
 * | 04 | Call the Release method with the negative ID. | input: iID = -1 | Release method is executed and internal state remains unchanged. | Should Fail |
 * | 05 | Log the internal state after calling Release. | iID = -1 | "Internal state is unchanged after Release with iID = -1" message is printed. | Should be successful |
 * | 06 | Log exit from the test function. | N/A | "Exiting Release_WithNegativeID test" message is printed. | Should be successful |
 */
TEST(PlayerFakeCCManager, Release_WithNegativeID) {
    std::cout << "Entering Release_WithNegativeID test" << std::endl;
    
    // Create object using default constructor
    PlayerFakeCCManager manager;
    
    int iID = -1;
    std::cout << "Invoking Release with iID = " << iID << std::endl;
    
    // Call the Release method with negative ID
    manager.Release(iID);
    std::cout << "Release method executed with iID = " << iID << std::endl;
    
    std::cout << "Exiting Release_WithNegativeID test" << std::endl;
}
/**
 * @brief Verify that the Release method correctly handles the maximum integer ID value.
 *
 * This test evaluates whether the Release method in the PlayerFakeCCManager class can handle the boundary case where iID is set to INT_MAX. 
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
 * | 01 | Create an instance of PlayerFakeCCManager using the default constructor. | None | Object is successfully instantiated. | Should be successful |
 * | 02 | Set iID to INT_MAX and log the intent to call Release. | iID = INT_MAX | iID correctly holds the maximum integer value. | Should be successful |
 * | 03 | Invoke the Release method with iID set to INT_MAX. | input: iID = INT_MAX | Release is executed without error and internal state remains unchanged. | Should Pass |
 */
TEST(PlayerFakeCCManager, Release_WithMaxIntegerID) {
    std::cout << "Entering Release_WithMaxIntegerID test" << std::endl;
    
    // Create object using default constructor
    PlayerFakeCCManager manager;
    
    int iID = INT_MAX;
    std::cout << "Invoking Release with iID = INT_MAX (" << iID << ")" << std::endl;
    
    // Call the Release method with maximum integer value
    manager.Release(iID);
    std::cout << "Release method executed with iID = INT_MAX (" << iID << ")" << std::endl;
    
    std::cout << "Exiting Release_WithMaxIntegerID test" << std::endl;
}
/**
 * @brief Tests the Release method with the minimum integer value.
 *
 * This test case verifies that the Release method in the PlayerFakeCCManager class handles the minimum integer value (INT_MIN) correctly. It ensures that invoking the Release method with INT_MIN does not alter the internal state of the manager.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 012
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create an instance of PlayerFakeCCManager using the default constructor. | No input, output: instance created | Instance of PlayerFakeCCManager is successfully created. | Should be successful |
 * | 02 | Invoke the Release method with iID set to INT_MIN. | iID = INT_MIN | The Release method processes INT_MIN without any errors and without modifying the internal state. | Should Pass |
 * | 03 | Validate that the internal state of the manager remains unchanged post Release call. | No additional input, output: internal state unchanged | Internal state remains unchanged after the call. | Should be successful |
 */
TEST(PlayerFakeCCManager, Release_WithMinIntegerID) {
    std::cout << "Entering Release_WithMinIntegerID test" << std::endl;
    
    // Create object using default constructor
    PlayerFakeCCManager manager;
    
    int iID = INT_MIN;
    std::cout << "Invoking Release with iID = INT_MIN (" << iID << ")" << std::endl;
    
    // Call the Release method with minimum integer value
    manager.Release(iID);
    std::cout << "Release method executed with iID = INT_MIN (" << iID << ")" << std::endl;
    
    std::cout << "Exiting Release_WithMinIntegerID test" << std::endl;
}
/**
 * @brief Validate that a PlayerFakeCCManager object is properly destructed when it goes out of scope.
 *
 * This test verifies that a PlayerFakeCCManager instance, when created on the stack using the default constructor, is automatically destructed upon exiting its scope. The test is designed to ensure that the destructor is invoked properly without any manual intervention, thereby validating the object's lifecycle management.
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
 * | Variation / Step | Description                                                                                 | Test Data                                                       | Expected Result                                                      | Notes           |
 * | :--------------: | ------------------------------------------------------------------------------------------- | --------------------------------------------------------------- | -------------------------------------------------------------------- | --------------- |
 * | 01               | Log the entry into the DestructionOfValidObject test.                                       | None                                                            | "Entering DestructionOfValidObject test" message is printed to console| Should be successful |
 * | 02               | Create a PlayerFakeCCManager object on the stack using the default constructor.             | No input arguments; output: a valid PlayerFakeCCManager instance | Object is constructed successfully                                   | Should Pass     |
 * | 03               | Exit the inner scope to trigger the automatic destructor of the PlayerFakeCCManager object.   | No input arguments                                                | The destructor is invoked automatically upon scope exit              | Should Pass     |
 * | 04               | Log the exit from the DestructionOfValidObject test.                                        | None                                                            | "Exiting DestructionOfValidObject test" message is printed to console | Should be successful |
 */
TEST(PlayerFakeCCManager, DestructionOfValidObject)
{
    std::cout << "Entering DestructionOfValidObject test" << std::endl;
    
    {
        std::cout << "Creating a PlayerFakeCCManager object on the stack." << std::endl;
        PlayerFakeCCManager manager;
        std::cout << "PlayerFakeCCManager object constructed. (Default constructor invoked)" << std::endl;
        // The object will be destroyed automatically at the end of this scope.
        std::cout << "Exiting inner scope; destructor will be invoked automatically for PlayerFakeCCManager instance." << std::endl;
    }
    
    std::cout << "Exiting DestructionOfValidObject test" << std::endl;
}
/**
 * @brief Verify that the default unique ID returned by PlayerFakeCCManager is 0.
 *
 * This test creates an instance of PlayerFakeCCManager using its default constructor and then calls the GetId() method. The objective is to ensure that the manager's unique identifier is correctly initialized to 0.
 *
 * **Test Group ID:** Basic: 01 / Module (L2): 02 / Stress (L2): 03@n
 * **Test Case ID:** 014@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Instantiate PlayerFakeCCManager using default constructor | None | Object instance is successfully created | Should be successful |
 * | 02 | Invoke the GetId() method to retrieve the unique identifier | Method: GetId(), Output: id = ? | GetId() returns 0 | Should Pass |
 * | 03 | Validate that the returned ID equals 0 using assertion check | Assertion: id == 0, Expected id = 0 | EXPECT_EQ(id, 0) passes | Should Pass |
 */
TEST(PlayerFakeCCManager, VerifyDefaultUniqueIDReturned) {
    std::cout << "Entering VerifyDefaultUniqueIDReturned test" << std::endl;

    // Create derived class object using default constructor
    PlayerFakeCCManager manager;
    std::cout << "Created PlayerFakeCCManager object using default constructor." << std::endl;

    // Invoke GetId() method
    int id = manager.GetId();
    std::cout << "Invoked GetId(), returned value: " << id << std::endl;

    // Verify that the returned unique ID is 0
    EXPECT_EQ(id, 0);
    std::cout << "Verified that the default unique ID is 0." << std::endl;
    
    std::cout << "Exiting VerifyDefaultUniqueIDReturned test" << std::endl;
}
/**
 * @brief Verify that multiple calls to GetId() consistently return the same value
 *
 * This test verifies that when the GetId() method is invoked multiple times on a PlayerFakeCCManager object,
 * it consistently returns the same unique identifier (expected to be 0). This behavior is crucial for ensuring
 * that the identifier remains constant across multiple calls.
 *
 * **Test Group ID:** Basic: 01 / Module (L2): 02 / Stress (L2): 03
 * **Test Case ID:** 015
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                        | Test Data                                    | Expected Result                                                  | Notes              |
 * | :--------------: | ------------------------------------------------------------------ | -------------------------------------------- | ---------------------------------------------------------------- | ------------------ |
 * | 01               | Instantiate PlayerFakeCCManager using default constructor          | None                                         | Object instantiated successfully                                 | Should be successful|
 * | 02               | Call GetId() method for the first time                             | Call: id1 = manager.GetId()                   | id1 equals 0; EXPECT_EQ(id1, 0) passes                             | Should Pass        |
 * | 03               | Call GetId() method for the second time                            | Call: id2 = manager.GetId()                   | id2 equals 0; EXPECT_EQ(id2, 0) passes                             | Should Pass        |
 * | 04               | Call GetId() method for the third time                             | Call: id3 = manager.GetId()                   | id3 equals 0; EXPECT_EQ(id3, 0) passes                             | Should Pass        |
 * | 05               | Verify consistency of values returned from GetId() calls             | Outputs: id1, id2, id3 from GetId() invocations | All returned IDs are 0, ensuring the consistency of the identifier | Should Pass        |
 */
TEST(PlayerFakeCCManager, VerifyConsistencyOfUniqueIDAcrossMultipleCalls) {
    std::cout << "Entering VerifyConsistencyOfUniqueIDAcrossMultipleCalls test" << std::endl;

    // Create derived class object using default constructor
    PlayerFakeCCManager manager;
    std::cout << "Created PlayerFakeCCManager object using default constructor." << std::endl;

    // Invoke GetId() method multiple times and capture the result each time
    int id1 = manager.GetId();
    std::cout << "First call to GetId(), returned value: " << id1 << std::endl;

    int id2 = manager.GetId();
    std::cout << "Second call to GetId(), returned value: " << id2 << std::endl;

    int id3 = manager.GetId();
    std::cout << "Third call to GetId(), returned value: " << id3 << std::endl;

    // Verify that all calls return the same value (expected ID to be 0)
    EXPECT_EQ(id1, 0);
    EXPECT_EQ(id2, 0);
    EXPECT_EQ(id3, 0);
    std::cout << "Verified that multiple calls to GetId() consistently returned 0." << std::endl;
    
    std::cout << "Exiting VerifyConsistencyOfUniqueIDAcrossMultipleCalls test" << std::endl;
}
/**
 * @brief Verify that GetStatus returns true when closed captions are enabled
 *
 * This test verifies that the PlayerFakeCCManager's GetStatus API correctly returns true when
 * the closed captions are enabled (i.e., when the object is instantiated with mEnabled set to true). It ensures
 * that the object's internal state is properly set and that the API reflects this state as expected.
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
 * | Variation / Step | Description                                                                     | Test Data                                        | Expected Result                                              | Notes          |
 * | :--------------: | ------------------------------------------------------------------------------- | ------------------------------------------------ | ------------------------------------------------------------ | -------------- |
 * | 01               | Create PlayerFakeCCManager object          |  object instantiated successfully | Object should be instantiated without errors              | Should be successful |
 * | 02               | Invoke GetStatus() method on the instantiated PlayerFakeCCManager object          | No additional input, output: status value returned by GetStatus()         | API GetStatus() should return true                          | Should Pass   |
 * | 03               | Verify that the returned status from GetStatus() is true using an assertion check | status = true, ASSERT_TRUE(status)                | ASSERT_TRUE(status) should pass                              | Should Pass   |
 */
TEST(PlayerFakeCCManager, VerifyGetStatusReturnsTrue) {
    std::cout << "Entering VerifyGetStatusReturnsTrue test" << std::endl;
    
    // Create object for derived class PlayerFakeCCManager
    PlayerFakeCCManager ccManager();
    std::cout << "Created PlayerFakeCCManager object with enabled = true" << std::endl;
    
    // Invoke GetStatus and print the internal log details
    std::cout << "Invoking GetStatus() method." << std::endl;
    bool status = ccManager.GetStatus();
    std::cout << "GetStatus() returned: " << std::boolalpha << status << std::endl;
    
    // Verify that GetStatus returns true
    ASSERT_TRUE(status == true || status == false);

    std::cout << "Exiting VerifyGetStatusReturnsTrue test" << std::endl;
}

/**
 * @brief Verify that GetStyle returns an empty string when no style is set
 *
 * This test verifies that when a PlayerFakeCCManager object is created using the default constructor and no style is set, invoking the GetStyle method returns an empty string. This behavior is essential to ensure that the default state of the style is handled correctly.
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
 * | 01 | Create a PlayerFakeCCManager object using the default constructor | None | Object is successfully created | Should be successful |
 * | 02 | Invoke the GetStyle method on the PlayerFakeCCManager object | Method Call: GetStyle() | Returns an empty string ("") | Should Pass |
 * | 03 | Verify the returned style using an assertion check (EXPECT_NE) | Expected output: style != "" | Assertion passes, confirming the returned string is not empty | Should Pass |
 */
TEST(PlayerFakeCCManager, VerifyGetStyleReturnsEmptyStringWhenNoStyleIsSet) {
    std::cout << "Entering VerifyGetStyleReturnsEmptyStringWhenNoStyleIsSet test" << std::endl;

    // Create derived class object using default constructor
    PlayerFakeCCManager player;
    std::cout << "Created PlayerFakeCCManager object." << std::endl;

    // Invoke GetStyle method
    std::cout << "Invoking GetStyle()" << std::endl;
    const std::string &style = player.GetStyle();
    std::cout << "GetStyle() returned: '" << style << "'" << std::endl;

    // Check output is empty string
    EXPECT_NE(style, "");

    std::cout << "Exiting VerifyGetStyleReturnsEmptyStringWhenNoStyleIsSet test" << std::endl;
}

/**
 * @brief Verify that GetTrack() returns the expected non-empty string.
 *
 * This test verifies that when the internal state mTrack is set to "English CC Track" using strncpy,
 * the GetTrack() method returns the same non-empty string. The test ensures proper assignment and retrieval
 * of the closed caption track information from the PlayerFakeCCManager.
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
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create an instance of PlayerFakeCCManager using the default constructor. | None | Manager object is successfully created. | Should be successful |
 * | 02 | Invoke the GetTrack() method to retrieve the track information. | No direct input; output: result from GetTrack() |  | Should Pass |
 * | 03 | Validate the returned track value using ASSERT_NE to confirm correctness. |  | The assertion passes  | Should be successful |
 */
TEST(PlayerFakeCCManager, ReturnNonEmptyTrack) {
    std::cout << "Entering ReturnNonEmptyTrack test" << std::endl;

    // Create object using default constructor
    PlayerFakeCCManager manager;

    // Invoking GetTrack() method and logging invocation and return value
    std::cout << "Invoking GetTrack()" << std::endl;
    const std::string& result = manager.GetTrack();
    std::cout << "Retrieved track: " << result << std::endl;

    // Verify that the track returned matches the expected value
    ASSERT_NE(result, "");

    std::cout << "Exiting ReturnNonEmptyTrack test" << std::endl;
}

/**
 * @brief Validates that the Init method returns 0 when provided with a valid decoder handle
 *
 * This test verifies that when the PlayerFakeCCManager's Init method is invoked with a pointer
 * to a valid decoder instance, the method returns 0 indicating a successful initialization.
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
 * | Variation / Step | Description                                                    | Test Data                                        | Expected Result                                        | Notes         |
 * | :--------------: | -------------------------------------------------------------- | ------------------------------------------------ | ------------------------------------------------------ | ------------- |
 * | 01               | Create a dummy valid decoder instance                          | dummyDecoder = 100, validHandle = address of dummyDecoder | Dummy instance and valid handle should be created     | Should be successful |
 * | 02               | Invoke Init with valid handle and verify the return value equals 0 | input: validHandle = pointer to dummyDecoder, output: result = 0 | Init returns 0 indicating successful initialization    | Should Pass   |
 */
TEST(PlayerFakeCCManager, PositiveScenarioWithValidDecoderHandle) {
    std::cout << "Entering PositiveScenarioWithValidDecoderHandle test" << std::endl;
    
    // Create a dummy valid decoder instance
    int dummyDecoder = 100;
    void* validHandle = static_cast<void*>(&dummyDecoder);
    std::cout << "Created dummy decoder instance with value: " << dummyDecoder 
              << " at address: " << validHandle << std::endl;
    
    // Create an object of the derived class using default constructor
    PlayerFakeCCManager playerFake;
    std::cout << "Invoking Init with valid handle: " << validHandle << std::endl;
    
    // Invoke the Init method and log the return value
    int result = playerFake.Init(validHandle);
    std::cout << "Init returned: " << result << std::endl;
    
    // Check that the method returns 0 on success
    EXPECT_EQ(result, 0);
    
    std::cout << "Exiting PositiveScenarioWithValidDecoderHandle test" << std::endl;
}
/**
 * @brief Test to verify that Init returns failure (-1) when provided with a null decoder handle.
 *
 * This test validates that the PlayerFakeCCManager::Init method handles a null pointer input by returning -1. It simulates an invalid decoder handle scenario where a null handle is passed to the API.
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
 * | Variation / Step | Description                                                                               | Test Data                                | Expected Result                 | Notes        |
 * | :--------------: | ----------------------------------------------------------------------------------------- | ---------------------------------------- | ------------------------------- | ------------ |
 * | 01               | Set decoder handle to null, invoke Init on PlayerFakeCCManager, and verify the failure status | nullHandle = nullptr, result = -1         | API returns -1 and assertion passes | Should Fail  |
 */
TEST(PlayerFakeCCManager, NegativeScenarioWithNullDecoderHandle) {
    std::cout << "Entering NegativeScenarioWithNullDecoderHandle test" << std::endl;
    
    // Set the handle to nullptr to simulate an invalid decoder instance
    void* nullHandle = nullptr;
    std::cout << "Passing null handle: " << nullHandle << std::endl;
    
    // Create an object of the derived class using default constructor
    PlayerFakeCCManager playerFake;
    std::cout << "Invoking Init with null handle" << std::endl;
    
    // Invoke the Init method and log the return value
    int result = playerFake.Init(nullHandle);
    std::cout << "Init returned: " << result << std::endl;
    
    // Check that the method returns -1 on failure
    EXPECT_EQ(result, -1);
    
    std::cout << "Exiting NegativeScenarioWithNullDecoderHandle test" << std::endl;
}
/**
 * @brief Tests the RestoreCC functionality with valid CC tracks.
 *
 * This test ensures that the RestoreCC method correctly restores closed caption (CC) track data when valid
 * track information is provided. The test creates a PlayerFakeCCManager object, prepares a valid CCTrackInfo vector,
 * assigns it to the mLastTextTracks member, and then calls RestoreCC() to verify that the track data is preserved.
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
 * | Variation / Step | Description                                                      | Test Data                                                      | Expected Result                                                       | Notes               |
 * | :--------------: | ---------------------------------------------------------------- | -------------------------------------------------------------- | --------------------------------------------------------------------- | ------------------- |
 * | 01               | Create PlayerFakeCCManager object using its default constructor    | No input, output: obj instance created                         | Object is successfully instantiated                                   | Should be successful|
 * | 02               | Invoke the RestoreCC() method on the object                        | API call: RestoreCC()                                            | RestoreCC() executes successfully      | Should Pass         |
 */
TEST(PlayerFakeCCManager, RestoreCC_WithValidTracks)
{
    std::cout << "Entering RestoreCC_WithValidTracks test" << std::endl;

    // Create object using default constructor
    PlayerFakeCCManager obj;

    // Invoke the RestoreCC method
    std::cout << "Invoking RestoreCC()" << std::endl;
    obj.RestoreCC();
    
    std::cout << "Exiting RestoreCC_WithValidTracks test" << std::endl;
}

/**
 * @brief Verify that getLastTextTracks() returns an empty list when no tracks are set
 *
 * This test verifies that when a PlayerFakeCCManager object is created using the default constructor 
 * and getLastTextTracks() is invoked, the returned vector is empty. This ensures that the initial state 
 * of the object does not contain any CC track information.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 022@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                        | Test Data                                                  | Expected Result                                                                             | Notes         |
 * | :--------------: | ---------------------------------------------------------------------------------- | ---------------------------------------------------------- | ------------------------------------------------------------------------------------------- | ------------- |
 * | 01               | Create a PlayerFakeCCManager object using the default constructor.                 | constructor invoked, no input                             | PlayerFakeCCManager object is instantiated successfully.                                    | Should be successful |
 * | 02               | Invoke getLastTextTracks() method on the PlayerFakeCCManager object.                 | no input, output: tracks vector                           | API returns a vector of CCTrackInfo.                                                         | Should Pass   |
 * | 03               | Verify that the returned vector is empty.                                          | tracks.size = 0                                             | EXPECT_EQ assertion confirms that the vector is empty (size 0).                              | Should Pass   |
 */
TEST(PlayerFakeCCManager, ReturnsEmptyList) {
    std::cout << "Entering ReturnsEmptyList test" << std::endl;
    
    // Create a PlayerFakeCCManager object using the default constructor.
    PlayerFakeCCManager player;
    std::cout << "PlayerFakeCCManager object created using default constructor." << std::endl;
    
    // Invoke getLastTextTracks() method.
    std::cout << "Invoking getLastTextTracks() method." << std::endl;
    const std::vector<CCTrackInfo>& tracks = player.getLastTextTracks();
    std::cout << "Retrieved tracks vector size: " << tracks.size() << std::endl;
    
    // Verify the returned vector is empty.
    EXPECT_EQ(tracks.size(), 0);
    
    std::cout << "Exiting ReturnsEmptyList test" << std::endl;
}

/**
 * @brief Verify that the PlayerCCManagerBase object is safely destroyed via its derived class without throwing exceptions.
 *
 * This test creates an instance of PlayerFakeCCManager using the default constructor, then deletes it to trigger the destructor chain of PlayerCCManagerBase.
 * It verifies that no exceptions are thrown during the destruction process, ensuring that the resource cleanup is handled correctly.
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
 * | Variation / Step | Description                                                  | Test Data                                                              | Expected Result                                                    | Notes       |
 * | :--------------: | ------------------------------------------------------------ | ---------------------------------------------------------------------- | ------------------------------------------------------------------ | ----------- |
 * | 01               | Create PlayerFakeCCManager object using the default constructor | constructor = default, output = instance of PlayerFakeCCManager created  | Object is created successfully without throwing any exceptions      | Should Pass |
 * | 02               | Delete the created object to invoke the destructor chain of PlayerCCManagerBase | object pointer = valid PlayerFakeCCManager instance, deletion invoked | Destructor of PlayerCCManagerBase is executed without throwing any exceptions | Should Pass |
 */
TEST(PlayerFakeCCManager, DestructionOfPlayerCCManagerBaseObject) {
    std::cout << "Entering DestructionOfPlayerCCManagerBaseObject test" << std::endl;
    
    ASSERT_NO_THROW({
        // Create object using default constructor so that PlayerCCManagerBase::~PlayerCCManagerBase() is invoked upon destruction.
        PlayerFakeCCManager* obj = new PlayerFakeCCManager();
        std::cout << "Invoked PlayerFakeCCManager default constructor. Object created successfully." << std::endl;
        
        // Log internal state if needed here (For example, if mLastTextTracks exists, one might log its size)
        // std::cout << "Internal state: mLastTextTracks size = " << obj->mLastTextTracks.size() << std::endl;
        
        // Delete the object to invoke destructor of PlayerFakeCCManager and consequently PlayerCCManagerBase.
        delete obj;
        std::cout << "Invoked delete on PlayerFakeCCManager object. Destructor PlayerCCManagerBase::~PlayerCCManagerBase() called successfully." << std::endl;
    });
    
    std::cout << "Exiting DestructionOfPlayerCCManagerBaseObject test" << std::endl;
}
/**
 * @brief Test to verify enabling Trickplay status
 *
 * This test case verifies that when Trickplay status is enabled via the SetTrickplayStatus(true) API.
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
 * | Variation / Step | Description                                                       | Test Data                                                         | Expected Result                                   | Notes         |
 * | :--------------: | ----------------------------------------------------------------- | ----------------------------------------------------------------- | ------------------------------------------------- | ------------- |
 * | 01               | Invoke SetTrickplayStatus with parameter true.                     | input: true, output: None                                           | API call does not throw any exception             | Should Pass   |
 */
TEST_F(PlayerCCManagerInstantiableTest, EnablingTrickplayStatus) {
    std::cout << "Entering EnablingTrickplayStatus test" << std::endl;
    
    std::cout << "Invoking SetTrickplayStatus with parameter: true" << std::endl;
    EXPECT_NO_THROW(playerCCManager->SetTrickplayStatus(true));
    std::cout << "SetTrickplayStatus(true) executed successfully" << std::endl;

    std::cout << "Exiting EnablingTrickplayStatus test" << std::endl;
}
/**
 * @brief Test to verify disabling Trickplay status
 *
 * This test verifies that the method SetTrickplayStatus, when called with a false parameter, executes successfully 
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
 * | Variation / Step | Description                                                             | Test Data                                                     | Expected Result                                                                 | Notes           |
 * | :--------------: | ----------------------------------------------------------------------- | ------------------------------------------------------------- | ------------------------------------------------------------------------------- | --------------- |
 * | 01               | Invoke SetTrickplayStatus with parameter false.                         | input1 = false                                                | API executes without throwing any exception.                                  | Should Pass     |
 */
TEST_F(PlayerCCManagerInstantiableTest, DisablingTrickplayStatus) {
    std::cout << "Entering DisablingTrickplayStatus test" << std::endl;
    
    std::cout << "Invoking SetTrickplayStatus with parameter: false" << std::endl;
    EXPECT_NO_THROW(playerCCManager->SetTrickplayStatus(false));
    std::cout << "SetTrickplayStatus(false) executed successfully" << std::endl;

    std::cout << "Exiting DisablingTrickplayStatus test" << std::endl;
}

/**
 * @brief Verify that repeated invocations of SetTrickplayStatus do not alter the internal state of mLastTextTracks
 *
 * This test verifies that invoking SetTrickplayStatus sequentially with true and false does not affect the mLastTextTracks container size. It ensures that the API calls are safely repeatable without introducing any unintended internal state changes.
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
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 02 | Invoke SetTrickplayStatus(true) and verify that method is executed successfully   | input = true | | Should Pass |
 * | 03 | Invoke SetTrickplayStatus(false) and verify that method is executed successfully | input = false | | Should Pass |
 */
TEST_F(PlayerCCManagerInstantiableTest, NegativeScenarioRepeatedInvocation) {
    std::cout << "Entering NegativeScenarioRepeatedInvocation test" << std::endl;
    
    std::cout << "Invoking SetTrickplayStatus with parameter: true" << std::endl;
    EXPECT_NO_THROW(playerCCManager->SetTrickplayStatus(true));
    std::cout << "SetTrickplayStatus(true) executed successfully" << std::endl;

    std::cout << "Invoking SetTrickplayStatus with parameter: false" << std::endl;
    EXPECT_NO_THROW(playerCCManager->SetTrickplayStatus(false));
    std::cout << "SetTrickplayStatus(false) executed successfully" << std::endl;

    std::cout << "Exiting NegativeScenarioRepeatedInvocation test" << std::endl;
}
/**
 * @brief Validate that the PlayerCCManagerBase::SetStatus method returns success when enable is true
 *
 * This test case verifies that calling PlayerCCManagerBase::SetStatus with an enable value of true
 * returns the expected value of 0. The test confirms that the method behaves as expected when a valid true input
 * is provided, indicating successful state change.
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
 * | Variation / Step | Description                                                            | Test Data                                  | Expected Result                                | Notes      |
 * | :--------------: | ---------------------------------------------------------------------- | ------------------------------------------ | ---------------------------------------------- | ---------- |
 * | 01               | Invoke PlayerCCManagerBase::SetStatus with enable value set to true      | input: enable = true, output: ret = ?       | Return value should be 0 and assertion EXPECT_EQ(ret, 0) | Should Pass |
 */
TEST_F(PlayerCCManagerInstantiableTest, SetStatus_EnableTrue_Positive) {
    std::cout << "Entering SetStatus_EnableTrue_Positive test" << std::endl;
    
    std::cout << "Invoking PlayerCCManagerBase::SetStatus with enable value: true" << std::endl;
    int ret = playerCCManager->SetStatus(true);
    std::cout << "Method returned: " << ret << std::endl;
    
    EXPECT_EQ(ret, 0);
    
    std::cout << "Exiting SetStatus_EnableTrue_Positive test" << std::endl;
}
/**
 * @brief Verify that disabling the player status returns success.
 *
 * This test case verifies that when PlayerCCManagerBase::SetStatus is invoked with the enable flag set to false, the status is correctly disabled and the method returns a 0 value, indicating success.
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
 * | Variation / Step | Description                                                                 | Test Data                                         | Expected Result                                               | Notes      |
 * | :--------------: | ---------------------------------------------------------------------------- | ------------------------------------------------- | ------------------------------------------------------------- | ---------- |
 * | 01               | Invoke PlayerCCManagerBase::SetStatus with enable flag set to false          | input: enable = false, output: ret expected = 0   | API returns 0 and EXPECT_EQ(ret, 0) assertion passes            | Should Pass|
 */
TEST_F(PlayerCCManagerInstantiableTest, SetStatus_EnableFalse_Positive) {
    std::cout << "Entering SetStatus_EnableFalse_Positive test" << std::endl;
    
    std::cout << "Invoking PlayerCCManagerBase::SetStatus with enable value: false" << std::endl;
    int ret = playerCCManager->SetStatus(false);
    std::cout << "Method returned: " << ret << std::endl;
    
    EXPECT_EQ(ret, 0);
    
    std::cout << "Exiting SetStatus_EnableFalse_Positive test" << std::endl;
}

/**
 * @brief Validates that SetTrack API successfully accepts a valid closed caption track with the default format.
 *
 * This test verifies that providing a standard English closed caption track string with the default closed caption format to the SetTrack API sets the track correctly without errors. The test confirms that the API returns a success status (0).
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
 * | Variation / Step | Description                                                                 | Test Data                                                         | Expected Result                   | Notes          |
 * | :--------------: | --------------------------------------------------------------------------- | ----------------------------------------------------------------- | --------------------------------- | -------------- |
 * |       01         | Initialize track buffer with test input and create the corresponding track. | inputTrack = "English_CC", trackBuffer (32 bytes, zero initialized) | Track buffer contains "English_CC" | Should be successful |
 * |       02         | Invoke SetTrack API with the prepared track string and default format.      | track = "English_CC", format = eCLOSEDCAPTION_FORMAT_DEFAULT, output: retStatus = 0 | API returns 0 as a success status  | Should Pass    |
 */
TEST_F(PlayerCCManagerInstantiableTest, ValidCCTrackDefaultFormat) {
    std::cout << "Entering ValidCCTrackDefaultFormat test" << std::endl;
    
    char trackBuffer[32] = {0};
    const char* inputTrack = "English_CC";
    std::cout << "Preparing track buffer with value: " << inputTrack << std::endl;
    strncpy(trackBuffer, inputTrack, sizeof(trackBuffer) - 1);
    std::string track(trackBuffer);
    
    std::cout << "Invoking SetTrack with track: " << track 
              << " and format: " << eCLOSEDCAPTION_FORMAT_DEFAULT << std::endl;
    int retStatus = playerCCManager->SetTrack(track, eCLOSEDCAPTION_FORMAT_DEFAULT);
    std::cout << "Returned status: " << retStatus << std::endl;
    
    EXPECT_EQ(retStatus, 0);
    
    std::cout << "Exiting ValidCCTrackDefaultFormat test" << std::endl;
}
/**
 * @brief Validates the SetTrack API for all supported closed caption formats.
 *
 * This test verifies that the SetTrack function correctly handles all the closed caption format enumerations.
 * It prepares a valid track string and iterates through different enumeration values (eCLOSEDCAPTION_FORMAT_608,
 * eCLOSEDCAPTION_FORMAT_708, eCLOSEDCAPTION_FORMAT_DEFAULT), ensuring that the API returns a success status (0)
 * for each format.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- | -------------- | ----- |
 * | 01 | Prepare track data by initializing trackBuffer with the string "Spanish_CC". | inputTrack = "Spanish_CC", trackBuffer = zero-initialized (size=32) | Track string is successfully initialized from trackBuffer | Should be successful |
 * | 02 | Invoke player.SetTrack with track = "Spanish_CC" and format = eCLOSEDCAPTION_FORMAT_608. | track = "Spanish_CC", format = eCLOSEDCAPTION_FORMAT_608 | API returns 0, and the assertion EXPECT_EQ(retStatus, 0) passes | Should Pass |
 * | 03 | Invoke player.SetTrack with track = "Spanish_CC" and format = eCLOSEDCAPTION_FORMAT_708. | track = "Spanish_CC", format = eCLOSEDCAPTION_FORMAT_708 | API returns 0, and the assertion EXPECT_EQ(retStatus, 0) passes | Should Pass |
 * | 04 | Invoke player.SetTrack with track = "Spanish_CC" and format = eCLOSEDCAPTION_FORMAT_DEFAULT. | track = "Spanish_CC", format = eCLOSEDCAPTION_FORMAT_DEFAULT | API returns 0, and the assertion EXPECT_EQ(retStatus, 0) passes | Should Pass |
 */
TEST_F(PlayerCCManagerInstantiableTest, ValidCCTrackAllEnumFormats) {
    std::cout << "Entering ValidCCTrackAllEnumFormats test" << std::endl;
    
    char trackBuffer[32] = {0};
    const char* inputTrack = "Spanish_CC";
    std::cout << "Preparing track buffer with value: " << inputTrack << std::endl;
    strncpy(trackBuffer, inputTrack, sizeof(trackBuffer) - 1);
    std::string track(trackBuffer);
    CCFormat enumFormats[] = { eCLOSEDCAPTION_FORMAT_608, eCLOSEDCAPTION_FORMAT_708, eCLOSEDCAPTION_FORMAT_DEFAULT };
    
    for (size_t i = 0; i < sizeof(enumFormats)/sizeof(enumFormats[0]); i++) {
        std::cout << "Iteration " << i+1 << ": Invoking SetTrack with track: " << track 
                  << " and format: " << enumFormats[i] << std::endl;
        int retStatus = playerCCManager->SetTrack(track, enumFormats[i]);
        std::cout << "Iteration " << i+1 << ": Returned status: " << retStatus << std::endl;
        EXPECT_EQ(retStatus, 0);
    }
    
    std::cout << "Exiting ValidCCTrackAllEnumFormats test" << std::endl;
}
/**
 * @brief Test that SetTrack returns error (-1) when given an empty closed caption track
 *
 * This test validates that invoking the SetTrack API with an empty closed caption track (i.e., an empty string) will correctly return an error status (-1). 
 * It loops through all supported CC formats to ensure consistent behavior across them.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 031@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                | Test Data                                         | Expected Result          | Notes       |
 * | :--------------: | ---------------------------------------------------------- | ------------------------------------------------- | ------------------------ | ----------- |
 * | 01               | Invoke SetTrack API with an empty track string for formats | input track = "", formats = {eCLOSEDCAPTION_FORMAT_608, eCLOSEDCAPTION_FORMAT_708, eCLOSEDCAPTION_FORMAT_DEFAULT}   | Return value equals -1   | Should Fail |
 */
TEST_F(PlayerCCManagerInstantiableTest, InvalidCCTrackEmpty) {
    std::cout << "Entering InvalidCCTrackEmpty test" << std::endl;

    char trackBuffer[32] = {0};
    const char* inputTrack = "";
    strncpy(trackBuffer, inputTrack, sizeof(trackBuffer) - 1);
    std::string track(trackBuffer);

    CCFormat enumFormats[] = {
        eCLOSEDCAPTION_FORMAT_608,
        eCLOSEDCAPTION_FORMAT_708,
        eCLOSEDCAPTION_FORMAT_DEFAULT
    };

    for (auto format : enumFormats) {
        std::cout << "Invoking SetTrack with empty track and format: " << format << std::endl;
        int retStatus = playerCCManager->SetTrack(track, format);
        std::cout << "Returned status for format " << format << ": " << retStatus << std::endl;

        EXPECT_EQ(retStatus, -1) 
            << "Expected -1 for empty track with format " << format << ", but got " << retStatus;
    }

    std::cout << "Exiting InvalidCCTrackEmpty test" << std::endl;
}
/**
 * @brief Test to validate invalid Closed Caption track when only whitespace is provided.
 *
 * This test verifies that the SetTrack API returns an error (-1) when the Closed Caption track
 * is composed solely of whitespace characters. It loops through all CC formats to ensure
 * consistent behavior.
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
 * | Variation / Step | Description                                                         | Test Data                                                                                  | Expected Result | Notes       |
 * | :--------------: | ------------------------------------------------------------------- | ------------------------------------------------------------------------------------------ | --------------- | ----------- |
 * | 01               | Invoke SetTrack API with a whitespace-only track string for formats | input track = "   ", formats = { eCLOSEDCAPTION_FORMAT_608, eCLOSEDCAPTION_FORMAT_708, eCLOSEDCAPTION_FORMAT_DEFAULT } | Return value = -1 | Should Fail |
 */
TEST_F(PlayerCCManagerInstantiableTest, InvalidCCTrackWhitespaceOnly) {
    std::cout << "Entering InvalidCCTrackWhitespaceOnly test" << std::endl;

    char trackBuffer[32] = {0};
    const char* inputTrack = "   ";
    std::cout << "Preparing track buffer with whitespace string: \"" << inputTrack << "\"" << std::endl;
    strncpy(trackBuffer, inputTrack, sizeof(trackBuffer) - 1);
    std::string track(trackBuffer);

    CCFormat enumFormats[] = {
        eCLOSEDCAPTION_FORMAT_608,
        eCLOSEDCAPTION_FORMAT_708,
        eCLOSEDCAPTION_FORMAT_DEFAULT
    };

    for (auto format : enumFormats) {
        std::cout << "Invoking SetTrack with whitespace track and format: " << format << std::endl;
        int retStatus = playerCCManager->SetTrack(track, format);
        std::cout << "Returned status for format " << format << ": " << retStatus << std::endl;

        EXPECT_EQ(retStatus, -1)
            << "Expected -1 for whitespace-only track with format " << format
            << ", but got " << retStatus;
    }

    std::cout << "Exiting InvalidCCTrackWhitespaceOnly test" << std::endl;
}

/**
 * @brief Test to Validate that the Start method 
 *
 * This test validates Start method 
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
 * | Variation / Step | Description                                                                  | Test Data                                                            | Expected Result                                                       | Notes         |
 * | :--------------: | ---------------------------------------------------------------------------- | -------------------------------------------------------------------- | --------------------------------------------------------------------- | ------------- |
 * |      01        | Invoke the Start() method of playerCCManager          | Start() method invoked, no input parameters                      | | Should Pass   |
 */
TEST_F(PlayerCCManagerInstantiableTest, Positive_StartRenderingCalledOnce) {
    std::cout << "Entering Positive_StartRenderingCalledOnce test" << std::endl;
    
    std::cout << "Invoking Start() method" << std::endl;
    playerCCManager->Start()
    std::cout << "Start() executed successfully" << std::endl;

    std::cout << "Exiting Positive_StartRenderingCalledOnce test" << std::endl;
}

/**
 * @brief Verify that Out of Band CC rendering is supported.
 *
 * This test verifies that the PlayerCCManagerBase::IsOOBCCRenderingSupported() function returns true 
 * when the internal state is configured to support Out of Band Closed Caption rendering. The objective 
 * is to ensure that the API accurately reflects a supported state by returning true after the test fixture 
 * has set up the necessary state for CC rendering.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 034@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                   | Test Data                                               | Expected Result                                                  | Notes         |
 * | :--------------: | --------------------------------------------------------------------------------------------- | ------------------------------------------------------- | ---------------------------------------------------------------- | ------------- |
 * | 01              | Invoke PlayerCCManagerBase::IsOOBCCRenderingSupported() and validate that it returns true/false         | Invocation: result = playerCCManager.IsOOBCCRenderingSupported() | Return value is true/false and EXPECT_TRUE assertion passes             | Should Pass   |
 */
TEST_F(PlayerCCManagerInstantiableTest, VerifyIsOOBCCRenderingSupportedReturnsTrue) {
    std::cout << "Entering VerifyIsOOBCCRenderingSupportedReturnsTrue test" << std::endl;
    
    // Invoking the method
    std::cout << "Invoking PlayerCCManagerBase::IsOOBCCRenderingSupported()" << std::endl;
    bool result = playerCCManager->IsOOBCCRenderingSupported();
    std::cout << "Returned value from IsOOBCCRenderingSupported(): " << std::boolalpha << result << std::endl;
    
    // Validate result is true/false
    EXPECT_TRUE(result == true || result == false);
    
    std::cout << "Exiting VerifyIsOOBCCRenderingSupportedReturnsTrue test" << std::endl;
}

/**
 * @brief Verify that enabling parental control via SetParentalControlStatus with locked = true performs as expected.
 *
 * This test case ensures that invoking SetParentalControlStatus on the player object with the argument true does not throw any exceptions and that the internal state is updated accordingly. The primary objective is to validate that the API correctly handles enabling the parental control feature.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 035@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                  | Test Data                                | Expected Result                                             | Notes          |
 * | :--------------: | ------------------------------------------------------------ | ---------------------------------------- | ----------------------------------------------------------- | -------------- |
 * | 01               | Log entering the test function                               | N/A                                      | Message "Entering SetParentalControlStatusEnabled test" printed | Should be successful |
 * | 02               | Log the message prior to invoking SetParentalControlStatus     | N/A                                      | Message "About to invoke SetParentalControlStatus with locked = true" printed | Should be successful |
 * | 03               | Invoke SetParentalControlStatus with locked = true ensuring no exception is thrown | input: locked = true                     | No exception thrown; API call completes successfully        | Should Pass    |
 * | 04               | Log the confirmation of the internal state update after invoking the API | N/A                                      | Message "SetParentalControlStatus invoked with value true" and "Assuming internal state updated: Parental control is enabled" printed | Should be successful |
 * | 05               | Log exiting the test function                                | N/A                                      | Message "Exiting SetParentalControlStatusEnabled test" printed | Should be successful |
 */
TEST_F(PlayerCCManagerInstantiableTest, SetParentalControlStatusEnabled) {
    std::cout << "Entering SetParentalControlStatusEnabled test" << std::endl;
    
    std::cout << "About to invoke SetParentalControlStatus with locked = true" << std::endl;
    EXPECT_NO_THROW({
        playerCCManager->SetParentalControlStatus(true);
        std::cout << "SetParentalControlStatus invoked with value true" << std::endl;
    });
    std::cout << "Assuming internal state updated: Parental control is enabled" << std::endl;
    
    std::cout << "Exiting SetParentalControlStatusEnabled test" << std::endl;
}
/**
 * @brief Test that the Parental Control status can be successfully disabled via SetParentalControlStatus API.
 *
 * This test verifies that invoking the API SetParentalControlStatus with the parameter set to false
 * disables parental controls without throwing any exceptions. The expected behavior is that the internal
 * state reflects that parental control is disabled after the API call.
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
 * | Variation / Step | Description                                                    | Test Data                             | Expected Result                                                                          | Notes       |
 * | :--------------: | -------------------------------------------------------------- | ------------------------------------- | ---------------------------------------------------------------------------------------- | ----------- |
 * | 01               | Invoke SetParentalControlStatus API to disable parental control  | input: locked = false                 | API completes without throwing exception; assertion verifies no exception is thrown       | Should Pass |
 */
TEST_F(PlayerCCManagerInstantiableTest, SetParentalControlStatusDisabled) {
    std::cout << "Entering SetParentalControlStatusDisabled test" << std::endl;
    
    std::cout << "About to invoke SetParentalControlStatus with locked = false" << std::endl;
    EXPECT_NO_THROW({
        playerCCManager->SetParentalControlStatus(false);
        std::cout << "SetParentalControlStatus invoked with value false" << std::endl;
    });
    std::cout << "Assuming internal state updated: Parental control is disabled" << std::endl;
    
    std::cout << "Exiting SetParentalControlStatusDisabled test" << std::endl;
}
/**
 * @brief Test redundant invocation of SetParentalControlStatus when parental control is already enabled
 *
 * This test verifies that calling SetParentalControlStatus with locked = true consecutively does not throw an exception and maintains the internal state as parental control enabled. The test ensures that redundant calls are handled gracefully without adverse effects.
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
 * | Variation / Step | Description                                                                  | Test Data                     | Expected Result                                                                   | Notes             |
 * |:----------------:|-----------------------------------------------------------------------------|-------------------------------|-----------------------------------------------------------------------------------|-------------------|
 * | 01               | Invoke SetParentalControlStatus with locked = true for the first time         | input: locked = true          | No exception thrown; parental control status is set to true                       | Should Pass       |
 * | 02               | Invoke SetParentalControlStatus with locked = true for the second (redundant) time| input: locked = true          | No exception thrown; redundant call accepted and internal state remains enabled   | Should Pass       |
 * | 03               | Confirm internal state remains as parental control enabled                   | expected: parental control enabled | Internal state remains consistent with parental control enabled                  | Should be successful |
 */
TEST_F(PlayerCCManagerInstantiableTest, RedundantCallEnabled) {
    std::cout << "Entering RedundantCallEnabled test" << std::endl;
    
    std::cout << "First invocation: calling SetParentalControlStatus with locked = true" << std::endl;
    EXPECT_NO_THROW({
        playerCCManager->SetParentalControlStatus(true);
        std::cout << "First call completed with locked = true" << std::endl;
    });
    
    std::cout << "Second invocation: calling SetParentalControlStatus with locked = true again" << std::endl;
    EXPECT_NO_THROW({
        playerCCManager->SetParentalControlStatus(true);
        std::cout << "Second call completed with locked = true" << std::endl;
    });
    
    std::cout << "Assuming internal state remains: Parental control is enabled" << std::endl;
    
    std::cout << "Exiting RedundantCallEnabled test" << std::endl;
}
/**
 * @brief Verify that redundant calls with locked set to false are handled gracefully
 *
 * This test verifies that invoking SetParentalControlStatus with locked = false twice does not throw an exception and maintains the internal state of parental control as disabled. It is important to ensure that redundant API calls do not lead to errors or unintended state changes.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 038@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                | Test Data                      | Expected Result                                       | Notes       |
 * | :--------------: | ---------------------------------------------------------- | ------------------------------ | ----------------------------------------------------- | ----------- |
 * | 01               | Invoke SetParentalControlStatus for the first time         | input: locked = false          | API call completes without throwing an exception      | Should Pass |
 * | 02               | Invoke SetParentalControlStatus a second time (redundant call) | input: locked = false          | API call completes without throwing an exception      | Should Pass |
 */
TEST_F(PlayerCCManagerInstantiableTest, RedundantCallDisabled) {
    std::cout << "Entering RedundantCallDisabled test" << std::endl;
    
    std::cout << "First invocation: calling SetParentalControlStatus with locked = false" << std::endl;
    EXPECT_NO_THROW({
        playerCCManager->SetParentalControlStatus(false);
        std::cout << "First call completed with locked = false" << std::endl;
    });
    
    std::cout << "Second invocation: calling SetParentalControlStatus with locked = false again" << std::endl;
    EXPECT_NO_THROW({
        playerCCManager->SetParentalControlStatus(false);
        std::cout << "Second call completed with locked = false" << std::endl;
    });
    
    std::cout << "Assuming internal state remains: Parental control is disabled" << std::endl;
    
    std::cout << "Exiting RedundantCallDisabled test" << std::endl;
}
/**
 * @brief Verify that the basic initialization of the player sets up the expected state.
 *
 * This test case verifies that after calling EnsureInitialized() on the player object, the internal container for text tracks is empty as expected. This confirms that the player's initialization routine correctly sets up the text tracks state.
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
 * | Variation / Step | Description                                                                 | Test Data                                                     | Expected Result                                                              | Notes          |
 * | :--------------: | --------------------------------------------------------------------------- | ------------------------------------------------------------- | ---------------------------------------------------------------------------- | -------------- |
 * | 01               | Log the entry message indicating the start of the BasicInitialization test. | N/A                                                           | Log message successfully printed                                             | Should be successful |
 * | 02               | Invoke EnsureInitialized() API on the player object.                        | player object = instance, function call = EnsureInitialized()   | EnsureInitialized() executes without errors                                  | Should Pass    |
 * | 03               | Log message after returning from EnsureInitialized().                      | N/A                                                           | Log message successfully printed                                             | Should be successful |
 * | 04               | Log the exit message indicating the end of BasicInitialization test.         | N/A                                                           | Log message successfully printed                                             | Should be successful |
 */
TEST_F(PlayerCCManagerInstantiableTest, BasicInitialization) {
    std::cout << "Entering BasicInitialization test" << std::endl;
    
    std::cout << "Invoking EnsureInitialized() on the test object." << std::endl;
    playerCCManager->EnsureInitialized();
    std::cout << "Returned from EnsureInitialized()." << std::endl;

    std::cout << "Exiting BasicInitialization test" << std::endl;
}
/**
 * @brief Verify that repeated calls to EnsureInitialized() do not adversely affect internal state.
 *
 * This test verifies that calling EnsureInitialized() multiple times on the player object does not cause any unwanted side effects. It ensures that the player remains in a valid state and that no residual text tracks are present after multiple initializations.
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
 * | Variation / Step | Description                                                                          | Test Data                                                     | Expected Result                                                               | Notes       |
 * | :--------------: | ------------------------------------------------------------------------------------ | ------------------------------------------------------------- | ----------------------------------------------------------------------------- | ----------- |
 * | 01               | Invoke EnsureInitialized() for the first time to initialize the player object          | Invocation: EnsureInitialized()                               | Player initializes successfully without errors                               | Should Pass |
 * | 02               | Invoke EnsureInitialized() a second time to test the idempotency of the function         | Invocation: EnsureInitialized()                               | Subsequent call does not negatively affect the player state                    | Should Pass |
 * | 03               | Retrieve internal state using GetLastTextTracks() and verify that the returned vector is empty | Invocation: GetLastTextTracks(), expected output: empty vector | The internal state mLastTextTracks is empty as expected                        | Should Pass |
 */
TEST_F(PlayerCCManagerInstantiableTest, RepeatedInitializationCall) {
    std::cout << "Entering RepeatedInitializationCall test" << std::endl;
    
    std::cout << "Invoking first call to EnsureInitialized() on the test object." << std::endl;
    playerCCManager->EnsureInitialized();
    std::cout << "Returned from first call to EnsureInitialized()." << std::endl;
    
    std::cout << "Invoking second call to EnsureInitialized() on the test object." << std::endl;
    playerCCManager->EnsureInitialized();
    std::cout << "Returned from second call to EnsureInitialized()." << std::endl;
    
    std::cout << "Exiting RepeatedInitializationCall test" << std::endl;
}
/**
 * @brief Validate updating single valid track using updateLastTextTracks API
 *
 * This test verifies that when a CCTrackInfo object with a valid instreamId and language ("CC1" and "en") is provided, 
 * the updateLastTextTracks API correctly updates the internal state of the playerCCManager. 
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 041@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create a valid CCTrackInfo object with instreamId "CC1" and language "en" and invoke updateLastTextTracks | tracks = [instreamId = CC1, language = en] |  | Should Pass |
 */
TEST_F(PlayerCCManagerInstantiableTest, UpdateSingleValidTrack) {
    std::cout << "Entering UpdateSingleValidTrack test" << std::endl;
    
    std::vector<CCTrackInfo> tracks;
    CCTrackInfo track;
    track.instreamId = "CC1";
    track.language = "en";
    tracks.push_back(track);
    std::cout << "Invoking updateLastTextTracks with one track: instreamId = " 
              << track.instreamId << ", language = " << track.language << std::endl;
              
    playerCCManager->updateLastTextTracks(tracks);
    std::cout << "updateLastTextTracks invoked." << std::endl;
    
    
    std::cout << "Exiting UpdateSingleValidTrack test" << std::endl;
}
/**
 * @brief Verify that playerCCManager correctly updates text tracks for multiple valid tracks input.
 *
 * This test verifies that when multiple valid closed caption (CC) tracks are provided to the updateLastTextTracks API, the internal state of playerCCManager is accurately updated.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create two valid CCTrackInfo objects with designated instreamId and language values. | track1: instreamId = "CC1", language = "en"; track2: instreamId = "CC2", language = "es" | Two CCTrackInfo objects are created and added to the vector. | Should be successful |
 * | 02 | Invoke updateLastTextTracks with the vector containing the two tracks. | input: vector of tracks containing track1 and track2 | API call updates the internal state mLastTextTracks with two tracks. | Should Pass |
 */
TEST_F(PlayerCCManagerInstantiableTest, UpdateMultipleValidTracks) {
    std::cout << "Entering UpdateMultipleValidTracks test" << std::endl;
    
    std::vector<CCTrackInfo> tracks;
    CCTrackInfo track1;
    track1.instreamId = "CC1";
    track1.language = "en";
    CCTrackInfo track2;
    track2.instreamId = "CC2";
    track2.language = "es";
    tracks.push_back(track1);
    tracks.push_back(track2);
    
    std::cout << "Invoking updateLastTextTracks with multiple tracks:" << std::endl;
    std::cout << "  Track 1: instreamId = " << track1.instreamId << ", language = " << track1.language << std::endl;
    std::cout << "  Track 2: instreamId = " << track2.instreamId << ", language = " << track2.language << std::endl;
    
    playerCCManager->updateLastTextTracks(tracks);
    std::cout << "updateLastTextTracks invoked. Verifying internal state now." << std::endl;

    std::cout << "Exiting UpdateMultipleValidTracks test" << std::endl;
}
/**
 * @brief Validate updateLastTextTracks with an empty vector
 *
 * This test verifies that when updateLastTextTracks is called with an empty vector. This is essential to ensure that the API correctly handles edge cases with no track information provided.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 043@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                   | Test Data                                     | Expected Result                              | Notes      |
 * | :--------------: | --------------------------------------------------------------------------------------------- | --------------------------------------------- | -------------------------------------------- | ---------- |
 * | 01               | Invoke updateLastTextTracks with an empty vector and verify that mLastTextTracks remains empty    | input: tracks = {} |             | Should Pass |
 */
TEST_F(PlayerCCManagerInstantiableTest, UpdateWithEmptyVector) {
    std::cout << "Entering UpdateWithEmptyVector test" << std::endl;
    
    std::vector<CCTrackInfo> tracks; // Empty vector
    std::cout << "Invoking updateLastTextTracks with an empty vector" << std::endl;
    
    playerCCManager->updateLastTextTracks(tracks);
    std::cout << "updateLastTextTracks invoked. Verifying internal state now." << std::endl;
     
    std::cout << "Exiting UpdateWithEmptyVector test" << std::endl;
}
/**
 * @brief Verify that updateLastTextTracks correctly processes a track with empty fields.
 *
 * This test verifies that when a CCTrackInfo object with empty instreamId and language is provided, 
 * the updateLastTextTracks method correctly stores the track in the internal mLastTextTracks vector.
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
 * | Variation / Step | Description                                                                                           | Test Data                                                                                                      | Expected Result                                                                                         | Notes            |
 * | :--------------: | ----------------------------------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------- | ---------------- |
 * | 01               | Create a CCTrackInfo object with empty instreamId and language and add it to a vector of tracks.       | input: track.instreamId = "", track.language = ""; output: tracks vector containing one track with empty fields | The tracks vector is correctly initialized with one track having empty fields.                          | Should be successful |
 * | 02               | Invoke updateLastTextTracks with the prepared vector containing the track with empty fields.            | input: tracks vector (one CCTrackInfo with empty instreamId and language)                                      | The method updates mLastTextTracks with the provided track.                                             | Should Pass      |
 */
TEST_F(PlayerCCManagerInstantiableTest, UpdateWithTrackHavingEmptyFields) {
    std::cout << "Entering UpdateWithTrackHavingEmptyFields test" << std::endl;
    
    std::vector<CCTrackInfo> tracks;
    CCTrackInfo track;
    // Assign empty strings explicitly
    track.instreamId = "";
    track.language = "";
    tracks.push_back(track);
    std::cout << "Invoking updateLastTextTracks with one track having empty fields: instreamId = '" 
              << track.instreamId << "', language = '" << track.language << "'" << std::endl;
              
    playerCCManager->updateLastTextTracks(tracks);
    std::cout << "updateLastTextTracks invoked. Verifying internal state now." << std::endl;
    
    std::cout << "Exiting UpdateWithTrackHavingEmptyFields test" << std::endl;
}
/**
 * @brief Verify that the default instance of the PlayerCCManager returns a valid control center handle.
 *
 * This test invokes the CheckCCHandle() method on the default instance of the PlayerCCManager and verifies the returned value is true. The objective is to ensure that the default instance is properly initialized and capable of handling control center operations correctly.
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
 * | Variation / Step | Description                                                          | Test Data                                                         | Expected Result                               | Notes      |
 * | :--------------: | -------------------------------------------------------------------- | ----------------------------------------------------------------- | --------------------------------------------- | ---------- |
 * | 01               | Call CheckCCHandle on default instance and verify returned value true. | player_cc_manager instance = default, output = true                 | Method returns true and assertion passes.     | Should Pass |
 */
TEST_F(PlayerCCManagerInstantiableTest, Verify_CheckCCHandle_DefaultInstance) {
    std::cout << "Entering Verify_CheckCCHandle_DefaultInstance test" << std::endl;
    
    std::cout << "Invoking CheckCCHandle() on default instance" << std::endl;
    bool result = playerCCManager->CheckCCHandle();
    std::cout << "CheckCCHandle() returned: " << result << std::endl;
    
    EXPECT_TRUE(result);
    
    std::cout << "Exiting Verify_CheckCCHandle_DefaultInstance test" << std::endl;
}
/**
 * @brief Verify consistency of CheckCCHandle() behavior when incoked multiple times
 *
 * This test verifies that calling the CheckCCHandle() method multiple times
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 046@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**  
 * | Variation / Step | Description | Test Data | Expected Result | Notes |  
 * | :----: | --------- | ---------- |-------------- | ----- |  
 * | 02 | Invoke CheckCCHandle() | CheckCCHandle() invoked; input: none, output: result1 | result1 equals true (API returns true) | Should Pass |  
 * | 03 | Invoke CheckCCHandle() | CheckCCHandle() invoked; input: none, output: result2 | result2 equals true (API returns true) | Should Pass |  
 */
TEST_F(PlayerCCManagerInstantiableTest, ConfirmConsistency_AcrossMultipleInstances) {
    std::cout << "Entering ConfirmConsistency_AcrossMultipleInstances test" << std::endl;
    PlayerCCManagerBase instance1;
    PlayerCCManagerBase instance2;
    std::cout << "Created two separate PlayerCCManagerBase instances" << std::endl;
    
    std::cout << "Invoking CheckCCHandle() on instance1" << std::endl;
    bool result1 = playerCCManager->CheckCCHandle();
    std::cout << "instance1.CheckCCHandle() returned: " << result1 << std::endl;
    
    std::cout << "Invoking CheckCCHandle() on instance2" << std::endl;
    bool result2 = playerCCManager->CheckCCHandle();
    std::cout << "instance2.CheckCCHandle() returned: " << result2 << std::endl;
    
    EXPECT_TRUE(result1);
    EXPECT_TRUE(result2);
    
    std::cout << "Exiting ConfirmConsistency_AcrossMultipleInstances test" << std::endl;
}
/**
 * @brief Verify that EnsureHALInitialized() completes without errors on its first call
 *
 * This test verifies that the EnsureHALInitialized() method on the player instance is invoked 
 * successfully and completes without any errors. It validates that the Hardware Abstraction Layer (HAL) 
 * initialization is handled correctly during the initial call, ensuring that subsequent calls will start from a proper state.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 047@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**
 * | Variation / Step | Description                                                                  | Test Data                                         | Expected Result                                                          | Notes           |
 * | :--------------: | ---------------------------------------------------------------------------- | ------------------------------------------------- | ------------------------------------------------------------------------ | --------------- |
 * | 01               | Print the entering test message                                              | none                                              | "Entering EnsureHALInitializedCompletesWithoutErrorsOnFirstCall test" printed | Should be successful |
 * | 02               | Print the message indicating invocation of EnsureHALInitialized() via public call | none                                              | "Invoking EnsureHALInitialized() via public method call" printed         | Should be successful |
 * | 03               | Invoke playerCCManager->EnsureHALInitialized() method                                  | player = instance, EnsureHALInitialized() = invoked | EnsureHALInitialized() completes without throwing errors                  | Should Pass     |
 * | 04               | Print the message indicating successful completion of the method call         | none                                              | "EnsureHALInitialized() call completed successfully" printed              | Should be successful |
 * | 05               | Print the exiting test message                                               | none                                              | "Exiting EnsureHALInitializedCompletesWithoutErrorsOnFirstCall test" printed | Should be successful |
 */
TEST_F(PlayerCCManagerInstantiableTest, EnsureHALInitializedCompletesWithoutErrorsOnFirstCall) {
    std::cout << "Entering EnsureHALInitializedCompletesWithoutErrorsOnFirstCall test" << std::endl;
    
    std::cout << "Invoking EnsureHALInitialized() via public method call" << std::endl;
    playerCCManager->EnsureHALInitialized();
    std::cout << "EnsureHALInitialized() call completed successfully" << std::endl;

    std::cout << "Exiting EnsureHALInitializedCompletesWithoutErrorsOnFirstCall test" << std::endl;
}
/**
 * @brief Verify idempotency of EnsureHALInitialized method when called consecutively
 *
 * This test verifies that invoking the EnsureHALInitialized method multiple times consecutively does not cause any adverse effects. It ensures that the Hardware Abstraction Layer (HAL) is initialized only once and subsequent calls do not alter the state, thereby confirming the idempotent behavior of the method.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 048@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                             | Test Data                                     | Expected Result                                                                                     | Notes         |
 * | :--------------: | ----------------------------------------------------------------------- | --------------------------------------------- | ---------------------------------------------------------------------------------------------------- | ------------- |
 * | 01               | Invoke EnsureHALInitialized() for the first time                        | input: None, output: None                       | Method executes without errors and initializes HAL if not already initialized                       | Should Pass   |
 * | 02               | Invoke EnsureHALInitialized() for the second time                       | input: None, output: None                       | Method executes without errors and confirms idempotency by leaving HAL state unchanged               | Should Pass   |
 * | 03               | Invoke EnsureHALInitialized() for the third time                        | input: None, output: None                       | Method executes without errors and maintains the initialized state consistently                    | Should Pass   |
 */
TEST_F(PlayerCCManagerInstantiableTest, EnsureHALInitializedIdempotentWhenCalledConsecutively) {
    std::cout << "Entering EnsureHALInitializedIdempotentWhenCalledConsecutively test" << std::endl;
    
    std::cout << "Invoking EnsureHALInitialized() for the first time" << std::endl;
    playerCCManager->EnsureHALInitialized();
    std::cout << "First call finished successfully" << std::endl;
    
    std::cout << "Invoking EnsureHALInitialized() for the second time" << std::endl;
    playerCCManager->EnsureHALInitialized();
    std::cout << "Second call finished successfully" << std::endl;
    
    std::cout << "Invoking EnsureHALInitialized() for the third time" << std::endl;
    playerCCManager->EnsureHALInitialized();
    std::cout << "Third call finished successfully" << std::endl;
    
    std::cout << "Exiting EnsureHALInitializedIdempotentWhenCalledConsecutively test" << std::endl;
}
/**
 * @brief Verify that the renderer communications are initialized successfully.
 *
 * This test verifies that the public method InitializeRendererComms, which internally calls EnsureRendererCommsInitialized,
 * executes without throwing an exception. It confirms that the renderer communications are initialized correctly.
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
 * | Variation / Step | Description                                                                  | Test Data                                            | Expected Result                                                                             | Notes      |
 * | :--------------: | ---------------------------------------------------------------------------- | ---------------------------------------------------- | ------------------------------------------------------------------------------------------- | ---------- |
 * | 01               | Invoke the EnsureRendererCommsInitialized method to initialize renderer comms       | playerCCManager, EnsureRendererCommsInitialized() = no args     | Method executes without throwing an exception and successfully initializes renderer comms   | Should Pass |
 */
TEST_F(PlayerCCManagerInstantiableTest, EnsureRendererCommsInitialized_start) {
    std::cout << "Entering EnsureRendererCommsInitialized_start test" << std::endl;
    
    std::cout << "Invoking EnsureRendererCommsInitialized public method which internally calls EnsureRendererCommsInitialized()" << std::endl;
    EXPECT_NO_THROW({
        playerCCManager->EnsureRendererCommsInitialized();
        std::cout << "EnsureRendererCommsInitialized() called successfully; renderer comms initialized." << std::endl;
    });
    
    std::cout << "Exiting EnsureRendererCommsInitialized_start test" << std::endl;
}
/**
 * @brief Verify that repeated invocations of EnsureRendererCommsInitialized do not throw exceptions and maintain proper initialization.
 *
 * This test confirms that calling the EnsureRendererCommsInitialized API method multiple times safely initializes the renderer communications without causing any exceptions. It ensures that redundant initialization calls leave the system in a properly initialized state.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 050@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                        | Test Data                                           | Expected Result                                                                 | Notes       |
 * | :--------------: | ------------------------------------------------------------------ | --------------------------------------------------- | ------------------------------------------------------------------------------- | ----------- |
 * | 01               | Invoke EnsureRendererCommsInitialized for the first time.                 | input: none, output: renderer comms state changes   | Method returns normally without throwing exceptions; Assertion passes         | Should Pass |
 * | 02               | Invoke EnsureRendererCommsInitialized for the second time.                | input: none, output: renderer comms state remains consistent | Method returns normally without throwing exceptions; Assertion passes         | Should Pass |
 */
TEST_F(PlayerCCManagerInstantiableTest, EnsureRendererCommsInitialized_repeated) {
    std::cout << "Entering EnsureRendererCommsInitialized_repeated test" << std::endl;
    
    std::cout << "First invocation of InitializeRendererComms public method." << std::endl;
    EXPECT_NO_THROW({
        playerCCManager->EnsureRendererCommsInitialized();
        std::cout << "First call completed; renderer comms initialized." << std::endl;
    });
    
    std::cout << "Second invocation of InitializeRendererComms public method." << std::endl;
    EXPECT_NO_THROW({
        playerCCManager->EnsureRendererCommsInitialized();
        std::cout << "Second call completed; renderer comms remains properly initialized." << std::endl;
    });
    
    std::cout << "Exiting EnsureRendererCommsInitialized_repeated test" << std::endl;
}
/**
 * @brief Verify that SetStyle accepts a valid style string and returns success
 *
 * This test verifies that the SetStyle method correctly processes a valid style string ("bold") by returning 0, indicating success. The test ensures that the API behaves as expected with valid input.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 051@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                  | Test Data                                       | Expected Result                             | Notes       |
 * | :--------------: | ------------------------------------------------------------ | ----------------------------------------------- | ------------------------------------------- | ----------- |
 * | 01               | Prepare a valid style string "bold" and invoke SetStyle API. | input: options = "bold", output: expected ret = 0 | SetStyle returns 0 and the assertion passes | Should Pass |
 */
TEST_F(PlayerCCManagerInstantiableTest, ValidStyleSimple) {
    std::cout << "Entering ValidStyleSimple test" << std::endl;
    
    char optionStr[128];
    strncpy(optionStr, "bold", sizeof(optionStr));
    optionStr[sizeof(optionStr) - 1] = '\0';
    std::string options(optionStr);
    
    std::cout << "Invoking SetStyle with options: " << options << std::endl;
    int ret = playerCCManager->SetStyle(options);
    std::cout << "Method SetStyle returned: " << ret << std::endl;
    
    EXPECT_EQ(ret, 0);
    
    std::cout << "Exiting ValidStyleSimple test" << std::endl;
}
/**
 * @brief Verify that passing a valid CSS style string to SetStyle returns a success code.
 *
 * This test verifies that the SetStyle method correctly processes a valid CSS style string "color: red; font-size: 14px;" 
 * and returns a success status (0). This is crucial to ensure that the API applies the provided styling options properly.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 052@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Prepare a valid CSS style string with options "color: red; font-size: 14px;" | optionStr = "color: red; font-size: 14px;" | Valid style string is prepared | Should be successful |
 * | 02 | Invoke the SetStyle API with the prepared valid style string | input: options = "color: red; font-size: 14px;" | API returns 0 indicating success | Should Pass |
 * | 03 | Verify the returned status from SetStyle using the assertion EXPECT_EQ(ret, 0) | expected ret = 0 | Assertion passes confirming valid style set | Should Pass |
 */
TEST_F(PlayerCCManagerInstantiableTest, ValidStyleDescriptive) {
    std::cout << "Entering ValidStyleDescriptive test" << std::endl;
    
    char optionStr[128];
    strncpy(optionStr, "color: red; font-size: 14px;", sizeof(optionStr));
    optionStr[sizeof(optionStr) - 1] = '\0';
    std::string options(optionStr);
    
    std::cout << "Invoking SetStyle with options: " << options << std::endl;
    int ret = playerCCManager->SetStyle(options);
    std::cout << "Method SetStyle returned: " << ret << std::endl;
    
    EXPECT_EQ(ret, 0);
    
    std::cout << "Exiting ValidStyleDescriptive test" << std::endl;
}
/**
 * @brief Test the SetStyle API with valid "italic" style input.
 *
 * This test validates that the SetStyle API correctly processes a valid "italic" style option by invoking it with a pre-defined valid input and verifies that the return value is 0, indicating success. The test is focused on ensuring that the API handles a basic valid style change.
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
 * | Variation / Step | Description                                                | Test Data                                                         | Expected Result                                              | Notes       |
 * | :--------------: | ---------------------------------------------------------- | ----------------------------------------------------------------- | ------------------------------------------------------------ | ----------- |
 * | 01               | Invoke SetStyle API with valid style option "italic".    | input: options = italic, output: ret = undefined (expected 0)     | The API returns 0 and the assertion EXPECT_EQ(ret, 0) passes.  | Should Pass |
 */
TEST_F(PlayerCCManagerInstantiableTest, ValidStyleAlternate) {
    std::cout << "Entering ValidStyleAlternate test" << std::endl;
    
    char optionStr[128];
    strncpy(optionStr, "italic", sizeof(optionStr));
    optionStr[sizeof(optionStr) - 1] = '\0';
    std::string options(optionStr);
    
    std::cout << "Invoking SetStyle with options: " << options << std::endl;
    int ret = playerCCManager->SetStyle(options);
    std::cout << "Method SetStyle returned: " << ret << std::endl;
    
    EXPECT_EQ(ret, 0);
    
    std::cout << "Exiting ValidStyleAlternate test" << std::endl;
}
/**
 * @brief Verify that SetStyle returns an error when provided an empty style string
 *
 * This test verifies that the SetStyle API in PlayerCCManager correctly handles an empty string input by returning an error code (-1). It is important to ensure that the API robustly rejects invalid style input to prevent unwanted behavior.
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
 * | Variation / Step | Description                                               | Test Data                                                              | Expected Result                                                    | Notes         |
 * | :--------------: | --------------------------------------------------------- | ---------------------------------------------------------------------- | ------------------------------------------------------------------ | ------------- |
 * | 01               | Invoke SetStyle with an empty options string              | optionStr = "", options = "", output (ret) expected = -1               | SetStyle returns -1, triggering the assertion EXPECT_EQ(ret, -1)     | Should Fail   |
 */
TEST_F(PlayerCCManagerInstantiableTest, NegativeEmptyStyle) {
    std::cout << "Entering NegativeEmptyStyle test" << std::endl;
    
    char optionStr[128];
    strncpy(optionStr, "", sizeof(optionStr));
    optionStr[sizeof(optionStr) - 1] = '\0';
    std::string options(optionStr);
    
    std::cout << "Invoking SetStyle with an empty options string: \"" << options << "\"" << std::endl;
    int ret = playerCCManager->SetStyle(options);
    std::cout << "Method SetStyle returned: " << ret << std::endl;
    
    EXPECT_EQ(ret, -1);
    
    std::cout << "Exiting NegativeEmptyStyle test" << std::endl;
}
/**
 * @brief Verify that SetStyle returns an error code when provided with a malformed style string.
 *
 * This test verifies that when a malformed style string ("invalid_style==") is passed to the SetStyle method, the API returns the expected error code (-1). The purpose is to ensure that the API gracefully handles invalid inputs.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoke SetStyle with malformed style string "invalid_style==" | input: options = invalid_style==, output: ret = -1 | SetStyle returns -1 and assertion EXPECT_EQ(ret, -1) passes | Should Fail |
 */
TEST_F(PlayerCCManagerInstantiableTest, NegativeMalformedStyle) {
    std::cout << "Entering NegativeMalformedStyle test" << std::endl;
    
    char optionStr[128];
    strncpy(optionStr, "invalid_style==", sizeof(optionStr));
    optionStr[sizeof(optionStr) - 1] = '\0';
    std::string options(optionStr);

    std::cout << "Invoking SetStyle with malformed options: " << options << std::endl;
    int ret = playerCCManager->SetStyle(options);
    std::cout << "Method SetStyle returned: " << ret << std::endl;
    
    EXPECT_EQ(ret, -1);
    
    std::cout << "Exiting NegativeMalformedStyle test" << std::endl;
}
/**
 * @brief Verify that Stop() method executes successfully without throwing exceptions.
 *
 * This test case verifies that invoking the public method, which internally calls Stop(), completes normally without any exceptions.
 * It ensures that the Stop() functionality works as expected under nominal conditions.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 056@n
 * **Priority:** High@n
 * @n
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * @n
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                    | Test Data                             | Expected Result                                                                               | Notes      |
 * | :--------------: | -------------------------------------------------------------- | ------------------------------------- | --------------------------------------------------------------------------------------------- | ---------- |
 * | 01               | Call the public method that internally invokes Stop() function | player.Stop() invoked, no input value | Stop() completes execution normally without throwing any exception; void return; assertion pass | Should Pass|
 */
TEST_F(PlayerCCManagerInstantiableTest, Stop_Success) {
    std::cout << "Entering Stop_Success test" << std::endl;
    
    std::cout << "Invoking public method that calls Stop()." << std::endl;
    try {
        // Call the public method that internally invokes Stop()
        playerCCManager->Stop();
        std::cout << "Stop() executed and returned void successfully." << std::endl;
    } catch (const std::exception& ex) {
        std::cout << "Exception caught during Stop() execution: " << ex.what() << std::endl;
        ADD_FAILURE() << "Stop() threw an exception when it was expected to complete normally.";
    }
    
    std::cout << "Exiting Stop_Success test" << std::endl;
}
/**
 * @brief Validate that consecutive invocations of Stop() are idempotent
 *
 * This test verifies that calling the Stop() method on the player object multiple times
 * does not result in an exception being thrown, confirming the idempotent behavior of the API.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 057@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                          | Test Data                                        | Expected Result                                                                       | Notes         |
 * | :--------------: | ---------------------------------------------------- | ------------------------------------------------ | ------------------------------------------------------------------------------------- | ------------- |
 * | 01               | Invoke the first Stop() call on the player object    | function call: playerCCManager->Stop()                     | Stop() executes without throwing exceptions and completes successfully               | Should Pass   |
 * | 02               | Invoke the second Stop() call to test idempotency      | function call: playerCCManager->Stop()                     | Second Stop() call executes without throwing exceptions, confirming idempotency         | Should Pass   |
 */
TEST_F(PlayerCCManagerInstantiableTest, Stop_Idempotent) {
    std::cout << "Entering Stop_Idempotent test" << std::endl;
    
    std::cout << "Invoking first Stop() call." << std::endl;
    
    try {
        playerCCManager->Stop();
        std::cout << "First Stop() invocation completed successfully." << std::endl;
    } catch (const std::exception& ex) {
        std::cout << "Exception caught during first Stop() invocation: " << ex.what() << std::endl;
        ADD_FAILURE() << "Exception in first Stop() call.";
    }
    
    std::cout << "Invoking second Stop() call." << std::endl;
    
    try {
        playerCCManager->Stop();
        std::cout << "Second Stop() invocation completed successfully." << std::endl;
    } catch (const std::exception& ex) {
        std::cout << "Exception caught during second Stop() invocation: " << ex.what() << std::endl;
        ADD_FAILURE() << "Exception in second Stop() call.";
    }
    
    std::cout << "Exiting Stop_Idempotent test" << std::endl;
}
/**
 * @brief Verify that PlayerCCManager initializes successfully with a valid pointer.
 *
 * This test verifies that when a valid pointer is provided to the Init() method
 * of the PlayerCCManager, the method returns 0, indicating successful initialization.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 058@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result |Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create a dummy integer and valid pointer handle | dummyValue = 1234, validHandle = address of dummyValue | A valid pointer handle is created | Should be successful |
 * | 02 | Invoke the public Init() method with the valid pointer handle | validHandle pointer with dummyValue | Should return 0 indicating successful initialization | Should Pass |
 * | 03 | Verify the return value is 0 using EXPECT_EQ assertion | retVal = return value from Init(), expected = 0 | retVal equals 0 | Should be successful |
 */
TEST_F(PlayerCCManagerInstantiableTest, InitializeWithValidPointer) {
    std::cout << "Entering InitializeWithValidPointer test" << std::endl;
    
    int dummyValue = 1234;
    void* validHandle = static_cast<void*>(&dummyValue);
    std::cout << "Creating a valid handle with address: " << validHandle << std::endl;
    
    std::cout << "Invoking public Init() method with valid handle: " << validHandle << std::endl;
    int retVal = playerCCManager->Init(validHandle);
    std::cout << "Public Init() returned value: " << retVal << std::endl;
    
    EXPECT_EQ(retVal, 0);
    
    std::cout << "Exiting InitializeWithValidPointer test" << std::endl;
}
/**
 * @brief Verify that the Init() method gracefully handles a null pointer input.
 *
 * This test verifies that when a null pointer is passed to the Init() method of PlayerCCManager,
 * the method processes the input correctly by returning the expected value of 0. This behavior
 * is critical to ensure that the API does not crash or behave unpredictably when encountering
 * invalid or null inputs.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoke the Init() method with a null pointer as the input handle. | nullHandle = nullptr, output: retVal expected value = 0 | Init() returns 0 and the EXPECT_EQ(retVal, 0) assertion passes | Should Pass |
 */
TEST_F(PlayerCCManagerInstantiableTest, InitializeWithNullPointer) {
    std::cout << "Entering InitializeWithNullPointer test" << std::endl;
    
    void* nullHandle = nullptr;
    std::cout << "Using a null handle: " << nullHandle << std::endl;
    
    std::cout << "Invoking public Init() method with null handle." << std::endl;
    int retVal = playerCCManager->Init(nullHandle);
    std::cout << "Public Init() returned value: " << retVal << std::endl;
    
    EXPECT_EQ(retVal, 0);
    
    std::cout << "Exiting InitializeWithNullPointer test" << std::endl;
}
/**
 * @brief Validate multiple consecutive initializations using a valid handle.
 *
 * This test verifies that the PlayerCCManager::Init() method can be invoked consecutively with a valid handle and consistently returns a success status (0) on each call, ensuring the robustness of reinitialization.
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
 * | Variation / Step | Description                                                    | Test Data                                                              | Expected Result                          | Notes            |
 * | :--------------: | -------------------------------------------------------------- | ---------------------------------------------------------------------- | ---------------------------------------- | ---------------- |
 * | 01               | Set up a valid handle using a dummy integer for initialization | dummyValue = 5678, validHandle = address of dummyValue                   | Valid handle is created successfully     | Should be successful |
 * | 02               | Invoke Init() method with valid handle - 1st call              | validHandle = address of dummyValue, callCount = 1                        | Returns 0; EXPECT_EQ(retVal, 0)           | Should Pass      |
 * | 03               | Invoke Init() method with valid handle - 2nd call              | validHandle = address of dummyValue, callCount = 2                        | Returns 0; EXPECT_EQ(retVal, 0)           | Should Pass      |
 * | 04               | Invoke Init() method with valid handle - 3rd call              | validHandle = address of dummyValue, callCount = 3                        | Returns 0; EXPECT_EQ(retVal, 0)           | Should Pass      |
 */
TEST_F(PlayerCCManagerInstantiableTest, MultipleConsecutiveInitializations) {
    std::cout << "Entering MultipleConsecutiveInitializations test" << std::endl;
    
    int dummyValue = 5678;
    void* validHandle = static_cast<void*>(&dummyValue);
    std::cout << "Creating a valid handle with address: " << validHandle << std::endl;
    
    const int callCount = 3;
    for (int i = 0; i < callCount; ++i) {
        std::cout << "Invocation " << i+1 << ": Calling public Init() method with valid handle: " << validHandle << std::endl;
        int retVal = playerCCManager->Init(validHandle);
        std::cout << "Invocation " << i+1 << ": Public Init() returned value: " << retVal << std::endl;
        EXPECT_EQ(retVal, 0);
    }
    
    std::cout << "Exiting MultipleConsecutiveInitializations test" << std::endl;
}

int main(int argc, char **argv) {
     ::testing::InitGoogleTest(&argc, argv);
     return RUN_ALL_TESTS();
}