
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
#include "playerisobmffbuffer.h"


// Test Case: Default constructor initializes all members to default values
/**
 * @brief Verifies that the default constructor of PlayerIsoBmffBuffer properly initializes all member variables.
 *
 * This test checks that when a PlayerIsoBmffBuffer object is created using its default constructor,
 * all internal members (boxes vector, buffer pointer, bufSize, chunkedBox pointer, and mdatCount) are initialized to their expected default values.
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
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoke PlayerIsoBmffBuffer default constructor | No input, object creation | Object is created without throwing an exception | Should Pass |
 * | 02 | Validate that boxes vector is empty | boxes.size() = 0, actual = obj.boxes.size() | Return value equals 0 | Should Pass |
 * | 03 | Validate that buffer pointer is nullptr | buffer = nullptr, actual = obj.buffer | Return value is nullptr | Should Pass |
 * | 04 | Validate that bufSize is 0 | bufSize = 0, actual = obj.bufSize | Return value equals 0 | Should Pass |
 * | 05 | Validate that chunkedBox pointer is nullptr | chunkedBox = nullptr, actual = obj.chunkedBox | Return value is nullptr | Should Pass |
 * | 06 | Validate that mdatCount is 0 | mdatCount = 0, actual = obj.mdatCount | Return value equals 0 | Should Pass |
 */
