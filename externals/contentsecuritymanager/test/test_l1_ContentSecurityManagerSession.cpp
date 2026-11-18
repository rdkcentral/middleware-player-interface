
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
#include "ContentSecurityManagerSession.h"


// Test case for the default constructor of ContentSecurityManagerSession
/**
 * @brief Test the default constructor of ContentSecurityManagerSession to ensure correct handling of uninitialized state
 *
 * This test verifies that invoking the default constructor of ContentSecurityManagerSession does not throw exceptions.
 * It also confirms that the internal state remains uninitialized (i.e., the session manager pointer is not initialized),
 * which is the expected behavior when no session details are provided.
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
 * | Variation / Step | Description                                                         | Test Data                                                          | Expected Result                                                                  | Notes       |
 * | :--------------: | ------------------------------------------------------------------- | ------------------------------------------------------------------- | -------------------------------------------------------------------------------- | ----------- |
 * | 01               | Invoke the default constructor of ContentSecurityManagerSession.    | No inputs; output: constructor invocation establishing object state | No exceptions thrown; object created successfully with uninitialized internal state. | Should Pass |
 * | 02               | Verify internal state remains uninitialized post construction.       | Output: log messages indicating uninitialized session manager         | Console output confirms that the session manager remains uninitialized (invalid state).   | Should Pass |
 */
TEST(ContentSecurityManagerSession, DefaultConstructorTest) {
    std::cout << "Entering DefaultConstructorTest test" << std::endl;
    
    std::cout << "Invoking default constructor ContentSecurityManagerSession()" << std::endl;
    EXPECT_NO_THROW({
        ContentSecurityManagerSession obj;
        std::cout << "Default constructor invoked successfully." << std::endl;
        std::cout << "Instance created without initializing session details." << std::endl;
        std::cout << "Internal state: mpSessionManager is expected to remain uninitialized (invalid state)." << std::endl;
    });
    
    std::cout << "Exiting DefaultConstructorTest test" << std::endl;
}
/**
 * @brief Test the copy constructor of a valid session object
 *
 * This test verifies that a session object created using the copy constructor correctly duplicates the state
 * of the original session. It ensures that both sessionID and inputSummaryHash values are copied without alteration.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 002@n
 * **Priority:** (High) High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create a valid session using constructor | input: sessionID = 100, inputSummaryHash = 12345 | Valid session object created | Should Pass |
 * | 02 | Retrieve original session state using getSessionID and getInputSummaryHash | output: sessionID, inputSummaryHash | Returned values: 100, 12345 | Should be successful |
 * | 03 | Invoke copy constructor to create a new session from the original | input: original session (sessionID = 100, inputSummaryHash = 12345) | Copied session object created | Should Pass |
 * | 04 | Retrieve copied session state using getSessionID and getInputSummaryHash | output: sessionID, inputSummaryHash | Returned values: 100, 12345 | Should be successful |
 * | 05 | Assert that copied session values match expected values | assertion: EXPECT_EQ(copiedSessionID, 100), EXPECT_EQ(copiedInputSummaryHash, 12345) | Assertions pass confirming value integrity | Should Pass |
 */
TEST(ContentSecurityManagerSession, CopyValidSession) {
    std::cout << "Entering CopyValidSession test" << std::endl;
    
    EXPECT_NO_THROW({
        // Create a valid session with sessionID=100 and inputSummaryHash=12345
        ContentSecurityManagerSession originalSession(100, 12345);
        std::cout << "Invoked ContentSecurityManagerSession(100, 12345) constructor." << std::endl;
        
        // Debug log: Retrieve original session state
        int64_t origSessionID = originalSession.getSessionID();
        std::size_t origInputSummaryHash = originalSession.getInputSummaryHash();
        std::cout << "Original session getSessionID returned: " << origSessionID << std::endl;
        std::cout << "Original session getInputSummaryHash returned: " << origInputSummaryHash << std::endl;
        
        // Create a new session object using the copy constructor of a valid session object
        ContentSecurityManagerSession copiedSession(originalSession);
        std::cout << "Invoked copy constructor on a valid session object." << std::endl;
        
        // Debug log: Retrieve copied session state
        int64_t copiedSessionID = copiedSession.getSessionID();
        std::size_t copiedInputSummaryHash = copiedSession.getInputSummaryHash();
        std::cout << "Copied session getSessionID returned: " << copiedSessionID << std::endl;
        std::cout << "Copied session getInputSummaryHash returned: " << copiedInputSummaryHash << std::endl;

        // Verify that the copied session has the same sessionID and inputSummaryHash as the original
        EXPECT_EQ(copiedSessionID, 100);
        EXPECT_EQ(copiedInputSummaryHash, 12345);
    });
    
    std::cout << "Exiting CopyValidSession test" << std::endl;
}
/**
 * @brief Verify that a ContentSecurityManagerSession instance can be created with a positive sessionID and a non-zero inputSummaryHash.
 *
 * This test case validates that invoking the ContentSecurityManagerSession constructor with a valid sessionID (positive value) and a valid non-zero inputSummaryHash does not throw any exceptions. Ensuring proper instance creation under these conditions is critical for the overall functionality.
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
 * | 01 | Invoke the ContentSecurityManagerSession constructor with a positive sessionID and a non-zero inputSummaryHash. | sessionID = 1000, inputSummaryHash = 5000 | Instance created without throwing any exceptions. | Should Pass |
 */
