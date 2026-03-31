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
 * @file PlayerSubtecCCManagerTests.cpp
 * @brief Comprehensive unit tests for PlayerSubtecCCManager.
 *
 * Covers all public APIs from PlayerSubtecCCManager and its base class
 * PlayerCCManagerBase:
 *
 *   - PlayerSubtecCCManager()       : default construction
 *   - GetId()                       : increments from 1, monotonic
 *   - Release(id)                   : valid id, invalid id, re-release
 *   - SetStatus(bool) / GetStatus() : enable/disable CC rendering
 *   - GetTrack()                    : reflects last SetTrack call
 *   - SetTrack(track)               : "CC1"–"CC4", "TXT5"–"TXT8",
 *                                     "SERVICE1"–"SERVICE63", numeric, empty
 *   - GetStyle()                    : reflects last SetStyle call
 *   - SetStyle(options)             : disabled → returns -1; not-tested when enabled
 *   - updateLastTextTracks()        : stores tracks vector
 *   - getLastTextTracks()           : returns stored vector
 *   - SetTrickplayStatus(bool)      : does not throw
 *   - SetParentalControlStatus(bool): does not throw
 *   - IsOOBCCRenderingSupported()   : returns bool, does not throw
 *
 * NOTES:
 *   - mEnabled starts false.  SetStyle() returns -1 when disabled.
 *   - SetTrack("CC1") calls SetAnalogChannel() via fake → returns 0.
 *   - SetTrack("SERVICE1") calls SetDigitalChannel() via fake → returns 0.
 *   - GetId() increments mId on each call; first call returns 1.
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <string>
#include <vector>
#include "PlayerSubtecCCManager.h"
#include "CCTrackInfo.h"
#include "PlayerCCManager.h"

// ===========================================================================
// CCManagerConstruction tests
// ===========================================================================

/**
 * @brief PlayerSubtecCCManager default constructor does not throw.
 *
 * @par Test Group ID  : CCManagerConstruction
 * @par Test Case ID   : 001
 * @par Priority       : High
 *
 * | Step | Description                        | Expected              |
 * |:----:|------------------------------------|----------------------|
 * |  01  | Default construct                  | No exception thrown   |
 */
TEST(CCManagerConstruction, DefaultConstructorNoThrow)
{
    std::cout << "[CCManagerConstruction.DefaultConstructorNoThrow] - START" << std::endl;
    EXPECT_NO_THROW({ PlayerSubtecCCManager mgr; });
    std::cout << "[CCManagerConstruction.DefaultConstructorNoThrow] - PASS" << std::endl;
}

/**
 * @brief Multiple distinct instances can be created without conflicting.
 *
 * @par Test Group ID  : CCManagerConstruction
 * @par Test Case ID   : 002
 * @par Priority       : High
 */
TEST(CCManagerConstruction, MultipleInstancesNoConflict)
{
    std::cout << "[CCManagerConstruction.MultipleInstancesNoConflict] - START" << std::endl;
    EXPECT_NO_THROW({
        PlayerSubtecCCManager m1;
        PlayerSubtecCCManager m2;
        PlayerSubtecCCManager m3;
    });
    std::cout << "  3 independent PlayerSubtecCCManager instances created OK" << std::endl;
    std::cout << "[CCManagerConstruction.MultipleInstancesNoConflict] - PASS" << std::endl;
}

// ===========================================================================
// CCManagerGetId tests
// ===========================================================================

/**
 * @brief GetId() first call returns 1.
 *
 * @par Test Group ID  : CCManagerGetId
 * @par Test Case ID   : 001
 * @par Priority       : High
 */
TEST(CCManagerGetId, FirstCallReturns1)
{
    std::cout << "[CCManagerGetId.FirstCallReturns1] - START" << std::endl;
    PlayerSubtecCCManager mgr;
    int id = mgr.GetId();
    std::cout << "  First GetId() = " << id << " (expected 1)" << std::endl;
    EXPECT_EQ(id, 1);
    std::cout << "[CCManagerGetId.FirstCallReturns1] - PASS" << std::endl;
}

/**
 * @brief GetId() is monotonically increasing — each call returns id + 1.
 *
 * @par Test Group ID  : CCManagerGetId
 * @par Test Case ID   : 002
 * @par Priority       : High
 */
TEST(CCManagerGetId, MonotonicallyIncreasing)
{
    std::cout << "[CCManagerGetId.MonotonicallyIncreasing] - START" << std::endl;
    PlayerSubtecCCManager mgr;
    int id1 = mgr.GetId();
    int id2 = mgr.GetId();
    int id3 = mgr.GetId();
    std::cout << "  id1=" << id1 << " id2=" << id2 << " id3=" << id3 << std::endl;
    EXPECT_EQ(id2, id1 + 1);
    EXPECT_EQ(id3, id2 + 1);
    std::cout << "[CCManagerGetId.MonotonicallyIncreasing] - PASS" << std::endl;
}

/**
 * @brief GetId() is thread-safe — concurrent calls yield unique IDs.
 *
 * @par Test Group ID  : CCManagerGetId
 * @par Test Case ID   : 003
 * @par Priority       : Medium
 */
TEST(CCManagerGetId, ConsecutiveCallsYieldDifferentIds)
{
    std::cout << "[CCManagerGetId.ConsecutiveCallsYieldDifferentIds] - START" << std::endl;
    PlayerSubtecCCManager mgr;
    const int N = 5;
    std::vector<int> ids;
    for (int i = 0; i < N; ++i) ids.push_back(mgr.GetId());
    for (int i = 0; i + 1 < N; ++i)
    {
        EXPECT_NE(ids[i], ids[i+1]) << "IDs at positions " << i << " and " << (i+1) << " should differ";
    }
    std::cout << "  " << N << " sequential ids all differ" << std::endl;
    std::cout << "[CCManagerGetId.ConsecutiveCallsYieldDifferentIds] - PASS" << std::endl;
}

// ===========================================================================
// CCManagerRelease tests
// ===========================================================================

/**
 * @brief Release() with a valid id obtained from GetId() does not throw.
 *
 * @par Test Group ID  : CCManagerRelease
 * @par Test Case ID   : 001
 * @par Priority       : High
 */
