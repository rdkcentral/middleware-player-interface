
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
* @file test_l1_gstplayreadydecryptor.cpp
* @page gstplayreadydecryptor Level 1 Tests
*
* ## Module's Role
* This module includes Level 1 functional tests (success and failure scenarios).
* This is to ensure that the gstplayreadydecryptor methods meet the requirements.
*
* **Pre-Conditions:**  None @n
* **Dependencies:** None @n
*
*/

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <stdio.h>
#include "gst-plugins/drm/gst/gstplayreadydecryptor.h"

/**
 * @brief Verify that gst_playreadydecryptor_get_type() returns a valid non-zero type
 *
 * This test validates that the gst_playreadydecryptor_get_type() API properly returns a valid type,
 * ensuring that the returned value is not zero. The non-zero value indicates that the type has been correctly registered.
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
 * | Variation / Step | Description                                                                 | Test Data                                   | Expected Result                                      | Notes            |
 * | :--------------: | --------------------------------------------------------------------------- | ------------------------------------------- | ---------------------------------------------------- | ---------------- |
 * | 01               | Print entry message "Entering RetrieveValidTypeTest test"                   | N/A                                         | Entry message displayed on stdout                    | Should be successful |
 * | 02               | Print invocation message "Invoking gst_playreadydecryptor_get_type()"         | N/A                                         | Invocation message displayed on stdout               | Should be successful |
 * | 03               | Invoke gst_playreadydecryptor_get_type() API to retrieve the type             | input: none, output: type = valid non-zero  | API returns a valid non-zero type                    | Should Pass      |
 * | 04               | Assert that the returned type is not equal to zero                           | output: type = returned value               | Assertion passes if type != 0                        | Should Pass      |
 * | 05               | Print exit message "Exiting RetrieveValidTypeTest test"                       | N/A                                         | Exit message displayed on stdout                     | Should be successful |
 */
TEST(PlayreadyDecryptorGetType, RetrieveValidTypeTest) {
    std::cout << "Entering RetrieveValidTypeTest test" << std::endl;
    std::cout << "Invoking gst_playreadydecryptor_get_type()" << std::endl;
    GType type = gst_playreadydecryptor_get_type();
    std::cout << "gst_playreadydecryptor_get_type() returned: " << type << std::endl;

    ASSERT_NE(type, 0UL);
    std::cout << "Exiting RetrieveValidTypeTest test" << std::endl;
}
/**
 * @brief Validate that gst_playreadydecryptor_get_type() returns a consistent, non-zero type
 *
 * This test invokes gst_playreadydecryptor_get_type() twice to ensure that the returned type is valid (non-zero) and remains consistent across multiple calls. The objective is to verify that the API consistently returns the same GType value, which is critical for reliable type identification and usage in the system.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoke the first gst_playreadydecryptor_get_type() call and capture the returned type value. | input: none, output: type1 = value returned from gst_playreadydecryptor_get_type() | type1 should be a non-zero GType value | Should Pass |
 * | 02 | Invoke the second gst_playreadydecryptor_get_type() call and capture the returned type value. | input: none, output: type2 = value returned from gst_playreadydecryptor_get_type() | type2 should be a non-zero GType value | Should Pass |
 * | 03 | Compare the two captured type values to ensure consistency. | input: type1 from first call, type2 from second call, expected: type1 and type2 values | Both type1 and type2 should be equal and non-zero | Should be successful |
 */
TEST(PlayreadyDecryptorGetType, ConsistentTypeTest) {
    std::cout << "Entering ConsistentTypeTest test" << std::endl;
    
    std::cout << "Invoking first gst_playreadydecryptor_get_type() call" << std::endl;
    GType type1 = gst_playreadydecryptor_get_type();
    std::cout << "First call returned: " << type1 << std::endl;
    
    std::cout << "Invoking second gst_playreadydecryptor_get_type() call" << std::endl;
    GType type2 = gst_playreadydecryptor_get_type();
    std::cout << "Second call returned: " << type2 << std::endl;
    
    ASSERT_NE(type1, 0UL);
    ASSERT_NE(type2, 0UL);
    ASSERT_EQ(type1, type2);
    
    std::cout << "Exiting ConsistentTypeTest test" << std::endl;
}

int main(int argc, char **argv) {
     ::testing::InitGoogleTest(&argc, argv);
     return RUN_ALL_TESTS();
}
