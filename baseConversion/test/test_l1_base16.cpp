
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
* @file test_l1_base16.cpp
* @page base16 Level 1 Tests
*
* ## Module's Role
* This module includes Level 1 functional tests (success and failure scenarios).
* This is to ensure that the base16 methods meet the requirements.
*
* **Pre-Conditions:**  None @n
* **Dependencies:** None @n
*
*/

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <stdio.h>
#include "base16.h"

/**
 * @brief Test base16_Encode with a valid non-empty input array to verify correct hexadecimal conversion
 *
 * This test case verifies that when base16_Encode is provided with a valid non-empty byte array (representing "ABC"),
 * it correctly converts the input to its hexadecimal string equivalent ("414243"). The test ensures that the function call
 * does not throw an exception, that the returned pointer is not NULL, and that the resulting string matches the expected output.
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
 * | Variation / Step | Description                                                                 | Test Data                                                     | Expected Result                                | Notes           |
 * | :--------------: | --------------------------------------------------------------------------- | ------------------------------------------------------------- | ---------------------------------------------- | --------------- |
 * | 01               | Initialize test: log entry message and prepare input array                  | src = {0x41, 0x42, 0x43}, len = 3                                | Input array is prepared                        | Should be successful |
 * | 02               | Invoke base16_Encode with the given input                                   | input: src = {0x41, 0x42, 0x43}, len = 3; output: result pointer  | No exception thrown; valid result pointer      | Should Pass     |
 * | 03               | Compare the returned result with the expected hexadecimal string "414243"   | result pointer, expected = "414243"                           | result equals "414243"                         | Should Pass     |
 * | 04               | Deallocate the allocated memory for result if not NULL                      | result pointer                                                | Memory is freed successfully                   | Should be successful |
 * | 05               | Log exit message for test completion                                        | None                                                          | Exit message logged                            | Should be successful |
 */
TEST(Base16Encode_ValidNonEmpty, TestCase1) {
    std::cout << "Entering Base16Encode_ValidNonEmpty test\n";
    unsigned char src[] = {0x41, 0x42, 0x43}; // "ABC"
    size_t len = sizeof(src);
    std::cout << "Invoking base16_Encode with src bytes: [0x41, 0x42, 0x43] and len: " << len << "\n";

    char* result = nullptr;
    EXPECT_NO_THROW(result = base16_Encode(src, len));
    std::cout << "Returned string pointer: " << (result ? result : (char*)"NULL") << "\n";

    // Validate that result is not NULL and matches "414243"
    EXPECT_NE(result, nullptr);
    std::cout << "Comparing result with expected \"414243\"\n";
    EXPECT_STREQ(result, "414243");

    // Free the allocated memory if result is not NULL
    if(result) {
        free(result);
        std::cout << "Freed allocated memory for result\n";
    }
    std::cout << "Exiting Base16Encode_ValidNonEmpty test\n";
}
/**
 * @brief Verify that base16_Encode returns an empty string for zero-length input.
 *
 * This test verifies that when a valid non-NULL pointer is provided with a length of zero,
 * the base16_Encode function does not throw an exception, returns a non-NULL pointer,
 * and produces an empty string. This behavior ensures that the API correctly handles empty input.
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
 * | Variation / Step | Description                                                                 | Test Data                                                     | Expected Result                                                                          | Notes             |
 * | :--------------: | --------------------------------------------------------------------------- | ------------------------------------------------------------- | ---------------------------------------------------------------------------------------- | ----------------- |
 * | 01               | Initialize valid pointer and set input length to zero                       | dummy = 0, src = address of dummy, len = 0                      | Variables properly initialized                                                           | Should be successful  |
 * | 02               | Invoke base16_Encode with valid pointer and zero length                     | input: src = &dummy, len = 0                                    | No exception thrown and valid non-NULL result pointer returned                           | Should Pass       |
 * | 03               | Validate that the returned string is empty                                  | output: result, expected output = ""                          | Function returns a non-NULL pointer with an empty string                                 | Should Pass       |
 * | 04               | Free allocated memory for result                                            | output: result pointer, free(result)                          | Memory freed without errors                                                              | Should be successful  |
 */
