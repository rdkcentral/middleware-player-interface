
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
#include "SubtecPacket.hpp"

class PacketChild : public Packet {
public:
    using Packet::getBuffer;
    using Packet::append32;
    using Packet::append64;
    using Packet::appendType;
};

class PacketTest : public ::testing::Test {
protected:
    PacketChild* packet;
    void SetUp() override {
        packet = new PacketChild();
    }
    void TearDown() override {
        delete packet;
    }
};

#define NUMBER_OF_ATTRIBUTES 14
using attributesType = std::array<uint32_t, NUMBER_OF_ATTRIBUTES>;

// Test Case: Construct packet with channelId = 0
/**
 * @brief Test the construction of CCSetAttributePacket when channelId is zero
 *
 * This test verifies that the CCSetAttributePacket constructor properly handles a channelId of zero 
 * while being provided with valid counter, ccType, attribType and a valid set of attribute values. 
 * The objective is to ensure that the object is created without throwing any exceptions, given proper inputs.
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
 * | 01 | Invoke CCSetAttributePacket constructor with channelId=0, counter=1, ccType=2, attribType=3, and attributesValues populated with multiples of 100 (i.e., attributesValues[i] = 100*(i+1) for i in range of NUMBER_OF_ATTRIBUTES) | channelId = 0, counter = 1, ccType = 2, attribType = 3, attributesValues[i] = 100*(i+1) (for i = 0 to NUMBER_OF_ATTRIBUTES-1) | CCSetAttributePacket should be constructed without throwing any exception | Should Pass |
 */
TEST(CCSetAttributePacket, ConstructPacketChannelIdZero) {
    std::cout << "Entering ConstructPacketChannelIdZero test" << std::endl;
    
    uint32_t channelId = 0;
    uint32_t counter = 1;
    uint32_t ccType = 2;
    uint32_t attribType = 3;
    attributesType attributesValues;
    // Fill attributesValues with values 100, 200, ..., 1400
    for (size_t i = 0; i < NUMBER_OF_ATTRIBUTES; ++i) {
        attributesValues[i] = static_cast<uint32_t>(100 * (i + 1));
    }
    std::cout << "Invoking CCSetAttributePacket constructor with channelId = " << channelId 
              << ", counter = " << counter 
              << ", ccType = " << ccType 
              << ", attribType = " << attribType << std::endl;
              
    for (size_t i = 0; i < NUMBER_OF_ATTRIBUTES; ++i) {
        std::cout << "attributesValues[" << i << "] = " << attributesValues[i] << std::endl;
    }
    
    EXPECT_NO_THROW({
        CCSetAttributePacket packet(channelId, counter, ccType, attribType, attributesValues);
        std::cout << "CCSetAttributePacket object constructed successfully." << std::endl;
    });
    
    std::cout << "Exiting ConstructPacketChannelIdZero test" << std::endl;
}
/**
 * @brief Verify that CCSetAttributePacket is constructed successfully when counter is zero.
 *
 * This test ensures that the CCSetAttributePacket constructor does not throw an exception when invoked with a counter value of zero.
 * The test validates that the object is constructed correctly using a valid set of input parameters including a set of attribute values.
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
 * | 01 | Invoke CCSetAttributePacket constructor with channelId=10, counter=0, ccType=2, attribType=3 and attributesValues populated with multiples of 100 | channelId = 10, counter = 0, ccType = 2, attribType = 3, attributesValues = 100,200,300,400,500,600,700,800,900,1000,1100,1200,1300,1400 | Constructor should execute without throwing any exception and construct the object successfully | Should Pass |
 */
TEST(CCSetAttributePacket, ConstructPacketCounterZero) {
    std::cout << "Entering ConstructPacketCounterZero test" << std::endl;
    
    uint32_t channelId = 10;
    uint32_t counter = 0;
    uint32_t ccType = 2;
    uint32_t attribType = 3;
    attributesType attributesValues;
    // Fill attributesValues with values 100, 200, ..., 1400
    for (size_t i = 0; i < NUMBER_OF_ATTRIBUTES; ++i) {
        attributesValues[i] = static_cast<uint32_t>(100 * (i + 1));
    }
    std::cout << "Invoking CCSetAttributePacket constructor with channelId = " << channelId 
              << ", counter = " << counter 
              << ", ccType = " << ccType 
              << ", attribType = " << attribType << std::endl;
    for (size_t i = 0; i < NUMBER_OF_ATTRIBUTES; ++i) {
        std::cout << "attributesValues[" << i << "] = " << attributesValues[i] << std::endl;
    }
              
    EXPECT_NO_THROW({
        CCSetAttributePacket packet(channelId, counter, ccType, attribType, attributesValues);
        std::cout << "CCSetAttributePacket object constructed successfully." << std::endl;
    });
    
    std::cout << "Exiting ConstructPacketCounterZero test" << std::endl;
}
/**
 * @brief Verify CCSetAttributePacket construction with ccType zero
 *
 * This test verifies that the CCSetAttributePacket constructor does not throw any exception when invoked with ccType set to 0. It checks the correct construction by passing a specific set of input values including channelId, counter, ccType, attribType, and an array of attribute values.
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
 * | 01 | Invoke CCSetAttributePacket constructor with predefined input values. | channelId = 10, counter = 1, ccType = 0, attribType = 3, attributesValues = 100,200,300,400,500,600,700,800,900,1000,1100,1200,1300,1400 | Constructor executes without throwing any exception. | Should Pass |
 */
TEST(CCSetAttributePacket, ConstructPacketCcTypeZero) {
    std::cout << "Entering ConstructPacketCcTypeZero test" << std::endl;
    
    uint32_t channelId = 10;
    uint32_t counter = 1;
    uint32_t ccType = 0;
    uint32_t attribType = 3;
    attributesType attributesValues;
    // Fill attributesValues with values 100, 200, ..., 1400
    for (size_t i = 0; i < NUMBER_OF_ATTRIBUTES; ++i) {
        attributesValues[i] = static_cast<uint32_t>(100 * (i + 1));
    }
    std::cout << "Invoking CCSetAttributePacket constructor with channelId = " << channelId 
              << ", counter = " << counter 
              << ", ccType = " << ccType 
              << ", attribType = " << attribType << std::endl;
              
    for (size_t i = 0; i < NUMBER_OF_ATTRIBUTES; ++i) {
        std::cout << "attributesValues[" << i << "] = " << attributesValues[i] << std::endl;
    }
              
    EXPECT_NO_THROW({
        CCSetAttributePacket packet(channelId, counter, ccType, attribType, attributesValues);
        std::cout << "CCSetAttributePacket object constructed successfully." << std::endl;
    });
    
    std::cout << "Exiting ConstructPacketCcTypeZero test" << std::endl;
}
/**
 * @brief Verifies construction of CCSetAttributePacket when attribType is zero.
 *
 * This test ensures that the CCSetAttributePacket constructor correctly initializes the packet without throwing an exception when the attribType parameter is zero. It verifies that all input parameters, including a set of attribute values, are properly handled.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Initialize test variables and populate attributesValues with multiples of 100. | channelId = 10, counter = 1, ccType = 2, attribType = 0, attributesValues = {100, 200, 300, ..., 1400} | Variables are correctly initialized before object construction. | Should be successful |
 * | 02 | Log the initialization values to the console. | N/A | Console outputs display the initialized values including each entry of attributesValues. | Should be successful |
 * | 03 | Invoke the CCSetAttributePacket constructor inside EXPECT_NO_THROW and verify no exceptions are thrown. | Constructor arguments: channelId, counter, ccType, attribType, attributesValues | CCSetAttributePacket object is constructed successfully without throwing any exceptions. | Should Pass |
 */
TEST(CCSetAttributePacket, ConstructPacketAttribTypeZero) {
    std::cout << "Entering ConstructPacketAttribTypeZero test" << std::endl;
    
    uint32_t channelId = 10;
    uint32_t counter = 1;
    uint32_t ccType = 2;
    uint32_t attribType = 0;
    attributesType attributesValues;
    // Fill attributesValues with values 100, 200, ..., 1400
    for (size_t i = 0; i < NUMBER_OF_ATTRIBUTES; ++i) {
        attributesValues[i] = static_cast<uint32_t>(100 * (i + 1));
    }
    
    std::cout << "Invoking CCSetAttributePacket constructor with channelId = " << channelId 
              << ", counter = " << counter 
              << ", ccType = " << ccType 
              << ", attribType = " << attribType << std::endl;
              
    for (size_t i = 0; i < NUMBER_OF_ATTRIBUTES; ++i) {
        std::cout << "attributesValues[" << i << "] = " << attributesValues[i] << std::endl;
    }
              
    EXPECT_NO_THROW({
        CCSetAttributePacket packet(channelId, counter, ccType, attribType, attributesValues);
        std::cout << "CCSetAttributePacket object constructed successfully." << std::endl;
    });
    
    std::cout << "Exiting ConstructPacketAttribTypeZero test" << std::endl;
}
/**
 * @brief Validate that the CCSetAttributePacket constructor successfully creates an object when provided with zero initialized attributes.
 *
 * This test verifies that the CCSetAttributePacket can be constructed without throwing any exceptions when the input attributes array is zero initialized. It also checks that the printed values correspond to the provided inputs.
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
 * | Variation / Step | Description                                                                                                                    | Test Data                                                                            | Expected Result                                                            | Notes       |
 * | :--------------: | ------------------------------------------------------------------------------------------------------------------------------ | ------------------------------------------------------------------------------------ | -------------------------------------------------------------------------- | ----------- |
 * | 01               | Invoke the CCSetAttributePacket constructor with channelId = 10, counter = 1, ccType = 2, attribType = 3, and an array of zero initialized attributes, and verify that no exception is thrown during object construction. | channelId = 10, counter = 1, ccType = 2, attribType = 3, attributesValues[i] = 0 (for all i) | CCSetAttributePacket object is successfully constructed without any exceptions | Should Pass |
 */
TEST(CCSetAttributePacket, ConstructPacketZeroInitializedAttributes) {
    std::cout << "Entering ConstructPacketZeroInitializedAttributes test" << std::endl;
    
    uint32_t channelId = 10;
    uint32_t counter = 1;
    uint32_t ccType = 2;
    uint32_t attribType = 3;
    attributesType attributesValues;
    // Zero initialize all elements
    for (size_t i = 0; i < NUMBER_OF_ATTRIBUTES; ++i) {
        attributesValues[i] = 0;
    }
    
    std::cout << "Invoking CCSetAttributePacket constructor with channelId = " << channelId 
              << ", counter = " << counter 
              << ", ccType = " << ccType 
              << ", attribType = " << attribType << std::endl;
    for (size_t i = 0; i < NUMBER_OF_ATTRIBUTES; ++i) {
        std::cout << "attributesValues[" << i << "] = " << attributesValues[i] << std::endl;
    }
              
    EXPECT_NO_THROW({
        CCSetAttributePacket packet(channelId, counter, ccType, attribType, attributesValues);
        std::cout << "CCSetAttributePacket object constructed successfully." << std::endl;
    });
    
    std::cout << "Exiting ConstructPacketZeroInitializedAttributes test" << std::endl;
}
/**
 * @brief Validate that the CCSetAttributePacket constructor handles typical mid-range parameter values correctly.
 *
 * This test verifies that the constructor of CCSetAttributePacket successfully constructs a packet using mid-range values for channelId, counter, ccType, attribType, and attributesValues. It ensures that the object instantiation is exception safe by confirming that no exceptions are thrown during the constructor call.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Set up typical mid-range values and invoke the CCSetAttributePacket constructor | channelId = 10, counter = 1, ccType = 2, attribType = 3, attributesValues = {100,200,...,1400} | Constructor successfully creates the object without throwing any exception; EXPECT_NO_THROW passes | Should Pass |
 */
