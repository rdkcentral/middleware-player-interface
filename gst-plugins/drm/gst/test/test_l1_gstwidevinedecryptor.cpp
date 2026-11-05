
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
* @file test_l1_gstwidevinedecryptor.cpp
* @page gstwidevinedecryptor Level 1 Tests
*
* ## Module's Role
* This module includes Level 1 functional tests (success and failure scenarios).
* This is to ensure that the gstwidevinedecryptor methods meet the requirements.
*
* **Pre-Conditions:**  None @n
* **Dependencies:** None @n
*
*/

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <stdio.h>
#include "gst-plugins/drm/gst/gstwidevinedecryptor.h"

/**
 * @brief Validate that gst_widevinedecryptor_get_type returns a valid GType.
 *
 * This test verifies that the function gst_widevinedecryptor_get_type correctly registers and returns a valid GType.
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
 * | Variation / Step | Description                                               | Test Data                                          | Expected Result                                       | Notes               |
 * | :--------------: | --------------------------------------------------------- | -------------------------------------------------- | ----------------------------------------------------- | ------------------- |
 * | 01               | Log the entry message indicating the start of the test    | None                                               | Entry log message printed to console                  | Should be successful|
 * | 02               | Invoke gst_widevinedecryptor_get_type API                  | output = type (GType)                              | Returns a valid GType value (non-zero)                | Should Pass         |
 * | 03               | Assert that the returned GType is valid (non-zero)         | type != 0UL (assertion)                            | Assertion passes if type is non-zero                  | Should Pass         |
 * | 04               | Log the exit message indicating the end of the test        | None                                               | Exit log message printed to console                   | Should be successful|
 */
TEST(GstWidevineDecryptorGetTypeTest, ValidGType) {
    std::cout << "Entering Verify gst_widevinedecryptor_get_type returns a valid GType test" << std::endl;
    
    std::cout << "Invoking gst_widevinedecryptor_get_type()" << std::endl;
    GType type = gst_widevinedecryptor_get_type();
    std::cout << "gst_widevinedecryptor_get_type() returned value: " << type << std::endl;
    
    ASSERT_NE(type, 0UL);
    
    std::cout << "Exiting Verify gst_widevinedecryptor_get_type returns a valid GType test" << std::endl;
}
/**
 * @brief Verify that gst_widevinedecryptor_get_type() returns a consistent GType value on multiple calls
 *
 * This test invokes gst_widevinedecryptor_get_type() three times and verifies that the returned value is non-zero and remains consistent across all calls.
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
 * | Variation / Step | Description                                                         | Test Data                                                        | Expected Result                                                               | Notes         |
 * | :--------------: | ------------------------------------------------------------------- | ---------------------------------------------------------------- | ----------------------------------------------------------------------------- | ------------- |
 * | 01               | Invoke gst_widevinedecryptor_get_type() for the first time          | output1 = gst_widevinedecryptor_get_type() value                 | Returns a valid, non-zero GType value                                         | Should Pass   |
 * | 02               | Invoke gst_widevinedecryptor_get_type() for the second time         | output2 = gst_widevinedecryptor_get_type() value                 | Returns the same GType value as returned by the first call                    | Should Pass   |
 * | 03               | Invoke gst_widevinedecryptor_get_type() for the third time          | output3 = gst_widevinedecryptor_get_type() value                 | Returns the same GType value as returned by the first and second calls          | Should Pass   |
 * | 04               | Verify the first call returned a non-zero GType value               | input: none, output1 = valid GType value                          | ASSERT_NE(output1, 0UL)                                                         | Should Pass   |
 * | 05               | Assert that all three calls returned consistent GType values        | input: output1, output2, output3                                   | ASSERT_EQ(output1, output2) and ASSERT_EQ(output2, output3)                     | Should Pass   |
 */
TEST(GstWidevineDecryptorGetTypeTest, ConsistentGType) {
    std::cout << "Entering Verify gst_widevinedecryptor_get_type returns a consistent GType value on multiple calls test" << std::endl;
    
    std::cout << "Invoking gst_widevinedecryptor_get_type() for the first time" << std::endl;
    GType type1 = gst_widevinedecryptor_get_type();
    std::cout << "First call returned value: " << type1 << std::endl;
    
    std::cout << "Invoking gst_widevinedecryptor_get_type() for the second time" << std::endl;
    GType type2 = gst_widevinedecryptor_get_type();
    std::cout << "Second call returned value: " << type2 << std::endl;
    
    std::cout << "Invoking gst_widevinedecryptor_get_type() for the third time" << std::endl;
    GType type3 = gst_widevinedecryptor_get_type();
    std::cout << "Third call returned value: " << type3 << std::endl;
    
    ASSERT_NE(type1, 0UL);
    ASSERT_EQ(type1, type2);
    ASSERT_EQ(type2, type3);
    
    std::cout << "Exiting Verify gst_widevinedecryptor_get_type returns a consistent GType value on multiple calls test" << std::endl;
}

int main(int argc, char **argv) {
     ::testing::InitGoogleTest(&argc, argv);
     return RUN_ALL_TESTS();
}
