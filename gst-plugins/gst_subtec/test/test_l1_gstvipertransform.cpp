
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
* @file test_l1_gstvipertransform.cpp
* @page gstvipertransform Level 1 Tests
*
* ## Module's Role
* This module includes Level 1 functional tests (success and failure scenarios).
* This is to ensure that the gstvipertransform methods meet the requirements.
*
* **Pre-Conditions:**  None @n
* **Dependencies:** None @n
*
*/

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <stdio.h>
#include "gst-plugins/gst_subtec/gstvipertransform.h"

/**
 * @brief Validate that gst_vipertransform_get_type() returns a valid, non-zero GType.
 *
 * This test invokes the gst_vipertransform_get_type() API to ensure it returns a non-zero GType value. 
 * The test verifies that the returned value is valid by checking it is not zero, ensuring that the API is correctly registered and functional.
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
 * | Variation / Step | Description                                                       | Test Data                              | Expected Result                                                      | Notes          |
 * | :--------------: | ----------------------------------------------------------------- | -------------------------------------- | -------------------------------------------------------------------- | -------------- |
 * | 01               | Log the entry message for the test                                | No input                               | Entry message logged                                                 | Should be successful  |
 * | 02               | Invoke gst_vipertransform_get_type()                              | input = none                           | API is called                                                        | Should Pass    |
 * | 03               | Print the returned GType value                                    | output = type (non-zero expected)      | Returned value is printed                                            | Should be successful  |
 * | 04               | Assert that the returned GType is non-zero                          | output = type != 0                     | EXPECT_NE assertion passes confirming non-zero value                 | Should Pass    |
 * | 05               | Log the exit message for the test                                 | No input                               | Exit message logged                                                  | Should be successful  |
 */
TEST(GstVipertransformGetTypeTests, ValidNonZeroGType) {
    std::cout << "Entering ValidNonZeroGType test" << std::endl;
    
    std::cout << "Invoking gst_vipertransform_get_type()" << std::endl;
    GType type = gst_vipertransform_get_type();
    std::cout << "Returned GType value: " << type << std::endl;
    
    EXPECT_NE(type, 0) << "gst_vipertransform_get_type() should return non-zero GType.";

    std::cout << "Exiting ValidNonZeroGType test" << std::endl;
}

/**
 * @brief Test the idempotency of gst_vipertransform_get_type() API
 *
 * This test verifies that multiple invocations of the gst_vipertransform_get_type() function return the same GType value. Ensuring idempotency is critical as it confirms that the API consistently provides a valid and stable type registration without unintended variations.
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
 * | Variation / Step | Description                                                    | Test Data                           | Expected Result                                          | Notes       |
 * | :--------------: | -------------------------------------------------------------- | ----------------------------------- | -------------------------------------------------------- | ----------- |
 * | 01               | Invoke gst_vipertransform_get_type() for the first time        | output1 = firstCall (GType value)   | Returns a valid GType value                              | Should Pass |
 * | 02               | Invoke gst_vipertransform_get_type() for the second time       | output1 = secondCall (GType value)  | Returns a valid GType value, identical to firstCall      | Should Pass |
 * | 03               | Compare the GType values obtained from the two invocations     | input1 = firstCall, input2 = secondCall | Both calls should yield identical GType values via EXPECT_EQ check | Should Pass |
 */
TEST(GstVipertransformGetTypeTests, IdempotencyTest) {
    std::cout << "Entering IdempotencyTest test" << std::endl;
    
    std::cout << "Invoking gst_vipertransform_get_type() first time" << std::endl;
    GType firstCall = gst_vipertransform_get_type();
    std::cout << "First call returned GType: " << firstCall << std::endl;
    
    std::cout << "Invoking gst_vipertransform_get_type() second time" << std::endl;
    GType secondCall = gst_vipertransform_get_type();
    std::cout << "Second call returned GType: " << secondCall << std::endl;
    
    EXPECT_EQ(firstCall, secondCall) << "Multiple invocations of gst_vipertransform_get_type() should return the same GType value.";
    
    std::cout << "Exiting IdempotencyTest test" << std::endl;
}

int main(int argc, char **argv) {
     ::testing::InitGoogleTest(&argc, argv);
     return RUN_ALL_TESTS();
}
