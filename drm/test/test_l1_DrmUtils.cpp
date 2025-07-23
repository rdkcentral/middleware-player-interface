
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
* @file test_l1_DrmUtils.cpp
* @page DrmUtils Level 1 Tests
*
* ## Module's Role
* This module includes Level 1 functional tests (success and failure scenarios).
* This is to ensure that the DrmUtils methods meet the requirements.
*
* **Pre-Conditions:**  None @n
* **Dependencies:** None @n
*
*/

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <stdio.h>
#include "DrmUtils.h"

/**
 * @brief Verify that the convertEndianness function correctly converts a valid 16-byte input array by swapping bytes as expected.
 *
 * This test checks whether the DrmUtils::convertEndianness function handles a valid pointer to a 16-byte input array and produces the reversed endianness output without throwing an exception. The test sets up the original and destination arrays, invokes the function, and confirms that each byte in the destination array matches the manually computed expected result.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 001@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Prepare the original input array with 16 bytes using strncpy from a predefined source string. | original = {0}, origSource = "\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F\x10" | Original array correctly set with byte values from the source string. | Should be successful |
 * | 02 | Prepare the destination array (guidBytes) initialized to zero for storing the converted output. | guidBytes = {0} | Destination array allocated and initialized to zero. | Should be successful |
 * | 03 | Invoke the DrmUtils::convertEndianness function with valid pointers for the original and destination arrays, ensuring no exceptions are thrown. | input: original (16 bytes), output: guidBytes (initially all zeros) | Function call does not throw an exception. | Should Pass |
 * | 04 | Compare each element in the guidBytes array against the expected output array after conversion. | expected = {0x04,0x03,0x02,0x01,0x06,0x05,0x08,0x07,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10} | Each index in guidBytes matches the corresponding index in the expected array. | Should Pass |
 */
TEST(ConvertEndiannessTest, PositiveValidConversion) {
    std::cout << "Entering PositiveValidConversion test" << std::endl;
    
    // Prepare input original (16 bytes) using strncpy
    unsigned char original[16] = {0};
    const char* origSource = "\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F\x10";
    // Use strncpy to fill the original array
    strncpy((char*)original, origSource, 16);
    std::cout << "Original array set with values:" << std::endl;
    for (int i = 0; i < 16; i++) {
        std::cout << "original[" << i << "] = 0x" 
                  << std::hex << (int)original[i] << std::dec << std::endl;
    }
    
    // Prepare destination array
    unsigned char guidBytes[16] = {0};
    
    // Invoke convertEndianness, ensuring no throw
    EXPECT_NO_THROW({
        std::cout << "Invoking DrmUtils::convertEndianness with valid pointers" << std::endl;
        DrmUtils::convertEndianness(original, guidBytes);
    });
    
    std::cout << "After conversion, guidBytes contains:" << std::endl;
    for (int i = 0; i < 16; i++) {
        std::cout << "guidBytes[" << i << "] = 0x" 
                  << std::hex << (int)guidBytes[i] << std::dec << std::endl;
    }
    
    // Expected result array after conversion:
    unsigned char expected[16] = {
        0x04, 0x03, 0x02, 0x01,
        0x06, 0x05, 0x08, 0x07,
        0x09, 0x0A, 0x0B, 0x0C,
        0x0D, 0x0E, 0x0F, 0x10
    };
    
    for (int i = 0; i < 16; i++) {
        EXPECT_EQ(guidBytes[i], expected[i]) << "Mismatch at index " << i;
    }
    
    std::cout << "Exiting PositiveValidConversion test" << std::endl;
}
/**
 * @brief Test that DrmUtils::convertEndianness correctly handles a null original pointer without modifying the destination data.
 *
 * This test verifies that when a null pointer is passed as the original pointer to DrmUtils::convertEndianness, the function does not alter the destination array (guidBytes) and no exception is thrown. This is important to ensure that the function gracefully handles invalid input without causing unexpected behavior.
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
 * | 01 | Setup input arguments by setting the original pointer to nullptr and initializing guidBytes with 0xAA values. | original = nullptr, guidBytes[0-15] = 0xAA | guidBytes is correctly initialized with 0xAA before the function call. | Should be successful |
 * | 02 | Display the initial values of guidBytes to confirm proper initialization. | guidBytes initial printout: each index = 0xAA | Initial values of guidBytes are printed as expected. | Should be successful |
 * | 03 | Invoke DrmUtils::convertEndianness with the null original pointer and the initialized guidBytes array. | API call: DrmUtils::convertEndianness(original, guidBytes) with original = nullptr, guidBytes[0-15] = 0xAA | The function does not throw any exceptions and handles the null pointer gracefully. | Should Fail |
 * | 04 | Verify that guidBytes remains unchanged after the conversion attempt. | guidBytes after call: each index expected value = 0xAA | All elements of guidBytes remain 0xAA; assertions pass. | Should be successful |
 */
