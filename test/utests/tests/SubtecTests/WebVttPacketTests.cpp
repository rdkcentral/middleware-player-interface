
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
#include "WebVttPacket.hpp"


// Test Case: Valid data packet with default time offset
/**
 * @brief Verify default construction and SendDataPacket functionality with valid inputs.
 *
 * This test ensures that a WebVttChannel object can be created using the default constructor and that the SendDataPacket method executes without throwing exceptions when provided with a valid data vector and a default time offset value.
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
 * | Variation / Step | Description                                                            | Test Data                                                      | Expected Result                                                  | Notes         |
 * | :--------------: | ---------------------------------------------------------------------- | -------------------------------------------------------------- | ---------------------------------------------------------------- | ------------- |
 * | 01               | Create a WebVttChannel object using the default constructor.           | N/A                                                            | Object is instantiated without throwing an exception.          | Should be successful |
 * | 02               | Prepare test data: a vector with values {0x01, 0x02, 0x03} and time_offset_ms = 0. | data = {0x01,0x02,0x03}, time_offset_ms = 0                      | Data vector and time offset are correctly prepared.              | Should be successful |
 * | 03               | Invoke SendDataPacket with the prepared test data.                     | input: data = {0x01,0x02,0x03}, time_offset_ms = 0                 | API executes without throwing an exception.                      | Should Pass   |
 */
TEST(WebVttChannel, ValidDataPacketDefault) {
    std::cout << "Entering ValidDataPacketDefault test" << std::endl;
    
    // Create a WebVttChannel object using default constructor
    EXPECT_NO_THROW({
        WebVttChannel channel;
        std::cout << "WebVttChannel object created using default constructor." << std::endl;
        
        // Prepare test data
        std::vector<uint8_t> data = {0x01, 0x02, 0x03};
        int64_t time_offset_ms = 0;
        std::cout << "Prepared data vector with values: ";
        for(auto val : data) {
            std::cout << "0x" << std::hex << static_cast<int>(val) << " ";
        }
        std::cout << std::dec << " and time_offset_ms: " << time_offset_ms << std::endl;
        
        // Invoke SendDataPacket method using EXPECT_NO_THROW
        std::cout << "Invoking SendDataPacket with default time offset." << std::endl;
        EXPECT_NO_THROW(channel.SendDataPacket(std::move(data), time_offset_ms));
        std::cout << "SendDataPacket invoked successfully with default time offset." << std::endl;
    });
    
    std::cout << "Exiting ValidDataPacketDefault test" << std::endl;
}
/**
 * @brief Test the SendDataPacket API by providing a valid data packet with a positive time offset
 *
 * This test verifies that the SendDataPacket method of the WebVttChannel class functions properly when provided with a valid data packet and a positive time offset value. The method is expected to handle the operation without throwing any exceptions.
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
 * | Variation / Step | Description                                                          | Test Data                                                                                       | Expected Result                                                            | Notes           |
 * | :--------------: | -------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------ | -------------------------------------------------------------------------- | --------------- |
 * | 01               | Create a WebVttChannel object using its default constructor          | N/A                                                                                             | Object created successfully                                                | Should be successful |
 * | 02               | Prepare data vector and time offset                                  | data = {0x10, 0x20, 0x30}, time_offset_ms = 100                                                  | Data and time offset prepared correctly                                  | Should be successful |
 * | 03               | Invoke SendDataPacket with the valid data packet and positive time offset | data (moved) = {0x10, 0x20, 0x30}, time_offset_ms = 100                                            | SendDataPacket completes without throwing any exceptions; assertion passes | Should Pass     |
 */
TEST(WebVttChannel, ValidDataPacketPositiveTimeOffset) {
    std::cout << "Entering ValidDataPacketPositiveTimeOffset test" << std::endl;
    
    EXPECT_NO_THROW({
        WebVttChannel channel;
        std::cout << "WebVttChannel object created using default constructor." << std::endl;
        
        // Prepare test data
        std::vector<uint8_t> data = {0x10, 0x20, 0x30};
        int64_t time_offset_ms = 100;
        std::cout << "Prepared data vector with values: ";
        for(auto val : data) {
            std::cout << "0x" << std::hex << static_cast<int>(val) << " ";
        }
        std::cout << std::dec << "and time_offset_ms: " << time_offset_ms << std::endl;
        
        // Invoke SendDataPacket method with positive time offset
        std::cout << "Invoking SendDataPacket with positive time offset." << std::endl;
        EXPECT_NO_THROW(channel.SendDataPacket(std::move(data), time_offset_ms));
        std::cout << "SendDataPacket invoked successfully with positive time offset." << std::endl;
    });
    
    std::cout << "Exiting ValidDataPacketPositiveTimeOffset test" << std::endl;
}
/**
 * @brief Validates that WebVttChannel::SendDataPacket can handle a negative time offset without throwing exceptions.
 *
 * This test verifies that when a negative time offset is passed along with a valid data packet, the SendDataPacket method processes the inputs without throwing exceptions. This ensures that the API handles negative time offsets appropriately.
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
 * | 01 | Create a WebVttChannel instance using the default constructor. | None | Instance created without throwing exceptions. | Should be successful |
 * | 02 | Prepare the test data vector and a negative time offset. | data = {0xAA, 0xBB, 0xCC}, time_offset_ms = -50 | Test data prepared correctly. | Should be successful |
 * | 03 | Invoke SendDataPacket with the prepared negative time offset and data packet. | data = {0xAA,0xBB,0xCC}, time_offset_ms = -50 | SendDataPacket executes without throwing exceptions. | Should Pass |
 */
TEST(WebVttChannel, ValidDataPacketNegativeTimeOffset) {
    std::cout << "Entering ValidDataPacketNegativeTimeOffset test" << std::endl;
    
    EXPECT_NO_THROW({
        WebVttChannel channel;
        std::cout << "WebVttChannel object created using default constructor." << std::endl;
        
        // Prepare test data
        std::vector<uint8_t> data = {0xAA, 0xBB, 0xCC};
        int64_t time_offset_ms = -50;
        std::cout << "Prepared data vector with values: ";
        for(auto val : data) {
            std::cout << "0x" << std::hex << static_cast<int>(val) << " ";
        }
        std::cout << std::dec << "and time_offset_ms: " << time_offset_ms << std::endl;
        
        // Invoke SendDataPacket method with negative time offset
        std::cout << "Invoking SendDataPacket with negative time offset." << std::endl;
        EXPECT_NO_THROW(channel.SendDataPacket(std::move(data), time_offset_ms));
        std::cout << "SendDataPacket invoked successfully with negative time offset." << std::endl;
    });
    
    std::cout << "Exiting ValidDataPacketNegativeTimeOffset test" << std::endl;
}
/**
 * @brief Verifies that SendDataPacket does not throw when provided with an empty data vector and default time offset.
 *
 * This test case creates a WebVttChannel object using the default constructor, prepares an empty data vector along with a default time offset value, and then invokes the SendDataPacket API. The test ensures that no exceptions are thrown when calling SendDataPacket with empty inputs.
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
 * | Variation / Step | Description                                                                                        | Test Data                                       | Expected Result                            | Notes             |
 * | :--------------: | -------------------------------------------------------------------------------------------------- | ----------------------------------------------- | ------------------------------------------ | ----------------- |
 * | 01               | Create a WebVttChannel object using the default constructor                                        | (none)                                          | WebVttChannel object is created successfully| Should be successful |
 * | 02               | Prepare an empty data vector and initialize time_offset_ms to 0                                   | data: empty vector, time_offset_ms: 0           | Data is prepared correctly                 | Should be successful |
 * | 03               | Invoke SendDataPacket with the empty data vector and default time offset to verify exception safety | data: empty vector, time_offset_ms: 0           | No exceptions are thrown during SendDataPacket call | Should Pass       |
 */
