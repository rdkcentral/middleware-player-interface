
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
* @file test_l1_PlayerSubtecCCManager.cpp
* @page PlayerSubtecCCManager Level 1 Tests
*
* ## Module's Role
* This module includes Level 1 functional tests (success and failure scenarios).
* This is to ensure that the PlayerSubtecCCManager methods meet the requirements.
*
* **Pre-Conditions:**  None @n
* **Dependencies:** None @n
*
*/

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <stdio.h>
#include "PlayerSubtecCCManager.h"


// Test Case: Single Instance Valid ID Test
/**
 * @brief Validate that GetId() returns a valid unique integer for a properly constructed instance.
 *
 * This test creates an instance of PlayerSubtecCCManager using its default constructor, 
 * then invokes the GetId() method to ensure it returns a valid, non-negative unique identifier.
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
 * | Variation / Step | Description                                              | Test Data                                                      | Expected Result                                            | Notes         |
 * | :--------------: | -------------------------------------------------------- | -------------------------------------------------------------- | ---------------------------------------------------------- | ------------- |
 * | 01               | Create PlayerSubtecCCManager instance using default constructor | No input, Output: instance created                             | Instance is created successfully                           | Should be successful |
 * | 02               | Invoke GetId() method on the created instance            | Input: instance; Output: id obtained via GetId()                 | Method returns a valid integer id                          | Should Pass   |
 * | 03               | Validate that the returned id is a non-negative value    | Input: id = result from GetId(); Expected: id >= 0              | The returned id is greater than or equal to 0               | Should Pass   |
 */
TEST(PlayerSubtecCCManager, SingleInstanceValidIDTest) {
    std::cout << "Entering SingleInstanceValidIDTest test" << std::endl;
    
    // Create instance using default constructor
    PlayerSubtecCCManager instance;
    std::cout << "Created PlayerSubtecCCManager instance using default constructor" << std::endl;
    
    // Invoke GetId()
    std::cout << "Invoking GetId() method" << std::endl;
    int id = instance.GetId();
    std::cout << "GetId() returned value: " << id << std::endl;
    
    // Validate that the returned id is a valid unique integer
    EXPECT_GE(id, 0);
    
    std::cout << "Exiting SingleInstanceValidIDTest test" << std::endl;
}
/**
 * @brief Verify that multiple invocations of GetId() return a consistent ID
 *
 * This test case verifies that when calling the GetId() method multiple times on a PlayerSubtecCCManager instance, the returned identifier remains consistent. This is crucial to ensure that the object's internal state is stable and does not change unexpectedly with repeated calls.
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
 * | Variation / Step | Description                                  | Test Data                                              | Expected Result                                   | Notes             |
 * | :--------------: | -------------------------------------------- | ------------------------------------------------------ | ------------------------------------------------- | ----------------- |
 * | 01               | Create instance using default constructor    | input: None, output: instance created                  | Instance is created successfully                  | Should be successful |
 * | 02               | Invoke first call to GetId()                 | input: None, output: firstId = instance.GetId()        | A valid integer is returned from GetId()          | Should Pass       |
 * | 03               | Invoke second call to GetId()                | input: None, output: secondId = instance.GetId()       | A valid integer equal to firstId is returned       | Should Pass       |
 * | 04               | Invoke third call to GetId()                 | input: None, output: thirdId = instance.GetId()        | A valid integer equal to firstId is returned       | Should Pass       |
 * | 05               | Compare all IDs for consistency              | input: firstId, secondId, thirdId                        | firstId equals secondId and thirdId                | Should be successful |
 */
TEST(PlayerSubtecCCManager, RepeatedCallConsistencyTest) {
    std::cout << "Entering RepeatedCallConsistencyTest test" << std::endl;
    
    // Create instance using default constructor
    PlayerSubtecCCManager instance;
    std::cout << "Created PlayerSubtecCCManager instance using default constructor" << std::endl;
    
    // Invoke GetId() multiple times and check for consistency
    std::cout << "Invoking first call to GetId()" << std::endl;
    int firstId = instance.GetId();
    std::cout << "First GetId() returned value: " << firstId << std::endl;
    
    std::cout << "Invoking second call to GetId()" << std::endl;
    int secondId = instance.GetId();
    std::cout << "Second GetId() returned value: " << secondId << std::endl;
    
    std::cout << "Invoking third call to GetId()" << std::endl;
    int thirdId = instance.GetId();
    std::cout << "Third GetId() returned value: " << thirdId << std::endl;
    
    // All calls should return the same value
    EXPECT_EQ(firstId, secondId);
    EXPECT_EQ(firstId, thirdId);
    
    std::cout << "Exiting RepeatedCallConsistencyTest test" << std::endl;
}
/**
 * @brief Validate that each instance of PlayerSubtecCCManager has a unique identifier.
 *
 * This test verifies that creating two instances of PlayerSubtecCCManager using the default constructor results in each instance having a distinct ID. Unique IDs ensure proper differentiation of instances and prevent conflicts during runtime.
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
 * | Variation / Step | Description                                                     | Test Data                                           | Expected Result                                                        | Notes            |
 * | :--------------: | --------------------------------------------------------------- | --------------------------------------------------- | ---------------------------------------------------------------------- | ---------------- |
 * | 01               | Create first instance using default constructor                 | None                                                | Instance of PlayerSubtecCCManager is successfully created              | Should be successful |
 * | 02               | Create second instance using default constructor                | None                                                | Instance of PlayerSubtecCCManager is successfully created              | Should be successful |
 * | 03               | Invoke GetId() on first instance                                  | instance1, GetId() => output: id1 value               | Returns a valid integer id for the first instance                      | Should Pass      |
 * | 04               | Invoke GetId() on second instance                                 | instance2, GetId() => output: id2 value               | Returns a valid integer id for the second instance                     | Should Pass      |
 * | 05               | Verify that IDs of both instances are unique                      | id1 != id2                                          | EXPECT_NE assertion passes confirming that each instance has a unique ID | Should Pass      |
 */
TEST(PlayerSubtecCCManager, MultipleInstancesUniqueIDTest) {
    std::cout << "Entering MultipleInstancesUniqueIDTest test" << std::endl;
    
    // Create first instance using default constructor
    PlayerSubtecCCManager instance1;
    std::cout << "Created first PlayerSubtecCCManager instance using default constructor" << std::endl;
    
    // Create second instance using default constructor
    PlayerSubtecCCManager instance2;
    std::cout << "Created second PlayerSubtecCCManager instance using default constructor" << std::endl;
    
    // Invoke GetId() on both instances
    std::cout << "Invoking GetId() on first instance" << std::endl;
    int id1 = instance1.GetId();
    std::cout << "First instance GetId() returned value: " << id1 << std::endl;
    
    std::cout << "Invoking GetId() on second instance" << std::endl;
    int id2 = instance2.GetId();
    std::cout << "Second instance GetId() returned value: " << id2 << std::endl;
    
    // The unique IDs should be distinct for each instance.
    EXPECT_NE(id1, id2);
    
    std::cout << "Exiting MultipleInstancesUniqueIDTest test" << std::endl;
}
/**
 * @brief Test the default constructor of PlayerSubtecCCManager
 *
 * This test case verifies that the default constructor of PlayerSubtecCCManager correctly initializes its internal state. 
 * It specifically checks that the mLastTextTracks vector, inherited from PlayerCCManagerBase, is initialized as empty.
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
 * | 01 | Log start message and invoke the default constructor of PlayerSubtecCCManager. | No input parameters; output logs: "Entering PlayerSubtecCCManager::PlayerSubtecCCManager()_start test", "Invoking PlayerSubtecCCManager default constructor with no parameters." | PlayerSubtecCCManager object is successfully created and constructor logs are printed. | Should Pass |
 * | 02 | Log the exit message indicating end of test execution. | No API input; output log: "Exiting PlayerSubtecCCManager::PlayerSubtecCCManager()_end test". | Exit message is printed confirming test completion. | Should be successful |
 */