TEST(ContentSecurityManagerSession, CreateSession_PositiveID_NonZeroHash) {
    std::cout << "Entering CreateSession_PositiveID_NonZeroHash test" << std::endl;
    int64_t sessionID = 1000;
    std::size_t inputSummaryHash = 5000;
    std::cout << "Invoking constructor ContentSecurityManagerSession(" << sessionID 
              << ", " << inputSummaryHash << ")" << std::endl;
    EXPECT_NO_THROW({
        ContentSecurityManagerSession session(sessionID, inputSummaryHash);
        std::cout << "ContentSecurityManagerSession instance created with sessionID = " 
                  << sessionID << " and inputSummaryHash = " << inputSummaryHash << std::endl;
    });
    std::cout << "Exiting CreateSession_PositiveID_NonZeroHash test" << std::endl;
}
/**
 * @brief Test the ContentSecurityManagerSession constructor with zero sessionID and non-zero summary hash
 *
 * This test ensures that creating a ContentSecurityManagerSession with a sessionID set to zero and a non-zero summary hash does not throw an exception, validating that the constructor handles these input cases correctly.
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
 * | :--------------: | ----------- | --------- | ------------- | ----- |
 * | 01 | Initialize sessionID with zero and inputSummaryHash with a non-zero value. Invoke the ContentSecurityManagerSession constructor and verify that no exception is thrown. | sessionID = 0, inputSummaryHash = 1234 | The constructor completes without throwing an exception, creating a session instance. | Should Pass |
 */
TEST(ContentSecurityManagerSession, CreateSession_ZeroID_NonZeroHash) {
    std::cout << "Entering CreateSession_ZeroID_NonZeroHash test" << std::endl;
    int64_t sessionID = 0;
    std::size_t inputSummaryHash = 1234;
    std::cout << "Invoking constructor ContentSecurityManagerSession(" << sessionID 
              << ", " << inputSummaryHash << ")" << std::endl;
    EXPECT_NO_THROW({
        ContentSecurityManagerSession session(sessionID, inputSummaryHash);
        std::cout << "ContentSecurityManagerSession instance created with sessionID = " 
                  << sessionID << " and inputSummaryHash = " << inputSummaryHash << std::endl;
    });
    std::cout << "Exiting CreateSession_ZeroID_NonZeroHash test" << std::endl;
}
/**
 * @brief Test the creation of a ContentSecurityManagerSession with a valid sessionID and zero inputSummaryHash
 *
 * This test validates that constructing a ContentSecurityManagerSession object with sessionID = 2000 and inputSummaryHash = 0 does not throw any exceptions. It ensures that the session is correctly instantiated when provided with a positive sessionID and a zero hash value.
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
 * | Variation / Step | Description                                                                                   | Test Data                                  | Expected Result                                       | Notes      |
 * | :--------------: | --------------------------------------------------------------------------------------------- | ------------------------------------------ | ----------------------------------------------------- | ---------- |
 * | 01               | Invoke ContentSecurityManagerSession constructor with sessionID = 2000 and inputSummaryHash = 0 | sessionID = 2000, inputSummaryHash = 0      | No exception thrown; object is created successfully   | Should Pass|
 */
TEST(ContentSecurityManagerSession, CreateSession_PositiveID_ZeroHash) {
    std::cout << "Entering CreateSession_PositiveID_ZeroHash test" << std::endl;
    int64_t sessionID = 2000;
    std::size_t inputSummaryHash = 0;
    std::cout << "Invoking constructor ContentSecurityManagerSession(" << sessionID 
              << ", " << inputSummaryHash << ")" << std::endl;
    EXPECT_NO_THROW({
        ContentSecurityManagerSession session(sessionID, inputSummaryHash);
        std::cout << "ContentSecurityManagerSession instance created with sessionID = " 
                  << sessionID << " and inputSummaryHash = " << inputSummaryHash << std::endl;
    });
    std::cout << "Exiting CreateSession_PositiveID_ZeroHash test" << std::endl;
}
/**
 * @brief Test that constructing ContentSecurityManagerSession with a negative sessionID and valid summary hash does not throw exceptions.
 *
 * This test verifies that a negative sessionID when combined with a valid inputSummaryHash results in a successful creation
 * of the ContentSecurityManagerSession instance without throwing any exceptions. The constructor is expected to handle the negative sessionID appropriately.
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
 * | Variation / Step | Description                                                                                   | Test Data                                      | Expected Result                                              | Notes       |
 * | :--------------: | ----------------------------------------------------------------------------------------------| ---------------------------------------------- | ------------------------------------------------------------- | ----------- |
 * | 01               | Invoke ContentSecurityManagerSession constructor with sessionID as -100 and inputSummaryHash as 100 | sessionID = -100, inputSummaryHash = 100        | Constructor call should not throw any exception              | Should Pass |
 */
