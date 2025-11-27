
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
#include "SubtecConnector.h"

using namespace subtecConnector::ccMgrAPI;
using namespace subtecConnector;


// Test Case: Successfully close an open subtecConnector instance
/**
 * @brief Test to verify that the subtecConnector's close() method executes without throwing exceptions when invoked on an open instance.
 *
 * This test ensures that upon calling the close() method of the subtecConnector, no exception is thrown. It helps validate that the proper cleanup mechanism is in place when closing an instance.
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
 * | Variation / Step | Description                                                      | Test Data                                                  | Expected Result                                               | Notes            |
 * | :--------------: | ---------------------------------------------------------------- | ---------------------------------------------------------- | ------------------------------------------------------------- | ---------------- |
 * | 01               | Log the entry into the test case                                  | N/A                                                        | Entry message printed to console                              | Should be successful |
 * | 02               | Invoke the close() method on the subtecConnector instance           | No input arguments, method invoked as: subtecConnector::close() | Method completes without throwing an exception (EXPECT_NO_THROW check) | Should Pass      |
 * | 03               | Log the exit from the test case                                   | N/A                                                        | Exit message printed to console                               | Should be successful |
 */
TEST(SubtecConnectorTest, SuccessfullyCloseAnOpenInstance) {
    std::cout << "Entering Successfully close an open subtecConnector instance test" << std::endl;

    std::cout << "Invoking close() method" << std::endl;
    EXPECT_NO_THROW({
        subtecConnector::close();
        std::cout << "close() method invoked successfully" << std::endl;
    });
    
    std::cout << "Exiting Successfully close an open subtecConnector instance test" << std::endl;
}
/**
 * @brief Verify initHal function returns success for valid handle pointer input
 *
 * This test verifies that the initHal function properly initializes the HAL when provided with a valid handle pointer. It ensures that the function returns a success code (0) when a valid pointer is used.
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
 * | Variation / Step | Description                                              | Test Data                                         | Expected Result                                         | Notes       |
 * | :--------------: | -------------------------------------------------------- | ------------------------------------------------- | ------------------------------------------------------- | ----------- |
 * | 01               | Create a valid handle pointer and invoke initHal function| validData = 42, validHandle = address of validData, output result = 0 | initHal should return 0 indicating success and EXPECT_EQ(result, 0) | Should Pass |
 */
TEST(InitHalTest, ValidHandlePointerInput) {
    std::cout << "Entering Valid handle pointer input test" << std::endl;
    
    // Create a valid handle, here using an integer variable as dummy handle
    int validData = 42;
    void* validHandle = static_cast<void*>(&validData);
    std::cout << "Valid handle pointer created with address: " << validHandle << std::endl;
    // Invoke the method
    std::cout << "Invoking initHal with valid handle pointer: " << validHandle << std::endl;
    mrcc_Error result = subtecConnector::initHal(validHandle);
    std::cout << "initHal returned: " << result << std::endl;
    
    // Check the expected result (0 indicates success)
    EXPECT_EQ(result, 0);
    std::cout << "Exiting Valid handle pointer input test" << std::endl;
}
/**
 * @brief Tests initHal API's behavior when provided with a null handle pointer.
 *
 * This test verifies that calling the initHal function with a null handle pointer returns an error (non-zero value), ensuring that the API correctly handles invalid input.
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
 * | Variation / Step | Description                                         | Test Data                                           | Expected Result                                                                                   | Notes              |
 * | :--------------: | --------------------------------------------------- | --------------------------------------------------- | -------------------------------------------------------------------------------------------------- | ------------------ |
 * | 01               | Prepare a null handle pointer for input             | nullHandle = NULL                                   | nullHandle is set to NULL                                                                           | Should be successful |
 * | 02               | Invoke initHal with the null handle pointer         | input: nullHandle = NULL, output: result             | The API returns a non-zero error value and EXPECT_NE(result, 0) assertion passes                     | Should Fail        |
 */
TEST(InitHalTest, NullHandlePointerInput) {

    std::cout << "Entering Null handle pointer input test" << std::endl;
    // Prepare a null handle pointer
    void* nullHandle = NULL;
    std::cout << "Null handle pointer prepared with value: " << nullHandle << std::endl;
    // Invoke the method
    std::cout << "Invoking initHal with null handle pointer: " << nullHandle << std::endl;
    mrcc_Error result = subtecConnector::initHal(nullHandle);
    std::cout << "initHal returned: " << result << std::endl;
    // Check the expected failure result (non-zero indicates error)
    EXPECT_NE(result, 0);    

    std::cout << "Exiting Null handle pointer input test" << std::endl;
}
/**
 * @brief Test to verify the successful initialization of the packet sender.
 *
 * This test checks whether the initPacketSender API initializes the packet sender successfully by returning 0.
 * It verifies that no exceptions are thrown during the function call and that the returned error value is as expected.
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
 * | Variation / Step | Description                                                                                     | Test Data                                               | Expected Result                                                                                     | Notes       |
 * | :--------------: | ----------------------------------------------------------------------------------------------- | ------------------------------------------------------- | --------------------------------------------------------------------------------------------------- | ----------- |
 * | 01               | Invoke initPacketSender() and check that it returns 0 without throwing exceptions.              | output: error initial = -1, API call: initPacketSender(), expected error = 0 | initPacketSender() returns 0 and no exception is thrown; assertion passes                              | Should Pass |
 */