TEST(WebVttChannel, EmptyDataPacketDefaultTimeOffset) {
    std::cout << "Entering EmptyDataPacketDefaultTimeOffset test" << std::endl;
    
    EXPECT_NO_THROW({
        WebVttChannel channel;
        std::cout << "WebVttChannel object created using default constructor." << std::endl;
        
        // Prepare empty data vector
        std::vector<uint8_t> data;
        int64_t time_offset_ms = 0;
        std::cout << "Prepared an empty data vector and time_offset_ms: " << time_offset_ms << std::endl;
        
        // Invoke SendDataPacket method for empty data
        std::cout << "Invoking SendDataPacket with empty data and default time offset." << std::endl;
        EXPECT_NO_THROW(channel.SendDataPacket(std::move(data), time_offset_ms));
        std::cout << "SendDataPacket invoked successfully with empty data." << std::endl;
    });
    
    std::cout << "Exiting EmptyDataPacketDefaultTimeOffset test" << std::endl;
}
/**
 * @brief Validate that the SendDataPacket method processes a large data packet with default time offset without exceptions.
 *
 * This test creates a WebVttChannel object using its default constructor, prepares a large data vector containing 10,000 sequential bytes, and invokes the SendDataPacket method with a default time offset (0 ms). The objective is to ensure that the API can handle large data packets without throwing any exceptions.
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
 * | Variation / Step | Description | Test Data |Expected Result |Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create a WebVttChannel object using its default constructor. | None | Object is successfully instantiated without any exceptions. | Should Pass |
 * | 02 | Prepare a large data vector containing 10,000 sequential bytes and set time_offset_ms to 0. | data = vector with 10000 sequential bytes, time_offset_ms = 0 | Data vector is correctly prepared and time_offset_ms is correctly set. | Should be successful |
 * | 03 | Invoke the SendDataPacket method with the prepared large data vector and default time offset. | input: data = large data vector, time_offset_ms = 0 | The SendDataPacket API call completes without throwing any exceptions. | Should Pass |
 */
TEST(WebVttChannel, LargeDataPacketDefaultTimeOffset) {
    std::cout << "Entering LargeDataPacketDefaultTimeOffset test" << std::endl;
    
    EXPECT_NO_THROW({
        WebVttChannel channel;
        std::cout << "WebVttChannel object created using default constructor." << std::endl;
        
        // Prepare large data vector with 10,000 sequential bytes
        std::vector<uint8_t> data;
        data.reserve(10000);
        for (int i = 0; i < 10000; ++i) {
            data.push_back(static_cast<uint8_t>(i % 256));
        }
        int64_t time_offset_ms = 0;
        std::cout << "Prepared large data vector with 10,000 sequential bytes and time_offset_ms: " << time_offset_ms << std::endl;
        
        // Invoke SendDataPacket method for large data
        std::cout << "Invoking SendDataPacket with large data packet and default time offset." << std::endl;
        EXPECT_NO_THROW(channel.SendDataPacket(std::move(data), time_offset_ms));
        std::cout << "SendDataPacket invoked successfully with large data packet." << std::endl;
    });
    
    std::cout << "Exiting LargeDataPacketDefaultTimeOffset test" << std::endl;
}
/**
 * @brief Test to verify that SendDataPacket handles maximum int64_t time offset with valid data.
 *
 * This test validates that a WebVttChannel object can be created successfully and that its SendDataPacket method
 * accepts a data packet with a valid data vector and the maximum possible int64_t time offset without throwing an exception.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01| Create the WebVttChannel object, prepare a data vector and set the maximum int64_t time offset | data = {0xDE, 0xAD, 0xBE, 0xEF}, time_offset_ms = std::numeric_limits<int64_t>::max() | WebVttChannel object is created and test data is prepared successfully | Should be successful |
 * | 02| Invoke SendDataPacket with the prepared data vector and maximum time offset | data (moved) = {0xDE, 0xAD, 0xBE, 0xEF}, time_offset_ms = maximum int64_t value | Method call completes without throwing an exception | Should Pass |
 */
TEST(WebVttChannel, ValidDataPacketMaxTimeOffset) {
    std::cout << "Entering ValidDataPacketMaxTimeOffset test" << std::endl;
    
    EXPECT_NO_THROW({
        WebVttChannel channel;
        std::cout << "WebVttChannel object created using default constructor." << std::endl;
        
        // Prepare test data
        std::vector<uint8_t> data = {0xDE, 0xAD, 0xBE, 0xEF};
        int64_t time_offset_ms = std::numeric_limits<int64_t>::max();
        std::cout << "Prepared data vector with values: ";
        for(auto val : data) {
            std::cout << "0x" << std::hex << static_cast<int>(val) << " ";
        }
        std::cout << std::dec << "and maximum time_offset_ms: " << time_offset_ms << std::endl;
        
        // Invoke SendDataPacket method with maximum time offset
        std::cout << "Invoking SendDataPacket with maximum int64_t time offset." << std::endl;
        EXPECT_NO_THROW(channel.SendDataPacket(std::move(data), time_offset_ms));
        std::cout << "SendDataPacket invoked successfully with maximum int64_t time offset." << std::endl;
    });
    
    std::cout << "Exiting ValidDataPacketMaxTimeOffset test" << std::endl;
}
/**
 * @brief Validates that the WebVttChannel can process a data packet with the minimum possible time offset.
 *
 * Tests that the SendDataPacket method in the WebVttChannel class correctly handles a data packet when provided with the minimum int64_t value as the time offset.
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
 * | 01 | Create a WebVttChannel object using the default constructor | N/A | Object instantiated successfully | Should be successful |
 * | 02 | Prepare test data by creating a vector with values 0xFE, 0xED, 0xFA, 0xCE and setting time_offset_ms to the minimum int64_t value | input: data = 0xFE,0xED,0xFA,0xCE; time_offset_ms = std::numeric_limits<int64_t>::min() | Data vector and time offset prepared successfully | Should be successful |
 * | 03 | Invoke SendDataPacket with the prepared data and minimum time offset | input: data = 0xFE,0xED,0xFA,0xCE; time_offset_ms = minimum int64_t | API call completes without throwing an exception | Should Pass |
 */
TEST(WebVttChannel, ValidDataPacketMinTimeOffset) {
    std::cout << "Entering ValidDataPacketMinTimeOffset test" << std::endl;
    
    EXPECT_NO_THROW({
        WebVttChannel channel;
        std::cout << "WebVttChannel object created using default constructor." << std::endl;
        
        // Prepare test data
        std::vector<uint8_t> data = {0xFE, 0xED, 0xFA, 0xCE};
        int64_t time_offset_ms = std::numeric_limits<int64_t>::min();
        std::cout << "Prepared data vector with values: ";
        for(auto val : data) {
            std::cout << "0x" << std::hex << static_cast<int>(val) << " ";
        }
        std::cout << std::dec << "and minimum time_offset_ms: " << time_offset_ms << std::endl;
        
        // Invoke SendDataPacket method with minimum time offset
        std::cout << "Invoking SendDataPacket with minimum int64_t time offset." << std::endl;
        EXPECT_NO_THROW(channel.SendDataPacket(std::move(data), time_offset_ms));
        std::cout << "SendDataPacket invoked successfully with minimum int64_t time offset." << std::endl;
    });
    
    std::cout << "Exiting ValidDataPacketMinTimeOffset test" << std::endl;
}
/**
 * @brief Verify that WebVttChannel successfully processes valid dimensions without exceptions
 *
 * This test case verifies that when valid width and height values (640 and 480)
 * are provided to the SendSelectionPacket API of the WebVttChannel class, the method
 * executes without throwing any exceptions. The test ensures the correct usage
 * of the default constructor and validates that the API accepts and processes the
 * provided dimensions.
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
 * | Variation / Step | Description                                                            | Test Data                             | Expected Result                                                           | Notes        |
 * | :--------------: | ---------------------------------------------------------------------- | ------------------------------------- | ------------------------------------------------------------------------- | ------------ |
 * | 01               | Create WebVttChannel object using default constructor                  | N/A                                   | Object is successfully created without throwing any exceptions          | Should Pass  |
 * | 02               | Invoke SendSelectionPacket using width and height parameters (640, 480)  | width = 640, height = 480               | API call does not throw and processes the packet with provided dimensions | Should Pass  |
 */