TEST(ConvertEndiannessTest, NegativeNullOriginalPointer) {
    std::cout << "Entering NegativeNullOriginalPointer test" << std::endl;
    
    // original is nullptr
    unsigned char *original = nullptr;
    
    // Prepare destination array with initial known values using strncpy
    unsigned char guidBytes[16] = {0};
    char guidSource[17] = "\xAA\xAA\xAA\xAA\xAA\xAA\xAA\xAA\xAA\xAA\xAA\xAA\xAA\xAA\xAA\xAA";
    strncpy((char*)guidBytes, guidSource, 16);
    std::cout << "guidBytes initially set with values:" << std::endl;
    for (int i = 0; i < 16; i++) {
        std::cout << "guidBytes[" << i << "] = 0x" 
                  << std::hex << (int)guidBytes[i] << std::dec << std::endl;
    }
    
    // Invoke method with null original, expecting no throw (as per test instructions)
    EXPECT_NO_THROW({
        std::cout << "Invoking DrmUtils::convertEndianness with original pointer as nullptr" << std::endl;
        DrmUtils::convertEndianness(original, guidBytes);
    });
    
    std::cout << "After conversion attempt, guidBytes contains:" << std::endl;
    for (int i = 0; i < 16; i++) {
        std::cout << "guidBytes[" << i << "] = 0x" 
                  << std::hex << (int)guidBytes[i] << std::dec << std::endl;
    }
    
    // Expect that guidBytes remains unchanged
    for (int i = 0; i < 16; i++) {
        EXPECT_EQ(guidBytes[i], (unsigned char)0xAA) << "Mismatch at index " << i;
    }
    
    std::cout << "Exiting NegativeNullOriginalPointer test" << std::endl;
}
/**
 * @brief Validate that DrmUtils::convertEndianness handles a nullptr for guidBytes without throwing an exception
 *
 * This test verifies that when a valid original array is passed to DrmUtils::convertEndianness along with a null pointer for guidBytes, 
 * the function does not throw any exceptions. This ensures proper handling of invalid output pointers.
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
 * | Variation / Step | Description                                                                                        | Test Data                                                                                                                                                   | Expected Result                   | Notes        |
 * | :--------------: | -------------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------- | --------------------------------- | ------------ |
 * | 01               | Prepare valid original array using strncpy                                                         | original = {0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xAA,0xBB,0xCC,0xDD,0xEE,0xFF,0x00}, origSource = "\x11\x22\x33\x44\x55\x66\x77\x88\x99\xAA\xBB\xCC\xDD\xEE\xFF\x00" | original array correctly set        | Should be successful |
 * | 02               | Set guidBytes pointer to nullptr                                                                   | guidBytes = nullptr                                                                                                                                         | guidBytes remains nullptr         | Should be successful |
 * | 03               | Invoke DrmUtils::convertEndianness with provided original array and null guidBytes pointer           | input original = {0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xAA,0xBB,0xCC,0xDD,0xEE,0xFF,0x00}, guidBytes = nullptr                                      | No exception thrown               | Should Pass          |
 */
TEST(ConvertEndiannessTest, NegativeNullGuidBytesPointer) {
    std::cout << "Entering NegativeNullGuidBytesPointer test" << std::endl;
    
    // Prepare valid original array using strncpy
    unsigned char original[16] = {0};
    const char* origSource = "\x11\x22\x33\x44\x55\x66\x77\x88\x99\xAA\xBB\xCC\xDD\xEE\xFF\x00";
    strncpy((char*)original, origSource, 16);
    std::cout << "Original array set with values:" << std::endl;
    for (int i = 0; i < 16; i++) {
        std::cout << "original[" << i << "] = 0x" 
                  << std::hex << (int)original[i] << std::dec << std::endl;
    }
    
    // guidBytes is nullptr
    unsigned char *guidBytes = nullptr;
    
    // Invoke method with null guidBytes, expecting no throw (per test instructions)
    EXPECT_NO_THROW({
        std::cout << "Invoking DrmUtils::convertEndianness with guidBytes pointer as nullptr" << std::endl;
        DrmUtils::convertEndianness(original, guidBytes);
    });
    
    std::cout << "Exiting NegativeNullGuidBytesPointer test" << std::endl;
}
/**
 * @brief Validate endianness conversion for boundary value arrays.
 *
 * This test verifies that the function DrmUtils::convertEndianness correctly rearranges the bytes in an array containing boundary values. The test ensures that the conversion is performed without throwing exceptions and that the resulting byte order matches the expected transformation from the original boundary input.
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
 * | 01 | Prepare original array with boundary values using strncpy. | original = "\x00, \x00, \x00, \x00, \xFF, \xFF, \xFF, \xFF, \xAA, \xBB, \xCC, \xDD, \x11, \x22, \x33, \x44" | Original array is populated with the provided boundary bytes. | Should be successful |
 * | 02 | Initialize destination array to store the converted bytes. | guidBytes = "Array of 16 bytes initialized to 0" | Destination array is set to zero before conversion. | Should be successful |
 * | 03 | Invoke DrmUtils::convertEndianness with prepared arrays. | input1 = original, output1 = guidBytes | API call completes without throwing any exceptions. | Should Pass |
 * | 04 | Compute expected result using specific byte reordering and verify each element. | expected = "[ original[3], original[2], original[1], original[0], original[5], original[4], original[7], original[6], original[8], original[9], original[10], original[11], original[12], original[13], original[14], original[15] ]" | Each element in guidBytes matches the corresponding element in the expected array. | Should be successful |
 */
TEST(ConvertEndiannessTest, PositiveBoundaryValuesConversion) {
    std::cout << "Entering PositiveBoundaryValuesConversion test" << std::endl;
    
    // Prepare original array with boundary values using strncpy
    unsigned char original[16] = {0};
    const char* origSource = "\x00\x00\x00\x00\xFF\xFF\xFF\xFF\xAA\xBB\xCC\xDD\x11\x22\x33\x44";
    strncpy((char*)original, origSource, 16);
    std::cout << "Original array set with boundary values:" << std::endl;
    for (int i = 0; i < 16; i++) {
        std::cout << "original[" << i << "] = 0x" 
                  << std::hex << (int)original[i] << std::dec << std::endl;
    }
    
    // Prepare destination array
    unsigned char guidBytes[16] = {0};
    
    // Invoke conversion and log the invocation
    EXPECT_NO_THROW({
        std::cout << "Invoking DrmUtils::convertEndianness with boundary values" << std::endl;
        DrmUtils::convertEndianness(original, guidBytes);
    });
    
    std::cout << "After conversion, guidBytes contains:" << std::endl;
    for (int i = 0; i < 16; i++) {
        std::cout << "guidBytes[" << i << "] = 0x" 
                  << std::hex << (int)guidBytes[i] << std::dec << std::endl;
    }
    
    unsigned char expected[16] = {
        original[3], original[2], original[1], original[0],
        original[5], original[4], original[7], original[6],
        original[8], original[9], original[10], original[11],
        original[12], original[13], original[14], original[15]
    };
    
    for (int i = 0; i < 16; i++) {
        EXPECT_EQ(guidBytes[i], expected[i]) << "Mismatch at index " << i;
    }
    
    std::cout << "Exiting PositiveBoundaryValuesConversion test" << std::endl;
}
/**
 * @brief Validate successful extraction of text from PSSH input when proper delimiters are provided
 *
 * This test verifies that the function DrmUtils::extractDataFromPssh correctly extracts the substring "extracted_text" from the given psshData when valid start and end delimiters are provided. It ensures that the extracted text matches the expected output and that the output length is updated correctly.
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
 * | 01 | Initialize input data with proper delimiters for extraction | psshData = "XYZ<start>extracted_text<end>ABC", dataLength = 31, startStr = "<start>", endStr = "<end>", verStr = "v1" | Input parameters are set as expected | Should be successful |
 * | 02 | Invoke DrmUtils::extractDataFromPssh and validate the extracted output | API call with: psshData, dataLength, startStr, endStr, len pointer, verStr | Returned pointer is not nullptr, extracted text equals "extracted_text", and updated length equals 14 | Should Pass |
 */
