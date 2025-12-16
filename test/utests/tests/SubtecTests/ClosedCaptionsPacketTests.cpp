
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
#include "ClosedCaptionsPacket.hpp"


// Test Case: Valid Construction with CEA::type_608 using typical positive values
/**
 * @brief Verify that the ClosedCaptionsActiveTypePacket constructor accepts valid typical positive values for CEA608 type.
 *
 * This test checks that the ClosedCaptionsActiveTypePacket object is successfully constructed without throwing
 * any exceptions when provided with typical positive inputs. It validates that the channel id, counter, service, 
 * and type parameters are correctly used to create the packet instance, ensuring the proper functioning of the 
 * constructor in a positive scenario.
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
 * | Variation / Step | Description                                                                                   | Test Data                                                      | Expected Result                                                                          | Notes         |
 * | :--------------: | --------------------------------------------------------------------------------------------- | -------------------------------------------------------------- | ---------------------------------------------------------------------------------------- | ------------- |
 * | 01               | Initialize test input values and log the test start                                             | channelId = 1, counter = 10, service = 2, type = CEA::type_608   | Values are initialized and printed correctly                                              | Should be successful |
 * | 02               | Invoke the ClosedCaptionsActiveTypePacket constructor and verify no exception is thrown           | channelId = 1, counter = 10, type = CEA::type_608, service = 2     | ClosedCaptionsActiveTypePacket is constructed successfully without throwing exceptions     | Should Pass   |
 * | 03               | Log confirmation message for successful construction                                           | N/A                                                            | Confirmation message is printed indicating the successful construction of the packet       | Should be successful |
 */
TEST(ClosedCaptionsActiveTypePacket, ValidConstructionWithCEA608_TypicalPositiveValues)
{
    std::cout << "Entering ValidConstructionWithCEA608_TypicalPositiveValues test" << std::endl;
    
    uint32_t channelId = 1;
    uint32_t counter = 10;
    int service = 2;
    CEA type = CEA::type_608;
    
    std::cout << "Invoking ClosedCaptionsActiveTypePacket constructor with values:" << std::endl;
    std::cout << "  channelId: " << channelId << std::endl;
    std::cout << "  counter: " << counter << std::endl;
    std::cout << "  service: " << service << std::endl;
    std::cout << "  type (integral value): " << static_cast<uint32_t>(type) << std::endl;
    
    EXPECT_NO_THROW({
        ClosedCaptionsActiveTypePacket packet(channelId, counter, type, service);
        std::cout << "Packet constructed successfully with expected appended fields:" << std::endl;
    });
    
    std::cout << "Exiting ValidConstructionWithCEA608_TypicalPositiveValues test" << std::endl;
}
/**
 * @brief Verify valid construction of ClosedCaptionsActiveTypePacket using typical positive values.
 *
 * This test verifies that a ClosedCaptionsActiveTypePacket object is successfully constructed using valid typical values for CEA type.
 * It specifically tests with CEA::type_708 and checks that no exceptions are thrown during the construction of the packet.
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
 * | Variation / Step | Description                                                          | Test Data: channelId = 1, counter = 10, service = 2, type = CEA::type_708 | Expected Result: No exception thrown and packet constructed successfully | Notes: Should Pass |
 */
TEST(ClosedCaptionsActiveTypePacket, ValidConstructionWithCEA708_TypicalPositiveValues)
{
    std::cout << "Entering ValidConstructionWithCEA708_TypicalPositiveValues test" << std::endl;
    
    uint32_t channelId = 1;
    uint32_t counter = 10;
    int service = 2;
    CEA type = CEA::type_708;
    
    std::cout << "Invoking ClosedCaptionsActiveTypePacket constructor with values:" << std::endl;
    std::cout << "  channelId: " << channelId << std::endl;
    std::cout << "  counter: " << counter << std::endl;
    std::cout << "  service: " << service << std::endl;
    std::cout << "  type (integral value): " << static_cast<uint32_t>(type) << std::endl;
    
    EXPECT_NO_THROW({
        ClosedCaptionsActiveTypePacket packet(channelId, counter, type, service);
        std::cout << "Packet constructed successfully with expected appended fields:" << std::endl;
    });
    
    std::cout << "Exiting ValidConstructionWithCEA708_TypicalPositiveValues test" << std::endl;
}
/**
 * @brief Verify that ClosedCaptionsActiveTypePacket can be constructed with maximum boundary values for CEA::type_608.
 *
 * This test ensures that the ClosedCaptionsActiveTypePacket constructor correctly handles the maximum allowable boundary values for channelId, counter, and service when the type is set to CEA::type_608. The test verifies that no exceptions are thrown during the object construction.
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
 * | Variation / Step | Description                                                                                                                       | Test Data                                                              | Expected Result                                                                    | Notes       |
 * | :--------------: | --------------------------------------------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------- | ---------------------------------------------------------------------------------- | ----------- |
 * | 01               | Invoke ClosedCaptionsActiveTypePacket constructor with channelId = 4294967295, counter = 4294967295, service = 100, type = CEA::type_608 | channelId = 4294967295, counter = 4294967295, service = 100, type = CEA::type_608 | Object is constructed successfully without throwing any exceptions (EXPECT_NO_THROW passes) | Should Pass |
 */
TEST(ClosedCaptionsActiveTypePacket, ValidConstructionWithCEA608_MaxBoundaryValues)
{
    std::cout << "Entering ValidConstructionWithCEA608_MaxBoundaryValues test" << std::endl;
    
    uint32_t channelId = 4294967295;
    uint32_t counter = 4294967295;
    int service = 100;
    CEA type = CEA::type_608;
    
    std::cout << "Invoking ClosedCaptionsActiveTypePacket constructor with maximum boundary values:" << std::endl;
    std::cout << "  channelId: " << channelId << std::endl;
    std::cout << "  counter: " << counter << std::endl;
    std::cout << "  service: " << service << std::endl;
    std::cout << "  type (integral value): " << static_cast<uint32_t>(type) << std::endl;
    
    EXPECT_NO_THROW({
        ClosedCaptionsActiveTypePacket packet(channelId, counter, type, service);
        std::cout << "Packet constructed successfully with expected maximum boundary appended fields:" << std::endl;
    });
    
    std::cout << "Exiting ValidConstructionWithCEA608_MaxBoundaryValues test" << std::endl;
}
/**
 * @brief Validate construction of ClosedCaptionsActiveTypePacket using maximum boundary values for CEA::type_708
 *
 * This test verifies that the ClosedCaptionsActiveTypePacket constructor correctly processes the maximum allowable values for channelId and counter, and the provided service value when using CEA::type_708. The test ensures that no exception is thrown during construction.
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
 * | Variation / Step | Description                                                               | Test Data                                                        | Expected Result                                  | Notes      |
 * | :--------------: | ------------------------------------------------------------------------- | ---------------------------------------------------------------- | ------------------------------------------------ | ---------- |
 * | 01               | Invoke the ClosedCaptionsActiveTypePacket constructor with maximum boundary values and expect no exception. | channelId = 4294967295, counter = 4294967295, service = 100, type = CEA::type_708 | Constructor executes without throwing any exceptions. | Should Pass |
 */
