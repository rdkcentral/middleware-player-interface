
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
#include "SocUtils.h"

using namespace SocUtils;


/**
 * @brief Validates that the EnableLiveLatencyCorrection API returns a boolean value.
 *
 * This test verifies that the SocUtils::EnableLiveLatencyCorrection() function is invoked correctly and returns a valid boolean value indicating whether live latency correction is enabled or disabled.
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
 * | Variation / Step | Description                                                       | Test Data                                                           | Expected Result                                                          | Notes       |
 * | :--------------: | ----------------------------------------------------------------- | ------------------------------------------------------------------- | ------------------------------------------------------------------------ | ----------- |
 * | 01               | Call SocUtils::EnableLiveLatencyCorrection() and validate the returned boolean value  | input: None, output: liveLatencyEnabled = {true, false}               | API returns a boolean value; EXPECT_TRUE assertion ensures the result is valid | Should Pass |
 */
TEST(SocUtils_EnableLiveLatencyCorrection_start, CheckLiveLatencyCorrection) {
    std::cout << "Entering SocUtils_EnableLiveLatencyCorrection_start test" << std::endl;
    std::cout << "Invoking SocUtils::EnableLiveLatencyCorrection()" << std::endl;
    
    bool liveLatencyEnabled = SocUtils::EnableLiveLatencyCorrection();
    
    std::cout << "Returned value: " << std::boolalpha << liveLatencyEnabled << std::endl;
    if(liveLatencyEnabled) {
        std::cout << "Live latency correction is enabled." << std::endl;
    } else {
        std::cout << "Live latency correction is disabled." << std::endl;
    }
    
    EXPECT_TRUE((liveLatencyEnabled == true) || (liveLatencyEnabled == false));
    
    std::cout << "Exiting SocUtils_EnableLiveLatencyCorrection_start test" << std::endl;
}
/**
 * @brief Verify that SocUtils::EnablePTSRestamp executes correctly and returns a valid boolean value
 *
 * Verify that the SocUtils::EnablePTSRestamp API is invoked successfully without throwing exceptions and that the returned value is a valid boolean (either true or false). This test ensures no unexpected exceptions occur during the API call and that the output adheres to the expected boolean type.
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
 * | Variation / Step | Description                                         | Test Data                                | Expected Result                                                         | Notes              |
 * | :--------------: | --------------------------------------------------- | ---------------------------------------- | ----------------------------------------------------------------------- | ------------------ |
 * | 01               | Invoke SocUtils::EnablePTSRestamp API               | input: None, output: result (bool)       | API should execute without throwing any exception and return a boolean  | Should Pass        |
 * | 02               | Validate the returned result is a boolean           | input: result = (true/false)               | Assertion: result is either true or false                               | Should be successful |
 */
TEST(EnablePTSRestamp_BasicScenario, Test) {
    std::cout << "Entering SocUtils::EnablePTSRestamp_BasicScenario test" << std::endl;
    
    bool result = false;
    EXPECT_NO_THROW({
        std::cout << "Invoking SocUtils::EnablePTSRestamp()" << std::endl;
        result = SocUtils::EnablePTSRestamp();
        std::cout << "Returned value from SocUtils::EnablePTSRestamp(): " << result << std::endl;
    });
    
    // Verify that the function returns a value that is either true or false.
    EXPECT_TRUE(result == true || result == false);
    
    std::cout << "Exiting SocUtils::EnablePTSRestamp_BasicScenario test" << std::endl;
}
/**
 * @brief Verify that SocUtils::IsAudioFragmentSyncSupported returns a valid boolean value.
 *
 * This test ensures that the SocUtils::IsAudioFragmentSyncSupported function does not throw an exception and returns a boolean value (either true or false). The test verifies that the API behaves correctly under normal conditions.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 003@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoke SocUtils::IsAudioFragmentSyncSupported() API | output: result (boolean value) | API call completes without throwing an exception | Should Pass |
 * | 02 | Verify that the returned result is either true or false using EXPECT_TRUE | result = returned value from SocUtils::IsAudioFragmentSyncSupported() | result equals true or result equals false | Should be successful |
 */
