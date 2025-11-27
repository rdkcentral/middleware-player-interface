
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
#include "TtmlPacket.hpp"


// Test Case: SendDataPacket with valid non-empty TTML data using default time offset
/**
 * @brief Validate that SendDataPacket accepts non-empty TTML data with a default time offset
 *
 * This test verifies that the SendDataPacket API in the TtmlChannel class accepts a non-empty TTML data packet and a default time offset of 0 without throwing any exceptions. The objective is to ensure that the function properly handles typical valid input parameters.
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
 * | Variation / Step | Description                                                                                   | Test Data                                                       | Expected Result                                                | Notes        |
 * | :--------------: | --------------------------------------------------------------------------------------------- | --------------------------------------------------------------- | -------------------------------------------------------------- | ------------ |
 * | 01               | Initialize TtmlChannel and test data, then invoke the SendDataPacket API with default time offset | data = {0x01, 0x02, 0x03}, time_offset_ms = 0                     | No exception is thrown and API call completes successfully     | Should Pass  |
 */
TEST(TtmlChannel, SendDataPacket_DefaultTimeOffset) {
    std::cout << "Entering SendDataPacket_DefaultTimeOffset test" << std::endl;

    // Create object of TtmlChannel using default constructor
    TtmlChannel channel;
    std::vector<uint8_t> data = {0x01, 0x02, 0x03};
    int64_t time_offset_ms = 0;
    
    std::cout << "Invoking SendDataPacket with data: ";
    for (const auto &byte : data) {
        std::cout << "0x" << std::hex << static_cast<int>(byte) << " ";
    }
    std::cout << std::dec << "and time_offset_ms: " << time_offset_ms << std::endl;
    
    EXPECT_NO_THROW({
        channel.SendDataPacket(std::move(data), time_offset_ms);
    });
    
    std::cout << "SendDataPacket invoked successfully with default time offset." << std::endl;
    std::cout << "Exiting SendDataPacket_DefaultTimeOffset test" << std::endl;
}
/**
 * @brief Validate that SendDataPacket API handles valid TTML data with a positive time offset.
 *
 * This test verifies that the SendDataPacket function can successfully process a valid, non-empty TTML data packet with a positive time offset value. The test ensures that no exceptions are thrown during the operation, indicating correct handling of the input parameters.
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
 * | Variation / Step | Description                                                                         | Test Data                                                                          | Expected Result                                                                               | Notes       |
 * | :--------------: | ----------------------------------------------------------------------------------- | ---------------------------------------------------------------------------------- | --------------------------------------------------------------------------------------------- | ----------- |
 * | 01               | Initialize the TtmlChannel instance, prepare TTML data vector and set positive time offset | TtmlChannel instance, data = {0x10, 0x20, 0x30, 0x40}, time_offset_ms = 500          | API invocation executes without throwing an exception; EXPECT_NO_THROW assertion passes       | Should Pass |
 */
TEST(TtmlChannel, SendDataPacket_PositiveTimeOffset) {
    std::cout << "Entering SendDataPacket_PositiveTimeOffset test" << std::endl;

    TtmlChannel channel;
    std::vector<uint8_t> data = {0x10, 0x20, 0x30, 0x40};
    int64_t time_offset_ms = 500;
    
    std::cout << "Invoking SendDataPacket with data: ";
    for (const auto &byte : data) {
        std::cout << "0x" << std::hex << static_cast<int>(byte) << " ";
    }
    std::cout << std::dec << "and time_offset_ms: " << time_offset_ms << std::endl;
    
    EXPECT_NO_THROW({
        channel.SendDataPacket(std::move(data), time_offset_ms);
    });
    
    std::cout << "SendDataPacket invoked successfully with positive time offset." << std::endl;
    std::cout << "Exiting SendDataPacket_PositiveTimeOffset test" << std::endl;
}
/**
 * @brief Test SendDataPacket with negative time offset to ensure robust handling of negative time values
 *
 * This test case verifies that the TtmlChannel::SendDataPacket API can process a negative time offset without throwing any exceptions.
 * The API is invoked with a pre-defined data vector and a negative time offset, and the test asserts that the function completes successfully.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 003
 * **Priority:** High
 * 
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 * 
 * **Test Procedure:**
 * | Variation / Step | Description                                                      | Test Data                                               | Expected Result                                              | Notes       |
 * | :--------------: | ---------------------------------------------------------------- | ------------------------------------------------------- | ------------------------------------------------------------ | ----------- |
 * | 01               | Invoke SendDataPacket with a valid data vector and negative time offset | data = {0xFF, 0xEE, 0xDD}, time_offset_ms = -250        | API call should not throw any exceptions                     | Should Pass |
 */
TEST(TtmlChannel, SendDataPacket_NegativeTimeOffset) {
    std::cout << "Entering SendDataPacket_NegativeTimeOffset test" << std::endl;

    TtmlChannel channel;
    std::vector<uint8_t> data = {0xFF, 0xEE, 0xDD};
    int64_t time_offset_ms = -250;
    
    std::cout << "Invoking SendDataPacket with data: ";
    for (const auto &byte : data) {
        std::cout << "0x" << std::hex << static_cast<int>(byte) << " ";
    }
    std::cout << std::dec << "and time_offset_ms: " << time_offset_ms << std::endl;
    
    EXPECT_NO_THROW({
        channel.SendDataPacket(std::move(data), time_offset_ms);
    });
    
    std::cout << "SendDataPacket invoked successfully with negative time offset." << std::endl;
    std::cout << "Exiting SendDataPacket_NegativeTimeOffset test" << std::endl;
}
/**
 * @brief Test that SendDataPacket API handles an empty data vector and default time offset correctly.
 *
 * This test verifies that the TtmlChannel::SendDataPacket() method can accept an empty data vector and a default time offset (0 ms) without throwing any exceptions. Verifying this behavior is critical to ensure that the API gracefully handles edge cases where no data is provided.
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
 * | Variation / Step | Description                                                                 | Test Data                                          | Expected Result                                               | Notes             |
 * | :--------------: | --------------------------------------------------------------------------- | -------------------------------------------------- | ------------------------------------------------------------- | ----------------- |
 * | 01               | Display entry log message for the SendDataPacket test                       | None                                               | Entry message printed to standard output                      | Should be successful |
 * | 02               | Instantiate TtmlChannel object and initialize an empty data vector          | TtmlChannel channel, data = {}                     | Channel object created and data vector is empty               | Should be successful |
 * | 03               | Log details before invoking SendDataPacket, including time_offset_ms         | time_offset_ms = 0                                 | Pre-invocation log message printed                            | Should be successful |
 * | 04               | Call SendDataPacket API with empty data vector and default time offset         | data = {} (empty), time_offset_ms = 0              | No exception thrown; API processes call successfully          | Should Pass       |
 * | 05               | Print log confirming API call was successful                                | None                                               | Post-invocation success message printed                       | Should be successful |
 * | 06               | Display exit log message for the test case                                  | None                                               | Exit message printed to standard output                       | Should be successful |
 */