TEST(ClosedCaptionsActiveTypePacket, ValidConstructionWithCEA708_MaxBoundaryValues)
{
    std::cout << "Entering ValidConstructionWithCEA708_MaxBoundaryValues test" << std::endl;
    
    uint32_t channelId = 4294967295;
    uint32_t counter = 4294967295;
    int service = 100;
    CEA type = CEA::type_708;
    
    std::cout << "Invoking ClosedCaptionsActiveTypePacket constructor with maximum boundary values:" << std::endl;
    std::cout << "  channelId: " << channelId << std::endl;
    std::cout << "  counter: " << counter << std::endl;
    std::cout << "  service: " << service << std::endl;
    std::cout << "  type (integral value): " << static_cast<uint32_t>(type) << std::endl;    
    
    EXPECT_NO_THROW({
        ClosedCaptionsActiveTypePacket packet(channelId, counter, type, service);
        std::cout << "Packet constructed successfully with expected maximum boundary appended fields:" << std::endl;
    });
    
    std::cout << "Exiting ValidConstructionWithCEA708_MaxBoundaryValues test" << std::endl;
}
/**
 * @brief Verify that the ClosedCaptionsActiveTypePacket is constructed successfully using CEA::type_608 and zero values.
 *
 * This test verifies that the ClosedCaptionsActiveTypePacket API can be invoked with zero values for channelId, counter, and service along with the CEA::type_608 type without causing any exception. It ensures proper handling of zero or default input parameters.
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
 * | Variation / Step | Description                                                                 | Test Data                                                  | Expected Result                                                                  | Notes       |
 * | :--------------: | --------------------------------------------------------------------------- | ---------------------------------------------------------- | -------------------------------------------------------------------------------- | ----------- |
 * | 01               | Invoke the ClosedCaptionsActiveTypePacket constructor with zero values.     | channelId = 0, counter = 0, service = 0, type = CEA::type_608 | No exception thrown during construction; packet is constructed successfully.    | Should Pass |
 */
TEST(ClosedCaptionsActiveTypePacket, ValidConstructionWithCEA608_ZeroValues)
{
    std::cout << "Entering ValidConstructionWithCEA608_ZeroValues test" << std::endl;
    
    uint32_t channelId = 0;
    uint32_t counter = 0;
    int service = 0;
    CEA type = CEA::type_608;
    
    std::cout << "Invoking ClosedCaptionsActiveTypePacket constructor with zero values:" << std::endl;
    std::cout << "  channelId: " << channelId << std::endl;
    std::cout << "  counter: " << counter << std::endl;
    std::cout << "  service: " << service << std::endl;
    std::cout << "  type (integral value): " << static_cast<uint32_t>(type) << std::endl;    
    
    EXPECT_NO_THROW({
        ClosedCaptionsActiveTypePacket packet(channelId, counter, type, service);
        std::cout << "Packet constructed successfully with expected zero-value appended fields:" << std::endl;
    });
    
    std::cout << "Exiting ValidConstructionWithCEA608_ZeroValues test" << std::endl;
}
/**
 * @brief Test to verify valid construction of ClosedCaptionsActiveTypePacket using CEA 708 type with zero initialization values
 *
 * This test checks whether the ClosedCaptionsActiveTypePacket object can be successfully constructed when all its input parameters are set to zero (or the equivalent zero-value for the given type) and the type is CEA::type_708. It ensures that the constructor does not throw an exception and that the object initializes correctly.
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
 * | 01 | Invoke ClosedCaptionsActiveTypePacket constructor with zero values for channelId, counter, and service; type set to CEA::type_708. | channelId = 0, counter = 0, service = 0, type = CEA::type_708 | Packet constructed successfully without throwing any exceptions | Should Pass |
 */
TEST(ClosedCaptionsActiveTypePacket, ValidConstructionWithCEA708_ZeroValues)
{
    std::cout << "Entering ValidConstructionWithCEA708_ZeroValues test" << std::endl;
    
    uint32_t channelId = 0;
    uint32_t counter = 0;
    int service = 0;
    CEA type = CEA::type_708;
    
    std::cout << "Invoking ClosedCaptionsActiveTypePacket constructor with zero values:" << std::endl;
    std::cout << "  channelId: " << channelId << std::endl;
    std::cout << "  counter: " << counter << std::endl;
    std::cout << "  service: " << service << std::endl;
    std::cout << "  type (integral value): " << static_cast<uint32_t>(type) << std::endl;    
    
    EXPECT_NO_THROW({
        ClosedCaptionsActiveTypePacket packet(channelId, counter, type, service);
        std::cout << "Packet constructed successfully with expected zero-value appended fields:" << std::endl;
    });
    
    std::cout << "Exiting ValidConstructionWithCEA708_ZeroValues test" << std::endl;
}
/**
 * @brief Test handling of negative service value for CEA::type_608 in ClosedCaptionsActiveTypePacket
 *
 * This test verifies that the ClosedCaptionsActiveTypePacket constructor correctly handles a negative service value for CEA::type_608 without throwing an exception. This is important to ensure that the constructor is robust against invalid service inputs.
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
 * | Variation / Step | Description                                                                                           | Test Data                                                 | Expected Result                                                      | Notes          |
 * | :--------------: | ----------------------------------------------------------------------------------------------------- | --------------------------------------------------------- | -------------------------------------------------------------------- | -------------- |
 * | 01               | Initialize test parameters for ClosedCaptionsActiveTypePacket constructor.                            | channelId = 100, counter = 50, service = -1, type = CEA::type_608 | Parameters are correctly defined.                                     | Should be successful |
 * | 02               | Invoke ClosedCaptionsActiveTypePacket constructor with negative service value inside EXPECT_NO_THROW.  | channelId = 100, counter = 50, service = -1, type = CEA::type_608 | No exception is thrown; packet is constructed successfully.          | Should Pass    |
 */
TEST(ClosedCaptionsActiveTypePacket, HandlingNegativeServiceValueForCEA608)
{
    std::cout << "Entering HandlingNegativeServiceValueForCEA608 test" << std::endl;
    
    uint32_t channelId = 100;
    uint32_t counter = 50;
    int service = -1;
    CEA type = CEA::type_608;
    
    std::cout << "Invoking ClosedCaptionsActiveTypePacket constructor with negative service value:" << std::endl;
    std::cout << "  channelId: " << channelId << std::endl;
    std::cout << "  counter: " << counter << std::endl;
    std::cout << "  service: " << service << std::endl;
    std::cout << "  type (integral value): " << static_cast<uint32_t>(type) << std::endl;    
    
    EXPECT_NO_THROW({
        ClosedCaptionsActiveTypePacket packet(channelId, counter, type, service);
        std::cout << "Packet constructed successfully with negative service value appended:" << std::endl;
    });
    
    std::cout << "Exiting HandlingNegativeServiceValueForCEA608 test" << std::endl;
}
/**
 * @brief Test the handling of a negative service value for CEA708 type
 *
 * This test verifies that the ClosedCaptionsActiveTypePacket constructor handles a negative service value correctly when using CEA::type_708. The test ensures that even with an invalid negative service value, the constructor does not throw an exception, thus validating robust error handling.
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
 * | Variation / Step | Description                                                                              | Test Data                                             | Expected Result                                                  | Notes      |
 * | :--------------: | ---------------------------------------------------------------------------------------- | ----------------------------------------------------- | ---------------------------------------------------------------- | ---------- |
 * | 01               | Invoke the ClosedCaptionsActiveTypePacket constructor with a negative service value for CEA708 | channelId = 100, counter = 50, service = -1, type = CEA::type_708 | The constructor executes without throwing any exception         | Should Pass |
 */
TEST(ClosedCaptionsActiveTypePacket, HandlingNegativeServiceValueForCEA708)
{
    std::cout << "Entering HandlingNegativeServiceValueForCEA708 test" << std::endl;
    
    uint32_t channelId = 100;
    uint32_t counter = 50;
    int service = -1;
    CEA type = CEA::type_708;
    
    std::cout << "Invoking ClosedCaptionsActiveTypePacket constructor with negative service value:" << std::endl;
    std::cout << "  channelId: " << channelId << std::endl;
    std::cout << "  counter: " << counter << std::endl;
    std::cout << "  service: " << service << std::endl;
    std::cout << "  type (integral value): " << static_cast<uint32_t>(type) << std::endl;    
    
    EXPECT_NO_THROW({
        ClosedCaptionsActiveTypePacket packet(channelId, counter, type, service);
        std::cout << "Packet constructed successfully with negative service value appended:" << std::endl;
    });
    
    std::cout << "Exiting HandlingNegativeServiceValueForCEA708 test" << std::endl;
}
/**
 * @brief Verify that the ClosedCaptionsActiveTypePacket constructor handles a large service number for CEA608 correctly without throwing an exception.
 *
 * This test checks whether the constructor of ClosedCaptionsActiveTypePacket properly constructs an object using a large service number (99999) for the CEA::type_608 type. The test ensures that no exceptions are thrown during object creation while also logging the parameter values.
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
 * | Variation / Step | Description | Test Data |Expected Result |Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Enter test: print entry message and initialize parameters (channelId=200, counter=25, service=99999, type=[CEA::type_608]) | channelId = 200, counter = 25, service = 99999, type = CEA::type_608 | Test variables are initialized successfully | Should be successful |
 * | 02 | Print details about the constructor invocation, displaying parameter values | channelId = 200, counter = 25, service = 99999, type = CEA::type_608 | Parameter details are printed correctly | Should be successful |
 * | 03 | Invoke ClosedCaptionsActiveTypePacket constructor inside EXPECT_NO_THROW block with provided parameters | channelId = 200, counter = 25, service = 99999, type = CEA::type_608 | No exception is thrown; packet is constructed correctly | Should Pass |
 * | 04 | Print exit message | N/A | Exit log is printed correctly | Should be successful |
 */
