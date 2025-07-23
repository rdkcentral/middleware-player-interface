
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
* @file test_l1__base64.cpp
* @page _base64 Level 1 Tests
*
* ## Module's Role
* This module includes Level 1 functional tests (success and failure scenarios).
* This is to ensure that the _base64 methods meet the requirements.
*
* **Pre-Conditions:**  None @n
* **Dependencies:** None @n
*
*/

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <stdio.h>
#include "_base64.h"

/**
 * @brief Test the base64 encoding functionality for a typical string input.
 *
 * This test verifies that the base64_Encode API correctly encodes a typical string "Hello" without throwing exceptions. It checks that the returned encoded string matches the expected value "SGVsbG8=".
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
 * | Variation / Step | Description                                                              | Test Data                                                                                       | Expected Result                                                                           | Notes         |
 * | :--------------: | ------------------------------------------------------------------------ | ------------------------------------------------------------------------------------------------ | ----------------------------------------------------------------------------------------- | ------------- |
 * | 01               | Initialize the input buffer with the string "Hello"                      | input = "Hello", length = 5                                                                       | Input buffer is correctly initialized with "Hello"                                      | Should be successful |
 * | 02               | Invoke base64_Encode with the initialized input                          | input buffer ("Hello"), length = 5                                                                | Function returns a base64 encoded string "SGVsbG8=" without throwing exceptions           | Should Pass   |
 * | 03               | Compare the returned encoded string with the expected value ("SGVsbG8=")   | expected = "SGVsbG8=", result from base64_Encode                                                  | EXPECT_STREQ check validates that the returned string equals "SGVsbG8="                    | Should Pass   |
 * | 04               | Free the allocated memory for the result                                 | result pointer allocated by base64_Encode                                                         | Memory is freed properly without leaking                                                 | Should be successful |
 */
TEST(Base64EncodeTest, EncodeTypicalStringHello) {
    std::cout << "Entering EncodeTypicalStringHello test" << std::endl;

    char input[10];
    memset(input, 0, sizeof(input));
    strncpy(input, "Hello", sizeof(input) - 1);
    std::cout << "Input string: " << input << ", length: " << 5 << std::endl;

    std::cout << "Invoking base64_Encode with input value: " << input << std::endl;
    char* result = nullptr;
    EXPECT_NO_THROW(result = base64_Encode(reinterpret_cast<unsigned char*>(input), 5));
    std::cout << "Returned value from base64_Encode: " << (result ? result : "NULL") << std::endl;

    const char* expected = "SGVsbG8=";
    EXPECT_STREQ(expected, result);
    
    // Free the allocated memory if necessary
    if(result) {
        free(result);
    }
    
    std::cout << "Exiting EncodeTypicalStringHello test" << std::endl;
}
/**
 * @brief Validate that the base64_Encode function correctly handles an empty input.
 *
 * This test verifies that when provided with an empty string as input, the base64_Encode API does not throw an exception and returns an empty string as output. This test ensures the function gracefully handles edge cases.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 002@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :---------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Initialize an empty input buffer for encoding. | input = "", output = NULL | Empty input is prepared without error. | Should be successful |
 * | 02 | Invoke base64_Encode with the empty input. | input = "", length = 0, output = result pointer | API does not throw an exception and returns an empty string. | Should Pass |
 */
TEST(Base64EncodeTest, EncodeEmptyInput) {
    std::cout << "Entering EncodeEmptyInput test" << std::endl;

    char input[1];
    memset(input, 0, sizeof(input));
    strncpy(input, "", sizeof(input) - 1);
    std::cout << "Input string: \"" << input << "\" (empty), length: " << 0 << std::endl;

    std::cout << "Invoking base64_Encode with empty input" << std::endl;
    char* result = nullptr;
    EXPECT_NO_THROW(result = base64_Encode(reinterpret_cast<unsigned char*>(input), 0));
    std::cout << "Returned value from base64_Encode: " << (result ? result : "NULL") << std::endl;

    const char* expected = "";
    EXPECT_STREQ(expected, result);
    
    if(result) {
        free(result);
    }

    std::cout << "Exiting EncodeEmptyInput test" << std::endl;
}
/**
 * @brief Test the base64_Encode API with a valid input string.
 *
 * This test verifies that the base64_Encode function correctly encodes a valid input string "Ma". It checks for no exceptions during the function call, validates that the returned pointer is allocated, and confirms that the output matches the expected Base64 encoding "TWE=". The test also ensures proper memory deallocation to prevent leaks.
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
 * | Variation / Step | Description                                                          | Test Data                                                       | Expected Result                                                      | Notes          |
 * | :--------------: | -------------------------------------------------------------------- | --------------------------------------------------------------- | -------------------------------------------------------------------- | -------------- |
 * | 01               | Initialize the input array with "Ma" and set its length to 2         | input = "Ma", length = 2                                          | Input array is properly initialized                                  | Should be successful |
 * | 02               | Invoke base64_Encode API with the prepared input                     | input pointer = address of input, input length = 2                | No exception is thrown and result pointer is allocated               | Should Pass    |
 * | 03               | Compare the returned Base64 encoded string with expected "TWE="        | expected = "TWE=", result = returned value                        | The result string matches the expected string "TWE="                 | Should Pass    |
 * | 04               | Free the allocated memory for the result pointer                     | result pointer freed                                              | Memory is freed without issues                                       | Should be successful |
 */