TEST(DrmUtilsExtractDataTest, ValidExtractionWithProperDelimiters) {
    std::cout << "Entering ValidExtractionWithProperDelimiters test" << std::endl;

    char psshData[50];
    strncpy(psshData, "XYZ<start>extracted_text<end>ABC", sizeof(psshData)-1);
    psshData[sizeof(psshData)-1] = '\0';
    int dataLength = 31;
    char startStr[20];
    strncpy(startStr, "<start>", sizeof(startStr)-1);
    startStr[sizeof(startStr)-1] = '\0';
    char endStr[20];
    strncpy(endStr, "<end>", sizeof(endStr)-1);
    endStr[sizeof(endStr)-1] = '\0';
    int extractedLen = 0;
    int* len = &extractedLen;
    char verStr[10];
    strncpy(verStr, "v1", sizeof(verStr)-1);
    verStr[sizeof(verStr)-1] = '\0';

    std::cout << "Invoking DrmUtils::extractDataFromPssh with:" << std::endl;
    std::cout << "psshData: " << psshData << ", dataLength: " << dataLength << std::endl;
    std::cout << "startStr: " << startStr << ", endStr: " << endStr << ", verStr: " << verStr << std::endl;
    unsigned char* result = DrmUtils::extractDataFromPssh(psshData, dataLength, startStr, endStr, len, verStr);
    std::cout << "Returned pointer: " << static_cast<void*>(result) << std::endl;
    if(result) {
        std::cout << "Extracted text: " << result << ", updated length: " << extractedLen << std::endl;
    }
    EXPECT_NE(result, nullptr);
    if(result) {
        EXPECT_EQ(std::string(reinterpret_cast<char*>(result)), "extracted_text");
        EXPECT_EQ(extractedLen, 14);
        delete [] result;
    }

    std::cout << "Exiting ValidExtractionWithProperDelimiters test" << std::endl;
}
/**
 * @brief Verify that DrmUtils::extractDataFromPssh correctly handles a missing start delimiter scenario.
 *
 * This test verifies that when the input psshData does not contain the specified start delimiter, the API
 * DrmUtils::extractDataFromPssh returns a nullptr and leaves the extracted length unchanged. This ensures that
 * the function fails gracefully in the absence of expected delimiters.
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
 * | Variation / Step | Description                                                                                               | Test Data                                                                                                                        | Expected Result                                                   | Notes         |
 * | :--------------: | --------------------------------------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------ | ------------- |
 * | 01               | Initialize input and output buffers and variables                                                         | psshData = "XYZextracted_text<end>ABC", dataLength = 27, startStr = "<start>", endStr = "<end>", verStr = "v1", extractedLen = -1 | Buffers and variables are initialized with the expected values      | Should be successful |
 * | 02               | Invoke DrmUtils::extractDataFromPssh to attempt data extraction with a missing start delimiter              | psshData = "XYZextracted_text<end>ABC", dataLength = 27, startStr = "<start>", endStr = "<end>", verStr = "v1", len pointer to extractedLen | API returns a nullptr indicating extraction failure due to missing start delimiter | Should Fail       |
 * | 03               | Assert that the extracted length remains unchanged                                                       | extractedLen = -1                                                                                                                 | extractedLen remains -1                                             | Should be successful |
 */
TEST(DrmUtilsExtractDataTest, MissingStartDelimiter) {
    std::cout << "Entering MissingStartDelimiter test" << std::endl;

    char psshData[50];
    strncpy(psshData, "XYZextracted_text<end>ABC", sizeof(psshData)-1);
    psshData[sizeof(psshData)-1] = '\0';
    int dataLength = 27;
    char startStr[20];
    strncpy(startStr, "<start>", sizeof(startStr)-1);
    startStr[sizeof(startStr)-1] = '\0';
    char endStr[20];
    strncpy(endStr, "<end>", sizeof(endStr)-1);
    endStr[sizeof(endStr)-1] = '\0';
    int extractedLen = -1; // initialize with a known value
    int* len = &extractedLen;
    char verStr[10];
    strncpy(verStr, "v1", sizeof(verStr)-1);
    verStr[sizeof(verStr)-1] = '\0';

    std::cout << "Invoking DrmUtils::extractDataFromPssh with:" << std::endl;
    std::cout << "psshData: " << psshData << ", dataLength: " << dataLength << std::endl;
    std::cout << "startStr: " << startStr << ", endStr: " << endStr << ", verStr: " << verStr << std::endl;
    unsigned char* result = DrmUtils::extractDataFromPssh(psshData, dataLength, startStr, endStr, len, verStr);
    std::cout << "Returned pointer: " << static_cast<void*>(result) << std::endl;
    EXPECT_EQ(result, nullptr);
    EXPECT_EQ(extractedLen, -1);

    std::cout << "Exiting MissingStartDelimiter test" << std::endl;
}
/**
 * @brief Verify that DrmUtils::extractDataFromPssh returns nullptr and leaves extracted length unchanged when the end delimiter is missing.
 *
 * This test case checks the scenario where the input string (psshData) contains a valid start delimiter but lacks the corresponding end delimiter. The function is expected to detect the missing end delimiter and return a nullptr pointer while keeping the extracted length (extractedLen) at its initialized value (-1). This validation is critical to ensure robustness in data extraction logic with incomplete delimiter specifications.@n
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 007@n
 * **Priority:** High@n
 * @n
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * @n
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Initialize input parameters with missing end delimiter | psshData = "XYZ<start>extracted_textABC", dataLength = 28, startStr = "<start>", endStr = "<end>", verStr = "v1", extractedLen = -1 | Input buffers are initialized correctly | Should be successful |
 * | 02 | Invoke DrmUtils::extractDataFromPssh and validate response | Invocation with psshData, dataLength, startStr, endStr, pointer to extractedLen, verStr | Returns nullptr and extractedLen remains -1 | Should Fail |
 */