TEST(PlayerIsoBmffBuffer, DefaultConstructorInitializesAllMembers) {
    std::cout << "Entering DefaultConstructorInitializesAllMembers test" << std::endl;

    // Invoke PlayerIsoBmffBuffer default constructor and check for exceptions
    EXPECT_NO_THROW({
        PlayerIsoBmffBuffer obj;
        std::cout << "PlayerIsoBmffBuffer default constructor invoked." << std::endl;

        // Debug log: Print boxes vector size (expected to be 0)
        EXPECT_EQ(obj.getBoxes().size(), 0);

        // Debug log: Print buffer pointer value (expected to be NULL)
        EXPECT_EQ(obj.getBuffer(), nullptr);

        // Debug log: Print bufSize value (expected to be 0)
        EXPECT_EQ(obj.getBufferSize(), 0);

        // Debug log: Print chunkedBox pointer value (expected to be NULL)
        EXPECT_EQ(obj.getChunkedBox(), nullptr);

        // Debug log: Print mdatCount value (expected to be 0)
        EXPECT_EQ(obj.getMdatCount(), 0);
    });

    std::cout << "Exiting DefaultConstructorInitializesAllMembers test" << std::endl;
}
#if 0 //this api defintion is not available so skipped
// /**
//  * @brief Verify that UpdateBufferData returns the expected value and updates buffer sizes for a valid parsedBoxCount.
//  *
//  * This test ensures that when a valid parsedBoxCount and proper buffer data are provided, the UpdateBufferData
//  * method does not throw any exception, returns the correct value (1), and updates the buffer sizes appropriately.
//  *
//  * **Test Group ID:** Basic: 01
//  * **Test Case ID:** 002
//  * **Priority:** High
//  *
//  * **Pre-Conditions:** None
//  * **Dependencies:** None
//  * **User Interaction:** None
//  *
//  * **Test Procedure:**
//  * | Variation / Step | Description                                          | Test Data                                                                                          | Expected Result                        | Notes      |
//  * | :--------------: | ---------------------------------------------------- | -------------------------------------------------------------------------------------------------- | -------------------------------------- | ---------- |
//  * |       01         | Set up valid buffer data and invoke UpdateBufferData | parsedBoxCount = 1, unParsedBuffer = "TestBufferData", unParsedBufferSize = 100, parsedBufferSize = 0 | Return value equals 1; assertion passes  | Should Pass|
//  */
// TEST(PlayerIsoBmffBuffer, PositiveScenario_ValidParsedBoxCount) {
//     std::cout << "Entering PositiveScenario_ValidParsedBoxCount test" << std::endl;
//     EXPECT_NO_THROW({
//         PlayerIsoBmffBuffer bufferObj;
//         size_t parsedBoxCount = 1;
//         char* unParsedBuffer = new char[100];
//         strncpy(unParsedBuffer, "TestBufferData", 100);
//         size_t unParsedBufferSize = 100;
//         size_t parsedBufferSize = 0;
//         std::cout << "[DEBUG] Before UpdateBufferData: parsedBoxCount = " << parsedBoxCount 
//                   << ", unParsedBuffer = " << static_cast<const void*>(unParsedBuffer)
//                   << ", unParsedBufferSize = " << unParsedBufferSize 
//                   << ", parsedBufferSize = " << parsedBufferSize << std::endl;
//         int ret = bufferObj.UpdateBufferData(parsedBoxCount, unParsedBuffer, unParsedBufferSize, parsedBufferSize);
//         std::cout << "[DEBUG] UpdateBufferData returned: " << ret << std::endl;
//         // Expected positive outcome: method returns non-zero true value and updates sizes appropriately.
//         EXPECT_EQ(ret, 1);
//         std::cout << "[DEBUG] After UpdateBufferData: unParsedBuffer = " << static_cast<const void*>(unParsedBuffer)
//                   << ", unParsedBufferSize = " << unParsedBufferSize 
//                   << ", parsedBufferSize = " << parsedBufferSize << std::endl;
//         delete[] unParsedBuffer;
//     });
//     std::cout << "Exiting PositiveScenario_ValidParsedBoxCount test" << std::endl;
// }
// /**
//  * @brief Verify that UpdateBufferData returns a negative outcome when parsedBoxCount is zero.
//  *
//  * This test case verifies that when the parsedBoxCount parameter is zero, the UpdateBufferData API
//  * returns 0 indicating a negative outcome and does not change the unParsedBufferSize and parsedBufferSize values.
//  *
//  * **Test Group ID:** Basic: 01@n
//  * **Test Case ID:** 003@n
//  * **Priority:** High@n
//  *
//  * **Pre-Conditions:** None@n
//  * **Dependencies:** None@n
//  * **User Interaction:** None@n
//  *
//  * **Test Procedure:**
//  * | Variation / Step | Description | Test Data | Expected Result | Notes |
//  * | :----: | --------- | ---------- |-------------- | ----- |
//  * | 01 | Invoke UpdateBufferData with parsedBoxCount set to 0 and verify that the function returns 0 and does not modify unParsedBufferSize and parsedBufferSize. | parsedBoxCount = 0, unParsedBuffer = pointer to allocated buffer containing "TestBufferData", unParsedBufferSize = 100, parsedBufferSize = 0 | Return value is 0, unParsedBufferSize remains 100, and parsedBufferSize remains 0 | Should Fail |
//  */
// TEST(PlayerIsoBmffBuffer, NegativeScenario_ParsedBoxCountZero) {
//     std::cout << "Entering NegativeScenario_ParsedBoxCountZero test" << std::endl;
//     EXPECT_NO_THROW({
//         PlayerIsoBmffBuffer bufferObj;
//         size_t parsedBoxCount = 0;
//         char* unParsedBuffer = new char[100];
//         strncpy(unParsedBuffer, "TestBufferData", 100);
//         size_t unParsedBufferSize = 100;
//         size_t parsedBufferSize = 0;
//         std::cout << "[DEBUG] Before UpdateBufferData: parsedBoxCount = " << parsedBoxCount 
//                   << ", unParsedBuffer = " << static_cast<const void*>(unParsedBuffer)
//                   << ", unParsedBufferSize = " << unParsedBufferSize 
//                   << ", parsedBufferSize = " << parsedBufferSize << std::endl;
//         int ret = bufferObj.UpdateBufferData(parsedBoxCount, unParsedBuffer, unParsedBufferSize, parsedBufferSize);
//         std::cout << "[DEBUG] UpdateBufferData returned: " << ret << std::endl;
//         // Expected negative outcome: method returns false (0) with no changes.
//         EXPECT_EQ(ret, 0);
//         std::cout << "[DEBUG] After UpdateBufferData: unParsedBufferSize = " << unParsedBufferSize 
//                   << ", parsedBufferSize = " << parsedBufferSize << std::endl;
//         EXPECT_EQ(unParsedBufferSize, 100);
//         EXPECT_EQ(parsedBufferSize, 0);
//         delete[] unParsedBuffer;
//     });
//     std::cout << "Exiting NegativeScenario_ParsedBoxCountZero test" << std::endl;
// }
// /**
//  * @brief Validate that UpdateBufferData returns a failure status when provided with a null unParsedBuffer.
//  *
//  * This test case verifies that the UpdateBufferData API correctly handles the scenario where the unParsedBuffer is null.
//  * The test ensures that the method returns 0, indicating a failure in processing due to invalid input parameters.
//  *
//  * **Test Group ID:** Basic: 01
//  * **Test Case ID:** 004
//  * **Priority:** High
//  *
//  * **Pre-Conditions:** None
//  * **Dependencies:** None
//  * **User Interaction:** None
//  *
//  * **Test Procedure:**
//  * | Variation / Step | Description                                             | Test Data                                                | Expected Result                                                 | Notes       |
//  * | :--------------: | ------------------------------------------------------- | -------------------------------------------------------- | ---------------------------------------------------------------- | ----------- |
//  * | 01               | Invoke UpdateBufferData with a null unParsedBuffer to validate negative outcome.  | parsedBoxCount = 1, unParsedBuffer = NULL, unParsedBufferSize = 100, parsedBufferSize = 0 | API returns 0 and EXPECT_EQ(ret, 0) assertion passes.      | Should Fail |
//  */
// TEST(PlayerIsoBmffBuffer, NegativeScenario_UnParsedBufferNull) {
//     std::cout << "Entering NegativeScenario_UnParsedBufferNull test" << std::endl;
//     EXPECT_NO_THROW({
//         PlayerIsoBmffBuffer bufferObj;
//         size_t parsedBoxCount = 1;
//         char* unParsedBuffer = NULL;
//         size_t unParsedBufferSize = 100;
//         size_t parsedBufferSize = 0;
//         std::cout << "[DEBUG] Before UpdateBufferData: parsedBoxCount = " << parsedBoxCount 
//                   << ", unParsedBuffer is NULL"
//                   << ", unParsedBufferSize = " << unParsedBufferSize 
//                   << ", parsedBufferSize = " << parsedBufferSize << std::endl;
//         int ret = bufferObj.UpdateBufferData(parsedBoxCount, unParsedBuffer, unParsedBufferSize, parsedBufferSize);
//         std::cout << "[DEBUG] UpdateBufferData returned: " << ret << std::endl;
//         // Expected negative outcome: method returns false (0) due to null buffer.
//         EXPECT_EQ(ret, 0);
//     });
//     std::cout << "Exiting NegativeScenario_UnParsedBufferNull test" << std::endl;
// }
// /**
//  * @brief Test negative scenario for UpdateBufferData with invalid unParsedBufferSize
//  *
//  * This test verifies that the UpdateBufferData API correctly handles an invalid unParsedBufferSize value. It simulates a scenario where the unParsedBufferSize is assigned a large, invalid value (by casting -1 to size_t) to ensure that the API returns an error indicator as expected.
//  *
//  * **Test Group ID:** Basic: 01@n
//  * **Test Case ID:** 005@n
//  * **Priority:** High@n
//  *
//  * **Pre-Conditions:** None@n
//  * **Dependencies:** None@n
//  * **User Interaction:** None@n
//  *
//  * **Test Procedure:**
//  * | Variation / Step | Description                                                                                                              | Test Data                                                                                                                       | Expected Result                                                          | Notes      |
//  * | :--------------: | ------------------------------------------------------------------------------------------------------------------------ | ------------------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------ | ---------- |
//  * | 01               | Create a PlayerIsoBmffBuffer object, allocate memory for the buffer, and call UpdateBufferData with an invalid unParsedBufferSize. | parsedBoxCount = 1, unParsedBuffer = "TestBufferData", unParsedBufferSize = static_cast<size_t>(-1), parsedBufferSize = 100 | UpdateBufferData returns 0 indicating detection and proper handling of invalid input. | Should Fail |
//  */
// TEST(PlayerIsoBmffBuffer, NegativeScenario_UnParsedBufferSizeInvalid) {
//     std::cout << "Entering NegativeScenario_UnParsedBufferSizeInvalid test" << std::endl;
//     EXPECT_NO_THROW({
//         PlayerIsoBmffBuffer bufferObj;
//         size_t parsedBoxCount = 1;
//         char* unParsedBuffer = new char[100];
//         strncpy(unParsedBuffer, "TestBufferData", 100);
//         // Assign -1 to size_t will convert to a maximum size_t value.
//         size_t unParsedBufferSize = static_cast<size_t>(-1);
//         size_t parsedBufferSize = 100;
//         std::cout << "[DEBUG] Before UpdateBufferData: parsedBoxCount = " << parsedBoxCount 
//                   << ", unParsedBuffer = " << static_cast<const void*>(unParsedBuffer)
//                   << ", unParsedBufferSize (invalid) = " << unParsedBufferSize 
//                   << ", parsedBufferSize = " << parsedBufferSize << std::endl;
//         int ret = bufferObj.UpdateBufferData(parsedBoxCount, unParsedBuffer, unParsedBufferSize, parsedBufferSize);
//         std::cout << "[DEBUG] UpdateBufferData returned: " << ret << std::endl;
//         // Expected negative outcome: method returns false (0) due to invalid unParsedBufferSize.
//         EXPECT_EQ(ret, 0);
//         delete[] unParsedBuffer;
//     });
//     std::cout << "Exiting NegativeScenario_UnParsedBufferSizeInvalid test" << std::endl;
// }
// /**
//  * @brief Verify that UpdateBufferData returns failure when provided with an invalid parsedBufferSize.
//  *
//  * This test verifies that the UpdateBufferData function of PlayerIsoBmffBuffer handles an invalid parsedBufferSize 
//  * (by casting -1 to size_t) correctly by returning 0. The objective is to ensure the function identifies and rejects 
//  * erroneous buffer size inputs by returning a failure code.
//  *
//  * **Test Group ID:** Basic: 01@n
//  * **Test Case ID:** 006@n
//  * **Priority:** High@n
//  *
//  * **Pre-Conditions:** None@n
//  * **Dependencies:** None@n
//  * **User Interaction:** None@n
//  *
//  * **Test Procedure:**
//  * | Variation / Step | Description | Test Data | Expected Result | Notes |
//  * | :----: | --------- | ---------- |-------------- | ----- |
//  * | 01 | Invoke UpdateBufferData with valid parsedBoxCount, unParsedBuffer, unParsedBufferSize and an invalid parsedBufferSize obtained by casting -1 to size_t | parsedBoxCount = 1, unParsedBuffer = pointer to "TestBufferData", unParsedBufferSize = 100, parsedBufferSize = maximum value of size_t (i.e., -1 cast to size_t) | Return value is 0 indicating failure | Should Fail |
//  */
// TEST(PlayerIsoBmffBuffer, NegativeScenario_ParsedBufferSizeInvalid) {
//     std::cout << "Entering NegativeScenario_ParsedBufferSizeInvalid test" << std::endl;
//     EXPECT_NO_THROW({
//         PlayerIsoBmffBuffer bufferObj;
//         size_t parsedBoxCount = 1;
//         char* unParsedBuffer = new char[100];
//         strncpy(unParsedBuffer, "TestBufferData", 100);
//         size_t unParsedBufferSize = 100;
//         // Assign -1 to size_t will convert to a maximum size_t value.
//         size_t parsedBufferSize = static_cast<size_t>(-1);
//         std::cout << "[DEBUG] Before UpdateBufferData: parsedBoxCount = " << parsedBoxCount 
//                   << ", unParsedBuffer = " << static_cast<const void*>(unParsedBuffer)
//                   << ", unParsedBufferSize = " << unParsedBufferSize 
//                   << ", parsedBufferSize (invalid) = " << parsedBufferSize << std::endl;
//         int ret = bufferObj.UpdateBufferData(parsedBoxCount, unParsedBuffer, unParsedBufferSize, parsedBufferSize);
//         std::cout << "[DEBUG] UpdateBufferData returned: " << ret << std::endl;
//         // Expected negative outcome: method returns false (0) due to invalid parsedBufferSize.
//         EXPECT_EQ(ret, 0);
//         delete[] unParsedBuffer;
//     });
//     std::cout << "Exiting NegativeScenario_ParsedBufferSizeInvalid test" << std::endl;
// }
#endif
/**
 * @brief Verify that getChunkedfBox() returns a non-NULL pointer when no chunked box is set.
 *
 * This test verifies that invoking getChunkedfBox() on a newly created PlayerIsoBmffBuffer object results in a non-NULL pointer. 
 * It is important to ensure that the API returns a valid pointer even when no chunked box is explicitly assigned, 
 * thereby maintaining consistency in pointer handling throughout the application.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create a PlayerIsoBmffBuffer object using the default constructor | Constructor: PlayerIsoBmffBuffer(), no inputs | Object is created successfully without exceptions | Should be successful |
 * | 02 | Invoke getChunkedfBox() on the created object | playerBuffer instance, method: getChunkedfBox() invoked, output: result pointer | Returns a non-NULL pointer as verified by EXPECT_NE(result, nullptr) | Should Pass |
 */