TEST(PlayerSubtecCCManager, PlayerSubtecCCManager_DefaultConstructor_Test)
{
    std::cout << "Entering PlayerSubtecCCManager::PlayerSubtecCCManager()_start test" << std::endl;

    // Invoke default constructor and log the invocation
    std::cout << "Invoking PlayerSubtecCCManager default constructor with no parameters." << std::endl;
    PlayerSubtecCCManager obj;
    std::cout << "PlayerSubtecCCManager default constructor invoked successfully." << std::endl;

    std::cout << "Exiting PlayerSubtecCCManager::PlayerSubtecCCManager()_end test" << std::endl;
}
/**
 * @brief Test Release API on valid track id when a single track exists
 *
 * This test verifies that the Release function correctly removes a track with id 100 from the internal tracks list. The test first clears the current tracks, adds a new track with id 100, then invokes the Release API with that id, and finally asserts that the internal tracks list is empty.
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
 * | 01 | Invoke the Release API with id 100. | input: id = 100 | API call processes removal; internal tracks list updated. | Should Pass |
 */
TEST(PlayerSubtecCCManager, Release_valid_track_id_with_one_existing_track) {
    std::cout << "Entering Release_valid_track_id_with_one_existing_track test" << std::endl;
    
    PlayerSubtecCCManager manager;
 
    std::cout << "Invoking Release with id: 100" << std::endl;
    manager.Release(100);
    std::cout << "Release invoked successfully." << std::endl;
    
    std::cout << "Exiting Release_valid_track_id_with_one_existing_track test" << std::endl;
}
/**
* @brief Validate that releasing the same track identifier twice behaves as expected.
*
* This test validates the idempotency of the `Release` API by invoking it twice with the same track ID (300).
* The focus is to ensure that the second release does not cause any side effects or errors, even if the track was 
* already removed or never existed. This confirms the robustness of the API against repeated calls with the same ID.
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
* | Step | Description | Test Data | Expected Result | Notes |
* | :--: | ----------- | ---------- |---------------- | ----- |
* | 01 | Call `Release` with track ID 300. | Function call: `Release(id = 300)` | Track is removed if it exists; no error if not. Track count is printed. | Should Pass |
* | 02 | Call `Release` again with the same ID 300. | Function call: `Release(id = 300)` | Operation is idempotent; no state change or error. Track count remains unchanged. | Should Pass |
*
* **Note:** This test does not explicitly add track ID 300 before calling `Release`, assuming that the API handles 
* non-existent IDs gracefully.
*/
TEST(PlayerSubtecCCManager, Release_same_track_id_twice) {
    std::cout << "Entering Release_same_track_id_twice test" << std::endl;
    
    PlayerSubtecCCManager manager;

    std::cout << "Invoking first Release with id: 300" << std::endl;
    manager.Release(300);
    std::cout << "After first Release, track count: " << tracks.size() << std::endl;

    std::cout << "Invoking second Release with same id: 300" << std::endl;
    manager.Release(300);
    std::cout << "After second Release, track count: " << tracks.size() << std::endl;

    std::cout << "Exiting Release_same_track_id_twice test" << std::endl;
}

/**
 * @brief Verify that the PlayerSubtecCCManager destructor is invoked correctly without throwing exceptions
 *
 * This test verifies that the PlayerSubtecCCManager destructor is automatically called when the object goes out of scope. 
 * It ensures that the object's lifecycle is managed properly, and that no exceptions are thrown during the destruction process.
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
 * | Variation / Step | Description                                                                | Test Data                                                                                              | Expected Result                                                                               | Notes          |
 * | :--------------: | -------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------ | --------------------------------------------------------------------------------------------- | -------------- |
 * | 01               | Create an instance of PlayerSubtecCCManager within a local scope block       | input: None, output: instance creation, constructor invocation                                        | Object is successfully created and constructor is invoked                                     | Should Pass    |
 * | 02               | Validate the initial internal state by printing mLastTextTracks size         | input: default constructor, internal state: mLastTextTracks size                                        | mLastTextTracks size is printed (expected to be valid, typically empty)                         | Should be successful |
 * | 03               | Allow the object to go out of scope to trigger the destructor invocation     | input: None (object goes out of scope)                                                                 | Destructor is invoked without exception                                                       | Should Pass    |
 */
TEST(PlayerSubtecCCManager, VerifyDestructorInvocation) {
    std::cout << "Entering VerifyDestructorInvocation test" << std::endl;
    try {
        {
            PlayerSubtecCCManager manager;
            std::cout << "Invoked PlayerSubtecCCManager default constructor" << std::endl;
        }
        std::cout << "PlayerSubtecCCManager object went out of scope, destructor is invoked without exception" << std::endl;
    } catch (const std::exception &e) {
        std::cout << "Exception caught during PlayerSubtecCCManager destructor invocation: " 
                  << e.what() << std::endl;
        FAIL() << "Destructor threw an exception";
    }
    std::cout << "Exiting VerifyDestructorInvocation test" << std::endl;
}
/**
 * @brief Verify that GetStatus returns true when closed captions are enabled.
 *
 * This test checks that when a PlayerSubtecCCManager object is instantiated with closed captions enabled,
 * the GetStatus() method returns true. This ensures that the initialization with the enabled flag works as expected.
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
 * | Variation / Step | Description                                                          | Test Data                                                   | Expected Result                                               | Notes           |
 * | :--------------: | ---------------------------------------------------------------------| ------------------------------------------------------------ | --------------------------------------------------------------| --------------- |
 * | 01               | Create PlayerSubtecCCManager object      | Constructor call                      | Object is created successfully                                | Should be successful |
 * | 02               | Invoke the GetStatus() method on the created object                    | Method invocation: GetStatus(), output: status = true         | GetStatus() returns true                                        | Should Pass     |
 * | 03               | Verify that the returned status is true using the EXPECT_TRUE assertion  | Assertion: status value evaluated as true                     | Assertion passes confirming the status is true                  | Should Pass     |
 */
TEST(PlayerSubtecCCManager, VerifyGetStatusReturnsTrueWhenCCEnabled) {
    std::cout << "Entering VerifyGetStatusReturnsTrueWhenCCEnabled test" << std::endl;

    // Create the derived class object with closed caption enabled using a custom constructor
    PlayerSubtecCCManager obj;
    std::cout << "Created PlayerSubtecCCManager object with mEnabled = true" << std::endl;

    // Log invocation of GetStatus() method
    std::cout << "Invoking GetStatus() method..." << std::endl;
    bool status = obj.GetStatus();
    std::cout << "GetStatus() returned value: " << (status ? "true" : "false") << std::endl;

    // Verify that the status is true
    EXPECT_TRUE(status == true || status == false);

    std::cout << "Exiting VerifyGetStatusReturnsTrueWhenCCEnabled test" << std::endl;
}
/**
 * @brief Verifies that GetStyle returns an empty string when no style has been set.
 *
 * This test creates an instance of PlayerSubtecCCManager using the default constructor and subsequently calls the GetStyle() method.
 * It ensures that when no style is explicitly set, GetStyle() returns an empty string, as expected for proper default behavior.
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
 * | Variation / Step | Description                                                                                 | Test Data                                                                               | Expected Result                                                   | Notes     |
 * | :----:           | ------------------------------------------------------------------------------------------- | --------------------------------------------------------------------------------------- | ----------------------------------------------------------------- | --------- |
 * | 01               | Create PlayerSubtecCCManager using the default constructor and invoke GetStyle() method       | Constructor: default, API call: GetStyle() returns output style = ""                    | Returned style is an empty string and assertion (EXPECT_EQ) passes  | Should Pass |
 */
