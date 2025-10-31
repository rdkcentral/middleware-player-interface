
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
#include "DrmUtils.h"

using namespace DrmUtils;


/**
 * @brief Validate that the convertEndianness API properly converts a 16-byte input by reversing specified segments.
 *
 * This test verifies that the DrmUtils::convertEndianness function correctly reverses the order of the first four bytes, the next two bytes, and the following two bytes of a standard 16-byte input array while keeping the rest of the bytes unchanged. The verification includes checking that no exceptions are thrown during the conversion and that the output array exactly matches the expected byte order.
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
 * | :--------------: | ----------- | --------- | ------------- | ----- |
 * | 01 | Log the start of the test execution | None | "Entering ValidConversionWithStandard16ByteInput test" is printed to the console | Should be successful |
 * | 02 | Prepare the input arrays with standard 16-byte values and initialize output array | original = [0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10], guidBytes = [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0] | Input arrays are initialized properly | Should be successful |
 * | 03 | Debug print the input array values to the console | original array values | Input array values printed in hex format | Should be successful |
 * | 04 | Invoke DrmUtils::convertEndianness using EXPECT_NO_THROW to perform conversion | input1 = original (16-byte array), output1 = guidBytes (16-byte array pointer) | No exception is thrown and the conversion is executed | Should Pass |
 * | 05 | Debug print the output array values after conversion | guidBytes array after API call | Output array values printed in hex format | Should be successful |
 * | 06 | Debug print the expected array values for verification | expected = [0x04, 0x03, 0x02, 0x01, 0x06, 0x05, 0x08, 0x07, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10] | Expected array values printed in hex format | Should be successful |
 * | 07 | Validate each byte of the output array against expected values using EXPECT_EQ | For each index i: input = guidBytes[i], expected = expected[i] | All corresponding bytes match exactly; any mismatch is asserted with a failure message | Should Pass |
 * | 08 | Log the end of the test execution | None | "Exiting ValidConversionWithStandard16ByteInput test" is printed to the console | Should be successful |
 */
TEST(ConvertEndiannessTest, ValidConversionWithStandard16ByteInput) {
    std::cout << "Entering ValidConversionWithStandard16ByteInput test" << std::endl;
    
    // Prepare input arrays
    unsigned char original[16] = {0x01, 0x02, 0x03, 0x04,
                                  0x05, 0x06, 0x07, 0x08,
                                  0x09, 0x0A, 0x0B, 0x0C,
                                  0x0D, 0x0E, 0x0F, 0x10};
    unsigned char guidBytes[16] = {0};

    // Debug: print input values
    std::cout << "Invoking DrmUtils::convertEndianness with original = [";
    for (int i = 0; i < 16; i++) {
        std::cout << "0x" << std::hex << static_cast<int>(original[i]);
        if(i != 15) std::cout << ", ";
    }
    std::cout << "]" << std::dec << std::endl;
    
    // Invocation of the method using EXPECT_NO_THROW
    EXPECT_NO_THROW({
        std::cout << "Calling DrmUtils::convertEndianness(original, guidBytes)" << std::endl;
        DrmUtils::convertEndianness(original, guidBytes);
    });
    
    // Debug: print output values
    std::cout << "Returned guidBytes = [";
    for (int i = 0; i < 16; i++) {
        std::cout << "0x" << std::hex << static_cast<int>(guidBytes[i]);
        if(i != 15) std::cout << ", ";
    }
    std::cout << "]" << std::dec << std::endl;
    
    // Expected result: bytes 0-3, 4-5, 6-7 reversed.
    unsigned char expected[16] = {0x04, 0x03, 0x02, 0x01,
                                  0x06, 0x05, 0x08, 0x07,
                                  0x09, 0x0A, 0x0B, 0x0C,
                                  0x0D, 0x0E, 0x0F, 0x10};
    
    // Debug: print expected values
    std::cout << "Expected guidBytes = [";
    for (int i = 0; i < 16; i++) {
        std::cout << "0x" << std::hex << static_cast<int>(expected[i]);
        if(i != 15) std::cout << ", ";
    }
    std::cout << "]" << std::dec << std::endl;
    
    // Validate the result byte-by-byte
    for (int i = 0; i < 16; i++) {
        EXPECT_EQ(guidBytes[i], expected[i]) << "Mismatch at index " << i;
    }
    
    std::cout << "Exiting ValidConversionWithStandard16ByteInput test" << std::endl;
}
/**
 * @brief Test for verifying convertEndianness functionality with an input array that contains 16 zero bytes.
 *
 * This test validates that the DrmUtils::convertEndianness function correctly processes an input array of 16 zero bytes by converting the endianness without throwing exceptions. The output guidBytes array is expected to match the input zeros exactly.
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
 * | Variation / Step | Description                                                                                         | Test Data                                                                                                                     | Expected Result                                                                                                                  | Notes         |
 * | :--------------: | --------------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------- | ------------- |
 * | 01               | Initialize input arrays with 16 zeros, invoke DrmUtils::convertEndianness, and validate output array | original = [0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00], guidBytes = [0x00,...], expected = [0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00] | DrmUtils::convertEndianness executes without throwing and the guidBytes array exactly matches the expected array of zeros              | Should Pass   |
 */
TEST(ConvertEndiannessTest, ValidConversionWith16ZeroBytes) {
    std::cout << "Entering ValidConversionWith16ZeroBytes test" << std::endl;
    
    // Prepare input arrays with 16 zeros
    unsigned char original[16] = {0};
    unsigned char guidBytes[16] = {0};

    // Debug: print input values
    std::cout << "Invoking DrmUtils::convertEndianness with original = [";
    for (int i = 0; i < 16; i++) {
        std::cout << "0x" << std::hex << static_cast<int>(original[i]);
        if(i != 15) std::cout << ", ";
    }
    std::cout << "]" << std::dec << std::endl;
    
    // Invocation of the method using EXPECT_NO_THROW
    EXPECT_NO_THROW({
        std::cout << "Calling DrmUtils::convertEndianness(original, guidBytes)" << std::endl;
        DrmUtils::convertEndianness(original, guidBytes);
    });
    
    // Debug: print output values
    std::cout << "Returned guidBytes = [";
    for (int i = 0; i < 16; i++) {
        std::cout << "0x" << std::hex << static_cast<int>(guidBytes[i]);
        if(i != 15) std::cout << ", ";
    }
    std::cout << "]" << std::dec << std::endl;
    
    // Expected: same as input zeros.
    unsigned char expected[16] = {0};
    
    // Debug: print expected values
    std::cout << "Expected guidBytes = [";
    for (int i = 0; i < 16; i++) {
        std::cout << "0x" << std::hex << static_cast<int>(expected[i]);
        if(i != 15) std::cout << ", ";
    }
    std::cout << "]" << std::dec << std::endl;
    
    // Validate the result byte-by-byte
    for (int i = 0; i < 16; i++) {
        EXPECT_EQ(guidBytes[i], expected[i]) << "Mismatch at index " << i;
    }
    
    std::cout << "Exiting ValidConversionWith16ZeroBytes test" << std::endl;
}
/**
 * @brief Validate that convertEndianness correctly handles a NULL original pointer.
 *
 * This test verifies that passing a NULL pointer as the original parameter to DrmUtils::convertEndianness
 * leads to a controlled failure (i.e., death) as expected. This helps in ensuring that the function properly
 * asserts against invalid input and prevents undefined behavior.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoke DrmUtils::convertEndianness with a NULL original pointer and a destination buffer initialized to zeros. | original = nullptr, guidBytes = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0} | API call should result in death (crash/assertion) as detected by EXPECT_DEATH. | Should Fail |
 */