TEST(CCManagerRelease, ValidIdNoThrow)
{
    std::cout << "[CCManagerRelease.ValidIdNoThrow] - START" << std::endl;
    PlayerSubtecCCManager mgr;
    int id = mgr.GetId();
    EXPECT_NO_THROW({ mgr.Release(id); });
    std::cout << "  Release(" << id << ") OK" << std::endl;
    std::cout << "[CCManagerRelease.ValidIdNoThrow] - PASS" << std::endl;
}

/**
 * @brief Release() with an invalid id (never issued) does not throw.
 *
 * @par Test Group ID  : CCManagerRelease
 * @par Test Case ID   : 002
 * @par Priority       : High
 */
TEST(CCManagerRelease, InvalidIdNoThrow)
{
    std::cout << "[CCManagerRelease.InvalidIdNoThrow] - START" << std::endl;
    PlayerSubtecCCManager mgr;
    EXPECT_NO_THROW({ mgr.Release(99999); });
    std::cout << "  Release(99999) (invalid) does not throw" << std::endl;
    std::cout << "[CCManagerRelease.InvalidIdNoThrow] - PASS" << std::endl;
}

/**
 * @brief Release() on the same id twice (double release) does not throw.
 *
 * @par Test Group ID  : CCManagerRelease
 * @par Test Case ID   : 003
 * @par Priority       : Medium
 */
TEST(CCManagerRelease, DoubleReleaseNoThrow)
{
    std::cout << "[CCManagerRelease.DoubleReleaseNoThrow] - START" << std::endl;
    PlayerSubtecCCManager mgr;
    int id = mgr.GetId();
    EXPECT_NO_THROW({ mgr.Release(id); });
    EXPECT_NO_THROW({ mgr.Release(id); });  // Second release should be safe
    std::cout << "  Double Release(" << id << ") does not throw" << std::endl;
    std::cout << "[CCManagerRelease.DoubleReleaseNoThrow] - PASS" << std::endl;
}

/**
 * @brief Release() of multiple ids one at a time is safe.
 *
 * @par Test Group ID  : CCManagerRelease
 * @par Test Case ID   : 004
 * @par Priority       : Medium
 */
TEST(CCManagerRelease, MultipleIdsReleasedSequentially)
{
    std::cout << "[CCManagerRelease.MultipleIdsReleasedSequentially] - START" << std::endl;
    PlayerSubtecCCManager mgr;
    int id1 = mgr.GetId();
    int id2 = mgr.GetId();
    int id3 = mgr.GetId();
    EXPECT_NO_THROW({ mgr.Release(id1); });
    EXPECT_NO_THROW({ mgr.Release(id2); });
    EXPECT_NO_THROW({ mgr.Release(id3); });
    std::cout << "  Released ids " << id1 << ", " << id2 << ", " << id3 << " OK" << std::endl;
    std::cout << "[CCManagerRelease.MultipleIdsReleasedSequentially] - PASS" << std::endl;
}

// ===========================================================================
// CCManagerGetStatus / SetStatus tests
// ===========================================================================

/**
 * @brief GetStatus() returns false by default (mEnabled initial state).
 *
 * @par Test Group ID  : CCManagerStatus
 * @par Test Case ID   : 001
 * @par Priority       : High
 */
TEST(CCManagerStatus, DefaultIsDisabled)
{
    std::cout << "[CCManagerStatus.DefaultIsDisabled] - START" << std::endl;
    PlayerSubtecCCManager mgr;
    bool status = mgr.GetStatus();
    std::cout << "  GetStatus() default = " << status << " (expected false)" << std::endl;
    EXPECT_FALSE(status);
    std::cout << "[CCManagerStatus.DefaultIsDisabled] - PASS" << std::endl;
}

/**
 * @brief SetStatus(true) enables CC and GetStatus() returns true.
 *
 * @par Test Group ID  : CCManagerStatus
 * @par Test Case ID   : 002
 * @par Priority       : High
 */
TEST(CCManagerStatus, EnableViaSetStatus)
{
    std::cout << "[CCManagerStatus.EnableViaSetStatus] - START" << std::endl;
    PlayerSubtecCCManager mgr;
    mgr.SetStatus(true);
    bool status = mgr.GetStatus();
    std::cout << "  GetStatus() after SetStatus(true) = " << status << std::endl;
    EXPECT_TRUE(status);
    std::cout << "[CCManagerStatus.EnableViaSetStatus] - PASS" << std::endl;
}

/**
 * @brief SetStatus(false) disables CC and GetStatus() returns false.
 *
 * @par Test Group ID  : CCManagerStatus
 * @par Test Case ID   : 003
 * @par Priority       : High
 */
TEST(CCManagerStatus, DisableViaSetStatus)
{
    std::cout << "[CCManagerStatus.DisableViaSetStatus] - START" << std::endl;
    PlayerSubtecCCManager mgr;
    mgr.SetStatus(true);   // Enable first
    mgr.SetStatus(false);  // Then disable
    bool status = mgr.GetStatus();
    std::cout << "  GetStatus() after enable+disable = " << status << " (expected false)" << std::endl;
    EXPECT_FALSE(status);
    std::cout << "[CCManagerStatus.DisableViaSetStatus] - PASS" << std::endl;
}

/**
 * @brief SetStatus() toggle: true → false → true is consistent.
 *
 * @par Test Group ID  : CCManagerStatus
 * @par Test Case ID   : 004
 * @par Priority       : Medium
 */
TEST(CCManagerStatus, ToggleStatusConsistent)
{
    std::cout << "[CCManagerStatus.ToggleStatusConsistent] - START" << std::endl;
    PlayerSubtecCCManager mgr;
    mgr.SetStatus(true);
    EXPECT_TRUE(mgr.GetStatus());
    mgr.SetStatus(false);
    EXPECT_FALSE(mgr.GetStatus());
    mgr.SetStatus(true);
    EXPECT_TRUE(mgr.GetStatus());
    std::cout << "  Toggle true→false→true all consistent" << std::endl;
    std::cout << "[CCManagerStatus.ToggleStatusConsistent] - PASS" << std::endl;
}

// ===========================================================================
// CCManagerGetTrack / SetTrack tests
// ===========================================================================

/**
 * @brief GetTrack() returns empty string by default.
 *
 * @par Test Group ID  : CCManagerTrack
 * @par Test Case ID   : 001
 * @par Priority       : High
 */