TEST(DrmUtilsExtractDataTest, MissingEndDelimiter) {
    std::cout << "Entering MissingEndDelimiter test" << std::endl;

    char psshData[50];
    strncpy(psshData, "XYZ<start>extracted_textABC", sizeof(psshData)-1);
    psshData[sizeof(psshData)-1] = '\0';
    int dataLength = 28;
    char startStr[20];
    strncpy(startStr, "<start>", sizeof(startStr)-1);
    startStr[sizeof(startStr)-1] = '\0';
    char endStr[20];
    strncpy(endStr, "<end>", sizeof(endStr)-1);
    endStr[sizeof(endStr)-1] = '\0';
    int extractedLen = -1;
    int* len = &extractedLen;
    char verStr[10];
    strncpy(verStr, "v1", sizeof(verStr)-1);
    verStr[sizeof(verStr)-1] = '\0';

    std::cout << "Invoking DrmUtils::extractDataFromPssh with:" << std::endl;
    std::cout << "psshData: " << psshData << ", dataLength: " << dataLength << std::endl;
    std::cout << "startStr: " << startStr << ", endStr: " << endStr << ", verStr: " << verStr << std::endl;
    unsigned char* result = DrmUtils::extractDataFromPssh(psshData, dataLength, startStr, endStr, len, verStr);
    std::cout << "Returned pointer: " << static_cast<void*>(result) << std::endl;
    EXPECT_EQ(result, nullptr);
    EXPECT_EQ(extractedLen, -1);

    std::cout << "Exiting MissingEndDelimiter test" << std::endl;
}
/**
 * @brief Validate that extractDataFromPssh returns nullptr and does not modify extractedLen when provided with an empty psshData string
 *
 * This test verifies that the DrmUtils::extractDataFromPssh API properly handles an empty psshData input by returning a nullptr and leaving the extractedLen value unchanged. This ensures that the API correctly handles and returns expected values for edge case input.
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
 * | 01 | Initialize test variables with an empty psshData and valid start and end strings | psshData = "", dataLength = 0, startStr = "<start>", endStr = "<end>", extractedLen = -1, verStr = "v1" | Variables are correctly initialized before API invocation | Should be successful |
 * | 02 | Invoke DrmUtils::extractDataFromPssh with the provided parameters | psshData = "", dataLength = 0, startStr = "<start>", endStr = "<end>", len pointer to extractedLen, verStr = "v1" | API returns a nullptr | Should Pass |
 * | 03 | Validate that extractedLen remains unchanged after API call | extractedLen initial = -1, API result = nullptr | extractedLen equals -1 | Should Pass |
 */
TEST(DrmUtilsExtractDataTest, EmptyPsshDataString) {
    std::cout << "Entering EmptyPsshDataString test" << std::endl;

    char psshData[50];
    strncpy(psshData, "", sizeof(psshData)-1);
    psshData[sizeof(psshData)-1] = '\0';
    int dataLength = 0;
    char startStr[20];
    strncpy(startStr, "<start>", sizeof(startStr)-1);
    startStr[sizeof(startStr)-1] = '\0';
    char endStr[20];
    strncpy(endStr, "<end>", sizeof(endStr)-1);
    endStr[sizeof(endStr)-1] = '\0';
    int extractedLen = -1;
    int* len = &extractedLen;
    char verStr[10];
    strncpy(verStr, "v1", sizeof(verStr)-1);
    verStr[sizeof(verStr)-1] = '\0';

    std::cout << "Invoking DrmUtils::extractDataFromPssh with empty psshData." << std::endl;
    unsigned char* result = DrmUtils::extractDataFromPssh(psshData, dataLength, startStr, endStr, len, verStr);
    std::cout << "Returned pointer: " << static_cast<void*>(result) << std::endl;
    EXPECT_EQ(result, nullptr);
    EXPECT_EQ(extractedLen, -1);

    std::cout << "Exiting EmptyPsshDataString test" << std::endl;
}
/**
 * @brief Validate API behavior when provided with a NULL psshData pointer.
 *
 * This test verifies that when DrmUtils::extractDataFromPssh is invoked with a NULL pointer
 * for psshData, the API returns a nullptr and does not modify the length parameter (remains -1).
 * Ensuring the API handles null inputs gracefully is critical for robustness.
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
 * | Variation / Step | Description                                                                                          | Test Data                                                                                                                           | Expected Result                                                           | Notes          |
 * | :---------------:| ---------------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------- | -------------- |
 * | 01               | Initialize input parameters with NULL psshData and set start/end markers along with version and length pointer. | psshData = NULL, dataLength = 0, startStr = <start>, endStr = <end>, verStr = v1, len pointing to extractedLen (-1)                | Inputs are initialized correctly for API invocation.                    | Should be successful |
 * | 02               | Invoke DrmUtils::extractDataFromPssh API with the initialized inputs.                                | psshData = NULL, dataLength = 0, startStr = <start>, endStr = <end>, verStr = v1, len pointing to extractedLen (-1)                | API returns a nullptr.                                                    | Should Pass    |
 * | 03               | Validate that the extracted length remains unchanged after API invocation.                           | extractedLen = -1                                                                                                                   | extractedLen remains -1 confirming no modification occurred.              | Should Pass    |
 */