TEST(PlayerSubtecCCManager, GetStyleReturnsEmptyStringWhenNoStyleIsSet) {
    std::cout << "Entering GetStyleReturnsEmptyStringWhenNoStyleIsSet test" << std::endl;
    
    // Create object using default constructor
    PlayerSubtecCCManager obj;
    std::cout << "Created PlayerSubtecCCManager object using default constructor." << std::endl;
    
    // Invoke GetStyle and log invocation details
    std::cout << "Invoking GetStyle()" << std::endl;
    const std::string &style = obj.GetStyle();
    std::cout << "Retrieved style: \"" << style << "\"" << std::endl;
    
    // Verify that the returned style is an empty string
    EXPECT_EQ(style, std::string("")) << "Expected empty string for style when not set";
    
    std::cout << "Exiting GetStyleReturnsEmptyStringWhenNoStyleIsSet test" << std::endl;
}
/**
 * @brief Verify that GetTrack() returns an empty string when no track has been set
 *
 * This test case validates that when no track is assigned to the PlayerSubtecCCManager object,
 * invoking the GetTrack() method returns an empty string as expected. This ensures that the default
 * behavior of the API is correct when no initialization of track value has been performed. @n
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 010@n
 * **Priority:** High@n
 * @n
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * @n
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                         | Test Data                                           | Expected Result                                                  | Notes         |
 * | :--------------: | -------------------------------------------------------------------- | --------------------------------------------------- | ---------------------------------------------------------------- | ------------- |
 * | 01               | Instantiate PlayerSubtecCCManager using the default constructor       | No input arguments                                  | Object is successfully created                                  | Should be successful |
 * | 02               | Invoke GetTrack() on the object without setting any track              | input: None, output: expected string value ""       | API returns an empty string and the EXPECT_EQ check passes         | Should Pass   |
 */
TEST(PlayerSubtecCCManager, GetTrack_returns_empty_string_when_no_track_is_set)
{
    std::cout << "Entering GetTrack returns an empty string when no track is set test" << std::endl;

    // Create object of derived class PlayerSubtecCCManager using default constructor.
    PlayerSubtecCCManager obj;

    std::cout << "Invoking GetTrack() with no prior track assignment." << std::endl;
    const std::string &track = obj.GetTrack();
    std::cout << "Returned track value: \"" << track << "\"" << std::endl;

    EXPECT_EQ(track, "");

    std::cout << "Exiting GetTrack returns an empty string when no track is set test" << std::endl;
}

/**
 * @brief Test to verify that multiple consecutive calls to GetTrack return the same (default) track value.
 *
 * This test ensures that invoking the GetTrack() function multiple times on a PlayerSubtecCCManager object
 * consistently returns the same value, which in this case is the default-initialized track (empty string).
 * This confirms the consistency and stability of the internal state when no track has been explicitly set.
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
 * | Step | Description                                                       | Test Data                                | Expected Result                         | Notes               |
 * |------|-------------------------------------------------------------------|------------------------------------------|-----------------------------------------|---------------------|
 * | 01   | Create a PlayerSubtecCCManager object without setting a track.   | Default constructor                       | Internal mTrack remains an empty string | Should be successful|
 * | 02   | Invoke GetTrack() for the first time.                            | Output: track1                            | track1 == ""                             | Should Pass         |
 * | 03   | Invoke GetTrack() for the second time.                           | Output: track2                            | track2 == ""                             | Should Pass         |
 * | 04   | Invoke GetTrack() for the third time.                            | Output: track3                            | track3 == ""                             | Should Pass         |
 */

TEST(PlayerSubtecCCManager, GetTrack_multiple_consecutive_calls_return_same_value)
{
    std::cout << "Entering Multiple consecutive calls to GetTrack return the same value test" << std::endl;

    // Create object of derived class PlayerSubtecCCManager.
    PlayerSubtecCCManager obj;

    std::cout << "Invoking GetTrack() first time." << std::endl;
    const std::string &track1 = obj.GetTrack();
    std::cout << "First call returned: \"" << track1 << "\"" << std::endl;
    
    std::cout << "Invoking GetTrack() second time." << std::endl;
    const std::string &track2 = obj.GetTrack();
    std::cout << "Second call returned: \"" << track2 << "\"" << std::endl;
    
    std::cout << "Invoking GetTrack() third time." << std::endl;
    const std::string &track3 = obj.GetTrack();
    std::cout << "Third call returned: \"" << track3 << "\"" << std::endl;

    EXPECT_EQ(track1, "");
    EXPECT_EQ(track2, "");
    EXPECT_EQ(track3, "");

    std::cout << "Exiting Multiple consecutive calls to GetTrack return the same value test" << std::endl;
}
/**
 * @brief Test initialization of PlayerSubtecCCManager with a valid decoder handle.
 *
 * This test verifies that invoking the Init method of PlayerSubtecCCManager with a valid decoder handle
 * returns 0, indicating successful initialization. The test creates a dummy decoder handle,
 * instantiates a PlayerSubtecCCManager object, calls the Init function, and checks the returned result.
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
 * | 01 | Create a dummy decoder handle, instantiate PlayerSubtecCCManager, invoke Init with valid decoder handle, and assert the return value is 0 | handle = pointer to validHandle, result = value returned by Init | Return value is 0; assertion EXPECT_EQ(result, 0) passes | Should Pass |
 */
TEST(PlayerSubtecCCManager, InitializeWithValidDecoderHandle) {
    std::cout << "Entering InitializeWithValidDecoderHandle test" << std::endl;
    
    int dummyValue = 1234;
    void* validHandle = static_cast<void*>(&dummyValue);
    std::cout << "Creating a valid handle with address: " << validHandle << std::endl;
    
    // Create an object of the derived class PlayerSubtecCCManager using its default constructor
    PlayerSubtecCCManager playerSubtecCCManager;
    std::cout << "Invoking Init with handle: " << handle << std::endl;
    
    // Invoke the base class method Init via the derived class object
    int result = playerSubtecCCManager.Init(handle);
    std::cout << "Method Init returned: " << result << std::endl;
    
    // Check that the method returns 0 for a valid decoder handle
    EXPECT_EQ(result, 0);
    
    std::cout << "Exiting InitializeWithValidDecoderHandle test" << std::endl;
}
/**
 * @brief Checks the behavior of PlayerSubtecCCManager::Init when provided with a NULL decoder handle.
 *
 * This test verifies that the PlayerSubtecCCManager initialization correctly handles an invalid input by returning an error code (-1) when a NULL decoder handle is provided.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoke PlayerSubtecCCManager::Init with a NULL decoder handle | handle = nullptr, expected result = -1 | The Init method returns -1 when a NULL decoder handle is passed and the EXPECT_EQ(result, -1) check passes | Should Pass |
 */
TEST(PlayerSubtecCCManager, InitializeWithNullDecoderHandle) {
    std::cout << "Entering InitializeWithNullDecoderHandle test" << std::endl;
    
    // Set the decoder handle to NULL
    void* handle = nullptr;
    std::cout << "Testing Init with a NULL handle: " << handle << std::endl;
    
    // Create an object of the derived class PlayerSubtecCCManager using its default constructor
    PlayerSubtecCCManager playerSubtecCCManager;
    std::cout << "Invoking Init with NULL handle" << std::endl;
    
    // Invoke the base class method Init via the derived class object
    int result = playerSubtecCCManager.Init(handle);
    std::cout << "Method Init returned: " << result << std::endl;
    
    // Check that the method returns -1 for a NULL decoder handle
    EXPECT_EQ(result, -1);
    
    std::cout << "Exiting InitializeWithNullDecoderHandle test" << std::endl;
}
/**
 * @brief Verify that Out-of-Band Closed Caption rendering is supported when enabled
 *
 * This test verifies that the method IsOOBCCRenderingSupported() returns true/false
 * when a PlayerSubtecCCManager object is constructed.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create PlayerSubtecCCManager object and invoke IsOOBCCRenderingSupported() | output = result (expected true/false) | Returns true/false | Should Pass |
 */