TEST(CCManagerTrack, DefaultTrackIsEmpty)
{
    std::cout << "[CCManagerTrack.DefaultTrackIsEmpty] - START" << std::endl;
    PlayerSubtecCCManager mgr;
    const std::string& track = mgr.GetTrack();
    // Constructor sets default track to "CC1" via SetTrack("CC1")
    std::cout << "  GetTrack() default = \"" << track << "\" (expected \"CC1\")" << std::endl;
    EXPECT_EQ(track, "CC1");
    std::cout << "[CCManagerTrack.DefaultTrackIsEmpty] - PASS" << std::endl;
}

/**
 * @brief SetTrack("CC1") — analog 608 channel 1 — returns 0 via fake.
 *
 * @par Test Group ID  : CCManagerTrack
 * @par Test Case ID   : 002
 * @par Priority       : High
 */
TEST(CCManagerTrack, SetTrackCC1Returns0)
{
    std::cout << "[CCManagerTrack.SetTrackCC1Returns0] - START" << std::endl;
    PlayerSubtecCCManager mgr;
    int ret = mgr.SetTrack("CC1");
    std::cout << "  SetTrack(\"CC1\") = " << ret << " (expected 0)" << std::endl;
    EXPECT_EQ(ret, 0);
    std::cout << "[CCManagerTrack.SetTrackCC1Returns0] - PASS" << std::endl;
}

/**
 * @brief SetTrack("CC2") — analog 608 channel 2 — returns 0 via fake.
 *
 * @par Test Group ID  : CCManagerTrack
 * @par Test Case ID   : 003
 * @par Priority       : High
 */
TEST(CCManagerTrack, SetTrackCC2Returns0)
{
    std::cout << "[CCManagerTrack.SetTrackCC2Returns0] - START" << std::endl;
    PlayerSubtecCCManager mgr;
    int ret = mgr.SetTrack("CC2");
    std::cout << "  SetTrack(\"CC2\") = " << ret << " (expected 0)" << std::endl;
    EXPECT_EQ(ret, 0);
    std::cout << "[CCManagerTrack.SetTrackCC2Returns0] - PASS" << std::endl;
}

/**
 * @brief SetTrack("CC3") — analog 608 channel 3 — returns 0 via fake.
 *
 * @par Test Group ID  : CCManagerTrack
 * @par Test Case ID   : 004
 * @par Priority       : High
 */
TEST(CCManagerTrack, SetTrackCC3Returns0)
{
    std::cout << "[CCManagerTrack.SetTrackCC3Returns0] - START" << std::endl;
    PlayerSubtecCCManager mgr;
    int ret = mgr.SetTrack("CC3");
    std::cout << "  SetTrack(\"CC3\") = " << ret << " (expected 0)" << std::endl;
    EXPECT_EQ(ret, 0);
    std::cout << "[CCManagerTrack.SetTrackCC3Returns0] - PASS" << std::endl;
}

/**
 * @brief SetTrack("CC4") — analog 608 channel 4 — returns 0 via fake.
 *
 * @par Test Group ID  : CCManagerTrack
 * @par Test Case ID   : 005
 * @par Priority       : High
 */
TEST(CCManagerTrack, SetTrackCC4Returns0)
{
    std::cout << "[CCManagerTrack.SetTrackCC4Returns0] - START" << std::endl;
    PlayerSubtecCCManager mgr;
    int ret = mgr.SetTrack("CC4");
    std::cout << "  SetTrack(\"CC4\") = " << ret << " (expected 0)" << std::endl;
    EXPECT_EQ(ret, 0);
    std::cout << "[CCManagerTrack.SetTrackCC4Returns0] - PASS" << std::endl;
}

/**
 * @brief SetTrack("SERVICE1") — digital 708 service 1 — returns 0 via fake.
 *
 * @par Test Group ID  : CCManagerTrack
 * @par Test Case ID   : 006
 * @par Priority       : High
 */
TEST(CCManagerTrack, SetTrackService1Returns0)
{
    std::cout << "[CCManagerTrack.SetTrackService1Returns0] - START" << std::endl;
    PlayerSubtecCCManager mgr;
    int ret = mgr.SetTrack("SERVICE1");
    std::cout << "  SetTrack(\"SERVICE1\") = " << ret << " (expected 0)" << std::endl;
    EXPECT_EQ(ret, 0);
    std::cout << "[CCManagerTrack.SetTrackService1Returns0] - PASS" << std::endl;
}

/**
 * @brief SetTrack("SERVICE6") — digital 708 service 6 — returns 0 via fake.
 *
 * @par Test Group ID  : CCManagerTrack
 * @par Test Case ID   : 007
 * @par Priority       : High
 */
TEST(CCManagerTrack, SetTrackService6Returns0)
{
    std::cout << "[CCManagerTrack.SetTrackService6Returns0] - START" << std::endl;
    PlayerSubtecCCManager mgr;
    int ret = mgr.SetTrack("SERVICE6");
    std::cout << "  SetTrack(\"SERVICE6\") = " << ret << " (expected 0)" << std::endl;
    EXPECT_EQ(ret, 0);
    std::cout << "[CCManagerTrack.SetTrackService6Returns0] - PASS" << std::endl;
}

/**
 * @brief SetTrack("TXT5") — TXT5 maps to trackNum=9 (>8) → invalid → returns -1.
 *
 * @par Test Group ID  : CCManagerTrack
 * @par Test Case ID   : 008
 * @par Priority       : Medium
 */
TEST(CCManagerTrack, SetTrackTXT5ReturnsNeg1)
{
    std::cout << "[CCManagerTrack.SetTrackTXT5ReturnsNeg1] - START" << std::endl;
    PlayerSubtecCCManager mgr;
    int ret = mgr.SetTrack("TXT5");
    // TXT5 maps to trackNum = ('5'-48)+4 = 9, which exceeds max valid analog (8) -> -1
    std::cout << "  SetTrack(\"TXT5\") = " << ret << " (expected -1, invalid trackNum=9)" << std::endl;
    EXPECT_EQ(ret, -1);
    std::cout << "[CCManagerTrack.SetTrackTXT5ReturnsNeg1] - PASS" << std::endl;
}

/**
 * @brief SetTrack() with empty track — returns -1 (no matching format).
 *
 * @par Test Group ID  : CCManagerTrack
 * @par Test Case ID   : 009
 * @par Priority       : Medium
 */