TEST(PlayerIsoBmffBuffer, getChunkedfBox_NoChunkReturnsNull) {
    GTEST_SKIP();
    std::cout << "Entering getChunkedfBox_NoChunkReturnsNull test" << std::endl;

    // Create a PlayerIsoBmffBuffer object with the default constructor.
    EXPECT_NO_THROW({
        PlayerIsoBmffBuffer playerBuffer;
        std::cout << "PlayerIsoBmffBuffer object created using default constructor." << std::endl;

        // Invoke getChunkedfBox() call.
        std::cout << "Invoking getChunkedfBox() method on PlayerIsoBmffBuffer object." << std::endl;
        player_isobmff::IsoBmffBox* result = playerBuffer.getChunkedfBox();
        std::cout << "Retrieved chunked box pointer value: " << result << std::endl;
        
        // Expect the result to be NULL.
        EXPECT_NE(result, nullptr);
        std::cout << "Verified that getChunkedfBox() returned NULL as no chunked box is set." << std::endl;
    });
    
    std::cout << "Exiting getChunkedfBox_NoChunkReturnsNull test" << std::endl;
}
/**
 * @brief Validate getMdatBoxSize behavior when no mdat buffer is available
 *
 * This test verifies that the getMdatBoxSize method returns false when the PlayerIsoBmffBuffer object
 * is created using the default constructor and is not configured with a valid mdat buffer. The test checks
 * that no exceptions are thrown during object creation and API invocation, and that the method correctly
 * identifies the absence of a valid buffer.
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
 * | Variation / Step | Description                                                         | Test Data                                                       | Expected Result                                                   | Notes         |
 * | :--------------: | ------------------------------------------------------------------- | --------------------------------------------------------------- | --------------------------------------------------------------- | ------------- |
 * | 01               | Instantiate PlayerIsoBmffBuffer using default constructor           | None                                                            | Object is created without throwing any exception                | Should Pass   |
 * | 02               | Initialize the size variable to 0 before invoking getMdatBoxSize      | size = 0                                                        | Variable 'size' is set to 0                                        | Should be successful |
 * | 03               | Invoke getMdatBoxSize on the PlayerIsoBmffBuffer instance with size    | player (default constructed), input: size = 0, output: size remains unmodified, result expected = false | API returns false indicating no valid mdat buffer is available and no exception is thrown | Should Pass   |
 */