TEST(CCSetAttributePacket, ConstructPacketTypicalMidRangeValues) {
    std::cout << "Entering ConstructPacketTypicalMidRangeValues test" << std::endl;
    
    uint32_t channelId = 10;
    uint32_t counter = 1;
    uint32_t ccType = 2;
    uint32_t attribType = 3;
    attributesType attributesValues;
    // Fill attributesValues with typical mid-range values: 100, 200, ..., 1400
    for (size_t i = 0; i < NUMBER_OF_ATTRIBUTES; ++i) {
        attributesValues[i] = static_cast<uint32_t>(100 * (i + 1));
    }
    
    std::cout << "Invoking CCSetAttributePacket constructor with channelId = " << channelId 
              << ", counter = " << counter 
              << ", ccType = " << ccType 
              << ", attribType = " << attribType << std::endl;
              
    for (size_t i = 0; i < NUMBER_OF_ATTRIBUTES; ++i) {
        std::cout << "attributesValues[" << i << "] = " << attributesValues[i] << std::endl;
    }
              
    EXPECT_NO_THROW({
        CCSetAttributePacket packet(channelId, counter, ccType, attribType, attributesValues);
        std::cout << "CCSetAttributePacket object constructed successfully." << std::endl;
    });
    
    std::cout << "Exiting ConstructPacketTypicalMidRangeValues test" << std::endl;
}
/**
 * @brief Test construction of CCSetAttributePacket with maximum channelId value
 *
 * This test verifies that the CCSetAttributePacket constructor can successfully create an object when the channelId is set to UINT32_MAX and valid attribute values are provided. The test ensures that no exceptions are thrown during object construction, confirming that the constructor correctly handles the extreme channelId value.
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
 * | Variation / Step | Description                                                                                                                    | Test Data                                                                                   | Expected Result                                                                                           | Notes      |
 * | :--------------: | ------------------------------------------------------------------------------------------------------------------------------ | ------------------------------------------------------------------------------------------- | --------------------------------------------------------------------------------------------------------- | ---------- |
 * |       01       | Invoke CCSetAttributePacket constructor with channelId set to UINT32_MAX, counter = 1, ccType = 2, attribType = 3, and attributesValues filled with 100, 200, ..., 1400 | channelId = UINT32_MAX, counter = 1, ccType = 2, attribType = 3, attributesValues = [100,200,...,1400] | CCSetAttributePacket object is constructed without any exceptions (EXPECT_NO_THROW passes) | Should Pass |
 */
TEST(CCSetAttributePacket, ConstructPacketChannelIdMax) {
    std::cout << "Entering ConstructPacketChannelIdMax test" << std::endl;
    
    uint32_t channelId = std::numeric_limits<uint32_t>::max();
    uint32_t counter = 1;
    uint32_t ccType = 2;
    uint32_t attribType = 3;
    attributesType attributesValues;
    // Fill attributesValues with values 100, 200, ..., 1400
    for (size_t i = 0; i < NUMBER_OF_ATTRIBUTES; ++i) {
        attributesValues[i] = static_cast<uint32_t>(100 * (i + 1));
    }
    
    std::cout << "Invoking CCSetAttributePacket constructor with channelId = " << channelId 
              << " (UINT32_MAX), counter = " << counter 
              << ", ccType = " << ccType 
              << ", attribType = " << attribType << std::endl;
              
    for (size_t i = 0; i < NUMBER_OF_ATTRIBUTES; ++i) {
        std::cout << "attributesValues[" << i << "] = " << attributesValues[i] << std::endl;
    }
              
    EXPECT_NO_THROW({
        CCSetAttributePacket packet(channelId, counter, ccType, attribType, attributesValues);
        std::cout << "CCSetAttributePacket object constructed successfully." << std::endl;
    });
    
    std::cout << "Exiting ConstructPacketChannelIdMax test" << std::endl;
}
/**
 * @brief Validates that the CCSetAttributePacket constructor properly handles the maximum counter value.
 *
 * This test verifies that when the CCSetAttributePacket constructor is invoked with a counter value of UINT32_MAX,
 * it creates an object successfully without throwing any exceptions. In addition, the test confirms that all attribute values are correctly set.
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
 * | 01 | Invoke the CCSetAttributePacket constructor with channelId=10, counter=UINT32_MAX, ccType=2, attribType=3, and attributesValues filled with values from 100 to 1400. | channelId = 10, counter = UINT32_MAX, ccType = 2, attribType = 3, attributesValues = [100,200,300,...,1400] | Constructor should not throw any exception; packet object should be constructed successfully. | Should Pass |
 */
TEST(CCSetAttributePacket, ConstructPacketCounterMax) {
    std::cout << "Entering ConstructPacketCounterMax test" << std::endl;
    
    uint32_t channelId = 10;
    uint32_t counter = std::numeric_limits<uint32_t>::max();
    uint32_t ccType = 2;
    uint32_t attribType = 3;
    attributesType attributesValues;
    // Fill attributesValues with values 100, 200, ..., 1400
    for (size_t i = 0; i < NUMBER_OF_ATTRIBUTES; ++i) {
        attributesValues[i] = static_cast<uint32_t>(100 * (i + 1));
    }
    
    std::cout << "Invoking CCSetAttributePacket constructor with channelId = " << channelId 
              << ", counter = " << counter << " (UINT32_MAX)"
              << ", ccType = " << ccType 
              << ", attribType = " << attribType << std::endl;
              
    for (size_t i = 0; i < NUMBER_OF_ATTRIBUTES; ++i) {
        std::cout << "attributesValues[" << i << "] = " << attributesValues[i] << std::endl;
    }
              
    EXPECT_NO_THROW({
        CCSetAttributePacket packet(channelId, counter, ccType, attribType, attributesValues);
        std::cout << "CCSetAttributePacket object constructed successfully." << std::endl;
    });
    
    std::cout << "Exiting ConstructPacketCounterMax test" << std::endl;
}
/**
 * @brief Test the construction of a CCSetAttributePacket with maximum CC type value.
 *
 * This test verifies that a CCSetAttributePacket object is successfully constructed when provided with a maximum ccType value (UINT32_MAX) along with valid channelId, counter, attribType, and attribute values. The test ensures that no exceptions are thrown during the construction process.
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
 * | 01 | Initialize test parameters including channelId, counter, ccType (UINT32_MAX), attribType, and attributesValues populated with multiples of 100. | channelId = 10, counter = 1, ccType = UINT32_MAX, attribType = 3, attributesValues[i] = 100*(i+1) for i in [0, NUMBER_OF_ATTRIBUTES-1] | Test parameters are correctly initialized. | Should be successful |
 * | 02 | Invoke the CCSetAttributePacket constructor with the initialized parameters and verify that no exception is thrown. | channelId = 10, counter = 1, ccType = UINT32_MAX, attribType = 3, attributesValues filled with values from 100 to 1400 | Object constructed without exceptions (EXPECT_NO_THROW passes). | Should Pass |
 */
TEST(CCSetAttributePacket, ConstructPacketCcTypeMax) {
    std::cout << "Entering ConstructPacketCcTypeMax test" << std::endl;
    
    uint32_t channelId = 10;
    uint32_t counter = 1;
    uint32_t ccType = std::numeric_limits<uint32_t>::max();
    uint32_t attribType = 3;
    attributesType attributesValues;
    // Fill attributesValues with values 100, 200, ..., 1400
    for (size_t i = 0; i < NUMBER_OF_ATTRIBUTES; ++i) {
        attributesValues[i] = static_cast<uint32_t>(100 * (i + 1));
    }
    
    std::cout << "Invoking CCSetAttributePacket constructor with channelId = " << channelId 
              << ", counter = " << counter 
              << ", ccType = " << ccType << " (UINT32_MAX)"
              << ", attribType = " << attribType << std::endl;
              
    for (size_t i = 0; i < NUMBER_OF_ATTRIBUTES; ++i) {
        std::cout << "attributesValues[" << i << "] = " << attributesValues[i] << std::endl;
    }
              
    EXPECT_NO_THROW({
        CCSetAttributePacket packet(channelId, counter, ccType, attribType, attributesValues);
        std::cout << "CCSetAttributePacket object constructed successfully." << std::endl;
    });
    
    std::cout << "Exiting ConstructPacketCcTypeMax test" << std::endl;
}
/**
 * @brief Test the construction of a CCSetAttributePacket with maximum attribute type value
 *
 * This test verifies that the CCSetAttributePacket constructor correctly handles the maximum possible value for attribType (UINT32_MAX) when provided with valid channelId, counter, ccType, and a valid array of attributesValues. It ensures that no exception is thrown during object instantiation.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 010@n
 * **Priority:** High@n
 * @n
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * @n
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |@n
 * | :--------------: | ----------- | --------- | --------------- | ----- |@n
 * | 01 | Prepare input parameters and invoke CCSetAttributePacket constructor with maximum attribType value and a set of attribute values | channelId = 10, counter = 1, ccType = 2, attribType = UINT32_MAX, attributesValues = [100, 200, 300, 400, 500, 600, 700, 800, 900, 1000, 1100, 1200, 1300, 1400] | Constructor completes without throwing an exception and the object is successfully created | Should Pass |
 */
TEST(CCSetAttributePacket, ConstructPacketAttribTypeMax) {
    std::cout << "Entering ConstructPacketAttribTypeMax test" << std::endl;
    
    uint32_t channelId = 10;
    uint32_t counter = 1;
    uint32_t ccType = 2;
    uint32_t attribType = std::numeric_limits<uint32_t>::max();
    attributesType attributesValues;
    // Fill attributesValues with values 100, 200, ..., 1400
    for (size_t i = 0; i < NUMBER_OF_ATTRIBUTES; ++i) {
        attributesValues[i] = static_cast<uint32_t>(100 * (i + 1));
    }
    
    std::cout << "Invoking CCSetAttributePacket constructor with channelId = " << channelId 
              << ", counter = " << counter 
              << ", ccType = " << ccType 
              << ", attribType = " << attribType << " (UINT32_MAX)" << std::endl;
              
    for (size_t i = 0; i < NUMBER_OF_ATTRIBUTES; ++i) {
        std::cout << "attributesValues[" << i << "] = " << attributesValues[i] << std::endl;
    }
              
    EXPECT_NO_THROW({
        CCSetAttributePacket packet(channelId, counter, ccType, attribType, attributesValues);
        std::cout << "CCSetAttributePacket object constructed successfully." << std::endl;
    });
    
    std::cout << "Exiting ConstructPacketAttribTypeMax test" << std::endl;
}
/**
 * @brief Validate that the DummyPacket default constructor initializes an object without throwing any exception.
 *
 * This test case verifies that invoking the default constructor of the DummyPacket class does not result in an exception.
 * The test ensures that the object is successfully created and demonstrates basic functionality.
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
 * | Variation / Step | Description                                                                                           | Test Data                                                              | Expected Result                                                        | Notes      |
 * | :--------------: | ----------------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------- | ---------------------------------------------------------------------- | ---------- |
 * | 01               | Invoke the DummyPacket default constructor within an EXPECT_NO_THROW block to ensure no exception is thrown. | API: DummyPacket(), input: none, output: dummyObj instance created       | DummyPacket object is created successfully without any exceptions.     | Should Pass |
 */
TEST(DummyPacket, PositiveDefaultConstructor) {
    std::cout << "Entering PositiveDefaultConstructor test" << std::endl;
    
    std::cout << "Invoking DummyPacket default constructor" << std::endl;
    EXPECT_NO_THROW({
        DummyPacket dummyObj;
        std::cout << "DummyPacket object successfully created with default constructor" << std::endl;
    });
    
    std::cout << "Exiting PositiveDefaultConstructor test" << std::endl;
}
/**
 * @brief Validate MutePacket object creation with minimal channel ID.
 *
 * This test case verifies that invoking the MutePacket constructor with a minimal channelId (0) 
 * and a valid counter (50) does not throw an exception, ensuring correct handling of lower boundary values.
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
 * | Variation / Step | Description                                                         | Test Data                         | Expected Result                                               | Notes      |
 * | :--------------: | ------------------------------------------------------------------- | --------------------------------- | ------------------------------------------------------------- | ---------- |
 * |      01        | Call the MutePacket constructor with minimal channelId and counter. | channelId = 0, counter = 50       | No exception thrown and MutePacket instance is successfully created. | Should Pass |
 */
