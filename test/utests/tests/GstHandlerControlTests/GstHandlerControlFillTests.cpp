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
 * @file GstHandlerControlFillTests.cpp
 * @brief Gap-fill coverage for GstHandlerControl: enable/disable semantics,
 *        ScopeHelper RAII counting, move-assignment, nested scopes, and the
 *        waitForDone() timeout/completion matrix with a real worker thread.
 */

#include <gtest/gtest.h>
#include <atomic>
#include <chrono>
#include <thread>
#include <vector>

#include "GstHandlerControl.h"

class GstHandlerControlFillTests : public ::testing::Test
{
protected:
	GstHandlerControl mControl;
};

/* ================================================================== */
/* enable / disable / isEnabled                                       */
/* ================================================================== */

TEST_F(GstHandlerControlFillTests, InitiallyEnabled)
{
	EXPECT_TRUE(mControl.isEnabled());
}

TEST_F(GstHandlerControlFillTests, DisableClearsEnabled)
{
	mControl.disable();
	EXPECT_FALSE(mControl.isEnabled());
}

TEST_F(GstHandlerControlFillTests, EnableSetsEnabled)
{
	mControl.disable();
	mControl.enable();
	EXPECT_TRUE(mControl.isEnabled());
}

TEST_F(GstHandlerControlFillTests, DoubleDisableStaysDisabled)
{
	mControl.disable();
	mControl.disable();
	EXPECT_FALSE(mControl.isEnabled());
}

TEST_F(GstHandlerControlFillTests, EnableDisableCycles)
{
	for (int i = 0; i < 5; i++)
	{
		mControl.enable();
		EXPECT_TRUE(mControl.isEnabled());
		mControl.disable();
		EXPECT_FALSE(mControl.isEnabled());
	}
}

/* ================================================================== */
/* ScopeHelper counting                                               */
/* ================================================================== */

TEST_F(GstHandlerControlFillTests, ScopeHelperCountsSingleScope)
{
	EXPECT_TRUE(mControl.waitForDone(10, "idle")); // no scopes outstanding
	{
		auto scope = mControl.getScopeHelper();
		EXPECT_FALSE(mControl.waitForDone(30, "onescope")); // still held -> times out
	}
	EXPECT_TRUE(mControl.waitForDone(100, "released"));
}

TEST_F(GstHandlerControlFillTests, NestedScopeHelpersRequireAllToExit)
{
	{
		auto s1 = mControl.getScopeHelper();
		{
			auto s2 = mControl.getScopeHelper();
			EXPECT_FALSE(mControl.waitForDone(20, "nested"));
		} // s2 released - s1 still held
		EXPECT_FALSE(mControl.waitForDone(20, "outerheld"));
	}
	EXPECT_TRUE(mControl.waitForDone(100, "allreleased"));
}

TEST_F(GstHandlerControlFillTests, ScopeHelperMoveAssignTransfersOwnership)
{
	{
		auto s1 = mControl.getScopeHelper();
		auto s2 = mControl.getScopeHelper(); // count == 2
		s1 = std::move(s2);                  // s1's old scope ends -> count back to 1
		EXPECT_FALSE(mControl.waitForDone(20, "stillone"));
	} // moved scope ends
	EXPECT_TRUE(mControl.waitForDone(100, "done"));
}

/* ================================================================== */
/* waitForDone                                                        */
/* ================================================================== */

TEST_F(GstHandlerControlFillTests, WaitForDoneNegativeDelayImmediateFalse)
{
	EXPECT_FALSE(mControl.waitForDone(-1, "neg"));
	EXPECT_FALSE(mControl.waitForDone(-500, "neg2"));
}

TEST_F(GstHandlerControlFillTests, WaitForDoneZeroInstancesImmediate)
{
	EXPECT_TRUE(mControl.waitForDone(0, "none"));
}

TEST_F(GstHandlerControlFillTests, WaitForDoneDisablesControl)
{
	mControl.waitForDone(0, "x");
	EXPECT_FALSE(mControl.isEnabled());
}

TEST_F(GstHandlerControlFillTests, WaitForDoneTimesOutWhileHeld)
{
	auto scope = mControl.getScopeHelper();
	auto start = std::chrono::steady_clock::now();
	EXPECT_FALSE(mControl.waitForDone(50, "held"));
	auto elapsed = std::chrono::steady_clock::now() - start;
	EXPECT_GE(std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count(), 40);
}

TEST_F(GstHandlerControlFillTests, WaitForDoneReturnsWhenScopeEndsEarly)
{
	std::atomic<bool> finished{false};
	auto scope = mControl.getScopeHelper();

	std::thread t([&]
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(40));
		finished = mControl.waitForDone(2000, "worker");
	});

	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	{ auto releaseMe = std::move(scope); } // release scope while waiter sleeps
	t.join();
	EXPECT_TRUE(finished);
}

TEST_F(GstHandlerControlFillTests, ReturnStraightAwayEnabledFalse)
{
	auto scope = mControl.getScopeHelper();
	EXPECT_FALSE(scope.returnStraightAway());
}

TEST_F(GstHandlerControlFillTests, ReturnStraightAwayDisabledTrue)
{
	mControl.disable();
	auto scope = mControl.getScopeHelper();
	EXPECT_TRUE(scope.returnStraightAway());
}

TEST_F(GstHandlerControlFillTests, ReturnStraightAwayNullControllerTrue)
{
	GstHandlerControl::ScopeHelper scope; // default ctor - no controller
	EXPECT_TRUE(scope.returnStraightAway());
}

TEST_F(GstHandlerControlFillTests, ScopeCountedEvenWhenDisabled)
{
	mControl.disable();
	{
		auto scope = mControl.getScopeHelper(); // counts despite being disabled
		EXPECT_TRUE(scope.returnStraightAway());
		EXPECT_FALSE(mControl.waitForDone(20, "disabledscope"));
	}
	EXPECT_TRUE(mControl.waitForDone(100, "released"));
}

TEST_F(GstHandlerControlFillTests, MultipleScopesReleasedByWorker)
{
	std::vector<std::thread> threads;
	for (int i = 0; i < 3; i++)
	{
		threads.emplace_back([this]
		{
			auto scope = mControl.getScopeHelper();
			std::this_thread::sleep_for(std::chrono::milliseconds(15));
		});
	}
	for (auto &t : threads)
	{
		t.join();
	}
	EXPECT_TRUE(mControl.waitForDone(500, "multi"));
}