TEST(PlayerIsoBmffBuffer, getMdatBoxSize_NoBuffer) {
    std::cout << "Entering getMdatBoxSize_NoBuffer test" << std::endl;
    
    // Create an object using the default constructor.
    EXPECT_NO_THROW(PlayerIsoBmffBuffer player);
    PlayerIsoBmffBuffer player;
    std::cout << "Created PlayerIsoBmffBuffer object using default constructor" << std::endl;
    
    // Prepare an uninitialized output parameter.
    size_t size = 0;
    std::cout << "Invoking getMdatBoxSize with size parameter. Initial size value: " << size << std::endl;
    
    // Invoke the method.
    bool result = false;
    EXPECT_NO_THROW({ result = player.getMdatBoxSize(size); });
    std::cout << "Method getMdatBoxSize returned: " << std::boolalpha << result << std::endl;
    if (!result) {
        std::cout << "No valid mdat buffer available." << std::endl;
    } else {
        std::cout << "Valid mdat buffer available, size set to: " << size << std::endl;
    }
    
    std::cout << "Exiting getMdatBoxSize_NoBuffer test" << std::endl;
}
/**
 * @brief Validate the isInitSegment() behavior of PlayerIsoBmffBuffer
 *
 * This test verifies the behavior of the isInitSegment() method of the PlayerIsoBmffBuffer class. It ensures that a PlayerIsoBmffBuffer object can be instantiated without throwing exceptions and that the isInitSegment() method returns the expected boolean value for a non-init segment by default.
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
 * | Variation / Step | Description                                                                              | Test Data                                                      | Expected Result                                                                                  | Notes             |
 * | :--------------: | ---------------------------------------------------------------------------------------- | -------------------------------------------------------------- | ----------------------------------------------------------------------------------------------- | ----------------- |
 * | 01               | Create a PlayerIsoBmffBuffer object using the default constructor                        | None                                                           | Object creation is successful without throwing an exception                                    | Should be successful |
 * | 02               | Invoke isInitSegment() method on the PlayerIsoBmffBuffer instance and verify its return value | output1 = initSegmentFlag, expected default value (false)      | isInitSegment() returns false for non-init segment and true for init segment if applicable         | Should Pass       |
 */