TEST(InitPacketSenderTest, SuccessfulInitializationOfPacketSender) {
    std::cout << "Entering SuccessfulInitializationOfPacketSender test" << std::endl;
    
    // Invocation of initPacketSender()
    std::cout << "Invoking initPacketSender()" << std::endl;
    mrcc_Error error = -1;
    EXPECT_NO_THROW({
        error = subtecConnector::initPacketSender();
        std::cout << "initPacketSender() returned: " << error << std::endl;
    });
    // Check that returned value is 0
    EXPECT_EQ(error, 0) << "Expected initPacketSender() to return 0 for successful initialization, but got " << error;
    
    std::cout << "Exiting SuccessfulInitializationOfPacketSender test" << std::endl;
}
/**
 * @brief Test that the resetChannel API executes successfully without throwing exceptions
 *
 * This test verifies that the subtecConnector::resetChannel() method can be called and executed properly, ensuring its behavior is as expected by not throwing any exceptions during execution.
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
 * | Variation / Step | Description                                             | Test Data                                                          | Expected Result                                                                 | Notes       |
 * | :--------------: | ------------------------------------------------------- | ------------------------------------------------------------------ | ------------------------------------------------------------------------------- | ----------- |
 * | 01               | Invoke resetChannel() to ensure no exceptions are thrown | input: none, output: none, API: subtecConnector::resetChannel()      | API executes successfully without throwing exceptions and completes normally | Should Pass |
 */
TEST(ResetChannelTest, SuccessfulChannelReset) {
    std::cout << "Entering SuccessfulChannelReset test" << std::endl;
    
    // Invoke the resetChannel() method and ensure it executes without throwing any exception
    std::cout << "Invoking resetChannel() method." << std::endl;
    EXPECT_NO_THROW({
        subtecConnector::resetChannel();
        std::cout << "resetChannel() invoked successfully; no exception thrown." << std::endl;
    });
    
    // Since resetChannel() does not return any value, we log that its invocation is complete.
    std::cout << "Channel reset operation completed." << std::endl;
    std::cout << "Exiting SuccessfulChannelReset test" << std::endl;
}
/**
 * @brief Validates that ccGetAttributes returns success and correctly populates the attributes structure when provided with a valid pointer and valid enum values.
 *
 * This test iterates over valid closed caption type values (GSW_CC_TYPE_ANALOG and GSW_CC_TYPE_DIGITAL), calling the ccGetAttributes API with each enum value. It verifies that the API returns MRCC_SUCCESS (assumed to be 0) and that the attributes structure is properly populated for each valid input.
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
 * | 01 | Log the entry message indicating the start of the test | N/A | "Entering CCGetAttributes_PositiveTest test" message logged | Should be successful |
 * | 02 | Iterate over valid ccType values and invoke ccGetAttributes API | For each iteration: input: attrib pointer, ccType = GSW_CC_TYPE_ANALOG or GSW_CC_TYPE_DIGITAL; output: error value from API | API returns MRCC_SUCCESS (0) and attrib structure is populated; Assertion EXPECT_EQ(error, 0) passes | Should Pass |
 * | 03 | Log the exit message after test completion | N/A | "Exiting CCGetAttributes_PositiveTest test" message logged | Should be successful |
 */
TEST(CCGetAttributes_PositiveTest, ValidPointerAndEnum) 
{
    std::cout << "Entering CCGetAttributes_PositiveTest test" << std::endl;
    gsw_CcAttributes attrib;
    // Iterate over valid ccType values: GSW_CC_TYPE_ANALOG and GSW_CC_TYPE_DIGITAL.
    for (int ccTypeValue = GSW_CC_TYPE_ANALOG; ccTypeValue < GSW_CC_TYPE_MAX; ++ccTypeValue) {
        gsw_CcType ccType = static_cast<gsw_CcType>(ccTypeValue);
        std::cout << "Invoking ccGetAttributes with ccType value: " << ccTypeValue << std::endl;
        mrcc_Error error = subtecConnector::ccMgrAPI::ccGetAttributes(&attrib, ccType);
        std::cout << "Method ccGetAttributes returned: " << error << std::endl;
        // Assuming MRCC_SUCCESS is 0 and that on success the 'attrib' structure is populated
        EXPECT_EQ(error, 0);
        std::cout << "ccGetAttributes populated attrib with closed caption display attributes for ccType: " << ccTypeValue << std::endl;
    }
    std::cout << "Exiting CCGetAttributes_PositiveTest test" << std::endl;
}
/**
 * @brief Test ccGetAttributes API with a NULL pointer to verify proper error handling
 *
 * This test verifies that calling ccGetAttributes with a NULL attrib pointer returns a non-zero error code, which is expected as the API should handle the null pointer case gracefully.
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
 * | Variation / Step | Description                                                                       | Test Data                                                       | Expected Result                                           | Notes       |
 * | :--------------: | --------------------------------------------------------------------------------- | ---------------------------------------------------------------- | --------------------------------------------------------- | ----------- |
 * | 01               | Invoke ccGetAttributes with attrib pointer set to nullptr and valid ccType value.   | attribPtr = nullptr, ccType = GSW_CC_TYPE_ANALOG                 | API returns a non-zero error code (error != 0), assertion passes | Should Fail |
 */
TEST(CCGetAttributes_NegativeTest, NullPointerAttrib) {
    std::cout << "Entering CCGetAttributes_NegativeTest_NullPointer test" << std::endl;

    // Here, we are passing a NULL pointer for attrib.
    gsw_CcAttributes* attribPtr = nullptr;
    std::cout << "Invoking ccGetAttributes with attrib pointer: NULL and ccType: " << GSW_CC_TYPE_ANALOG << std::endl;
    mrcc_Error error = subtecConnector::ccMgrAPI::ccGetAttributes(attribPtr, gsw_CcType::GSW_CC_TYPE_ANALOG);
    std::cout << "Method ccGetAttributes returned: " << error << std::endl;
    
    // Check that the error code indicates a null pointer error.
    // Here we assume any non-zero error code indicates an error.
    EXPECT_NE(error, 0);

    std::cout << "Exiting CCGetAttributes_NegativeTest_NullPointer test" << std::endl;
}
/**
 * @brief To validate ccGetAttributes API returns an error when provided an invalid closed caption type.
 *
 * This test verifies that when an out-of-range ccType value is provided to ccGetAttributes, the API returns an error code different from 0. The test ensures that the API correctly handles invalid input parameters.
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
 * | :----: | --------- | ---------- | -------------- | ----- |
 * | 01 | Initialize a valid gsw_CcAttributes structure and set an invalid ccType value | attrib = valid gsw_CcAttributes, invalidCcType = 5 | The attribute structure is initialized and invalidCcType is assigned a value outside of the defined enum range | Should be successful |
 * | 02 | Invoke ccGetAttributes API with the valid attrib pointer and invalid ccType | Input: attrib pointer, ccType = 5, Output: error (mrcc_Error) | The API returns an error code indicating invalid closed caption type | Should Fail |
 * | 03 | Assert that the returned error code is non-zero | Error code from API call (error != 0) | The assertion passes confirming that the error code is not 0 | Should be successful |
 */
