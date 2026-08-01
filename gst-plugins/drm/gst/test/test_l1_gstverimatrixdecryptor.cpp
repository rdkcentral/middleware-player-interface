
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
* @file test_l1_gstverimatrixdecryptor.cpp
* @page gstverimatrixdecryptor Level 1 Tests
*
* ## Module's Role
* This module includes Level 1 functional tests (success and failure scenarios).
* This is to ensure that the gstverimatrixdecryptor methods meet the requirements.
*
* **Pre-Conditions:**  None @n
* **Dependencies:** None @n
*
*/

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <stdio.h>
#include "gst-plugins/drm/gst/gstverimatrixdecryptor.h"

/**
 * @brief Verifies that gst_verimatrixdecryptor_get_type() returns a valid non-zero type
 *
 * This test verifies that the gst_verimatrixdecryptor_get_type() function returns a non-zero GType value.
 * A non-zero result indicates that the type has been correctly initialized and is valid for further use.
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
 * | Variation / Step | Description                                                                                | Test Data                                   | Expected Result                                                  | Notes         |
 * | :--------------: | ------------------------------------------------------------------------------------------ | ------------------------------------------- | --------------------------------------------------------------- | ------------- |
 * | 01               | Log the entry message indicating the start of the test                                     | N/A                                         | "Entering ReturnsValidNonZeroType test" message printed         | Should be successful |
 * | 02               | Invoke gst_verimatrixdecryptor_get_type() to retrieve the GType value                        | output: type (from gst_verimatrixdecryptor_get_type()) | GType value is retrieved                                          | Should Pass   |
 * | 03               | Assert that the retrieved GType value is not zero using EXPECT_NE                            | Expected: type != 0u                        | Assertion passes if type is non-zero                             | Should be successful |
 * | 04               | Log the exit message indicating the end of the test                                        | N/A                                         | "Exiting ReturnsValidNonZeroType test" message printed            | Should be successful |
 */
TEST(GstVerimatrixDecryptorGetType, ReturnsValidNonZeroType) {
    std::cout << "Entering ReturnsValidNonZeroType test" << std::endl;
    
    std::cout << "Invoking gst_verimatrixdecryptor_get_type()" << std::endl;
    GType type = gst_verimatrixdecryptor_get_type();
    std::cout << "Retrieved GType value: " << type << std::endl;
    
    EXPECT_NE(type, 0u);
    
    std::cout << "Exiting ReturnsValidNonZeroType test" << std::endl;
}
/**
 * @brief Test to verify that repeated calls to gst_verimatrixdecryptor_get_type return the same type.
 *
 * This test calls gst_verimatrixdecryptor_get_type() twice and verifies that both calls return the identical GType.
 * It ensures the consistency of the type registration mechanism and validates that the function behaves as expected when invoked multiple times.
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
 * | Variation / Step | Description                                                     | Test Data                                   | Expected Result                                               | Notes                |
 * | :--------------: | ---------------------------------------------------------------- | ------------------------------------------- | ------------------------------------------------------------- | -------------------- |
 * | 01               | Invoke gst_verimatrixdecryptor_get_type() for the first time       | output1 = GType value from first invocation | Should return a valid GType value                             | Should Pass          |
 * | 02               | Invoke gst_verimatrixdecryptor_get_type() for the second time      | output2 = GType value from second invocation | Should return a valid GType value                             | Should Pass          |
 * | 03               | Compare the two returned GType values to verify they are identical | type1 = first value, type2 = second value    | Both returned GType values should be equal                    | Should be successful |
 */
TEST(GstVerimatrixDecryptorGetType, RepeatedCallsReturnSameType) {
    std::cout << "Entering RepeatedCallsReturnSameType test" << std::endl;
    
    std::cout << "Invoking gst_verimatrixdecryptor_get_type() for the first time" << std::endl;
    GType type1 = gst_verimatrixdecryptor_get_type();
    std::cout << "First invocation returned: " << type1 << std::endl;
    
    std::cout << "Invoking gst_verimatrixdecryptor_get_type() for the second time" << std::endl;
    GType type2 = gst_verimatrixdecryptor_get_type();
    std::cout << "Second invocation returned: " << type2 << std::endl;
    
    EXPECT_EQ(type1, type2);
    
    std::cout << "Exiting RepeatedCallsReturnSameType test" << std::endl;
}

int main(int argc, char **argv) {
     ::testing::InitGoogleTest(&argc, argv);
     return RUN_ALL_TESTS();
}