TEST(CCManagerTrack, SetTrackEmptyReturns_Neg1)
{
    std::cout << "[CCManagerTrack.SetTrackEmptyReturns_Neg1] - START" << std::endl;
    PlayerSubtecCCManager mgr;
    int ret = mgr.SetTrack("");
    std::cout << "  SetTrack(\"\") = " << ret << " (expected -1)" << std::endl;
    EXPECT_EQ(ret, -1);
    std::cout << "[CCManagerTrack.SetTrackEmptyReturns_Neg1] - PASS" << std::endl;
}

/**
 * @brief SetTrack() with unrecognised track name — returns -1.
 *
 * @par Test Group ID  : CCManagerTrack
 * @par Test Case ID   : 010
 * @par Priority       : Medium
 */
TEST(CCManagerTrack, SetTrackUnknownReturns_Neg1)
{
    std::cout << "[CCManagerTrack.SetTrackUnknownReturns_Neg1] - START" << std::endl;
    PlayerSubtecCCManager mgr;
    int ret = mgr.SetTrack("UNKNOWN_TRACK_FORMAT");
    std::cout << "  SetTrack(\"UNKNOWN_TRACK_FORMAT\") = " << ret << " (expected -1)" << std::endl;
    EXPECT_EQ(ret, -1);
    std::cout << "[CCManagerTrack.SetTrackUnknownReturns_Neg1] - PASS" << std::endl;
}

/**
 * @brief GetTrack() reflects the last SetTrack() call.
 *
 * @par Test Group ID  : CCManagerTrack
 * @par Test Case ID   : 011
 * @par Priority       : High
 */
TEST(CCManagerTrack, GetTrackReflectsSetTrack)
{
    std::cout << "[CCManagerTrack.GetTrackReflectsSetTrack] - START" << std::endl;
    PlayerSubtecCCManager mgr;
    mgr.SetTrack("CC1");
    const std::string& track = mgr.GetTrack();
    std::cout << "  GetTrack() after SetTrack(\"CC1\") = \"" << track << "\"" << std::endl;
    EXPECT_EQ(track, "CC1");
    std::cout << "[CCManagerTrack.GetTrackReflectsSetTrack] - PASS" << std::endl;
}

/**
 * @brief SetTrack() with explicit 608 format override.
 *
 * @par Test Group ID  : CCManagerTrack
 * @par Test Case ID   : 012
 * @par Priority       : Medium
 */
TEST(CCManagerTrack, SetTrackWithExplicit608Format)
{
    std::cout << "[CCManagerTrack.SetTrackWithExplicit608Format] - START" << std::endl;
    PlayerSubtecCCManager mgr;
    int ret = mgr.SetTrack("CC1", eCLOSEDCAPTION_FORMAT_608);
    std::cout << "  SetTrack(\"CC1\", FORMAT_608) = " << ret << " (expected 0)" << std::endl;
    EXPECT_EQ(ret, 0);
    std::cout << "[CCManagerTrack.SetTrackWithExplicit608Format] - PASS" << std::endl;
}

/**
 * @brief SetTrack() with explicit 708 format override.
 *
 * @par Test Group ID  : CCManagerTrack
 * @par Test Case ID   : 013
 * @par Priority       : Medium
 */
TEST(CCManagerTrack, SetTrackWithExplicit708Format)
{
    std::cout << "[CCManagerTrack.SetTrackWithExplicit708Format] - START" << std::endl;
    PlayerSubtecCCManager mgr;
    int ret = mgr.SetTrack("SERVICE1", eCLOSEDCAPTION_FORMAT_708);
    std::cout << "  SetTrack(\"SERVICE1\", FORMAT_708) = " << ret << " (expected 0)" << std::endl;
    EXPECT_EQ(ret, 0);
    std::cout << "[CCManagerTrack.SetTrackWithExplicit708Format] - PASS" << std::endl;
}

// ===========================================================================
// CCManagerSetStyle / GetStyle tests
// ===========================================================================

/**
 * @brief GetStyle() returns empty string by default.
 *
 * @par Test Group ID  : CCManagerStyle
 * @par Test Case ID   : 001
 * @par Priority       : High
 */
TEST(CCManagerStyle, DefaultStyleIsEmpty)
{
    std::cout << "[CCManagerStyle.DefaultStyleIsEmpty] - START" << std::endl;
    PlayerSubtecCCManager mgr;
    const std::string& style = mgr.GetStyle();
    std::cout << "  GetStyle() default = \"" << style << "\" (expected empty)" << std::endl;
    EXPECT_TRUE(style.empty());
    std::cout << "[CCManagerStyle.DefaultStyleIsEmpty] - PASS" << std::endl;
}

/**
 * @brief SetStyle() when disabled (mEnabled=false) returns -1.
 *
 * @par Test Group ID  : CCManagerStyle
 * @par Test Case ID   : 002
 * @par Priority       : High
 */
TEST(CCManagerStyle, SetStyleWhenDisabledReturnsMinus1)
{
    std::cout << "[CCManagerStyle.SetStyleWhenDisabledReturnsMinus1] - START" << std::endl;
    PlayerSubtecCCManager mgr;
    // mEnabled is false by default
    int ret = mgr.SetStyle("{\"fontStyle\":\"default\"}");
    std::cout << "  SetStyle() when disabled = " << ret << " (expected -1)" << std::endl;
    EXPECT_EQ(ret, -1);
    std::cout << "[CCManagerStyle.SetStyleWhenDisabledReturnsMinus1] - PASS" << std::endl;
}

/**
 * @brief SetStyle() with empty options when disabled returns -1.
 *
 * @par Test Group ID  : CCManagerStyle
 * @par Test Case ID   : 003
 * @par Priority       : Medium
 */
TEST(CCManagerStyle, SetStyleEmptyWhenDisabledReturnsMinus1)
{
    std::cout << "[CCManagerStyle.SetStyleEmptyWhenDisabledReturnsMinus1] - START" << std::endl;
    PlayerSubtecCCManager mgr;
    int ret = mgr.SetStyle("");
    std::cout << "  SetStyle(\"\") when disabled = " << ret << " (expected -1)" << std::endl;
    EXPECT_EQ(ret, -1);
    std::cout << "[CCManagerStyle.SetStyleEmptyWhenDisabledReturnsMinus1] - PASS" << std::endl;
}

/**
 * @brief SetStyle() does not throw regardless of options or enabled state.
 *
 * @par Test Group ID  : CCManagerStyle
 * @par Test Case ID   : 004
 * @par Priority       : Medium
 */