TEST(CCGetAttributes_NegativeTest, InvalidCcTypeValue) {
    std::cout << "Entering CCGetAttributes_NegativeTest_InvalidCcType test" << std::endl;
    // Create a valid gsw_CcAttributes structure object.
    gsw_CcAttributes attrib;

    // Use an invalid ccType value (5 is out of defined enum range).
    int invalidCcType = 5;
    std::cout << "Invoking ccGetAttributes with valid attrib pointer and invalid ccType value: " << invalidCcType << std::endl;
    mrcc_Error error = subtecConnector::ccMgrAPI::ccGetAttributes(&attrib, static_cast<gsw_CcType>(invalidCcType));
    std::cout << "Method ccGetAttributes returned: " << error << std::endl;
    
    // Check that the error code indicates an invalid closed caption type.
    EXPECT_NE(error, 0);
    
    std::cout << "Exiting CCGetAttributes_NegativeTest_InvalidCcType test" << std::endl;
}
/**
 * @brief Verify that ccGetCapability API returns success for valid attribute and closed caption types.
 *
 * This test iterates through a list of valid attribute types and closed caption types, invokes the ccGetCapability API with a fixed-size buffer, and checks if the returned error code is MRCC_SUCCESS. The test ensures that the API properly handles valid input parameters and returns expected capability data.
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
 * | Variation / Step | Description                                                                                 | Test Data                                                                                                  | Expected Result                                        | Notes      |
 * | :--------------: | ------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------------------------------------------- | ------------------------------------------------------ | ---------- |
 * | 01               | Loop through each attribType and ccType, prepare a fixed-size capability buffer, and invoke ccGetCapability API | attribType = GSW_CC_ATTRIB_FONT_COLOR, GSW_CC_ATTRIB_BACKGROUND_COLOR, etc., ccType = GSW_CC_TYPE_ANALOG, GSW_CC_TYPE_DIGITAL, value = pointer to capabilityBuffer (initialized with empty string), size = 0 | API returns MRCC_SUCCESS for each valid API call      | Should Pass|
 */
TEST(CCGetCapabilityTest, PositiveTest)
{
    std::cout << "Entering PositiveTest test" << std::endl;

    gsw_CcAttribType attribTypes[] = {
        GSW_CC_ATTRIB_FONT_COLOR,
        GSW_CC_ATTRIB_BACKGROUND_COLOR,
        GSW_CC_ATTRIB_FONT_OPACITY,
        GSW_CC_ATTRIB_BACKGROUND_OPACITY,
        GSW_CC_ATTRIB_FONT_STYLE,
        GSW_CC_ATTRIB_FONT_SIZE,
        GSW_CC_ATTRIB_FONT_ITALIC,
        GSW_CC_ATTRIB_FONT_UNDERLINE,
        GSW_CC_ATTRIB_BORDER_TYPE,
        GSW_CC_ATTRIB_BORDER_COLOR,
        GSW_CC_ATTRIB_WIN_COLOR,
        GSW_CC_ATTRIB_WIN_OPACITY,
        GSW_CC_ATTRIB_EDGE_TYPE,
        GSW_CC_ATTRIB_EDGE_COLOR
    };
    gsw_CcType ccTypes[] = { GSW_CC_TYPE_ANALOG, GSW_CC_TYPE_DIGITAL };

    for (size_t i = 0; i < sizeof(attribTypes)/sizeof(attribTypes[0]); i++)
    {
        for (size_t j = 0; j < sizeof(ccTypes)/sizeof(ccTypes[0]); j++)
        {
            // Prepare a fixed-size buffer for capability data.
            char capabilityBuffer[100] = { 0 };
            // Use strncpy to initialize the buffer (demonstration of using strncpy for fixed-size arrays)
            strncpy(capabilityBuffer, "", sizeof(capabilityBuffer) - 1);
            void* value = reinterpret_cast<void*>(capabilityBuffer);
            unsigned int size = 0;

            std::cout << "Invoking ccGetCapability with attribType: " << attribTypes[i]
                      << " and ccType: " << ccTypes[j] << std::endl;
            mrcc_Error ret = subtecConnector::ccMgrAPI::ccGetCapability(attribTypes[i], ccTypes[j], &value, &size);
            std::cout << "Method ccGetCapability returned error code: " << ret << std::endl;
            std::cout << "Returned capability size: " << size << std::endl;
            std::cout << "Returned capability data: " << static_cast<char*>(value) << std::endl;
            EXPECT_EQ(ret, MRCC_SUCCESS);
        }
    }
    std::cout << "Exiting PositiveTest test" << std::endl;
}
/**
 * @brief Verify that ccGetCapability API returns an error when passed a NULL value pointer
 *
 * This test checks the behavior of the ccGetCapability API when the value pointer argument is NULL.
 * It ensures that the API does not succeed (i.e., returns an error code different from MRCC_SUCCESS)
 * when an invalid input is provided, which is important for robust error handling.
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
 * | 01 | Call ccGetCapability API with attribType set to GSW_CC_ATTRIB_FONT_COLOR, ccType set to GSW_CC_TYPE_ANALOG, NULL pointer for the value, and valid pointer for the size variable | attribType = GSW_CC_ATTRIB_FONT_COLOR, ccType = GSW_CC_TYPE_ANALOG, value pointer = NULL, size pointer = &size | The API should return an error code that is not MRCC_SUCCESS | Should Fail |
 */