TEST(TtmlChannel, SendDataPacket_EmptyData_DefaultTimeOffset) {
    std::cout << "Entering SendDataPacket_EmptyData_DefaultTimeOffset test" << std::endl;

    TtmlChannel channel;
    std::vector<uint8_t> data = {}; // empty vector
    int64_t time_offset_ms = 0;
    
    std::cout << "Invoking SendDataPacket with empty data vector and time_offset_ms: " << time_offset_ms << std::endl;
    
    EXPECT_NO_THROW({
        channel.SendDataPacket(std::move(data), time_offset_ms);
    });
    
    std::cout << "SendDataPacket invoked successfully with empty data vector." << std::endl;
    std::cout << "Exiting SendDataPacket_EmptyData_DefaultTimeOffset test" << std::endl;
}
/**
 * @brief Validate SendDataPacket handles extremely large positive time offset correctly
 *
 * This test verifies that the TtmlChannel::SendDataPacket API correctly processes valid data and an extremely large positive time offset without throwing any exceptions.
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
 * | Variation / Step | Description                                                                                     | Test Data                                          | Expected Result                                             | Notes      |
 * | :--------------: | ----------------------------------------------------------------------------------------------- | -------------------------------------------------- | ----------------------------------------------------------- | ---------- |
 * | 01               | Instantiate TtmlChannel, prepare data vector and maximum positive time offset, and invoke SendDataPacket. | data = {0xAA,0xBB,0xCC}, time_offset_ms = 9223372036854775807 | API call completes without throwing exceptions           | Should Pass |
 */
TEST(TtmlChannel, SendDataPacket_ExtremelyLargePositiveTimeOffset) {
    std::cout << "Entering SendDataPacket_ExtremelyLargePositiveTimeOffset test" << std::endl;

    TtmlChannel channel;
    std::vector<uint8_t> data = {0xAA, 0xBB, 0xCC};
    int64_t time_offset_ms = std::numeric_limits<int64_t>::max();
    
    std::cout << "Invoking SendDataPacket with data: ";
    for (const auto &byte : data) {
        std::cout << "0x" << std::hex << static_cast<int>(byte) << " ";
    }
    std::cout << std::dec << "and extremely large positive time_offset_ms: " << time_offset_ms << std::endl;
    
    EXPECT_NO_THROW({
        channel.SendDataPacket(std::move(data), time_offset_ms);
    });
    
    std::cout << "SendDataPacket invoked successfully with extremely large positive time offset." << std::endl;
    std::cout << "Exiting SendDataPacket_ExtremelyLargePositiveTimeOffset test" << std::endl;
}
/**
 * @brief Validate that TtmlChannel::SendDataPacket handles an extremely large negative time offset without throwing exceptions.
 *
 * This test verifies that when SendDataPacket is invoked with a valid data packet and an extremely large negative time offset (minimum int64_t value), the API executes without throwing any exceptions. The objective is to ensure that even with negative time offsets at limits, the channel correctly processes the input.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoke SendDataPacket API with valid data and an extremely large negative time offset. | data = {0x11, 0x22, 0x33}, time_offset_ms = std::numeric_limits<int64_t>::min() | API executes without throwing exceptions; Assertion EXPECT_NO_THROW passes. | Should Pass |
 */
TEST(TtmlChannel, SendDataPacket_ExtremelyLargeNegativeTimeOffset) {
    std::cout << "Entering SendDataPacket_ExtremelyLargeNegativeTimeOffset test" << std::endl;

    TtmlChannel channel;
    std::vector<uint8_t> data = {0x11, 0x22, 0x33};
    int64_t time_offset_ms = std::numeric_limits<int64_t>::min();
    
    std::cout << "Invoking SendDataPacket with data: ";
    for (const auto &byte : data) {
        std::cout << "0x" << std::hex << static_cast<int>(byte) << " ";
    }
    std::cout << std::dec << "and extremely large negative time_offset_ms: " << time_offset_ms << std::endl;
    
    EXPECT_NO_THROW({
        channel.SendDataPacket(std::move(data), time_offset_ms);
    });
    
    std::cout << "SendDataPacket invoked successfully with extremely large negative time offset." << std::endl;
    std::cout << "Exiting SendDataPacket_ExtremelyLargeNegativeTimeOffset test" << std::endl;
}
/**
 * @brief Verify that SendSelectionPacket API correctly processes normal dimensions.
 *
 * This test verifies that the SendSelectionPacket method of the TtmlChannel class is invoked
 * successfully with typical dimensions (1920x1080) without throwing any exceptions.
 * It ensures that the API handles standard input values and behaves as expected in a positive scenario.
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
 * | Variation / Step | Description                                                     | Test Data                          | Expected Result                                                         | Notes             |
 * | :--------------: | ---------------------------------------------------------------- | ---------------------------------- | ----------------------------------------------------------------------- | ----------------- |
 * |       01       | Create a TtmlChannel object using the default constructor         | None                               | Object is created without throwing an exception                         | Should be successful |
 * |       02       | Invoke SendSelectionPacket with typical dimensions (1920, 1080)     | width = 1920, height = 1080          | Method invocation completes without throwing an exception               | Should Pass       |
 */
