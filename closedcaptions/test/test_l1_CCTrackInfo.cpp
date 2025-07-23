
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
* @file test_l1_CCTrackInfo.cpp
* @page CCTrackInfo Level 1 Tests
*
* ## Module's Role
* This module includes Level 1 functional tests (success and failure scenarios).
* This is to ensure that the CCTrackInfo methods meet the requirements.
*
* **Pre-Conditions:**  None @n
* **Dependencies:** None @n
*
*/

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <stdio.h>
#include "CCTrackInfo.h"


// Test Case: Verify default constructor initializes instreamId and language without exceptions
/**
 * @brief Verify that default CCTrackInfo constructor initializes instreamId and language members to empty strings without throwing any exceptions.
 *
 * This test creates an instance of CCTrackInfo using its default constructor and verifies that the members instreamId and language are initialized as empty strings. It ensures that the construction process does not throw any exceptions and that the object's default state is valid.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 001@n
 * **Priority:** High@n
 * @n
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * @n
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoke default constructor of CCTrackInfo and verify instreamId and language are empty strings | input: none, output: instreamId = "", language = "" | Default constructed object has instreamId = "" and language = "" verified using EXPECT_EQ | Should Pass |
 */
TEST(CCTrackInfo, VerifyDefaultConstructorInitializesInstreamIdAndLanguageWithoutExceptions) {
    std::cout << "Entering VerifyDefaultConstructorInitializesInstreamIdAndLanguageWithoutExceptions test" << std::endl;
    
    // Invoke the default constructor of CCTrackInfo
    std::cout << "Invoking CCTrackInfo default constructor" << std::endl;
    CCTrackInfo ccTrackInfo;
    std::cout << "After construction: instreamId = \"" << ccTrackInfo.instreamId << "\", language = \"" << ccTrackInfo.language << "\"" << std::endl;
    
    // Debug logs for expected default values from getters (here using direct access members)
    std::cout << "Retrieved value of instreamId: \"" << ccTrackInfo.instreamId << "\"" << std::endl;
    std::cout << "Retrieved value of language: \"" << ccTrackInfo.language << "\"" << std::endl;
    
    // Validate that the default constructed values are empty strings
    EXPECT_EQ(ccTrackInfo.instreamId, "");
    EXPECT_EQ(ccTrackInfo.language, "");
    
    std::cout << "Exiting VerifyDefaultConstructorInitializesInstreamIdAndLanguageWithoutExceptions test" << std::endl;
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