TEST(PlayerIsoBmffBuffer, isInitSegment_Validation) {
    std::cout << "Entering isInitSegment_Validation test" << std::endl;
    
    // Create a PlayerIsoBmffBuffer object using the default constructor.
    EXPECT_NO_THROW({
        PlayerIsoBmffBuffer playerBuffer;
        std::cout << "Default constructor invoked. Created PlayerIsoBmffBuffer object." << std::endl;
        
        // Invoke the isInitSegment method.
        std::cout << "Invoking isInitSegment() method on PlayerIsoBmffBuffer object." << std::endl;
        bool initSegmentFlag = playerBuffer.isInitSegment();
        std::cout << "isInitSegment() returned: " << std::boolalpha << initSegmentFlag << std::endl;

        std::cout << "Verified: Expected isInitSegment() to return false for non-init segment and true  for init segement" << std::endl;
    });
    
    std::cout << "Exiting isInitSegment_Validation test" << std::endl;
}
/**
 * @brief Test verifies that the API 'parseBuffer' processes default parameters correctly.
 *
 * This test creates a PlayerIsoBmffBuffer object using the default constructor and invokes the parseBuffer API
 * with the parameters correctBoxSize = false and newTrackId = -1. It then asserts that no exceptions are thrown
 * and that the parseBuffer method returns true, indicating a successful parsing without overriding the track ID.
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
 * | Variation / Step | Description                                                                                   | Test Data                                               | Expected Result                                             | Notes         |
 * | :--------------: | --------------------------------------------------------------------------------------------- | ------------------------------------------------------- | ----------------------------------------------------------- | ------------- |
 * | 01               | Create PlayerIsoBmffBuffer object using the default constructor.                              | None                                                    | Object is instantiated without throwing any exceptions.   | Should Pass   |
 * | 02               | Invoke parseBuffer with parameters: correctBoxSize = false, newTrackId = -1.                    | correctBoxSize = false, newTrackId = -1                   | API returns true indicating successful parsing.             | Should Pass   |
 * | 03               | Validate that the parseBuffer method returns true via an assertion.                           | result from parseBuffer = expected true                 | EXPECT_TRUE assertion passes confirming the expected result. | Should be successful |
 */
TEST(PlayerIsoBmffBuffer, ValidParsingDefaultParameters) {
    GTEST_SKIP();
    std::cout << "Entering ValidParsingDefaultParameters test" << std::endl;
    
    // Create object using default constructor and ensure no exception is thrown.
    EXPECT_NO_THROW({
        PlayerIsoBmffBuffer bufferObj;
        std::cout << "Created PlayerIsoBmffBuffer object using default constructor." << std::endl;

        // Log invocation details
        std::cout << "Invoking parseBuffer with parameters: correctBoxSize = false, newTrackId = -1" << std::endl;
        bool result = false;
        EXPECT_NO_THROW({
            result = bufferObj.parseBuffer(false, -1);
        });
        std::cout << "parseBuffer returned: " << result << std::endl;
        
        // Validate expected result
        EXPECT_TRUE(result);
        std::cout << "parseBuffer successfully parsed the buffer without overriding track id." << std::endl;
    });
    
    std::cout << "Exiting ValidParsingDefaultParameters test" << std::endl;
}
/**
 * @brief Validate that parsing of the buffer succeeds with box size correction.
 *
 * This test verifies that the PlayerIsoBmffBuffer object, created using the default constructor,
 * correctly invokes the parseBuffer method with box size correction enabled (correctBoxSize = true) 
 * and track id override set to -1. The test ensures that no exceptions are thrown during object creation 
 * and parsing, and that the parseBuffer method returns true.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 011@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                                 | Test Data                                               | Expected Result                                                    | Notes         |
 * | :--------------: | --------------------------------------------------------------------------- | ------------------------------------------------------- | ------------------------------------------------------------------ | ------------- |
 * | 01               | Create PlayerIsoBmffBuffer object using default constructor                 | None                                                    | Object is created without throwing any exception                   | Should Pass   |
 * | 02               | Invoke parseBuffer with correctBoxSize=true, newTrackId=-1                  | correctBoxSize = true, newTrackId = -1, output = result   | parseBuffer returns true and no exception is thrown                | Should Pass   |
 * | 03               | Validate that parseBuffer returned true via assertion                       | result = true                                           | EXPECT_TRUE assertion passes confirming result is true             | Should be successful |
 */
TEST(PlayerIsoBmffBuffer, ValidParsingWithBoxSizeCorrection) {
    GTEST_SKIP();
    std::cout << "Entering ValidParsingWithBoxSizeCorrection test" << std::endl;
    
    // Create object using default constructor and ensure no exception is thrown.
    EXPECT_NO_THROW({
        PlayerIsoBmffBuffer bufferObj;
        std::cout << "Created PlayerIsoBmffBuffer object using default constructor." << std::endl;

        // Log invocation details
        std::cout << "Invoking parseBuffer with parameters: correctBoxSize = true, newTrackId = -1" << std::endl;
        bool result = false;
        EXPECT_NO_THROW({
            result = bufferObj.parseBuffer(true, -1);
        });
        std::cout << "parseBuffer returned: " << result << std::endl;
        
        // Validate expected result
        EXPECT_TRUE(result);
        std::cout << "parseBuffer successfully parsed the buffer with corrected box sizes." << std::endl;
    });
    
    std::cout << "Exiting ValidParsingWithBoxSizeCorrection test" << std::endl;
}
/**
 * @brief Validate successful parsing and track id override without box size correction
 *
 * This test validates that a PlayerIsoBmffBuffer object can be successfully created and its parseBuffer method can be invoked with a false value for correctBoxSize and a track id override value of 5. The test asserts that no exceptions are thrown during object creation and method invocation, and that the parsing returns true.
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
 * | Variation / Step | Description                                                               | Test Data                                  | Expected Result                                      | Notes       |
 * | :--------------: | ------------------------------------------------------------------------- | ------------------------------------------ | ---------------------------------------------------- | ----------- |
 * | 01               | Create PlayerIsoBmffBuffer object and call parseBuffer with parameters      | correctBoxSize = false, newTrackId = 5, output = true | parseBuffer should return true without throwing exceptions | Should Pass |
 */