TEST(MutePacket, CreateMutePacketWithMinimalChannelId) {
    std::cout << "Entering CreateMutePacketWithMinimalChannelId test" << std::endl;
    uint32_t channelId = 0;
    uint32_t counter = 50;
    std::cout << "Invoking MutePacket constructor with channelId: " << channelId 
              << ", counter: " << counter << std::endl;
    EXPECT_NO_THROW({
        MutePacket mutePacket(channelId, counter);
        std::cout << "MutePacket instance created successfully." << std::endl;
    });
    std::cout << "Exiting CreateMutePacketWithMinimalChannelId test" << std::endl;
}
/**
 * @brief Verify that creating a MutePacket object with minimal counter value does not throw exceptions
 *
 * This test validates that the MutePacket constructor works as expected when provided with a minimal counter value alongside a valid channelId. The test ensures that no exceptions are thrown during the creation of the MutePacket instance, thereby verifying proper initialization under these conditions.
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
 * | Variation / Step | Description                                                                                     | Test Data                                         | Expected Result                                             | Notes            |
 * | :--------------: | ----------------------------------------------------------------------------------------------- | ------------------------------------------------- | ----------------------------------------------------------- | ---------------- |
 * | 01               | Initialize test parameters and log the entry of the test with channelId=100, counter=0.           | channelId = 100, counter = 0                        | Log message "Entering CreateMutePacketWithMinimalCounter test" | Should be successful |
 * | 02               | Invoke MutePacket constructor with channelId and counter, and verify no exception is thrown.      | channelId = 100, counter = 0, API: MutePacket constructor | MutePacket instance is created without throwing an exception   | Should Pass      |
 * | 03               | Log the exit of the test execution.                                                             | None                                              | Log message "Exiting CreateMutePacketWithMinimalCounter test" | Should be successful |
 */
TEST(MutePacket, CreateMutePacketWithMinimalCounter) {
    std::cout << "Entering CreateMutePacketWithMinimalCounter test" << std::endl;
    uint32_t channelId = 100;
    uint32_t counter = 0;
    std::cout << "Invoking MutePacket constructor with channelId: " << channelId 
              << ", counter: " << counter << std::endl;
    EXPECT_NO_THROW({
        MutePacket mutePacket(channelId, counter);
        std::cout << "MutePacket instance created successfully." << std::endl;
    });
    std::cout << "Exiting CreateMutePacketWithMinimalCounter test" << std::endl;
}
/**
 * @brief Test the creation of a MutePacket object using the maximum possible channel ID.
 *
 * This test verifies that the MutePacket constructor successfully creates an instance 
 * when provided with the maximum channel ID value (4294967295) and a valid counter value (50). 
 * The test ensures that no exceptions are thrown during the creation process, which validates 
 * the robustness of the constructor with boundary input values.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 014
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                              | Test Data                                   | Expected Result                                                                                         | Notes      |
 * | :--------------: | -------------------------------------------------------------------------------------------------------- | ------------------------------------------- | ------------------------------------------------------------------------------------------------------- | ---------- |
 * | 01               | Invoke the MutePacket constructor with maximum channel ID (4294967295) and counter value (50)             | channelId = 4294967295, counter = 50          | MutePacket instance is created without throwing any exception                                          | Should Pass|
 */
TEST(MutePacket, CreateMutePacketWithMaximumChannelId) {
    std::cout << "Entering CreateMutePacketWithMaximumChannelId test" << std::endl;
    uint32_t channelId = 4294967295;
    uint32_t counter = 50;
    std::cout << "Invoking MutePacket constructor with channelId: " << channelId 
              << ", counter: " << counter << std::endl;
    EXPECT_NO_THROW({
        MutePacket mutePacket(channelId, counter);
        std::cout << "MutePacket instance created successfully." << std::endl;
    });
    std::cout << "Exiting CreateMutePacketWithMaximumChannelId test" << std::endl;
}
/**
 * @brief Validates that MutePacket object creation with a maximum counter value does not throw any exception.
 *
 * This test verifies that invoking the MutePacket constructor with a channel ID of 100 and the maximum unsigned 32-bit counter value (4294967295) does not result in any exceptions. This ensures that the API correctly handles boundary conditions for the counter parameter.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Setup test parameters with valid channelId and maximum counter value | channelId = 100, counter = 4294967295 | Test parameters are correctly set for API invocation | Should be successful |
 * | 02 | Invoke the MutePacket constructor and verify it does not throw an exception | Invocation: MutePacket(channelId, counter); output: mutePacket instance creation | No exception is thrown and the instance is created successfully | Should Pass |
 */
TEST(MutePacket, CreateMutePacketWithMaximumCounter) {
    std::cout << "Entering CreateMutePacketWithMaximumCounter test" << std::endl;
    uint32_t channelId = 100;
    uint32_t counter = 4294967295;
    std::cout << "Invoking MutePacket constructor with channelId: " << channelId 
              << ", counter: " << counter << std::endl;
    EXPECT_NO_THROW({
        MutePacket mutePacket(channelId, counter);
        std::cout << "MutePacket instance created successfully." << std::endl;
    });
    std::cout << "Exiting CreateMutePacketWithMaximumCounter test" << std::endl;
}
/**
 * @brief Validate correct instantiation of MutePacket by invoking its constructor with mid-range values.
 *
 * This test verifies that the MutePacket constructor successfully creates an instance when passed a mid-range channelId and counter value, ensuring that no exceptions are thrown during object creation.
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
 * | Variation / Step | Description                                                     | Test Data                                  | Expected Result                                          | Notes      |
 * | :--------------: | --------------------------------------------------------------- | ------------------------------------------ | -------------------------------------------------------- | ---------- |
 * | 01               | Invoke the MutePacket constructor with mid-range values         | input channelId = 100, input counter = 50  | No exception thrown; MutePacket instance is created      | Should Pass|
 */
TEST(MutePacket, CreateMutePacketWithMidRangeValues) {
    std::cout << "Entering CreateMutePacketWithMidRangeValues test" << std::endl;
    uint32_t channelId = 100;
    uint32_t counter = 50;
    std::cout << "Invoking MutePacket constructor with channelId: " << channelId 
              << ", counter: " << counter << std::endl;
    EXPECT_NO_THROW({
        MutePacket mutePacket(channelId, counter);
        std::cout << "MutePacket instance created successfully." << std::endl;
    });
    std::cout << "Exiting CreateMutePacketWithMidRangeValues test" << std::endl;
}
/**
 * @brief Verify that the default constructor of Packet initializes the object correctly without throwing an exception.
 *
 * This test case verifies that invoking the default constructor Packet::Packet() does not throw any exceptions, ensuring that the object is instantiated without errors. The test is crucial as it confirms the basic functionality of the default constructor which is essential for the reliable creation of Packet objects.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 017
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoke the default constructor Packet::Packet() within an EXPECT_NO_THROW block to ensure no exception occurs | input: none, output: Packet instance | Packet is constructed successfully without throwing any exception; assertion EXPECT_NO_THROW passes | Should Pass |
 */
TEST(Packet, VerifyDefaultConstructorInitializes) {
    std::cout << "Entering VerifyDefaultConstructorInitializes test" << std::endl;
    std::cout << "Invoking default constructor Packet::Packet() ..." << std::endl;
    EXPECT_NO_THROW({
        Packet packet;
        std::cout << "Default constructor invoked successfully." << std::endl;
    });
    std::cout << "Exiting VerifyDefaultConstructorInitializes test" << std::endl;
}
/**
 * @brief Test to validate Packet constructor with counter value zero.
 *
 * This test is designed to verify that the Packet class constructor properly handles a counter value of zero without throwing any exceptions. It ensures that the creation of a Packet instance is successful when the counter is set to zero.
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
 * | Variation / Step | Description                                             | Test Data         | Expected Result                                                                                    | Notes      |
 * | :--------------: | ------------------------------------------------------- | ----------------- | -------------------------------------------------------------------------------------------------- | ---------- |
 * | 01               | Call Packet constructor with counter value set to 0.    | counter = 0       | Packet instance is successfully created without throwing an exception, and no assertion fails.   | Should Pass|
 */
TEST(Packet, ConstructWithZero) {
    std::cout << "Entering ConstructWithZero test" << std::endl;
    uint32_t counter = 0;
    std::cout << "Preparing to invoke Packet constructor with counter value: " << counter << std::endl;
    EXPECT_NO_THROW({
        std::cout << "Invoking Packet constructor" << std::endl;
        Packet packet(counter);
        std::cout << "Packet instance created with counter value: " << counter << std::endl;
    });
    std::cout << "Exiting ConstructWithZero test" << std::endl;
}
/**
 * @brief Tests Packet constructor with maximum counter value
 *
 * This test verifies that the Packet constructor can handle the maximum possible uint32_t counter value without throwing exceptions, ensuring robustness in handling edge cases.
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
 * | Variation / Step | Description                                                   | Test Data                        | Expected Result                                            | Notes     |
 * | :--------------: | ------------------------------------------------------------- | -------------------------------- | ---------------------------------------------------------- | --------- |
 * | 01               | Invoke Packet constructor with maximum counter value UINT32_MAX | counter = UINT32_MAX             | Constructor does not throw exception and Packet instance is created | Should Pass |
 */
TEST(Packet, ConstructWithMaxCounter) {
    std::cout << "Entering ConstructWithMaxCounter test" << std::endl;
    uint32_t counter = UINT32_MAX;
    std::cout << "Preparing to invoke Packet constructor with counter value: " << counter << std::endl;
    EXPECT_NO_THROW({
        std::cout << "Invoking Packet constructor" << std::endl;
        Packet packet(counter);
        std::cout << "Packet instance created with counter value: " << counter << std::endl;
    });
    std::cout << "Exiting ConstructWithMaxCounter test" << std::endl;
}
/**
 * @brief Test Packet constructor with a typical counter value.
 *
 * This test verifies that the Packet constructor can handle a typical input (counter = 12345)
 * without throwing exceptions. It ensures that object instantiation functions correctly under normal conditions.
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
 * | 01 | Call Packet constructor with a typical counter value and verify no exception is thrown. | input: counter = 12345 | Packet instance creation succeeds without exceptions. | Should Pass |
 */
TEST(Packet, ConstructWithTypicalValue) {
    std::cout << "Entering ConstructWithTypicalValue test" << std::endl;
    uint32_t counter = 12345;
    std::cout << "Preparing to invoke Packet constructor with counter value: " << counter << std::endl;
    EXPECT_NO_THROW({
        std::cout << "Invoking Packet constructor" << std::endl;
        Packet packet(counter);
        std::cout << "Packet instance created with counter value: " << counter << std::endl;
    });
    std::cout << "Exiting ConstructWithTypicalValue test" << std::endl;
}
/**
 * @brief This test verifies that Packet's getBytes() method returns a valid vector of bytes without throwing exceptions.
 *
 * This test creates a Packet object using the default constructor and then invokes the getBytes() method on it.
 * It checks that no exceptions are thrown during object creation and method invocation, and validates that the returned
 * vector has a non-negative size.
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
 * | 01 | Invoke the default constructor to create a Packet object. | No input, output: Packet object created | Packet object is created successfully without throwing exceptions. | Should Pass |
 * | 02 | Call the getBytes() method on the Packet object. | input: Packet object; output: vector<uint8_t> returned | getBytes() returns a valid vector of bytes. | Should Pass |
 * | 03 | Verify that the size of the returned vector is non-negative. | input: size = bytes.size() | Assertion check passes with size >= 0. | Should Pass |
 */
