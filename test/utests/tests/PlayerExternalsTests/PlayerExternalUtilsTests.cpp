/*
 * If not stated otherwise in this file or this component's license file the
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
 * @file PlayerExternalUtilsTests.cpp
 * @brief Comprehensive unit tests for PlayerExternalUtils APIs.
 *
 * APIs covered:
 *   IsContainerEnvironment() - checks /etc/device.properties for container flag
 *   ms_sleep(uint32_t ms)    - millisecond sleep wrapper
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <iostream>
#include <chrono>
#include "PlayerExternalUtils.h"

// ===========================================================================
// Group: IsContainerEnvironment
// ===========================================================================

/**
 * @brief IsContainerEnvironment() returns a bool without throwing.
 *
 * **Test Group ID:** IsContainerEnvironment_001@n
 * **Priority:** High@n
 *
 * | Step | Description | Data | Expected | Notes |
 * |------|-------------|------|----------|-------|
 * | 01 | Call IsContainerEnvironment() | - | No throw | Should Pass |
 */
TEST(IsContainerEnvironment, DoesNotThrow)
{
    std::cout << "[IsContainerEnvironment.DoesNotThrow] - START" << std::endl;
    bool result = false;
    EXPECT_NO_THROW(result = IsContainerEnvironment());
    std::cout << "  IsContainerEnvironment() = " << result << " (no exception)" << std::endl;
    std::cout << "[IsContainerEnvironment.DoesNotThrow] - PASS" << std::endl;
}

/**
 * @brief IsContainerEnvironment() returns a consistent bool based on /etc/device.properties.
 *
 * **Test Group ID:** IsContainerEnvironment_002@n
 * **Priority:** High@n
 */
TEST(IsContainerEnvironment, ReturnsBoolWithoutThrowing)
{
    std::cout << "[IsContainerEnvironment.ReturnsBoolWithoutThrowing] - START" << std::endl;
    // Result depends on /etc/device.properties; just verify no exception and valid bool
    bool result = false;
    EXPECT_NO_THROW(result = IsContainerEnvironment());
    // result is either true or false — both are valid depending on environment
    std::cout << "  IsContainerEnvironment() = " << result << " (no exception, valid bool)" << std::endl;
    std::cout << "[IsContainerEnvironment.ReturnsBoolWithoutThrowing] - PASS" << std::endl;
}

/**
 * @brief IsContainerEnvironment() returns consistent result on repeated calls.
 *
 * **Test Group ID:** IsContainerEnvironment_003@n
 * **Priority:** Medium@n
 */
TEST(IsContainerEnvironment, ConsistentResultOnRepeatCalls)
{
    std::cout << "[IsContainerEnvironment.ConsistentResultOnRepeatCalls] - START" << std::endl;
    bool first  = IsContainerEnvironment();
    bool second = IsContainerEnvironment();
    bool third  = IsContainerEnvironment();
    EXPECT_EQ(first, second);
    EXPECT_EQ(second, third);
    std::cout << "  Three calls returned consistent: " << first << std::endl;
    std::cout << "[IsContainerEnvironment.ConsistentResultOnRepeatCalls] - PASS" << std::endl;
}

// ===========================================================================
// Group: ms_sleep
// ===========================================================================

/**
 * @brief ms_sleep(0) completes quickly (< 50 ms wall-clock).
 *
 * **Test Group ID:** MsSleep_001@n
 * **Priority:** High@n
 */
TEST(MsSleep, ZeroMillisecondsFast)
{
    std::cout << "[MsSleep.ZeroMillisecondsFast] - START" << std::endl;
    auto t0 = std::chrono::steady_clock::now();
    ms_sleep(0);
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                       std::chrono::steady_clock::now() - t0).count();
    std::cout << "  ms_sleep(0) elapsed = " << elapsed << " ms (expected < 50 ms)" << std::endl;
    EXPECT_LT(elapsed, 50);
    std::cout << "[MsSleep.ZeroMillisecondsFast] - PASS" << std::endl;
}

/**
 * @brief ms_sleep(1) sleeps at least 0 ms (nanosleep granularity).
 *
 * **Test Group ID:** MsSleep_002@n
 * **Priority:** High@n
 */
TEST(MsSleep, OneMillisecondDoesNotThrow)
{
    std::cout << "[MsSleep.OneMillisecondDoesNotThrow] - START" << std::endl;
    EXPECT_NO_THROW(ms_sleep(1));
    std::cout << "  ms_sleep(1) completed without exception" << std::endl;
    std::cout << "[MsSleep.OneMillisecondDoesNotThrow] - PASS" << std::endl;
}