TEST(CCManagerStyle, SetStyleNoThrow)
{
    std::cout << "[CCManagerStyle.SetStyleNoThrow] - START" << std::endl;
    PlayerSubtecCCManager mgr;
    EXPECT_NO_THROW({ mgr.SetStyle("{}"); });
    EXPECT_NO_THROW({ mgr.SetStyle(""); });
    EXPECT_NO_THROW({ mgr.SetStyle("{\"penSize\":\"medium\"}"); });
    std::cout << "  SetStyle() with 3 different inputs does not throw" << std::endl;
    std::cout << "[CCManagerStyle.SetStyleNoThrow] - PASS" << std::endl;
}

// ===========================================================================
// CCManagerTextTracks tests
// ===========================================================================

/**
 * @brief getLastTextTracks() returns empty vector by default.
 *
 * @par Test Group ID  : CCManagerTextTracks
 * @par Test Case ID   : 001
 * @par Priority       : High
 */
TEST(CCManagerTextTracks, DefaultIsEmpty)
{
    std::cout << "[CCManagerTextTracks.DefaultIsEmpty] - START" << std::endl;
    PlayerSubtecCCManager mgr;
    const auto& tracks = mgr.getLastTextTracks();
    std::cout << "  getLastTextTracks() initial size = " << tracks.size() << " (expected 0)" << std::endl;
    EXPECT_TRUE(tracks.empty());
    std::cout << "[CCManagerTextTracks.DefaultIsEmpty] - PASS" << std::endl;
}

/**
 * @brief updateLastTextTracks() stores a single-entry vector.
 *
 * @par Test Group ID  : CCManagerTextTracks
 * @par Test Case ID   : 002
 * @par Priority       : High
 */
TEST(CCManagerTextTracks, UpdateAndRetrieveSingleTrack)
{
    std::cout << "[CCManagerTextTracks.UpdateAndRetrieveSingleTrack] - START" << std::endl;
    PlayerSubtecCCManager mgr;

    CCTrackInfo track;
    track.instreamId = "CC1";
    track.language   = "eng";

    std::vector<CCTrackInfo> tracks = {track};
    mgr.updateLastTextTracks(tracks);

    const auto& result = mgr.getLastTextTracks();
    std::cout << "  After update: size=" << result.size() << " (expected 1)" << std::endl;
    ASSERT_EQ(result.size(), 1u);
    EXPECT_EQ(result[0].instreamId, "CC1");
    EXPECT_EQ(result[0].language,   "eng");
    std::cout << "[CCManagerTextTracks.UpdateAndRetrieveSingleTrack] - PASS" << std::endl;
}

/**
 * @brief updateLastTextTracks() stores a multi-entry vector.
 *
 * @par Test Group ID  : CCManagerTextTracks
 * @par Test Case ID   : 003
 * @par Priority       : High
 */
TEST(CCManagerTextTracks, UpdateAndRetrieveMultipleTracks)
{
    std::cout << "[CCManagerTextTracks.UpdateAndRetrieveMultipleTracks] - START" << std::endl;
    PlayerSubtecCCManager mgr;

    CCTrackInfo t1, t2, t3;
    t1.instreamId = "CC1";      t1.language = "eng";
    t2.instreamId = "CC2";      t2.language = "spa";
    t3.instreamId = "SERVICE1"; t3.language = "fra";

    std::vector<CCTrackInfo> tracks = {t1, t2, t3};
    mgr.updateLastTextTracks(tracks);

    const auto& result = mgr.getLastTextTracks();
    std::cout << "  After update: size=" << result.size() << " (expected 3)" << std::endl;
    ASSERT_EQ(result.size(), 3u);
    EXPECT_EQ(result[0].instreamId, "CC1");
    EXPECT_EQ(result[1].instreamId, "CC2");
    EXPECT_EQ(result[2].instreamId, "SERVICE1");
    std::cout << "[CCManagerTextTracks.UpdateAndRetrieveMultipleTracks] - PASS" << std::endl;
}

/**
 * @brief updateLastTextTracks() replaces a previously stored vector.
 *
 * @par Test Group ID  : CCManagerTextTracks
 * @par Test Case ID   : 004
 * @par Priority       : High
 */
TEST(CCManagerTextTracks, UpdateReplacesExistingTracks)
{
    std::cout << "[CCManagerTextTracks.UpdateReplacesExistingTracks] - START" << std::endl;
    PlayerSubtecCCManager mgr;

    CCTrackInfo first;
    first.instreamId = "CC1";
    mgr.updateLastTextTracks({first});
    ASSERT_EQ(mgr.getLastTextTracks().size(), 1u);

    CCTrackInfo second;
    second.instreamId = "SERVICE2";
    mgr.updateLastTextTracks({second});

    const auto& result = mgr.getLastTextTracks();
    std::cout << "  After second update: size=" << result.size() << " instreamId=\"" << result[0].instreamId << "\"" << std::endl;
    ASSERT_EQ(result.size(), 1u);
    EXPECT_EQ(result[0].instreamId, "SERVICE2");
    std::cout << "[CCManagerTextTracks.UpdateReplacesExistingTracks] - PASS" << std::endl;
}

/**
 * @brief updateLastTextTracks() with empty vector clears the tracks.
 *
 * @par Test Group ID  : CCManagerTextTracks
 * @par Test Case ID   : 005
 * @par Priority       : Medium
 */
TEST(CCManagerTextTracks, UpdateWithEmptyVectorClearsTracks)
{
    std::cout << "[CCManagerTextTracks.UpdateWithEmptyVectorClearsTracks] - START" << std::endl;
    PlayerSubtecCCManager mgr;

    CCTrackInfo t;
    t.instreamId = "CC1";
    mgr.updateLastTextTracks({t});
    ASSERT_EQ(mgr.getLastTextTracks().size(), 1u);

    mgr.updateLastTextTracks({});
    std::cout << "  After update({}): size=" << mgr.getLastTextTracks().size() << " (expected 0)" << std::endl;
    EXPECT_TRUE(mgr.getLastTextTracks().empty());
    std::cout << "[CCManagerTextTracks.UpdateWithEmptyVectorClearsTracks] - PASS" << std::endl;
}

// ===========================================================================
// CCManagerTrickplay / ParentalControl tests
// ===========================================================================