TEST(TtmlChannel, SendSelectionPacket_NormalDimensions) {
    std::cout << "Entering SendSelectionPacket_NormalDimensions test" << std::endl;
    uint32_t width = 1920;
    uint32_t height = 1080;
    std::cout << "Creating TtmlChannel object using default constructor" << std::endl;
    EXPECT_NO_THROW({
        TtmlChannel ttml;
        std::cout << "Invoking SendSelectionPacket with width: " << width << " and height: " << height << std::endl;
        EXPECT_NO_THROW(ttml.SendSelectionPacket(width, height));
        std::cout << "SendSelectionPacket invoked successfully with width: " << width << " and height: " << height << std::endl;
    });
    std::cout << "Exiting SendSelectionPacket_NormalDimensions test" << std::endl;
}
/**
 * @brief Verify that SendSelectionPacket handles zero dimensions correctly.
 *
 * This test verifies that calling the SendSelectionPacket API with both width and height set to zero does not throw any exceptions.
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
 * | Variation / Step | Description                                                         | Test Data                  | Expected Result                         | Notes      |
 * | :--------------: | ------------------------------------------------------------------- | -------------------------- | --------------------------------------- | ---------- |
 * | 01               | Create TtmlChannel object and invoke SendSelectionPacket with zero dimensions | width = 0, height = 0      | API should execute without throwing exceptions, and assertions pass | Should Pass |
 */
TEST(TtmlChannel, SendSelectionPacket_ZeroDimensions) {
    std::cout << "Entering SendSelectionPacket_ZeroDimensions test" << std::endl;
    uint32_t width = 0;
    uint32_t height = 0;
    std::cout << "Creating TtmlChannel object using default constructor" << std::endl;
    EXPECT_NO_THROW({
        TtmlChannel ttml;
        std::cout << "Invoking SendSelectionPacket with width: " << width << " and height: " << height << std::endl;
        EXPECT_NO_THROW(ttml.SendSelectionPacket(width, height));
        std::cout << "SendSelectionPacket invoked successfully with width: " << width << " and height: " << height << std::endl;
    });
    std::cout << "Exiting SendSelectionPacket_ZeroDimensions test" << std::endl;
}
/**
 * @brief Test the SendSelectionPacket API with maximum allowed dimensions
 *
 * This test verifies that invoking SendSelectionPacket with maximum width and height does not throw any exceptions, ensuring that the API can handle extreme input values.
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
 * | Variation / Step | Description                                                      | Test Data                                                                                | Expected Result                                                     | Notes             |
 * | :---------------:| ---------------------------------------------------------------- | ---------------------------------------------------------------------------------------- | -------------------------------------------------------------------- | ----------------- |
 * | 01               | Create TtmlChannel object using default constructor              | None                                                                                     | Object is created without throwing exceptions                      | Should be successful |
 * | 02               | Invoke SendSelectionPacket with maximum width and height           | input1 = width = std::numeric_limits<uint32_t>::max(), input2 = height = std::numeric_limits<uint32_t>::max() | No exception is thrown during method invocation                     | Should Pass       |
 */
TEST(TtmlChannel, SendSelectionPacket_MaximumDimensions) {
    std::cout << "Entering SendSelectionPacket_MaximumDimensions test" << std::endl;
    uint32_t width = std::numeric_limits<uint32_t>::max();
    uint32_t height = std::numeric_limits<uint32_t>::max();
    std::cout << "Creating TtmlChannel object using default constructor" << std::endl;
    EXPECT_NO_THROW({
        TtmlChannel ttml;
        std::cout << "Invoking SendSelectionPacket with width: " << width << " and height: " << height << std::endl;
        EXPECT_NO_THROW(ttml.SendSelectionPacket(width, height));
        std::cout << "SendSelectionPacket invoked successfully with width: " << width << " and height: " << height << std::endl;
    });
    std::cout << "Exiting SendSelectionPacket_MaximumDimensions test" << std::endl;
}
/**
 * @brief Validate SendSelectionPacket API behavior when width is zero and height is non-zero.
 *
 * Tests that the SendSelectionPacket function in TtmlChannel correctly handles the edge case where the width is zero while the height is non-zero, ensuring that the API can process these input values without throwing an exception.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 010
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create a TtmlChannel object and invoke SendSelectionPacket with width set to 0 and height set to 1080 | input: width = 0, height = 1080 | Function call does not throw any exception; SendSelectionPacket executes successfully | Should Pass |
 */
TEST(TtmlChannel, SendSelectionPacket_WidthZero_NonZeroHeight) {
    std::cout << "Entering SendSelectionPacket_WidthZero_NonZeroHeight test" << std::endl;
    uint32_t width = 0;
    uint32_t height = 1080;
    std::cout << "Creating TtmlChannel object using default constructor" << std::endl;
    EXPECT_NO_THROW({
        TtmlChannel ttml;
        std::cout << "Invoking SendSelectionPacket with width: " << width << " and height: " << height << std::endl;
        EXPECT_NO_THROW(ttml.SendSelectionPacket(width, height));
        std::cout << "SendSelectionPacket invoked successfully with width: " << width << " and height: " << height << std::endl;
    });
    std::cout << "Exiting SendSelectionPacket_WidthZero_NonZeroHeight test" << std::endl;
}
/**
 * @brief Validate SendSelectionPacket API with valid width and zero height
 *
 * This test ensures that when SendSelectionPacket is invoked using a non-zero width (1920) and a zero height, the API does not throw any exceptions. The test verifies that the API can handle an extreme ratio scenario where the height is zero without crashing or exhibiting unexpected behavior.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 011
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                             | Test Data                          | Expected Result                                         | Notes          |
 * | :--------------: | ------------------------------------------------------- | ---------------------------------- | ------------------------------------------------------- | -------------- |
 * | 01               | Create TtmlChannel instance using default constructor   | None                               | TtmlChannel object is created successfully              | Should be successful |
 * | 02               | Invoke SendSelectionPacket with width=1920 and height=0   | width = 1920, height = 0             | API executes without throwing exceptions; assertion passes | Should Pass    |
 */