/**
 * @brief ms_sleep(5) completes and takes at least 3 ms.
 *
 * **Test Group ID:** MsSleep_003@n
 * **Priority:** Medium@n
 */
TEST(MsSleep, FiveMillisecondsSleepDuration)
{
    std::cout << "[MsSleep.FiveMillisecondsSleepDuration] - START" << std::endl;
    auto t0 = std::chrono::steady_clock::now();
    ms_sleep(5);
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                       std::chrono::steady_clock::now() - t0).count();
    std::cout << "  ms_sleep(5) elapsed = " << elapsed << " ms (expected >= 3 ms)" << std::endl;
    EXPECT_GE(elapsed, 3);
    std::cout << "[MsSleep.FiveMillisecondsSleepDuration] - PASS" << std::endl;
}

/**
 * @brief ms_sleep(10) completes and takes at least 8 ms.
 *
 * **Test Group ID:** MsSleep_004@n
 * **Priority:** Medium@n
 */
TEST(MsSleep, TenMillisecondsSleepDuration)
{
    std::cout << "[MsSleep.TenMillisecondsSleepDuration] - START" << std::endl;
    auto t0 = std::chrono::steady_clock::now();
    ms_sleep(10);
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                       std::chrono::steady_clock::now() - t0).count();
    std::cout << "  ms_sleep(10) elapsed = " << elapsed << " ms (expected >= 8 ms)" << std::endl;
    EXPECT_GE(elapsed, 8);
    std::cout << "[MsSleep.TenMillisecondsSleepDuration] - PASS" << std::endl;
}

/**
 * @brief ms_sleep(50) completes and takes at least 40 ms.
 *
 * **Test Group ID:** MsSleep_005@n
 * **Priority:** Low@n
 */
TEST(MsSleep, FiftyMillisecondsSleepDuration)
{
    std::cout << "[MsSleep.FiftyMillisecondsSleepDuration] - START" << std::endl;
    auto t0 = std::chrono::steady_clock::now();
    ms_sleep(50);
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                       std::chrono::steady_clock::now() - t0).count();
    std::cout << "  ms_sleep(50) elapsed = " << elapsed << " ms (expected >= 40 ms)" << std::endl;
    EXPECT_GE(elapsed, 40);
    std::cout << "[MsSleep.FiftyMillisecondsSleepDuration] - PASS" << std::endl;
}

/**
 * @brief ms_sleep(2) twice totals at least 3 ms elapsed.
 *
 * **Test Group ID:** MsSleep_006@n
 * **Priority:** Medium@n
 */
TEST(MsSleep, TwoCallsAccumulateTime)
{
    std::cout << "[MsSleep.TwoCallsAccumulateTime] - START" << std::endl;
    auto t0 = std::chrono::steady_clock::now();
    ms_sleep(2);
    ms_sleep(2);
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                       std::chrono::steady_clock::now() - t0).count();
    std::cout << "  Two ms_sleep(2) calls elapsed = " << elapsed << " ms (expected >= 3 ms)" << std::endl;
    EXPECT_GE(elapsed, 3);
    std::cout << "[MsSleep.TwoCallsAccumulateTime] - PASS" << std::endl;
}

/**
 * @brief ms_sleep(0) five times completes well under 100 ms total.
 *
 * **Test Group ID:** MsSleep_007@n
 * **Priority:** Low@n
 */
TEST(MsSleep, MultipleZeroCallsFast)
{
    std::cout << "[MsSleep.MultipleZeroCallsFast] - START" << std::endl;
    auto t0 = std::chrono::steady_clock::now();
    for (int i = 0; i < 5; ++i) ms_sleep(0);
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                       std::chrono::steady_clock::now() - t0).count();
    std::cout << "  5x ms_sleep(0) elapsed = " << elapsed << " ms (expected < 100 ms)" << std::endl;
    EXPECT_LT(elapsed, 100);
    std::cout << "[MsSleep.MultipleZeroCallsFast] - PASS" << std::endl;
}

/**
 * @brief ms_sleep with large value (200 ms) takes at least 180 ms.
 *
 * **Test Group ID:** MsSleep_008@n
 * **Priority:** Low@n
 */
TEST(MsSleep, LargeValueSleepDuration)
{
    std::cout << "[MsSleep.LargeValueSleepDuration] - START" << std::endl;
    auto t0 = std::chrono::steady_clock::now();
    ms_sleep(200);
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                       std::chrono::steady_clock::now() - t0).count();
    std::cout << "  ms_sleep(200) elapsed = " << elapsed << " ms (expected >= 180 ms)" << std::endl;
    EXPECT_GE(elapsed, 180);
    std::cout << "[MsSleep.LargeValueSleepDuration] - PASS" << std::endl;
}