TEST(CCGetCapabilityTest, NullValuePointerTest)
{
    std::cout << "Entering NullValuePointerTest test" << std::endl;
    unsigned int size = 0;

    std::cout << "Invoking ccGetCapability with attribType: " << GSW_CC_ATTRIB_FONT_COLOR
              << ", ccType: " << GSW_CC_TYPE_ANALOG << " and NULL value pointer" << std::endl;
    mrcc_Error ret = subtecConnector::ccMgrAPI::ccGetCapability(gsw_CcAttribType::GSW_CC_ATTRIB_FONT_COLOR, gsw_CcType::GSW_CC_TYPE_ANALOG, NULL, &size);
    std::cout << "Method ccGetCapability returned error code: " << ret << std::endl;
    EXPECT_NE(ret, MRCC_SUCCESS);
    std::cout << "Exiting NullValuePointerTest test" << std::endl;
}
/**
 * @brief Verify ccGetCapability returns an error when provided with a NULL size pointer
 *
 * This test verifies that the ccGetCapability API correctly handles a NULL size pointer by returning an error code instead of MRCC_SUCCESS.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Call ccGetCapability with a valid capability buffer pointer and a NULL size pointer | attribType = GSW_CC_ATTRIB_FONT_COLOR, ccType = GSW_CC_TYPE_DIGITAL, value pointer = capabilityBuffer, size pointer = NULL | API returns an error (ret != MRCC_SUCCESS); Assertion validates error code | Should Fail |
 */
TEST(CCGetCapabilityTest, NullSizePointerTest)
{
    std::cout << "Entering NullSizePointerTest test" << std::endl;
    char capabilityBuffer[100] = { 0 };
    // Use strncpy to assign an empty string to the fixed size array (demonstration)
    strncpy(capabilityBuffer, "", sizeof(capabilityBuffer) - 1);
    void* value = reinterpret_cast<void*>(capabilityBuffer);

    std::cout << "Invoking ccGetCapability with attribType: " << GSW_CC_ATTRIB_FONT_COLOR
              << ", ccType: " << GSW_CC_TYPE_DIGITAL << " and NULL size pointer" << std::endl;
    mrcc_Error ret = subtecConnector::ccMgrAPI::ccGetCapability(gsw_CcAttribType::GSW_CC_ATTRIB_FONT_COLOR, gsw_CcType::GSW_CC_TYPE_DIGITAL, &value, NULL);
    std::cout << "Method ccGetCapability returned error code: " << ret << std::endl;
    EXPECT_NE(ret, MRCC_SUCCESS);
    std::cout << "Exiting NullSizePointerTest test" << std::endl;
}
/**
 * @brief Test the ccGetCapability API with an invalid attribute type.
 *
 * This test verifies that calling ccGetCapability with an invalid attribute type,
 * specifically one greater than GSW_CC_ATTRIB_MAX, results in a failure (i.e., the API does not return MRCC_SUCCESS).
 * The API is expected to perform input validation and return an error code accordingly.
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
 * | Variation / Step | Description                                                                                                      | Test Data                                                                                                          | Expected Result                                              | Notes      |
 * | :--------------: | ---------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------ | ------------------------------------------------------------ | ---------- |
 * | 01               | Prepare a capability buffer and set an invalid attribute type (GSW_CC_ATTRIB_MAX + 1) and valid ccType.         | capabilityBuffer = empty, invalidAttribType = (GSW_CC_ATTRIB_MAX + 1), ccType = GSW_CC_TYPE_DIGITAL, value pointer, size pointer | API returns an error code not equal to MRCC_SUCCESS and the assertion EXPECT_NE(ret, MRCC_SUCCESS) passes | Should Fail |
 */
TEST(CCGetCapabilityTest, InvalidAttribTypeTest)
{
    std::cout << "Entering InvalidAttribTypeTest test" << std::endl;
    char capabilityBuffer[100] = { 0 };
    strncpy(capabilityBuffer, "", sizeof(capabilityBuffer) - 1);
    void* value = reinterpret_cast<void*>(capabilityBuffer);
    unsigned int size = 0;

    // Invalid attribute type: one greater than GSW_CC_ATTRIB_MAX
    gsw_CcAttribType invalidAttribType = static_cast<gsw_CcAttribType>(GSW_CC_ATTRIB_MAX + 1);

    std::cout << "Invoking ccGetCapability with invalid attribType: " << invalidAttribType
              << " and ccType: " << GSW_CC_TYPE_DIGITAL << std::endl;
    mrcc_Error ret = subtecConnector::ccMgrAPI::ccGetCapability(invalidAttribType, gsw_CcType::GSW_CC_TYPE_DIGITAL, &value, &size);
    std::cout << "Method ccGetCapability returned error code: " << ret << std::endl;
    EXPECT_NE(ret, MRCC_SUCCESS);
    std::cout << "Exiting InvalidAttribTypeTest test" << std::endl;
}
/**
 * @brief Verify that ccGetCapability handles an invalid ccType value properly.
 *
 * This test verifies that invoking ccGetCapability with an invalid ccType value (one greater than GSW_CC_TYPE_MAX)
 * returns an error code. The test ensures that the API correctly rejects the invalid input and does not return MRCC_SUCCESS.@n
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
 * | 01 | Initialize test variables: set up capabilityBuffer, value pointer, size, and define an invalid ccType value (GSW_CC_TYPE_MAX + 1) | capabilityBuffer = "", value = address of capabilityBuffer, size = 0, invalidCcType = GSW_CC_TYPE_MAX + 1 | Variables are initialized with the intended values before the API call | Should be successful |
 * | 02 | Invoke ccGetCapability API with attribType = GSW_CC_ATTRIB_BORDER_COLOR and the invalid ccType value | input1 = GSW_CC_ATTRIB_BORDER_COLOR, input2 = invalidCcType, input3 = value, input4 = size | API returns an error code (ret != MRCC_SUCCESS) indicating rejection of the invalid ccType | Should Fail |
 * | 03 | Log exiting message to indicate test completion | N/A | Log output confirms the test completed execution | Should be successful |
 */