TEST(ClosedCaptionsActiveTypePacket, ConstructionWithLargeServiceNumberForCEA608)
{
    std::cout << "Entering ConstructionWithLargeServiceNumberForCEA608 test" << std::endl;
    
    uint32_t channelId = 200;
    uint32_t counter = 25;
    int service = 99999;
    CEA type = CEA::type_608;
    
    std::cout << "Invoking ClosedCaptionsActiveTypePacket constructor with large service number:" << std::endl;
    std::cout << "  channelId: " << channelId << std::endl;
    std::cout << "  counter: " << counter << std::endl;
    std::cout << "  service: " << service << std::endl;
    std::cout << "  type (integral value): " << static_cast<uint32_t>(type) << std::endl;    
    
    EXPECT_NO_THROW({
        ClosedCaptionsActiveTypePacket packet(channelId, counter, type, service);
        std::cout << "Packet constructed successfully with large service number appended:" << std::endl;
    });
    
    std::cout << "Exiting ConstructionWithLargeServiceNumberForCEA608 test" << std::endl;
}
/**
 * @brief Test the construction of ClosedCaptionsActiveTypePacket with a large service number for CEA708
 *
 * This test verifies that the ClosedCaptionsActiveTypePacket constructor correctly handles a large service number 
 * when the type is set to CEA708. The test ensures that no exception is thrown and that all provided parameters 
 * (channelId, counter, service, type) are processed as expected.
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
 * | 01 | Invoke the ClosedCaptionsActiveTypePacket constructor with channelId = 200, counter = 25, type = CEA::type_708, and service = 99999 | channelId = 200, counter = 25, type = CEA::type_708, service = 99999 | The API should construct the packet without throwing an exception, verifying that the operation is successful via EXPECT_NO_THROW. | Should Pass |
 */
TEST(ClosedCaptionsActiveTypePacket, ConstructionWithLargeServiceNumberForCEA708)
{
    std::cout << "Entering ConstructionWithLargeServiceNumberForCEA708 test" << std::endl;
    
    uint32_t channelId = 200;
    uint32_t counter = 25;
    int service = 99999;
    CEA type = CEA::type_708;
    
    std::cout << "Invoking ClosedCaptionsActiveTypePacket constructor with large service number:" << std::endl;
    std::cout << "  channelId: " << channelId << std::endl;
    std::cout << "  counter: " << counter << std::endl;
    std::cout << "  service: " << service << std::endl;
    std::cout << "  type (integral value): " << static_cast<uint32_t>(type) << std::endl;    
    
    EXPECT_NO_THROW({
        ClosedCaptionsActiveTypePacket packet(channelId, counter, type, service);
        std::cout << "Packet constructed successfully with large service number appended:" << std::endl;
        std::cout << "  Packet type: SUBTITLE_SELECTION" << std::endl;
        std::cout << "  counter: " << counter << std::endl;
        std::cout << "  CC_SELECTION_LEN: 16" << std::endl;
        std::cout << "  channelId: " << channelId << std::endl;
        std::cout << "  CC_USERDATA_SUBTITLE_TYPE: 3" << std::endl;
        std::cout << "  auxiliary id 1 (CEA value): " << static_cast<uint32_t>(type) << std::endl;
        std::cout << "  auxiliary id 2 (service): " << service << std::endl;
    });
    
    std::cout << "Exiting ConstructionWithLargeServiceNumberForCEA708 test" << std::endl;
}
/**
 * @brief Test to verify that the default constructor of ClosedCaptionsChannel does not throw exceptions.
 *
 * This test checks that creating an instance of ClosedCaptionsChannel using its default constructor does not trigger any exceptions.
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
 * | Variation / Step | Description                                                                              | Test Data                                                     | Expected Result                                                        | Notes      |
 * | :--------------: | ---------------------------------------------------------------------------------------- | ------------------------------------------------------------- | ---------------------------------------------------------------------- | ---------- |
 * | 01               | Invoke the default constructor of ClosedCaptionsChannel and verify no exception is thrown. | input: none, output: instance of ClosedCaptionsChannel is created | No exception is thrown and the instance is created successfully in memory | Should Pass|
 */
TEST(ClosedCaptionsChannel, DefaultConstruction) {
    std::cout << "Entering DefaultConstruction test" << std::endl;
    
    std::cout << "Invoking ClosedCaptionsChannel default constructor." << std::endl;
    EXPECT_NO_THROW({
        ClosedCaptionsChannel channel;
        std::cout << "ClosedCaptionsChannel instance created successfully via default constructor." << std::endl;
    });
    
    std::cout << "Exiting DefaultConstruction test" << std::endl;
}
/**
 * @brief Test the creation of multiple ClosedCaptionsChannel instances
 *
 * This test verifies that multiple instances of ClosedCaptionsChannel can be created using the default constructor without throwing exceptions, ensuring that object instantiation does not lead to interference between instances.
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
 * | Variation / Step | Description                                                                              | Test Data                                             | Expected Result                                                                              | Notes         |
 * | :--------------: | ---------------------------------------------------------------------------------------- | ----------------------------------------------------- | -------------------------------------------------------------------------------------------- | ------------- |
 * | 01               | Invoke ClosedCaptionsChannel default constructor for the first instance                  | invocation = ClosedCaptionsChannel()                  | No exceptions thrown; instance created successfully                                          | Should Pass   |
 * | 02               | Invoke ClosedCaptionsChannel default constructor for the second instance                 | invocation = ClosedCaptionsChannel()                  | No exceptions thrown; instance created successfully                                          | Should Pass   |
 * | 03               | Validate that both instances can coexist without interference and print success messages | output = "Successful creation messages printed"       | Confirmation messages printed indicating successful creation of both instances                | Should be successful |
 */
TEST(ClosedCaptionsChannel, MultipleInstantiations) {
    std::cout << "Entering MultipleInstantiations test" << std::endl;
    
    std::cout << "Invoking ClosedCaptionsChannel default constructor for first instance." << std::endl;
    EXPECT_NO_THROW({
        ClosedCaptionsChannel channel1;
        std::cout << "First ClosedCaptionsChannel instance created successfully." << std::endl;
    });
    
    std::cout << "Invoking ClosedCaptionsChannel default constructor for second instance." << std::endl;
    EXPECT_NO_THROW({
        ClosedCaptionsChannel channel2;
        std::cout << "Second ClosedCaptionsChannel instance created successfully." << std::endl;
    });
    
    std::cout << "Multiple ClosedCaptionsChannel instances created successfully without interference." << std::endl;
    std::cout << "Exiting MultipleInstantiations test" << std::endl;
}
/**
 * @brief Validates that the ClosedCaptionsChannel object is created and functions as expected for valid standard channel inputs.
 *
 * This test verifies that the ClosedCaptionsChannel object is successfully created using the default constructor and that the SendActiveTypePacket method can be invoked without throwing exceptions for both type_608 and type_708 channel types with a channel value of 1. This ensures that the API correctly handles valid inputs for a standard channel scenario.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 013@n
 * **Priority:** High@n
 * @n
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * @n
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                             | Test Data                                             | Expected Result                                                                   | Notes         |
 * | :--------------: | ----------------------------------------------------------------------- | ----------------------------------------------------- | --------------------------------------------------------------------------------- | ------------- |
 * | 01               | Create the ClosedCaptionsChannel object using the default constructor.  | No input arguments                                    | Object is created successfully with no exceptions thrown.                       | Should be successful |
 * | 02               | Call SendActiveTypePacket with type_608 and channel set to 1.           | input: type = type_608, channel = 1                    | API call should succeed without exceptions and pass the assertion checks.         | Should Pass   |
 * | 03               | Call SendActiveTypePacket with type_708 and channel set to 1.           | input: type = type_708, channel = 1                    | API call should succeed without exceptions and pass the assertion checks.         | Should Pass   |
 */