TEST(WebVttChannel, ValidDimensions)
{
    std::cout << "Entering ValidDimensions test" << std::endl;
    
    // Create WebVttChannel object using default constructor
    EXPECT_NO_THROW({
        WebVttChannel channel;
        std::cout << "Created WebVttChannel object." << std::endl;
        
        // Define test values for width and height
        uint32_t width = 640;
        uint32_t height = 480;
        std::cout << "Invoking SendSelectionPacket with width: " << width << " and height: " << height << std::endl;
        
        // Call SendSelectionPacket method and log the action
        EXPECT_NO_THROW(channel.SendSelectionPacket(width, height));
        std::cout << "SendSelectionPacket executed." << std::endl;
        std::cout << "Expected: Packet sent with width " << width << " and height " << height << std::endl;
    });
    
    std::cout << "Exiting ValidDimensions test" << std::endl;
}
/**
 * @brief Verifies that the SendSelectionPacket method can handle maximum dimension values.
 *
 * This test creates a WebVttChannel object, assigns the maximum possible uint32_t values to width and height, and then calls the SendSelectionPacket method. It ensures that no exceptions are thrown during object creation and method invocation, validating the handling of extreme input parameters.
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
 * | Variation / Step | Description                                                                   | Test Data                                                    | Expected Result                                                        | Notes             |
 * | :--------------: | ----------------------------------------------------------------------------- | ------------------------------------------------------------ | ---------------------------------------------------------------------- | ----------------- |
 * | 01               | Create a WebVttChannel object using the default constructor                   | None                                                         | Object is created successfully without exceptions                    | Should be successful |
 * | 02               | Define maximum dimensions by setting width and height to uint32_t maximum value | width = std::numeric_limits<uint32_t>::max(), height = std::numeric_limits<uint32_t>::max() | Maximum dimension values are correctly assigned                        | Should Pass       |
 * | 03               | Invoke SendSelectionPacket with the maximum dimensions                        | input: width = std::numeric_limits<uint32_t>::max(), height = std::numeric_limits<uint32_t>::max() | Method executes without throwing exceptions and selection packet is sent | Should Pass       |
 */
TEST(WebVttChannel, MaxDimensions)
{
    std::cout << "Entering MaxDimensions test" << std::endl;
    
    // Create WebVttChannel object using default constructor
    EXPECT_NO_THROW({
        WebVttChannel channel;
        std::cout << "Created WebVttChannel object." << std::endl;
        
        // Define test values for width and height as maximum uint32_t values
        uint32_t width = std::numeric_limits<uint32_t>::max();
        uint32_t height = std::numeric_limits<uint32_t>::max();
        std::cout << "Invoking SendSelectionPacket with width: " << width << " and height: " << height << std::endl;
        
        // Call SendSelectionPacket method and log the action
        EXPECT_NO_THROW(channel.SendSelectionPacket(width, height));
        std::cout << "SendSelectionPacket executed." << std::endl;
        std::cout << "Expected: Packet sent with maximum dimensions: width " << width << " and height " << height << std::endl;
    });
    
    std::cout << "Exiting MaxDimensions test" << std::endl;
}
/**
 * @brief Test sending of a selection packet with a zero width and a valid positive height
 *
 * This test verifies that the WebVttChannel API properly handles a selection packet when the width is zero while the height is positive. It ensures that no exceptions are thrown during object creation and when invoking the SendSelectionPacket method.
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
 * | Variation / Step | Description                                                    | Test Data                          | Expected Result                                                   | Notes          |
 * | :--------------: | -------------------------------------------------------------- | ---------------------------------- | ----------------------------------------------------------------- | -------------- |
 * | 01               | Create a WebVttChannel object using the default constructor      | None                               | Object is created without throwing any exception                  | Should be successful |
 * | 02               | Invoke SendSelectionPacket with width = 0 and height = 720         | width = 0, height = 720              | Method executes without exceptions and sends a packet with width 0 and height 720 | Should Pass    |
 */
TEST(WebVttChannel, MixedDimensionsWidthZero)
{
    std::cout << "Entering MixedDimensionsWidthZero test" << std::endl;
    
    // Create WebVttChannel object using default constructor
    EXPECT_NO_THROW({
        WebVttChannel channel;
        std::cout << "Created WebVttChannel object." << std::endl;
        
        // Define test values with width zero and positive height
        uint32_t width = 0;
        uint32_t height = 720;
        std::cout << "Invoking SendSelectionPacket with width: " << width << " and height: " << height << std::endl;
        
        // Call SendSelectionPacket method and log the action
        EXPECT_NO_THROW(channel.SendSelectionPacket(width, height));
        std::cout << "SendSelectionPacket executed." << std::endl;
        std::cout << "Expected: Packet sent with width " << width << " and height " << height << std::endl;
    });
    
    std::cout << "Exiting MixedDimensionsWidthZero test" << std::endl;
}
/**
 * @brief Verifies SendSelectionPacket API handles mixed dimensions including zero height.
 *
 * This test validates that when the SendSelectionPacket method is called with a positive width and a zero height,
 * the API handles the mixed dimension scenario without throwing any exceptions. This ensures that even edge cases
 * like a zero height are managed gracefully.
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
 * | Variation / Step | Description                                                      | Test Data                 | Expected Result                                                                      | Notes          |
 * | :--------------: | ---------------------------------------------------------------- | ------------------------- | ------------------------------------------------------------------------------------ | -------------- |
 * | 01               | Create WebVttChannel object using default constructor            | N/A                       | Object is successfully created                                                       | Should Pass    |
 * | 02               | Define test values with positive width and zero height           | width=1280, height=0        | Test variables are set correctly                                                     | Should be successful |
 * | 03               | Invoke SendSelectionPacket with provided test values             | width=1280, height=0        | API call executes without throwing; packet sent with width 1280 and height 0           | Should Pass    |
 */
TEST(WebVttChannel, MixedDimensionsHeightZero)
{
    std::cout << "Entering MixedDimensionsHeightZero test" << std::endl;
    
    // Create WebVttChannel object using default constructor
    EXPECT_NO_THROW({
        WebVttChannel channel;
        std::cout << "Created WebVttChannel object." << std::endl;
        
        // Define test values with positive width and height zero
        uint32_t width = 1280;
        uint32_t height = 0;
        std::cout << "Invoking SendSelectionPacket with width: " << width << " and height: " << height << std::endl;
        
        // Call SendSelectionPacket method and log the action
        EXPECT_NO_THROW(channel.SendSelectionPacket(width, height));
        std::cout << "SendSelectionPacket executed." << std::endl;
        std::cout << "Expected: Packet sent with width " << width << " and height " << height << std::endl;
    });
    
    std::cout << "Exiting MixedDimensionsHeightZero test" << std::endl;
}
/**
 * @brief Test SendTimestampPacket API with a typical timestamp value.
 *
 * This test verifies that the SendTimestampPacket API in the WebVttChannel class executes successfully when invoked with a typical timestamp value (1000 milliseconds). It confirms that both the object construction and the API invocation do not throw any exceptions.
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
 * | Variation / Step | Description                                                        | Test Data                                             | Expected Result                                         | Notes         |
 * | :--------------: | ------------------------------------------------------------------ | ----------------------------------------------------- | ------------------------------------------------------- | ------------- |
 * | 01               | Create a WebVttChannel object using the default constructor and confirm no exceptions are thrown. | No input; output: WebVttChannel object creation       | Object is successfully created without exceptions.    | Should be successful |
 * | 02               | Define a typical timestamp value and log it to the console.        | timestamp = 1000                                      | Timestamp value is defined and logged correctly.        | Should be successful |
 * | 03               | Invoke SendTimestampPacket with the defined timestamp and verify no exceptions occur.  | channel.SendTimestampPacket(timestamp), timestamp = 1000 | SendTimestampPacket executes without throwing exceptions. | Should Pass   |
 */