TEST(CCGetCapabilityTest, InvalidCcTypeTest)
{
    std::cout << "Entering InvalidCcTypeTest test" << std::endl;

    char capabilityBuffer[100] = { 0 };
    strncpy(capabilityBuffer, "", sizeof(capabilityBuffer) - 1);
    void* value = reinterpret_cast<void*>(capabilityBuffer);
    unsigned int size = 0;

    // Invalid ccType value, one greater than GSW_CC_TYPE_MAX
    gsw_CcType invalidCcType = static_cast<gsw_CcType>(GSW_CC_TYPE_MAX + 1);

    std::cout << "Invoking ccGetCapability with attribType: " << GSW_CC_ATTRIB_BORDER_COLOR
              << " and invalid ccType: " << invalidCcType << std::endl;
    mrcc_Error ret = subtecConnector::ccMgrAPI::ccGetCapability(gsw_CcAttribType::GSW_CC_ATTRIB_BORDER_COLOR, invalidCcType, &value, &size);
    std::cout << "Method ccGetCapability returned error code: " << ret << std::endl;
    EXPECT_NE(ret, MRCC_SUCCESS);
    std::cout << "Exiting InvalidCcTypeTest test" << std::endl;
}
/**
 * @brief Test to verify that the closed captions are successfully hidden using the ccHide API
 *
 * This test validates that invoking the ccHide API under valid conditions returns a success status (MRCC_SUCCESS). It ensures that the closed captions feature is correctly disabled when the API is called.
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
 * | Variation / Step | Description                                                                | Test Data                                      | Expected Result                                         | Notes       |
 * | :--------------: | -------------------------------------------------------------------------- | ---------------------------------------------- | ------------------------------------------------------- | ----------- |
 * | 01               | Invoke the ccHide API and verify that its return value equals MRCC_SUCCESS   | input: none, output: result = MRCC_SUCCESS      | API returns MRCC_SUCCESS and the assertion check passes  | Should Pass |
 */
TEST(ccHideTests, SuccessfullyHideClosedCaptions) {
    std::cout << "Entering SuccessfullyHideClosedCaptions test" << std::endl;
    
    // Invocation of ccHide() method
    std::cout << "Invoking ccHide() method" << std::endl;
    mrcc_Error result = subtecConnector::ccMgrAPI::ccHide();
    std::cout << "ccHide() returned value: " << result << std::endl;
    
    // Verify the returned value equals MRCC_SUCCESS
    EXPECT_EQ(result, MRCC_SUCCESS);
    std::cout << "Exiting SuccessfullyHideClosedCaptions test" << std::endl;
}
/**
 * @brief Validate that ccSetAnalogChannel API sets the analog channel successfully.
 *
 * This test verifies that invoking ccSetAnalogChannel with a valid channel value (channel = 1)
 * executes without throwing any exceptions and returns the expected success code (MRCC_SUCCESS).
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
 * | Variation / Step | Description                                                     | Test Data                                      | Expected Result                                                       | Notes       |
 * | :--------------: | ---------------------------------------------------------------- | ---------------------------------------------- | --------------------------------------------------------------------- | ----------- |
 * | 01               | Invoke ccSetAnalogChannel API with a valid channel value of 1     | input: channel = 1, output: result = undefined | No exception is thrown and API returns MRCC_SUCCESS                     | Should Pass |
 * | 02               | Validate that the returned result matches the expected MRCC_SUCCESS | input: result from API call = MRCC_SUCCESS       | The returned value equals MRCC_SUCCESS as verified by EXPECT_EQ macro   | Should Pass |
 */
TEST(CCSetAnalogChannelTest, ValidAnalogChannel)
{
    std::cout << "Entering ValidAnalogChannel test" << std::endl;

    // Create the object for testing ccMgrAPI using default constructor.
    unsigned int channel = 1;
    std::cout << "Invoking ccSetAnalogChannel with channel value: " << channel << std::endl;
    mrcc_Error result = 0;
    EXPECT_NO_THROW({
        result = subtecConnector::ccMgrAPI::ccSetAnalogChannel(channel);
    });
    std::cout << "Returned value from ccSetAnalogChannel: " << result << std::endl;
    std::cout << "Comparing returned value to MRCC_SUCCESS (" << MRCC_SUCCESS << ")" << std::endl;
    EXPECT_EQ(result, MRCC_SUCCESS);
    std::cout << "Exiting ValidAnalogChannel test" << std::endl;
}
/**
 * @brief Test the ccSetAnalogChannel API with an invalid channel value of zero.
 *
 * This test verifies that when an invalid analog channel (channel = 0) is provided, the ccSetAnalogChannel API returns an error code different from MRCC_SUCCESS, ensuring proper error handling.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoke ccSetAnalogChannel with channel set to 0 and verify error handling | channel = 0, expected_return ≠ MRCC_SUCCESS | The API should return an error code (not MRCC_SUCCESS) and meet the assertion check | Should Fail |
 */
TEST(CCSetAnalogChannelTest, InvalidAnalogChannelZero)
{
    std::cout << "Entering InvalidAnalogChannelZero test" << std::endl;

    unsigned int channel = 0;
    std::cout << "Invoking ccSetAnalogChannel with invalid channel value: " << channel << std::endl;
    mrcc_Error result = 0;
    EXPECT_NO_THROW({
        result = subtecConnector::ccMgrAPI::ccSetAnalogChannel(channel);
    });
    std::cout << "Returned value from ccSetAnalogChannel: " << result << std::endl;

    std::cout << "Expecting an error code (not MRCC_SUCCESS = " << MRCC_SUCCESS << ")" << std::endl;
    EXPECT_NE(result, MRCC_SUCCESS);
    std::cout << "Exiting InvalidAnalogChannelZero test" << std::endl;
}
/**
 * @brief Test the behavior of ccSetAnalogChannel when provided with an excessively high channel value.
 *
 * This test verifies that calling ccSetAnalogChannel with a channel value set to UINT_MAX (an excessively high value)
 * does not cause an exception but returns an error code (i.e., a value not equal to MRCC_SUCCESS) indicating that the
 * input is out of the acceptable range.
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
 * | Variation / Step | Description                                                                                          | Test Data                                                | Expected Result                                                                                  | Notes            |
 * | :--------------: | ---------------------------------------------------------------------------------------------------- | -------------------------------------------------------- | ------------------------------------------------------------------------------------------------ | ---------------- |
 * | 01               | Initialize the channel variable with UINT_MAX to simulate an excessively high input.                | channel = UINT_MAX                                       | Variable 'channel' is set to UINT_MAX.                                                          | Should be successful |
 * | 02               | Invoke ccSetAnalogChannel API with the excessively high channel value and ensure no exception is thrown. | input: channel = UINT_MAX, output: result                | API call completes without throwing an exception.                                              | Should Fail      |
 * | 03               | Verify that the API returns an error code indicating that the channel value is out of range.          | result (returned from ccSetAnalogChannel)                | The return value is not equal to MRCC_SUCCESS.                                                   | Should Fail      |
 */