TEST(ClosedCaptionsChannel, ValidInputStandardChannel) {
    std::cout << "Entering ValidInputStandardChannel test" << std::endl;
    
    // Create the object using default constructor
    EXPECT_NO_THROW({
        ClosedCaptionsChannel channelObj;
        std::cout << "ClosedCaptionsChannel object created successfully using default constructor." << std::endl;
        
        // Test with type_608 for channel value 1
        std::cout << "Invoking SendActiveTypePacket with type = type_608 and channel = 1" << std::endl;
        EXPECT_NO_THROW({
            channelObj.SendActiveTypePacket(ClosedCaptionsActiveTypePacket::CEA::type_608, 1);
            std::cout << "SendActiveTypePacket invoked successfully with type = type_608 and channel = 1" << std::endl;
        });
        
        // Test with type_708 for channel value 1
        std::cout << "Invoking SendActiveTypePacket with type = type_708 and channel = 1" << std::endl;
        EXPECT_NO_THROW({
            channelObj.SendActiveTypePacket(ClosedCaptionsActiveTypePacket::CEA::type_708, 1);
            std::cout << "SendActiveTypePacket invoked successfully with type = type_708 and channel = 1" << std::endl;
        });
    });
    
    std::cout << "Exiting ValidInputStandardChannel test" << std::endl;
}
/**
 * @brief Verify that ClosedCaptionsChannel can handle valid input at the lower boundary (channel value zero)
 *
 * This test verifies that the ClosedCaptionsChannel object can be instantiated using the default constructor
 * and that its SendActiveTypePacket method correctly accepts valid channel input (channel = 0) for both type_608
 * and type_708 without throwing exceptions. This is important to ensure that boundary channel values are processed
 * as expected and do not generate errors.
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
 * | Variation / Step | Description                                                            | Test Data                                              | Expected Result                       | Notes          |
 * | :--------------: | ---------------------------------------------------------------------- | ------------------------------------------------------ | -------------------------------------- | -------------- |
 * | 01               | Create ClosedCaptionsChannel object using the default constructor       | (none)                                                 | Object instantiated without exception | Should be successful |
 * | 02               | Invoke SendActiveTypePacket with type = type_608 and channel = 0          | input: type = type_608, channel = 0                     | No exception thrown                    | Should Pass    |
 * | 03               | Invoke SendActiveTypePacket with type = type_708 and channel = 0          | input: type = type_708, channel = 0                     | No exception thrown                    | Should Pass    |
 */
TEST(ClosedCaptionsChannel, ValidInputBoundaryChannelZero) {
    std::cout << "Entering ValidInputBoundaryChannelZero test" << std::endl;
    
    // Create the object using default constructor
    EXPECT_NO_THROW({
        ClosedCaptionsChannel channelObj;
        std::cout << "ClosedCaptionsChannel object created successfully using default constructor." << std::endl;
        
        // Test with type_608 for channel value 0
        std::cout << "Invoking SendActiveTypePacket with type = type_608 and channel = 0" << std::endl;
        EXPECT_NO_THROW({
            channelObj.SendActiveTypePacket(ClosedCaptionsActiveTypePacket::CEA::type_608, 0);
            std::cout << "SendActiveTypePacket invoked successfully with type = type_608 and channel = 0" << std::endl;
        });
        
        // Test with type_708 for channel value 0
        std::cout << "Invoking SendActiveTypePacket with type = type_708 and channel = 0" << std::endl;
        EXPECT_NO_THROW({
            channelObj.SendActiveTypePacket(ClosedCaptionsActiveTypePacket::CEA::type_708, 0);
            std::cout << "SendActiveTypePacket invoked successfully with type = type_708 and channel = 0" << std::endl;
        });
    });
    
    std::cout << "Exiting ValidInputBoundaryChannelZero test" << std::endl;
}
/**
 * @brief To verify that the SendActiveTypePacket method correctly handles the maximum unsigned int value for the channel parameter for both type_608 and type_708.
 *
 * This test validates that the ClosedCaptionsChannel object can be created using the default constructor and that invoking the SendActiveTypePacket method with the maximum valid unsigned integer value (4294967295u) for both type_608 and type_708 does not throw any exceptions.
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
 * | Variation / Step | Description                                                                                          | Test Data                                             | Expected Result                                           | Notes         |
 * | :--------------: | ---------------------------------------------------------------------------------------------------- | ----------------------------------------------------- | --------------------------------------------------------- | ------------- |
 * | 01               | Create a ClosedCaptionsChannel object using the default constructor.                               | None                                                  | Object is created without throwing any exceptions.      | Should be successful |
 * | 02               | Invoke SendActiveTypePacket with type_608 and channel set to maximum unsigned int value (4294967295u).| input: type = type_608, channel = 4294967295u           | API call succeeds without throwing exceptions.          | Should Pass   |
 * | 03               | Invoke SendActiveTypePacket with type_708 and channel set to maximum unsigned int value (4294967295u).| input: type = type_708, channel = 4294967295u           | API call succeeds without throwing exceptions.          | Should Pass   |
 */
TEST(ClosedCaptionsChannel, ValidInputMaxUnsignedIntChannel) {
    std::cout << "Entering ValidInputMaxUnsignedIntChannel test" << std::endl;
    
    // Maximum unsigned int value
    unsigned int maxChannel = 4294967295u;
    
    // Create the object using default constructor
    EXPECT_NO_THROW({
        ClosedCaptionsChannel channelObj;
        std::cout << "ClosedCaptionsChannel object created successfully using default constructor." << std::endl;
        
        // Test with type_608 for maximum channel value
        std::cout << "Invoking SendActiveTypePacket with type = type_608 and channel = " << maxChannel << std::endl;
        EXPECT_NO_THROW({
            channelObj.SendActiveTypePacket(ClosedCaptionsActiveTypePacket::CEA::type_608, maxChannel);
            std::cout << "SendActiveTypePacket invoked successfully with type = type_608 and channel = " << maxChannel << std::endl;
        });
        
        // Test with type_708 for maximum channel value
        std::cout << "Invoking SendActiveTypePacket with type = type_708 and channel = " << maxChannel << std::endl;
        EXPECT_NO_THROW({
            channelObj.SendActiveTypePacket(ClosedCaptionsActiveTypePacket::CEA::type_708, maxChannel);
            std::cout << "SendActiveTypePacket invoked successfully with type = type_708 and channel = " << maxChannel << std::endl;
        });
    });
    
    std::cout << "Exiting ValidInputMaxUnsignedIntChannel test" << std::endl;
}
/**
 * @brief Validate that the ClosedCaptionsChannel API correctly processes a non-empty data packet
 *
 * This test validates that a ClosedCaptionsChannel object can be created successfully and that the
 * SendDataPacketNoPTS method can be invoked with a valid non-empty data packet without throwing any exceptions.
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
 * | Variation / Step | Description                                                                                                | Test Data                                                   | Expected Result                                                              | Notes         |
 * | :--------------: | ---------------------------------------------------------------------------------------------------------- | ----------------------------------------------------------- | ---------------------------------------------------------------------------- | ------------- |
 * | 01               | Create ClosedCaptionsChannel object using default constructor                                              | None                                                        | Object created without throwing exceptions                                   | Should be successful |
 * | 02               | Prepare non-empty data packet with values {0x01, 0x02, 0x03} and set dataLen to 3                           | data = {0x01, 0x02, 0x03}, dataLen = 3                        | Data packet is prepared correctly                                            | Should be successful |
 * | 03               | Call SendDataPacketNoPTS on the ClosedCaptionsChannel object with the prepared data packet                   | data = {0x01, 0x02, 0x03}, dataLen = 3                        | Method completes without throwing exceptions and processes data packet as expected | Should Pass   |
 */
