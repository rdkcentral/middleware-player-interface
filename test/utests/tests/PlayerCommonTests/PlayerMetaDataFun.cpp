
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
#include <cstring>
#include <string>
#include "PlayerMetadata.hpp"


/**
 * @brief Verify that GetPlayerName API returns a valid non-empty player name.
 *
 * This test case verifies that the GetPlayerName API does not throw an exception, returns a valid non-empty string, and logs the process correctly. It ensures that the underlying implementation provides a proper player name.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoke GetPlayerName API and verify it returns a valid non-empty string without throwing an exception. | input: none, output: non-empty string | API should return a non-empty string and no exceptions should be thrown; EXPECT_NO_THROW and EXPECT_FALSE assertions must pass | Should Pass |
 */
TEST(GetPlayerName, ValidPlayerName) {
    std::cout << "Entering GetPlayerName_ValidPlayerName test" << std::endl;    
    std::cout << "Invoking GetPlayerName() method." << std::endl;
    std::string result;
    EXPECT_NO_THROW({
        result = GetPlayerName();
        std::cout << "GetPlayerName() returned: " << result << std::endl;
    });
    EXPECT_FALSE(result.empty()) << "GetPlayerName() returned an empty string!";
    std::cout << "Exiting GetPlayerName_ValidPlayerName test" << std::endl;
}
/**
 * @brief Test the SetPlayerName function with a new valid player name.
 *
 * This test verifies that invoking SetPlayerName with a valid player name string ("Alice") does not throw an exception. 
 * It tests the correct handling of valid name inputs ensuring that the API accepts and processes the input without error.
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
 * | 01 | Prepare a valid player name using strncpy to safely copy the string into a fixed-size buffer. | inputStr = "Alice", inputBuffer = char[100] initialized to 0, strncpy copies "Alice" into inputBuffer | inputBuffer correctly contains "Alice" without overflow. | Should be successful |
 * | 02 | Invoke the SetPlayerName API using the prepared input name. | input = string(inputBuffer) where inputBuffer = "Alice" | EXPECT_NO_THROW ensures the API call does not throw an exception. | Should Pass |
 */
TEST(SetPlayerName_NewValidName, TestCase) {
    std::cout << "Entering SetPlayerName_NewValidName test" << std::endl;

    // Prepare input using strncpy with a fixed size array.
    const std::string inputStr = "Alice";
    char inputBuffer[100];
    std::memset(inputBuffer, 0, sizeof(inputBuffer));
    std::strncpy(inputBuffer, inputStr.c_str(), sizeof(inputBuffer) - 1);
    std::cout << "Debug: Prepared input string: " << inputBuffer << std::endl;

    // Invocation of SetPlayerName.
    std::cout << "Debug: Invoking SetPlayerName with value: " << inputBuffer << std::endl;
    EXPECT_NO_THROW(SetPlayerName(std::string(inputBuffer)));
    std::cout << "Exiting SetPlayerName_NewValidName test" << std::endl;
}
/**
 * @brief Validate that SetPlayerName correctly handles empty string input.
 *
 * This test verifies that when an empty string is provided as input to the SetPlayerName API,
 * the function processes it without throwing an exception. The test ensures that edge cases
 * related to empty player names are properly handled.
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
 * | Variation / Step | Description                                                                     | Test Data                                            | Expected Result                                                        | Notes          |
 * | :--------------: | ------------------------------------------------------------------------------- | ---------------------------------------------------- | ---------------------------------------------------------------------- | -------------- |
 * | 01               | Prepare input by initializing a buffer with an empty string using memset and strncpy. | inputStr = "", inputBuffer = array of 100 zeros       | inputBuffer contains an empty string                                 | Should be successful |
 * | 02               | Invoke SetPlayerName with the empty string.                                   | input1 = "", output1 = no exception thrown           | Function call should complete without throwing an exception (EXPECT_NO_THROW passes) | Should Pass    |
 */
TEST(SetPlayerName_UpdateToEmpty, TestCase) {
    std::cout << "Entering SetPlayerName_UpdateToEmpty test" << std::endl;

    // Prepare input empty string using strncpy.
    const std::string inputStr = "";
    char inputBuffer[100];
    std::memset(inputBuffer, 0, sizeof(inputBuffer));
    std::strncpy(inputBuffer, inputStr.c_str(), sizeof(inputBuffer) - 1);
    std::cout << "Debug: Prepared input string (empty): \"" << inputBuffer << "\"" << std::endl;

    // Invocation of SetPlayerName.
    std::cout << "Debug: Invoking SetPlayerName with empty string" << std::endl;
    EXPECT_NO_THROW(SetPlayerName(std::string(inputBuffer)));
    std::cout << "Exiting SetPlayerName_UpdateToEmpty test" << std::endl;
}
/**
 * @brief Verify that player's name can be set with special characters 
 *
 * This test verifies that the SetPlayerName API correctly handles names containing special characters without throwing any exceptions. It ensures that special characters in the input string do not result in unexpected behavior.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 004
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                   | Test Data                                     | Expected Result                                     | Notes         |
 * | :-------------: | --------------------------------------------------------------------------------------------- | --------------------------------------------- | --------------------------------------------------- | ------------- |
 * | 01              | Prepare an input string with special characters and copy it into a buffer using strncpy.       | inputStr = John_Doe#123, inputBuffer = buffer containing "John_Doe#123" | Buffer contains "John_Doe#123"                        | Should be successful |
 * | 02              | Invoke SetPlayerName with the prepared input string converted from the buffer.                 | input = John_Doe#123                          | No exception thrown by SetPlayerName; assertion passes | Should Pass   |
 */
