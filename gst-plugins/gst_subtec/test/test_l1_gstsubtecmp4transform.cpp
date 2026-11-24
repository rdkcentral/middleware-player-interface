
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
* @file test_l1_gstsubtecmp4transform.cpp
* @page gstsubtecmp4transform Level 1 Tests
*
* ## Module's Role
* This module includes Level 1 functional tests (success and failure scenarios).
* This is to ensure that the gstsubtecmp4transform methods meet the requirements.
*
* **Pre-Conditions:**  None @n
* **Dependencies:** None @n
*
*/

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <stdio.h>
#include "gst-plugins/gst_subtec/gstsubtecmp4transform.h"

/**
 * @brief Test to validate that gst_subtecmp4transform_get_type returns a non-zero value
 *
 * This test verifies that the gst_subtecmp4transform_get_type() API, when invoked, returns a valid non-zero GType value. A non-zero return value indicates that the type registration has been done correctly and the API is functioning as expected.
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
 * | 01 | Invoke gst_subtecmp4transform_get_type and verify the returned GType is non-zero | output1 = type; value = non-zero GType | Returns a non-zero GType value; EXPECT_NE(type, 0UL) assertion passes | Should Pass |
 */
TEST(GstSubtecmp4transformGetTypeTest, ValidNonZero) {
    char buffer[100];
    strncpy(buffer, "Entering gst_subtecmp4transform_get_type_valid_non_zero test", sizeof(buffer)-1);
    buffer[sizeof(buffer)-1] = '\0';
    std::cout << buffer << std::endl;

    std::cout << "Invoking gst_subtecmp4transform_get_type()" << std::endl;
    GType type = gst_subtecmp4transform_get_type();
    std::cout << "Retrieved GType value: " << type << std::endl;
    
    EXPECT_NE(type, 0UL);

    strncpy(buffer, "Exiting gst_subtecmp4transform_get_type_valid_non_zero test", sizeof(buffer)-1);
    buffer[sizeof(buffer)-1] = '\0';
    std::cout << buffer << std::endl;
}
/**
 * @brief Verify that gst_subtecmp4transform_get_type() consistently returns the same GType on repeated calls
 *
 * This test ensures that invoking gst_subtecmp4transform_get_type() multiple times produces the same GType value each time. It verifies the stability and consistency of the type registration mechanism.
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
 * | 01 | Call gst_subtecmp4transform_get_type() for the first time to obtain a GType value | output1 = GType value from first invocation | A valid GType value is returned | Should Pass |
 * | 02 | Call gst_subtecmp4transform_get_type() for the second time and compare it with the first GType value | output2 = GType value from second invocation, type1 = value from step 01 | The returned GType value matches the one from the first call (EXPECT_EQ passes) | Should Pass |
 */
TEST(GstSubtecmp4transformGetTypeTest, RepeatedCallsSameValue) {
    char buffer[100];
    strncpy(buffer, "Entering gst_subtecmp4transform_get_type_repeated_calls test", sizeof(buffer)-1);
    buffer[sizeof(buffer)-1] = '\0';
    std::cout << buffer << std::endl;

    std::cout << "Invoking gst_subtecmp4transform_get_type() for the first call" << std::endl;
    GType type1 = gst_subtecmp4transform_get_type();
    std::cout << "First call returned GType value: " << type1 << std::endl;
    
    std::cout << "Invoking gst_subtecmp4transform_get_type() for the second call" << std::endl;
    GType type2 = gst_subtecmp4transform_get_type();
    std::cout << "Second call returned GType value: " << type2 << std::endl;

    EXPECT_EQ(type1, type2);

    strncpy(buffer, "Exiting gst_subtecmp4transform_get_type_repeated_calls test", sizeof(buffer)-1);
    buffer[sizeof(buffer)-1] = '\0';
    std::cout << buffer << std::endl;
}

int main(int argc, char **argv) {
     ::testing::InitGoogleTest(&argc, argv);
     return RUN_ALL_TESTS();
}