TEST(ConvertEndiannessTest, NegativeCaseWithNullOriginalPointer) {
    std::cout << "Entering NegativeCaseWithNullOriginalPointer test" << std::endl;
    
    // Prepare destination buffer
    unsigned char guidBytes[16] = {0};
    
    // Debug: Log that original pointer is NULL.
    std::cout << "Setting original pointer to NULL" << std::endl;
    
    // Invocation and expect death because NULL original may cause crash/assertion.
    EXPECT_DEATH({
        std::cout << "Calling DrmUtils::convertEndianness(NULL, guidBytes)" << std::endl;
        DrmUtils::convertEndianness(nullptr, guidBytes);
    }, ".*");
    
    std::cout << "Exiting NegativeCaseWithNullOriginalPointer test" << std::endl;
}
/**
 * @brief Verify that convertEndianness function gracefully handles a null destination pointer in a negative test scenario.
 *
 * This test validates the error handling of the convertEndianness API when provided with a null destination pointer. It ensures that the API triggers a death assertion, indicating proper handling of invalid input and reinforcing the robustness of the system.
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
 * | Variation / Step | Description                                                                                | Test Data                                                                                                                                                                  | Expected Result                                             | Notes       |
 * | :--------------: | ------------------------------------------------------------------------------------------ | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ---------------------------------------------------------- | ----------- |
 * | 01               | Invoke DrmUtils::convertEndianness with a valid source buffer and a NULL destination pointer | source buffer = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10}, destination = nullptr | API terminates execution (death test) due to null destination pointer | Should Fail |
 */
TEST(ConvertEndiannessTest, NegativeCaseWithNullDestinationPointer) {
    std::cout << "Entering NegativeCaseWithNullDestinationPointer test" << std::endl;
    
    // Prepare source buffer with valid data.
    unsigned char original[16] = {0x01, 0x02, 0x03, 0x04,
                                  0x05, 0x06, 0x07, 0x08,
                                  0x09, 0x0A, 0x0B, 0x0C,
                                  0x0D, 0x0E, 0x0F, 0x10};
    
    // Debug: Log that guidBytes pointer is NULL.
    std::cout << "Setting guidBytes pointer to NULL" << std::endl;
    
    // Invocation and expect death because NULL destination may cause crash/assertion.
    EXPECT_DEATH({
        std::cout << "Calling DrmUtils::convertEndianness(original, NULL)" << std::endl;
        DrmUtils::convertEndianness(original, nullptr);
    }, ".*");
    
    std::cout << "Exiting NegativeCaseWithNullDestinationPointer test" << std::endl;
}
/**
 * @brief Validate proper extraction using valid PSSH input data.
 *
 * This test verifies that DrmUtils::extractDataFromPssh function correctly extracts the "meta_data" segment when provided with valid, well-formed PSSH input data. It ensures that the returned pointer is non-null and that the extracted substring and its length exactly match the expected values.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 005@n
 * **Priority:** High@n
 * @n
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * @n
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Initialize and configure valid PSSH test data | psshData = "SOMEPSH_START_meta_data_ENDPSH", dataLength = 31, startStr = "START_", endStr = "_END", verStr = "v1" | Input variables are correctly initialized for API invocation | Should be successful |
 * | 02 | Invoke DrmUtils::extractDataFromPssh API with valid inputs | input1 = psshData, input2 = dataLength, input3 = startStr, input4 = endStr, input5 = verStr, output1 = retData, output2 = len | The API returns a non-null pointer with extracted valid data and correct length | Should Pass |
 * | 03 | Validate extracted data and length | retData = pointer to extracted data, len = length of extracted data | retData is not nullptr; extracted data equals "meta_data" and len equals 9 | Should Pass |
 */
TEST(DrmUtilsTest, PositiveExtractionValidData) {
    std::cout << "Entering PositiveExtractionValidData test" << std::endl;

    // Prepare input using strncpy for fixed size arrays
    char psshData[50];
    const char* srcData = "SOMEPSH_START_meta_data_ENDPSH";
    strncpy(psshData, srcData, sizeof(psshData));
    psshData[sizeof(psshData)-1] = '\0';
    int dataLength = 31;
    char startStr[10];
    strncpy(startStr, "START_", sizeof(startStr));
    startStr[sizeof(startStr)-1] = '\0';
    char endStr[10];
    strncpy(endStr, "_END", sizeof(endStr));
    endStr[sizeof(endStr)-1] = '\0';
    char verStr[5];
    strncpy(verStr, "v1", sizeof(verStr));
    verStr[sizeof(verStr)-1] = '\0';
    int len = 0;

    std::cout << "Invoking DrmUtils::extractDataFromPssh with:" << std::endl;
    std::cout << "  psshData: " << psshData << std::endl;
    std::cout << "  dataLength: " << dataLength << std::endl;
    std::cout << "  startStr: " << startStr << std::endl;
    std::cout << "  endStr: " << endStr << std::endl;
    std::cout << "  verStr: " << verStr << std::endl;

    unsigned char* retData = DrmUtils::extractDataFromPssh(psshData, dataLength, startStr, endStr, &len, verStr);

    std::cout << "Returned pointer: " << static_cast<void*>(retData) << std::endl;
    if(retData) {
        std::cout << "Extracted data: " << retData << std::endl;
    }
    std::cout << "Extracted length: " << len << std::endl;

    EXPECT_NE(retData, nullptr);
    if(retData) {
        EXPECT_STREQ(reinterpret_cast<char*>(retData), "meta_data");
        EXPECT_EQ(len, 9);
    }

    std::cout << "Exiting PositiveExtractionValidData test" << std::endl;
}
/**
 * @brief Validate that DrmUtils::extractDataFromPssh returns nullptr when the start delimiter is missing.
 *
 * This test verifies that when the provided psshData does not contain the required start delimiter ("START_"),
 * the DrmUtils::extractDataFromPssh function correctly returns a nullptr. This behavior ensures that the API
 * properly handles malformed input data by not extracting any data when the delimiter is missing.
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
 * | Variation / Step | Description                                                                                       | Test Data                                                                                                     | Expected Result                                               | Notes       |
 * | :--------------: | ------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------- | ----------- |
 * | 01               | Invoke DrmUtils::extractDataFromPssh with a psshData missing the "START_" delimiter               | psshData = SOMEPSH_meta_data_ENDPSH, dataLength = 26, startStr = START_, endStr = _END, verStr = v1, len pointer | API returns nullptr; Assertion check EXPECT_EQ(retData, nullptr) | Should Pass |
 */