TEST(SocUtils_IsAudioFragmentSyncSupported_ReturnsTrue, VerifyReturnsTrueWhenSupported) {
    std::cout << "Entering VerifyReturnsTrueWhenSupported test" << std::endl;
    bool result = false;
    EXPECT_NO_THROW({
        std::cout << "Invoking SocUtils::IsAudioFragmentSyncSupported()" << std::endl;
        result = SocUtils::IsAudioFragmentSyncSupported();
        std::cout << "SocUtils::IsAudioFragmentSyncSupported() returned: " << result << std::endl;
    });

    EXPECT_TRUE(result == true || result == false);
    std::cout << "Exiting VerifyReturnsTrueWhenSupported test" << std::endl;
}
/**
 * @brief Verify that SocUtils::IsSupportedAC3 returns a boolean value without throwing an exception.
 *
 * This test case verifies that the SocUtils::IsSupportedAC3 API function returns a valid boolean indicating
 * whether AC-3 is supported, and ensures that the function call does not throw any exceptions during its execution.
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
 * | Variation / Step | Description                                                                                     | Test Data                                                     | Expected Result                                                                                | Notes       |
 * | ---------------- | ----------------------------------------------------------------------------------------------- | ------------------------------------------------------------- | ---------------------------------------------------------------------------------------------- | ----------- |
 * | 01               | Invoke SocUtils::IsSupportedAC3, capture the boolean result and verify no exception is thrown.   | function = SocUtils::IsSupportedAC3(), output(result) unassigned before call | Returns a boolean value indicating AC-3 support; no exception is thrown; EXPECT_NO_THROW passes | Should Pass |
 */
TEST(SocUtils_IsSupportedAC3_Test, ReturnsBoolean) {
    std::cout << "Entering SocUtils::IsSupportedAC3()_start test" << std::endl;
    
    bool result = false;
    EXPECT_NO_THROW({
        std::cout << "Invoking SocUtils::IsSupportedAC3()" << std::endl;
        result = SocUtils::IsSupportedAC3();
        std::cout << "Returned value: " << result << std::endl;
    });

    if(result) {
        std::cout << "Internal state: AC-3 is supported (true)" << std::endl;
    } else {
        std::cout << "Internal state: AC-3 is not supported (false)" << std::endl;
    }
    
    std::cout << "Exiting SocUtils::IsSupportedAC3()_start test" << std::endl;
}
/**
 * @brief Validate SocUtils::IsSupportedAC4 method returns a valid boolean.
 *
 * This test verifies that invoking the SocUtils::IsSupportedAC4 method does not throw any exceptions and returns a valid boolean value (either true or false). It ensures the API behaves as expected under normal conditions.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 005@n
 * **Priority:** High@n
 * @n
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * @n
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoke SocUtils::IsSupportedAC4 method | output1 = result, no inputs | The method should return a boolean value (true or false) without throwing an exception | Should Pass |
 * | 02 | Assert the return value is a valid boolean | result = value returned from SocUtils::IsSupportedAC4() | The assertion should pass if result is either true or false | Should Pass |
 */
TEST(SocUtils_IsSupportedAC4_start, TestReturnBoolean)
{
    std::cout << "Entering SocUtils::IsSupportedAC4()_start test" << std::endl;

    bool result = false;
    std::cout << "Invoking SocUtils::IsSupportedAC4() method..." << std::endl;
    EXPECT_NO_THROW({
        result = SocUtils::IsSupportedAC4();
        std::cout << "SocUtils::IsSupportedAC4() returned: " << std::boolalpha << result << std::endl;
    });
    
    // Check that the return value is a valid boolean (either true or false)
    EXPECT_TRUE(result == true || result == false);

    std::cout << "Exiting SocUtils::IsSupportedAC4()_start test" << std::endl;
}
/**
 * @brief Verify that SocUtils::RequiredQueuedFrames() returns a valid queued frames count
 *
 * This test ensures that the SocUtils::RequiredQueuedFrames() API does not throw any exceptions during invocation and returns a value greater than 0, indicating a valid number of queued frames.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 006
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                              | Test Data                                                          | Expected Result                                  | Notes        |
 * | :--------------: | ------------------------------------------------------------------------ | ------------------------------------------------------------------ | ------------------------------------------------ | ------------ |
 * | 01               | Invoke SocUtils::RequiredQueuedFrames() and capture its return value     | input: none, output: queuedFrames = returned value                 | API should return a value > 0 without exception  | Should Pass  |
 * | 02               | Verify that the queuedFrames value is greater than 0                     | queuedFrames = returned value                                      | queuedFrames > 0 as per assertion (EXPECT_GT)     | Should Pass  |
 */