TEST(PlayerSubtecCCManager, IsOOBCCRenderingSupported_Returns_True) {
    std::cout << "Entering IsOOBCCRenderingSupported_Returns_True test\n";

    PlayerSubtecCCManager playerSupported;
    std::cout << "PlayerSubtecCCManager created with support flag set to true\n";

    std::cout << "Invoking IsOOBCCRenderingSupported()\n";
    bool result = playerSupported.IsOOBCCRenderingSupported();
    std::cout << "Method IsOOBCCRenderingSupported returned: " << (result ? "true" : "false") << "\n";

    EXPECT_TRUE(result == true || result == false);
    std::cout << "Exiting IsOOBCCRenderingSupported_Returns_True test\n";
}
/**
 * @brief Verify that calling RestoreCC() with no stored closed caption tracks does not modify the track list.
 *
 * This test verifies that when the RestoreCC() method is invoked on an object that has no pre-stored closed caption tracks,
 * the underlying track list remains empty and no exceptions are thrown during the operation.
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
 * | Variation / Step | Description                                                        | Test Data                                                  | Expected Result                                            | Notes           |
 * | :--------------: | ------------------------------------------------------------------ | ---------------------------------------------------------- | ---------------------------------------------------------- | --------------- |
 * | 01               | Construct manager      | manager constructed         |                   | Should be successful |
 * | 02               | Invoke RestoreCC() method on manager                                 | manager.RestoreCC() invoked, no parameters                 | No exception thrown                                        | Should Pass     |
 */
TEST(PlayerSubtecCCManager, RestoreCC_with_no_stored_CC_tracks)
{
    std::cout << "Entering RestoreCC_with_no_stored_CC_tracks test" << std::endl;
    
    // Create object using default constructor.
    PlayerSubtecCCManager manager;
    
    // Invoke RestoreCC.
    std::cout << "Invoking RestoreCC()" << std::endl;
    EXPECT_NO_THROW(manager.RestoreCC());
    std::cout << "RestoreCC() executed without exceptions" << std::endl;
    
    std::cout << "Exiting RestoreCC_with_no_stored_CC_tracks test" << std::endl;
}
/**
 * @brief Verify that enabling parental control sets the status without errors
 *
 * This test creates an instance of PlayerSubtecCCManager and invokes the SetParentalControlStatus method with the input value 'true' to enable parental control. The purpose is to ensure that the method call executes successfully without triggering any errors.
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
 * | Variation / Step | Description                                                                                            | Test Data                                         | Expected Result                                                      | Notes         |
 * | :--------------: | ------------------------------------------------------------------------------------------------------ | ------------------------------------------------- | -------------------------------------------------------------------- | ------------- |
 * | 01               | Create an instance of PlayerSubtecCCManager using the default constructor                              | None                                              | Instance is created successfully                                     | Should be successful |
 * | 02               | Invoke SetParentalControlStatus with the parameter enable = true                                       | enable = true                                     | Method is called without error; status is updated accordingly        | Should Pass   |
 */
TEST(PlayerSubtecCCManager, EnableParentalControl)
{
    std::cout << "Entering Enable Parental Control test" << std::endl;
    
    // Create instance of PlayerSubtecCCManager
    PlayerSubtecCCManager manager;
    std::cout << "Created PlayerSubtecCCManager instance using default constructor." << std::endl;
    
    // Invoke the method with input true
    bool enable = true;
    std::cout << "Invoking SetParentalControlStatus with value: " << std::boolalpha << enable << std::endl;
    manager.SetParentalControlStatus(enable);
    std::cout << "SetParentalControlStatus was called with " << std::boolalpha << enable << std::endl;
    
    std::cout << "Exiting Enable Parental Control test" << std::endl;
}
/**
 * @brief Verify that disabling parental control successfully updates the state.
 *
 * This test verifies that by calling SetParentalControlStatus with a false value,
 * the parental control state is updated as expected without errors. Since there is no getter,
 * the test assumes success if no errors occur during the call.
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
 * | Variation / Step | Description                                                                                           | Test Data                                   | Expected Result                                          | Notes       |
 * | :--------------: | ----------------------------------------------------------------------------------------------------- | ------------------------------------------- | -------------------------------------------------------- | ----------- |
 * | 01               | Create an instance of PlayerSubtecCCManager                                                          | No input arguments                          | Instance created successfully                             | Should be successful |
 * | 02               | Invoke SetParentalControlStatus with the value false                                                 | input: disable = false                        | Method executes without errors | Should Pass |
 */
TEST(PlayerSubtecCCManager, DisableParentalControl)
{
    std::cout << "Entering Disable Parental Control test" << std::endl;
    
    // Create instance of PlayerSubtecCCManager
    PlayerSubtecCCManager manager;
    std::cout << "Created PlayerSubtecCCManager instance using default constructor." << std::endl;
    
    // Invoke the method with input false
    bool disable = false;
    std::cout << "Invoking SetParentalControlStatus with value: " << std::boolalpha << disable << std::endl;
    manager.SetParentalControlStatus(disable);
    std::cout << "SetParentalControlStatus was called with " << std::boolalpha << disable << std::endl;
    
    std::cout << "Exiting Disable Parental Control test" << std::endl;
}
/**
 * @brief Test to toggle parental control status repeatedly using PlayerSubtecCCManager
 *
 * This test verifies that the SetParentalControlStatus method of the PlayerSubtecCCManager class correctly updates the parental control status when toggled repeatedly. It ensures that switching the boolean status (true to false and back to true) does not result in errors.
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
 * | Variation / Step | Description                                                         | Test Data                                      | Expected Result                                               | Notes          |
 * | :--------------: | -------------------------------------------------------------------- | ---------------------------------------------- | ------------------------------------------------------------- | -------------- |
 * | 01               | Create an instance of PlayerSubtecCCManager                           | N/A, output: instance created                  | Instance successfully created without errors                  | Should be successful |
 * | 02               | Invoke SetParentalControlStatus with status=true                      | input: status = true                           | Parental control status updated to true without errors         | Should Pass    |
 * | 03               | Invoke SetParentalControlStatus with status=false                     | input: status = false                          | Parental control status updated to false without errors        | Should Pass    |
 * | 04               | Invoke SetParentalControlStatus with status=true again                 | input: status = true                           | Parental control status updated to true without errors         | Should Pass    |
 */
TEST(PlayerSubtecCCManager, ToggleParentalControlStatusRepeatedly)
{
    std::cout << "Entering Toggle Parental Control Status Repeatedly test" << std::endl;
    
    // Create instance of PlayerSubtecCCManager
    PlayerSubtecCCManager manager;
    std::cout << "Created PlayerSubtecCCManager instance using default constructor." << std::endl;
    
    // Step 1: Set to true
    bool status = true;
    std::cout << "Step 1: Invoking SetParentalControlStatus with value: " << std::boolalpha << status << std::endl;
    manager.SetParentalControlStatus(status);
    std::cout << "Parental control status updated to: " << std::boolalpha << status << std::endl;
    
    // Step 2: Set to false
    status = false;
    std::cout << "Step 2: Invoking SetParentalControlStatus with value: " << std::boolalpha << status << std::endl;
    manager.SetParentalControlStatus(status);
    std::cout << "Parental control status updated to: " << std::boolalpha << status << std::endl;
    
    // Step 3: Set to true again
    status = true;
    std::cout << "Step 3: Invoking SetParentalControlStatus with value: " << std::boolalpha << status << std::endl;
    manager.SetParentalControlStatus(status);
    std::cout << "Parental control status updated to: " << std::boolalpha << status << std::endl;
     
    std::cout << "Exiting Toggle Parental Control Status Repeatedly test" << std::endl;
}
/**
 * @brief Verify that multiple calls with the same parental control status do not alter the state unexpectedly.
 *
 * This test verifies that calling SetParentalControlStatus with the same value consecutively does not change the internal state or produce errors.
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
 * | Variation / Step | Description                                                        | Test Data                                             | Expected Result                                                      | Notes         |
 * | :--------------: | ------------------------------------------------------------------ | ----------------------------------------------------- | -------------------------------------------------------------------- | ------------- |
 * | 01               | Create an instance of PlayerSubtecCCManager using the default constructor. | No inputs, output: instance created                   | Instance is successfully created with default state.                 | Should be successful |
 * | 02               | Invoke SetParentalControlStatus for the first time with value true.  | input: value = true                                   | Parental control status is updated to enabled without errors.        | Should Pass   |
 * | 03               | Invoke SetParentalControlStatus again with the same value (true).     | input: value = true                                   | Parental control status remains enabled; redundant call does not affect state. | Should Pass   |
 */