TEST(DrmUtilsTest, MissingStartDelimiter) {
    std::cout << "Entering MissingStartDelimiter test" << std::endl;

    char psshData[50];
    const char* srcData = "SOMEPSH_meta_data_ENDPSH";
    strncpy(psshData, srcData, sizeof(psshData));
    psshData[sizeof(psshData)-1] = '\0';
    int dataLength = 26;
    char startStr[10];
    strncpy(startStr, "START_", sizeof(startStr));
    startStr[sizeof(startStr)-1] = '\0';
    char endStr[10];
    strncpy(endStr, "_END", sizeof(endStr));
    endStr[sizeof(endStr)-1] = '\0';
    char verStr[5];
    strncpy(verStr, "v1", sizeof(verStr));
    verStr[sizeof(verStr)-1] = '\0';
    int len = 0;

    std::cout << "Invoking DrmUtils::extractDataFromPssh with missing start delimiter" << std::endl;
    std::cout << "  psshData: " << psshData << std::endl;
    std::cout << "  dataLength: " << dataLength << std::endl;
    std::cout << "  startStr: " << startStr << ", endStr: " << endStr << ", verStr: " << verStr << std::endl;

    unsigned char* retData = DrmUtils::extractDataFromPssh(psshData, dataLength, startStr, endStr, &len, verStr);

    std::cout << "Returned pointer: " << static_cast<void*>(retData) << std::endl;
    std::cout << "Length value after call: " << len << std::endl;

    EXPECT_EQ(retData, nullptr);

    std::cout << "Exiting MissingStartDelimiter test" << std::endl;
}
/**
 * @brief Validate extractDataFromPssh behavior when the end delimiter is missing in the PSSH data.
 *
 * This test verifies that when the provided PSSH data does not contain the expected end delimiter, 
 * the extractDataFromPssh function returns a null pointer. The test ensures that the API correctly 
 * handles malformed input by not extracting any data.
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
 * | Variation / Step | Description                                                              | Test Data                                                                                          | Expected Result                                           | Notes       |
 * | :--------------: | ------------------------------------------------------------------------ | -------------------------------------------------------------------------------------------------- | ---------------------------------------------------------- | ----------- |
 * | 01               | Invoke extractDataFromPssh with missing end delimiter in the PSSH data     | psshData = "SOMEPSH_START_meta_data", dataLength = 23, startStr = "START_", endStr = "_END", verStr = "v1", output pointer retData | retData should be nullptr (indicating extraction failure) | Should Fail |
 */
TEST(DrmUtilsTest, MissingEndDelimiter) {
    std::cout << "Entering MissingEndDelimiter test" << std::endl;

    char psshData[50];
    const char* srcData = "SOMEPSH_START_meta_data";
    strncpy(psshData, srcData, sizeof(psshData));
    psshData[sizeof(psshData)-1] = '\0';
    int dataLength = 23;
    char startStr[10];
    strncpy(startStr, "START_", sizeof(startStr));
    startStr[sizeof(startStr)-1] = '\0';
    char endStr[10];
    strncpy(endStr, "_END", sizeof(endStr));
    endStr[sizeof(endStr)-1] = '\0';
    char verStr[5];
    strncpy(verStr, "v1", sizeof(verStr));
    verStr[sizeof(verStr)-1] = '\0';
    int len = 0;

    std::cout << "Invoking DrmUtils::extractDataFromPssh missing end delimiter" << std::endl;
    std::cout << "  psshData: " << psshData << ", dataLength: " << dataLength << std::endl;
    std::cout << "  startStr: " << startStr << ", endStr: " << endStr << ", verStr: " << verStr << std::endl;

    unsigned char* retData = DrmUtils::extractDataFromPssh(psshData, dataLength, startStr, endStr, &len, verStr);

    std::cout << "Returned pointer: " << static_cast<void*>(retData) << std::endl;
    std::cout << "Length value after call: " << len << std::endl;

    EXPECT_EQ(retData, nullptr);

    std::cout << "Exiting MissingEndDelimiter test" << std::endl;
}
/**
 * @brief Test to verify the behavior of DrmUtils::extractDataFromPssh when psshData is NULL
 *
 * This test ensures that when a NULL pointer is passed as the psshData to DrmUtils::extractDataFromPssh, 
 * the API returns a NULL pointer. This is important to verify how the function handles invalid input parameters.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoke DrmUtils::extractDataFromPssh with psshData set to NULL | psshData = nullptr, dataLength = 10, startStr = "START_", endStr = "_END", verStr = "v1", len pointer = valid pointer | Function returns nullptr and EXPECT_EQ assertion validates the returned pointer is nullptr | Should Pass |
 */
TEST(DrmUtilsTest, PsshDataIsNull) {
    std::cout << "Entering PsshDataIsNull test" << std::endl;

    const char* psshData = nullptr;
    int dataLength = 10;
    char startStr[10];
    strncpy(startStr, "START_", sizeof(startStr));
    startStr[sizeof(startStr)-1] = '\0';
    char endStr[10];
    strncpy(endStr, "_END", sizeof(endStr));
    endStr[sizeof(endStr)-1] = '\0';
    char verStr[5];
    strncpy(verStr, "v1", sizeof(verStr));
    verStr[sizeof(verStr)-1] = '\0';
    int len = 0;

    std::cout << "Invoking DrmUtils::extractDataFromPssh with psshData as NULL" << std::endl;
    std::cout << "  dataLength: " << dataLength << ", startStr: " << startStr 
              << ", endStr: " << endStr << ", verStr: " << verStr << std::endl;

    unsigned char* retData = DrmUtils::extractDataFromPssh(psshData, dataLength, startStr, endStr, &len, verStr);

    std::cout << "Returned pointer: " << static_cast<void*>(retData) << std::endl;

    EXPECT_EQ(retData, nullptr);

    std::cout << "Exiting PsshDataIsNull test" << std::endl;
}
/**
 * @brief Validate extraction behavior with data length zero.
 *
 * This test verifies that when the dataLength parameter is set to zero, the DrmUtils::extractDataFromPssh API returns a nullptr, indicating that no data extraction occurs.
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
 * | 01 | Initialize input parameters and invoke extractDataFromPssh with zero dataLength | psshData = "SOMEPSH_START_meta_data_END", dataLength = 0, startStr = "START_", endStr = "_END", verStr = "v1", len = 0 | API returns nullptr; EXPECT_EQ verifies that retData is nullptr | Should Pass |
 */
TEST(DrmUtilsTest, DataLengthZero) {
    std::cout << "Entering DataLengthZero test" << std::endl;

    char psshData[50];
    const char* srcData = "SOMEPSH_START_meta_data_END";
    strncpy(psshData, srcData, sizeof(psshData));
    psshData[sizeof(psshData)-1] = '\0';
    int dataLength = 0;
    char startStr[10];
    strncpy(startStr, "START_", sizeof(startStr));
    startStr[sizeof(startStr)-1] = '\0';
    char endStr[10];
    strncpy(endStr, "_END", sizeof(endStr));
    endStr[sizeof(endStr)-1] = '\0';
    char verStr[5];
    strncpy(verStr, "v1", sizeof(verStr));
    verStr[sizeof(verStr)-1] = '\0';
    int len = 0;

    std::cout << "Invoking DrmUtils::extractDataFromPssh with dataLength zero" << std::endl;
    std::cout << "  psshData: " << psshData << ", dataLength: " << dataLength << std::endl;
    std::cout << "  startStr: " << startStr << ", endStr: " << endStr << ", verStr: " << verStr << std::endl;

    unsigned char* retData = DrmUtils::extractDataFromPssh(psshData, dataLength, startStr, endStr, &len, verStr);

    std::cout << "Returned pointer: " << static_cast<void*>(retData) << std::endl;

    EXPECT_EQ(retData, nullptr);

    std::cout << "Exiting DataLengthZero test" << std::endl;
}
/**
 * @brief Verify that extractDataFromPssh returns a nullptr when the length pointer is NULL.
 *
 * This test case validates that the DrmUtils::extractDataFromPssh method correctly handles a scenario where the length pointer parameter is provided as NULL. It ensures that the function does not process the length pointer and returns a null pointer, as expected, under these conditions.
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
 * | 01 | Invoke DrmUtils::extractDataFromPssh with valid pssh data, valid dataLength and string identifiers but with a NULL length pointer. | psshData = "SOMEPSH_START_meta_data_END", dataLength = 26, startStr = "START_", endStr = "_END", length pointer = NULL, verStr = "v1" | The function returns a nullptr and the EXPECT_EQ check confirms the returned value is nullptr. | Should Pass |
 */