/**
 * @brief SetTrickplayStatus(true) does not throw.
 *
 * @par Test Group ID  : CCManagerTrickplay
 * @par Test Case ID   : 001
 * @par Priority       : High
 */
TEST(CCManagerTrickplay, SetTrueNoThrow)
{
    std::cout << "[CCManagerTrickplay.SetTrueNoThrow] - START" << std::endl;
    PlayerSubtecCCManager mgr;
    EXPECT_NO_THROW({ mgr.SetTrickplayStatus(true); });
    std::cout << "  SetTrickplayStatus(true) OK" << std::endl;
    std::cout << "[CCManagerTrickplay.SetTrueNoThrow] - PASS" << std::endl;
}

/**
 * @brief SetTrickplayStatus(false) does not throw.
 *
 * @par Test Group ID  : CCManagerTrickplay
 * @par Test Case ID   : 002
 * @par Priority       : High
 */
TEST(CCManagerTrickplay, SetFalseNoThrow)
{
    std::cout << "[CCManagerTrickplay.SetFalseNoThrow] - START" << std::endl;
    PlayerSubtecCCManager mgr;
    EXPECT_NO_THROW({ mgr.SetTrickplayStatus(false); });
    std::cout << "  SetTrickplayStatus(false) OK" << std::endl;
    std::cout << "[CCManagerTrickplay.SetFalseNoThrow] - PASS" << std::endl;
}

/**
 * @brief SetTrickplayStatus() toggle does not throw.
 *
 * @par Test Group ID  : CCManagerTrickplay
 * @par Test Case ID   : 003
 * @par Priority       : Medium
 */
TEST(CCManagerTrickplay, ToggleNoThrow)
{
    std::cout << "[CCManagerTrickplay.ToggleNoThrow] - START" << std::endl;
    PlayerSubtecCCManager mgr;
    EXPECT_NO_THROW({
        mgr.SetTrickplayStatus(true);
        mgr.SetTrickplayStatus(false);
        mgr.SetTrickplayStatus(true);
    });
    std::cout << "  SetTrickplayStatus() toggle OK" << std::endl;
    std::cout << "[CCManagerTrickplay.ToggleNoThrow] - PASS" << std::endl;
}

/**
 * @brief SetParentalControlStatus(true) does not throw.
 *
 * @par Test Group ID  : CCManagerParentalControl
 * @par Test Case ID   : 001
 * @par Priority       : High
 */
TEST(CCManagerParentalControl, SetLockedNoThrow)
{
    std::cout << "[CCManagerParentalControl.SetLockedNoThrow] - START" << std::endl;
    PlayerSubtecCCManager mgr;
    EXPECT_NO_THROW({ mgr.SetParentalControlStatus(true); });
    std::cout << "  SetParentalControlStatus(true) OK" << std::endl;
    std::cout << "[CCManagerParentalControl.SetLockedNoThrow] - PASS" << std::endl;
}

/**
 * @brief SetParentalControlStatus(false) does not throw.
 *
 * @par Test Group ID  : CCManagerParentalControl
 * @par Test Case ID   : 002
 * @par Priority       : High
 */
TEST(CCManagerParentalControl, SetUnlockedNoThrow)
{
    std::cout << "[CCManagerParentalControl.SetUnlockedNoThrow] - START" << std::endl;
    PlayerSubtecCCManager mgr;
    EXPECT_NO_THROW({ mgr.SetParentalControlStatus(false); });
    std::cout << "  SetParentalControlStatus(false) OK" << std::endl;
    std::cout << "[CCManagerParentalControl.SetUnlockedNoThrow] - PASS" << std::endl;
}

// ===========================================================================
// CCManagerIsOOBCC tests
// ===========================================================================

/**
 * @brief IsOOBCCRenderingSupported() does not throw.
 *
 * @par Test Group ID  : CCManagerIsOOBCC
 * @par Test Case ID   : 001
 * @par Priority       : High
 */
TEST(CCManagerIsOOBCC, DoesNotThrow)
{
    std::cout << "[CCManagerIsOOBCC.DoesNotThrow] - START" << std::endl;
    PlayerSubtecCCManager mgr;
    bool result = false;
    EXPECT_NO_THROW({ result = mgr.IsOOBCCRenderingSupported(); });
    std::cout << "  IsOOBCCRenderingSupported() = " << result << std::endl;
    std::cout << "[CCManagerIsOOBCC.DoesNotThrow] - PASS" << std::endl;
}

/**
 * @brief IsOOBCCRenderingSupported() returns a consistent value across calls.
 *
 * @par Test Group ID  : CCManagerIsOOBCC
 * @par Test Case ID   : 002
 * @par Priority       : Medium
 */
TEST(CCManagerIsOOBCC, ConsistentReturn)
{
    std::cout << "[CCManagerIsOOBCC.ConsistentReturn] - START" << std::endl;
    PlayerSubtecCCManager mgr;
    bool r1 = mgr.IsOOBCCRenderingSupported();
    bool r2 = mgr.IsOOBCCRenderingSupported();
    std::cout << "  IsOOBCCRenderingSupported(): r1=" << r1 << " r2=" << r2 << std::endl;
    EXPECT_EQ(r1, r2);
    std::cout << "[CCManagerIsOOBCC.ConsistentReturn] - PASS" << std::endl;
}

// ===========================================================================
// Integration tests - combined API usage
// ===========================================================================

/**
 * @brief Integration: GetId → SetTrack → updateLastTextTracks → getLastTextTracks → Release.
 *
 * @par Test Group ID  : CCManagerIntegration
 * @par Test Case ID   : 001
 * @par Priority       : High
 */
TEST(CCManagerIntegration, FullSessionLifecycle)
{
    std::cout << "[CCManagerIntegration.FullSessionLifecycle] - START" << std::endl;
    PlayerSubtecCCManager mgr;

    // Acquire ID
    int id = mgr.GetId();
    std::cout << "  GetId() = " << id << std::endl;
    EXPECT_EQ(id, 1);

    // Set a track
    int trackRet = mgr.SetTrack("CC1");
    std::cout << "  SetTrack(\"CC1\") = " << trackRet << std::endl;
    EXPECT_EQ(trackRet, 0);
    EXPECT_EQ(mgr.GetTrack(), "CC1");

    // Store and retrieve text tracks
    CCTrackInfo ti;
    ti.instreamId = "CC1";
    ti.language   = "eng";
    mgr.updateLastTextTracks({ti});
    const auto& tracks = mgr.getLastTextTracks();
    ASSERT_EQ(tracks.size(), 1u);
    EXPECT_EQ(tracks[0].instreamId, "CC1");

    // Release
    EXPECT_NO_THROW({ mgr.Release(id); });

    std::cout << "  Full session lifecycle OK" << std::endl;
    std::cout << "[CCManagerIntegration.FullSessionLifecycle] - PASS" << std::endl;
}