TEST(Base16Encode_ValidEmpty, TestCase2) {
    std::cout << "Entering Base16Encode_ValidEmpty test\n";
    // Provide a valid non-NULL pointer even though there is no data to encode.
    unsigned char dummy = 0;
    unsigned char *src = &dummy;
    size_t len = 0;
    std::cout << "Invoking base16_Encode with valid pointer and len: " << len << "\n";

    char* result = nullptr;
    EXPECT_NO_THROW(result = base16_Encode(src, len));
    std::cout << "Returned string pointer: " << (result ? result : (char*)"NULL") << "\n";

    // Validate that result is not NULL and is an empty string
    EXPECT_NE(result, nullptr);
    std::cout << "Comparing result with expected empty string \"\"\n";
    EXPECT_STREQ(result, "");

    if(result) {
        free(result);
        std::cout << "Freed allocated memory for result\n";
    }
    std::cout << "Exiting Base16Encode_ValidEmpty test\n";
}
/**
 * @brief Verify that base16_Encode returns a NULL pointer when provided with a NULL input parameter.
 *
 * This test checks the behavior of the base16_Encode API when the input source pointer is NULL.
 * The function is expected to handle the invalid input gracefully by returning a NULL pointer.
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
 * | Variation / Step | Description                                                            | Test Data                           | Expected Result                                                             | Notes       |
 * | :--------------: | ---------------------------------------------------------------------- | ----------------------------------- | ---------------------------------------------------------------------------- | ----------- |
 * | 01               | Invoke base16_Encode with NULL as the source pointer and length 5      | src = NULL, len = 5, output = result | Expect the API to return a NULL pointer and the assertion EXPECT_EQ to pass    | Should Fail |
 */
TEST(Base16Encode_NegativeNullInput, TestCase3) {
    std::cout << "Entering Base16Encode_NegativeNullInput test\n";
    size_t len = 5;
    std::cout << "Invoking base16_Encode with src: NULL and len: " << len << "\n";

    char* result = nullptr;
    EXPECT_NO_THROW(result = base16_Encode(NULL, len));
    std::cout << "Returned string pointer: " << (result ? result : (char*)"NULL") << "\n";

    // Expecting a NULL pointer due to invalid input parameter.
    EXPECT_EQ(result, nullptr);
    std::cout << "Exiting Base16Encode_NegativeNullInput test\n";
}
/**
 * @brief Verify that base16_Encode returns a NULL pointer when an invalid length is provided.
 *
 * This test case verifies that the base16_Encode function handles an invalid input combination properly.
 * Specifically, it tests the function with a source byte array of three characters ("ABC") and an
 * incorrect length parameter (1), which should result in an error condition by returning a NULL pointer.
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
 * | 01 | Initialize test inputs including source byte array and invalid length. | src = {0x41, 0x42, 0x43}, len = 1, result = uninitialized | Variables are initialized correctly. | Should be successful |
 * | 02 | Invoke base16_Encode API and validate that no exception is thrown and the returned pointer is NULL. | input: src = {0x41, 0x42, 0x43}, input: len = 1, output: result = nullptr | Function returns a NULL pointer indicating an error for invalid parameter combination, assertion passes. | Should Fail |
 */
TEST(Base16Encode_NegativeWrongLength, TestCase4) {
    std::cout << "Entering Base16Encode_NegativeWrongLength test\n";

    unsigned char src[] = {0x41, 0x42, 0x43}; // "ABC"
    size_t len = 1;
    std::cout << "Invoking base16_Encode with src bytes: [0x41, 0x42, 0x43] and len: " << len << "\n";

    char* result = nullptr;
    EXPECT_NO_THROW(result = base16_Encode(src, len));
    std::cout << "Returned string pointer: " << (result ? result : (char*)"NULL") << "\n";

    // Expected to return an error i.e. NULL pointer due to invalid parameter combination.
    EXPECT_EQ(result, nullptr);
    std::cout << "Exiting Base16Encode_NegativeWrongLength test\n";
}
/**
 * @brief Validate base16_Encode correctly encodes a full range of byte values into its hexadecimal string representation
 *
 * This test validates that the base16_Encode function properly encodes an array containing all 256 possible byte values (0-255) into a 512-character uppercase hexadecimal string. It ensures that the memory is allocated as expected, the encoding is accurate, and the output string matches the expected result generated within the test.
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
 * | Variation / Step | Description                                                                                                             | Test Data                                                                  | Expected Result                                                                                           | Notes          |
 * | :--------------: | ----------------------------------------------------------------------------------------------------------------------- | -------------------------------------------------------------------------- | ---------------------------------------------------------------------------------------------------------- | -------------- |
 * | 01               | Initialize source array with ascending byte values from 0 to 255 and set length to 256                                  | src = array{0,1,...,255}, len = 256                                        | Array is correctly initialized with values 0-255                                                            | Should be successful |
 * | 02               | Invoke base16_Encode using the initialized array and length                                                              | input: src = array{0,1,...,255}, len = 256                                 | API returns a non-NULL string pointer containing the encoded hexadecimal string                             | Should Pass    |
 * | 03               | Generate the expected hexadecimal string by converting each byte to its two-digit uppercase hex representation             | For each byte i: value = i in src, using format "%02X" for conversion        | Expected string is a 512-character string representing each byte in uppercase hexadecimal format             | Should be successful |
 * | 04               | Compare the returned result from base16_Encode to the generated expected hexadecimal string                               | result = output of base16_Encode, expected = generated expected string       | Test passes if result is non-NULL and exactly matches the expected hexadecimal string                          | Should Pass    |
 * | 05               | Free the allocated memory for the returned result                                                                        | result pointer                                                             | Memory is successfully freed without any errors                                                           | Should be successful |
 */