TEST(DrmUtilsTest, LenPointerIsNull) {
    std::cout << "Entering LenPointerIsNull test" << std::endl;

    char psshData[50];
    const char* srcData = "SOMEPSH_START_meta_data_END";
    strncpy(psshData, srcData, sizeof(psshData));
    psshData[sizeof(psshData)-1] = '\0';
    int dataLength = 26;
    char startStr[10];
    strncpy(startStr, "START_", sizeof(startStr));
    startStr[sizeof(startStr)-1] = '\0';
    char endStr[10];
    strncpy(endStr, "_END", sizeof(endStr));
    endStr[sizeof(endStr)-1] = '\0';
    char verStr[5];
    strncpy(verStr, "v1", sizeof(verStr));
    verStr[sizeof(verStr)-1] = '\0';

    std::cout << "Invoking DrmUtils::extractDataFromPssh with len pointer as NULL" << std::endl;
    std::cout << "  psshData: " << psshData << ", dataLength: " << dataLength << std::endl;
    std::cout << "  startStr: " << startStr << ", endStr: " << endStr << ", verStr: " << verStr << std::endl;

    unsigned char* retData = DrmUtils::extractDataFromPssh(psshData, dataLength, startStr, endStr, nullptr, verStr);

    std::cout << "Returned pointer: " << static_cast<void*>(retData) << std::endl;

    EXPECT_EQ(retData, nullptr);

    std::cout << "Exiting LenPointerIsNull test" << std::endl;
}
/**
 * @brief Verifies that the extractDataFromPssh API returns a null pointer when the verStr argument is null
 *
 * This test checks the behavior of DrmUtils::extractDataFromPssh when invoked with a null pointer for the verStr parameter.
 * It ensures that the function handles the null verStr correctly by returning a null pointer. This behavior is important to avoid
 * potential crashes or undefined behavior when the API is misused.
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
 * | Variation / Step | Description                                                               | Test Data                                                                                                    | Expected Result                                                          | Notes       |
 * | :--------------: | ------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------ | ------------------------------------------------------------------------ | ----------- |
 * | 01               | Invoke DrmUtils::extractDataFromPssh with psshData, dataLength, startStr, endStr, pointer to len, and verStr as null | psshData = SOMEPSH_START_meta_data_END, dataLength = 26, startStr = START_, endStr = _END, len = &len, verStr = nullptr | API should return a nullptr and the assertion (EXPECT_EQ) should pass      | Should Pass |
 */
TEST(DrmUtilsTest, VerStrIsNull) {
    std::cout << "Entering VerStrIsNull test" << std::endl;

    char psshData[50];
    const char* srcData = "SOMEPSH_START_meta_data_END";
    strncpy(psshData, srcData, sizeof(psshData));
    psshData[sizeof(psshData)-1] = '\0';
    int dataLength = 26;
    char startStr[10];
    strncpy(startStr, "START_", sizeof(startStr));
    startStr[sizeof(startStr)-1] = '\0';
    char endStr[10];
    strncpy(endStr, "_END", sizeof(endStr));
    endStr[sizeof(endStr)-1] = '\0';
    int len = 0;

    std::cout << "Invoking DrmUtils::extractDataFromPssh with verStr as NULL" << std::endl;
    std::cout << "  psshData: " << psshData << ", dataLength: " << dataLength << std::endl;
    std::cout << "  startStr: " << startStr << ", endStr: " << endStr << std::endl;

    unsigned char* retData = DrmUtils::extractDataFromPssh(psshData, dataLength, startStr, endStr, &len, nullptr);

    std::cout << "Returned pointer: " << static_cast<void*>(retData) << std::endl;

    EXPECT_EQ(retData, nullptr);

    std::cout << "Exiting VerStrIsNull test" << std::endl;
}
/**
 * @brief Tests extraction of metadata from PSSH data using identical start and end delimiters
 *
 * This test verifies that DrmUtils::extractDataFromPssh correctly extracts the metadata from a given PSSH string when the start and end delimiters are identical. It specifically checks that the returned pointer is not null, the extracted data matches "meta_data", and the length of the extracted data is 9.
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
 * | Variation / Step | Description                                                                                                  | Test Data                                                                                                                         | Expected Result                                                         | Notes       |
 * | :--------------: | ------------------------------------------------------------------------------------------------------------ | --------------------------------------------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------- | ----------- |
 * | 01               | Invoke DrmUtils::extractDataFromPssh with psshData containing "SOMEPSH_DELIMmeta_dataDELIMPSH", dataLength = 29, startStr = "DELIM", endStr = "DELIM", and verStr = "v1". | psshData = SOMEPSH_DELIMmeta_dataDELIMPSH, dataLength = 29, startStr = DELIM, endStr = DELIM, verStr = v1, len output pointer | Return pointer not null; extracted data equals "meta_data" and len equals 9 | Should Pass |
 */
TEST(DrmUtilsTest, IdenticalStartEndDelimiters) {
    std::cout << "Entering IdenticalStartEndDelimiters test" << std::endl;

    char psshData[50];
    const char* srcData = "SOMEPSH_DELIMmeta_dataDELIMPSH";
    strncpy(psshData, srcData, sizeof(psshData));
    psshData[sizeof(psshData)-1] = '\0';
    int dataLength = 29;
    char startStr[10];
    strncpy(startStr, "DELIM", sizeof(startStr));
    startStr[sizeof(startStr)-1] = '\0';
    char endStr[10];
    strncpy(endStr, "DELIM", sizeof(endStr));
    endStr[sizeof(endStr)-1] = '\0';
    char verStr[5];
    strncpy(verStr, "v1", sizeof(verStr));
    verStr[sizeof(verStr)-1] = '\0';
    int len = 0;

    std::cout << "Invoking DrmUtils::extractDataFromPssh with identical delimiters" << std::endl;
    std::cout << "  psshData: " << psshData << ", dataLength: " << dataLength << std::endl;
    std::cout << "  startStr & endStr: " << startStr << ", verStr: " << verStr << std::endl;

    unsigned char* retData = DrmUtils::extractDataFromPssh(psshData, dataLength, startStr, endStr, &len, verStr);

    std::cout << "Returned pointer: " << static_cast<void*>(retData) << std::endl;
    if(retData) {
        std::cout << "Extracted data: " << retData << std::endl;
    }
    std::cout << "Extracted length: " << len << std::endl;

    EXPECT_NE(retData, nullptr);
    if(retData) {
        EXPECT_STREQ(reinterpret_cast<char*>(retData), "meta_data");
        EXPECT_EQ(len, 9);
    }

    std::cout << "Exiting IdenticalStartEndDelimiters test" << std::endl;
}
/**
 * @brief Test to verify extraction of data from a PSSH buffer when multiple occurrences of delimiters are present
 *
 * This test validates that DrmUtils::extractDataFromPssh properly extracts the first occurrence of data between the provided start and end delimiters even when the pssh data contains multiple valid delimiter sets. It ensures that the extracted string and its length match expected values for a positive scenario.
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
 * | Variation / Step | Description                                                                                   | Test Data                                                                                                                       | Expected Result                                                                              | Notes        |
 * | :--------------: | --------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------- | ------------ |
 * | 01               | Initialize pssh data with multiple occurrences of start and end delimiters and invoke function | psshData = "SOMEPSH_START_first_END_START_second_END", dataLength = 41, startStr = "START_", endStr = "_END", verStr = "v1" | Return non-null pointer; extracted string equals "first" and extracted length equals 5 | Should Pass  |
 */