TEST(Packet, VerifyGetBytesWorksProperly) {
    std::cout << "Entering VerifyGetBytesWorksProperly test" << std::endl;

    // Creating Packet object using default constructor
    EXPECT_NO_THROW({
        Packet packet;
        std::cout << "Packet object created successfully using default constructor." << std::endl;

        // Invoking getBytes() method
        std::cout << "Invoking getBytes() method on Packet object." << std::endl;
        const std::vector<uint8_t>& bytes = packet.getBytes();
        std::cout << "getBytes() method invoked successfully." << std::endl;

        // Logging the size of the returned vector
        size_t size = bytes.size();
        std::cout << "Retrieved bytes vector size: " << size << std::endl;

        // Checking that the returned vector size is >= 0
        EXPECT_GE(size, 0);
    });

    std::cout << "Exiting VerifyGetBytesWorksProperly test" << std::endl;
}
/**
 * @brief Tests that the default counter of a Packet object is 0 upon creation
 *
 * This test creates a Packet object using its default constructor and verifies that the getCounter() method returns the default counter value 0.
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
 * | Variation / Step | Description                                                      | Test Data                                                    | Expected Result                                          | Notes            |
 * | :--------------: | ---------------------------------------------------------------- | ------------------------------------------------------------ | -------------------------------------------------------- | ---------------- |
 * | 01               | Invoke the default constructor to create a Packet object         | No input; output: Packet instance created                    | Packet object instantiated without errors             | Should Pass      |
 * | 02               | Call the getCounter() method on the Packet object                | Invocation: pkt.getCounter(), (output1 = counter, expected value = 0) | Returned counter value equals 0u                       | Should Pass      |
 * | 03               | Verify that the retrieved counter value equals the expected value 0 | Variable: counter = value obtained from getCounter() (counter = 0) | Assertion passes confirming counter equals 0u          | Should be successful |
 */
TEST(Packet, RetrieveDefaultPacketCounter) {
    std::cout << "Entering RetrieveDefaultPacketCounter test" << std::endl;
    
    EXPECT_NO_THROW({
        Packet pkt;
        std::cout << "Invoked default constructor of Packet object." << std::endl;
        
        std::cout << "Invoking getCounter() method on Packet object." << std::endl;
        std::uint32_t counter = pkt.getCounter();
        std::cout << "Retrieved counter value: " << counter << std::endl;
        
        // Verifying the default counter value. Expected default value is 0.
        EXPECT_EQ(counter, 0u);
        std::cout << "Assertion passed: Retrieved counter value equals expected default value (0u)." << std::endl;
    });
    
    std::cout << "Exiting RetrieveDefaultPacketCounter test" << std::endl;
}
/**
 * @brief Verify that the getType() method returns a non-negative type value.
 *
 * This test verifies that a Packet object created using the default constructor successfully invokes the getType() method and returns a valid type value that is greater than or equal to 0. The goal is to ensure that both object construction and API invocation work correctly under normal conditions.
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
 * | Variation / Step | Description                                                                 | Test Data                                                          | Expected Result                                        | Notes          |
 * | :--------------: | --------------------------------------------------------------------------- | ------------------------------------------------------------------ | ------------------------------------------------------ | -------------- |
 * | 01               | Create a Packet object using the default constructor.                       | Invocation: Packet()                                               | Packet object is created without throwing an exception.| Should Pass    |
 * | 02               | Invoke the getType() method on the Packet object to retrieve its type value.  | Invocation: getType(), output: type (value returned by getType())    | getType() returns a valid unsigned integer value.      | Should Pass    |
 * | 03               | Validate that the returned type value is greater than or equal to 0.          | Input: type = returned value from getType()                          | The returned type value satisfies EXPECT_GE(type, 0u).   | Should be successful |
 */
TEST(Packet, VerifyGetTypeWorksProperly) {
    std::cout << "Entering VerifyGetTypeWorksProperly test" << std::endl;

    // Create a Packet object using the default constructor.
    EXPECT_NO_THROW({
        Packet packet;
        std::cout << "Packet object created using default constructor." << std::endl;
        
        // Invoke getType() and log the invocation.
        std::cout << "Invoking getType() method on Packet object." << std::endl;
        uint32_t type = packet.getType();
        std::cout << "getType() returned value: " << type << std::endl;
        
        // Validate that the return value is >= 0.
        std::cout << "Verifying that the returned type value (" << type << ") is >= 0." << std::endl;
        EXPECT_GE(type, 0u);
    });

    std::cout << "Exiting VerifyGetTypeWorksProperly test" << std::endl;
}
/**
 * @brief Validate that Packet::getTypeString returns "UNKNOWN" for an undefined packet type
 *
 * Validate that when an undefined packet type (9999) is provided to Packet::getTypeString, the API does not throw an exception and returns "UNKNOWN". This test ensures proper handling of unrecognized packet types.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 024
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                              | Test Data                         | Expected Result                                              | Notes         |
 * | :--------------: | -------------------------------------------------------- | --------------------------------- | ------------------------------------------------------------ | ------------- |
 * | 01               | Instantiate Packet object using default constructor      | None                              | Packet object created successfully                          | Should be successful |
 * | 02               | Invoke Packet::getTypeString with undefined packet type    | testValue = 9999                  | API returns the string "UNKNOWN" with no exception thrown     | Should Pass   |
 * | 03               | Verify the output of getTypeString call                  | output: result variable holds returned value | result equals "UNKNOWN"                                    | Should Pass   |
 */
TEST(Packet, UndefinedPacketType) {
    std::cout << "Entering UndefinedPacketType test" << std::endl;

    // Create Packet object using default constructor (even though getTypeString is static)
    Packet packet;
    
    uint32_t testValue = 9999;
    std::cout << "Invoking Packet::getTypeString with input: " << testValue << std::endl;
    
    std::string result;
    // Ensure no exception is thrown during invocation
    EXPECT_NO_THROW(result = Packet::getTypeString(testValue));
    
    std::cout << "Returned value: " << result << std::endl;
    EXPECT_EQ(result, "UNKNOWN");

    std::cout << "Exiting UndefinedPacketType test" << std::endl;
}
/**
 * @brief Validate that Packet::getTypeString correctly maps PacketType values to their string representations.
 *
 * This test verifies that for each PacketType enum value, the Packet::getTypeString API returns the expected string value.
 * It ensures that the enum to string mapping remains consistent and that no exceptions are thrown when invoking the method.
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
 * | 01 | Construct a Packet object and initialize the type mapping container | No input | Packet object is created and mapping container is initialized with 18 {PacketType, ExpectedString} pairs | Should be successful |
 * | 02 | Invoke getTypeString for PacketType::PES_DATA | input1 = typeValue = value of PacketType::PES_DATA, expectedString = PES_DATA | Returns "PES_DATA" without throwing exception | Should Pass |
 * | 03 | Invoke getTypeString for PacketType::TIMESTAMP | input1 = typeValue = value of PacketType::TIMESTAMP, expectedString = TIMESTAMP | Returns "TIMESTAMP" without throwing exception | Should Pass |
 * | 04 | Invoke getTypeString for PacketType::RESET_ALL | input1 = typeValue = value of PacketType::RESET_ALL, expectedString = RESET_ALL | Returns "RESET_ALL" without throwing exception | Should Pass |
 * | 05 | Invoke getTypeString for PacketType::RESET_CHANNEL | input1 = typeValue = value of PacketType::RESET_CHANNEL, expectedString = RESET_CHANNEL | Returns "RESET_CHANNEL" without throwing exception | Should Pass |
 * | 06 | Invoke getTypeString for PacketType::SUBTITLE_SELECTION | input1 = typeValue = value of PacketType::SUBTITLE_SELECTION, expectedString = SUBTITLE_SELECTION | Returns "SUBTITLE_SELECTION" without throwing exception | Should Pass |
 * | 07 | Invoke getTypeString for PacketType::TELETEXT_SELECTION | input1 = typeValue = value of PacketType::TELETEXT_SELECTION, expectedString = TELETEXT_SELECTION | Returns "TELETEXT_SELECTION" without throwing exception | Should Pass |
 * | 08 | Invoke getTypeString for PacketType::TTML_SELECTION | input1 = typeValue = value of PacketType::TTML_SELECTION, expectedString = TTML_SELECTION | Returns "TTML_SELECTION" without throwing exception | Should Pass |
 * | 09 | Invoke getTypeString for PacketType::TTML_DATA | input1 = typeValue = value of PacketType::TTML_DATA, expectedString = TTML_DATA | Returns "TTML_DATA" without throwing exception | Should Pass |
 * | 10 | Invoke getTypeString for PacketType::TTML_TIMESTAMP | input1 = typeValue = value of PacketType::TTML_TIMESTAMP, expectedString = TTML_TIMESTAMP | Returns "TTML_TIMESTAMP" without throwing exception | Should Pass |
 * | 11 | Invoke getTypeString for PacketType::WEBVTT_SELECTION | input1 = typeValue = value of PacketType::WEBVTT_SELECTION, expectedString = WEBVTT_SELECTION | Returns "WEBVTT_SELECTION" without throwing exception | Should Pass |
 * | 12 | Invoke getTypeString for PacketType::WEBVTT_DATA | input1 = typeValue = value of PacketType::WEBVTT_DATA, expectedString = WEBVTT_DATA | Returns "WEBVTT_DATA" without throwing exception | Should Pass |
 * | 13 | Invoke getTypeString for PacketType::WEBVTT_TIMESTAMP | input1 = typeValue = value of PacketType::WEBVTT_TIMESTAMP, expectedString = WEBVTT_TIMESTAMP | Returns "WEBVTT_TIMESTAMP" without throwing exception | Should Pass |
 * | 14 | Invoke getTypeString for PacketType::CC_DATA | input1 = typeValue = value of PacketType::CC_DATA, expectedString = CC_DATA | Returns "CC_DATA" without throwing exception | Should Pass |
 * | 15 | Invoke getTypeString for PacketType::PAUSE | input1 = typeValue = value of PacketType::PAUSE, expectedString = PAUSE | Returns "PAUSE" without throwing exception | Should Pass |
 * | 16 | Invoke getTypeString for PacketType::RESUME | input1 = typeValue = value of PacketType::RESUME, expectedString = RESUME | Returns "RESUME" without throwing exception | Should Pass |
 * | 17 | Invoke getTypeString for PacketType::MUTE | input1 = typeValue = value of PacketType::MUTE, expectedString = MUTE | Returns "MUTE" without throwing exception | Should Pass |
 * | 18 | Invoke getTypeString for PacketType::UNMUTE | input1 = typeValue = value of PacketType::UNMUTE, expectedString = UNMUTE | Returns "UNMUTE" without throwing exception | Should Pass |
 * | 19 | Invoke getTypeString for PacketType::CC_SET_ATTRIBUTE | input1 = typeValue = value of PacketType::CC_SET_ATTRIBUTE, expectedString = CC_SET_ATTRIBUTE | Returns "CC_SET_ATTRIBUTE" without throwing exception | Should Pass |
 */