TEST(Base64EncodeTest, EncodeStringvalidvalue) {
    std::cout << "Entering EncodeStringvalidvalue test" << std::endl;

    char input[10];
    memset(input, 0, sizeof(input));
    strncpy(input, "Ma", sizeof(input) - 1);
    std::cout << "Input string: " << input << ", length: " << 2 << std::endl;

    std::cout << "Invoking base64_Encode with input value: " << input << std::endl;
    char* result = nullptr;
    EXPECT_NO_THROW(result = base64_Encode(reinterpret_cast<unsigned char*>(input), 2));
    std::cout << "Returned value from base64_Encode: " << (result ? result : "NULL") << std::endl;

    const char* expected = "TWE=";
    EXPECT_STREQ(expected, result);
    
    if(result) {
        free(result);
    }

    std::cout << "Exiting EncodeStringvalidvalue test" << std::endl;
}
/**
 * @brief Validates the base64_Encode API with binary data containing special bytes.
 *
 * This test verifies that the base64_Encode function correctly encodes binary data that includes special bytes such as 0x00, 0xFF, 0xAA, and 0x55. The expected base64 encoded result is compared against a known correct value.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Initialize binary data with special bytes (0x00, 0xFF, 0xAA, 0x55) | input[0]=0x00, input[1]=0xFF, input[2]=0xAA, input[3]=0x55 | Binary data array is properly populated with the expected values | Should be successful |
 * | 02 | Invoke base64_Encode with the prepared binary data | input pointer, length=4 | Returns a non-null pointer to a valid base64 encoded string without throwing exceptions | Should Pass |
 * | 03 | Verify the returned base64 encoded string matches the expected value | expected="AP+qVQ==", result from base64_Encode | The returned string equals "AP+qVQ==" as asserted by EXPECT_STREQ | Should Pass |
 * | 04 | Free the allocated memory for the encoded result | result pointer | Memory is freed without causing any issues | Should be successful |
 */
TEST(Base64EncodeTest, EncodeBinaryDataWithSpecialBytes) {
    std::cout << "Entering EncodeBinaryDataWithSpecialBytes test" << std::endl;

    unsigned char input[10];
    memset(input, 0, sizeof(input));
    // Create binary data: 0x00, 0xFF, 0xAA, 0x55
    input[0] = 0x00;
    input[1] = 0xFF;
    input[2] = 0xAA;
    input[3] = 0x55;
    std::cout << "Input binary data bytes: ";
    for (size_t i = 0; i < 4; i++) {
        std::cout << "0x" << std::hex << static_cast<int>(input[i]) << " ";
    }
    std::cout << std::dec << ", length: " << 4 << std::endl;

    std::cout << "Invoking base64_Encode with binary data" << std::endl;
    char* result = nullptr;
    EXPECT_NO_THROW(result = base64_Encode(input, 4));
    std::cout << "Returned value from base64_Encode: " << (result ? result : "NULL") << std::endl;

    const char* expected = "AP+qVQ==";
    EXPECT_STREQ(expected, result);
    
    if(result) {
        free(result);
    }

    std::cout << "Exiting EncodeBinaryDataWithSpecialBytes test" << std::endl;
}
/**
 * @brief Verify base64_Encode gracefully handles a null input pointer.
 *
 * This test verifies that the base64_Encode API correctly processes a null input pointer by not throwing any exception and returning a null result pointer. The test is essential to ensure the API can handle invalid input without causing crashes.
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
 * | Variation / Step | Description                                                             | Test Data                                | Expected Result                                                       | Notes           |
 * | :------------: | ----------------------------------------------------------------------- | ---------------------------------------- | --------------------------------------------------------------------- | --------------- |
 * | 01             | Initialize test variables with a null pointer for input and a fixed length.         | input = nullptr, len = 5                   | Variables initialized as expected                                     | Should be successful |
 * | 02             | Invoke base64_Encode with the null input pointer ensuring no exceptions are thrown. | input = nullptr, len = 5                   | Function returns a null pointer without throwing exceptions           | Should Pass     |
 * | 03             | Check that the returned result pointer is null.                                 | result (output) = nullptr                  | Assertion confirms that the result equals nullptr                     | Should Pass     |
 */