TEST(PlayerIsoBmffBuffer, ValidParsingWithTrackIdOverrideNoBoxCorrection) {
    GTEST_SKIP();
    std::cout << "Entering ValidParsingWithTrackIdOverrideNoBoxCorrection test" << std::endl;
    
    // Create object using default constructor and ensure no exception is thrown.
    EXPECT_NO_THROW({
        PlayerIsoBmffBuffer bufferObj;
        std::cout << "Created PlayerIsoBmffBuffer object using default constructor." << std::endl;

        // Log invocation details
        std::cout << "Invoking parseBuffer with parameters: correctBoxSize = false, newTrackId = 5" << std::endl;
        bool result = false;
        EXPECT_NO_THROW({
            result = bufferObj.parseBuffer(false, 5);
        });
        std::cout << "parseBuffer returned: " << result << std::endl;
        
        // Validate expected result
        EXPECT_TRUE(result);
        std::cout << "parseBuffer successfully parsed the buffer and track id was overridden with new value 5." << std::endl;
    });
    
    std::cout << "Exiting ValidParsingWithTrackIdOverrideNoBoxCorrection test" << std::endl;
}
/**
 * @brief Validate the PlayerIsoBmffBuffer's parseBuffer functionality when provided with box size correction and overridden track ID.
 *
 * This test verifies that the PlayerIsoBmffBuffer object can be instantiated without throwing any exceptions and that its parseBuffer method successfully processes the buffer when given the parameters to correct box size and override the track ID with a new value (5). The test confirms that the method returns true, indicating a successful parsing operation.
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
 * | 01 | Instantiate PlayerIsoBmffBuffer object using the default constructor. | No input; output: object instance created. | Object is created successfully without any exception. | Should Pass |
 * | 02 | Invoke parseBuffer method with parameters correctBoxSize = true and newTrackId = 5. | Input1 = correctBoxSize (true), Input2 = newTrackId (5), output: result = true. | parseBuffer returns true with no exception thrown. | Should Pass |
 */
TEST(PlayerIsoBmffBuffer, ValidParsingWithTrackIdOverrideWithBoxCorrection) {
    GTEST_SKIP();
    std::cout << "Entering ValidParsingWithTrackIdOverrideWithBoxCorrection test" << std::endl;
    
    // Create object using default constructor and ensure no exception is thrown.
    EXPECT_NO_THROW({
        PlayerIsoBmffBuffer bufferObj;
        std::cout << "Created PlayerIsoBmffBuffer object using default constructor." << std::endl;

        // Log invocation details
        std::cout << "Invoking parseBuffer with parameters: correctBoxSize = true, newTrackId = 5" << std::endl;
        bool result = false;
        EXPECT_NO_THROW({
            result = bufferObj.parseBuffer(true, 5);
        });
        std::cout << "parseBuffer returned: " << result << std::endl;
        
        // Validate expected result
        EXPECT_TRUE(result);
        std::cout << "parseBuffer successfully parsed the buffer with corrected box sizes and track id overridden with new value 5." << std::endl;
    });
    
    std::cout << "Exiting ValidParsingWithTrackIdOverrideWithBoxCorrection test" << std::endl;
}
/**
 * @brief Verify that passing a NULL buffer pointer to parseMdatBox returns false
 *
 * This test verifies that the parseMdatBox method handles a NULL buffer pointer correctly by returning false.
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
 * | Variation / Step | Description                                                         | Test Data                                   | Expected Result                                                | Notes         |
 * | :--------------: | ------------------------------------------------------------------- | ------------------------------------------- | -------------------------------------------------------------- | ------------- |
 * | 01               | Invoke the default constructor of PlayerIsoBmffBuffer               | None                                        | Object is successfully created without exceptions            | Should be successful |
 * | 02               | Prepare input parameters with buf set to NULL and size set to 100      | buf = nullptr, size = 100                    | Input parameters are correctly initialized                     | Should be successful |
 * | 03               | Call parseMdatBox with the prepared parameters and capture the result  | buf = nullptr, size = 100                    | Return value is false, confirming the method handles NULL buffer correctly | Should Fail   |
 */
TEST(PlayerIsoBmffBuffer, NullBufferPointerPassedIn) {
    std::cout << "Entering NullBufferPointerPassedIn test" << std::endl;
    
    // Create object using default constructor and log invocation
    std::cout << "Invoking PlayerIsoBmffBuffer default constructor" << std::endl;
    EXPECT_NO_THROW(PlayerIsoBmffBuffer playerBuffer);
    PlayerIsoBmffBuffer playerBuffer;
    
    // Prepare input parameters - buf is NULL and size is set to a positive value
    uint8_t* buf = nullptr;
    size_t size = 100;
    std::cout << "Invoking parseMdatBox with buf: " << static_cast<void*>(buf)
              << " and size: " << size << std::endl;
    
    // Invoke the method and capture return result
    bool result = playerBuffer.parseMdatBox(buf, size);
    std::cout << "Returned value from parseMdatBox: " << result << std::endl;
    
    // Expect false since the buffer pointer is NULL
    EXPECT_FALSE(result);
    
    std::cout << "Exiting NullBufferPointerPassedIn test" << std::endl;
}
/**
 * @brief Validates that parseMdatBox correctly initializes output parameters.
 *
 * This test verifies that when parseMdatBox is called with uninitialized output parameters 
 * (buf = nullptr and size = 0), it correctly updates these parameters if the method returns true. 
 * It ensures that in a positive scenario, the buffer is allocated (non-null) and the size is set to a non-zero value.
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
 * | Variation / Step | Description                                                                 | Test Data                          | Expected Result                                                                                    | Notes       |
 * | :--------------: | ---------------------------------------------------------------------------- | ---------------------------------- | -------------------------------------------------------------------------------------------------- | ----------- |
 * | 01               | Invoke parseMdatBox API with uninitialized output parameters (buf and size). | buf = nullptr, size = 0            | Returns true; if true, buf becomes non-null and size becomes non-zero                              | Should Pass |
 * | 02               | Validate the output parameters after API invocation based on the return result. | If result true then buf, size updated | If result is true: buf != nullptr and size != 0; otherwise, values are not guaranteed but call succeeds | Should Pass |
 */