TEST(Packet, ValidPacketTypeMapping) {
    std::cout << "Entering ValidPacketTypeMapping test" << std::endl;

    // Create Packet object using default constructor (even though getTypeString is static)
    Packet packet;
    
    // Collection of {PacketType, ExpectedString} pairs; using uint32_t values from PacketType enum.
    std::vector<std::pair<uint32_t, std::string>> typeMapping = {
        {static_cast<uint32_t>(PacketType::PES_DATA), "PES_DATA"},
        {static_cast<uint32_t>(PacketType::TIMESTAMP), "TIMESTAMP"},
        {static_cast<uint32_t>(PacketType::RESET_ALL), "RESET_ALL"},
        {static_cast<uint32_t>(PacketType::RESET_CHANNEL), "RESET_CHANNEL"},
        {static_cast<uint32_t>(PacketType::SUBTITLE_SELECTION), "SUBTITLE_SELECTION"},
        {static_cast<uint32_t>(PacketType::TELETEXT_SELECTION), "TELETEXT_SELECTION"},
        {static_cast<uint32_t>(PacketType::TTML_SELECTION), "TTML_SELECTION"},
        {static_cast<uint32_t>(PacketType::TTML_DATA), "TTML_DATA"},
        {static_cast<uint32_t>(PacketType::TTML_TIMESTAMP), "TTML_TIMESTAMP"},
        {static_cast<uint32_t>(PacketType::WEBVTT_SELECTION), "WEBVTT_SELECTION"},
        {static_cast<uint32_t>(PacketType::WEBVTT_DATA), "WEBVTT_DATA"},
        {static_cast<uint32_t>(PacketType::WEBVTT_TIMESTAMP), "WEBVTT_TIMESTAMP"},
        {static_cast<uint32_t>(PacketType::CC_DATA), "CC_DATA"},
        {static_cast<uint32_t>(PacketType::PAUSE), "PAUSE"},
        {static_cast<uint32_t>(PacketType::RESUME), "RESUME"},
        {static_cast<uint32_t>(PacketType::MUTE), "MUTE"},
        {static_cast<uint32_t>(PacketType::UNMUTE), "UNMUTE"},
        {static_cast<uint32_t>(PacketType::CC_SET_ATTRIBUTE), "CC_SET_ATTRIBUTE"}
    };

    // Iterate over each {PacketType, ExpectedString} pair, invoking getTypeString for each and verifying the result.
    for (const auto& mapping : typeMapping) {
        uint32_t typeValue = mapping.first;
        std::string expectedString = mapping.second;
        std::cout << "Invoking Packet::getTypeString with input: " << typeValue << std::endl;

        std::string result;
        EXPECT_NO_THROW(result = Packet::getTypeString(typeValue));
        std::cout << "For input " << typeValue << ", returned value: " << result << std::endl;
        std::cout << "Expected value: " << expectedString << std::endl;
        EXPECT_EQ(result, expectedString);
    }

    std::cout << "Exiting ValidPacketTypeMapping test" << std::endl;
}
/**
 * @brief Validate appending a zero value to the packet.
 *
 * This test verifies that appending a zero value (0x00000000) using the append32 API to an initially empty packet does not throw any exceptions. The test ensures that the packet remains valid after the operation.
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
 * | Variation / Step | Description                                              | Test Data                     | Expected Result                                        | Notes       |
 * | :--------------: | --------------------------------------------------------- | ----------------------------- | ------------------------------------------------------ | ----------- |
 * | 01               | Invoke append32 API with input value 0x00000000            | input1 = 0x00000000           | No exception thrown; packet updated successfully       | Should Pass |
 */
TEST_F(PacketTest, AppendZeroValueTest) {
    std::cout << "Entering AppendZeroValueTest test" << std::endl;
    std::uint32_t value = 0x00000000;
    std::cout << "Invoking append32 with value: 0x" << std::hex << value << std::dec << std::endl;
    EXPECT_NO_THROW(packet->append32(value));
    std::cout << "Exiting AppendZeroValueTest test" << std::endl;
}
/**
 * @brief Verify that PacketChild::append32 successfully appends the maximum unsigned 32-bit integer value without throwing an exception.
 *
 * This test validates that appending the maximum 32-bit unsigned integer value (0xFFFFFFFF) to a packet using the append32 function correctly handles boundary conditions by not throwing any exceptions.
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
 * | Variation / Step | Description                                                                                       | Test Data                                              | Expected Result                                  | Notes           |
 * | :--------------: | ------------------------------------------------------------------------------------------------- | ------------------------------------------------------ | ------------------------------------------------ | --------------- |
 * | 01               | Log the entry message indicating the start of AppendMaxUint32ValueTest                              | None                                                   | Entry message logged                             | Should be successful |
 * | 02               | Call packet->append32 with value 0xFFFFFFFF and verify that no exception is thrown                  | input value = 0xFFFFFFFF, output: none expected          | No exception thrown; EXPECT_NO_THROW passes      | Should Pass     |
 * | 03               | Log the exit message indicating the end of AppendMaxUint32ValueTest                                 | None                                                   | Exit message logged                              | Should be successful |
 */
TEST_F(PacketTest, AppendMaxUint32ValueTest) {
    std::cout << "Entering AppendMaxUint32ValueTest test" << std::endl;
    std::uint32_t value = 0xFFFFFFFF;
    std::cout << "Invoking append32 with value: 0x" << std::hex << value << std::dec << std::endl;
    EXPECT_NO_THROW(packet->append32(value));
    std::cout << "Exiting AppendMaxUint32ValueTest test" << std::endl;
}
/**
 * @brief Test the append32 method of PacketChild with a typical 32-bit value.
 *
 * This test verifies that the append32 function properly handles a typical 32-bit hexadecimal value (0x12345678) by not throwing any exceptions.
 * It also outputs log messages indicating the start and end of the test to validate the execution order.
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
 * | Variation / Step | Description | Test Data |Expected Result |Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Print entry test message "Entering AppendTypicalValueTest test" | None | "Entering AppendTypicalValueTest test" is displayed on the console | Should be successful |
 * | 02 | Print API invocation message "Invoking append32 with value: 0x12345678" | value = 0x12345678 | "Invoking append32 with value: 0x12345678" is displayed on the console | Should be successful |
 * | 03 | Invoke API append32 on PacketChild with a 32-bit value | input: value = 0x12345678, output: none | No exception is thrown; EXPECT_NO_THROW passes | Should Pass |
 * | 04 | Print exit test message "Exiting AppendTypicalValueTest test" | None | "Exiting AppendTypicalValueTest test" is displayed on the console | Should be successful |
 */
TEST_F(PacketTest, AppendTypicalValueTest) {
    std::cout << "Entering AppendTypicalValueTest test" << std::endl;
    std::uint32_t value = 0x12345678;
    std::cout << "Invoking append32 with value: 0x" << std::hex << value << std::dec << std::endl;
    EXPECT_NO_THROW(packet->append32(value));
    std::cout << "Exiting AppendTypicalValueTest test" << std::endl;
}
/**
 * @brief Test that appending zero value does not throw an exception.
 *
 * This test verifies that when the append64 API of the PacketChild class is invoked with a zero value, no exceptions are thrown. This confirms the method's ability to handle a zero input correctly.
 *
 * **Test Group ID:** Basic: 01 / Module (L2): 02 / Stress (L2): 03
 * **Test Case ID:** 029
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                      | Test Data                                   | Expected Result                                                    | Notes       |
 * | :--------------: | ------------------------------------------------ | ------------------------------------------- | ------------------------------------------------------------------ | ----------- |
 * | 01               | Call append64 API with a zero value              | input: value = 0, output: method execution  | No exception thrown; EXPECT_NO_THROW check passes                  | Should Pass |
 */
TEST_F(PacketTest, AppendZero64) {
    std::cout << "Entering AppendZero64 test" << std::endl;
    
    int64_t value = 0;
    std::cout << "Invoking append64 with value: " << value << std::endl;
    
    EXPECT_NO_THROW({
        packet->append64(value);
        std::cout << "append64 executed successfully with value: " << value << std::endl;
    });
    
    // If getBuffer method was available, we could log its state here.
    std::cout << "Exiting AppendZero64 test" << std::endl;
}
/**
 * @brief Verify that the append64 method correctly appends a positive 64-bit integer without throwing exceptions.
 *
 * This test verifies that the PacketChild::append64 method correctly handles a positive 64-bit integer value. It ensures that the method executes without throwing any exceptions and confirms that the appending logic for the provided value functions as expected.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 030@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:** 
 * | Variation / Step | Description                                                                     | Test Data                                                  | Expected Result                                                    | Notes           |
 * | :--------------: | ------------------------------------------------------------------------------- | ---------------------------------------------------------- | ------------------------------------------------------------------ | --------------- |
 * | 01               | Print message indicating entry into the AppendPositive64 test                   | None                                                       | Entry message is printed                                           | Should be successful |
 * | 02               | Initialize a 64-bit positive value and print the invocation message             | value = 1234567890123456789                                  | Initialization message displays the correct value                  | Should be successful |
 * | 03               | Invoke append64 with the initialized value and check that no exception is thrown  | input value = 1234567890123456789                            | append64 executes without throwing an exception                    | Should Pass     |
 * | 04               | Print message confirming that append64 executed successfully                    | None                                                       | Success message is printed                                         | Should be successful |
 * | 05               | Print message indicating exit from the AppendPositive64 test                    | None                                                       | Exit message is printed                                            | Should be successful |
 */
TEST_F(PacketTest, AppendPositive64) {
    std::cout << "Entering AppendPositive64 test" << std::endl;
    
    int64_t value = 1234567890123456789LL;
    std::cout << "Invoking append64 with value: " << value << std::endl;
    
    EXPECT_NO_THROW({
        packet->append64(value);
        std::cout << "append64 executed successfully with value: " << value << std::endl;
    });
    
    std::cout << "Exiting AppendPositive64 test" << std::endl;
}
/**
 * @brief Verifies that append64 correctly handles negative 64-bit integer input.
 *
 * This test ensures that when a negative value (-1) is passed to the append64 method of the PacketChild class,
 * the method executes without throwing an exception. It confirms that negative inputs are appropriately handled,
 * which is essential to maintain robust data packet processing.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 031@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:** 
 * | Variation / Step | Description                                                       | Test Data                                          | Expected Result                                                                        | Notes           |
 * | :--------------: | ----------------------------------------------------------------- | -------------------------------------------------- | -------------------------------------------------------------------------------------- | --------------- |
 * | 01               | Log the entry into the AppendNegative64 test                      | --                                                 | "Entering AppendNegative64 test" is printed                                            | Should be successful |
 * | 02               | Invoke append64 API with a negative 64-bit integer value (-1)       | input: value = -1, output: none                     | API call does not throw any exception and "append64 executed successfully with value: -1" is printed | Should Pass     |
 * | 03               | Log the exit from the AppendNegative64 test                       | --                                                 | "Exiting AppendNegative64 test" is printed                                             | Should be successful |
 */
TEST_F(PacketTest, AppendNegative64) {
    std::cout << "Entering AppendNegative64 test" << std::endl;
    
    int64_t value = -1;
    std::cout << "Invoking append64 with value: " << value << std::endl;
    
    EXPECT_NO_THROW({
        packet->append64(value);
        std::cout << "append64 executed successfully with value: " << value << std::endl;
    });
    
    std::cout << "Exiting AppendNegative64 test" << std::endl;
}
/**
 * @brief Validate that the append64 API correctly appends the maximum int64_t value without throwing an exception.
 *
 * This test verifies that when the maximum 64-bit integer value (9223372036854775807) is passed to the append64 API, the function executes without throwing an exception. It ensures proper handling of boundary values and logs the test entry and exit to validate the execution flow.
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
 * | Variation / Step | Description                                                  | Test Data                                              | Expected Result                                                  | Notes         |
 * | :--------------: | ------------------------------------------------------------ | ------------------------------------------------------ | ---------------------------------------------------------------- | ------------- |
 * | 01               | Start the AppendMax64 test and print the entry message       | No inputs                                              | Entry log printed correctly                                       | Should be successful |
 * | 02               | Invoke the append64 API with the maximum int64_t value (9223372036854775807) | input: value = 9223372036854775807, API: append64(value) | API executes without throwing any exception                       | Should Pass   |
 * | 03               | End the test by printing the exit message                    | No inputs                                              | Exit log printed correctly; no exceptions during API call         | Should be successful |
 */