TEST(Base64EncodeTest, HandleNullPointerInput) {
    std::cout << "Entering HandleNullPointerInput test" << std::endl;

    unsigned char* input = nullptr;
    size_t len = 5;
    std::cout << "Input pointer is NULL, length: " << len << std::endl;

    std::cout << "Invoking base64_Encode with NULL pointer" << std::endl;
    char* result = nullptr;
    EXPECT_NO_THROW(result = base64_Encode(input, len));
    std::cout << "Returned value from base64_Encode: " << (result ? result : "NULL") << std::endl;

    EXPECT_EQ(result, nullptr);

    std::cout << "Exiting HandleNullPointerInput test" << std::endl;
}
/**
 * @brief Validate correct decoding of a valid Base64 encoded string.
 *
 * This test verifies that the base64_Decode API correctly decodes a valid Base64 encoded input. It checks that the returned decoded pointer is non-null, that the decoded length is properly set to 5, and that the decoded content exactly matches the expected string "Hello". This functionality is critical for ensuring the reliability of the Base64 decoding process.
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
 * | Variation / Step | Description                                       | Test Data                                                | Expected Result                                                                             | Notes               |
 * | :--------------: | ------------------------------------------------- | -------------------------------------------------------- | ------------------------------------------------------------------------------------------ | ------------------- |
 * | 01               | Assign Base64 encoded string to src               | src = "SGVsbG8="                                         | src holds the value "SGVsbG8="                                                               | Should be successful|
 * | 02               | Invoke base64_Decode with src and &decodedLen       | src = "SGVsbG8=", decodedLen pointer = address of decodedLen | API returns a non-null pointer and decodedLen equals 5                                     | Should Pass         |
 * | 03               | Compare decoded result with expected string "Hello" | decodedResult content from API, expected "Hello"         | std::strncmp returns 0 indicating decoded string matches "Hello"                             | Should Pass         |
 */
TEST(Base64DecodeTests, ValidBase64StringDecoding) {
    std::cout << "Entering ValidBase64StringDecoding test" << std::endl;
    
    char src[16];
    std::cout << "Assigning Base64 encoded string 'SGVsbG8=' to src" << std::endl;
    std::strncpy(src, "SGVsbG8=", sizeof(src));
    
    size_t decodedLen = 0;
    std::cout << "Invoking base64_Decode with src: " << src << " and address of decodedLen: " << &decodedLen << std::endl;
    unsigned char* decodedResult = base64_Decode(src, &decodedLen);
    std::cout << "base64_Decode returned pointer: " << static_cast<void*>(decodedResult) << " with decodedLen: " << decodedLen << std::endl;
    
    EXPECT_NE(decodedResult, nullptr);
    EXPECT_EQ(decodedLen, 5);
    if(decodedResult != nullptr) {
        std::cout << "Comparing decoded result with expected 'Hello'" << std::endl;
        EXPECT_EQ(std::strncmp(reinterpret_cast<const char*>(decodedResult), "Hello", 5), 0);
        std::cout << "Decoded string is: " << std::string(reinterpret_cast<const char*>(decodedResult), decodedLen) << std::endl;
        free(decodedResult);
    }
    
    std::cout << "Exiting ValidBase64StringDecoding test" << std::endl;
}
/**
 * @brief Verify that base64_Decode correctly handles an empty string input.
 *
 * This test ensures that when provided with an empty string as input, the base64_Decode function returns a non-null pointer and sets the decoded length to 0. This is essential to confirm that the function can gracefully handle cases without any data.
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
 * | Variation / Step | Description                                                       | Test Data                                              | Expected Result                              | Notes         |
 * | :--------------: | ----------------------------------------------------------------- | ------------------------------------------------------ | -------------------------------------------- | ------------- |
 * | 01               | Initialize a character buffer and assign it an empty string ('')    | src = "", buffer size = 16                             | src contains empty string                    | Should be successful |
 * | 02               | Invoke base64_Decode with the empty input string and address of decodedLen | src = "", decodedLen pointer = &decodedLen             | Function returns non-null pointer and decodedLen equals 0 | Should Pass   |
 * | 03               | Free the allocated memory if base64_Decode returned a non-null pointer | decodedResult pointer as returned by base64_Decode     | Memory freed without error                   | Should be successful |
 */
