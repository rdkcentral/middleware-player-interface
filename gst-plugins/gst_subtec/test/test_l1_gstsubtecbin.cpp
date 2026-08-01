
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
* @file test_l1_gstsubtecbin.cpp
* @page gstsubtecbin Level 1 Tests
*
* ## Module's Role
* This module includes Level 1 functional tests (success and failure scenarios).
* This is to ensure that the gstsubtecbin methods meet the requirements.
*
* **Pre-Conditions:**  None @n
* **Dependencies:** None @n
*
*/

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <stdio.h>
#include "gst-plugins/gst_subtec/gstsubtecbin.h"

/**
 * @brief Test for gst_subtecbin_get_type() ensuring it returns a non-zero value.
 *
 * This test verifies that the gst_subtecbin_get_type() function returns a valid non-zero type value. The EXPECT_NE assertion checks that the returned value is not equal to 0UL, ensuring proper type definition.
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
 * | Variation / Step | Description                               | Test Data                                            | Expected Result                                          | Notes           |
 * | :--------------: | ----------------------------------------- | ---------------------------------------------------- | -------------------------------------------------------- | --------------- |
 * | 01               | Log the entry of the test                 | No input, output "Entering ReturnsNonZeroValue test" | Test start message is logged                             | Should be successful |
 * | 02               | Invoke gst_subtecbin_get_type() and verify  | No input, output value from gst_subtecbin_get_type()   | Return value is not equal to 0UL as verified by EXPECT_NE | Should Pass     |
 * | 03               | Log the exit of the test                  | No input, output "Exiting ReturnsNonZeroValue test"  | Test exit message is logged                              | Should be successful |
 */
TEST(GstSubtecbinGetTypeTest, ReturnsNonZeroValue) {
    std::cout << "Entering ReturnsNonZeroValue test" << std::endl;
    
    std::cout << "Invoking gst_subtecbin_get_type()" << std::endl;
    GType type_value = gst_subtecbin_get_type();
    std::cout << "gst_subtecbin_get_type() returned value: " << type_value << std::endl;
    
    EXPECT_NE(type_value, 0UL);
    
    std::cout << "Exiting ReturnsNonZeroValue test" << std::endl;
}
/**
 * @brief Verify that gst_subtecbin_get_type() returns a consistent value on multiple calls.
 *
 * This test verifies that the function gst_subtecbin_get_type() returns the same GType value upon multiple invocations, ensuring consistency in type registration and identification.
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
 * | Variation / Step | Description                                                         | Test Data                                | Expected Result                                                       | Notes      |
 * | :----:           | ------------------------------------------------------------------- | ---------------------------------------- | --------------------------------------------------------------------- | ---------- |
 * | 01               | Invoke gst_subtecbin_get_type() for the first time                  | output1 = first_call (GType)             | Returns a valid GType value                                           | Should Pass|
 * | 02               | Invoke gst_subtecbin_get_type() for the second time                 | output1 = second_call (GType)            | Returns a valid GType value identical to the first call               | Should Pass|
 * | 03               | Compare the GType values from both invocations                      | first_call = value, second_call = value  | Both invocations produce the same GType value (assertion succeeds)      | Should Pass|
 */
TEST(GstSubtecbinGetTypeTest, ReturnsConsistentValueOnMultipleCalls) {
    std::cout << "Entering ReturnsConsistentValueOnMultipleCalls test" << std::endl;
    
    std::cout << "Invoking gst_subtecbin_get_type() for the first time" << std::endl;
    GType first_call = gst_subtecbin_get_type();
    std::cout << "First invocation returned value: " << first_call << std::endl;
    
    std::cout << "Invoking gst_subtecbin_get_type() for the second time" << std::endl;
    GType second_call = gst_subtecbin_get_type();
    std::cout << "Second invocation returned value: " << second_call << std::endl;
    
    EXPECT_EQ(first_call, second_call);
    
    std::cout << "Exiting ReturnsConsistentValueOnMultipleCalls test" << std::endl;
}

int main(int argc, char **argv) {
     ::testing::InitGoogleTest(&argc, argv);
     return RUN_ALL_TESTS();
}