TEST(WebVttChannel, SendTimestampPacket_TypicalTimestampTest) {
    std::cout << "Entering SendTimestampPacket_TypicalTimestampTest test" << std::endl;
    
    // Create WebVttChannel object using default constructor and log creation.
    EXPECT_NO_THROW({
        WebVttChannel channel;
        std::cout << "WebVttChannel object created using default constructor." << std::endl;
        
        // Define input timestamp of 1000 milliseconds.
        uint64_t timestamp = 1000;
        std::cout << "Invoking SendTimestampPacket with timestamp: " << timestamp << std::endl;
        
        // Invoke the method and ensure it completes without errors.
        EXPECT_NO_THROW(channel.SendTimestampPacket(timestamp));
        std::cout << "SendTimestampPacket executed successfully with timestamp: " << timestamp << std::endl;
    });
    
    std::cout << "Exiting SendTimestampPacket_TypicalTimestampTest test" << std::endl;
}
/**
 * @brief To verify that the SendTimestampPacket function can handle a zero timestamp value.
 *
 * This test verifies that when a timestamp value of zero is provided to the SendTimestampPacket method of the WebVttChannel class, the method executes successfully without throwing any exceptions. It ensures that the boundary condition (zero) is correctly handled.
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
 * | Variation / Step | Description                                                  | Test Data                                   | Expected Result                                      | Notes         |
 * | :--------------: | ------------------------------------------------------------ | ------------------------------------------- | ---------------------------------------------------- | ------------- |
 * | 01               | Create a WebVttChannel object using default constructor      | None                                        | Object is created without throwing exceptions      | Should be successful |
 * | 02               | Define a timestamp value of 0 milliseconds                   | timestamp = 0                               | Timestamp is correctly initialized with value 0    | Should be successful |
 * | 03               | Invoke SendTimestampPacket with zero timestamp               | input: timestamp = 0, output: none            | API executes without throwing exceptions             | Should Pass   |
 */
TEST(WebVttChannel, SendTimestampPacket_ZeroTimestampTest) {
    std::cout << "Entering SendTimestampPacket_ZeroTimestampTest test" << std::endl;
    
    // Create WebVttChannel object using default constructor and log creation.
    EXPECT_NO_THROW({
        WebVttChannel channel;
        std::cout << "WebVttChannel object created using default constructor." << std::endl;
        
        // Define input timestamp of 0 milliseconds.
        uint64_t timestamp = 0;
        std::cout << "Invoking SendTimestampPacket with timestamp: " << timestamp << std::endl;
        
        // Invoke the method and ensure it completes without errors.
        EXPECT_NO_THROW(channel.SendTimestampPacket(timestamp));
        std::cout << "SendTimestampPacket executed successfully with timestamp: " << timestamp << std::endl;
    });
    
    std::cout << "Exiting SendTimestampPacket_ZeroTimestampTest test" << std::endl;
}
/**
 * @brief Test to validate SendTimestampPacket using the maximum timestamp value.
 *
 * This test verifies that the SendTimestampPacket method of the WebVttChannel class handles the maximum uint64_t timestamp value correctly without throwing an exception. It ensures proper object creation, correct boundary input assignment, and successful method execution.
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
 * | 01 | Create a WebVttChannel object using the default constructor. | (None) | Object created without throwing any exception. | Should be successful |
 * | 02 | Define the timestamp as the maximum uint64_t value using std::numeric_limits<uint64_t>::max(). | timestamp = std::numeric_limits<uint64_t>::max() | Timestamp is correctly assigned to the maximum value. | Should be successful |
 * | 03 | Invoke SendTimestampPacket with the maximum timestamp value. | input: timestamp = std::numeric_limits<uint64_t>::max() | Method completes without errors and no exceptions are thrown. | Should Pass |
 */
TEST(WebVttChannel, SendTimestampPacket_MaxTimestampTest) {
    std::cout << "Entering SendTimestampPacket_MaxTimestampTest test" << std::endl;
    
    // Create WebVttChannel object using default constructor and log creation.
    EXPECT_NO_THROW({
        WebVttChannel channel;
        std::cout << "WebVttChannel object created using default constructor." << std::endl;
        
        // Define input timestamp as maximum uint64_t value.
        uint64_t timestamp = std::numeric_limits<uint64_t>::max();
        std::cout << "Invoking SendTimestampPacket with timestamp: " << timestamp << std::endl;
        
        // Invoke the method and ensure it completes without errors.
        EXPECT_NO_THROW(channel.SendTimestampPacket(timestamp));
        std::cout << "SendTimestampPacket executed successfully with timestamp: " << timestamp << std::endl;
    });
    std::cout << "Exiting SendTimestampPacket_MaxTimestampTest test" << std::endl;
}
/**
 * @brief Verify that the WebVttChannel default constructor does not throw exceptions.
 *
 * This test verifies that the default constructor of the WebVttChannel class executes successfully without throwing any exceptions. The test ensures that a new instance of WebVttChannel is created as expected when no parameters are provided.
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
 * | Variation / Step | Description                                                                  | Test Data                                                  | Expected Result                                              | Notes             |
 * | :--------------: | -----------------------------------------------------------------------------| ---------------------------------------------------------- | ------------------------------------------------------------ | ----------------- |
 * | 01               | Print entering message for the test execution.                             | None                                                       | "Entering DefaultConstructionSuccess test" logged.         | Should be successful |
 * | 02               | Invoke WebVttChannel default constructor inside EXPECT_NO_THROW block.       | Input: None, Output: instance creation                     | No exception is thrown and the instance is created successfully. | Should Pass       |
 * | 03               | Print message confirming successful creation of WebVttChannel instance.      | None                                                       | "WebVttChannel instance created successfully." logged.       | Should be successful |
 * | 04               | Print exiting message indicating the end of the test execution.            | None                                                       | "Exiting DefaultConstructionSuccess test" logged.            | Should be successful |
 */
TEST(WebVttChannel, DefaultConstructionSuccess) {
    std::cout << "Entering DefaultConstructionSuccess test" << std::endl;
    
    std::cout << "Invoking WebVttChannel default constructor." << std::endl;
    EXPECT_NO_THROW({
        WebVttChannel channel;
    });
    std::cout << "WebVttChannel instance created successfully." << std::endl;
    std::cout << "Exiting DefaultConstructionSuccess test" << std::endl;
}
/**
 * @brief Tests the construction of a WebVttDataPacket object with valid positive inputs
 *
 * This test verifies that a WebVttDataPacket instance is successfully created when provided with valid inputs.
 * It ensures that no exceptions are thrown during the construction of the object with valid channelId, counter, timeOffsetMs,
 * and a non-empty dataBuffer. This test is important to confirm that the object instantiation works as expected under normal conditions.
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
 * | 01 | Set valid positive inputs for WebVttDataPacket constructor | channelId = 1, counter = 123, timeOffsetMs = 1000, dataBuffer = {0x01, 0x02, 0x03, 0x04} | Inputs are correctly assigned to create a valid object instance | Should be successful |
 * | 02 | Invoke the WebVttDataPacket constructor and verify no exception is thrown | Invoking constructor with the provided inputs | WebVttDataPacket object is constructed successfully without throwing any exceptions | Should Pass |
 */