TEST(DrmUtilsExtractDataTest, NullPsshData) {
    std::cout << "Entering NullPsshData test" << std::endl;

    const char* psshData = NULL;
    int dataLength = 0;
    char startStr[20];
    strncpy(startStr, "<start>", sizeof(startStr)-1);
    startStr[sizeof(startStr)-1] = '\0';
    char endStr[20];
    strncpy(endStr, "<end>", sizeof(endStr)-1);
    endStr[sizeof(endStr)-1] = '\0';
    int extractedLen = -1;
    int* len = &extractedLen;
    char verStr[10];
    strncpy(verStr, "v1", sizeof(verStr)-1);
    verStr[sizeof(verStr)-1] = '\0';

    std::cout << "Invoking DrmUtils::extractDataFromPssh with psshData as NULL." << std::endl;
    unsigned char* result = DrmUtils::extractDataFromPssh(psshData, dataLength, startStr, endStr, len, verStr);
    std::cout << "Returned pointer: " << static_cast<void*>(result) << std::endl;
    EXPECT_EQ(result, nullptr);
    EXPECT_EQ(extractedLen, -1);

    std::cout << "Exiting NullPsshData test" << std::endl;
}
/**
 * @brief Test handling of NULL start string in the extractDataFromPssh API
 *
 * This test verifies that when a NULL start string is provided to DrmUtils::extractDataFromPssh,
 * the API correctly returns a nullptr and the extracted length remains unchanged. This ensures that
 * the function gracefully handles invalid input parameters which is critical for preventing potential
 * crashes or undefined behaviors.
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
 * | Variation / Step | Description                                                                                  | Test Data                                                                                                          | Expected Result                                                        | Notes          |
 * | :--------------: | -------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------ | ---------------------------------------------------------------------- | -------------- |
 * | 01               | Initialize all input buffers and parameters with their appropriate values                    | psshData = "XYZ<start>data<end>ABC", dataLength = 23, startStr = NULL, endStr = "<end>", verStr = "v1", extractedLen = -1  | All inputs initialized correctly                                      | Should be successful  |
 * | 02               | Invoke DrmUtils::extractDataFromPssh with the prepared parameters                             | Same as above                                                                                                      | The API returns a nullptr                                             | Should Pass    |
 * | 03               | Validate the extracted length does not change after API invocation                           | extractedLen initially set to -1                                                                                   | extractedLen remains -1                                                | Should Pass    |
 */
TEST(DrmUtilsExtractDataTest, NullStartStr) {
    std::cout << "Entering NullStartStr test" << std::endl;

    char psshData[50];
    strncpy(psshData, "XYZ<start>data<end>ABC", sizeof(psshData)-1);
    psshData[sizeof(psshData)-1] = '\0';
    int dataLength = 23;
    const char* startStr = NULL;
    char endStr[20];
    strncpy(endStr, "<end>", sizeof(endStr)-1);
    endStr[sizeof(endStr)-1] = '\0';
    int extractedLen = -1;
    int* len = &extractedLen;
    char verStr[10];
    strncpy(verStr, "v1", sizeof(verStr)-1);
    verStr[sizeof(verStr)-1] = '\0';

    std::cout << "Invoking DrmUtils::extractDataFromPssh with startStr as NULL." << std::endl;
    unsigned char* result = DrmUtils::extractDataFromPssh(psshData, dataLength, startStr, endStr, len, verStr);
    std::cout << "Returned pointer: " << static_cast<void*>(result) << std::endl;
    EXPECT_EQ(result, nullptr);
    EXPECT_EQ(extractedLen, -1);

    std::cout << "Exiting NullStartStr test" << std::endl;
}
/**
 * @brief Validates that DrmUtils::extractDataFromPssh returns nullptr when endStr is NULL.
 *
 * This test checks if the extractDataFromPssh API correctly handles a NULL endStr input by returning a nullptr
 * and leaving the extracted length unchanged (i.e., -1). It ensures that the function fails gracefully with invalid parameters.
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
 * | Variation / Step | Description                                                                                                   | Test Data                                                                                                                    | Expected Result                                                                                  | Notes         |
 * | :--------------: | ------------------------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------------- | ------------- |
 * | 01               | Invoke DrmUtils::extractDataFromPssh with psshData="XYZ<start>data<end>ABC", dataLength=23, startStr="<start>", endStr=NULL, verStr="v1". Validate that the function returns a nullptr and that the extracted length remains -1. | psshData = "XYZ<start>data<end>ABC", dataLength = 23, startStr = "<start>", endStr = NULL, verStr = "v1", extractedLen = -1 | Function returns nullptr; extractedLen remains -1; EXPECT_EQ(result, nullptr) and EXPECT_EQ(extractedLen, -1) | Should Fail  |
 */