TEST(DrmUtilsTest, MultipleOccurrencesOfDelimiters) {
    std::cout << "Entering MultipleOccurrencesOfDelimiters test" << std::endl;

    char psshData[100];
    const char* srcData = "SOMEPSH_START_first_END_START_second_END";
    strncpy(psshData, srcData, sizeof(psshData));
    psshData[sizeof(psshData)-1] = '\0';
    int dataLength = 41;
    char startStr[10];
    strncpy(startStr, "START_", sizeof(startStr));
    startStr[sizeof(startStr)-1] = '\0';
    char endStr[10];
    strncpy(endStr, "_END", sizeof(endStr));
    endStr[sizeof(endStr)-1] = '\0';
    char verStr[5];
    strncpy(verStr, "v1", sizeof(verStr));
    verStr[sizeof(verStr)-1] = '\0';
    int len = 0;

    std::cout << "Invoking DrmUtils::extractDataFromPssh for multiple occurrences" << std::endl;
    std::cout << "  psshData: " << psshData << ", dataLength: " << dataLength << std::endl;
    std::cout << "  startStr: " << startStr << ", endStr: " << endStr << ", verStr: " << verStr << std::endl;

    unsigned char* retData = DrmUtils::extractDataFromPssh(psshData, dataLength, startStr, endStr, &len, verStr);

    std::cout << "Returned pointer: " << static_cast<void*>(retData) << std::endl;
    if(retData) {
        std::cout << "Extracted data: " << retData << std::endl;
    }
    std::cout << "Extracted length: " << len << std::endl;

    EXPECT_NE(retData, nullptr);
    if(retData) {
        EXPECT_STREQ(reinterpret_cast<char*>(retData), "first");
        EXPECT_EQ(len, 5);
    }

    std::cout << "Exiting MultipleOccurrencesOfDelimiters test" << std::endl;
}
/**
 * @brief Verify that extractDataFromPssh returns nullptr when delimiters are in the wrong order
 *
 * This test checks that DrmUtils::extractDataFromPssh properly handles the situation where the start and end delimiters in the input string are provided in an incorrect order. The function is expected to return a nullptr in this case, ensuring that invalid formatted data is not processed.
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
 * | 01 | Invoke extractDataFromPssh with wrong delimiter order | psshData = "SOMEPSH_END_meta_data_START", dataLength = 27, startStr = "START_", endStr = "_END", verStr = "v1", len = 0 | API returns nullptr indicating failure to extract data due to wrong delimiter order | Should Fail |
 */
TEST(DrmUtilsTest, DelimitersWrongOrder) {
    std::cout << "Entering DelimitersWrongOrder test" << std::endl;

    char psshData[50];
    const char* srcData = "SOMEPSH_END_meta_data_START";
    strncpy(psshData, srcData, sizeof(psshData));
    psshData[sizeof(psshData)-1] = '\0';
    int dataLength = 27;
    char startStr[10];
    strncpy(startStr, "START_", sizeof(startStr));
    startStr[sizeof(startStr)-1] = '\0';
    char endStr[10];
    strncpy(endStr, "_END", sizeof(endStr));
    endStr[sizeof(endStr)-1] = '\0';
    char verStr[5];
    strncpy(verStr, "v1", sizeof(verStr));
    verStr[sizeof(verStr)-1] = '\0';
    int len = 0;

    std::cout << "Invoking DrmUtils::extractDataFromPssh with delimiters in wrong order" << std::endl;
    std::cout << "  psshData: " << psshData << ", dataLength: " << dataLength << std::endl;
    std::cout << "  startStr: " << startStr << ", endStr: " << endStr << ", verStr: " << verStr << std::endl;

    unsigned char* retData = DrmUtils::extractDataFromPssh(psshData, dataLength, startStr, endStr, &len, verStr);

    std::cout << "Returned pointer: " << static_cast<void*>(retData) << std::endl;
    std::cout << "Length value after call: " << len << std::endl;

    EXPECT_EQ(retData, nullptr);

    std::cout << "Exiting DelimitersWrongOrder test" << std::endl;
}
/**
 * @brief Validates that the extraction of the key ID from a Widevine PSSH box works correctly.
 *
 * This test verifies that calling DrmUtils::extractKeyIdFromPssh with a valid PSSH box string for Widevine does not throw an exception, returns a non-null pointer, and produces a key ID of non-zero length. The test also checks the proper memory management by freeing the returned buffer.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Prepare the input parameters, including psshData using strncpy and dataLength. | psshData = "valid_pssh_box_with_key_id", dataLength = 30, keyIdLen initially 0 | psshData is copied correctly with null termination, dataLength is set appropriately | Should be successful |
 * | 02 | Invoke DrmUtils::extractKeyIdFromPssh API to extract the key ID from the given PSSH data. | input: psshData = "valid_pssh_box_with_key_id", dataLength = 30, output: keyIdBuffer, keyIdLen pointer, DRMSystems::eDRM_WIDEVINE | API call does not throw an exception and returns a valid pointer for keyIdBuffer | Should Pass |
 * | 03 | Validate the output by checking that keyIdBuffer is not nullptr and that keyIdLen is greater than 0. | output: keyIdBuffer and keyIdLen as returned by DrmUtils::extractKeyIdFromPssh | keyIdBuffer is non-null and keyIdLen > 0 | Should Pass |
 * | 04 | If a valid keyIdBuffer is returned, print the key ID information and free the allocated memory. | keyIdBuffer containing the key ID bytes | Key information is printed successfully and allocated memory is freed without error | Should be successful |
 */
TEST(DrmUtilsTest, ValidExtraction_Widevine) {
    std::cout << "Entering ValidExtraction_Widevine test" << std::endl;
    
    char psshData[50];
    // Use strncpy as per instruction to assign value to fixed size arrays
    strncpy(psshData, "valid_pssh_box_with_key_id", sizeof(psshData) - 1);
    psshData[sizeof(psshData) - 1] = '\0';
    int dataLength = 30;
    int keyIdLen = 0;
    std::cout << "Invoking DrmUtils::extractKeyIdFromPssh with parameters:" << std::endl;
    std::cout << "psshData: " << psshData << ", dataLength: " << dataLength << ", DRMSystems::eDRM_WIDEVINE" << std::endl;
    
    unsigned char* keyIdBuffer = nullptr;
    EXPECT_NO_THROW(
        keyIdBuffer = DrmUtils::extractKeyIdFromPssh(psshData, dataLength, &keyIdLen, DRMSystems::eDRM_WIDEVINE);
    );
    
    std::cout << "Returned pointer: " << static_cast<const void*>(keyIdBuffer) << std::endl;
    std::cout << "Extracted key length: " << keyIdLen << std::endl;
    
    EXPECT_NE(keyIdBuffer, nullptr);
    EXPECT_GT(keyIdLen, 0);
    
    if (keyIdBuffer != nullptr) {
        std::cout << "Key ID information: ";
        for (int i = 0; i < keyIdLen; i++) {
            std::cout << std::hex << static_cast<int>(keyIdBuffer[i]) << " ";
        }
        std::cout << std::dec << std::endl;
        free(keyIdBuffer);
    }
    
    std::cout << "Exiting ValidExtraction_Widevine test" << std::endl;
}
/**
 * @brief Test to validate key extraction from a valid PlayReady PSSH data input
 *
 * This test verifies that the DrmUtils::extractKeyIdFromPssh API correctly processes a valid PlayReady formatted PSSH data 
 * and extracts a valid key ID without throwing exceptions. It confirms that the API returns a non-null pointer and key length greater than zero.
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
 * | :--------------: | ----------- | --------- | ------------- | ----- |
 * | 01 | Invoke DrmUtils::extractKeyIdFromPssh with valid PlayReady parameters and verify output | psshData = "valid_pssh_box_with_key_id", dataLength = 30, keyIdLen pointer, DRMSystems::eDRM_PLAYREADY | API does not throw; keyIdBuffer is non-null and keyIdLen > 0 | Should Pass |
 */
