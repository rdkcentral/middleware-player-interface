
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
* @file test_l1_gstclearkeydecryptor.cpp
* @page gstclearkeydecryptor Level 1 Tests
*
* ## Module's Role
* This module includes Level 1 functional tests (success and failure scenarios).
* This is to ensure that the gstclearkeydecryptor methods meet the requirements.
*
* **Pre-Conditions:**  None @n
* **Dependencies:** None @n
*
*/

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <stdio.h>
#include "gst-plugins/drm/gst/gstclearkeydecryptor.h"

/**
 * @brief Validate that gst_clearkeydecryptor_get_type() returns a valid non-zero type
 *
 * This test verifies that the function gst_clearkeydecryptor_get_type() does not return 0, which indicates a valid type is returned.
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
 * | Variation / Step | Description                                                    | Test Data                                                                                 | Expected Result                                              | Notes          |
 * | :--------------: | -------------------------------------------------------------- | ----------------------------------------------------------------------------------------- | ------------------------------------------------------------ | -------------- |
 * | 01               | Set up the test environment and initialize the testName buffer   | testName = "ReturnsValidType"                                                               | Test environment is set up and testName initialized correctly | Should be successful |
 * | 02               | Invoke gst_clearkeydecryptor_get_type() to get the GType value      | Function Call: gst_clearkeydecryptor_get_type(), output1 = typeValue                        | API should return a non-zero GType value                      | Should Pass    |
 * | 03               | Validate the returned GType is non-zero using EXPECT_NE assertion   | Assertion Check: typeValue != 0u                                                          | EXPECT_NE assertion verifies that typeValue is not 0          | Should Pass    |
 */
TEST(GstClearkeyDecryptorGetTypeTest, ReturnsValidType) {
    char testName[64];
    std::memset(testName, 0, sizeof(testName));
    std::strncpy(testName, "ReturnsValidType", sizeof(testName)-1);
    std::cout << "Entering " << testName << " test" << std::endl;
    
    std::cout << "Invoking gst_clearkeydecryptor_get_type()" << std::endl;
    GType typeValue = gst_clearkeydecryptor_get_type();
    std::cout << "Returned GType value: " << typeValue << std::endl;
    
    EXPECT_NE(typeValue, 0u);
    std::cout << "Exiting " << testName << " test" << std::endl;
}
/**
 * @brief Verify consistent behavior of gst_clearkeydecryptor_get_type()
 *
 * This test validates that calling gst_clearkeydecryptor_get_type() multiple times returns a consistent and non-zero GType value. 
 * It ensures that the first invocation yields a valid value and that subsequent calls return the same value, confirming proper singleton behavior.
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
 * | Variation / Step | Description                                                                                   | Test Data                                                                                          | Expected Result                                                       | Notes       |
 * | :--------------: | --------------------------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------- | --------------------------------------------------------------------- | ----------- |
 * | 01               | Invoke gst_clearkeydecryptor_get_type() for the first time to obtain the initial GType value.  | output firstTypeValue = gst_clearkeydecryptor_get_type()                                           | firstTypeValue should be a non-zero value.                            | Should Pass |
 * | 02               | Invoke gst_clearkeydecryptor_get_type() for the second time and compare with the first value.    | output secondTypeValue = gst_clearkeydecryptor_get_type(), firstTypeValue (from step 01)             | secondTypeValue should equal firstTypeValue and be non-zero.            | Should Pass |
 */
TEST(GstClearkeyDecryptorGetTypeTest, ConsistentBehavior) {
    char testName[64];
    std::memset(testName, 0, sizeof(testName));
    std::strncpy(testName, "ConsistentBehavior", sizeof(testName)-1);
    std::cout << "Entering " << testName << " test" << std::endl;
    
    std::cout << "Invoking gst_clearkeydecryptor_get_type() for the first time" << std::endl;
    GType firstTypeValue = gst_clearkeydecryptor_get_type();
    std::cout << "First returned GType value: " << firstTypeValue << std::endl;
    
    std::cout << "Invoking gst_clearkeydecryptor_get_type() for the second time" << std::endl;
    GType secondTypeValue = gst_clearkeydecryptor_get_type();
    std::cout << "Second returned GType value: " << secondTypeValue << std::endl;
    
    EXPECT_NE(firstTypeValue, 0u);
    EXPECT_EQ(firstTypeValue, secondTypeValue);
    std::cout << "Exiting " << testName << " test" << std::endl;
}

int main(int argc, char **argv) {
     ::testing::InitGoogleTest(&argc, argv);
     return RUN_ALL_TESTS();
}