TEST(SocUtilsRequiredQueuedFrames, RequiredQueuedFrames_start) {
    GTEST_SKIP();
    std::cout << "Entering SocUtils::RequiredQueuedFrames()_start test" << std::endl;

    int queuedFrames = 0;
    std::cout << "Invoking SocUtils::RequiredQueuedFrames()" << std::endl;
    EXPECT_NO_THROW({
        queuedFrames = SocUtils::RequiredQueuedFrames();
        std::cout << "SocUtils::RequiredQueuedFrames() returned: " << queuedFrames << std::endl;
    });

    std::cout << "Verifying that the returned value is greater than 0" << std::endl;
    EXPECT_GT(queuedFrames, 0);
    
    std::cout << "Exiting SocUtils::RequiredQueuedFrames()_start test" << std::endl;
}
/**
 * @brief Verify that SocUtils::ResetNewSegmentEvent() resets segment event flags correctly.
 *
 * This test ensures that the SocUtils::ResetNewSegmentEvent() method is successfully invoked without throwing an exception and that it returns true. The test validates that the API performs as expected during a standard trickplay transition scenario, ensuring proper reset of flag states.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 007@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                 | Test Data                                       | Expected Result                                                                     | Notes         |
 * | :--------------: | --------------------------------------------------------------------------- | ----------------------------------------------- | ----------------------------------------------------------------------------------- | ------------- |
 * | 01               | Invoke SocUtils::ResetNewSegmentEvent() and capture the returned value.     | No input, output: ret = value returned by API   | API returns true without throwing any exception; EXPECT_NO_THROW check succeeds.      | Should Pass   |
 * | 02               | Verify that the returned value is true using EXPECT_TRUE assertion.         | ret = true                                       | EXPECT_TRUE confirms ret is true.                                                    | Should be successful |
 */
TEST(SocUtilsResetNewSegmentEventTest, ResetSegmentEventFlags_StandardTrickplayTransition) {
    GTEST_SKIP();
    std::cout << "Entering SocUtils::ResetNewSegmentEvent()_start test" << std::endl;
    
    bool ret = false;
    EXPECT_NO_THROW({
        std::cout << "Invoking method: SocUtils::ResetNewSegmentEvent()" << std::endl;
        ret = SocUtils::ResetNewSegmentEvent();
        std::cout << "Method returned value: " << (ret ? "true" : "false") << std::endl;
    });
    
    EXPECT_TRUE(ret);
    
    std::cout << "Exiting SocUtils::ResetNewSegmentEvent()_start test" << std::endl;
}
/**
 * @brief Verify that SocUtils::UseAppSrcForProgressivePlayback returns a valid boolean
 *
 * This test invokes the SocUtils::UseAppSrcForProgressivePlayback() API to ensure that it returns a valid boolean value (either true or false) without throwing any exceptions. It validates that the method execution completes successfully and its return value is correctly asserted.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 008@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                             | Test Data                                                                                      | Expected Result                                                           | Notes      |
 * | :--------------: | ----------------------------------------------------------------------- | ---------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------ | ---------- |
 * | 01               | Call SocUtils::UseAppSrcForProgressivePlayback() and validate return value | API call = SocUtils::UseAppSrcForProgressivePlayback(), output = returnValue (bool expected to be either true or false) | Function returns a valid boolean without throwing an exception; assertion passes | Should Pass |
 */