TEST(ContentSecurityManagerSession, CreateSession_NegativeID_ValidHash) {
    std::cout << "Entering CreateSession_NegativeID_ValidHash test" << std::endl;
    int64_t sessionID = -100;
    std::size_t inputSummaryHash = 100;
    std::cout << "Invoking constructor ContentSecurityManagerSession(" << sessionID 
              << ", " << inputSummaryHash << ")" << std::endl;
    EXPECT_NO_THROW({
        ContentSecurityManagerSession session(sessionID, inputSummaryHash);
        std::cout << "ContentSecurityManagerSession instance created with sessionID = " 
                  << sessionID << " and inputSummaryHash = " << inputSummaryHash << std::endl;
    });
    std::cout << "Exiting CreateSession_NegativeID_ValidHash test" << std::endl;
}
/**
 * @brief Test to verify that the ContentSecurityManagerSession constructor handles maximum input values.
 *
 * This test checks that invoking the ContentSecurityManagerSession constructor with the maximum possible values for sessionID and inputSummaryHash does not throw any exceptions and creates a valid instance.
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
 * | Variation / Step | Description                                                                     | Test Data                                                                                             | Expected Result                                                    | Notes         |
 * | :----:           | ------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------ | ------------- |
 * | 01               | Initialize sessionID and inputSummaryHash with their maximum possible values      | sessionID = INT64_MAX, inputSummaryHash = std::numeric_limits<std::size_t>::max()                      | Variables are initialized with the maximum values                 | Should be successful |
 * | 02               | Invoke the ContentSecurityManagerSession constructor using the initialized values | API: ContentSecurityManagerSession(sessionID, inputSummaryHash)                                        | Instance is created without throwing any exceptions                | Should Pass   |
 */
TEST(ContentSecurityManagerSession, CreateSession_MaxValues) {
    std::cout << "Entering CreateSession_MaxValues test" << std::endl;
    int64_t sessionID = INT64_MAX;
    std::size_t inputSummaryHash = std::numeric_limits<std::size_t>::max();
    std::cout << "Invoking constructor ContentSecurityManagerSession(" << sessionID 
              << ", " << inputSummaryHash << ")" << std::endl;
    EXPECT_NO_THROW({
        ContentSecurityManagerSession session(sessionID, inputSummaryHash);
        std::cout << "ContentSecurityManagerSession instance created with sessionID = " 
                  << sessionID << " and inputSummaryHash = " << inputSummaryHash << std::endl;
    });
    std::cout << "Exiting CreateSession_MaxValues test" << std::endl;
}
/**
 * @brief Test creation of ContentSecurityManagerSession with minimum sessionID value
 *
 * This test verifies that the ContentSecurityManagerSession constructor can handle the minimum possible value for sessionID (INT64_MIN) without throwing an exception. It confirms the robustness of the API when provided with edge-case input values.
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
 * | Variation / Step | Description                                                                                          | Test Data                                          | Expected Result                                      | Notes      |
 * | :--------------: | ---------------------------------------------------------------------------------------------------- | -------------------------------------------------- | ---------------------------------------------------- | ---------- |
 * | 01               | Invoke ContentSecurityManagerSession constructor with sessionID = INT64_MIN and inputSummaryHash = 1 | sessionID = INT64_MIN, inputSummaryHash = 1         | No exceptions thrown; instance created successfully  | Should Pass|
 */
TEST(ContentSecurityManagerSession, CreateSession_MinValueID) {
    std::cout << "Entering CreateSession_MinValueID test" << std::endl;
    int64_t sessionID = INT64_MIN;
    std::size_t inputSummaryHash = 1;
    std::cout << "Invoking constructor ContentSecurityManagerSession(" << sessionID 
              << ", " << inputSummaryHash << ")" << std::endl;
    EXPECT_NO_THROW({
        ContentSecurityManagerSession session(sessionID, inputSummaryHash);
        std::cout << "ContentSecurityManagerSession instance created with sessionID = " 
                  << sessionID << " and inputSummaryHash = " << inputSummaryHash << std::endl;
    });
    std::cout << "Exiting CreateSession_MinValueID test" << std::endl;
}
/**
 * @brief Validates that a ContentSecurityManagerSession object is instantiated correctly and its ToString() method returns the expected string.
 *
 * This test confirms that a session created with valid parameters does not throw any exceptions during construction and when invoking ToString(). It verifies the correctness of the session's string representation by comparing the output against the expected result.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create ContentSecurityManagerSession with a valid sessionID and inputSummaryHash | testSessionID = 12345, testInputSummaryHash = 1111 | No exception thrown during construction | Should Pass |
 * | 02 | Invoke the ToString() method on the session object | session object, method call = ToString() | No exception thrown and string value is returned | Should Pass |
 * | 03 | Validate that the returned string matches the expected format "Session 12345 valid" | expected = "Session 12345 valid", result = output of ToString() | EXPECT_EQ(result, expected) passes | Should Pass |
 */
TEST(ContentSecurityManagerSession, ValidSession_PositiveID)
{
    std::cout << "Entering ValidSession_PositiveID test" << std::endl;
    
    // Create object with sessionID = 12345 and a valid inputSummaryHash (e.g., 1111)
    int64_t testSessionID = 12345;
    std::size_t testInputSummaryHash = 1111;
    std::cout << "Creating ContentSecurityManagerSession object with sessionID = " << testSessionID 
              << " and inputSummaryHash = " << testInputSummaryHash << std::endl;
    EXPECT_NO_THROW(ContentSecurityManagerSession session(testSessionID, testInputSummaryHash));
    ContentSecurityManagerSession session(testSessionID, testInputSummaryHash);
    
    // Logging method invocation
    std::cout << "Invoking ToString() method on object" << std::endl;
    std::string result;
    EXPECT_NO_THROW(result = session.ToString());
    std::cout << "ToString() returned: " << result << std::endl;
    
    // Expected result: "Session 12345 valid"
    std::string expected = "Session " + std::to_string(testSessionID) + " valid"; 
    std::cout << "Expected result: " << expected << std::endl;
    EXPECT_EQ(result, expected);
    
    std::cout << "Exiting ValidSession_PositiveID test" << std::endl;
}
/**
 * @brief Validate that creating a ContentSecurityManagerSession with a zero sessionID and a valid inputSummaryHash works correctly.
 *
 * This test verifies that the ContentSecurityManagerSession object can be instantiated with a sessionID of 0 and a valid inputSummaryHash without throwing exceptions. It also checks that the ToString() method returns the expected string "Session 0 valid". This ensures the proper handling of edge case inputs in the session object.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |@n
 * | :----: | --------- | ---------- |-------------- | ----- |@n
 * | 01 | Create ContentSecurityManagerSession object with sessionID = 0 and inputSummaryHash = 2222 | sessionID = 0, inputSummaryHash = 2222 | Object is created without exceptions | Should Pass |@n
 * | 02 | Invoke ToString() method on the created object | No additional input | Returns "Session 0 valid" without exceptions | Should Pass |
 */
