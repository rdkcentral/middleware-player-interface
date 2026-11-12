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
 * @file VttCueFillTests.cpp
 * @brief Gap-fill coverage for the VTTCue value type: constructor field
 *        assignment across boundary inputs (zero/negative times, empty and
 *        long strings, settings variants).
 */

#include <gtest/gtest.h>
#include <string>
#include "vttCue.h"

class VttCueFillTests : public ::testing::Test
{
};

TEST_F(VttCueFillTests, CtorStoresAllFields)
{
	VTTCue cue(1.5, 2.25, "hello", "line:0 position:50%");
	EXPECT_DOUBLE_EQ(cue.mStart, 1.5);
	EXPECT_DOUBLE_EQ(cue.mDuration, 2.25);
	EXPECT_EQ(cue.mText, "hello");
	EXPECT_EQ(cue.mSettings, "line:0 position:50%");
}

TEST_F(VttCueFillTests, CtorZeroTimes)
{
	VTTCue cue(0.0, 0.0, "", "");
	EXPECT_DOUBLE_EQ(cue.mStart, 0.0);
	EXPECT_DOUBLE_EQ(cue.mDuration, 0.0);
	EXPECT_TRUE(cue.mText.empty());
	EXPECT_TRUE(cue.mSettings.empty());
}

TEST_F(VttCueFillTests, CtorNegativeStartAccepted)
{
	// struct does not validate - pins current behaviour
	VTTCue cue(-3.0, 1.0, "x", "");
	EXPECT_DOUBLE_EQ(cue.mStart, -3.0);
}

TEST_F(VttCueFillTests, CtorNegativeDurationAccepted)
{
	VTTCue cue(0.0, -2.0, "x", "");
	EXPECT_DOUBLE_EQ(cue.mDuration, -2.0);
}

TEST_F(VttCueFillTests, CtorFractionalPrecision)
{
	VTTCue cue(0.001, 0.0005, "t", "s");
	EXPECT_DOUBLE_EQ(cue.mStart, 0.001);
	EXPECT_DOUBLE_EQ(cue.mDuration, 0.0005);
}

TEST_F(VttCueFillTests, CtorLongTextPreserved)
{
	std::string longText(4096, 'a');
	VTTCue cue(0.0, 1.0, longText, "");
	EXPECT_EQ(cue.mText, longText);
	EXPECT_EQ(cue.mText.size(), 4096u);
}

TEST_F(VttCueFillTests, CtorMultilineTextPreserved)
{
	std::string text = "line one\nline two\r\nline three";
	VTTCue cue(0.0, 1.0, text, "");
	EXPECT_EQ(cue.mText, text);
}

TEST_F(VttCueFillTests, CtorUtf8TextPreserved)
{
	std::string text = "\xE4\xBD\xA0\xE5\xA5\xBD"; // UTF-8 CJK bytes
	VTTCue cue(0.0, 1.0, text, "");
	EXPECT_EQ(cue.mText, text);
}

TEST_F(VttCueFillTests, CtorSettingsVariants)
{
	VTTCue cue(0.0, 1.0, "x", "vertical:rl");
	EXPECT_EQ(cue.mSettings, "vertical:rl");
	VTTCue cue2(0.0, 1.0, "x", "align:center size:50%");
	EXPECT_EQ(cue2.mSettings, "align:center size:50%");
}

TEST_F(VttCueFillTests, CopySemanticsValueType)
{
	VTTCue a(1.0, 2.0, "t", "s");
	VTTCue b = a;
	EXPECT_DOUBLE_EQ(b.mStart, 1.0);
	EXPECT_EQ(b.mText, "t");
	// copies are independent
	b.mStart = 9.0;
	EXPECT_DOUBLE_EQ(a.mStart, 1.0);
}
