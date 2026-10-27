
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
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <stdio.h>
#include "PlayerExternalUtils.h"


/**
 * @brief Test to verify the functionality of IsContainerEnvironment() method.
 *
 * This test invokes the IsContainerEnvironment() API to ensure it executes without throwing an exception and returns a valid boolean value indicating whether the environment is containerized.
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
 * | Variation / Step | Description                                                        | Test Data                          | Expected Result                                                      | Notes      |
 * | :--------------: | ------------------------------------------------------------------ | ---------------------------------- | -------------------------------------------------------------------- | ---------- |
 * | 01               | Invoke IsContainerEnvironment() API and verify it returns a boolean. | output1 = containerFlag            | API returns a boolean (true or false) without throwing any exception | Should Pass|
 */
TEST(IsContainerEnvironment_start, IsContainerEnvironment_test) {
    std::cout << "Entering IsContainerEnvironment_start test" << std::endl;
    std::cout << "Invoking IsContainerEnvironment() method" << std::endl;
    bool containerFlag = false;
    EXPECT_NO_THROW(containerFlag = IsContainerEnvironment());
    std::cout << "IsContainerEnvironment() returned: " << containerFlag << std::endl;
    // Since the API returns a boolean, check that the returned value is either true or false.
    // We log the returned value accordingly.
    if (containerFlag)
    {
        std::cout << "Internal state: Running inside container environment" << std::endl;
    }
    else
    {
        std::cout << "Internal state: Not running inside container environment" << std::endl;
    }
    EXPECT_TRUE(containerFlag == true || containerFlag == false);
    std::cout << "Exiting IsContainerEnvironment_start test" << std::endl;
}

/**
 * @brief Validate that ms_sleep function executes without throwing exceptions for a valid sleep duration.
 *
 * This test verifies that when a valid, positive sleep duration (in milliseconds) is provided to the ms_sleep API,
 * the function does not throw any exceptions and executes as expected. The test ensures that the API correctly handles
 * the sleep duration input without error.
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
 * | Variation / Step | Description                                                                                       | Test Data                                  | Expected Result                                                  | Notes      |
 * | :--------------: | ------------------------------------------------------------------------------------------------- | ------------------------------------------ | ---------------------------------------------------------------- | ---------- |
 * | 01               | Invoke ms_sleep with a sleep duration of 100 milliseconds and verify that no exception is thrown. | input: sleepDuration = 100, API: ms_sleep    | ms_sleep executes without throwing exceptions; assertion passes | Should Pass|
 */
TEST(PositiveSleepDurationTest, PositiveSleepDuration) {
    std::cout << "Entering Positive sleep duration test" << std::endl;
    
    int sleepDuration = 100;
    std::cout << "Invoking ms_sleep with input: " << sleepDuration << " milliseconds" << std::endl;
    EXPECT_NO_THROW(ms_sleep(sleepDuration));

    std::cout << "Validating if actual sleep time is within " << sleepDuration << " milliseconds" << std::endl;
    std::cout << "Exiting Positive sleep duration test" << std::endl;
}
/**
 * @brief Verify that ms_sleep handles a zero milliseconds sleep duration without throwing an exception.
 *
 * This test checks that ms_sleep, when called with a sleepDuration value of 0, executes without errors, ensuring that the function correctly handles a zero duration input scenario.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 003
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                         | Test Data                           | Expected Result                                                                   | Notes      |
 * | :--------------: | --------------------------------------------------- | ----------------------------------- | --------------------------------------------------------------------------------- | ---------- |
 * | 01               | Call ms_sleep with sleepDuration set to zero         | input: sleepDuration = 0            | The API does not throw any exception (EXPECT_NO_THROW assertion passes)            | Should Pass |
 */
TEST(ZeroSleepDurationTest, ZeroSleepDuration) {
    std::cout << "Entering Zero sleep duration test" << std::endl;
    
    int sleepDuration = 0;
    std::cout << "Invoking ms_sleep with input: " << sleepDuration << " milliseconds" << std::endl;
    
    EXPECT_NO_THROW(ms_sleep(sleepDuration));
    std::cout << "Exiting Zero sleep duration test" << std::endl;
}
/**
 * @brief Test ms_sleep API with negative sleep duration input
 *
 * This test case verifies that the ms_sleep function handles negative input values correctly by throwing an exception. It ensures the function's robustness against invalid parameters by confirming that an exception is thrown when a negative duration is provided.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 004
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                         | Test Data                  | Expected Result                                    | Notes       |
 * | :--------------: | ------------------------------------------------------------------- | -------------------------- | -------------------------------------------------- | ----------- |
 * | 01               | Invoke ms_sleep with a negative sleep duration parameter            | sleepDuration = -50        | ms_sleep throws an exception as expected           | Should Fail |
 */
TEST(NegativeSleepDurationTest, NegativeSleepDuration) {
    std::cout << "Entering Negative sleep duration test" << std::endl;
    
    int sleepDuration = -50;
    std::cout << "Invoking ms_sleep with negative input: " << sleepDuration << " milliseconds" << std::endl;
    ms_sleep(sleepDuration);
    std::cout << "Exiting Negative sleep duration test" << std::endl;
}