TEST(Base16Encode_ValidAllBytes, TestCase5) {
    std::cout << "Entering Base16Encode_ValidAllBytes test\n";
    unsigned char src[256];
    for (size_t i = 0; i < 256; i++) {
        src[i] = static_cast<unsigned char>(i);
    }
    size_t len = 256;
    std::cout << "Invoking base16_Encode with an array of 256 ascending byte values and len: " << len << "\n";

    char* result = nullptr;
    EXPECT_NO_THROW(result = base16_Encode(src, len));
    if(result) {
        std::cout << "Returned string pointer is non-NULL\n";
    } else {
        std::cout << "Returned string pointer is NULL\n";
    }

    // Generate the expected string which is 512 hex characters representing each byte in two-digit uppercase hex.
    char expected[513];
    expected[512] = '\0';
    for (size_t i = 0; i < 256; i++) {
        char buf[3];
        std::snprintf(buf, sizeof(buf), "%02X", static_cast<unsigned int>(src[i]));
        // Use strncpy to copy 2 characters into expected array at correct offset.
        std::strncpy(&expected[i*2], buf, 2);
        std::cout << "Byte " << i << " value: " << static_cast<unsigned int>(src[i]) << " encoded as: " << buf << "\n";
    }
    std::cout << "Expected encoded string: " << expected << "\n";

    // Check that result is not NULL and matches the expected hex string.
    EXPECT_NE(result, nullptr);
    std::cout << "Comparing returned string with expected hex encoded string.\n";
    EXPECT_STREQ(result, expected);

    if(result) {
        free(result);
        std::cout << "Freed allocated memory for result\n";
    }
    std::cout << "Exiting Base16Encode_ValidAllBytes test\n";
}
/**
 * @brief Verify that a valid single byte hexadecimal string is correctly decoded.
 *
 * This test validates that the base16_Decode function correctly converts a valid single byte hexadecimal string ("4A") to its corresponding byte value.
 * It also ensures that the output length is updated appropriately (from the initial value of 100 to 1), and that the returned pointer is not null.
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
 * | Variation / Step | Description                                                                                      | Test Data                                             | Expected Result                                                               | Notes       |
 * | :----:           | -----------------------------------------------------------------------------------------------  | ----------------------------------------------------- | ----------------------------------------------------------------------------- | ----------- |
 * | 01               | Prepare a valid single byte hex input and invoke the base16_Decode API.                           | src = "4A", srcLen = 2, resultLen (initial) = 100       | Returns a non-null pointer, resultLen updated to 1, decoded byte equals 0x4A   | Should Pass |
 */