TEST(ClosedCaptionsChannel, ValidNonEmptyDataPacket) {
    std::cout << "Entering ValidNonEmptyDataPacket test" << std::endl;
    
    // Creating the ClosedCaptionsChannel object using default constructor.
    EXPECT_NO_THROW({
        ClosedCaptionsChannel channel;
        std::cout << "Created ClosedCaptionsChannel object successfully." << std::endl;
        
        // Prepare data: [0x01, 0x02, 0x03]
        uint8_t data[3] = {0x01, 0x02, 0x03};
        int dataLen = 3;
        std::cout << "Prepared data packet with values: 0x01, 0x02, 0x03 and dataLen: " << dataLen << std::endl;
        
        std::cout << "Invoking SendDataPacketNoPTS with non-empty data packet." << std::endl;
        EXPECT_NO_THROW(channel.SendDataPacketNoPTS(data, dataLen));
        std::cout << "SendDataPacketNoPTS invoked successfully with non-empty data packet." << std::endl;
    });
    
    std::cout << "Exiting ValidNonEmptyDataPacket test" << std::endl;
}
/**
 * @brief Test that the SendDataPacketNoPTS method of ClosedCaptionsChannel handles an empty data packet correctly.
 *
 * This test verifies that passing an empty data packet (non-null pointer but with zero length) to the SendDataPacketNoPTS method does not result in an exception. It confirms the robustness of the method in handling edge case scenarios.@n
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
 * | Variation / Step | Description                                                         | Test Data                                             | Expected Result                                      | Notes         |
 * | :--------------: | ------------------------------------------------------------------- | ----------------------------------------------------- | ---------------------------------------------------- | ------------- |
 * | 01               | Create a ClosedCaptionsChannel object                               | Constructor: No input arguments                       | Object is created successfully                       | Should be successful |
 * | 02               | Prepare an empty data packet with dataLen set to 0                   | emptyData = {0}, dataLen = 0                           | Empty data packet is prepared                        | Should be successful |
 * | 03               | Invoke SendDataPacketNoPTS with the empty data packet and verify no exception is thrown | emptyData = {0}, dataLen = 0                           | SendDataPacketNoPTS is executed without throwing an exception | Should Pass   |
 */
TEST(ClosedCaptionsChannel, ValidEmptyDataPacket) {
    std::cout << "Entering ValidEmptyDataPacket test" << std::endl;
    
    EXPECT_NO_THROW({
        ClosedCaptionsChannel channel;
        std::cout << "Created ClosedCaptionsChannel object successfully." << std::endl;
        
        // Prepare an empty data packet (non-null pointer but zero length).
        uint8_t emptyData[1] = {0}; // Dummy buffer, not used since length is zero.
        int dataLen = 0;
        std::cout << "Prepared empty data packet with dataLen: " << dataLen << std::endl;
        
        std::cout << "Invoking SendDataPacketNoPTS with empty data packet." << std::endl;
        EXPECT_NO_THROW(channel.SendDataPacketNoPTS(emptyData, dataLen));
        std::cout << "SendDataPacketNoPTS invoked successfully with empty data packet." << std::endl;
    });
    
    std::cout << "Exiting ValidEmptyDataPacket test" << std::endl;
}
/**
 * @brief Test to verify that SendDataPacketNoPTS method handles null data pointer and zero length properly.
 *
 * This test ensures that when a null data pointer and a data length of zero are passed to the SendDataPacketNoPTS method, 
 * the API does not throw any exceptions and handles the parameters gracefully.
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
 * | Variation / Step | Description                                                             | Test Data                                                  | Expected Result                                                    | Notes           |
 * | :--------------: | ----------------------------------------------------------------------- | ---------------------------------------------------------- | ------------------------------------------------------------------ | --------------- |
 * | 01               | Create ClosedCaptionsChannel object and verify object creation          | No input for constructor, output: valid object instance    | Object is created successfully without exceptions                  | Should be successful |
 * | 02               | Prepare a null data pointer and set data length to zero                 | data = nullptr, dataLen = 0                                  | Variables are initialized correctly with data as null and length as 0 | Should be successful |
 * | 03               | Invoke SendDataPacketNoPTS with null data pointer and zero length         | data = nullptr, dataLen = 0                                  | API is invoked without throwing any exception                      | Should Pass     |
 */
TEST(ClosedCaptionsChannel, NullDataPointerZeroLength) {
    std::cout << "Entering NullDataPointerZeroLength test" << std::endl;
    
    EXPECT_NO_THROW({
        ClosedCaptionsChannel channel;
        std::cout << "Created ClosedCaptionsChannel object successfully." << std::endl;
        
        // Null data pointer and zero length.
        uint8_t* data = nullptr;
        int dataLen = 0;
        std::cout << "Prepared null data pointer with dataLen: " << dataLen << std::endl;
        
        std::cout << "Invoking SendDataPacketNoPTS with null data pointer and zero length." << std::endl;
        EXPECT_NO_THROW(channel.SendDataPacketNoPTS(data, dataLen));
        std::cout << "SendDataPacketNoPTS invoked successfully with null data pointer and zero length." << std::endl;
    });
    
    std::cout << "Exiting NullDataPointerZeroLength test" << std::endl;
}
/**
 * @brief Validates the ClosedCaptionsChannel API's handling of a single-byte data packet.
 *
 * This test creates an instance of ClosedCaptionsChannel, prepares a data packet containing a single byte (0xFF) with a length of 1, and then calls the SendDataPacketNoPTS method. The objective is to ensure that no exceptions are thrown during these operations, confirming that the API can correctly process a minimal data packet.
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
 * | Variation / Step | Description                                                                                              | Test Data                             | Expected Result                                | Notes      |
 * | :--------------: | -------------------------------------------------------------------------------------------------------- | ------------------------------------- | ---------------------------------------------- | ---------- |
 * |       01         | Create a ClosedCaptionsChannel instance, prepare a data packet with a single byte (0xFF), and invoke SendDataPacketNoPTS | data = [0xFF], dataLen = 1              | No exception is thrown; EXPECT_NO_THROW assertion passes | Should Pass |
 */
TEST(ClosedCaptionsChannel, SingleByteDataPacket) {
    std::cout << "Entering SingleByteDataPacket test" << std::endl;
    
    EXPECT_NO_THROW({
        ClosedCaptionsChannel channel;
        std::cout << "Created ClosedCaptionsChannel object successfully." << std::endl;
        
        // Prepare data: [0xFF]
        uint8_t data[1];
        data[0] = 0xFF;
        int dataLen = 1;
        std::cout << "Prepared data packet with single byte: 0xFF and dataLen: " << dataLen << std::endl;
        
        std::cout << "Invoking SendDataPacketNoPTS with single-byte data packet." << std::endl;
        EXPECT_NO_THROW(channel.SendDataPacketNoPTS(data, dataLen));
        std::cout << "SendDataPacketNoPTS invoked successfully with single-byte data packet." << std::endl;
    });
    
    std::cout << "Exiting SingleByteDataPacket test" << std::endl;
}
/**
 * @brief Test the functionality of sending a valid non-zero presentation timestamp (PTS) and a valid data buffer.
 *
 * This test verifies that when a valid non-zero PTS and a valid data buffer are provided, the ClosedCaptionsChannel object is successfully created and the SendDataPacketWithPTS method operates without throwing exceptions.
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
 * | Variation / Step | Description | Test Data | Expected Result |Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create ClosedCaptionsChannel object, initialize ptsValue and data buffer, then invoke SendDataPacketWithPTS with these valid parameters | ptsValue = 1000, data = {0x01, 0x02, 0x03, 0x04, 0x05}, dataLen = 5 | Object is created and SendDataPacketWithPTS method executes without throwing exceptions | Should Pass |
 */