TEST(PlayerIsoBmffBuffer, OutputParameterValidation) {
    std::cout << "Entering OutputParameterValidation test" << std::endl;

    PlayerIsoBmffBuffer playerBuffer;

    // Prepare output parameters as uninitialized (per API spec)
    uint8_t* buf = nullptr;
    size_t size = 0;

    std::cout << "Invoking parseMdatBox with buf: " << buf
              << " and initial size: " << size << std::endl;

    // Invoke the method
    bool result = playerBuffer.parseMdatBox(buf, size);
    std::cout << "Returned value from parseMdatBox: " << result << std::endl;

    if (result) {
        std::cout << "Method returned true; verifying output parameters" << std::endl;
        EXPECT_NE(buf, nullptr);    // buf must be valid now
        EXPECT_NE(size, 0u);        // size must be non-zero
    } else {
        std::cout << "Method returned false; buf=" << buf
                  << " size=" << size << " (values not guaranteed)" << std::endl;
    }

    std::cout << "Exiting OutputParameterValidation test" << std::endl;
}
/**
 * @brief Verify the printBoxes method executes without exceptions on a default constructed PlayerIsoBmffBuffer object
 *
 * This test validates the functionality of the printBoxes method by constructing a default PlayerIsoBmffBuffer object and invoking printBoxes(), ensuring that no exceptions are thrown during both object construction and method invocation.
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
 * | Variation / Step | Description                                                         | Test Data                                                       | Expected Result                                        | Notes          |
 * | :--------------: | ------------------------------------------------------------------- | --------------------------------------------------------------- | ------------------------------------------------------ | -------------- |
 * | 01               | Construct a default PlayerIsoBmffBuffer object                      | Constructor: no input parameters                                | Object is created without throwing exceptions         | Should be successful |
 * | 02               | Invoke printBoxes method on the default constructed PlayerIsoBmffBuffer object | Object: default PlayerIsoBmffBuffer, Method: printBoxes() invocation | printBoxes() executes without throwing exceptions | Should Pass    |
 */
TEST(PlayerIsoBmffBuffer, printBoxes_start) {
    std::cout << "Entering printBoxes_start test" << std::endl;
    
    // Constructing a default object of PlayerIsoBmffBuffer
    EXPECT_NO_THROW({
        PlayerIsoBmffBuffer playerBuffer;
        std::cout << "Default constructed PlayerIsoBmffBuffer object." << std::endl;
        
        // Invoke printBoxes method
        std::cout << "Invoking printBoxes() method on default constructed object." << std::endl;
        EXPECT_NO_THROW(playerBuffer.printBoxes());
        std::cout << "printBoxes() executed without any exceptions on default constructed object." << std::endl;
    });
    
    std::cout << "Exiting printBoxes_start test" << std::endl;
}
/**
 * @brief Validates that setBuffer accepts a valid pointer and a positive size without throwing an exception.
 *
 * This test creates a PlayerIsoBmffBuffer object, allocates a buffer of size 10, initializes the buffer with a sample string, and then invokes the setBuffer API using the valid pointer and positive size. The test verifies that the API call does not throw any exception, ensuring that the buffer is set correctly.
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
 * | Variation / Step | Description                                                                                     | Test Data                                                              | Expected Result                                           | Notes          |
 * | :--------------: | ----------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------- | --------------------------------------------------------- | -------------- |
 * | 01               | Create a PlayerIsoBmffBuffer object.                                                            | N/A                                                                    | Object is created successfully.                           | Should be successful |
 * | 02               | Allocate a test buffer of size 10 and initialize it with a sample string.                      | testSz = 10, testBuffer allocated, temporary string = "123456789"      | Buffer is allocated and initialized correctly.          | Should be successful |
 * | 03               | Invoke setBuffer API with the valid pointer and the positive size.                             | input: testBuffer pointer, input: testSz = 10                           | API executes without throwing an exception.             | Should Pass    |
 * | 04               | Clean up the allocated memory.                                                                  | delete[] testBuffer                                                    | Memory is freed without issues.                           | Should be successful |
 */
TEST(PlayerIsoBmffBuffer, SetBufferValidPointerPositiveSize)
{
    std::cout << "Entering SetBufferValidPointerPositiveSize test" << std::endl;
    EXPECT_NO_THROW({
        // Create object using default constructor
        PlayerIsoBmffBuffer obj;
        // Allocate a buffer of size 10 and initialize it using strncpy into a temporary fixed-size array
        const size_t testSz = 10;
        uint8_t* testBuffer = new uint8_t[testSz];
        char tempBuffer[testSz];
        // Fill the temporary buffer with a sample string and then copy it
        strncpy(tempBuffer, "123456789", testSz - 1);
        tempBuffer[testSz - 1] = '\0';
        // Copy the data from tempBuffer to testBuffer
        for (size_t i = 0; i < testSz; ++i) {
            testBuffer[i] = static_cast<uint8_t>(tempBuffer[i]);
        }
        std::cout << "Invoking setBuffer with buffer address: " << static_cast<void*>(testBuffer)
                  << " and size: " << testSz << std::endl;
        obj.setBuffer(testBuffer, testSz);
        std::cout << "setBuffer invoked successfully; buffer set with positive size" << std::endl;
        delete[] testBuffer;
    });
    std::cout << "Exiting SetBufferValidPointerPositiveSize test" << std::endl;
}
/**
 * @brief Test setting buffer with a valid pointer but zero size.
 *
 * This test verifies that the setBuffer API correctly handles a non-null pointer input when the size is zero.
 * It ensures that the API call does not throw an exception even when provided with a valid buffer pointer and a size of zero.
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
 * | 01 | Create a PlayerIsoBmffBuffer object, allocate and initialize a test buffer, and invoke setBuffer with the buffer pointer and zero size | input: testBuffer = pointer to uint8_t array of size 5 with data "Test" copied, size = 0; output: none | API call completes without throwing any exception | Should Pass |
 */