TEST(Base16DecodeTest, ValidSingleByteHex) {
    std::cout << "Entering ValidSingleByteHex test" << std::endl;

    char src[3] = {0};
    strncpy(src, "4A", sizeof(src)-1);
    size_t srcLen = 2;
    size_t resultLen = 100; // initialize to a non-zero value to check update

    std::cout << "Invoking base16_Decode with srcPtr: " << src 
              << ", srcLen: " << srcLen 
              << ", len pointer address: " << &resultLen << std::endl;
    unsigned char *decoded = base16_Decode(src, srcLen, &resultLen);
    std::cout << "Returned pointer: " << static_cast<const void*>(decoded) 
              << ", resultLen: " << resultLen << std::endl;

    EXPECT_NE(decoded, nullptr);
    EXPECT_EQ(resultLen, 1);
    if (decoded) {
        std::cout << "Decoded byte value: 0x" << std::hex << static_cast<int>(decoded[0]) << std::dec << std::endl;
        EXPECT_EQ(decoded[0], 0x4A);
    }

    std::cout << "Exiting ValidSingleByteHex test" << std::endl;
}
/**
 * @brief Verifies that base16_Decode correctly decodes a valid multi-byte hex encoded string.
 *
 * This test validates the base16_Decode API by providing a valid multi-byte hexadecimal string "48656C6C6F" as input. 
 * It verifies that the function returns a non-null pointer, updates the result length to the expected value, and each decoded byte matches the expected ASCII representation for "Hello".
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Initialize input parameters for the test. | src = "48656C6C6F", srcLen = 10, resultLen = 100 | Variables initialized as expected. | Should be successful |
 * | 02 | Invoke base16_Decode with the initialized inputs. | input: src = "48656C6C6F", srcLen = 10, resultLen pointer = address of resultLen | Returned decoded pointer is not nullptr. | Should Pass |
 * | 03 | Verify the result length after decoding. | resultLen updated by API | resultLen equals 5. | Should Pass |
 * | 04 | Validate each byte of the decoded output against expected values. | decoded bytes, expected bytes = 0x48,0x65,0x6C,0x6C,0x6F | Each decoded byte matches the corresponding expected value. | Should Pass |
 */
TEST(Base16DecodeTest, ValidMultiByteHex) {
    std::cout << "Entering ValidMultiByteHex test" << std::endl;

    char src[20] = {0};
    strncpy(src, "48656C6C6F", sizeof(src)-1);
    size_t srcLen = 10;
    size_t resultLen = 100;

    std::cout << "Invoking base16_Decode with srcPtr: " << src 
              << ", srcLen: " << srcLen 
              << ", len pointer address: " << &resultLen << std::endl;
    unsigned char *decoded = base16_Decode(src, srcLen, &resultLen);
    std::cout << "Returned pointer: " << static_cast<const void*>(decoded) 
              << ", resultLen: " << resultLen << std::endl;

    EXPECT_NE(decoded, nullptr);
    EXPECT_EQ(resultLen, 5);
    unsigned char expected[5] = {0x48, 0x65, 0x6C, 0x6C, 0x6F};
    for (size_t i = 0; i < resultLen; ++i) {
        std::cout << "Byte " << i << " of decoded output: 0x" 
                  << std::hex << static_cast<int>(decoded[i]) << std::dec << std::endl;
        EXPECT_EQ(decoded[i], expected[i]);
    }

    std::cout << "Exiting ValidMultiByteHex test" << std::endl;
}
/**
 * @brief Validate base16_Decode function behavior with an odd-length hexadecimal string
 *
 * This test verifies that the base16_Decode function correctly handles an input hexadecimal string of odd length, which is considered invalid. The test ensures the function returns a nullptr and resets the output length to 0.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 008@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Initialize input with odd-length hex string "ABC" and call base16_Decode | src = "ABC", srcLen = 3, resultLen = 100 | Decoded pointer is nullptr and resultLen is 0 | Should Fail |
 */
TEST(Base16DecodeTest, OddLengthHexString) {
    std::cout << "Entering OddLengthHexString test" << std::endl;

    char src[10] = {0};
    strncpy(src, "ABC", sizeof(src)-1);
    size_t srcLen = 3;
    size_t resultLen = 100;

    std::cout << "Invoking base16_Decode with srcPtr: " << src 
              << ", srcLen: " << srcLen 
              << ", len pointer address: " << &resultLen << std::endl;
    unsigned char *decoded = base16_Decode(src, srcLen, &resultLen);
    std::cout << "Returned pointer: " << static_cast<const void*>(decoded) 
              << ", resultLen: " << resultLen << std::endl;

    EXPECT_EQ(decoded, nullptr);
    EXPECT_EQ(resultLen, 0);

    std::cout << "Exiting OddLengthHexString test" << std::endl;
}
/**
 * @brief Test base16_Decode API with an empty input string
 *
 * This test validates that base16_Decode returns a non-null pointer and updates the result length to 0 when provided with an empty input string. It ensures proper handling of an edge case where no data is available to decode.
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
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Initialize input data with an empty string and set length parameters | src = "", srcLen = 0, resultLen = 100 | Input parameters are correctly initialized | Should be successful |
 * | 02 | Invoke base16_Decode and verify that the decoded pointer is not null and resultLen is updated to 0 | src = "", srcLen = 0, &resultLen provided | decoded pointer is not nullptr, resultLen equals 0, assertions pass | Should Pass |
 */