TEST_F(PacketTest, AppendMax64) {
    std::cout << "Entering AppendMax64 test" << std::endl;
    
    int64_t value = std::numeric_limits<int64_t>::max(); // 9223372036854775807
    std::cout << "Invoking append64 with value: " << value << std::endl;
    
    EXPECT_NO_THROW({
        packet->append64(value);
        std::cout << "append64 executed successfully with maximum value: " << value << std::endl;
    });
    
    std::cout << "Exiting AppendMax64 test" << std::endl;
}
/**
 * @brief Test to verify that appending the minimum 64-bit integer value does not throw an exception.
 *
 * This test verifies that the append64 function correctly handles the minimum value for a 64-bit integer.
 * It confirms that the API call does not throw any exceptions when invoked with the minimum value.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 033@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                  | Test Data                                             | Expected Result                                          | Notes                |
 * | :--------------: | ------------------------------------------------------------ | ----------------------------------------------------- | -------------------------------------------------------- | -------------------- |
 * | 01               | Log the entry message indicating the test start              | None                                                  | Entry log message printed                                | Should be successful |
 * | 02               | Invoke append64 API with the minimum int64_t value             | value = -9223372036854775808                           | Function executes without throwing any exception       | Should Pass          |
 * | 03               | Log the success message and indicate the test exit             | None                                                  | Exit log message printed                                 | Should be successful |
 */
TEST_F(PacketTest, AppendMin64) {
    std::cout << "Entering AppendMin64 test" << std::endl;
    
    int64_t value = std::numeric_limits<int64_t>::min(); // -9223372036854775808
    std::cout << "Invoking append64 with value: " << value << std::endl;
    
    EXPECT_NO_THROW({
        packet->append64(value);
        std::cout << "append64 executed successfully with minimum value: " << value << std::endl;
    });
    
    std::cout << "Exiting AppendMin64 test" << std::endl;
}
/**
 * @brief Verify appendType() handles valid packet types correctly
 *
 * This test verifies that the appendType() method in the PacketChild class properly handles all valid PacketType values without throwing exceptions. By iterating over a pre-defined set of valid packet types (excluding INVALID), the test confirms the method’s stability and correctness for positive input scenarios.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 034@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                         | Test Data                                                                                                                  | Expected Result                                                                                               | Notes             |
 * | :--------------: | --------------------------------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------- | ----------------- |
 * | 01               | Print entering test message to indicate the start of test execution.                                | None                                                                                                                     | "Entering AppendValidPacketTypes test" message printed.                                                     | Should be successful |
 * | 02               | Iterate through each valid PacketType, call appendType() and check that no exception is thrown.     | packet->appendType(type) where type = PacketType::ZERO, PacketType::PES_DATA, PacketType::TIMESTAMP, PacketType::RESET_ALL, PacketType::RESET_CHANNEL, PacketType::SUBTITLE_SELECTION, PacketType::TELETEXT_SELECTION, PacketType::TTML_SELECTION, PacketType::TTML_DATA, PacketType::TTML_TIMESTAMP, PacketType::CC_DATA, PacketType::PAUSE, PacketType::RESUME, PacketType::MUTE, PacketType::UNMUTE, PacketType::WEBVTT_SELECTION, PacketType::WEBVTT_DATA, PacketType::WEBVTT_TIMESTAMP, PacketType::CC_SET_ATTRIBUTE | appendType() executes without throwing an exception.                                                   | appendType() should execute without exception for all valid PacketType values.                               | Should Pass       |
 * | 03               | Print exiting test message to indicate the completion of test execution.                            | None                                                                                                                     | "Exiting AppendValidPacketTypes test" message printed.                                                       | Should be successful |
 */
TEST_F(PacketTest, AppendValidPacketTypes)
{
    std::cout << "Entering AppendValidPacketTypes test" << std::endl;

    // Create a vector of all valid PacketType values (excluding INVALID)
    std::vector<PacketType> packetTypes = {
        PacketType::ZERO,
        PacketType::PES_DATA,
        PacketType::TIMESTAMP,
        PacketType::RESET_ALL,
        PacketType::RESET_CHANNEL,
        PacketType::SUBTITLE_SELECTION,
        PacketType::TELETEXT_SELECTION,
        PacketType::TTML_SELECTION,
        PacketType::TTML_DATA,
        PacketType::TTML_TIMESTAMP,
        PacketType::CC_DATA,
        PacketType::PAUSE,
        PacketType::RESUME,
        PacketType::MUTE,
        PacketType::UNMUTE,
        PacketType::WEBVTT_SELECTION,
        PacketType::WEBVTT_DATA,
        PacketType::WEBVTT_TIMESTAMP,
        PacketType::CC_SET_ATTRIBUTE
    };

    for (const auto& type : packetTypes)
    {
        std::cout << "Testing PacketType: " << static_cast<uint32_t>(type) << std::endl;
        EXPECT_NO_THROW(packet->appendType(type)) 
            << "appendType() threw an exception for PacketType: " << static_cast<uint32_t>(type);
    }

    std::cout << "Exiting AppendValidPacketTypes test" << std::endl;
}
/**
 * @brief Verify that appendType handles an invalid packet type appropriately.
 *
 * This test validates that calling the appendType function with a PacketType value of INVALID does not throw any exceptions.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 035
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                        | Test Data                                               | Expected Result                                                         | Notes       |
 * | :--------------: | ------------------------------------------------------------------ | ------------------------------------------------------- | ----------------------------------------------------------------------- | ----------- |
 * | 01               | Call appendType with PacketType::INVALID and check for exceptions. | input = PacketType = INVALID, output = void             | Function executes without throwing an exception; EXPECT_NO_THROW passes. | Should Pass |
 */
TEST_F(PacketTest, AppendInvalidPacketType)
{
    std::cout << "Entering AppendInvalidPacketType test" << std::endl;
    
    std::cout << "Invoking Packet::appendType with PacketType value: " 
              << static_cast<std::underlying_type<PacketType>::type>(PacketType::INVALID) << std::endl;
    
    // Expect no exception when calling appendType with PacketType::INVALID
    EXPECT_NO_THROW(packet->appendType(PacketType::INVALID));
    
    std::cout << "Exiting AppendInvalidPacketType test" << std::endl;
}
/**
 * @brief Validate that a newly instantiated Packet object's buffer is empty.
 *
 * This test verifies that invoking the getBuffer method on a freshly instantiated Packet object does not throw any exceptions and returns an empty buffer. It ensures that the initial state of the buffer meets the expected condition for further operations.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 036@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Print entering test message | log message = "Entering RetrieveEmptyBuffer test" | Console prints the entering message | Should be successful |
 * | 02 | Print invoking API message | log message = "Invoking Packet::getBuffer method on the packet object." | Console prints the invoking message | Should be successful |
 * | 03 | Invoke getBuffer and validate empty buffer | packet->getBuffer(), expected buffer size = 0 | API returns an empty vector with size 0 and no exception is thrown | Should Pass |
 * | 04 | Print exiting test message | log message = "Exiting RetrieveEmptyBuffer test" | Console prints the exiting message | Should be successful |
 */
TEST_F(PacketTest, RetrieveEmptyBuffer) {
    std::cout << "Entering RetrieveEmptyBuffer test" << std::endl;

    std::cout << "Invoking Packet::getBuffer method on the packet object." << std::endl;
    EXPECT_NO_THROW({
        std::vector<uint8_t>& buffer = packet->getBuffer();
        std::cout << "Retrieved buffer. Current buffer size: " << buffer.size() << std::endl;
        EXPECT_EQ(buffer.size(), 0u) << "Expected buffer size to be 0 for a newly instantiated Packet object.";
    });

    std::cout << "Exiting RetrieveEmptyBuffer test" << std::endl;
}
/**
 * @brief Verify PausePacket construction with valid positive numbers.
 *
 * This test verifies that constructing a PausePacket object using positive values for channelId and counter does not throw any exceptions,
 * ensuring the object is correctly initialized with valid inputs.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 037
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invokes PausePacket constructor with valid channelId and counter values. | channelId = 10, counter = 5 | PausePacket object is constructed successfully without throwing any exception. | Should Pass |
 */
TEST(PausePacket, ConstructWithValidPositiveNumbers) {
    std::cout << "Entering ConstructWithValidPositiveNumbers test" << std::endl;
    
    uint32_t channelId = 10;
    uint32_t counter = 5;
    std::cout << "Invoking PausePacket constructor with channelId = " << channelId 
              << " and counter = " << counter << std::endl;
    
    EXPECT_NO_THROW({
        PausePacket packet(channelId, counter);
        std::cout << "PausePacket object constructed successfully with channelId = " 
                  << channelId << " and counter = " << counter << std::endl;
    });
    
    std::cout << "Exiting ConstructWithValidPositiveNumbers test" << std::endl;
}
/**
 * @brief Verify that PausePacket constructor properly handles zero channelId.
 *
 * This test checks whether the PausePacket object can be constructed successfully when the channelId is zero, ensuring that edge-case values are handled without exceptions during object creation.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 038@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                           | Test Data                     | Expected Result                                                    | Notes      |
 * | :--------------: | ------------------------------------------------------------------------------------- | ----------------------------- | ------------------------------------------------------------------ | ---------- |
 * | 01               | Invoke PausePacket constructor with channelId set to 0 and counter set to 5           | channelId = 0, counter = 5      | PausePacket object is constructed successfully with no exceptions  | Should Pass|
 */
TEST(PausePacket, ConstructWithZeroChannelId) {
    std::cout << "Entering ConstructWithZeroChannelId test" << std::endl;
    
    uint32_t channelId = 0;
    uint32_t counter = 5;
    std::cout << "Invoking PausePacket constructor with channelId = " << channelId 
              << " and counter = " << counter << std::endl;
    
    EXPECT_NO_THROW({
        PausePacket packet(channelId, counter);
        std::cout << "PausePacket object constructed successfully with channelId = " 
                  << channelId << " and counter = " << counter << std::endl;
    });
    
    std::cout << "Exiting ConstructWithZeroChannelId test" << std::endl;
}
/**
 * @brief Test to verify that constructing a PausePacket object with a zero counter value does not throw an exception.
 *
 * This test verifies that the PausePacket constructor can handle a scenario when the counter is zero. 
 * The test ensures that no exception is thrown during construction and that the object is correctly instantiated.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 039@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                              | Test Data                          | Expected Result                                                    | Notes       |
 * | :--------------: | -------------------------------------------------------- | ---------------------------------- | ------------------------------------------------------------------ | ----------- |
 * | 01               | Invoke the PausePacket constructor with channelId and counter values | channelId = 10, counter = 0          | PausePacket object is constructed successfully without throwing an exception | Should Pass |
 */
TEST(PausePacket, ConstructWithZeroCounter) {
    std::cout << "Entering ConstructWithZeroCounter test" << std::endl;
    
    uint32_t channelId = 10;
    uint32_t counter = 0;
    std::cout << "Invoking PausePacket constructor with channelId = " << channelId 
              << " and counter = " << counter << std::endl;
    
    EXPECT_NO_THROW({
        PausePacket packet(channelId, counter);
        std::cout << "PausePacket object constructed successfully with channelId = " 
                  << channelId << " and counter = " << counter << std::endl;
    });
    
    std::cout << "Exiting ConstructWithZeroCounter test" << std::endl;
}
/**
 * @brief Test the PausePacket constructor with the maximum channelId value
 *
 * This test verifies that a PausePacket object can be successfully constructed when the channelId is set to its maximum possible value. It ensures that the constructor handles extreme boundary conditions without throwing any exceptions.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Print the entry message indicating the start of the test. | None | Entry message "Entering ConstructWithMaxChannelId test" is printed to the console. | Should be successful |
 * | 02 | Initialize variables with channelId set to its maximum value and counter set to 5. | channelId = std::numeric_limits<uint32_t>::max(), counter = 5 | Variables are correctly set with the provided values. | Should be successful |
 * | 03 | Print the invocation message for the PausePacket constructor with the initialized values. | channelId = max value, counter = 5 | The invocation message with channelId and counter details is printed. | Should be successful |
 * | 04 | Invoke the PausePacket constructor inside an EXPECT_NO_THROW block to ensure no exception is thrown. | channelId = max value, counter = 5 | PausePacket is constructed successfully without throwing an exception. | Should Pass |
 * | 05 | Print the success message confirming the successful construction of the PausePacket object. | channelId = max value, counter = 5 | Success message indicating proper object construction is printed. | Should be successful |
 * | 06 | Print the exit message indicating the end of the test. | None | Exit message "Exiting ConstructWithMaxChannelId test" is printed to the console. | Should be successful |
 */