TEST(DrmUtilsExtractDataTest, NullEndStr) {
    std::cout << "Entering NullEndStr test" << std::endl;

    char psshData[50];
    strncpy(psshData, "XYZ<start>data<end>ABC", sizeof(psshData)-1);
    psshData[sizeof(psshData)-1] = '\0';
    int dataLength = 23;
    char startStr[20];
    strncpy(startStr, "<start>", sizeof(startStr)-1);
    startStr[sizeof(startStr)-1] = '\0';
    const char* endStr = NULL;
    int extractedLen = -1;
    int* len = &extractedLen;
    char verStr[10];
    strncpy(verStr, "v1", sizeof(verStr)-1);
    verStr[sizeof(verStr)-1] = '\0';

    std::cout << "Invoking DrmUtils::extractDataFromPssh with endStr as NULL." << std::endl;
    unsigned char* result = DrmUtils::extractDataFromPssh(psshData, dataLength, startStr, endStr, len, verStr);
    std::cout << "Returned pointer: " << static_cast<void*>(result) << std::endl;
    EXPECT_EQ(result, nullptr);
    EXPECT_EQ(extractedLen, -1);

    std::cout << "Exiting NullEndStr test" << std::endl;
}
/**
 * @brief Verify that DrmUtils::extractDataFromPssh returns a nullptr when the len pointer is NULL
 *
 * This test verifies that when a NULL pointer is provided for the len parameter, the function
 * DrmUtils::extractDataFromPssh correctly handles the invalid input scenario by returning a nullptr.
 * This behavior ensures stability and prevents potential segmentation faults or undefined behavior in the API.
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
 * | Variation / Step | Description                                                                              | Test Data                                                                                                    | Expected Result                                 | Notes         |
 * | :--------------: | ---------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------ | ----------------------------------------------- | ------------- |
 * | 01               | Initialize input data and parameters including psshData, dataLength, startStr, endStr, verStr | psshData = "XYZ<start>data<end>ABC", dataLength = 23, startStr = "<start>", endStr = "<end>", verStr = "v1"       | Variables set correctly                         | Should be successful |
 * | 02               | Set the length pointer to NULL                                                           | len = NULL                                                                                                   | len remains NULL                                | Should be successful |
 * | 03               | Invoke DrmUtils::extractDataFromPssh with the provided parameters                           | Function call with: psshData, dataLength, startStr, endStr, len = NULL, verStr = "v1"                           | Function is called and returns a pointer         | Should Pass   |
 * | 04               | Validate that the returned pointer is NULL using EXPECT_EQ                                 | result = DrmUtils::extractDataFromPssh(...)                                                                    | EXPECT_EQ(result, nullptr) passes               | Should Pass   |
 */
TEST(DrmUtilsExtractDataTest, NullLenPointer) {
    std::cout << "Entering NullLenPointer test" << std::endl;

    char psshData[50];
    strncpy(psshData, "XYZ<start>data<end>ABC", sizeof(psshData)-1);
    psshData[sizeof(psshData)-1] = '\0';
    int dataLength = 23;
    char startStr[20];
    strncpy(startStr, "<start>", sizeof(startStr)-1);
    startStr[sizeof(startStr)-1] = '\0';
    char endStr[20];
    strncpy(endStr, "<end>", sizeof(endStr)-1);
    endStr[sizeof(endStr)-1] = '\0';
    // len pointer is NULL
    int* len = NULL;
    char verStr[10];
    strncpy(verStr, "v1", sizeof(verStr)-1);
    verStr[sizeof(verStr)-1] = '\0';

    std::cout << "Invoking DrmUtils::extractDataFromPssh with len pointer as NULL." << std::endl;
    unsigned char* result = DrmUtils::extractDataFromPssh(psshData, dataLength, startStr, endStr, len, verStr);
    std::cout << "Returned pointer: " << static_cast<void*>(result) << std::endl;
    EXPECT_EQ(result, nullptr);

    std::cout << "Exiting NullLenPointer test" << std::endl;
}
/**
 * @brief Validate the extraction failure when version string is NULL.
 *
 * This test verifies that the DrmUtils::extractDataFromPssh API handles the case where the version string (verStr) is passed as NULL. The test checks if the API returns a NULL pointer and if the output data length remains unchanged (-1) when the verStr is invalid.
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
 * | 01 | Initialize input data and invoke extractDataFromPssh with verStr as NULL | psshData = "XYZ<start>data<end>ABC", dataLength = 23, startStr = "<start>", endStr = "<end>", extractedLen = -1, verStr = NULL | API returns a NULL pointer and extractedLen remains -1 | Should Pass |
 */
TEST(DrmUtilsExtractDataTest, NullVerStr) {
    std::cout << "Entering NullVerStr test" << std::endl;

    char psshData[50];
    strncpy(psshData, "XYZ<start>data<end>ABC", sizeof(psshData)-1);
    psshData[sizeof(psshData)-1] = '\0';
    int dataLength = 23;
    char startStr[20];
    strncpy(startStr, "<start>", sizeof(startStr)-1);
    startStr[sizeof(startStr)-1] = '\0';
    char endStr[20];
    strncpy(endStr, "<end>", sizeof(endStr)-1);
    endStr[sizeof(endStr)-1] = '\0';
    int extractedLen = -1;
    int* len = &extractedLen;
    const char* verStr = NULL;

    std::cout << "Invoking DrmUtils::extractDataFromPssh with verStr as NULL." << std::endl;
    unsigned char* result = DrmUtils::extractDataFromPssh(psshData, dataLength, startStr, endStr, len, verStr);
    std::cout << "Returned pointer: " << static_cast<void*>(result) << std::endl;
    EXPECT_EQ(result, nullptr);
    EXPECT_EQ(extractedLen, -1);

    std::cout << "Exiting NullVerStr test" << std::endl;
}
/**
 * @brief Tests the extraction function's behavior when provided with a truncated input data length.
 *
 * This test validates that when a truncated data length is provided to the DrmUtils::extractDataFromPssh API, the function returns a nullptr and leaves the output length unchanged (-1). This ensures that the API robustly handles incomplete input data without extracting erroneous information.
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
 * | 01 | Initialize test inputs including psshData, dataLength, startStr, endStr, verStr, and len pointer for extracted length. | psshData = "XYZ<start>data<end>ABC", dataLength = 10, startStr = "<start>", endStr = "<end>", verStr = "v1", extractedLen = -1 | Test inputs are correctly initialized | Should be successful |
 * | 02 | Invoke DrmUtils::extractDataFromPssh with truncated dataLength input and validate the output. | psshData = "XYZ<start>data<end>ABC", dataLength = 10, startStr = "<start>", endStr = "<end>", verStr = "v1", len pointer = address of extractedLen (-1) | API returns nullptr and extractedLen remains -1 | Should Fail |
 */