TEST(Base64DecodeTests, EmptyStringInput) {
    std::cout << "Entering EmptyStringInput test" << std::endl;
    
    char src[16];
    std::cout << "Assigning empty string '' to src" << std::endl;
    std::strncpy(src, "", sizeof(src));
    
    size_t decodedLen = 0;
    std::cout << "Invoking base64_Decode with empty src and address of decodedLen: " << &decodedLen << std::endl;
    unsigned char* decodedResult = base64_Decode(src, &decodedLen);
    std::cout << "base64_Decode returned pointer: " << static_cast<void*>(decodedResult) << " with decodedLen: " << decodedLen << std::endl;
    
    EXPECT_NE(decodedResult, nullptr);
    EXPECT_EQ(decodedLen, 0);
    
    if(decodedResult != nullptr) {
        free(decodedResult);
    }
    
    std::cout << "Exiting EmptyStringInput test" << std::endl;
}
/**
 * @brief Verifies that base64_Decode handles a NULL source pointer gracefully.
 *
 * This test case checks whether the base64_Decode function correctly returns a NULL pointer and sets the decoded length to zero when provided with a NULL source pointer. It ensures robust error handling for invalid input.
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
 * | 01 | Invoke base64_Decode with a NULL source pointer and a valid pointer for decodedLen | src = NULL, decodedLen pointer = valid, initial decodedLen = 0 | Returns a NULL pointer and decodedLen remains 0; Assertions: decodedResult == NULL and decodedLen == 0 | Should Pass |
 */
TEST(Base64DecodeTests, NullSourcePointer) {
    std::cout << "Entering NullSourcePointer test" << std::endl;
    
    size_t decodedLen = 0;
    std::cout << "Invoking base64_Decode with src: NULL and address of decodedLen: " << &decodedLen << std::endl;
    unsigned char* decodedResult = base64_Decode(NULL, &decodedLen);
    std::cout << "base64_Decode returned pointer: " << static_cast<void*>(decodedResult) << " with decodedLen: " << decodedLen << std::endl;
    
    EXPECT_EQ(decodedResult, nullptr);
    EXPECT_EQ(decodedLen, 0);
    
    std::cout << "Exiting NullSourcePointer test" << std::endl;
}
/**
 * @brief Validate base64_Decode API handling when len pointer is NULL with valid input.
 *
 * This test verifies that the base64_Decode function correctly returns a nullptr when provided with a valid Base64 encoded string and a NULL pointer for the length parameter. It ensures that the API can handle cases where the length of the decoded output is not required, hence improving robustness.
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
 * | Variation / Step | Description                                                                              | Test Data                                       | Expected Result                                        | Notes          |
 * | :--------------: | ---------------------------------------------------------------------------------------- | ----------------------------------------------- | ------------------------------------------------------ | -------------- |
 * | 01               | Initialize src with Base64 encoded string 'SGVsbG8='                                    | src = "SGVsbG8="                                | src is correctly populated                             | Should be successful |
 * | 02               | Invoke base64_Decode with src: "SGVsbG8=" and NULL length pointer                         | input: src = "SGVsbG8=", len = NULL               | Returns nullptr                                        | Should Pass    |
 * | 03               | Validate via EXPECT_EQ that the returned decodedResult is nullptr                         | output: decodedResult = nullptr                  | Assertion returns true confirming decodedResult is nullptr | Should be successful |
 */
TEST(Base64DecodeTests, NullLenPointerWithValidInput) {
    std::cout << "Entering NullLenPointerWithValidInput test" << std::endl;
    
    char src[16];
    std::cout << "Assigning Base64 encoded string 'SGVsbG8=' to src" << std::endl;
    std::strncpy(src, "SGVsbG8=", sizeof(src));
    
    std::cout << "Invoking base64_Decode with src: " << src << " and len pointer: NULL" << std::endl;
    unsigned char* decodedResult = base64_Decode(src, NULL);
    std::cout << "base64_Decode returned pointer: " << static_cast<void*>(decodedResult) << std::endl;
    
    EXPECT_EQ(decodedResult, nullptr);
    
    std::cout << "Exiting NullLenPointerWithValidInput test" << std::endl;
}
/**
 * @brief Test for proper handling of illegal characters in a Base64 string
 *
 * This test verifies that the base64_Decode function returns a null pointer and sets decodedLen to 0 when provided with an input string that contains characters not allowed in Base64 encoding.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- | -------------- | ----- |
 * | 01 | Assign an invalid Base64 string "Invalid!!" to the src buffer | src = "Invalid!!" | src contains "Invalid!!" correctly | Should be successful |
 * | 02 | Invoke base64_Decode with src and the address of decodedLen | input: src = "Invalid!!", decodedLen address; output: decodedResult pointer, decodedLen value | base64_Decode returns nullptr and decodedLen equals 0 | Should Pass |
 * | 03 | Validate API behavior by asserting the result pointer and decoded length | EXPECT_EQ(decodedResult, nullptr), EXPECT_EQ(decodedLen, 0) | Both assertions pass confirming proper error handling | Should Pass |
 */