TEST(TtmlChannel, SendSelectionPacket_NonZeroWidth_HeightZero) {
    std::cout << "Entering SendSelectionPacket_NonZeroWidth_HeightZero test" << std::endl;
    uint32_t width = 1920;
    uint32_t height = 0;
    std::cout << "Creating TtmlChannel object using default constructor" << std::endl;
    EXPECT_NO_THROW({
        TtmlChannel ttml;
        std::cout << "Invoking SendSelectionPacket with width: " << width << " and height: " << height << std::endl;
        EXPECT_NO_THROW(ttml.SendSelectionPacket(width, height));
        std::cout << "SendSelectionPacket invoked successfully with width: " << width << " and height: " << height << std::endl;
    });
    std::cout << "Exiting SendSelectionPacket_NonZeroWidth_HeightZero test" << std::endl;
}
/**
 * @brief Validate SendSelectionPacket with extreme dimension ratios
 *
 * This test verifies whether the SendSelectionPacket function of the TtmlChannel class can handle extreme aspect ratios, specifically when the width is set to the maximum value for a 32-bit unsigned integer and the height is set to 1. The test ensures that no exceptions are thrown during object construction and the method call.
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
 * | :--------------: | ----------- | --------- | ------------- | ----- |
 * | 01 | Create a TtmlChannel object using the default constructor. | None | TtmlChannel object is created successfully without throwing an exception. | Should be successful |
 * | 02 | Invoke SendSelectionPacket with extreme ratio dimensions. | width = std::numeric_limits<uint32_t>::max(), height = 1 | Function executes without throwing an exception. | Should Pass |
 */
TEST(TtmlChannel, SendSelectionPacket_ExtremeRatioDimensions) {
    std::cout << "Entering SendSelectionPacket_ExtremeRatioDimensions test" << std::endl;
    uint32_t width = std::numeric_limits<uint32_t>::max();
    uint32_t height = 1;
    std::cout << "Creating TtmlChannel object using default constructor" << std::endl;
    EXPECT_NO_THROW({
        TtmlChannel ttml;
        std::cout << "Invoking SendSelectionPacket with width: " << width << " and height: " << height << std::endl;
        EXPECT_NO_THROW(ttml.SendSelectionPacket(width, height));
        std::cout << "SendSelectionPacket invoked successfully with width: " << width << " and height: " << height << std::endl;
    });
    std::cout << "Exiting SendSelectionPacket_ExtremeRatioDimensions test" << std::endl;
}
/**
 * @brief Verify that SendTimestampPacket correctly handles the zero timestamp case.
 *
 * This test validates that invoking the SendTimestampPacket method with a timestamp value of zero does not throw any exceptions. It checks that the TtmlChannel object is created successfully and that calling the API with a zero timestamp is handled properly.
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
 * | 01 | Create a TtmlChannel object using the default constructor. | None | TtmlChannel object is created without exception. | Should be successful |
 * | 02 | Set timestamp variable to zero and log the value. | timestamp = 0 | Timestamp variable is set to 0. | Should be successful |
 * | 03 | Invoke SendTimestampPacket with the timestamp value of zero. | input: timestamp = 0 | API call completes without throwing an exception. | Should Pass |
 */
TEST(TtmlChannel, SendTimestampPacket_zero) {
    std::cout << "Entering SendTimestampPacket_zero test" << std::endl;
    
    // Create TtmlChannel object using the default constructor and log the creation.
    EXPECT_NO_THROW({
        TtmlChannel channel;
        std::cout << "TtmlChannel object created using default constructor." << std::endl;
        
        uint64_t timestamp = 0;
        std::cout << "Invoking SendTimestampPacket with timestampMs = " << timestamp << std::endl;
        
        // Call SendTimestampPacket and check for exceptions.
        EXPECT_NO_THROW({
            channel.SendTimestampPacket(timestamp);
            std::cout << "sendPacket<TtmlTimestampPacket> invoked with timestampMs = " << timestamp << std::endl;
        });
    });
    
    std::cout << "Exiting SendTimestampPacket_zero test" << std::endl;
}
/**
 * @brief Validate that SendTimestampPacket executes without exceptions when provided with a valid timestamp.
 *
 * This test verifies that a TtmlChannel object can be successfully created using its default constructor and that invoking the SendTimestampPacket method with a valid timestamp (123456789) does not throw any exceptions. This ensures that the basic functionality of sending a timestamp packet is working as expected.
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
 * | Variation / Step | Description                                                         | Test Data                                  | Expected Result                              | Notes         |
 * | :--------------: | ------------------------------------------------------------------- | ------------------------------------------ | -------------------------------------------- | ------------- |
 * | 01               | Create a TtmlChannel object using the default constructor.          | (No input parameters)                      | Object is created successfully without error | Should be successful |
 * | 02               | Invoke SendTimestampPacket with a valid timestamp value (123456789).   | timestamp = 123456789                      | SendTimestampPacket executes without throwing an exception. | Should Pass   |
 */
TEST(TtmlChannel, SendTimestampPacket_positive) {
    std::cout << "Entering SendTimestampPacket_positive test" << std::endl;
    
    // Create TtmlChannel object using the default constructor and log the creation.
    EXPECT_NO_THROW({
        TtmlChannel channel;
        std::cout << "TtmlChannel object created using default constructor." << std::endl;
        
        uint64_t timestamp = 123456789;
        std::cout << "Invoking SendTimestampPacket with timestampMs = " << timestamp << std::endl;
        
        // Call SendTimestampPacket and check for exceptions.
        EXPECT_NO_THROW({
            channel.SendTimestampPacket(timestamp);
            std::cout << "sendPacket<TtmlTimestampPacket> invoked with timestampMs = " << timestamp << std::endl;
        });
    });
    
    std::cout << "Exiting SendTimestampPacket_positive test" << std::endl;
}
/**
 * @brief Verifies successful construction of a TtmlChannel object using its default constructor
 *
 * This test confirms that the TtmlChannel object can be instantiated without throwing any exceptions. 
 * It checks that the constructor is invoked properly and corresponding log messages are printed, ensuring basic functionality.
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
 * | Variation / Step | Description                                           | Test Data                                        | Expected Result                                                                           | Notes            |
 * | :--------------: | ------------------------------------------------------ | ------------------------------------------------ | ----------------------------------------------------------------------------------------- | ---------------- |
 * | 01               | Log the message indicating entry into the test       | None                                             | "Entering ConstructTtmlChannelSuccessfully test" printed                                  | Should be successful |
 * | 02               | Log the message indicating the invocation of the constructor | None                                       | "Invoking TtmlChannel() constructor" printed                                              | Should be successful |
 * | 03               | Invoke the TtmlChannel default constructor             | None                                             | No exception thrown; object constructed; "TtmlChannel object constructed successfully" printed | Should Pass      |
 * | 04               | Log the message indicating exit from the test          | None                                             | "Exiting ConstructTtmlChannelSuccessfully test" printed                                  | Should be successful |
 */