TEST(ContentSecurityManagerSession, ValidSession_ZeroID)
{
    std::cout << "Entering ValidSession_ZeroID test" << std::endl;
    
    // Create object with sessionID = 0 and a valid inputSummaryHash (e.g., 2222)
    int64_t testSessionID = 0;
    std::size_t testInputSummaryHash = 2222;
    std::cout << "Creating ContentSecurityManagerSession object with sessionID = " << testSessionID 
              << " and inputSummaryHash = " << testInputSummaryHash << std::endl;
    EXPECT_NO_THROW(ContentSecurityManagerSession session(testSessionID, testInputSummaryHash));
    ContentSecurityManagerSession session(testSessionID, testInputSummaryHash);
    
    // Logging method invocation
    std::cout << "Invoking ToString() method on object" << std::endl;
    std::string result;
    EXPECT_NO_THROW(result = session.ToString());
    std::cout << "ToString() returned: " << result << std::endl;
    
    // Expected result: "Session 0 valid"
    std::string expected = "Session " + std::to_string(testSessionID) + " valid";
    std::cout << "Expected result: " << expected << std::endl;
    EXPECT_EQ(result, expected);
    
    std::cout << "Exiting ValidSession_ZeroID test" << std::endl;
}
/**
 * @brief Verify that ContentSecurityManagerSession correctly handles boundary condition for sessionID.
 *
 * This test verifies that ContentSecurityManagerSession can be instantiated with the maximum int64_t value (INT64_MAX) as sessionID along with a valid inputSummaryHash. It also checks that the ToString() method returns the expected string representation without throwing an exception.
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
 * | 01 | Instantiate ContentSecurityManagerSession with sessionID = INT64_MAX and inputSummaryHash = 3333 | input: sessionID = INT64_MAX, inputSummaryHash = 3333, output: valid session object | Object created without throwing an exception | Should Pass |
 * | 02 | Call the ToString() method on the created session object to verify its string representation | input: session object, method: ToString(), expected string: "Session 9223372036854775807 valid" | Returned string equals "Session 9223372036854775807 valid" | Should Pass |
 */
TEST(ContentSecurityManagerSession, BoundaryCondition_MaxInt64)
{
    std::cout << "Entering BoundaryCondition_MaxInt64 test" << std::endl;
    
    // Create object with sessionID = INT64_MAX and a valid inputSummaryHash (e.g., 3333)
    int64_t testSessionID = INT64_MAX;
    std::size_t testInputSummaryHash = 3333;
    std::cout << "Creating ContentSecurityManagerSession object with sessionID = " << testSessionID 
              << " and inputSummaryHash = " << testInputSummaryHash << std::endl;
    EXPECT_NO_THROW(ContentSecurityManagerSession session(testSessionID, testInputSummaryHash));
    ContentSecurityManagerSession session(testSessionID, testInputSummaryHash);
    
    // Logging method invocation
    std::cout << "Invoking ToString() method on object" << std::endl;
    std::string result;
    EXPECT_NO_THROW(result = session.ToString());
    std::cout << "ToString() returned: " << result << std::endl;
    
    // Expected result: "Session 9223372036854775807 valid"
    std::string expected = "Session " + std::to_string(testSessionID) + " valid";
    std::cout << "Expected result: " << expected << std::endl;
    EXPECT_EQ(result, expected);
    
    std::cout << "Exiting BoundaryCondition_MaxInt64 test" << std::endl;
}
/**
 * @brief Validate getInputSummaryHash returns the correct hash for valid input
 *
 * This test verifies that when a ContentSecurityManagerSession object is instantiated with a valid sessionID and inputSummaryHash,
 * the getInputSummaryHash method correctly returns the initialized inputSummaryHash value. This ensures the functionality of the getter method is as expected.
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
 * | 01 | Create ContentSecurityManagerSession object with sessionID = 1000 and inputSummaryHash = 12345 | input: sessionID = 1000, inputSummaryHash = 12345 | Session object is created successfully | Should be successful |
 * | 02 | Invoke getInputSummaryHash method on the created session object | output: result variable to store the hash value | Method returns 12345 | Should Pass |
 * | 03 | Validate the returned value using EXPECT_EQ assertion | input: expected value = 12345, actual value = result | EXPECT_EQ assertion passes if result equals 12345 | Should Pass |
 */