TEST(WebVttDataPacket, ConstructWithValidPositiveInputs) {
    std::cout << "Entering ConstructWithValidPositiveInputs test" << std::endl;
    
    std::uint32_t channelId = 1;
    std::uint32_t counter = 123;
    std::int64_t timeOffsetMs = 1000;
    std::vector<std::uint8_t> dataBuffer = {0x01, 0x02, 0x03, 0x04};
    
    std::cout << "Invoking WebVttDataPacket constructor with channelId: " << channelId 
              << ", counter: " << counter 
              << ", timeOffsetMs: " << timeOffsetMs 
              << ", dataBuffer: [";
    for (size_t i = 0; i < dataBuffer.size(); i++) {
        std::cout << "0x" << std::hex << static_cast<int>(dataBuffer[i]);
        if(i != dataBuffer.size()-1)
            std::cout << ", ";
    }
    std::cout << std::dec << "]" << std::endl;
    
    EXPECT_NO_THROW({
        WebVttDataPacket packet(channelId, counter, timeOffsetMs, std::move(dataBuffer));
        std::cout << "WebVttDataPacket object constructed successfully." << std::endl;
    });
    
    std::cout << "Exiting ConstructWithValidPositiveInputs test" << std::endl;
}
/**
 * @brief Test constructing WebVttDataPacket with an empty dataBuffer
 *
 * This test verifies that the WebVttDataPacket object can be constructed successfully
 * when provided with a valid channelId, counter, timeOffsetMs, and an empty dataBuffer.
 * It ensures that no exceptions are thrown and the class handles an empty dataBuffer correctly.
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
 * | Variation / Step | Description                                                                     | Test Data                                                      | Expected Result                                                          | Notes       |
 * | :--------------: | ------------------------------------------------------------------------------- | -------------------------------------------------------------- | ------------------------------------------------------------------------ | ----------- |
 * | 01               | Invoke WebVttDataPacket constructor with valid parameters and an empty dataBuffer | channelId = 10, counter = 456, timeOffsetMs = 2000, dataBuffer = [] | The constructor should not throw any exception and construct the object successfully | Should Pass |
 */
TEST(WebVttDataPacket, ConstructWithEmptyDataBuffer) {
    std::cout << "Entering ConstructWithEmptyDataBuffer test" << std::endl;
    
    std::uint32_t channelId = 10;
    std::uint32_t counter = 456;
    std::int64_t timeOffsetMs = 2000;
    std::vector<std::uint8_t> dataBuffer; // empty buffer
    
    std::cout << "Invoking WebVttDataPacket constructor with channelId: " << channelId 
              << ", counter: " << counter 
              << ", timeOffsetMs: " << timeOffsetMs 
              << ", dataBuffer: []" << std::endl;
    
    EXPECT_NO_THROW({
        WebVttDataPacket packet(channelId, counter, timeOffsetMs, std::move(dataBuffer));
    });
    std::cout << "WebVttDataPacket object constructed successfully with empty dataBuffer." << std::endl;    
    std::cout << "Exiting ConstructWithEmptyDataBuffer test" << std::endl;
}
/**
 * @brief Verify that WebVttDataPacket can be constructed with a zero channelId without throwing exceptions
 *
 * This test verifies that when the WebVttDataPacket constructor is invoked with a channelId of 0 along with valid counter, timeOffsetMs, and dataBuffer values, it does not throw any exceptions. This scenario is important to ensure that the constructor gracefully handles a zero channel identifier.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 018
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                        | Test Data                                                          | Expected Result                                                               | Notes       |
 * | :--------------: | ------------------------------------------------------------------ | ------------------------------------------------------------------ | ----------------------------------------------------------------------------- | ----------- |
 * | 01               | Invoke WebVttDataPacket constructor with channelId = 0 and valid parameters | channelId = 0, counter = 123, timeOffsetMs = 1000, dataBuffer = 0x01,0x02,0x03,0x04 | Constructor does not throw an exception and creates a valid WebVttDataPacket instance | Should Pass |
 */
TEST(WebVttDataPacket, ConstructWithZeroChannelId) {
    std::cout << "Entering ConstructWithZeroChannelId test" << std::endl;
    
    std::uint32_t channelId = 0;
    std::uint32_t counter = 123;
    std::int64_t timeOffsetMs = 1000;
    std::vector<std::uint8_t> dataBuffer = {0x01, 0x02, 0x03, 0x04};
    
    std::cout << "Invoking WebVttDataPacket constructor with channelId: " << channelId 
              << " (zero), counter: " << counter 
              << ", timeOffsetMs: " << timeOffsetMs 
              << ", dataBuffer: [";
    for (size_t i = 0; i < dataBuffer.size(); i++) {
        std::cout << "0x" << std::hex << static_cast<int>(dataBuffer[i]);
        if(i != dataBuffer.size()-1)
            std::cout << ", ";
    }
    std::cout << std::dec << "]" << std::endl;
    
    EXPECT_NO_THROW({
        WebVttDataPacket packet(channelId, counter, timeOffsetMs, std::move(dataBuffer));
    });
    std::cout << "WebVttDataPacket object constructed successfully with zero channelId." << std::endl; 
    std::cout << "Exiting ConstructWithZeroChannelId test" << std::endl;
}
/**
 * @brief Test the WebVttDataPacket constructor when provided with a zero counter.
 *
 * This test verifies that constructing a WebVttDataPacket object with a counter value of zero does not throw any exceptions. It ensures that the object is correctly instantiated even when the counter is zero, validating proper handling of such input.
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
 * | Variation / Step | Description                                                                     | Test Data                                             | Expected Result                                                     | Notes      |
 * | :--------------: | ------------------------------------------------------------------------------- | ----------------------------------------------------- | ------------------------------------------------------------------- | ---------- |
 * | 01               | Invoke WebVttDataPacket constructor with channelId=1, counter=0, timeOffsetMs=1000, and a valid dataBuffer | channelId = 1, counter = 0, timeOffsetMs = 1000, dataBuffer = {0x01, 0x02, 0x03, 0x04} | Constructor should complete without throwing any exceptions | Should Pass |
 */
TEST(WebVttDataPacket, ConstructWithZeroCounter) {
    std::cout << "Entering ConstructWithZeroCounter test" << std::endl;
    
    std::uint32_t channelId = 1;
    std::uint32_t counter = 0;
    std::int64_t timeOffsetMs = 1000;
    std::vector<std::uint8_t> dataBuffer = {0x01, 0x02, 0x03, 0x04};
    
    std::cout << "Invoking WebVttDataPacket constructor with channelId: " << channelId 
              << ", counter: " << counter << " (zero)" 
              << ", timeOffsetMs: " << timeOffsetMs 
              << ", dataBuffer: [";
    for (size_t i = 0; i < dataBuffer.size(); i++) {
        std::cout << "0x" << std::hex << static_cast<int>(dataBuffer[i]);
        if(i != dataBuffer.size()-1)
            std::cout << ", ";
    }
    std::cout << std::dec << "]" << std::endl;
    
    EXPECT_NO_THROW({
        WebVttDataPacket packet(channelId, counter, timeOffsetMs, std::move(dataBuffer));
    });
    std::cout << "WebVttDataPacket object constructed successfully with zero counter." << std::endl;    
    std::cout << "Exiting ConstructWithZeroCounter test" << std::endl;
}
/**
 * @brief Verify construction of WebVttDataPacket with zero time offset
 *
 * This test verifies that creating a WebVttDataPacket object with a time offset value of zero does not result in an exception being thrown. It validates that the API correctly handles a zero value for timeOffsetMs.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | -------------- | ----- |
 * | 01 | Invoke the WebVttDataPacket constructor with channelId, counter, zero timeOffsetMs, and dataBuffer | channelId = 1, counter = 123, timeOffsetMs = 0, dataBuffer = {0x01, 0x02, 0x03, 0x04} | No exception is thrown; the object is constructed successfully | Should Pass |
 */