TEST(TtmlChannel, ConstructTtmlChannelSuccessfully) {
    std::cout << "Entering ConstructTtmlChannelSuccessfully test" << std::endl;
    
    // Log invocation of the TtmlChannel default constructor
    std::cout << "Invoking TtmlChannel() constructor" << std::endl;
    
    // Construct TtmlChannel object and expect no exceptions.
    EXPECT_NO_THROW({
        TtmlChannel ttmlChannel;
        std::cout << "TtmlChannel object constructed successfully" << std::endl;
    });
    
    std::cout << "Exiting ConstructTtmlChannelSuccessfully test" << std::endl;
}
/**
 * @brief Validates the construction of a TTML data packet with typical valid values.
 *
 * This test verifies that constructing a TTML data packet with valid input parameters such as a positive channelId,
 * a valid counter value, a proper data offset, and a non-empty data buffer completes successfully without throwing any exceptions.
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
 * | 01 | Prepare input values and invoke TtmlDataPacket constructor with typical valid values | channelId = 10, counter = 20, dataOffset = 1000, dataBuffer = {0x01, 0x02, 0x03} | Packet is successfully constructed without exceptions | Should Pass |
 */
TEST(TtmlDataPacket, TypicalValidValues) {
    std::cout << "Entering TypicalValidValues test" << std::endl;
    
    // Prepare input values
    std::uint32_t channelId = 10;
    std::uint32_t counter = 20;
    std::int64_t dataOffset = 1000;
    std::vector<std::uint8_t> dataBuffer = {0x01, 0x02, 0x03};
    std::cout << "Invoking TtmlDataPacket constructor with channelId=" << channelId
              << ", counter=" << counter << ", dataOffset=" << dataOffset
              << " and dataBuffer values {0x01, 0x02, 0x03}" << std::endl;

    // Create object and ensure no exception is thrown
    EXPECT_NO_THROW({
        TtmlDataPacket packet(channelId, counter, dataOffset, std::move(dataBuffer));
        std::cout << "packet construction, completed " << std::endl;
    });
    
    std::cout << "Exiting TypicalValidValues test" << std::endl;
}
/**
 * @brief Verify that TtmlDataPacket constructor handles a zero channel ID without throwing exceptions
 *
 * This test verifies that the TtmlDataPacket constructor executes correctly when provided with a zero channel ID.
 * It ensures that even with a channelId of 0 (which might be considered edge-case), the constructor is invoked without exceptions.
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
 * | Variation / Step | Description                                                                                  | Test Data                                                            | Expected Result                                                                             | Notes      |
 * | :--------------: | -------------------------------------------------------------------------------------------- | -------------------------------------------------------------------- | ------------------------------------------------------------------------------------------- | ---------- |
 * | 01               | Invoke TtmlDataPacket constructor with channelId = 0, counter = 20, dataOffset = 1000, and dataBuffer = {0x01, 0x02, 0x03} | channelId = 0, counter = 20, dataOffset = 1000, dataBuffer = {0x01,0x02,0x03} | TtmlDataPacket is successfully constructed without throwing any exceptions (EXPECT_NO_THROW assertion) | Should Pass |
 */
TEST(TtmlDataPacket, ZeroChannelId) {
    std::cout << "Entering ZeroChannelId test" << std::endl;
    
    std::uint32_t channelId = 0;
    std::uint32_t counter = 20;
    std::int64_t dataOffset = 1000;
    std::vector<std::uint8_t> dataBuffer = {0x01, 0x02, 0x03};
    std::cout << "Invoking TtmlDataPacket constructor with channelId=" << channelId
              << ", counter=" << counter << ", dataOffset=" << dataOffset
              << " and dataBuffer values {0x01, 0x02, 0x03}" << std::endl;
    
    EXPECT_NO_THROW({
        TtmlDataPacket packet(channelId, counter, dataOffset, std::move(dataBuffer));
        std::cout << " constructor invocation  completed  " <<std::endl;
    });
    
    std::cout << "Exiting ZeroChannelId test" << std::endl;
}
/**
 * @brief Validate TtmlDataPacket constructor with a zero counter value
 *
 * This test verifies that the TtmlDataPacket constructor correctly creates an instance when provided with a zero counter value.
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
 * | 01 | Invoke TtmlDataPacket constructor with channelId=10, counter=0, dataOffset=1000, and dataBuffer={0x01,0x02,0x03} | channelId=10, counter=0, dataOffset=1000, dataBuffer={0x01,0x02,0x03} | No exception thrown; TtmlDataPacket instance successfully constructed | Should Pass |
 */
TEST(TtmlDataPacket, ZeroCounter) {
    std::cout << "Entering ZeroCounter test" << std::endl;
    
    std::uint32_t channelId = 10;
    std::uint32_t counter = 0;
    std::int64_t dataOffset = 1000;
    std::vector<std::uint8_t> dataBuffer = {0x01, 0x02, 0x03};
    std::cout << "Invoking TtmlDataPacket constructor with channelId=" << channelId
              << ", counter=" << counter << ", dataOffset=" << dataOffset 
              << " and dataBuffer values {0x01, 0x02, 0x03}" << std::endl;
    
    EXPECT_NO_THROW({
        TtmlDataPacket packet(channelId, counter, dataOffset, std::move(dataBuffer));
        std::cout << " constructor invocation  completed  " << std::endl;
    });
    
    std::cout << "Exiting ZeroCounter test" << std::endl;
}
/**
 * @brief Verify TtmlDataPacket construction with zero data offset
 *
 * This test verifies that the TtmlDataPacket is correctly constructed when dataOffset is set to zero. It ensures that invoking the constructor with a valid channelId, counter, zero dataOffset, and a non-empty dataBuffer does not throw any exceptions.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Set test parameters including channelId, counter, dataOffset (0), and dataBuffer with {0x01, 0x02, 0x03} | channelId = 10, counter = 20, dataOffset = 0, dataBuffer = {0x01, 0x02, 0x03} | Parameters are correctly set | Should be successful |
 * | 02 | Invoke TtmlDataPacket constructor using the parameters and verify that no exceptions are thrown | Invocation of constructor with channelId, counter, dataOffset, dataBuffer | TtmlDataPacket object is constructed without exceptions | Should Pass |
 */