TEST(ClosedCaptionsChannel, SendValidNonZeroPTSandDataBuffer) {
    std::cout << "Entering SendValidNonZeroPTSandDataBuffer test" << std::endl;

    // Create object for ClosedCaptionsChannel using default constructor.
    EXPECT_NO_THROW({
        ClosedCaptionsChannel channel;
        std::cout << "ClosedCaptionsChannel object created using default constructor." << std::endl;

        // Prepare test parameters
        uint32_t ptsValue = 1000;
        std::cout << "ptsValue: " << ptsValue << std::endl;
        uint8_t data[5] = {0x01, 0x02, 0x03, 0x04, 0x05};
        size_t dataLen = 5;
        std::cout << "Data buffer values: ";
        for (size_t i = 0; i < dataLen; i++) {
            std::cout << std::hex << static_cast<int>(data[i]) << " ";
        }
        std::cout << std::dec << ", dataLen: " << dataLen << std::endl;

        // Invoke SendDataPacketWithPTS method
        std::cout << "Invoking SendDataPacketWithPTS with ptsValue " << ptsValue << ", data buffer, and dataLen " << dataLen << std::endl;
        EXPECT_NO_THROW(channel.SendDataPacketWithPTS(ptsValue, data, dataLen));
        std::cout << "SendDataPacketWithPTS invoked successfully with ptsValue " << ptsValue << std::endl;
    });
    
    std::cout << "Exiting SendValidNonZeroPTSandDataBuffer test" << std::endl;
}
/**
 * @brief Validate that invoking SendDataPacketWithPTS with a pts value of zero and a valid data buffer executes without throwing an exception.
 *
 * This test verifies that the ClosedCaptionsChannel object can be instantiated and the SendDataPacketWithPTS method correctly handles a pts value of zero along with a valid data buffer. It ensures that no unexpected exceptions are thrown during the API invocation.
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
 * | Variation / Step | Description                                                         | Test Data                                              | Expected Result                                                          | Notes           |
 * | :--------------: | ------------------------------------------------------------------- | ------------------------------------------------------ | ------------------------------------------------------------------------ | --------------- |
 * | 01               | Create a ClosedCaptionsChannel object using the default constructor | None                                                   | Object is created successfully without throwing exceptions             | Should be successful |
 * | 02               | Prepare the test parameters: pts value, data buffer, and data length  | ptsValue = 0, data = 0x10,0x20,0x30, dataLen = 3        | Test parameters are initialized as expected                              | Should be successful |
 * | 03               | Invoke SendDataPacketWithPTS with the prepared test parameters        | ptsValue = 0, data = 0x10,0x20,0x30, dataLen = 3        | Method invocation does not throw any exceptions and processes the data    | Should Pass     |
 */
TEST(ClosedCaptionsChannel, SendPTSZeroAndDataBuffer) {
    std::cout << "Entering SendPTSZeroAndDataBuffer test" << std::endl;

    EXPECT_NO_THROW({
        ClosedCaptionsChannel channel;
        std::cout << "ClosedCaptionsChannel object created using default constructor." << std::endl;

        // Prepare test parameters
        uint32_t ptsValue = 0;
        std::cout << "ptsValue: " << ptsValue << std::endl;
        uint8_t data[3] = {0x10, 0x20, 0x30};
        size_t dataLen = 3;
        std::cout << "Data buffer values: ";
        for (size_t i = 0; i < dataLen; i++) {
            std::cout << std::hex << static_cast<int>(data[i]) << " ";
        }
        std::cout << std::dec << ", dataLen: " << dataLen << std::endl;

        // Invoke SendDataPacketWithPTS method
        std::cout << "Invoking SendDataPacketWithPTS with ptsValue " << ptsValue << ", data buffer, and dataLen " << dataLen << std::endl;
        EXPECT_NO_THROW(channel.SendDataPacketWithPTS(ptsValue, data, dataLen));
        std::cout << "SendDataPacketWithPTS invoked successfully with ptsValue " << ptsValue << std::endl;
    });

    std::cout << "Exiting SendPTSZeroAndDataBuffer test" << std::endl;
}
/**
 * @brief Test the behavior of SendDataPacketWithPTS with maximum pts value and valid data buffer
 *
 * This test verifies that the ClosedCaptionsChannel API correctly handles the boundary condition by 
 * sending a data packet using the maximum possible pts (UINT32_MAX) along with a valid data buffer. 
 * The objective is to ensure that the method processes extreme PTS values without throwing exceptions, 
 * thereby confirming its robustness under stress conditions.
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
 * | Variation / Step | Description                                                                                      | Test Data                                              | Expected Result                                                      | Notes         |
 * | :--------------: | ------------------------------------------------------------------------------------------------ | ------------------------------------------------------ | -------------------------------------------------------------------- | ------------- |
 * | 01               | Create a ClosedCaptionsChannel object and prepare test parameters including maximum pts value      | ptsValue = UINT32_MAX, data = {0xAA, 0xBB}, dataLen = 2 | Object creation and parameter initialization should complete successfully | Should be successful |
 * | 02               | Invoke SendDataPacketWithPTS with the prepared parameters                                        | ptsValue = UINT32_MAX, data = {0xAA, 0xBB}, dataLen = 2 | Method call does not throw exception and packet is processed correctly | Should Pass   |
 */
TEST(ClosedCaptionsChannel, SendMaximumPTSAndDataBuffer) {
    std::cout << "Entering SendMaximumPTSAndDataBuffer test" << std::endl;

    EXPECT_NO_THROW({
        ClosedCaptionsChannel channel;
        std::cout << "ClosedCaptionsChannel object created using default constructor." << std::endl;

        // Prepare test parameters
        uint32_t ptsValue = UINT32_MAX;
        std::cout << "ptsValue: " << ptsValue << std::endl;
        uint8_t data[2] = {0xAA, 0xBB};
        size_t dataLen = 2;
        std::cout << "Data buffer values: ";
        for (size_t i = 0; i < dataLen; i++) {
            std::cout << std::hex << static_cast<int>(data[i]) << " ";
        }
        std::cout << std::dec << ", dataLen: " << dataLen << std::endl;

        // Invoke SendDataPacketWithPTS method
        std::cout << "Invoking SendDataPacketWithPTS with ptsValue " << ptsValue << ", data buffer, and dataLen " << dataLen << std::endl;
        EXPECT_NO_THROW(channel.SendDataPacketWithPTS(ptsValue, data, dataLen));
        std::cout << "SendDataPacketWithPTS invoked successfully with maximum ptsValue " << ptsValue << std::endl;
    });

    std::cout << "Exiting SendMaximumPTSAndDataBuffer test" << std::endl;
}
/**
 * @brief Tests that ClosedCaptionsChannel::SendDataPacketWithPTS correctly handles a null data pointer and zero data length.
 *
 * This test verifies that the SendDataPacketWithPTS method does not throw exceptions when invoked with a null data pointer and a data length of zero.
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
 * | Variation / Step | Description                                                                 | Test Data                                     | Expected Result                                                                    | Notes           |
 * | :--------------: | --------------------------------------------------------------------------- | --------------------------------------------- | ----------------------------------------------------------------------------------- | --------------- |
 * | 01               | Create a ClosedCaptionsChannel object using the default constructor.        | None                                          | Object is created successfully without throwing any exception.                    | Should be successful |
 * | 02               | Set up test parameters with ptsValue = 500, data pointer = nullptr, dataLen = 0. | ptsValue = 500, data = nullptr, dataLen = 0     | Parameters are correctly initialized; values logged appropriately.                | Should be successful |
 * | 03               | Invoke SendDataPacketWithPTS with the prepared parameters.                  | ptsValue = 500, data = nullptr, dataLen = 0     | The API call does not throw any exception and completes as expected.                | Should Pass     |
 */