TEST(WebVttDataPacket, ConstructWithZeroTimeOffsetMs) {
    std::cout << "Entering ConstructWithZeroTimeOffsetMs test" << std::endl;
    
    std::uint32_t channelId = 1;
    std::uint32_t counter = 123;
    std::int64_t timeOffsetMs = 0;
    std::vector<std::uint8_t> dataBuffer = {0x01, 0x02, 0x03, 0x04};
    
    std::cout << "Invoking WebVttDataPacket constructor with channelId: " << channelId 
              << ", counter: " << counter 
              << ", timeOffsetMs: " << timeOffsetMs << " (zero)"
              << ", dataBuffer: [";
    for (size_t i = 0; i < dataBuffer.size(); i++) {
        std::cout << "0x" << std::hex << static_cast<int>(dataBuffer[i]);
        if(i != dataBuffer.size()-1)
            std::cout << ", ";
    }
    std::cout << std::dec << "]" << std::endl;
    
    EXPECT_NO_THROW({
        WebVttDataPacket packet(channelId, counter, timeOffsetMs, std::move(dataBuffer));
    });
    std::cout << "WebVttDataPacket object constructed successfully with zero timeOffsetMs." << std::endl;    
    std::cout << "Exiting ConstructWithZeroTimeOffsetMs test" << std::endl;
}
/**
 * @brief Test construction of WebVttDataPacket with maximum channelId.
 *
 * This test verifies that creating a WebVttDataPacket with the maximum value for channelId (UINT32_MAX), along with valid counter, timeOffsetMs, and dataBuffer, does not throw an exception. This test ensures that the constructor handles the boundary value for channelId correctly.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 021@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                               | Test Data                                                                                | Expected Result                                                            | Notes      |
 * | :--------------: | --------------------------------------------------------- | ---------------------------------------------------------------------------------------- | -------------------------------------------------------------------------- | ---------- |
 * | 01               | Initialize test parameters and invoke constructor         | channelId = UINT32_MAX, counter = 123, timeOffsetMs = 1000, dataBuffer = [0x01, 0x02, 0x03, 0x04] | WebVttDataPacket object is created with no exception thrown; assert passes  | Should Pass|
 */
TEST(WebVttDataPacket, ConstructWithMaxChannelId) {
    std::cout << "Entering ConstructWithMaxChannelId test" << std::endl;
    
    std::uint32_t channelId = std::numeric_limits<std::uint32_t>::max();
    std::uint32_t counter = 123;
    std::int64_t timeOffsetMs = 1000;
    std::vector<std::uint8_t> dataBuffer = {0x01, 0x02, 0x03, 0x04};
    
    std::cout << "Invoking WebVttDataPacket constructor with channelId: " << channelId 
              << " (UINT32_MAX), counter: " << counter 
              << ", timeOffsetMs: " << timeOffsetMs 
              << ", dataBuffer: [";
    for (size_t i = 0; i < dataBuffer.size(); i++) {
        std::cout << "0x" << std::hex << static_cast<int>(dataBuffer[i]);
        if(i != dataBuffer.size()-1)
            std::cout << ", ";
    }
    std::cout << std::dec << "]" << std::endl;
    
    EXPECT_NO_THROW({
        WebVttDataPacket packet(channelId, counter, timeOffsetMs, std::move(dataBuffer));
    });
    std::cout << "WebVttDataPacket object constructed successfully with maximum channelId." << std::endl;   
    std::cout << "Exiting ConstructWithMaxChannelId test" << std::endl;
}
/**
 * @brief Test constructing WebVttDataPacket with maximum counter value
 *
 * This test verifies that the WebVttDataPacket API correctly handles the construction of an object when the counter parameter is set to its maximum possible value. The test ensures that invoking the constructor with a maximum counter (UINT32_MAX) along with valid channelId, timeOffsetMs, and dataBuffer does not throw any exceptions.
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
 * | Variation / Step | Description                                                                 | Test Data                                                                                  | Expected Result                                                                                              | Notes       |
 * | :--------------: | --------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------ | ------------------------------------------------------------------------------------------------------------ | ----------- |
 * | 01               | Call WebVttDataPacket constructor with channelId, maximum counter, timeOffsetMs, and a sample dataBuffer | channelId = 1, counter = UINT32_MAX, timeOffsetMs = 1000, dataBuffer = [0x01, 0x02, 0x03, 0x04] | WebVttDataPacket object is constructed without throwing any exceptions (EXPECT_NO_THROW passes) | Should Pass |
 */
TEST(WebVttDataPacket, ConstructWithMaxCounter) {
    std::cout << "Entering ConstructWithMaxCounter test" << std::endl;
    
    std::uint32_t channelId = 1;
    std::uint32_t counter = std::numeric_limits<std::uint32_t>::max();
    std::int64_t timeOffsetMs = 1000;
    std::vector<std::uint8_t> dataBuffer = {0x01, 0x02, 0x03, 0x04};
    
    std::cout << "Invoking WebVttDataPacket constructor with channelId: " << channelId 
              << ", counter: " << counter << " (UINT32_MAX)"
              << ", timeOffsetMs: " << timeOffsetMs 
              << ", dataBuffer: [";
    for (size_t i = 0; i < dataBuffer.size(); i++) {
        std::cout << "0x" << std::hex << static_cast<int>(dataBuffer[i]);
        if(i != dataBuffer.size()-1)
            std::cout << ", ";
    }
    std::cout << std::dec << "]" << std::endl;
    
    EXPECT_NO_THROW({
        WebVttDataPacket packet(channelId, counter, timeOffsetMs, std::move(dataBuffer));
    });
    std::cout << "WebVttDataPacket object constructed successfully with maximum counter." << std::endl;
    std::cout << "Exiting ConstructWithMaxCounter test" << std::endl;
}
/**
 * @brief Test construction of WebVttDataPacket with maximum timeOffsetMs value
 *
 * This test verifies that the WebVttDataPacket constructor can handle an extreme valid scenario by using INT64_MAX for the timeOffsetMs parameter without throwing an exception.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 023@n
 * **Priority:** (High) 
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data |Expected Result |Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Initialize test variables including channelId, counter, timeOffsetMs, and dataBuffer | channelId = 1, counter = 123, timeOffsetMs = INT64_MAX, dataBuffer = 0x01,0x02,0x03,0x04 | Variables are initialized successfully | Should be successful |
 * | 02 | Invoke the WebVttDataPacket constructor with the initialized variables | channelId = 1, counter = 123, timeOffsetMs = INT64_MAX, dataBuffer = 0x01,0x02,0x03,0x04 | Constructor completes without throwing an exception | Should Pass |
 * | 03 | Verify that the constructor's execution prints the appropriate success messages | Console output capturing the printed messages | Success messages are printed indicating proper object construction | Should be successful |
 */
TEST(WebVttDataPacket, ConstructWithMaxTimeOffsetMs) {
    std::cout << "Entering ConstructWithMaxTimeOffsetMs test" << std::endl;
    
    std::uint32_t channelId = 1;
    std::uint32_t counter = 123;
    std::int64_t timeOffsetMs = std::numeric_limits<std::int64_t>::max();
    std::vector<std::uint8_t> dataBuffer = {0x01, 0x02, 0x03, 0x04};
    
    std::cout << "Invoking WebVttDataPacket constructor with channelId: " << channelId 
              << ", counter: " << counter 
              << ", timeOffsetMs: " << timeOffsetMs << " (INT64_MAX)"
              << ", dataBuffer: [";
    for (size_t i = 0; i < dataBuffer.size(); i++) {
        std::cout << "0x" << std::hex << static_cast<int>(dataBuffer[i]);
        if(i != dataBuffer.size()-1)
            std::cout << ", ";
    }
    std::cout << std::dec << "]" << std::endl;
    
    EXPECT_NO_THROW({
        WebVttDataPacket packet(channelId, counter, timeOffsetMs, std::move(dataBuffer));
    });
    std::cout << "WebVttDataPacket object constructed successfully with maximum timeOffsetMs." << std::endl;    
    std::cout << "Exiting ConstructWithMaxTimeOffsetMs test" << std::endl;
}
/**
 * @brief Verify that WebVttDataPacket constructor handles negative time offset correctly
 *
 * This test ensures that the WebVttDataPacket constructor can correctly handle a negative value for timeOffsetMs without throwing any exceptions. Negative time offsets may be valid in certain scenarios, and the constructor must properly process the given inputs.
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
 * | Variation / Step | Description                                                                               | Test Data                                                        | Expected Result                                                    | Notes        |
 * | :--------------: | ----------------------------------------------------------------------------------------- | ---------------------------------------------------------------- | ------------------------------------------------------------------ | ------------ |
 * | 01               | Invoke WebVttDataPacket constructor with a negative timeOffsetMs and valid inputs           | channelId = 100, counter = 789, timeOffsetMs = -500, dataBuffer = 0x00,0xAB,0xCD | Constructor does not throw an exception and constructs the packet successfully | Should Pass  |
 */