TEST(PausePacket, ConstructWithMaxChannelId) {
    std::cout << "Entering ConstructWithMaxChannelId test" << std::endl;
    
    uint32_t channelId = std::numeric_limits<uint32_t>::max();
    uint32_t counter = 5;
    std::cout << "Invoking PausePacket constructor with channelId = " << channelId 
              << " (max value) and counter = " << counter << std::endl;
    
    EXPECT_NO_THROW({
        PausePacket packet(channelId, counter);
        std::cout << "PausePacket object constructed successfully with channelId = " 
                  << channelId << " (max value) and counter = " << counter << std::endl;
    });
    
    std::cout << "Exiting ConstructWithMaxChannelId test" << std::endl;
}
/**
 * @brief Verify that constructing a PausePacket object with the maximum counter value does not throw an exception
 *
 * This test case verifies that the PausePacket constructor can handle the maximum possible value for the counter parameter. It confirms that providing a valid channelId and a counter set to the maximum uint32_t value does not result in any exceptions. This is critical to ensure that the class correctly handles edge values and behaves as expected without crashing.
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
 * | Variation / Step | Description                                                                 | Test Data                                           | Expected Result                               | Notes       |
 * | :--------------: | --------------------------------------------------------------------------- | --------------------------------------------------- | --------------------------------------------- | ----------- |
 * | 01               | Invoke PausePacket constructor with valid channelId and maximum counter value. | channelId = 10, counter = 4294967295 (max uint32_t) | No exception is thrown during object construction. | Should Pass |
 */
TEST(PausePacket, ConstructWithMaxCounter) {
    std::cout << "Entering ConstructWithMaxCounter test" << std::endl;
    
    uint32_t channelId = 10;
    uint32_t counter = std::numeric_limits<uint32_t>::max();
    std::cout << "Invoking PausePacket constructor with channelId = " << channelId 
              << " and counter = " << counter << " (max value)" << std::endl;
    
    EXPECT_NO_THROW({
        PausePacket packet(channelId, counter);
        std::cout << "PausePacket object constructed successfully with channelId = " 
                  << channelId << " and counter = " << counter << " (max value)" << std::endl;
    });
    
    std::cout << "Exiting ConstructWithMaxCounter test" << std::endl;
}
/**
 * @brief Verify that the default constructor of ResetAllPacket initializes the object correctly without throwing exceptions.
 *
 * This test verifies that invoking the default constructor of the ResetAllPacket class correctly constructs an instance without any exceptions.
 * It ensures that the internal buffer content and counter are initialized to their expected states upon construction.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 042@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                  | Test Data                                     | Expected Result                                          | Notes       |
 * | :---------------:| ---------------------------------------------------------------------------------------------| --------------------------------------------- | ---------------------------------------------------------| ----------- |
 * | 01               | Invoke the default constructor of ResetAllPacket to create an object and verify no exception is thrown. | input: None, output: ResetAllPacket object   | The API should not throw an exception; object is constructed successfully. | Should Pass |
 */
TEST(ResetAllPacket, DefaultConstructionInitializesResetAllPacketWithCorrectBufferContentAndCounterValue)
{
    std::cout << "Entering DefaultConstructionInitializesResetAllPacketWithCorrectBufferContentAndCounterValue test" << std::endl;
    
    EXPECT_NO_THROW(
        {
            std::cout << "Invoking ResetAllPacket() default constructor." << std::endl;
            ResetAllPacket packet;
            std::cout << "Constructor invoked successfully. No exception thrown." << std::endl;
        }
    );
    
    std::cout << "Exiting DefaultConstructionInitializesResetAllPacketWithCorrectBufferContentAndCounterValue test" << std::endl;
}
/**
 * @brief Validate that ResetChannelPacket constructor operates correctly with typical positive values
 *
 * This test verifies that the ResetChannelPacket constructor, when provided with a typical positive channelId 
 * and counter value, executes without throwing an exception. The test ensures that the instance creation is safe 
 * and meets the operational expectations, as shown by the EXPECT_NO_THROW assertion.
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
 * | 01 | Invoke ResetChannelPacket constructor with channelId = 50 and counter = 100 | channelId = 50, counter = 100 | Constructor executes without throwing exception; EXPECT_NO_THROW passes | Should Pass |
 */
TEST(ResetChannelPacket, TypicalPositiveValues) {
    std::cout << "Entering TypicalPositiveValues test" << std::endl;
    
    uint32_t channelId = 50;
    uint32_t counter = 100;
    std::cout << "Invoking ResetChannelPacket constructor with channelId = " << channelId 
              << " and counter = " << counter << std::endl;
    
    EXPECT_NO_THROW({
        ResetChannelPacket packet(channelId, counter);
        std::cout << "ResetChannelPacket constructor executed successfully." << std::endl;
    });
    
    std::cout << "Exiting TypicalPositiveValues test" << std::endl;
}
/**
 * @brief Test ResetChannelPacket constructor with the minimum channel id
 *
 * This test verifies that the ResetChannelPacket constructor handles the minimum channel id value (0) along with a specific counter value (100) without throwing any exceptions.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 044@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Initialize channelId with 0 and counter with 100, then call ResetChannelPacket constructor | channelId = 0, counter = 100 | ResetChannelPacket constructor executes without throwing exceptions | Should Pass |
 */
TEST(ResetChannelPacket, MinimumChannelId) {
    std::cout << "Entering MinimumChannelId test" << std::endl;
    
    uint32_t channelId = 0;
    uint32_t counter = 100;
    std::cout << "Invoking ResetChannelPacket constructor with channelId = " << channelId 
              << " and counter = " << counter << std::endl;
    
    EXPECT_NO_THROW({
        ResetChannelPacket packet(channelId, counter);
        std::cout << "ResetChannelPacket constructor executed successfully with channelId = 0." << std::endl;
    });
    
    std::cout << "Exiting MinimumChannelId test" << std::endl;
}
/**
 * @brief Verify ResetChannelPacket constructor handles minimum counter value correctly.
 *
 * This test verifies that the ResetChannelPacket constructor is able to handle the smallest valid counter value (i.e., zero) for a given channelId (50) without throwing an exception. The test confirms that initializing the packet under these boundary conditions succeeds and the constructor behaves as expected.
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
 * | Variation / Step | Description                                             | Test Data                            | Expected Result                                                    | Notes          |
 * | :--------------: | ------------------------------------------------------- | ------------------------------------ | ------------------------------------------------------------------ | -------------- |
 * | 01               | Initialize channelId and counter variables              | channelId = 50, counter = 0            | Variables set correctly to their minimum boundary values           | Should be successful |
 * | 02               | Invoke ResetChannelPacket constructor with the given values | channelId = 50, counter = 0            | Constructor execution does not throw any exceptions                 | Should Pass    |
 */
TEST(ResetChannelPacket, MinimumCounter) {
    std::cout << "Entering MinimumCounter test" << std::endl;
    
    uint32_t channelId = 50;
    uint32_t counter = 0;
    std::cout << "Invoking ResetChannelPacket constructor with channelId = " << channelId 
              << " and counter = " << counter << std::endl;
    
    EXPECT_NO_THROW({
        ResetChannelPacket packet(channelId, counter);
        std::cout << "ResetChannelPacket constructor executed successfully with counter = 0." << std::endl;
    });
    
    std::cout << "Exiting MinimumCounter test" << std::endl;
}
/**
 * @brief Test ResetChannelPacket constructor with maximum channelId value
 *
 * This test verifies that constructing a ResetChannelPacket object with the maximum possible channelId (4294967295U) and a valid counter (100) does not throw any exceptions, ensuring the API can handle edge-case channel identifiers.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 046@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**
 * | Variation / Step | Description                                                                              | Test Data                                           | Expected Result                                                             | Notes      |
 * | :--------------: | ---------------------------------------------------------------------------------------- | --------------------------------------------------- | --------------------------------------------------------------------------- | ---------- |
 * | 01               | Invoke ResetChannelPacket constructor with maximum channelId and a valid counter value.   | channelId = 4294967295, counter = 100               | Constructor does not throw any exception and passes EXPECT_NO_THROW check.   | Should Pass |
 */
TEST(ResetChannelPacket, MaximumChannelId) {
    std::cout << "Entering MaximumChannelId test" << std::endl;
    
    uint32_t channelId = 4294967295U;
    uint32_t counter = 100;
    std::cout << "Invoking ResetChannelPacket constructor with channelId = " << channelId 
              << " and counter = " << counter << std::endl;
    
    EXPECT_NO_THROW({
        ResetChannelPacket packet(channelId, counter);
        std::cout << "ResetChannelPacket constructor executed successfully with maximum channelId." << std::endl;
    });
    
    std::cout << "Exiting MaximumChannelId test" << std::endl;
}
/**
 * @brief Test ResetChannelPacket constructor with maximum counter value
 *
 * This test verifies that invoking the ResetChannelPacket constructor with a channel ID of 50 and the maximum counter value of 4294967295U does not throw an exception. It confirms that the API correctly handles edge-case input parameters.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 047@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                              | Test Data                                          | Expected Result                                                       | Notes       |
 * | :--------------: | ------------------------------------------------------------------------ | -------------------------------------------------- | --------------------------------------------------------------------- | ----------- |
 * | 01               | Invoke ResetChannelPacket constructor with channelId and maximum counter | input: channelId = 50, counter = 4294967295U         | API does not throw an exception, object is created successfully in the constructor | Should Pass |
 */
TEST(ResetChannelPacket, MaximumCounter) {
    std::cout << "Entering MaximumCounter test" << std::endl;
    
    uint32_t channelId = 50;
    uint32_t counter = 4294967295U;
    std::cout << "Invoking ResetChannelPacket constructor with channelId = " << channelId 
              << " and counter = " << counter << std::endl;
    
    EXPECT_NO_THROW({
        ResetChannelPacket packet(channelId, counter);
        std::cout << "ResetChannelPacket constructor executed successfully with maximum counter." << std::endl;
    });
    
    std::cout << "Exiting MaximumCounter test" << std::endl;
}
/**
 * @brief Validate ResumePacket construction with valid positive input values
 *
 * This test verifies that the ResumePacket constructor correctly creates an object when provided with valid positive values for channelId and counter. The test ensures that no exceptions are thrown during the object construction, confirming the normal operation of the API under typical conditions.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 048@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                         | Test Data                                  | Expected Result                                                         | Notes      |
 * | :--------------: | ------------------------------------------------------------------- | ------------------------------------------ | ----------------------------------------------------------------------- | ---------- |
 * | 01               | Invoke ResumePacket constructor with channelId = 100 and counter = 200 | channelId = 100, counter = 200               | ResumePacket object is created without throwing any exceptions; EXPECT_NO_THROW passes | Should Pass |
 */
TEST(ResumePacket, ValidInputPositiveValues) {
    std::cout << "Entering ValidInputPositiveValues test" << std::endl;
    uint32_t channelId = 100;
    uint32_t counter = 200;
    std::cout << "Invoking ResumePacket constructor with channelId = " << channelId 
              << " and counter = " << counter << std::endl;
    EXPECT_NO_THROW({
        ResumePacket packet(channelId, counter);
        std::cout << "ResumePacket object created successfully." << std::endl;
    });
    std::cout << "Exiting ValidInputPositiveValues test" << std::endl;
}
/**
 * @brief Verify that creating a ResumePacket object with a zero channel identifier does not throw an exception
 *
 * This test verifies that the ResumePacket constructor correctly handles the case where the channelId is set to zero without throwing any exceptions. Handling a zero channel ID is important for ensuring that the API can manage edge-case inputs, maintaining robust behavior even with boundary values.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 049@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                               | Test Data                           | Expected Result                                                                 | Notes      |
 * | :--------------: | ----------------------------------------------------------------------------------------- | ----------------------------------- | ------------------------------------------------------------------------------- | ---------- |
 * | 01               | Invoke ResumePacket constructor with channelId set to 0 and counter set to 200.           | channelId = 0, counter = 200          | ResumePacket object is created without exceptions and passes EXPECT_NO_THROW.   | Should Pass |
 */