TEST(CCSetAnalogChannelTest, AnalogChannelExcessivelyHigh)
{
    std::cout << "Entering AnalogChannelExcessivelyHigh test" << std::endl;

    unsigned int channel = UINT_MAX;
    std::cout << "Invoking ccSetAnalogChannel with excessively high channel value: " << channel << std::endl;
    mrcc_Error result = 0;
    EXPECT_NO_THROW({
        result = subtecConnector::ccMgrAPI::ccSetAnalogChannel(channel);
    });
    std::cout << "Returned value from ccSetAnalogChannel: " << result << std::endl;

    std::cout << "Expecting an error code due to channel out of range (not MRCC_SUCCESS = " << MRCC_SUCCESS << ")" << std::endl;
    EXPECT_NE(result, MRCC_SUCCESS);

    std::cout << "Exiting AnalogChannelExcessivelyHigh test" << std::endl;
}
/**
 * @brief Validate that ccSetAttributes correctly configures digital captions.
 *
 * This test verifies that the ccSetAttributes API can handle a valid gsw_CcAttributes pointer,
 * a valid digital caption type, and various valid gsw_CcType enum values without throwing exceptions.
 * The API is expected to succeed (i.e., return MRCC_SUCCESS) for each valid invocation, ensuring that both
 * the input parsing and internal processing are correctly handled.
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
 * | Variation / Step | Description                                                                 | Test Data                                                         | Expected Result                                                                | Notes          |
 * | :--------------: | --------------------------------------------------------------------------- | ----------------------------------------------------------------- | ------------------------------------------------------------------------------ | -------------- |
 * | 01               | Create a valid gsw_CcAttributes object and set the type for digital captions  | attrib = valid gsw_CcAttributes object, type = 1                  | Object is created successfully and type is set correctly                       | Should be successful |
 * | 02               | Loop over valid gsw_CcType enum values and invoke ccSetAttributes for each      | attrib pointer = &attrib, type = 1, ccType = GSW_CC_TYPE_ANALOG, GSW_CC_TYPE_DIGITAL, GSW_CC_TYPE_MAX | Each API call returns MRCC_SUCCESS without throwing an exception               | Should Pass    |
 * | 03               | Validate that the API does not throw exceptions and returns the correct value | ret = output of ccSetAttributes call per iteration                | Return value equals MRCC_SUCCESS                                               | Should Pass    |
 */
TEST(CCSetAttributesDigital, ValidDigitalCaptions)
{
    std::cout << "Entering ValidDigitalCaptions test" << std::endl;

    // Create a valid gsw_CcAttributes object (structure assumed to be defined externally)
    gsw_CcAttributes attrib;
    // Set the type for digital captions
    short type = 1; // digital

    // Loop over valid enum values of gsw_CcType: GSW_CC_TYPE_ANALOG, GSW_CC_TYPE_DIGITAL, GSW_CC_TYPE_MAX
    gsw_CcType ccTypes[] = { GSW_CC_TYPE_ANALOG, GSW_CC_TYPE_DIGITAL, GSW_CC_TYPE_MAX };
    for (size_t i = 0; i < sizeof(ccTypes)/sizeof(ccTypes[0]); ++i)
    {
        std::cout << "Invoking ccSetAttributes with attrib pointer: " << &attrib
                  << ", type: " << type
                  << ", ccType: " << ccTypes[i] << std::endl;
        int ret = 0;
        EXPECT_NO_THROW({
            ret = subtecConnector::ccMgrAPI::ccSetAttributes(&attrib, type, ccTypes[i]);
        });
        std::cout << "Returned value: " << ret << std::endl;
        EXPECT_EQ(ret, MRCC_SUCCESS);
    }

    std::cout << "Exiting ValidDigitalCaptions test" << std::endl;
}
/**
 * @brief Validate the ccSetAttributes API with valid analog caption attributes.
 *
 * This test verifies that the ccSetAttributes API successfully handles a valid gsw_CcAttributes object with type set for analog captions across various enum values (GSW_CC_TYPE_ANALOG, GSW_CC_TYPE_DIGITAL, GSW_CC_TYPE_MAX). The test ensures no exceptions are thrown and the return value is MRCC_SUCCESS for all valid inputs.
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
 * | Variation / Step | Description                                                                                     | Test Data                                                            | Expected Result                                                             | Notes         |
 * | :--------------: | ----------------------------------------------------------------------------------------------- | -------------------------------------------------------------------- | --------------------------------------------------------------------------- | ------------- |
 * | 01               | Invoke ccSetAttributes with a valid attribute pointer, type set to analog (0), and ccType = GSW_CC_TYPE_ANALOG. | attrib pointer = valid address, type = 0, ccType = GSW_CC_TYPE_ANALOG   | API returns MRCC_SUCCESS without throwing exceptions.                     | Should Pass   |
 * | 02               | Invoke ccSetAttributes with a valid attribute pointer, type set to analog (0), and ccType = GSW_CC_TYPE_DIGITAL. | attrib pointer = valid address, type = 0, ccType = GSW_CC_TYPE_DIGITAL  | API returns MRCC_SUCCESS without throwing exceptions.                     | Should Pass   |
 * | 03               | Invoke ccSetAttributes with a valid attribute pointer, type set to analog (0), and ccType = GSW_CC_TYPE_MAX.    | attrib pointer = valid address, type = 0, ccType = GSW_CC_TYPE_MAX         | API returns MRCC_SUCCESS without throwing exceptions.                     | Should Pass   |
 */