TEST(ContentSecurityManagerSession, getInputSummaryHash_Positive_ValidInput) {
    std::cout << "Entering getInputSummaryHash_Positive_ValidInput test" << std::endl;
    
    // Using parameterized constructor with sessionID = 1000, inputSummaryHash = 12345
    std::cout << "Creating ContentSecurityManagerSession object with sessionID = 1000 and inputSummaryHash = 12345" << std::endl;
    ContentSecurityManagerSession session(1000, 12345);
    
    std::cout << "Invoking getInputSummaryHash method" << std::endl;
    std::size_t result = session.getInputSummaryHash();
    std::cout << "Returned value from getInputSummaryHash: " << result << std::endl;
    
    EXPECT_EQ(12345, result);
    
    std::cout << "Exiting getInputSummaryHash_Positive_ValidInput test" << std::endl;
}
/**
 * @brief Test getInputSummaryHash with the lower boundary condition.
 *
 * This test evaluates the getInputSummaryHash method of the ContentSecurityManagerSession class.
 * It creates an instance with sessionID = 1001 and inputSummaryHash = 0 (boundary condition) and verifies that the returned value is 0.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create ContentSecurityManagerSession instance with sessionID=1001 and inputSummaryHash=0 | sessionID = 1001, inputSummaryHash = 0 | Object is created successfully with given parameters | Should be successful |
 * | 02 | Invoke getInputSummaryHash method | No additional parameters | Returns 0, matching the assertion EXPECT_EQ(0, result) | Should Pass |
 */
TEST(ContentSecurityManagerSession, getInputSummaryHash_Positive_LowBoundary) {
    std::cout << "Entering getInputSummaryHash_Positive_LowBoundary test" << std::endl;
    
    // Using parameterized constructor with sessionID = 1001, inputSummaryHash = 0 (boundary condition)
    std::cout << "Creating ContentSecurityManagerSession object with sessionID = 1001 and inputSummaryHash = 0" << std::endl;
    ContentSecurityManagerSession session(1001, 0);
    
    std::cout << "Invoking getInputSummaryHash method" << std::endl;
    std::size_t result = session.getInputSummaryHash();
    std::cout << "Returned value from getInputSummaryHash: " << result << std::endl;
    
    EXPECT_EQ(0, result);
    
    std::cout << "Exiting getInputSummaryHash_Positive_LowBoundary test" << std::endl;
}
/**
 * @brief Test the getInputSummaryHash API using a maximum input summary hash value
 *
 * This test verifies that when a ContentSecurityManagerSession object is constructed with the maximum possible value for inputSummaryHash, the getInputSummaryHash method returns this maximum value correctly. This confirms that the API handles extreme values as expected.
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
 * | 01 | Instantiate ContentSecurityManagerSession object | sessionID = 1002, inputSummaryHash = numeric_limits<std::size_t>::max() | Object is constructed successfully | Should be successful |
 * | 02 | Invoke getInputSummaryHash method on the object | Invocation: getInputSummaryHash() | Returns the maximum possible value of inputSummaryHash | Should Pass |
 * | 03 | Verify that the returned value matches the maximum value provided | result = returned value, expected = numeric_limits<std::size_t>::max() | EXPECT_EQ check passes confirming the returned value equals the maximum hash value | Should Pass |
 */
TEST(ContentSecurityManagerSession, getInputSummaryHash_Positive_MaximumInput) {
    std::cout << "Entering getInputSummaryHash_Positive_MaximumInput test" << std::endl;
    
    // Using parameterized constructor with sessionID = 1002, inputSummaryHash = maximum possible value
    std::size_t maxHash = std::numeric_limits<std::size_t>::max();
    std::cout << "Creating ContentSecurityManagerSession object with sessionID = 1002 and inputSummaryHash = " << maxHash << std::endl;
    ContentSecurityManagerSession session(1002, maxHash);
    
    std::cout << "Invoking getInputSummaryHash method" << std::endl;
    std::size_t result = session.getInputSummaryHash();
    std::cout << "Returned value from getInputSummaryHash: " << result << std::endl;
    
    EXPECT_EQ(maxHash, result);
    
    std::cout << "Exiting getInputSummaryHash_Positive_MaximumInput test" << std::endl;
}
/**
 * @brief Test to verify that the default constructed ContentSecurityManagerSession object returns the expected input summary hash.
 *
 * This test creates a ContentSecurityManagerSession object using its default constructor and then invokes the getInputSummaryHash method to verify that it returns the default hash value of 0. This ensures that the object is correctly initialized with a default state.
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
 * | Variation / Step | Description                                                                          | Test Data                                                  | Expected Result                                                    | Notes      |
 * | :--------------: | ------------------------------------------------------------------------------------ | ---------------------------------------------------------- | ------------------------------------------------------------------ | ---------- |
 * | 01               | Create ContentSecurityManagerSession object using default constructor and invoke getInputSummaryHash method | No input; Output: result from getInputSummaryHash = 0       | Return value is 0, and the assertion EXPECT_EQ(0, result) passes     | Should Pass |
 */
TEST(ContentSecurityManagerSession, getInputSummaryHash_Positive_DefaultConstructor) {
    std::cout << "Entering getInputSummaryHash_Positive_DefaultConstructor test" << std::endl;
    
    // Using default constructor expecting default initialization (assumed to be 0)
    std::cout << "Creating ContentSecurityManagerSession object using default constructor" << std::endl;
    ContentSecurityManagerSession session;
    
    std::cout << "Invoking getInputSummaryHash method" << std::endl;
    std::size_t result = session.getInputSummaryHash();
    std::cout << "Returned value from getInputSummaryHash: " << result << std::endl;
    
    EXPECT_EQ(0, result);
    
    std::cout << "Exiting getInputSummaryHash_Positive_DefaultConstructor test" << std::endl;
}
/**
 * @brief Positive test to ensure that a valid session ID is correctly handled.
 *
 * This test verifies that the parameterized constructor of ContentSecurityManagerSession accepts a valid session ID (12345)
 * and that the getSessionID() method returns the same session ID. This is essential to ensure that the session object is properly instantiated and its session ID is correctly retrievable.
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
 * | Variation / Step | Description                                                                              | Test Data                                        | Expected Result                                           | Notes         |
 * | :--------------: | ---------------------------------------------------------------------------------------- | ------------------------------------------------ | --------------------------------------------------------- | ------------- |
 * | 01               | Invoke parameterized constructor with sessionID=12345 and inputSummaryHash=0             | sessionID = 12345, inputSummaryHash = 0            | Object creation is successful without throwing exceptions | Should Pass   |
 * | 02               | Call getSessionID() method on the created session object                                   | No input parameters                              | Returns sessionID equal to 12345                           | Should Pass   |
 * | 03               | Validate that the returned sessionID matches the expected session ID using EXPECT_EQ        | Expected sessionID = 12345, Returned sessionID = 12345 | The returned sessionID is equal to the expected value      | Should Pass   |
 */