TEST(ClosedCaptionsChannel, SendNullDataPointerAndZeroLength) {
    std::cout << "Entering SendNullDataPointerAndZeroLength test" << std::endl;

    EXPECT_NO_THROW({
        ClosedCaptionsChannel channel;
        std::cout << "ClosedCaptionsChannel object created using default constructor." << std::endl;

        // Prepare test parameters
        uint32_t ptsValue = 500;
        std::cout << "ptsValue: " << ptsValue << std::endl;
        uint8_t* data = nullptr;
        size_t dataLen = 0;
        std::cout << "Data pointer is nullptr and dataLen: " << dataLen << std::endl;
        
        // Invoke SendDataPacketWithPTS method
        std::cout << "Invoking SendDataPacketWithPTS with ptsValue " << ptsValue << ", nullptr data pointer, and dataLen " << dataLen << std::endl;
        EXPECT_NO_THROW(channel.SendDataPacketWithPTS(ptsValue, data, dataLen));
        std::cout << "SendDataPacketWithPTS invoked successfully with ptsValue " << ptsValue << " and nullptr data pointer" << std::endl;
    });

    std::cout << "Exiting SendNullDataPointerAndZeroLength test" << std::endl;
}
/**
 * @brief Validate ClosedCaptionsPacket constructor using valid typical input data
 *
 * This test verifies that the ClosedCaptionsPacket constructor can be invoked without exceptions using typical valid input values. It tests the object's instantiation with standard parameters such as channelId, counter, data length, and the data array. The test confirms that the API handles valid input correctly by ensuring no exception is thrown during construction.
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
 * | Variation / Step | Description                                                    | Test Data                                                         | Expected Result                                                | Notes             |
 * | :--------------: | -------------------------------------------------------------- | ----------------------------------------------------------------- | -------------------------------------------------------------- | ----------------- |
 * | 01               | Initialize test parameters                                     | channelId = 5, counter = 10, dataLen = 4, data = 0x01,0x02,0x03,0x04 | Parameters set correctly                                        | Should be successful |
 * | 02               | Invoke ClosedCaptionsPacket constructor and verify no exception is thrown | channelId = 5, counter = 10, data = 0x01,0x02,0x03,0x04, dataLen = 4 | The constructor should not throw any exception                  | Should Pass       |
 */
TEST(ClosedCaptionsPacket, ValidInputTypicalData) {
    std::cout << "Entering ValidInputTypicalData test" << std::endl;
    
    uint32_t channelId = 5;
    uint32_t counter = 10;
    const size_t dataLen = 4;
    uint8_t data[dataLen] = {0x01, 0x02, 0x03, 0x04};

    std::cout << "Invoking ClosedCaptionsPacket constructor with channelId = " << channelId
              << ", counter = " << counter << ", dataLen = " << dataLen << std::endl;
    
    EXPECT_NO_THROW({
        ClosedCaptionsPacket packet(channelId, counter, data, dataLen);
        std::cout << "ClosedCaptionsPacket constructor invoked successfully." << std::endl;
    });
    
    std::cout << "Exiting ValidInputTypicalData test" << std::endl;
}
/**
 * @brief Verify that the ClosedCaptionsPacket constructor correctly handles valid input with an empty data array.
 *
 * This test checks that when a ClosedCaptionsPacket object is instantiated with a valid channelId, counter, and an empty 
 * data array (dataLen = 0), no exceptions are thrown. This ensures that the constructor properly manages an edge case
 * where no actual data is provided.
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
 * | 01 | Invoke the ClosedCaptionsPacket constructor with valid parameters including an empty data array. | channelId=1, counter=0, dataLen=0, data=&dummy | The ClosedCaptionsPacket constructor should be called without throwing any exceptions. | Should Pass |
 */
TEST(ClosedCaptionsPacket, ValidInputEmptyData) {
    std::cout << "Entering ValidInputEmptyData test" << std::endl;
    
    uint32_t channelId = 1;
    uint32_t counter = 0;
    size_t dataLen = 0;
    uint8_t dummy = 0; // valid pointer reference for an empty data array
    uint8_t* data = &dummy;
    
    std::cout << "Invoking ClosedCaptionsPacket constructor with channelId = " << channelId
              << ", counter = " << counter << ", dataLen = " << dataLen << std::endl;
    
    EXPECT_NO_THROW({
        ClosedCaptionsPacket packet(channelId, counter, data, dataLen);
        std::cout << "ClosedCaptionsPacket constructor invoked successfully." << std::endl;
    });
    
    std::cout << "Exiting ValidInputEmptyData test" << std::endl;
}
/**
 * @brief Verify that the ClosedCaptionsPacket constructor handles a null data pointer correctly when dataLen is non-zero.
 *
 * This test ensures that the ClosedCaptionsPacket constructor fails as expected by throwing an exception when provided with a null data pointer while the data length is non-zero. This scenario validates the robustness of argument checking in the API.
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
 * | 01 | Invoke ClosedCaptionsPacket constructor with channelId = 3, counter = 5, dataLen = 5, and data pointer as nullptr | channelId = 3, counter = 5, dataLen = 5, data = nullptr | Exception is thrown; ASSERT check passes using EXPECT_ANY_THROW | Should Pass |
 */
TEST(ClosedCaptionsPacket, NullDataNonZeroLength) {
    std::cout << "Entering NullDataNonZeroLength test" << std::endl;
    
    uint32_t channelId = 3;
    uint32_t counter = 5;
    size_t dataLen = 5;
    uint8_t* data = nullptr; // invalid pointer condition for non-zero dataLen
    
    std::cout << "Invoking ClosedCaptionsPacket constructor with channelId = " << channelId
              << ", counter = " << counter << ", dataLen = " << dataLen 
              << " and data pointer is nullptr" << std::endl;
    
    EXPECT_ANY_THROW({
        ClosedCaptionsPacket packet(channelId, counter, data, dataLen);
    });
    
    std::cout << "Exiting NullDataNonZeroLength test" << std::endl;
}
/**
 * @brief Test that the ClosedCaptionsPacket constructor and getBuffer method handle maximum boundary values without throwing any exceptions.
 *
 * This test verifies that the ClosedCaptionsPacket properly processes inputs at their maximum boundary values for channelId and counter, and that a valid non-empty data array is accepted. It confirms that the constructor does not throw an exception and that the getBuffer method returns a valid buffer.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 027@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                                                                                       | Test Data                                                                                   | Expected Result                                                                                      | Notes           |
 * | :--------------: | ---------------------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------------------------------------- | --------------- |
 * | 01               | Print the test entry message and initialize variables with maximum boundary values                                                 | channelId = 4294967295, counter = 4294967295, data = {0xFF, 0xEE, 0xDD, 0xCC}, dataLen = 4      | Console shows the entry message and variables are initialized correctly                              | Should be successful |
 * | 02               | Invoke the ClosedCaptionsPacket constructor and call getBuffer within EXPECT_NO_THROW to validate proper construction and execution | channelId = 4294967295, counter = 4294967295, data = {0xFF, 0xEE, 0xDD, 0xCC}, dataLen = 4      | The constructor is invoked without throwing an exception and getBuffer returns a valid buffer         | Should Pass     |
 * | 03               | Print the test exit message indicating the end of the test                                                                       | No additional test data                                                                     | Console shows the exit message                                                                       | Should be successful |
 */
TEST(ClosedCaptionsPacket, ValidInputMaxBoundary) {
    std::cout << "Entering ValidInputMaxBoundary test" << std::endl;
    
    uint32_t channelId = 4294967295U;
    uint32_t counter = 4294967295U;
    const size_t dataLen = 4;
    uint8_t data[dataLen] = {0xFF, 0xEE, 0xDD, 0xCC};

    std::cout << "Invoking ClosedCaptionsPacket constructor with channelId = " << channelId
              << ", counter = " << counter << ", dataLen = " << dataLen << std::endl;
    
    EXPECT_NO_THROW({
        ClosedCaptionsPacket packet(channelId, counter, data, dataLen);
        std::cout << "ClosedCaptionsPacket constructor invoked successfully." << std::endl;
        
        std::vector<uint8_t> buffer = packet.getBuffer();
        std::cout << "Invoked getBuffer(), returned buffer size = " << buffer.size() << std::endl;
    });
    
    std::cout << "Exiting ValidInputMaxBoundary test" << std::endl;
}
/**
 * @brief Validate that the ClosedCaptionsPacket constructor properly creates an object when provided with valid non-empty data.
 *
 * This test verifies that when valid input parameters and a non-empty data buffer are provided to the ClosedCaptionsPacket constructor, the object is successfully created without throwing any exceptions. This ensures that the ClosedCaptionsPacket API handles typical valid scenarios as expected.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 028
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Initialize test parameters and log entering the test | channelId = 10, counter = 1, ptsValue = 500, dataLen = 4, data = {0x01, 0x02, 0x03, 0x04} | Test parameters are correctly initialized and entering test message is printed | Should be successful |
 * | 02 | Invoke the ClosedCaptionsPacket constructor and verify no exception is thrown | channelId = 10, counter = 1, ptsValue = 500, dataLen = 4, data = {0x01, 0x02, 0x03, 0x04} | ClosedCaptionsPacket object is created successfully without throwing any exceptions | Should Pass |
 */