TEST(WebVttDataPacket, ConstructWithNegativeTimeOffsetMs) {
    std::cout << "Entering ConstructWithNegativeTimeOffsetMs test" << std::endl;
    
    std::uint32_t channelId = 100;
    std::uint32_t counter = 789;
    std::int64_t timeOffsetMs = -500;
    std::vector<std::uint8_t> dataBuffer = {0x00, 0xAB, 0xCD};
    
    std::cout << "Invoking WebVttDataPacket constructor with channelId: " << channelId 
              << ", counter: " << counter 
              << ", timeOffsetMs: " << timeOffsetMs << " (negative)"
              << ", dataBuffer: [";
    for (size_t i = 0; i < dataBuffer.size(); i++) {
        std::cout << "0x" << std::hex << static_cast<int>(dataBuffer[i]);
        if(i != dataBuffer.size()-1)
            std::cout << ", ";
    }
    std::cout << std::dec << "]" << std::endl;
    
    EXPECT_NO_THROW({
        WebVttDataPacket packet(channelId, counter, timeOffsetMs, std::move(dataBuffer));
    });
    std::cout << "WebVttDataPacket object constructed successfully with negative timeOffsetMs." << std::endl;  
    std::cout << "Exiting ConstructWithNegativeTimeOffsetMs test" << std::endl;
}
/**
 * @brief Positive test to validate creation of WebVttSelectionPacket with normal valid values.
 *
 * This test verifies that the WebVttSelectionPacket constructor successfully creates an object when provided with valid channelId, counter, width, and height values, ensuring no exceptions are thrown.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 025
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoke WebVttSelectionPacket constructor with valid parameters | channelId = 10, counter = 20, width = 1920, height = 1080 | Object is created successfully without throwing an exception | Should Pass |
 */
TEST(WebVttSelectionPacket, PositiveTest_NormalValidValues) {
    std::cout << "Entering PositiveTest_NormalValidValues test" << std::endl;
    uint32_t channelId = 10;
    uint32_t counter = 20;
    uint32_t width = 1920;
    uint32_t height = 1080;
    std::cout << "Invoking WebVttSelectionPacket constructor with channelId=" << channelId 
              << ", counter=" << counter 
              << ", width=" << width 
              << ", height=" << height << std::endl;
    EXPECT_NO_THROW({
        WebVttSelectionPacket packet(channelId, counter, width, height);
    });
    std::cout << "WebVttSelectionPacket object created successfully with channelId=" << channelId 
                << ", counter=" << counter 
                << ", width=" << width 
                << ", height=" << height << std::endl;
    std::cout << "Exiting PositiveTest_NormalValidValues test" << std::endl;
}
/**
 * @brief Verify that the WebVttSelectionPacket constructor handles a zero channel ID correctly.
 *
 * This test verifies that passing a zero channel ID along with valid counter, width, and height values to the WebVttSelectionPacket constructor does not throw any exceptions. It ensures that the object is created successfully even when the channel ID is zero.
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
 * | Variation / Step | Description                                                                                  | Test Data                                     | Expected Result                                                               | Notes      |
 * | :--------------: | -------------------------------------------------------------------------------------------- | --------------------------------------------- | ----------------------------------------------------------------------------- | ---------- |
 * | 01               | Invoke the WebVttSelectionPacket constructor with channelId=0, counter=20, width=1920, height=1080 | channelId = 0, counter = 20, width = 1920, height = 1080 | Constructor does not throw an exception and the packet object is created successfully | Should Pass |
 */
TEST(WebVttSelectionPacket, ZeroChannelId) {
    std::cout << "Entering ZeroChannelId test" << std::endl;
    uint32_t channelId = 0;
    uint32_t counter = 20;
    uint32_t width = 1920;
    uint32_t height = 1080;
    std::cout << "Invoking WebVttSelectionPacket constructor with channelId=" << channelId 
              << ", counter=" << counter 
              << ", width=" << width 
              << ", height=" << height << std::endl;
    EXPECT_NO_THROW({
        WebVttSelectionPacket packet(channelId, counter, width, height);
    });
    std::cout << "WebVttSelectionPacket object created successfully with channelId=" << channelId 
                << ", counter=" << counter 
                << ", width=" << width 
                << ", height=" << height << std::endl;
    std::cout << "Exiting ZeroChannelId test" << std::endl;
}
/**
 * @brief Verify that WebVttSelectionPacket handles a zero counter properly.
 *
 * This test verifies that the WebVttSelectionPacket constructor can handle a scenario 
 * where the counter value is zero without throwing exceptions. It ensures that the object is 
 * created successfully with the provided input parameters.
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
 * | Variation / Step | Description                                                       | Test Data                                                      | Expected Result                                                                                                          | Notes               |
 * | :----:           | :---------------------------------------------------------------  | :------------------------------------------------------------- | :----------------------------------------------------------------------------------------------------------------------- | :------------------ |
 * | 01               | Initialize variables for testing the constructor                  | channelId = 10, counter = 0, width = 1920, height = 1080         | Variables are set with the expected test values                                                                          | Should be successful |
 * | 02               | Invoke WebVttSelectionPacket constructor with the zero counter value | input: channelId = 10, counter = 0, width = 1920, height = 1080, output: WebVttSelectionPacket object | Constructor completes without throwing exceptions as asserted by EXPECT_NO_THROW                                         | Should Pass         |
 */
TEST(WebVttSelectionPacket, ZeroCounter) {
    std::cout << "Entering ZeroCounter test" << std::endl;
    uint32_t channelId = 10;
    uint32_t counter = 0;
    uint32_t width = 1920;
    uint32_t height = 1080;
    std::cout << "Invoking WebVttSelectionPacket constructor with channelId=" << channelId 
              << ", counter=" << counter 
              << ", width=" << width 
              << ", height=" << height << std::endl;
    EXPECT_NO_THROW({
        WebVttSelectionPacket packet(channelId, counter, width, height);
    });
    std::cout << "WebVttSelectionPacket object created successfully with channelId=" << channelId 
                << ", counter=" << counter 
                << ", width=" << width 
                << ", height=" << height << std::endl;
    std::cout << "Exiting ZeroCounter test" << std::endl;
}
/**
 * @brief Validate WebVttSelectionPacket constructor with a zero width parameter.
 *
 * Validate that the WebVttSelectionPacket constructor can handle a zero width parameter while all other parameters are set to valid values. This test ensures that providing a width of zero does not throw an exception and that the object is created successfully.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 028@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                               | Test Data                         | Expected Result                                         | Notes       |
 * | :--------------: | ------------------------------------------------------------------------- | --------------------------------- | ------------------------------------------------------- | ----------- |
 * |       01         | Invoke WebVttSelectionPacket constructor with channelId=10, counter=20, width=0, height=1080 | channelId=10, counter=20, width=0, height=1080 | No exception thrown and object created successfully | Should Pass |
 */