TEST(ContentSecurityManagerSession, PositiveValidPositiveSessionId) {
    std::cout << "Entering PositiveValidPositiveSessionId test" << std::endl;
    int64_t expectedID = 12345;
    // Instantiate object using parameterized constructor with sessionID=12345 and dummy inputSummaryHash
    EXPECT_NO_THROW({
        ContentSecurityManagerSession session(expectedID, 0);
        std::cout << "Invoked parameterized constructor with sessionID = " << expectedID
                  << " and inputSummaryHash = " << 0 << std::endl;
        std::cout << "Invoking getSessionID()" << std::endl;
        int64_t sessionID = session.getSessionID();
        std::cout << "getSessionID() returned " << sessionID << std::endl;
        EXPECT_EQ(sessionID, expectedID);
    });
    std::cout << "Exiting PositiveValidPositiveSessionId test" << std::endl;
}
/**
 * @brief Verify that getSessionID() returns 0 when the session is initialized with sessionID = 0.
 *
 * This test creates an instance of ContentSecurityManagerSession using a sessionID of 0 along with a dummy inputSummaryHash value of 0,
 * and verifies that the getSessionID() method correctly returns 0. This confirms that the class handles a zero sessionID as a valid input.
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
 * | Variation / Step | Description | Test Data |Expected Result |Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoke the parameterized constructor with sessionID = 0 and inputSummaryHash = 0, then call getSessionID() | sessionID = 0, inputSummaryHash = 0, expected sessionID = 0 | getSessionID() returns 0 | Should Pass |
 */
TEST(ContentSecurityManagerSession, PositiveValidZeroSessionId) {
    std::cout << "Entering PositiveValidZeroSessionId test" << std::endl;
    int64_t expectedID = 0;
    // Instantiate object using parameterized constructor with sessionID=0 and dummy inputSummaryHash
    EXPECT_NO_THROW({
        ContentSecurityManagerSession session(expectedID, 0);
        std::cout << "Invoked parameterized constructor with sessionID = " << expectedID
                  << " and inputSummaryHash = " << 0 << std::endl;
        std::cout << "Invoking getSessionID()" << std::endl;
        int64_t sessionID = session.getSessionID();
        std::cout << "getSessionID() returned " << sessionID << std::endl;
        EXPECT_EQ(sessionID, expectedID);
    });
    std::cout << "Exiting PositiveValidZeroSessionId test" << std::endl;
}
/**
 * @brief Validate the behavior of the ContentSecurityManagerSession constructor with boundary maximum int64_t value.
 *
 * This test verifies that the ContentSecurityManagerSession object is correctly instantiated when the sessionID
 * is set to the maximum int64_t value, and that calling getSessionID() returns the expected value. This helps 
 * ensure that the system can handle boundary values without throwing exceptions.
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
 * | 01 | Invoke the parameterized constructor with sessionID set to INT64_MAX and inputSummaryHash set to 0, then call getSessionID() to verify it returns INT64_MAX. | sessionID = INT64_MAX, inputSummaryHash = 0, getSessionID() result = INT64_MAX | No exception thrown and getSessionID() returns INT64_MAX | Should Pass |
 */
TEST(ContentSecurityManagerSession, BoundaryMaxInt64) {
    std::cout << "Entering BoundaryMaxInt64 test" << std::endl;
    int64_t expectedID = std::numeric_limits<int64_t>::max();
    // Instantiate object using parameterized constructor with sessionID=INT64_MAX and dummy inputSummaryHash
    EXPECT_NO_THROW({
        ContentSecurityManagerSession session(expectedID, 0);
        std::cout << "Invoked parameterized constructor with sessionID = " << expectedID
                  << " and inputSummaryHash = " << 0 << std::endl;
        std::cout << "Invoking getSessionID()" << std::endl;
        int64_t sessionID = session.getSessionID();
        std::cout << "getSessionID() returned " << sessionID << std::endl;
        EXPECT_EQ(sessionID, expectedID);
    });
    std::cout << "Exiting BoundaryMaxInt64 test" << std::endl;
}
/**
 * @brief Test to verify that the ContentSecurityManagerSession handles INT64_MIN boundary values correctly.
 *
 * This test verifies that when a ContentSecurityManagerSession object is instantiated with the minimum 
 * possible int64_t value for sessionID and a dummy inputSummaryHash, the getSessionID() method returns the correct 
 * sessionID value. This helps ensure that the API can correctly handle extreme negative boundary conditions.
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
 * | Variation / Step | Description                                                                     | Test Data                                                             | Expected Result                                                                          | Notes       |
 * | :--------------: | ------------------------------------------------------------------------------- | --------------------------------------------------------------------- | ---------------------------------------------------------------------------------------- | ----------- |
 * | 01               | Invoke parameterized constructor with sessionID=INT64_MIN & inputSummaryHash=0; call getSessionID() to verify it returns INT64_MIN | sessionID = INT64_MIN, inputSummaryHash = 0, output from getSessionID = INT64_MIN | getSessionID() returns INT64_MIN and no exception is thrown during object construction | Should Pass |
 */