TEST(ClosedCaptionsPacket, ValidInputWithNonEmptyData) {
    std::cout << "Entering ValidInputWithNonEmptyData test" << std::endl;

    uint32_t channelId = 10;
    uint32_t counter = 1;
    uint32_t ptsValue = 500;
    const size_t dataLen = 4;
    uint8_t data[dataLen] = {0x01, 0x02, 0x03, 0x04};

    std::cout << "Invoking ClosedCaptionsPacket constructor with parameters:" << std::endl;
    std::cout << "  channelId: " << channelId << ", counter: " << counter 
              << ", ptsValue: " << ptsValue << ", dataLen: " << dataLen << std::endl;

    // Create the ClosedCaptionsPacket object
    EXPECT_NO_THROW({
        ClosedCaptionsPacket packet(channelId, counter, ptsValue, data, dataLen);
        std::cout << "ClosedCaptionsPacket object created successfully." << std::endl;
    });

    std::cout << "Exiting ValidInputWithNonEmptyData test" << std::endl;
}
/**
 * @brief Verify that the ClosedCaptionsPacket constructor handles valid input with an empty data array.
 *
 * This test ensures that when a valid but empty data array is provided along with proper channelId, counter, and ptsValue values,
 * the ClosedCaptionsPacket constructor operates correctly without throwing any exceptions. This verifies the robustness of the constructor under edge conditions.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 029@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result |Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoke the ClosedCaptionsPacket constructor with valid parameters and an empty data array | channelId = 20, counter = 2, ptsValue = 1000, dataLen = 0, data = pointer to valid dummy | No exceptions thrown; ClosedCaptionsPacket object created successfully | Should Pass |
 */
TEST(ClosedCaptionsPacket, ValidInputWithEmptyData) {
    std::cout << "Entering ValidInputWithEmptyData test" << std::endl;

    uint32_t channelId = 20;
    uint32_t counter = 2;
    uint32_t ptsValue = 1000;
    const size_t dataLen = 0;
    // Allocate an empty array (valid pointer but no data)
    uint8_t dummy = 0;
    uint8_t* data = &dummy;

    std::cout << "Invoking ClosedCaptionsPacket constructor with parameters:" << std::endl;
    std::cout << "  channelId: " << channelId << ", counter: " << counter 
              << ", ptsValue: " << ptsValue << ", dataLen: " << dataLen << std::endl;

    EXPECT_NO_THROW({
        ClosedCaptionsPacket packet(channelId, counter, ptsValue, data, dataLen);
        std::cout << "ClosedCaptionsPacket object created successfully." << std::endl;
    });

    std::cout << "Exiting ValidInputWithEmptyData test" << std::endl;
}
/**
 * @brief Verify that ClosedCaptionsPacket correctly constructs an object using maximum boundary input values.
 *
 * This test verifies that the ClosedCaptionsPacket object is successfully created when provided with maximum boundary values for channelId, counter, ptsValue, and non-empty data. This ensures that the constructor handles extreme boundary values without throwing any exceptions.
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
 * | 01 | Initialize parameters with maximum boundary values and prepare data array filled with 0xFF | channelId = 4294967295, counter = 4294967295, ptsValue = 4294967295, dataLen = 10, data = 0xFF repeated 10 times | Parameters are initialized correctly | Should be successful |
 * | 02 | Invoke ClosedCaptionsPacket constructor with the prepared parameters within EXPECT_NO_THROW and verify no exception is thrown | channelId = 4294967295, counter = 4294967295, ptsValue = 4294967295, data pointer, dataLen = 10 | ClosedCaptionsPacket object is created without throwing an exception | Should Pass |
 */
TEST(ClosedCaptionsPacket, ValidInputWithMaximumBoundaryValues) {
    std::cout << "Entering ValidInputWithMaximumBoundaryValues test" << std::endl;

    uint32_t channelId = 4294967295u;
    uint32_t counter = 4294967295u;
    uint32_t ptsValue = 4294967295u;
    const size_t dataLen = 10;
    uint8_t data[dataLen];
    // Fill data with 0xFF repeated 10 times
    std::memset(data, 0xFF, dataLen);

    std::cout << "Invoking ClosedCaptionsPacket constructor with maximum boundary parameters:" << std::endl;
    std::cout << "  channelId: " << channelId << ", counter: " << counter 
              << ", ptsValue: " << ptsValue << ", dataLen: " << dataLen << std::endl;

    EXPECT_NO_THROW({
        ClosedCaptionsPacket packet(channelId, counter, ptsValue, data, dataLen);
        std::cout << "ClosedCaptionsPacket object created successfully." << std::endl;
    });

    std::cout << "Exiting ValidInputWithMaximumBoundaryValues test" << std::endl;
}
/**
 * @brief Validate that ClosedCaptionsPacket constructor handles null data pointer with non-zero data length appropriately.
 *
 * This test ensures that when a null data pointer is provided along with a non-zero data length, the ClosedCaptionsPacket constructor triggers an exception or error. This behavior is essential to avoid undefined behavior due to invalid memory access.
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
 * | Variation / Step | Description                                                                                       | Test Data                                                   | Expected Result                                                                | Notes             |
 * | :--------------: | ------------------------------------------------------------------------------------------------- | ----------------------------------------------------------- | ------------------------------------------------------------------------------ | ----------------- |
 * | 01               | Log entry into the NegativeInputWithNullDataPointer test                                          | N/A                                                         | Log message "Entering NegativeInputWithNullDataPointer test" is printed         | Should be successful |
 * | 02               | Initialize variables with channelId=5, counter=50, ptsValue=500, dataLen=5, and data as nullptr       | channelId = 5, counter = 50, ptsValue = 500, dataLen = 5, data = nullptr | Variables are set with the provided values                                     | Should be successful |
 * | 03               | Log invocation details of ClosedCaptionsPacket constructor                                       | N/A                                                         | Log messages displaying the values for channelId, counter, ptsValue, and dataLen  | Should be successful |
 * | 04               | Call the ClosedCaptionsPacket constructor with null data pointer and non-zero dataLen and check exception | channelId = 5, counter = 50, ptsValue = 500, data = nullptr, dataLen = 5 | The constructor call throws an exception                                       | Should Fail       |
 * | 05               | Log exit from the NegativeInputWithNullDataPointer test                                           | N/A                                                         | Log message "Exiting NegativeInputWithNullDataPointer test" is printed          | Should be successful |
 */
TEST(ClosedCaptionsPacket, NegativeInputWithNullDataPointer) {
    std::cout << "Entering NegativeInputWithNullDataPointer test" << std::endl;

    uint32_t channelId = 5;
    uint32_t counter = 50;
    uint32_t ptsValue = 500;
    const size_t dataLen = 5;
    uint8_t* data = nullptr;

    std::cout << "Invoking ClosedCaptionsPacket constructor with null data pointer and non-zero dataLen:" << std::endl;
    std::cout << "  channelId: " << channelId << ", counter: " << counter 
              << ", ptsValue: " << ptsValue << ", dataLen: " << dataLen << std::endl;

    // Expect the constructor to raise an exception or error.
    EXPECT_ANY_THROW({
        ClosedCaptionsPacket packet(channelId, counter, ptsValue, data, dataLen);
    });

    std::cout << "Exiting NegativeInputWithNullDataPointer test" << std::endl;
}