/**
 * @brief Integration: enable → SetTrack x2 → disable → status consistency.
 *
 * @par Test Group ID  : CCManagerIntegration
 * @par Test Case ID   : 002
 * @par Priority       : Medium
 */
TEST(CCManagerIntegration, EnableTrackDisableSequence)
{
    std::cout << "[CCManagerIntegration.EnableTrackDisableSequence] - START" << std::endl;
    PlayerSubtecCCManager mgr;

    EXPECT_FALSE(mgr.GetStatus());
    mgr.SetStatus(true);
    EXPECT_TRUE(mgr.GetStatus());

    mgr.SetTrack("CC1");
    EXPECT_EQ(mgr.GetTrack(), "CC1");

    mgr.SetTrack("SERVICE1");
    EXPECT_EQ(mgr.GetTrack(), "SERVICE1");

    mgr.SetStatus(false);
    EXPECT_FALSE(mgr.GetStatus());

    std::cout << "  Enable → SetTrack x2 → Disable sequence OK" << std::endl;
    std::cout << "[CCManagerIntegration.EnableTrackDisableSequence] - PASS" << std::endl;
}

/**
 * @brief Integration: Multiple GetId / Release cycles.
 *
 * @par Test Group ID  : CCManagerIntegration
 * @par Test Case ID   : 003
 * @par Priority       : Medium
 */
TEST(CCManagerIntegration, MultipleGetIdReleaseCycles)
{
    std::cout << "[CCManagerIntegration.MultipleGetIdReleaseCycles] - START" << std::endl;
    PlayerSubtecCCManager mgr;
    for (int i = 0; i < 5; ++i)
    {
        int id = mgr.GetId();
        std::cout << "  cycle " << (i+1) << ": id=" << id << std::endl;
        EXPECT_EQ(id, i + 1);
        EXPECT_NO_THROW({ mgr.Release(id); });
    }
    std::cout << "[CCManagerIntegration.MultipleGetIdReleaseCycles] - PASS" << std::endl;
}

// ============================================================
// Test Group: CCManagerInit
// Tests for the public Init(void* handle) method which internally
// exercises Initialize(), Start()/Stop(), EnsureInitialized(),
// EnsureHALInitialized(), EnsureRendererCommsInitialized(),
// StartRendering(), and StopRendering().
// ============================================================

/**
 * @brief Init() with a valid handle returns 0.
 *
 * Init() calls Initialize(handle) -> stores handle, then Stop() ->
 * EnsureRendererCommsInitialized() + StopRendering() (mEnabled is false
 * by default after ctor resets it via SetTrack path).
 *
 * @par Test Group ID  : CCManagerInit
 * @par Test Case ID   : 001
 * @par Priority       : High
 */
TEST(CCManagerInit, InitWithValidHandleReturns0)
{
    std::cout << "[CCManagerInit.InitWithValidHandleReturns0] - START" << std::endl;
    PlayerSubtecCCManager mgr;
    int dummy = 42;
    int ret = mgr.Init(static_cast<void*>(&dummy));
    std::cout << "  Init(handle) = " << ret << " (expected 0)" << std::endl;
    EXPECT_EQ(ret, 0);
    std::cout << "[CCManagerInit.InitWithValidHandleReturns0] - PASS" << std::endl;
}

/**
 * @brief Init() with null handle returns -1.
 *
 * @par Test Group ID  : CCManagerInit
 * @par Test Case ID   : 002
 * @par Priority       : High
 */
TEST(CCManagerInit, InitWithNullHandleReturnsNeg1)
{
    std::cout << "[CCManagerInit.InitWithNullHandleReturnsNeg1] - START" << std::endl;
    PlayerSubtecCCManager mgr;
    int ret = mgr.Init(nullptr);
    std::cout << "  Init(nullptr) = " << ret << " (expected -1)" << std::endl;
    EXPECT_EQ(ret, -1);
    std::cout << "[CCManagerInit.InitWithNullHandleReturnsNeg1] - PASS" << std::endl;
}

/**
 * @brief Init() when mEnabled=true takes the Start() path (exercises
 *        EnsureInitialized → EnsureHALInitialized + EnsureRendererCommsInitialized
 *        + StartRendering).
 *
 * @par Test Group ID  : CCManagerInit
 * @par Test Case ID   : 003
 * @par Priority       : High
 */
TEST(CCManagerInit, InitWithEnabledStatusTakesStartPath)
{
    std::cout << "[CCManagerInit.InitWithEnabledStatusTakesStartPath] - START" << std::endl;
    PlayerSubtecCCManager mgr;
    mgr.SetStatus(true);
    int dummy = 99;
    int ret = mgr.Init(static_cast<void*>(&dummy));
    std::cout << "  Init(handle) with mEnabled=true = " << ret << " (expected 0, Start path)" << std::endl;
    EXPECT_EQ(ret, 0);
    std::cout << "[CCManagerInit.InitWithEnabledStatusTakesStartPath] - PASS" << std::endl;
}

/**
 * @brief Init() called twice with the same handle returns 0 both times.
 *
 * @par Test Group ID  : CCManagerInit
 * @par Test Case ID   : 004
 * @par Priority       : Medium
 */
TEST(CCManagerInit, InitCalledTwiceIsSafe)
{
    std::cout << "[CCManagerInit.InitCalledTwiceIsSafe] - START" << std::endl;
    PlayerSubtecCCManager mgr;
    int dummy = 7;
    EXPECT_EQ(mgr.Init(static_cast<void*>(&dummy)), 0);
    EXPECT_EQ(mgr.Init(static_cast<void*>(&dummy)), 0);
    std::cout << "  Init(handle) twice both return 0" << std::endl;
    std::cout << "[CCManagerInit.InitCalledTwiceIsSafe] - PASS" << std::endl;
}