TEST(DrmUtilsExtractDataTest, TruncatedInputDataLength) {
    std::cout << "Entering TruncatedInputDataLength test" << std::endl;

    char psshData[50];
    strncpy(psshData, "XYZ<start>data<end>ABC", sizeof(psshData)-1);
    psshData[sizeof(psshData)-1] = '\0';
    int dataLength = 10; // truncated length
    char startStr[20];
    strncpy(startStr, "<start>", sizeof(startStr)-1);
    startStr[sizeof(startStr)-1] = '\0';
    char endStr[20];
    strncpy(endStr, "<end>", sizeof(endStr)-1);
    endStr[sizeof(endStr)-1] = '\0';
    int extractedLen = -1;
    int* len = &extractedLen;
    char verStr[10];
    strncpy(verStr, "v1", sizeof(verStr)-1);
    verStr[sizeof(verStr)-1] = '\0';

    std::cout << "Invoking DrmUtils::extractDataFromPssh with truncated dataLength: " << dataLength << std::endl;
    std::cout << "psshData: " << psshData << ", startStr: " << startStr << ", endStr: " << endStr << ", verStr: " << verStr << std::endl;
    unsigned char* result = DrmUtils::extractDataFromPssh(psshData, dataLength, startStr, endStr, len, verStr);
    std::cout << "Returned pointer: " << static_cast<void*>(result) << std::endl;
    EXPECT_EQ(result, nullptr);
    EXPECT_EQ(extractedLen, -1);

    std::cout << "Exiting TruncatedInputDataLength test" << std::endl;
}
/**
 * @brief Verify that DrmUtils::extractWVContentMetadataFromPssh extracts valid WV metadata from a correct PSSH input.
 *
 * This test verifies the functionality of DrmUtils::extractWVContentMetadataFromPssh by providing a valid PSSH data string and data length.
 * It ensures the API successfully processes the input without throwing exceptions and returns the expected metadata string.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 015
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                         | Test Data                                                      | Expected Result                                                        | Notes         |
 * | :--------------: | --------------------------------------------------------------------------------------------------- | -------------------------------------------------------------- | ---------------------------------------------------------------------- | ------------- |
 * | 01               | Prepare input data by copying a valid source string into a fixed-size array and setting dataLength   | inputStr = valid_wv_pssh_data, psshData = {valid_wv_pssh_data}, dataLength = 18 | psshData is correctly initialized                                      | Should be successful |
 * | 02               | Invoke DrmUtils::extractWVContentMetadataFromPssh using the prepared input                           | psshData = valid_wv_pssh_data, dataLength = 18                   | No exception thrown and function is executed                           | Should Pass   |
 * | 03               | Validate the output by checking that the returned metadata equals "ExtractedWVContentMetadata"        | result (output) = value from API call                            | result equals "ExtractedWVContentMetadata"                              | Should Pass   |
 */
TEST(ExtractWVContentMetadata, ValidWVMetadataExtraction) {
    std::cout << "Entering ValidWVMetadataExtraction test" << std::endl;
    
    // Prepare input using fixed-size array and strncpy
    const char* inputStr = "valid_wv_pssh_data";
    char psshData[256] = {0};
    std::strncpy(psshData, inputStr, sizeof(psshData) - 1);
    int dataLength = 18;
    
    std::cout << "Invoking DrmUtils::extractWVContentMetadataFromPssh with psshData: " << psshData 
              << " and dataLength: " << dataLength << std::endl;
    
    std::string result;
    EXPECT_NO_THROW(result = DrmUtils::extractWVContentMetadataFromPssh(psshData, dataLength));
    
    std::cout << "Received result: " << result << std::endl;
    
    EXPECT_EQ(result, "ExtractedWVContentMetadata");
    
    std::cout << "Exiting ValidWVMetadataExtraction test" << std::endl;
}
/**
 * @brief Validates extraction of Widevine metadata when provided with a null psshData pointer.
 *
 * This test case checks that the function DrmUtils::extractWVContentMetadataFromPssh handles a null pointer for psshData without throwing an exception. It ensures that an empty string is returned when no valid data is provided, thus verifying robust error handling.
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
 * | Variation / Step | Description | Test Data |Expected Result |Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Log the entry message for the test start | No data input | "Entering NullPsshData test" printed to console | Should be successful |
 * | 02 | Initialize inputs by setting psshData to nullptr and dataLength to 10 | psshData = nullptr, dataLength = 10 | Variables initialized correctly | Should be successful |
 * | 03 | Invoke DrmUtils::extractWVContentMetadataFromPssh with the initialized inputs | psshData = nullptr, dataLength = 10 | Function does not throw exception and returns an empty string | Should Pass |
 * | 04 | Validate that the returned result is an empty string | output result = "" | EXPECT_EQ(result, "") passes | Should Pass |
 * | 05 | Log the exit message for the test end | No data input | "Exiting NullPsshData test" printed to console | Should be successful |
 */
TEST(ExtractWVContentMetadata, NullPsshData) {
    std::cout << "Entering NullPsshData test" << std::endl;
    
    // Passing null pointer for psshData
    const char* psshData = nullptr;
    int dataLength = 10;
    
    std::cout << "Invoking DrmUtils::extractWVContentMetadataFromPssh with psshData: nullptr and dataLength: " 
              << dataLength << std::endl;
    
    std::string result;
    EXPECT_NO_THROW(result = DrmUtils::extractWVContentMetadataFromPssh(psshData, dataLength));
    
    std::cout << "Received result: " << result << std::endl;
    
    EXPECT_EQ(result, "");
    
    std::cout << "Exiting NullPsshData test" << std::endl;
}
/**
 * @brief Test to validate extraction of Widevine content metadata when provided a non-null psshData with zero data length.
 *
 * This test verifies that when a non-null psshData is provided with a zero data length, the
 * DrmUtils::extractWVContentMetadataFromPssh function does not throw an exception and returns an empty string.
 * This ensures proper handling of edge cases with minimal or missing data.
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
 * | 01 | Prepare input buffer with valid non-null psshData and zero data length | inputStr = any_data, psshData = any_data, dataLength = 0 | psshData is correctly initialized with "any_data" and dataLength is set to 0 | Should be successful |
 * | 02 | Invoke extractWVContentMetadataFromPssh API and capture result | psshData = any_data, dataLength = 0, output: result | API does not throw an exception and returns an empty string | Should Pass |
 */