TEST(Base64DecodeTests, InvalidBase64StringIllegalCharacters) {
    std::cout << "Entering InvalidBase64StringIllegalCharacters test" << std::endl;
    
    char src[16];
    std::cout << "Assigning invalid Base64 string 'Invalid!!' to src" << std::endl;
    std::strncpy(src, "Invalid!!", sizeof(src));
    
    size_t decodedLen = 0;
    std::cout << "Invoking base64_Decode with src: " << src << " and address of decodedLen: " << &decodedLen << std::endl;
    unsigned char* decodedResult = base64_Decode(src, &decodedLen);
    std::cout << "base64_Decode returned pointer: " << static_cast<void*>(decodedResult) << " with decodedLen: " << decodedLen << std::endl;
    
    EXPECT_EQ(decodedResult, nullptr);
    EXPECT_EQ(decodedLen, 0);
    
    if(decodedResult != nullptr) {
        free(decodedResult);
    }
    
    std::cout << "Exiting InvalidBase64StringIllegalCharacters test" << std::endl;
}
/**
 * @brief Validate that base64_Decode returns nullptr and zero decoded length for a Base64 string with missing padding.
 *
 * This test verifies that when a valid Base64 string missing the necessary padding is decoded by base64_Decode, the function returns a nullptr and the decoded length remains zero. This ensures the API properly handles input errors related to incorrect padding.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Log entering the test message | None | Console outputs "Entering ValidBase64StringWithIncorrectPadding test" | Should be successful |
 * | 02 | Assign a Base64 string with missing padding "SGVsbG8" to src | src = SGVsbG8 | src is correctly assigned with the value "SGVsbG8" | Should be successful |
 * | 03 | Invoke base64_Decode with src and pointer to decodedLen | src = SGVsbG8, decodedLen = 0 | Returns nullptr and decodedLen remains 0 | Should Pass |
 * | 04 | Validate the returned pointer and decoded length using assertions | decodedResult = nullptr, decodedLen = 0 | EXPECT_EQ assertions pass confirming nullptr and zero length | Should Pass |
 * | 05 | Free the decoded memory if it is allocated | decodedResult pointer | Memory is freed if allocated; No action if decodedResult is nullptr | Should be successful |
 */
TEST(Base64DecodeTests, ValidBase64StringWithIncorrectPadding) {
    std::cout << "Entering ValidBase64StringWithIncorrectPadding test" << std::endl;
    
    char src[16];
    std::cout << "Assigning Base64 string with missing padding 'SGVsbG8' to src" << std::endl;
    std::strncpy(src, "SGVsbG8", sizeof(src));
    
    size_t decodedLen = 0;
    std::cout << "Invoking base64_Decode with src: " << src << " and address of decodedLen: " << &decodedLen << std::endl;
    unsigned char* decodedResult = base64_Decode(src, &decodedLen);
    std::cout << "base64_Decode returned pointer: " << static_cast<void*>(decodedResult) << " with decodedLen: " << decodedLen << std::endl;
    
    EXPECT_EQ(decodedResult, nullptr);
    EXPECT_EQ(decodedLen, 0);
    
    if(decodedResult != nullptr) {
        free(decodedResult);
    }
    
    std::cout << "Exiting ValidBase64StringWithIncorrectPadding test" << std::endl;
}
/**
 * @brief Test the proper decoding of a valid Base64 encoded string.
 *
 * This test verifies that the base64_Decode function correctly decodes a valid Base64 encoded input ("TWFu") to the expected output ("Man"). 
 * It confirms that the function does not throw exceptions, returns a non-null pointer, and produces the correct decoded length and string value.
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
 * | Variation / Step | Description                                                                           | Test Data                                                          | Expected Result                                                                           | Notes         |
 * | :--------------: | ------------------------------------------------------------------------------------- | ------------------------------------------------------------------ | ----------------------------------------------------------------------------------------- | ------------- |
 * | 01               | Initialize the src array with the value "TWFu".                                       | src = "TWFu"                                                       | src array contains "TWFu" with proper null termination.                                   | Should be successful |
 * | 02               | Invoke base64_Decode with src "TWFu" and srcLen 4.                                    | input: src = "TWFu", srcLen = 4                                      | Function does not throw an exception and returns a valid pointer with decodedLen updated. | Should Pass   |
 * | 03               | Validate that the decoded pointer is not null and the decoded length equals 3.          | output: decoded pointer, decodedLen = 3                           | decoded pointer is not nullptr and decodedLen equals 3.                                   | Should be successful |
 * | 04               | Verify that the decoded string equals "Man" and free the allocated memory.              | output: decoded string = "Man"                                      | Decoded string matches "Man".                                                               | Should be successful |
 */
