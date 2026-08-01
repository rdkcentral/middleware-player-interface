
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
* @file test_l1_gstcdmidecryptor.cpp
* @page gstcdmidecryptor Level 1 Tests
*
* ## Module's Role
* This module includes Level 1 functional tests (success and failure scenarios).
* This is to ensure that the gstcdmidecryptor methods meet the requirements.
*
* **Pre-Conditions:**  None @n
* **Dependencies:** None @n
*
*/

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <stdio.h>
#include "gst-plugins/drm/gst/gstcdmidecryptor.h"

/**
 * @brief Verify that gst_cdmidecryptor_get_type() returns a valid non-zero GType.
 *
 * This test case checks that gst_cdmidecryptor_get_type() returns a valid GType identifier which is non-zero. This ensures that the CDMI Decryptor is properly registered.
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
 * | Variation / Step | Description                                                      | Test Data                                                          | Expected Result                   | Notes      |
 * | :--------------: | ----------------------------------------------------------------- | ------------------------------------------------------------------ | --------------------------------- | ---------- |
 * | 01               | Invoke gst_cdmidecryptor_get_type() and verify non-zero GType is returned. | output1 = decryptorType (returned from gst_cdmidecryptor_get_type()) | decryptorType is not equal to (GType)0 | Should Pass |
 */
TEST(gst_cdmidecryptor_get_type, VerifyValidCDMIDecryptorTypeIsReturned) {
    char logBuffer[100];
    std::strncpy(logBuffer, "Entering VerifyValidCDMIDecryptorTypeIsReturned test", sizeof(logBuffer)-1);
    logBuffer[sizeof(logBuffer)-1] = '\0';
    std::cout << logBuffer << std::endl;
    
    std::cout << "Invoking gst_cdmidecryptor_get_type()" << std::endl;
    GType decryptorType = gst_cdmidecryptor_get_type();
    std::cout << "Returned GType value: " << decryptorType << std::endl;
    
    // Expect a valid non-zero GType value.
    EXPECT_NE(decryptorType, static_cast<GType>(0));
    
    std::strncpy(logBuffer, "Exiting VerifyValidCDMIDecryptorTypeIsReturned test", sizeof(logBuffer)-1);
    logBuffer[sizeof(logBuffer)-1] = '\0';
    std::cout << logBuffer << std::endl;
}
/**
 * @brief Verify consistency of gst_cdmidecryptor_get_type across multiple calls
 *
 * This test verifies that multiple invocations of the gst_cdmidecryptor_get_type() function return a consistent and valid non-zero GType value. It ensures that the API behaves deterministically by returning the same GType on subsequent calls, which is critical for proper type registration within the system.
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
 * | 01 | Invoke gst_cdmidecryptor_get_type() for the first time and store the returned value. | output1 = firstCall (GType, expected non-zero) | Returned GType value should be non-zero. | Should Pass |
 * | 02 | Invoke gst_cdmidecryptor_get_type() for the second time and store the returned value. | output1 = secondCall (GType, expected same as firstCall) | Returned GType value should match the first call's value. | Should Pass |
 * | 03 | Compare the two returned GType values from the first and second calls. | input1 = firstCall, input2 = secondCall | Both values should be equal and non-zero. | Should Pass |
 */
TEST(gst_cdmidecryptor_get_type, VerifyConsistencyAcrossMultipleCalls) {
    char logBuffer[100];
    std::strncpy(logBuffer, "Entering VerifyConsistencyAcrossMultipleCalls test", sizeof(logBuffer)-1);
    logBuffer[sizeof(logBuffer)-1] = '\0';
    std::cout << logBuffer << std::endl;
    
    std::cout << "Invoking gst_cdmidecryptor_get_type() - first call" << std::endl;
    GType firstCall = gst_cdmidecryptor_get_type();
    std::cout << "Returned GType value from first call: " << firstCall << std::endl;
    
    std::cout << "Invoking gst_cdmidecryptor_get_type() - second call" << std::endl;
    GType secondCall = gst_cdmidecryptor_get_type();
    std::cout << "Returned GType value from second call: " << secondCall << std::endl;
    
    // Expect both calls return the same non-zero GType value.
    EXPECT_NE(firstCall, static_cast<GType>(0));
    EXPECT_EQ(firstCall, secondCall);
    
    std::strncpy(logBuffer, "Exiting VerifyConsistencyAcrossMultipleCalls test", sizeof(logBuffer)-1);
    logBuffer[sizeof(logBuffer)-1] = '\0';
    std::cout << logBuffer << std::endl;
}

int main(int argc, char **argv) {
     ::testing::InitGoogleTest(&argc, argv);
     return RUN_ALL_TESTS();
}