TEST(ExtractWVContentMetadata, ZeroDataLengthNonNullPsshData) {
    std::cout << "Entering ZeroDataLengthNonNullPsshData test" << std::endl;
    
    // Prepare input using fixed-size array and strncpy
    const char* inputStr = "any_data";
    char psshData[256] = {0};
    std::strncpy(psshData, inputStr, sizeof(psshData) - 1);
    int dataLength = 0;
    
    std::cout << "Invoking DrmUtils::extractWVContentMetadataFromPssh with psshData: " << psshData 
              << " and dataLength: " << dataLength << std::endl;
    
    std::string result;
    EXPECT_NO_THROW(result = DrmUtils::extractWVContentMetadataFromPssh(psshData, dataLength));
    
    std::cout << "Received result: " << result << std::endl;
    
    EXPECT_EQ(result, "");
    
    std::cout << "Exiting ZeroDataLengthNonNullPsshData test" << std::endl;
}
/**
 * @brief Verify that DrmUtils::extractWVContentMetadataFromPssh handles negative data length appropriately.
 *
 * This test ensures that when a negative value is provided for dataLength while invoking
 * DrmUtils::extractWVContentMetadataFromPssh with valid psshData, the function does not throw an exception
 * and returns an empty string. This behavior is critical to validate the function's robustness against invalid input.
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
 * | Variation / Step | Description                                                                                         | Test Data                                               | Expected Result                                                                                     | Notes          |
 * | :--------------: | --------------------------------------------------------------------------------------------------- | ------------------------------------------------------- | ---------------------------------------------------------------------------------------------------- | -------------- |
 * | 01               | Prepare input: create a constant input string, copy it into a fixed-size character array, and set a negative data length. | inputStr = "any_data", psshData = "any_data", dataLength = -5 | Input parameters are prepared correctly.                                                           | Should be successful |
 * | 02               | Invoke DrmUtils::extractWVContentMetadataFromPssh with the prepared psshData and negative dataLength.  | psshData = "any_data", dataLength = -5                    | The API does not throw an exception upon invocation.                                               | Should Pass    |
 * | 03               | Validate the result: check that the returned string is empty.                                         | result = output string from API                         | The result should be an empty string as the negative data length is invalid.                         | Should Pass    |
 */
TEST(ExtractWVContentMetadata, NegativeDataLengthValidPsshData) {
    std::cout << "Entering NegativeDataLengthValidPsshData test" << std::endl;
    
    // Prepare input using fixed-size array and strncpy
    const char* inputStr = "any_data";
    char psshData[256] = {0};
    std::strncpy(psshData, inputStr, sizeof(psshData) - 1);
    int dataLength = -5;
    
    std::cout << "Invoking DrmUtils::extractWVContentMetadataFromPssh with psshData: " << psshData 
              << " and dataLength: " << dataLength << std::endl;
    
    std::string result;
    EXPECT_NO_THROW(result = DrmUtils::extractWVContentMetadataFromPssh(psshData, dataLength));
    
    std::cout << "Received result: " << result << std::endl;
    
    EXPECT_EQ(result, "");
    
    std::cout << "Exiting NegativeDataLengthValidPsshData test" << std::endl;
}
/**
 * @brief Test that extractWVContentMetadataFromPssh returns an empty string when provided with an empty psshData and zero dataLength
 *
 * This test verifies that when psshData is an empty string and dataLength is set to 0, the API DrmUtils::extractWVContentMetadataFromPssh executes without throwing any exceptions and returns an empty string as output. This ensures that the function correctly handles edge cases with minimal input.
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
 * | Variation / Step | Description                                                                                  | Test Data                                   | Expected Result                                             | Notes         |
 * | :--------------: | -------------------------------------------------------------------------------------------- | ------------------------------------------- | ----------------------------------------------------------- | ------------- |
 * | 01               | Prepare the input by initializing psshData with an empty string and setting dataLength to 0 | psshData = "", dataLength = 0               | Input is correctly initialized                              | Should be successful |
 * | 02               | Invoke DrmUtils::extractWVContentMetadataFromPssh with the prepared psshData and dataLength     | psshData = "", dataLength = 0               | The API does not throw any exceptions                        | Should Pass   |
 * | 03               | Validate that the result returned by the API is an empty string                              | Output result from API call, expected ""    | The returned string equals "" as verified using EXPECT_EQ     | Should Pass   |
 */
TEST(ExtractWVContentMetadata, EmptyStringPsshDataZeroDataLength) {
    std::cout << "Entering EmptyStringPsshDataZeroDataLength test" << std::endl;
    
    // Prepare input using fixed-size array and strncpy
    const char* inputStr = "";
    char psshData[256] = {0};
    std::strncpy(psshData, inputStr, sizeof(psshData) - 1);
    int dataLength = 0;
    
    std::cout << "Invoking DrmUtils::extractWVContentMetadataFromPssh with psshData: \"" << psshData 
              << "\" and dataLength: " << dataLength << std::endl;
    
    std::string result;
    EXPECT_NO_THROW(result = DrmUtils::extractWVContentMetadataFromPssh(psshData, dataLength));
    
    std::cout << "Received result: " << result << std::endl;
    
    EXPECT_EQ(result, "");
    
    std::cout << "Exiting EmptyStringPsshDataZeroDataLength test" << std::endl;
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