TEST(Base64DecodeTest, ValidBase64DecodeProperInput) {
    std::cout << "Entering ValidBase64DecodeProperInput test" << std::endl;
    
    char src[10];
    std::cout << "Initializing src with value \"TWFu\"" << std::endl;
    strncpy(src, "TWFu", sizeof(src) - 1);
    src[sizeof(src) - 1] = '\0';
    
    size_t decodedLen = 0;
    std::cout << "Calling base64_Decode with src: " << src << ", srcLen: 4" << std::endl;
    unsigned char *decoded = nullptr;
    EXPECT_NO_THROW(decoded = base64_Decode(src, &decodedLen, 4));
    
    std::cout << "Returned pointer: " << static_cast<void*>(decoded) << ", decodedLen: " << decodedLen << std::endl;
    
    EXPECT_NE(decoded, nullptr);
    EXPECT_EQ(decodedLen, 3);
    std::string result(reinterpret_cast<char*>(decoded), decodedLen);
    std::cout << "Decoded string: " << result << std::endl;
    EXPECT_EQ(result, "Man");
    
    // Free the memory if allocated by base64_Decode
    if(decoded) {
        free(decoded);
        decoded = nullptr;
    }
    
    std::cout << "Exiting ValidBase64DecodeProperInput test" << std::endl;
}
/**
 * @brief Validate proper decoding of a Base64 string with padding via base64_Decode API.
 *
 * This test validates that the base64_Decode API correctly decodes a valid Base64 encoded string "U29mdHdhcmU=" with proper padding.
 * It verifies that the decoded output length matches the expected value (8), ensures the decoded pointer is not null,
 * and confirms that the decoded string matches "Software".
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
 * | 01 | Initialize the source buffer with the Base64 encoded string. | src = "U29mdHdhcmU=" (copied into a char array of size 20) | src is properly initialized with the expected string. | Should be successful |
 * | 02 | Invoke base64_Decode API with the source string. | input: src = "U29mdHdhcmU=", srcLen = 12; output: decodedLen pointer | base64_Decode returns a non-null pointer and sets decodedLen to 8. | Should Pass |
 * | 03 | Validate the decoded output by converting the buffer to a string. | output: result = "Software", decodedLen = 8 | The decoded string matches "Software". | Should Pass |
 * | 04 | Free the allocated memory for the decoded output. | decoded pointer to be freed | Memory is released successfully and decoded pointer is reset to nullptr. | Should be successful |
 */
TEST(Base64DecodeTest, ValidBase64DecodeWithPadding) {
    std::cout << "Entering ValidBase64DecodeWithPadding test" << std::endl;
    
    char src[20];
    std::cout << "Initializing src with value \"U29mdHdhcmU=\"" << std::endl;
    strncpy(src, "U29mdHdhcmU=", sizeof(src) - 1);
    src[sizeof(src) - 1] = '\0';
    
    size_t decodedLen = 0;
    std::cout << "Calling base64_Decode with src: " << src << ", srcLen: 12" << std::endl;
    unsigned char *decoded = nullptr;
    EXPECT_NO_THROW(decoded = base64_Decode(src, &decodedLen, 12));
    
    std::cout << "Returned pointer: " << static_cast<void*>(decoded) << ", decodedLen: " << decodedLen << std::endl;
    
    EXPECT_NE(decoded, nullptr);
    EXPECT_EQ(decodedLen, 8);
    std::string result(reinterpret_cast<char*>(decoded), decodedLen);
    std::cout << "Decoded string: " << result << std::endl;
    EXPECT_EQ(result, "Software");
    
    if(decoded) {
        free(decoded);
        decoded = nullptr;
    }
    
    std::cout << "Exiting ValidBase64DecodeWithPadding test" << std::endl;
}
/**
 * @brief Verify that providing an empty Base64 string returns a valid pointer with a decoded length of 0
 *
 * This test validates that when an empty string is provided to the base64_Decode function, the function returns a non-null pointer and sets the decoded length to 0. This ensures that the API handles empty inputs gracefully without crashing.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Initialize the source with an empty string and set a non-zero initial value for decodedLen | src = "", decodedLen = 123 | src is empty and decodedLen is initialized | Should be successful |
 * | 02 | Call base64_Decode with the empty src and a src length of 0, then verify that the function returns a valid pointer and decodedLen is updated to 0 | input: src = "", input: srcLen = 0, initial decodedLen = 123; output: decoded pointer, updated decodedLen = 0 | Base64 decoding returns a valid pointer and decodedLen equals 0 | Should Pass |
 * | 03 | Free the allocated memory for the decoded pointer, if any | decoded pointer | Memory is freed without errors | Should be successful |
 */