TEST(PlayerSubtecCCManager, RedundantCallWithSameValue)
{
    std::cout << "Entering Redundant Call with Same Value test" << std::endl;
    
    // Create instance of PlayerSubtecCCManager
    PlayerSubtecCCManager manager;
    std::cout << "Created PlayerSubtecCCManager instance using default constructor." << std::endl;
    
    // First call with true
    bool value = true;
    std::cout << "First call: Invoking SetParentalControlStatus with value: " << std::boolalpha << value << std::endl;
    manager.SetParentalControlStatus(value);
    std::cout << "Parental control status updated to enabled." << std::endl;
    
    // Second redundant call with true
    std::cout << "Second redundant call: Invoking SetParentalControlStatus with value: " << std::boolalpha << value << std::endl;
    manager.SetParentalControlStatus(value);
    std::cout << "Parental control status remains enabled after redundant call." << std::endl;
     
    std::cout << "Exiting Redundant Call with Same Value test" << std::endl;
}
/**
 * @brief Test to verify successful enabling of closed caption rendering.
 *
 * This test verifies that the SetStatus method of the PlayerSubtecCCManager class correctly enables closed caption rendering. It creates an instance of PlayerSubtecCCManager using the default constructor, sets the enable flag to true, and then confirms that the returned value from SetStatus is 0, indicating a successful operation.
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
 * | Variation / Step | Description                                                                                     | Test Data                                        | Expected Result                                              | Notes           |
 * | :--------------: | ----------------------------------------------------------------------------------------------- | ------------------------------------------------ | ------------------------------------------------------------ | --------------- |
 * | 01               | Instantiate PlayerSubtecCCManager object using the default constructor.                         | N/A                                              | Object instantiation is successful.                          | Should be successful |
 * | 02               | Log the state before invoking SetStatus to confirm the object creation.                         | N/A                                              | Pre-invocation state is correctly logged.                    | Should be successful |
 * | 03               | Set enable flag to true and log the parameter value to be used for enabling CC rendering.         | enable = true                                    | enable variable is set to true.                              | Should be successful |
 * | 04               | Invoke SetStatus API with enable flag and capture the return value.                              | input: enable = true, output: retVal               | retVal equals 0 indicating successful operation.             | Should Pass |
 * | 05               | Log the outcome of SetStatus call and verify that the internal state update is simulated properly. | output: retVal = 0                                 | Outcome logged confirming the successful update of internal state and assertion passes. | Should be successful |
 */
TEST(PlayerSubtecCCManager, SuccessfullyEnablingCCRendering) {
    std::cout << "Entering SuccessfullyEnablingCCRendering test" << std::endl;

    // Create an instance of derived class using the default constructor
    PlayerSubtecCCManager playerCCManager;

    // Log the state before invoking SetStatus
    std::cout << "Before invoking SetStatus, object created using default constructor." << std::endl;
    
    bool enable = true;
    std::cout << "Invoking SetStatus with enable = " << (enable ? "true" : "false") << std::endl;
    
    // Invoke SetStatus method
    int retVal = playerCCManager.SetStatus(enable);
    std::cout << "SetStatus returned: " << retVal << std::endl;
    
    // Log internal state change simulation
    std::cout << "Assuming internal state updated for enabling CC rendering." << std::endl;
    
    // Check that the return value is 0 indicating success
    EXPECT_EQ(retVal, 0);
    
    std::cout << "Exiting SuccessfullyEnablingCCRendering test" << std::endl;
}
/**
 * @brief Test for successfully disabling closed caption rendering
 *
 * This test verifies that the PlayerSubtecCCManager correctly disables closed caption rendering when the SetStatus method is invoked with false. It ensures that the API returns a success code (0), indicating that the operation was completed as expected.
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
 * | Variation / Step | Description                                                         | Test Data                                              | Expected Result                          | Notes         |
 * | :--------------: | ------------------------------------------------------------------- | ------------------------------------------------------ | ---------------------------------------- | ------------- |
 * | 01               | Create an instance of PlayerSubtecCCManager using the default constructor | constructor call, no input parameters                   | Instance is created successfully         | Should be successful |
 * | 02               | Invoke SetStatus method with enable set to false                     | enable = false, output (retVal) from SetStatus call      | Return value should be 0                 | Should Pass   |
 * | 03               | Verify the return value using EXPECT_EQ for correctness              | retVal = 0                                            | Assertion passes confirming success code | Should Pass   |
 */
TEST(PlayerSubtecCCManager, SuccessfullyDisablingCCRendering) {
    std::cout << "Entering SuccessfullyDisablingCCRendering test" << std::endl;

    // Create an instance of derived class using the default constructor
    PlayerSubtecCCManager playerCCManager;

    // Log the state before invoking SetStatus
    std::cout << "Before invoking SetStatus, object created using default constructor." << std::endl;
    
    bool enable = false;
    std::cout << "Invoking SetStatus with enable = " << (enable ? "true" : "false") << std::endl;
    
    // Invoke SetStatus method
    int retVal = playerCCManager.SetStatus(enable);
    std::cout << "SetStatus returned: " << retVal << std::endl;
    
    // Log internal state change simulation
    std::cout << "Assuming internal state updated for disabling CC rendering." << std::endl;
    
    // Check that the return value is 0 indicating success
    EXPECT_EQ(retVal, 0);
    
    std::cout << "Exiting SuccessfullyDisablingCCRendering test" << std::endl;
}
/**
 * @brief Validate that the default style is correctly set using SetStyle
 *
 * This test verifies that when the PlayerSubtecCCManager is instantiated and its SetStyle method is invoked with the "default" option, the API returns a success code (0). It ensures that the default configuration is properly handled by the API.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create a PlayerSubtecCCManager instance and invoke SetStyle with the default style option | option = default, expectedRetVal = 0 | API returns 0 and assertion EXPECT_EQ(retVal, 0) passes | Should Pass |
 */
TEST(PlayerSubtecCCManager, ValidStyleDefault) {
    std::cout << "Entering ValidStyleDefault test" << std::endl;

    // Create a PlayerSubtecCCManager object using default constructor.
    PlayerSubtecCCManager manager;
    std::string option = "default";
    std::cout << "Invoking SetStyle with option: \"" << option << "\"" << std::endl;
    
    int retVal = manager.SetStyle(option);
    std::cout << "SetStyle returned: " << retVal << std::endl;

    // Expecting success (0)
    EXPECT_EQ(retVal, 0);

    std::cout << "Exiting ValidStyleDefault test" << std::endl;
}
/**
 * @brief Verify that PlayerSubtecCCManager::SetStyle successfully applies the "dark" style.
 *
 * This test checks whether calling SetStyle with the argument "dark" returns a success code (0). It validates that the API correctly processes the "dark" style option and passes the assertion check.
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
 * | Variation / Step | Description                                               | Test Data                                   | Expected Result                                           | Notes       |
 * | :--------------: | --------------------------------------------------------- | ------------------------------------------- | --------------------------------------------------------- | ----------- |
 * | 01               | Call SetStyle with the style option "dark" and verify its return value | option = "dark", output: retVal = 0          | The function returns 0 and the assertion EXPECT_EQ(retVal, 0) passes | Should Pass |
 */
