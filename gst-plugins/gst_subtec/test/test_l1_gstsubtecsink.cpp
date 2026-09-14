
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
* @file test_l1_gstsubtecsink.cpp
* @page gstsubtecsink Level 1 Tests
*
* ## Module's Role
* This module includes Level 1 functional tests (success and failure scenarios).
* This is to ensure that the gstsubtecsink methods meet the requirements.
*
* **Pre-Conditions:**  None @n
* **Dependencies:** None @n
*
*/

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <stdio.h>
#include "gst-plugins/gst_subtec/gstsubtecsink.h"

/**
 * @brief Verify that gst_subtecsink_get_type() returns a valid non-zero GType value.
 *
 * This test ensures that when gst_subtecsink_get_type() is invoked, it returns a non-zero value indicating a valid type registration. The function's proper behavior is verified by checking that the returned GType is not equal to 0.
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
 * | 01 | Invoke gst_subtecsink_get_type() and verify the returned GType is non-zero. | input = None, output = non-zero GType | API returns a valid non-zero GType value; assertion EXPECT_NE(typeValue, 0) passes | Should Pass |
 */
TEST(GstSubtecSinkGetTypeTest, VerifyValidGTypeReturn) {
    std::cout << "Entering VerifyValidGTypeReturn test" << std::endl;
    
    std::cout << "Invoking gst_subtecsink_get_type()" << std::endl;
    GType typeValue = gst_subtecsink_get_type();
    std::cout << "Returned GType value: " << typeValue << std::endl;
    
    EXPECT_NE(typeValue, 0) << "Expected non-zero GType indicating valid type registration.";
    
    char debugLog[64];
    const char* message = "Test: Valid GType value retrieved.";
    std::strncpy(debugLog, message, sizeof(debugLog) - 1);
    debugLog[sizeof(debugLog) - 1] = '\0';
    std::cout << debugLog << std::endl;
    
    std::cout << "Exiting VerifyValidGTypeReturn test" << std::endl;
}

/**
 * @brief Verify that gst_subtecsink_get_type() produces a consistent and valid GType value across multiple invocations.
 *
 * This test ensures that repeated calls to gst_subtecsink_get_type() return the same non-zero GType,
 * verifying both consistency and valid type registration. This is critical because consistent type registration is required for proper operation.
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
 * |01| Invoke gst_subtecsink_get_type() for the first time. | No input; Output: firstCall = GType value | Returns a valid, non-zero GType value. | Should Pass |
 * |02| Capture the value from the first invocation. | Output: firstCall = value | firstCall holds the GType value for subsequent comparison. | Should be successful |
 * |03| Invoke gst_subtecsink_get_type() for the second time. | No input; Output: secondCall = GType value | Returns the same valid GType value as the first call. | Should Pass |
 * |04| Capture the value from the second invocation. | Output: secondCall = value | secondCall holds the GType value for consistency check. | Should be successful |
 * |05| Compare the GType values from both calls. | Input: firstCall, secondCall; Assertion: EXPECT_EQ(firstCall, secondCall) | Both calls return identical GType values. | Should Pass |
 * |06| Validate that the GType is non-zero. | Input: firstCall; Assertion: EXPECT_NE(firstCall, 0) | GType is non-zero, indicating valid type registration. | Should Pass |
 * |07| Log the debug message to indicate successful test execution. | debugLog = "Test: Consistent and valid GType across multiple invocations." | Debug log message printed correctly. | Should be successful |
 * |08| Exit the test function. | N/A | Test completes execution successfully. | Should be successful |
 */
TEST(GstSubtecSinkGetTypeTest, VerifyConsistencyAcrossMultipleCalls) {
    std::cout << "Entering VerifyConsistencyAcrossMultipleCalls test" << std::endl;
    
    std::cout << "Invoking gst_subtecsink_get_type() for the first time" << std::endl;
    GType firstCall = gst_subtecsink_get_type();
    std::cout << "First call returned: " << firstCall << std::endl;
    
    std::cout << "Invoking gst_subtecsink_get_type() for the second time" << std::endl;
    GType secondCall = gst_subtecsink_get_type();
    std::cout << "Second call returned: " << secondCall << std::endl;
    
    EXPECT_EQ(firstCall, secondCall) << "Expected consistent GType value across multiple calls.";
    EXPECT_NE(firstCall, 0) << "Expected non-zero GType indicating valid type registration.";
    
    char debugLog[64];
    const char* message = "Test: Consistent and valid GType across multiple invocations.";
    std::strncpy(debugLog, message, sizeof(debugLog) - 1);
    debugLog[sizeof(debugLog) - 1] = '\0';
    std::cout << debugLog << std::endl;
    
    std::cout << "Exiting VerifyConsistencyAcrossMultipleCalls test" << std::endl;
}

int main(int argc, char **argv) {
     ::testing::InitGoogleTest(&argc, argv);
     return RUN_ALL_TESTS();
}