TEST(Base64DecodeTest, EmptyBase64String) {
    std::cout << "Entering EmptyBase64String test" << std::endl;
    
    char src[1];
    std::cout << "Initializing src with empty string \"\"" << std::endl;
    strncpy(src, "", sizeof(src) - 1);
    src[sizeof(src) - 1] = '\0';
    
    size_t decodedLen = 123; // arbitrary initial non-zero value
    std::cout << "Calling base64_Decode with empty src, srcLen: 0" << std::endl;
    unsigned char *decoded = nullptr;
    EXPECT_NO_THROW(decoded = base64_Decode(src, &decodedLen, 0));
    
    std::cout << "Returned pointer: " << static_cast<void*>(decoded) << ", decodedLen: " << decodedLen << std::endl;
    
    // For empty input, a valid empty pointer is expected; decoded pointer can be non-NULL representing empty data,
    // but *len must be 0.
    EXPECT_NE(decoded, nullptr);
    EXPECT_EQ(decodedLen, 0);
    
    if(decoded) {
        free(decoded);
        decoded = nullptr;
    }
    
    std::cout << "Exiting EmptyBase64String test" << std::endl;
}
/**
 * @brief Verify that base64_Decode properly handles a NULL source pointer input.
 *
 * This test verifies that when a NULL source pointer is passed to base64_Decode with a srcLen of 0, the function does not throw any exceptions and returns a null pointer with the decoded length set to 0. This confirms that the API gracefully handles null inputs.
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
 * | Variation / Step | Description                                                           | Test Data                                             | Expected Result                                                | Notes           |
 * | :--------------: | --------------------------------------------------------------------- | ----------------------------------------------------- | -------------------------------------------------------------- | --------------- |
 * | 01               | Initialize decodedLen with 999 and src pointer as NULL                | decodedLen = 999, src = NULL                            | Variables initialized correctly                                | Should be successful |
 * | 02               | Invoke base64_Decode with src as NULL and srcLen as 0                  | src = NULL, srcLen = 0, decodedLen initial value = 999    | API does not throw, processes input                              | Should Pass     |
 * | 03               | Verify that the returned decoded pointer is nullptr                  | output: decoded pointer from API                       | decoded equals nullptr                                           | Should Pass     |
 * | 04               | Verify that the decoded length is set to 0                             | output: decodedLen from API                            | decodedLen equals 0                                               | Should Pass     |
 */
TEST(Base64DecodeTest, NullSrcPointer) {
    std::cout << "Entering NullSrcPointer test" << std::endl;
    
    size_t decodedLen = 999; // initial non-zero value to see if it gets set to 0
    const char *src = NULL;
    std::cout << "Calling base64_Decode with src as NULL and srcLen: 0" << std::endl;
    unsigned char *decoded = nullptr;
    EXPECT_NO_THROW(decoded = base64_Decode(src, &decodedLen, 0));
    
    std::cout << "Returned pointer: " << static_cast<void*>(decoded) << ", decodedLen: " << decodedLen << std::endl;
    
    EXPECT_EQ(decoded, nullptr);
    EXPECT_EQ(decodedLen, 0);
    
    std::cout << "Exiting NullSrcPointer test" << std::endl;
}
/**
 * @brief Test the base64_Decode API when the len pointer is NULL
 *
 * This test verifies that when a NULL pointer is passed for the length parameter to base64_Decode along with a valid source string "TWFu" and a source length of 4, the function does not throw an exception and returns a nullptr. This confirms that the function handles NULL len pointers gracefully.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Initialize src array with the value "TWFu" ensuring null termination | src = "TWFu", array size = 10 | src properly contains "TWFu" with null termination | Should be successful |
 * | 02 | Invoke base64_Decode with len pointer as NULL and srcLen as 4 | input1 = src ("TWFu"), input2 = NULL, input3 = 4 | API should not throw an exception and returns a nullptr | Should Pass |
 * | 03 | Validate the returned decoded pointer equals nullptr | decoded = returned pointer | EXPECT_EQ(decoded, nullptr) passes | Should Pass |
 */