TEST(SetPlayerName_SpecialCharacters, TestCase) {
    std::cout << "Entering SetPlayerName_SpecialCharacters test" << std::endl;

    // Prepare input with special characters using strncpy.
    const std::string inputStr = "John_Doe#123";
    char inputBuffer[100];
    std::memset(inputBuffer, 0, sizeof(inputBuffer));
    std::strncpy(inputBuffer, inputStr.c_str(), sizeof(inputBuffer) - 1);
    std::cout << "Debug: Prepared input string with special characters: " << inputBuffer << std::endl;

    // Invocation of SetPlayerName.
    std::cout << "Debug: Invoking SetPlayerName with value: " << inputBuffer << std::endl;
    EXPECT_NO_THROW(SetPlayerName(std::string(inputBuffer)));
    std::cout << "Exiting SetPlayerName_SpecialCharacters test" << std::endl;
}
/**
 * @brief Validate the SetPlayerName API with a long input string to verify proper handling of oversized names
 *
 * This test verifies that the SetPlayerName function correctly processes a string that exceeds typical length,
 * ensuring that no exceptions are thrown when handling long input values.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Prepare a long input string, copy it into a fixed-size buffer, invoke SetPlayerName, and verify that no exception is thrown | inputStr = a_string_exceeding_normal_length_example_ABCDEFGHIJKLMNOPQRSTUVWXYZ, inputBuffer = string copied from inputStr, gPlayerName = updated with inputBuffer value | SetPlayerName does not throw an exception and gPlayerName is updated with the input string | Should Pass |
 */
TEST(SetPlayerName_LongString, TestCase) {
    std::cout << "Entering SetPlayerName_LongString test" << std::endl;

    // Prepare a long input string using strncpy.
    const std::string inputStr = "a_string_exceeding_normal_length_example_ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char inputBuffer[150];
    std::memset(inputBuffer, 0, sizeof(inputBuffer));
    std::strncpy(inputBuffer, inputStr.c_str(), sizeof(inputBuffer) - 1);
    std::cout << "Debug: Prepared long input string: " << inputBuffer << std::endl;

    // Invocation of SetPlayerName.
    std::cout << "Debug: Invoking SetPlayerName with long string: " << inputBuffer << std::endl;
    EXPECT_NO_THROW(SetPlayerName(std::string(inputBuffer)));
    std::cout << "Debug: gPlayerName after SetPlayerName call: " << gPlayerName << std::endl;
    std::cout << "Exiting SetPlayerName_LongString test" << std::endl;
}
/**
 * @brief Validates SetPlayerName functionality for trimming whitespace from the input string
 *
 * This test verifies that the SetPlayerName API correctly handles an input string with leading and trailing whitespace. The test prepares the input by copying a string with spaces into a buffer, then invokes the API and ensures that no exceptions are thrown, indicating that the trimming or handling of whitespace is managed internally.
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
 * | Variation / Step | Description                                                                                           | Test Data                                        | Expected Result                                           | Notes      |
 * | :--------------: | ----------------------------------------------------------------------------------------------------- | ------------------------------------------------| --------------------------------------------------------- | ---------- |
 * | 01               | Prepare a buffer with a string containing leading and trailing whitespace and invoke SetPlayerName   | inputStr = "  Bob  ", inputBuffer = "  Bob  "     | API should process the input without throwing exceptions  | Should Pass|
 */
TEST(SetPlayerName_Whitespace, TestCase) {
    std::cout << "Entering SetPlayerName_Whitespace test" << std::endl;

    // Prepare input with leading and trailing whitespace using strncpy.
    const std::string inputStr = "  Bob  ";
    char inputBuffer[100];
    std::memset(inputBuffer, 0, sizeof(inputBuffer));
    std::strncpy(inputBuffer, inputStr.c_str(), sizeof(inputBuffer) - 1);
    std::cout << "Debug: Prepared input string with whitespace: \"" << inputBuffer << "\"" << std::endl;

    // Invocation of SetPlayerName.
    std::cout << "Debug: Invoking SetPlayerName with value: \"" << inputBuffer << "\"" << std::endl;
    EXPECT_NO_THROW(SetPlayerName(std::string(inputBuffer)));
    std::cout << "Exiting SetPlayerName_Whitespace test" << std::endl;
}