TEST(TtmlDataPacket, ZeroDataOffset) {
    std::cout << "Entering ZeroDataOffset test" << std::endl;
    
    std::uint32_t channelId = 10;
    std::uint32_t counter = 20;
    std::int64_t dataOffset = 0;
    std::vector<std::uint8_t> dataBuffer = {0x01, 0x02, 0x03};
    std::cout << "Invoking TtmlDataPacket constructor with channelId=" << channelId
              << ", counter=" << counter << ", dataOffset=" << dataOffset 
              << " and dataBuffer values {0x01, 0x02, 0x03}" << std::endl;
    
    EXPECT_NO_THROW({
        TtmlDataPacket packet(channelId, counter, dataOffset, std::move(dataBuffer));
        std::cout << " constructor invocation  completed  " <<std::endl;
    });
    
    std::cout << "Exiting ZeroDataOffset test" << std::endl;
}
/**
 * @brief Verify that constructing a TtmlDataPacket with an empty data buffer does not throw exceptions
 *
 * This test ensures that the TtmlDataPacket constructor properly handles an empty dataBuffer without raising any exceptions.
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
 * | Variation / Step | Description                                                                 | Test Data                                               | Expected Result                                                      | Notes      |
 * | :--------------: | --------------------------------------------------------------------------- | ------------------------------------------------------- | -------------------------------------------------------------------- | ---------- |
 * | 01               | Invoke TtmlDataPacket constructor with empty dataBuffer and given parameters| channelId = 10, counter = 20, dataOffset = 1000, dataBuffer = empty vector | The object is constructed without throwing any exceptions (EXPECT_NO_THROW passes) | Should Pass |
 */
TEST(TtmlDataPacket, EmptyDataBuffer) {
    std::cout << "Entering EmptyDataBuffer test" << std::endl;
    
    std::uint32_t channelId = 10;
    std::uint32_t counter = 20;
    std::int64_t dataOffset = 1000;
    std::vector<std::uint8_t> dataBuffer;  // empty buffer
    std::cout << "Invoking TtmlDataPacket constructor with channelId=" << channelId
              << ", counter=" << counter << ", dataOffset=" << dataOffset 
              << " and an empty dataBuffer" << std::endl;
    
    EXPECT_NO_THROW({
        TtmlDataPacket packet(channelId, counter, dataOffset, std::move(dataBuffer));
        std::cout << " constructor invocation  completed  " <<std::endl;
    });
    
    std::cout << "Exiting EmptyDataBuffer test" << std::endl;
}
/**
 * @brief Verify that TtmlDataPacket correctly handles a negative dataOffset value without throwing exceptions.
 *
 * This test case validates that providing a negative dataOffset during the construction of a TtmlDataPacket does not lead to any exceptions. The test logs entry and exit messages, sets predefined values, and checks that the constructor call completes normally with the given inputs.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Log the start of NegativeDataOffset test | None | "Entering NegativeDataOffset test" message printed on console | Should be successful |
 * | 02 | Define channelId, counter, negative dataOffset and dataBuffer; log details for constructor invocation | channelId = 123, counter = 456, dataOffset = -50, dataBuffer = {0x10, 0x20} | Values are set and logged correctly | Should be successful |
 * | 03 | Call TtmlDataPacket constructor with the negative dataOffset and check for exceptions | channelId = 123, counter = 456, dataOffset = -50, dataBuffer values {0x10, 0x20} | The EXPECT_NO_THROW block passes, indicating the constructor did not throw | Should Pass |
 * | 04 | Log the completion of NegativeDataOffset test | None | "Exiting NegativeDataOffset test" message printed on console | Should be successful |
 */
TEST(TtmlDataPacket, NegativeDataOffset) {
    std::cout << "Entering NegativeDataOffset test" << std::endl;
    
    std::uint32_t channelId = 123;
    std::uint32_t counter = 456;
    std::int64_t dataOffset = -50;
    std::vector<std::uint8_t> dataBuffer = {0x10, 0x20};
    std::cout << "Invoking TtmlDataPacket constructor with channelId=" << channelId
              << ", counter=" << counter << ", dataOffset=" << dataOffset 
              << " and dataBuffer values {0x10, 0x20}" << std::endl;
    
    EXPECT_NO_THROW({
        TtmlDataPacket packet(channelId, counter, dataOffset, std::move(dataBuffer));
        std::cout << " constructor invocation  completed  " <<std::endl;
    });
    
    std::cout << "Exiting NegativeDataOffset test" << std::endl;
}
/**
 * @brief Verify that TtmlSelectionPacket constructor handles typical mid-range positive values without throwing exceptions
 *
 * This test verifies that the TtmlSelectionPacket constructor is able to construct an object without throwing any exceptions when provided with typical mid-range positive values. The test ensures that standard operational parameters are managed correctly by the API.
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
 * | Variation / Step | Description                                                                                       | Test Data                                             | Expected Result                                              | Notes      |
 * | :--------------: | ------------------------------------------------------------------------------------------------- | ----------------------------------------------------- | ------------------------------------------------------------ | ---------- |
 * | 01               | Invoke TtmlSelectionPacket constructor with typical mid-range positive values and expect no exception. | channelId = 123, counter = 456, width = 640, height = 480 | No exception thrown; object constructed successfully          | Should Pass |
 */
