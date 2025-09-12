
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
#include "playerisobmffbox.h"

using namespace player_isobmff;


/**
 * @brief Verify that ReadCStringLenFromBuffer correctly computes the length of a valid, null-terminated C-string.
 *
 * This test verifies that when a properly null-terminated C-string ("Test") is stored in a fixed-size buffer, the API ReadCStringLenFromBuffer returns the correct string length. It is important to ensure that the function correctly interprets the null termination to prevent reading unintended memory.
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
 * | 01 | Prepare input buffer with the string "Test" and ensure proper null termination. | input: buffer = {'T', 'e', 's', 't', '\0'}, size = 5 | Buffer is correctly initialized with the null-terminated string. | Should be successful |
 * | 02 | Print buffer content and length for debugging purposes. | output: console prints buffer content and buffer length = 5 | Debug information is printed to console showing expected values. | Should be successful |
 * | 03 | Invoke ReadCStringLenFromBuffer with the prepared buffer. | input: buffer pointer, buffer length = 5; output: result variable | API call returns the integer 4, which represents the length of "Test". | Should Pass |
 * | 04 | Verify the API's return value using EXPECT_EQ. | assertion: EXPECT_EQ(result, 4) | Assertion passes confirming the expected length of 4. | Should Pass |
 */
TEST(ReadCStringLenFromBuffer_Test, ValidCStringWithProperNullTermination) {
    std::cout << "Entering ValidCStringWithProperNullTermination test" << std::endl;
    
    // Prepare input buffer with "Test\0"
    uint8_t buffer[5];
    // Using strncpy to assign value to fixed size array including null termination
    const char* testStr = "Test";
    strncpy(reinterpret_cast<char*>(buffer), testStr, sizeof(buffer));
    // Explicitly add null termination (strncpy might not add it if size exceeded)
    buffer[4] = '\0';
    std::cout << "Input buffer: ";
    for (uint32_t i = 0; i < 5; ++i) {
        std::cout << buffer[i] << " ";
    }
    std::cout << std::endl;
    std::cout << "Buffer length: 5" << std::endl;
    
    std::cout << "Invoking ReadCStringLenFromBuffer method" << std::endl;
    int result = ReadCStringLenFromBuffer(buffer, 5);
    std::cout << "Method returned result: " << result << std::endl;
    
    // Expected string length is 4 ("Test")
    EXPECT_EQ(result, 4);
    
    std::cout << "Exiting ValidCStringWithProperNullTermination test" << std::endl;
}
/**
 * @brief Verify that an empty C string returns a length of 0.
 *
 * This test verifies that the ReadCStringLenFromBuffer function correctly interprets a buffer containing only a null terminator as an empty string and returns a string length of 0. It is important to check that the function handles empty strings appropriately.
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
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Prepare a buffer with a null terminator, invoke ReadCStringLenFromBuffer, and verify that it returns 0. | buffer[0] = 0, buffer length = 1, result expected = 0 | Return value is 0 with EXPECT_EQ(result, 0) check. | Should Pass |
 */
TEST(ReadCStringLenFromBuffer_Test, ValidEmptyCString) {
    std::cout << "Entering ValidEmptyCString test" << std::endl;
    
    // Prepare input buffer with just the null terminator
    uint8_t buffer[1];
    memset(buffer, 0, sizeof(buffer));
    std::cout << "Input buffer: " << static_cast<int>(buffer[0]) << " (null terminator)" << std::endl;
    std::cout << "Buffer length: 1" << std::endl;
    
    std::cout << "Invoking ReadCStringLenFromBuffer method" << std::endl;
    int result = ReadCStringLenFromBuffer(buffer, 1);
    std::cout << "Method returned result: " << result << std::endl;
    
    // Expected string length is 0 for an empty string
    EXPECT_EQ(result, 0);
    
    std::cout << "Exiting ValidEmptyCString test" << std::endl;
}
/**
 * @brief Test the behavior of ReadCStringLenFromBuffer when a null buffer pointer is provided with a positive buffer length.
 *
 * This test verifies that the ReadCStringLenFromBuffer API correctly handles the case when the buffer pointer is null while the buffer length is positive.
 * It ensures that the function returns the expected error code (-1) for invalid input. 
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
 * | Variation / Step | Description                                                      | Test Data                               | Expected Result                                          | Notes           |
 * | :--------------: | ---------------------------------------------------------------- | --------------------------------------- | -------------------------------------------------------- | --------------- |
 * | 01               | Initialize test variables with a null buffer pointer and a positive buffer length. | buffer = nullptr, bufferLen = 5           | Variables are initialized as expected.                 | Should be successful |
 * | 02               | Invoke the ReadCStringLenFromBuffer API function with the test inputs.         | buffer = nullptr, bufferLen = 5           | API returns error code -1, indicating an invalid input. | Should Fail     |
 * | 03               | Validate the return value from the API using an assertion check.               | result = API output (-1)                 | Assertion passes if result equals -1.                   | Should be successful |
 * | 04               | Log the exit message indicating the completion of the test.                   | NA                                      | Successful logging to standard output.                  | Should be successful |
 */
TEST(ReadCStringLenFromBuffer_Test, NullBufferPointerWithPositiveBufferLength) {
    std::cout << "Entering NullBufferPointerWithPositiveBufferLength test" << std::endl;
    
    uint8_t* buffer = nullptr;
    uint32_t bufferLen = 5;
    std::cout << "Input buffer: nullptr" << std::endl;
    std::cout << "Buffer length: " << bufferLen << std::endl;
    
    std::cout << "Invoking ReadCStringLenFromBuffer method" << std::endl;
    int result = ReadCStringLenFromBuffer(buffer, bufferLen);
    std::cout << "Method returned result: " << result << std::endl;
    
    // Expected error status, assume error code is -1 for invalid input
    EXPECT_EQ(result, -1);
    
    std::cout << "Exiting NullBufferPointerWithPositiveBufferLength test" << std::endl;
}
/**
 * @brief Test to validate that ReadCStringLenFromBuffer returns an error when buffer length is zero
 *
 * This test verifies that providing a valid buffer with a zero buffer length results in an error return value from ReadCStringLenFromBuffer. The test ensures that the API correctly handles the edge case where the buffer length is zero, despite having a valid buffer pointer.
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
 * | Variation / Step | Description                                                                 | Test Data                                                           | Expected Result                                                     | Notes           |
 * | :--------------: | --------------------------------------------------------------------------- | ------------------------------------------------------------------- | ------------------------------------------------------------------- | --------------- |
 * | 01               | Initialize a valid buffer with content "ABC" and ensure proper null termination | input: buffer = "ABC", ensuring buffer[3] = '\0'                   | Buffer is correctly initialized with "ABC" followed by a null character | Should be successful |
 * | 02               | Invoke ReadCStringLenFromBuffer API with the valid buffer pointer and zero buffer length | input: buffer pointer = valid buffer, bufferLen = 0, output: expected error code -1 | API returns error code -1 and assertion EXPECT_EQ validates the error code   | Should Fail     |
 */

TEST(ReadCStringLenFromBuffer_Test, ValidBufferPointerWithZeroBufferLen) {
    std::cout << "Entering ValidBufferPointerWithZeroBufferLen test" << std::endl;
    
    uint8_t buffer[4];
    // Even though buffer is valid, bufferLen is 0 so method should indicate error.
    strncpy(reinterpret_cast<char*>(buffer), "ABC", 4);
    // Ensure null termination in memory though it won't be used when bufferLen is zero.
    buffer[3] = '\0';
    std::cout << "Input buffer: ";
    for (int i = 0; i < 4; i++) {
        std::cout << buffer[i] << " ";
    }
    std::cout << std::endl;
    std::cout << "Buffer length: 0" << std::endl;
    
    std::cout << "Invoking ReadCStringLenFromBuffer method" << std::endl;
    int result = ReadCStringLenFromBuffer(buffer, 0);
    std::cout << "Method returned result: " << result << std::endl;
    
    // Expected error status due to invalid buffer length, assume error code -1.
    EXPECT_EQ(result, -1);
    
    std::cout << "Exiting ValidBufferPointerWithZeroBufferLen test" << std::endl;
}
/**
 * @brief Verify that ReadCStringLenFromBuffer returns an error for non null-terminated strings.
 *
 * This test validates that the ReadCStringLenFromBuffer API correctly identifies a buffer that does not contain a null terminator and returns an error code (-1). It is crucial to ensure that the API does not process invalid input strings, thereby preventing potential issues in string handling.
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
 * | Variation / Step | Description                                                          | Test Data                                                | Expected Result                                   | Notes                |
 * | :--------------: | -------------------------------------------------------------------- | -------------------------------------------------------- | -------------------------------------------------- | -------------------- |
 * | 01               | Initialize buffer with non null-terminated string "ABCD"             | buffer = A,B,C,D, buffer length = 4                      | Buffer is correctly populated                      | Should be successful |
 * | 02               | Invoke ReadCStringLenFromBuffer and verify it returns error code (-1)  | buffer = A,B,C,D, buffer length = 4, expected output = -1 | Function returns -1 and assertion passes           | Should Fail          |
 */
TEST(ReadCStringLenFromBuffer_Test, NonNullTerminatedStringWithinProvidedBufferLen) {
    std::cout << "Entering NonNullTerminatedStringWithinProvidedBufferLen test" << std::endl;
    
    uint8_t buffer[4];
    // Prepare non null terminated string (no '\0' character)
    strncpy(reinterpret_cast<char*>(buffer), "ABCD", 4);
    std::cout << "Input buffer: ";
    for (int i = 0; i < 4; i++) {
        std::cout << buffer[i] << " ";
    }
    std::cout << std::endl;
    std::cout << "Buffer length: 4" << std::endl;
    
    std::cout << "Invoking ReadCStringLenFromBuffer method" << std::endl;
    int result = ReadCStringLenFromBuffer(buffer, 4);
    std::cout << "Method returned result: " << result << std::endl;
    
    // Expected error status indicating missing null termination, assume error code -1.
    EXPECT_EQ(result, -1);
    
    std::cout << "Exiting NonNullTerminatedStringWithinProvidedBufferLen test" << std::endl;
}
/**
 * @brief Tests conversion of a fixed byte buffer to a 64-bit unsigned integer using a valid known pattern.
 *
 * This test verifies that the function ReadUint64FromBuffer correctly converts an 8-byte array containing a known pattern
 * (with bytes 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08) into the corresponding 64-bit unsigned integer.
 * This ensures the API correctly processes the byte order and produces an expected output value.
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
 * | Variation / Step | Description                                                                                           | Test Data                                                                                                   | Expected Result                                             | Notes       |
 * | :--------------: | ----------------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------------------------- | ----------------------------------------------------------- | ----------- |
 * | 01               | Invoke ReadUint64FromBuffer with an 8-byte array containing the valid known pattern from 0x01 to 0x08 | buf = [0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08], result expected = 0x0102030405060708ULL              | Returned value equals 0x0102030405060708ULL and assertion passes | Should Pass |
 */
TEST(ReadUint64FromBuffer_ValidKnownPattern, Test) {
    std::cout << "Entering ReadUint64FromBuffer_ValidKnownPattern test" << std::endl;
    uint8_t buf[8] = {0};
    // Using strncpy to copy the byte pattern into the fixed size array.
    strncpy(reinterpret_cast<char*>(buf), "\x01\x02\x03\x04\x05\x06\x07\x08", 8);
    std::cout << "Invoking ReadUint64FromBuffer with buffer bytes: ";
    for (int i = 0; i < 8; ++i) {
        std::cout << std::hex << static_cast<unsigned>(buf[i]) << " ";
    }
    std::cout << std::dec << std::endl;
    uint64_t result = ReadUint64FromBuffer(buf);
    std::cout << "Method ReadUint64FromBuffer returned value: 0x" << std::hex << result << std::dec << std::endl;
    EXPECT_EQ(result, 0x0102030405060708ULL);
    std::cout << "Exiting ReadUint64FromBuffer_ValidKnownPattern test" << std::endl;
}
/**
 * @brief Verify that ReadUint64FromBuffer correctly reads a buffer filled with zeros
 *
 * This test ensures that when an 8-byte buffer initialized with all zero values is passed to ReadUint64FromBuffer, the function returns 0. This is important to confirm that the function correctly interprets zero bytes without any unintended data conversion.
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
 * | 01 | Initialize an 8-byte buffer with all zeros, invoke ReadUint64FromBuffer, and assert that the output is 0. | Input: buf = [0, 0, 0, 0, 0, 0, 0, 0], Output: expected result = 0 | ReadUint64FromBuffer returns 0 and the EXPECT_EQ assertion passes | Should Pass |
 */
TEST(ReadUint64FromBuffer_AllZeros, Test) {
    std::cout << "Entering ReadUint64FromBuffer_AllZeros test" << std::endl;
    uint8_t buf[8] = {0};
    strncpy(reinterpret_cast<char*>(buf), "\x00\x00\x00\x00\x00\x00\x00\x00", 8);
    std::cout << "Invoking ReadUint64FromBuffer with buffer bytes (all zeros): ";
    for (int i = 0; i < 8; ++i) {
        std::cout << std::hex << static_cast<unsigned>(buf[i]) << " ";
    }
    std::cout << std::dec << std::endl;
    uint64_t result = ReadUint64FromBuffer(buf);
    std::cout << "Method ReadUint64FromBuffer returned value: " << result << std::endl;
    EXPECT_EQ(result, 0ULL);
    std::cout << "Exiting ReadUint64FromBuffer_AllZeros test" << std::endl;
}
/**
 * @brief Verify that ReadUint64FromBuffer correctly converts a buffer with all bits set to the maximum 64-bit unsigned integer value
 *
 * This test validates that when a buffer containing eight bytes, each set to 0xFF, is provided to ReadUint64FromBuffer, the function returns 0xFFFFFFFFFFFFFFFFULL. This ensures correct handling of maximum unsigned 64-bit values.
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
 * | Variation / Step | Description                                                               | Test Data                                                        | Expected Result                                                                                                | Notes          |
 * | :--------------: | ------------------------------------------------------------------------- | ---------------------------------------------------------------- | -------------------------------------------------------------------------------------------------------------- | -------------- |
 * |       01         | Initialize an 8-byte buffer with each byte set to 0xFF                    | buf[8] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF}                | Buffer is correctly initialized with maximum values for each byte                                             | Should be successful |
 * |       02         | Call ReadUint64FromBuffer with the initialized buffer                     | Input: buf = pointer to the 8-byte array                         | The API returns 0xFFFFFFFFFFFFFFFFULL                                                                          | Should Pass    |
 * |       03         | Verify that the return value from ReadUint64FromBuffer equals the expected value | Input: result from ReadUint64FromBuffer, expected output = 0xFFFFFFFFFFFFFFFFULL | The test assertion (EXPECT_EQ) confirms that result is 0xFFFFFFFFFFFFFFFFULL                                      | Should be successful |
 */
TEST(ReadUint64FromBuffer_MaxValue, Test) {
    std::cout << "Entering ReadUint64FromBuffer_MaxValue test" << std::endl;
    uint8_t buf[8] = {0};
    strncpy(reinterpret_cast<char*>(buf), "\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF", 8);
    std::cout << "Invoking ReadUint64FromBuffer with buffer bytes (all bits set): ";
    for (int i = 0; i < 8; ++i) {
        std::cout << std::hex << static_cast<unsigned>(buf[i]) << " ";
    }
    std::cout << std::dec << std::endl;
    uint64_t result = ReadUint64FromBuffer(buf);
    std::cout << "Method ReadUint64FromBuffer returned value: 0x" << std::hex << result << std::dec << std::endl;
    EXPECT_EQ(result, 0xFFFFFFFFFFFFFFFFULL);
    std::cout << "Exiting ReadUint64FromBuffer_MaxValue test" << std::endl;
}
/**
 * @brief Verify that ReadUint64FromBuffer handles a NULL buffer correctly
 *
 * This test checks that when a NULL pointer is provided to ReadUint64FromBuffer, the function does not throw any exceptions and returns 0. This behavior ensures robust error handling for invalid inputs.
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
 * | Variation / Step | Description                                                  | Test Data                                         | Expected Result                                                 | Notes       |
 * | :---------------: | ------------------------------------------------------------ | ------------------------------------------------- | --------------------------------------------------------------- | ----------- |
 * | 01                | Invoke ReadUint64FromBuffer with a NULL pointer              | input: pointer = nullptr, output: expected = 0ULL | No exception thrown; returned value equals 0ULL                 | Should Pass |
 */
TEST(ReadUint64FromBuffer_NullBuffer, Test) {
    std::cout << "Entering ReadUint64FromBuffer_NullBuffer test" << std::endl;
    uint64_t result = 0;
    std::cout << "Invoking ReadUint64FromBuffer with NULL pointer" << std::endl;
    EXPECT_NO_THROW(result = ReadUint64FromBuffer(nullptr));
    std::cout << "Method ReadUint64FromBuffer returned value: " << result << std::endl;
    EXPECT_EQ(result, 0ULL);
    std::cout << "Exiting ReadUint64FromBuffer_NullBuffer test" << std::endl;
}
/**
 * @brief Test WriteUint64ToBuffer functionality with zero value.
 *
 * This test verifies that WriteUint64ToBuffer correctly writes a uint64_t zero value into an 8‐byte buffer.
 * The test initially fills the buffer with a non-zero value to ensure that the function actually writes all bytes.
 * Then, it calls WriteUint64ToBuffer with a value of 0 and finally checks that every byte in the buffer is updated to zero.
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
 * | 01 | Allocate an 8-byte buffer and initialize each byte with 0xAA to ensure the function writes correctly. | buffer = allocated memory (8-byte), initial value for each byte = 0xAA | Buffer allocated and contains non-zero values (0xAA in every byte) | Should be successful |
 * | 02 | Invoke WriteUint64ToBuffer with the buffer and a uint64_t value of 0. | input: buffer pointer, value = 0 | The API completes without throwing an exception. | Should Pass |
 * | 03 | Verify that every byte in the buffer is set to 0x00 after the API call. | For each index in buffer: expected output value = 0x00 | All 8 bytes of the buffer equal 0x00. | Should Pass |
 */
TEST(WriteUint64ToBufferTests, WriteZeroValue)
{
    std::cout << "Entering WriteZeroValue test" << std::endl;
    // Allocate an 8‐byte buffer.
    uint8_t buffer[8];
    // Optionally zero-initialize the buffer (not required, since the function writes all bytes).
    std::memset(buffer, 0xAA, sizeof(buffer)); // Fill with non-zero to ensure function writes correctly.
    uint64_t val = 0;
    std::cout << "Invoking WriteUint64ToBuffer with value: " << val << std::endl;
    
    EXPECT_NO_THROW(WriteUint64ToBuffer(buffer, val));
    std::cout << "Function WriteUint64ToBuffer completed. Verifying buffer content..." << std::endl;
    for (int i = 0; i < 8; i++)
    {
        std::cout << "buffer[" << i << "]: 0x" << std::hex << static_cast<int>(buffer[i]) << std::endl;
        EXPECT_EQ(buffer[i], 0x00);
    }
    std::cout << "Exiting WriteZeroValue test" << std::endl;
}
/**
 * @brief Validate WriteUint64ToBuffer with maximum uint64 value.
 *
 * This test verifies that calling WriteUint64ToBuffer with the maximum value (0xFFFFFFFFFFFFFFFF) correctly updates an 8-byte buffer. It ensures that the API does not throw any exceptions and that each byte in the buffer is set to 0xFF.
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
 * | 01 | Initialize an 8-byte buffer to zeros | buffer[8] = 0x00 | Buffer is zeroed | Should be successful |
 * | 02 | Invoke WriteUint64ToBuffer with value 0xFFFFFFFFFFFFFFFF on the buffer | buffer (address of an 8-byte array), val = 0xFFFFFFFFFFFFFFFF | Function call does not throw an exception | Should Pass |
 * | 03 | Verify each byte in the buffer equals 0xFF | buffer[i] for i=0 to 7, expected value = 0xFF | Each byte in buffer equals 0xFF | Should Pass |
 */
TEST(WriteUint64ToBufferTests, WriteMaxUint64Value)
{
    std::cout << "Entering WriteMaxUint64Value test" << std::endl;
    
    uint8_t buffer[8];
    std::memset(buffer, 0x00, sizeof(buffer));
    uint64_t val = 0xFFFFFFFFFFFFFFFFull;
    std::cout << "Invoking WriteUint64ToBuffer with value: " << std::hex << val << std::endl;
    
    EXPECT_NO_THROW(WriteUint64ToBuffer(buffer, val));
    
    std::cout << "Function WriteUint64ToBuffer completed. Verifying buffer content..." << std::endl;
    for (int i = 0; i < 8; i++)
    {
        std::cout << "buffer[" << i << "]: 0x" << std::hex << static_cast<int>(buffer[i]) << std::endl;
        EXPECT_EQ(buffer[i], 0xFF);
    }
    
    std::cout << "Exiting WriteMaxUint64Value test" << std::endl;
}
/**
 * @brief Test WriteUint64ToBuffer with a null destination buffer
 *
 * This test verifies that the WriteUint64ToBuffer function properly handles a null destination by throwing a std::invalid_argument exception when invoked with a nullptr as the destination pointer.
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
 * | Variation / Step | Description                                                         | Test Data                                         | Expected Result                                           | Notes      |
 * | :--------------: | ------------------------------------------------------------------- | ------------------------------------------------- | --------------------------------------------------------- | ---------- |
 * | 01               | Invoke WriteUint64ToBuffer with a null destination and a valid value | destination = nullptr, value = 123456789            | std::invalid_argument exception is thrown; assertion passes | Should Fail |
 */
TEST(WriteUint64ToBufferTests, NullDestinationBuffer)
{
    std::cout << "Entering NullDestinationBuffer test" << std::endl;
    
    uint64_t val = 123456789ull;
    std::cout << "Invoking WriteUint64ToBuffer with NULL destination and value: " << val << std::endl;
    
    EXPECT_THROW(WriteUint64ToBuffer(nullptr, val), std::invalid_argument);
    std::cout << "Exiting NullDestinationBuffer test" << std::endl;
}
/**
 * @brief Validate typical valid input for constructing an EmsgIsoBmffBox object
 *
 * This test verifies that the EmsgIsoBmffBox constructor successfully creates an object 
 * when provided with a valid FullIsoBmffBox and valid timing parameters. It ensures that no exception 
 * is thrown during the instantiation with correct values.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 013@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Initialize FullIsoBmffBox with valid parameters | boxSize = 1000, btype = "emsg", version = 0, flags = 0 | FullIsoBmffBox object is created without error | Should be successful |
 * | 02 | Invoke EmsgIsoBmffBox constructor with valid parameters | tScale = 48000, evtDur = 1000, _id = 1, presTime = 5000, presTimeDelta = 500, FullIsoBmffBox from step 01 | EmsgIsoBmffBox object is instantiated without throwing exceptions | Should Pass |
 */
TEST(player_isobmff_EmsgIsoBmffBox, TypicalValidInput) {
    std::cout << "Entering TypicalValidInput test" << std::endl;
    
    // Initialize FullIsoBmffBox parameters
    uint32_t boxSize = 1000;
    char btype[4];
    std::memset(btype, 0, sizeof(btype));
    std::strncpy(btype, "emsg", 4);
    uint8_t version = 0;
    uint32_t flags = 0;
    
    std::cout << "Creating FullIsoBmffBox with boxSize=" << boxSize
              << ", btype=" << std::string(btype, 4)
              << ", version=" << static_cast<uint32_t>(version)
              << ", flags=" << flags << std::endl;
              
    // Create FullIsoBmffBox object
    FullIsoBmffBox fbox(boxSize, btype, version, flags);
    
    // Parameters for EmsgIsoBmffBox
    uint32_t tScale = 48000;
    uint32_t evtDur = 1000;
    uint32_t _id = 1;
    uint64_t presTime = 5000;
    uint32_t presTimeDelta = 500;
    
    std::cout << "Invoking EmsgIsoBmffBox constructor with parameters: "
              << "tScale=" << tScale
              << ", evtDur=" << evtDur
              << ", _id=" << _id
              << ", presTime=" << presTime
              << ", presTimeDelta=" << presTimeDelta << std::endl;
              
    // Construct EmsgIsoBmffBox and check no exception is thrown.
    EXPECT_NO_THROW({
        player_isobmff::EmsgIsoBmffBox emsgBox(fbox, tScale, evtDur, _id, presTime, presTimeDelta);
        std::cout << "EmsgIsoBmffBox object created successfully in TypicalValidInput test" << std::endl;
    });
    
    std::cout << "Exiting TypicalValidInput test" << std::endl;
}
/**
 * @brief Verify that the EmsgIsoBmffBox constructor handles a zero time scale without exceptions.
 *
 * This test creates a FullIsoBmffBox object with predetermined parameters and then invokes the EmsgIsoBmffBox constructor with a zero time scale. The test ensures that the constructor does not throw any exceptions when provided with a zero time scale.
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
 * | Variation / Step | Description                                                       | Test Data                                                      | Expected Result                                                                 | Notes             |
 * | :--------------: | ----------------------------------------------------------------- | -------------------------------------------------------------- | ------------------------------------------------------------------------------- | ----------------- |
 * | 01               | Initialize FullIsoBmffBox parameters and construct a FullIsoBmffBox   | boxSize = 1000, btype = emsg, version = 1, flags = 0             | FullIsoBmffBox object created successfully, as confirmed through logs             | Should be successful |
 * | 02               | Invoke the EmsgIsoBmffBox constructor with a zero time scale and verify no exception is thrown | tScale = 0, evtDur = 1000, _id = 2, presTime = 5000, presTimeDelta = 500         | EmsgIsoBmffBox object is created successfully without throwing exceptions          | Should Pass       |
 */
TEST(player_isobmff_EmsgIsoBmffBox, ZeroTimeScale) {
    std::cout << "Entering ZeroTimeScale test" << std::endl;
    
    // Initialize FullIsoBmffBox parameters
    uint32_t boxSize = 1000;
    char btype[4];
    std::memset(btype, 0, sizeof(btype));
    std::strncpy(btype, "emsg", 4);
    uint8_t version = 1;
    uint32_t flags = 0;
    
    std::cout << "Creating FullIsoBmffBox with boxSize=" << boxSize
              << ", btype=" << std::string(btype, 4)
              << ", version=" << static_cast<uint32_t>(version)
              << ", flags=" << flags << std::endl;
              
    FullIsoBmffBox fbox(boxSize, btype, version, flags);
    
    // Parameters for EmsgIsoBmffBox with zero time scale
    uint32_t tScale = 0;
    uint32_t evtDur = 1000;
    uint32_t _id = 2;
    uint64_t presTime = 5000;
    uint32_t presTimeDelta = 500;
    
    std::cout << "Invoking EmsgIsoBmffBox constructor with parameters: "
              << "tScale=" << tScale
              << ", evtDur=" << evtDur
              << ", _id=" << _id
              << ", presTime=" << presTime
              << ", presTimeDelta=" << presTimeDelta << std::endl;
              
    EXPECT_NO_THROW({
        player_isobmff::EmsgIsoBmffBox emsgBox(fbox, tScale, evtDur, _id, presTime, presTimeDelta);
        std::cout << "EmsgIsoBmffBox object created successfully in ZeroTimeScale test" << std::endl;
    });
    
    std::cout << "Exiting ZeroTimeScale test" << std::endl;
}
/**
 * @brief Test to check the creation of EmsgIsoBmffBox object with zero event duration.
 *
 * This test verifies that the EmsgIsoBmffBox constructor successfully creates an object when the event duration parameter is zero. It ensures that initialization of both the FullIsoBmffBox and EmsgIsoBmffBox is handled correctly without throwing any exceptions.
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
 * | Variation / Step | Description | Test Data | Expected Result |Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoke EmsgIsoBmffBox constructor with valid parameters including a zero event duration. | boxSize = 1000, btype = emsg, version = 1, flags = 0, tScale = 48000, evtDur = 0, _id = 3, presTime = 5000, presTimeDelta = 500 | Object creation succeeds with no exception thrown | Should Pass |
 */
TEST(player_isobmff_EmsgIsoBmffBox, ZeroEventDuration) {
    std::cout << "Entering ZeroEventDuration test" << std::endl;
    
    // Initialize FullIsoBmffBox parameters
    uint32_t boxSize = 1000;
    char btype[4];
    std::memset(btype, 0, sizeof(btype));
    std::strncpy(btype, "emsg", 4);
    uint8_t version = 1;
    uint32_t flags = 0;
    
    std::cout << "Creating FullIsoBmffBox with boxSize=" << boxSize
              << ", btype=" << std::string(btype, 4)
              << ", version=" << static_cast<uint32_t>(version)
              << ", flags=" << flags << std::endl;
              
    FullIsoBmffBox fbox(boxSize, btype, version, flags);
    
    // Parameters for EmsgIsoBmffBox with zero event duration
    uint32_t tScale = 48000;
    uint32_t evtDur = 0;
    uint32_t _id = 3;
    uint64_t presTime = 5000;
    uint32_t presTimeDelta = 500;
    
    std::cout << "Invoking EmsgIsoBmffBox constructor with parameters: "
              << "tScale=" << tScale
              << ", evtDur=" << evtDur
              << ", _id=" << _id
              << ", presTime=" << presTime
              << ", presTimeDelta=" << presTimeDelta << std::endl;
              
    EXPECT_NO_THROW({
        player_isobmff::EmsgIsoBmffBox emsgBox(fbox, tScale, evtDur, _id, presTime, presTimeDelta);
        std::cout << "EmsgIsoBmffBox object created successfully in ZeroEventDuration test" << std::endl;
    });
    
    std::cout << "Exiting ZeroEventDuration test" << std::endl;
}
/**
 * @brief Validate the creation of an EmsgIsoBmffBox object using a valid FullIsoBmffBox and zero presentation time
 *
 * This test verifies that an EmsgIsoBmffBox object can be instantiated with a correctly initialized FullIsoBmffBox and a presentation time of zero. 
 * It ensures that the constructor properly handles the zero presentation time scenario and does not throw an exception.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Initialize FullIsoBmffBox with defined parameters and invoke the EmsgIsoBmffBox constructor with zero presentation time | boxSize = 1000, btype = "emsg", version = 1, flags = 0, tScale = 48000, evtDur = 1000, _id = 4, presTime = 0, presTimeDelta = 500 | No exception is thrown upon object creation | Should Pass |
 */
TEST(player_isobmff_EmsgIsoBmffBox, ZeroPresentationTime) {
    std::cout << "Entering ZeroPresentationTime test" << std::endl;
    
    // Initialize FullIsoBmffBox parameters
    uint32_t boxSize = 1000;
    char btype[4];
    std::memset(btype, 0, sizeof(btype));
    std::strncpy(btype, "emsg", 4);
    uint8_t version = 1;
    uint32_t flags = 0;
    
    std::cout << "Creating FullIsoBmffBox with boxSize=" << boxSize
              << ", btype=" << std::string(btype, 4)
              << ", version=" << static_cast<uint32_t>(version)
              << ", flags=" << flags << std::endl;
              
    FullIsoBmffBox fbox(boxSize, btype, version, flags);
    
    // Parameters for EmsgIsoBmffBox with zero presentation time
    uint32_t tScale = 48000;
    uint32_t evtDur = 1000;
    uint32_t _id = 4;
    uint64_t presTime = 0;
    uint32_t presTimeDelta = 500;
    
    std::cout << "Invoking EmsgIsoBmffBox constructor with parameters: "
              << "tScale=" << tScale
              << ", evtDur=" << evtDur
              << ", _id=" << _id
              << ", presTime=" << presTime
              << ", presTimeDelta=" << presTimeDelta << std::endl;
              
    EXPECT_NO_THROW({
        player_isobmff::EmsgIsoBmffBox emsgBox(fbox, tScale, evtDur, _id, presTime, presTimeDelta);
        std::cout << "EmsgIsoBmffBox object created successfully in ZeroPresentationTime test" << std::endl;
    });
    
    std::cout << "Exiting ZeroPresentationTime test" << std::endl;
}
/**
 * @brief Validate that EmsgIsoBmffBox handles a zero presentation time delta correctly.
 *
 * This test verifies that when the EmsgIsoBmffBox is instantiated with a zero presentation time delta, it is created successfully without throwing any exceptions. The test first initializes the FullIsoBmffBox with typical parameters and then uses it to construct an EmsgIsoBmffBox object with the zero presentation time delta. This ensures that the API correctly handles edge cases with timing parameters.
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
 * | Variation / Step | Description                                                                                   | Test Data                                                                                  | Expected Result                                                                           | Notes           |
 * | :--------------: | --------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------ | ----------------------------------------------------------------------------------------- | --------------- |
 * | 01               | Initialize FullIsoBmffBox parameters and create a FullIsoBmffBox object.                        | boxSize = 1000, btype = "emsg", version = 1, flags = 0                                     | FullIsoBmffBox object is created with the provided parameters.                            | Should be successful |
 * | 02               | Invoke EmsgIsoBmffBox constructor with a zero presentation time delta and validate no exceptions.| tScale = 48000, evtDur = 1000, _id = 5, presTime = 5000, presTimeDelta = 0                     | EmsgIsoBmffBox object is created successfully without throwing any exception.             | Should Pass     |
 */
TEST(player_isobmff_EmsgIsoBmffBox, ZeroPresentationTimeDelta) {
    std::cout << "Entering ZeroPresentationTimeDelta test" << std::endl;
    
    // Initialize FullIsoBmffBox parameters
    uint32_t boxSize = 1000;
    char btype[4];
    std::memset(btype, 0, sizeof(btype));
    std::strncpy(btype, "emsg", 4);
    uint8_t version = 1;
    uint32_t flags = 0;
    
    std::cout << "Creating FullIsoBmffBox with boxSize=" << boxSize
              << ", btype=" << std::string(btype, 4)
              << ", version=" << static_cast<uint32_t>(version)
              << ", flags=" << flags << std::endl;
              
    FullIsoBmffBox fbox(boxSize, btype, version, flags);
    
    // Parameters for EmsgIsoBmffBox with zero presentation time delta
    uint32_t tScale = 48000;
    uint32_t evtDur = 1000;
    uint32_t _id = 5;
    uint64_t presTime = 5000;
    uint32_t presTimeDelta = 0;
    
    std::cout << "Invoking EmsgIsoBmffBox constructor with parameters: "
              << "tScale=" << tScale
              << ", evtDur=" << evtDur
              << ", _id=" << _id
              << ", presTime=" << presTime
              << ", presTimeDelta=" << presTimeDelta << std::endl;
              
    EXPECT_NO_THROW({
        player_isobmff::EmsgIsoBmffBox emsgBox(fbox, tScale, evtDur, _id, presTime, presTimeDelta);
        std::cout << "EmsgIsoBmffBox object created successfully in ZeroPresentationTimeDelta test" << std::endl;
    });
    
    std::cout << "Exiting ZeroPresentationTimeDelta test" << std::endl;
}
/**
 * @brief Tests the valid initialization of player_isobmff::EmsgIsoBmffBox with typical values.
 *
 * This test case verifies that the constructor of EmsgIsoBmffBox properly initializes the object when provided with typical values. It ensures that no exceptions are thrown during construction and that the object's internal state matches the parameters passed.
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
 * | 01 | Invoke the constructor of EmsgIsoBmffBox with typical values and verify no exceptions are thrown and the internal state matches the input parameters. | sz = 100, tScale = 1000, evtDur = 500, _id = 1 | Constructor should successfully initialize the object without throwing exceptions and internal state should reflect the input values. | Should Pass |
 */
TEST(player_isobmff_EmsgIsoBmffBox, ValidInitializationTypicalValues) {
    std::cout << "Entering ValidInitializationTypicalValues test" << std::endl;
    uint32_t sz = 100;
    uint32_t tScale = 1000;
    uint32_t evtDur = 500;
    uint32_t _id = 1;
    std::cout << "Invoking constructor with parameters: sz = " << sz 
              << ", tScale = " << tScale 
              << ", evtDur = " << evtDur 
              << ", _id = " << _id << std::endl;
    EXPECT_NO_THROW({
        player_isobmff::EmsgIsoBmffBox box(sz, tScale, evtDur, _id);
        std::cout << "Constructed object with expected internal state:" << std::endl;
        std::cout << "Expected box size: " << sz << std::endl;
        std::cout << "Expected timeScale: " << tScale << std::endl;
        std::cout << "Expected eventDuration: " << evtDur << std::endl;
        std::cout << "Expected id: " << _id << std::endl;
    });
    std::cout << "Exiting ValidInitializationTypicalValues test" << std::endl;
}
/**
 * @brief Verify that EmsgIsoBmffBox constructor accepts a zero size value without throwing exceptions.
 *
 * This test checks the behavior of the player_isobmff::EmsgIsoBmffBox constructor when provided with a zero size.
 * It ensures that the constructor does not throw any exceptions and correctly initializes the object with the given 
 * parameters, including a zero size, which can be a valid input scenario.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Invoke the constructor of EmsgIsoBmffBox with sz = 0, tScale = 1000, evtDur = 500, _id = 1 | input: sz = 0, tScale = 1000, evtDur = 500, _id = 1 | The constructor should complete without throwing an exception and initialize the object with the specified values | Should Pass |
 */
TEST(player_isobmff_EmsgIsoBmffBox, InitializationWithZeroSize) {
    std::cout << "Entering InitializationWithZeroSize test" << std::endl;
    uint32_t sz = 0;
    uint32_t tScale = 1000;
    uint32_t evtDur = 500;
    uint32_t _id = 1;
    std::cout << "Invoking constructor with parameters: sz = " << sz 
              << ", tScale = " << tScale 
              << ", evtDur = " << evtDur 
              << ", _id = " << _id << std::endl;
    EXPECT_NO_THROW({
        player_isobmff::EmsgIsoBmffBox box(sz, tScale, evtDur, _id);
        std::cout << "Constructed object with box size 0:" << std::endl;
        std::cout << "box size: " << sz << " (expected 0)" << std::endl;
        std::cout << "timeScale: " << tScale << std::endl;
        std::cout << "eventDuration: " << evtDur << std::endl;
        std::cout << "id: " << _id << std::endl;
    });
    std::cout << "Exiting InitializationWithZeroSize test" << std::endl;
}
/**
 * @brief Validate object's construction behavior when id is zero
 *
 * Validate that the EmsgIsoBmffBox is constructed properly with size, timeScale, eventDuration, and an identifier value of zero. This test ensures that providing an id of 0 to the constructor does not result in any exception and that the object initializes its members as expected.
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
 * | Variation / Step | Description                                                              | Test Data (input and output declarations)             | Expected Result (return value and assertion check)                           | Notes       |
 * | :--------------: | ------------------------------------------------------------------------ | ------------------------------------------------------- | -----------------------------------------------------------------------------| ----------- |
 * | 01               | Invoke EmsgIsoBmffBox constructor with valid parameters, including id = 0 | sz = 100, tScale = 1000, evtDur = 500, _id = 0            | Object is constructed without throwing an exception and id is set to 0        | Should Pass |
 */
TEST(player_isobmff_EmsgIsoBmffBox, InitializationWithZeroId) {
    std::cout << "Entering InitializationWithZeroId test" << std::endl;
    uint32_t sz = 100;
    uint32_t tScale = 1000;
    uint32_t evtDur = 500;
    uint32_t _id = 0;
    std::cout << "Invoking constructor with parameters: sz = " << sz 
              << ", tScale = " << tScale 
              << ", evtDur = " << evtDur 
              << ", _id = " << _id << std::endl;
    EXPECT_NO_THROW({
        player_isobmff::EmsgIsoBmffBox box(sz, tScale, evtDur, _id);
        std::cout << "Constructed object with id 0:" << std::endl;
        std::cout << "box size: " << sz << std::endl;
        std::cout << "timeScale: " << tScale << std::endl;
        std::cout << "eventDuration: " << evtDur << std::endl;
        std::cout << "id: " << _id << " (expected 0)" << std::endl;
    });
    std::cout << "Exiting InitializationWithZeroId test" << std::endl;
}
/**
 * @brief Verify that the EmsgIsoBmffBox constructor handles zero event duration correctly.
 *
 * This test verifies that initializing an EmsgIsoBmffBox object with a valid size, time scale, a zero event duration, and a valid id does not throw any exceptions. This confirms that the API correctly supports an event duration of zero.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 021
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                 | Test Data                                         | Expected Result                                                         | Notes       |
 * | :--------------: | --------------------------------------------------------------------------- | ------------------------------------------------- | ----------------------------------------------------------------------- | ----------- |
 * | 01               | Invoke the constructor of EmsgIsoBmffBox with zero event duration parameter | sz = 100, tScale = 1000, evtDur = 0, _id = 1         | No exception is thrown and the object is constructed successfully      | Should Pass |
 */
TEST(player_isobmff_EmsgIsoBmffBox, InitializationWithZeroEventDuration) {
    std::cout << "Entering InitializationWithZeroEventDuration test" << std::endl;
    uint32_t sz = 100;
    uint32_t tScale = 1000;
    uint32_t evtDur = 0;
    uint32_t _id = 1;
    std::cout << "Invoking constructor with parameters: sz = " << sz 
              << ", tScale = " << tScale 
              << ", evtDur = " << evtDur 
              << ", _id = " << _id << std::endl;
    EXPECT_NO_THROW({
        player_isobmff::EmsgIsoBmffBox box(sz, tScale, evtDur, _id);
        std::cout << "Constructed object with eventDuration 0:" << std::endl;
        std::cout << "box size: " << sz << std::endl;
        std::cout << "timeScale: " << tScale << std::endl;
        std::cout << "eventDuration: " << evtDur << " (expected 0)" << std::endl;
        std::cout << "id: " << _id << std::endl;
    });
    std::cout << "Exiting InitializationWithZeroEventDuration test" << std::endl;
}
/**
 * @brief Validate that EmsgIsoBmffBox constructor throws an exception for an invalid timeScale value.
 *
 * This test verifies that when a timeScale value of 0 is provided to the EmsgIsoBmffBox constructor, the function correctly throws an exception. The test ensures proper error handling for invalid timeScale values.
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
 * | 01 | Print entering test message | None | "Entering NegativeInvalidTimeScaleValue test" is printed | Should be successful |
 * | 02 | Set test parameters and print constructor inputs | sz = 100, tScale = 0, evtDur = 500, _id = 1 | Correct test data is set up and printed | Should be successful |
 * | 03 | Invoke constructor within exception expectation | input: sz = 100, tScale = 0, evtDur = 500, _id = 1; output: exception thrown | Exception is thrown due to invalid tScale value | Should Fail |
 * | 04 | Print exiting test message | None | "Exiting NegativeInvalidTimeScaleValue test" is printed | Should be successful |
 */
TEST(player_isobmff_EmsgIsoBmffBox, NegativeInvalidTimeScaleValue) {
    std::cout << "Entering NegativeInvalidTimeScaleValue test" << std::endl;
    uint32_t sz = 100;
    uint32_t tScale = 0; // invalid timeScale value as per negative test expectation
    uint32_t evtDur = 500;
    uint32_t _id = 1;
    std::cout << "Invoking constructor with parameters: sz = " << sz 
              << ", tScale = " << tScale 
              << ", evtDur = " << evtDur 
              << ", _id = " << _id << std::endl;
    EXPECT_ANY_THROW({
        // Assuming that a timeScale value of 0 should trigger an exception
        player_isobmff::EmsgIsoBmffBox box(sz, tScale, evtDur, _id);
    });
    std::cout << "Exiting NegativeInvalidTimeScaleValue test" << std::endl;
}
/**
 * @brief Validates construction of EmsgIsoBmffBox with valid input data.
 *
 * This test verifies that constructing an EmsgIsoBmffBox with valid box data and size does not throw an exception and returns a non-null pointer. The test allocates an array of 100 bytes, populates it with dummy data, and then calls the constructEmsgBox API. It confirms that the object is constructed as expected.
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
 * | Variation / Step | Description | Test Data | Expected Result |Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Allocate and initialize a 100-byte array with dummy data. | input: testSize = 100, boxData = array filled with values 0,1,...,99 modulo 256 | Data is initialized successfully without exceptions. | Should be successful |
 * | 02 | Invoke constructEmsgBox API with testSize and boxData. | input: testSize = 100, boxData pointer; output: emsgBox pointer | API call does not throw exceptions and returns a valid (non-null) pointer. | Should Pass |
 * | 03 | Verify the returned pointer is non-null and perform cleanup. | input: emsgBox pointer, boxData pointer | The pointer is non-null and the cleanup (delete and delete[]) occurs without issues. | Should be successful |
 */
TEST(player_isobmff_EmsgIsoBmffBox, ValidEmsgBoxConstruction) {
    std::cout << "Entering ValidEmsgBoxConstruction test" << std::endl;
    
    const uint32_t testSize = 100;
    std::cout << "Preparing valid box data with size = " << testSize << std::endl;
    uint8_t* boxData = new uint8_t[testSize];
    // Fill boxData with dummy valid data (for testing purposes)
    for (uint32_t i = 0; i < testSize; ++i) {
        boxData[i] = static_cast<uint8_t>(i % 256);
    }
    
    std::cout << "Invoking constructEmsgBox with sz = " << testSize << " and boxData pointer = " << static_cast<void*>(boxData) << std::endl;
    player_isobmff::EmsgIsoBmffBox* emsgBox = nullptr;
    EXPECT_NO_THROW({
        emsgBox = player_isobmff::EmsgIsoBmffBox::constructEmsgBox(testSize, boxData);
        std::cout << "constructEmsgBox returned pointer = " << static_cast<void*>(emsgBox) << std::endl;
    });
    
    EXPECT_NE(emsgBox, nullptr);
    
    if (emsgBox) {
        std::cout << "Successfully constructed EmsgIsoBmffBox. Deleting the object now." << std::endl;
        delete emsgBox;
    }
    delete[] boxData;
    
    std::cout << "Exiting ValidEmsgBoxConstruction test" << std::endl;
}
/**
 * @brief Test that constructEmsgBox returns nullptr when a zero box size is provided.
 *
 * This test invokes constructEmsgBox with a box size of zero and a valid dummy data pointer to verify that the API returns a nullptr as expected when an invalid (zero) size is provided.
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
 * | Variation / Step | Description                                                                                           | Test Data                                                        | Expected Result                                                                            | Notes       |
 * | :--------------: | ----------------------------------------------------------------------------------------------------- | ---------------------------------------------------------------- | ------------------------------------------------------------------------------------------ | ----------- |
 * | 01               | Call constructEmsgBox with testSize = 0 and a valid dummyData pointer (allocated with size 10 bytes). | testSize = 0, dummyData = valid pointer, output: emsgBox pointer  | Returned pointer (emsgBox) should be nullptr as asserted by EXPECT_EQ(emsgBox, nullptr).      | Should Pass |
 */
TEST(player_isobmff_EmsgIsoBmffBox, ZeroBoxSizeProvided) {
    std::cout << "Entering ZeroBoxSizeProvided test" << std::endl;
    
    const uint32_t testSize = 0;
    std::cout << "Preparing valid dummy data pointer while box size is zero." << std::endl;
    // Allocate a small memory block though sz is 0 for the test purpose.
    uint8_t* dummyData = new uint8_t[10];
    std::cout << "Invoking constructEmsgBox with sz = " << testSize << " and dummyData pointer = " << static_cast<void*>(dummyData) << std::endl;
    player_isobmff::EmsgIsoBmffBox* emsgBox = player_isobmff::EmsgIsoBmffBox::constructEmsgBox(testSize, dummyData);
    std::cout << "constructEmsgBox returned pointer = " << static_cast<void*>(emsgBox) << std::endl;
    
    EXPECT_EQ(emsgBox, nullptr);
    
    delete[] dummyData;
    std::cout << "Exiting ZeroBoxSizeProvided test" << std::endl;
}
/**
 * @brief Verify that constructEmsgBox returns nullptr when provided with a null pointer for box data
 *
 * This test case validates that the EmsgIsoBmffBox::constructEmsgBox method correctly handles a scenario when the box data pointer is null. The purpose is to ensure that the method safely returns a nullptr under such conditions.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 025@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoke constructEmsgBox() with testSize=100 and a null pointer for box data | testSize = 100, boxData = nullptr | Return value is nullptr and EXPECT_EQ assertion passes | Should Fail |
 */
TEST(player_isobmff_EmsgIsoBmffBox, NullPointerForBoxData) {
    std::cout << "Entering NullPointerForBoxData test" << std::endl;
    
    const uint32_t testSize = 100;
    std::cout << "Invoking constructEmsgBox with sz = " << testSize << " and null ptr" << std::endl;
    player_isobmff::EmsgIsoBmffBox* emsgBox = player_isobmff::EmsgIsoBmffBox::constructEmsgBox(testSize, nullptr);
    std::cout << "constructEmsgBox returned pointer = " << static_cast<void*>(emsgBox) << std::endl;
    
    EXPECT_EQ(emsgBox, nullptr);
    
    std::cout << "Exiting NullPointerForBoxData test" << std::endl;
}
/**
 * @brief Verify that getEventDuration returns the expected duration for a properly constructed EmsgIsoBmffBox object.
 *
 * This test constructs an EmsgIsoBmffBox object using its first constructor with the parameters sz = 1000, tScale = 100, evtDur = 200, and _id = 1, 
 * and then verifies that the getEventDuration method returns the correct value of 200. This ensures that the object is initialized correctly and that the method behaves as expected.
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
 * | 01 | Construct the EmsgIsoBmffBox object using the first constructor and invoke the getEventDuration method. | sz = 1000, tScale = 100, evtDur = 200, _id = 1 | getEventDuration returns 200 and no exceptions are thrown | Should Pass |
 */
TEST(player_isobmff_EmsgIsoBmffBox, getEventDuration_Constructor1) {
    std::cout << "Entering getEventDuration_Constructor1 test" << std::endl;
    
    // Constructing EmsgIsoBmffBox object using first constructor with parameters:
    // sz = 1000, tScale = 100, evtDur = 200, _id = 1
    EXPECT_NO_THROW({
        player_isobmff::EmsgIsoBmffBox box(1000, 100, 200, 1);
        
        std::cout << "Constructed EmsgIsoBmffBox object with: sz=1000, tScale=100, evtDur=200, _id=1" << std::endl;
        std::cout << "Invoking getEventDuration method" << std::endl;
        
        uint32_t eventDuration = box.getEventDuration();
        
        std::cout << "Returned eventDuration value: " << eventDuration << std::endl;
        EXPECT_EQ(eventDuration, 200);
    });

    std::cout << "Exiting getEventDuration_Constructor1 test" << std::endl;
}
/**
 * @brief Test to verify that the getEventDuration method returns the correct event duration value using the second constructor.
 *
 * This test creates a FullIsoBmffBox with fixed parameters and subsequently constructs an EmsgIsoBmffBox using the second constructor.
 * It then invokes the getEventDuration method to confirm that the returned event duration is as expected. This ensures that the correct
 * initialization and method execution occur.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 027@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Construct FullIsoBmffBox with sz=1000, btype="emsg", ver=1, f=0 and then create EmsgIsoBmffBox with tScale=100, evtDur=300, _id=2, presTime=5000, presTimeDelta=20. Invoke getEventDuration to retrieve the event duration. | input1: sz=1000, input2: btype="emsg", input3: ver=1, input4: f=0, input5: tScale=100, input6: evtDur=300, input7: _id=2, input8: presTime=5000, input9: presTimeDelta=20 | API returns event duration value of 300 as checked using EXPECT_EQ(listener's eventDuration, 300). | Should Pass |
 */
TEST(player_isobmff_EmsgIsoBmffBox, getEventDuration_Constructor2) {
    std::cout << "Entering getEventDuration_Constructor2 test" << std::endl;
    
    // Construct FullIsoBmffBox object for initializing EmsgIsoBmffBox using second constructor.
    // FullIsoBmffBox parameters: sz = 1000, btype = "emsg", ver = 1, f = 0
    // Note: The btype parameter is a fixed-size char array.
    EXPECT_NO_THROW({
        player_isobmff::FullIsoBmffBox fullBox(1000, "emsg", 1, 0);
        std::cout << "Constructed FullIsoBmffBox object with: sz=1000, btype=\"emsg\", ver=1, f=0" << std::endl;
        
        // Constructing EmsgIsoBmffBox object using second constructor with parameters:
        // tScale = 100, evtDur = 300, _id = 2, presTime = 5000, presTimeDelta = 20
        player_isobmff::EmsgIsoBmffBox box(fullBox, 100, 300, 2, 5000, 20);
        std::cout << "Constructed EmsgIsoBmffBox object using second constructor with: tScale=100, evtDur=300, _id=2, presTime=5000, presTimeDelta=20" << std::endl;
        
        std::cout << "Invoking getEventDuration method" << std::endl;
        uint32_t eventDuration = box.getEventDuration();
        
        std::cout << "Returned eventDuration value: " << eventDuration << std::endl;
        EXPECT_EQ(eventDuration, 300);
    });
    
    std::cout << "Exiting getEventDuration_Constructor2 test" << std::endl;
}
/**
 * @brief Verify that getId() returns the correct id when using the typical first constructor.
 *
 * This test checks that the EmsgIsoBmffBox constructor sets up the box properly so that the getId() method returns the expected _id value. It confirms that no exceptions are thrown during object creation and that the id value is correctly retrieved.
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
 * | 01 | Invoke the EmsgIsoBmffBox constructor with typical parameters and log the action | sz = 10, tScale = 100, evtDur = 50, _id = 123 | Object is constructed without throwing exceptions | Should Pass |@n
 * | 02 | Call getId() on the constructed object and log the returned id value | No additional inputs; output: retId should be equal to _id (123) | Return value is equal to 123 and assertion passes | Should Pass |
 */
TEST(player_isobmff_EmsgIsoBmffBox, getId_Typical_FirstConstructor) {
    std::cout << "Entering getId_Typical_FirstConstructor test" << std::endl;
    
    // Logging the constructor parameters
    uint32_t sz = 10;
    uint32_t tScale = 100;
    uint32_t evtDur = 50;
    uint32_t _id = 123;
    std::cout << "Invoking EmsgIsoBmffBox constructor with parameters: sz = " << sz 
              << ", tScale = " << tScale << ", evtDur = " << evtDur 
              << ", _id = " << _id << std::endl;
              
    EXPECT_NO_THROW({
        player_isobmff::EmsgIsoBmffBox box(sz, tScale, evtDur, _id);
        
        // Log method invocation for getId()
        std::cout << "Calling getId() on constructed EmsgIsoBmffBox object" << std::endl;
        uint32_t retId = box.getId();
        std::cout << "getId() returned: " << retId << std::endl;
        
        // Check if returned value is as expected.
        EXPECT_EQ(retId, _id);
    });
    
    std::cout << "Exiting getId_Typical_FirstConstructor test" << std::endl;
}
/**
 * @brief Verify that getId() returns correct maximum id value using the first constructor
 *
 * This test verifies that when an EmsgIsoBmffBox object is constructed with the maximum uint32_t value for _id, 
 * the getId() method returns the identical id. This is important to ensure the constructor initializes the object's state correctly.
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
 * | Variation / Step | Description                                                        | Test Data                                                       | Expected Result                                                | Notes       |
 * | :--------------: | ------------------------------------------------------------------ | ---------------------------------------------------------------- | -------------------------------------------------------------- | ----------- |
 * | 01               | Construct the EmsgIsoBmffBox object with maximum uint32_t _id        | sz = 10, tScale = 100, evtDur = 50, _id = 4294967295U             | Object is created without throwing exception                   | Should Pass |
 * | 02               | Invoke getId() method and verify the returned id matches input _id    | Function Call: box.getId(), Expected Return: 4294967295U           | Returned id equals 4294967295U and assertion passes              | Should Pass |
 */
TEST(player_isobmff_EmsgIsoBmffBox, getId_MaxValue_FirstConstructor) {
    std::cout << "Entering getId_MaxValue_FirstConstructor test" << std::endl;
    
    // Logging the constructor parameters with maximum uint32_t id value
    uint32_t sz = 10;
    uint32_t tScale = 100;
    uint32_t evtDur = 50;
    uint32_t _id = 4294967295U;
    std::cout << "Invoking EmsgIsoBmffBox constructor with parameters: sz = " << sz 
              << ", tScale = " << tScale << ", evtDur = " << evtDur 
              << ", _id = " << _id << std::endl;
              
    EXPECT_NO_THROW({
        player_isobmff::EmsgIsoBmffBox box(sz, tScale, evtDur, _id);
        
        // Log method invocation for getId()
        std::cout << "Calling getId() on constructed EmsgIsoBmffBox object" << std::endl;
        uint32_t retId = box.getId();
        std::cout << "getId() returned: " << retId << std::endl;
        
        // Check if returned value is as expected.
        EXPECT_EQ(retId, _id);
    });
    
    std::cout << "Exiting getId_MaxValue_FirstConstructor test" << std::endl;
}
/**
 * @brief Validate that the getId() method returns the correct identifier using the second constructor of EmsgIsoBmffBox.
 *
 * This test verifies that after constructing a FullIsoBmffBox and then using it to construct an EmsgIsoBmffBox via its second constructor, the getId() method returns the expected identifier. It ensures that the internal state is correctly set during object instantiation and that the getId() function behaves as expected.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 030@n
 * **Priority:** (High) High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Log the start of the test execution | N/A | "Entering getId_Typical_SecondConstructor test" printed to console | Should be successful |
 * | 02 | Construct FullIsoBmffBox object using its constructor | sz = 10, btype = "emsg", ver = 0, f = 1 | FullIsoBmffBox object constructed without exceptions | Should Pass |
 * | 03 | Construct EmsgIsoBmffBox using the second constructor | tScale = 200, evtDur = 100, _id = 456, presTime = 1000, presTimeDelta = 50 | EmsgIsoBmffBox object constructed without exceptions | Should Pass |
 * | 04 | Invoke getId() method on the constructed EmsgIsoBmffBox object | Method call: getId() | Returns 456 as the identifier | Should Pass |
 * | 05 | Validate the returned identifier using an assertion | Expected: _id = 456, Actual: retId from getId() | EXPECT_EQ(retId, 456) assertion passes | Should Pass |
 */
TEST(player_isobmff_EmsgIsoBmffBox, getId_Typical_SecondConstructor) {
    std::cout << "Entering getId_Typical_SecondConstructor test" << std::endl;
    
    // Prepare FullIsoBmffBox object using its constructor.
    // Using strncpy to assign value to the fixed-size array for btype.
    uint32_t sz = 10;
    char btype[5] = {0};  // Allocating extra byte for null-terminator
    std::cout << "Using strncpy to set btype to \"emsg\"" << std::endl;
    strncpy(btype, "emsg", 4);
    uint8_t ver = 0;
    uint32_t f = 1;
    std::cout << "Invoking FullIsoBmffBox constructor with parameters: sz = " << sz 
              << ", btype = " << btype << ", ver = " << static_cast<uint32_t>(ver)
              << ", f = " << f << std::endl;
    
    EXPECT_NO_THROW({
        player_isobmff::FullIsoBmffBox fbox(sz, btype, ver, f);
        
        // Parameters for second EmsgIsoBmffBox constructor.
        uint32_t tScale = 200;
        uint32_t evtDur = 100;
        uint32_t _id = 456;
        uint64_t presTime = 1000;
        uint32_t presTimeDelta = 50;
        std::cout << "Invoking EmsgIsoBmffBox second constructor with parameters: tScale = " 
                  << tScale << ", evtDur = " << evtDur << ", _id = " << _id 
                  << ", presTime = " << presTime << ", presTimeDelta = " << presTimeDelta << std::endl;
                  
        player_isobmff::EmsgIsoBmffBox box(fbox, tScale, evtDur, _id, presTime, presTimeDelta);
        
        // Log method invocation for getId()
        std::cout << "Calling getId() on constructed EmsgIsoBmffBox object" << std::endl;
        uint32_t retId = box.getId();
        std::cout << "getId() returned: " << retId << std::endl;
        
        // Check if returned value is as expected.
        EXPECT_EQ(retId, _id);
    });
    
    std::cout << "Exiting getId_Typical_SecondConstructor test" << std::endl;
}
/**
 * @brief Validate that EmsgIsoBmffBox::getMessage() correctly returns a valid message pointer when invoked on a properly constructed object.
 *
 * This test verifies that the EmsgIsoBmffBox object, when constructed with specific parameters, does not throw an exception and that the subsequent call to getMessage() returns a non-null pointer. This ensures that the message retrieval functionality works as expected under normal operating conditions.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 031
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create an EmsgIsoBmffBox object using the first constructor with specific parameters | sz = 100, tScale = 1000, evtDur = 60, id = 1 | Object is constructed without throwing an exception | Should be successful |
 * | 02 | Invoke getMessage() on the constructed EmsgIsoBmffBox object | Invocation of getMessage() on the object; no explicit input arguments; output: pointer message | getMessage() returns a non-null pointer and does not throw an exception | Should Pass |
 */
TEST(player_isobmff_EmsgIsoBmffBox, getMessage()_start) {
    std::cout << "Entering getMessage()_start test" << std::endl;

    // Parameters for the first constructor
    uint32_t sz = 100;
    uint32_t tScale = 1000;
    uint32_t evtDur = 60;
    uint32_t id = 1;

    std::cout << "Creating EmsgIsoBmffBox object using first constructor with parameters:" 
              << " sz=" << sz 
              << ", tScale=" << tScale 
              << ", evtDur=" << evtDur 
              << ", id=" << id << std::endl;

    // Construct the object using the first constructor safely.
    EXPECT_NO_THROW({
        player_isobmff::EmsgIsoBmffBox box(sz, tScale, evtDur, id);
        
        std::cout << "Invoking getMessage() on constructed EmsgIsoBmffBox object" << std::endl;
        uint8_t* message = nullptr;
        EXPECT_NO_THROW(message = box.getMessage());

        std::cout << "getMessage() returned pointer value: " << static_cast<void*>(message) << std::endl;
        EXPECT_NE(message, nullptr);
    });

    std::cout << "Exiting getMessage()_start test" << std::endl;
}
/**
 * @brief Verify that the getMessageLen method returns the correct message length.
 *
 * This test ensures that an EmsgIsoBmffBox object can be successfully instantiated using the first constructor with specified parameters, and that invoking the getMessageLen method does not throw an exception and returns a valid message length.
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
 * | Variation / Step | Description                                                                                         | Test Data                                              | Expected Result                                                                                       | Notes         |
 * | :--------------: | --------------------------------------------------------------------------------------------------- | ------------------------------------------------------ | ----------------------------------------------------------------------------------------------------- | ------------- |
 * | 01               | Instantiate an EmsgIsoBmffBox object using first constructor with parameters: sz=100, tScale=1, evtDur=1, _id=1 | sz=100, tScale=1, evtDur=1, _id=1                        | Object is created successfully without any exceptions                                               | Should Pass   |
 * | 02               | Invoke the getMessageLen method on the created object and capture the returned message length        | output=messageLen                                      | The method returns a valid, non-zero message length without throwing any exception                    | Should Pass   |
 */
TEST(player_isobmff_EmsgIsoBmffBox, getMessageLen_start) {
    std::cout << "Entering getMessageLen_start test" << std::endl;
    // Creating object using first constructor: EmsgIsoBmffBox(sz = 100, tScale = 1, evtDur = 1, _id = 1)
    EXPECT_NO_THROW({
        player_isobmff::EmsgIsoBmffBox box(100, 1, 1, 1);
        std::cout << "Created EmsgIsoBmffBox using first constructor with parameters: sz=100, tScale=1, evtDur=1, _id=1" << std::endl;
        
        // Invoke getMessageLen method
        std::cout << "Invoking getMessageLen method" << std::endl;
        uint32_t messageLen = box.getMessageLen();
        std::cout << "getMessageLen returned: " << messageLen << std::endl;

    });
    std::cout << "Exiting getMessageLen_start test" << std::endl;
}
/**
 * @brief Verify that getPresentationTime() returns the expected presentation time.
 *
 * This test verifies that the getPresentationTime() method in the EmsgIsoBmffBox class correctly returns the presentation time passed during its construction. The test first creates a FullIsoBmffBox instance with specific parameters, then uses it to create an EmsgIsoBmffBox instance with a known presentation time. It subsequently calls the getPresentationTime() method and asserts that the returned value matches the expected presentation time. This ensures proper initialization and functioning of the associated method.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 033
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create FullIsoBmffBox instance with size=100, btype="TEST", version=1, flags=0 | size = 100, btype = "TEST", version = 1, flags = 0 | FullIsoBmffBox object is successfully created | Should be successful |
 * | 02 | Create EmsgIsoBmffBox instance using FullIsoBmffBox and parameters timeScale=1, eventDuration=10, id=100, presentationTime=12345, presentationTimeDelta=5 | timeScale = 1, eventDuration = 10, id = 100, presentationTime = 12345, presentationTimeDelta = 5 | EmsgIsoBmffBox object is successfully created | Should be successful |
 * | 03 | Invoke getPresentationTime() on the EmsgIsoBmffBox instance | Invocation of getPresentationTime(), expected return presentationTime = 12345 | Returned presentation time equals 12345 | Should Pass |
 */

TEST(player_isobmff_EmsgIsoBmffBox, getPresentationTime_start) {
    std::cout << "Entering getPresentationTime()_start test" << std::endl;
    
    // Prepare the btype for FullIsoBmffBox using strncpy.
    char btype[5] = {0};
    strncpy(btype, "TEST", 4);
    std::cout << "Creating FullIsoBmffBox with size: 100, btype: " << btype 
              << ", version: 1, flags: 0" << std::endl;
    
    // Create a FullIsoBmffBox object using its custom constructor.
    player_isobmff::FullIsoBmffBox fullBox(100, btype, 1, 0);
    
    // Define parameters for EmsgIsoBmffBox using the second constructor.
    uint32_t timeScale = 1;
    uint32_t eventDuration = 10;
    uint32_t id = 100;
    uint64_t presTime = 12345;
    uint32_t presTimeDelta = 5;
    
    std::cout << "Creating EmsgIsoBmffBox with parameters:" << std::endl;
    std::cout << "  timeScale: " << timeScale << std::endl;
    std::cout << "  eventDuration: " << eventDuration << std::endl;
    std::cout << "  id: " << id << std::endl;
    std::cout << "  presentationTime: " << presTime << std::endl;
    std::cout << "  presentationTimeDelta: " << presTimeDelta << std::endl;
    
    // Create an EmsgIsoBmffBox object using the second constructor.
    player_isobmff::EmsgIsoBmffBox emsgBox(fullBox, timeScale, eventDuration, id, presTime, presTimeDelta);
    
    // Invoke the getPresentationTime method and log its invocation.
    std::cout << "Invoking getPresentationTime() method." << std::endl;
    uint64_t returnedPresTime = emsgBox.getPresentationTime();
    std::cout << "getPresentationTime() returned: " << returnedPresTime << std::endl;
    
    // Validate that the returned presentation time matches the expected value.
    EXPECT_EQ(returnedPresTime, presTime);
    
    std::cout << "Exiting getPresentationTime()_start test" << std::endl;
}
/**
 * @brief Validate that getPresentationTimeDelta returns the expected non-zero value
 *
 * This test verifies the correct construction of FullIsoBmffBox and EmsgIsoBmffBox objects using valid parameters, and subsequently checks that the getPresentationTimeDelta method returns the expected non-zero presentation time delta value without throwing exceptions.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Construct FullIsoBmffBox object with valid inputs | fullBoxSize = 100, fixedBoxType = "test", version = 1, flags = 10 | FullIsoBmffBox constructed without exceptions | Should Pass |
 * | 02 | Construct EmsgIsoBmffBox object using FullIsoBmffBox and valid parameters | timeScale = 1000, eventDuration = 200, id = 123, presentationTime = 10000, presentationTimeDelta = 500 | EmsgIsoBmffBox constructed without exceptions | Should Pass |
 * | 03 | Invoke getPresentationTimeDelta() and verify the returned value | Method Call: getPresentationTimeDelta() | Returns 500; EXPECT_EQ verifies value | Should Pass |
 */
TEST(player_isobmff_EmsgIsoBmffBox, getPresentationTimeDelta_validPositiveNonZero) {
    std::cout << "Entering getPresentationTimeDelta_validPositiveNonZero test" << std::endl;

    uint32_t fullBoxSize = 100;
    char boxType[5] = "test";  // Using 5 size to include null terminator, but boxType is of fixed size of 4 characters in the constructor.
    // Using strncpy to safely assign fixed size array.
    char fixedBoxType[4];
    std::memset(fixedBoxType, 0, sizeof(fixedBoxType));
    std::strncpy(fixedBoxType, boxType, sizeof(fixedBoxType));
    
    uint8_t version = 1;
    uint32_t flags = 10;
    // Creating a FullIsoBmffBox object. 
    // EXPECT_NO_THROW is used to validate that there is no exception thrown during construction.
    EXPECT_NO_THROW({
        player_isobmff::FullIsoBmffBox fullBox(fullBoxSize, fixedBoxType, version, flags);
        
        // Create an EmsgIsoBmffBox object using the second constructor:
        // EmsgIsoBmffBox ( FullIsoBmffBox & fbox, uint32_t tScale, uint32_t evtDur, uint32_t _id, uint64_t presTime, uint32_t presTimeDelta )
        uint32_t timeScale = 1000;
        uint32_t eventDuration = 200;
        uint32_t id = 123;
        uint64_t presentationTime = 10000;
        uint32_t presentationTimeDeltaExpected = 500;

        std::cout << "Creating player_isobmff::EmsgIsoBmffBox with parameters:" << std::endl;
        std::cout << "  timeScale: " << timeScale << std::endl;
        std::cout << "  eventDuration: " << eventDuration << std::endl;
        std::cout << "  id: " << id << std::endl;
        std::cout << "  presentationTime: " << presentationTime << std::endl;
        std::cout << "  presentationTimeDelta: " << presentationTimeDeltaExpected << std::endl;

        player_isobmff::EmsgIsoBmffBox emsgBox(fullBox, timeScale, eventDuration, id, presentationTime, presentationTimeDeltaExpected);

        // Invoke getPresentationTimeDelta method and capture the result.
        std::cout << "Invoking getPresentationTimeDelta() method." << std::endl;
        uint32_t presTimeDeltaReturned = 0;
        EXPECT_NO_THROW({
            presTimeDeltaReturned = emsgBox.getPresentationTimeDelta();
        });
        std::cout << "getPresentationTimeDelta() returned: " << presTimeDeltaReturned << std::endl;

        // Check if the returned presentationTimeDelta matches the expected output.
        EXPECT_EQ(presTimeDeltaReturned, presentationTimeDeltaExpected);
    });

    std::cout << "Exiting getPresentationTimeDelta_validPositiveNonZero test" << std::endl;
}
/**
 * @brief Test the getSchemeIdUri() method using the first constructor
 *
 * This test validates that the getSchemeIdUri() method returns a non-null pointer when an object is constructed using the first constructor with valid parameters. The test checks that no exception is thrown during construction and that the returned schemeIdUri is valid.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Construct object using first constructor with parameters | sz = 100, timeScale = 1000, eventDuration = 50, id = 1 | Object is constructed without throwing an exception | Should Pass |
 * | 02 | Invoke getSchemeIdUri() method on the constructed object | Object created above; method getSchemeIdUri() invoked | Returned pointer is non-null and the string is not empty | Should Pass |
 */
TEST(player_isobmff_EmsgIsoBmffBox, getSchemeIdUri_positive_first_constructor)
{
    std::cout << "Entering getSchemeIdUri_positive_first_constructor test" << std::endl;
    
    // Construct the object using the first constructor
    // Parameters: sz, timeScale, eventDuration, id
    const uint32_t sz = 100;
    const uint32_t tScale = 1000;
    const uint32_t evtDur = 50;
    const uint32_t id = 1;
    EXPECT_NO_THROW({ player_isobmff::EmsgIsoBmffBox obj(sz, tScale, evtDur, id); });
    player_isobmff::EmsgIsoBmffBox obj(sz, tScale, evtDur, id);
    std::cout << "Invoked first constructor with parameters: sz=" << sz 
              << ", timeScale=" << tScale 
              << ", eventDuration=" << evtDur 
              << ", id=" << id << std::endl;             

    // Invoke the method getSchemeIdUri()
    std::cout << "Invoking getSchemeIdUri() method on object constructed via first constructor" << std::endl;
    char* schemeIdUri = obj.getSchemeIdUri();
    std::cout << "Method getSchemeIdUri() returned: " 
              << (schemeIdUri ? schemeIdUri : "null") << std::endl;

    // Validate that the returned pointer is non-null and the string is not empty.
    EXPECT_NE(schemeIdUri, nullptr);

    std::cout << "Exiting getSchemeIdUri_positive_first_constructor test" << std::endl;
}
/**
 * @brief Verify getSchemeIdUri returns the correct scheme identifier URI using the second constructor of EmsgIsoBmffBox.
 *
 * This test verifies that the method getSchemeIdUri() returns the correct scheme identifier URI as specified during object construction using the second constructor. It ensures that the FullIsoBmffBox and EmsgIsoBmffBox objects are correctly constructed without exceptions and that getSchemeIdUri() returns the expected value.
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
 * | Variation / Step | Description                                                                                  | Test Data                                                                | Expected Result                                                      | Notes        |
 * | :--------------: | -------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------ | -------------------------------------------------------------------- | ------------ |
 * | 01               | Construct FullIsoBmffBox using the second constructor parameters.                             | sz = 200, btype = abcd, version = 1, flags = 0                           | Object constructed without throwing exception.                     | Should Pass  |
 * | 02               | Construct EmsgIsoBmffBox using the second constructor with FullIsoBmffBox and additional parameters. | tScale = 1000, evtDur = 50, id = 2, presTime = 123456789, presTimeDelta = 30 | Object constructed without throwing exception.                     | Should Pass  |
 * | 03               | Invoke getSchemeIdUri on the constructed EmsgIsoBmffBox and validate the returned schemeIdUri.  | N/A                                                                    | Return value is not null and equals "expected_scheme".               | Should Pass  |
 */
TEST(player_isobmff_EmsgIsoBmffBox, getSchemeIdUri_positive_second_constructor)
{
    std::cout << "Entering getSchemeIdUri_positive_second_constructor test" << std::endl;
    
    // Create a FullIsoBmffBox object required for the second constructor.
    // FullIsoBmffBox constructor parameters: sz, btype, version, flags.
    const uint32_t fullBoxSz = 200;
    char btype[4];
    strncpy(btype, "abcd", 4);
    const uint8_t version = 1;
    const uint32_t flags = 0;
    EXPECT_NO_THROW({
        player_isobmff::FullIsoBmffBox fullBox(fullBoxSz, btype, version, flags);
    });
    player_isobmff::FullIsoBmffBox fullBox(fullBoxSz, btype, version, flags);
    std::cout << "Constructed FullIsoBmffBox with parameters: sz=" << fullBoxSz 
              << ", btype=";
    for (int i = 0; i < 4; ++i)
    {
        std::cout << btype[i];
    }
    std::cout << ", version=" << static_cast<uint32_t>(version)
              << ", flags=" << flags << std::endl;
    
    // Now create an EmsgIsoBmffBox using the second constructor.
    // Second constructor parameters: FullIsoBmffBox&, timeScale, eventDuration, id, presentationTime, presentationTimeDelta.
    const uint32_t tScale = 1000;
    const uint32_t evtDur = 50;
    const uint32_t id = 2;
    const uint64_t presTime = 123456789;
    const uint32_t presTimeDelta = 30;
    EXPECT_NO_THROW({
        player_isobmff::EmsgIsoBmffBox obj(fullBox, tScale, evtDur, id, presTime, presTimeDelta);
    });
    player_isobmff::EmsgIsoBmffBox obj(fullBox, tScale, evtDur, id, presTime, presTimeDelta);
    std::cout << "Invoked second constructor with parameters: timeScale=" << tScale 
              << ", eventDuration=" << evtDur 
              << ", id=" << id 
              << ", presentationTime=" << presTime 
              << ", presentationTimeDelta=" << presTimeDelta << std::endl;
    
    // Invoke the method getSchemeIdUri()
    std::cout << "Invoking getSchemeIdUri() method on object constructed via second constructor" << std::endl;
    char* schemeIdUri = obj.getSchemeIdUri();
    std::cout << "Method getSchemeIdUri() returned: " 
              << (schemeIdUri ? schemeIdUri : "null") << std::endl;
              
    // Define the expected schemeIdUri based on the construction details.
    // For the purpose of this test, we assume the constructor sets schemeIdUri to "expected_scheme".
    const char expectedSchemeIdUri[] = "expected_scheme";
    std::cout << "Expected schemeIdUri is: " << expectedSchemeIdUri << std::endl;
    
    // Validate that the retrieved schemeIdUri matches the expected value.
    EXPECT_NE(schemeIdUri, nullptr);
    EXPECT_STREQ(schemeIdUri, expectedSchemeIdUri);
    std::cout << "Internal state: Retrieved schemeIdUri = " << schemeIdUri << std::endl;
    
    std::cout << "Exiting getSchemeIdUri_positive_second_constructor test" << std::endl;
}
/**
 * @brief Verify that getTimeScale() returns 1000 when constructing EmsgIsoBmffBox using the first constructor
 *
 * This test verifies that when an EmsgIsoBmffBox object is constructed with specific arbitrary parameter values (boxSize: 64, timeScale: 1000, eventDuration: 500, id: 1) using the first constructor, the getTimeScale() method correctly returns 1000. This confirms that the object's time scale is properly assigned during initialization.
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
 * | Variation / Step | Description                                                           | Test Data                                                      | Expected Result                                                    | Notes         |
 * | :--------------: | --------------------------------------------------------------------- | -------------------------------------------------------------- | ------------------------------------------------------------------ | ------------- |
 * | 01               | Construct EmsgIsoBmffBox with arbitrary parameters using the first constructor | boxSize = 64, timeScale = 1000, eventDuration = 500, id = 1      | No exception thrown during object construction                     | Should Pass   |
 * | 02               | Call getTimeScale() on the constructed object to verify the time scale value   | Method call: getTimeScale(); Expected return value = 1000       | getTimeScale() returns 1000 as expected                              | Should Pass   |
 */
TEST(player_isobmff_EmsgIsoBmffBox, getTimeScale_Returns_1000_FromFirstConstructor) {
    std::cout << "Entering getTimeScale_Returns_1000_FromFirstConstructor test" << std::endl;
    
    // Expected timeScale value
    uint32_t expectedTimeScale = 1000;
    // Using arbitrary example values for other constructor parameters
    uint32_t boxSize = 64;
    uint32_t eventDuration = 500;
    uint32_t id = 1;
    
    std::cout << "Invoking first constructor of EmsgIsoBmffBox with parameters:" << std::endl;
    std::cout << "boxSize: " << boxSize << ", timeScale: " << expectedTimeScale 
              << ", eventDuration: " << eventDuration << ", id: " << id << std::endl;
              
    EXPECT_NO_THROW({
        player_isobmff::EmsgIsoBmffBox emsgBox(boxSize, expectedTimeScale, eventDuration, id);
        std::cout << "Calling getTimeScale() on EmsgIsoBmffBox" << std::endl;
        uint32_t timeScale = emsgBox.getTimeScale();
        std::cout << "Retrieved timeScale value: " << timeScale << std::endl;
        EXPECT_EQ(timeScale, expectedTimeScale);
    });
    
    std::cout << "Exiting getTimeScale_Returns_1000_FromFirstConstructor test" << std::endl;
}
/**
 * @brief Verify that getTimeScale returns the expected time scale value (2000) when using the second constructor of EmsgIsoBmffBox
 *
 * This test verifies that when an EmsgIsoBmffBox object is constructed using its second constructor with a dummy FullIsoBmffBox and specific parameters, the getTimeScale() method returns the correct value. It validates the proper initialization and functionality of the method.
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
 * | 01 | Create a dummy FullIsoBmffBox object to simulate valid input for the second constructor of EmsgIsoBmffBox | fullBoxSize = 128, bType = "BTP1", version = 1, flags = 0 | FullIsoBmffBox object is created without any exception | Should be successful |
 * | 02 | Invoke the second constructor of EmsgIsoBmffBox with predefined parameters | expectedTimeScale = 2000, eventDuration = 600, id = 2, presentationTime = 123456789, presentationTimeDelta = 50 | EmsgIsoBmffBox object is successfully instantiated | Should Pass |
 * | 03 | Call getTimeScale() on the EmsgIsoBmffBox object | method call: getTimeScale() | Returns timeScale value equal to 2000 and passes the assertion (EXPECT_EQ) | Should Pass |
 */
TEST(player_isobmff_EmsgIsoBmffBox, getTimeScale_Returns_2000_FromSecondConstructor) {
    std::cout << "Entering getTimeScale_Returns_2000_FromSecondConstructor test" << std::endl;
    
    // Prepare dummy FullIsoBmffBox for input to second constructor.
    uint32_t fullBoxSize = 128;
    char bType[4];
    // Using strncpy to assign value to fixed size array bType
    std::strncpy(bType, "BTP1", 4);
    uint8_t version = 1;
    uint32_t flags = 0;
    
    std::cout << "Invoking constructor of FullIsoBmffBox with parameters:" << std::endl;
    std::cout << "fullBoxSize: " << fullBoxSize << ", bType: " << bType 
              << ", version: " << static_cast<unsigned>(version) << ", flags: " << flags << std::endl;
              
    EXPECT_NO_THROW({
        // Create dummy FullIsoBmffBox object (assuming its namespace and constructor are accessible)
        player_isobmff::FullIsoBmffBox fullBox(fullBoxSize, bType, version, flags);
        
        // Parameters for second constructor of EmsgIsoBmffBox
        uint32_t expectedTimeScale = 2000;
        uint32_t eventDuration = 600;
        uint32_t id = 2;
        uint64_t presentationTime = 123456789;
        uint32_t presentationTimeDelta = 50;
        
        std::cout << "Invoking second constructor of EmsgIsoBmffBox with parameters:" << std::endl;
        std::cout << "timeScale: " << expectedTimeScale << ", eventDuration: " << eventDuration 
                  << ", id: " << id << ", presentationTime: " << presentationTime 
                  << ", presentationTimeDelta: " << presentationTimeDelta << std::endl;
                  
        player_isobmff::EmsgIsoBmffBox emsgBox(fullBox, expectedTimeScale, eventDuration, id, presentationTime, presentationTimeDelta);
        
        std::cout << "Calling getTimeScale() on EmsgIsoBmffBox" << std::endl;
        uint32_t timeScale = emsgBox.getTimeScale();
        std::cout << "Retrieved timeScale value: " << timeScale << std::endl;
        EXPECT_EQ(timeScale, expectedTimeScale);
    });
    
    std::cout << "Exiting getTimeScale_Returns_2000_FromSecondConstructor test" << std::endl;
}
/**
 * @brief Test retrieving the value pointer from a valid EmsgIsoBmffBox instance
 *
 * This test case verifies that invoking getValue() on a validly constructed EmsgIsoBmffBox instance returns a non-null pointer.
 * The objective is to ensure that the method does not throw any exceptions during construction and invocation, and that it properly provides the expected output.
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
 * | Variation / Step | Description                                                                          | Test Data                                               | Expected Result                                                        | Notes      |
 * | :--------------: | ------------------------------------------------------------------------------------ | ------------------------------------------------------- | ---------------------------------------------------------------------- | ---------- |
 * | 01               | Create an instance of EmsgIsoBmffBox with valid parameters and invoke getValue() method | size=100, timeScale=1000, eventDuration=200, id=1; output pointer variable expected | getValue() returns a non-null pointer and no exception is thrown | Should Pass |
 */
TEST(player_isobmff_EmsgIsoBmffBox, getValue_start) {
    std::cout << "Entering player_isobmff::EmsgIsoBmffBox::getValue()_start test" << std::endl;
    
    // Expect no throw during construction and method invocation
    EXPECT_NO_THROW({
        // Create an instance using the first constructor with valid parameters
        // Parameters: size=100, timeScale=1000, eventDuration=200, id=1
        player_isobmff::EmsgIsoBmffBox box(100, 1000, 200, 1);
        std::cout << "Created EmsgIsoBmffBox object using first constructor with parameters: "
                  << "size=100, timeScale=1000, eventDuration=200, id=1" << std::endl;
        
        // Invoke getValue() method
        std::cout << "Invoking getValue() method on the constructed object." << std::endl;
        uint8_t* retValue = box.getValue();
        std::cout << "getValue() returned pointer: " << static_cast<void*>(retValue) << std::endl;
        
        // Verify that the returned pointer is not null
        EXPECT_NE(retValue, nullptr);
        
    });
    
    std::cout << "Exiting player_isobmff::EmsgIsoBmffBox::getValue()_start test" << std::endl;
}
/**
 * @brief Verifies that getValue() returns a valid non-null pointer.
 *
 * This test case creates a FullIsoBmffBox object and then uses it to construct an EmsgIsoBmffBox object using its second constructor. The test then invokes the getValue() method and checks that the returned pointer is not null, ensuring the proper behavior of the getValue() functionality.
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
 * | Variation / Step | Description                                                                  | Test Data                                                                                       | Expected Result                                      | Notes          |
 * | :--------------: | ---------------------------------------------------------------------------- | ---------------------------------------------------------------------------------------------- | ---------------------------------------------------- | -------------- |
 * | 01               | Create a FullIsoBmffBox object                                               | size = 100, btype = "test", version = 1, flags = 0                                             | Object is created successfully                       | Should be successful |
 * | 02               | Create an EmsgIsoBmffBox object using the second constructor                   | timeScale = 2000, eventDuration = 400, id = 2, presentationTime = 5000, presentationTimeDelta = 100 | Object is created successfully                       | Should be successful |
 * | 03               | Invoke getValue() method on the constructed EmsgIsoBmffBox object and verify it  | Calling getValue() on the constructed EmsgIsoBmffBox object                                      | Returned pointer is not null and passes EXPECT_NE    | Should Pass    |
 */
TEST(player_isobmff_EmsgIsoBmffBox, getValue_end) {
    std::cout << "Entering player_isobmff::EmsgIsoBmffBox::getValue()_end test" << std::endl;
    
    EXPECT_NO_THROW({
        FullIsoBmffBox fullBox(100, "test", 1, 0);
        std::cout << "Created FullIsoBmffBox object with parameters: "
                  << "size=100, btype=\"test\", version=1, flags=0" << std::endl;
        
        player_isobmff::EmsgIsoBmffBox box(fullBox, 2000, 400, 2, 5000, 100);
        std::cout << "Created EmsgIsoBmffBox object using second constructor with parameters: "
                  << "timeScale=2000, eventDuration=400, id=2, presentationTime=5000, presentationTimeDelta=100" << std::endl;
        
        // Invoke getValue() method
        std::cout << "Invoking getValue() method on the constructed object." << std::endl;
        uint8_t* retValue = box.getValue();
        std::cout << "getValue() returned pointer: " << static_cast<void*>(retValue) << std::endl;
        
        // Verify that the returned pointer is not null
        EXPECT_NE(retValue, nullptr);
        
    });
    
    std::cout << "Exiting player_isobmff::EmsgIsoBmffBox::getValue()_end test" << std::endl;
}
/**
 * @brief Verify that setting event duration to zero does not throw an exception.
 *
 * Validate that invoking setEventDuration with 0 on a valid EmsgIsoBmffBox object successfully handles the update without throwing any exceptions.@n
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
 * | 01 | Create an instance of EmsgIsoBmffBox with an initial eventDuration value of 500 | input: size = 128, type = 1, eventDuration = 500, additional = 42 | Object is successfully constructed without throwing an exception | Should be successful |
 * | 02 | Invoke setEventDuration with new event duration value of 0 on the created object | input: newEvtDur = 0, output: updated eventDuration property | setEventDuration executes without throwing any exception | Should Pass |
 */
TEST(player_isobmff_EmsgIsoBmffBox, SetEventDurationZero)
{
    std::cout << "Entering SetEventDurationZero test" << std::endl;
    // Create object using custom constructor with initial eventDuration value (e.g., 500)
    EXPECT_NO_THROW({
        player_isobmff::EmsgIsoBmffBox box(128, 1, 500, 42);
        std::cout << "Constructed EmsgIsoBmffBox with initial eventDuration: " << 500 << std::endl;
        
        // Define new event duration value 0 and invoke setEventDuration.
        uint32_t newEvtDur = 0;
        std::cout << "Invoking setEventDuration with value: " << newEvtDur << std::endl;
        box.setEventDuration(newEvtDur);
        std::cout << "setEventDuration invoked" << std::endl;
        
    });
    std::cout << "Exiting SetEventDurationZero test" << std::endl;
}
/**
 * @brief Test that verifies setting a typical event duration value using setEventDuration on an EmsgIsoBmffBox object
 *
 * This test creates an instance of EmsgIsoBmffBox with an initial event duration value of 300 and then updates the event duration to a new typical positive value (1000) by invoking setEventDuration. The test ensures that no exceptions are thrown during object construction and the method call, indicating that the API functions correctly in a normal scenario.
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
 * | 01 | Create an instance of EmsgIsoBmffBox using the custom constructor with initial eventDuration of 300 | input1 = 256, input2 = 1, input3 = 300, input4 = 42 | No exception thrown during object construction | Should Pass |
 * | 02 | Invoke setEventDuration on the constructed object with a new event duration value of 1000 | newEvtDur = 1000 | No exception thrown; event duration successfully updated to 1000 | Should Pass |
 */
TEST(player_isobmff_EmsgIsoBmffBox, SetEventDurationTypical)
{
    std::cout << "Entering SetEventDurationTypical test" << std::endl;
    // Create object using custom constructor with initial eventDuration value (e.g., 300)
    EXPECT_NO_THROW({
        player_isobmff::EmsgIsoBmffBox box(256, 1, 300, 42);
        std::cout << "Constructed EmsgIsoBmffBox with initial eventDuration: " << 300 << std::endl;
        
        // Define new event duration value 1000 and invoke setEventDuration.
        uint32_t newEvtDur = 1000;
        std::cout << "Invoking setEventDuration with value: " << newEvtDur << std::endl;
        box.setEventDuration(newEvtDur);
        std::cout << "setEventDuration invoked" << std::endl;
        
    });
    std::cout << "Exiting SetEventDurationTypical test" << std::endl;
}
/**
 * @brief Validate that setId correctly assigns a typical positive number.
 *
 * This test verifies that the EmsgIsoBmffBox object's setId method properly sets a typical positive value without throwing exceptions. It checks the object's construction using the provided constructor and then invokes setId with a typical positive number to ensure the operation succeeds as expected.
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
 * | 01 | Construct an EmsgIsoBmffBox object using the constructor with valid parameters. | sz = 100, tScale = 50, evtDur = 20, _id = 0 | Object is created successfully without throwing an exception. | Should be successful |
 * | 02 | Invoke setId on the constructed object with a positive number to update its id. | newId = 123 | setId updates the id to 123 without throwing an exception. | Should Pass |
 */
TEST(player_isobmff_EmsgIsoBmffBox, SetId_TypicalPositiveNumber) {
    std::cout << "Entering SetId_TypicalPositiveNumber test" << std::endl;
    
    // Create an object using the provided constructor.
    // Using: EmsgIsoBmffBox(uint32_t sz, uint32_t tScale, uint32_t evtDur, uint32_t _id)
    EXPECT_NO_THROW({
        player_isobmff::EmsgIsoBmffBox box(100, 50, 20, 0);
        std::cout << "Constructed EmsgIsoBmffBox object with initial id 0" << std::endl;
        
        // Invoke setId with a typical positive number
        uint32_t newId = 123;
        std::cout << "Invoking setId with value: " << newId << std::endl;
        EXPECT_NO_THROW({
            box.setId(newId);
        });
        std::cout << "setId invoked successfully" << std::endl;
    });
    
    std::cout << "Exiting SetId_TypicalPositiveNumber test" << std::endl;
}
/**
 * @brief Verify that EmsgIsoBmffBox properly handles a valid non-zero length message.
 *
 * This test case verifies that the EmsgIsoBmffBox object can be constructed using a custom constructor with specific arbitrary values, and that its setMessage method can be invoked with a valid non-null message array and non-zero length without throwing exceptions.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Construct the EmsgIsoBmffBox object using parameters sz=100, timeScale=1000, eventDuration=200, id=1. | input: sz=100, timeScale=1000, eventDuration=200, id=1 | Object constructed without throwing an exception. | Should Pass |
 * | 02 | Prepare a valid uint8_t array message with values {0x01, 0x02, 0x03} and length = 3. | input: msg = {0x01, 0x02, 0x03}, len = 3 | Message array prepared successfully. | Should be successful |
 * | 03 | Invoke the setMessage method on the constructed object with the prepared message and length. | input: msg pointer and len = 3 | Method executed without throwing an exception. | Should Pass |
 */
TEST(player_isobmff_EmsgIsoBmffBox, PositiveValidMessage_NonZeroLength) {
    std::cout << "Entering PositiveValidMessage_NonZeroLength test" << std::endl;

    // Create object using custom constructor
    // Using arbitrary values for sz, timeScale, eventDuration, and id
    EXPECT_NO_THROW({
        player_isobmff::EmsgIsoBmffBox box(100, 1000, 200, 1);
        std::cout << "Constructed EmsgIsoBmffBox object using constructor with parameters: "
                  << "sz=100, timeScale=1000, eventDuration=200, id=1" << std::endl;

        // Prepare a valid uint8_t array message: {0x01, 0x02, 0x03}
        uint8_t msg[3] = {0x01, 0x02, 0x03};
        uint32_t len = 3;
        std::cout << "Prepared message array: {0x01, 0x02, 0x03} with length = " << len << std::endl;

        // Invoke setMessage method
        std::cout << "Invoking setMessage with the above message and length = " << len << std::endl;
        EXPECT_NO_THROW(box.setMessage(msg, len));
        std::cout << "setMessage invoked successfully." << std::endl;
    });

    std::cout << "Exiting PositiveValidMessage_NonZeroLength test" << std::endl;
}
/**
 * @brief Validate handling of a zero-length message input in setMessage API.
 *
 * This test verifies that calling the setMessage method on an EmsgIsoBmffBox object with a valid message pointer but a zero length does not throw any exception. The test confirms that the implementation can gracefully handle zero-length message inputs, maintaining robust operation without errors.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 045@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                      | Test Data                                                        | Expected Result                                                 | Notes           |
 * | :--------------: | ------------------------------------------------------------------------------------------------ | ---------------------------------------------------------------- | --------------------------------------------------------------- | --------------- |
 * | 01               | Construct an EmsgIsoBmffBox object using the custom constructor with parameters                  | sz = 100, timeScale = 2000, eventDuration = 300, id = 2            | Object is created without exceptions                             | Should be successful |
 * | 02               | Prepare a valid message array containing {0x01, 0x02, 0x03} and set length to 0                  | msg = {0x01, 0x02, 0x03}, len = 0                                  | Message array is prepared correctly with the specified length      | Should be successful |
 * | 03               | Invoke setMessage on the object with the prepared message array and zero length                  | msg = {0x01, 0x02, 0x03}, len = 0                                  | setMessage executes without throwing any exceptions                | Should Pass     |
 */
TEST(player_isobmff_EmsgIsoBmffBox, PositiveValidMessage_ZeroLength) {
    std::cout << "Entering PositiveValidMessage_ZeroLength test" << std::endl;

    EXPECT_NO_THROW({
        // Create object using custom constructor
        player_isobmff::EmsgIsoBmffBox box(100, 2000, 300, 2);
        std::cout << "Constructed EmsgIsoBmffBox object using constructor with parameters: "
                  << "sz=100, timeScale=2000, eventDuration=300, id=2" << std::endl;

        // Prepare a valid uint8_t array message: {0x01, 0x02, 0x03}
        uint8_t msg[3] = {0x01, 0x02, 0x03};
        uint32_t len = 0;
        std::cout << "Prepared message array: {0x01, 0x02, 0x03} with length = " << len << std::endl;

        // Invoke setMessage method with zero length
        std::cout << "Invoking setMessage with the above message and length = " << len << std::endl;
        EXPECT_NO_THROW(box.setMessage(msg, len));
        std::cout << "setMessage invoked successfully with zero length." << std::endl;

    });

    std::cout << "Exiting PositiveValidMessage_ZeroLength test" << std::endl;
}
/**
 * @brief Verify that setMessage correctly handles a null message pointer when a non-zero length is provided.
 *
 * This test case verifies that the EmsgIsoBmffBox class properly handles a scenario where a null message pointer is passed to the setMessage method with a non-zero length. The test ensures that the API either throws an exception or safely ignores the input, thereby preventing any corruption of the object's internal state.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Construct an EmsgIsoBmffBox object using the custom constructor. | sz = 100, timeScale = 3000, eventDuration = 400, id = 3 | Object is successfully constructed with the specified parameters. | Should be successful |
 * | 02 | Prepare a null message pointer with a non-zero length value. | nullMessage = nullptr, len = 5 | A null pointer is prepared alongside the length value. | Should be successful |
 * | 03 | Invoke setMessage on the EmsgIsoBmffBox object with the null message pointer and non-zero length. | nullMessage = nullptr, len = 5 | The API either throws an exception or safely ignores the null pointer input. | Should Fail |
 * | 04 | Verify that the method handles the null pointer input appropriately without corrupting the internal state. | Check exceptionCaught flag and internal state | Test passes if either an exception is caught or the state remains valid. | Should be successful |
 */
TEST(player_isobmff_EmsgIsoBmffBox, NegativeNullMessage_NonZeroLength) {
    std::cout << "Entering NegativeNullMessage_NonZeroLength test" << std::endl;

    // Create object using custom constructor
    player_isobmff::EmsgIsoBmffBox box(100, 3000, 400, 3);
    std::cout << "Constructed EmsgIsoBmffBox object using constructor with parameters: "
              << "sz=100, timeScale=3000, eventDuration=400, id=3" << std::endl;

    uint8_t* nullMessage = nullptr;
    uint32_t len = 5;
    std::cout << "Prepared null message pointer with length = " << len << std::endl;

    // Invoke setMessage with null message pointer and non-zero length
    std::cout << "Invoking setMessage with null message pointer and length = " << len << std::endl;
    
    bool exceptionCaught = false;
    try {
        box.setMessage(nullMessage, len);
        std::cout << "setMessage invoked without throwing an exception." << std::endl;
    } catch(const std::exception &e) {
        exceptionCaught = true;
        std::cout << "Caught std::exception: " << e.what() << std::endl;
    } catch(...) {
        exceptionCaught = true;
        std::cout << "Caught unknown exception during setMessage with null pointer." << std::endl;
    }
    
    // Both behaviors (throwing or safely ignoring) are acceptable as per specification.
    if (exceptionCaught) {
        SUCCEED() << "setMessage threw an exception as expected when provided a null message pointer.";
    } else {
        SUCCEED() << "setMessage safely ignored the null message pointer without corrupting internal state.";
    }
    
    std::cout << "DEBUG: Expected internal state: For null message input, the method should either trigger an error/exception or ignore the input safely." << std::endl;
    std::cout << "Exiting NegativeNullMessage_NonZeroLength test" << std::endl;
}
/**
 * @brief Test the setPresentationTime API with a typical positive value.
 *
 * This test verifies that the setPresentationTime method correctly updates the internal presentation time of the EmsgIsoBmffBox object when a typical positive value is provided. It ensures the object is constructed without exceptions and the presentation time update is successful.
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
 * | Variation / Step | Description                                                                   | Test Data                                               | Expected Result                                                     | Notes       |
 * | :--------------: | ----------------------------------------------------------------------------- | ------------------------------------------------------- | ------------------------------------------------------------------- | ----------- |
 * | 01               | Create an EmsgIsoBmffBox object using constructor with provided parameters.   | size=1024, timeScale=1, eventDuration=200, id=10          | Object is created successfully without throwing any exceptions.     | Should Pass |
 * | 02               | Invoke setPresentationTime with a typical positive value to update its state.   | presTime=1000                                           | Presentation time is updated to 1000 without throwing any exceptions. | Should Pass |
 */
TEST(player_isobmff_EmsgIsoBmffBox, setPresentationTime_typical_positive) {
    std::cout << "Entering setPresentationTime_typical_positive test" << std::endl;

    // Create an object of EmsgIsoBmffBox using a parameterized constructor.
    // Parameters: box size, timeScale, eventDuration, id.
    EXPECT_NO_THROW({
        player_isobmff::EmsgIsoBmffBox obj(1024, 1, 200, 10);
        std::cout << "Created EmsgIsoBmffBox object with size=1024, timeScale=1, eventDuration=200, id=10" << std::endl;

        // Set presentation time to a typical positive value.
        uint64_t presTime = 1000;
        std::cout << "Invoking setPresentationTime with presTime=" << presTime << std::endl;
        obj.setPresentationTime(presTime);
        std::cout << "After invoking setPresentationTime, expected internal presentationTime is updated to " << presTime << std::endl;
    });

    std::cout << "Exiting setPresentationTime_typical_positive test" << std::endl;
}
/**
 * @brief Verify that setPresentationTime correctly sets a zero value.
 *
 * This test creates an instance of EmsgIsoBmffBox with predefined parameters and then sets the presentation time to zero.
 * It verifies that no exceptions are thrown and that the internal presentation time is updated to zero.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 048
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | ------------- | ----- |
 * | 01 | Construct EmsgIsoBmffBox object and invoke setPresentationTime with zero | box size = 1024, timeScale = 1, eventDuration = 200, id = 10, presTime = 0 | Object is created and presentationTime is set to 0 without exceptions | Should Pass |
 */
TEST(player_isobmff_EmsgIsoBmffBox, setPresentationTime_zero_value) {
    std::cout << "Entering setPresentationTime_zero_value test" << std::endl;

    // Create an object of EmsgIsoBmffBox using a parameterized constructor.
    // Parameters: box size, timeScale, eventDuration, id.
    EXPECT_NO_THROW({
        player_isobmff::EmsgIsoBmffBox obj(1024, 1, 200, 10);
        std::cout << "Created EmsgIsoBmffBox object with size=1024, timeScale=1, eventDuration=200, id=10" << std::endl;

        // Set presentation time to zero.
        uint64_t presTime = 0;
        std::cout << "Invoking setPresentationTime with presTime=" << presTime << std::endl;
        obj.setPresentationTime(presTime);
        std::cout << "After invoking setPresentationTime, expected internal presentationTime is updated to " << presTime << std::endl;
    });

    std::cout << "Exiting setPresentationTime_zero_value test" << std::endl;
}
/**
 * @brief Verify that the setPresentationTimeDelta method sets the presentation time delta correctly when a normal value is provided.
 *
 * This test creates an EmsgIsoBmffBox object with specified parameters and then invokes the setPresentationTimeDelta method with a normal value. It ensures that the API correctly handles the input without throwing any exceptions. This is important to validate proper functionality in the normal usage scenario.
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
 * | 01 | Create EmsgIsoBmffBox object with parameters and invoke setPresentationTimeDelta | size = 1024, timeScale = 50, eventDuration = 200, id = 123, presTimeDelta = 1000 | API executes without throwing an exception | Should Pass |
 */
TEST(player_isobmff_EmsgIsoBmffBox, SetPresentationTimeDelta_Normal) {
    std::cout << "Entering SetPresentationTimeDelta_Normal test" << std::endl;
    
    // Create an object using the custom constructor with sample parameters.
    // Parameters: size = 1024, timeScale = 50, eventDuration = 200, id = 123.
    EXPECT_NO_THROW({
        player_isobmff::EmsgIsoBmffBox obj(1024, 50, 200, 123);
        std::cout << "Created EmsgIsoBmffBox object with parameters: sz=1024, tScale=50, evtDur=200, id=123" << std::endl;
        
        // Invoke setPresentationTimeDelta with normal value 1000.
        uint32_t presTimeDelta = 1000;
        std::cout << "Invoking setPresentationTimeDelta with value: " << presTimeDelta << std::endl;
        EXPECT_NO_THROW(obj.setPresentationTimeDelta(presTimeDelta));
        std::cout << "setPresentationTimeDelta invoked successfully with value: " << presTimeDelta << std::endl;

    });
    
    std::cout << "Exiting SetPresentationTimeDelta_Normal test" << std::endl;
}
/**
 * @brief Test the setPresentationTimeDelta function with the minimum boundary value.
 *
 * This test verifies that the setPresentationTimeDelta method of the EmsgIsoBmffBox class accepts the minimum boundary value (0) without throwing any exceptions. It involves creating an object with valid parameters and then invoking setPresentationTimeDelta with the minimum value.
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
 * | Variation / Step | Description                                                                  | Test Data                                                            | Expected Result                                               | Notes               |
 * | :--------------: | ---------------------------------------------------------------------------- | -------------------------------------------------------------------- | ------------------------------------------------------------- | ------------------- |
 * | 01               | Create an instance of EmsgIsoBmffBox with valid parameters using the custom constructor. | size = 512, timeScale = 25, eventDuration = 100, id = 456             | Object is created without any exceptions.                     | Should be successful|
 * | 02               | Invoke setPresentationTimeDelta with the minimum boundary value (0) on the created object. | presTimeDelta = 0                                                    | setPresentationTimeDelta is executed without throwing exceptions. | Should Pass         |
 */
TEST(player_isobmff_EmsgIsoBmffBox, SetPresentationTimeDelta_Minimum) {
    std::cout << "Entering SetPresentationTimeDelta_Minimum test" << std::endl;
    
    // Create an object using the custom constructor with sample parameters.
    // Parameters: size = 512, timeScale = 25, eventDuration = 100, id = 456.
    EXPECT_NO_THROW({
        player_isobmff::EmsgIsoBmffBox obj(512, 25, 100, 456);
        std::cout << "Created EmsgIsoBmffBox object with parameters: sz=512, tScale=25, evtDur=100, id=456" << std::endl;
        
        // Invoke setPresentationTimeDelta with minimum boundary value 0.
        uint32_t presTimeDelta = 0;
        std::cout << "Invoking setPresentationTimeDelta with value: " << presTimeDelta << std::endl;
        EXPECT_NO_THROW(obj.setPresentationTimeDelta(presTimeDelta));
        std::cout << "setPresentationTimeDelta invoked successfully with value: " << presTimeDelta << std::endl;
    });
    
    std::cout << "Exiting SetPresentationTimeDelta_Minimum test" << std::endl;
}
/**
 * @brief Tests that the setSchemeIdUri method updates the internal schemeIdUri with a valid URL.
 *
 * This test verifies that when a valid URL string is provided to the setSchemeIdUri method of the EmsgIsoBmffBox instance, 
 * the method executes without throwing any exceptions and updates the internal schemeIdUri accordingly. The objective is to ensure
 * that the API handles valid URL inputs properly to prevent potential runtime errors.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create an instance of EmsgIsoBmffBox with valid integer parameters, prepare a valid URL string, copy it to a buffer, and invoke setSchemeIdUri with this buffer. | input1 = 32, input2 = 1000, input3 = 2000, input4 = 1; url = "http://example.com/scheme"; buffer size = 256 | setSchemeIdUri should execute without throwing any exception and update the internal schemeIdUri to the provided URL. | Should Pass |
 */
TEST(player_isobmff_EmsgIsoBmffBox, SetSchemeIdUriWithValidUrl) {
    std::cout << "Entering SetSchemeIdUriWithValidUrl test" << std::endl;
    
    // Create an instance using a custom constructor.
    // Using arbitrary valid values for parameters.
    EXPECT_NO_THROW({
        player_isobmff::EmsgIsoBmffBox box(32, 1000, 2000, 1);
        
        // Prepare the valid URL string.
        const char* url = "http://example.com/scheme";
        char schemeBuffer[256] = {0};
        strncpy(schemeBuffer, url, sizeof(schemeBuffer) - 1);
        std::cout << "Invoking setSchemeIdUri with value: " << schemeBuffer << std::endl;
        
        // Invoke the method and expect no exception.
        EXPECT_NO_THROW(box.setSchemeIdUri(schemeBuffer));
        
        // Debug log for the expected updated state.
        std::cout << "Method setSchemeIdUri completed. Expected internal schemeIdUri updated to: " << schemeBuffer << std::endl;
    });
    
    std::cout << "Exiting SetSchemeIdUriWithValidUrl test" << std::endl;
}
/**
 * @brief Verify that setSchemeIdUri handles an empty string input without throwing exceptions
 *
 * This test verifies that the setSchemeIdUri function in the EmsgIsoBmffBox class accepts an empty string 
 * as input and updates its internal schemeIdUri accordingly without throwing an exception. This is important 
 * to ensure that the API gracefully handles empty string inputs.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 052
 * **Priority:** High
 * 
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 * 
 * **Test Procedure:**
 * | Variation / Step | Description                                                                 | Test Data                                                                                   | Expected Result                                                                          | Notes         |
 * | :--------------: | --------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------------------------- | ------------- |
 * | 01               | Create an instance of EmsgIsoBmffBox with valid parameters and prepare an empty string input. | width = 32, duration_start = 1000, duration_end = 2000, timescale = 2, empty string = ""     | Instance created successfully and input string prepared.                               | Should be successful |
 * | 02               | Invoke setSchemeIdUri with the empty string and verify that it does not throw an exception.             | input: schemeBuffer = ""                                                                    | No exception thrown; method executes successfully and internal schemeIdUri is updated.   | Should Pass   |
 */
TEST(player_isobmff_EmsgIsoBmffBox, SetSchemeIdUriWithEmptyString) {
    std::cout << "Entering SetSchemeIdUriWithEmptyString test" << std::endl;
    
    EXPECT_NO_THROW({
        player_isobmff::EmsgIsoBmffBox box(32, 1000, 2000, 2);
        
        // Prepare an empty string.
        const char* emptyStr = "";
        char schemeBuffer[256] = {0};
        strncpy(schemeBuffer, emptyStr, sizeof(schemeBuffer) - 1);
        std::cout << "Invoking setSchemeIdUri with empty string: \"" << schemeBuffer << "\"" << std::endl;
        
        // Invoke the method and expect no exception.
        EXPECT_NO_THROW(box.setSchemeIdUri(schemeBuffer));
        
        std::cout << "Method setSchemeIdUri completed. Expected internal schemeIdUri updated to an empty string." << std::endl;
    });
    
    std::cout << "Exiting SetSchemeIdUriWithEmptyString test" << std::endl;
}
/**
 * @brief Verify that setSchemeIdUri handles a null pointer without throwing an exception
 *
 * This test ensures that the setSchemeIdUri method of the EmsgIsoBmffBox class processes a null pointer input correctly by not throwing any exceptions. This behavior is crucial to confirm the robustness of the API when faced with null pointer scenarios.
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
 * | Variation / Step | Description                                              | Test Data                                                             | Expected Result                                                              | Notes        |
 * | :--------------: | --------------------------------------------------------- | --------------------------------------------------------------------- | --------------------------------------------------------------------------- | ------------ |
 * | 01               | Instantiate EmsgIsoBmffBox with predefined constructor parameters. | input1 = size 32, input2 = timeScale 1000, input3 = duration 2000, input4 = version 3 | Object is created successfully without exceptions.                        | Should be successful |
 * | 02               | Invoke setSchemeIdUri with a null pointer.                | input: schemeUri = nullptr                                              | Method completes without throwing an exception as verified by EXPECT_NO_THROW. | Should Pass  |
 */
TEST(player_isobmff_EmsgIsoBmffBox, SetSchemeIdUriWithNullPointer) {
    std::cout << "Entering SetSchemeIdUriWithNullPointer test" << std::endl;
    
    EXPECT_NO_THROW({
        player_isobmff::EmsgIsoBmffBox box(32, 1000, 2000, 3);
        
        // Use a null pointer as input.
        char* nullPtr = nullptr;
        std::cout << "Invoking setSchemeIdUri with null pointer." << std::endl;
        
        // Invoke the method and expect no exception.
        EXPECT_NO_THROW(box.setSchemeIdUri(nullPtr));
        
        std::cout << "Method setSchemeIdUri completed." << std::endl;
    });
    
    std::cout << "Exiting SetSchemeIdUriWithNullPointer test" << std::endl;
}
/**
 * @brief Verify that setTimeScale correctly sets a typical valid value for time scale
 *
 * This test verifies that the player_isobmff::EmsgIsoBmffBox object is correctly instantiated with an initial time scale value and that invoking setTimeScale with a new valid time scale value (100) does not throw any exceptions.
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
 * | Variation / Step | Description                                                                                         | Test Data                                          | Expected Result                                                        | Notes           |
 * | :--------------: | --------------------------------------------------------------------------------------------------- | -------------------------------------------------- | ----------------------------------------------------------------------- | --------------- |
 * | 01               | Create player_isobmff::EmsgIsoBmffBox using custom constructor with initial timeScale value 50     | size = 32, initial timeScale = 50, eventDuration = 200, id = 1  | Object is created successfully without throwing any exceptions         | Should be successful |
 * | 02               | Call setTimeScale with a new valid value of 100                                                     | input newTimeScale = 100                           | setTimeScale API completes without throwing an exception; assertion passes | Should Pass     |
 */
TEST(player_isobmff_EmsgIsoBmffBox, SetTypicalValidTimeScaleValue) {
    std::cout << "Entering SetTypicalValidTimeScaleValue test" << std::endl;
    
    // Create an object using the custom constructor
    // Initial values: size = 32, initial timeScale = 50, eventDuration = 200, id = 1
    EXPECT_NO_THROW({
        player_isobmff::EmsgIsoBmffBox box(32, 50, 200, 1);
        std::cout << "Created player_isobmff::EmsgIsoBmffBox object with initial timeScale value 50" << std::endl;
        
        // Invoke setTimeScale with tScale = 100
        uint32_t newTimeScale = 100;
        std::cout << "Invoking setTimeScale with value: " << newTimeScale << std::endl;
        EXPECT_NO_THROW(box.setTimeScale(newTimeScale));
        
    });
    
    std::cout << "Exiting SetTypicalValidTimeScaleValue test" << std::endl;
}
/**
 * @brief Validate that setTimeScale correctly updates internal timeScale to the minimum valid value (0).
 *
 * This test creates an instance of player_isobmff::EmsgIsoBmffBox using initial parameters (size = 32, initialTimeScale = 50, eventDuration = 200, id = 1) and then calls setTimeScale with a value of 0. The objective is to confirm that the internal timeScale is updated to 0 without throwing exceptions.
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
 * | 01 | Construct player_isobmff::EmsgIsoBmffBox object with valid initial parameters | size = 32, initialTimeScale = 50, eventDuration = 200, id = 1 | Object is created without exception | Should be successful |
 * | 02 | Invoke setTimeScale with the minimum valid value (0) and verify internal state update | newTimeScale = 0, expected internal timeScale = 0 | setTimeScale executes without exception and internal timeScale is updated to 0 | Should Pass |
 */
TEST(player_isobmff_EmsgIsoBmffBox, SetMinimumValidTimeScaleValue) {
    std::cout << "Entering SetMinimumValidTimeScaleValue test" << std::endl;
    
    // Create an object using the custom constructor
    // Initial values: size = 32, initial timeScale = 50, eventDuration = 200, id = 1
    EXPECT_NO_THROW({
        player_isobmff::EmsgIsoBmffBox box(32, 50, 200, 1);
        std::cout << "Created player_isobmff::EmsgIsoBmffBox object with initial timeScale value 50" << std::endl;
        
        // Invoke setTimeScale with tScale = 0 (minimum valid value)
        uint32_t newTimeScale = 0;
        std::cout << "Invoking setTimeScale with value: " << newTimeScale << std::endl;
        EXPECT_NO_THROW(box.setTimeScale(newTimeScale));        
        // Verify that internal timeScale is updated correctly to 0
        EXPECT_EQ(updatedTimeScale, newTimeScale) << "Internal timeScale should be updated to " << newTimeScale;
    });
    
    std::cout << "Exiting SetMinimumValidTimeScaleValue test" << std::endl;
}
/**
 * @brief Tests that setValue correctly updates the internal pointer when provided with a valid non-null buffer.
 *
 * This test verifies that the setValue method of the EmsgIsoBmffBox class does not throw an exception and properly updates
 * the internal value pointer when a valid non-null uint8_t buffer is provided. The test ensures that the function handles
 * a typical scenario correctly without causing runtime errors.
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
 * | 01 | Create an instance of EmsgIsoBmffBox with valid parameters and invoke setValue with a valid non-null buffer | Construction: sz=100, tScale=50, evtDur=30, id=1; Buffer: {0x01, 0x02, 0x03} | setValue does not throw and updates the internal value pointer correctly | Should Pass |
 */
TEST(player_isobmff_EmsgIsoBmffBox, SetValueWithValidNonNullPointer)
{
    std::cout << "Entering SetValueWithValidNonNullPointer test" << std::endl;
    // Using the custom constructor: EmsgIsoBmffBox(uint32_t sz, uint32_t tScale, uint32_t evtDur, uint32_t _id)
    EXPECT_NO_THROW({
        player_isobmff::EmsgIsoBmffBox box(100, 50, 30, 1);
        // Prepare a valid uint8_t buffer containing {0x01, 0x02, 0x03}
        uint8_t testBuffer[3];
        testBuffer[0] = 0x01;
        testBuffer[1] = 0x02;
        testBuffer[2] = 0x03;
        std::cout << "Invoking setValue with buffer: {0x" 
                  << std::hex << static_cast<int>(testBuffer[0]) << ", 0x" 
                  << static_cast<int>(testBuffer[1]) << ", 0x" 
                  << static_cast<int>(testBuffer[2]) << std::dec << "}" << std::endl;
        // Invoke the setValue method with valid pointer
        box.setValue(testBuffer);
        std::cout << "Method setValue successfully updated the internal value pointer to the provided buffer." << std::endl;
    });
    std::cout << "Exiting SetValueWithValidNonNullPointer test" << std::endl;
}
/**
 * @brief Verify that setValue() handles nullptr input without throwing exceptions
 *
 * This test creates an instance of EmsgIsoBmffBox using its custom constructor and then calls setValue() with a nullptr. The test ensures that the method can handle a null pointer input gracefully without throwing an exception.
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
 * | Variation / Step | Description                                                                  | Test Data                                          | Expected Result                                                                     | Notes        |
 * | :--------------: | ---------------------------------------------------------------------------- | -------------------------------------------------- | ------------------------------------------------------------------------------------ | ------------ |
 * | 01               | Create an EmsgIsoBmffBox instance using the parameters (sz=200, tScale=60, evtDur=40, id=2) and invoke setValue() with nullptr | input: sz = 200, tScale = 60, evtDur = 40, id = 2, value = nullptr | Method setValue() should complete without throwing any exception (EXPECT_NO_THROW) | Should Pass  |
 */
TEST(player_isobmff_EmsgIsoBmffBox, SetValueWithNullPointer)
{
    std::cout << "Entering SetValueWithNullPointer test" << std::endl;
    // Using the custom constructor: EmsgIsoBmffBox(uint32_t sz, uint32_t tScale, uint32_t evtDur, uint32_t _id)
    EXPECT_NO_THROW({
        player_isobmff::EmsgIsoBmffBox box(200, 60, 40, 2);
        std::cout << "Invoking setValue with nullptr" << std::endl;
        // Call setValue with nullptr as input
        box.setValue(nullptr);
        std::cout << "Method setValue successfully handled nullptr input." << std::endl;
    });
    std::cout << "Exiting SetValueWithNullPointer test" << std::endl;
}
/**
 * @brief Verifies that the player_isobmff::EmsgIsoBmffBox object can be constructed and destructed successfully using the first constructor
 *
 * This test case checks whether creating an object using the first constructor of player_isobmff::EmsgIsoBmffBox with specified parameter values proceeds without any exceptions. It then verifies that the destructor is executed successfully when the object is deleted. This ensures that the object lifecycle management is working as expected.
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
 * | Variation / Step | Description                                                                               | Test Data                                         | Expected Result                                                 | Notes         |
 * | :--------------: | ----------------------------------------------------------------------------------------- | ------------------------------------------------- | --------------------------------------------------------------- | ------------- |
 * | 01               | Invoke player_isobmff::EmsgIsoBmffBox first constructor with predetermined parameters       | sz = 100, tScale = 50, evtDur = 200, id = 1         | Object is created without throwing an exception                 | Should Pass   |
 * | 02               | Log the internal state by printing the object pointer                                       | object pointer = <pointer returned by constructor> | Valid object pointer value is logged                             | Should be successful |
 * | 03               | Delete the object to invoke the destructor                                                  | delete box                                        | Destructor is executed without throwing an exception             | Should Pass   |
 */
TEST(player_isobmff_EmsgIsoBmffBox, PositiveDestructionUsingFirstConstructor) {
    std::cout << "Entering PositiveDestructionUsingFirstConstructor test" << std::endl;
    
    // Values for the first constructor
    uint32_t sz = 100;
    uint32_t tScale = 50;
    uint32_t evtDur = 200;
    uint32_t id = 1;
    
    std::cout << "Invoking player_isobmff::EmsgIsoBmffBox(first constructor) with parameters:" << std::endl;
    std::cout << "  sz = " << sz << ", tScale = " << tScale << ", evtDur = " << evtDur << ", id = " << id << std::endl;
    
    EXPECT_NO_THROW({
        // Create the object using the first constructor
        player_isobmff::EmsgIsoBmffBox* box = new player_isobmff::EmsgIsoBmffBox(sz, tScale, evtDur, id);
        std::cout << "EmsgIsoBmffBox object created successfully using first constructor" << std::endl;
        
        // Log internal state (if any available internal state, but since none accessible, we log object pointer)
        std::cout << "Internal state: Object pointer = " << box << std::endl;
        
        // Delete the object to invoke destructor
        std::cout << "Deleting EmsgIsoBmffBox object to invoke destructor" << std::endl;
        delete box;
        std::cout << "EmsgIsoBmffBox object deleted successfully" << std::endl;
    });
    
    std::cout << "Exiting PositiveDestructionUsingFirstConstructor test" << std::endl;
}
/**
 * @brief Verify that the EmsgIsoBmffBox object is correctly destroyed when using the second constructor with a full IsoBmffBox reference.
 *
 * This test creates a FullIsoBmffBox object with specific parameters, and then uses it to invoke the second constructor of the EmsgIsoBmffBox.
 * The objective is to ensure that the object is constructed and its destructor is safely invoked without exceptions.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 059@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * |01| Prepare input test data and create a FullIsoBmffBox object with specified parameters. | fullBoxSize = 150, btype = "ABCD", version = 1, flags = 0 | FullIsoBmffBox object is created successfully | Should be successful |
 * |02| Invoke the second constructor of EmsgIsoBmffBox using the FullIsoBmffBox reference along with additional parameters. | tScale = 60, evtDur = 210, id = 2, presTime = 1000, presTimeDelta = 30, input: fullBox object reference | EmsgIsoBmffBox object is created without throwing exceptions and returns a valid pointer | Should Pass |
 * |03| Delete the created EmsgIsoBmffBox object to trigger its destructor. | No additional data after construction | Destructor invoked safely without any exceptions | Should be successful |
 */
TEST(player_isobmff_EmsgIsoBmffBox, PositiveDestructionUsingSecondConstructorWithFullIsoBmffBoxReference) {
    std::cout << "Entering PositiveDestructionUsingSecondConstructorWithFullIsoBmffBoxReference test" << std::endl;
    
    // Prepare parameter values for FullIsoBmffBox
    uint32_t fullBoxSize = 150;
    char btype[4];
    // Using strncpy to assign btype with exactly 4 characters.
    strncpy(btype, "ABCD", sizeof(btype));
    uint8_t version = 1;
    uint32_t flags = 0;
    
    std::cout << "Creating FullIsoBmffBox object with parameters:" << std::endl;
    std::cout << "  sz = " << fullBoxSize << ", btype = " << std::string(btype, sizeof(btype))
              << ", version = " << static_cast<uint32_t>(version) << ", flags = " << flags << std::endl;
    
    // Create a FullIsoBmffBox object
    // Assume that FullIsoBmffBox is defined and available in the linking process.
    player_isobmff::FullIsoBmffBox fullBox(fullBoxSize, btype, version, flags);
    std::cout << "FullIsoBmffBox object created successfully" << std::endl;
    
    // Values for the second constructor of EmsgIsoBmffBox
    uint32_t tScale = 60;
    uint32_t evtDur = 210;
    uint32_t id = 2;
    uint64_t presTime = 1000;
    uint32_t presTimeDelta = 30;
    
    std::cout << "Invoking player_isobmff::EmsgIsoBmffBox(second constructor) with parameters:" << std::endl;
    std::cout << "  tScale = " << tScale << ", evtDur = " << evtDur << ", id = " << id
              << ", presTime = " << presTime << ", presTimeDelta = " << presTimeDelta << std::endl;
    
    EXPECT_NO_THROW({
        // Create the object using the second constructor
        player_isobmff::EmsgIsoBmffBox* box = new player_isobmff::EmsgIsoBmffBox(fullBox, tScale, evtDur, id, presTime, presTimeDelta);
        std::cout << "EmsgIsoBmffBox object created successfully using second constructor" << std::endl;
        
        // Log internal state (if any accessible, log the pointer value)
        std::cout << "Internal state: Object pointer = " << box << std::endl;
        
        // Delete the object to invoke destructor
        std::cout << "Deleting EmsgIsoBmffBox object to invoke destructor" << std::endl;
        delete box;
        std::cout << "EmsgIsoBmffBox object deleted successfully" << std::endl;
    });
    
    std::cout << "Exiting PositiveDestructionUsingSecondConstructorWithFullIsoBmffBoxReference test" << std::endl;
}
/**
 * @brief Constructs a FullIsoBmffBox with typical parameters and verifies successful construction.
 *
 * This test validates that the FullIsoBmffBox object is properly constructed when provided with typical parameters:
 * a size of 1024 bytes, a box type of "moov", version set to 1, and flags set to 0x000001. The test ensures that the
 * constructor does not throw any exceptions.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 060
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                             | Test Data                                          | Expected Result                        | Notes       |
 * | :--------------: | ------------------------------------------------------- | -------------------------------------------------- | -------------------------------------- | ----------- |
 * | 01               | Invoke FullIsoBmffBox constructor with typical parameters | sz = 1024, btype = "moov", ver = 1, flags = 0x000001 | Constructor executes without throwing | Should Pass |
 */
TEST(player_isobmff::FullIsoBmffBox, ValidConstructionWithTypicalParameters) {
    std::cout << "Entering ValidConstructionWithTypicalParameters test" << std::endl;

    // Test input values
    uint32_t sz = 1024;
    char btype[4];
    std::cout << "Assigning box type 'moov' using strncpy" << std::endl;
    std::strncpy(btype, "moov", 4);
    uint8_t ver = 1;
    uint32_t flags = 0x000001;

    // Debug log before invocation
    std::cout << "Invoking FullIsoBmffBox constructor with parameters:" << std::endl;
    std::cout << "  sz = " << sz << std::endl;
    std::cout << "  btype = ";
    for (int i = 0; i < 4; i++) {
        std::cout << btype[i];
    }
    std::cout << std::endl;
    std::cout << "  ver = " << static_cast<uint32_t>(ver) << std::endl;
    std::cout << "  flags = " << flags << std::endl;

    // Invoke the constructor and ensure it does not throw
    EXPECT_NO_THROW({
        player_isobmff::FullIsoBmffBox fullBox(sz, btype, ver, flags);
    });

    std::cout << "Exiting ValidConstructionWithTypicalParameters test" << std::endl;
}
/**
 * @brief Test case to verify exception throwing when FullIsoBmffBox is constructed with a btype length less than 4 characters
 *
 * This test case verifies that the FullIsoBmffBox constructor throws an exception when a btype value with fewer than 4 characters is provided.
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
 * | Variation / Step | Description                                                                                  | Test Data                                      | Expected Result                                                      | Notes       |
 * | :--------------: | -------------------------------------------------------------------------------------------- | ---------------------------------------------- | -------------------------------------------------------------------- | ----------- |
 * | 01               | Invoke the FullIsoBmffBox constructor with sz = 512, btype = "moo", ver = 1, flags = 5 and expect an exception due to btype length < 4 characters | sz = 512, btype = moo, ver = 1, flags = 5        | Constructor should throw an exception indicating invalid btype length | Should Fail |
 */
TEST(player_isobmff::FullIsoBmffBox, NegativeTestWithBtypeLengthLessThan4Characters) {
    std::cout << "Entering NegativeTestWithBtypeLengthLessThan4Characters test" << std::endl;

    // Test input values
    uint32_t sz = 512;
    char btype[4];
    std::cout << "Assigning box type 'moo' using strncpy" << std::endl;
    std::strncpy(btype, "moo", 4); // "moo" is less than 4 characters when considering expected criteria
    uint8_t ver = 1;
    uint32_t flags = 5;

    // Debug log before invocation
    std::cout << "Invoking FullIsoBmffBox constructor with parameters:" << std::endl;
    std::cout << "  sz = " << sz << std::endl;
    std::cout << "  btype = ";
    for (int i = 0; i < 4; i++) {
        std::cout << btype[i];
    }
    std::cout << std::endl;
    std::cout << "  ver = " << static_cast<uint32_t>(ver) << std::endl;
    std::cout << "  flags = " << flags << std::endl;
    
    // Expect the constructor to throw due to invalid btype length
    EXPECT_ANY_THROW({
        player_isobmff::FullIsoBmffBox fullBox(sz, btype, ver, flags);
    });

    std::cout << "Exiting NegativeTestWithBtypeLengthLessThan4Characters test" << std::endl;
}
/**
 * @brief Validate the construction of a FullIsoBmffBox object with a size of 100 and btype "ftyp"
 *
 * This test verifies that the player_isobmff::FullIsoBmffBox constructor correctly initializes an object with a specified size (100)
 * and a box type "ftyp", ensuring no exceptions are thrown during object creation.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 062@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                         | Test Data                                  | Expected Result                                               | Notes        |
 * | :--------------: | ----------------------------------------------------------------------------------- | ------------------------------------------ | ------------------------------------------------------------- | ------------ |
 * | 01               | Invoke player_isobmff::FullIsoBmffBox constructor with size=100 and btype="ftyp"    | sz=100, btype=ftyp                         | No exception thrown and object successfully created          | Should Pass  |
 */
TEST(player_isobmff::FullIsoBmffBox, ConstructWithSize100AndFtyp) {
    std::cout << "Entering ConstructWithSize100AndFtyp test" << std::endl;
    
    uint32_t sz = 100;
    char btype[4];
    // Assign "ftyp" to btype using strncpy
    strncpy(btype, "ftyp", sizeof(btype));
    std::cout << "Preparing to invoke player_isobmff::FullIsoBmffBox constructor with sz = " 
              << sz << " and btype = " << std::string(btype, sizeof(btype)) << std::endl;
    
    // Invoke the constructor and check no exception is thrown
    EXPECT_NO_THROW({
        player_isobmff::FullIsoBmffBox box(sz, btype);
        std::cout << "player_isobmff::FullIsoBmffBox object created with sz = " 
                  << sz << " and btype = " << std::string(btype, sizeof(btype)) << std::endl;
    });
    
    std::cout << "Exiting ConstructWithSize100AndFtyp test" << std::endl;
}
/**
 * @brief Verify construction of FullIsoBmffBox with maximum size and box type "mdat"
 *
 * This test verifies that constructing player_isobmff::FullIsoBmffBox using the maximum allowed size (4294967295) and the box type "mdat" does not throw any exceptions. This ensures that the constructor appropriately handles boundary conditions and valid box type values.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 063@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Prepare test data with maximum size and valid box type "mdat". | input: sz = 4294967295, btype = "mdat" | Test data prepared successfully. | Should be successful |
 * | 02 | Invoke the player_isobmff::FullIsoBmffBox constructor with the prepared test data. | input: sz = 4294967295, btype = "mdat" | Constructor does not throw an exception. | Should Pass |
 */
TEST(player_isobmff::FullIsoBmffBox, ConstructWithMaxSizeAndMdat) {
    std::cout << "Entering ConstructWithMaxSizeAndMdat test" << std::endl;
    
    uint32_t sz = 4294967295;
    char btype[4];
    // Assign "mdat" to btype using strncpy
    strncpy(btype, "mdat", sizeof(btype));
    std::cout << "Preparing to invoke player_isobmff::FullIsoBmffBox constructor with sz = " 
              << sz << " and btype = " << std::string(btype, sizeof(btype)) << std::endl;
    
    // Invoke the constructor and check no exception is thrown
    EXPECT_NO_THROW({
        player_isobmff::FullIsoBmffBox box(sz, btype);
        std::cout << "player_isobmff::FullIsoBmffBox object created with sz = " 
                  << sz << " and btype = " << std::string(btype, sizeof(btype)) << std::endl;
    });
    
    std::cout << "Exiting ConstructWithMaxSizeAndMdat test" << std::endl;
}
/**
 * @brief Test the successful creation of a GenericContainerIsoBmffBox when valid parameters are provided
 *
 * This test validates that providing a valid size (1024) and a valid box type ("ftyp") to the GenericContainerIsoBmffBox constructor
 * results in the object being created successfully without throwing any exceptions. This ensures that the constructor properly
 * sets the internal state based on the supplied parameters.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Initialize the test parameters and invoke the GenericContainerIsoBmffBox constructor | input: sz = 1024, input: btype = ftyp | GenericContainerIsoBmffBox is instantiated without throwing an exception (EXPECT_NO_THROW passes) | Should Pass |
 */
TEST(player_isobmff::GenericContainerIsoBmffBox, ValidBoxCreation_ftyp) {
    std::cout << "Entering ValidBoxCreation_ftyp test" << std::endl;
    
    uint32_t sz = 1024;
    char btype[4];
    std::memset(btype, 0, sizeof(btype));
    std::strncpy(btype, "ftyp", 4);
    std::cout << "Invoking GenericContainerIsoBmffBox constructor with sz = " << sz 
              << " and btype = " << std::string(btype, 4) << std::endl;
    
    EXPECT_NO_THROW({
        player_isobmff::GenericContainerIsoBmffBox box(sz, btype);
        std::cout << "Constructor invoked successfully. Internal state set to: sz = " 
                  << sz << ", btype = " << std::string(btype, 4) << std::endl;
    });
    
    std::cout << "Exiting ValidBoxCreation_ftyp test" << std::endl;
}
/**
 * @brief Validates that GenericContainerIsoBmffBox constructor creates a valid 'moov' box without throwing exceptions.
 *
 * This test verifies that invoking the GenericContainerIsoBmffBox constructor with a valid size (sz = 1) and a box type ("moov") does not throw any exceptions and correctly sets the internal state of the object.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Set up test input by initializing sz and btype with a valid "moov" string | sz = 1, btype = "moov" | Inputs are correctly initialized | Should be successful |
 * | 02 | Invoke the GenericContainerIsoBmffBox constructor and verify no exception is thrown; check internal state is set as expected | sz = 1, btype = "moov", output: instance with sz = 1 and btype = "moov" | Constructor executes without throwing; internal state is configured correctly | Should Pass |
 */
TEST(player_isobmff::GenericContainerIsoBmffBox, ValidBoxCreation_moov) {
    std::cout << "Entering ValidBoxCreation_moov test" << std::endl;
    
    uint32_t sz = 1;
    char btype[4];
    std::memset(btype, 0, sizeof(btype));
    std::strncpy(btype, "moov", 4);
    std::cout << "Invoking GenericContainerIsoBmffBox constructor with sz = " << sz 
              << " and btype = " << std::string(btype, 4) << std::endl;
    
    EXPECT_NO_THROW({
        player_isobmff::GenericContainerIsoBmffBox box(sz, btype);
        std::cout << "Constructor invoked successfully. Internal state set to: sz = " 
                  << sz << ", btype = " << std::string(btype, 4) << std::endl;
    });
    
    std::cout << "Exiting ValidBoxCreation_moov test" << std::endl;
}
/**
 * @brief Verify that generic container creation fails with a zero size box.
 *
 * This test checks that the GenericContainerIsoBmffBox constructor throws an exception when provided with a zero size. It ensures that the API properly rejects invalid box configurations to prevent further processing of incorrect parameters.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 066
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoke the GenericContainerIsoBmffBox constructor with sz = 0 and btype = "mdat" | sz = 0, btype = "mdat" | Constructor should throw an exception indicating invalid input | Should Pass |
 */
TEST(player_isobmff::GenericContainerIsoBmffBox, InvalidBoxCreation_ZeroSize) {
    std::cout << "Entering InvalidBoxCreation_ZeroSize test" << std::endl;
    
    uint32_t sz = 0;
    char btype[4];
    std::memset(btype, 0, sizeof(btype));
    std::strncpy(btype, "mdat", 4);
    std::cout << "Invoking GenericContainerIsoBmffBox constructor with sz = " << sz 
              << " and btype = " << std::string(btype, 4) << std::endl;
    
    EXPECT_THROW({
        player_isobmff::GenericContainerIsoBmffBox box(sz, btype);
        std::cout << "ERROR: Constructor did not throw an exception for zero size." << std::endl;
    }, std::exception);
    
    std::cout << "Exiting InvalidBoxCreation_ZeroSize test" << std::endl;
}
/**
 * @brief Validate that GenericContainerIsoBmffBox throws an exception when initialized with a non-alphabetic btype
 *
 * This test verifies that the GenericContainerIsoBmffBox constructor correctly handles the error condition when provided with a non-alphabetic btype by throwing a std::exception. The test ensures robustness of input validation in the constructor.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 067@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoke the GenericContainerIsoBmffBox constructor with valid size and non-alphabetic btype | input: sz = 512, btype = !!@# | Exception (std::exception) is thrown; Assertion EXPECT_THROW passes | Should Pass |
 */
TEST(player_isobmff::GenericContainerIsoBmffBox, InvalidBoxCreation_NonAlphabeticBtype) {
    std::cout << "Entering InvalidBoxCreation_NonAlphabeticBtype test" << std::endl;
    
    uint32_t sz = 512;
    char btype[4];
    std::memset(btype, 0, sizeof(btype));
    std::strncpy(btype, "!!@#", 4);
    std::cout << "Invoking GenericContainerIsoBmffBox constructor with sz = " << sz 
              << " and btype = " << std::string(btype, 4) << std::endl;
    
    EXPECT_THROW({
        player_isobmff::GenericContainerIsoBmffBox box(sz, btype);
        std::cout << "ERROR: Constructor did not throw an exception for non-alphabetic btype." << std::endl;
    }, std::exception);
    
    std::cout << "Exiting InvalidBoxCreation_NonAlphabeticBtype test" << std::endl;
}
/**
 * @brief Verify that a valid child box is added successfully to an empty container
 *
 * This test creates a GenericContainerIsoBmffBox with a specified container size and type, then creates a valid IsoBmffBox with a specified child size and type, and finally calls the addChildren function to add the child box to the container. It confirms that no exceptions are thrown during the creation and addition process.
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
 * | Variation / Step | Description                                                         | Test Data                                                     | Expected Result                                          | Notes       |
 * | :--------------: | ------------------------------------------------------------------- | ------------------------------------------------------------- | -------------------------------------------------------- | ----------- |
 * | 01               | Create a GenericContainerIsoBmffBox with specified container size and type. | containerSize = 200, containerType = "ctnr"                   | Object is created without throwing an exception.       | Should Pass |
 * | 02               | Create a valid IsoBmffBox with specified child size and type.         | childSize = 100, childType = "moov"                             | Object is created without throwing an exception.       | Should Pass |
 * | 03               | Invoke addChildren to add the IsoBmffBox pointer to the container.      | child pointer = IsoBmffBox(childSize, childType)                | addChildren is invoked without throwing an exception.  | Should Pass |
 */
TEST(player_isobmff::GenericContainerIsoBmffBox, addValidChildBoxToEmptyContainer) {
    std::cout << "Entering addValidChildBoxToEmptyContainer test" << std::endl;
    
    // Create a GenericContainerIsoBmffBox object with custom parameters
    uint32_t containerSize = 200;
    char containerType[5] = "ctnr";
    std::cout << "Creating GenericContainerIsoBmffBox with size: " << containerSize 
              << " and type: " << containerType << std::endl;
    EXPECT_NO_THROW({
        player_isobmff::GenericContainerIsoBmffBox container(containerSize, containerType);
        
        // Create a valid IsoBmffBox pointer with size 100 and type "moov"
        uint32_t childSize = 100;
        char childType[5] = "moov";
        std::cout << "Creating IsoBmffBox with size: " << childSize 
                  << " and type: " << childType << std::endl;
        IsoBmffBox *childBox = new IsoBmffBox(childSize, childType);
        std::cout << "Invoking addChildren with IsoBmffBox pointer (child size: " << childSize 
                  << ", child type: " << childType << ")" << std::endl;
        
        EXPECT_NO_THROW({
            container.addChildren(childBox);
            std::cout << "addChildren invoked successfully." << std::endl;
        });
        
    });
    
    std::cout << "Exiting addValidChildBoxToEmptyContainer test" << std::endl;
}
/**
 * @brief Verify that calling addChildren with a null child pointer does not throw an exception
 *
 * This test verifies that the GenericContainerIsoBmffBox class method addChildren correctly handles a null IsoBmffBox pointer without throwing an exception. This ensures that the API is robust and can gracefully ignore null inputs without causing runtime errors.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 069@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data |Expected Result |Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create GenericContainerIsoBmffBox object with valid size and type | containerSize = 200, containerType = "ctnr" | Container object is created successfully without throwing an exception | Should be successful |
 * | 02 | Invoke addChildren with a null IsoBmffBox pointer | child pointer = nullptr | addChildren handles the null pointer gracefully, without throwing an exception | Should Pass |
 */
TEST(player_isobmff::GenericContainerIsoBmffBox, addNullChildBoxPointer) {
    std::cout << "Entering addNullChildBoxPointer test" << std::endl;
    
    // Create a GenericContainerIsoBmffBox object with custom parameters
    uint32_t containerSize = 200;
    char containerType[5] = "ctnr";
    std::cout << "Creating GenericContainerIsoBmffBox with size: " << containerSize 
              << " and type: " << containerType << std::endl;
    EXPECT_NO_THROW({
        player_isobmff::GenericContainerIsoBmffBox container(containerSize, containerType);
        
        // Prepare a null IsoBmffBox pointer
        IsoBmffBox *nullChildBox = nullptr;
        std::cout << "Invoking addChildren with null IsoBmffBox pointer" << std::endl;
        
        EXPECT_NO_THROW({
            container.addChildren(nullChildBox);
            std::cout << "addChildren handled null pointer gracefully." << std::endl;
        });
        
    });
    
    std::cout << "Exiting addNullChildBoxPointer test" << std::endl;
}
/**
 * @brief Verify that constructing a GenericContainerIsoBmffBox with a default newTrackId (-1) successfully creates a valid container.
 *
 * This test allocates a buffer, sets up the required box type ("moov"), and invokes the constructContainer API with a default newTrackId value (-1). It then verifies that no exception is thrown and the container object returned is not null, indicating a successful construction.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 070@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                                                | Test Data                                                   | Expected Result                                                        | Notes       |
 * | :--------------: | ------------------------------------------------------------------------------------------ | ----------------------------------------------------------- | ---------------------------------------------------------------------- | ----------- |
 * | 01               | Invoke constructContainer with input parameters: size=100, btype="moov", ptr, newTrackId=-1   | input1 = sz(100), input2 = btype("moov"), input3 = ptr, input4 = newTrackId(-1), output1 = containerBox | No exception thrown; containerBox is not nullptr                        | Should Pass |
 */
TEST(player_isobmff::GenericContainerIsoBmffBox, ValidContainerConstructionWithDefaultNewTrackId) {
    std::cout << "Entering ValidContainerConstructionWithDefaultNewTrackId test" << std::endl;

    uint32_t sz = 100;
    char btype[5] = {0};
    // Use strncpy to initialize btype with "moov"
    strncpy(btype, "moov", 4);
    uint8_t* ptr = new uint8_t[sz];
    std::cout << "Allocated buffer at address: " << static_cast<const void*>(ptr) << " with size: " << sz << std::endl;
    std::cout << "Invoking constructContainer with sz=" << sz 
              << ", btype=" << btype 
              << ", ptr address=" << static_cast<const void*>(ptr) 
              << ", newTrackId=-1" << std::endl;
    
    player_isobmff::GenericContainerIsoBmffBox* containerBox = nullptr;
    EXPECT_NO_THROW({
        containerBox = player_isobmff::GenericContainerIsoBmffBox::constructContainer(sz, btype, ptr, -1);
    });
    EXPECT_NE(containerBox, nullptr);
    
    // Free allocated memory used for buffer
    delete[] ptr;
    std::cout << "Exiting ValidContainerConstructionWithDefaultNewTrackId test" << std::endl;
}
/**
 * @brief Validates the construction of a GenericContainerIsoBmffBox container with valid input parameters and a new track ID override.
 *
 * This test verifies that the GenericContainerIsoBmffBox::constructContainer function correctly constructs a container when provided with a valid buffer size, a valid box type ("trak"), a dynamically allocated buffer, and a new track ID. The test checks for no exceptions thrown during construction and ensures that a non-null container pointer is returned.
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
 * | Variation / Step | Description                                                                          | Test Data                                                               | Expected Result                                             | Notes           |
 * | :--------------: | ------------------------------------------------------------------------------------ | ----------------------------------------------------------------------- | ----------------------------------------------------------- | --------------- |
 * | 01               | Initialize buffer size, box type, allocate memory for buffer, and set new track ID.    | sz = 200, btype = trak, ptr = allocated memory address, newTrackId = 123   | Memory allocated and parameters initialized successfully    | Should be successful |
 * | 02               | Invoke the constructContainer API with the prepared parameters.                      | sz = 200, btype = trak, ptr = allocated memory address, newTrackId = 123   | API should not throw and a valid container pointer is returned | Should Pass     |
 * | 03               | Check that the returned container pointer from constructContainer is not null.         | containerBox expected != nullptr                                        | containerBox is not nullptr                                 | Should Pass     |
 * | 04               | Clean up by deallocating the memory allocated for the buffer.                          | ptr memory deallocated                                                 | Memory successfully freed                                   | Should be successful |
 */
TEST(player_isobmff::GenericContainerIsoBmffBox, ValidContainerConstructionWithNewTrackIdOverride) {
    std::cout << "Entering ValidContainerConstructionWithNewTrackIdOverride test" << std::endl;

    uint32_t sz = 200;
    char btype[5] = {0};
    // Use strncpy to initialize btype with "trak"
    strncpy(btype, "trak", 4);
    uint8_t* ptr = new uint8_t[sz];
    std::cout << "Allocated buffer at address: " << static_cast<const void*>(ptr) << " with size: " << sz << std::endl;
    int newTrackId = 123;
    std::cout << "Invoking constructContainer with sz=" << sz 
              << ", btype=" << btype 
              << ", ptr address=" << static_cast<const void*>(ptr) 
              << ", newTrackId=" << newTrackId << std::endl;
    
    player_isobmff::GenericContainerIsoBmffBox* containerBox = nullptr;
    EXPECT_NO_THROW({
        containerBox = player_isobmff::GenericContainerIsoBmffBox::constructContainer(sz, btype, ptr, newTrackId);
    });
    EXPECT_NE(containerBox, nullptr);    
    // Free allocated memory used for buffer
    delete[] ptr;
    std::cout << "Exiting ValidContainerConstructionWithNewTrackIdOverride test" << std::endl;
}
/**
 * @brief Verify that constructing a GenericContainerIsoBmffBox with a zero size returns nullptr.
 *
 * This test verifies that when attempting to construct a GenericContainerIsoBmffBox using a zero size (sz = 0),
 * the API does not throw an exception but returns a nullptr, ensuring that invalid input is handled gracefully.
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
 * |Variation / Step|Description|Test Data|Expected Result|Notes|
 * |:----:|---------|----------|--------------|-----|
 * |01|Initialize test parameters and allocate a buffer for the box.|sz = 0, btype = "moov", ptr = allocated non-null pointer, newTrackId = -1|Parameters set and buffer allocated successfully.|Should be successful|
 * |02|Invoke the constructContainer API with a zero size to attempt constructing the box.|sz = 0, btype = "moov", ptr = allocated non-null pointer, newTrackId = -1|API returns a nullptr without throwing an exception; assertion passes if containerBox is nullptr.|Should Pass|
 * |03|Clean up the allocated buffer after the test.|ptr freed|Memory is freed without errors.|Should be successful|
 */
TEST(player_isobmff::GenericContainerIsoBmffBox, InvalidConstructionZeroSize) {
    std::cout << "Entering InvalidConstructionZeroSize test" << std::endl;

    uint32_t sz = 0;
    char btype[5] = {0};
    // Use strncpy to initialize btype with "moov"
    strncpy(btype, "moov", 4);
    uint8_t* ptr = new uint8_t[100];  // allocate buffer even though size is zero for the box
    std::cout << "Allocated buffer at address: " << static_cast<const void*>(ptr) << " for testing zero size." << std::endl;
    std::cout << "Invoking constructContainer with sz=" << sz 
              << ", btype=" << btype 
              << ", ptr address=" << static_cast<const void*>(ptr) 
              << ", newTrackId=-1" << std::endl;
    
    player_isobmff::GenericContainerIsoBmffBox* containerBox = nullptr;
    EXPECT_NO_THROW({
        containerBox = player_isobmff::GenericContainerIsoBmffBox::constructContainer(sz, btype, ptr, -1);
    });
    EXPECT_EQ(containerBox, nullptr);
    delete[] ptr;
    std::cout << "Exiting InvalidConstructionZeroSize test" << std::endl;
}
/**
 * @brief Verify that constructContainer handles null box data pointer correctly.
 *
 * This test verifies that when a null pointer is passed for the box data in the constructContainer method,
 * the function does not throw an exception and returns a nullptr. This ensures proper error handling for invalid box data.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 073@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |@n
 * | :----: | --------- | ---------- |-------------- | ----- |@n
 * | 01 | Invoke constructContainer with invalid null box data pointer | sz = 150, btype = trak, ptr = nullptr, newTrackId = -1 | No exception thrown and returned pointer is nullptr | Should Pass |
 */
TEST(player_isobmff::GenericContainerIsoBmffBox, InvalidConstructionNullBoxDataPointer) {
    std::cout << "Entering InvalidConstructionNullBoxDataPointer test" << std::endl;

    uint32_t sz = 150;
    char btype[5] = {0};
    // Use strncpy to initialize btype with "trak"
    strncpy(btype, "trak", 4);
    uint8_t* ptr = nullptr;
    std::cout << "Invoking constructContainer with sz=" << sz 
              << ", btype=" << btype 
              << ", ptr as nullptr" 
              << ", newTrackId=-1" << std::endl;
    
    player_isobmff::GenericContainerIsoBmffBox* containerBox = nullptr;
    EXPECT_NO_THROW({
        containerBox = player_isobmff::GenericContainerIsoBmffBox::constructContainer(sz, btype, ptr, -1);
    });
    EXPECT_EQ(containerBox, nullptr);
    
    std::cout << "Exiting InvalidConstructionNullBoxDataPointer test" << std::endl;
}
/**
 * @brief Validate construction of GenericContainerIsoBmffBox with null btype pointer.
 *
 * Validate that constructing a GenericContainerIsoBmffBox with a null pointer for the btype parameter correctly handles this edge case by not creating a container instance, ensuring robust error handling.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 074
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Allocate memory buffer and setup input parameters (sz and btype pointer) | input: sz = 120, btypePtr = nullptr | Memory allocated successfully; btype pointer is null | Should be successful |
 * | 02 | Invoke constructContainer API with provided sz, null btype pointer, allocated buffer, and newTrackId = -1 | input: sz = 120, btypePtr = nullptr, ptr = allocated buffer, newTrackId = -1 | constructContainer returns nullptr and does not throw an exception | Should Pass |
 * | 03 | Verify that the container pointer is null and free the allocated memory buffer | output: containerBox = nullptr | containerBox is confirmed to be nullptr; memory is freed | Should be successful |
 */
TEST(player_isobmff::GenericContainerIsoBmffBox, InvalidConstructionNullBtypePointer) {
    std::cout << "Entering InvalidConstructionNullBtypePointer test" << std::endl;

    uint32_t sz = 120;
    const char* btypePtr = nullptr;  // btype is nullptr
    uint8_t* ptr = new uint8_t[sz];
    std::cout << "Allocated buffer at address: " << static_cast<const void*>(ptr) << " with size: " << sz << std::endl;
    std::cout << "Invoking constructContainer with sz=" << sz 
              << ", btype as nullptr" 
              << ", ptr address=" << static_cast<const void*>(ptr) 
              << ", newTrackId=-1" << std::endl;
    
    player_isobmff::GenericContainerIsoBmffBox* containerBox = nullptr;
    EXPECT_NO_THROW({
        containerBox = player_isobmff::GenericContainerIsoBmffBox::constructContainer(sz, btypePtr, ptr, -1);
    });
    EXPECT_EQ(containerBox, nullptr);
    delete[] ptr;
    std::cout << "Exiting InvalidConstructionNullBtypePointer test" << std::endl;
}
/**
 * @brief Verify getChildren returns a valid pointer when no children are added
 *
 * This test ensures that when a GenericContainerIsoBmffBox is instantiated with no children, invoking the getChildren method returns a valid pointer and does not throw any exception. This confirms that the container's basic functionality behaves correctly under normal conditions.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 075@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**
 * | Variation / Step | Description                                                        | Test Data                             | Expected Result                                                                                          | Notes             |
 * | :---------------: | ------------------------------------------------------------------ | ------------------------------------- | -------------------------------------------------------------------------------------------------------- | ----------------- |
 * | 01               | Create a GenericContainerIsoBmffBox object using the custom constructor | size = 100, type = "test"               | Object should be constructed successfully without any exception                                          | Should be successful |
 * | 02               | Invoke the getChildren method on the constructed object              | object from step 01                   | Should return a pointer to an empty children list and should not throw any exception; API returns valid pointer | Should Pass       |
 */
TEST(player_isobmff::GenericContainerIsoBmffBox, RetrieveChildrenEmpty) {
    std::cout << "Entering RetrieveChildrenEmpty test" << std::endl;
    EXPECT_NO_THROW({
        // Creating GenericContainerIsoBmffBox object using custom constructor
        player_isobmff::GenericContainerIsoBmffBox box(100, "test");
        std::cout << "Constructed GenericContainerIsoBmffBox with size 100 and type \"test\"" << std::endl;
        
        // Invocation of getChildren method
        const std::vector<player_isobmff::IsoBmffBox*> *children = box.getChildren();
        std::cout << "Invoked getChildren(), returned pointer: " << children << std::endl;
    
    });
    std::cout << "Exiting RetrieveChildrenEmpty test" << std::endl;
}
/**
 * @brief Verify that hasChildren returns false when no children are present.
 *
 * This test creates a GenericContainerIsoBmffBox object using a specific size and btype value.
 * It then invokes the hasChildren() method to confirm that it returns false when the object has no children.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 076
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                     | Test Data                                  | Expected Result                                            | Notes       |
 * | :--------------: | --------------------------------------------------------------- | ------------------------------------------ | ---------------------------------------------------------- | ----------- |
 * | 01               | Construct GenericContainerIsoBmffBox with provided size and btype | size = 100, btype = "tst1"                   | Constructor executes without throwing                       | Should Pass |
 * | 02               | Invoke hasChildren() method on the box with no children added     | Method: hasChildren(), output: has_child = false | hasChildren() returns false                                  | Should Pass |
 */
TEST(player_isobmff::GenericContainerIsoBmffBox, hasChildren_NoChildrenReturnsFalse) {
    std::cout << "Entering hasChildren_NoChildrenReturnsFalse test" << std::endl;

    // Create a btype array and initialize with 4 characters using strncpy
    char btype[4];
    std::memset(btype, 0, sizeof(btype));
    std::strncpy(btype, "tst1", 4); // assign 4 characters

    // Construct the GenericContainerIsoBmffBox object and log the constructor invocation
    uint32_t size = 100;
    std::cout << "Invoking GenericContainerIsoBmffBox constructor with size: " << size 
              << " and btype: " << std::string(btype, 4) << std::endl;
    EXPECT_NO_THROW({
        player_isobmff::GenericContainerIsoBmffBox box(size, btype);

        // Invoke hasChildren() method and log the invocation and returned value
        std::cout << "Invoking hasChildren() method on box with no children" << std::endl;
        bool has_child = box.hasChildren();
        std::cout << "hasChildren() returned: " << (has_child ? "true" : "false") << std::endl;

    });

    std::cout << "Exiting hasChildren_NoChildrenReturnsFalse test" << std::endl;
}
/**
 * @brief Test that GenericContainerIsoBmffBox destructor does not throw an exception when no children are present.
 *
 * This test verifies that a GenericContainerIsoBmffBox object is created with a size of 100 and a box type of "abcd" and is destroyed without throwing any exceptions. The test ensures that the virtual destructor behaves correctly when the object contains no children.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 077@n
 * **Priority:** High@n
 * @n
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * @n
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                  | Test Data                                     | Expected Result                             | Notes              |
 * | :--------------: | ------------------------------------------------------------ | --------------------------------------------- | ------------------------------------------- | ------------------ |
 * |       01         | Invoke constructor to create GenericContainerIsoBmffBox.     | input: size=100, boxType="abcd"               | Object is constructed without exception.  | Should Pass        |
 * |       02         | Allow the object to go out of scope, triggering the destructor. | output: object destruction, no exception thrown | Object is destroyed without exception. | Should be successful |
 */
TEST(player_isobmff::GenericContainerIsoBmffBox, DestructorNoChildren) {
    std::cout << "Entering DestructorNoChildren test" << std::endl;
    
    char boxType[4];
    // Using strncpy to assign value to fixed size array
    strncpy(boxType, "abcd", sizeof(boxType));
    
    // Create and destroy the object inside a block
    EXPECT_NO_THROW({
        player_isobmff::GenericContainerIsoBmffBox box(100, boxType);
        std::cout << "Invoked constructor: Created GenericContainerIsoBmffBox with size = 100 and type = " 
                  << std::string(boxType, sizeof(boxType)) << std::endl;
    });
    
    std::cout << "GenericContainerIsoBmffBox object with no children destroyed without exception" << std::endl;
    std::cout << "Exiting DestructorNoChildren test" << std::endl;
}
/**
 * @brief Verify that the virtual destructor of GenericContainerIsoBmffBox is correctly invoked via a base class pointer.
 *
 * This test creates an instance of GenericContainerIsoBmffBox with a specific size and type, casts it to the base class pointer (IsoBmffBox*), and then deletes it via this pointer to ensure that the virtual destructor of the derived class is invoked without throwing any exceptions.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 078
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                                                               | Test Data                                 | Expected Result                                                                                                         | Notes        |
 * | :--------------: | ----------------------------------------------------------------------------------------------------------------------------------------- | ----------------------------------------- | ----------------------------------------------------------------------------------------------------------------------- | ------------ |
 * | 01               | Create an instance of GenericContainerIsoBmffBox with size 150 and type "efgh", cast the object to a IsoBmffBox pointer, then delete it using the base pointer. | size = 150, boxType = efgh                | No exception thrown and the virtual destructor of GenericContainerIsoBmffBox is successfully invoked when deleted via base pointer. | Should Pass  |
 */
TEST(player_isobmff::GenericContainerIsoBmffBox, VirtualDestructorViaBasePointer) {
    std::cout << "Entering VirtualDestructorViaBasePointer test" << std::endl;
    
    char boxType[4];
    // Using strncpy to assign value to fixed size array
    strncpy(boxType, "efgh", sizeof(boxType));
    
    EXPECT_NO_THROW({
        // Dynamically allocate the object using the derived class constructor
        player_isobmff::GenericContainerIsoBmffBox* derivedBox = new player_isobmff::GenericContainerIsoBmffBox(150, boxType);
        std::cout << "Invoked constructor: Created GenericContainerIsoBmffBox with size = 150 and type = "
                  << std::string(boxType, sizeof(boxType)) << std::endl;
        
        // Obtain a pointer to the base class (IsoBmffBox) to test virtual destructor behavior.
        IsoBmffBox* basePtr = static_cast<IsoBmffBox*>(derivedBox);
        std::cout << "Casted derived object to base class pointer (IsoBmffBox*)" << std::endl;
        
        // Delete via the base class pointer and observe the destructor behavior.
        EXPECT_NO_THROW({
            delete basePtr;
            std::cout << "Invoked delete on IsoBmffBox pointer; virtual destructor of GenericContainerIsoBmffBox called successfully" << std::endl;
        });
    });
    
    std::cout << "Exiting VirtualDestructorViaBasePointer test" << std::endl;
}
/**
 * @brief Verify that the IsoBmffBox constructor creates a valid object with a valid 'ftyp' box type.
 *
 * This test validates that invoking the player_isobmff::IsoBmffBox constructor with a valid size (sz = 100)
 * and a valid box type ("ftyp") does not throw any exceptions. The test ensures that the object is initialized
 * correctly with the given parameters.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 079@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                          | Test Data                         | Expected Result                                                 | Notes       |
 * | :--------------: | ---------------------------------------------------------------------------------------------------- | --------------------------------- | --------------------------------------------------------------- | ----------- |
 * | 01               | Invoke player_isobmff::IsoBmffBox constructor with valid inputs (sz = 100 and btype = "ftyp").         | sz = 100, btype = "ftyp"            | Object is successfully created without throwing an exception.  | Should Pass |
 */
TEST(player_isobmff_IsoBmffBox, CreateWithValidFtyp) {
    std::cout << "Entering CreateWithValidFtyp test" << std::endl;
    
    uint32_t sz = 100;
    char btype[4];
    std::cout << "Assigning btype value 'ftyp' using strncpy" << std::endl;
    strncpy(btype, "ftyp", 4);
    std::cout << "Invoking player_isobmff::IsoBmffBox constructor with sz = " << sz 
              << " and btype = " << std::string(btype, 4) << std::endl;
    
    EXPECT_NO_THROW({
        player_isobmff::IsoBmffBox box(sz, btype);
        std::cout << "player_isobmff::IsoBmffBox object created with sz: " << sz 
                  << " and btype: " << std::string(btype, 4) << std::endl;
    });
    
    std::cout << "Exiting CreateWithValidFtyp test" << std::endl;
}
/**
 * @brief Verify that constructing an IsoBmffBox with zero size and 'moov' type does not throw an exception.
 *
 * This test checks that providing a box size of zero and a valid 'moov' type to the IsoBmffBox constructor gracefully handles the condition 
 * without throwing an exception, ensuring correct behavior when a zero-size box is created.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 080@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- | -------------- | ----- |
 * | 01 | Invoke the IsoBmffBox constructor with zero size and 'moov' btype | input1 = sz, value = 0; input2 = btype, value = "moov" | No exception thrown; object created successfully | Should Pass |
 */
TEST(player_isobmff_IsoBmffBox, CreateWithZeroSizeMoov) {
    std::cout << "Entering CreateWithZeroSizeMoov test" << std::endl;
    
    uint32_t sz = 0;
    char btype[4];
    std::cout << "Assigning btype value 'moov' using strncpy" << std::endl;
    strncpy(btype, "moov", 4);
    std::cout << "Invoking player_isobmff::IsoBmffBox constructor with sz = " << sz 
              << " and btype = " << std::string(btype, 4) << std::endl;
    
    EXPECT_NO_THROW({
        player_isobmff::IsoBmffBox box(sz, btype);
        std::cout << "player_isobmff::IsoBmffBox object created with sz: " << sz 
                  << " and btype: " << std::string(btype, 4) << std::endl;
    });
    
    std::cout << "Exiting CreateWithZeroSizeMoov test" << std::endl;
}
/**
 * @brief Validates that IsoBmffBox is constructed correctly using default parameters.
 *
 * This test verifies that when provided with a correctly formatted header that simulates the 'ftyp' box type,
 * along with a maximum size value of 1000, the constructBox method returns a non-null pointer indicating a
 * successful box construction. The test confirms that the box is properly instantiated under default parameters.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 081
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                         | Test Data                                                                                                  | Expected Result                                                    | Notes      |
 * | :--------------: | --------------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------ | ---------- |
 * | 01               | Prepare header by setting a 4-byte size (1000) and a 4-byte box type ("ftyp"), then call constructBox | hdr: [size = 1000, type = "ftyp"], maxSz = 1000, correctBoxSize = false, newTrackId = -1                   | Returned box pointer is non-null, confirming a valid IsoBmffBox instance | Should Pass |
 */
TEST(player_isobmff_IsoBmffBox, ConstructValidBoxWithDefaultParameters) {
    std::cout << "Entering ConstructValidBoxWithDefaultParameters test" << std::endl;

    // Prepare header buffer with 8 bytes:
    // First 4 bytes for box size (1000), next 4 bytes for box type "ftyp"
    uint8_t hdr[8] = {0};
    uint32_t sizeValue = 1000;
    memcpy(hdr, &sizeValue, sizeof(uint32_t));
    // Using strncpy to assign the fixed array for box type in header
    strncpy(reinterpret_cast<char*>(hdr + 4), "ftyp", 4);
    
    std::cout << "Invoking constructBox with hdr simulating 'ftyp', maxSz: " 
              << 1000 << ", correctBoxSize: false, newTrackId: -1" << std::endl;
    
    // Invoke the constructBox method
    player_isobmff::IsoBmffBox* box = player_isobmff::IsoBmffBox::constructBox(hdr, 1000, false, -1);
    std::cout << "Returned box pointer: " << box << std::endl;

    EXPECT_NE(box, nullptr);    
    std::cout << "Exiting ConstructValidBoxWithDefaultParameters test" << std::endl;
}
/**
 * @brief Test the construction of a valid IsoBmffBox with correction enabled and a new track ID override.
 *
 * This test verifies that the IsoBmffBox is correctly constructed when provided with a header buffer simulating a 'trak' box,
 * a maximum size of 1500, correction enabled, and a track ID override set to 10. It ensures that the API returns a valid (non-null)
 * pointer to an IsoBmffBox instance.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 082@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- | -------------- | ----- |
 * | 01 | Prepare header buffer with 8 bytes simulating a box header | input: hdr buffer with first 4 bytes = sizeValue (1500), next 4 bytes = "trak" | Header buffer is correctly initialized with the expected values | Should be successful |
 * | 02 | Invoke constructBox API with header, maxSz of 1500, correction flag true, and newTrackId as 10 | input: hdr pointer, size = 1500, correctBoxSize = true, newTrackId = 10 | API returns a valid non-null pointer to an IsoBmffBox instance | Should Pass |
 * | 03 | Validate that the returned box pointer is not null using EXPECT_NE | input: output from constructBox (box pointer) | EXPECT_NE(box, nullptr) passes confirming valid box creation | Should Pass |
 */
TEST(player_isobmff_IsoBmffBox, ConstructValidBoxWithCorrectionAndTrackId) {
    std::cout << "Entering ConstructValidBoxWithCorrectionAndTrackId test" << std::endl;

    // Prepare header buffer with 8 bytes:
    // First 4 bytes for box size (1500), next 4 bytes for box type "trak"
    uint8_t hdr[8] = {0};
    uint32_t sizeValue = 1500;
    memcpy(hdr, &sizeValue, sizeof(uint32_t));
    strncpy(reinterpret_cast<char*>(hdr + 4), "trak", 4);
    
    std::cout << "Invoking constructBox with hdr simulating 'trak', maxSz: " 
              << 1500 << ", correctBoxSize: true, newTrackId: 10" << std::endl;
    
    // Invoke the constructBox method with correction flag and newTrackId override.
    player_isobmff::IsoBmffBox* box = player_isobmff::IsoBmffBox::constructBox(hdr, 1500, true, 10);
    std::cout << "Returned box pointer: " << box << std::endl;

    EXPECT_NE(box, nullptr);    
    std::cout << "Exiting ConstructValidBoxWithCorrectionAndTrackId test" << std::endl;
}
/**
 * @brief Test to verify that constructing an IsoBmffBox with a null header returns nullptr.
 *
 * This test verifies that when the constructBox API is invoked with a nullptr header, along with specified values for maxSz, correctBoxSize, and newTrackId, it properly handles the null pointer by returning a nullptr. The test ensures the API's robustness in handling invalid input pointers.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 083@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                                 | Test Data                                                         | Expected Result                                              | Notes       |
 * | :--------------: | --------------------------------------------------------------------------- | ----------------------------------------------------------------- | ------------------------------------------------------------ | ----------- |
 * | 01               | Call constructBox with a null header and verify that it returns nullptr.     | hdr = nullptr, maxSz = 500, correctBoxSize = false, newTrackId = -1 | The constructBox API returns a nullptr; the EXPECT_EQ assertion passes. | Should Pass |
 */
TEST(player_isobmff_IsoBmffBox, ConstructBoxWithNullHeader) {
    std::cout << "Entering ConstructBoxWithNullHeader test" << std::endl;

    std::cout << "Invoking constructBox with hdr as nullptr, maxSz: " 
              << 500 << ", correctBoxSize: false, newTrackId: -1" << std::endl;
    
    // Invoke the constructBox method with null header pointer.
    player_isobmff::IsoBmffBox* box = player_isobmff::IsoBmffBox::constructBox(nullptr, 500, false, -1);
    std::cout << "Returned box pointer: " << box << std::endl;

    EXPECT_EQ(box, nullptr);
    std::cout << "Exiting ConstructBoxWithNullHeader test" << std::endl;
}
/**
 * @brief Verify that the getBase() method returns a valid non-null pointer
 *
 * This test checks whether the IsoBmffBox object correctly initializes its base pointer. It first sets up test
 * parameters including the size and type. Then, it constructs an IsoBmffBox object and invokes its getBase() method.
 * Finally, the test asserts that the returned base pointer is not null. This ensures the proper initialization and
 * memory allocation of the underlying buffer used by the IsoBmffBox instance.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 084
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | ------------- | ----- |
 * | 01 | Prepare test parameters including size and type. | testSize = 1024, testType = "ftyp" | Test parameters are initialized correctly. | Should be successful |
 * | 02 | Construct the IsoBmffBox instance using the provided parameters. | Constructor: testSize = 1024, testType = "ftyp" | IsoBmffBox object is constructed successfully without exceptions. | Should Pass |
 * | 03 | Invoke the getBase() method on the constructed IsoBmffBox instance. | Method Call: getBase() | getBase() returns a pointer value. | Should Pass |
 * | 04 | Validate that the returned base pointer is non-null. | Output: basePtr != nullptr | Assertion passes confirming that base pointer is not null. | Should Pass |
 */
TEST(player_isobmff_IsoBmffBox, getBase_valid_non_null) {
    std::cout << "Entering getBase_valid_non_null test" << std::endl;

    // Prepare test parameters
    uint32_t testSize = 1024;
    char testType[4];
    std::memset(testType, 0, sizeof(testType));
    std::strncpy(testType, "ftyp", 4);
    std::cout << "Creating IsoBmffBox with size: " << testSize << " and type: " << std::string(testType, 4) << std::endl;

    // Construct the object
    EXPECT_NO_THROW({
        player_isobmff::IsoBmffBox box(testSize, testType);
        std::cout << "IsoBmffBox object constructed successfully." << std::endl;

        // Invoke getBase() method
        std::cout << "Invoking getBase() method." << std::endl;
        uint8_t* basePtr = box.getBase();
        std::cout << "getBase() returned pointer: " << static_cast<void*>(basePtr) << std::endl;

        // Validate that returned pointer is not null
        EXPECT_NE(basePtr, nullptr) << "Expected a valid non-null base pointer.";
        std::cout << "Verified that the base pointer is non-null." << std::endl;
    });

    std::cout << "Exiting getBase_valid_non_null test" << std::endl;
}
/**
 * @brief Verify that getChildren returns a non-null pointer to an empty vector when no children exist in the IsoBmffBox object
 *
 * This test ensures that when an IsoBmffBox is created without any children added, calling getChildren returns a valid non-null pointer,
 * even if the underlying vector is empty. It validates proper construction of the object, correct handling of the children vector, and
 * that no exceptions are thrown during the process.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 085@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**
 * | Variation / Step | Description                                                                               | Test Data                                  | Expected Result                                                        | Notes         |
 * | :--------------: | ----------------------------------------------------------------------------------------- | ------------------------------------------ | ---------------------------------------------------------------------- | ------------- |
 * | 01               | Prepare fixed size array for the box type and create an IsoBmffBox object using custom constructor | boxSize = 100, btype = "ftyp"                | No exception thrown; IsoBmffBox object is constructed successfully    | Should Pass   |
 * | 02               | Invoke getChildren() on the IsoBmffBox object and verify that the returned pointer is not null  | No input arguments; output: pointer to vector | getChildren returns a non-null pointer; vector is empty (no children) | Should Pass   |
 */
TEST(player_isobmff_IsoBmffBox, getChildren_ReturnsEmptyVectorWhenNoChildrenExist) {
    std::cout << "Entering getChildren_ReturnsEmptyVectorWhenNoChildrenExist test" << std::endl;
    
    // Prepare box type using strncpy for fixed size array assignment
    char btype[5] = {0};
    strncpy(btype, "ftyp", 4);
    
    // Create IsoBmffBox object using the custom constructor
    uint32_t boxSize = 100;
    EXPECT_NO_THROW({
        player_isobmff::IsoBmffBox tempBox(boxSize, btype);
    });
    player_isobmff::IsoBmffBox box(boxSize, btype);
    std::cout << "Created IsoBmffBox object with size: " << boxSize << " and type: " << btype << std::endl;
    
    // Invoke getChildren and log the call and returned value
    std::cout << "Invoking getChildren() on IsoBmffBox object" << std::endl;
    const std::vector<player_isobmff::IsoBmffBox*> *children = nullptr;
    EXPECT_NO_THROW({
        children = box.getChildren();
    });
    std::cout << "getChildren() returned pointer: " << children << std::endl;
    
    // Verify that the returned pointer is not null
    ASSERT_NE(children, nullptr) << "getChildren() returned a nullptr";    
    std::cout << "Exiting getChildren_ReturnsEmptyVectorWhenNoChildrenExist test" << std::endl;
}
/**
 * @brief Validate that getOffset() returns the correct offset value.
 *
 * This test case verifies that the IsoBmffBox::getOffset() method returns the offset value provided during the creation of the IsoBmffBox object. It checks that no exceptions are thrown and that the returned value matches the expected offset. 
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 086@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                      | Test Data                                        | Expected Result                                                                  | Notes             |
 * | :--------------: | ------------------------------------------------------------------------------------------------ | ------------------------------------------------ | -------------------------------------------------------------------------------- | ----------------- |
 * | 01               | Log the entry into the test.                                                                     | None                                             | Console displays entry message.                                                  | Should be successful |
 * | 02               | Assign the box type using strncpy with the value "ftyp".                                       | boxType = "ftyp"                                 | boxType is correctly assigned to "ftyp".                                         | Should be successful |
 * | 03               | Define the expected offset and log the creation details of the IsoBmffBox object.                | expectedOffset = 100, boxType = "ftyp"             | Expected offset is set and a correct creation message is logged.                 | Should be successful |
 * | 04               | Create IsoBmffBox object and invoke getOffset() method within a block verifying no exception is thrown. | expectedOffset = 100, boxType = "ftyp"             | getOffset() returns 100 and no exceptions are thrown during the call.            | Should Pass         |
 * | 05               | Log the exit from the test.                                                                      | None                                             | Console displays exit message.                                                   | Should be successful |
 */
TEST(player_isobmff_IsoBmffBox, getOffset_returns_valid_value) {
    std::cout << "Entering getOffset_returns_valid_value test" << std::endl;

    // Prepare the box type array using strncpy for fixed size array of 4 characters.
    char boxType[4];
    std::cout << "Assigning box type using strncpy with value \"ftyp\"" << std::endl;
    strncpy(boxType, "ftyp", 4);

    // Define a sample size which will be used as offset.
    uint32_t expectedOffset = 100;
    std::cout << "Creating IsoBmffBox object with size (expected offset): " << expectedOffset
              << " and boxType: " << std::string(boxType, 4) << std::endl;

    // Create the IsoBmffBox object and invoke getOffset()
    EXPECT_NO_THROW({
        player_isobmff::IsoBmffBox box(expectedOffset, boxType);
        std::cout << "Invoking getOffset() method on IsoBmffBox object" << std::endl;
        uint32_t offset = box.getOffset();
        std::cout << "Retrieved offset value: " << offset << std::endl;
    });

    std::cout << "Exiting getOffset_returns_valid_value test" << std::endl;
}
/**
 * @brief Verify that IsoBmffBox::getSize returns the correct positive size.
 *
 * This test verifies that when an IsoBmffBox object is created with a specified size and type, the getSize method returns the correct size. It also ensures that no exceptions are thrown during object creation.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 087@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Assign test input values for expectedSize and boxType. | expectedSize = 1024, boxType = "ftyp" | Input values are correctly assigned. | Should be successful |
 * | 02 | Create IsoBmffBox object using the provided inputs. | expectedSize = 1024, boxType = "ftyp" | Object is created successfully without exception. | Should Pass |
 * | 03 | Invoke the getSize method and validate that the returned size matches the expected size using an assertion. | getSize() output = 1024 | getSize returns 1024 and assertion EXPECT_EQ passes. | Should Pass |
 */
TEST(player_isobmff_IsoBmffBox, getSize_VerifyReturnsCorrectPositiveSize) {
    std::cout << "Entering getSize_VerifyReturnsCorrectPositiveSize test" << std::endl;
    
    // Prepare test input values
    uint32_t expectedSize = 1024;
    char boxType[5] = {0}; // extra space for safety
    std::cout << "Assigning boxType value using strncpy. Input string: \"ftyp\"" << std::endl;
    strncpy(boxType, "ftyp", 4);
    std::cout << "boxType value after assignment: " << boxType << std::endl;
    std::cout << "Creating IsoBmffBox object with size: " << expectedSize 
              << " and type: " << boxType << std::endl;
    
    // Creating IsoBmffBox object using custom constructor, ensure no exception is thrown
    EXPECT_NO_THROW({
        player_isobmff::IsoBmffBox isoBox(expectedSize, boxType);
        
        // Invoking getSize method and logging the invocation and returned value
        std::cout << "Invoking getSize method on IsoBmffBox object." << std::endl;
        uint32_t sizeReturned = isoBox.getSize();
        std::cout << "getSize returned: " << sizeReturned << std::endl;
        
        // Validate that the returned size is equal to the expected size
        EXPECT_EQ(sizeReturned, expectedSize);
    });
    
    std::cout << "Exiting getSize_VerifyReturnsCorrectPositiveSize test" << std::endl;
}
/**
 * @brief Validate getType method for 'ftyp' box type
 *
 * This test verifies that an instance of IsoBmffBox constructed with the box type "ftyp" returns the correct type when getType is called. It ensures that the object correctly stores and returns the assigned box type, thereby confirming the proper functionality of the getType method.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 088
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Assign "ftyp" to boxType, construct an IsoBmffBox object, call getType method and verify the return value. | input: boxType = "ftyp", output: result = "ftyp" | getType() returns "ftyp" and EXPECT_STREQ assertion passes | Should Pass |
 */
TEST(player_isobmff_IsoBmffBox, getType_ftyp) {
    std::cout << "Entering getType_ftyp test" << std::endl;

    char boxType[5];
    std::strncpy(boxType, "ftyp", 4);
    boxType[4] = '\0';
    std::cout << "Assigned boxType value: " << boxType << std::endl;

    EXPECT_NO_THROW({
        player_isobmff::IsoBmffBox box(0, boxType);
        std::cout << "Constructed IsoBmffBox object with boxType: " << boxType << std::endl;
        const char* result = box.getType();
        std::cout << "Invoked getType method, returned: " << result << std::endl;
        EXPECT_STREQ(result, boxType);
    });

    std::cout << "Exiting getType_ftyp test" << std::endl;
}
/**
 * @brief Verify that IsoBmffBox::getType returns the correct box type
 *
 * This test creates an IsoBmffBox object using the "moov" box type and verifies that the getType method returns the same "moov" value. It tests the integrity of the box type handling within the IsoBmffBox implementation.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 089@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Initialize the boxType string with "moov" | boxType = "moov" | boxType is correctly assigned | Should be successful |
 * | 02 | Construct IsoBmffBox object with boxType and invoke getType to verify output | constructor: input1 = 0, input2 = "moov"; method getType returns output = "moov" | getType returns "moov" and assertion passes | Should Pass |
 */
TEST(player_isobmff_IsoBmffBox, getType_moov) {
    std::cout << "Entering getType_moov test" << std::endl;

    char boxType[5];
    std::strncpy(boxType, "moov", 4);
    boxType[4] = '\0';
    std::cout << "Assigned boxType value: " << boxType << std::endl;

    EXPECT_NO_THROW({
        player_isobmff::IsoBmffBox box(0, boxType);
        std::cout << "Constructed IsoBmffBox object with boxType: " << boxType << std::endl;
        const char* result = box.getType();
        std::cout << "Invoked getType method, returned: " << result << std::endl;
        EXPECT_STREQ(result, boxType);
    });

    std::cout << "Exiting getType_moov test" << std::endl;
}
/**
 * @brief Verify that IsoBmffBox returns the correct box type "mvhd" via getType API.
 *
 * This test verifies the behavior of the getType method by constructing an IsoBmffBox object with
 * the box type "mvhd" and confirming that the returned type from getType matches the input value.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 090
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                              | Test Data                                               | Expected Result                                                      | Notes       |
 * | :--------------: | ---------------------------------------------------------------------------------------- | ------------------------------------------------------- | ------------------------------------------------------------------- | ----------- |
 * | 01               | Initialize test input by assigning boxType "mvhd" and constructing an IsoBmffBox object.   | boxType = "mvhd"                                        | Object is constructed without exceptions and boxType is correctly assigned. | Should Pass |
 * | 02               | Invoke getType API and verify that the returned value matches the expected box type "mvhd". | boxType = "mvhd", result = box.getType()                | Returned value equals "mvhd" with successful assertion verification.  | Should Pass |
 */
TEST(player_isobmff_IsoBmffBox, getType_mvhd) {
    std::cout << "Entering getType_mvhd test" << std::endl;

    char boxType[5];
    std::strncpy(boxType, "mvhd", 4);
    boxType[4] = '\0';
    std::cout << "Assigned boxType value: " << boxType << std::endl;

    EXPECT_NO_THROW({
        player_isobmff::IsoBmffBox box(0, boxType);
        std::cout << "Constructed IsoBmffBox object with boxType: " << boxType << std::endl;
        const char* result = box.getType();
        std::cout << "Invoked getType method, returned: " << result << std::endl;
        EXPECT_STREQ(result, boxType);
    });

    std::cout << "Exiting getType_mvhd test" << std::endl;
}
/**
 * @brief Verify that IsoBmffBox::hasChildren works correctly when invoked on a valid object.
 *
 * This test case verifies that creating an IsoBmffBox object with a valid size and box type does not throw any exceptions.
 * It then checks that calling the hasChildren() method on the created object does not throw an exception and returns a valid boolean result.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 091@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                    | Test Data                                        | Expected Result                                          | Notes       |
 * | :--------------: | -------------------------------------------------------------- | ------------------------------------------------ | -------------------------------------------------------- | ----------- |
 * | 01               | Invoke IsoBmffBox constructor with valid size and box type       | sz = 100, btype = "moov"                         | No exception thrown and object successfully constructed  | Should Pass |
 * | 02               | Invoke hasChildren() method to check for child elements          | object created from step 01; method call hasChildren() | No exception thrown and returned boolean value (expected false) | Should Pass |
 */
TEST(player_isobmff_IsoBmffBox, verifyHasChildren) {
    std::cout << "Entering verifyHasChildren test" << std::endl;
    
    uint32_t sz = 100;
    char btype[5] = {0};
    strncpy(btype, "moov", 4);
    std::cout << "Invoking player_isobmff::IsoBmffBox constructor with sz = " << sz << " and btype = " << btype << std::endl;
    EXPECT_NO_THROW({
        player_isobmff::IsoBmffBox tempBox(sz, btype);
    });
    
    player_isobmff::IsoBmffBox box(sz, btype);
    std::cout << "Player_isobmff::IsoBmffBox object created with internal state: sz = " << sz << " and btype = " << btype << std::endl;
    
    std::cout << "Invoking hasChildren() method" << std::endl;
    bool result = false;
    EXPECT_NO_THROW({
        result = box.hasChildren();
    });
    
    std::cout << "hasChildren() returned: " << std::boolalpha << result << std::endl;    
    std::cout << "Exiting verifyHasChildren test" << std::endl;
}
/**
 * @brief To test the construction of IsoBmffBox and validate that rewriteAsSkipBox rewrites the box buffer to represent a skip box.
 *
 * This test verifies that an IsoBmffBox object can be successfully created using a valid box size and type, and that invoking the rewriteAsSkipBox() method does not throw any exceptions. It ensures that both the constructor and the method behave as expected under normal conditions.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 092
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | --------- | -------------- | ----- |
 * | 01 | Create an IsoBmffBox with size 1024 and type 'moov'. | boxSize = 1024, type = "moov" | The IsoBmffBox constructor should execute without throwing an exception. | Should Pass |
 * | 02 | Invoke the rewriteAsSkipBox() method on the constructed object. | Method call: rewriteAsSkipBox(), no additional input parameters. | The method should execute successfully without throwing an exception and rewrite the box buffer as a skip box. | Should Pass |
 */
TEST(player_isobmff_IsoBmffBox, rewriteAsSkipBox_start) {
    std::cout << "Entering rewriteAsSkipBox_start test" << std::endl;

    // Create a valid box with size 1024 and type "moov"
    uint32_t boxSize = 1024;
    char type[5] = {0};
    std::cout << "Assigning box type 'moov' using strncpy with size 4." << std::endl;
    std::strncpy(type, "moov", 4);
    std::cout << "Created box with size: " << boxSize << " and type: " << type << std::endl;
    
    // Construct the object using the provided constructor
    EXPECT_NO_THROW({
        player_isobmff::IsoBmffBox box(boxSize, type);
        std::cout << "IsoBmffBox object constructed successfully." << std::endl;
        
        // Invoke rewriteAsSkipBox(), which rewrites the box buffer to represent a skip box.
        std::cout << "Invoking rewriteAsSkipBox() method on the IsoBmffBox object." << std::endl;
        EXPECT_NO_THROW({
            box.rewriteAsSkipBox();
            std::cout << "rewriteAsSkipBox() executed successfully. The box buffer data has been rewritten to a skip box." << std::endl;
        });
        
        // Note: After rewriteAsSkipBox() the object should not be used further.
        std::cout << "After rewriteAsSkipBox(), the object should not be used further." << std::endl;
    });

    std::cout << "Exiting rewriteAsSkipBox_start test" << std::endl;
}

// Assuming PLAYER_SIZEOF_SIZE_AND_TAG is defined externally.
// For testing purpose, we define it here.
#ifndef PLAYER_SIZEOF_SIZE_AND_TAG
  #define PLAYER_SIZEOF_SIZE_AND_TAG 8
#endif
/**
 * @brief Test the setBase method with a valid pointer input.
 *
 * This test verifies that the IsoBmffBox object's setBase method does not throw an exception when provided with a valid pointer to a buffer. The test creates a valid buffer, calculates a pointer offset by PLAYER_SIZEOF_SIZE_AND_TAG, and then calls setBase on an instance of IsoBmffBox. This ensures that both the IsoBmffBox constructor and the setBase method can correctly handle valid inputs.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 093@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | ----------- | --------- | -------------- | ----- |
 * | 01 | Create an IsoBmffBox instance with valid size and box type, compute a valid pointer offset, and invoke setBase method | buffer pointer = (buffer + PLAYER_SIZEOF_SIZE_AND_TAG), bufferSize = 100, box size = 100, boxType = "ftyp" | IsoBmffBox instance is created successfully and setBase does not throw an exception | Should Pass |
 */
TEST(player_isobmff_IsoBmffBox, setBase_validPointer) {
    std::cout << "Entering setBase_validPointer test" << std::endl;
    
    // Prepare a valid buffer.
    const size_t bufferSize = 100;
    uint8_t buffer[bufferSize] = {0};
    
    // Calculate the pointer that will be passed to setBase.
    uint8_t * inputPtr = buffer + PLAYER_SIZEOF_SIZE_AND_TAG;
    std::cout << "Allocated buffer address: " << static_cast<void*>(buffer) << std::endl;
    std::cout << "Input pointer (buffer + PLAYER_SIZEOF_SIZE_AND_TAG): " << static_cast<void*>(inputPtr) << std::endl;
    
    // Prepare the box type using strncpy for fixed size array.
    char boxType[5] = {0};
    strncpy(boxType, "ftyp", 4);
    std::cout << "Box type passed to constructor: " << boxType << std::endl;
    
    // Create an instance of IsoBmffBox using the custom constructor.
    EXPECT_NO_THROW({
        player_isobmff::IsoBmffBox box(100, boxType);
        std::cout << "Created player_isobmff::IsoBmffBox instance with size 100 and type " << boxType << std::endl;
        
        // Invoke setBase method with the valid pointer.
        std::cout << "Invoking setBase with input pointer: " << static_cast<void*>(inputPtr) << std::endl;
        EXPECT_NO_THROW(box.setBase(inputPtr));
        std::cout << "setBase invoked successfully" << std::endl;
        
    });
    
    std::cout << "Exiting setBase_validPointer test" << std::endl;
}
/**
 * @brief Test to verify that IsoBmffBox::setBase correctly handles a nullptr parameter
 *
 * This test verifies that the IsoBmffBox::setBase method does not throw an exception when provided a nullptr.
 * It confirms that the method gracefully handles a situation where a null base pointer is supplied, ensuring robust error handling.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 094@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**
 * | Variation / Step | Description                                                        | Test Data                                               | Expected Result                                           | Notes            |
 * | :--------------: | ------------------------------------------------------------------ | ------------------------------------------------------- | --------------------------------------------------------- | ---------------- |
 * | 01               | Prepare the fixed size array for box type using strncpy            | boxType: array initialized to "ftyp"                    | boxType correctly populated with "ftyp"                   | Should be successful |
 * | 02               | Create an instance of IsoBmffBox using size 100 and the fixed box type| size = 100, boxType = "ftyp"                              | Instance of IsoBmffBox created without throwing an exception | Should be successful |
 * | 03               | Invoke setBase method with a nullptr pointer                         | nullPtr = nullptr                                       | setBase executes without throwing an exception            | Should Pass      |
 */
TEST(player_isobmff_IsoBmffBox, setBase_nullPointer) {
    std::cout << "Entering setBase_nullPointer test" << std::endl;
    
    // Prepare the box type using strncpy for fixed size array.
    char boxType[5] = {0};
    strncpy(boxType, "ftyp", 4);
    std::cout << "Box type passed to constructor: " << boxType << std::endl;
    
    // Create an instance of IsoBmffBox.
    EXPECT_NO_THROW({
        player_isobmff::IsoBmffBox box(100, boxType);
        std::cout << "Created player_isobmff::IsoBmffBox instance with size 100 and type " << boxType << std::endl;
        
        // Use nullptr for setBase.
        uint8_t * nullPtr = nullptr;
        std::cout << "Invoking setBase with nullptr" << std::endl;
        EXPECT_NO_THROW(box.setBase(nullPtr));
        std::cout << "setBase invoked successfully with nullptr" << std::endl;
    });
    
    std::cout << "Exiting setBase_nullPointer test" << std::endl;
}
/**
 * @brief Tests that the IsoBmffBox's setOffset method accepts the minimum valid value.
 *
 * This test verifies that an instance of player_isobmff::IsoBmffBox can be created with a valid size and type,
 * and that invoking the setOffset method with a minimum valid offset value (0) does not throw any exceptions.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 095
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                 | Test Data                                       | Expected Result                                                                        | Notes       |
 * | :--------------: | --------------------------------------------------------------------------- | ----------------------------------------------- | -------------------------------------------------------------------------------------- | ----------- |
 * | 01               | Create a player_isobmff::IsoBmffBox object with size = 100 and type = "ftyp"  | size = 100, type = "ftyp"                         | Object is created successfully without any exceptions                                   | Should Pass |
 * | 02               | Invoke setOffset method with the minimum valid offset value 0               | input_offset = 0                                | Method setOffset executes successfully without throwing an exception                   | Should Pass |
 */
TEST(player_isobmff_IsoBmffBox, setOffset_minimum_value) {
    std::cout << "Entering setOffset_minimum_value test" << std::endl;
    
    // Prepare type parameter using strncpy for fixed size char array
    char type[5] = {0};
    strncpy(type, "ftyp", 4);
    
    // Create object using custom constructor
    EXPECT_NO_THROW({
        player_isobmff::IsoBmffBox box(100, type);
        std::cout << "Created player_isobmff::IsoBmffBox object with size=100 and type=" << type << std::endl;
        
        // Invoke setOffset with minimum valid value 0
        uint32_t input_offset = 0;
        std::cout << "Invoking setOffset with os = " << input_offset << std::endl;
        EXPECT_NO_THROW(box.setOffset(input_offset));
        std::cout << "Method setOffset invoked successfully with value " << input_offset << std::endl;
    });
    
    std::cout << "Exiting setOffset_minimum_value test" << std::endl;
}
/**
 * @brief Test setOffset API with typical value.
 *
 * This test validates that the player_isobmff::IsoBmffBox's setOffset method correctly handles a typical offset value (100) without throwing any exceptions.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 096@n
 * **Priority:** (High) High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * |01| Create IsoBmffBox object with size=100 and type "ftyp". | input: size = 100, type = "ftyp" | Object is created without throwing an exception | Should Pass |
 * |02| Invoke setOffset method with offset = 100 on the created object. | input: offset = 100 | Method call executes without throwing an exception | Should Pass |
 */
TEST(player_isobmff_IsoBmffBox, setOffset_typical_value) {
    std::cout << "Entering setOffset_typical_value test" << std::endl;
    
    char type[5] = {0};
    strncpy(type, "ftyp", 4);
    
    EXPECT_NO_THROW({
        player_isobmff::IsoBmffBox box(100, type);
        std::cout << "Created player_isobmff::IsoBmffBox object with size=100 and type=" << type << std::endl;
        
        // Invoke setOffset with a typical value 100
        uint32_t input_offset = 100;
        std::cout << "Invoking setOffset with os = " << input_offset << std::endl;
        EXPECT_NO_THROW(box.setOffset(input_offset));
        std::cout << "Method setOffset invoked successfully with value " << input_offset << std::endl;
    });
    
    std::cout << "Exiting setOffset_typical_value test" << std::endl;
}
/**
 * @brief Validate setting the IsoBmffBox size to zero
 *
 * This test validates that the IsoBmffBox API correctly handles setting its size parameter to zero without throwing an exception. It first constructs an IsoBmffBox object with an initial size of 500 and a predefined box type, and then invokes the setSize method with 0 to ensure that the implementation correctly processes this edge case.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 097@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data |Expected Result |Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Construct IsoBmffBox object using the custom constructor with initial size 500 and box type FTYP | input: size = 500, boxType = player_isobmff::IsoBmffBox::FTYP; output: valid object instance | Object is constructed without throwing an exception | Should Pass |
 * | 02 | Invoke setSize API with newSize = 0 to update the object's size | input: newSize = 0; output: not applicable | setSize function call completes without throwing an exception | Should Pass |
 */
TEST(player_isobmff_IsoBmffBox, setSize_to_zero) {
    std::cout << "Entering setSize_to_zero test" << std::endl;
    
    // Prepare a fixed size array for box type using strncpy.
    char boxType[4];
    std::cout << "Assigning box type using strncpy with value: " << player_isobmff::IsoBmffBox::FTYP << std::endl;
    strncpy(boxType, player_isobmff::IsoBmffBox::FTYP, 4);
    
    // Create an IsoBmffBox object using the custom constructor.
    EXPECT_NO_THROW({
        player_isobmff::IsoBmffBox box(500, boxType);
    });
    player_isobmff::IsoBmffBox box(500, boxType);
    std::cout << "Constructed IsoBmffBox object with initial size: 500 and type: " << boxType << std::endl;
    
    // Invoke setSize with 0.
    uint32_t newSize = 0;
    std::cout << "Invoking setSize with newSize: " << newSize << std::endl;
    EXPECT_NO_THROW(box.setSize(newSize));
    std::cout << "setSize invoked without throwing exception." << std::endl;
    std::cout << "Exiting setSize_to_zero test" << std::endl;
}
/**
 * @brief Validate that the setSize method correctly updates the size of IsoBmffBox for a typical positive value
 *
 * This test case verifies that the setSize method of the IsoBmffBox class correctly updates the size attribute when provided with a typical positive value. It ensures that the IsoBmffBox object is constructed properly using a fixed box type and that setSize does not throw any exceptions.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 098
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                        | Test Data                                                                          | Expected Result                                                                     | Notes            |
 * | :---------------:| ------------------------------------------------------------------- | ---------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------- | ---------------- |
 * | 01               | Prepare a fixed box type and construct an IsoBmffBox object            | boxType = player_isobmff::IsoBmffBox::FTYP, initial size = 500                       | IsoBmffBox object is created without throwing exceptions                            | Should be successful |
 * | 02               | Invoke setSize with a new size and verify no exception occurs          | newSize = 1000                                                                     | setSize updates the box size without throwing an exception                          | Should Pass      |
 */
TEST(player_isobmff_IsoBmffBox, setSize_to_typical_positive_value) {
    std::cout << "Entering setSize_to_typical_positive_value test" << std::endl;
    
    // Prepare a fixed size array for box type using strncpy.
    char boxType[4];
    std::cout << "Assigning box type using strncpy with value: " << player_isobmff::IsoBmffBox::FTYP << std::endl;
    strncpy(boxType, player_isobmff::IsoBmffBox::FTYP, 4);
    
    // Create an IsoBmffBox object using the custom constructor.
    EXPECT_NO_THROW({
        player_isobmff::IsoBmffBox box(500, boxType);
    });
    player_isobmff::IsoBmffBox box(500, boxType);
    std::cout << "Constructed IsoBmffBox object with initial size: 500 and type: " << boxType << std::endl;
    
    // Invoke setSize with a typical positive value.
    uint32_t newSize = 1000;
    std::cout << "Invoking setSize with newSize: " << newSize << std::endl;
    EXPECT_NO_THROW(box.setSize(newSize));
    std::cout << "setSize invoked without throwing exception." << std::endl;    
    std::cout << "Exiting setSize_to_typical_positive_value test" << std::endl;
}
/**
 * @brief Test IsoBmffBox truncate() method to ensure proper functionality.
 *
 * This test verifies that the IsoBmffBox::truncate() method executes without throwing exceptions.
 * It first creates an IsoBmffBox object with a defined size and type, then calls the truncate() method, 
 * ensuring that both operations complete successfully.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 099@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                       | Test Data                                | Expected Result                                                         | Notes      |
 * | :--------------: | ----------------------------------------------------------------- | ---------------------------------------- | ----------------------------------------------------------------------- | ---------- |
 * | 01               | Create an IsoBmffBox object with size=500 and type="ftyp".         | input: size = 500, type = "ftyp"           | Object is created successfully without throwing exceptions.           | Should Pass|
 * | 02               | Invoke the truncate() method on the IsoBmffBox object.              | input: none, output: no exception thrown  | truncate() method executes successfully without throwing exceptions.   | Should Pass|
 */
TEST(player_isobmff_IsoBmffBox, truncate()_start) {
    std::cout << "Entering player_isobmff::IsoBmffBox::truncate()_start test" << std::endl;

    // Create an IsoBmffBox object with size = 500 and type = "ftyp"
    uint32_t size = 500;
    char type[5] = {0}; // extra space for null-terminator for debugging clarity
    // Using strncpy to assign value to fixed size array.
    std::strncpy(type, "ftyp", 4);
    std::cout << "Creating IsoBmffBox object with size: " << size << " and type: " << type << std::endl;
    EXPECT_NO_THROW({
        player_isobmff::IsoBmffBox box(size, type);
        std::cout << "IsoBmffBox object created successfully." << std::endl;

        // Invoke the truncate() method without parameters.
        std::cout << "Invoking truncate() with no parameters on the IsoBmffBox object." << std::endl;
        EXPECT_NO_THROW({
            box.truncate();
        });
        std::cout << "truncate() invoked successfully. (No exceptions thrown)" << std::endl;
    });

    std::cout << "Exiting player_isobmff::IsoBmffBox::truncate()_start test" << std::endl;
}
/**
 * @brief Verify that calling truncate with 0 does not throw exceptions
 *
 * This test validates that the IsoBmffBox object, when constructed with an initial size of 100 and a valid box type ("ftyp"), successfully handles a call to the truncate method with a parameter value of 0 without throwing any exceptions. The purpose is to ensure that the API gracefully manages a truncation request where no actual truncation is intended.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 100@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**
 * | Variation / Step | Description                                                            | Test Data                                                      | Expected Result                                                        | Notes         |
 * | :--------------: | ---------------------------------------------------------------------- | -------------------------------------------------------------- | ---------------------------------------------------------------------- | ------------- |
 * | 01               | Create an IsoBmffBox object with initial size 100 and type "ftyp".     | input: initial size = 100, boxType = "ftyp"                      | Object is constructed without throwing exceptions                      | Should Pass   |
 * | 02               | Invoke the truncate method on the IsoBmffBox object with parameter 0.    | input: truncate param = 0                                         | truncate function executes without throwing exceptions                 | Should Pass   |
 */
TEST(player_isobmff_IsoBmffBox, TruncateWithZeroParam) {
    std::cout << "Entering TruncateWithZeroParam test" << std::endl;

    // Prepare btype using strncpy
    char boxType[5] = {0}; // allocate extra space for null termination
    std::strncpy(boxType, "ftyp", 4);
    std::cout << "Created btype: " << boxType << std::endl;

    // Create IsoBmffBox object with initial size 100
    EXPECT_NO_THROW({
        player_isobmff::IsoBmffBox box(100, boxType);
        std::cout << "Constructed IsoBmffBox with initial size 100 and type: " << boxType << std::endl;

        // Invoke truncate with parameter 0
        std::cout << "Invoking truncate(0)" << std::endl;
        EXPECT_NO_THROW(box.truncate(0));
    });

    std::cout << "Exiting TruncateWithZeroParam test" << std::endl;
}
/**
 * @brief Test for truncating IsoBmffBox with valid parameter.
 *
 * This test verifies that an IsoBmffBox object, when constructed with an initial size of 200 and a valid type ("moov"), can be successfully truncated to 150 without throwing any exceptions. It ensures proper behavior of the truncate method with valid input.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 101@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                        | Test Data                                       | Expected Result                                         | Notes          |
 * | :--------------: | ------------------------------------------------------------------ | ----------------------------------------------- | ------------------------------------------------------- | -------------- |
 * | 01               | Prepare the box type using strncpy with value "moov"              | boxType = "moov"                                | Proper initialization of boxType without exception      | Should be successful |
 * | 02               | Construct IsoBmffBox with initial size 200 and type "moov"          | input_size = 200, input_btype = "moov"            | IsoBmffBox object is created successfully without exceptions | Should Pass    |
 * | 03               | Invoke truncate API with parameter 150                             | input_truncate_value = 150                        | truncate method executes without exceptions            | Should Pass    |
 */
TEST(player_isobmff_IsoBmffBox, TruncateWithValidParam) {
    std::cout << "Entering TruncateWithValidParam test" << std::endl;

    // Prepare btype using strncpy
    char boxType[5] = {0};
    std::strncpy(boxType, "moov", 4);
    std::cout << "Created btype: " << boxType << std::endl;

    // Create IsoBmffBox object with initial size 200
    EXPECT_NO_THROW({
        player_isobmff::IsoBmffBox box(200, boxType);
        std::cout << "Constructed IsoBmffBox with initial size 200 and type: " << boxType << std::endl;

        // Invoke truncate with parameter 150
        std::cout << "Invoking truncate(150)" << std::endl;
        EXPECT_NO_THROW(box.truncate(150));
    });

    std::cout << "Exiting TruncateWithValidParam test" << std::endl;
}
/**
 * @brief Validate destructor invocation on heap allocated IsoBmffBox object.
 *
 * Verifies that IsoBmffBox's destructor is properly called when the object is dynamically allocated on the heap and then deleted, ensuring proper resource and memory cleanup.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 102@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                           | Test Data                           | Expected Result                                                                | Notes       |
 * | :----:           | ------------------------------------------------------------------------------------- | ----------------------------------- | ------------------------------------------------------------------------------ | ----------- |
 * | 01               | Allocate an IsoBmffBox object on the heap without throwing an exception.              | sizeValue = 1024, boxType = ftyp      | Constructor completes without throwing an exception.                         | Should Pass |
 * | 02               | Delete the allocated IsoBmffBox object to invoke its destructor without throwing an exception. | sizeValue = 1024, boxType = ftyp (object deletion) | Destructor is invoked and completes without throwing an exception. | Should Pass |
 */
TEST(IsoBmffBox, player_isobmff_IsoBmffBox_destructor_invoked_on_heap_allocated_object_start)
{
    std::cout << "Entering player_isobmff::IsoBmffBox::~IsoBmffBox()_start test" << std::endl;
    
    // Prepare parameters for construction
    uint32_t sizeValue = 1024;
    char boxType[5] = {0}; // Fixed size array for 4 characters (+null terminator)
    // Using strncpy to assign value to fixed size array
    strncpy(boxType, "ftyp", 4);
    std::cout << "Assigned box type: " << boxType << " using strncpy" << std::endl;
    std::cout << "Size value provided: " << sizeValue << std::endl;
    
    // Create object on heap and log the invocation
    player_isobmff::IsoBmffBox* boxPtr = nullptr;
    EXPECT_NO_THROW({
        boxPtr = new player_isobmff::IsoBmffBox(sizeValue, boxType);
        std::cout << "Invoked constructor IsoBmffBox(" << sizeValue << ", " << boxType << ") successfully." << std::endl;
    });
    
    // Delete the object to invoke destructor and log the steps
    EXPECT_NO_THROW({
        std::cout << "Invoking destructor by deleting the IsoBmffBox object." << std::endl;
        delete boxPtr;
        std::cout << "IsoBmffBox destructor completed; object successfully destroyed." << std::endl;
    });
    
    std::cout << "Exiting player_isobmff::IsoBmffBox::~IsoBmffBox()_start test" << std::endl;
}
/**
 * @brief Validates that the MdatIsoBmffBox constructor successfully creates an object when provided with a valid H.264 sample.
 *
 * This test constructs an H.264 sample buffer, allocates memory for it, and passes it along with its size to the MdatIsoBmffBox constructor.
 * The objective is to ensure that the constructor does not throw any exceptions and the object is created properly with valid input data.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 103@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | ------------- | ----- |
 * | 01 | Allocate memory block and copy H.264 sample data from the provided array. | h264Sample = {0x00,0x00,0x00,0x01,0x67,0x42,0x00,0x29,0x8D,0x8D,0x40,0x78}, sz = sizeof(h264Sample), locn = pointer to allocated memory | Memory allocated and contents copied exactly as in the source sample. | Should be successful |
 * | 02 | Print the H.264 sample data in hexadecimal format for verification. | locn = pointer to allocated memory, sz = size of memory block | Correct hex output matching the sample data is printed. | Should be successful |
 * | 03 | Invoke the MdatIsoBmffBox constructor with valid sample size and pointer. | input: sz = sizeof(h264Sample), locn = pointer to allocated memory; output: MdatIsoBmffBox object | Constructor does not throw any exception and constructs the object successfully. | Should Pass |
 * | 04 | Deallocate the memory allocated for the H.264 sample data after test execution. | locn = pointer to allocated memory to be deleted | Memory is deallocated with no errors or memory leaks. | Should be successful |
 */
TEST(player_isobmff_MdatIsoBmffBox, ValidConstructionWithH264Sample) {
    std::cout << "Entering ValidConstructionWithH264Sample test" << std::endl;

    // Example H.264 sample (SPS NAL unit fragment)
    uint8_t h264Sample[] = {
        0x00, 0x00, 0x00, 0x01, // NAL start code
        0x67, 0x42, 0x00, 0x29, // SPS (sequence parameter set)
        0x8D, 0x8D, 0x40, 0x78  // example payload
    };

    uint32_t sz = sizeof(h264Sample);
    uint8_t* locn = new uint8_t[sz];
    std::memcpy(locn, h264Sample, sz);

    std::cout << "Allocated memory block at location: " << static_cast<void*>(locn)
              << " with size: " << sz << std::endl;

    // Print buffer contents in hex for verification
    std::cout << "H.264 sample data (hex): ";
    for (uint32_t i = 0; i < sz; ++i) {
        std::cout << std::hex << static_cast<int>(locn[i]) << " ";
    }
    std::cout << std::dec << std::endl;

    std::cout << "Invoking constructor with sz: " << sz
              << " and locn: " << static_cast<void*>(locn) << std::endl;

    // Expect no exception during valid construction
    EXPECT_NO_THROW({
        player_isobmff::MdatIsoBmffBox box(sz, locn);
        std::cout << "MdatIsoBmffBox object constructed successfully." << std::endl;
    });

    delete[] locn;
    std::cout << "Exiting ValidConstructionWithH264Sample test" << std::endl;
}
/**
 * @brief Verify that the MdatIsoBmffBox constructor throws an exception for zero size input
 *
 * This test verifies that when a zero size value is passed to the MdatIsoBmffBox constructor along with a valid dummy memory block, an exception is thrown to indicate an error in the object construction process.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 104@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**
 * | Variation / Step | Description                                                        | Test Data                                                       | Expected Result                                                | Notes       |
 * | :--------------: | ------------------------------------------------------------------- | --------------------------------------------------------------- | -------------------------------------------------------------- | ----------- |
 * | 01               | Call MdatIsoBmffBox constructor with zero size and a valid memory block | sz = 0, buffer = pointer to dummy memory block (10 bytes)         | Exception is thrown indicating invalid zero size input         | Should Fail |
 */
TEST(player_isobmff_MdatIsoBmffBox, ZeroSize) {
    std::cout << "Entering ZeroSize test" << std::endl;
    
    uint32_t sz = 0;
    // Create a small valid memory block
    uint8_t buffer[10] = {0};
    
    std::cout << "Using memory block at location: " << static_cast<void*>(buffer)
              << " with dummy size: 10 bytes" << std::endl;
    std::cout << "Invoking constructor with sz: " << sz 
              << " and locn: " << static_cast<void*>(buffer) << std::endl;
    
    // Expect an exception to be thrown for zero size construction
    EXPECT_ANY_THROW({
        player_isobmff::MdatIsoBmffBox box(sz, buffer);
        std::cout << "MdatIsoBmffBox object constructed with zero size (unexpected)." << std::endl;
    });
    
    std::cout << "Exiting ZeroSize test" << std::endl;
}
/**
 * @brief Verify that the MdatIsoBmffBox constructor throws an exception when a null pointer is provided.
 *
 * This test checks that providing a null pointer for the location parameter in the MdatIsoBmffBox constructor results in an exception.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 105@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                      | Test Data                         | Expected Result                          | Notes       |
 * | :--------------: | ------------------------------------------------------------------------------------------------ | --------------------------------- | ---------------------------------------- | ----------- |
 * | 01               | Invoke the MdatIsoBmffBox constructor with size = 1024 and locn = nullptr to test exception handling | sz = 1024, locn = nullptr          | An exception is thrown by the constructor | Should Pass |
 */
TEST(player_isobmff_MdatIsoBmffBox, NullLocation) {
    std::cout << "Entering NullLocation test" << std::endl;
    
    uint32_t sz = 1024;
    uint8_t *locn = nullptr;
    
    std::cout << "Invoking constructor with sz: " << sz 
              << " and locn: " << locn << std::endl;
    
    // Expect an exception to be thrown when a null pointer is provided
    EXPECT_ANY_THROW({
        player_isobmff::MdatIsoBmffBox box(sz, locn);
        std::cout << "MdatIsoBmffBox object constructed with null location (unexpected)." << std::endl;
    });
    
    std::cout << "Exiting NullLocation test" << std::endl;
}
/**
 * @brief Verify that constructing an MdatIsoBmffBox with a valid typical size correctly initializes the object.
 *
 * This test verifies that the API player_isobmff::MdatIsoBmffBox::constructMdatBox correctly processes a valid typical size (1024)
 * and a correctly initialized buffer. The test checks that no exceptions are thrown and that the returned pointer is non-null,
 * thereby ensuring that the construct method behaves as expected under normal conditions.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 106@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**
 * | Variation / Step | Description                                                                         | Test Data                                                       | Expected Result                                     | Notes         |
 * | :--------------: | ----------------------------------------------------------------------------------- | ---------------------------------------------------------------- | --------------------------------------------------- | ------------- |
 * | 01               | Initialize test parameters with size = 1024 and allocate a buffer with a known pattern | sz = 1024, buffer = allocated array of 1024 bytes with pattern     | Buffer is allocated and initialized without errors | Should be successful |
 * | 02               | Invoke player_isobmff::MdatIsoBmffBox::constructMdatBox with valid size and buffer     | input: sz = 1024, input: buffer = valid pointer, output: mdatBox pointer | API returns a valid non-null pointer               | Should Pass   |
 * | 03               | Validate that the returned pointer from constructMdatBox is non-null                   | mdatBox != nullptr                                               | Assertion passes confirming pointer is non-null      | Should be successful |
 */
TEST(player_isobmff_MdatIsoBmffBox, ConstructWithValidTypicalSize) {
    std::cout << "Entering ConstructWithValidTypicalSize test" << std::endl;

    // Initialize test parameters
    uint32_t sz = 1024;
    // Allocate a buffer of at least 1024 bytes
    uint8_t* buffer = new uint8_t[1024];
    // Fill buffer with a known pattern for debug logging
    for (uint32_t i = 0; i < sz; ++i) {
        buffer[i] = static_cast<uint8_t>(i % 256);
    }
    std::cout << "Initialized buffer at address: " << static_cast<void*>(buffer) 
              << " with size: " << sz << std::endl;

    // Invoke constructMdatBox and capture the returned pointer
    player_isobmff::MdatIsoBmffBox* mdatBox = nullptr;
    EXPECT_NO_THROW({
        std::cout << "Invoking player_isobmff::MdatIsoBmffBox::constructMdatBox with sz = " 
                  << sz << " and valid buffer pointer." << std::endl;
        mdatBox = player_isobmff::MdatIsoBmffBox::constructMdatBox(sz, buffer);
        std::cout << "Method returned pointer: " << static_cast<void*>(mdatBox) << std::endl;
    });

    // Validate that the returned pointer is non-null
    EXPECT_NE(mdatBox, nullptr);
    // Clean up allocated buffer if needed (depends on ownership semantics)
    delete[] buffer;
    std::cout << "Exiting ConstructWithValidTypicalSize test" << std::endl;
}
/**
 * @brief Verify the MdatIsoBmffBox::constructMdatBox method handles a null buffer pointer properly
 *
 * This test verifies that calling player_isobmff::MdatIsoBmffBox::constructMdatBox with a valid size (1024)
 * and a null pointer as the buffer does not throw any exceptions and returns a null pointer as expected.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 107@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                         | Test Data                                          | Expected Result                                                                 | Notes         |
 * | :--------------: | --------------------------------------------------------------------| -------------------------------------------------- | ------------------------------------------------------------------------------- | ------------- |
 * | 01               | Invoke constructMdatBox with size = 1024 and a null buffer pointer    | sz = 1024, buffer = nullptr, output = mdatBox        | Method returns a null pointer without throwing an exception; EXPECT_NO_THROW check passes | Should Pass   |
 */
TEST(player_isobmff_MdatIsoBmffBox, ConstructWithNullPointer) {
    std::cout << "Entering ConstructWithNullPointer test" << std::endl;
    
    uint32_t sz = 1024;
    uint8_t* buffer = nullptr; // Explicitly null pointer
    
    std::cout << "Invoking player_isobmff::MdatIsoBmffBox::constructMdatBox with sz = " 
              << sz << " and null pointer." << std::endl;
    player_isobmff::MdatIsoBmffBox* mdatBox = nullptr;
    EXPECT_NO_THROW({
        mdatBox = player_isobmff::MdatIsoBmffBox::constructMdatBox(sz, buffer);
        std::cout << "Method returned pointer: " << static_cast<void*>(mdatBox) << std::endl;
    });
    
    // According to the test plan, expect NULL or error indication
    EXPECT_EQ(mdatBox, nullptr);
    if (mdatBox == nullptr) {
        std::cout << "Received NULL pointer as expected when passing a null buffer." << std::endl;
    }
    
    std::cout << "Exiting ConstructWithNullPointer test" << std::endl;
}
/**
 * @brief Verify that the constructMdatBox API handles a zero size input without throwing exceptions.
 *
 * This test verifies the behavior of player_isobmff::MdatIsoBmffBox::constructMdatBox when invoked with a size of zero.
 * The test ensures that even when a valid minimal non-null pointer is provided for the buffer, the API handles the zero size 
 * by returning a NULL pointer without throwing any exceptions.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 108@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                                                          | Test Data                                                  | Expected Result                                             | Notes       |
 * | :--------------: | ---------------------------------------------------------------------------------------------------- | ---------------------------------------------------------- | ----------------------------------------------------------- | ----------- |
 * | 01               | Initialize a minimal buffer with a size of 1 and set sz to 0; invoke constructMdatBox with these parameters. | sz = 0, buffer = non-null pointer (allocated as new uint8_t[1]) | API returns a NULL pointer and no exception is thrown.      | Should Pass |
 */
TEST(player_isobmff_MdatIsoBmffBox, ConstructWithZeroSize) {
    std::cout << "Entering ConstructWithZeroSize test" << std::endl;
    
    uint32_t sz = 0;
    // Allocate a minimal buffer; even if size is 0, we provide a valid non-null pointer
    uint8_t* buffer = new uint8_t[1];
    std::cout << "Initialized minimal buffer at address: " << static_cast<void*>(buffer) 
              << " for zero size test." << std::endl;
    
    std::cout << "Invoking player_isobmff::MdatIsoBmffBox::constructMdatBox with sz = " 
              << sz << " and valid minimal buffer pointer." << std::endl;
    player_isobmff::MdatIsoBmffBox* mdatBox = nullptr;
    EXPECT_NO_THROW({
        mdatBox = player_isobmff::MdatIsoBmffBox::constructMdatBox(sz, buffer);
        std::cout << "Method returned pointer: " << static_cast<void*>(mdatBox) << std::endl;
    });
    
    // According to the test plan, expect NULL or error indication when size is 0.
    EXPECT_EQ(mdatBox, nullptr);
    if (mdatBox == nullptr) {
        std::cout << "Received NULL pointer as expected when passing a size of zero." << std::endl;
    }
    
    // Clean up allocated buffer
    delete[] buffer;
    std::cout << "Exiting ConstructWithZeroSize test" << std::endl;
}
/**
 * @brief Verify truncation of MdatIsoBmffBox to a smaller valid new size.
 *
 * This test verifies that when the truncate method is called with a newSize that is half of the original size,
 * the MdatIsoBmffBox correctly handles the truncation without throwing exceptions.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 109@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Construct the MdatIsoBmffBox with an original size of 1000 and a dummy buffer filled with known pattern | originalSize = 1000, buffer = "TestDataForBuffer" | Object constructed successfully without exceptions | Should Pass |
 * | 02 | Invoke the truncate method with newSize = originalSize/2 (500) | newSize = 500 | Truncate operation executed successfully without exceptions | Should Pass |
 */
TEST(player_isobmff_MdatIsoBmffBox, TruncateWithSmallerValidNewSize) {
    std::cout << "Entering TruncateWithSmallerValidNewSize test" << std::endl;
    
    uint32_t originalSize = 1000;
    uint32_t newSize = originalSize / 2; // 500
    // Allocate dummy buffer for MdatIsoBmffBox (simulate location data)
    uint8_t buffer[1000];
    // Fill the buffer with a known pattern using strncpy equivalent for binary data.
    // Since strncpy works on char*, we simulate by copying a string into our buffer.
    const char* dummyData = "TestDataForBuffer";
    std::strncpy(reinterpret_cast<char*>(buffer), dummyData, sizeof(buffer));

    // Construct the object expecting no throw.
    EXPECT_NO_THROW({
        player_isobmff::MdatIsoBmffBox box(originalSize, buffer);
        std::cout << "Constructed MdatIsoBmffBox with originalSize: " << originalSize << std::endl;
        
        // Invoke truncate method with newSize (500)
        std::cout << "Invoking truncate with newSize: " << newSize << std::endl;
        EXPECT_NO_THROW(box.truncate(newSize));

    });
    
    std::cout << "Exiting TruncateWithSmallerValidNewSize test" << std::endl;
}
/**
 * @brief Verify that truncating a MdatIsoBmffBox with a new size of zero does not throw an exception.
 *
 * This test checks the behavior of the MdatIsoBmffBox::truncate method when the new size provided is zero.
 * It ensures that the object is constructed successfully and that the truncate method handles the zero value
 * without throwing any exceptions.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 110@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Instantiate MdatIsoBmffBox object with originalSize=1000 and a dummy buffer initialized with "BufferDataForTest". | originalSize = 1000, buffer = "BufferDataForTest" | Object is constructed without any exception. | Should be successful |
 * | 02 | Invoke the truncate method on the box with newSize set to 0. | newSize = 0 | truncate method execution does not throw an exception. | Should Pass |
 * | 03 | Ensure overall test execution completes without exceptions. | N/A | Entire test runs successfully without throwing any exceptions. | Should Pass |
 */
TEST(player_isobmff_MdatIsoBmffBox, TruncateWithZeroNewSize) {
    std::cout << "Entering TruncateWithZeroNewSize test" << std::endl;
    
    uint32_t originalSize = 1000;
    uint32_t newSize = 0;
    // Allocate dummy buffer.
    uint8_t buffer[1000];
    const char* dummyData = "BufferDataForTest";
    std::strncpy(reinterpret_cast<char*>(buffer), dummyData, sizeof(buffer));
    
    EXPECT_NO_THROW({
        player_isobmff::MdatIsoBmffBox box(originalSize, buffer);
        std::cout << "Constructed MdatIsoBmffBox with originalSize: " << originalSize << std::endl;
        
        // Invoke truncate method with newSize = 0.
        std::cout << "Invoking truncate with newSize: " << newSize << std::endl;
        EXPECT_NO_THROW(box.truncate(newSize));
    });
    
    std::cout << "Exiting TruncateWithZeroNewSize test" << std::endl;
}
/**
 * @brief Verify that calling truncate with UINT32_MAX throws an exception
 *
 * This test verifies that when the truncate method of the MdatIsoBmffBox class is invoked with a new size equal to UINT32_MAX, the method fails as expected by throwing an exception. Testing this behavior is crucial to ensure that invalid size values are correctly handled by the API.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 111@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                    | Test Data                                                                                     | Expected Result                                                          | Notes          |
 * | :--------------: | -------------------------------------------------------------- | --------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------ | -------------- |
 * | 01               | Initialize test variables, create dummy buffer and instantiate MdatIsoBmffBox with originalSize = 1000 | originalSize = 1000, newSize = UINT32_MAX, buffer = "AnotherBufferDataTest"                    | Box is successfully constructed and logs printed                         | Should be successful |
 * | 02               | Invoke truncate method with newSize = UINT32_MAX expecting an exception | newSize = UINT32_MAX, output = exception thrown as validated by EXPECT_THROW                    | truncate method should throw an exception, as asserted by EXPECT_THROW     | Should Fail    |
 */
TEST(player_isobmff_MdatIsoBmffBox, TruncateWithUINT32MAX) {
    std::cout << "Entering TruncateWithUINT32MAX test" << std::endl;
    
    uint32_t originalSize = 1000;
    uint32_t newSize = UINT32_MAX;  // 4294967295
    // Allocate dummy buffer.
    uint8_t buffer[1000];
    const char* dummyData = "AnotherBufferDataTest";
    std::strncpy(reinterpret_cast<char*>(buffer), dummyData, sizeof(buffer));
    
    player_isobmff::MdatIsoBmffBox box(originalSize, buffer);
    std::cout << "Constructed MdatIsoBmffBox with originalSize: " << originalSize << std::endl;
    
    
    // Expect the truncate method to fail (throw an exception) due to newSize being UINT32_MAX.
    std::cout << "Invoking truncate with newSize: " << newSize << std::endl;
    EXPECT_THROW(box.truncate(newSize), std::exception);
    
    std::cout << "Exiting TruncateWithUINT32MAX test" << std::endl;
}
/**
 * @brief Verify that the MdhdIsoBmffBox constructor succeeds with valid normal tScale values.
 *
 * This test validates the creation of a MdhdIsoBmffBox object using a valid FullIsoBmffBox object along with a proper tScale value and a valid tScale_loc pointer. The test confirms that the constructor does not throw any exceptions and that the object is instantiated correctly.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 112
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | ------------- | ----- |
 * | 01 | Invoke MdhdIsoBmffBox constructor after creating a valid FullIsoBmffBox and preparing the tScale buffer. | fullBox: (size = 100, btype = "mdhd", version = 0, flags = 0), tScale = 1000, tScale_loc pointer = address(tScaleBuffer) | No exception thrown; MdhdIsoBmffBox object is created successfully. | Should Pass |
 */
TEST(player_isobmff_MdhdIsoBmffBox, ValidInputNormalTScaleValue) {
    std::cout << "Entering ValidInputNormalTScaleValue test" << std::endl;
    
    // Create a FullIsoBmffBox object with valid parameters:
    // size = 100, btype = "mdhd", version = 0, flags = 0.
    uint32_t size = 100;
    char btype[5] = {0};
    strncpy(btype, "mdhd", 4);
    uint8_t version = 0;
    uint32_t flags = 0;
    
    // Log the FullIsoBmffBox parameters
    std::cout << "Creating FullIsoBmffBox with parameters:" 
              << " size = " << size 
              << ", btype = " << btype 
              << ", version = " << static_cast<uint32_t>(version) 
              << ", flags = " << flags 
              << std::endl;
    
    FullIsoBmffBox fullBox(size, btype, version, flags);
    
    // Prepare the tScale_loc buffer
    uint8_t tScaleBuffer[10] = {0};
    uint32_t tScale = 1000;
    
    // Log the parameters passed to MdhdIsoBmffBox constructor
    std::cout << "Invoking MdhdIsoBmffBox constructor with:"
              << " tScale = " << tScale
              << ", tScale_loc pointer = " << static_cast<void*>(tScaleBuffer)
              << std::endl;
    
    // Invoke MdhdIsoBmffBox constructor and expect no exceptions.
    EXPECT_NO_THROW({
        player_isobmff::MdhdIsoBmffBox mdhdBox(fullBox, tScale, tScaleBuffer);
        std::cout << "MdhdIsoBmffBox object created successfully." << std::endl;
    });
    
    std::cout << "Exiting ValidInputNormalTScaleValue test" << std::endl;
}
/**
 * @brief Validate that MdhdIsoBmffBox constructor successfully handles zero tScale input.
 *
 * This test verifies that an MdhdIsoBmffBox object can be created with tScale set to zero without throwing any exception.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 113
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data |Expected Result |Notes |
 * | :--------------: | ----------- | --------- | ------------- |----- |
 * | 01 | Create FullIsoBmffBox with parameters (size=50, btype="mdhd", version=0, flags=0), prepare tScale_loc buffer with tScale set to zero, then invoke MdhdIsoBmffBox constructor and verify no exception is thrown. | input: size=50, btype="mdhd", version=0, flags=0, tScale=0, tScale_loc pointer=tScaleBuffer | MdhdIsoBmffBox object is created successfully without throwing an exception. | Should Pass |
 */
TEST(player_isobmff_MdhdIsoBmffBox, ValidInputZeroTScale) {
    std::cout << "Entering ValidInputZeroTScale test" << std::endl;
    
    // Create a FullIsoBmffBox object with valid parameters:
    // size = 50, btype = "mdhd", version = 0, flags = 0.
    uint32_t size = 50;
    char btype[5] = {0};
    strncpy(btype, "mdhd", 4);
    uint8_t version = 0;
    uint32_t flags = 0;
    
    std::cout << "Creating FullIsoBmffBox with parameters:" 
              << " size = " << size 
              << ", btype = " << btype 
              << ", version = " << static_cast<uint32_t>(version)
              << ", flags = " << flags 
              << std::endl;
    
    FullIsoBmffBox fullBox(size, btype, version, flags);
    
    // Prepare the tScale_loc buffer and set tScale to zero
    uint8_t tScaleBuffer[10] = {0};
    uint32_t tScale = 0;
    
    std::cout << "Invoking MdhdIsoBmffBox constructor with:"
              << " tScale = " << tScale
              << ", tScale_loc pointer = " << static_cast<void*>(tScaleBuffer)
              << std::endl;
    
    EXPECT_NO_THROW({
        player_isobmff::MdhdIsoBmffBox mdhdBox(fullBox, tScale, tScaleBuffer);
        std::cout << "MdhdIsoBmffBox object created successfully with tScale zero." << std::endl;
    });
    
    std::cout << "Exiting ValidInputZeroTScale test" << std::endl;
}
/**
 * @brief Validate that the MdhdIsoBmffBox constructor handles a null tScale_loc pointer gracefully.
 *
 * This test verifies that when a null pointer is passed for tScale_loc to the MdhdIsoBmffBox constructor,
 * the object is created without throwing an exception. It ensures robust handling of invalid memory input.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 114@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create a FullIsoBmffBox object with valid parameters. | size = 150, btype = "mdhd", version = 0, flags = 0 | FullIsoBmffBox object is successfully created. | Should be successful |
 * | 02 | Invoke the MdhdIsoBmffBox constructor with tScale = 500 and a null pointer for tScale_loc and verify no exception is thrown. | tScale = 500, tScale_loc = nullptr | MdhdIsoBmffBox object is created without throwing an exception. | Should Pass |
 */
TEST(player_isobmff_MdhdIsoBmffBox, NegativeInputNullTScaleLoc) {
    std::cout << "Entering NegativeInputNullTScaleLoc test" << std::endl;
    
    // Create a FullIsoBmffBox object with valid parameters:
    // size = 150, btype = "mdhd", version = 0, flags = 0.
    uint32_t size = 150;
    char btype[5] = {0};
    strncpy(btype, "mdhd", 4);
    uint8_t version = 0;
    uint32_t flags = 0;
    
    std::cout << "Creating FullIsoBmffBox with parameters:" 
              << " size = " << size 
              << ", btype = " << btype 
              << ", version = " << static_cast<uint32_t>(version) 
              << ", flags = " << flags 
              << std::endl;
    
    FullIsoBmffBox fullBox(size, btype, version, flags);
    
    uint32_t tScale = 500;
    uint8_t* tScale_loc = nullptr; // Null pointer
    
    std::cout << "Invoking MdhdIsoBmffBox constructor with:"
              << " tScale = " << tScale
              << ", tScale_loc pointer = " << static_cast<void*>(tScale_loc)
              << std::endl;
    
    // Expect the constructor to handle the null pointer gracefully without throwing an exception.
    EXPECT_NO_THROW({
        player_isobmff::MdhdIsoBmffBox mdhdBox(fullBox, tScale, tScale_loc);
        std::cout << "MdhdIsoBmffBox object created with null tScale_loc pointer (handled gracefully)." << std::endl;
    });
    
    std::cout << "Exiting NegativeInputNullTScaleLoc test" << std::endl;
}
/**
 * @brief Test the MdhdIsoBmffBox constructor with valid input parameters.
 *
 * This test verifies that the MdhdIsoBmffBox constructor accepts valid input parameters without throwing any exceptions.
 * The test checks for successful creation of the MdhdIsoBmffBox instance by invoking the constructor with a valid size, tScale, and buffer address.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 115@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                      | Test Data                                                      | Expected Result                              | Notes          |
 * | :--------------: | ---------------------------------------------------------------- | -------------------------------------------------------------- | -------------------------------------------- | -------------- |
 * | 01               | Prepare valid input parameters: initialize sz, tScale, and buffer. | sz = 100, tScale = 48000, buffer pointer = valid memory address  | Parameters initialized successfully.       | Should be successful |
 * | 02               | Invoke the MdhdIsoBmffBox constructor with the provided parameters. | sz = 100, tScale = 48000, buffer pointer = valid memory address  | Constructor does not throw any exception.  | Should Pass    |
 * | 03               | Log messages indicating the successful creation of the MdhdIsoBmffBox instance. | No API parameters applicable; log messages output to console.   | Instance created and messages logged.        | Should be successful |
 */
TEST(player_isobmff_MdhdIsoBmffBox, ValidInputParameters) {
    std::cout << "Entering ValidInputParameters test" << std::endl;
    
    uint32_t sz = 100;
    uint32_t tScale = 48000;
    uint8_t buffer[10] = {0};

    std::cout << "Preparing valid input parameters:" << std::endl;
    std::cout << "sz = " << sz << ", tScale = " << tScale 
              << ", tScale_loc (buffer address) = " << static_cast<void*>(buffer) << std::endl;
    
    std::cout << "Invoking MdhdIsoBmffBox constructor with provided valid parameters." << std::endl;
    EXPECT_NO_THROW({
        player_isobmff::MdhdIsoBmffBox box(sz, tScale, buffer);
        std::cout << "MdhdIsoBmffBox instance created successfully with sz = " << sz
                  << ", tScale = " << tScale 
                  << ", tScale_loc = " << static_cast<void*>(buffer) << std::endl;
    });
    
    std::cout << "Exiting ValidInputParameters test" << std::endl;
}
/**
 * @brief Verify that MdhdIsoBmffBox constructor handles zero tScale value
 *
 * This test validates that creating an instance of MdhdIsoBmffBox with a box size of 50, a tScale value of 0, and a buffer address does not throw an exception. It ensures that the constructor properly handles a tScale value of 0, which is considered an edge case.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 116@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                     | Test Data                                              | Expected Result                                 | Notes         |
 * | :--------------: | ----------------------------------------------------------------------------------------------- | ------------------------------------------------------ | ----------------------------------------------- | ------------- |
 * |       01         | Prepare input parameters with a box size of 50, tScale value of 0, and a buffer for the instance | sz = 50, tScale = 0, buffer = an array of 10 uint8_t zeros | Parameters initialized correctly without errors | Should be successful |
 * |       02         | Invoke the MdhdIsoBmffBox constructor with the prepared input parameters                        | input: sz = 50, tScale = 0, buffer address; output: instance created | MdhdIsoBmffBox instance created without throwing an exception | Should Pass  |
 */
TEST(player_isobmff_MdhdIsoBmffBox, ZeroTScale) {
    std::cout << "Entering ZeroTScale test" << std::endl;
    
    uint32_t sz = 50;
    uint32_t tScale = 0;
    uint8_t buffer[10] = {0};

    std::cout << "Preparing input parameters with zero tScale:" << std::endl;
    std::cout << "sz = " << sz << ", tScale = " << tScale 
              << ", tScale_loc (buffer address) = " << static_cast<void*>(buffer) << std::endl;
    
    std::cout << "Invoking MdhdIsoBmffBox constructor with tScale value of 0." << std::endl;
    EXPECT_NO_THROW({
        player_isobmff::MdhdIsoBmffBox box(sz, tScale, buffer);
        std::cout << "MdhdIsoBmffBox instance created successfully with sz = " << sz
                  << ", tScale = " << tScale 
                  << ", tScale_loc = " << static_cast<void*>(buffer) << std::endl;
    });
    
    std::cout << "Exiting ZeroTScale test" << std::endl;
}
/**
 * @brief Verify that MdhdIsoBmffBox constructor throws an exception when box size is zero
 *
 * This test verifies that the MdhdIsoBmffBox constructor correctly throws an exception when provided with a box size of 0.
 * An exception is expected since a box with size 0 is considered invalid.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 117
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                   | Test Data                                                    | Expected Result                    | Notes       |
 * | :--------------: | --------------------------------------------------------------------------------------------- | ------------------------------------------------------------ | ---------------------------------- | ----------- |
 * | 01               | Invoke the MdhdIsoBmffBox constructor with box size set to 0, expecting an exception to be thrown | input: sz = 0, tScale = 24000, tScale_loc = address of buffer; output: exception thrown | Exception is thrown from the MdhdIsoBmffBox constructor | Should Fail |
 */
TEST(player_isobmff_MdhdIsoBmffBox, ZeroBoxSize) {
    std::cout << "Entering ZeroBoxSize test" << std::endl;
    
    uint32_t sz = 0;
    uint32_t tScale = 24000;
    uint8_t buffer[10] = {0};
    
    std::cout << "Preparing input parameters with zero box size:" << std::endl;
    std::cout << "sz = " << sz << ", tScale = " << tScale 
              << ", tScale_loc (buffer address) = " << static_cast<void*>(buffer) << std::endl;
    
    std::cout << "Invoking MdhdIsoBmffBox constructor with box size = 0 expecting an exception." << std::endl;
    EXPECT_THROW({
        player_isobmff::MdhdIsoBmffBox box(sz, tScale, buffer);
    }, std::exception);
    
    std::cout << "Exiting ZeroBoxSize test" << std::endl;
}
/**
 * @brief Tests if MdhdIsoBmffBox throws an exception when provided with a null tScale_loc pointer.
 *
 * This test verifies that the MdhdIsoBmffBox constructor correctly handles an invalid input by throwing an exception when the tScale_loc pointer is null. This ensures proper error handling in the API.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 118@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                           | Test Data                                     | Expected Result                                         | Notes          |
 * | :--------------: | --------------------------------------------------------------------- | --------------------------------------------- | ------------------------------------------------------- | -------------- |
 * | 01               | Setup input parameters for MdhdIsoBmffBox construction                | sz = 100, tScale = 48000, tScale_loc = nullptr | Parameters initialized correctly                        | Should be successful |
 * | 02               | Invoke MdhdIsoBmffBox constructor with null tScale_loc expecting exception | sz = 100, tScale = 48000, tScale_loc = nullptr | Exception thrown as expected in constructor invocation | Should Pass    |
 */
TEST(player_isobmff_MdhdIsoBmffBox, NullTimeScaleLoc) {
    std::cout << "Entering NullTimeScaleLoc test" << std::endl;
    
    uint32_t sz = 100;
    uint32_t tScale = 48000;
    uint8_t* buffer = nullptr;
    
    std::cout << "Preparing input parameters with null tScale_loc:" << std::endl;
    std::cout << "sz = " << sz << ", tScale = " << tScale 
              << ", tScale_loc = " << static_cast<void*>(buffer) << std::endl;
    
    std::cout << "Invoking MdhdIsoBmffBox constructor with null tScale_loc expecting an exception." << std::endl;
    EXPECT_THROW({
        player_isobmff::MdhdIsoBmffBox box(sz, tScale, buffer);
    }, std::exception);
    
    std::cout << "Exiting NullTimeScaleLoc test" << std::endl;
}
/**
 * @brief Validate that MdhdIsoBmffBox is constructed with valid box size and valid pointer.
 *
 * This test verifies that the constructMdhdBox method of the MdhdIsoBmffBox class correctly creates an instance when provided with a valid box size and a properly initialized buffer. The method is expected to return a non-null pointer, confirming successful processing of the valid input parameters.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 119@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Arrange valid box size and initialize buffer with a dummy MDHD pattern | boxSize = 32, buffer = [0,1,2,...,31] | Buffer is properly allocated and populated with expected pattern | Should be successful |
 * | 02 | Invoke player_isobmff::MdhdIsoBmffBox::constructMdhdBox with valid parameters | boxSize = 32, buffer pointer = address of buffer | Returns a non-null pointer to MdhdIsoBmffBox object | Should Pass |
 * | 03 | Assert that the returned pointer is non-null | mdhdBox != nullptr | EXPECT_NE(mdhdBox, nullptr) assertion passes | Should be successful |
 */
TEST(player_isobmff_MdhdIsoBmffBox, ConstructMdhdBoxWithValidBoxSizeAndValidPointer) {
    std::cout << "Entering ConstructMdhdBoxWithValidBoxSizeAndValidPointer test" << std::endl;
    
    // Arrange: valid box size and valid pointer to buffer.
    uint32_t boxSize = 32;
    // Allocate a buffer with at least boxSize bytes.
    uint8_t buffer[32];
    // Populate the buffer with dummy valid MDHD data. For example, fill buffer with a pattern.
    for (uint32_t i = 0; i < boxSize; i++){
        buffer[i] = static_cast<uint8_t>(i);
    }
    std::cout << "Invoking constructMdhdBox with boxSize = " << boxSize 
              << " and valid pointer address: " << static_cast<void*>(buffer) << std::endl;
    
    // Act: call the method.
    player_isobmff::MdhdIsoBmffBox* mdhdBox = player_isobmff::MdhdIsoBmffBox::constructMdhdBox(boxSize, buffer);
    
    // Log the pointer value returned
    std::cout << "Returned MdhdIsoBmffBox pointer: " << mdhdBox << std::endl;
    
    // Assert: non-null pointer expected.
    EXPECT_NE(mdhdBox, nullptr);
    
    std::cout << "Exiting ConstructMdhdBoxWithValidBoxSizeAndValidPointer test" << std::endl;
}
/**
 * @brief Verify MdhdIsoBmffBox construction fails with zero box size.
 *
 * This test validates that invoking player_isobmff::MdhdIsoBmffBox::constructMdhdBox with a zero box size and a valid pointer returns a nullptr. This behavior is expected to prevent the creation of an invalid MdhdIsoBmffBox when essential size information is missing.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 120@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:** 
 * |Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoke constructMdhdBox with boxSize = 0 and a valid buffer filled with pattern 0xAA | boxSize = 0, buffer = [0xAA,0xAA,0xAA,... (32 bytes)] | API returns nullptr and assertion EXPECT_EQ(mdhdBox, nullptr) passes | Should Pass |
 */
TEST(player_isobmff_MdhdIsoBmffBox, ConstructMdhdBoxWithZeroBoxSize) {
    std::cout << "Entering ConstructMdhdBoxWithZeroBoxSize test" << std::endl;
    
    // Arrange: zero box size and a valid pointer.
    uint32_t boxSize = 0;
    uint8_t buffer[32];
    // Fill buffer with a pattern.
    std::memset(buffer, 0xAA, sizeof(buffer));
    std::cout << "Invoking constructMdhdBox with boxSize = " << boxSize 
              << " and valid pointer address: " << static_cast<void*>(buffer) << std::endl;

    // Act: call the method.
    player_isobmff::MdhdIsoBmffBox* mdhdBox = player_isobmff::MdhdIsoBmffBox::constructMdhdBox(boxSize, buffer);
    
    // Log the pointer value returned
    std::cout << "Returned MdhdIsoBmffBox pointer: " << mdhdBox << std::endl;

    // Assert: Expect nullptr due to zero box size.
    EXPECT_EQ(mdhdBox, nullptr);
    
    std::cout << "Exiting ConstructMdhdBoxWithZeroBoxSize test" << std::endl;
}
/**
 * @brief Verify that constructMdhdBox returns nullptr when provided with a null buffer.
 *
 * This test verifies that when a valid box size but a null buffer is given to constructMdhdBox, the function returns a nullptr. It ensures that the API correctly handles invalid (null) buffer input.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 121
 * **Priority:** High
 * 
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 * 
 * **Test Procedure:**
 * | Variation / Step | Description                                                    | Test Data                            | Expected Result                                                          | Notes       |
 * | :--------------: | -------------------------------------------------------------- | ------------------------------------ | ------------------------------------------------------------------------ | ----------- |
 * | 01               | Invoke constructMdhdBox API with valid box size and null pointer | boxSize = 32, nullBuffer = nullptr   | Should return nullptr; EXPECT_EQ(mdhdBox, nullptr) assertion passes       | Should Fail |
 */
TEST(player_isobmff_MdhdIsoBmffBox, ConstructMdhdBoxWithNullPointer) {
    std::cout << "Entering ConstructMdhdBoxWithNullPointer test" << std::endl;
    
    // Arrange: valid box size but pointer is null.
    uint32_t boxSize = 32;
    uint8_t* nullBuffer = nullptr;
    std::cout << "Invoking constructMdhdBox with boxSize = " << boxSize 
              << " and null pointer." << std::endl;
              
    // Act: call the method.
    player_isobmff::MdhdIsoBmffBox* mdhdBox = player_isobmff::MdhdIsoBmffBox::constructMdhdBox(boxSize, nullBuffer);
    
    // Log the pointer value returned
    std::cout << "Returned MdhdIsoBmffBox pointer: " << mdhdBox << std::endl;
    
    // Assert: Expect nullptr due to null pointer for the buffer.
    EXPECT_EQ(mdhdBox, nullptr);
    
    std::cout << "Exiting ConstructMdhdBoxWithNullPointer test" << std::endl;
}
/**
 * @brief Verifies that getTimeScale() returns the correct positive value with valid input.
 *
 * This test checks that the MdhdIsoBmffBox object's getTimeScale() method accurately returns the expected time scale value.
 * The test initializes a fixed-size array with the string "TmSc" and sets an expected time scale value of 48000.
 * It then creates a MdhdIsoBmffBox object with these values and verifies that the getTimeScale() method returns the correct positive value.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 122@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                                  | Test Data                                                  | Expected Result                                                          | Notes         |
 * | :--------------: | ---------------------------------------------------------------------------- | ---------------------------------------------------------- | ------------------------------------------------------------------------ | ------------- |
 * | 01               | Initialize a fixed-size array and assign the test string "TmSc" to it         | tScale_loc = {0}, initStr = "TmSc"                         | Array tScale_loc is correctly initialized with "TmSc"                    | Should be successful |
 * | 02               | Set the expected time scale value                                            | expectedTimeScale = 48000                                  | expectedTimeScale is set to 48000                                        | Should be successful |
 * | 03               | Create MdhdIsoBmffBox object with the prepared parameters                      | id = 100, expectedTimeScale = 48000, tScale_loc = "TmSc"     | Object is created with correct timeScale value                           | Should Pass   |
 * | 04               | Invoke getTimeScale() method to retrieve the time scale value                  | No additional input; retValue captures the return value     | retValue equals expectedTimeScale (48000)                                | Should Pass   |
 * | 05               | Assert that the returned value matches the expected time scale using EXPECT_EQ  | actual = retValue, expected = 48000                        | EXPECT_EQ(retValue, expectedTimeScale) passes                            | Should Pass   |
 */
TEST(player_isobmff_MdhdIsoBmffBox, Verify_getTimeScale_Returns_Correct_Positive_Value) {
    std::cout << "Entering Verify_getTimeScale_Returns_Correct_Positive_Value test" << std::endl;
    
    // Prepare a fixed-size array for timeScale_loc and assign a test value using strncpy
    uint8_t tScale_loc[4] = {0};
    char initStr[5] = "TmSc";  // 4 characters for the fixed-size array
    strncpy(reinterpret_cast<char*>(tScale_loc), initStr, 4);
    std::cout << "Assigned tScale_loc with value: " 
              << std::string(reinterpret_cast<char*>(tScale_loc), 4) << std::endl;
    
    // Expected time scale value set to a standard positive value (48000)
    uint32_t expectedTimeScale = 48000;
    
    // Create the object using the first constructor
    player_isobmff::MdhdIsoBmffBox box(100, expectedTimeScale, tScale_loc);
    std::cout << "Created player_isobmff::MdhdIsoBmffBox object using first constructor with timeScale: " 
              << expectedTimeScale << std::endl;
    
    // Invoke the getTimeScale() method and capture the return value
    std::cout << "Invoking getTimeScale method" << std::endl;
    uint32_t retValue = box.getTimeScale();
    std::cout << "Method getTimeScale returned value: " << retValue << std::endl;
    
    // Verify that the returned value matches the expected value
    EXPECT_EQ(retValue, expectedTimeScale);
    
    std::cout << "Exiting Verify_getTimeScale_Returns_Correct_Positive_Value test" << std::endl;
}
/**
 * @brief Verify that the getTimeScale method returns zero.
 *
 * This test instantiates a MdhdIsoBmffBox object with a pre-defined time scale value of zero and verifies that the getTimeScale method returns zero as expected.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 123@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Prepare a fixed-size array and assign the string "Zero" using strncpy | tScale_loc = {0}, initStr = "Zero" | tScale_loc correctly contains "Zero" in the first 4 bytes | Should be successful |
 * | 02 | Set the expected time scale to zero | expectedTimeScale = 0 | expectedTimeScale is set to 0 | Should be successful |
 * | 03 | Create a MdhdIsoBmffBox object using the constructor with parameters (100, expectedTimeScale, tScale_loc) | Object parameters: 100, 0, tScale_loc ("Zero") | Object is correctly instantiated with the provided parameters | Should Pass |
 * | 04 | Invoke the getTimeScale() method on the object and capture the returned value | Call to getTimeScale() | The returned value should equal expectedTimeScale (0) | Should Pass |
 * | 05 | Verify that the returned value matches the expected value using EXPECT_EQ | retValue, expectedTimeScale = 0 | EXPECT_EQ confirms retValue equals 0 | Should Pass |
 */
TEST(player_isobmff_MdhdIsoBmffBox, Verify_getTimeScale_Returns_Zero) {
    std::cout << "Entering Verify_getTimeScale_Returns_Zero test" << std::endl;
    
    // Prepare a fixed-size array for timeScale_loc and assign a test value using strncpy
    uint8_t tScale_loc[4] = {0};
    char initStr[5] = "Zero";  // 4 characters for the fixed-size array
    strncpy(reinterpret_cast<char*>(tScale_loc), initStr, 4);
    std::cout << "Assigned tScale_loc with value: " 
              << std::string(reinterpret_cast<char*>(tScale_loc), 4) << std::endl;
    
    // Expected time scale value is zero
    uint32_t expectedTimeScale = 0;
    
    // Create the object using the first constructor
    player_isobmff::MdhdIsoBmffBox box(100, expectedTimeScale, tScale_loc);
    std::cout << "Created player_isobmff::MdhdIsoBmffBox object using first constructor with timeScale: " 
              << expectedTimeScale << std::endl;
    
    // Invoke the getTimeScale() method and capture the return value
    std::cout << "Invoking getTimeScale method" << std::endl;
    uint32_t retValue = box.getTimeScale();
    std::cout << "Method getTimeScale returned value: " << retValue << std::endl;
    
    // Verify that the returned value matches the expected value of zero
    EXPECT_EQ(retValue, expectedTimeScale);
    
    std::cout << "Exiting Verify_getTimeScale_Returns_Zero test" << std::endl;
}
/**
 * @brief Verify that the setTimeScale method handles typical valid input.
 *
 * This test validates that the MdhdIsoBmffBox object correctly updates its timeScale value when provided with a typical valid input (48000), ensuring normal behavior without exceptions.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 124
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result |Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Construct MdhdIsoBmffBox object with an initial timeScale value | initialTimeScale = 100, timeScaleMemory = {0,0,0,0} | Object is constructed without any exception | Should be successful |
 * | 02 | Invoke setTimeScale with a typical positive value 48000 | inputTimeScale = 48000 | Method call completes without throwing an exception | Should Pass |
 */
TEST(player_isobmff_MdhdIsoBmffBox, SetTimeScale_typical) {
    std::cout << "Entering SetTimeScale_typical test" << std::endl;
    
    // Allocate memory to simulate the timeScale location (4 bytes for uint32_t)
    uint8_t timeScaleMemory[4] = {0};
    
    // Construct the object with an initial timeScale value (using a custom constructor)
    EXPECT_NO_THROW({
        player_isobmff::MdhdIsoBmffBox mdhdBox(100, 100, timeScaleMemory);
        std::cout << "Constructed MdhdIsoBmffBox object with initial timeScale value 100." << std::endl;
        
        // Invoke setTimeScale with typical positive value 48000
        std::cout << "Invoking setTimeScale with value: 48000" << std::endl;
        EXPECT_NO_THROW({
            mdhdBox.setTimeScale(48000);
        });
        
    });
    
    std::cout << "Exiting SetTimeScale_typical test" << std::endl;
}
/**
 * @brief Validate that setTimeScale correctly handles the edge case value of zero.
 *
 * This test ensures that the setTimeScale method of the MdhdIsoBmffBox class correctly processes an input value of 0 without throwing any exceptions, thereby confirming proper handling of edge cases for time scale modifications.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 125@n
 * **Priority:** High@n
 * @n
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * @n
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                         | Test Data                                                              | Expected Result                                                 | Notes             |
 * | :--------------: | ------------------------------------------------------------------- | ---------------------------------------------------------------------- | --------------------------------------------------------------- | ----------------- |
 * | 01               | Construct MdhdIsoBmffBox object with initial timeScale value 12345    | input1 = initial timeScale value (12345), input2 = pointer to timeScaleMemory (all bytes 0) | Object is constructed without throwing any exceptions          | Should be successful |
 * | 02               | Invoke setTimeScale with edge value 0                                 | input1 = value passed to setTimeScale (0)                              | setTimeScale executes without throwing exceptions and processes the value correctly | Should Pass       |
 */
TEST(player_isobmff_MdhdIsoBmffBox, SetTimeScale_edge_zero) {
    std::cout << "Entering SetTimeScale_edge_zero test" << std::endl;
    
    // Allocate memory to simulate the timeScale location (4 bytes for uint32_t)
    uint8_t timeScaleMemory[4] = {0};
    
    // Construct the object with an initial timeScale value
    EXPECT_NO_THROW({
        player_isobmff::MdhdIsoBmffBox mdhdBox(100, 12345, timeScaleMemory);
        std::cout << "Constructed MdhdIsoBmffBox object with initial timeScale value 12345." << std::endl;
        
        // Invoke setTimeScale with edge case value 0
        std::cout << "Invoking setTimeScale with value: 0" << std::endl;
        EXPECT_NO_THROW({
            mdhdBox.setTimeScale(0);
        });
        
    });
    
    std::cout << "Exiting SetTimeScale_edge_zero test" << std::endl;
}
/**
 * @brief Validate the proper instantiation of MvhdIsoBmffBox with a valid FullIsoBmffBox and associated parameters
 *
 * This test verifies that invoking the MvhdIsoBmffBox constructor with a properly constructed FullIsoBmffBox, a valid timescale (tScale), and an allocated memory location (tScale_loc) does not throw an exception. It ensures that the API correctly initializes the object using valid input arguments.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 126@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- | -------------- | ----- |
 * | 01 | Construct FullIsoBmffBox and allocate memory for timescale location | sz = 100, btype = mvhd, ver = 1, f = 0x00; tScale = 1000; tScale_loc = pointer to 4 bytes allocated and initialized to 0 | FullIsoBmffBox is constructed successfully and memory is allocated with proper initialization | Should be successful |
 * | 02 | Invoke MvhdIsoBmffBox constructor and validate no exception is thrown | fbox = FullIsoBmffBox(100, "mvhd", 1, 0x00), tScale = 1000, tScale_loc = pointer to allocated memory | Object is created successfully without throwing any exceptions (EXPECT_NO_THROW passes) | Should Pass |
 * | 03 | Clean up allocated memory for timescale location | delete[] tScale_loc | Memory is deallocated successfully | Should be successful |
 */
TEST(player_isobmff_MvhdIsoBmffBox, ValidCreation_WithProperFullIsoBmffBox) {
    std::cout << "Entering ValidCreation_WithProperFullIsoBmffBox test" << std::endl;

    // Construct fbox with (sz=100, btype="mvhd", ver=1, f=0x00)
    char btype[4];
    strncpy(btype, "mvhd", 4);
    FullIsoBmffBox fbox(100, btype, 1, 0x00);

    uint32_t tScale = 1000;
    // Allocate memory for tScale_loc (4 bytes to store a uint32_t)
    uint8_t* tScale_loc = new uint8_t[4];
    // Initialize the allocated memory to zero
    std::memset(tScale_loc, 0, 4);

    std::cout << "Invoking MvhdIsoBmffBox constructor with fbox(100, \"mvhd\", 1, 0x00), tScale = " 
              << tScale << ", tScale_loc = " << static_cast<void*>(tScale_loc) << std::endl;

    EXPECT_NO_THROW({
        player_isobmff::MvhdIsoBmffBox obj(fbox, tScale, tScale_loc);
        std::cout << "MvhdIsoBmffBox object created successfully." << std::endl;
    });

    delete[] tScale_loc;
    std::cout << "Exiting ValidCreation_WithProperFullIsoBmffBox test" << std::endl;
}
/**
 * @brief Verify the creation of MvhdIsoBmffBox with a zero time scale.
 *
 * This test verifies that the MvhdIsoBmffBox constructor can successfully create an object when provided with a FullIsoBmffBox,
 * a time scale value of zero, and a valid pointer (initialized with zeros). The test ensures that the constructor does not throw
 * any exceptions under these conditions.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 127
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                                                                 | Test Data                                                                                                 | Expected Result                                                                               | Notes       |
 * | :--------------: | ------------------------------------------------------------------------------------------------------------------------------------------- | --------------------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------------------------------- | ----------- |
 * | 01               | Invoke the MvhdIsoBmffBox constructor with a valid FullIsoBmffBox object, a time scale (tScale) value of zero, and a tScale_loc pointer initialized to zeros. | fbox = FullIsoBmffBox(100, "mvhd", 1, 0x00), tScale = 0, tScale_loc = pointer to an array of 4 zeros | The object should be created successfully without throwing any exception (EXPECT_NO_THROW passes). | Should Pass |
 */
TEST(player_isobmff_MvhdIsoBmffBox, ValidCreation_WithZeroTimeScale) {
    std::cout << "Entering ValidCreation_WithZeroTimeScale test" << std::endl;

    char btype[4];
    strncpy(btype, "mvhd", 4);
    FullIsoBmffBox fbox(100, btype, 1, 0x00);

    uint32_t tScale = 0;
    uint8_t* tScale_loc = new uint8_t[4];
    std::memset(tScale_loc, 0, 4);

    std::cout << "Invoking MvhdIsoBmffBox constructor with fbox(100, \"mvhd\", 1, 0x00), tScale = " 
              << tScale << ", tScale_loc = " << static_cast<void*>(tScale_loc) << std::endl;

    EXPECT_NO_THROW({
        player_isobmff::MvhdIsoBmffBox obj(fbox, tScale, tScale_loc);
        std::cout << "MvhdIsoBmffBox object created successfully." << std::endl;

    });

    delete[] tScale_loc;
    std::cout << "Exiting ValidCreation_WithZeroTimeScale test" << std::endl;
}
/**
 * @brief Test to verify that the MvhdIsoBmffBox constructor properly handles a null tScale_loc argument
 *
 * This test case verifies that passing a null pointer for the tScale_loc parameter in the MvhdIsoBmffBox constructor
 * causes the expected exception to be thrown. It ensures that the API is robust against the invalid null parameter,
 * thus preserving the integrity of the object's creation.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 128@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                               | Test Data                                                                                         | Expected Result                          | Notes      |
 * | :--------------: | ------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------- | ---------------------------------------- | ---------- |
 * | 01               | Invoke the MvhdIsoBmffBox constructor with null tScale_loc parameter      | fbox = (size=100, btype="mvhd", version=1, flag=0x00), tScale = 1000, tScale_loc = nullptr           | Exception is thrown as expected by EXPECT_ANY_THROW | Should Fail |
 */
TEST(player_isobmff_MvhdIsoBmffBox, NegativeCreation_WithNulltScaleLoc) {
    std::cout << "Entering NegativeCreation_WithNulltScaleLoc test" << std::endl;

    char btype[4];
    strncpy(btype, "mvhd", 4);
    FullIsoBmffBox fbox(100, btype, 1, 0x00);

    uint32_t tScale = 1000;
    uint8_t* tScale_loc = nullptr;

    std::cout << "Invoking MvhdIsoBmffBox constructor with fbox(100, \"mvhd\", 1, 0x00), tScale = " 
              << tScale << ", tScale_loc = " << tScale_loc << std::endl;

    EXPECT_ANY_THROW({
        player_isobmff::MvhdIsoBmffBox obj(fbox, tScale, tScale_loc);
    });

    std::cout << "Exiting NegativeCreation_WithNulltScaleLoc test" << std::endl;
}
/**
 * @brief Verify that the MvhdIsoBmffBox object is constructed successfully with valid parameters.
 *
 * This test verifies that when the MvhdIsoBmffBox constructor is invoked with a valid size (sz = 32), a valid time scale (tScale = 1000),
 * and a valid uint8_t buffer containing the string "ABCD", no exception is thrown. It ensures that the API behaves as expected for valid inputs.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 129
 * **Priority:** High
 * 
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 * 
 * **Test Procedure:**
 * | Variation / Step | Description                                                                 | Test Data                                             | Expected Result                                                              | Notes       |
 * | :--------------: | --------------------------------------------------------------------------- | ----------------------------------------------------- | --------------------------------------------------------------------------- | ----------- |
 * | 01               | Initialize valid parameters: set sz to 32, tScale to 1000, and fill valid_buffer with "ABCD". | sz = 32, tScale = 1000, valid_buffer = "ABCD"          | Input parameters are correctly initialized.                                  | Should be successful |
 * | 02               | Invoke the constructor of MvhdIsoBmffBox with the initialized parameters and verify no exception is thrown.     | sz = 32, tScale = 1000, valid_buffer pointer referencing "ABCD" | MvhdIsoBmffBox object is created successfully without throwing any exceptions. | Should Pass |
 */
TEST(player_isobmff_MvhdIsoBmffBox, ConstructWithValidParameters) {
    std::cout << "Entering ConstructWithValidParameters test" << std::endl;
    uint32_t sz = 32;
    uint32_t tScale = 1000;
    uint8_t valid_buffer[4];
    // Initialize buffer with some data using strncpy equivalent for uint8_t array.
    const char * initStr = "ABCD";
    std::strncpy(reinterpret_cast<char*>(valid_buffer), initStr, sizeof(valid_buffer) - 1);
    valid_buffer[sizeof(valid_buffer) - 1] = '\0';
    std::cout << "Invoking MvhdIsoBmffBox constructor with sz: " << sz 
              << ", tScale: " << tScale 
              << ", and tScale_loc address: " << static_cast<void*>(valid_buffer) 
              << std::endl;
    EXPECT_NO_THROW({
        player_isobmff::MvhdIsoBmffBox obj(sz, tScale, valid_buffer);
        std::cout << "MvhdIsoBmffBox object created successfully with valid parameters." << std::endl;
    });
    std::cout << "Exiting ConstructWithValidParameters test" << std::endl;
}
/**
 * @brief To verify that the MvhdIsoBmffBox constructor throws an exception when provided with a null pointer for tScale_loc.
 *
 * This test checks whether the MvhdIsoBmffBox constructor correctly handles an invalid input by throwing an exception when a null pointer is passed for the tScale_loc parameter. A null pointer input is used to simulate an error scenario, ensuring that the object is not created.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 130@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                                                   | Test Data                               | Expected Result                                                       | Notes        |
 * | :--------------: | --------------------------------------------------------------------------------------------- | --------------------------------------- | --------------------------------------------------------------------- | ------------ |
 * | 01               | Call MvhdIsoBmffBox constructor with sz=32, tScale=1000, and tScale_loc set to nullptr; expect exception to be thrown | sz = 32, tScale = 1000, tScale_loc = nullptr | Exception is thrown, ensuring that object creation fails with a null pointer | Should Fail  |
 */
TEST(player_isobmff_MvhdIsoBmffBox, ConstructWithNullPointerForTScaleLoc) {
    std::cout << "Entering ConstructWithNullPointerForTScaleLoc test" << std::endl;
    uint32_t sz = 32;
    uint32_t tScale = 1000;
    uint8_t* null_ptr = nullptr;
    std::cout << "Invoking MvhdIsoBmffBox constructor with sz: " << sz 
              << ", tScale: " << tScale 
              << ", and tScale_loc: nullptr" << std::endl;
    EXPECT_ANY_THROW({
        player_isobmff::MvhdIsoBmffBox obj(sz, tScale, null_ptr);
        std::cout << "Error: MvhdIsoBmffBox object should not be created with null tScale_loc pointer." << std::endl;
    });
    std::cout << "Exiting ConstructWithNullPointerForTScaleLoc test" << std::endl;
}
/**
 * @brief Verify that constructing a MvhdIsoBmffBox with a zero time scale triggers an exception.
 *
 * This test verifies that if the time scale parameter (tScale) provided to the MvhdIsoBmffBox constructor is zero,
 * the constructor should throw an exception. This ensures that the box is not created with an invalid
 * time scale value, thereby maintaining the integrity of the isobmff box creation process.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 131
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                        | Test Data                                    | Expected Result                                            | Notes      |
 * | :--------------: | ------------------------------------------------------------------ | -------------------------------------------- | -----------------------------------------------------------| ---------- |
 * | 01               | Invoke MvhdIsoBmffBox constructor with a zero time scale parameter.  | sz = 32, tScale = 0, valid_buffer = "EFGH"     | Exception is thrown indicating invalid time scale value.    | Should Fail|
 */
TEST(player_isobmff_MvhdIsoBmffBox, ConstructWithZeroTimeScale) {
    std::cout << "Entering ConstructWithZeroTimeScale test" << std::endl;
    uint32_t sz = 32;
    uint32_t tScale = 0;
    uint8_t valid_buffer[4];
    const char * initStr = "EFGH";
    std::strncpy(reinterpret_cast<char*>(valid_buffer), initStr, sizeof(valid_buffer) - 1);
    valid_buffer[sizeof(valid_buffer) - 1] = '\0';
    std::cout << "Invoking MvhdIsoBmffBox constructor with sz: " << sz 
              << ", tScale: " << tScale 
              << ", and tScale_loc address: " << static_cast<void*>(valid_buffer) 
              << std::endl;
    EXPECT_ANY_THROW({
        player_isobmff::MvhdIsoBmffBox obj(sz, tScale, valid_buffer);
        std::cout << "Error: MvhdIsoBmffBox object should not be created with zero tScale value." << std::endl;
    });
    std::cout << "Exiting ConstructWithZeroTimeScale test" << std::endl;
}
/**
 * @brief Verifies that the MvhdIsoBmffBox constructor throws an exception when constructed with a zero box size.
 *
 * This test case checks that the MvhdIsoBmffBox constructor correctly handles a zero box size input by throwing an exception.
 * Providing a zero box size is considered an invalid operation, and the constructor is expected to reject this input by raising an exception.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 132@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                               | Test Data                                      | Expected Result                                              | Notes       |
 * | :----:           | ----------------------------------------------------------------------------------------- | ---------------------------------------------- | ------------------------------------------------------------ | ----------- |
 * | 01               | Invoke MvhdIsoBmffBox constructor with zero box size and valid time scale and buffer values. | sz = 0, tScale = 1000, valid_buffer = "IJKL"    | An exception is thrown from the constructor indicating invalid input. | Should Fail |
 */
TEST(player_isobmff_MvhdIsoBmffBox, ConstructWithZeroBoxSize) {
    std::cout << "Entering ConstructWithZeroBoxSize test" << std::endl;
    uint32_t sz = 0;
    uint32_t tScale = 1000;
    uint8_t valid_buffer[4];
    const char * initStr = "IJKL";
    std::strncpy(reinterpret_cast<char*>(valid_buffer), initStr, sizeof(valid_buffer) - 1);
    valid_buffer[sizeof(valid_buffer) - 1] = '\0';
    std::cout << "Invoking MvhdIsoBmffBox constructor with sz: " << sz 
              << ", tScale: " << tScale 
              << ", and tScale_loc address: " << static_cast<void*>(valid_buffer) 
              << std::endl;
    EXPECT_ANY_THROW({
        player_isobmff::MvhdIsoBmffBox obj(sz, tScale, valid_buffer);
        std::cout << "Error: MvhdIsoBmffBox object should not be created with zero box size." << std::endl;
    });
    std::cout << "Exiting ConstructWithZeroBoxSize test" << std::endl;
}
/**
 * @brief Validate successful construction of MvhdIsoBmffBox using valid inputs
 *
 * This test verifies that the static method constructMvhdBox correctly constructs an MvhdIsoBmffBox object when provided with a valid box size and a properly initialized buffer filled with dummy non-zero values. The test ensures that no exception is thrown during object creation and that the returned pointer is non-null, confirming proper construction.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 133
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                     | Test Data                                                                | Expected Result                                                                 | Notes         |
 * | :--------------: | ----------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------ | ------------------------------------------------------------------------------- | ------------- |
 * | 01               | Initialize test variables with box size and a buffer filled with dummy non-zero values            | sz = 32, buffer = an array of 32 bytes filled with 0xAB                    | Variables are initialized correctly                                             | Should be successful |
 * | 02               | Invoke constructMvhdBox API with the provided inputs and ensure no exception is thrown           | input: sz = 32, buffer = [0xAB,0xAB,...] (32 values), output: pointer object | API invocation does not throw and returns a non-null pointer                    | Should Pass   |
 * | 03               | Validate that the returned pointer from constructMvhdBox is not null using EXPECT_NE             | output: pointer returned from constructMvhdBox                           | Returned pointer is non-null as asserted by the test                             | Should Pass   |
 */
TEST(player_isobmff_MvhdIsoBmffBox, ValidConstruct)
{
    std::cout << "Entering ValidConstruct test" << std::endl;
    
    // Setup test input: a typical valid box size and a valid buffer
    uint32_t sz = 32;
    uint8_t buffer[32];
    // Prepare a dummy valid MVHD box buffer (for testing only, fill with non-zero values)
    std::memset(buffer, 0xAB, sizeof(buffer));
    
    std::cout << "Invoking constructMvhdBox with sz = " << sz 
              << " and ptr = " << static_cast<void*>(buffer) << std::endl;
    
    // Invoking the static method constructMvhdBox and ensuring no throw occurs.
    player_isobmff::MvhdIsoBmffBox* mvhdBox = nullptr;
    EXPECT_NO_THROW(mvhdBox = player_isobmff::MvhdIsoBmffBox::constructMvhdBox(sz, buffer));
    
    std::cout << "Method constructMvhdBox returned pointer = " << mvhdBox << std::endl;
    
    // Validate that a non-null pointer is returned.
    EXPECT_NE(mvhdBox, nullptr);
    std::cout << "Exiting ValidConstruct test" << std::endl;
}
/**
 * @brief Verify that constructMvhdBox returns nullptr when a null pointer is provided as buffer input
 *
 * This test verifies that when a valid box size is provided along with a null pointer for the buffer, the constructMvhdBox API
 * returns a nullptr, indicating a failure to construct the MvhdIsoBmffBox. This behavior confirms that the function appropriately
 * handles invalid memory input scenarios.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 134
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                          | Test Data                             | Expected Result                                                                                       | Notes           |
 * | :----:           | ---------------------------------------------------------------------------------------------------- | ------------------------------------- | ----------------------------------------------------------------------------------------------------- | --------------- |
 * | 01               | Setup test input by defining a valid box size and a null pointer, then log these values               | sz = 32, ptr = nullptr                | Inputs are set correctly without error                                                               | Should be successful |
 * | 02               | Invoke player_isobmff::MvhdIsoBmffBox::constructMvhdBox with the test inputs and capture the output pointer | input: sz = 32, ptr = nullptr; output: mvhdBox          | The API call should not throw an exception and should return a pointer value (expected nullptr here) | Should Pass     |
 * | 03               | Validate that the returned pointer is nullptr, indicating failure in constructing the box            | output: mvhdBox should be nullptr      | The API is expected to return nullptr when provided a null pointer as buffer input                    | Should Pass     |
 * | 04               | Log exit message indicating the end of the test case                                                | None                                 | Logging completes successfully; no change in test result                                            | Should be successful |
 */
TEST(player_isobmff_MvhdIsoBmffBox, NullPointerConstruct)
{
    std::cout << "Entering NullPointerConstruct test" << std::endl;
    
    // Setup test input: valid box size but null pointer for buffer
    uint32_t sz = 32;
    uint8_t* ptr = nullptr;
    
    std::cout << "Invoking constructMvhdBox with sz = " << sz 
              << " and ptr = " << static_cast<void*>(ptr) << std::endl;
    
    player_isobmff::MvhdIsoBmffBox* mvhdBox = nullptr;
    EXPECT_NO_THROW(mvhdBox = player_isobmff::MvhdIsoBmffBox::constructMvhdBox(sz, ptr));
    
    std::cout << "Method constructMvhdBox returned pointer = " << mvhdBox << std::endl;
    
    // Validate that the method returns nullptr indicating a failure to construct the box.
    EXPECT_EQ(mvhdBox, nullptr);
    
    std::cout << "Exiting NullPointerConstruct test" << std::endl;
}
/**
 * @brief Verify that the constructMvhdBox API returns a nullptr when given a zero size.
 *
 * This test checks that calling constructMvhdBox with a zero box size and a valid memory buffer does not throw an exception and returns a nullptr. This behavior is expected because a zero size is considered invalid for constructing a MvhdIsoBmffBox.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 135@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**
 * | Variation / Step | Description                                                       | Test Data                                             | Expected Result                                                    | Notes             |
 * | :--------------: | ----------------------------------------------------------------- | ----------------------------------------------------- | ------------------------------------------------------------------- | ----------------- |
 * | 01               | Setup test input by initializing a zero box size and a memory buffer | sz = 0, buffer = 32 bytes initialized with 0xCD       | Memory buffer is correctly initialized; variable sz equals 0       | Should be successful |
 * | 02               | Invoke constructMvhdBox with the prepared input                     | input: sz = 0, buffer pointer; output: mvhdBox pointer  | No exception thrown; function is called without error              | Should Pass       |
 * | 03               | Validate that the API returns a nullptr due to invalid box size      | mvhdBox pointer                                       | Assertion: mvhdBox == nullptr                                        | Should Pass       |
 */
TEST(player_isobmff_MvhdIsoBmffBox, ZeroSizeConstruct)
{
    std::cout << "Entering ZeroSizeConstruct test" << std::endl;
    
    // Setup test input: zero box size and a valid memory buffer.
    uint32_t sz = 0;
    uint8_t buffer[32];
    std::memset(buffer, 0xCD, sizeof(buffer));
    
    std::cout << "Invoking constructMvhdBox with sz = " << sz 
              << " and ptr = " << static_cast<void*>(buffer) << std::endl;
    
    player_isobmff::MvhdIsoBmffBox* mvhdBox = nullptr;
    EXPECT_NO_THROW(mvhdBox = player_isobmff::MvhdIsoBmffBox::constructMvhdBox(sz, buffer));
    
    std::cout << "Method constructMvhdBox returned pointer = " << mvhdBox << std::endl;
    
    // Validate that the method returns nullptr due to invalid box size.
    EXPECT_EQ(mvhdBox, nullptr);
    
    std::cout << "Exiting ZeroSizeConstruct test" << std::endl;
}
/**
 * @brief Validate that getTimeScale() returns the correct time scale for a valid input.
 *
 * Validate that the getTimeScale() method of the MvhdIsoBmffBox correctly returns the expected timeScale value when constructed with valid inputs. This test verifies that, given a valid size, timeScale value, and character buffer, the implementation behaves as expected.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 136@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Instantiate MvhdIsoBmffBox with size = 50, expectedTimeScale = 1000, and tScaleBuffer set to "valid"; invoke getTimeScale() and verify returned value matches expectedTimeScale | size = 50, expectedTimeScale = 1000, tScaleBuffer = "valid" | getTimeScale() returns 1000 and EXPECT_EQ(result, expectedTimeScale) passes | Should Pass |
 */
TEST(player_isobmff_MvhdIsoBmffBox, getTimeScale_NormalPositive) {
    std::cout << "Entering getTimeScale_NormalPositive test" << std::endl;
    
    uint32_t size = 50; // Arbitrary size for the box
    uint32_t expectedTimeScale = 1000; 
    uint8_t tScaleBuffer[10];
    // Use strncpy to assign a test string to the fixed-size array tScaleBuffer
    std::strncpy(reinterpret_cast<char*>(tScaleBuffer), "valid", sizeof(tScaleBuffer));
    std::cout << "Creating object with size: " << size 
              << ", timeScale: " << expectedTimeScale 
              << ", tScale_loc: " << static_cast<void*>(tScaleBuffer) << std::endl;
    
    EXPECT_NO_THROW({
        player_isobmff::MvhdIsoBmffBox box(size, expectedTimeScale, tScaleBuffer);
        std::cout << "Invoking getTimeScale() method" << std::endl;
        uint32_t result = box.getTimeScale();
        std::cout << "getTimeScale() returned value: " << result << std::endl;
        EXPECT_EQ(result, expectedTimeScale);
    });
    
    std::cout << "Exiting getTimeScale_NormalPositive test" << std::endl;
}
/**
 * @brief Test that getTimeScale returns zero when initialized with a zero value.
 *
 * This test ensures that the getTimeScale method of the MvhdIsoBmffBox class correctly returns the timeScale value provided during object creation, even when the timeScale is zero. It verifies that no exceptions are thrown during object initialization and method invocation, and that the method returns the expected value.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 137
 * **Priority:** High
 * 
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 * 
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                  | Test Data                                               | Expected Result                                                               | Notes          |
 * | :--------------: | -------------------------------------------------------------------------------------------- | ------------------------------------------------------- | ---------------------------------------------------------------------------- | -------------- |
 * | 01               | Initialize an MvhdIsoBmffBox object with a specified size, a zero timeScale, and tScaleBuffer. | size = 50, expectedTimeScale = 0, tScaleBuffer = "valid"  | Object is created without throwing any exception                             | Should be successful |
 * | 02               | Invoke getTimeScale() on the created object and verify the return value is zero.               | API Call: box.getTimeScale(), expected return = 0       | getTimeScale() returns 0 and the assertion EXPECT_EQ confirms the correct value | Should Pass    |
 */
TEST(player_isobmff_MvhdIsoBmffBox, getTimeScale_ZeroValue) {
    std::cout << "Entering getTimeScale_ZeroValue test" << std::endl;
    
    uint32_t size = 50; // Arbitrary size for the box
    uint32_t expectedTimeScale = 0; 
    uint8_t tScaleBuffer[10];
    // Use strncpy to assign a test string to the fixed-size array tScaleBuffer
    std::strncpy(reinterpret_cast<char*>(tScaleBuffer), "valid", sizeof(tScaleBuffer));
    std::cout << "Creating object with size: " << size 
              << ", timeScale: " << expectedTimeScale 
              << ", tScale_loc: " << static_cast<void*>(tScaleBuffer) << std::endl;
              
    EXPECT_NO_THROW({
        player_isobmff::MvhdIsoBmffBox box(size, expectedTimeScale, tScaleBuffer);
        std::cout << "Invoking getTimeScale() method" << std::endl;
        uint32_t result = box.getTimeScale();
        std::cout << "getTimeScale() returned value: " << result << std::endl;
        EXPECT_EQ(result, expectedTimeScale);
    });
    
    std::cout << "Exiting getTimeScale_ZeroValue test" << std::endl;
}
/**
 * @brief Verify that the setTimeScale API correctly updates the time scale of the MvhdIsoBmffBox object
 *
 * This test verifies that the setTimeScale method of the MvhdIsoBmffBox class works correctly when provided with a typical time scale value of 1000. It ensures that the object is correctly instantiated with an initial time scale of 0 and that the setTimeScale method successfully updates the internal time scale without throwing any exceptions.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 138@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create the MvhdIsoBmffBox object using the custom constructor with an initial time scale of 0 | input: capacity = 32, initialTimeScale = 0, timeScaleStorage = [0,0,0,0] | Object is created successfully without throwing an exception | Should be successful |
 * | 02 | Invoke the setTimeScale method with a typical value of 1000 to update the time scale | input: timeScale = 1000 | setTimeScale completes without throwing an exception and updates the time scale accordingly | Should Pass |
 */
TEST(player_isobmff_MvhdIsoBmffBox, SetTimeScaleWithTypicalValue) {
    std::cout << "Entering SetTimeScaleWithTypicalValue test" << std::endl;
    
    // Allocate fixed size array for timeScale storage
    uint8_t timeScaleStorage[sizeof(uint32_t)] = {0};
    
    // Create object using custom constructor with initial timeScale = 0
    EXPECT_NO_THROW({
        player_isobmff::MvhdIsoBmffBox box(32, 0, timeScaleStorage);
        std::cout << "Created MvhdIsoBmffBox object with initial timeScale = 0" << std::endl;
        
        // Set timeScale to typical value 1000
        uint32_t inputValue = 1000;
        std::cout << "Invoking setTimeScale with value: " << inputValue << std::endl;
        EXPECT_NO_THROW(box.setTimeScale(inputValue));
    });
    
    std::cout << "Exiting SetTimeScaleWithTypicalValue test" << std::endl;
}
/**
 * @brief Test to verify setting timeScale to its minimum boundary value.
 *
 * This test validates that the MvhdIsoBmffBox object can successfully handle setting the
 * timeScale to the minimum boundary value (0). The test first creates the object with a non-zero
 * initial timeScale and then sets the timeScale to 0, ensuring no exceptions are thrown during the process.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 139@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                 | Test Data                                                             | Expected Result                                             | Notes           |
 * | :--------------: | ------------------------------------------------------------------------------------------- | --------------------------------------------------------------------- | ----------------------------------------------------------- | --------------- |
 * | 01               | Create a MvhdIsoBmffBox instance with initial timeScale set to 1234 and a pre-allocated array | size = 32, initial_timeScale = 1234, timeScaleStorage = allocated array | Object is created successfully with no exception thrown     | Should be successful |
 * | 02               | Call setTimeScale() with the minimum boundary value (0)                                       | inputValue = 0                                                        | API call completes with no exception thrown                 | Should Pass     |
 */
TEST(player_isobmff_MvhdIsoBmffBox, SetTimeScaleWithMinimumBoundary) {
    std::cout << "Entering SetTimeScaleWithMinimumBoundary test" << std::endl;
    
    // Allocate fixed size array for timeScale storage
    uint8_t timeScaleStorage[sizeof(uint32_t)] = {0};
    
    // Create object using custom constructor with initial timeScale = 1234 (arbitrary non-zero)
    EXPECT_NO_THROW({
        player_isobmff::MvhdIsoBmffBox box(32, 1234, timeScaleStorage);
        std::cout << "Created MvhdIsoBmffBox object with initial timeScale = 1234" << std::endl;
        
        // Set timeScale to minimum boundary value 0
        uint32_t inputValue = 0;
        std::cout << "Invoking setTimeScale with value: " << inputValue << std::endl;
        EXPECT_NO_THROW(box.setTimeScale(inputValue));
    });
    
    std::cout << "Exiting SetTimeScaleWithMinimumBoundary test" << std::endl;
}
/**
 * @brief ConstructPrftIsoBmffBox with typical valid parameters to test its proper creation
 *
 * This test verifies that the PrftIsoBmffBox constructor properly initializes an object when provided with a valid FullIsoBmffBox object and typical constructor parameters (trackId, ntpTs, mediaTime). It ensures that no exceptions are thrown during object creation.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 140@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Prepare FullIsoBmffBox parameters (sz, btype, ver, f) and create the FullIsoBmffBox object | sz = 100, btype = "abcd", ver = 1, f = 0 | FullIsoBmffBox object is created successfully | Should be successful |
 * | 02 | Prepare parameters for PrftIsoBmffBox constructor (trackId, ntpTs, mediaTime) | trackId = 1, ntpTs = 1000, mediaTime = 500 | Parameters are prepared correctly | Should be successful |
 * | 03 | Invoke PrftIsoBmffBox constructor using created FullIsoBmffBox and provided parameters | FullIsoBmffBox object, trackId = 1, ntpTs = 1000, mediaTime = 500 | Object creation does not throw exception and object's state is valid | Should Pass |
 */
TEST(player_isobmff_PrftIsoBmffBox, ConstructPrftIsoBmffBoxTypical) {
    std::cout << "Entering ConstructPrftIsoBmffBoxTypical test" << std::endl;
    
    // Prepare parameters for FullIsoBmffBox
    uint32_t sz = 100;
    char btype[4];
    // Using strncpy to assign value to fixed size array
    std::cout << "Invoking strncpy to assign 'abcd' to btype array" << std::endl;
    std::strncpy(btype, "abcd", sizeof(btype));
    uint8_t ver = 1;
    uint32_t f = 0;
    std::cout << "Constructing FullIsoBmffBox with sz=" << sz 
              << ", btype=" << std::string(btype, sizeof(btype))
              << ", ver=" << static_cast<uint32_t>(ver)
              << ", f=" << f << std::endl;
    
    // Create FullIsoBmffBox object (using its custom constructor)
    FullIsoBmffBox fullBox(sz, btype, ver, f);
    
    // Parameters for PrftIsoBmffBox constructor
    uint32_t trackId = 1;
    uint64_t ntpTs = 1000;
    uint64_t mediaTime = 500;
    std::cout << "Parameters for PrftIsoBmffBox constructor: "
              << "trackId=" << trackId << ", ntpTs=" << ntpTs 
              << ", mediaTime=" << mediaTime << std::endl;
    
    // Invoke the PrftIsoBmffBox constructor using the FullIsoBmffBox object and provided parameters
    EXPECT_NO_THROW({
        std::cout << "Invoking PrftIsoBmffBox(FullIsoBmffBox&, uint32_t, uint64_t, uint64_t) constructor" << std::endl;
        player_isobmff::PrftIsoBmffBox prftBox(fullBox, trackId, ntpTs, mediaTime);
        std::cout << "PrftIsoBmffBox object created successfully." << std::endl;
    });
    
    std::cout << "Exiting ConstructPrftIsoBmffBoxTypical test" << std::endl;
}
/**
 * @brief Verify that an instance of PrftIsoBmffBox is created successfully with typical valid values.
 *
 * This test verifies that the PrftIsoBmffBox constructor, when invoked with typical valid input values,
 * does not throw an exception and creates an object instance as expected. This is crucial to ensure that
 * the basic instantiation functionality of the class works correctly under nominal conditions.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 141@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                  | Test Data                                                       | Expected Result                                                                                        | Notes      |
 * | :--------------: | ---------------------------------------------------------------------------- | --------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------ | ---------- |
 * | 01               | Invoke the PrftIsoBmffBox constructor with typical valid input values          | sz = 100, trackId = 1, ntpTs = 123456789, mediaTime = 987654321   | The constructor executes without throwing an exception and an instance is created successfully.        | Should Pass |
 */
TEST(player_isobmff_PrftIsoBmffBox, CreateInstanceWithTypicalValidValues) {
    std::cout << "Entering CreateInstanceWithTypicalValidValues test" << std::endl;
    uint32_t sz = 100;
    uint32_t trackId = 1;
    uint64_t ntpTs = 123456789;
    uint64_t mediaTime = 987654321;
    std::cout << "Invoking player_isobmff::PrftIsoBmffBox::PrftIsoBmffBox(" 
              << sz << ", " << trackId << ", " << ntpTs << ", " << mediaTime << ")" << std::endl;
    
    EXPECT_NO_THROW({
        player_isobmff::PrftIsoBmffBox box(sz, trackId, ntpTs, mediaTime);
        std::cout << "Instance created." << std::endl;
    });
    std::cout << "Exiting CreateInstanceWithTypicalValidValues test" << std::endl;
}
/**
 * @brief Verify PrftIsoBmffBox instance creation with sz = 0.
 *
 * This test verifies that constructing a PrftIsoBmffBox object with a size value of 0 does not throw an exception. 
 * It confirms that the object's internal fields (track_id, ntp_ts, media_time) are correctly initialized with the provided values.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 142
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoke PrftIsoBmffBox constructor with parameters sz=0, trackId=5, ntpTs=100, mediaTime=200 | sz = 0, trackId = 5, ntpTs = 100, mediaTime = 200 | No exception thrown and object's internal state (track_id, ntp_ts, media_time) matches expected values | Should Pass |
 */
TEST(player_isobmff_PrftIsoBmffBox, CreateInstanceWithSzZero) {
    std::cout << "Entering CreateInstanceWithSzZero test" << std::endl;
    uint32_t sz = 0;
    uint32_t trackId = 5;
    uint64_t ntpTs = 100;
    uint64_t mediaTime = 200;
    std::cout << "Invoking player_isobmff::PrftIsoBmffBox::PrftIsoBmffBox(" 
              << sz << ", " << trackId << ", " << ntpTs << ", " << mediaTime << ")" << std::endl;
    
    EXPECT_NO_THROW({
        player_isobmff::PrftIsoBmffBox box(sz, trackId, ntpTs, mediaTime);
        std::cout << "Instance created with sz = " << sz << "." << std::endl;
        std::cout << "Retrieved internal values: track_id = " << box.track_id
                  << ", ntp_ts = " << box.ntp_ts
                  << ", media_time = " << box.media_time << std::endl;
        EXPECT_EQ(sz, 0u);
        EXPECT_EQ(box.track_id, trackId);
        EXPECT_EQ(box.ntp_ts, ntpTs);
        EXPECT_EQ(box.media_time, mediaTime);
    });
    std::cout << "Exiting CreateInstanceWithSzZero test" << std::endl;
}
/**
 * @brief Test the creation of a PrftIsoBmffBox instance with trackId set to zero.
 *
 * This test verifies that creating an instance of PrftIsoBmffBox with trackId equal to 0 does not throw an exception and that the internal fields (track_id, ntp_ts, media_time) are initialized correctly according to the input parameters.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 143@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Call the PrftIsoBmffBox constructor with parameters: sz=100, trackId=0, ntpTs=100, mediaTime=200 | input: sz=100, trackId=0, ntpTs=100, mediaTime=200 | No exception thrown and object fields correctly set: track_id==0, ntp_ts==100, media_time==200 | Should Pass |
 */
TEST(player_isobmff_PrftIsoBmffBox, CreateInstanceWithTrackIdZero) {
    std::cout << "Entering CreateInstanceWithTrackIdZero test" << std::endl;
    uint32_t sz = 100;
    uint32_t trackId = 0;
    uint64_t ntpTs = 100;
    uint64_t mediaTime = 200;
    std::cout << "Invoking player_isobmff::PrftIsoBmffBox::PrftIsoBmffBox(" 
              << sz << ", " << trackId << ", " << ntpTs << ", " << mediaTime << ")" << std::endl;
    
    EXPECT_NO_THROW({
        player_isobmff::PrftIsoBmffBox box(sz, trackId, ntpTs, mediaTime);
        std::cout << "Instance created with trackId = " << trackId << "." << std::endl;
        std::cout << "Retrieved internal values: track_id = " << box.track_id
                  << ", ntp_ts = " << box.ntp_ts
                  << ", media_time = " << box.media_time << std::endl;
        EXPECT_EQ(box.track_id, trackId);
        EXPECT_EQ(box.ntp_ts, ntpTs);
        EXPECT_EQ(box.media_time, mediaTime);
    });
    std::cout << "Exiting CreateInstanceWithTrackIdZero test" << std::endl;
}
/**
 * @brief Test creation of player_isobmff::PrftIsoBmffBox instance with ntpTs set to zero.
 *
 * This test verifies that setting ntpTs to zero in the constructor of PrftIsoBmffBox does not throw any exceptions and that the object's internal values (track_id, ntp_ts, media_time) are initialized correctly with the provided parameters.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 144@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoke the constructor of PrftIsoBmffBox with sz = 100, trackId = 10, ntpTs = 0, and mediaTime = 200; verify that no exception is thrown and internal values are set accordingly. | sz = 100, trackId = 10, ntpTs = 0, mediaTime = 200 | No exception thrown; track_id equals 10, ntp_ts equals 0, media_time equals 200 | Should Pass |
 */
TEST(player_isobmff_PrftIsoBmffBox, CreateInstanceWithNtpTsZero) {
    std::cout << "Entering CreateInstanceWithNtpTsZero test" << std::endl;
    uint32_t sz = 100;
    uint32_t trackId = 10;
    uint64_t ntpTs = 0;
    uint64_t mediaTime = 200;
    std::cout << "Invoking player_isobmff::PrftIsoBmffBox::PrftIsoBmffBox(" 
              << sz << ", " << trackId << ", " << ntpTs << ", " << mediaTime << ")" << std::endl;
    
    EXPECT_NO_THROW({
        player_isobmff::PrftIsoBmffBox box(sz, trackId, ntpTs, mediaTime);
        std::cout << "Instance created with ntpTs = " << ntpTs << "." << std::endl;
        std::cout << "Retrieved internal values: track_id = " << box.track_id
                  << ", ntp_ts = " << box.ntp_ts
                  << ", media_time = " << box.media_time << std::endl;
        EXPECT_EQ(box.track_id, trackId);
        EXPECT_EQ(box.ntp_ts, ntpTs);
        EXPECT_EQ(box.media_time, mediaTime);
    });
    std::cout << "Exiting CreateInstanceWithNtpTsZero test" << std::endl;
}
/**
 * @brief Verify instance creation of PrftIsoBmffBox with mediaTime set to zero
 *
 * This test verifies that an instance of player_isobmff::PrftIsoBmffBox can be successfully created with the provided parameters where mediaTime is set to zero. The test ensures that no exceptions are thrown and that the internal values of the instance match the expected inputs.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 145@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                                  | Test Data                                                       | Expected Result                                                                  | Notes       |
 * | :--------------: | -----------------------------------------------------------------------------|-----------------------------------------------------------------|----------------------------------------------------------------------------------|-------------|
 * | 01               | Invoke the PrftIsoBmffBox constructor with mediaTime set to 0                 | input: sz = 100, trackId = 10, ntpTs = 100, mediaTime = 0         | Constructor should execute without throwing any exception                      | Should Pass |
 * | 02               | Validate the internal values of the constructed instance                      | output: track_id = 10, ntp_ts = 100, media_time = 0                | Instance's track_id equals 10, ntp_ts equals 100, and media_time equals 0         | Should Pass |
 */
TEST(player_isobmff_PrftIsoBmffBox, CreateInstanceWithMediaTimeZero) {
    std::cout << "Entering CreateInstanceWithMediaTimeZero test" << std::endl;
    uint32_t sz = 100;
    uint32_t trackId = 10;
    uint64_t ntpTs = 100;
    uint64_t mediaTime = 0;
    std::cout << "Invoking player_isobmff::PrftIsoBmffBox::PrftIsoBmffBox(" 
              << sz << ", " << trackId << ", " << ntpTs << ", " << mediaTime << ")" << std::endl;
    
    EXPECT_NO_THROW({
        player_isobmff::PrftIsoBmffBox box(sz, trackId, ntpTs, mediaTime);
        std::cout << "Instance created with mediaTime = " << mediaTime << "." << std::endl;
        std::cout << "Retrieved internal values: track_id = " << box.track_id
                  << ", ntp_ts = " << box.ntp_ts
                  << ", media_time = " << box.media_time << std::endl;
        EXPECT_EQ(box.track_id, trackId);
        EXPECT_EQ(box.ntp_ts, ntpTs);
        EXPECT_EQ(box.media_time, mediaTime);
    });
    std::cout << "Exiting CreateInstanceWithMediaTimeZero test" << std::endl;
}
/**
 * @brief Test the valid construction of an ISO BMFF PRFT box using a well-formed header and data bytes
 *
 * This test verifies that constructPrftBox properly constructs and returns a non-null pointer when supplied 
 * with a valid memory block containing a well-formed box header and the expected data. The test ensures that 
 * the function behaves as expected in a positive scenario.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 146@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                           | Test Data                                          | Expected Result                  | Notes          |
 * | :--------------: | ----------------------------------------------------------------------------------------------------- | -------------------------------------------------- | -------------------------------- | -------------- |
 * | 01               | Prepare a valid memory block with a well-formed box header and required data bytes                    | validData = "ABCDEFGHIJKLMNOP"                      | Memory block prepared            | Should be successful |
 * | 02               | Invoke constructPrftBox with sz = 16 and ptr = validData, and verify that the returned pointer is not null | sz = 16, ptr = validData                             | prftBox != nullptr               | Should Pass    |
 */
TEST(player_isobmff_PrftIsoBmffBox, ValidBoxConstruction) {
    std::cout << "Entering ValidBoxConstruction test" << std::endl;
    
    // Prepare a valid memory block with a well-formed box header and required data bytes
    uint8_t validData[16];
    // Using strncpy to assign test data to the fixed size array.
    strncpy(reinterpret_cast<char*>(validData), "ABCDEFGHIJKLMNOP", sizeof(validData));

    std::cout << "Invoking constructPrftBox with sz = 16 and ptr = " << static_cast<void*>(validData) << std::endl;
    
    // Call the static method to construct the box
    player_isobmff::PrftIsoBmffBox* prftBox = player_isobmff::PrftIsoBmffBox::constructPrftBox(16, validData);
    
    std::cout << "Returned pointer from constructPrftBox: " << prftBox << std::endl;
    
    // Validate that the returned pointer is not null.
    EXPECT_NE(prftBox, nullptr);    
    std::cout << "Exiting ValidBoxConstruction test" << std::endl;
}
/**
 * @brief Validate that constructPrftBox properly handles input with zero box size.
 *
 * This test verifies that calling the constructPrftBox API with a size of zero returns a null pointer.
 * It prepares a valid memory block containing a test string, then invokes the API with the zero size.
 * The expected behavior is that, due to the zero size, the API does not construct a valid box and instead returns a nullptr.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 147@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                                                   | Test Data                                                | Expected Result                                                | Notes         |
 * | :--------------: | --------------------------------------------------------------------------------------------- | -------------------------------------------------------- | -------------------------------------------------------------- | ------------- |
 * | 01               | Prepare a 16-byte memory block and copy "ZeroBoxSizeTest" into the block to simulate valid data.| data[16] = "ZeroBoxSizeTest"                             | Memory block is initialized with the correct test string.      | Should be successful |
 * | 02               | Invoke constructPrftBox API with a size of 0 and the prepared memory block pointer.             | size = 0, ptr = address of data block                    | API returns a nullptr indicating no box was constructed.       | Should Pass   |
 * | 03               | Assert that the returned pointer from constructPrftBox is indeed a nullptr.                     | output prftBox = nullptr                                 | EXPECT_EQ verifies that prftBox equals nullptr.                 | Should Pass   |
 */
TEST(player_isobmff_PrftIsoBmffBox, ZeroBoxSize) {
    std::cout << "Entering ZeroBoxSize test" << std::endl;
    
    // Prepare a valid memory block even though size is zero.
    uint8_t data[16];
    strncpy(reinterpret_cast<char*>(data), "ZeroBoxSizeTest", sizeof(data));
    
    std::cout << "Invoking constructPrftBox with sz = 0 and ptr = " << static_cast<void*>(data) << std::endl;
    
    // Call constructPrftBox with zero size, expecting a null pointer.
    player_isobmff::PrftIsoBmffBox* prftBox = player_isobmff::PrftIsoBmffBox::constructPrftBox(0, data);
    
    std::cout << "Returned pointer from constructPrftBox: " << prftBox << std::endl;
    
    EXPECT_EQ(prftBox, nullptr);
    
    std::cout << "Exiting ZeroBoxSize test" << std::endl;
}
/**
 * @brief Test constructPrftBox with null pointer input
 *
 * This test verifies that constructPrftBox returns a nullptr when called with a valid size (16) and a null pointer. The test ensures that the API handles null pointer inputs gracefully.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 148
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Call constructPrftBox with sz=16 and ptr=nullptr | sz = 16, ptr = nullptr, output = expected nullptr | API returns a nullptr and EXPECT_EQ(prftBox, nullptr) passes | Should Pass |
 */
TEST(player_isobmff_PrftIsoBmffBox, NullPointerInput) {
    std::cout << "Entering NullPointerInput test" << std::endl;
    
    std::cout << "Invoking constructPrftBox with sz = 16 and ptr = nullptr" << std::endl;
    
    // Call constructPrftBox with a null pointer, expecting a null pointer in return.
    player_isobmff::PrftIsoBmffBox* prftBox = player_isobmff::PrftIsoBmffBox::constructPrftBox(16, nullptr);
    
    std::cout << "Returned pointer from constructPrftBox: " << prftBox << std::endl;
    
    EXPECT_EQ(prftBox, nullptr);
    
    std::cout << "Exiting NullPointerInput test" << std::endl;
}
/**
 * @brief Verify that getMediaTime returns the expected media time using the first constructor
 *
 * This test verifies that when a player_isobmff::PrftIsoBmffBox object is created using the first constructor with specified parameters,
 * the getMediaTime() method returns the value that was initially provided during construction. This ensures the correct assignment and retrieval of the media time.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 149@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                 | Test Data                                                            | Expected Result                                                       | Notes           |
 * | :--------------: | ------------------------------------------------------------------------------------------- | -------------------------------------------------------------------- | ---------------------------------------------------------------------- | --------------- |
 * | 01               | Create a player_isobmff::PrftIsoBmffBox using the first constructor with specified parameters | size = 100, trackId = 1, ntpTs = 123456789, mediaTime = 1000            | Object is created without any exceptions                              | Should be successful |
 * | 02               | Invoke getMediaTime() on the created object and verify the returned media time               | Call getMediaTime(), expected mediaTime = 1000                        | Returned media time equals 1000, and EXPECT_EQ check passes             | Should Pass     |
 */
TEST(player_isobmff_PrftIsoBmffBox, GetMediaTime_Using_First_Constructor)
{
    std::cout << "Entering GetMediaTime_Using_First_Constructor test" << std::endl;
    
    // Setup input parameters for the first constructor
    uint32_t size = 100;
    uint32_t trackId = 1;
    uint64_t ntpTs = 123456789;
    uint64_t mediaTime = 1000;
    
    std::cout << "Creating player_isobmff::PrftIsoBmffBox using first constructor with parameters:" 
              << " size = " << size 
              << ", trackId = " << trackId 
              << ", ntpTs = " << ntpTs 
              << ", mediaTime = " << mediaTime << std::endl;
    
    EXPECT_NO_THROW({
        player_isobmff::PrftIsoBmffBox prftBox(size, trackId, ntpTs, mediaTime);
        
        std::cout << "Invoking getMediaTime() on prftBox" << std::endl;
        uint64_t returnedMediaTime = prftBox.getMediaTime();
        std::cout << "getMediaTime() returned: " << returnedMediaTime << std::endl;
        
        EXPECT_EQ(mediaTime, returnedMediaTime);
    });
    
    std::cout << "Exiting GetMediaTime_Using_First_Constructor test" << std::endl;
}
/**
 * @brief Test the getMediaTime() functionality using the second constructor of PrftIsoBmffBox
 *
 * This test verifies that the getMediaTime() method of the player_isobmff::PrftIsoBmffBox object returns the correct media time that was provided during construction. It does so by first creating a FullIsoBmffBox object with valid parameters and then using it to instantiate a PrftIsoBmffBox using its second constructor with additional parameters. The test checks that the output of getMediaTime() matches the expected media time.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 150@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Setup input parameters for FullIsoBmffBox construction | fullBoxSize = 150, btype = "ftyp", version = 0, flags = 0 | Parameters are defined without errors | Should be successful |
 * | 02 | Instantiate FullIsoBmffBox using its constructor | fullBoxSize = 150, btype = "ftyp", version = 0, flags = 0 | FullIsoBmffBox object is successfully created | Should be successful |
 * | 03 | Setup additional parameters for PrftIsoBmffBox second constructor | trackId = 5, ntpTs = 987654321, mediaTime = 2000 | Parameters for PrftIsoBmffBox are ready | Should be successful |
 * | 04 | Instantiate PrftIsoBmffBox using second constructor with a FullIsoBmffBox reference | FullIsoBmffBox reference, trackId = 5, ntpTs = 987654321, mediaTime = 2000 | PrftIsoBmffBox object is successfully created and initialized | Should be successful |
 * | 05 | Invoke getMediaTime() and validate the returned media time | Within object: mediaTime = 2000 | getMediaTime() returns 2000 and passes the EXPECT_EQ check | Should Pass |
 */
TEST(player_isobmff_PrftIsoBmffBox, GetMediaTime_Using_Second_Constructor)
{
    std::cout << "Entering GetMediaTime_Using_Second_Constructor test" << std::endl;
    
    // Setup input parameters for creating a FullIsoBmffBox object
    uint32_t fullBoxSize = 150;
    char btype[4];
    // Using strncpy to assign value to fixed size array
    std::strncpy(btype, "ftyp", 4);
    uint8_t version = 0;
    uint32_t flags = 0;
    
    std::cout << "Creating FullIsoBmffBox using its constructor with parameters:" 
              << " size = " << fullBoxSize 
              << ", btype = " << std::string(btype, 4) 
              << ", version = " << static_cast<uint32_t>(version) 
              << ", flags = " << flags << std::endl;
    
    EXPECT_NO_THROW({
        // Instantiate a FullIsoBmffBox object using the available constructor.
        FullIsoBmffBox fullBox(fullBoxSize, btype, version, flags);
        
        // Setup additional parameters for PrftIsoBmffBox second constructor
        uint32_t trackId = 5;
        uint64_t ntpTs = 987654321;
        uint64_t mediaTime = 2000;
        
        std::cout << "Creating player_isobmff::PrftIsoBmffBox using second constructor with parameters:"
                  << " FullIsoBmffBox reference, trackId = " << trackId 
                  << ", ntpTs = " << ntpTs 
                  << ", mediaTime = " << mediaTime << std::endl;
        
        player_isobmff::PrftIsoBmffBox prftBox(fullBox, trackId, ntpTs, mediaTime);
        
        std::cout << "Invoking getMediaTime() on prftBox" << std::endl;
        uint64_t returnedMediaTime = prftBox.getMediaTime();
        std::cout << "getMediaTime() returned: " << returnedMediaTime << std::endl;
        
        EXPECT_EQ(mediaTime, returnedMediaTime);
    });
    
    std::cout << "Exiting GetMediaTime_Using_Second_Constructor test" << std::endl;
}
/**
 * @brief Verify that PrftIsoBmffBox returns correct ntpTs value when created using the first constructor.
 *
 * This test creates a PrftIsoBmffBox object using the first constructor with predefined values and tests that the
 * getNtpTs() method returns the ntpTs value initialized during construction. This ensures that the constructor properly
 * assigns the ntpTs value and that the getter correctly retrieves it.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 151@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data |Expected Result |Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01| Create object using first constructor with size=100, trackId=10, ntpTs=123456789, mediaTime=500. | size = 100, trackId = 10, ntpTs = 123456789, mediaTime = 500 | Object creation should not throw an exception and parameters should be set correctly. | Should Pass |
 * | 02| Invoke getNtpTs() to retrieve the ntpTs value. | output: getNtpTs() | Returns 123456789 as verified by EXPECT_EQ assertion. | Should Pass |
 */
TEST(player_isobmff_PrftIsoBmffBox, getNtpTs_positive_first_constructor) {
    std::cout << "Entering getNtpTs_positive_first_constructor test" << std::endl;
    
    // Using first constructor: PrftIsoBmffBox(uint32_t sz, uint32_t trackId, uint64_t ntpTs, uint64_t mediaTime)
    EXPECT_NO_THROW({
        // Create an object with ntpTs = 123456789, trackId = 10, mediaTime = 500, and size = 100.
        player_isobmff::PrftIsoBmffBox box(100, 10, 123456789ULL, 500ULL);
        std::cout << "Created PrftIsoBmffBox using first constructor with parameters:" << std::endl;
        std::cout << "  size = " << 100 << ", trackId = " << 10 
                  << ", ntpTs = " << 123456789ULL << ", mediaTime = " << 500ULL << std::endl;
        
        std::cout << "Invoking getNtpTs()" << std::endl;
        uint64_t retNtpTs = box.getNtpTs();
        std::cout << "Returned ntpTs value: " << retNtpTs << std::endl;
        
        EXPECT_EQ(retNtpTs, 123456789ULL);
    });
    
    std::cout << "Exiting getNtpTs_positive_first_constructor test" << std::endl;
}
/**
 * @brief Positive test case for getNtpTs using the second constructor of PrftIsoBmffBox.
 *
 * This test verifies that the getNtpTs method returns the correct ntpTs value when a PrftIsoBmffBox object
 * is constructed using the second constructor with valid parameters. It first creates a FullIsoBmffBox, then uses
 * it to create a PrftIsoBmffBox, and finally asserts that the returned ntpTs value is as expected.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 152@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create FullIsoBmffBox using size=200, btype="prft", version=1, flags=0; then create PrftIsoBmffBox using trackId=20, ntpTs=987654321, mediaTime=600; invoke getNtpTs() to obtain the ntpTs value. | FullIsoBmffBox: size=200, btype="prft", version=1, flags=0; PrftIsoBmffBox: trackId=20, ntpTs=987654321, mediaTime=600; getNtpTs() return value expected=987654321 | The getNtpTs method should return 987654321. | Should Pass |
 */
TEST(player_isobmff_PrftIsoBmffBox, getNtpTs_positive_second_constructor) {
    std::cout << "Entering getNtpTs_positive_second_constructor test" << std::endl;
    
    EXPECT_NO_THROW({
        // Creating a FullIsoBmffBox instance first.
        // Prepare btype array using strncpy.
        char btype[4];
        std::cout << "Preparing btype for FullIsoBmffBox constructor using strncpy" << std::endl;
        strncpy(btype, "prft", sizeof(btype));
        std::cout << "btype value set to: " << std::string(btype, sizeof(btype)) << std::endl;
        
        // Using FullIsoBmffBox(uint32_t sz, const char btype[4], uint8_t ver, uint32_t f)
        FullIsoBmffBox fullBox(200, btype, 1, 0);
        std::cout << "Created FullIsoBmffBox with parameters:" << std::endl;
        std::cout << "  size = " << 200 << ", btype = " << std::string(btype, sizeof(btype))
                  << ", version = " << 1 << ", flags = " << 0 << std::endl;
        
        // Using second constructor: PrftIsoBmffBox(FullIsoBmffBox & fbox, uint32_t trackId, uint64_t ntpTs, uint64_t mediaTime)
        player_isobmff::PrftIsoBmffBox box(fullBox, 20, 987654321ULL, 600ULL);
        std::cout << "Created PrftIsoBmffBox using second constructor with parameters:" << std::endl;
        std::cout << "  trackId = " << 20 << ", ntpTs = " << 987654321ULL << ", mediaTime = " << 600ULL << std::endl;
        
        std::cout << "Invoking getNtpTs()" << std::endl;
        uint64_t retNtpTs = box.getNtpTs();
        std::cout << "Returned ntpTs value: " << retNtpTs << std::endl;
        
        EXPECT_EQ(retNtpTs, 987654321ULL);
    });
    
    std::cout << "Exiting getNtpTs_positive_second_constructor test" << std::endl;
}
/**
 * @brief Verify that getTrackId returns the expected track ID from PrftIsoBmffBox constructed with the first constructor
 *
 * This test verifies that the player_isobmff::PrftIsoBmffBox constructor correctly initializes the object with the provided
 * parameters and that the getTrackId() method returns the trackId value that was input during construction. This ensures the correct
 * instantiation of the object and functioning of its accessor method.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 153@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                                                   | Test Data                                         | Expected Result                                                    | Notes       |
 * | :--------------: | --------------------------------------------------------------------------------------------- | ------------------------------------------------- | ------------------------------------------------------------------ | ----------- |
 * | 01               | Construct PrftIsoBmffBox with valid parameters and invoke getTrackId() to verify correct value. | size = 100, trackId = 123, ntpTs = 1000, mediaTime = 2000, expected return = 123 | getTrackId() returns 123 and EXPECT_EQ assertion passes.             | Should Pass |
 */
TEST(player_isobmff_PrftIsoBmffBox, TestGetTrackIdFirstConstructor) {
    std::cout << "Entering TestGetTrackIdFirstConstructor test" << std::endl;
    
    // Define test input values
    uint32_t size = 100;
    uint32_t trackId = 123;
    uint64_t ntpTs = 1000;
    uint64_t mediaTime = 2000;
    
    // Invoke the constructor and create the object
    EXPECT_NO_THROW({
        player_isobmff::PrftIsoBmffBox box(size, trackId, ntpTs, mediaTime);
        std::cout << "Constructed player_isobmff::PrftIsoBmffBox with parameters: "
                  << "size = " << size << ", trackId = " << trackId 
                  << ", ntpTs = " << ntpTs << ", mediaTime = " << mediaTime << std::endl;
        
        // Invoke getTrackId() and capture the value
        uint32_t retTrackId = box.getTrackId();
        std::cout << "Called getTrackId(), returned value: " << retTrackId << std::endl;
        
        // Verify that the returned track id is as expected
        EXPECT_EQ(retTrackId, trackId);
    });
    
    std::cout << "Exiting TestGetTrackIdFirstConstructor test" << std::endl;
}
/**
 * @brief Validate that PrftIsoBmffBox constructed from a FullIsoBmffBox returns the correct track ID
 *
 * This test verifies that the second constructor of PrftIsoBmffBox, when invoked using a valid FullIsoBmffBox instance along with specified trackId, ntpTs, and mediaTime parameters, correctly initializes the object and that the getTrackId() method returns the expected trackId. This ensures that the track ID is properly set and retrieved.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 154
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Construct FullIsoBmffBox with valid parameters. | size=100, btype=ftyp, version=1, flags=0 | FullIsoBmffBox constructed successfully without exceptions. | Should Pass |
 * | 02 | Construct PrftIsoBmffBox using the FullIsoBmffBox instance and provided parameters. | fullBox instance, trackId=456, ntpTs=3000, mediaTime=4000 | PrftIsoBmffBox constructed successfully without exceptions. | Should Pass |
 * | 03 | Invoke getTrackId() on PrftIsoBmffBox and validate the returned trackId. | prftBox.getTrackId(), expected trackId=456 | Returns trackId equal to 456 and passes the equality assertion. | Should Pass |
 */
TEST(player_isobmff_PrftIsoBmffBox, TestGetTrackIdSecondConstructor) {
    std::cout << "Entering TestGetTrackIdSecondConstructor test" << std::endl;
    
    // Create a valid FullIsoBmffBox instance using its constructor
    uint32_t size = 100;
    char btype[4];
    strncpy(btype, "ftyp", 4);
    uint8_t version = 1;
    uint32_t flags = 0;
    EXPECT_NO_THROW({
        player_isobmff::FullIsoBmffBox fullBox(size, btype, version, flags);
        std::cout << "Constructed player_isobmff::FullIsoBmffBox with parameters: "
                  << "size = " << size << ", btype = " << std::string(btype, 4)
                  << ", version = " << static_cast<uint32_t>(version)
                  << ", flags = " << flags << std::endl;
        
        // Define parameters for PrftIsoBmffBox second constructor
        uint32_t trackId = 456;
        uint64_t ntpTs = 3000;
        uint64_t mediaTime = 4000;
        
        // Invoke the second constructor of PrftIsoBmffBox using the FullIsoBmffBox instance
        player_isobmff::PrftIsoBmffBox prftBox(fullBox, trackId, ntpTs, mediaTime);
        std::cout << "Constructed player_isobmff::PrftIsoBmffBox (using FullIsoBmffBox) with parameters: "
                  << "trackId = " << trackId << ", ntpTs = " << ntpTs 
                  << ", mediaTime = " << mediaTime << std::endl;
        
        // Invoke getTrackId() and capture the value
        uint32_t retTrackId = prftBox.getTrackId();
        std::cout << "Called getTrackId(), returned value: " << retTrackId << std::endl;
        
        // Verify that the returned track id is as expected
        EXPECT_EQ(retTrackId, trackId);
    });
    
    std::cout << "Exiting TestGetTrackIdSecondConstructor test" << std::endl;
}
/**
 * @brief Verify that setMediaTime method correctly updates the media time with a normal value.
 *
 * This test case creates a PrftIsoBmffBox object with valid initial parameters and then calls
 * the setMediaTime method with a normal positive value to ensure that the media_time field is
 * updated accordingly without throwing any exceptions.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 155@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                        | Test Data                                                       | Expected Result                                                                 | Notes            |
 * | :--------------: | ------------------------------------------------------------------ | --------------------------------------------------------------- | ------------------------------------------------------------------------------- | ---------------- |
 * | 01               | Create PrftIsoBmffBox object with valid initial parameters           | sz = 32, trackId = 1, ntpTs = 123456789, initialMediaTime = 0      | Object is created successfully without throwing any exception                   | Should be successful |
 * | 02               | Invoke setMediaTime with a normal positive value to update mediaTime | newMediaTime = 100                                              | setMediaTime method executes without throwing exception, updating media_time to 100 | Should Pass      |
 */
TEST(player_isobmff_PrftIsoBmffBox, setMediaTime_with_normal_value) {
    std::cout << "Entering setMediaTime_with_normal_value test" << std::endl;
    
    // Create object using the custom constructor of PrftIsoBmffBox.
    uint32_t sz = 32;
    uint32_t trackId = 1;
    uint64_t ntpTs = 123456789;
    uint64_t initialMediaTime = 0;
    std::cout << "Creating player_isobmff::PrftIsoBmffBox object with parameters:" << std::endl;
    std::cout << "  sz = " << sz << ", trackId = " << trackId 
              << ", ntpTs = " << ntpTs << ", initialMediaTime = " << initialMediaTime << std::endl;
              
    EXPECT_NO_THROW({
        player_isobmff::PrftIsoBmffBox prftBox(sz, trackId, ntpTs, initialMediaTime);
        std::cout << "Object created successfully." << std::endl;
       
        // Invoke setMediaTime with a normal positive value.
        uint64_t newMediaTime = 100;
        std::cout << "Invoking setMediaTime with mediaTime = " << newMediaTime << std::endl;
        EXPECT_NO_THROW({
            prftBox.setMediaTime(newMediaTime);
        });
        std::cout << "setMediaTime method invoked successfully." << std::endl;
        std::cout << "Expected that the object's media_time field is updated to " << newMediaTime << std::endl;
    });
    
    std::cout << "Exiting setMediaTime_with_normal_value test" << std::endl;
}
/**
 * @brief Test the PrftIsoBmffBox::setNtpTs function for updating the ntp timestamp with a typical value.
 *
 * This test verifies that setting a typical new NTP timestamp value using the setNtpTs function correctly updates the internal state of the PrftIsoBmffBox object. The test ensures that the method accepts a valid numeric input and updates the internal ntp_ts without throwing any exceptions.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 156@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Construct the PrftIsoBmffBox object with initial parameters. | sz = 1000, trackId = 1, initialNtpTs = 0, mediaTime = 500 | Object is created without throwing any exceptions. | Should be successful |
 * | 02 | Invoke setNtpTs on the constructed object with a new ntp timestamp value. | newNtpTs = 100, (internal ntp_ts updated to 100) | Internal ntp_ts is updated to 100 and no exception is thrown. | Should Pass |
 */
TEST(player_isobmff_PrftIsoBmffBox, setNtpTs_typicalValue) {
    std::cout << "Entering setNtpTs_typicalValue test" << std::endl;
    
    // Setup initial values for constructing the object.
    uint32_t sz = 1000;
    uint32_t trackId = 1;
    uint64_t initialNtpTs = 0; // initial ntp_ts value before setting
    uint64_t mediaTime = 500;
    
    // Log the creation of the object.
    std::cout << "Creating PrftIsoBmffBox object with sz = " << sz 
              << ", trackId = " << trackId 
              << ", initial ntpTs = " << initialNtpTs 
              << ", mediaTime = " << mediaTime << std::endl;
    
    // Create the PrftIsoBmffBox object using the constructor.
    EXPECT_NO_THROW({
        player_isobmff::PrftIsoBmffBox box(sz, trackId, initialNtpTs, mediaTime);
        
        // Log the method invocation.
        uint64_t newNtpTs = 100;
        std::cout << "Invoking setNtpTs with value: " << newNtpTs << std::endl;
        EXPECT_NO_THROW(box.setNtpTs(newNtpTs));
        
        // Log the expected internal state update.
        std::cout << "After invoking setNtpTs, the internal ntp_ts should be updated to: " << newNtpTs << std::endl;
    });
    
    std::cout << "Exiting setNtpTs_typicalValue test" << std::endl;
}
/**
 * @brief Test the typical scenario for updating the track ID in PrftIsoBmffBox.
 *
 * This test verifies that the setTrackId method of the PrftIsoBmffBox class correctly updates the internal track_id when provided with a new valid track ID. The test constructs an object with an initial track ID, logs the initial value, updates it using setTrackId, and then asserts that the internal track_id is updated as expected.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 157
 * **Priority:** High
 * 
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                       | Test Data                                                                                   | Expected Result                                                          | Notes             |
 * | :--------------: | --------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------ | ----------------- |
 * | 01               | Create PrftIsoBmffBox object using custom constructor with initial track ID set to 5 | size = 100, initialTrackId = 5, ntpTs = 1000, mediaTime = 2000                                 | Object constructed successfully without exceptions                     | Should be successful |
 * | 02               | Log the initial internal state of track_id                                        | object.track_id before modification                                                         | Log shows track_id equal to 5                                             | Should be successful |
 * | 03               | Call setTrackId method with new track ID value 12                                  | newTrackId = 12                                                                              | Function call updates track_id to 12; no exceptions thrown                | Should Pass       |
 * | 04               | Log updated internal state and assert that track_id equals new track ID              | object.track_id after modification, assertion: track_id equals newTrackId                    | Log confirms track_id is 12 and EXPECT_EQ assertion passes                | Should be successful |
 */
TEST(player_isobmff_PrftIsoBmffBox, setTrackId_typical) {
    std::cout << "Entering setTrackId_typical test" << std::endl;

    EXPECT_NO_THROW({
        // Create a PrftIsoBmffBox object using custom constructor with initial trackId = 5
        uint32_t size = 100;
        uint32_t initialTrackId = 5;
        uint64_t ntpTs = 1000;
        uint64_t mediaTime = 2000;
        player_isobmff::PrftIsoBmffBox box(size, initialTrackId, ntpTs, mediaTime);
        std::cout << "Created PrftIsoBmffBox object with initial parameters:" << std::endl;
        std::cout << "  size: " << size << ", initial trackId: " << initialTrackId 
                  << ", ntpTs: " << ntpTs << ", mediaTime: " << mediaTime << std::endl;

        // Log the current internal state track_id
        std::cout << "Before setTrackId, internal track_id = " << box.track_id << std::endl;

        // Invoke setTrackId with new track id value of 12
        uint32_t newTrackId = 12;
        std::cout << "Invoking setTrackId with value: " << newTrackId << std::endl;
        box.setTrackId(newTrackId);

        // Log the updated internal state track_id
        std::cout << "After setTrackId, internal track_id = " << box.track_id << std::endl;

        // Verify that track_id is updated correctly
        EXPECT_EQ(box.track_id, newTrackId);
    });

    std::cout << "Exiting setTrackId_typical test" << std::endl;
}
/**
 * @brief Validate construction of SaizIsoBmffBox using valid standard values
 *
 * This test verifies the successful construction of a SaizIsoBmffBox instance when provided with standard valid input values. It first constructs a FullIsoBmffBox instance with defined parameters and then uses it to create a SaizIsoBmffBox. The objective is to ensure that no exceptions are thrown during object construction and that the internal state correctly mirrors the input parameters.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 158@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                     | Test Data                                                                  | Expected Result                                                                                     | Notes       |
 * | :--------------: | ----------------------------------------------------------------------------------------------- | -------------------------------------------------------------------------- | --------------------------------------------------------------------------------------------------- | ----------- |
 * | 01               | Invoke FullIsoBmffBox constructor with given parameters                                         | size = 100, btype = "saiz", version = 1, flags = 0x0F                      | FullIsoBmffBox instance is created without throwing any exception                                    | Should Pass |
 * | 02               | Invoke SaizIsoBmffBox constructor using the FullIsoBmffBox object and additional parameters       | sampleCountLoc = pointer to uint8_t (value 10), numSamples = 5, sample_info_size = 64          | SaizIsoBmffBox instance is created without throwing any exception and internal state reflects inputs | Should Pass |
 */
TEST(player_isobmff_SaizIsoBmffBox, ValidConstructionStandardValues) {
    std::cout << "Entering ValidConstructionStandardValues test" << std::endl;
    
    // Prepare btype array using strncpy
    char btype[4];
    std::memset(btype, 0, sizeof(btype));
    std::strncpy(btype, "saiz", sizeof(btype));

    // Create a FullIsoBmffBox object with defined parameters
    uint8_t sampleCount = 10;
    uint8_t* sampleCountLoc = &sampleCount;
    EXPECT_NO_THROW({
        FullIsoBmffBox fbox(100, btype, 1, 0x0F);
        std::cout << "Invoked FullIsoBmffBox constructor with parameters:" << std::endl;
        std::cout << "    size: 100" << std::endl;
        std::cout << "    btype: " << btype << std::endl;
        std::cout << "    version: 1" << std::endl;
        std::cout << "    flags: 0x0F" << std::endl;
        
        // Construct SaizIsoBmffBox using the previously created fbox
        player_isobmff::SaizIsoBmffBox box(fbox, sampleCountLoc, 5, 64);
        std::cout << "Invoked SaizIsoBmffBox constructor with parameters:" << std::endl;
        std::cout << "    sampleCountLoc value: " << static_cast<int>(*sampleCountLoc) << std::endl;
        std::cout << "    numSamples: " << 5 << std::endl;
        std::cout << "    sample_info_size: " << 64 << std::endl;
        // Debug logs: internal state assumed to mirror the parameters provided.
    });
    
    std::cout << "Exiting ValidConstructionStandardValues test" << std::endl;
}
/**
 * @brief Validate SaizIsoBmffBox constructor handling of zero sample_info_size
 *
 * This test verifies that the SaizIsoBmffBox constructor behaves correctly when the sample_info_size is set to zero. It does so by first initializing the necessary btype array and sample counting variables, then creating a FullIsoBmffBox and subsequently a SaizIsoBmffBox with zero sample_info_size. The objective is to confirm that the API correctly handles this edge case without throwing exceptions.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 159@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Prepare the btype array and initialize data | btype = "saiz", sampleCount = 10, sampleCountLoc = address of sampleCount | btype array properly set with "saiz" and sample count variable initialized | Should be successful |
 * | 02 | Construct FullIsoBmffBox instance to set up the base box | size = 50, btype = "saiz", version = 0, flags = 0 | FullIsoBmffBox instance created successfully without exceptions | Should Pass |
 * | 03 | Construct SaizIsoBmffBox instance with sample_info_size as 0 | sampleCount = 10, numSamples = 5, sample_info_size = 0, sampleCountLoc pointing to sampleCount | SaizIsoBmffBox instance created successfully with no exception thrown | Should Pass |
 */
TEST(player_isobmff_SaizIsoBmffBox, EdgeCaseZeroSampleInfoSize) {
    std::cout << "Entering EdgeCaseZeroSampleInfoSize test" << std::endl;
    
    // Prepare btype array using strncpy
    char btype[4];
    std::memset(btype, 0, sizeof(btype));
    std::strncpy(btype, "saiz", sizeof(btype));

    // Setup sample count and pointer
    uint8_t sampleCount = 10;
    uint8_t* sampleCountLoc = &sampleCount;
    
    EXPECT_NO_THROW({
        FullIsoBmffBox fbox(50, btype, 0, 0);
        std::cout << "Invoked FullIsoBmffBox constructor with parameters:" << std::endl;
        std::cout << "    size: 50" << std::endl;
        std::cout << "    btype: " << btype << std::endl;
        std::cout << "    version: 0" << std::endl;
        std::cout << "    flags: 0" << std::endl;
        
        // Construct SaizIsoBmffBox with sample_info_size = 0
        player_isobmff::SaizIsoBmffBox box(fbox, sampleCountLoc, 5, 0);
        std::cout << "Invoked SaizIsoBmffBox constructor with parameters:" << std::endl;
        std::cout << "    sampleCountLoc value: " << static_cast<int>(*sampleCountLoc) << std::endl;
        std::cout << "    numSamples: " << 5 << std::endl;
        std::cout << "    sample_info_size: " << 0 << std::endl;
    });
    
    std::cout << "Exiting EdgeCaseZeroSampleInfoSize test" << std::endl;
}
/**
 * @brief Verify that SaizIsoBmffBox constructor correctly handles the edge case with zero samples.
 *
 * This test verifies that when SaizIsoBmffBox is constructed with numSamples set to 0, the API handles
 * the edge case gracefully without throwing any exceptions. It sets up the required FullIsoBmffBox and
 * sample count pointer, then creates a SaizIsoBmffBox instance to ensure the correct behavior on a zero sample count.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 160
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data |Expected Result |Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Prepare btype array and initialize sample count pointer | btype = "saiz", sampleCount = 10, sampleCountLoc = address of sampleCount | btype array is correctly set; sampleCount pointer is valid | Should be successful |
 * | 02 | Invoke FullIsoBmffBox constructor with valid parameters | size = 50, btype = "saiz", version = 0, flags = 0 | No exception thrown; FullIsoBmffBox constructed successfully | Should Pass |
 * | 03 | Invoke SaizIsoBmffBox constructor with numSamples equal to 0 and sample_info_size of 64 | sampleCountLoc value = 10, numSamples = 0, sample_info_size = 64 | No exception thrown; SaizIsoBmffBox handles zero samples appropriately | Should Pass |
 */
TEST(player_isobmff_SaizIsoBmffBox, EdgeCaseZeroSamples) {
    std::cout << "Entering EdgeCaseZeroSamples test" << std::endl;
    
    // Prepare btype array using strncpy
    char btype[4];
    std::memset(btype, 0, sizeof(btype));
    std::strncpy(btype, "saiz", sizeof(btype));

    // Setup sample count and pointer
    uint8_t sampleCount = 10;
    uint8_t* sampleCountLoc = &sampleCount;
    
    EXPECT_NO_THROW({
        FullIsoBmffBox fbox(50, btype, 0, 0);
        std::cout << "Invoked FullIsoBmffBox constructor with parameters:" << std::endl;
        std::cout << "    size: 50" << std::endl;
        std::cout << "    btype: " << btype << std::endl;
        std::cout << "    version: 0" << std::endl;
        std::cout << "    flags: 0" << std::endl;
        
        // Construct SaizIsoBmffBox with numSamples = 0
        player_isobmff::SaizIsoBmffBox box(fbox, sampleCountLoc, 0, 64);
        std::cout << "Invoked SaizIsoBmffBox constructor with parameters:" << std::endl;
        std::cout << "    sampleCountLoc value: " << static_cast<int>(*sampleCountLoc) << std::endl;
        std::cout << "    numSamples: " << 0 << std::endl;
        std::cout << "    sample_info_size: " << 64 << std::endl;
    });
    
    std::cout << "Exiting EdgeCaseZeroSamples test" << std::endl;
}
/**
 * @brief Test to verify that the SaizIsoBmffBox constructor throws an exception when a null sampleCountLoc pointer is provided.
 *
 * This test validates that the API correctly identifies an invalid null pointer for the sampleCountLoc parameter by throwing an exception.
 * This is essential to ensure that the API handles erroneous input gracefully and avoids undefined behavior.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 161@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                           | Test Data                                                        | Expected Result                                                    | Notes         |
 * | :--------------: | ----------------------------------------------------------------------------------------------------- | ---------------------------------------------------------------- | ------------------------------------------------------------------ | ------------- |
 * | 01               | Prepare btype array with "saiz" using strncpy and clear the array using memset                        | btype = "saiz"                                                   | btype correctly initialized                                        | Should be successful |
 * | 02               | Set sampleCountLoc pointer to nullptr to simulate invalid input                                       | sampleCountLoc = nullptr                                           | sampleCountLoc is nullptr                                            | Should be successful |
 * | 03               | Create a FullIsoBmffBox object with valid parameters                                                  | size = 80, btype = "saiz", version = 1, flags = 0xAA                | FullIsoBmffBox object is constructed successfully                    | Should be successful |
 * | 04               | Attempt to invoke the SaizIsoBmffBox constructor with a null sampleCountLoc pointer and verify exception  | sampleCountLoc = nullptr, numSamples = 10, sample_info_size = 32      | Exception is thrown as expected                                      | Should Fail   |
 */
TEST(player_isobmff_SaizIsoBmffBox, NegativeCaseNullSampleCountLoc) {
    std::cout << "Entering NegativeCaseNullSampleCountLoc test" << std::endl;
    
    // Prepare btype array using strncpy
    char btype[4];
    std::memset(btype, 0, sizeof(btype));
    std::strncpy(btype, "saiz", sizeof(btype));
    
    // Set sampleCountLoc to nullptr to simulate invalid pointer
    uint8_t* sampleCountLoc = nullptr;
    
    EXPECT_THROW({
        FullIsoBmffBox fbox(80, btype, 1, 0xAA);
        std::cout << "Invoked FullIsoBmffBox constructor with parameters:" << std::endl;
        std::cout << "    size: 80" << std::endl;
        std::cout << "    btype: " << btype << std::endl;
        std::cout << "    version: 1" << std::endl;
        std::cout << "    flags: 0xAA" << std::endl;
        
        // Attempt to construct SaizIsoBmffBox with a null sampleCountLoc pointer
        player_isobmff::SaizIsoBmffBox box(fbox, sampleCountLoc, 10, 32);
        std::cout << "Invoked SaizIsoBmffBox constructor with parameters:" << std::endl;
        std::cout << "    sampleCountLoc: nullptr" << std::endl;
        std::cout << "    numSamples: " << 10 << std::endl;
        std::cout << "    sample_info_size: " << 32 << std::endl;
    }, std::exception);
    
    std::cout << "Exiting NegativeCaseNullSampleCountLoc test" << std::endl;
}
/**
 * @brief Verify that a SaizIsoBmffBox is constructed successfully using valid inputs
 *
 * This test verifies that the static method constructSaizBox successfully creates a SaizIsoBmffBox when provided with valid parameters.
 * It prepares a valid memory block with dummy box structure content and then invokes constructSaizBox.
 * The test ensures that no exceptions are thrown and the returned box pointer is not nullptr, thereby validating the correctness of input handling.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 162@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                         | Test Data                                  | Expected Result                                        | Notes         |
 * | :--------------: | ------------------------------------------------------------------- | ------------------------------------------ | ------------------------------------------------------ | ------------- |
 * | 01               | Prepare a valid memory block and initialize with dummy box structure content | validMemory size = 32, dummyContent = "BOX1" | Memory block is correctly initialized                | Should be successful |
 * | 02               | Invoke the constructSaizBox API with the valid memory block pointer and size | input: sz = 32, ptr = validMemory.data()    | API call does not throw any exceptions                 | Should Pass   |
 * | 03               | Verify that the returned SaizIsoBmffBox pointer is not nullptr       | output: box pointer                        | box is not equal to nullptr                            | Should Pass   |
 */
TEST(player_isobmff_SaizIsoBmffBox, Construct_valid_SaizIsoBmffBox_nominal_valid_inputs) {
    std::cout << "Entering Construct_valid_SaizIsoBmffBox_nominal_valid_inputs test" << std::endl;
    
    // Prepare a valid memory block of 32 bytes and initialize with dummy box structure content.
    std::vector<uint8_t> validMemory(32, 0);
    // Assign some dummy values to simulate proper box structure content.
    const char dummyContent[5] = "BOX1"; // 4 characters + null terminator
    std::memcpy(validMemory.data(), dummyContent, 4);
    
    std::cout << "Invoking constructSaizBox with sz = 32 and ptr = " << static_cast<void*>(validMemory.data()) << std::endl;
    
    // Invoke the static method to construct the box.
    EXPECT_NO_THROW({
        player_isobmff::SaizIsoBmffBox* box = player_isobmff::SaizIsoBmffBox::constructSaizBox(32, validMemory.data());
        std::cout << "Returned pointer from constructSaizBox: " << box << std::endl;
        
        // Verify that a valid box is returned.
        EXPECT_NE(box, nullptr);
        
    });
    
    std::cout << "Exiting Construct_valid_SaizIsoBmffBox_nominal_valid_inputs test" << std::endl;
}
/**
 * @brief Test constructSaizBox API with null pointer input
 *
 * This test verifies that calling SaizIsoBmffBox::constructSaizBox with a null pointer as the second argument returns a null pointer. It ensures that the API handles null pointer input gracefully.@n
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 163@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- | ------------ | ----- |
 * | 01 | Invoke constructSaizBox with sz = 32 and null pointer input to verify proper handling of null input. | sz = 32, nullPtr = nullptr, output = box | API returns nullptr and EXPECT_EQ verifies the null value | Should Pass |
 */
TEST(player_isobmff_SaizIsoBmffBox, Construct_SaizIsoBmffBox_null_pointer_input) {
    std::cout << "Entering Construct_SaizIsoBmffBox_null_pointer_input test" << std::endl;
    
    uint32_t sz = 32;
    uint8_t* nullPtr = nullptr;
    std::cout << "Invoking constructSaizBox with sz = " << sz << " and ptr = " << static_cast<void*>(nullPtr) << std::endl;
    
    player_isobmff::SaizIsoBmffBox* box = player_isobmff::SaizIsoBmffBox::constructSaizBox(sz, nullPtr);
    std::cout << "Returned pointer from constructSaizBox: " << box << std::endl;
    
    // Expecting null pointer output for null input.
    EXPECT_EQ(box, nullptr);
    
    std::cout << "Exiting Construct_SaizIsoBmffBox_null_pointer_input test" << std::endl;
}
/**
 * @brief Test the construction of SaizIsoBmffBox with a zero size input.
 *
 * This test verifies that calling constructSaizBox with a size of zero and a valid memory pointer returns a null pointer. It is important to confirm that the function handles the zero-length input case gracefully by not allocating or constructing an invalid box.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 164@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                         | Test Data                                      | Expected Result                                                               | Notes       |
 * | :--------------: | ------------------------------------------------------------------- | ---------------------------------------------- | ----------------------------------------------------------------------------- | ----------- |
 * | 01               | Invoke constructSaizBox with sz = 0 and a valid memory pointer        | sz = 0, validMemory pointer = validMemory.data() | API returns a nullptr and EXPECT_EQ confirms the null pointer                | Should Pass |
 */
TEST(player_isobmff_SaizIsoBmffBox, Construct_SaizIsoBmffBox_zero_size_input) {
    std::cout << "Entering Construct_SaizIsoBmffBox_zero_size_input test" << std::endl;
    
    uint32_t sz = 0;
    // Allocate a valid memory block even though sz is 0.
    std::vector<uint8_t> validMemory(32, 0);
    std::cout << "Invoking constructSaizBox with sz = " << sz << " and ptr = " << static_cast<void*>(validMemory.data()) << std::endl;
    
    player_isobmff::SaizIsoBmffBox* box = player_isobmff::SaizIsoBmffBox::constructSaizBox(sz, validMemory.data());
    std::cout << "Returned pointer from constructSaizBox: " << box << std::endl;
    
    // Expecting null pointer output for zero size input.
    EXPECT_EQ(box, nullptr);
    
    std::cout << "Exiting Construct_SaizIsoBmffBox_zero_size_input test" << std::endl;
}
/**
 * @brief Test to verify that the SaizIsoBmffBox returns the correct first sample info size.
 *
 * This test verifies that when a SaizIsoBmffBox object is instantiated with a nonzero number of samples 
 * and a specified first sample info size (i.e., 10), the getFirstSampleInfoSize method returns the expected value.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 165@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                                                                 | Test Data                                                                                                       | Expected Result                                                       | Notes       |
 * | :--------------: | ----------------------------------------------------------------------------------------------------------- | --------------------------------------------------------------------------------------------------------------- | --------------------------------------------------------------------- | ----------- |
 * | 01               | Initialize the FullIsoBmffBox with size = 32, btype = "ftyp", version = 1, flags = 0, and set up sampleCount = 5  | input: size = 32, btype = "ftyp", version = 1, flags = 0; sampleCount = 5; numSamples = 5; firstSampleInfoSize = 10 | FullIsoBmffBox and SaizIsoBmffBox objects created without exception; getFirstSampleInfoSize returns 10 | Should Pass |
 */
TEST(player_isobmff_SaizIsoBmffBox, getFirstSampleInfoSize_nonzero)
{
    std::cout << "Entering getFirstSampleInfoSize_nonzero test" << std::endl;

    // Prepare the btype fixed size array using strncpy
    char btype[5] = {0};
    const char *initBtype = "ftyp";
    std::strncpy(btype, initBtype, 4);
    btype[4] = '\0';
    std::cout << "Initialized btype with value: " << btype << std::endl;

    // Instantiate FullIsoBmffBox using its constructor
    // Using dummy values: size = 32, version = 1, flags = 0 (for example)
    EXPECT_NO_THROW({
        player_isobmff::FullIsoBmffBox fullBox(32, btype, 1, 0);
        std::cout << "Created FullIsoBmffBox with size: 32, btype: " << btype 
                  << ", version: 1, flags: 0" << std::endl;

        // Prepare a sampleCountLoc variable
        uint8_t sampleCount = 5; // arbitrary non-zero value
        uint8_t *sampleCountLoc = &sampleCount;
        std::cout << "sampleCountLoc points to value: " << static_cast<uint32_t>(*sampleCountLoc) << std::endl;

        // Instantiate SaizIsoBmffBox with first sample info size = 10, numSamples = 5 (non-zero)
        player_isobmff::SaizIsoBmffBox saizBox(fullBox, sampleCountLoc, 5, 10);
        std::cout << "Created SaizIsoBmffBox with numSamples: 5 and firstSampleInfoSize: 10" << std::endl;
        
        // Invoke the method getFirstSampleInfoSize and print the returned value
        std::cout << "Invoking getFirstSampleInfoSize()" << std::endl;
        uint32_t retValue = saizBox.getFirstSampleInfoSize();
        std::cout << "Retrieved first sample info size: " << retValue << std::endl;
        
        // Check the returned value is 10
        EXPECT_EQ(retValue, 10);
    });
    
    std::cout << "Exiting getFirstSampleInfoSize_nonzero test" << std::endl;
}
/**
 * @brief Verify that getFirstSampleInfoSize returns zero when first sample info size is set to 0
 *
 * This test validates that the SaizIsoBmffBox API correctly returns a first sample info size of zero when it is
 * initialized with a firstSampleInfoSize value of 0. The test constructs the necessary FullIsoBmffBox and SaizIsoBmffBox
 * objects with dummy values and verifies the method returns the expected value.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 166@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Initialize btype using strncpy with "ftyp" | initBtype = ftyp, btype (output) = ftyp | btype is correctly initialized as "ftyp" | Should be successful |
 * | 02 | Instantiate FullIsoBmffBox with dummy values | size = 32, btype = ftyp, version = 1, flags = 0 | FullIsoBmffBox object is created without throwing exceptions | Should be successful |
 * | 03 | Set sampleCount to 8 and assign its address to sampleCountLoc | sampleCount = 8, sampleCountLoc points to 8 | sampleCountLoc holds the value 8 | Should be successful |
 * | 04 | Instantiate SaizIsoBmffBox with firstSampleInfoSize = 0 and numSamples = 3 | fullBox object, sampleCountLoc, numSamples = 3, firstSampleInfoSize = 0 | SaizIsoBmffBox object is created without throwing exceptions | Should be successful |
 * | 05 | Invoke getFirstSampleInfoSize API method | Invocation of getFirstSampleInfoSize(), no additional parameters | Returns 0 and passes the EXPECT_EQ(retValue, 0) check | Should Pass |
 */
TEST(player_isobmff_SaizIsoBmffBox, getFirstSampleInfoSize_zero)
{
    std::cout << "Entering getFirstSampleInfoSize_zero test" << std::endl;

    // Prepare the btype fixed size array using strncpy
    char btype[5] = {0};
    const char *initBtype = "ftyp";
    std::strncpy(btype, initBtype, 4);
    btype[4] = '\0';
    std::cout << "Initialized btype with value: " << btype << std::endl;

    // Instantiate FullIsoBmffBox using its constructor with dummy values
    EXPECT_NO_THROW({
        player_isobmff::FullIsoBmffBox fullBox(32, btype, 1, 0);
        std::cout << "Created FullIsoBmffBox with size: 32, btype: " << btype 
                  << ", version: 1, flags: 0" << std::endl;

        // Prepare a sampleCountLoc variable; using an arbitrary uint8_t value
        uint8_t sampleCount = 8; 
        uint8_t *sampleCountLoc = &sampleCount;
        std::cout << "sampleCountLoc points to value: " << static_cast<uint32_t>(*sampleCountLoc) << std::endl;

        // Instantiate SaizIsoBmffBox with first sample info size = 0, numSamples = 3 (can be any)
        player_isobmff::SaizIsoBmffBox saizBox(fullBox, sampleCountLoc, 3, 0);
        std::cout << "Created SaizIsoBmffBox with numSamples: 3 and firstSampleInfoSize: 0" << std::endl;
        
        // Invoke the method getFirstSampleInfoSize and print the returned value
        std::cout << "Invoking getFirstSampleInfoSize()" << std::endl;
        uint32_t retValue = saizBox.getFirstSampleInfoSize();
        std::cout << "Retrieved first sample info size: " << retValue << std::endl;
        
        // Check the returned value is 0
        EXPECT_EQ(retValue, 0);
    });
    
    std::cout << "Exiting getFirstSampleInfoSize_zero test" << std::endl;
}
/**
 * @brief Test truncate() method on SaizIsoBmffBox with valid sample data
 *
 * This test verifies that the truncate() method of the SaizIsoBmffBox object executes without throwing an exception when constructed using valid FullIsoBmffBox parameters, a valid sampleCountLoc pointer, and typical values for numSamples and sample_info_size. This positive test case ensures that the API behaves as expected.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 167@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Construct FullIsoBmffBox with valid parameters | size = 100, btype = "saiz", version = 1, flags = 0 | FullIsoBmffBox object constructed successfully | Should be successful |
 * | 02 | Setup sampleCountLoc pointer with an initial value | sampleCountValue = 10 | sampleCountLoc initialized to 10 | Should be successful |
 * | 03 | Create SaizIsoBmffBox using FullIsoBmffBox and sampleCountLoc | numSamples = 5, sample_info_size = 10 | SaizIsoBmffBox object constructed successfully | Should Pass |
 * | 04 | Invoke truncate() method on SaizIsoBmffBox object | No additional input | truncate() method executes without throwing an exception | Should Pass |
 * | 05 | Log internal state of sampleCountLoc after truncate() | sampleCountLoc value before truncate() = 10, sampleCountLoc value after truncate() = updated value | Internal state logged without error | Should be successful |
 */
TEST(player_isobmff_SaizIsoBmffBox, Truncate_with_valid_sample_data) {
    std::cout << "Entering Truncate_with_valid_sample_data test" << std::endl;
    
    // Construct FullIsoBmffBox with size=100, btype="saiz", version=1, flags=0
    uint32_t size = 100;
    char btype[4];
    std::strncpy(btype, "saiz", 4);
    uint8_t version = 1;
    uint32_t flags = 0;
    std::cout << "Invoking FullIsoBmffBox constructor with: size = " << size 
              << ", btype = \"" << std::string(btype, 4) 
              << "\", version = " << static_cast<uint32_t>(version)
              << ", flags = " << flags << std::endl;
    // Creating a FullIsoBmffBox object (assuming public constructor exists)
    player_isobmff::SaizIsoBmffBox* saizBoxPtr = nullptr;
    try {

        FullIsoBmffBox fbox(size, btype, version, flags);
        std::cout << "FullIsoBmffBox object constructed successfully. Size = " 
                  << size << ", btype = \"" << std::string(btype, 4) 
                  << "\", version = " << static_cast<uint32_t>(version)
                  << ", flags = " << flags << std::endl;
        
        // Setup sampleCountLoc pointer with a pre-initialized value, e.g., 10.
        uint8_t sampleCountValue = 10;
        uint8_t* sampleCountLoc = &sampleCountValue;
        std::cout << "Setting up sampleCountLoc with initial value = " 
                  << static_cast<uint32_t>(*sampleCountLoc) << std::endl;
        
        // Parameters for SaizIsoBmffBox constructor: numSamples=5, sample_info_size=10
        uint32_t numSamples = 5;
        uint32_t sample_info_size = 10;
        std::cout << "Invoking SaizIsoBmffBox constructor with: numSamples = " 
                  << numSamples << ", sample_info_size = " << sample_info_size << std::endl;
        
        // Create a SaizIsoBmffBox object using the provided constructor signature.
        player_isobmff::SaizIsoBmffBox saizBox(fbox, sampleCountLoc, numSamples, sample_info_size);
        std::cout << "SaizIsoBmffBox object constructed successfully." << std::endl;
        
        // Invoke the truncate() method and expect no throw.
        std::cout << "Invoking truncate() method on SaizIsoBmffBox object." << std::endl;
        EXPECT_NO_THROW({
            saizBox.truncate();
        });
        std::cout << "truncate() method executed successfully." << std::endl;
        
        // Log internal state changes (assuming sampleCountLoc might have been modified)
        std::cout << "After truncate(), sampleCountLoc value = " 
                  << static_cast<uint32_t>(*sampleCountLoc) << std::endl;
    } catch(const std::exception& e) {
        std::cout << "Exception caught during test execution: " << e.what() << std::endl;
        FAIL() << "Exception thrown during object construction or truncate() invocation.";
    } catch(...) {
        std::cout << "Unknown exception caught during test execution." << std::endl;
        FAIL() << "Unknown exception thrown during object construction or truncate() invocation.";
    }
    
    std::cout << "Exiting Truncate_with_valid_sample_data test" << std::endl;
}
/**
 * @brief Verify construction of SencIsoBmffBox with valid parameters
 *
 * This test checks the successful construction of a FullIsoBmffBox object with valid parameters and subsequently uses it to construct a SencIsoBmffBox instance. The test ensures that both constructors do not throw exceptions when provided with valid input values, verifying the expected behavior of the API.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 168
 * **Priority:** High
 * 
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 * 
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                      | Test Data                                                                                                   | Expected Result                                             | Notes       |
 * | :--------------: | ------------------------------------------------------------------------------------------------ | ----------------------------------------------------------------------------------------------------------- | ----------------------------------------------------------- | ----------- |
 * | 01               | Invoke FullIsoBmffBox constructor with valid parameters                                          | sz = 100, btype = test, ver = 1, flags = 0xFF                                                               | FullIsoBmffBox instance is created without throwing         | Should Pass |
 * | 02               | Invoke SencIsoBmffBox constructor using the constructed FullIsoBmffBox, a valid sampleCountLoc, and numSamples = 10 | fullIsoBmffBox instance (from Step 01), sampleCount = address of valid uint8_t with initial value 0, numSamples = 10 | SencIsoBmffBox instance is created without throwing         | Should Pass |
 */
TEST(player_isobmff_SencIsoBmffBox, ConstructionWithValidParameters) {
    std::cout << "Entering ConstructionWithValidParameters test" << std::endl;

    {
        // Prepare FullIsoBmffBox parameters
        uint32_t sz = 100;
        char btype[4];
        strncpy(btype, "test", 4);
        uint8_t ver = 1;
        uint32_t flags = 0xFF;

        // Construct FullIsoBmffBox object using valid parameters.
        EXPECT_NO_THROW({
            player_isobmff::FullIsoBmffBox fbox(sz, btype, ver, flags);
            std::cout << "Invoked FullIsoBmffBox constructor with parameters: sz = " << sz
                      << ", btype = 'test'" << ", ver = " << static_cast<unsigned>(ver)
                      << ", flags = 0x" << std::hex << flags << std::dec << std::endl;

            // Prepare sampleCountLoc
            uint8_t sampleCount = 0;
            std::cout << "Assigning sampleCountLoc with address: " << static_cast<const void*>(&sampleCount)
                      << " and initial value: " << static_cast<unsigned>(sampleCount) << std::endl;

            // Construct SencIsoBmffBox instance with typical numSamples value.
            player_isobmff::SencIsoBmffBox sencBox(fbox, &sampleCount, 10);
            std::cout << "Invoked SencIsoBmffBox constructor with numSamples = 10" << std::endl;
        });
    }

    std::cout << "Exiting ConstructionWithValidParameters test" << std::endl;
}
/**
 * @brief Test SencIsoBmffBox construction with a null sampleCountLoc pointer
 *
 * This test verifies that the SencIsoBmffBox constructor throws an exception when provided with a null pointer for the sampleCountLoc parameter. It first constructs a valid FullIsoBmffBox object and then attempts to construct a SencIsoBmffBox object with an invalid (nullptr) sampleCountLoc. This negative test ensures proper error handling for invalid inputs.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 169@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Construct FullIsoBmffBox with valid parameters and verify successful creation | sz = 120, btype = "senc", ver = 2, flags = 0xAA | FullIsoBmffBox object created without exception | Should be successful |
 * | 02 | Attempt to construct SencIsoBmffBox with a null sampleCountLoc pointer and verify exception is thrown | sampleCountLoc = nullptr, numSamples = 5 | Exception is thrown during SencIsoBmffBox construction | Should Fail |
 */
TEST(player_isobmff_SencIsoBmffBox, ConstructionWithNullSampleCountLoc) {
    std::cout << "Entering ConstructionWithNullSampleCountLoc test" << std::endl;

    {
        // Prepare FullIsoBmffBox parameters
        uint32_t sz = 120;
        char btype[4];
        strncpy(btype, "senc", 4);
        uint8_t ver = 2;
        uint32_t flags = 0xAA;

        // Construct FullIsoBmffBox object using valid parameters.
        EXPECT_NO_THROW({
            player_isobmff::FullIsoBmffBox fbox(sz, btype, ver, flags);
            std::cout << "Invoked FullIsoBmffBox constructor with parameters: sz = " << sz
                      << ", btype = 'senc'" << ", ver = " << static_cast<unsigned>(ver)
                      << ", flags = 0x" << std::hex << flags << std::dec << std::endl;

            // Prepare sampleCountLoc as a nullptr to simulate an invalid parameter.
            uint8_t *samplePtr = nullptr;
            std::cout << "Assigning sampleCountLoc as nullptr" << std::endl;

            // Attempt to construct SencIsoBmffBox; expecting an exception due to nullptr sampleCountLoc.
            EXPECT_THROW({
                player_isobmff::SencIsoBmffBox sencBox(fbox, samplePtr, 5);
                std::cout << "Invoked SencIsoBmffBox constructor with numSamples = 5" << std::endl;
            }, std::exception);
        });
    }

    std::cout << "Exiting ConstructionWithNullSampleCountLoc test" << std::endl;
}
/**
 * @brief Verify valid construction of SencIsoBmffBox using valid input data
 *
 * This test validates that the static factory method constructSencBox correctly creates a SencIsoBmffBox instance when provided with a valid memory buffer. The test ensures that no exception is thrown during the invocation and that the returned pointer is non-null, indicating successful construction.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 170@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                                     | Test Data                                                                                             | Expected Result                                                       | Notes           |
 * | :--------------: | ------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------------------- | --------------------------------------------------------------------- | --------------- |
 * | 01               | Prepare a valid memory block of 32 bytes with a dummy header "senc"             | boxSize = 32, data = allocated 32 bytes, header = "senc"                                              | Memory block is properly prepared with header copied                  | Should be successful |
 * | 02               | Invoke constructSencBox with the valid memory block and verify no exception is thrown | boxSize = 32, data = pointer to the valid memory block                                                 | API returns a valid non-null pointer without throwing an exception     | Should Pass     |
 * | 03               | Validate that the returned object pointer is non-null indicating successful construction | output1 = boxPtr (expected non-null)                                                                  | Assertion check confirms that boxPtr is not null                        | Should Pass     |
 * | 04               | Confirm that the debug console outputs indicate proper test entry and exit       | Console output for "Entering ValidBoxConstruction test" and "Exiting ValidBoxConstruction test"       | Printed debug messages confirm the correct flow of test execution         | Should be successful |
 */
TEST(player_isobmff_SencIsoBmffBox, ValidBoxConstruction) {
    std::cout << "Entering ValidBoxConstruction test" << std::endl;
    
    // Prepare a valid memory block with at least 32 bytes and set dummy header data using strncpy.
    const uint32_t boxSize = 32;
    uint8_t data[32] = {0};
    const char header[] = "senc"; // dummy header value
    // Use strncpy to copy header to the beginning of data
    std::strncpy(reinterpret_cast<char*>(data), header, sizeof(header) - 1);
    std::cout << "Invoking constructSencBox with boxSize = " << boxSize 
              << " and ptr address = " << static_cast<void*>(data) << std::endl;
    
    // Invoke static factory method and check for no exception
    player_isobmff::SencIsoBmffBox* boxPtr = nullptr;
    EXPECT_NO_THROW(
        boxPtr = player_isobmff::SencIsoBmffBox::constructSencBox(boxSize, data);
    );
    std::cout << "Method constructSencBox returned pointer = " << static_cast<void*>(boxPtr) << std::endl;
    
    // Validate that the returned object is non-null indicating successful construction.
    EXPECT_NE(boxPtr, nullptr);
    std::cout << "Exiting ValidBoxConstruction test" << std::endl;
}
/**
 * @brief Validate that SencIsoBmffBox::constructSencBox returns null for zero size input
 *
 * This test verifies that invoking constructSencBox with a boxSize of zero and a valid data pointer
 * returns a null pointer, indicating that the API properly handles insufficient box size.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 171@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- | -------------- | ----- |
 * | 01 | Prepare memory block: set boxSize to 0 and initialize a 32-byte data array with zeros. | boxSize = 0, data = {0,0,...,0} | Memory block is prepared successfully. | Should be successful |
 * | 02 | Invoke constructSencBox API with the zero boxSize and valid data pointer. | input: boxSize = 0, data pointer = valid memory address; output: boxPtr uninitialized | API call does not throw and returns a null pointer. | Should Pass |
 * | 03 | Verify that the returned pointer is null due to insufficient box size. | No additional test data | boxPtr equals nullptr. | Should Pass |
 */
TEST(player_isobmff_SencIsoBmffBox, ZeroSizeConstruction) {
    std::cout << "Entering ZeroSizeConstruction test" << std::endl;
    
    // Prepare a valid memory block even though size is zero; contents irrelevant
    const uint32_t boxSize = 0;
    uint8_t data[32] = {0};
    std::cout << "Invoking constructSencBox with boxSize = " << boxSize 
              << " and ptr address = " << static_cast<void*>(data) << std::endl;
    
    // Invoke the method
    player_isobmff::SencIsoBmffBox* boxPtr = nullptr;
    EXPECT_NO_THROW(
        boxPtr = player_isobmff::SencIsoBmffBox::constructSencBox(boxSize, data);
    );
    std::cout << "Method constructSencBox returned pointer = " << static_cast<void*>(boxPtr) << std::endl;
    
    // Expect null pointer due to insufficient box size.
    EXPECT_EQ(boxPtr, nullptr);
    
    std::cout << "Exiting ZeroSizeConstruction test" << std::endl;
}
/**
 * @brief Verify that constructSencBox handles a null pointer input gracefully.
 *
 * This test checks that when a valid box size is provided but a null data pointer is passed to 
 * player_isobmff::SencIsoBmffBox::constructSencBox, the function does not throw an exception and 
 * returns a null pointer, indicating proper handling of invalid input.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 172@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                                                 | Test Data                                          | Expected Result                                        | Notes       |
 * | :--------------: | ------------------------------------------------------------------------------------------- | -------------------------------------------------- | ------------------------------------------------------ | ----------- |
 * | 01               | Invoke constructSencBox with a valid box size (32) and a null data pointer                    | boxSize = 32, data = nullptr, boxPtr = nullptr       | No exception thrown and boxPtr is returned as nullptr  | Should Pass |
 */
TEST(player_isobmff_SencIsoBmffBox, NullPointerConstruction) {
    std::cout << "Entering NullPointerConstruction test" << std::endl;
    
    // Use a valid box size but pass a null pointer.
    const uint32_t boxSize = 32;
    uint8_t* data = nullptr;
    std::cout << "Invoking constructSencBox with boxSize = " << boxSize 
              << " and ptr = " << static_cast<void*>(data) << std::endl;
    
    // Invoke the method and expect no exception.
    player_isobmff::SencIsoBmffBox* boxPtr = nullptr;
    EXPECT_NO_THROW(
        boxPtr = player_isobmff::SencIsoBmffBox::constructSencBox(boxSize, data);
    );
    std::cout << "Method constructSencBox returned pointer = " << static_cast<void*>(boxPtr) << std::endl;
    
    // Expect null pointer due to invalid pointer input.
    EXPECT_EQ(boxPtr, nullptr);
    
    std::cout << "Exiting NullPointerConstruction test" << std::endl;
}
/**
 * @brief Tests SencIsoBmffBox::truncate with a firstSampleSize of zero.
 *
 * This test verifies that invoking the truncate method of SencIsoBmffBox with a firstSampleSize of zero does not throw any exceptions, ensuring that the API can gracefully handle an edge case where the truncation parameter is zero.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 173@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                               | Test Data                                           | Expected Result                                          | Notes                |
 * | :--------------: | --------------------------------------------------------- | --------------------------------------------------- | --------------------------------------------------------- | -------------------- |
 * | 01               | Construct FullIsoBmffBox object with arbitrary parameters  | size = 1024, btype = "abcd", version = 1, flags = 0   | FullIsoBmffBox is created without throwing exceptions     | Should be successful |
 * | 02               | Construct SencIsoBmffBox object using FullIsoBmffBox        | sampleCountLoc = {0}, numSamples = 10               | SencIsoBmffBox is created without throwing exceptions       | Should be successful |
 * | 03               | Invoke truncate API with firstSampleSize = 0              | firstSampleSize = 0                                 | truncate method completes without throwing exceptions     | Should Pass          |
 */
TEST(player_isobmff_SencIsoBmffBox, TruncateWithZero) {
    std::cout << "Entering TruncateWithZero test" << std::endl;

    // Prepare a FullIsoBmffBox object with arbitrary parameters.
    // Construct a dummy four-character type identifier.
    char btype[5] = "abcd"; // ensure null-terminated even if constructor uses only 4 bytes.
    // Using arbitrary values for size, version, and flags.
    EXPECT_NO_THROW({
        player_isobmff::FullIsoBmffBox fullBox(1024, btype, 1, 0);
        
        // Prepare a buffer for sampleCountLoc with enough size to store a uint32_t.
        uint8_t sampleCountLoc[4] = {0};
        // Choose an arbitrary number of samples.
        uint32_t numSamples = 10;
        
        // Create SencIsoBmffBox object using custom constructor.
        player_isobmff::SencIsoBmffBox sencBox(fullBox, sampleCountLoc, numSamples);
                
        // Invoke truncate with firstSampleSize = 0.
        uint32_t firstSampleSize = 0;
        std::cout << "Invoking truncate with firstSampleSize = " << firstSampleSize << std::endl;
        EXPECT_NO_THROW({
            sencBox.truncate(firstSampleSize);
        });

    });

    std::cout << "Exiting TruncateWithZero test" << std::endl;
}
/**
 * @brief Test truncation functionality of SencIsoBmffBox with mid-range first sample size.
 *
 * This test validates that the SencIsoBmffBox's truncate method operates correctly when provided with a typical mid-range first sample size value. It ensures that object construction and the subsequent truncation call do not throw any exceptions.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 174@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                                    | Test Data                                                                                                                  | Expected Result                                                    | Notes         |
 * | :--------------: | ------------------------------------------------------------------------------ | -------------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------ | ------------- |
 * | 01               | Create FullIsoBmffBox and SencIsoBmffBox objects with defined parameters       | size = 2048, btype = "efgh", param2 = 2, offset = 0, sampleCountLoc = {0,0,0,0}, numSamples = 20                           | Objects are constructed without throwing any exceptions            | Should be successful |
 * | 02               | Invoke the truncate method on SencIsoBmffBox with firstSampleSize = 500          | firstSampleSize = 500                                                                                                      | truncate executes without throwing exceptions, validating proper API behavior | Should Pass   |
 */
TEST(player_isobmff_SencIsoBmffBox, TruncateWithMidRange) {
    std::cout << "Entering TruncateWithMidRange test" << std::endl;

    // Prepare a FullIsoBmffBox object with arbitrary parameters.
    char btype[5] = "efgh"; // arbitrary four-character type.
    EXPECT_NO_THROW({
        player_isobmff::FullIsoBmffBox fullBox(2048, btype, 2, 0);
        
        // Prepare a buffer for sampleCountLoc with enough size to store a uint32_t.
        uint8_t sampleCountLoc[4] = {0};
        // Choose an arbitrary number of samples.
        uint32_t numSamples = 20;
        
        // Create SencIsoBmffBox object using custom constructor.
        player_isobmff::SencIsoBmffBox sencBox(fullBox, sampleCountLoc, numSamples);
        
        
        // Invoke truncate with firstSampleSize = 500.
        uint32_t firstSampleSize = 500;
        std::cout << "Invoking truncate with firstSampleSize = " << firstSampleSize << std::endl;
        EXPECT_NO_THROW({
            sencBox.truncate(firstSampleSize);
        });
        
    });

    std::cout << "Exiting TruncateWithMidRange test" << std::endl;
}
/**
 * @brief Validate SidxIsoBmffBox construction with valid parameters
 *
 * This test verifies that the SidxIsoBmffBox constructor properly initializes the object when provided with valid FullIsoBmffBox parameters along with valid time scale (tScale) and sidxDuration values. The test checks for no exceptions thrown during construction to ensure normal operation.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 175@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**
 * | Variation / Step | Description                                                          | Test Data                                                      | Expected Result                                                                         | Notes           |
 * | :--------------: | -------------------------------------------------------------------- | -------------------------------------------------------------- | --------------------------------------------------------------------------------------- | --------------- |
 * | 01               | Prepare input parameters and create FullIsoBmffBox object            | boxSize = 100, btype = "sidx", version = 1, flags = 0            | FullIsoBmffBox object is created successfully without error                             | Should be successful |
 * | 02               | Invoke SidxIsoBmffBox constructor with valid tScale and sidxDuration   | tScale = 1000, sidxDuration = 5000                              | No exception thrown from SidxIsoBmffBox constructor; EXPECT_NO_THROW passes                 | Should Pass     |
 * | 03               | Confirm test execution flow via console output                       | N/A                                                            | Console displays entering and exiting messages indicating successful test execution      | Should be successful |
 */
TEST(player_isobmff_SidxIsoBmffBox, ValidSidxConstruction) {
    std::cout << "Entering ValidSidxConstruction test" << std::endl;
    
    // Prepare input for FullIsoBmffBox
    uint32_t boxSize = 100;
    char btype[4];
    strncpy(btype, "sidx", 4);
    uint8_t version = 1;
    uint32_t flags = 0;
    
    std::cout << "Creating FullIsoBmffBox with parameters: "
              << "sz = " << boxSize << ", btype = " << std::string(btype, 4)
              << ", ver = " << static_cast<uint32_t>(version)
              << ", f = " << flags << std::endl;
    
    // Create the FullIsoBmffBox object
    FullIsoBmffBox fullBox(boxSize, btype, version, flags);
    
    uint32_t tScale = 1000;
    uint64_t sidxDuration = 5000;
    std::cout << "Invoking SidxIsoBmffBox constructor with tScale = " << tScale
              << " and sidxDuration = " << sidxDuration << std::endl;
    
    // Expect no exception thrown
    EXPECT_NO_THROW({
        player_isobmff::SidxIsoBmffBox sidxBox(fullBox, tScale, sidxDuration);
        std::cout << "SidxIsoBmffBox object created successfully." << std::endl;
    });
    
    std::cout << "Exiting ValidSidxConstruction test" << std::endl;
}
/**
 * @brief Test the construction of SidxIsoBmffBox with zero time scale.
 *
 * This test verifies that the SidxIsoBmffBox object is correctly constructed when the time scale is provided as zero,
 * which is a boundary condition. It checks that no exception is thrown during the creation of the object.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 176@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create FullIsoBmffBox object with valid parameters | boxSize = 100, btype = "sidx", version = 1, flags = 0 | FullIsoBmffBox object instantiated successfully | Should be successful |
 * | 02 | Invoke SidxIsoBmffBox constructor with boundary zero time scale and defined duration | tScale = 0, sidxDuration = 500 | No exception thrown; SidxIsoBmffBox object created | Should Pass |
 */
TEST(player_isobmff_SidxIsoBmffBox, ConstructionWithZeroTimeScale) {
    std::cout << "Entering ConstructionWithZeroTimeScale test" << std::endl;
    
    // Prepare input for FullIsoBmffBox
    uint32_t boxSize = 100;
    char btype[4];
    strncpy(btype, "sidx", 4);
    uint8_t version = 1;
    uint32_t flags = 0;
    
    std::cout << "Creating FullIsoBmffBox with parameters: "
              << "sz = " << boxSize << ", btype = " << std::string(btype, 4)
              << ", ver = " << static_cast<uint32_t>(version)
              << ", f = " << flags << std::endl;
    
    FullIsoBmffBox fullBox(boxSize, btype, version, flags);
    
    uint32_t tScale = 0; // Boundary case: zero timeScale
    uint64_t sidxDuration = 500;
    std::cout << "Invoking SidxIsoBmffBox constructor with tScale = " << tScale
              << " (boundary case) and sidxDuration = " << sidxDuration << std::endl;
    
    // Expect no exception thrown (error status is indicated by internal state, not by exceptions)
    EXPECT_NO_THROW({
        player_isobmff::SidxIsoBmffBox sidxBox(fullBox, tScale, sidxDuration);
        std::cout << "SidxIsoBmffBox object created with zero tScale." << std::endl;
    });
    
    std::cout << "Exiting ConstructionWithZeroTimeScale test" << std::endl;
}
/**
 * @brief Test SidxIsoBmffBox construction with zero sidxDuration to ensure boundary conditions are handled
 *
 * This test verifies that constructing a SidxIsoBmffBox with a zero sidxDuration does not throw an exception.
 * It evaluates the boundary condition for sidxDuration, ensuring that even when the duration is zero,
 * the API handles the situation correctly without triggering exceptions.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 177@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                          | Test Data                                              | Expected Result                                                    | Notes             |
 * | :--------------: | -------------------------------------------------------------------- | ------------------------------------------------------ | ------------------------------------------------------------------ | ----------------- |
 * | 01               | Prepare input parameters and create a FullIsoBmffBox object            | boxSize = 100, btype = "sidx", version = 1, flags = 0    | FullIsoBmffBox instance is created successfully                    | Should be successful |
 * | 02               | Invoke SidxIsoBmffBox constructor with tScale and zero sidxDuration      | tScale = 48000, sidxDuration = 0, FullIsoBmffBox instance reference | SidxIsoBmffBox object is created without throwing an exception       | Should Pass       |
 */
TEST(player_isobmff_SidxIsoBmffBox, ConstructionWithZeroSidxDuration) {
    std::cout << "Entering ConstructionWithZeroSidxDuration test" << std::endl;
    
    // Prepare input for FullIsoBmffBox
    uint32_t boxSize = 100;
    char btype[4];
    strncpy(btype, "sidx", 4);
    uint8_t version = 1;
    uint32_t flags = 0;
    
    std::cout << "Creating FullIsoBmffBox with parameters: "
              << "sz = " << boxSize << ", btype = " << std::string(btype, 4)
              << ", ver = " << static_cast<uint32_t>(version)
              << ", f = " << flags << std::endl;
    
    FullIsoBmffBox fullBox(boxSize, btype, version, flags);
    
    uint32_t tScale = 48000;
    uint64_t sidxDuration = 0; // Boundary case: zero sidxDuration
    std::cout << "Invoking SidxIsoBmffBox constructor with tScale = " << tScale
              << " and sidxDuration = " << sidxDuration << " (boundary case)" << std::endl;
    
    // Expect no exception thrown (error status is indicated by internal state, not by exceptions)
    EXPECT_NO_THROW({
        player_isobmff::SidxIsoBmffBox sidxBox(fullBox, tScale, sidxDuration);
        std::cout << "SidxIsoBmffBox object created with zero sidxDuration." << std::endl;
    });
    
    std::cout << "Exiting ConstructionWithZeroSidxDuration test" << std::endl;
}
/**
 * @brief Validate typical construction of SidxIsoBmffBox without exceptions
 *
 * This test verifies that constructing a SidxIsoBmffBox object with valid, typical values does not throw any exceptions. The purpose of the test is to ensure that the object is created correctly under normal conditions.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 178@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoke SidxIsoBmffBox constructor with parameters (100, 48000, 5000) and verify that no exception is thrown | input1 = 100, input2 = 48000, input3 = 5000 | Constructor should not throw any exception | Should Pass |
 */
TEST(player_isobmff_SidxIsoBmffBox, ValidConstructionTypicalValues) {
    std::cout << "Entering ValidConstructionTypicalValues test" << std::endl;
    EXPECT_NO_THROW({
        std::cout << "Invoking SidxIsoBmffBox(100, 48000, 5000)" << std::endl;
        player_isobmff::SidxIsoBmffBox obj(100, 48000, 5000);
    });
    std::cout << "Exiting ValidConstructionTypicalValues test" << std::endl;
}
/**
 * @brief Test to verify that constructing SidxIsoBmffBox with a zero timeScale throws an invalid_argument exception
 *
 * This test case verifies that the SidxIsoBmffBox constructor throws a std::invalid_argument exception when provided with a zero timeScale value. The test is important because a zero timeScale is considered invalid for constructing the box, and proper error handling is essential for robustness.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 179@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoke SidxIsoBmffBox constructor with size=100, timeScale=0, sidxDuration=5000 and verify that it throws a std::invalid_argument exception | size = 100, timeScale = 0, sidxDuration = 5000 | std::invalid_argument exception is thrown as expected | Should Fail |
 */
TEST(player_isobmff_SidxIsoBmffBox, ConstructionWithZeroTimeScale) {
    std::cout << "Entering ConstructionWithZeroTimeScale test" << std::endl;
    std::cout << "Invoking SidxIsoBmffBox(100, 0, 5000)" << std::endl;
    EXPECT_THROW({
        player_isobmff::SidxIsoBmffBox obj(100, 0, 5000);
    }, std::invalid_argument);
    std::cout << "Exiting ConstructionWithZeroTimeScale test" << std::endl;
}
/**
 * @brief Verify that constructing SidxIsoBmffBox with a zero sidxDuration throws an exception
 *
 * This test ensures that when the SidxIsoBmffBox constructor is invoked with a zero value for the sidxDuration parameter,
 * an std::invalid_argument exception is thrown. This behavior validates that the API correctly handles edge cases where
 * an invalid time parameter is provided.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 180@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Call SidxIsoBmffBox constructor with valid id and time parameters but with sidxDuration set to zero | input1 = 100, input2 = 48000, input3 = 0 | The constructor should throw an std::invalid_argument exception | Should Fail |
 */
TEST(player_isobmff_SidxIsoBmffBox, ConstructionWithZeroSidxDuration) {
    std::cout << "Entering ConstructionWithZeroSidxDuration test" << std::endl;
    std::cout << "Invoking SidxIsoBmffBox(100, 48000, 0)" << std::endl;
    EXPECT_THROW({
        player_isobmff::SidxIsoBmffBox obj(100, 48000, 0);
        // Even if construction continues, log the internal state (hypothetical)
    }, std::invalid_argument);
    std::cout << "Exiting ConstructionWithZeroSidxDuration test" << std::endl;
}
/**
 * @brief Test construction of SidxIsoBmffBox with invalid (zero) size
 *
 * This test verifies that constructing a SidxIsoBmffBox object with a size of zero correctly throws a std::invalid_argument exception. It ensures that the API properly validates input parameters and prevents the creation of an object with an invalid size.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 181@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                     | Test Data                                   | Expected Result                                               | Notes      |
 * | :--------------: | ----------------------------------------------------------------------------------------------- | ------------------------------------------- | ------------------------------------------------------------- | ---------- |
 * | 01               | Invoke SidxIsoBmffBox constructor with zero size, timescale and duration values and expect exception | input: size = 0, timescale = 48000, duration = 5000, output: exception thrown | std::invalid_argument exception thrown as asserted by EXPECT_THROW | Should Fail |
 */
TEST(player_isobmff_SidxIsoBmffBox, ConstructionWithZeroSize) {
    std::cout << "Entering ConstructionWithZeroSize test" << std::endl;
    std::cout << "Invoking SidxIsoBmffBox(0, 48000, 5000)" << std::endl;
    EXPECT_THROW({
        player_isobmff::SidxIsoBmffBox obj(0, 48000, 5000);
    }, std::invalid_argument);
    std::cout << "Exiting ConstructionWithZeroSize test" << std::endl;
}
/**
 * @brief Test constructing SidxIsoBmffBox with valid SIDX box data
 *
 * This test validates that the SidxIsoBmffBox object is constructed successfully when provided with valid SIDX box data. It ensures that the API does not throw exceptions and returns a valid object pointer.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 182@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                 | Test Data                                                                                         | Expected Result                                                                                   | Notes           |
 * | :--------------: | ------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------- | --------------- |
 * | 01               | Initialize test parameters: allocate a buffer of size 32, set to zero, and simulate SIDX data | sz = 32, buffer allocated, expectedTimeScale = 1000, expectedDuration = 50000                      | Buffer is allocated and populated with valid SIDX box data                                        | Should be successful |
 * | 02               | Invoke player_isobmff::SidxIsoBmffBox::constructSidxBox using the valid buffer                | sz = 32, buffer = allocated valid buffer                                                          | API call does not throw exception and returns a valid pointer                                      | Should Pass     |
 * | 03               | Validate the output pointer from constructSidxBox                                           | sidxBox pointer output                                                                            | sidxBox is not a nullptr                                                                          | Should be successful |
 * | 04               | Clean up allocated resources by deleting the buffer                                         | buffer pointer                                                                                    | Memory is deallocated without issues                                                              | Should be successful |
 */
TEST(player_isobmff_SidxIsoBmffBox, ConstructSidxBox_with_valid_SIDX_box_data) {
    std::cout << "Entering ConstructSidxBox with valid SIDX box data test" << std::endl;
    
    // Prepare test parameters
    uint32_t sz = 32;
    // Allocate memory buffer for SIDX box data with size 32 
    uint8_t* buffer = new uint8_t[sz];
    // Initialize buffer to zero
    std::memset(buffer, 0, sz);
    
    // For this test, we simulate encoded values in the buffer:
    // Let's assume that the timeScale is stored at offset 8 and the sample duration at offset 12.
    uint32_t expectedTimeScale = 1000;
    uint64_t expectedDuration = 50000;
    // Use memcpy to place encoded values into the buffer (simulate box data)
    std::memcpy(buffer + 8, &expectedTimeScale, sizeof(uint32_t));
    std::memcpy(buffer + 12, &expectedDuration, sizeof(uint64_t));
    
    std::cout << "Invoking constructSidxBox with sz = " << sz << " and valid buffer pointer" << std::endl;
    player_isobmff::SidxIsoBmffBox* sidxBox = nullptr;
    EXPECT_NO_THROW({
        sidxBox = player_isobmff::SidxIsoBmffBox::constructSidxBox(sz, buffer);
    });
    
    if (sidxBox == nullptr) {
        std::cout << "constructSidxBox returned nullptr unexpectedly." << std::endl;
    } else {
        std::cout << "constructSidxBox returned a valid pointer." << std::endl;
    }
    
    // Clean up allocated buffer
    delete [] buffer;
    std::cout << "Exiting ConstructSidxBox with valid SIDX box data test" << std::endl;
}
/**
 * @brief Verify construction of SidxIsoBmffBox with null pointer input.
 *
 * This test validates that the constructSidxBox API properly handles a null pointer as input by not throwing any exception and returning a nullptr. It confirms the function's robustness against invalid pointer inputs.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 183@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Call constructSidxBox with size 32 and a null pointer as input | sz = 32, buffer = nullptr, output: sidxBox = nullptr | Function should not throw exception and must return nullptr | Should Pass |
 */
TEST(player_isobmff_SidxIsoBmffBox, ConstructSidxBox_with_null_pointer) {
    std::cout << "Entering ConstructSidxBox with null pointer test" << std::endl;
    
    uint32_t sz = 32;
    uint8_t* buffer = nullptr;
    
    std::cout << "Invoking constructSidxBox with sz = " << sz << " and ptr = nullptr" << std::endl;
    player_isobmff::SidxIsoBmffBox* sidxBox = nullptr;
    EXPECT_NO_THROW({
        sidxBox = player_isobmff::SidxIsoBmffBox::constructSidxBox(sz, buffer);
    });
    
    if (sidxBox == nullptr) {
        std::cout << "constructSidxBox correctly returned nullptr for null pointer input." << std::endl;
    } else {
        std::cout << "constructSidxBox did not return nullptr as expected for null pointer input." << std::endl;
    }
    EXPECT_EQ(sidxBox, nullptr);
    
    std::cout << "Exiting ConstructSidxBox with null pointer test" << std::endl;
}
/**
 * @brief Test to verify that SidxIsoBmffBox constructor returns nullptr when provided with a zero box size.
 *
 * This test checks the behavior of player_isobmff::SidxIsoBmffBox::constructSidxBox when invoked with a zero box size.
 * The objective is to ensure that the function does not attempt to process invalid input and correctly returns a null pointer,
 * avoiding potential undefined behavior.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 184@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                              | Test Data                                                       | Expected Result                                                             | Notes           |
 * | :--------------: | ------------------------------------------------------------------------ | --------------------------------------------------------------- | --------------------------------------------------------------------------- | --------------- |
 * | 01              | Initialize test parameters including a zero box size and a valid buffer  | sz = 0, buffer allocated as 32 bytes, all zeros                   | Pre-conditions setup successfully                                           | Should be successful |
 * | 02              | Invoke constructSidxBox API with sz = 0 and valid buffer pointer           | sz = 0, buffer pointer                                            | API does not throw an exception                                             | Should Pass     |
 * | 03              | Validate that the returned pointer from constructSidxBox is nullptr         | output: sidxBox pointer                                           | sidxBox is nullptr as asserted by EXPECT_EQ                                 | Should Pass     |
 * | 04              | Clean up allocated memory                                                | delete [] buffer                                                  | Memory freed without errors                                                 | Should be successful |
 */
TEST(player_isobmff_SidxIsoBmffBox, ConstructSidxBox_with_zero_box_size) {
    std::cout << "Entering ConstructSidxBox with zero box size test" << std::endl;
    
    uint32_t sz = 0;
    // Allocate a valid memory buffer (contents will be ignored)
    uint8_t* buffer = new uint8_t[32];
    std::memset(buffer, 0, 32);
    
    std::cout << "Invoking constructSidxBox with sz = " << sz << " and a valid buffer pointer" << std::endl;
    player_isobmff::SidxIsoBmffBox* sidxBox = nullptr;
    EXPECT_NO_THROW({
        sidxBox = player_isobmff::SidxIsoBmffBox::constructSidxBox(sz, buffer);
    });
    
    if (sidxBox == nullptr) {
        std::cout << "constructSidxBox correctly returned nullptr for zero box size." << std::endl;
    } else {
        std::cout << "constructSidxBox did not return nullptr as expected for zero box size." << std::endl;
    }
    EXPECT_EQ(sidxBox, nullptr);
    
    // Clean up allocated buffer
    delete [] buffer;
    std::cout << "Exiting ConstructSidxBox with zero box size test" << std::endl;
}
/**
 * @brief Validate that getSampleDuration returns the correct duration when using the second constructor.
 *
 * This test verifies that when a SidxIsoBmffBox object is constructed using its second constructor with positive parameters (sz = 10, tScale = 1000, and sidxDuration = 5000), the getSampleDuration method returns the correct value corresponding to the sidxDuration. This confirms proper initialization and functionality of the getter method.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 185
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                          | Test Data                                             | Expected Result                                                                  | Notes       |
 * | :--------------: | ------------------------------------------------------------------------------------ | ----------------------------------------------------- | -------------------------------------------------------------------------------- | ----------- |
 * | 01               | Construct SidxIsoBmffBox using the second constructor, invoke getSampleDuration(), and verify the returned value. | sz = 10, tScale = 1000, sidxDuration = 5000, return = 5000 | getSampleDuration() returns 5000; no exception is thrown and EXPECT_EQ assertion passes | Should Pass |
 */
TEST(player_isobmff_SidxIsoBmffBox, getSampleDuration_secondConstructor_positiveDuration) {
    std::cout << "Entering getSampleDuration_secondConstructor_positiveDuration test" << std::endl;
    
    EXPECT_NO_THROW({
        // Construct SidxIsoBmffBox using the second constructor with parameters: sz = 10, tScale = 1000, sidxDuration = 5000
        player_isobmff::SidxIsoBmffBox sidxBox(10, 1000, 5000);
        std::cout << "Constructed SidxIsoBmffBox with second constructor - parameters: sz = 10, tScale = 1000, sidxDuration = 5000" << std::endl;
        
        // Invoke getSampleDuration() and capture the returned value
        uint64_t sampleDuration = sidxBox.getSampleDuration();
        std::cout << "Invoked getSampleDuration(), returned value: " << sampleDuration << std::endl;
        
        // Check that the returned sample duration matches the expected value 5000
        EXPECT_EQ(sampleDuration, 5000);
    });
    
    std::cout << "Exiting getSampleDuration_secondConstructor_positiveDuration test" << std::endl;
}
/**
 * @brief Verify getSampleDuration returns the correct sample duration when constructed using the first constructor
 *
 * This test verifies that the getSampleDuration API returns the expected sample duration value when a SidxIsoBmffBox object is constructed using the first constructor with a valid FullIsoBmffBox reference. The test ensures that the object is correctly initialized with the predetermined values and that the getSampleDuration method returns 2500 as expected.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 186@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Assign value to btype using strncpy | btype = "abcd" | btype successfully set to "abcd" | Should be successful |
 * | 02 | Construct FullIsoBmffBox with parameters | sz = 12, btype = "abcd", ver = 1, f = 0 | FullIsoBmffBox object constructed successfully | Should be successful |
 * | 03 | Construct SidxIsoBmffBox using the first constructor | Using FullIsoBmffBox reference, tScale = 100, sidxDuration = 2500 | SidxIsoBmffBox object constructed successfully | Should be successful |
 * | 04 | Invoke getSampleDuration() and verify returned value | Invocation on sidxBox, expected sampleDuration = 2500 | Return value equals 2500 via EXPECT_EQ | Should Pass |
 */
TEST(player_isobmff_SidxIsoBmffBox, getSampleDuration_firstConstructor_positiveDuration) {
    std::cout << "Entering getSampleDuration_firstConstructor_positiveDuration test" << std::endl;
    
    EXPECT_NO_THROW({
        // Prepare parameters for FullIsoBmffBox construction
        // Using strncpy to assign value to fixed size array for btype
        char btype[4];
        std::strncpy(btype, "abcd", 4);
        std::cout << "Using strncpy to assign btype with value: abcd" << std::endl;
        
        // Construct a FullIsoBmffBox object with parameters: sz = 12, btype = "abcd", ver = 1, f = 0
        FullIsoBmffBox fullBox(12, btype, 1, 0);
        std::cout << "Constructed FullIsoBmffBox with parameters: sz = 12, btype = \"abcd\", ver = 1, f = 0" << std::endl;
        
        // Construct SidxIsoBmffBox using the first constructor with the FullIsoBmffBox reference, tScale = 100, and sidxDuration = 2500
        player_isobmff::SidxIsoBmffBox sidxBox(fullBox, 100, 2500);
        std::cout << "Constructed SidxIsoBmffBox with first constructor using FullIsoBmffBox reference - parameters: tScale = 100, sidxDuration = 2500" << std::endl;
        
        // Invoke getSampleDuration() and capture the returned value
        uint64_t sampleDuration = sidxBox.getSampleDuration();
        std::cout << "Invoked getSampleDuration(), returned value: " << sampleDuration << std::endl;
        
        // Check that the returned sample duration matches the expected value 2500
        EXPECT_EQ(sampleDuration, 2500);
    });
    
    std::cout << "Exiting getSampleDuration_firstConstructor_positiveDuration test" << std::endl;
}
/**
 * @brief Verify that getTimeScale() returns the correct value when the object is created using the first constructor.
 *
 * This test verifies that when a SidxIsoBmffBox object is created using a FullIsoBmffBox object (constructed with given parameters),
 * the getTimeScale() method returns the expected time scale value. The test confirms that the instantiation does not throw exceptions
 * and that the correct time scale is retrieved.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 187@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create a FullIsoBmffBox object using provided parameters. | sz = 40, btype = "TEST", ver = 1, flags = 0 | FullIsoBmffBox object is created without throwing an exception. | Should be successful |
 * | 02 | Create a SidxIsoBmffBox object using the FullIsoBmffBox object with tScale and sidxDuration. | FullIsoBmffBox object, tScale = 1000, sidxDuration = 5000 | SidxIsoBmffBox object is created without throwing an exception. | Should be successful |
 * | 03 | Invoke getTimeScale() on the SidxIsoBmffBox object and verify the returned value equals tScale. | tScale input = 1000, expected return value = 1000 | getTimeScale() returns 1000, matching the provided tScale. | Should Pass |
 */
TEST(player_isobmff_SidxIsoBmffBox, getTimeScale_returns_correct_value_for_constructor1) {
    std::cout << "Entering getTimeScale_returns_correct_value_for_constructor1 test" << std::endl;

    // Create a FullIsoBmffBox object required for the first constructor.
    // Create a fixed size char array for btype and assign value using strncpy.
    char btype[4];
    strncpy(btype, "TEST", 4);
    const uint32_t sz = 40;
    const uint8_t ver = 1;
    const uint32_t flags = 0;
    std::cout << "Creating FullIsoBmffBox with sz=" << sz << ", btype=" 
              << std::string(btype, 4) << ", ver=" << static_cast<uint32_t>(ver) 
              << ", flags=" << flags << std::endl;
    
    // Assuming FullIsoBmffBox has a valid constructor that accepts these parameters.
    EXPECT_NO_THROW({
        // Object 'fBox' is created using the FullIsoBmffBox constructor.
        player_isobmff::FullIsoBmffBox fBox(sz, btype, ver, flags);
        
        const uint32_t tScale = 1000;
        const uint64_t sidxDuration = 5000;
        std::cout << "Invoking SidxIsoBmffBox constructor with FullIsoBmffBox reference, tScale=" 
                  << tScale << ", sidxDuration=" << sidxDuration << std::endl;
        
        player_isobmff::SidxIsoBmffBox sidxBox(fBox, tScale, sidxDuration);
        
        std::cout << "Invoking getTimeScale() method on SidxIsoBmffBox object" << std::endl;
        uint32_t retTimeScale = sidxBox.getTimeScale();
        std::cout << "getTimeScale() returned value: " << retTimeScale << std::endl;
        EXPECT_EQ(retTimeScale, tScale);
    });

    std::cout << "Exiting getTimeScale_returns_correct_value_for_constructor1 test" << std::endl;
}
/**
 * @brief Verify that the getTimeScale method returns the correct value when the SidxIsoBmffBox is constructed with specific parameters.
 *
 * This test verifies that when a SidxIsoBmffBox object is constructed with a size value of 50, a time scale of 500, and a sidx duration of 3000, calling the getTimeScale() method correctly returns the time scale value (500). The test ensures the constructor properly initializes the internal state used by getTimeScale().
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 188@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                                                         | Test Data                                               | Expected Result                                                                        | Notes       |
 * | :--------------: | --------------------------------------------------------------------------------------------------- | ------------------------------------------------------- | -------------------------------------------------------------------------------------- | ----------- |
 * | 01               | Invoke SidxIsoBmffBox constructor with sz=50, tScale=500, sidxDuration=3000 and call getTimeScale() | sz = 50, tScale = 500, sidxDuration = 3000, output retTimeScale = 500 | getTimeScale() returns 500 and the EXPECT_EQ assertion verifies the correct value is returned | Should Pass |
 */
TEST(player_isobmff_SidxIsoBmffBox, getTimeScale_returns_correct_value_for_constructor2) {
    std::cout << "Entering getTimeScale_returns_correct_value_for_constructor2 test" << std::endl;

    const uint32_t sz = 50;
    const uint32_t tScale = 500;
    const uint64_t sidxDuration = 3000;
    std::cout << "Invoking SidxIsoBmffBox constructor with parameters sz=" << sz 
              << ", tScale=" << tScale << ", sidxDuration=" << sidxDuration << std::endl;
    
    EXPECT_NO_THROW({
        player_isobmff::SidxIsoBmffBox sidxBox(sz, tScale, sidxDuration);
        std::cout << "Invoking getTimeScale() method on SidxIsoBmffBox object" << std::endl;
        uint32_t retTimeScale = sidxBox.getTimeScale();
        std::cout << "getTimeScale() returned value: " << retTimeScale << std::endl;
        EXPECT_EQ(retTimeScale, tScale);
    });

    std::cout << "Exiting getTimeScale_returns_correct_value_for_constructor2 test" << std::endl;
}
/**
 * @brief Verify that setTimeScale updates the time scale parameter with a typical positive value.
 *
 * This test creates a SidxIsoBmffBox object using the custom constructor with initial parameters and then calls the setTimeScale method with a typical positive value. It verifies that the object is created without exceptions and that the setTimeScale method successfully updates the time scale without throwing exceptions.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 189@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                          | Test Data                                                     | Expected Result                                                                           | Notes        |
 * | :--------------: | -------------------------------------------------------------------- | ------------------------------------------------------------- | ----------------------------------------------------------------------------------------- | ------------ |
 * | 01               | Construct SidxIsoBmffBox object using custom constructor with initial parameters. | initialSize = 1024, initialTimeScale = 500, sidxDuration = 2000 | Object is created without throwing exceptions (EXPECT_NO_THROW check).                    | Should Pass  |
 * | 02               | Invoke setTimeScale method with a typical positive value.            | newTimeScale = 1000                                           | setTimeScale executes without throwing exceptions (EXPECT_NO_THROW check).               | Should Pass  |
 */
TEST(player_isobmff_SidxIsoBmffBox, setTimeScale_typical_positive_value) {
    std::cout << "Entering setTimeScale_typical_positive_value test" << std::endl;
    
    // Create an object of player_isobmff::SidxIsoBmffBox using the custom constructor.
    uint32_t initialSize = 1024;
    uint32_t initialTimeScale = 500;
    uint64_t sidxDuration = 2000;
    // Invocation of non-parameterized constructor or custom constructor (EXPECT_NO_THROW to check no exceptions)
    EXPECT_NO_THROW(player_isobmff::SidxIsoBmffBox sbox(initialSize, initialTimeScale, sidxDuration));
    player_isobmff::SidxIsoBmffBox sbox(initialSize, initialTimeScale, sidxDuration);
    std::cout << "Object created with initial timeScale: " << initialTimeScale << std::endl;
    
    // Invoke setTimeScale with a typical positive value.
    uint32_t newTimeScale = 1000;
    std::cout << "Invoking setTimeScale() with value: " << newTimeScale << std::endl;
    EXPECT_NO_THROW(sbox.setTimeScale(newTimeScale));
    std::cout << "setTimeScale() invoked successfully." << std::endl;    
    std::cout << "Exiting setTimeScale_typical_positive_value test" << std::endl;
}
/**
 * @brief Test the valid construction of a player_isobmff::SkipIsoBmffBox object
 *
 * This test verifies that the player_isobmff::SkipIsoBmffBox constructor can be invoked with valid parameters without throwing any exceptions.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 190@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                      | Test Data                                                  | Expected Result                                                              | Notes      |
 * | :--------------: | ------------------------------------------------------------------------------------------------ | ---------------------------------------------------------- | ---------------------------------------------------------------------------- | ---------- |
 * | 01               | Allocate buffer of 8 bytes, set size to 50 and invoke the player_isobmff::SkipIsoBmffBox constructor | sz = 50, buffer = pointer to 8-byte array with all elements set to 0 | Constructor executes without throwing exception and the object is created successfully | Should Pass |
 */
TEST(player_isobmff_SkipIsoBmffBox, ConstructValid) {
    std::cout << "Entering ConstructValid test" << std::endl;
    
    uint32_t sz = 50;
    // Allocate buffer with at least 8 bytes
    uint8_t buffer[8];
    std::memset(buffer, 0, sizeof(buffer));
    std::cout << "Allocated buffer of 8 bytes and set size to " << sz << std::endl;
    
    // Invoke the constructor and log the method invocation along with parameters
    EXPECT_NO_THROW({
        std::cout << "Invoking player_isobmff::SkipIsoBmffBox::SkipIsoBmffBox with sz = " << sz 
                  << " and locn = " << static_cast<void*>(buffer) << std::endl;
        player_isobmff::SkipIsoBmffBox obj(sz, buffer);
        std::cout << "Player skip box object created successfully." << std::endl;
        
    });
    
    std::cout << "Exiting ConstructValid test" << std::endl;
}
/**
 * @brief Validates initialization of TfdtIsoBmffBox with valid common values.
 *
 * This test verifies that constructing a TfdtIsoBmffBox object with a correctly initialized FullIsoBmffBox,
 * along with valid mdt and mdt_loc parameters, does not throw any exception. The test ensures that the API
 * handles typical and expected input for object initialization.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 191@n
 * **Priority:** High@n
 * @n
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * @n
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |@n
 * | :----: | --------- | ---------- |-------------- | ----- |@n
 * | 01 | Setup FullIsoBmffBox parameters | boxSize = 1024, btype = "tfdt", version = 1, flags = 0x0F | FullIsoBmffBox object is created with the provided parameters | Should be successful |@n
 * | 02 | Setup mdt and mdt_loc values | mdt = 1024, mdt_loc = pointer to dummyLocation | mdt and mdt_loc are initialized correctly | Should be successful |@n
 * | 03 | Invoke TfdtIsoBmffBox constructor within exception check | fbox: FullIsoBmffBox, mdt = 1024, mdt_loc = pointer to dummyLocation | No exception is thrown during object construction | Should Pass |
 */
TEST(player_isobmff_TfdtIsoBmffBox, ValidInitializationCommonValues) {
    std::cout << "Entering ValidInitializationCommonValues test" << std::endl;

    // Prepare input values for fbox: box size 1024, btype "tfdt", version 1, flags 0x0F
    uint32_t boxSize = 1024;
    char btype[4];
    std::strncpy(btype, "tfdt", sizeof(btype));
    uint8_t version = 1;
    uint32_t flags = 0x0F;

    // Create a FullIsoBmffBox object with common values.
    FullIsoBmffBox fbox(boxSize, btype, version, flags);
    std::cout << "Created FullIsoBmffBox with boxSize=" << boxSize 
              << ", btype=" << btype 
              << ", version=" << static_cast<uint32_t>(version)
              << ", flags=" << flags << std::endl;

    // Set mdt and allocate a valid memory for mdt_loc.
    uint64_t mdt = 1024;
    uint8_t dummyLocation = 0;
    uint8_t* mdt_loc = &dummyLocation;
    std::cout << "Input mdt=" << mdt << ", mdt_loc=" << static_cast<void*>(mdt_loc) << std::endl;

    // Invoke the constructor and verify that no exception is thrown.
    EXPECT_NO_THROW({
        player_isobmff::TfdtIsoBmffBox obj(fbox, mdt, mdt_loc);
        std::cout << "Constructed TfdtIsoBmffBox object successfully." << std::endl;
    });

    std::cout << "Exiting ValidInitializationCommonValues test" << std::endl;
}
/**
 * @brief Verify that TfdtIsoBmffBox initializes correctly when MDT is zero
 *
 * This test checks that the TfdtIsoBmffBox constructor handles a zero MDT value without throwing exceptions.
 * The test specifically validates that when a FullIsoBmffBox is constructed with given parameters and MDT is set to 0
 * with a valid memory location, the TfdtIsoBmffBox object initializes as expected.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 192@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Prepare input values for the FullIsoBmffBox including boxSize, btype, version, and flags | boxSize = 1024, btype = "tfdt", version = 1, flags = 0x0F | Input values correctly set for further object creation | Should be successful |
 * | 02 | Create a FullIsoBmffBox object using the prepared parameters | fbox: boxSize = 1024, btype = "tfdt", version = 1, flags = 0x0F | FullIsoBmffBox object is constructed successfully | Should be successful |
 * | 03 | Set MDT value to zero and allocate a valid memory location for mdt_loc | mdt = 0, mdt_loc = address of dummyLocation | MDT is set to zero and mdt_loc is valid for use | Should be successful |
 * | 04 | Invoke the TfdtIsoBmffBox constructor with the FullIsoBmffBox object, MDT, and mdt_loc; check for exceptions | fbox, mdt = 0, mdt_loc = valid pointer | No exception is thrown upon construction of TfdtIsoBmffBox | Should Pass |
 */
TEST(player_isobmff_TfdtIsoBmffBox, ValidInitializationZeroMDT) {
    std::cout << "Entering ValidInitializationZeroMDT test" << std::endl;

    // Prepare input values for fbox.
    uint32_t boxSize = 1024;
    char btype[4];
    std::strncpy(btype, "tfdt", sizeof(btype));
    uint8_t version = 1;
    uint32_t flags = 0x0F;
    
    // Create FullIsoBmffBox object.
    FullIsoBmffBox fbox(boxSize, btype, version, flags);
    std::cout << "Created FullIsoBmffBox with boxSize=" << boxSize 
              << ", btype=" << btype 
              << ", version=" << static_cast<uint32_t>(version)
              << ", flags=" << flags << std::endl;

    // Set mdt to zero and allocate valid memory for mdt_loc.
    uint64_t mdt = 0;
    uint8_t dummyLocation = 0;
    uint8_t* mdt_loc = &dummyLocation;
    std::cout << "Input mdt=" << mdt << ", mdt_loc=" << static_cast<void*>(mdt_loc) << std::endl;

    // Invoke the constructor and expect no exceptions.
    EXPECT_NO_THROW({
        player_isobmff::TfdtIsoBmffBox obj(fbox, mdt, mdt_loc);
        std::cout << "Constructed TfdtIsoBmffBox object successfully with mdt 0." << std::endl;
    });

    std::cout << "Exiting ValidInitializationZeroMDT test" << std::endl;
}
/**
 * @brief Verify that the TfdtIsoBmffBox constructor handles a null mdt_loc correctly by throwing an exception.
 *
 * This test verifies that when a null pointer is provided for the mdt_loc parameter during the initialization of a TfdtIsoBmffBox object, the constructor throws the expected exception. This is important to ensure that invalid input is properly detected and handled to prevent undefined behavior.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 193@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                               | Test Data                                                                               | Expected Result                                                  | Notes         |
 * | :--------------: | ------------------------------------------------------------------------- | --------------------------------------------------------------------------------------- | --------------------------------------------------------------- | ------------- |
 * | 01               | Initialize FullIsoBmffBox with valid parameters                           | boxSize=1024, btype="tfdt", version=1, flags=0x0F                                        | FullIsoBmffBox object is created successfully                   | Should be successful |
 * | 02               | Invoke TfdtIsoBmffBox constructor with mdt=500 and mdt_loc set to nullptr     | fbox=created FullIsoBmffBox, mdt=500, mdt_loc=nullptr                                    | The constructor throws std::exception as expected                 | Should Fail   |
 */
TEST(player_isobmff_TfdtIsoBmffBox, NegativeInitializationNullMDTLoc) {
    std::cout << "Entering NegativeInitializationNullMDTLoc test" << std::endl;

    // Prepare input values for fbox.
    uint32_t boxSize = 1024;
    char btype[4];
    std::strncpy(btype, "tfdt", sizeof(btype));
    uint8_t version = 1;
    uint32_t flags = 0x0F;
    
    // Create FullIsoBmffBox object.
    FullIsoBmffBox fbox(boxSize, btype, version, flags);
    std::cout << "Created FullIsoBmffBox with boxSize=" << boxSize 
              << ", btype=" << btype 
              << ", version=" << static_cast<uint32_t>(version)
              << ", flags=" << flags << std::endl;

    // Set mdt and intentionally pass a null pointer for mdt_loc.
    uint64_t mdt = 500;
    uint8_t* mdt_loc = nullptr;
    std::cout << "Input mdt=" << mdt << ", mdt_loc=nullptr" << std::endl;

    // Expect that the constructor throws an exception due to invalid mdt_loc.
    EXPECT_THROW({
        player_isobmff::TfdtIsoBmffBox obj(fbox, mdt, mdt_loc);
        std::cout << "This line should not be reached if exception is thrown." << std::endl;
    }, std::exception);

    std::cout << "Exiting NegativeInitializationNullMDTLoc test" << std::endl;
}
/**
 * @brief Validate construction of TfdtIsoBmffBox with typical values
 *
 * This test verifies that providing typical valid parameters to the TfdtIsoBmffBox constructor does not throw exceptions and its internal state is set correctly.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 194@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                       | Test Data                                                      | Expected Result                                                                 | Notes      |
 * | :--------------: | ----------------------------------------------------------------- | -------------------------------------------------------------- | ------------------------------------------------------------------------------- | ---------- |
 * | 01               | Call TfdtIsoBmffBox constructor with typical values for sz, mdt and mdt_arr, and verify no exception is thrown. | sz = 100, mdt = 12345, mdt_arr[0] = 1, mdt_arr[1-9] = 0           | TfdtIsoBmffBox constructor should not throw an exception; EXPECT_NO_THROW passes. | Should Pass|
 */
TEST(player_isobmff_TfdtIsoBmffBox, ValidConstructionTypicalValues) {
    std::cout << "Entering ValidConstructionTypicalValues test" << std::endl;
    
    uint32_t sz = 100;
    uint64_t mdt = 12345;
    uint8_t mdt_arr[10];
    std::memset(mdt_arr, 0, sizeof(mdt_arr));
    // Set a specific value to show the array initialization
    mdt_arr[0] = 1;
    std::cout << "Invoking TfdtIsoBmffBox with: sz = " << sz 
              << ", mdt = " << mdt 
              << ", mdt_loc pointer = " << static_cast<void*>(mdt_arr)
              << ", first byte = " << static_cast<int>(mdt_arr[0]) << std::endl;
              
    // Construct the object and expect no exception
    EXPECT_NO_THROW({
        player_isobmff::TfdtIsoBmffBox box(sz, mdt, mdt_arr);
        std::cout << "TfdtIsoBmffBox object constructed successfully." << std::endl;
        std::cout << "Internal state expected: baseMDT = " << mdt 
                  << ", baseMDT_loc = " << static_cast<void*>(mdt_arr) << std::endl;
    });
    
    std::cout << "Exiting ValidConstructionTypicalValues test" << std::endl;
}
/**
 * @brief Test that constructing a TfdtIsoBmffBox object with a zero size does not throw an exception.
 *
 * This test verifies that the TfdtIsoBmffBox constructor correctly handles a scenario where the size (sz) is zero.
 * The test ensures that even with a boundary value for size, the object is constructed without throwing exceptions.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 195@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                    | Test Data                                                     | Expected Result                                           | Notes      |
 * | :--------------: | -------------------------------------------------------------- | ------------------------------------------------------------- | --------------------------------------------------------- | ---------- |
 * | 01               | Invoke the TfdtIsoBmffBox constructor with sz = 0, mdt = 12345, and mdt_loc pointing to a byte with value 0 | sz = 0, mdt = 12345, mdt_loc = pointer to uint8_t variable (value = 0) | The constructor is called without throwing an exception and object is successfully created | Should Pass |
 */
TEST(player_isobmff_TfdtIsoBmffBox, ConstructionZeroSz) {
    std::cout << "Entering ConstructionZeroSz test" << std::endl;
    
    uint32_t sz = 0;
    uint64_t mdt = 12345;
    uint8_t mdt_byte = 0;
    uint8_t* mdt_loc = &mdt_byte;
    std::cout << "Invoking TfdtIsoBmffBox with: sz = " << sz 
              << ", mdt = " << mdt 
              << ", mdt_loc pointer = " << static_cast<void*>(mdt_loc)
              << ", pointed value = " << static_cast<int>(*mdt_loc) << std::endl;
    
    EXPECT_NO_THROW({
        player_isobmff::TfdtIsoBmffBox box(sz, mdt, mdt_loc);
        std::cout << "TfdtIsoBmffBox object constructed successfully with sz = 0." << std::endl;
    });
    
    std::cout << "Exiting ConstructionZeroSz test" << std::endl;
}
/**
 * @brief Test the construction of TfdtIsoBmffBox with zero mdt.
 *
 * This test verifies that the constructor of TfdtIsoBmffBox does not throw any exceptions when provided with a valid size, a zero mdt value, and a valid pointer to mdt. The test ensures that the basic functionality of object construction works as expected when mdt is zero.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 196@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                              | Test Data                                                  | Expected Result                                                           | Notes       |
 * | :--------------: | ------------------------------------------------------------------------ | ---------------------------------------------------------- | ------------------------------------------------------------------------- | ----------- |
 * | 01               | Invoke TfdtIsoBmffBox constructor with valid parameters for zero mdt.      | sz = 100, mdt = 0, mdt_loc = valid pointer, pointed_value = 0 | Object constructed successfully without throwing any exceptions.        | Should Pass |
 */
TEST(player_isobmff_TfdtIsoBmffBox, ConstructionZeroMdt) {
    std::cout << "Entering ConstructionZeroMdt test" << std::endl;
    
    uint32_t sz = 100;
    uint64_t mdt = 0;
    uint8_t mdt_byte = 0;
    uint8_t* mdt_loc = &mdt_byte;
    std::cout << "Invoking TfdtIsoBmffBox with: sz = " << sz 
              << ", mdt = " << mdt 
              << ", mdt_loc pointer = " << static_cast<void*>(mdt_loc)
              << ", pointed value = " << static_cast<int>(*mdt_loc) << std::endl;
    
    EXPECT_NO_THROW({
        player_isobmff::TfdtIsoBmffBox box(sz, mdt, mdt_loc);
        std::cout << "TfdtIsoBmffBox object constructed successfully with mdt = 0." << std::endl;
    });
    
    std::cout << "Exiting ConstructionZeroMdt test" << std::endl;
}
/**
 * @brief Verify that constructing TfdtIsoBmffBox with a null pointer for mdt_loc throws an exception.
 *
 * This test case verifies that the TfdtIsoBmffBox constructor properly handles a scenario where the mdt_loc pointer is null, ensuring that an appropriate exception is thrown. This validation is crucial to prevent undefined behavior when invalid memory is accessed.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 197 @n
 * **Priority:** High @n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** None @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Initialize sz, mdt, and set mdt_loc to nullptr, then invoke the TfdtIsoBmffBox constructor | sz = 100, mdt = 12345, mdt_loc = nullptr | API is expected to throw an exception as verified by EXPECT_ANY_THROW | Should Pass |
 */
TEST(player_isobmff_TfdtIsoBmffBox, ConstructionNullMdtLoc) {
    std::cout << "Entering ConstructionNullMdtLoc test" << std::endl;
    
    uint32_t sz = 100;
    uint64_t mdt = 12345;
    uint8_t* mdt_loc = nullptr;
    std::cout << "Invoking TfdtIsoBmffBox with: sz = " << sz 
              << ", mdt = " << mdt 
              << ", mdt_loc pointer = " << static_cast<void*>(mdt_loc) << std::endl;
    
    // It is expected that constructing with a nullptr for mdt_loc will throw an exception.
    EXPECT_ANY_THROW({
        player_isobmff::TfdtIsoBmffBox box(sz, mdt, mdt_loc);
        std::cout << "TfdtIsoBmffBox object constructed unexpectedly with nullptr mdt_loc." << std::endl;
    });
    
    std::cout << "Exiting ConstructionNullMdtLoc test" << std::endl;
}
/**
 * @brief Test ConstructValidTfdtBox for proper creation of a TfdtIsoBmffBox
 *
 * This test verifies that a valid memory buffer simulating a proper Tfdt box is correctly processed
 * by the constructTfdtBox method. It checks that a buffer containing a 64-bit BaseMediaDecodeTime value can
 * be used to create a TfdtIsoBmffBox without throwing any exceptions and that a valid, non-null pointer is returned.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 198@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                                                           | Test Data                                                              | Expected Result                                                | Notes     |
 * | :--------------: | ----------------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------- | -------------------------------------------------------------- | --------- |
 * | 01               | Prepare a valid memory buffer containing a 64-bit BaseMediaDecodeTime value and invoke constructTfdtBox with sz and buffer. | sz = 16, buffer = array with binary representation of expectedMDT = 1234567890 | No exception thrown and returns a non-null pointer to TfdtIsoBmffBox | Should Pass |
 */
TEST(player_isobmff_TfdtIsoBmffBox, ConstructValidTfdtBox) {
    std::cout << "Entering ConstructValidTfdtBox test" << std::endl;
    
    uint32_t sz = 16; 
    uint8_t buffer[16] = {0};

    // Prepare a valid memory buffer; simulate a proper Tfdt box by storing a 64-bit BaseMediaDecodeTime value.
    uint64_t expectedMDT = 1234567890;
    // Use strncpy-like approach: copy binary data into the buffer.
    // Note: using memcpy is more appropriate for binary data.
    memcpy(buffer, &expectedMDT, sizeof(uint64_t));
    std::cout << "Prepared buffer with baseMDT value: " << expectedMDT << std::endl;
    std::cout << "Invoking constructTfdtBox with sz: " << sz << " and pointer: " << static_cast<void*>(buffer) << std::endl;
    
    player_isobmff::TfdtIsoBmffBox* box = nullptr;
    EXPECT_NO_THROW(box = player_isobmff::TfdtIsoBmffBox::constructTfdtBox(sz, buffer));
    
    EXPECT_NE(box, nullptr);    
    std::cout << "Exiting ConstructValidTfdtBox test" << std::endl;
}
/**
 * @brief Test constructTfdtBox with zero size input.
 *
 * This test verifies that calling the constructTfdtBox API with a size of zero and a valid buffer pointer does not generate an exception and returns a null pointer. This ensures that the function correctly handles the edge case where the box size is zero.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 199@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                            | Test Data                                        |Expected Result                                                                              |Notes         |
 * | :----:           | ---------------------------------------------------------------------- | ------------------------------------------------ |-------------------------------------------------------------------------------------------- |--------------|
 * | 01               | Invoke constructTfdtBox with size=0 and a valid buffer pointer         | sz = 0, buffer = uint8_t[16] initialized to zeros  | Function should not throw exception and return a null pointer; EXPECT_EQ(box, nullptr) check passes | Should Pass |
 */
TEST(player_isobmff_TfdtIsoBmffBox, ConstructTfdtBoxWithZeroSize) {
    std::cout << "Entering ConstructTfdtBoxWithZeroSize test" << std::endl;
    
    uint32_t sz = 0;
    uint8_t buffer[16] = {0};
    std::cout << "Invoking constructTfdtBox with zero size: " << sz << " and pointer: " << static_cast<void*>(buffer) << std::endl;
    
    player_isobmff::TfdtIsoBmffBox* box = nullptr;
    EXPECT_NO_THROW(box = player_isobmff::TfdtIsoBmffBox::constructTfdtBox(sz, buffer));
    
    EXPECT_EQ(box, nullptr);
    std::cout << "Exiting ConstructTfdtBoxWithZeroSize test" << std::endl;
}
/**
 * @brief Verify that constructing a TfdtIsoBmffBox with a null buffer returns a null pointer.
 *
 * This test verifies that when the constructTfdtBox API is invoked with a valid size and a null pointer
 * as the buffer, it does not throw an exception and properly returns a nullptr, ensuring robust handling
 * of invalid memory inputs.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 200
 * **Priority:** High
 * 
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 * 
 * **Test Procedure:**
 * | Variation / Step | Description                                                                 | Test Data                                                         | Expected Result                                                      | Notes       |
 * |:----------------:|-----------------------------------------------------------------------------|-------------------------------------------------------------------|----------------------------------------------------------------------|-------------|
 * | 01               | Invoke constructTfdtBox API with a null buffer and a valid size parameter.   | input: sz = 16, buffer = nullptr; output: box uninitialized         | API call does not throw an exception and returns a nullptr.          | Should Pass |
 * | 02               | Validate the returned TfdtIsoBmffBox instance is nullptr.                   | output: box value from API call                                    | box equals nullptr as verified by the assertion.                     | Should Pass |
 */
TEST(player_isobmff_TfdtIsoBmffBox, ConstructTfdtBoxWithNullPointer) {
    std::cout << "Entering ConstructTfdtBoxWithNullPointer test" << std::endl;
    
    uint32_t sz = 16;
    uint8_t* buffer = nullptr;
    std::cout << "Invoking constructTfdtBox with sz: " << sz << " and nullptr" << std::endl;
    
    player_isobmff::TfdtIsoBmffBox* box = nullptr;
    EXPECT_NO_THROW(box = player_isobmff::TfdtIsoBmffBox::constructTfdtBox(sz, buffer));
    
    EXPECT_EQ(box, nullptr);
    std::cout << "Exiting ConstructTfdtBoxWithNullPointer test" << std::endl;
}
/**
 * @brief Validate that getBaseMDT() returns the correct MDT value for a properly initialized object.
 *
 * This test verifies that when a player_isobmff::TfdtIsoBmffBox object is instantiated using the first constructor
 * with a valid BaseMDT value, the getBaseMDT() method returns the correct MDT value. The test ensures that the object
 * creation does not throw an exception and that the subsequent method call returns the expected value.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 201@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                                 | Test Data                                                 | Expected Result                                            | Notes         |
 * | :--------------: | --------------------------------------------------------------------------- | --------------------------------------------------------- | ---------------------------------------------------------- | ------------- |
 * | 01               | Create a player_isobmff::TfdtIsoBmffBox object using the first constructor. | size = 8, mdt = 100, mdt_loc_buffer = valid array pointer  | Object is created without throwing an exception.         | Should Pass   |
 * | 02               | Invoke getBaseMDT() method on the created object.                           | No additional input; uses the constructed object values   | Method call returns the BaseMDT value of 100.              | Should Pass   |
 * | 03               | Validate that the returned BaseMDT value is equal to 100.                   | base_mdt = returned value, expected = 100                | EXPECT_EQ confirms the returned value is exactly 100.      | Should Pass   |
 */
TEST(player_isobmff_TfdtIsoBmffBox, getBaseMDT_Typical)
{
    std::cout << "Entering getBaseMDT_Typical test" << std::endl;
    uint32_t size = 8;
    uint64_t mdt = 100;
    uint8_t mdt_loc_buffer[10] = {0};

    std::cout << "Creating player_isobmff::TfdtIsoBmffBox object using first constructor with sz: " 
              << size << ", mdt: " << mdt 
              << ", and valid mdt_loc pointer" << std::endl;
    EXPECT_NO_THROW(player_isobmff::TfdtIsoBmffBox box(size, mdt, mdt_loc_buffer));
    player_isobmff::TfdtIsoBmffBox box(size, mdt, mdt_loc_buffer);

    std::cout << "Invoking getBaseMDT() method" << std::endl;
    uint64_t base_mdt = box.getBaseMDT();
    std::cout << "Returned BaseMDT value: " << base_mdt << std::endl;

    EXPECT_EQ(base_mdt, 100);
    std::cout << "Exiting getBaseMDT_Typical test" << std::endl;
}
/**
 * @brief Verify that TfdtIsoBmffBox::getBaseMDT returns 0 when MDT is zero
 *
 * This test verifies that when a TfdtIsoBmffBox object is constructed with a zero MDT value and a valid MDT location buffer, the getBaseMDT method correctly returns 0. It ensures that the constructor and the getter function work as expected under these conditions.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 202@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                                                  | Test Data                                                       | Expected Result                                              | Notes      |
 * | :--------------: | -------------------------------------------------------------------------------------------- | --------------------------------------------------------------- | ------------------------------------------------------------ | ---------- |
 * | 01               | Create a TfdtIsoBmffBox object using size = 8, mdt = 0, and a valid mdt_loc_buffer; invoke getBaseMDT() and verify the returned value is 0 | input: size = 8, mdt = 0, mdt_loc_buffer = {0...}; output: base_mdt = 0 | getBaseMDT() should return 0 as verified by EXPECT_EQ         | Should Pass |
 */
TEST(player_isobmff_TfdtIsoBmffBox, getBaseMDT_ZeroMDT)
{
    std::cout << "Entering getBaseMDT_ZeroMDT test" << std::endl;
    uint32_t size = 8;
    uint64_t mdt = 0;
    uint8_t mdt_loc_buffer[10] = {0};

    std::cout << "Creating player_isobmff::TfdtIsoBmffBox object using first constructor with sz: " 
              << size << ", mdt: " << mdt 
              << ", and valid mdt_loc pointer" << std::endl;
    EXPECT_NO_THROW(player_isobmff::TfdtIsoBmffBox box(size, mdt, mdt_loc_buffer));
    player_isobmff::TfdtIsoBmffBox box(size, mdt, mdt_loc_buffer);

    std::cout << "Invoking getBaseMDT() method" << std::endl;
    uint64_t base_mdt = box.getBaseMDT();
    std::cout << "Returned BaseMDT value: " << base_mdt << std::endl;

    EXPECT_EQ(base_mdt, 0);
    std::cout << "Exiting getBaseMDT_ZeroMDT test" << std::endl;
}
/**
 * @brief Test retrieving the base MDT value using a full ISO BMFF box configuration
 *
 * This test constructs a FullIsoBmffBox object and uses it to create a TfdtIsoBmffBox object. It then calls the getBaseMDT() method to retrieve the base MDT value, ensuring that the returned value matches the expected value (750).
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 203
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                               | Test Data                                                                                         | Expected Result                                               | Notes             |
 * | :----:           | ------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------- | ------------------------------------------------------------- | ----------------- |
 * | 01               | Create FullIsoBmffBox object with provided input parameters               | size = 8, btype = "abcd", version = 1, flags = 0                                                  | FullIsoBmffBox instance constructed successfully              | Should be successful |
 * | 02               | Create TfdtIsoBmffBox object using FullIsoBmffBox reference and MDT value   | fullBox object, mdt = 750, mdt_loc_buffer = array of zeros (10 elements)                            | TfdtIsoBmffBox instance constructed successfully              | Should be successful |
 * | 03               | Invoke getBaseMDT() method to retrieve the base MDT value                 | No direct input; uses previously constructed TfdtIsoBmffBox instance                                | Return value equals 750 and assertion check passes            | Should Pass       |
 */
TEST(player_isobmff_TfdtIsoBmffBox, getBaseMDT_UsingFullIsoBmffBox)
{
    std::cout << "Entering getBaseMDT_UsingFullIsoBmffBox test" << std::endl;
    uint32_t size = 8;
    char btype[4];
    strncpy(btype, "abcd", sizeof(btype));
    uint8_t version = 1;
    uint32_t flags = 0;

    std::cout << "Creating player_isobmff::FullIsoBmffBox object using its constructor with sz: " 
              << size << ", btype: " << btype 
              << ", version: " << static_cast<int>(version)
              << ", flags: " << flags << std::endl;
    EXPECT_NO_THROW(player_isobmff::FullIsoBmffBox fullBox(size, btype, version, flags));
    player_isobmff::FullIsoBmffBox fullBox(size, btype, version, flags);

    uint64_t mdt = 750;
    uint8_t mdt_loc_buffer[10] = {0};
    std::cout << "Creating player_isobmff::TfdtIsoBmffBox object using second constructor with FullIsoBmffBox reference, mdt: " 
              << mdt << std::endl;
    EXPECT_NO_THROW(player_isobmff::TfdtIsoBmffBox box(fullBox, mdt, mdt_loc_buffer));
    player_isobmff::TfdtIsoBmffBox box(fullBox, mdt, mdt_loc_buffer);

    std::cout << "Invoking getBaseMDT() method" << std::endl;
    uint64_t base_mdt = box.getBaseMDT();
    std::cout << "Returned BaseMDT value: " << base_mdt << std::endl;

    EXPECT_EQ(base_mdt, 750);
    std::cout << "Exiting getBaseMDT_UsingFullIsoBmffBox test" << std::endl;
}
/**
 * @brief Validate setting the base MDT to zero in TfdtIsoBmffBox
 *
 * This test verifies that the player_isobmff::TfdtIsoBmffBox::setBaseMDT function correctly handles an MDT value of zero. It ensures that the object is constructed properly with an initial MDT of zero and that invoking setBaseMDT with zero does not throw any exceptions.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 204@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Construct a TfdtIsoBmffBox object with an arbitrary size and initial MDT set to zero, using an allocated MDT buffer filled with zeros | size = 42, initial MDT = 0, mdt_buffer = {0,0,0,0,0,0,0,0} | Object constructs without throwing an exception | Should Pass |
 * | 02 | Invoke setBaseMDT with MDT value of zero on the constructed object | mdt = 0 | setBaseMDT executes without throwing an exception | Should Pass |
 */
TEST(player_isobmff_TfdtIsoBmffBox, SetBaseMDTToZero) {
    std::cout << "Entering SetBaseMDTToZero test" << std::endl;
    
    // Prepare a local buffer to act as mdt_loc (assumed fixed size 8 bytes)
    uint8_t mdt_buffer[8] = {0};
    
    // Create an object of player_isobmff::TfdtIsoBmffBox using the custom constructor
    // Passing an arbitrary size value (e.g., 42) and initial mdt value as 0
    EXPECT_NO_THROW(player_isobmff::TfdtIsoBmffBox box(42, 0, mdt_buffer));
    player_isobmff::TfdtIsoBmffBox box(42, 0, mdt_buffer);
    std::cout << "Constructed TfdtIsoBmffBox object with initial mdt = 0 and mdt_buffer allocated" << std::endl;
    
    // Invoke setBaseMDT with mdt = 0
    uint64_t newMdt = 0;
    std::cout << "Invoking setBaseMDT with mdt = " << newMdt << std::endl;
    EXPECT_NO_THROW(box.setBaseMDT(newMdt));
    std::cout << "setBaseMDT invoked successfully with mdt = " << newMdt << std::endl;
    std::cout << "Exiting SetBaseMDTToZero test" << std::endl;
}
/**
 * @brief Validate that setting the base MDT with a positive value executes without error
 *
 * This test verifies that the TfdtIsoBmffBox object can be constructed with valid initial data and that the setBaseMDT method correctly accepts and applies a new MDT value (12345) without throwing exceptions@n
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 205@n
 * **Priority:** High@n
 * @n
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * @n
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                            | Test Data                                                     | Expected Result                                           | Notes       |
 * | :--------------: | ---------------------------------------------------------------------- | ------------------------------------------------------------- | --------------------------------------------------------- | ----------- |
 * | 01               | Prepare a local buffer and construct TfdtIsoBmffBox without exceptions | mdt_buffer = 0,0,0,0,0,0,0,0, size = 42, initial mdt = 0         | Object is constructed successfully without any thrown exception | Should Pass |
 * | 02               | Invoke setBaseMDT with new MDT value 12345                             | newMdt = 12345                                                | setBaseMDT is invoked without throwing exceptions; output MDT updated appropriately | Should Pass |
 */
TEST(player_isobmff_TfdtIsoBmffBox, SetBaseMDTToPositive) {
    std::cout << "Entering SetBaseMDTToPositive test" << std::endl;
    
    // Prepare a local buffer to act as mdt_loc (assumed fixed size 8 bytes)
    uint8_t mdt_buffer[8] = {0};
    
    // Create an object of player_isobmff::TfdtIsoBmffBox using the custom constructor
    // Passing an arbitrary size value (e.g., 42) and initial mdt value as 0
    EXPECT_NO_THROW(player_isobmff::TfdtIsoBmffBox box(42, 0, mdt_buffer));
    player_isobmff::TfdtIsoBmffBox box(42, 0, mdt_buffer);
    std::cout << "Constructed TfdtIsoBmffBox object with initial mdt = 0 and mdt_buffer allocated" << std::endl;
    
    // Invoke setBaseMDT with mdt = 12345
    uint64_t newMdt = 12345;
    std::cout << "Invoking setBaseMDT with mdt = " << newMdt << std::endl;
    EXPECT_NO_THROW(box.setBaseMDT(newMdt));
    std::cout << "setBaseMDT invoked successfully with mdt = " << newMdt << std::endl;   
    std::cout << "Exiting SetBaseMDTToPositive test" << std::endl;
}
/**
 * @brief Validates that constructing a TfhdIsoBmffBox with normal inputs does not throw an exception
 *
 * This test verifies that a TfhdIsoBmffBox can be successfully constructed with valid inputs. It first constructs a FullIsoBmffBox using predetermined parameters and then uses it to invoke the TfhdIsoBmffBox constructor with additional inputs for duration, sample size, and flags. The test ensures that no exceptions are thrown during the construction process.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 206@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |@n
 * | :----: | --------- | ---------- |-------------- | ----- |@n
 * | 01 | Construct FullIsoBmffBox with valid parameters (size, btype, version, flags) | btype = "tfhd", size = 100, version = 1, flags = 0 | FullIsoBmffBox is constructed successfully | Should be successful |@n
 * | 02 | Invoke TfhdIsoBmffBox constructor using the previously constructed FullIsoBmffBox and additional inputs | default_duration = 5000, default_duration_location = pointer to uint8_t value 10, default_sample_size = 1024, flags = 0x01 | TfhdIsoBmffBox is constructed without throwing any exceptions (EXPECT_NO_THROW) | Should Pass |
 */
TEST(player_isobmff_TfhdIsoBmffBox, ValidTfhdIsoBmffBoxConstructionWithNormalInputs) {
    std::cout << "Entering ValidTfhdIsoBmffBoxConstructionWithNormalInputs test" << std::endl;
    
    // Prepare input for FullIsoBmffBox
    char btype[4];
    std::strncpy(btype, "tfhd", 4);
    // Construct FullIsoBmffBox with size 100, btype "tfhd", version 1, and flags 0.
    player_isobmff::FullIsoBmffBox fbox(100, btype, 1, 0);
    std::cout << "Constructed FullIsoBmffBox with sz: 100, btype: " << btype 
              << ", version: 1, flags: " << 0 << std::endl;
    
    // Prepare inputs for TfhdIsoBmffBox constructor
    uint64_t default_duration = 5000;
    uint8_t sampleDurationBuffer = 10;  // valid uint8_t variable
    uint8_t* default_duration_location = &sampleDurationBuffer;
    uint32_t default_sample_size = 1024;
    uint32_t flags = 0x01;
    
    std::cout << "Invoking TfhdIsoBmffBox constructor with:" << std::endl;
    std::cout << "    default_duration = " << default_duration << std::endl;
    std::cout << "    default_duration_location = " << static_cast<void*>(default_duration_location) << std::endl;
    std::cout << "    default_sample_size = " << default_sample_size << std::endl;
    std::cout << "    flags = " << flags << std::endl;
    
    // Invoke constructor and verify no exception is thrown
    EXPECT_NO_THROW({
        player_isobmff::TfhdIsoBmffBox obj(fbox, default_duration, default_duration_location, default_sample_size, flags);
        // Log internal state simulation
        std::cout << "TfhdIsoBmffBox constructed successfully." << std::endl;
    });
    
    std::cout << "Exiting ValidTfhdIsoBmffBoxConstructionWithNormalInputs test" << std::endl;
}
/**
 * @brief Verify construction of TfhdIsoBmffBox with zero duration
 *
 * This test verifies that a TfhdIsoBmffBox object can be constructed successfully with a default duration of zero.
 * It first constructs a FullIsoBmffBox object, then uses it to invoke the TfhdIsoBmffBox constructor ensuring that no exceptions are thrown.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 207@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                             | Test Data                                                                                         | Expected Result                                                   | Notes         |
 * | :----:           | ----------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------- | ----------------------------------------------------------------- | ------------- |
 * | 01               | Construct FullIsoBmffBox object with given parameters                   | sz = 100, btype = "tfhd", version = 1, flags = 0                                                  | FullIsoBmffBox object is created successfully                     | Should be successful |
 * | 02               | Invoke TfhdIsoBmffBox constructor with zero default duration and provided inputs | default_duration = 0, default_duration_location = sampleDurationBuffer (value = 20), default_sample_size = 1024, flags = 0x01 | TfhdIsoBmffBox object is constructed without throwing any exceptions | Should Pass   |
 */
TEST(player_isobmff_TfhdIsoBmffBox, ValidTfhdIsoBmffBoxConstructionWithZeroDuration) {
    std::cout << "Entering ValidTfhdIsoBmffBoxConstructionWithZeroDuration test" << std::endl;
    
    // Prepare input for FullIsoBmffBox
    char btype[4];
    std::strncpy(btype, "tfhd", 4);
    player_isobmff::FullIsoBmffBox fbox(100, btype, 1, 0);
    std::cout << "Constructed FullIsoBmffBox with sz: 100, btype: " << btype 
              << ", version: 1, flags: " << 0 << std::endl;
    
    // Prepare inputs for TfhdIsoBmffBox constructor with default_duration = 0
    uint64_t default_duration = 0;
    uint8_t sampleDurationBuffer = 20;  // valid uint8_t variable
    uint8_t* default_duration_location = &sampleDurationBuffer;
    uint32_t default_sample_size = 1024;
    uint32_t flags = 0x01;
    
    std::cout << "Invoking TfhdIsoBmffBox constructor with:" << std::endl;
    std::cout << "    default_duration = " << default_duration << std::endl;
    std::cout << "    default_duration_location = " << static_cast<void*>(default_duration_location) << std::endl;
    std::cout << "    default_sample_size = " << default_sample_size << std::endl;
    std::cout << "    flags = " << flags << std::endl;
    
    EXPECT_NO_THROW({
        player_isobmff::TfhdIsoBmffBox obj(fbox, default_duration, default_duration_location, default_sample_size, flags);
        std::cout << "TfhdIsoBmffBox constructed successfully with zero duration." << std::endl;
    });
    
    std::cout << "Exiting ValidTfhdIsoBmffBoxConstructionWithZeroDuration test" << std::endl;
}
/**
 * @brief Validate that the TfhdIsoBmffBox constructor throws an exception when provided a null pointer for default_duration_location
 *
 * This test verifies that when a null pointer is passed for the default_duration_location parameter to the TfhdIsoBmffBox constructor, the API correctly responds by throwing an exception. This test ensures that the API properly handles invalid input scenarios to maintain the integrity of the box construction process@n
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 208@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Construct a FullIsoBmffBox and invoke the TfhdIsoBmffBox constructor with default_duration_location set to NULL | fbox: sz=100, btype="tfhd", version=1, flags=0; default_duration = 5000, default_duration_location = NULL, default_sample_size = 1024, flags = 0x01 | The constructor is expected to throw a std::exception indicating invalid input | Should Fail |
 */
TEST(player_isobmff_TfhdIsoBmffBox, NegativeCase_NullDefaultDurationLocation) {
    std::cout << "Entering NegativeCase_NullDefaultDurationLocation test" << std::endl;
    
    // Prepare input for FullIsoBmffBox
    char btype[4];
    std::strncpy(btype, "tfhd", 4);
    player_isobmff::FullIsoBmffBox fbox(100, btype, 1, 0);
    std::cout << "Constructed FullIsoBmffBox with sz: 100, btype: " << btype 
              << ", version: 1, flags: " << 0 << std::endl;
    
    // Prepare inputs for TfhdIsoBmffBox constructor with default_duration_location = NULL
    uint64_t default_duration = 5000;
    uint8_t* default_duration_location = NULL;
    uint32_t default_sample_size = 1024;
    uint32_t flags = 0x01;
    
    std::cout << "Invoking TfhdIsoBmffBox constructor with NULL default_duration_location:" << std::endl;
    std::cout << "    default_duration = " << default_duration << std::endl;
    std::cout << "    default_duration_location = " << default_duration_location << std::endl;
    std::cout << "    default_sample_size = " << default_sample_size << std::endl;
    std::cout << "    flags = " << flags << std::endl;
    
    // Expect the construction to throw an exception due to null pointer
    EXPECT_THROW({
        player_isobmff::TfhdIsoBmffBox obj(fbox, default_duration, default_duration_location, default_sample_size, flags);
    }, std::exception);
    
    std::cout << "Exiting NegativeCase_NullDefaultDurationLocation test" << std::endl;
}
/**
 * @brief Verify that TfhdIsoBmffBox can be constructed with a zero sample size.
 *
 * This test ensures that the TfhdIsoBmffBox API correctly handles the case where the default_sample_size is 0.
 * It first prepares a FullIsoBmffBox instance and then invokes the TfhdIsoBmffBox constructor with a zero
 * default_sample_size. The test verifies that no exceptions are thrown and that the internal state of the
 * created object reflects the input parameters.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 209@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                                 | Test Data                                                                                                      | Expected Result                                                                              | Notes          |
 * | :--------------: | --------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------- | ---------------|
 * | 01               | Create FullIsoBmffBox instance with specified parameters                    | btype = "tfhd", size = 100, version = 1, flags = 0                                                             | FullIsoBmffBox is constructed with correct internal state                                    | Should be successful |
 * | 02               | Invoke TfhdIsoBmffBox constructor with default_sample_size set to 0           | default_duration = 5000, default_duration_location = address of sampleDurationBuffer (value 30), default_sample_size = 0, flags = 0x01 | TfhdIsoBmffBox instance is created without throwing exceptions and internal state matches inputs | Should Pass    |
 */
TEST(player_isobmff_TfhdIsoBmffBox, TfhdIsoBmffBoxConstructionWithZeroSampleSize) {
    std::cout << "Entering TfhdIsoBmffBoxConstructionWithZeroSampleSize test" << std::endl;
    
    // Prepare input for FullIsoBmffBox
    char btype[4];
    std::strncpy(btype, "tfhd", 4);
    player_isobmff::FullIsoBmffBox fbox(100, btype, 1, 0);
    std::cout << "Constructed FullIsoBmffBox with sz: 100, btype: " << btype 
              << ", version: 1, flags: " << 0 << std::endl;
    
    // Prepare inputs for TfhdIsoBmffBox constructor with default_sample_size = 0
    uint64_t default_duration = 5000;
    uint8_t sampleDurationBuffer = 30;  // valid uint8_t variable
    uint8_t* default_duration_location = &sampleDurationBuffer;
    uint32_t default_sample_size = 0;
    uint32_t flags = 0x01;
    
    std::cout << "Invoking TfhdIsoBmffBox constructor with default_sample_size = 0:" << std::endl;
    std::cout << "    default_duration = " << default_duration << std::endl;
    std::cout << "    default_duration_location = " << static_cast<void*>(default_duration_location) << std::endl;
    std::cout << "    default_sample_size = " << default_sample_size << std::endl;
    std::cout << "    flags = " << flags << std::endl;
    
    EXPECT_NO_THROW({
        player_isobmff::TfhdIsoBmffBox obj(fbox, default_duration, default_duration_location, default_sample_size, flags);
        std::cout << "TfhdIsoBmffBox constructed successfully with zero sample size." << std::endl;
        std::cout << "Internal state expected:" << std::endl;
        std::cout << "    default_sample_duration = " << default_duration << std::endl;
        std::cout << "    default_duration_location = " << static_cast<void*>(default_duration_location) << std::endl;
        std::cout << "    default_sample_size = " << default_sample_size << std::endl;
        std::cout << "    flags = " << flags << std::endl;
    });
    
    std::cout << "Exiting TfhdIsoBmffBoxConstructionWithZeroSampleSize test" << std::endl;
}
/**
 * @brief Test the creation of TfhdIsoBmffBox with valid typical non-boundary values
 *
 * This test validates that the TfhdIsoBmffBox constructor can successfully create an object using typical non-boundary input values. The purpose is to ensure that no exceptions are thrown during the creation, verifying that the initialization with valid parameters works as expected.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 210@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | ----------- | --------- | --------------- | ----- |
 * | 01 | Invoke the TfhdIsoBmffBox constructor with valid typical non-boundary values (sz=100, sample_duration=5000, first_sample_duration_loc pointing to sample_data with "ABCDE" content, default_sample_size=150, flags=0x1) | input1 = sz: 100, input2 = sample_duration: 5000, input3 = first_sample_duration_loc: sample_data ("ABCDE" among 0s), input4 = default_sample_size: 150, input5 = flags: 0x1 | The TfhdIsoBmffBox object should be successfully constructed without throwing an exception. | Should Pass |
 */
TEST(player_isobmff_TfhdIsoBmffBox, ValidCreationWithTypicalNonBoundaryValues)
{
    std::cout << "Entering ValidCreationWithTypicalNonBoundaryValues test" << std::endl;

    uint8_t sample_data[10] = {0};
    const char* sampleStr = "ABCDE";
    // Use strncpy to assign value to fixed size array.
    std::strncpy(reinterpret_cast<char*>(sample_data), sampleStr, sizeof(sample_data));
    std::cout << "Invoking TfhdIsoBmffBox constructor with parameters:" << std::endl;
    std::cout << "  sz = 100, sample_duration = 5000, first_sample_duration_loc = " << reinterpret_cast<void*>(sample_data)
              << " (data: " << reinterpret_cast<char*>(sample_data) << "), default_sample_size = 150, flags = 0x1" << std::endl;
    
    EXPECT_NO_THROW({
        player_isobmff::TfhdIsoBmffBox box(100, 5000, sample_data, 150, 0x1);
        std::cout << "TfhdIsoBmffBox object constructed successfully." << std::endl;
    });
    
    std::cout << "Exiting ValidCreationWithTypicalNonBoundaryValues test" << std::endl;
}
/**
 * @brief Verify that the TfhdIsoBmffBox constructor throws an exception when passed a null pointer for the first_sample_duration_loc parameter.
 *
 * This test validates that constructing a TfhdIsoBmffBox object with a null pointer for the first_sample_duration_loc parameter triggers an exception, ensuring that the API correctly handles invalid memory references.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 211@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                                                       | Test Data                                                                                                 | Expected Result                                                                                                               | Notes       |
 * | :--------------: | ------------------------------------------------------------------------------------------------- | --------------------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------------------------------------------- | ----------- |
 * | 01               | Invoke the TfhdIsoBmffBox constructor with a null pointer for the first_sample_duration_loc parameter | sz = 100, sample_duration = 5000, first_sample_duration_loc = nullptr, default_sample_size = 150, flags = 0x1 | The constructor should throw a std::exception to indicate invalid input, and the EXPECT_THROW assertion should pass. | Should Fail |
 */
TEST(player_isobmff_TfhdIsoBmffBox, NegativeCreationWithNullPointer)
{
    std::cout << "Entering NegativeCreationWithNullPointer test" << std::endl;

    std::cout << "Invoking TfhdIsoBmffBox constructor with parameters:" << std::endl;
    std::cout << "  sz = 100, sample_duration = 5000, first_sample_duration_loc = nullptr, default_sample_size = 150, flags = 0x1" << std::endl;
    
    EXPECT_THROW({
        player_isobmff::TfhdIsoBmffBox box(100, 5000, nullptr, 150, 0x1);
    }, std::exception);
    std::cout << "Constructor threw an exception as expected due to null pointer." << std::endl;
    
    std::cout << "Exiting NegativeCreationWithNullPointer test" << std::endl;
}
/**
 * @brief Verify that TfhdIsoBmffBox constructor throws an exception when invoked with a zero box size.
 *
 * This test verifies that when the TfhdIsoBmffBox constructor is called with a box size of zero, it properly throws an exception. The test uses sample data for the first sample duration and other parameters to ensure that the error handling logic for invalid input is robust.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 212@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                     | Test Data                                                                                                                           | Expected Result                      | Notes      |
 * | :--------------: | ----------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------------------------------------------------- | ------------------------------------ | ---------- |
 * | 01               | Invoke TfhdIsoBmffBox constructor with zero box size and other parameters                       | sz = 0, sample_duration = 5000, first_sample_duration_loc = sample_data (data: "ABCDE"), default_sample_size = 150, flags = 0x1    | std::exception is thrown             | Should Fail|
 */
TEST(player_isobmff_TfhdIsoBmffBox, CreationWithZeroSize)
{
    std::cout << "Entering CreationWithZeroSize test" << std::endl;

    uint8_t sample_data[10] = {0};
    const char* sampleStr = "ABCDE";
    std::strncpy(reinterpret_cast<char*>(sample_data), sampleStr, sizeof(sample_data));
    std::cout << "Invoking TfhdIsoBmffBox constructor with parameters:" << std::endl;
    std::cout << "  sz = 0, sample_duration = 5000, first_sample_duration_loc = " << reinterpret_cast<void*>(sample_data) 
              << " (data: " << reinterpret_cast<char*>(sample_data) << "), default_sample_size = 150, flags = 0x1" << std::endl;
    
    EXPECT_THROW({
        player_isobmff::TfhdIsoBmffBox box(0, 5000, sample_data, 150, 0x1);
    }, std::exception);
    std::cout << "Constructor threw an exception as expected due to zero box size." << std::endl;

    std::cout << "Exiting CreationWithZeroSize test" << std::endl;
}
/**
 * @brief Verify that TfhdIsoBmffBox constructor throws an exception when invoked with a zero sample duration.
 *
 * This test validates that when a TfhdIsoBmffBox object is created with sample_duration set to zero, the constructor
 * correctly detects the invalid parameter and throws a std::exception. This behavior is crucial to prevent undefined
 * operations in the media processing pipeline.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 213@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Initialize sample_data array and copy the string "ABCDE" into it. | sample_data = {0,0,0,0,0,0,0,0,0,0}, sampleStr = "ABCDE" | sample_data contains "ABCDE" followed by zeros | Should be successful |
 * | 02 | Call the TfhdIsoBmffBox constructor with parameters: sz = 100, sample_duration = 0, first_sample_duration_loc = sample_data, default_sample_size = 150, flags = 0x1. | sz = 100, sample_duration = 0, first_sample_duration_loc = sample_data, default_sample_size = 150, flags = 0x1 | std::exception is thrown | Should Fail |
 * | 03 | Log the exception message confirming the correct behavior of the constructor. | None | Log indicates that the exception was thrown as expected | Should be successful |
 */
TEST(player_isobmff_TfhdIsoBmffBox, CreationWithZeroDuration)
{
    std::cout << "Entering CreationWithZeroDuration test" << std::endl;

    uint8_t sample_data[10] = {0};
    const char* sampleStr = "ABCDE";
    std::strncpy(reinterpret_cast<char*>(sample_data), sampleStr, sizeof(sample_data));
    std::cout << "Invoking TfhdIsoBmffBox constructor with parameters:" << std::endl;
    std::cout << "  sz = 100, sample_duration = 0, first_sample_duration_loc = " << reinterpret_cast<void*>(sample_data)
              << " (data: " << reinterpret_cast<char*>(sample_data) << "), default_sample_size = 150, flags = 0x1" << std::endl;
    
    EXPECT_THROW({
        player_isobmff::TfhdIsoBmffBox box(100, 0, sample_data, 150, 0x1);
    }, std::exception);
    std::cout << "Constructor threw an exception as expected due to zero sample duration." << std::endl;

    std::cout << "Exiting CreationWithZeroDuration test" << std::endl;
}
/**
 * @brief Validate constructor handling when default sample size is zero
 *
 * This test case verifies that creating a TfhdIsoBmffBox object with a default sample size of zero does not throw an exception. It ensures that the constructor accepts zero for the default_sample_size parameter while initializing the object properly.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 214
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Log the entry into the test function | None | "Entering CreationWithZeroSampleSize test" is printed to the console | Should be successful |
 * | 02 | Initialize sample_data array and copy sample string | sample_data = {0}, sampleStr = "ABCDE" | sample_data array contains "ABCDE" | Should be successful |
 * | 03 | Log constructor parameters | sz = 100, sample_duration = 5000, first_sample_duration_loc = sample_data, default_sample_size = 0, flags = 0x1 | Parameters are logged to the console | Should be successful |
 * | 04 | Invoke TfhdIsoBmffBox constructor with default_sample_size = 0 | Parameters: 100, 5000, sample_data ("ABCDE"), 0, 0x1 | Object is constructed successfully without exceptions | Should Pass |
 * | 05 | Log the exit from the test function | None | "Exiting CreationWithZeroSampleSize test" is printed to the console | Should be successful |
 */
TEST(player_isobmff_TfhdIsoBmffBox, CreationWithZeroSampleSize)
{
    std::cout << "Entering CreationWithZeroSampleSize test" << std::endl;

    uint8_t sample_data[10] = {0};
    const char* sampleStr = "ABCDE";
    std::strncpy(reinterpret_cast<char*>(sample_data), sampleStr, sizeof(sample_data));
    std::cout << "Invoking TfhdIsoBmffBox constructor with parameters:" << std::endl;
    std::cout << "  sz = 100, sample_duration = 5000, first_sample_duration_loc = " << reinterpret_cast<void*>(sample_data)
              << " (data: " << reinterpret_cast<char*>(sample_data) << "), default_sample_size = 0, flags = 0x1" << std::endl;
    
    // Assuming that a default_sample_size of zero is accepted.
    EXPECT_NO_THROW({
        player_isobmff::TfhdIsoBmffBox box(100, 5000, sample_data, 0, 0x1);
        std::cout << "TfhdIsoBmffBox object constructed with default_sample_size = 0 successfully." << std::endl;
    });
    
    std::cout << "Exiting CreationWithZeroSampleSize test" << std::endl;
}
/**
 * @brief Validate TfhdIsoBmffBox creation with zero flags
 *
 * This test validates that the TfhdIsoBmffBox constructor successfully creates an object when provided with a zero flag value. It verifies that no exceptions are thrown during object creation with the minimal set of valid parameters.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 215@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Construct TfhdIsoBmffBox with parameters: sz=100, sample_duration=5000, first_sample_duration_loc=pointer to sample_data ("ABCDE" in first 10 bytes), default_sample_size=150, flags=0 | sz = 100, sample_duration = 5000, first_sample_duration_loc = sample_data (data = "ABCDE"), default_sample_size = 150, flags = 0 | Object constructed successfully without throwing any exception | Should Pass |
 */
TEST(player_isobmff_TfhdIsoBmffBox, CreationWithZeroFlags)
{
    std::cout << "Entering CreationWithZeroFlags test" << std::endl;

    uint8_t sample_data[10] = {0};
    const char* sampleStr = "ABCDE";
    std::strncpy(reinterpret_cast<char*>(sample_data), sampleStr, sizeof(sample_data));
    std::cout << "Invoking TfhdIsoBmffBox constructor with parameters:" << std::endl;
    std::cout << "  sz = 100, sample_duration = 5000, first_sample_duration_loc = " << reinterpret_cast<void*>(sample_data)
              << " (data: " << reinterpret_cast<char*>(sample_data) << "), default_sample_size = 150, flags = 0" << std::endl;
    
    EXPECT_NO_THROW({
        player_isobmff::TfhdIsoBmffBox box(100, 5000, sample_data, 150, 0);
        std::cout << "TfhdIsoBmffBox object constructed successfully with flags set to 0." << std::endl;
    });
    
    std::cout << "Exiting CreationWithZeroFlags test" << std::endl;
}
/**
 * @brief Validate that a proper Tfhd box is constructed successfully.
 *
 * This test verifies that when valid input data is provided, the
 * player_isobmff::TfhdIsoBmffBox::constructTfhdBox API correctly returns a non-null pointer,
 * indicating a successful construction of the Tfhd box.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 216@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                                              | Test Data                                                                                           | Expected Result                                          | Notes        |
 * | :--------------: | ---------------------------------------------------------------------------------------- | --------------------------------------------------------------------------------------------------- | -------------------------------------------------------- | ------------ |
 * | 01               | Prepare valid Tfhd box data by initializing a byte array of size 16 with incremental values | sz = 16, data[0]=1, data[1]=2, ..., data[15]=16                                                       | Valid Tfhd box data is prepared                          | Should be successful |
 * | 02               | Invoke player_isobmff::TfhdIsoBmffBox::constructTfhdBox with the prepared size and data pointer  | input: sz = 16, data pointer = address of valid byte array                                          | API returns a non-null pointer                           | Should Pass  |
 * | 03               | Validate that the returned pointer from the API is non-null using EXPECT_NE                  | output: returned pointer != nullptr                                                                 | Assertion passes confirming valid Tfhd box construction  | Should be successful |
 */
TEST(player_isobmff_TfhdIsoBmffBox, ValidTfhdBoxConstruction)
{
    std::cout << "Entering ValidTfhdBoxConstruction test\n";
    uint32_t sz = 16;
    // Create a valid byte array to simulate a proper Tfhd box
    uint8_t data[16];
    for (uint32_t i = 0; i < sz; i++)
    {
        data[i] = static_cast<uint8_t>(i + 1);
    }
    std::cout << "Preparing valid Tfhd box data. Box size: " << sz << ", data pointer: " << static_cast<void*>(data) << "\n";
    std::cout << "Invoking player_isobmff::TfhdIsoBmffBox::constructTfhdBox with sz = " << sz 
              << " and ptr = " << static_cast<void*>(data) << "\n";
              
    player_isobmff::TfhdIsoBmffBox* box = player_isobmff::TfhdIsoBmffBox::constructTfhdBox(sz, data);
    
    std::cout << "Method returned pointer: " << box << "\n";
    EXPECT_NE(box, nullptr) << "Expected non-null pointer for valid Tfhd box construction.";
    std::cout << "Exiting ValidTfhdBoxConstruction test\n";
}
/**
 * @brief Test constructTfhdBox function with a null pointer input.
 *
 * This test verifies that invoking player_isobmff::TfhdIsoBmffBox::constructTfhdBox with a null pointer
 * for the input data returns a null pointer. Ensuring proper error handling in case of invalid inputs is critical.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 217@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                         | Test Data                                | Expected Result                                                    | Notes      |
 * | :--------------: | ------------------------------------------------------------------- | ---------------------------------------- | ------------------------------------------------------------------ | ---------- |
 * |       01         | Invoke constructTfhdBox with sz = 16 and ptr = nullptr              | sz = 16, ptr = nullptr, output = nullptr | Return value is nullptr and EXPECT_EQ confirms the result is null   | Should Pass |
 */
TEST(player_isobmff_TfhdIsoBmffBox, NullPointerInput)
{
    std::cout << "Entering NullPointerInput test\n";
    uint32_t sz = 16;
    uint8_t* ptr = nullptr;
    
    std::cout << "Invoking player_isobmff::TfhdIsoBmffBox::constructTfhdBox with sz = " << sz 
              << " and ptr = nullptr\n";
              
    player_isobmff::TfhdIsoBmffBox* box = player_isobmff::TfhdIsoBmffBox::constructTfhdBox(sz, ptr);
    
    std::cout << "Method returned pointer: " << box << "\n";
    EXPECT_EQ(box, nullptr) << "Expected null pointer for null pointer input.";
    std::cout << "Exiting NullPointerInput test\n";
}
/**
 * @brief Verify that the constructTfhdBox API returns a null pointer when provided with a zero box size.
 *
 * This test verifies that even when a valid memory block is provided, if the box size is zero, the 
 * player_isobmff::TfhdIsoBmffBox::constructTfhdBox API correctly identifies the input as invalid 
 * and returns a null pointer. This ensures that the API does not process a box with an invalid size.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 218@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Initialize variables; set box size to 0 and data array to all zeros. | sz = 0, data = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0} | Variables are initialized successfully. | Should be successful |
 * | 02 | Invoke player_isobmff::TfhdIsoBmffBox::constructTfhdBox with sz = 0 and data pointer. | input: sz = 0, data pointer = address of data array | Function returns a null pointer indicating invalid input. | Should Fail |
 * | 03 | Assert that the returned pointer is nullptr using EXPECT_EQ. | output: box pointer = result of API call (expected: nullptr) | The assertion passes if the returned pointer is nullptr. | Should be successful |
 */
TEST(player_isobmff_TfhdIsoBmffBox, ZeroBoxSizeProvided)
{
    std::cout << "Entering ZeroBoxSizeProvided test\n";
    uint32_t sz = 0;
    // Even though the memory is valid, the box size is zero
    uint8_t data[16];
    std::memset(data, 0, sizeof(data));
    
    std::cout << "Preparing Tfhd box data with zero size. Box size: " << sz 
              << ", data pointer: " << static_cast<void*>(data) << "\n";
    std::cout << "Invoking player_isobmff::TfhdIsoBmffBox::constructTfhdBox with sz = " << sz 
              << " and ptr = " << static_cast<void*>(data) << "\n";
              
    player_isobmff::TfhdIsoBmffBox* box = player_isobmff::TfhdIsoBmffBox::constructTfhdBox(sz, data);
    
    std::cout << "Method returned pointer: " << box << "\n";
    EXPECT_EQ(box, nullptr) << "Expected null pointer for zero box size input.";
    std::cout << "Exiting ZeroBoxSizeProvided test\n";
}
/**
 * @brief Test to verify that the defaultSampleDurationPresent method returns the expected boolean value when invoked with normal inputs.
 *
 * This test initializes a set of normal input parameters for the TfhdIsoBmffBox constructor and verifies that the defaultSampleDurationPresent()
 * method is invoked without throwing any exceptions. The test ensures that the API handles valid inputs correctly and returns a valid boolean result.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 219@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                                 | Test Data                                                                                                  | Expected Result                                                                                    | Notes             |
 * | :--------------: | --------------------------------------------------------------------------- | ---------------------------------------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------- | ----------------- |
 * | 01               | Initialize test inputs for TfhdIsoBmffBox constructor.                      | sz = 100, sample_duration = 5000, first_sample_duration_loc = "5000", default_sample_size = 256, flags = 1   | Parameters initialized with valid normal inputs.                                                 | Should be successful |
 * | 02               | Create TfhdIsoBmffBox object using the initialized parameters.              | sz = 100, sample_duration = 5000, first_sample_duration_loc = "5000", default_sample_size = 256, flags = 1   | Object is constructed without throwing an exception.                                           | Should Pass       |
 * | 03               | Invoke defaultSampleDurationPresent() method and check its return value.      | Method is called on the constructed object; no additional input.                                           | Method returns the expected boolean value indicating presence of default sample duration without exceptions. | Should Pass       |
 */
TEST(player_isobmff_TfhdIsoBmffBox, defaultSampleDurationPresent_NormalInputs) {
    std::cout << "Entering defaultSampleDurationPresent_NormalInputs test" << std::endl;
    
    uint32_t sz = 100;
    uint64_t sample_duration = 5000;
    uint8_t first_sample_duration_loc[16] = {0};
    const char* durationStr = "5000";
    strncpy(reinterpret_cast<char*>(first_sample_duration_loc), durationStr, sizeof(first_sample_duration_loc) - 1);
    uint32_t default_sample_size = 256;
    uint32_t flags = 1;
    
    std::cout << "Creating TfhdIsoBmffBox with parameters:" << std::endl;
    std::cout << "  sz = " << sz << std::endl;
    std::cout << "  sample_duration = " << sample_duration << std::endl;
    std::cout << "  first_sample_duration_loc (as string) = " << first_sample_duration_loc << std::endl;
    std::cout << "  default_sample_size = " << default_sample_size << std::endl;
    std::cout << "  flags = " << flags << std::endl;
    
    EXPECT_NO_THROW({
        player_isobmff::TfhdIsoBmffBox box(sz, sample_duration, first_sample_duration_loc, default_sample_size, flags);
        std::cout << "Invoking defaultSampleDurationPresent() method" << std::endl;
        
        bool result = box.defaultSampleDurationPresent();
        std::cout << "defaultSampleDurationPresent() returned: " << std::boolalpha << result << std::endl;
    });
    
    std::cout << "Exiting defaultSampleDurationPresent_NormalInputs test" << std::endl;
}
/**
 * @brief Verify that getDefaultSampleDuration returns the correct non-zero default sample duration
 *
 * This test verifies that the getDefaultSampleDuration method of the player_isobmff::TfhdIsoBmffBox class returns
 * the non-zero default sample duration provided during object construction. It ensures that the object correctly 
 * retains and returns the sample duration value, and no exceptions are thrown during the process.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 220@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Construct a player_isobmff::TfhdIsoBmffBox object with a given set of input parameters | sz = 100, sample_duration = 500, first_sample_duration_loc = "500", default_sample_size = 1024, flags = 1 | Object is successfully constructed without throwing any exception | Should be successful |
 * | 02 | Invoke getDefaultSampleDuration() on the constructed object and verify the returned value | Invocation of getDefaultSampleDuration(), expected return value = 500 | Returned value equals sample_duration (500) and no exception is thrown | Should Pass |
 */
TEST(player_isobmff_TfhdIsoBmffBox, getDefaultSampleDuration_returns_correct_non_zero_default_sample_duration) {
    std::cout << "Entering getDefaultSampleDuration_returns_correct_non_zero_default_sample_duration test" << std::endl;
    
    // Setup test values
    uint32_t sz = 100;
    uint64_t sample_duration = 500;
    uint8_t first_sample_duration_loc[10] = {0};
    const char* initStr = "500";
    // Using strncpy to assign value to fixed size array
    std::strncpy(reinterpret_cast<char*>(first_sample_duration_loc), initStr, sizeof(first_sample_duration_loc) - 1);
    uint32_t default_sample_size = 1024;
    uint32_t flags = 1;
    
    std::cout << "Constructing object player_isobmff::TfhdIsoBmffBox with argument values:" << std::endl;
    std::cout << "  sz: " << sz << std::endl;
    std::cout << "  sample_duration: " << sample_duration << std::endl;
    std::cout << "  first_sample_duration_loc (buffer initialized with): " << first_sample_duration_loc << std::endl;
    std::cout << "  default_sample_size: " << default_sample_size << std::endl;
    std::cout << "  flags: " << flags << std::endl;
    
    EXPECT_NO_THROW({
        player_isobmff::TfhdIsoBmffBox box(sz, sample_duration, first_sample_duration_loc, default_sample_size, flags);
        std::cout << "Invoking getDefaultSampleDuration() method on constructed object" << std::endl;
        
        uint64_t retVal = 0;
        EXPECT_NO_THROW({
            retVal = box.getDefaultSampleDuration();
        });
        
        std::cout << "Retrieved default sample duration: " << retVal << std::endl;
        EXPECT_EQ(retVal, sample_duration);
    });
    
    std::cout << "Exiting getDefaultSampleDuration_returns_correct_non_zero_default_sample_duration test" << std::endl;
}
/**
 * @brief Verify that getDefaultSampleSize returns the expected non-zero default sample size
 *
 * This test constructs a TfhdIsoBmffBox object using the first constructor with a non-zero default sample size.
 * It then invokes the getDefaultSampleSize method to confirm that the returned value matches the expected default sample size.
 * This ensures that the API function correctly handles and returns the provided default sample size.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 221@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                  | Test Data                                                                                      | Expected Result                                               | Notes      |
 * | :--------------: | -------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------------------------------- | ------------------------------------------------------------- | ---------- |
 * | 01               | Construct TfhdIsoBmffBox using the first constructor and call getDefaultSampleSize             | sz = 100, sample_duration = 2000, first_sample_duration_loc = [0,0,0,0,0,0,0,0,0,0], default_sample_size = 1024, flags = 0, retValue (output) | retValue equals 1024; assertion EXPECT_EQ passes              | Should Pass |
 */
TEST(player_isobmff_TfhdIsoBmffBox, getDefaultSampleSize_nonZero_firstConstructor) {
    std::cout << "Entering getDefaultSampleSize_nonZero_firstConstructor test" << std::endl;
    
    // Create a dummy buffer for first_sample_duration_loc
    uint8_t sampleDurationLoc[10] = {0};
    
    // Expected default sample size value provided to constructor
    uint32_t expectedDefaultSampleSize = 1024;
    
    // Construct TfhdIsoBmffBox object using the first constructor.
    // Arguments: (uint32_t sz, uint64_t sample_duration, uint8_t* first_sample_duration_loc, uint32_t default_sample_size, uint32_t flags)
    EXPECT_NO_THROW({
        player_isobmff::TfhdIsoBmffBox box(100, 2000, sampleDurationLoc, expectedDefaultSampleSize, 0);
        
        std::cout << "Invoking getDefaultSampleSize() on object constructed using first constructor" << std::endl;
        uint32_t retValue = box.getDefaultSampleSize();
        std::cout << "getDefaultSampleSize() returned: " << retValue << std::endl;
        
        EXPECT_EQ(retValue, expectedDefaultSampleSize);
    });
    
    std::cout << "Exiting getDefaultSampleSize_nonZero_firstConstructor test" << std::endl;
}
/**
 * @brief Validate that the getDefaultSampleSize method returns the correct non-zero value when the TfhdIsoBmffBox is constructed using the second constructor.
 *
 * This test constructs a FullIsoBmffBox and then uses it to create a TfhdIsoBmffBox with a specific default sample size. It then invokes getDefaultSampleSize() to verify that the default sample size is returned correctly. The test ensures that when a non-zero default sample size is provided during construction, the getDefaultSampleSize() method correctly reflects that value.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 222@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Construct FullIsoBmffBox using provided parameters as required by the second constructor. | sz = 100, btype = "abcd", ver = 1, flags = 0x55 | FullIsoBmffBox constructed successfully | Should be successful |
 * | 02 | Create a dummy buffer for default_duration_location initialized to zeros. | default_duration_location = [0,0,0,0,0,0,0,0,0,0] | Buffer created with zeros | Should be successful |
 * | 03 | Construct TfhdIsoBmffBox using the second constructor with pre-defined default parameters. | fullBox, default_duration = 3000, default_duration_location, default_sample_size = 512, flags = 0 | TfhdIsoBmffBox instance created successfully | Should Pass |
 * | 04 | Invoke getDefaultSampleSize() on the constructed TfhdIsoBmffBox and validate the returned default sample size. | operation: getDefaultSampleSize(), expectedDefaultSampleSize = 512 | Return value equals 512 | Should Pass |
 */
TEST(player_isobmff_TfhdIsoBmffBox, getDefaultSampleSize_nonZero_secondConstructor) {
    std::cout << "Entering getDefaultSampleSize_nonZero_secondConstructor test" << std::endl;
    
    // Prepare a fixed-size array for btype and assign value using strncpy
    char btype[4];
    std::memset(btype, 0, sizeof(btype));
    std::strncpy(btype, "abcd", sizeof(btype));

    // Construct a FullIsoBmffBox instance required for the second constructor.
    // Arguments for FullIsoBmffBox: (uint32_t sz, const char btype[4], uint8_t ver, uint32_t f)
    EXPECT_NO_THROW({
        player_isobmff::FullIsoBmffBox fullBox(100, btype, 1, 0x55);
        
        // Create a dummy buffer for default_duration_location.
        uint8_t defaultDurationLoc[10] = {0};
        
        // Expected default sample size value provided to constructor
        uint32_t expectedDefaultSampleSize = 512;
        
        // Construct TfhdIsoBmffBox object using the second constructor.
        // Arguments: (FullIsoBmffBox &fbox, uint64_t default_duration, uint8_t* default_duration_location, uint32_t default_sample_size, uint32_t flags)
        player_isobmff::TfhdIsoBmffBox box(fullBox, 3000, defaultDurationLoc, expectedDefaultSampleSize, 0);
        
        std::cout << "Invoking getDefaultSampleSize() on object constructed using second constructor" << std::endl;
        uint32_t retValue = box.getDefaultSampleSize();
        std::cout << "getDefaultSampleSize() returned: " << retValue << std::endl;
        
        EXPECT_EQ(retValue, expectedDefaultSampleSize);
    });
    
    std::cout << "Exiting getDefaultSampleSize_nonZero_secondConstructor test" << std::endl;
}
/**
 * @brief Validate the functionality of setting a non-zero default sample duration.
 *
 * This test verifies that the player_isobmff::TfhdIsoBmffBox object correctly handles setting a non-zero default sample duration by calling the setDefaultSampleDuration method. The test ensures that the object is instantiated without exceptions and that invoking setDefaultSampleDuration with a valid non-zero value (1000) does not throw any exception.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 223@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                                                          | Test Data                                                                                                                         | Expected Result                                   | Notes       |
 * | :--------------: | ---------------------------------------------------------------------------------------------------- | --------------------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------- | ----------- |
 * | 01               | Construct the TfhdIsoBmffBox object with an initial sample duration of 0 and a default duration buffer. | boxSize = 100, initialSampleDuration = 0, defaultDurationBuffer = {0,0,0,0,0,0,0,0}, defaultSampleSize = 0, flags = 0                       | Object is created without throwing an exception   | Should Pass |
 * | 02               | Invoke setDefaultSampleDuration on the created object with sample_duration = 1000.                    | sample_duration = 1000                                                                                                             | Method call does not throw any exception           | Should Pass |
 */
TEST(player_isobmff_TfhdIsoBmffBox, SetDefaultSampleDurationWithNonZeroValue) {
    std::cout << "Entering SetDefaultSampleDurationWithNonZeroValue test" << std::endl;
    
    // Allocate memory for default_sample_duration_location (8 bytes for uint64_t)
    uint8_t defaultDurationBuffer[8] = {0};
    
    // Setup initial parameters for the object creation
    uint32_t boxSize = 100; 
    uint64_t initialSampleDuration = 0; // Initial value before setting default sample duration
    uint32_t defaultSampleSize = 0;
    uint32_t flags = 0;
    
    // Construct the TfhdIsoBmffBox object using non-parameterized values for other members.
    EXPECT_NO_THROW({
        player_isobmff::TfhdIsoBmffBox box(boxSize, initialSampleDuration, defaultDurationBuffer, defaultSampleSize, flags);
        
        std::cout << "Created player_isobmff::TfhdIsoBmffBox object with initialSampleDuration = " 
                  << initialSampleDuration << std::endl;
        std::cout << "Invoking setDefaultSampleDuration with sample_duration = 1000" << std::endl;
        
        // Invoke the method setDefaultSampleDuration
        EXPECT_NO_THROW({
            box.setDefaultSampleDuration(1000);
        });
        
    });
    
    std::cout << "Exiting SetDefaultSampleDurationWithNonZeroValue test" << std::endl;
}
/**
 * @brief Validate behavior of setDefaultSampleDuration when provided with a zero value.
 *
 * This test ensures that the TfhdIsoBmffBox object's method setDefaultSampleDuration properly handles a zero value without throwing exceptions. The objective is to verify that calling setDefaultSampleDuration with a zero input does not disrupt the object's state or execution flow.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 224@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | -------------- | ----- |
 * | 01 | Initialize test parameters and allocate memory for default sample duration buffer. | defaultDurationBuffer = {0}, boxSize = 100, initialSampleDuration = 1234, defaultSampleSize = 0, flags = 0 | Parameters are set without issues. | Should be successful |
 * | 02 | Construct the TfhdIsoBmffBox object using the initialized parameters. | boxSize = 100, initialSampleDuration = 1234, defaultDurationBuffer = allocated buffer, defaultSampleSize = 0, flags = 0 | Object is created successfully without throwing an exception. | Should Pass |
 * | 03 | Invoke the setDefaultSampleDuration method with sample_duration = 0. | sample_duration = 0 | Method call executes without exceptions. | Should Pass |
 */
TEST(player_isobmff_TfhdIsoBmffBox, SetDefaultSampleDurationWithZeroValue) {
    std::cout << "Entering SetDefaultSampleDurationWithZeroValue test" << std::endl;
    
    // Allocate memory for default_sample_duration_location (8 bytes for uint64_t)
    uint8_t defaultDurationBuffer[8] = {0};
    
    // Setup initial parameters for the object creation
    uint32_t boxSize = 100; 
    uint64_t initialSampleDuration = 1234; // Some non-zero initial value to see the change
    uint32_t defaultSampleSize = 0;
    uint32_t flags = 0;
    
    // Construct the TfhdIsoBmffBox object using provided constructor.
    EXPECT_NO_THROW({
        player_isobmff::TfhdIsoBmffBox box(boxSize, initialSampleDuration, defaultDurationBuffer, defaultSampleSize, flags);
        
        std::cout << "Created player_isobmff::TfhdIsoBmffBox object with initialSampleDuration = " 
                  << initialSampleDuration << std::endl;
        std::cout << "Invoking setDefaultSampleDuration with sample_duration = 0" << std::endl;
        
        // Invoke the method setDefaultSampleDuration with zero value.
        EXPECT_NO_THROW({
            box.setDefaultSampleDuration(0);
        });
        
    });
    
    std::cout << "Exiting SetDefaultSampleDurationWithZeroValue test" << std::endl;
}
/**
 * @brief Validate the creation of player_isobmff::TrakIsoBmffBox with a valid typical size.
 *
 * This test checks that the player_isobmff::TrakIsoBmffBox constructor functions correctly when provided with a valid size (100) and that it does not throw any exception during object creation.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 225
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- | -------------- | ----- |
 * | 01 | Call the player_isobmff::TrakIsoBmffBox constructor with a valid size and verify that no exception is thrown | inputSize = 100 | Constructor executes without throwing an exception | Should Pass |
 */
TEST(player_isobmff_TrakIsoBmffBox, TrakIsoBmffBox_uint32_t_sz_start) {
    std::cout << "Entering TrakIsoBmffBox_uint32_t_sz_start test" << std::endl;
    
    // Test Case: Valid typical size (e.g., 100)
    uint32_t inputSize = 100;
    std::cout << "Invoking player_isobmff::TrakIsoBmffBox constructor with size: " << inputSize << std::endl;
    
    // Construct the object and expect no exception
    EXPECT_NO_THROW({
        player_isobmff::TrakIsoBmffBox trakBox(inputSize);
        std::cout << "player_isobmff::TrakIsoBmffBox constructor invoked successfully." << std::endl;
        
    });
    
    std::cout << "Exiting TrakIsoBmffBox_uint32_t_sz_start test" << std::endl;
}
/**
 * @brief Verify constructTrakBox returns a valid pointer when provided with valid input and default newTrackId override.
 *
 * This test verifies that the API player_isobmff::TrakIsoBmffBox::constructTrakBox successfully creates a TrakIsoBmffBox object when invoked with valid parameters including a default newTrackId (-1). The test ensures no exceptions are thrown and the returned pointer is non-null.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 226@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Initialize input buffer and test parameters with valid dummy data. | input1 = sz: 100, input2 = buffer initialized with "DUMMY_DATA_FOR_TEST", input3 = newTrackId: -1 | Buffer is correctly prepared and contains the dummy data without errors. | Should be successful |
 * | 02 | Invoke constructTrakBox and validate its successful execution. | input1 = sz: 100, input2 = pointer to buffer, input3 = newTrackId: -1; output1 = pointer returned by constructTrakBox | API call does not throw an exception and returns a non-null pointer. | Should Pass |
 */
TEST(player_isobmff_TrakIsoBmffBox, ValidInputDefaultNewTrackId) {
    std::cout << "Entering ValidInputDefaultNewTrackId test" << std::endl;

    uint32_t sz = 100;
    uint8_t buffer[100];
    const char* dummyData = "DUMMY_DATA_FOR_TEST";
    // Using strncpy to assign to fixed size array
    std::memset(buffer, 0, sizeof(buffer));
    std::strncpy(reinterpret_cast<char*>(buffer), dummyData, sizeof(buffer) - 1);

    std::cout << "Invoking constructTrakBox with sz: " << sz 
              << ", ptr: " << static_cast<void*>(buffer)
              << ", newTrackId: " << -1 << std::endl;

    player_isobmff::TrakIsoBmffBox* trakBox = nullptr;
    EXPECT_NO_THROW({
        trakBox = player_isobmff::TrakIsoBmffBox::constructTrakBox(sz, buffer, -1);
    });
    std::cout << "Method returned pointer: " << trakBox << std::endl;

    EXPECT_NE(trakBox, nullptr);
    std::cout << "Exiting ValidInputDefaultNewTrackId test" << std::endl;
}
/**
 * @brief Verifies that a valid input buffer and override new track ID correctly initialize TrakIsoBmffBox.
 *
 * This test verifies that when a valid buffer, its size, and a new track ID are provided to the
 * constructTrakBox function, the method does not throw an exception and returns a valid (non-null)
 * TrakIsoBmffBox object.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 227@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoke constructTrakBox with a valid buffer filled with "DUMMY_DATA_FOR_OVERRIDE_TEST", a size (150), and newTrackId (10) | input: sz = 150, buffer = "DUMMY_DATA_FOR_OVERRIDE_TEST", newTrackId = 10; output: trakBox pointer | Returns a non-null pointer to a TrakIsoBmffBox object without throwing any exceptions | Should Pass |
 */
TEST(player_isobmff_TrakIsoBmffBox, ValidInputWithOverride) {
    std::cout << "Entering ValidInputWithOverride test" << std::endl;

    uint32_t sz = 150;
    uint8_t buffer[150];
    const char* dummyData = "DUMMY_DATA_FOR_OVERRIDE_TEST";
    std::memset(buffer, 0, sizeof(buffer));
    std::strncpy(reinterpret_cast<char*>(buffer), dummyData, sizeof(buffer) - 1);

    int newTrackId = 10;
    std::cout << "Invoking constructTrakBox with sz: " << sz 
              << ", ptr: " << static_cast<void*>(buffer)
              << ", newTrackId: " << newTrackId << std::endl;

    player_isobmff::TrakIsoBmffBox* trakBox = nullptr;
    EXPECT_NO_THROW({
        trakBox = player_isobmff::TrakIsoBmffBox::constructTrakBox(sz, buffer, newTrackId);
    });
    std::cout << "Method returned pointer: " << trakBox << std::endl;

    EXPECT_NE(trakBox, nullptr);
    std::cout << "Exiting ValidInputWithOverride test" << std::endl;
}
/**
 * @brief Verify constructTrakBox with invalid input (null pointer)
 *
 * This test verifies that invoking constructTrakBox with a null pointer does not throw an exception and correctly returns a nullptr. This ensures that the API handles invalid input gracefully.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 228@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                    | Test Data                         | Expected Result                               | Notes       |
 * | :--------------: | -------------------------------------------------------------- | --------------------------------- | --------------------------------------------- | ----------- |
 * | 01               | Invoke constructTrakBox with sz=100, ptr=nullptr, newTrackId=-1  | sz=100, ptr=nullptr, newTrackId=-1 | API does not throw; returns nullptr           | Should Pass |
 */
TEST(player_isobmff_TrakIsoBmffBox, InvalidInputNullPtr) {
    std::cout << "Entering InvalidInputNullPtr test" << std::endl;

    uint32_t sz = 100;
    uint8_t* ptr = nullptr;
    std::cout << "Invoking constructTrakBox with sz: " << sz 
              << ", ptr: " << ptr
              << ", newTrackId: " << -1 << std::endl;

    player_isobmff::TrakIsoBmffBox* trakBox = nullptr;
    EXPECT_NO_THROW({
        trakBox = player_isobmff::TrakIsoBmffBox::constructTrakBox(sz, ptr, -1);
    });
    std::cout << "Method returned pointer: " << trakBox << std::endl;

    EXPECT_EQ(trakBox, nullptr);

    std::cout << "Exiting InvalidInputNullPtr test" << std::endl;
}
/**
 * @brief Validate that constructTrakBox correctly handles a zero size input.
 *
 * This test verifies that when constructTrakBox is invoked with a size of 0, a valid non-null buffer containing dummy data, 
 * and a newTrackId of -1, it does not throw an exception and returns a nullptr. The objective is to ensure that the API 
 * gracefully handles edge cases where the input size is zero.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 229@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**
 * | Variation / Step | Description                                                     | Test Data                                                              | Expected Result                                                                 | Notes       |
 * | :--------------: | --------------------------------------------------------------- | ---------------------------------------------------------------------- | ------------------------------------------------------------------------------- | ----------- |
 * | 01               | Initialize input parameters with zero size and valid dummy buffer | sz = 0, buffer = "DUMMY_DATA_FOR_ZERO_SIZE_TEST", newTrackId = -1         | API should return a nullptr without throwing an exception                     | Should Pass |
 * | 02               | Invoke constructTrakBox and verify the return value                | sz = 0, buffer pointer to the initialized buffer, newTrackId = -1       | Returned pointer is nullptr as expected, confirming proper error handling       | Should Pass |
 */
TEST(player_isobmff_TrakIsoBmffBox, InvalidInputZeroSize) {
    std::cout << "Entering InvalidInputZeroSize test" << std::endl;

    uint32_t sz = 0;
    uint8_t buffer[100];
    const char* dummyData = "DUMMY_DATA_FOR_ZERO_SIZE_TEST";
    std::memset(buffer, 0, sizeof(buffer));
    std::strncpy(reinterpret_cast<char*>(buffer), dummyData, sizeof(buffer) - 1);

    std::cout << "Invoking constructTrakBox with sz: " << sz 
              << ", ptr: " << static_cast<void*>(buffer)
              << ", newTrackId: " << -1 << std::endl;

    player_isobmff::TrakIsoBmffBox* trakBox = nullptr;
    EXPECT_NO_THROW({
        trakBox = player_isobmff::TrakIsoBmffBox::constructTrakBox(sz, buffer, -1);
    });
    std::cout << "Method returned pointer: " << trakBox << std::endl;

    EXPECT_EQ(trakBox, nullptr);

    std::cout << "Exiting InvalidInputZeroSize test" << std::endl;
}
/**
 * @brief Verify that getTrack_Id() returns the correct track identifier without throwing exceptions
 *
 * This test constructs a player_isobmff::TrakIsoBmffBox with a specified size and then calls getTrack_Id() to retrieve the track identifier.
 * It ensures that no exceptions are thrown during both the construction and the method invocation.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 230@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:** 
 * | Variation / Step | Description                                                       | Test Data                                          | Expected Result                                             | Notes       |
 * | :--------------: | ----------------------------------------------------------------- | -------------------------------------------------- | ----------------------------------------------------------- | ----------- |
 * | 01               | Construct player_isobmff::TrakIsoBmffBox object with size 100       | constructor input: size = 100                      | Object is constructed successfully without exceptions       | Should Pass |
 * | 02               | Invoke getTrack_Id() method to retrieve the track identifier         | method call: getTrack_Id(), returns uint32_t value   | Returns the correct track ID without throwing exceptions     | Should Pass |
 */
TEST(player_isobmff_TrakIsoBmffBox, getTrack_Id_start) {
    std::cout << "Entering getTrack_Id_start test" << std::endl;

    // Construct TrakIsoBmffBox object with a chosen size (e.g., 100)
    EXPECT_NO_THROW({
        player_isobmff::TrakIsoBmffBox box(100);
        std::cout << "Constructed player_isobmff::TrakIsoBmffBox with size 100" << std::endl;

        // Invoke getTrack_Id() method and log the invocation details
        std::cout << "Invoking getTrack_Id() method" << std::endl;
        uint32_t retrievedTrackId = box.getTrack_Id();
        std::cout << "getTrack_Id() returned: " << retrievedTrackId << std::endl;
    });

    std::cout << "Exiting getTrack_Id_start test" << std::endl;
}
/**
 * @brief Validate successful construction of TrunIsoBmffBox using typical valid parameters.
 *
 * This test verifies that constructing a player_isobmff::TrunIsoBmffBox object with typical valid parameters does not throw an exception and the underlying FullIsoBmffBox instance is correctly instantiated.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 231@n
 * **Priority:** High@n
 * @n
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * @n
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                                                          | Test Data                                                                                                                         | Expected Result                                                         | Notes             |
 * | :--------------: | ---------------------------------------------------------------------------------------------------- | --------------------------------------------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------- | ----------------- |
 * | 01               | Create FullIsoBmffBox instance with valid parameters                                                 | input: sz = 100, btype = "trun", version = 1, boxFlags = 0                                                                        | FullIsoBmffBox is constructed successfully                              | Should be successful |
 * | 02               | Invoke player_isobmff::TrunIsoBmffBox constructor with valid parameters using the created FullIsoBmffBox | input: sampleDuration = 1000, sampleCount = 5, firstSampleSize = 1024, trunFlags = 1, fbox, sampleCountLoc, sampleDurationLoc  | TrunIsoBmffBox is constructed without throwing an exception (EXPECT_NO_THROW) | Should Pass       |
 */
TEST(player_isobmff_TrunIsoBmffBox, ConstructWithTypicalValidParameters) {
    std::cout << "Entering ConstructWithTypicalValidParameters test" << std::endl;
    
    // Setup FullIsoBmffBox parameters.
    uint32_t sz = 100;
    char btype[4];
    std::strncpy(btype, "trun", 4);
    uint8_t version = 1;
    uint32_t boxFlags = 0;
    
    FullIsoBmffBox fbox(sz, btype, version, boxFlags);
    std::cout << "FullIsoBmffBox constructed with sz=" << sz 
              << ", btype=" << btype 
              << ", version=" << static_cast<int>(version)
              << ", flags=" << boxFlags << std::endl;
    
    // Setup TrunIsoBmffBox parameters.
    uint64_t sampleDuration = 1000;
    uint32_t sampleCount = 5;
    uint32_t firstSampleSize = 1024;
    uint32_t trunFlags = 1;
    
    uint8_t sampleCountMemory = 0;
    uint8_t sampleDurationMemory = 0;
    uint8_t* sampleCountLoc = &sampleCountMemory;
    uint8_t* sampleDurationLoc = &sampleDurationMemory;
    
    std::cout << "Invoking player_isobmff::TrunIsoBmffBox constructor with:" << std::endl;
    std::cout << "sampleDuration = " << sampleDuration << std::endl;
    std::cout << "sampleCount = " << sampleCount << std::endl;
    std::cout << "firstSampleSize = " << firstSampleSize << std::endl;
    std::cout << "flags = " << trunFlags << std::endl;
    std::cout << "sampleCountLoc address = " << static_cast<void*>(sampleCountLoc) << std::endl;
    std::cout << "sampleDurationLoc address = " << static_cast<void*>(sampleDurationLoc) << std::endl;
    
    EXPECT_NO_THROW({
        player_isobmff::TrunIsoBmffBox box(fbox, sampleDuration, sampleCount, sampleCountLoc, sampleDurationLoc, firstSampleSize, trunFlags);
        std::cout << "TrunIsoBmffBox constructed successfully." << std::endl;
    });
    
    std::cout << "Exiting ConstructWithTypicalValidParameters test" << std::endl;
}
/**
 * @brief Validate TrunIsoBmffBox constructor handles zero sample count scenario
 *
 * This test verifies that invoking the player_isobmff::TrunIsoBmffBox constructor with a FullIsoBmffBox and parameters 
 * where sampleCount and firstSampleSize are zero does not throw any exceptions. It confirms that the constructor handles 
 * this edge case gracefully when no samples are provided, thus ensuring stability in boundary conditions.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 232@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoke player_isobmff::TrunIsoBmffBox constructor with FullIsoBmffBox and zero sample count parameters | sz = 100, btype = "trun", version = 1, boxFlags = 0, sampleDuration = 500, sampleCount = 0, firstSampleSize = 0, trunFlags = 0, sampleCountLoc = valid pointer, sampleDurationLoc = valid pointer | No exception is thrown; constructor successfully completes | Should Pass |
 */
TEST(player_isobmff_TrunIsoBmffBox, ConstructWithZeroSampleCount) {
    std::cout << "Entering ConstructWithZeroSampleCount test" << std::endl;
    
    // Create FullIsoBmffBox.
    uint32_t sz = 100;
    char btype[4];
    std::strncpy(btype, "trun", 4);
    uint8_t version = 1;
    uint32_t boxFlags = 0;
    
    FullIsoBmffBox fbox(sz, btype, version, boxFlags);
    std::cout << "FullIsoBmffBox constructed with sz=" << sz 
              << ", btype=" << btype 
              << ", version=" << static_cast<int>(version)
              << ", flags=" << boxFlags << std::endl;
    
    // Setup parameters for TrunIsoBmffBox with zero sampleCount and zero firstSampleSize.
    uint64_t sampleDuration = 500;
    uint32_t sampleCount = 0;
    uint32_t firstSampleSize = 0;
    uint32_t trunFlags = 0;
    
    uint8_t sampleCountMemory = 0;
    uint8_t sampleDurationMemory = 0;
    uint8_t* sampleCountLoc = &sampleCountMemory;
    uint8_t* sampleDurationLoc = &sampleDurationMemory;
    
    std::cout << "Invoking player_isobmff::TrunIsoBmffBox constructor with:" << std::endl;
    std::cout << "sampleDuration = " << sampleDuration << std::endl;
    std::cout << "sampleCount = " << sampleCount << std::endl;
    std::cout << "firstSampleSize = " << firstSampleSize << std::endl;
    std::cout << "flags = " << trunFlags << std::endl;
    std::cout << "sampleCountLoc address = " << static_cast<void*>(sampleCountLoc) << std::endl;
    std::cout << "sampleDurationLoc address = " << static_cast<void*>(sampleDurationLoc) << std::endl;
    
    EXPECT_NO_THROW({
        player_isobmff::TrunIsoBmffBox box(fbox, sampleDuration, sampleCount, sampleCountLoc, sampleDurationLoc, firstSampleSize, trunFlags);
        std::cout << "TrunIsoBmffBox constructed successfully with zero sampleCount." << std::endl;
    });
    
    std::cout << "Exiting ConstructWithZeroSampleCount test" << std::endl;
}
/**
 * @brief Validate that constructing a TrunIsoBmffBox with a null sampleCountLoc pointer triggers an exception.
 *
 * This test verifies that passing a null pointer for the sampleCountLoc parameter to the player_isobmff::TrunIsoBmffBox constructor results in an exception. It ensures that the API handles invalid input parameters robustly by throwing an exception when a necessary pointer is missing.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 233
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Construct FullIsoBmffBox with valid parameters. | sz=100, btype="trun", version=1, boxFlags=0 | FullIsoBmffBox is constructed successfully. | Should be successful |
 * | 02 | Set up test parameters with a valid sampleDuration, sampleCount, firstSampleSize, trunFlags, and valid sampleDurationLoc while sampleCountLoc is set to NULL. | sampleDuration=1000, sampleCount=5, firstSampleSize=1024, trunFlags=1, sampleCountLoc=NULL, sampleDurationLoc=&sampleDurationMemory | Parameters initialized correctly with sampleCountLoc as NULL. | Should be successful |
 * | 03 | Invoke the player_isobmff::TrunIsoBmffBox constructor with the above parameters and verify that an exception is thrown due to the null sampleCountLoc pointer. | Input: FullIsoBmffBox, sampleDuration=1000, sampleCount=5, sampleCountLoc=NULL, sampleDurationLoc valid, firstSampleSize=1024, trunFlags=1 | Exception is thrown as expected. | Should Pass |
 */
TEST(player_isobmff_TrunIsoBmffBox, ConstructWithNullSampleCountLocPointer) {
    std::cout << "Entering ConstructWithNullSampleCountLocPointer test" << std::endl;
    
    // Create FullIsoBmffBox.
    uint32_t sz = 100;
    char btype[4];
    std::strncpy(btype, "trun", 4);
    uint8_t version = 1;
    uint32_t boxFlags = 0;
    
    FullIsoBmffBox fbox(sz, btype, version, boxFlags);
    std::cout << "FullIsoBmffBox constructed with sz=" << sz 
              << ", btype=" << btype 
              << ", version=" << static_cast<int>(version)
              << ", flags=" << boxFlags << std::endl;
    
    // Setup parameters with null sampleCountLoc.
    uint64_t sampleDuration = 1000;
    uint32_t sampleCount = 5;
    uint32_t firstSampleSize = 1024;
    uint32_t trunFlags = 1;
    
    uint8_t sampleDurationMemory = 0;
    uint8_t* sampleCountLoc = nullptr;
    uint8_t* sampleDurationLoc = &sampleDurationMemory;
    
    std::cout << "Invoking player_isobmff::TrunIsoBmffBox constructor with null sampleCountLoc:" << std::endl;
    std::cout << "sampleDuration = " << sampleDuration << std::endl;
    std::cout << "sampleCount = " << sampleCount << std::endl;
    std::cout << "firstSampleSize = " << firstSampleSize << std::endl;
    std::cout << "flags = " << trunFlags << std::endl;
    std::cout << "sampleCountLoc is NULL" << std::endl;
    std::cout << "sampleDurationLoc address = " << static_cast<void*>(sampleDurationLoc) << std::endl;
    
    EXPECT_ANY_THROW({
        player_isobmff::TrunIsoBmffBox box(fbox, sampleDuration, sampleCount, sampleCountLoc, sampleDurationLoc, firstSampleSize, trunFlags);
    });
    
    std::cout << "Exiting ConstructWithNullSampleCountLocPointer test" << std::endl;
}
/**
 * @brief Verify that the TrunIsoBmffBox constructor throws an exception when passed a null sampleDurationLoc pointer.
 *
 * This test constructs a FullIsoBmffBox with valid parameters and then attempts to create a TrunIsoBmffBox with sampleDurationLoc set to nullptr.
 * The objective is to ensure that the constructor properly handles the null sampleDurationLoc scenario by throwing an exception, indicating correct error handling.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 234@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                         | Test Data                                                                                                                           | Expected Result                                                    | Notes       |
 * | :--------------: | --------------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------ | ----------- |
 * | 01               | Invoke TrunIsoBmffBox constructor with a null sampleDurationLoc pointer after constructing FullIsoBmffBox | sz = 100, btype = trun, version = 1, boxFlags = 0, sampleDuration = 1000, sampleCount = 5, sampleCountLoc = valid pointer, sampleDurationLoc = nullptr, firstSampleSize = 1024, trunFlags = 1 | An exception is thrown from the TrunIsoBmffBox constructor indicating invalid input | Should Fail |
 */
TEST(player_isobmff_TrunIsoBmffBox, ConstructWithNullSampleDurationLocPointer) {
    std::cout << "Entering ConstructWithNullSampleDurationLocPointer test" << std::endl;
    
    // Create FullIsoBmffBox.
    uint32_t sz = 100;
    char btype[4];
    std::strncpy(btype, "trun", 4);
    uint8_t version = 1;
    uint32_t boxFlags = 0;
    
    FullIsoBmffBox fbox(sz, btype, version, boxFlags);
    std::cout << "FullIsoBmffBox constructed with sz=" << sz 
              << ", btype=" << btype 
              << ", version=" << static_cast<int>(version)
              << ", flags=" << boxFlags << std::endl;
    
    // Setup parameters with null sampleDurationLoc.
    uint64_t sampleDuration = 1000;
    uint32_t sampleCount = 5;
    uint32_t firstSampleSize = 1024;
    uint32_t trunFlags = 1;
    
    uint8_t sampleCountMemory = 0;
    uint8_t* sampleCountLoc = &sampleCountMemory;
    uint8_t* sampleDurationLoc = nullptr;
    
    std::cout << "Invoking player_isobmff::TrunIsoBmffBox constructor with null sampleDurationLoc:" << std::endl;
    std::cout << "sampleDuration = " << sampleDuration << std::endl;
    std::cout << "sampleCount = " << sampleCount << std::endl;
    std::cout << "firstSampleSize = " << firstSampleSize << std::endl;
    std::cout << "flags = " << trunFlags << std::endl;
    std::cout << "sampleCountLoc address = " << static_cast<void*>(sampleCountLoc) << std::endl;
    std::cout << "sampleDurationLoc is NULL" << std::endl;
    
    EXPECT_ANY_THROW({
        player_isobmff::TrunIsoBmffBox box(fbox, sampleDuration, sampleCount, sampleCountLoc, sampleDurationLoc, firstSampleSize, trunFlags);
    });
    
    std::cout << "Exiting ConstructWithNullSampleDurationLocPointer test" << std::endl;
}
/**
 * @brief Validate the construction of TrunIsoBmffBox using valid input parameters
 *
 * This test verifies that the TrunIsoBmffBox constructor does not throw any exceptions when 
 * provided with valid input parameters. The test provides parameters such as sz, sampleDuration, 
 * sampleCount, firstSampleSize, flags, and valid fixed-size arrays for sampleCountBuffer and sampleDurationBuffer 
 * (initialized with "A" and "B", respectively) to simulate a valid scenario. This helps ensure that the API 
 * successfully constructs the object under normal conditions.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 235@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                                            | Test Data                                                                                                 | Expected Result                                                       | Notes        |
 * | :--------------: | -------------------------------------------------------------------------------------- | --------------------------------------------------------------------------------------------------------- | --------------------------------------------------------------------- | ------------ |
 * | 01               | Invoke TrunIsoBmffBox constructor with valid parameters and verify no exception is thrown | sz = 100, sampleDuration = 1000, sampleCount = 10, firstSampleSize = 512, flags = 1, sampleCountBuffer = A, sampleDurationBuffer = B | No exception is thrown, and object is successfully constructed       | Should Pass  |
 */
TEST(player_isobmff_TrunIsoBmffBox, ValidConstruction) {
    std::cout << "Entering ValidConstruction test" << std::endl;
    
    uint32_t sz = 100;
    uint64_t sampleDuration = 1000;
    uint32_t sampleCount = 10;
    uint32_t firstSampleSize = 512;
    uint32_t flags = 1;
    
    // Create valid fixed size arrays for sampleCountLoc and sampleDurationLoc
    uint8_t sampleCountBuffer[10] = {0};
    uint8_t sampleDurationBuffer[10] = {0};
    strncpy(reinterpret_cast<char*>(sampleCountBuffer), "A", sizeof(sampleCountBuffer)-1);
    strncpy(reinterpret_cast<char*>(sampleDurationBuffer), "B", sizeof(sampleDurationBuffer)-1);

    std::cout << "Invoking TrunIsoBmffBox constructor with values:" << std::endl;
    std::cout << "  sz = " << sz << ", sampleDuration = " << sampleDuration
              << ", sampleCount = " << sampleCount << ", firstSampleSize = " << firstSampleSize
              << ", flags = " << flags << std::endl;
    std::cout << "  sampleCountLoc points to value: " << sampleCountBuffer[0] 
              << ", sampleDurationLoc points to value: " << sampleDurationBuffer[0] << std::endl;

    EXPECT_NO_THROW({
        player_isobmff::TrunIsoBmffBox box(sz, sampleDuration, sampleCount, 
                                           sampleCountBuffer, sampleDurationBuffer, 
                                           firstSampleSize, flags);
        std::cout << "Successfully constructed player_isobmff::TrunIsoBmffBox object." << std::endl;
    });
    
    std::cout << "Exiting ValidConstruction test" << std::endl;
}
/**
 * @brief Validates that the TrunIsoBmffBox constructor handles a zero size input gracefully.
 *
 * This test verifies that when the TrunIsoBmffBox constructor is called with a size of zero,
 * along with valid sample parameters and buffers, the constructor does not throw any exceptions.
 * This ensures that the API implementation correctly processes and initializes internal values even when
 * the size is zero, as expected in certain operational conditions.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 236@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                     | Test Data                                                                                                                                                                    | Expected Result                                                  | Notes       |
 * | :--------------: | ----------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ----------------------------------------------------------------- | ----------- |
 * | 01               | Initialize test input values including sz, sampleDuration, sampleCount, firstSampleSize, flags, and buffers. | sz = 0, sampleDuration = 1000, sampleCount = 10, firstSampleSize = 512, flags = 1, sampleCountBuffer = "C", sampleDurationBuffer = "D"                                | Correct initialization of variables and buffers.                  | Should be successful |
 * | 02               | Invoke the TrunIsoBmffBox constructor using the initialized inputs wrapped in an EXPECT_NO_THROW block. | sz = 0, sampleDuration = 1000, sampleCount = 10, firstSampleSize = 512, flags = 1, sampleCountBuffer = "C", sampleDurationBuffer = "D"                                | Constructor executes without throwing an exception.               | Should Pass |
 */
TEST(player_isobmff_TrunIsoBmffBox, ZeroSize) {
    std::cout << "Entering ZeroSize test" << std::endl;
    
    uint32_t sz = 0;
    uint64_t sampleDuration = 1000;
    uint32_t sampleCount = 10;
    uint32_t firstSampleSize = 512;
    uint32_t flags = 1;
    
    uint8_t sampleCountBuffer[10] = {0};
    uint8_t sampleDurationBuffer[10] = {0};
    strncpy(reinterpret_cast<char*>(sampleCountBuffer), "C", sizeof(sampleCountBuffer)-1);
    strncpy(reinterpret_cast<char*>(sampleDurationBuffer), "D", sizeof(sampleDurationBuffer)-1);

    std::cout << "Invoking TrunIsoBmffBox constructor with sz=0 and values:" << std::endl;
    std::cout << "  sz = " << sz << ", sampleDuration = " << sampleDuration
              << ", sampleCount = " << sampleCount << ", firstSampleSize = " << firstSampleSize
              << ", flags = " << flags << std::endl;
    std::cout << "  sampleCountLoc: " << sampleCountBuffer[0] << ", sampleDurationLoc: " << sampleDurationBuffer[0] << std::endl;

    EXPECT_NO_THROW({
        player_isobmff::TrunIsoBmffBox box(sz, sampleDuration, sampleCount,
                                           sampleCountBuffer, sampleDurationBuffer,
                                           firstSampleSize, flags);
        std::cout << "Constructed object with sz=0. Internal box size is 0 as provided." << std::endl;
    });
    
    std::cout << "Exiting ZeroSize test" << std::endl;
}
/**
 * @brief Verifies that the TrunIsoBmffBox constructor correctly handles a zero sampleDuration.
 *
 * This test checks that when a zero sampleDuration is passed to the TrunIsoBmffBox constructor
 * along with valid values for sample count, first sample size, flags, and sample buffers, no exception
 * is thrown. The test ensures that the constructor properly processes a zero sampleDuration scenario.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 237@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                               | Test Data                                                                                                                      | Expected Result                                                         | Notes      |
 * | :--------------: | --------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------ | ----------------------------------------------------------------------- | ---------- |
 * | 01               | Invoke the TrunIsoBmffBox constructor with sampleDuration set to zero along with valid parameters and buffers. | sz = 100, sampleDuration = 0, sampleCount = 10, firstSampleSize = 512, flags = 1, sampleCountBuffer = "E", sampleDurationBuffer = "F" | Constructor does not throw any exception; object is constructed successfully. | Should Pass |
 */
TEST(player_isobmff_TrunIsoBmffBox, ZeroSampleDuration) {
    std::cout << "Entering ZeroSampleDuration test" << std::endl;
    
    uint32_t sz = 100;
    uint64_t sampleDuration = 0;
    uint32_t sampleCount = 10;
    uint32_t firstSampleSize = 512;
    uint32_t flags = 1;
    
    uint8_t sampleCountBuffer[10] = {0};
    uint8_t sampleDurationBuffer[10] = {0};
    strncpy(reinterpret_cast<char*>(sampleCountBuffer), "E", sizeof(sampleCountBuffer)-1);
    strncpy(reinterpret_cast<char*>(sampleDurationBuffer), "F", sizeof(sampleDurationBuffer)-1);

    std::cout << "Invoking TrunIsoBmffBox constructor with sampleDuration=0 and values:" << std::endl;
    std::cout << "  sz = " << sz << ", sampleDuration = " << sampleDuration
              << ", sampleCount = " << sampleCount << ", firstSampleSize = " << firstSampleSize
              << ", flags = " << flags << std::endl;
    std::cout << "  sampleCountLoc: " << sampleCountBuffer[0] << ", sampleDurationLoc: " << sampleDurationBuffer[0] << std::endl;

    EXPECT_NO_THROW({
        player_isobmff::TrunIsoBmffBox box(sz, sampleDuration, sampleCount,
                                           sampleCountBuffer, sampleDurationBuffer,
                                           firstSampleSize, flags);
        std::cout << "Constructed object with sampleDuration=0; internal sampleDuration stored as 0." << std::endl;
    });
    
    std::cout << "Exiting ZeroSampleDuration test" << std::endl;
}
/**
 * @brief Validate TrunIsoBmffBox constructor handles zero sample count gracefully
 *
 * This test verifies that the TrunIsoBmffBox constructor does not throw an exception when invoked with sampleCount = 0. It ensures that the internal state is correctly set even when no samples are provided.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 238@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                                       | Test Data                                                                                                                   | Expected Result                                                               | Notes      |
 * | :----:           | :-------------------------------------------------------------------------------  | :-------------------------------------------------------------------------------------------------------------------------- | :---------------------------------------------------------------------------  | :--------  |
 * | 01               | Invoke TrunIsoBmffBox constructor with sampleCount = 0 and valid parameters           | sz = 100, sampleDuration = 1000, sampleCount = 0, firstSampleSize = 512, flags = 1, sampleCountBuffer = "G", sampleDurationBuffer = "H" | Object is constructed successfully without throwing an exception and internal sampleCount set to 0 | Should Pass |
 */
TEST(player_isobmff_TrunIsoBmffBox, ZeroSampleCount) {
    std::cout << "Entering ZeroSampleCount test" << std::endl;
    
    uint32_t sz = 100;
    uint64_t sampleDuration = 1000;
    uint32_t sampleCount = 0;
    uint32_t firstSampleSize = 512;
    uint32_t flags = 1;
    
    uint8_t sampleCountBuffer[10] = {0};
    uint8_t sampleDurationBuffer[10] = {0};
    strncpy(reinterpret_cast<char*>(sampleCountBuffer), "G", sizeof(sampleCountBuffer)-1);
    strncpy(reinterpret_cast<char*>(sampleDurationBuffer), "H", sizeof(sampleDurationBuffer)-1);

    std::cout << "Invoking TrunIsoBmffBox constructor with sampleCount=0 and values:" << std::endl;
    std::cout << "  sz = " << sz << ", sampleDuration = " << sampleDuration
              << ", sampleCount = " << sampleCount << ", firstSampleSize = " << firstSampleSize
              << ", flags = " << flags << std::endl;
    std::cout << "  sampleCountLoc: " << sampleCountBuffer[0] << ", sampleDurationLoc: " << sampleDurationBuffer[0] << std::endl;
    
    EXPECT_NO_THROW({
        player_isobmff::TrunIsoBmffBox box(sz, sampleDuration, sampleCount,
                                           sampleCountBuffer, sampleDurationBuffer,
                                           firstSampleSize, flags);
        std::cout << "Constructed object with sampleCount=0; internal sampleCount stored as 0." << std::endl;
    });
    
    std::cout << "Exiting ZeroSampleCount test" << std::endl;
}
/**
 * @brief Verify that player_isobmff::TrunIsoBmffBox constructor throws an exception when sampleCountLoc is nullptr
 *
 * This test validates that passing a nullptr for the sampleCountLoc parameter while providing a valid sampleDurationLoc buffer results in the constructor throwing a std::exception. This negative test case ensures that the constructor properly handles invalid input parameters.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 239
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                           | Test Data                                                                                                                    | Expected Result                                                                   | Notes       |
 * | :--------------: | ----------------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------------------- | ----------- |
 * |       01         | Call the TrunIsoBmffBox constructor with valid parameters except sampleCountLoc set to nullptr        | sz = 100, sampleDuration = 1000, sampleCount = 10, firstSampleSize = 512, flags = 1, sampleCountLoc = nullptr, sampleDurationLoc[0] = I | A std::exception is thrown indicating invalid input for sampleCountLoc            | Should Fail |
 */
TEST(player_isobmff_TrunIsoBmffBox, NullSampleCountLoc) {
    std::cout << "Entering NullSampleCountLoc test" << std::endl;
    
    uint32_t sz = 100;
    uint64_t sampleDuration = 1000;
    uint32_t sampleCount = 10;
    uint32_t firstSampleSize = 512;
    uint32_t flags = 1;
    
    // sampleCountLoc is set to nullptr, sampleDurationLoc is valid.
    uint8_t sampleDurationBuffer[10] = {0};
    strncpy(reinterpret_cast<char*>(sampleDurationBuffer), "I", sizeof(sampleDurationBuffer)-1);
    
    std::cout << "Invoking TrunIsoBmffBox constructor with sampleCountLoc = nullptr and values:" << std::endl;
    std::cout << "  sz = " << sz << ", sampleDuration = " << sampleDuration
              << ", sampleCount = " << sampleCount << ", firstSampleSize = " << firstSampleSize
              << ", flags = " << flags << std::endl;
    std::cout << "  sampleCountLoc: nullptr, sampleDurationLoc: " << sampleDurationBuffer[0] << std::endl;
    
    EXPECT_THROW({
        player_isobmff::TrunIsoBmffBox box(sz, sampleDuration, sampleCount,
                                           nullptr, sampleDurationBuffer,
                                           firstSampleSize, flags);
        std::cout << "This line should not be reached if constructor properly handles nullptr." << std::endl;
    }, std::exception);
    
    std::cout << "Exiting NullSampleCountLoc test" << std::endl;
}
/**
 * @brief Verify that the constructor handles a null sampleDurationLoc parameter correctly
 *
 * This test verifies that the player_isobmff::TrunIsoBmffBox constructor correctly handles the scenario where the sampleDurationLoc pointer is null. It ensures that an exception is thrown to indicate that an invalid null pointer was provided for sampleDurationLoc.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 240
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- | -------------- | ----- |
 * | 01 | Invoke the TrunIsoBmffBox constructor with sampleDurationLoc set to nullptr | sz = 100, sampleDuration = 1000, sampleCount = 10, firstSampleSize = 512, flags = 1, sampleCountLoc = "J", sampleDurationLoc = nullptr | The constructor throws an exception as the input sampleDurationLoc is invalid (nullptr) | Should Pass |
 */
TEST(player_isobmff_TrunIsoBmffBox, NullSampleDurationLoc) {
    std::cout << "Entering NullSampleDurationLoc test" << std::endl;
    
    uint32_t sz = 100;
    uint64_t sampleDuration = 1000;
    uint32_t sampleCount = 10;
    uint32_t firstSampleSize = 512;
    uint32_t flags = 1;
    
    // sampleDurationLoc is set to nullptr, sampleCountLoc is valid.
    uint8_t sampleCountBuffer[10] = {0};
    strncpy(reinterpret_cast<char*>(sampleCountBuffer), "J", sizeof(sampleCountBuffer)-1);
    
    std::cout << "Invoking TrunIsoBmffBox constructor with sampleDurationLoc = nullptr and values:" << std::endl;
    std::cout << "  sz = " << sz << ", sampleDuration = " << sampleDuration
              << ", sampleCount = " << sampleCount << ", firstSampleSize = " << firstSampleSize
              << ", flags = " << flags << std::endl;
    std::cout << "  sampleCountLoc: " << sampleCountBuffer[0] << ", sampleDurationLoc: nullptr" << std::endl;
    
    EXPECT_THROW({
        player_isobmff::TrunIsoBmffBox box(sz, sampleDuration, sampleCount,
                                           sampleCountBuffer, nullptr,
                                           firstSampleSize, flags);
        std::cout << "This line should not be reached if constructor properly handles nullptr." << std::endl;
    }, std::exception);
    
    std::cout << "Exiting NullSampleDurationLoc test" << std::endl;
}
/**
 * @brief Verify that TrunIsoBmffBox handles a firstSampleSize value of zero correctly
 *
 * This test verifies that the TrunIsoBmffBox constructor does not throw an exception
 * when invoked with a firstSampleSize value of 0. It ensures that the object is correctly
 * initialized with the provided parameters, particularly that a zero first sample size is processed appropriately.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 241@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Initialize input parameters and buffers with predefined values | sz = 100, sampleDuration = 1000, sampleCount = 10, firstSampleSize = 0, flags = 1, sampleCountBuffer[0] = "K", sampleDurationBuffer[0] = "L" | Variables and buffers are correctly assigned with the given values | Should be successful |
 * | 02 | Invoke the TrunIsoBmffBox constructor with the input parameters | sz = 100, sampleDuration = 1000, sampleCount = 10, sampleCountBuffer contains "K", sampleDurationBuffer contains "L", firstSampleSize = 0, flags = 1 | Constructor completes without throwing an exception and object is constructed with firstSampleSize set to 0 | Should Pass |
 * | 03 | Log the exit of the test case execution | None | Test exit message is displayed | Should be successful |
 */
TEST(player_isobmff_TrunIsoBmffBox, ZeroFirstSampleSize) {
    std::cout << "Entering ZeroFirstSampleSize test" << std::endl;
    
    uint32_t sz = 100;
    uint64_t sampleDuration = 1000;
    uint32_t sampleCount = 10;
    uint32_t firstSampleSize = 0;
    uint32_t flags = 1;
    
    uint8_t sampleCountBuffer[10] = {0};
    uint8_t sampleDurationBuffer[10] = {0};
    strncpy(reinterpret_cast<char*>(sampleCountBuffer), "K", sizeof(sampleCountBuffer)-1);
    strncpy(reinterpret_cast<char*>(sampleDurationBuffer), "L", sizeof(sampleDurationBuffer)-1);

    std::cout << "Invoking TrunIsoBmffBox constructor with firstSampleSize=0 and values:" << std::endl;
    std::cout << "  sz = " << sz << ", sampleDuration = " << sampleDuration
              << ", sampleCount = " << sampleCount << ", firstSampleSize = " << firstSampleSize
              << ", flags = " << flags << std::endl;
    std::cout << "  sampleCountLoc: " << sampleCountBuffer[0] 
              << ", sampleDurationLoc: " << sampleDurationBuffer[0] << std::endl;
    
    EXPECT_NO_THROW({
        player_isobmff::TrunIsoBmffBox box(sz, sampleDuration, sampleCount,
                                           sampleCountBuffer, sampleDurationBuffer,
                                           firstSampleSize, flags);
        std::cout << "Constructed object with firstSampleSize=0. Internal firstSampleSize stored as 0." << std::endl;
    });
    
    std::cout << "Exiting ZeroFirstSampleSize test" << std::endl;
}
/**
 * @brief Verify that TrunIsoBmffBox constructor successfully creates an object when flags are zero
 *
 * This test validates that the TrunIsoBmffBox constructor properly handles the scenario where the flags parameter is set to 0. It ensures that the API call does not throw exceptions and correctly initializes the object with internal flags equal to 0.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 242
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoke TrunIsoBmffBox constructor with specified parameters and zero flags to verify object construction | sz = 100, sampleDuration = 1000, sampleCount = 10, firstSampleSize = 512, flags = 0, sampleCountBuffer = M, sampleDurationBuffer = N | Object is created without throwing an exception and internal flags are set to 0 | Should Pass |
 */
TEST(player_isobmff_TrunIsoBmffBox, ZeroFlags) {
    std::cout << "Entering ZeroFlags test" << std::endl;
    
    uint32_t sz = 100;
    uint64_t sampleDuration = 1000;
    uint32_t sampleCount = 10;
    uint32_t firstSampleSize = 512;
    uint32_t flags = 0;
    
    uint8_t sampleCountBuffer[10] = {0};
    uint8_t sampleDurationBuffer[10] = {0};
    strncpy(reinterpret_cast<char*>(sampleCountBuffer), "M", sizeof(sampleCountBuffer)-1);
    strncpy(reinterpret_cast<char*>(sampleDurationBuffer), "N", sizeof(sampleDurationBuffer)-1);

    std::cout << "Invoking TrunIsoBmffBox constructor with flags=0 and values:" << std::endl;
    std::cout << "  sz = " << sz << ", sampleDuration = " << sampleDuration
              << ", sampleCount = " << sampleCount << ", firstSampleSize = " << firstSampleSize
              << ", flags = " << flags << std::endl;
    std::cout << "  sampleCountLoc: " << sampleCountBuffer[0] 
              << ", sampleDurationLoc: " << sampleDurationBuffer[0] << std::endl;
    
    EXPECT_NO_THROW({
        player_isobmff::TrunIsoBmffBox box(sz, sampleDuration, sampleCount,
                                           sampleCountBuffer, sampleDurationBuffer,
                                           firstSampleSize, flags);
        std::cout << "Constructed object with flags=0. Internal flags stored as 0." << std::endl;
    });
    
    std::cout << "Exiting ZeroFlags test" << std::endl;
}
/**
 * @brief Validate valid TRUN box construction using constructTrunBox.
 *
 * This test verifies that calling constructTrunBox with a valid buffer containing the expected "TRUN" header does not throw any exceptions and returns a non-null TRUN box instance. The test ensures that a properly allocated buffer with dummy data returns a valid object, thereby confirming the correct operation of the TRUN box construction logic.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 243
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Allocate a valid buffer and initialize the first 4 bytes with "TRUN" to simulate a valid box header. | sz = 32, buffer with first 4 bytes = "TRUN" | Buffer is correctly allocated and initialized with valid TRUN header data. | Should be successful |
 * | 02 | Invoke the constructTrunBox API with the valid buffer and size as inputs. | input: sz = 32, buffer = pointer to the allocated buffer | API does not throw an exception and returns a pointer to a TRUN box object. | Should Pass |
 * | 03 | Verify that the returned TRUN box pointer is non-null, ensuring that a valid object is constructed. | output: box != nullptr | Assertion passes confirming box pointer is non-null. | Should Pass |
 */
TEST(player_isobmff_TrunIsoBmffBox, constructTrunBox_ValidTRUNBoxConstruction) {
    std::cout << "Entering constructTrunBox_ValidTRUNBoxConstruction test" << std::endl;
    
    // Setup a valid buffer with a size that covers the expected TRUN header and fields.
    uint32_t sz = 32; // A valid box size
    uint8_t *buffer = new uint8_t[sz];
    // Initialize the buffer with dummy valid data.
    // Using strncpy to assign "TRUN" in the first 4 bytes as the box type.
    char boxType[5] = {0};
    strncpy(boxType, "TRUN", sizeof(boxType));
    memcpy(buffer, boxType, 4);
    std::cout << "Buffer created with size: " << sz
              << " and first 4 bytes set to 'TRUN'" << std::endl;
    
    std::cout << "Invoking constructTrunBox with sz = " << sz
              << ", ptr = " << static_cast<void*>(buffer) << std::endl;
    
    player_isobmff::TrunIsoBmffBox *box = nullptr;
    EXPECT_NO_THROW({
        box = player_isobmff::TrunIsoBmffBox::constructTrunBox(sz, buffer);
        std::cout << "constructTrunBox invoked successfully." << std::endl;
    });
    
    std::cout << "Method returned pointer: " << box << std::endl;
    EXPECT_NE(box, nullptr);
    std::cout << "Constructed TRUN box is valid and non-null." << std::endl;
    
    delete[] buffer;
    std::cout << "Exiting constructTrunBox_ValidTRUNBoxConstruction test" << std::endl;
}
/**
 * @brief Verify that constructTrunBox properly handles null pointer input.
 *
 * This test validates that when a valid size and a null pointer are provided to constructTrunBox, the function returns a null pointer. This behavior avoids potential memory access violations due to dereferencing a null pointer.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 244@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**
 * | Variation / Step | Description                                              | Test Data                                | Expected Result                                                       | Notes       |
 * | :--------------: | --------------------------------------------------------- | ---------------------------------------- | --------------------------------------------------------------------- | ----------- |
 * | 01               | Invoke constructTrunBox with a valid size and null pointer  | sz = 16, ptr = nullptr, output: box = ?  | Function returns a null pointer and EXPECT_EQ condition is satisfied  | Should Pass |
 */
TEST(player_isobmff_TrunIsoBmffBox, constructTrunBox_NullPointer) {
    std::cout << "Entering constructTrunBox_NullPointer test" << std::endl;
    
    uint32_t sz = 16;  // any valid size
    uint8_t *ptr = nullptr;
    std::cout << "Invoking constructTrunBox with sz = " << sz
              << ", ptr = " << ptr << std::endl;
    
    player_isobmff::TrunIsoBmffBox* box = player_isobmff::TrunIsoBmffBox::constructTrunBox(sz, ptr);
    std::cout << "Method returned pointer: " << box << std::endl;
    EXPECT_EQ(box, nullptr);
    std::cout << "Null pointer input correctly handled, returning null pointer." << std::endl;
    
    std::cout << "Exiting constructTrunBox_NullPointer test" << std::endl;
}
/**
 * @brief Verify that the constructTrunBox function returns a null pointer when invoked with a zero size parameter.
 *
 * This test validates that the constructTrunBox function handles a zero size input correctly by returning a null pointer. The test allocates arbitrary memory, initializes the first 4 bytes with "TRUN", and invokes the API with a size of 0. The expected behavior is that the API recognizes the insufficient box size and returns a null pointer, ensuring robustness in input validation.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 245@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                            | Test Data                                                       | Expected Result                                                         | Notes         |
 * | :--------------: | ------------------------------------------------------------------------------------------------------ | --------------------------------------------------------------- | ----------------------------------------------------------------------- | ------------- |
 * | 01               | Allocate a buffer of 16 bytes, set its first 4 bytes to "TRUN", and invoke constructTrunBox with sz=0 | sz = 0, buffer = pointer to 16 bytes with "TRUN" in first 4 bytes | API returns a null pointer; EXPECT_EQ(box, nullptr) assertion passes       | Should Pass   |
 */
TEST(player_isobmff_TrunIsoBmffBox, constructTrunBox_ZeroSize) {
    std::cout << "Entering constructTrunBox_ZeroSize test" << std::endl;
    
    uint32_t sz = 0;  // zero size, insufficient box size
    // Even if memory is allocated, the sz is invalid.
    uint8_t *buffer = new uint8_t[16];
    char boxType[5] = {0};
    strncpy(boxType, "TRUN", sizeof(boxType));
    memcpy(buffer, boxType, 4);
    std::cout << "Buffer allocated with arbitrary memory and first 4 bytes set to 'TRUN'" << std::endl;
    
    std::cout << "Invoking constructTrunBox with sz = " << sz
              << ", ptr = " << static_cast<void*>(buffer) << std::endl;
    
    player_isobmff::TrunIsoBmffBox* box = player_isobmff::TrunIsoBmffBox::constructTrunBox(sz, buffer);
    std::cout << "Method returned pointer: " << box << std::endl;
    EXPECT_EQ(box, nullptr);
    std::cout << "Zero size input correctly handled, returning null pointer." << std::endl;
    
    delete[] buffer;
    std::cout << "Exiting constructTrunBox_ZeroSize test" << std::endl;
}
/**
 * @brief Test to verify that getFirstSampleSize() returns the expected first sample size
 *
 * This test validates the functionality of the getFirstSampleSize() method by constructing a TrunIsoBmffBox object with a specified first sample size, invoking the method, and verifying that the returned value matches the expected first sample size. This ensures that the API correctly handles the initialization and retrieval of sample size.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 246@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result |Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Set up dummy parameters and fixed size arrays for the test. | sz = 100, sampleDuration = 1000, sampleCount = 10, sampleCountBuffer = "count", sampleDurationBuffer = "duration", firstSampleSize = 123, flags = 0 | Parameters are correctly prepared | Should be successful |
 * | 02 | Invoke the constructor of TrunIsoBmffBox with the prepared parameters. | Inputs: sz, sampleDuration, sampleCount, sampleCountBuffer, sampleDurationBuffer, firstSampleSize, flags | Object is constructed successfully | Should Pass |
 * | 03 | Invoke the getFirstSampleSize() method on the constructed object. | Method call with no additional inputs; Output: retrievedSize | Retrieved value equals the provided firstSampleSize (123) | Should Pass |
 * | 04 | Validate that the retrieved value matches the expected first sample size using EXPECT_EQ. | Comparison: retrievedSize vs firstSampleSize (123) | EXPECT_EQ passes confirming retrieved value is 123 | Should Pass |
 */
TEST(player_isobmff_TrunIsoBmffBox, getFirstSampleSize_Positive) {
    std::cout << "Entering getFirstSampleSize_Positive test" << std::endl;
    
    // Set up dummy parameters for constructor
    uint32_t sz = 100;
    uint64_t sampleDuration = 1000;
    uint32_t sampleCount = 10;
    
    // Initialize fixed size arrays using strncpy as required
    uint8_t sampleCountBuffer[10] = {0};
    uint8_t sampleDurationBuffer[10] = {0};
    const char* sampleCountStr = "count";
    const char* sampleDurationStr = "duration";
    strncpy(reinterpret_cast<char*>(sampleCountBuffer), sampleCountStr, sizeof(sampleCountBuffer) - 1);
    strncpy(reinterpret_cast<char*>(sampleDurationBuffer), sampleDurationStr, sizeof(sampleDurationBuffer) - 1);
    
    // Specify first sample size as required by test plan
    uint32_t firstSampleSize = 123;
    uint32_t flags = 0;

    // Construct the object and log the invocation
    std::cout << "Invoking constructor of TrunIsoBmffBox with firstSampleSize: " 
              << firstSampleSize << std::endl;
    player_isobmff::TrunIsoBmffBox box(sz, sampleDuration, sampleCount, 
                                       sampleCountBuffer, sampleDurationBuffer, 
                                       firstSampleSize, flags);
    std::cout << "Object constructed successfully with firstSampleSize: " 
              << firstSampleSize << std::endl;
    
    // Invoke the getFirstSampleSize() method and log the invocation and result
    std::cout << "Invoking getFirstSampleSize() method." << std::endl;
    uint32_t retrievedSize = box.getFirstSampleSize();
    std::cout << "Retrieved value from getFirstSampleSize(): " << retrievedSize << std::endl;
    
    // Validate that the retrieved value matches the expected value
    EXPECT_EQ(retrievedSize, firstSampleSize);
    
    std::cout << "Exiting getFirstSampleSize_Positive test" << std::endl;
}
/**
 * @brief Verify that getFirstSampleSize correctly returns 0 when initialized with firstSampleSize set to zero
 *
 * This test constructs a TrunIsoBmffBox object with firstSampleSize parameter set to zero. It then invokes the getFirstSampleSize() method and verifies that the returned value is 0, ensuring that the object's state reflects the initialized value.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 247@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result |Notes |@n
 * | :----: | --------- | ---------- |-------------- | ----- |@n
 * | 01 | Construct TrunIsoBmffBox with firstSampleSize=0 and invoke getFirstSampleSize() method | sz = 100, sampleDuration = 1000, sampleCount = 10, sampleCountBuffer = "count", sampleDurationBuffer = "duration", firstSampleSize = 0, flags = 0 | getFirstSampleSize() returns 0 and the EXPECT_EQ check passes | Should Pass |
 */
TEST(player_isobmff_TrunIsoBmffBox, getFirstSampleSize_Zero) {
    std::cout << "Entering getFirstSampleSize_Zero test" << std::endl;
    
    // Set up dummy parameters for constructor
    uint32_t sz = 100;
    uint64_t sampleDuration = 1000;
    uint32_t sampleCount = 10;
    
    // Initialize fixed size arrays using strncpy as required
    uint8_t sampleCountBuffer[10] = {0};
    uint8_t sampleDurationBuffer[10] = {0};
    const char* sampleCountStr = "count";
    const char* sampleDurationStr = "duration";
    strncpy(reinterpret_cast<char*>(sampleCountBuffer), sampleCountStr, sizeof(sampleCountBuffer) - 1);
    strncpy(reinterpret_cast<char*>(sampleDurationBuffer), sampleDurationStr, sizeof(sampleDurationBuffer) - 1);
    
    // Specify first sample size as 0 for this test case
    uint32_t firstSampleSize = 0;
    uint32_t flags = 0;

    // Construct the object and log the invocation
    std::cout << "Invoking constructor of TrunIsoBmffBox with firstSampleSize: " 
              << firstSampleSize << std::endl;
    player_isobmff::TrunIsoBmffBox box(sz, sampleDuration, sampleCount, 
                                       sampleCountBuffer, sampleDurationBuffer, 
                                       firstSampleSize, flags);
    std::cout << "Object constructed successfully with firstSampleSize: " 
              << firstSampleSize << std::endl;
    
    // Invoke the getFirstSampleSize() method and log the invocation and result
    std::cout << "Invoking getFirstSampleSize() method." << std::endl;
    uint32_t retrievedSize = box.getFirstSampleSize();
    std::cout << "Retrieved value from getFirstSampleSize(): " << retrievedSize << std::endl;
    
    // Validate that the retrieved value matches the expected value (0 in this case)
    EXPECT_EQ(retrievedSize, firstSampleSize);
    
    std::cout << "Exiting getFirstSampleSize_Zero test" << std::endl;
}
/**
 * @brief Verify that getSampleCount returns the expected non-zero value when using the first constructor.
 *
 * This test ensures that the TrunIsoBmffBox object can be constructed without throwing exceptions when provided 
 * with a non-zero sample count, and that the getSampleCount method returns the correct value. This is critical 
 * for confirming that the object initialization and the corresponding accessor function are working correctly.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 248@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                          | Test Data                                                                                                                      | Expected Result                                          | Notes       |
 * | :--------------: | ---------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------ | -------------------------------------------------------- | ----------- |
 * | 01               | Prepare parameters and create TrunIsoBmffBox using the first constructor; log all parameter values   | sz = 1024, sampleDuration = 5000, sampleCount = 100, firstSampleSize = 256, flags = 1, sampleCountLoc = "SC_DATA", sampleDurationLoc = "SD_DATA" | No exception thrown during object construction           | Should Pass |
 * | 02               | Invoke getSampleCount() and verify that it returns the expected sample count                         | getSampleCount() called on box, expected return value = 100                                                                    | Returned sample count equals 100                         | Should Pass |
 */
TEST(player_isobmff_TrunIsoBmffBox, VerifyGetSampleCountNonZero_FirstConstructor) {
    std::cout << "Entering VerifyGetSampleCountNonZero_FirstConstructor test" << std::endl;
    
    // Prepare parameters for the first constructor
    uint32_t sz = 1024;
    uint64_t sampleDuration = 5000;
    uint32_t sampleCount = 100; // non-zero sample count
    uint32_t firstSampleSize = 256;
    uint32_t flags = 1;
    
    // Create fixed size arrays for sampleCountLoc and sampleDurationLoc
    uint8_t sampleCountLoc[10] = {0};
    uint8_t sampleDurationLoc[10] = {0};
    
    // Use strncpy to assign sample values into the fixed size arrays
    strncpy(reinterpret_cast<char*>(sampleCountLoc), "SC_DATA", sizeof(sampleCountLoc));
    strncpy(reinterpret_cast<char*>(sampleDurationLoc), "SD_DATA", sizeof(sampleDurationLoc));

    // Log the values being passed
    std::cout << "Creating TrunIsoBmffBox with parameters:" << std::endl;
    std::cout << " sz = " << sz << ", sampleDuration = " << sampleDuration 
              << ", sampleCount = " << sampleCount << ", firstSampleSize = " << firstSampleSize 
              << ", flags = " << flags << std::endl;
    std::cout << " sampleCountLoc = " << reinterpret_cast<char*>(sampleCountLoc) 
              << ", sampleDurationLoc = " << reinterpret_cast<char*>(sampleDurationLoc) << std::endl;
    
    // Construct object using the first constructor and assert no exceptions are thrown
    EXPECT_NO_THROW(
        player_isobmff::TrunIsoBmffBox box(sz, sampleDuration, sampleCount, sampleCountLoc, sampleDurationLoc, firstSampleSize, flags);
    );
    
    player_isobmff::TrunIsoBmffBox box(sz, sampleDuration, sampleCount, sampleCountLoc, sampleDurationLoc, firstSampleSize, flags);
    
    // Invoke getSampleCount() and log the call
    std::cout << "Invoking getSampleCount()..." << std::endl;
    uint32_t retSampleCount = box.getSampleCount();
    std::cout << "Returned sample count: " << retSampleCount << std::endl;
    
    // Verify that getSampleCount returns the expected value
    EXPECT_EQ(retSampleCount, 100);
    
    std::cout << "Exiting VerifyGetSampleCountNonZero_FirstConstructor test" << std::endl;
}
/**
 * @brief Verify that getSampleCount returns 0 for an object constructed with a zero sample count using the first constructor.
 *
 * This test case verifies that when player_isobmff::TrunIsoBmffBox is constructed with sampleCount set to 0, the getSampleCount() method correctly returns 0.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 249@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Initialize test parameters and assign fixed size arrays with labels "SC_ZERO" and "SD_ZERO". | sz = 1024, sampleDuration = 5000, sampleCount = 0, firstSampleSize = 256, flags = 1, sampleCountLoc = "SC_ZERO", sampleDurationLoc = "SD_ZERO" | Parameters are initialized without errors. | Should be successful |
 * | 02 | Construct the TrunIsoBmffBox object using the first constructor with the initialized parameters. | sz = 1024, sampleDuration = 5000, sampleCount = 0, sampleCountLoc = "SC_ZERO", sampleDurationLoc = "SD_ZERO", firstSampleSize = 256, flags = 1 | Object is instantiated without throwing exceptions. | Should Pass |
 * | 03 | Invoke getSampleCount() on the constructed object and verify it returns 0. | Method: getSampleCount() | Return value equals 0. | Should Pass |
 */
TEST(player_isobmff_TrunIsoBmffBox, VerifyGetSampleCountZero_FirstConstructor) {
    std::cout << "Entering VerifyGetSampleCountZero_FirstConstructor test" << std::endl;
    
    // Prepare parameters for the first constructor with sampleCount = 0
    uint32_t sz = 1024;
    uint64_t sampleDuration = 5000;
    uint32_t sampleCount = 0; // zero sample count
    uint32_t firstSampleSize = 256;
    uint32_t flags = 1;
    
    // Create fixed size arrays for sampleCountLoc and sampleDurationLoc
    uint8_t sampleCountLoc[10] = {0};
    uint8_t sampleDurationLoc[10] = {0};
    
    // Use strncpy to assign values to the fixed size arrays
    strncpy(reinterpret_cast<char*>(sampleCountLoc), "SC_ZERO", sizeof(sampleCountLoc));
    strncpy(reinterpret_cast<char*>(sampleDurationLoc), "SD_ZERO", sizeof(sampleDurationLoc));
    
    // Log the input values
    std::cout << "Creating TrunIsoBmffBox with parameters:" << std::endl;
    std::cout << " sz = " << sz << ", sampleDuration = " << sampleDuration 
              << ", sampleCount = " << sampleCount << ", firstSampleSize = " << firstSampleSize 
              << ", flags = " << flags << std::endl;
    std::cout << " sampleCountLoc = " << reinterpret_cast<char*>(sampleCountLoc) 
              << ", sampleDurationLoc = " << reinterpret_cast<char*>(sampleDurationLoc) << std::endl;
    
    // Construct object using the first constructor and assert no exceptions are thrown
    EXPECT_NO_THROW(
        player_isobmff::TrunIsoBmffBox box(sz, sampleDuration, sampleCount, sampleCountLoc, sampleDurationLoc, firstSampleSize, flags);
    );
    
    player_isobmff::TrunIsoBmffBox box(sz, sampleDuration, sampleCount, sampleCountLoc, sampleDurationLoc, firstSampleSize, flags);
    
    // Invoke getSampleCount and log the result
    std::cout << "Invoking getSampleCount()..." << std::endl;
    uint32_t retSampleCount = box.getSampleCount();
    std::cout << "Returned sample count: " << retSampleCount << std::endl;
    
    // Verify that getSampleCount returns 0
    EXPECT_EQ(retSampleCount, 0);
    
    std::cout << "Exiting VerifyGetSampleCountZero_FirstConstructor test" << std::endl;
}
/**
 * @brief Verify that getSampleCount returns the correct count after constructing TrunIsoBmffBox using the second constructor
 *
 * This test verifies that the second constructor of the TrunIsoBmffBox class correctly initializes the object such that the getSampleCount method returns the expected sample count (50). The test constructs a FullIsoBmffBox first and then builds a TrunIsoBmffBox with predefined parameters. It ensures that no exceptions are thrown during object construction and that the API returns the correct value.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 250
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Construct FullIsoBmffBox and TrunIsoBmffBox with valid parameters and invoke getSampleCount() | fboxSize = 2048, btype = "ftyp", ver = 1, fboxFlags = 0, sampleDuration = 7500, sampleCount = 50, firstSampleSize = 512, flags = 2, sampleCountLoc = "SC_50", sampleDurationLoc = "SD_7500" | getSampleCount() returns 50 and no exceptions thrown during construction | Should Pass |
 */
TEST(player_isobmff_TrunIsoBmffBox, VerifyGetSampleCount_SecondConstructor) {
    std::cout << "Entering VerifyGetSampleCount_SecondConstructor test" << std::endl;
    
    // Prepare parameters for FullIsoBmffBox used in the second constructor
    uint32_t fboxSize = 2048;
    char btype[4] = {'f', 't', 'y', 'p'};  // 4-character box type
    uint8_t ver = 1;
    uint32_t fboxFlags = 0;
    
    // Create a FullIsoBmffBox object using its constructor (assuming it is implemented)
    EXPECT_NO_THROW(
        player_isobmff::FullIsoBmffBox fbox(fboxSize, btype, ver, fboxFlags);
    );
    player_isobmff::FullIsoBmffBox fbox(fboxSize, btype, ver, fboxFlags);
    
    // Prepare parameters for the second constructor of TrunIsoBmffBox
    uint64_t sampleDuration = 7500;
    uint32_t sampleCount = 50;
    uint32_t firstSampleSize = 512;
    uint32_t flags = 2;
    
    // Create fixed size arrays for sampleCountLoc and sampleDurationLoc
    uint8_t sampleCountLoc[10] = {0};
    uint8_t sampleDurationLoc[10] = {0};
    
    // Use strncpy to assign values to the fixed size arrays
    strncpy(reinterpret_cast<char*>(sampleCountLoc), "SC_50", sizeof(sampleCountLoc));
    strncpy(reinterpret_cast<char*>(sampleDurationLoc), "SD_7500", sizeof(sampleDurationLoc));
    
    // Log the values being passed into the second constructor
    std::cout << "Creating TrunIsoBmffBox (using FullIsoBmffBox) with parameters:" << std::endl;
    std::cout << " fboxSize = " << fboxSize << ", btype = " << std::string(btype, 4)
              << ", version = " << static_cast<uint32_t>(ver) << ", fboxFlags = " << fboxFlags << std::endl;
    std::cout << " sampleDuration = " << sampleDuration << ", sampleCount = " << sampleCount 
              << ", firstSampleSize = " << firstSampleSize << ", flags = " << flags << std::endl;
    std::cout << " sampleCountLoc = " << reinterpret_cast<char*>(sampleCountLoc)
              << ", sampleDurationLoc = " << reinterpret_cast<char*>(sampleDurationLoc) << std::endl;
    
    // Construct object using the second constructor and assert no exceptions are thrown.
    EXPECT_NO_THROW(
        player_isobmff::TrunIsoBmffBox box(fbox, sampleDuration, sampleCount, sampleCountLoc, sampleDurationLoc, firstSampleSize, flags);
    );
    
    player_isobmff::TrunIsoBmffBox box(fbox, sampleDuration, sampleCount, sampleCountLoc, sampleDurationLoc, firstSampleSize, flags);
    
    // Invoke getSampleCount() and log the call and output.
    std::cout << "Invoking getSampleCount()..." << std::endl;
    uint32_t retSampleCount = box.getSampleCount();
    std::cout << "Returned sample count: " << retSampleCount << std::endl;
    
    // Verify that getSampleCount returns the expected value 50.
    EXPECT_EQ(retSampleCount, 50);
    
    std::cout << "Exiting VerifyGetSampleCount_SecondConstructor test" << std::endl;
}
/**
 * @brief Test to verify that getSampleDuration returns the correct positive value.
 *
 * This test verifies that the getSampleDuration method returns a positive value equal to the provided input sample duration when the TrunIsoBmffBox object is constructed. It ensures the correctness of data initialization and method functionality.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 251@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Construct the TrunIsoBmffBox object with sampleDuration = 5000, sampleCount = 1, firstSampleSize = 100, flags = 0, and invoke getSampleDuration() method. | size = 0, sampleDurationInput = 5000, sampleCount = 1, sampleCountLoc = "scount", sampleDurationLoc = "sdur", firstSampleSize = 100, flags = 0 | The getSampleDuration method returns 5000 and no exceptions are thrown during object construction. | Should Pass |
 */
TEST(player_isobmff_TrunIsoBmffBox, getSampleDuration_returnsPositiveValue) {
    std::cout << "Entering getSampleDuration_returnsPositiveValue test" << std::endl;

    // Prepare fixed size arrays and initialize using strncpy
    uint8_t sampleCountLoc[10] = {0};
    uint8_t sampleDurationLoc[10] = {0};
    strncpy(reinterpret_cast<char*>(sampleCountLoc), "scount", sizeof(sampleCountLoc));
    strncpy(reinterpret_cast<char*>(sampleDurationLoc), "sdur", sizeof(sampleDurationLoc));

    // Log the constructor parameters
    uint32_t size = 0;
    uint64_t sampleDurationInput = 5000;
    uint32_t sampleCount = 1;
    uint32_t firstSampleSize = 100;
    uint32_t flags = 0;
    std::cout << "Constructing player_isobmff::TrunIsoBmffBox with sampleDuration = " 
              << sampleDurationInput << ", sampleCount = " << sampleCount 
              << ", firstSampleSize = " << firstSampleSize << ", flags = " << flags << std::endl;

    // Constructor invocation is expected not to throw
    EXPECT_NO_THROW(
        player_isobmff::TrunIsoBmffBox box(size, sampleDurationInput, sampleCount, sampleCountLoc, sampleDurationLoc, firstSampleSize, flags);
        std::cout << "TrunIsoBmffBox object constructed successfully." << std::endl;

        // Invoke the method getSampleDuration and log the call
        std::cout << "Invoking getSampleDuration() method." << std::endl;
        uint64_t result = box.getSampleDuration();
        std::cout << "getSampleDuration() returned: " << result << std::endl;
        EXPECT_EQ(result, sampleDurationInput);
    );

    std::cout << "Exiting getSampleDuration_returnsPositiveValue test" << std::endl;
}
/**
 * @brief Verifies that getSampleDuration returns the expected sample duration when initialized with zero.
 *
 * This test instantiates a player_isobmff::TrunIsoBmffBox object with sampleDuration set to 0 and validates that the method getSampleDuration returns 0.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 252@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Initialize input parameters, including fixed size arrays and strings for sampleCountLoc and sampleDurationLoc, with sampleDuration set to 0. | sampleCountLoc = "scount", sampleDurationLoc = "sdur", sampleDurationInput = 0, sampleCount = 1, firstSampleSize = 100, flags = 0 | Input parameters are initialized correctly. | Should be successful |
 * | 02 | Invoke the player_isobmff::TrunIsoBmffBox constructor and verify that no exception is thrown. | size = 0, sampleDurationInput = 0, sampleCount = 1, sampleCountLoc = "scount", sampleDurationLoc = "sdur", firstSampleSize = 100, flags = 0 | Object is constructed successfully without throwing an exception. | Should Pass |
 * | 03 | Call the getSampleDuration method and check that it returns the sampleDurationInput value (0). | sampleDurationInput = 0, result = getSampleDuration() | Method returns 0, matching the initialized sampleDurationInput. | Should Pass |
 */
TEST(player_isobmff_TrunIsoBmffBox, getSampleDuration_returnsZero) {
    std::cout << "Entering getSampleDuration_returnsZero test" << std::endl;

    // Prepare fixed size arrays and initialize using strncpy
    uint8_t sampleCountLoc[10] = {0};
    uint8_t sampleDurationLoc[10] = {0};
    strncpy(reinterpret_cast<char*>(sampleCountLoc), "scount", sizeof(sampleCountLoc));
    strncpy(reinterpret_cast<char*>(sampleDurationLoc), "sdur", sizeof(sampleDurationLoc));

    // Log the constructor parameters
    uint32_t size = 0;
    uint64_t sampleDurationInput = 0;
    uint32_t sampleCount = 1;
    uint32_t firstSampleSize = 100;
    uint32_t flags = 0;
    std::cout << "Constructing player_isobmff::TrunIsoBmffBox with sampleDuration = " 
              << sampleDurationInput << ", sampleCount = " << sampleCount 
              << ", firstSampleSize = " << firstSampleSize << ", flags = " << flags << std::endl;

    // Constructor invocation is expected not to throw
    EXPECT_NO_THROW(
        player_isobmff::TrunIsoBmffBox box(size, sampleDurationInput, sampleCount, sampleCountLoc, sampleDurationLoc, firstSampleSize, flags);
        std::cout << "TrunIsoBmffBox object constructed successfully." << std::endl;

        // Invoke the method getSampleDuration and log the call
        std::cout << "Invoking getSampleDuration() method." << std::endl;
        uint64_t result = box.getSampleDuration();
        std::cout << "getSampleDuration() returned: " << result << std::endl;
        EXPECT_EQ(result, sampleDurationInput);
    );

    std::cout << "Exiting getSampleDuration_returnsZero test" << std::endl;
}
/**
 * @brief Verify that sampleDurationPresent() returns true when the SAMPLE_DURATION_PRESENT flag is enabled.
 *
 * This test creates a TrunIsoBmffBox object with valid parameters including a flag enabling SAMPLE_DURATION_PRESENT.
 * It then calls the sampleDurationPresent() method to ensure that it correctly identifies the presence of the sample duration.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 253
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- | -------------- | ----- |
 * | 01 | Create TrunIsoBmffBox object with valid parameters using the custom constructor | sz = 100, sampleDuration = 200, sampleCount = 5, firstSampleSize = 50, flags = 0x000001, sampleCountLoc = "cnt1", sampleDurationLoc = "dur1" | Object is instantiated successfully without any exceptions | Should be successful |
 * | 02 | Invoke sampleDurationPresent() method on the created object | TrunIsoBmffBox object from step 01, no additional input parameters | Returns true indicating SAMPLE_DURATION_PRESENT flag is enabled | Should Pass |
 */
TEST(player_isobmff_TrunIsoBmffBox, sampleDurationPresent_returns_true_when_flag_enabled)
{
    std::cout << "Entering sampleDurationPresent_returns_true_when_flag_enabled test" << std::endl;

    // Prepare dummy parameters for constructor
    uint32_t sz = 100;
    uint64_t sampleDuration = 200;
    uint32_t sampleCount = 5;
    uint32_t firstSampleSize = 50;
    uint32_t flags = 0x000001; // SAMPLE_DURATION_PRESENT flag enabled

    // Fixed size arrays for sampleCountLoc and sampleDurationLoc
    uint8_t sampleCountLoc[4];
    uint8_t sampleDurationLoc[4];
    const char* countData = "cnt1";
    const char* durationData = "dur1";
    // Use strncpy to copy values into fixed size arrays (casting to char* as needed)
    std::strncpy(reinterpret_cast<char*>(sampleCountLoc), countData, sizeof(sampleCountLoc));
    std::strncpy(reinterpret_cast<char*>(sampleDurationLoc), durationData, sizeof(sampleDurationLoc));

    // Log internal state before object instantiation
    std::cout << "Creating TrunIsoBmffBox object with parameters:" << std::endl;
    std::cout << "  sz = " << sz << std::endl;
    std::cout << "  sampleDuration = " << sampleDuration << std::endl;
    std::cout << "  sampleCount = " << sampleCount << std::endl;
    std::cout << "  firstSampleSize = " << firstSampleSize << std::endl;
    std::cout << "  flags = 0x" << std::hex << flags << std::dec << std::endl;
    
    // Instantiate object using the custom constructor and ensure no exception is thrown.
    EXPECT_NO_THROW(
        player_isobmff::TrunIsoBmffBox trunBox(sz, sampleDuration, sampleCount, sampleCountLoc, sampleDurationLoc, firstSampleSize, flags)
    );
    player_isobmff::TrunIsoBmffBox trunBox(sz, sampleDuration, sampleCount, sampleCountLoc, sampleDurationLoc, firstSampleSize, flags);

    // Invoke the method sampleDurationPresent and log the call.
    std::cout << "Invoking sampleDurationPresent() method." << std::endl;
    bool result = false;
    EXPECT_NO_THROW(result = trunBox.sampleDurationPresent());
    std::cout << "Method sampleDurationPresent() returned: " << std::boolalpha << result << std::endl;
    std::cout << "Exiting sampleDurationPresent_returns_true_when_flag_enabled test" << std::endl;
}
/**
 * @brief Validate that sampleDurationPresent() returns false when the SAMPLE_DURATION_PRESENT flag is not enabled.
 *
 * This test verifies that when a TrunIsoBmffBox object is instantiated with the SAMPLE_DURATION_PRESENT flag disabled, the sampleDurationPresent() method correctly returns false.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 254@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | ------------- | ----- |
 * | 01 | Instantiate TrunIsoBmffBox with parameters: sz = 100, sampleDuration = 300, sampleCount = 6, firstSampleSize = 60, flags = 0x000000, sampleCountLoc = "cnt2", sampleDurationLoc = "dur2" | input1 = sz:100, input2 = sampleDuration:300, input3 = sampleCount:6, input4 = firstSampleSize:60, input5 = flags:0x000000, input6 = sampleCountLoc:"cnt2", input7 = sampleDurationLoc:"dur2" | Object is created without throwing an exception | Should be successful |
 * | 02 | Invoke sampleDurationPresent() to confirm it returns false when the flag is not enabled | No additional input; output1 = result:false | Method returns false without throwing an exception | Should Pass |
 */
TEST(player_isobmff_TrunIsoBmffBox, sampleDurationPresent_returns_false_when_flag_not_enabled)
{
    std::cout << "Entering sampleDurationPresent_returns_false_when_flag_not_enabled test" << std::endl;

    // Prepare dummy parameters for constructor
    uint32_t sz = 100;
    uint64_t sampleDuration = 300;
    uint32_t sampleCount = 6;
    uint32_t firstSampleSize = 60;
    uint32_t flags = 0x000000; // SAMPLE_DURATION_PRESENT flag not enabled

    // Fixed size arrays for sampleCountLoc and sampleDurationLoc
    uint8_t sampleCountLoc[4];
    uint8_t sampleDurationLoc[4];
    const char* countData = "cnt2";
    const char* durationData = "dur2";
    // Use strncpy to copy values into fixed size arrays
    std::strncpy(reinterpret_cast<char*>(sampleCountLoc), countData, sizeof(sampleCountLoc));
    std::strncpy(reinterpret_cast<char*>(sampleDurationLoc), durationData, sizeof(sampleDurationLoc));

    // Log internal state before object instantiation
    std::cout << "Creating TrunIsoBmffBox object with parameters:" << std::endl;
    std::cout << "  sz = " << sz << std::endl;
    std::cout << "  sampleDuration = " << sampleDuration << std::endl;
    std::cout << "  sampleCount = " << sampleCount << std::endl;
    std::cout << "  firstSampleSize = " << firstSampleSize << std::endl;
    std::cout << "  flags = 0x" << std::hex << flags << std::dec << std::endl;
    
    // Instantiate object using the custom constructor and ensure no exception is thrown.
    EXPECT_NO_THROW(
        player_isobmff::TrunIsoBmffBox trunBox(sz, sampleDuration, sampleCount, sampleCountLoc, sampleDurationLoc, firstSampleSize, flags)
    );
    player_isobmff::TrunIsoBmffBox trunBox(sz, sampleDuration, sampleCount, sampleCountLoc, sampleDurationLoc, firstSampleSize, flags);

    // Invoke the method sampleDurationPresent and log the call.
    std::cout << "Invoking sampleDurationPresent() method." << std::endl;
    bool result = true;
    EXPECT_NO_THROW(result = trunBox.sampleDurationPresent());
    std::cout << "Method sampleDurationPresent() returned: " << std::boolalpha << result << std::endl;
    std::cout << "Exiting sampleDurationPresent_returns_false_when_flag_not_enabled test" << std::endl;
}
/**
 * @brief Validate that setFirstSampleDuration correctly updates the first sample duration with a typical positive value
 *
 * This test verifies that the TrunIsoBmffBox object is properly constructed with an initial duration and that invoking
 * setFirstSampleDuration with a new valid duration value (1000) successfully updates the object's state without throwing exceptions.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 255@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | ------------- | ----- |
 * | 01 | Construct a TrunIsoBmffBox object with an initial duration value (500) and a sample count value (10) | initialDuration = 500, sampleCount = 10, sampleDurationLoc = {500}, sampleCountLoc = {0} | Object is constructed successfully without exceptions | Should be successful |
 * | 02 | Invoke setFirstSampleDuration with newDuration value (1000) on the previously constructed object | newDuration = 1000 | Method executes successfully, updating the sample duration without exceptions | Should Pass |
 */
TEST(player_isobmff_TrunIsoBmffBox, setFirstSampleDuration_UpdateTypicalPositiveValue) {
    std::cout << "Entering setFirstSampleDuration_UpdateTypicalPositiveValue test" << std::endl;
    
    // Prepare dummy memory for sample duration and sample count locations.
    uint8_t sampleDurationLoc[sizeof(uint64_t)] = {0};
    uint8_t sampleCountLoc[4] = {0};

    // Set an initial duration value different from the test value, e.g., 500.
    uint64_t initialDuration = 500;
    std::memcpy(sampleDurationLoc, &initialDuration, sizeof(uint64_t));
    std::cout << "Initialized sampleDurationLoc with initialDuration: " << initialDuration << std::endl;

    EXPECT_NO_THROW({
        player_isobmff::TrunIsoBmffBox trunBox(100, initialDuration, 10, sampleCountLoc, sampleDurationLoc, 50, 0);
        std::cout << "Constructed TrunIsoBmffBox object with initial duration: " << initialDuration << std::endl;

        // Invoke the setFirstSampleDuration method with sampleDuration = 1000.
        uint64_t newDuration = 1000;
        std::cout << "Invoking setFirstSampleDuration with sampleDuration: " << newDuration << std::endl;
        EXPECT_NO_THROW({ 
            trunBox.setFirstSampleDuration(newDuration);
            std::cout << "Method setFirstSampleDuration executed successfully." << std::endl;
        });
    
    });
    
    std::cout << "Exiting setFirstSampleDuration_UpdateTypicalPositiveValue test" << std::endl;
}
/**
 * @brief Verify that the truncate() method executes correctly on a valid instance created using the first constructor.
 *
 * This test validates that the player_isobmff::TrunIsoBmffBox object constructed using the first constructor with typical positive values behaves as expected when the truncate() method is invoked. It ensures that no exceptions are thrown during the process and that the method completes successfully.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 256@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Initialize test parameters and buffers using strncpy with valid positive values. | sz = 128, sampleDuration = 1000, sampleCount = 10, firstSampleSize = 256, flags = 1, sampleCountBuffer = "000000000", sampleDurationBuffer = "000000000" | Buffers initialized with the expected string values (no exceptions during assignment) | Should be successful |
 * | 02 | Construct the TrunIsoBmffBox object using the first constructor with the initialized test data. | sz = 128, sampleDuration = 1000, sampleCount = 10, sampleCountBuffer = "000000000", sampleDurationBuffer = "000000000", firstSampleSize = 256, flags = 1 | Object is constructed successfully without throwing exceptions | Should Pass |
 * | 03 | Invoke the truncate() method on the constructed object. | Invocation of box.truncate() on the constructed object instance | truncate() method executes without throwing exceptions | Should Pass |
 */
TEST(player_isobmff_TrunIsoBmffBox, Truncate_on_valid_instance_first_constructor) {
    std::cout << "Entering Truncate_on_valid_instance_first_constructor test" << std::endl;

    // Typical positive values setup
    uint32_t sz = 128;
    uint64_t sampleDuration = 1000;
    uint32_t sampleCount = 10;
    uint32_t firstSampleSize = 256;
    uint32_t flags = 1;

    // Prepare fixed size arrays for sampleCountLoc and sampleDurationLoc (size 10 bytes each)
    uint8_t sampleCountBuffer[10] = {0};
    uint8_t sampleDurationBuffer[10] = {0};

    // Assign initial values using strncpy
    std::cout << "Assigning initial values to sampleCountBuffer and sampleDurationBuffer using strncpy" << std::endl;
    strncpy(reinterpret_cast<char*>(sampleCountBuffer), "000000000", sizeof(sampleCountBuffer) - 1);
    strncpy(reinterpret_cast<char*>(sampleDurationBuffer), "000000000", sizeof(sampleDurationBuffer) - 1);
    std::cout << "Initial sampleCountBuffer: " << sampleCountBuffer << std::endl;
    std::cout << "Initial sampleDurationBuffer: " << sampleDurationBuffer << std::endl;

    EXPECT_NO_THROW({
        // Create object using the first constructor
        player_isobmff::TrunIsoBmffBox box(sz, sampleDuration, sampleCount, sampleCountBuffer, sampleDurationBuffer, firstSampleSize, flags);
        std::cout << "Constructed TrunIsoBmffBox instance using first constructor with:" << std::endl;
        std::cout << "  sz = " << sz << ", sampleDuration = " << sampleDuration 
                  << ", sampleCount = " << sampleCount << ", firstSampleSize = " << firstSampleSize 
                  << ", flags = " << flags << std::endl;

        // Invoke the truncate() method
        std::cout << "Invoking truncate() method..." << std::endl;
        box.truncate();
        std::cout << "truncate() method invoked" << std::endl;
    });

    std::cout << "Exiting Truncate_on_valid_instance_first_constructor test" << std::endl;
}
/**
 * @brief Test truncating a TrunIsoBmffBox object with boundary parameter values.
 *
 * This test verifies that the TrunIsoBmffBox object, created using the second constructor with boundary values (maximum for uint32_t and uint64_t), successfully executes the truncate() method without throwing exceptions. It ensures proper handling of extreme input values in a positive scenario.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 257
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create a FullIsoBmffBox instance using boundary values. | sz = 0xFFFFFFFF, btype = {'a','b','c','d'}, version = 1, flags = 0xFFFFFFFF | FullIsoBmffBox instance created successfully without exceptions. | Should Pass |
 * | 02 | Create a TrunIsoBmffBox instance using the second constructor with boundary values. | sampleDuration = 0xFFFFFFFFFFFFFFFF, sampleCount = 0xFFFFFFFF, sampleCountBuffer = "XXXXXXXXX", sampleDurationBuffer = "XXXXXXXXX", firstSampleSize = 0xFFFFFFFF, flags = 0xFFFFFFFF | TrunIsoBmffBox instance created successfully without exceptions. | Should Pass |
 * | 03 | Invoke the truncate() method on the TrunIsoBmffBox instance. | N/A | Method executes without throwing any exceptions. | Should Pass |
 */
TEST(player_isobmff_TrunIsoBmffBox, Truncate_on_valid_instance_second_constructor_boundary_values) {
    std::cout << "Entering Truncate_on_valid_instance_second_constructor_boundary_values test" << std::endl;

    // Boundary values setup using maximum values for 32-bit and 64-bit types
    uint32_t sz = 0xFFFFFFFF;  // maximum for uint32_t
    uint64_t sampleDuration = 0xFFFFFFFFFFFFFFFF;  // maximum for uint64_t
    uint32_t sampleCount = 0xFFFFFFFF; // maximum for uint32_t
    uint32_t firstSampleSize = 0xFFFFFFFF; // maximum for uint32_t
    uint32_t flags = 0xFFFFFFFF; // maximum for 32-bit flag representation

    // Prepare fixed size arrays for sampleCountLoc and sampleDurationLoc (size 10 bytes each)
    uint8_t sampleCountBuffer[10] = {0};
    uint8_t sampleDurationBuffer[10] = {0};

    // Assign initial values using strncpy
    std::cout << "Assigning initial values to sampleCountBuffer and sampleDurationBuffer using strncpy" << std::endl;
    strncpy(reinterpret_cast<char*>(sampleCountBuffer), "XXXXXXXXX", sizeof(sampleCountBuffer) - 1);
    strncpy(reinterpret_cast<char*>(sampleDurationBuffer), "XXXXXXXXX", sizeof(sampleDurationBuffer) - 1);
    std::cout << "Initial sampleCountBuffer: " << sampleCountBuffer << std::endl;
    std::cout << "Initial sampleDurationBuffer: " << sampleDurationBuffer << std::endl;

    EXPECT_NO_THROW({
        // Create a FullIsoBmffBox object needed for the second constructor.
        // Using dummy values for its parameters: sz, btype, version, flags.
        const char btype[4] = { 'a', 'b', 'c', 'd' };
        uint8_t version = 1;
        // Note: FullIsoBmffBox is assumed to be defined elsewhere.
        FullIsoBmffBox fbox(sz, btype, version, flags);
        std::cout << "Constructed FullIsoBmffBox instance with:" << std::endl;
        std::cout << "  sz = " << sz << ", btype = " << btype 
                  << ", version = " << static_cast<unsigned>(version) 
                  << ", flags = " << flags << std::endl;

        // Create object using the second constructor
        player_isobmff::TrunIsoBmffBox box(fbox, sampleDuration, sampleCount, sampleCountBuffer, sampleDurationBuffer, firstSampleSize, flags);
        std::cout << "Constructed TrunIsoBmffBox instance using second constructor with boundary values:" << std::endl;
        std::cout << "  sampleDuration = " << sampleDuration << ", sampleCount = " << sampleCount 
                  << ", firstSampleSize = " << firstSampleSize << ", flags = " << flags << std::endl;

        // Invoke the truncate() method
        std::cout << "Invoking truncate() method..." << std::endl;
        box.truncate();
        std::cout << "truncate() method invoked" << std::endl;

    });

    std::cout << "Exiting Truncate_on_valid_instance_second_constructor_boundary_values test" << std::endl;
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