TEST(CCSetAttributesAnalog, ValidAnalogCaptions)
{
    std::cout << "Entering ValidAnalogCaptions test" << std::endl;

    // Create a valid gsw_CcAttributes object (structure assumed to be defined externally)
    gsw_CcAttributes attrib;
    // Set the type for analog captions
    short type = 0; // analog
    // Loop over valid enum values of gsw_CcType: GSW_CC_TYPE_ANALOG, GSW_CC_TYPE_DIGITAL, GSW_CC_TYPE_MAX
    gsw_CcType ccTypes[] = { GSW_CC_TYPE_ANALOG, GSW_CC_TYPE_DIGITAL, GSW_CC_TYPE_MAX };
    for (size_t i = 0; i < sizeof(ccTypes)/sizeof(ccTypes[0]); ++i)
    {
        std::cout << "Invoking ccSetAttributes with attrib pointer: " << &attrib
                  << ", type: " << type
                  << ", ccType: " << ccTypes[i] << std::endl;
        int ret = 0;
        EXPECT_NO_THROW({
            ret = subtecConnector::ccMgrAPI::ccSetAttributes(&attrib, type, ccTypes[i]);
        });
        std::cout << "Returned value: " << ret << std::endl;
        EXPECT_EQ(ret, MRCC_SUCCESS);
    }

    std::cout << "Exiting ValidAnalogCaptions test" << std::endl;
}
/**
 * @brief Verify ccSetAttributes handles NULL attribute pointer appropriately.
 *
 * This test validates that the ccSetAttributes API properly handles a NULL attribute pointer by not performing the operation and returning an error code. By passing a NULL pointer with valid type values, the function's error handling mechanism is exercised to ensure robustness against invalid input.
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
 * | Variation / Step | Description                                                                                     | Test Data                                                      | Expected Result                                                                  | Notes       |  
 * | :--------------: | ------------------------------------------------------------------------------------------------ | -------------------------------------------------------------- | -------------------------------------------------------------------------------- | ----------- |  
 * | 01               | Invoke ccSetAttributes with a NULL attribute pointer, valid type (0), and valid ccType (GSW_CC_TYPE_ANALOG). | attrib = NULL, type = 0, ccType = GSW_CC_TYPE_ANALOG            | Function call returns a value not equal to MRCC_SUCCESS and no exception is thrown. | Should Fail |  
 */
TEST(CCSetAttributes_Negative, NullAttributePointer)
{
    std::cout << "Entering NullAttributePointer test" << std::endl;

    // Use NULL pointer for attribute structure
    gsw_CcAttributes* attrib = NULL;
    short type = 0; // analog type
    gsw_CcType ccType = GSW_CC_TYPE_ANALOG;

    std::cout << "Invoking ccSetAttributes with NULL attrib pointer, type: " << type
              << ", ccType: " << ccType << std::endl;
    int ret = 0;
    EXPECT_NO_THROW({
        ret = subtecConnector::ccMgrAPI::ccSetAttributes(attrib, type, ccType);
    });
    std::cout << "Returned value: " << ret << std::endl;
    EXPECT_NE(ret, MRCC_SUCCESS);

    std::cout << "Exiting NullAttributePointer test" << std::endl;
}
/**
 * @brief Verify that ccSetAttributes properly handles an invalid ccType boundary value.
 *
 * This test verifies that passing an out-of-bound value for the ccType parameter to the
 * ccSetAttributes API does not throw an exception and returns an error code. It ensures that
 * the API robustly handles invalid enumeration boundaries.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoke ccSetAttributes with a valid attribute pointer, valid type, and an invalid ccType boundary value | attrib pointer = valid address, type = 0, ccType = 5 | Return value not equal to MRCC_SUCCESS and no exception thrown | Should Fail |
 */
TEST(CCSetAttributes_Negative, InvalidCcTypeBoundary)
{
    std::cout << "Entering InvalidCcTypeBoundary test" << std::endl;

    // Create a valid gsw_CcAttributes object
    gsw_CcAttributes attrib;
    short type = 0; // analog type
    // Use an invalid ccType value (boundary value outside enum)
    gsw_CcType ccType = static_cast<gsw_CcType>(5);

    std::cout << "Invoking ccSetAttributes with attrib pointer: " << &attrib
              << ", type: " << type
              << ", ccType (invalid): " << static_cast<int>(ccType) << std::endl;
    int ret = 0;
    EXPECT_NO_THROW({
        ret = subtecConnector::ccMgrAPI::ccSetAttributes(&attrib, type, ccType);
    });
    std::cout << "Returned value: " << ret << std::endl;
    EXPECT_NE(ret, MRCC_SUCCESS);

    std::cout << "Exiting InvalidCcTypeBoundary test" << std::endl;
}
/**
 * @brief Verify that ccSetAttributes handles invalid type value appropriately
 *
 * This test verifies that the ccSetAttributes API function correctly identifies an invalid type value and returns an error code rather than MRCC_SUCCESS, ensuring proper error handling for negative input scenarios.
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
 * | Variation / Step | Description | Test Data |Expected Result |Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create a valid gsw_CcAttributes object and set an invalid type value | attrib = valid gsw_CcAttributes object, type = -1, ccType = GSW_CC_TYPE_DIGITAL | Object created successfully; invalid type is set | Should be successful |
 * | 02 | Invoke ccSetAttributes API with the invalid type value | pointer to attrib, type = -1, ccType = GSW_CC_TYPE_DIGITAL | API call does not throw an exception and returns an error code (not MRCC_SUCCESS) | Should Pass |
 * | 03 | Verify that the returned value indicates an error | ret = return value from API | ret != MRCC_SUCCESS | Should Pass |
 */