TEST(ResumePacket, ZeroChannelId) {
    std::cout << "Entering ZeroChannelId test" << std::endl;
    uint32_t channelId = 0;
    uint32_t counter = 200;
    std::cout << "Invoking ResumePacket constructor with channelId = " << channelId 
              << " and counter = " << counter << std::endl;
    EXPECT_NO_THROW({
        ResumePacket packet(channelId, counter);
        std::cout << "ResumePacket object created successfully." << std::endl;
    });
    std::cout << "Exiting ZeroChannelId test" << std::endl;
}
/**
 * @brief Verify that ResumePacket is successfully created with a zero counter.
 *
 * This test checks that the ResumePacket constructor does not throw an exception when provided with a valid channelId (100) and a counter value of zero. It ensures that the object handles a zero counter correctly.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 050
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoke ResumePacket constructor with channelId and counter | channelId = 100, counter = 0 | ResumePacket object is created without throwing any exception | Should Pass |
 */
TEST(ResumePacket, ZeroCounter) {
    std::cout << "Entering ZeroCounter test" << std::endl;
    uint32_t channelId = 100;
    uint32_t counter = 0;
    std::cout << "Invoking ResumePacket constructor with channelId = " << channelId 
              << " and counter = " << counter << std::endl;
    EXPECT_NO_THROW({
        ResumePacket packet(channelId, counter);
        std::cout << "ResumePacket object created successfully." << std::endl;
    });
    std::cout << "Exiting ZeroCounter test" << std::endl;
}
/**
 * @brief Validate that ResumePacket constructor handles maximum channel ID and valid counter without throwing exceptions
 *
 * This test verifies that the ResumePacket constructor correctly accepts the maximum unsigned 32-bit integer value for channelId (4294967295) along with a valid counter value (200). The objective is to ensure that the API properly handles boundary conditions and constructs the object without throwing any exceptions.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 051@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                          | Test Data                                   | Expected Result                                                          | Notes      |
 * | :--------------: | ------------------------------------------------------------------------------------ | ------------------------------------------- | ------------------------------------------------------------------------ | ---------- |
 * | 01               | Invoke ResumePacket constructor with maximum channelId and a valid counter value     | channelId = 4294967295, counter = 200         | Object creation succeeds without throwing any exceptions                 | Should Pass|
 */
TEST(ResumePacket, MaxChannelId) {
    std::cout << "Entering MaxChannelId test" << std::endl;
    uint32_t channelId = 4294967295u;
    uint32_t counter = 200;
    std::cout << "Invoking ResumePacket constructor with channelId = " << channelId 
              << " and counter = " << counter << std::endl;
    EXPECT_NO_THROW({
        ResumePacket packet(channelId, counter);
        std::cout << "ResumePacket object created successfully." << std::endl;
    });
    std::cout << "Exiting MaxChannelId test" << std::endl;
}
/**
 * @brief Verify ResumePacket constructor with maximum counter value.
 *
 * This test case verifies that the ResumePacket constructor does not throw an exception when called with a valid channelId and the maximum 32-bit unsigned integer value for counter. It ensures the integrity of object creation under boundary conditions for the counter.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 052@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                              | Test Data                                          | Expected Result                                                                           | Notes       |
 * | :--------------: | ------------------------------------------------------------------------ | -------------------------------------------------- | ----------------------------------------------------------------------------------------- | ----------- |
 * | 01               | Invoke ResumePacket constructor with valid channelId and maximum counter | input: channelId = 100, counter = 4294967295         | ResumePacket constructor should not throw an exception; EXPECT_NO_THROW passes             | Should Pass |
 */
TEST(ResumePacket, MaxCounter) {
    std::cout << "Entering MaxCounter test" << std::endl;
    uint32_t channelId = 100;
    uint32_t counter = 4294967295u;
    std::cout << "Invoking ResumePacket constructor with channelId = " << channelId 
              << " and counter = " << counter << std::endl;
    EXPECT_NO_THROW({
        ResumePacket packet(channelId, counter);
        std::cout << "ResumePacket object created successfully." << std::endl;
    });
    std::cout << "Exiting MaxCounter test" << std::endl;
}
/**
 * @brief Verify that constructing UnmutePacket with a channelId of 0 does not throw exceptions.
 *
 * This test verifies that creating an instance of UnmutePacket with a channelId value of 0 and a counter value of 67890 does not result in any exceptions. It ensures that the constructor handles a zero channelId correctly.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 053@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                        | Test Data                                               | Expected Result                                         | Notes        |
 * | :--------------: | ------------------------------------------------------------------ | ------------------------------------------------------- | ------------------------------------------------------- | ------------ |
 * |       01         | Initialize channelId and counter, then call UnmutePacket constructor | channelId = 0, counter = 67890                           | UnmutePacket object is created without throwing any exception | Should Pass  |
 */
TEST(UnmutePacket, ConstructWithZeroChannelId) {
    std::cout << "Entering ConstructWithZeroChannelId test" << std::endl;
    
    std::uint32_t channelId = 0;
    std::uint32_t counter = 67890;
    std::cout << "Input values - channelId: " << channelId << ", counter: " << counter << std::endl;
    
    std::cout << "Invoking UnmutePacket constructor with channelId = " << channelId << " and counter = " << counter << std::endl;
    EXPECT_NO_THROW({
        UnmutePacket packet(channelId, counter);
        std::cout << "Successfully created UnmutePacket object with channelId = " << channelId << " and counter = " << counter << std::endl;
    });
    
    std::cout << "Exiting ConstructWithZeroChannelId test" << std::endl;
}
/**
 * @brief Verify UnmutePacket constructor does not throw when invoked with a zero counter
 *
 * This test validates that the UnmutePacket constructor, when provided with a valid channelId and a counter value of zero, correctly initializes the object without throwing any exceptions. The test ensures that the class handles zero counter values gracefully, which is critical for proper operation in scenarios where the counter might be zero.
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
 * | Variation / Step | Description                                                                       | Test Data                                 | Expected Result                                                 | Notes      |
 * | :--------------: | --------------------------------------------------------------------------------- | ----------------------------------------- | --------------------------------------------------------------- | ---------- |
 * | 01               | Invoke UnmutePacket constructor with channelId set to 12345 and counter set to 0  | channelId = 12345, counter = 0, output = UnmutePacket object created | UnmutePacket object is successfully created without any exception | Should Pass |
 */
TEST(UnmutePacket, ConstructWithZeroCounter) {
    std::cout << "Entering ConstructWithZeroCounter test" << std::endl;
    
    std::uint32_t channelId = 12345;
    std::uint32_t counter = 0;
    std::cout << "Input values - channelId: " << channelId << ", counter: " << counter << std::endl;
    
    std::cout << "Invoking UnmutePacket constructor with channelId = " << channelId << " and counter = " << counter << std::endl;
    EXPECT_NO_THROW({
        UnmutePacket packet(channelId, counter);
        std::cout << "Successfully created UnmutePacket object with channelId = " << channelId << " and counter = " << counter << std::endl;
    });
    
    std::cout << "Exiting ConstructWithZeroCounter test" << std::endl;
}
/**
 * @brief Test that constructs UnmutePacket with typical non-zero values
 *
 * This test validates that invoking the UnmutePacket constructor with typical non-zero values does not throw an exception.
 * It verifies the proper construction of the UnmutePacket object when valid inputs are provided.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 055@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                        | Test Data                                   | Expected Result                                                    | Notes       |
 * | :--------------: | ------------------------------------------------------------------ | ------------------------------------------- | ------------------------------------------------------------------ | ----------- |
 * | 01               | Invoke the UnmutePacket constructor with channelId and counter values | channelId = 12345, counter = 67890            | No exception is thrown; UnmutePacket object is created successfully | Should Pass |
 */
TEST(UnmutePacket, ConstructWithTypicalNonZeroValues) {
    std::cout << "Entering ConstructWithTypicalNonZeroValues test" << std::endl;
    
    std::uint32_t channelId = 12345;
    std::uint32_t counter = 67890;
    std::cout << "Input values - channelId: " << channelId << ", counter: " << counter << std::endl;
    
    std::cout << "Invoking UnmutePacket constructor with channelId = " << channelId << " and counter = " << counter << std::endl;
    EXPECT_NO_THROW({
        UnmutePacket packet(channelId, counter);
        std::cout << "Successfully created UnmutePacket object with channelId = " << channelId << " and counter = " << counter << std::endl;
    });
    
    std::cout << "Exiting ConstructWithTypicalNonZeroValues test" << std::endl;
}
/**
 * @brief Validate construction of UnmutePacket with maximum channelId
 *
 * Validate that UnmutePacket's constructor correctly handles the maximum possible value for channelId (4294967295) along with a valid counter (67890) without throwing any exceptions. This test ensures that the boundary condition for the channelId is properly managed.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 056@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoke UnmutePacket constructor with maximum channelId and counter | channelId = 4294967295, counter = 67890 | Successfully constructs UnmutePacket object without throwing exceptions | Should Pass |
 */
TEST(UnmutePacket, ConstructWithMaxChannelId) {
    std::cout << "Entering ConstructWithMaxChannelId test" << std::endl;
    
    std::uint32_t channelId = 4294967295u; // maximum value for uint32_t
    std::uint32_t counter = 67890;
    std::cout << "Input values - channelId: " << channelId << ", counter: " << counter << std::endl;
    
    std::cout << "Invoking UnmutePacket constructor with channelId = " << channelId << " and counter = " << counter << std::endl;
    EXPECT_NO_THROW({
        UnmutePacket packet(channelId, counter);
        std::cout << "Successfully created UnmutePacket object with maximum channelId = " << channelId << " and counter = " << counter << std::endl;
    });
    
    std::cout << "Exiting ConstructWithMaxChannelId test" << std::endl;
}
/**
 * @brief Test the UnmutePacket constructor with maximum counter value.
 *
 * This test verifies that the UnmutePacket constructor can successfully handle the maximum possible value for a uint32_t counter without throwing any exceptions. The objective is to ensure robust behavior when using extreme boundary inputs.
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
 * | Variation / Step | Description                                                                       | Test Data                                           | Expected Result                                                         | Notes       |
 * | :--------------: | --------------------------------------------------------------------------------- | --------------------------------------------------- | ----------------------------------------------------------------------- | ----------- |
 * | 01               | Invoke UnmutePacket constructor with channelId = 12345 and counter = 4294967295u   | channelId = 12345, counter = 4294967295               | Constructor does not throw exception; UnmutePacket object is created successfully | Should Pass |
 */
TEST(UnmutePacket, ConstructWithMaxCounter) {
    std::cout << "Entering ConstructWithMaxCounter test" << std::endl;
    
    std::uint32_t channelId = 12345;
    std::uint32_t counter = 4294967295u; // maximum value for uint32_t
    std::cout << "Input values - channelId: " << channelId << ", counter: " << counter << std::endl;
    
    std::cout << "Invoking UnmutePacket constructor with channelId = " << channelId << " and counter = " << counter << std::endl;
    EXPECT_NO_THROW({
        UnmutePacket packet(channelId, counter);
        std::cout << "Successfully created UnmutePacket object with channelId = " << channelId << " and maximum counter = " << counter << std::endl;
    });
    
    std::cout << "Exiting ConstructWithMaxCounter test" << std::endl;
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