TEST(Base64DecodeTest, NullLenPointer) {
    std::cout << "Entering NullLenPointer test" << std::endl;
    
    char src[10];
    std::cout << "Initializing src with value \"TWFu\"" << std::endl;
    strncpy(src, "TWFu", sizeof(src) - 1);
    src[sizeof(src) - 1] = '\0';
    
    std::cout << "Calling base64_Decode with len pointer as NULL and srcLen: 4" << std::endl;
    unsigned char *decoded = nullptr;
    EXPECT_NO_THROW(decoded = base64_Decode(src, NULL, 4));
    
    std::cout << "Returned pointer: " << static_cast<void*>(decoded) << std::endl;
    EXPECT_EQ(decoded, nullptr);
    
    std::cout << "Exiting NullLenPointer test" << std::endl;
}
/**
 * @brief Verify that the base64_Decode function handles a source length that is too short.
 *
 * This test checks whether the base64_Decode API correctly returns a nullptr for the decoded buffer and a decoded length of 0 when the provided source length is less than required, ensuring proper handling of invalid input lengths.
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
 * | Variation / Step | Description                                                                       | Test Data                                           | Expected Result                                                                  | Notes         |
 * | :--------------: | --------------------------------------------------------------------------------- | --------------------------------------------------- | -------------------------------------------------------------------------------- | ------------- |
 * | 01               | Initialize src array with the value "TWFu".                                       | src = "TWFu"                                        | src array is correctly initialized with the string "TWFu".                       | Should be successful |
 * | 02               | Call base64_Decode with src, initial decodedLen = 555, and srcLen = 2 (too short).  | src = "TWFu", decodedLen = 555, srcLen = 2           | Returned pointer is nullptr and decodedLen is set to 0 with no exception thrown. | Should Pass   |
 * | 03               | Verify that the returned pointer is nullptr and decodedLen equals 0 using EXPECT.   | Returned pointer, decodedLen                        | The EXPECT_EQ checks confirm that decoded is nullptr and decodedLen is 0.         | Should be successful |
 */
TEST(Base64DecodeTest, SrcLenTooShort) {
    std::cout << "Entering SrcLenTooShort test" << std::endl;
    
    char src[10];
    std::cout << "Initializing src with value \"TWFu\"" << std::endl;
    strncpy(src, "TWFu", sizeof(src) - 1);
    src[sizeof(src) - 1] = '\0';
    
    size_t decodedLen = 555; // initial non-zero value
    std::cout << "Calling base64_Decode with src: " << src << " and srcLen: 2 (too short)" << std::endl;
    unsigned char *decoded = nullptr;
    EXPECT_NO_THROW(decoded = base64_Decode(src, &decodedLen, 2));
    
    std::cout << "Returned pointer: " << static_cast<void*>(decoded) << ", decodedLen: " << decodedLen << std::endl;
    
    EXPECT_EQ(decoded, nullptr);
    EXPECT_EQ(decodedLen, 0);
    
    std::cout << "Exiting SrcLenTooShort test" << std::endl;
}
/**
 * @brief Verify that base64_Decode handles invalid base64 characters correctly
 *
 * This test case evaluates the handling of invalid characters in a base64 encoded string.
 * The function base64_Decode is called with an input containing invalid base64 characters ("??==").
 * The test ensures that no exception is thrown during the call, and that the function returns a null pointer
 * while setting the decoded length to 0.
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
 * | 01 | Initialize the src array with the invalid base64 string "??==" | src = "??==", buffer size = 10, initial decodedLen = 321 | src contains "??==" properly terminated with '\0' | Should be successful |
 * | 02 | Call base64_Decode with the invalid src and srcLen of 4 | input: src = "??==", srcLen = 4, decodedLen = 321 | Function does not throw any exception and returns a decoded pointer | Should Pass |
 * | 03 | Validate that the returned decoded pointer is null | decoded pointer returned from base64_Decode | decoded pointer is nullptr | Should Pass |
 * | 04 | Validate that decodedLen is updated to 0 | decodedLen after function call | decodedLen equals 0 | Should Pass |
 */
TEST(Base64DecodeTest, InvalidBase64Characters) {
    std::cout << "Entering InvalidBase64Characters test" << std::endl;
    
    char src[10];
    std::cout << "Initializing src with value \"??==\"" << std::endl;
    strncpy(src, "??==", sizeof(src) - 1);
    src[sizeof(src) - 1] = '\0';
    
    size_t decodedLen = 321; // initial arbitrary value
    std::cout << "Calling base64_Decode with src: " << src << ", srcLen: 4" << std::endl;
    unsigned char *decoded = nullptr;
    EXPECT_NO_THROW(decoded = base64_Decode(src, &decodedLen, 4));
    
    std::cout << "Returned pointer: " << static_cast<void*>(decoded) << ", decodedLen: " << decodedLen << std::endl;
    
    EXPECT_EQ(decoded, nullptr);
    EXPECT_EQ(decodedLen, 0);
    
    std::cout << "Exiting InvalidBase64Characters test" << std::endl;
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