TEST(TtmlSelectionPacket, TypicalMidRange) {
    std::cout << "Entering TypicalMidRange test" << std::endl;
    EXPECT_NO_THROW({
        uint32_t channelId = 123;
        uint32_t counter = 456;
        uint32_t width = 640;
        uint32_t height = 480;
        std::cout << "Invoking TtmlSelectionPacket constructor with parameters: "
                  << "channelId = " << channelId << ", "
                  << "counter = " << counter << ", "
                  << "width = " << width << ", "
                  << "height = " << height << std::endl;
        TtmlSelectionPacket packet(channelId, counter, width, height);
        std::cout << "TtmlSelectionPacket object constructed successfully with typical mid-range positive values." << std::endl;
    });
    std::cout << "Exiting TypicalMidRange test" << std::endl;
}
/**
 * @brief Verify constructor behavior with zero channelId.
 *
 * This test case validates that creating a TtmlSelectionPacket object with a channelId value of zero does not throw any exceptions. It ensures that a zero channelId is handled as a valid input.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 023
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                        | Test Data                                 | Expected Result                                                               | Notes      |
 * | :--------------: | ------------------------------------------------------------------ | ----------------------------------------- | ----------------------------------------------------------------------------- | ---------- |
 * | 01               | Invoke TtmlSelectionPacket constructor with parameters including zero channelId. | channelId = 0, counter = 456, width = 640, height = 480 | Object constructed successfully without any exception thrown; EXPECT_NO_THROW passes. | Should Pass |
 */
TEST(TtmlSelectionPacket, ZeroChannelId) {
    std::cout << "Entering ZeroChannelId test" << std::endl;
    EXPECT_NO_THROW({
        uint32_t channelId = 0;
        uint32_t counter = 456;
        uint32_t width = 640;
        uint32_t height = 480;
        std::cout << "Invoking TtmlSelectionPacket constructor with parameters: "
                  << "channelId = " << channelId << ", "
                  << "counter = " << counter << ", "
                  << "width = " << width << ", "
                  << "height = " << height << std::endl;
        TtmlSelectionPacket packet(channelId, counter, width, height);
        std::cout << "TtmlSelectionPacket object constructed successfully with zero channelId." << std::endl;
    });
    std::cout << "Exiting ZeroChannelId test" << std::endl;
}
/**
 * @brief Test to verify that the TtmlSelectionPacket constructor handles a zero counter correctly without throwing an exception.
 *
 * This test verifies that constructing a TtmlSelectionPacket object with a counter value of zero and valid dimensions does not throw any exceptions. It ensures that the class can handle a zero counter scenario, which is a part of the expected functionality.
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
 * | Variation / Step | Description                                                                           | Test Data                                              | Expected Result                                       | Notes       |
 * | :--------------: | ------------------------------------------------------------------------------------- | ------------------------------------------------------ | ----------------------------------------------------- | ----------- |
 * | 01               | Invoke TtmlSelectionPacket constructor with zero counter and valid dimensions         | channelId = 123, counter = 0, width = 640, height = 480  | TtmlSelectionPacket constructed without exception     | Should Pass |
 */
TEST(TtmlSelectionPacket, ZeroCounter) {
    std::cout << "Entering ZeroCounter test" << std::endl;
    EXPECT_NO_THROW({
        uint32_t channelId = 123;
        uint32_t counter = 0;
        uint32_t width = 640;
        uint32_t height = 480;
        std::cout << "Invoking TtmlSelectionPacket constructor with parameters: "
                  << "channelId = " << channelId << ", "
                  << "counter = " << counter << ", "
                  << "width = " << width << ", "
                  << "height = " << height << std::endl;
        TtmlSelectionPacket packet(channelId, counter, width, height);
        std::cout << "TtmlSelectionPacket object constructed successfully with zero counter." << std::endl;
    });
    std::cout << "Exiting ZeroCounter test" << std::endl;
}
/**
 * @brief Test case to verify the TtmlSelectionPacket constructor handles a zero width scenario.
 *
 * This test checks that the TtmlSelectionPacket constructor does not throw an exception and constructs
 * the object successfully when provided with a width value of zero. This behavior ensures that even edge
 * cases with zero dimension parameters are handled correctly.
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
 * | 01 | Invoke TtmlSelectionPacket constructor with zero width parameter | channelId = 123, counter = 456, width = 0, height = 480 | API does not throw exception and object is constructed successfully | Should Pass |
 */
TEST(TtmlSelectionPacket, ZeroWidth) {
    std::cout << "Entering ZeroWidth test" << std::endl;
    EXPECT_NO_THROW({
        uint32_t channelId = 123;
        uint32_t counter = 456;
        uint32_t width = 0;
        uint32_t height = 480;
        std::cout << "Invoking TtmlSelectionPacket constructor with parameters: "
                  << "channelId = " << channelId << ", "
                  << "counter = " << counter << ", "
                  << "width = " << width << ", "
                  << "height = " << height << std::endl;
        TtmlSelectionPacket packet(channelId, counter, width, height);
        std::cout << "TtmlSelectionPacket object constructed successfully with zero width." << std::endl;
    });
    std::cout << "Exiting ZeroWidth test" << std::endl;
}
/**
 * @brief Test the TtmlSelectionPacket constructor with zero height.
 *
 * This test verifies that the TtmlSelectionPacket object can be constructed without throwing an exception even when the height parameter is zero. This is important to ensure that the constructor properly handles cases with a zero height without causing runtime errors.
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
 * | Variation / Step | Description                                                               | Test Data                                                       | Expected Result                                           | Notes       |
 * | :--------------: | ------------------------------------------------------------------------- | ---------------------------------------------------------------- | --------------------------------------------------------- | ----------- |
 * | 01               | Invoke the TtmlSelectionPacket constructor with a zero height value.      | channelId = 123, counter = 456, width = 640, height = 0            | TtmlSelectionPacket object is constructed without exception | Should Pass |
 */