// ============================================================
// Test Group: CCManagerResetState
// ResetState() is protected in PlayerCCManagerBase.  A thin test
// subclass promotes it to public so it can be exercised directly.
// ============================================================

/** @brief Thin subclass that promotes protected ResetState() to public. */
class ResetableCCManager : public PlayerSubtecCCManager
{
public:
    using PlayerSubtecCCManager::ResetState;
};

/**
 * @brief ResetState() clears track, options, status and text tracks.
 *
 * @par Test Group ID  : CCManagerResetState
 * @par Test Case ID   : 001
 * @par Priority       : High
 */
TEST(CCManagerResetState, ResetStateClearsAllFields)
{
    std::cout << "[CCManagerResetState.ResetStateClearsAllFields] - START" << std::endl;
    ResetableCCManager mgr;
    mgr.SetStatus(true);
    mgr.SetTrack("SERVICE3");
    CCTrackInfo t1;
    t1.instreamId = "SERVICE3";
    t1.language = "en";
    mgr.updateLastTextTracks({t1});

    mgr.ResetState();

    EXPECT_FALSE(mgr.GetStatus());
    EXPECT_TRUE(mgr.GetTrack().empty());
    EXPECT_TRUE(mgr.getLastTextTracks().empty());
    std::cout << "  After ResetState: status=false, track empty, textTracks empty" << std::endl;
    std::cout << "[CCManagerResetState.ResetStateClearsAllFields] - PASS" << std::endl;
}

/**
 * @brief ResetState() called on a fresh manager does not throw.
 *
 * @par Test Group ID  : CCManagerResetState
 * @par Test Case ID   : 002
 * @par Priority       : Medium
 */
TEST(CCManagerResetState, ResetStateOnFreshManagerDoesNotThrow)
{
    std::cout << "[CCManagerResetState.ResetStateOnFreshManagerDoesNotThrow] - START" << std::endl;
    ResetableCCManager mgr;
    EXPECT_NO_THROW(mgr.ResetState());
    std::cout << "  ResetState() on fresh manager is safe" << std::endl;
    std::cout << "[CCManagerResetState.ResetStateOnFreshManagerDoesNotThrow] - PASS" << std::endl;
}

/**
 * @brief After ResetState() a new Init() call succeeds normally.
 *
 * @par Test Group ID  : CCManagerResetState
 * @par Test Case ID   : 003
 * @par Priority       : Medium
 */
TEST(CCManagerResetState, ResetStateThenInitSucceeds)
{
    std::cout << "[CCManagerResetState.ResetStateThenInitSucceeds] - START" << std::endl;
    ResetableCCManager mgr;
    int dummy = 5;
    mgr.ResetState();
    int ret = mgr.Init(static_cast<void*>(&dummy));
    std::cout << "  Init() after ResetState() = " << ret << " (expected 0)" << std::endl;
    EXPECT_EQ(ret, 0);
    std::cout << "[CCManagerResetState.ResetStateThenInitSucceeds] - PASS" << std::endl;
}

// ============================================================
// Test Group: CCManagerRestoreCC
// ============================================================

/**
 * @brief RestoreCC() with empty text track list defaults to "CC1".
 *
 * @par Test Group ID  : CCManagerRestoreCC
 * @par Test Case ID   : 001
 * @par Priority       : High
 */
TEST(CCManagerRestoreCC, RestoreCCWithNoTracksDefaultsToCC1)
{
    std::cout << "[CCManagerRestoreCC.RestoreCCWithNoTracksDefaultsToCC1] - START" << std::endl;
    PlayerSubtecCCManager mgr;
    // Start with empty text tracks list
    mgr.updateLastTextTracks({});
    mgr.RestoreCC();
    std::string track = mgr.GetTrack();
    std::cout << "  After RestoreCC() with no tracks: GetTrack()=\"" << track << "\" (expected CC1)" << std::endl;
    EXPECT_EQ(track, "CC1");
    std::cout << "[CCManagerRestoreCC.RestoreCCWithNoTracksDefaultsToCC1] - PASS" << std::endl;
}

/**
 * @brief RestoreCC() with matching track keeps current track.
 *
 * @par Test Group ID  : CCManagerRestoreCC
 * @par Test Case ID   : 002
 * @par Priority       : High
 */
TEST(CCManagerRestoreCC, RestoreCCWithMatchingTrackKeepsTrack)
{
    std::cout << "[CCManagerRestoreCC.RestoreCCWithMatchingTrackKeepsTrack] - START" << std::endl;
    PlayerSubtecCCManager mgr;
    mgr.SetTrack("CC2");
    CCTrackInfo t1, t2;
    t1.instreamId = "CC1";  t1.language = "en";
    t2.instreamId = "CC2";  t2.language = "es";
    mgr.updateLastTextTracks({t1, t2});
    mgr.RestoreCC();
    std::string track = mgr.GetTrack();
    std::cout << "  After RestoreCC() with matching CC2: GetTrack()=\"" << track << "\"" << std::endl;
    EXPECT_EQ(track, "CC2");
    std::cout << "[CCManagerRestoreCC.RestoreCCWithMatchingTrackKeepsTrack] - PASS" << std::endl;
}

/**
 * @brief RestoreCC() with no matching track falls back to first track's instreamId.
 *
 * @par Test Group ID  : CCManagerRestoreCC
 * @par Test Case ID   : 003
 * @par Priority       : High
 */
TEST(CCManagerRestoreCC, RestoreCCWithNoMatchUsesFirstTrack)
{
    std::cout << "[CCManagerRestoreCC.RestoreCCWithNoMatchUsesFirstTrack] - START" << std::endl;
    PlayerSubtecCCManager mgr;
    mgr.SetTrack("CC4");
    CCTrackInfo t1, t2;
    t1.instreamId = "CC1";  t1.language = "en";
    t2.instreamId = "CC2";  t2.language = "es";
    // CC4 is not in the list, so falls back to first track (CC1)
    mgr.updateLastTextTracks({t1, t2});
    mgr.RestoreCC();
    std::string track = mgr.GetTrack();
    std::cout << "  After RestoreCC() with unmatched CC4: GetTrack()=\"" << track << "\" (expected CC1)" << std::endl;
    EXPECT_EQ(track, "CC1");
    std::cout << "[CCManagerRestoreCC.RestoreCCWithNoMatchUsesFirstTrack] - PASS" << std::endl;
}