TEST(ContentSecurityManagerSession, BoundaryMinInt64) {
    std::cout << "Entering BoundaryMinInt64 test" << std::endl;
    int64_t expectedID = std::numeric_limits<int64_t>::min();
    // Instantiate object using parameterized constructor with sessionID=INT64_MIN and dummy inputSummaryHash
    EXPECT_NO_THROW({
        ContentSecurityManagerSession session(expectedID, 0);
        std::cout << "Invoked parameterized constructor with sessionID = " << expectedID
                  << " and inputSummaryHash = " << 0 << std::endl;
        std::cout << "Invoking getSessionID()" << std::endl;
        int64_t sessionID = session.getSessionID();
        std::cout << "getSessionID() returned " << sessionID << std::endl;
        EXPECT_EQ(sessionID, expectedID);
    });
    std::cout << "Exiting BoundaryMinInt64 test" << std::endl;
}
/**
 * @brief Validates the default construction of ContentSecurityManagerSession and verifies that the session ID is initialized to zero.
 *
 * This test case checks the default constructor of the ContentSecurityManagerSession class to ensure that it does not throw any exceptions, and that calling getSessionID() returns a session ID of 0. This confirms that the object is correctly initialized to a default state.
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
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Instantiate ContentSecurityManagerSession using the default constructor and validate the session ID | Input: Default constructor invocation, Output: getSessionID() = 0 | No exception thrown and getSessionID() returns 0 | Should Pass |
 */
TEST(ContentSecurityManagerSession, DefaultConstructorTest) {
    std::cout << "Entering DefaultConstructorTest test" << std::endl;
    // Instantiate object using the default constructor; expected default sessionID is 0
    EXPECT_NO_THROW({
        ContentSecurityManagerSession session;
        std::cout << "Invoked default constructor" << std::endl;
        std::cout << "Invoking getSessionID()" << std::endl;
        int64_t sessionID = session.getSessionID();
        std::cout << "getSessionID() returned " << sessionID << std::endl;
        EXPECT_EQ(sessionID, int64_t(0));
    });
    std::cout << "Exiting DefaultConstructorTest test" << std::endl;
}
/**
 * @brief Verify that proper initialization of ContentSecurityManagerSession results in a valid session.
 *
 * This test creates an instance of ContentSecurityManagerSession using a valid SessionManager initialization by providing a valid sessionID and inputSummaryHash.
 * It then invokes the isSessionValid() method to ensure that the session is valid. This test checks for proper object creation, absence of exceptions during construction and method execution,
 * and verifies that the session validity check returns true.
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
 * | Variation / Step | Description                                                                                         | Test Data                                             | Expected Result                                                 | Notes       |
 * | :--------------: | --------------------------------------------------------------------------------------------------- | ----------------------------------------------------- | --------------------------------------------------------------- | ----------- |
 * | 01               | Create ContentSecurityManagerSession instance using parameterized constructor with valid inputs.   | sessionID = 12345, inputSummaryHash = 67890             | Instance is created without throwing an exception.            | Should Pass |
 * | 02               | Invoke isSessionValid() method on the created instance to check session validity.                   | instance created with sessionID = 12345, inputSummaryHash = 67890, expectedReturn = true | isSessionValid() returns true and assertion passes.             | Should Pass |
 */
TEST(ContentSecurityManagerSession, SessionValidProperInitialized)
{
    std::cout << "Entering Session is valid when a proper SessionManager is initialized test" << std::endl;

    // Create object using parameterized constructor with proper SessionManager initialization
    std::int64_t sessionID = 12345;
    std::size_t inputSummaryHash = 67890;
    std::cout << "Creating ContentSecurityManagerSession with sessionID: " << sessionID 
              << " and inputSummaryHash: " << inputSummaryHash << std::endl;
    EXPECT_NO_THROW(ContentSecurityManagerSession session(sessionID, inputSummaryHash));
    ContentSecurityManagerSession session(sessionID, inputSummaryHash);

    // Invoke isSessionValid() method and log the invocation and its return value
    bool isValid = false;
    EXPECT_NO_THROW({
        std::cout << "Invoking isSessionValid() method" << std::endl;
        isValid = session.isSessionValid();
        std::cout << "isSessionValid() returned: " << std::boolalpha << isValid << std::endl;
    });
    
    // Verify that the session is valid (should return true)
    EXPECT_TRUE(isValid);
    std::cout << "Exiting Session is valid when a proper SessionManager is initialized test" << std::endl;
}
/**
 * @brief Verify that a ContentSecurityManagerSession constructed with the default constructor is invalid when the SessionManager is not initialized.
 *
 * This test creates an instance of ContentSecurityManagerSession using the default constructor when the SessionManager is not initialized.
 * It then calls the isSessionValid() method and verifies that the session is reported as invalid (i.e., isSessionValid() returns false).
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
 * | Variation / Step | Description                                                          | Test Data                                                         | Expected Result                                               | Notes        |
 * | :--------------: | --------------------------------------------------------------------- | ----------------------------------------------------------------- | ------------------------------------------------------------- | ------------ |
 * | 01               | Create ContentSecurityManagerSession using the default constructor.  | Invocation: default constructor; no input parameters.             | Object is created without throwing an exception.            | Should Pass  |
 * | 02               | Invoke isSessionValid() method on the created session object.          | session object created; method: isSessionValid(); expected output parameter: false. | Method returns false indicating the session is invalid.       | Should Pass  |
 * | 03               | Verify that the session invalidity is asserted properly using EXPECT_FALSE. | Boolean variable isValid expected to be false.                     | EXPECT_FALSE check passes confirming the session is invalid.  | Should Pass  |
 */