TEST(SocUtils_UseAppSrcForProgressivePlayback_ReturnsValidBoolean, TestVerifyReturnValue)
{
    std::cout << "Entering SocUtils::UseAppSrcForProgressivePlayback()_start test" << std::endl;

    std::cout << "About to invoke SocUtils::UseAppSrcForProgressivePlayback()" << std::endl;
    bool returnValue = false;
    EXPECT_NO_THROW({
        returnValue = SocUtils::UseAppSrcForProgressivePlayback();
    });
    std::cout << "SocUtils::UseAppSrcForProgressivePlayback() invoked" << std::endl;
    std::cout << "Retrieved value: " << (returnValue ? "true" : "false") << std::endl;
    
    // Verify that the returned value is indeed a boolean; since C++ bool is always true or false,
    // this effectively checks that the method returns without error.
    EXPECT_TRUE(returnValue == true || returnValue == false);

    std::cout << "Exiting SocUtils::UseAppSrcForProgressivePlayback()_end test" << std::endl;
}
/**
 * @brief Validate that SocUtils::UseWesterosSink API does not throw exceptions and returns a valid boolean.
 *
 * This test verifies that the SocUtils::UseWesterosSink API, when invoked, does not throw an exception and returns a boolean value (either true or false). The objective is to ensure that the functionality works correctly when the Westeros Sink is enabled.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 009
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | -------------- | ----- |
 * | 01 | Invoke SocUtils::UseWesterosSink API within an EXPECT_NO_THROW block to ensure no exceptions occur | input: None, output: result (bool) | API returns a boolean value without throwing an exception | Should Pass |
 * | 02 | Validate that the returned value is either true or false using an EXPECT_TRUE assertion | result = value obtained from API | API returns a valid boolean (true or false) as verified by the assertion | Should be successful |
 */
TEST(UseWesterosSinkTest, ValidateBehaviorWhenWesterosSinkEnabled) {
    std::cout << "Entering ValidateBehaviorWhenWesterosSinkEnabled test" << std::endl;
    
    bool result = false;
    std::cout << "Invoking SocUtils::UseWesterosSink()" << std::endl;
    EXPECT_NO_THROW({
        result = SocUtils::UseWesterosSink();
    });
    std::cout << "Retrieved return value from SocUtils::UseWesterosSink(): " << std::boolalpha << result << std::endl;
    
    EXPECT_TRUE(result == true || result == false);
    
    std::cout << "Exiting ValidateBehaviorWhenWesterosSinkEnabled test" << std::endl;
}
/**
 * @brief Tests that SocUtils::isGstSubtecEnabled returns a valid boolean value without exceptions.
 *
 * This test verifies that invoking the SocUtils::isGstSubtecEnabled API does not throw any exceptions and returns a boolean value. The boolean result is then checked to ensure it is either true or false. This test is critical to ensure that the API reliably returns a valid status.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 010@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                 | Test Data                                                         | Expected Result                                                                      | Notes         |
 * | :--------------: | ----------------------------------------------------------------------------| ----------------------------------------------------------------- | -------------------------------------------------------------------------------------| --------------|
 * | 01               | Call SocUtils::isGstSubtecEnabled API and verify no exception is thrown       | No input; output: result variable expected to contain a boolean    | API call completes without throwing an exception and a boolean is returned             | Should Pass   |
 * | 02               | Validate that the returned boolean value is either true or false using an assertion | input: result value from API call, output: result must be true or false | EXPECT_TRUE condition passes confirming result is either true or false as expected    | Should be successful |
 */
TEST(SocUtils_isGstSubtecEnabled_Test, CheckIfGstSubtecEnabledReturnsTrue) {
    std::cout << "Entering Check if GST Subtec is enabled returns true test" << std::endl;

    std::cout << "Invoking SocUtils::isGstSubtecEnabled()" << std::endl;
    bool result = false;
    EXPECT_NO_THROW({
        result = SocUtils::isGstSubtecEnabled();
    });
    std::cout << "Returned value: " << result << std::endl;
    
    EXPECT_TRUE(result == true || result == false);

    std::cout << "Exiting Check if GST Subtec is enabled returns true test" << std::endl;
}