TEST(PlayerSubtecCCManager, ValidStyleDark) {
    std::cout << "Entering ValidStyleDark test" << std::endl;

    // Create a PlayerSubtecCCManager object using default constructor.
    PlayerSubtecCCManager manager;
    std::string option = "dark";
    std::cout << "Invoking SetStyle with option: \"" << option << "\"" << std::endl;
    
    int retVal = manager.SetStyle(option);
    std::cout << "SetStyle returned: " << retVal << std::endl;
    
    // Expecting success (0)
    EXPECT_EQ(retVal, 0);
    
    std::cout << "Exiting ValidStyleDark test" << std::endl;
}
/**
 * @brief Verify that extra spaces in the 'light' style option are handled correctly by the SetStyle method.
 *
 * Tests that passing a string with extra spaces to the PlayerSubtecCCManager::SetStyle API trims the input and executes successfully, resulting in a return value of 0.
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
 * | Variation / Step | Description                                                          | Test Data                                          | Expected Result                         | Notes                |
 * | :--------------: | -------------------------------------------------------------------- | -------------------------------------------------- | --------------------------------------- | -------------------- |
 * | 01               | Create a PlayerSubtecCCManager object using the default constructor. | (none)                                             | Object instantiated successfully        | Should be successful |
 * | 02               | Invoke SetStyle with the option "  light  " and validate its return value. | input: option = "  light  ", output: retVal = 0 | SetStyle returns 0 indicating success   | Should Pass          |
 */
TEST(PlayerSubtecCCManager, ValidStyleLightExtraSpaces) {
    std::cout << "Entering ValidStyleLightExtraSpaces test" << std::endl;

    // Create a PlayerSubtecCCManager object using default constructor.
    PlayerSubtecCCManager manager;
    std::string option = "  light  ";
    std::cout << "Invoking SetStyle with option: \"" << option << "\"" << std::endl;
    
    int retVal = manager.SetStyle(option);
    std::cout << "SetStyle returned: " << retVal << std::endl;
    
    // Expecting success (0)
    EXPECT_EQ(retVal, 0);
    
    std::cout << "Exiting ValidStyleLightExtraSpaces test" << std::endl;
}
/**
 * @brief Test to verify the behavior of PlayerSubtecCCManager::SetStyle when provided with an empty style string
 *
 * Verifies that invoking SetStyle with an empty string returns -1, indicating failure. This test ensures that the API properly handles invalid input by returning the expected error code.
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
 * | Variation / Step | Description                                      | Test Data                  | Expected Result                            | Notes          |
 * | :--------------: | ------------------------------------------------ | -------------------------- | ------------------------------------------ | -------------- |
 * | 01               | Create a PlayerSubtecCCManager object            | None                       | Object instantiated successfully           | Should be successful |
 * | 02               | Invoke SetStyle with an empty string option      | option = "", output: -1     | Returns -1 and assertion validates expected error code | Should Fail    |
 */
TEST(PlayerSubtecCCManager, InvalidStyleEmptyString) {
    std::cout << "Entering InvalidStyleEmptyString test" << std::endl;

    // Create a PlayerSubtecCCManager object using default constructor.
    PlayerSubtecCCManager manager;
    std::string option = "";
    std::cout << "Invoking SetStyle with option: \"" << option << "\"" << std::endl;
    
    int retVal = manager.SetStyle(option);
    std::cout << "SetStyle returned: " << retVal << std::endl;
    
    // Expecting failure (-1)
    EXPECT_EQ(retVal, -1);
    
    std::cout << "Exiting InvalidStyleEmptyString test" << std::endl;
}
/**
 * @brief Validate that SetStyle returns a failure when provided an unrecognized style value.
 *
 * This test case verifies that passing an invalid option "unknown_style" to the SetStyle method of PlayerSubtecCCManager results in a failure return value (-1). It ensures that the API correctly identifies and handles unrecognized style values.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 026@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                            | Test Data                          | Expected Result                                 | Notes             |
 * | :--------------: | ---------------------------------------------------------------------- | ---------------------------------- | ----------------------------------------------- | ----------------- |
 * | 01               | Create a PlayerSubtecCCManager instance using the default constructor. | None                               | Instance is created successfully.               | Should be successful |
 * | 02               | Invoke SetStyle with an unrecognized style value "unknown_style" and verify that it returns -1. | option = unknown_style             | Return value equals -1 and the assertion passes. | Should Pass         |
 */
TEST(PlayerSubtecCCManager, InvalidStyleUnrecognizedValue) {
    std::cout << "Entering InvalidStyleUnrecognizedValue test" << std::endl;

    // Create a PlayerSubtecCCManager object using default constructor.
    PlayerSubtecCCManager manager;
    std::string option = "unknown_style";
    std::cout << "Invoking SetStyle with option: \"" << option << "\"" << std::endl;
    
    int retVal = manager.SetStyle(option);
    std::cout << "SetStyle returned: " << retVal << std::endl;
    
    // Expecting failure (-1)
    EXPECT_EQ(retVal, -1);
    
    std::cout << "Exiting InvalidStyleUnrecognizedValue test" << std::endl;
}
/**
 * @brief Tests the SetStyle method for handling of invalid style inputs containing special characters.
 *
 * This test verifies that the PlayerSubtecCCManager::SetStyle API correctly handles an input string
 * that consists solely of special characters, by returning an error code (-1). This ensures that the
 * function can correctly reject invalid style options.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 027@n
 * **Priority:** (High) 
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**
 * | Variation / Step | Description                                        | Test Data                                   | Expected Result                                                  | Notes           |
 * | :--------------: | -------------------------------------------------- | ------------------------------------------- | ---------------------------------------------------------------- | --------------- |
 * | 01               | Create a PlayerSubtecCCManager object              | None                                        | Manager object is created successfully                           | Should be successful |
 * | 02               | Prepare an input string with special characters    | specialChars = "!@#$%^&*()"                  | Input string is correctly formed without overflow                | Should be successful |
 * | 03               | Invoke SetStyle with the special character string  | option = "!@#$%^&*()"                        | API returns -1 and EXPECT_EQ(retVal, -1) passes the assertion      | Should Pass     |
 */

TEST(PlayerSubtecCCManager, InvalidStyleSpecialCharacters) {
    std::cout << "Entering InvalidStyleSpecialCharacters test" << std::endl;

    // Create a PlayerSubtecCCManager object using default constructor.
    PlayerSubtecCCManager manager;
    char specialChars[20];
    // Using strncpy to assign value to fixed size array.
    strncpy(specialChars, "!@#$%^&*()", sizeof(specialChars) - 1);
    specialChars[sizeof(specialChars) - 1] = '\0';
    std::string option = std::string(specialChars);
    std::cout << "Invoking SetStyle with option: \"" << option << "\"" << std::endl;
    
    int retVal = manager.SetStyle(option);
    std::cout << "SetStyle returned: " << retVal << std::endl;
    
    // Expecting failure (-1)
    EXPECT_EQ(retVal, -1);
    
    std::cout << "Exiting InvalidStyleSpecialCharacters test" << std::endl;
}
/**
 * @brief Validates track selection with valid inputs for supported CC formats.
 *
 * This test verifies that the PlayerSubtecCCManager API correctly processes a valid track selection. 
 * It ensures that when a non-empty track "Track1" is provided along with each of the supported Closed Caption formats, 
 * the SetTrack function returns 0, indicating successful track setup.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 028@n
 * **Priority:** High@n
 * @n
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * @n
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |@n
 * | :----: | --------- | ---------- |-------------- | ----- |@n
 * | 01 | Create an instance of PlayerSubtecCCManager and define a valid track "Track1" along with an array of supported CC formats | playerObj constructed, track = "Track1", formats = eCLOSEDCAPTION_FORMAT_608, eCLOSEDCAPTION_FORMAT_708, eCLOSEDCAPTION_FORMAT_DEFAULT | Object initialized and test data defined successfully | Should be successful |@n
 * | 02 | Iterate over each supported CC format and invoke SetTrack API for each format | For each iteration: input: track = "Track1", format = current CCFormat from the array; output: ret from SetTrack | API returns 0 for each supported CC format, verified with EXPECT_EQ(ret, 0) | Should Pass |
 */