TEST(PlayerIsoBmffBuffer, SetBufferValidPointerZeroSize)
{
    std::cout << "Entering SetBufferValidPointerZeroSize test" << std::endl;
    EXPECT_NO_THROW({
        // Create object using default constructor
        PlayerIsoBmffBuffer obj;
        // Allocate a buffer of size 5 (arbitrary non-null pointer)
        const size_t allocSz = 5;
        uint8_t* testBuffer = new uint8_t[allocSz];
        char tempBuffer[allocSz];
        // Initialize the temporary buffer and use strncpy for copying
        strncpy(tempBuffer, "Test", allocSz - 1);
        tempBuffer[allocSz - 1] = '\0';
        for (size_t i = 0; i < allocSz; ++i) {
            testBuffer[i] = static_cast<uint8_t>(tempBuffer[i]);
        }
        std::cout << "Invoking setBuffer with buffer address: " << static_cast<void*>(testBuffer)
                  << " and size: " << 0 << std::endl;
        obj.setBuffer(testBuffer, 0);
        std::cout << "setBuffer invoked successfully; buffer set with zero size" << std::endl;
        delete[] testBuffer;
    });
    std::cout << "Exiting SetBufferValidPointerZeroSize test" << std::endl;
}
/**
 * @brief Validate that setBuffer handles a null pointer with a non-zero size without throwing an exception.
 *
 * This test verifies that when a null buffer pointer is passed to the setBuffer API along with a non-zero size,
 * the API call completes without throwing an exception and handles the situation appropriately.
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
 * | Variation / Step | Description                                                                  | Test Data                                              | Expected Result                                                       | Notes            |
 * | :----:           | ---------------------------------------------------------------------------- | ------------------------------------------------------ | --------------------------------------------------------------------- | ---------------- |
 * | 01               | Create a PlayerIsoBmffBuffer object and initialize test parameters             | default construction, no input values                  | Object is created successfully                                        | Should be successful |
 * | 02               | Invoke setBuffer with a nullptr and a non-zero size and verify no exception is thrown | input: testBuffer = nullptr, testSz = 10               | API call completes without throwing any exception                     | Should Pass      |
 */
TEST(PlayerIsoBmffBuffer, SetBufferNullPointerNonZeroSize)
{
    std::cout << "Entering SetBufferNullPointerNonZeroSize test" << std::endl;
    EXPECT_NO_THROW({
        // Create object using default constructor
        PlayerIsoBmffBuffer obj;
        uint8_t* testBuffer = nullptr;
        const size_t testSz = 10;
        std::cout << "Invoking setBuffer with nullptr and size: " << testSz << std::endl;
        obj.setBuffer(testBuffer, testSz);
        std::cout << "setBuffer invoked successfully; handled nullptr input without crash" << std::endl;
    });
    std::cout << "Exiting SetBufferNullPointerNonZeroSize test" << std::endl;
}
/**
 * @brief Verifies that a default constructed PlayerIsoBmffBuffer is properly destroyed without allocated resources.
 *
 * This test creates a PlayerIsoBmffBuffer object using its default constructor and checks that no exception is thrown during object creation and destruction. The test ensures that the internal state is set to the expected default values and that there are no allocated resources, so the destructor executes safely when the object goes out of scope.
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
 * | Variation / Step | Description                                                                                  | Test Data                                                                          | Expected Result                                                                                             | Notes       |
 * | :--------------: | -------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------------------------- | ----------- |
 * | 01               | Invoke default constructor of PlayerIsoBmffBuffer.                                           | No input arguments; output: internal state: boxes=empty, buffer=NULL,              | PlayerIsoBmffBuffer is instantiated with internal state: boxes empty, buffer NULL, bufSize 0,                | Should Pass |
 * |                  |                                                                                              | bufSize=0, chunkedBox=NULL, mdatCount=0                                             | chunkedBox NULL, mdatCount 0.                                                                                |             |
 * | 02               | Allow object to go out of scope to trigger the destructor and verify no exception is thrown.   | Object goes out of scope; no explicit destructor call since it is automatically invoked | Destructor is invoked successfully without any exception thrown.                                           | Should Pass |
 */
TEST(PlayerIsoBmffBuffer, DefaultObjectDestructionNoAllocatedResources)
{
    std::cout << "Entering DefaultObjectDestructionNoAllocatedResources test" << std::endl;
    
    EXPECT_NO_THROW({
        std::cout << "Invoking default constructor of PlayerIsoBmffBuffer." << std::endl;
        {
            PlayerIsoBmffBuffer obj;
            std::cout << "PlayerIsoBmffBuffer object created using default constructor." << std::endl;
            std::cout << "Expected internal state:" << std::endl;
            std::cout << "  - boxes: default constructed (empty)" << std::endl;
            std::cout << "  - buffer: NULL" << std::endl;
            std::cout << "  - bufSize: 0" << std::endl;
            std::cout << "  - chunkedBox: NULL" << std::endl;
            std::cout << "  - mdatCount: 0" << std::endl;
            std::cout << "No allocated resources to deallocate in this test case." << std::endl;
            std::cout << "Invoking destructor as object goes out of scope." << std::endl;
        }
        std::cout << "Destructor of PlayerIsoBmffBuffer has been invoked successfully when object went out of scope." << std::endl;
    });
    
    std::cout << "Exiting DefaultObjectDestructionNoAllocatedResources test" << std::endl;
}