TEST(TtmlSelectionPacket, ZeroHeight) {
    std::cout << "Entering ZeroHeight test" << std::endl;
    EXPECT_NO_THROW({
        uint32_t channelId = 123;
        uint32_t counter = 456;
        uint32_t width = 640;
        uint32_t height = 0;
        std::cout << "Invoking TtmlSelectionPacket constructor with parameters: "
                  << "channelId = " << channelId << ", "
                  << "counter = " << counter << ", "
                  << "width = " << width << ", "
                  << "height = " << height << std::endl;
        TtmlSelectionPacket packet(channelId, counter, width, height);
        std::cout << "TtmlSelectionPacket object constructed successfully with zero height." << std::endl;
    });
    std::cout << "Exiting ZeroHeight test" << std::endl;
}
/**
 * @brief Validates successful construction of TtmlTimestampPacket with typical valid input values.
 *
 * This test verifies that providing standard valid parameters to the TtmlTimestampPacket constructor, such as channelId, counter, and timestamp, results in successful packet creation without throwing any exceptions. This ensures that the object is properly instantiated when valid values are provided.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 027
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                 | Test Data                                    | Expected Result                                                                         | Notes      |
 * | :--------------: | --------------------------------------------------------------------------- | -------------------------------------------- | --------------------------------------------------------------------------------------- | ---------- |
 * | 01               | Invoke TtmlTimestampPacket constructor with channelId=123, counter=456, timestamp=789000 to validate successful construction | channelId = 123, counter = 456, timestamp = 789000 | Constructor completes without throwing any exception and the object is successfully constructed | Should Pass |
 */
TEST(TtmlTimestampPacket, ConstructWithTypicalValidValues) {
    std::cout << "Entering ConstructWithTypicalValidValues test" << std::endl;
    std::uint32_t channelId = 123;
    std::uint32_t counter = 456;
    std::uint64_t timestamp = 789000;
    std::cout << "Invoking TtmlTimestampPacket constructor with channelId: " << channelId
              << ", counter: " << counter << ", timestamp: " << timestamp << std::endl;
    EXPECT_NO_THROW({
        TtmlTimestampPacket packet(channelId, counter, timestamp);
        std::cout << "TtmlTimestampPacket constructed successfully with typical valid values." << std::endl;
        // Debug: (Assuming retrieval of internal states if getters were available)
    });
    std::cout << "Exiting ConstructWithTypicalValidValues test" << std::endl;
}
/**
 * @brief Test constructing TtmlTimestampPacket with zero channelId to ensure proper object creation.
 *
 * This test verifies that the TtmlTimestampPacket constructor works as expected when a zero channelId is provided.
 * The test confirms that the constructor completes execution without throwing any exceptions,
 * ensuring that the implementation handles a channelId of zero correctly.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 028@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                                                | Test Data                                      |Expected Result                                               |Notes       |
 * | :----:           | ------------------------------------------------------------------------------------------ | ---------------------------------------------- |-------------------------------------------------------------- |----------- |
 * | 01               | Invoke TtmlTimestampPacket constructor with channelId=0, counter=456, and timestamp=789000.  | channelId = 0, counter = 456, timestamp = 789000 | Constructor completes without throwing any exceptions.       | Should Pass |
 */
TEST(TtmlTimestampPacket, ConstructWithZeroChannelId) {
    std::cout << "Entering ConstructWithZeroChannelId test" << std::endl;
    std::uint32_t channelId = 0;
    std::uint32_t counter = 456;
    std::uint64_t timestamp = 789000;
    std::cout << "Invoking TtmlTimestampPacket constructor with channelId: " << channelId
              << ", counter: " << counter << ", timestamp: " << timestamp << std::endl;
    EXPECT_NO_THROW({
        TtmlTimestampPacket packet(channelId, counter, timestamp);
        std::cout << "TtmlTimestampPacket constructed successfully with zero channelId." << std::endl;
    });
    std::cout << "Exiting ConstructWithZeroChannelId test" << std::endl;
}
/**
 * @brief Tests the TtmlTimestampPacket constructor when a counter value of zero is provided
 *
 * This test verifies that the TtmlTimestampPacket constructor does not throw any exceptions when invoked with a counter value of zero. It checks the proper initialization of the packet with the specified channelId, counter, and timestamp values.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Initialize test parameters and log the test entry. | channelId = 123, counter = 0, timestamp = 789000 | Logs the entering test message with provided parameters | Should be successful |
 * | 02 | Invoke the TtmlTimestampPacket constructor with the given parameters. | Input: channelId = 123, counter = 0, timestamp = 789000, Output: TtmlTimestampPacket instance | Constructor does not throw any exception and packet is created successfully | Should Pass |
 * | 03 | Log the test completion. | None | Logs the exiting test message | Should be successful |
 */
TEST(TtmlTimestampPacket, ConstructWithZeroCounter) {
    std::cout << "Entering ConstructWithZeroCounter test" << std::endl;
    std::uint32_t channelId = 123;
    std::uint32_t counter = 0;
    std::uint64_t timestamp = 789000;
    std::cout << "Invoking TtmlTimestampPacket constructor with channelId: " << channelId
              << ", counter: " << counter << ", timestamp: " << timestamp << std::endl;
    EXPECT_NO_THROW({
        TtmlTimestampPacket packet(channelId, counter, timestamp);
        std::cout << "TtmlTimestampPacket constructed successfully with zero counter." << std::endl;
    });
    std::cout << "Exiting ConstructWithZeroCounter test" << std::endl;
}
/**
 * @brief Validate that TtmlTimestampPacket can be constructed with a zero timestamp without throwing exceptions
 *
 * This test verifies the construction of a TtmlTimestampPacket object when the timestamp value is zero.
 * It ensures that even with a timestamp of zero, the constructor initializes the object properly and does not throw any exceptions.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 030@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoke TtmlTimestampPacket constructor with valid channelId, counter and a zero timestamp | channelId = 123, counter = 456, timestamp = 0 | TtmlTimestampPacket is constructed successfully without throwing an exception | Should Pass |
 */
TEST(TtmlTimestampPacket, ConstructWithZeroTimestamp) {
    std::cout << "Entering ConstructWithZeroTimestamp test" << std::endl;
    std::uint32_t channelId = 123;
    std::uint32_t counter = 456;
    std::uint64_t timestamp = 0;
    std::cout << "Invoking TtmlTimestampPacket constructor with channelId: " << channelId
              << ", counter: " << counter << ", timestamp: " << timestamp << std::endl;
    EXPECT_NO_THROW({
        TtmlTimestampPacket packet(channelId, counter, timestamp);
        std::cout << "TtmlTimestampPacket constructed successfully with zero timestamp." << std::endl;
    });
    std::cout << "Exiting ConstructWithZeroTimestamp test" << std::endl;
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