TEST(Base16DecodeTest, EmptyInputString) {
    std::cout << "Entering EmptyInputString test" << std::endl;

    char src[2] = {0};
    strncpy(src, "", sizeof(src)-1);
    size_t srcLen = 0;
    size_t resultLen = 100;

    std::cout << "Invoking base16_Decode with srcPtr: \"" << src << "\""
              << ", srcLen: " << srcLen 
              << ", len pointer address: " << &resultLen << std::endl;
    unsigned char *decoded = base16_Decode(src, srcLen, &resultLen);
    std::cout << "Returned pointer: " << static_cast<const void*>(decoded) 
              << ", resultLen: " << resultLen << std::endl;

    EXPECT_NE(decoded, nullptr);
    EXPECT_EQ(resultLen, 0);

    std::cout << "Exiting EmptyInputString test" << std::endl;
}
/**
 * @brief Test the base16_Decode API with a null source pointer.
 *
 * This test verifies that when a null pointer is passed as the source for decoding, 
 * the base16_Decode API returns a null pointer and correctly sets the result length to zero. 
 * It ensures that the API handles invalid input gracefully.
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
 * | 01 | Invoke base16_Decode with a null source pointer | input: src = nullptr, srcLen = 0, resultLen = 100 | Returns a null pointer and resultLen is set to 0 | Should Pass |
 */
TEST(Base16DecodeTest, NullSrcPtr) {
    std::cout << "Entering NullSrcPtr test" << std::endl;

    const char *src = nullptr;
    size_t srcLen = 0;
    size_t resultLen = 100;

    std::cout << "Invoking base16_Decode with srcPtr: " << src 
              << " (NULL), srcLen: " << srcLen 
              << ", len pointer address: " << &resultLen << std::endl;
    unsigned char *decoded = base16_Decode(src, srcLen, &resultLen);
    std::cout << "Returned pointer: " << static_cast<const void*>(decoded) 
              << ", resultLen: " << resultLen << std::endl;

    EXPECT_EQ(decoded, nullptr);
    EXPECT_EQ(resultLen, 0);

    std::cout << "Exiting NullSrcPtr test" << std::endl;
}
/**
 * @brief Verify that base16_Decode returns nullptr when the len pointer parameter is NULL
 *
 * This test verifies that the base16_Decode API correctly handles a NULL length pointer by returning a nullptr.
 * It checks the proper initialization of the input parameters, invokes the API with a NULL len pointer, and asserts that the function returns a nullptr.
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
 * | Variation / Step | Description                                                 | Test Data                                             | Expected Result                                            | Notes          |
 * | :--------------: | ----------------------------------------------------------- | ----------------------------------------------------- | ---------------------------------------------------------- | -------------- |
 * | 01               | Setup test input variables, initialize src and srcLen        | input: src = "4A", srcLen = 2, len pointer = NULL      | Input parameters successfully initialized                  | Should be successful |
 * | 02               | Invoke base16_Decode API with provided input                 | input: src = "4A", srcLen = 2, len pointer = NULL      | API returns nullptr due to NULL len pointer handling       | Should Pass    |
 * | 03               | Assert that the returned value is nullptr using EXPECT_EQ    | output: decoded pointer value = nullptr               | EXPECT_EQ(decoded, nullptr) passes                         | Should be successful |
 */
TEST(Base16DecodeTest, NullLenPointer) {
    std::cout << "Entering NullLenPointer test" << std::endl;

    char src[3] = {0};
    strncpy(src, "4A", sizeof(src)-1);
    size_t srcLen = 2;

    std::cout << "Invoking base16_Decode with srcPtr: " << src 
              << ", srcLen: " << srcLen 
              << ", len pointer: NULL" << std::endl;
    unsigned char *decoded = base16_Decode(src, srcLen, nullptr);
    std::cout << "Returned pointer: " << static_cast<const void*>(decoded) << std::endl;

    EXPECT_EQ(decoded, nullptr);

    std::cout << "Exiting NullLenPointer test" << std::endl;
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