TEST(DrmUtilsTest, ValidExtraction_PlayReady) {
    std::cout << "Entering ValidExtraction_PlayReady test" << std::endl;
    
    char psshData[50];
    strncpy(psshData, "valid_pssh_box_with_key_id", sizeof(psshData) - 1);
    psshData[sizeof(psshData) - 1] = '\0';
    int dataLength = 30;
    int keyIdLen = 0;
    
    std::cout << "Invoking DrmUtils::extractKeyIdFromPssh with parameters:" << std::endl;
    std::cout << "psshData: " << psshData << ", dataLength: " << dataLength << ", DRMSystems::eDRM_PLAYREADY" << std::endl;
    
    unsigned char* keyIdBuffer = nullptr;
    EXPECT_NO_THROW(
        keyIdBuffer = DrmUtils::extractKeyIdFromPssh(psshData, dataLength, &keyIdLen, DRMSystems::eDRM_PLAYREADY);
    );
    
    std::cout << "Returned pointer: " << static_cast<const void*>(keyIdBuffer) << std::endl;
    std::cout << "Extracted key length: " << keyIdLen << std::endl;
    
    EXPECT_NE(keyIdBuffer, nullptr);
    EXPECT_GT(keyIdLen, 0);
    
    if (keyIdBuffer != nullptr) {
        std::cout << "Key ID information: ";
        for (int i = 0; i < keyIdLen; i++) {
            std::cout << std::hex << static_cast<int>(keyIdBuffer[i]) << " ";
        }
        std::cout << std::dec << std::endl;
        free(keyIdBuffer);
    }
    
    std::cout << "Exiting ValidExtraction_PlayReady test" << std::endl;
}
/**
 * @brief Verify that extractKeyIdFromPssh handles null psshData pointer correctly
 *
 * This test verifies that when a null pointer is passed as the psshData input to DrmUtils::extractKeyIdFromPssh,
 * the function does not throw an exception and returns a null keyIdBuffer. This ensures that the function
 * correctly handles null input scenarios.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 017@n
 * **Priority:** High@n
 * @n
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * @n
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoke DrmUtils::extractKeyIdFromPssh with nullptr for psshData | input: psshData = nullptr, dataLength = 30, keyIdLen pointer, DRMSystems = eDRM_WIDEVINE | API should not throw and should return a nullptr | Should Pass |
 * | 02 | Validate that the returned keyIdBuffer is nullptr | output: keyIdBuffer as returned by API | keyIdBuffer should be equal to nullptr as asserted by EXPECT_EQ | Should Pass |
 */
TEST(DrmUtilsTest, NullPsshDataPointer) {
    std::cout << "Entering NullPsshDataPointer test" << std::endl;
    
    int dataLength = 30;
    int keyIdLen = 0;
    
    std::cout << "Invoking DrmUtils::extractKeyIdFromPssh with nullptr for psshData, dataLength: " << dataLength 
              << ", DRMSystems::eDRM_WIDEVINE" << std::endl;
    
    unsigned char* keyIdBuffer = nullptr;
    EXPECT_NO_THROW(
        keyIdBuffer = DrmUtils::extractKeyIdFromPssh(nullptr, dataLength, &keyIdLen, DRMSystems::eDRM_WIDEVINE);
    );
    
    std::cout << "Returned pointer: " << static_cast<const void*>(keyIdBuffer) << std::endl;
    EXPECT_EQ(keyIdBuffer, nullptr);
    
    std::cout << "Exiting NullPsshDataPointer test" << std::endl;
}
/**
 * @brief Validate the behavior when dataLength is zero in the extractKeyIdFromPssh API.
 *
 * Test verifies that when a valid psshData is provided with zero dataLength, the API does not throw an exception and returns a nullptr for the keyIdBuffer, indicating no key extraction.
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
 * | Variation / Step | Description                                                                                                                           | Test Data                                                           | Expected Result                                                                         | Notes      |
 * | :--------------: | ------------------------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------- | --------------------------------------------------------------------------------------- | ---------- |
 * | 01               | Invoke DrmUtils::extractKeyIdFromPssh with psshData set to "any_data", dataLength as 0, keyIdLen as 0, and DRM system PLAYREADY | psshData = any_data, dataLength = 0, keyIdLen = 0, DRMSystems = eDRM_PLAYREADY | API should not throw an exception and should return a nullptr for keyIdBuffer | Should Pass |
 */
TEST(DrmUtilsTest, ZeroDataLength) {
    std::cout << "Entering ZeroDataLength test" << std::endl;
    
    char psshData[50];
    strncpy(psshData, "any_data", sizeof(psshData) - 1);
    psshData[sizeof(psshData) - 1] = '\0';
    int dataLength = 0;
    int keyIdLen = 0;
    
    std::cout << "Invoking DrmUtils::extractKeyIdFromPssh with psshData: " << psshData 
              << ", dataLength: " << dataLength << ", DRMSystems::eDRM_PLAYREADY" << std::endl;
    
    unsigned char* keyIdBuffer = nullptr;
    EXPECT_NO_THROW(
        keyIdBuffer = DrmUtils::extractKeyIdFromPssh(psshData, dataLength, &keyIdLen, DRMSystems::eDRM_PLAYREADY);
    );
    
    std::cout << "Returned pointer: " << static_cast<const void*>(keyIdBuffer) << std::endl;
    EXPECT_EQ(keyIdBuffer, nullptr);
    
    std::cout << "Exiting ZeroDataLength test" << std::endl;
}
/**
 * @brief Test to verify extractKeyIdFromPssh behavior when len pointer is null
 *
 * This test verifies that when a nullptr is passed for the len parameter in the call to DrmUtils::extractKeyIdFromPssh,
 * the API does not throw an exception and correctly returns a null pointer. This ensures that the function handles the
 * absence of a length output parameter gracefully.
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
 * | Variation / Step | Description                                                                                                          | Test Data                                                                                                              | Expected Result                                                              | Notes        |
 * | :--------------: | -------------------------------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------------- | ------------ |
 * | 01               | Invoke DrmUtils::extractKeyIdFromPssh with psshData, dataLength = 30, DRMSystems::eDRM_WIDEVINE, and nullptr for len. | psshData = "valid_pssh_box_with_key_id", dataLength = 30, len = nullptr, drmSystem = eDRM_WIDEVINE, output = nullptr | No exception thrown and returned pointer is nullptr                        | Should Pass  |
 */