TEST(WebVttSelectionPacket, ZeroWidth) {
    std::cout << "Entering ZeroWidth test" << std::endl;
    uint32_t channelId = 10;
    uint32_t counter = 20;
    uint32_t width = 0;
    uint32_t height = 1080;
    std::cout << "Invoking WebVttSelectionPacket constructor with channelId=" << channelId 
              << ", counter=" << counter 
              << ", width=" << width 
              << ", height=" << height << std::endl;
    EXPECT_NO_THROW({
        WebVttSelectionPacket packet(channelId, counter, width, height);
    });
    std::cout << "WebVttSelectionPacket object created successfully with channelId=" << channelId 
                << ", counter=" << counter 
                << ", width=" << width 
                << ", height=" << height << std::endl;
    std::cout << "Exiting ZeroWidth test" << std::endl;
}
/**
 * @brief Verify that the WebVttSelectionPacket constructor handles a zero height without throwing an exception.
 *
 * This test verifies that when the WebVttSelectionPacket constructor is called with a zero height value,
 * it does not throw an exception. This is important to ensure that the API gracefully handles edge cases involving zero dimensions.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 029
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                         | Test Data                                   | Expected Result                                                                     | Notes      |
 * | :--------------: | --------------------------------------------------------------------------------------------------- | ------------------------------------------- | ----------------------------------------------------------------------------------- | ---------- |
 * | 01               | Invoke the WebVttSelectionPacket constructor with specific parameter values to simulate a zero height scenario. | channelId = 10, counter = 20, width = 1920, height = 0 | The constructor should create a WebVttSelectionPacket object successfully without throwing an exception. | Should Pass |
 */
TEST(WebVttSelectionPacket, ZeroHeight) {
    std::cout << "Entering ZeroHeight test" << std::endl;
    uint32_t channelId = 10;
    uint32_t counter = 20;
    uint32_t width = 1920;
    uint32_t height = 0;
    std::cout << "Invoking WebVttSelectionPacket constructor with channelId=" << channelId 
              << ", counter=" << counter 
              << ", width=" << width 
              << ", height=" << height << std::endl;
    EXPECT_NO_THROW({
        WebVttSelectionPacket packet(channelId, counter, width, height);
    });
    std::cout << "WebVttSelectionPacket object created successfully with channelId=" << channelId 
                << ", counter=" << counter 
                << ", width=" << width 
                << ", height=" << height << std::endl;
    std::cout << "Exiting ZeroHeight test" << std::endl;
}
/**
 * @brief Verifies creation of WebVttSelectionPacket object using maximum channelId value.
 *
 * This test checks that the WebVttSelectionPacket constructor can handle the maximum unsigned 32-bit integer value for channelId along with valid counter, width, and height parameters without throwing an exception. It ensures the object is properly instantiated even with boundary channelId input.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 030
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoke WebVttSelectionPacket constructor with maximum channelId, counter, width, and height | channelId = numeric_limits<uint32_t>::max(), counter = 20, width = 1920, height = 1080 | Constructor does not throw and object is created successfully | Should Pass |
 */
TEST(WebVttSelectionPacket, MaxChannelId) {
    std::cout << "Entering MaxChannelId test" << std::endl;
    uint32_t channelId = std::numeric_limits<uint32_t>::max();
    uint32_t counter = 20;
    uint32_t width = 1920;
    uint32_t height = 1080;
    std::cout << "Invoking WebVttSelectionPacket constructor with channelId=" << channelId 
              << " (MAX), counter=" << counter 
              << ", width=" << width 
              << ", height=" << height << std::endl;
    EXPECT_NO_THROW({
        WebVttSelectionPacket packet(channelId, counter, width, height);
    });
    std::cout << "WebVttSelectionPacket object created successfully with channelId=" << channelId 
                << " (MAX), counter=" << counter 
                << ", width=" << width 
                << ", height=" << height << std::endl;
    std::cout << "Exiting MaxChannelId test" << std::endl;
}
/**
 * @brief Verifies that the WebVttSelectionPacket constructor correctly handles the maximum counter value.
 *
 * This test case checks that the WebVttSelectionPacket object can be instantiated without throwing exceptions even when provided with the maximum possible counter value (std::numeric_limits<uint32_t>::max()). This ensures robustness in handling extreme input boundaries.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 031@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoke the WebVttSelectionPacket constructor with channelId=10, counter=UINT32_MAX, width=1920, height=1080 | channelId = 10, counter = std::numeric_limits<uint32_t>::max(), width = 1920, height = 1080 | Constructor should not throw any exception | Should Pass |
 */
TEST(WebVttSelectionPacket, MaxCounter) {
    std::cout << "Entering MaxCounter test" << std::endl;
    uint32_t channelId = 10;
    uint32_t counter = std::numeric_limits<uint32_t>::max();
    uint32_t width = 1920;
    uint32_t height = 1080;
    std::cout << "Invoking WebVttSelectionPacket constructor with channelId=" << channelId 
              << ", counter=" << counter << " (MAX)"
              << ", width=" << width 
              << ", height=" << height << std::endl;
    EXPECT_NO_THROW({
        WebVttSelectionPacket packet(channelId, counter, width, height);
    });
    std::cout << "WebVttSelectionPacket object created successfully with channelId=" << channelId 
                << ", counter=" << counter << " (MAX)"
                << ", width=" << width 
                << ", height=" << height << std::endl;
    std::cout << "Exiting MaxCounter test" << std::endl;
}
/**
 * @brief Test to verify that the WebVttSelectionPacket constructor accepts the maximum width value without throwing an exception
 *
 * This test verifies that the WebVttSelectionPacket constructor does not throw any exceptions when invoked with the maximum valid width value (std::numeric_limits<uint32_t>::max()) along with normal channelId, counter, and height values. This is to ensure that the module can correctly handle edge case inputs.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 032
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoke WebVttSelectionPacket constructor with maximum width value and predefined channelId, counter, and height | channelId = 10, counter = 20, width = 4294967295, height = 1080 | Constructor does not throw any exception and a valid packet object is created | Should Pass |
 */
TEST(WebVttSelectionPacket, MaxWidth) {
    std::cout << "Entering MaxWidth test" << std::endl;
    uint32_t channelId = 10;
    uint32_t counter = 20;
    uint32_t width = std::numeric_limits<uint32_t>::max();
    uint32_t height = 1080;
    std::cout << "Invoking WebVttSelectionPacket constructor with channelId=" << channelId 
              << ", counter=" << counter 
              << ", width=" << width << " (MAX)"
              << ", height=" << height << std::endl;
    EXPECT_NO_THROW({
        WebVttSelectionPacket packet(channelId, counter, width, height);
    });
    std::cout << "WebVttSelectionPacket object created successfully with channelId=" << channelId 
                << ", counter=" << counter 
                << ", width=" << width << " (MAX)"
                << ", height=" << height << std::endl;
    std::cout << "Exiting MaxWidth test" << std::endl;
}
/**
 * @brief Verify that the WebVttSelectionPacket can be instantiated using the maximum height value.
 *
 * This test ensures that creating a WebVttSelectionPacket object with valid channelId, counter, width, and the maximum possible height does not throw an exception. It validates robust handling of large numeric inputs.
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
 * | Variation / Step | Description                                                                                      | Test Data                                                                | Expected Result                                                                           | Notes       |
 * | :--------------: | ------------------------------------------------------------------------------------------------ | ------------------------------------------------------------------------ | ----------------------------------------------------------------------------------------- | ----------- |
 * | 01               | Invoke the WebVttSelectionPacket constructor with parameters including maximum height value.    | channelId = 10, counter = 20, width = 1920, height = std::numeric_limits<uint32_t>::max() | Object is successfully created without any exception being thrown; EXPECT_NO_THROW check passes | Should Pass |
 */
TEST(WebVttSelectionPacket, MaxHeight) {
    std::cout << "Entering MaxHeight test" << std::endl;
    uint32_t channelId = 10;
    uint32_t counter = 20;
    uint32_t width = 1920;
    uint32_t height = std::numeric_limits<uint32_t>::max();
    std::cout << "Invoking WebVttSelectionPacket constructor with channelId=" << channelId 
              << ", counter=" << counter 
              << ", width=" << width 
              << ", height=" << height << " (MAX)" << std::endl;
    EXPECT_NO_THROW({
        WebVttSelectionPacket packet(channelId, counter, width, height);
    });
    std::cout << "WebVttSelectionPacket object created successfully with channelId=" << channelId 
                << ", counter=" << counter 
                << ", width=" << width 
                << ", height=" << height << " (MAX)" << std::endl;
    std::cout << "Exiting MaxHeight test" << std::endl;
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