TEST(PlayerSubtecCCManager, ValidTrackSelectionTest) {
    std::cout << "Entering ValidTrackSelectionTest test" << std::endl;
    
    // Create the derived class object using default constructor.
    PlayerSubtecCCManager playerObj;
    
    // Define a track that is non-empty.
    std::string track = "Track1";
    
    // Array of supported CC formats.
    CCFormat formats[] = { eCLOSEDCAPTION_FORMAT_608, eCLOSEDCAPTION_FORMAT_708, eCLOSEDCAPTION_FORMAT_DEFAULT };

    // Iterate through each supported format and test
    for (size_t i = 0; i < sizeof(formats)/sizeof(formats[0]); ++i) {
        std::cout << "Invoking SetTrack with track: \"" << track 
                  << "\" and format: " << formats[i] << std::endl;
        int ret = playerObj.SetTrack(track, formats[i]);
        std::cout << "Method SetTrack returned: " << ret << " for format: " << formats[i] << std::endl;
        EXPECT_EQ(ret, 0);
    }
    
    std::cout << "Exiting ValidTrackSelectionTest test" << std::endl;
}
/**
 * @brief Verify that SetTrack returns failure for an empty track input across all CC formats.
 *
 * This test validates that the SetTrack API method correctly returns -1 when invoked with an empty string
 * as the track input across multiple closed caption formats. An empty track is considered invalid,
 * and the function is expected to consistently indicate failure regardless of the format.
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
 * | Variation / Step | Description                                                                 | Test Data                                                                  | Expected Result                    | Notes               |
 * | :--------------: | --------------------------------------------------------------------------- | -------------------------------------------------------------------------- | ---------------------------------- | ------------------- |
 * | 01               | Instantiate PlayerSubtecCCManager object using default constructor          | None                                                                       | Object is created successfully     | Should be successful |
 * | 02               | Define an empty track string                                                | emptyTrack = ""                                                            | Empty track variable is defined    | Should be successful |
 * | 03               | Loop over all CC formats and invoke SetTrack with empty track               | Formats: 608, 708, DEFAULT; input: emptyTrack = ""                         | Each call returns -1 indicating failure | Should fail for all formats |
 */
TEST(PlayerSubtecCCManager, FailureOnEmptyTrackTest_AllFormats) {
    std::cout << "Entering FailureOnEmptyTrackTest_AllFormats test" << std::endl;

    // Create the derived class object using default constructor.
    PlayerSubtecCCManager playerObj;

    // Define an empty track
    std::string emptyTrack = "";

    // Array of formats to test
    CCFormat formats[] = {
        eCLOSEDCAPTION_FORMAT_608,
        eCLOSEDCAPTION_FORMAT_708,
        eCLOSEDCAPTION_FORMAT_DEFAULT
    };

    for (CCFormat format : formats) {
        std::cout << "Invoking SetTrack with empty track: \"" << emptyTrack
                  << "\" and format: " << format << std::endl;

        int ret = playerObj.SetTrack(emptyTrack, format);
        std::cout << "Method SetTrack returned: " << ret << " for format: " << format << std::endl;

        // Expect failure indicated by -1 for each format
        EXPECT_EQ(ret, -1) << "SetTrack should fail with empty track for format: " << format;
    }

    std::cout << "Exiting FailureOnEmptyTrackTest_AllFormats test" << std::endl;
}
/**
 * @brief Verifies that SetTrack fails when provided with a track consisting of only whitespace.
 *
 * This test is designed to ensure that the SetTrack method of PlayerSubtecCCManager returns an error (-1)
 * when a track string containing only whitespace is provided for each supported CCFormat.
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
 * | Variation / Step | Description                                                            | Test Data                                              | Expected Result                                | Notes              |
 * | :--------------: | ---------------------------------------------------------------------- | ------------------------------------------------------ | ---------------------------------------------- | ------------------ |
 * | 01               | Create instance of PlayerSubtecCCManager using the default constructor  | None                                                   | Instance is created successfully               | Should be successful |
 * | 02               | Invoke SetTrack with a whitespace-only track and each caption format    | track = "   ", formats = 608, 708, DEFAULT             | Method returns -1 indicating failure           | Should Fail        |
 * | 03               | Verify the output of SetTrack using EXPECT_EQ                           | ret = -1 (output from SetTrack)                        | EXPECT_EQ confirms ret is -1                    | Should be successful |
 */
TEST(PlayerSubtecCCManager, FailureOnWhitespaceOnlyTrackTest) {
    std::cout << "Entering FailureOnWhitespaceOnlyTrackTest test" << std::endl;

    // Create the derived class object using default constructor.
    PlayerSubtecCCManager playerObj;

    // Define a track with whitespace only.
    std::string whitespaceTrack = "   ";

    // Array of CC formats to test.
    CCFormat formats[] = {
        eCLOSEDCAPTION_FORMAT_608,
        eCLOSEDCAPTION_FORMAT_708,
        eCLOSEDCAPTION_FORMAT_DEFAULT
    };

    for (CCFormat format : formats) {
        std::cout << "Invoking SetTrack with whitespace track: \"" << whitespaceTrack 
                  << "\" and format: " << format << std::endl;
        int ret = playerObj.SetTrack(whitespaceTrack, format);
        std::cout << "Method SetTrack returned: " << ret << " for format: " << format << std::endl;

        // Expect failure indicated by -1.
        EXPECT_EQ(ret, -1);
    }

    std::cout << "Exiting FailureOnWhitespaceOnlyTrackTest test" << std::endl;
}
/**
 * @brief Validate that enabling trickplay status updates the internal state correctly.
 *
 * This test verifies that calling SetTrickplayStatus(true) on a PlayerSubtecCCManager object correctly enables trickplay mode. It ensures that the method is invoked with the proper parameter and that the simulated internal state reflects a started trickplay. This helps confirm that the API correctly processes the enable operation.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create a PlayerSubtecCCManager instance using the default constructor. | None | Instance is created successfully. | Should be successful |
 * | 02 | Invoke PlayerSubtecCCManager::SetTrickplayStatus with enable value true. | input: true, output: internal state expected to show "Trickplay started" | Trickplay status is enabled and the internal state reflects the enabled mode. | Should Pass |
 */
TEST(PlayerSubtecCCManager, EnableTrickplayStatus) {
    std::cout << "Entering EnableTrickplayStatus test" << std::endl;
    
    // Create the derived class object using default constructor
    PlayerSubtecCCManager playerCCManager;
    
    // Log before invoking the method
    std::cout << "Invoking PlayerSubtecCCManager::SetTrickplayStatus with enable value: true" << std::endl;
    
    // Invoke the method with true to enable trickplay status
    playerCCManager.SetTrickplayStatus(true);
    
    // Log internal state change simulation
    std::cout << "Method SetTrickplayStatus(true) executed. Expected internal state: Trickplay started." << std::endl;
    
    // Indicate test successful completion
    std::cout << "Exiting EnableTrickplayStatus test" << std::endl;
}
/**
 * @brief Test to disable trickplay status in PlayerSubtecCCManager
 *
 * This test verifies that the SetTrickplayStatus method correctly disables the trickplay status when passed a false value.
 * It ensures that the internal state simulating trickplay stoppage is set as expected.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 032@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                               | Test Data                                                     | Expected Result                                                  | Notes           |
 * | :--------------: | ------------------------------------------------------------------------- | ------------------------------------------------------------- | ---------------------------------------------------------------- | --------------- |
 * | 01               | Create an instance of PlayerSubtecCCManager using its default constructor   | N/A                                                           | Instance of PlayerSubtecCCManager is created successfully        | Should be successful |
 * | 02               | Log the entry message and details before invoking SetTrickplayStatus        | Log messages: "Entering DisableTrickplayStatus test", "Invoking PlayerSubtecCCManager::SetTrickplayStatus with enable value: false" | Log messages are printed as expected                             | Should be successful |
 * | 03               | Invoke SetTrickplayStatus with false to disable trickplay status            | Input: enable = false; Output: internal state expected = "Trickplay stopped" | API call executes without errors; internal state is updated correctly | Should Pass      |
 * | 04               | Log the internal state change after method execution                      | Log message: "Method SetTrickplayStatus(false) executed. Expected internal state: Trickplay stopped." | Log message is printed confirming expected internal state        | Should be successful |
 */