TEST(DrmUtilsTest, NullLenPointer) {
    std::cout << "Entering NullLenPointer test" << std::endl;
    
    char psshData[50];
    strncpy(psshData, "valid_pssh_box_with_key_id", sizeof(psshData) - 1);
    psshData[sizeof(psshData) - 1] = '\0';
    int dataLength = 30;
    
    std::cout << "Invoking DrmUtils::extractKeyIdFromPssh with psshData: " << psshData 
              << ", dataLength: " << dataLength << ",DRMSystems::eDRM_WIDEVINE and nullptr for len" << std::endl;
    
    unsigned char* keyIdBuffer = nullptr;
    EXPECT_NO_THROW(
        keyIdBuffer = DrmUtils::extractKeyIdFromPssh(psshData, dataLength, nullptr, DRMSystems::eDRM_WIDEVINE);
    );
    
    std::cout << "Returned pointer: " << static_cast<const void*>(keyIdBuffer) << std::endl;
    EXPECT_EQ(keyIdBuffer, nullptr);
    
    std::cout << "Exiting NullLenPointer test" << std::endl;
}
/**
 * @brief Validate correct handling of corrupted psshData input in key extraction.
 *
 * This test verifies that the DrmUtils::extractKeyIdFromPssh function gracefully handles a corrupted psshData string by not throwing an exception and returning a nullptr for the keyIdBuffer.
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
 * | 01 | Print entry log for CorruptedPsshData test | N/A | "Entering CorruptedPsshData test" logged to stdout | Should be successful |
 * | 02 | Set up corrupted psshData string and initialize parameters | psshData = "corrupted_data_without_key", dataLength = 25, keyIdLen = 0, DRMSystems::eDRM_PLAYREADY | psshData correctly set and null-terminated, parameters initialized | Should be successful |
 * | 03 | Invoke DrmUtils::extractKeyIdFromPssh with the corrupted psshData | input: psshData = "corrupted_data_without_key", dataLength = 25, pointer keyIdLen, DRMSystems::eDRM_PLAYREADY | Function does not throw exception; keyIdBuffer is assigned | Should Pass |
 * | 04 | Verify that keyIdBuffer is nullptr | keyIdBuffer = expected nullptr | Assertion: keyIdBuffer == nullptr | Should Pass |
 * | 05 | Print exit log for CorruptedPsshData test | N/A | "Exiting CorruptedPsshData test" logged to stdout | Should be successful |
 */
TEST(DrmUtilsTest, CorruptedPsshData) {
    std::cout << "Entering CorruptedPsshData test" << std::endl;
    
    char psshData[50];
    strncpy(psshData, "corrupted_data_without_key", sizeof(psshData) - 1);
    psshData[sizeof(psshData) - 1] = '\0';
    int dataLength = 25;
    int keyIdLen = 0;
    
    std::cout << "Invoking DrmUtils::extractKeyIdFromPssh with corrupted psshData: " << psshData 
              << ", dataLength: " << dataLength << ", DRMSystems::eDRM_PLAYREADY" << std::endl;
    
    unsigned char* keyIdBuffer = nullptr;
    EXPECT_NO_THROW(
        keyIdBuffer = DrmUtils::extractKeyIdFromPssh(psshData, dataLength, &keyIdLen, DRMSystems::eDRM_PLAYREADY);
    );
    
    std::cout << "Returned pointer: " << static_cast<const void*>(keyIdBuffer) << std::endl;
    EXPECT_EQ(keyIdBuffer, nullptr);
    
    std::cout << "Exiting CorruptedPsshData test" << std::endl;
}
/**
 * @brief Validate behavior when an unsupported DRM system is used.
 *
 * This test verifies that the DrmUtils::extractKeyIdFromPssh API correctly handles an unsupported DRM system by not throwing an exception and by returning a null pointer. It ensures that the API behaves as expected when provided with an invalid DRM system input.
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
 * | Variation / Step | Description                                                                                  | Test Data                                                                                                          | Expected Result                                                                                                                     | Notes         |
 * | :--------------: | -------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------ | ----------------------------------------------------------------------------------------------------------------------------------- | ------------- |
 * | 01               | Prepare test data with valid psshData and an unsupported DRM system value                       | psshData = "valid_pssh_box_with_key_id", dataLength = 30, drmSystem = static_cast<DRMSystems>(-1)                   | API call should not throw an exception                                                                                              | Should Pass   |
 * | 02               | Call DrmUtils::extractKeyIdFromPssh and verify that the returned keyIdBuffer is null             | keyIdBuffer initially set to nullptr                                                                               | Function returns nullptr as keyIdBuffer; EXPECT_EQ(keyIdBuffer, nullptr) assertion must pass                                          | Should Pass   |
 */
TEST(DrmUtilsTest, UnsupportedDrmSystem) {
    std::cout << "Entering UnsupportedDrmSystem test" << std::endl;
    
    char psshData[50];
    strncpy(psshData, "valid_pssh_box_with_key_id", sizeof(psshData) - 1);
    psshData[sizeof(psshData) - 1] = '\0';
    int dataLength = 30;
    int keyIdLen = 0;
    
    // Using an invalid/unsupported DRM system value.
    // Assuming that valid DRM systems are defined and an invalid one can be simulated by -1.
    std::cout << "Invoking DrmUtils::extractKeyIdFromPssh with psshData: " << psshData
              << ", dataLength: " << dataLength << ", drmSystem: static_cast<DRMSystems>(-1)" << std::endl;
    
    unsigned char* keyIdBuffer = nullptr;
    EXPECT_NO_THROW(
        keyIdBuffer = DrmUtils::extractKeyIdFromPssh(psshData, dataLength, &keyIdLen, static_cast<name_space::DRMSystems>(-1));
    );
    
    std::cout << "Returned pointer: " << static_cast<const void*>(keyIdBuffer) << std::endl;
    EXPECT_EQ(keyIdBuffer, nullptr);
    
    std::cout << "Exiting UnsupportedDrmSystem test" << std::endl;
}
/**
 * @brief Validates extraction of Widevine Content Metadata from valid PSSH data.
 *
 * This test verifies that the DrmUtils::extractWVContentMetadataFromPssh function correctly extracts metadata when provided with a valid PSSH buffer and its corresponding length. It ensures that the function executes without throwing exceptions and that a non-empty metadata string is returned.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 022
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Initialize test data and prepare psshBuffer with valid PSSH string | inputStr = "valid_pssh_data_example", dataLength = 24, psshBuffer = zero-initialized char array of size 100 | psshBuffer is correctly populated with the valid PSSH data using strncpy | Should be successful |
 * | 02 | Invoke DrmUtils::extractWVContentMetadataFromPssh with the prepared psshBuffer and dataLength | psshData = psshBuffer ("valid_pssh_data_example"), dataLength = 24 | Function executes without throwing exceptions and returns a valid metadata string | Should Pass |
 * | 03 | Validate that the returned metadata is not empty | result = metadata string returned by the API | Metadata string is non-empty, confirming successful extraction | Should Pass |
 */
TEST(ExtractWVContentMetadataFromPsshTest, ValidPsshData) {
    std::cout << "Entering ValidPsshData test" << std::endl;
    
    const char* inputStr = "valid_pssh_data_example";
    int dataLength = 24;
    char psshBuffer[100] = {0};
    std::cout << "Preparing psshBuffer with input string: " << inputStr << " and dataLength: " << dataLength << std::endl;
    
    strncpy(psshBuffer, inputStr, sizeof(psshBuffer)-1);
    std::cout << "psshBuffer after strncpy: " << psshBuffer << std::endl;
    
    std::cout << "Invoking DrmUtils::extractWVContentMetadataFromPssh with psshData: " << psshBuffer 
              << " and dataLength: " << dataLength << std::endl;
    std::string result;
    EXPECT_NO_THROW(result = DrmUtils::extractWVContentMetadataFromPssh(psshBuffer, dataLength));
    
    std::cout << "Returned metadata: " << result << std::endl;
    EXPECT_FALSE(result.empty());
    
    std::cout << "Exiting ValidPsshData test" << std::endl;
}
/**
 * @brief Validates behavior of extractWVContentMetadataFromPssh with null psshData and positive dataLength
 *
 * This test case verifies that when a null psshData pointer is provided along with a positive dataLength to the extractWVContentMetadataFromPssh API,
 * the function handles the null input gracefully by not throwing any exceptions and by returning an empty metadata string.
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
 * | Variation / Step | Description                                                            | Test Data                                               | Expected Result                                                              | Notes         |
 * | :--------------: | ---------------------------------------------------------------------- | ------------------------------------------------------- | ---------------------------------------------------------------------------- | ------------- |
 * | 01               | Initialize test parameters and print test entry messages               | psshData = nullptr, dataLength = 10                     | Parameters set and entry log printed                                         | Should be successful |
 * | 02               | Invoke DrmUtils::extractWVContentMetadataFromPssh API                  | psshData = nullptr, dataLength = 10, output = result      | API completes without throwing exception; result is assigned an empty string | Should Pass   |
 * | 03               | Validate that the returned metadata is empty and print exit messages     | output = result (empty string)                          | Assertion check confirms that result is empty                                | Should Pass   |
 */