TEST(CCSetAttributes_Negative, InvalidTypeValue)
{
    std::cout << "Entering InvalidTypeValue test" << std::endl;

    // Create a valid gsw_CcAttributes object
    gsw_CcAttributes attrib;

    short type = -1; // invalid type value
    gsw_CcType ccType = GSW_CC_TYPE_DIGITAL;

    std::cout << "Invoking ccSetAttributes with attrib pointer: " << &attrib
              << ", invalid type: " << type
              << ", ccType: " << ccType << std::endl;
    int ret = 0;
    EXPECT_NO_THROW({
        ret = subtecConnector::ccMgrAPI::ccSetAttributes(&attrib, type, ccType);
    });
    std::cout << "Returned value: " << ret << std::endl;
    EXPECT_NE(ret, MRCC_SUCCESS);

    std::cout << "Exiting InvalidTypeValue test" << std::endl;
}
/**
 * @brief Test the behavior of ccSetDigitalChannel for the lowest valid channel value.
 *
 * This test verifies that ccSetDigitalChannel correctly sets the digital channel when provided with the minimum valid channel number (channel = 1).
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Call ccSetDigitalChannel with channel = 1 using default ccMgrAPI instance | channel = 1, expected return = MRCC_SUCCESS | Function returns MRCC_SUCCESS and assertion passes | Should Pass |
 */
TEST(CCSetDigitalChannelTest, ValidChannelLowerBound)
{
    std::cout << "Entering ValidChannelLowerBound test" << std::endl;
    // Create instance of ccMgrAPI using default constructor
    unsigned int channel = 1;
    std::cout << "Invoking ccSetDigitalChannel with channel value: " << channel << std::endl;
    
    // Call the method
    mrcc_Error result = subtecConnector::ccMgrAPI::ccSetDigitalChannel(channel);
    std::cout << "ccSetDigitalChannel returned value: " << result << std::endl;
    
    // Validate the result
    EXPECT_EQ(result, MRCC_SUCCESS);
    std::cout << "Exiting ValidChannelLowerBound test" << std::endl;
}
/**
 * @brief Validate ccSetDigitalChannel with a channel value below valid range
 *
 * This test verifies that the ccSetDigitalChannel API returns an error when provided with a channel value that is below the valid range. The test is important to ensure that the API properly handles invalid input and does not erroneously process invalid channel values.@n
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
 * | Variation / Step | Description                                                             | Test Data         | Expected Result                                                             | Notes       |
 * | :--------------: | ----------------------------------------------------------------------- | ----------------- | --------------------------------------------------------------------------- | ----------- |
 * | 01               | Invoke ccSetDigitalChannel with channel value 0 (below valid range)       | channel = 0       | API should return an error (result != MRCC_SUCCESS), as verified by EXPECT_NE | Should Fail |
 */
TEST(CCSetDigitalChannelTest, ChannelBelowValidRange)
{
    std::cout << "Entering ChannelBelowValidRange test" << std::endl;
    unsigned int channel = 0;
    std::cout << "Invoking ccSetDigitalChannel with channel value: " << channel << std::endl;
    
    // Call the method
    mrcc_Error result = subtecConnector::ccMgrAPI::ccSetDigitalChannel(channel);
    std::cout << "ccSetDigitalChannel returned value: " << result << std::endl;
    
    // Check that result is not MRCC_SUCCESS indicating an error for invalid channel.
    EXPECT_NE(result, MRCC_SUCCESS);
    std::cout << "Exiting ChannelBelowValidRange test" << std::endl;
}
/**
 * @brief Verify that ccSetDigitalChannel returns an error when given the maximum unsigned integer value.
 *
 * This test verifies that the ccSetDigitalChannel API method properly handles an invalid channel value by using UINT_MAX as input. The API is expected to return an error value (i.e., a value not equal to MRCC_SUCCESS) when the input channel exceeds valid limits.
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
 * | Variation / Step | Description                                                       | Test Data                                      | Expected Result                     | Notes       |
 * | :--------------: | ----------------------------------------------------------------- | ---------------------------------------------- | ----------------------------------- | ----------- |
 * | 01               | Call ccSetDigitalChannel with the channel value set to UINT_MAX     | input: channel = UINT_MAX, output: result      | result != MRCC_SUCCESS              | Should Pass |
 */
TEST(CCSetDigitalChannelTest, ChannelMaxUnsignedInt)
{
    std::cout << "Entering ChannelMaxUnsignedInt test" << std::endl;
    unsigned int channel = UINT_MAX;
    std::cout << "Invoking ccSetDigitalChannel with channel value: " << channel << std::endl;
    
    // Call the method
    mrcc_Error result = subtecConnector::ccMgrAPI::ccSetDigitalChannel(channel);
    std::cout << "ccSetDigitalChannel returned value: " << result << std::endl;
    
    // Check that result is not MRCC_SUCCESS indicating an error for invalid channel.
    EXPECT_NE(result, MRCC_SUCCESS);
    std::cout << "Exiting ChannelMaxUnsignedInt test" << std::endl;
}
/**
 * @brief Validates that ccShow() successfully displays closed captions
 *
 * This test case invokes the ccShow() method from ccMgrAPI to ensure that closed captions are displayed by checking if the returned value equals MRCC_SUCCESS. It verifies the functional correctness of the ccShow() API under nominal operating conditions.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 026
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                         | Test Data                                           | Expected Result                      | Notes      |
 * | :--------------: | --------------------------------------------------- | --------------------------------------------------- | ------------------------------------ | ---------- |
 * | 01               | Invoke ccShow() API and verify the returned value   | output1 = retVal, expected output1 = MRCC_SUCCESS    | retVal equals MRCC_SUCCESS           | Should Pass|
 */
TEST(CCShowTest, SuccessfullyShowClosedCaptions) {
    std::cout << "Entering SuccessfullyShowClosedCaptions test" << std::endl;
    // Invocation of ccShow method
    std::cout << "Invoking ccShow() method." << std::endl;
    mrcc_Error retVal = subtecConnector::ccMgrAPI::ccShow();
    std::cout << "ccShow() returned value: " << retVal << std::endl; 
    // Validate the output: expected MRCC_SUCCESS indicating closed captions are displayed successfully
    EXPECT_EQ(retVal, MRCC_SUCCESS);
    std::cout << "Closed captions displayed successfully. Expected MRCC_SUCCESS received." << std::endl;
    std::cout << "Exiting SuccessfullyShowClosedCaptions test" << std::endl;
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}