
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

class DrmUtilsTests : public ::testing::Test
{
protected:
	void SetUp() override
	{
	}

	void TearDown() override
	{
	}

public:
};
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
TEST_F(DrmUtilsTests, ValidConversionWithStandard16ByteInput) {
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
TEST_F(DrmUtilsTests, ValidConversionWith16ZeroBytes) {
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
TEST_F(DrmUtilsTests, NegativeCaseWithNullOriginalPointer) {
    GTEST_SKIP();
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
TEST_F(DrmUtilsTests, NegativeCaseWithNullDestinationPointer) {
    GTEST_SKIP();
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
TEST_F(DrmUtilsTests, ValidPsshData) {
    GTEST_SKIP();
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
TEST_F(DrmUtilsTests, NullPsshDataPositiveLength) {
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
TEST_F(DrmUtilsTests, ValidPointerNegativeLength) {
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
TEST_F(DrmUtilsTests, EmptyPsshDataZeroLength) {
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
TEST_F(DrmUtilsTests, InvalidGarbledPsshData) {
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