TEST(ExtractWVContentMetadataFromPsshTest, NullPsshDataPositiveLength) {
    std::cout << "Entering NullPsshDataPositiveLength test" << std::endl;
    
    const char* psshData = nullptr;
    int dataLength = 10;
    std::cout << "Invoking DrmUtils::extractWVContentMetadataFromPssh with psshData: nullptr"
              << " and dataLength: " << dataLength << std::endl;
    
    std::string result;
    EXPECT_NO_THROW(result = DrmUtils::extractWVContentMetadataFromPssh(psshData, dataLength));
    
    std::cout << "Returned metadata: " << result << std::endl;
    EXPECT_TRUE(result.empty());
    
    std::cout << "Exiting NullPsshDataPositiveLength test" << std::endl;
}
/**
 * @brief Verify that extractWVContentMetadataFromPssh handles a valid pointer with a negative data length correctly
 *
 * This test verifies that when a valid psshBuffer is provided along with a negative data length, the function 
 * extractWVContentMetadataFromPssh does not throw an exception and returns an empty string. This behavior ensures 
 * that the API gracefully handles cases with negative length inputs.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Initialize input string "valid_pssh_data_example" and negative data length; copy the input into psshBuffer using strncpy | inputStr = "valid_pssh_data_example", dataLength = -5, psshBuffer = char[100] initialized to zero | psshBuffer contains "valid_pssh_data_example" after strncpy; setup is successful | Should be successful |
 * | 02 | Invoke DrmUtils::extractWVContentMetadataFromPssh using the prepared psshBuffer and negative dataLength | psshData = "valid_pssh_data_example", dataLength = -5 | API does not throw exception and returns an empty string; EXPECT_TRUE(result.empty()) passes | Should Pass |
 */
TEST(ExtractWVContentMetadataFromPsshTest, ValidPointerNegativeLength) {
    std::cout << "Entering ValidPointerNegativeLength test" << std::endl;
    
    const char* inputStr = "valid_pssh_data_example";
    int dataLength = -5;
    char psshBuffer[100] = {0};
    std::cout << "Preparing psshBuffer with input string: " << inputStr << " and negative dataLength: " << dataLength << std::endl;
    
    strncpy(psshBuffer, inputStr, sizeof(psshBuffer)-1);
    std::cout << "psshBuffer after strncpy: " << psshBuffer << std::endl;
    
    std::cout << "Invoking DrmUtils::extractWVContentMetadataFromPssh with psshData: " << psshBuffer
              << " and dataLength: " << dataLength << std::endl;
    std::string result;
    EXPECT_NO_THROW(result = DrmUtils::extractWVContentMetadataFromPssh(psshBuffer, dataLength));
    
    std::cout << "Returned metadata: " << result << std::endl;
    EXPECT_TRUE(result.empty());
    
    std::cout << "Exiting ValidPointerNegativeLength test" << std::endl;
}
/**
 * @brief Verifies that extractWVContentMetadataFromPssh correctly handles an empty pssh data input.
 *
 * This test checks whether the function DrmUtils::extractWVContentMetadataFromPssh can safely process an empty pssh string with zero data length without throwing an exception and returns an empty metadata string. It is important to ensure that the function behaves as expected with edge-case inputs.
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
 * | Variation / Step | Description                                                                 | Test Data                                                                                      | Expected Result                                                 | Notes         |
 * | :--------------: | --------------------------------------------------------------------------- | ---------------------------------------------------------------------------------------------- | --------------------------------------------------------------- | ------------- |
 * | 01               | Prepare psshBuffer with empty input string and dataLength set to zero         | inputStr = "", dataLength = 0, psshBuffer initialized with zeros                                 | psshBuffer remains an empty string                              | Should be successful |
 * | 02               | Invoke DrmUtils::extractWVContentMetadataFromPssh using empty psshBuffer      | psshBuffer = "", dataLength = 0                                                                 | API does not throw an exception, metadata is returned           | Should Pass   |
 * | 03               | Verify that the returned metadata string is empty                           | result = metadata string returned from API                                                      | RESULT is an empty string as verified by EXPECT_TRUE(result.empty()) | Should Pass   |
 */
TEST(ExtractWVContentMetadataFromPsshTest, EmptyPsshDataZeroLength) {
    std::cout << "Entering EmptyPsshDataZeroLength test" << std::endl;
    
    const char* inputStr = "";
    int dataLength = 0;
    char psshBuffer[100] = {0};
    std::cout << "Preparing psshBuffer with empty input string and dataLength: " << dataLength << std::endl;
    
    strncpy(psshBuffer, inputStr, sizeof(psshBuffer)-1);
    std::cout << "psshBuffer after strncpy (should be empty): '" << psshBuffer << "'" << std::endl;
    
    std::cout << "Invoking DrmUtils::extractWVContentMetadataFromPssh with empty psshData and dataLength: " << dataLength << std::endl;
    std::string result;
    EXPECT_NO_THROW(result = DrmUtils::extractWVContentMetadataFromPssh(psshBuffer, dataLength));
    
    std::cout << "Returned metadata: '" << result << "'" << std::endl;
    EXPECT_TRUE(result.empty());
    
    std::cout << "Exiting EmptyPsshDataZeroLength test" << std::endl;
}
/**
 * @brief Test to validate that extractWVContentMetadataFromPssh handles garbled/invalid pssh data.
 *
 * This test verifies that when invalid garbled pssh data is passed to extractWVContentMetadataFromPssh, 
 * the function does not throw an exception and returns an empty metadata string. This ensures that the API 
 * is robust against malformed input and behaves predictably.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Prepare garbled pssh buffer with invalid data and a specified data length | inputStr = "garbled_data_not_in_valid_format", dataLength = 36, psshBuffer initialized to 0 | psshBuffer should contain the truncated garbled string | Should be successful |
 * | 02 | Invoke DrmUtils::extractWVContentMetadataFromPssh using the prepared psshBuffer and data length | psshData = psshBuffer, dataLength = 36 | API should not throw an exception and should return an empty string | Should Pass |
 * | 03 | Validate that the returned metadata is empty | output = result from API call | Metadata should be an empty string | Should Pass |
 */
TEST(ExtractWVContentMetadataFromPsshTest, InvalidGarbledPsshData) {
    std::cout << "Entering InvalidGarbledPsshData test" << std::endl;
    
    const char* inputStr = "garbled_data_not_in_valid_format";
    int dataLength = 36;
    char psshBuffer[100] = {0};
    std::cout << "Preparing psshBuffer with garbled input string: " << inputStr << " and dataLength: " << dataLength << std::endl;
    
    strncpy(psshBuffer, inputStr, sizeof(psshBuffer)-1);
    std::cout << "psshBuffer after strncpy: " << psshBuffer << std::endl;
    
    std::cout << "Invoking DrmUtils::extractWVContentMetadataFromPssh with psshData: " << psshBuffer
              << " and dataLength: " << dataLength << std::endl;
    std::string result;
    EXPECT_NO_THROW(result = DrmUtils::extractWVContentMetadataFromPssh(psshBuffer, dataLength));
    
    std::cout << "Returned metadata: " << result << std::endl;
    EXPECT_TRUE(result.empty());
    
    std::cout << "Exiting InvalidGarbledPsshData test" << std::endl;
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