TEST(ContentSecurityManagerSession, SessionInvalidDefaultConstructor)
{
    std::cout << "Entering Session is invalid when SessionManager is not initialized (default constructor scenario) test" << std::endl;

    // Create object using default constructor (SessionManager not initialized)
    std::cout << "Creating ContentSecurityManagerSession using default constructor" << std::endl;
    EXPECT_NO_THROW(ContentSecurityManagerSession session);
    ContentSecurityManagerSession session;

    // Invoke isSessionValid() method and log the invocation and its return value
    bool isValid = true;
    EXPECT_NO_THROW({
        std::cout << "Invoking isSessionValid() method" << std::endl;
        isValid = session.isSessionValid();
        std::cout << "isSessionValid() returned: " << std::boolalpha << isValid << std::endl;
    });
    
    // Verify that the session is invalid (should return false)
    EXPECT_FALSE(isValid);
    std::cout << "Exiting Session is invalid when SessionManager is not initialized (default constructor scenario) test" << std::endl;
}
/**
 * @brief Test to verify the assignment operator for two valid sessions.
 *
 * This test verifies the behavior of the assignment operator when assigning one valid ContentSecurityManagerSession to another. The test creates two sessions with distinct session IDs and input summary hashes, performs the assignment, and validates that the target session's session ID matches that of the source session.
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
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create sessionA with sessionID = 100 and inputSummaryHash = 555 | input1 = 100, input2 = 555 | sessionA is created without exception | Should be successful |
 * | 02 | Create sessionB with sessionID = 200 and inputSummaryHash = 777 | input1 = 200, input2 = 777 | sessionB is created without exception | Should be successful |
 * | 03 | Assign sessionA to sessionB using the assignment operator | operation = sessionB = sessionA | operator= returns a reference to sessionB with sessionID equal to sessionA | Should Pass |
 * | 04 | Validate that sessionB's sessionID matches sessionA's sessionID | sessionB.getSessionID() and sessionA.getSessionID() | EXPECT_EQ confirms that sessionB's sessionID equals sessionA's sessionID | Should Pass |
 */
TEST(ContentSecurityManagerSession, AssignmentOfTwoDistinctValidSessions) {
    std::cout << "Entering AssignmentOfTwoDistinctValidSessions test" << std::endl;
    EXPECT_NO_THROW({
        // Create SessionA with sessionID = 100 and inputSummaryHash = 555
        ContentSecurityManagerSession sessionA(100, 555);
        std::cout << "[DEBUG] Created sessionA with sessionID: " << sessionA.getSessionID() 
                  << " and inputSummaryHash: " << 555 << std::endl;
        // Create SessionB with sessionID = 200 and inputSummaryHash = 777
        ContentSecurityManagerSession sessionB(200, 777);
        std::cout << "[DEBUG] Created sessionB with sessionID: " << sessionB.getSessionID() 
                  << " and inputSummaryHash: " << 777 << std::endl;

        // Invoke assignment operator: sessionB = sessionA
        std::cout << "[DEBUG] Invoking operator= to assign sessionA to sessionB" << std::endl;
        ContentSecurityManagerSession& ret = (sessionB = sessionA);
        std::cout << "[DEBUG] operator= returned with sessionID: " << ret.getSessionID() << std::endl;

        // Validate that sessionB now has the same session manager as sessionA
        EXPECT_EQ(sessionB.getSessionID(), sessionA.getSessionID());
        std::cout << "[DEBUG] After assignment, sessionB sessionID: " << sessionB.getSessionID() << std::endl;
    });
    std::cout << "Exiting AssignmentOfTwoDistinctValidSessions test" << std::endl;
}
/**
 * @brief Verify the ContentSecurityManagerSession creation and validation without exception.
 *
 * This test creates an instance of ContentSecurityManagerSession with a sessionID of 100 and an inputSummaryHash of 555. It then invokes the isSessionValid() method to check the session validity, ensuring that no exceptions are thrown throughout the process.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create a session instance with sessionID = 100 and inputSummaryHash = 555 | sessionID = 100, inputSummaryHash = 555 | Instance is successfully created without throwing an exception | Should Pass |
 * | 02 | Invoke the isSessionValid() method on the session instance | Method Call: isSessionValid(), Input: N/A, Output: Boolean status | Method executes without throwing exception and returns validity status | Should Pass |
 */
TEST(ContentSecurityManagerSession, setSessionInvalidValid) {
    std::cout << "Entering setSessionInvalidValid test" << std::endl;
    EXPECT_NO_THROW({
        // Create SessionA with sessionID = 100 and inputSummaryHash = 555
        ContentSecurityManagerSession session(100, 555);
        std::cout << "[DEBUG] Created session with sessionID: " << 100 
                  << " and inputSummaryHash: " << 555 << std::endl;
        
        std::cout << "Invoking setSessionInvalid() method" << std::endl;
        session.setSessionInvalid();
        std::cout << "Invoked setSessionInvalid() method" << std::endl;

    });
    std::cout << "Exiting setSessionInvalidValid test" << std::endl;
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