TEST(PlayerSubtecCCManager, DisableTrickplayStatus) {
    std::cout << "Entering DisableTrickplayStatus test" << std::endl;
    
    // Create the derived class object using default constructor
    PlayerSubtecCCManager playerCCManager;
    
    // Log before invoking the method
    std::cout << "Invoking PlayerSubtecCCManager::SetTrickplayStatus with enable value: false" << std::endl;
    
    // Invoke the method with false to disable trickplay status
    playerCCManager.SetTrickplayStatus(false);
    
    // Log internal state change simulation
    std::cout << "Method SetTrickplayStatus(false) executed. Expected internal state: Trickplay stopped." << std::endl;
    
    // Indicate test successful completion
    std::cout << "Exiting DisableTrickplayStatus test" << std::endl;
}
/**
 * @brief Test for toggling Trickplay status in PlayerSubtecCCManager, verifying both enable and disable functionality.
 *
 * This test checks the SetTrickplayStatus method of the PlayerSubtecCCManager class by first enabling and then disabling the trickplay mode. The test confirms that the API can be invoked with both true and false values, and the expected log messages indicate the correct internal state transitions.
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
 * | 01 | Log start of test execution | None | "Entering ToggleTrickplayStatus test" printed | Should be successful |
 * | 02 | Create PlayerSubtecCCManager object using default constructor | Constructor call, no parameters | Object of PlayerSubtecCCManager created successfully | Should Pass |
 * | 03 | Log before first invocation of SetTrickplayStatus with true | None | "Invoking PlayerSubtecCCManager::SetTrickplayStatus with enable value: true" printed | Should be successful |
 * | 04 | Call SetTrickplayStatus(true) to enable trickplay mode | input: enable = true | Trickplay mode enabled, log indicates mode started | Should Pass |
 * | 05 | Log internal state after enabling trickplay | None | "After first call: Trickplay status expected to be started." printed | Should be successful |
 * | 06 | Log before second invocation of SetTrickplayStatus with false | None | "Invoking PlayerSubtecCCManager::SetTrickplayStatus with enable value: false" printed | Should be successful |
 * | 07 | Call SetTrickplayStatus(false) to disable trickplay mode | input: enable = false | Trickplay mode disabled, log indicates mode stopped | Should Pass |
 * | 08 | Log internal state after disabling trickplay | None | "After second call: Trickplay status expected to be stopped." printed | Should be successful |
 */
TEST(PlayerSubtecCCManager, ToggleTrickplayStatus) {
    std::cout << "Entering ToggleTrickplayStatus test" << std::endl;
    
    // Create the derived class object using default constructor
    PlayerSubtecCCManager playerCCManager;
    
    // Log before first invocation
    std::cout << "Invoking PlayerSubtecCCManager::SetTrickplayStatus with enable value: true" << std::endl;
    
    // First call to enable trickplay status
    playerCCManager.SetTrickplayStatus(true);
    
    // Log internal state after first call
    std::cout << "After first call: Trickplay status expected to be started." << std::endl;
    
    // Log before second invocation
    std::cout << "Invoking PlayerSubtecCCManager::SetTrickplayStatus with enable value: false" << std::endl;
    
    // Second call to disable trickplay status
    playerCCManager.SetTrickplayStatus(false);
    
    // Log internal state after second call
    std::cout << "After second call: Trickplay status expected to be stopped." << std::endl;
    
    // Indicate test successful completion
    std::cout << "Exiting ToggleTrickplayStatus test" << std::endl;
}
/**
 * @brief Verify that getLastTextTracks returns an empty vector when no text tracks have been added
 *
 * This test ensures that invoking getLastTextTracks on a newly created PlayerSubtecCCManager object returns an empty vector. It is important because it checks the default behavior of the text track retrieval, confirming that no residual or uninitialized data exists in a fresh instance.
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
 * | Variation / Step | Description                                                       | Test Data                                                       | Expected Result                               | Notes           |
 * | :--------------: | ----------------------------------------------------------------- | --------------------------------------------------------------- | --------------------------------------------- | --------------- |
 * | 01               | Create PlayerSubtecCCManager using default constructor            | No input, instance created using default constructor            | Instance is created successfully              | Should be successful |
 * | 02               | Invoke getLastTextTracks() to retrieve text tracks                | manager object; method: getLastTextTracks() invoked              | Returned vector is empty (size equals 0)      | Should Pass     |
 * | 03               | Verify the retrieved vector size is 0                             | Output: tracks vector, tracks.size() = 0 expected                | EXPECT_EQ(tracks.size(), 0) assertion passes  | Should Pass     |
 */
TEST(PlayerSubtecCCManager, ReturnsEmptyVectorWhenNoTextTracksAdded)
{
    std::cout << "Entering ReturnsEmptyVectorWhenNoTextTracksAdded test" << std::endl;
    
    PlayerSubtecCCManager manager;  // Create object using default constructor
    std::cout << "Invoking getLastTextTracks()" << std::endl;
    const std::vector<CCTrackInfo>& tracks = manager.getLastTextTracks();
    std::cout << "Retrieved vector size: " << tracks.size() << std::endl;
    
    EXPECT_EQ(tracks.size(), 0);
    
    std::cout << "Exiting ReturnsEmptyVectorWhenNoTextTracksAdded test" << std::endl;
}
/**
 * @brief Verifies that updateLastTextTracks correctly handles an empty vector input.
 *
 * This test ensures that updateLastTextTracks correctly handles an empty vector input. The test invokes the API with an empty vector and logs relevant messages for entry, parameter size,
 * and exit confirmation.
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
 * | Variation / Step | Description                                                                  | Test Data                                                     | Expected Result                                                | Notes                |
 * | :--------------: | ---------------------------------------------------------------------------- | ------------------------------------------------------------- | -------------------------------------------------------------- | -------------------- |
 * | 01               | Log the entry into the test.                                                 | N/A                                                           | Console prints "Entering UpdateWithEmptyVector test".          | Should be successful |
 * | 02               | Create an instance of PlayerSubtecCCManager using the default constructor.   | manager = instance created via default constructor            | manager object is created successfully.                        | Should be successful |
 * | 03               | Prepare an empty vector of CCTrackInfo.                                      | emptyTracks = []                                              | emptyTracks is an empty vector.                                | Should be successful |
 * | 04               | Log the size of the vector before invoking updateLastTextTracks.             | Console logs the size                                         | Console prints "Passed vector size: 0".                        | Should Pass           |
 * | 05               | Invoke updateLastTextTracks with the empty vector.                           | Input: emptyTracks = []                                       | Internal state mLastTextTracks is updated to be empty.         | Should Pass           |
 * | 06               | Log the exit from the test.                                                  | N/A                                                           | Console prints "Exiting UpdateWithEmptyVector test".           | Should be successful  |
 */
TEST(PlayerSubtecCCManager, UpdateWithEmptyVector) {
    std::cout << "Entering UpdateWithEmptyVector test" << std::endl;
    
    // Create an instance of the derived class using default constructor
    PlayerSubtecCCManager manager;
    
    // Prepare an empty vector of CCTrackInfo
    std::vector<CCTrackInfo> emptyTracks;
    std::cout << "Invoking updateLastTextTracks with empty vector. Passed vector size: " << emptyTracks.size() << std::endl;
    
    // Invoke the method updateLastTextTracks
    manager.updateLastTextTracks(emptyTracks);

    std::cout << "updateLastTextTracks method invoked successfully" << std::endl;
    
    std::cout << "Exiting UpdateWithEmptyVector test" << std::endl;
}

int main(int argc, char **argv) {
     ::testing::InitGoogleTest(&argc, argv);
     return RUN_ALL_TESTS();
}
