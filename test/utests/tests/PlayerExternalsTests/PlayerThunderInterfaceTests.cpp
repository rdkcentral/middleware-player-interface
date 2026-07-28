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
 * @file PlayerThunderInterfaceTests.cpp
 * @brief Comprehensive unit tests for PlayerThunderInterface.
 *
 * In test builds USE_CPP_THUNDER_PLUGIN_ACCESS is NOT defined, so
 * PlayerThunderInterface uses FakeThunderAccess internally.  All fakes
 * return false/""/empty — the expected observable behaviour in a unit-test
 * environment.  No GTEST_SKIP calls.
 *
 * APIs covered:
 *   Constructor (all 10 enum values), ~PlayerThunderInterface,
 *   ActivatePlugin, UnSubscribeEvent, SetVideoRectangle,
 *   SetPreferredAudioLanguages,
 *   RegisterAllEventsVideoin, UnRegisterAllEventsVideoin,
 *   StartHelperVideoin, StopHelperVideoin,
 *   RegisterEventOnVideoStreamInfoUpdateHdmiin,
 *   RegisterOnPlayerStatusOta, ReleaseOta, StartOta, StopOta,
 *   GetAudioTracksOta, SetAudioTrackOta, GetTextTracksOta,
 *   DisableContentRestrictionsOta, EnableContentRestrictionsOta,
 *   InitRmf, StartRmf, StopRmf,
 *   DeleteWatermark, CreateWatermark, ShowWatermark, HideWatermark,
 *   UpdateWatermark, GetMetaDataWatermark,
 *   PersistentStoreSaveWatermark, PersistentStoreLoadWatermark,
 *   IsThunderAccess.
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <iostream>
#include <string>
#include <vector>
#include <climits>
#include "PlayerThunderInterface.h"

// ===========================================================================
// Helper: all valid plugin enum values
// ===========================================================================

static const PlayerThunderAccessPlugin kAllPlugins[] = {
    AVINPUT, MEDIAPLAYER, MEDIASETTINGS, RDKSHELL, RMF,
    DS, SECMANAGER, WATERMARK, HDMIINPUT, COMPOSITEINPUT
};
static const int kPluginCount = static_cast<int>(sizeof(kAllPlugins)/sizeof(kAllPlugins[0]));

// ===========================================================================
// Fixture
// ===========================================================================

class ThunderInterfaceTest : public ::testing::Test
{
protected:
    std::unique_ptr<PlayerThunderInterface> iface;
    void SetUp() override
    {
        iface = std::make_unique<PlayerThunderInterface>(WATERMARK);
    }
    void TearDown() override { iface.reset(); }
};

// ===========================================================================
// Group: Construction & Destruction
// ===========================================================================

/**
 * @brief Constructor does not throw for every valid plugin enum value.
 *
 * **Test Group ID:** ThunderConstruction_001@n
 * **Priority:** High@n
 */
TEST(ThunderConstruction, ConstructWithEveryPluginEnum)
{
    std::cout << "[ThunderConstruction.ConstructWithEveryPluginEnum] - START" << std::endl;
    for (auto plug : kAllPlugins)
    {
        EXPECT_NO_THROW({ PlayerThunderInterface pi(plug); });
        std::cout << "  Constructed enum=" << static_cast<int>(plug) << " OK" << std::endl;
    }
    std::cout << "[ThunderConstruction.ConstructWithEveryPluginEnum] - PASS" << std::endl;
}

/**
 * @brief Destructor does not throw.
 *
 * **Test Group ID:** ThunderConstruction_002@n
 * **Priority:** High@n
 */
TEST(ThunderConstruction, DestructorDoesNotThrow)
{
    std::cout << "[ThunderConstruction.DestructorDoesNotThrow] - START" << std::endl;
    EXPECT_NO_THROW({ PlayerThunderInterface pi(AVINPUT); });
    std::cout << "  AVINPUT instance constructed and destroyed safely" << std::endl;
    std::cout << "[ThunderConstruction.DestructorDoesNotThrow] - PASS" << std::endl;
}

/**
 * @brief 10 instances (one per enum) constructed and destroyed without leaks.
 *
 * **Test Group ID:** ThunderConstruction_003@n
 * **Priority:** Low@n
 */
TEST(ThunderConstruction, TenInstancesStress)
{
    std::cout << "[ThunderConstruction.TenInstancesStress] - START" << std::endl;
    std::vector<std::unique_ptr<PlayerThunderInterface>> v;
    for (auto plug : kAllPlugins)
        v.emplace_back(std::make_unique<PlayerThunderInterface>(plug));
    EXPECT_EQ(static_cast<int>(v.size()), kPluginCount);
    v.clear();
    std::cout << "  Created and destroyed " << kPluginCount << " instances" << std::endl;
    std::cout << "[ThunderConstruction.TenInstancesStress] - PASS" << std::endl;
}

// ===========================================================================
// Group: ActivatePlugin
// ===========================================================================

/**
 * @brief ActivatePlugin() returns false (FakeThunderAccess).
 *
 * **Test Group ID:** ThunderActivatePlugin_001@n
 */
TEST_F(ThunderInterfaceTest, ActivatePluginReturnsFalse)
{
    std::cout << "[ThunderActivatePlugin.ActivatePluginReturnsFalse] - START" << std::endl;
    EXPECT_FALSE(iface->ActivatePlugin());
    std::cout << "  ActivatePlugin() = false" << std::endl;
    std::cout << "[ThunderActivatePlugin.ActivatePluginReturnsFalse] - PASS" << std::endl;
}

/**
 * @brief ActivatePlugin() returns false for all valid plugin enums.
 *
 * **Test Group ID:** ThunderActivatePlugin_002@n
 */
TEST(ThunderActivatePlugin, AllEnumsReturnFalse)
{
    std::cout << "[ThunderActivatePlugin.AllEnumsReturnFalse] - START" << std::endl;
    for (auto plug : kAllPlugins)
    {
        PlayerThunderInterface pi(plug);
        EXPECT_FALSE(pi.ActivatePlugin());
    }
    std::cout << "  ActivatePlugin() = false for all 10 enums" << std::endl;
    std::cout << "[ThunderActivatePlugin.AllEnumsReturnFalse] - PASS" << std::endl;
}

/**
 * @brief ActivatePlugin() is stable across 5 repeated calls.
 *
 * **Test Group ID:** ThunderActivatePlugin_003@n
 */
TEST_F(ThunderInterfaceTest, ActivatePluginRepeatedCallsStable)
{
    std::cout << "[ThunderActivatePlugin.ActivatePluginRepeatedCallsStable] - START" << std::endl;
    for (int i = 0; i < 5; ++i) EXPECT_FALSE(iface->ActivatePlugin());
    std::cout << "  5 repeated ActivatePlugin() calls all returned false" << std::endl;
    std::cout << "[ThunderActivatePlugin.ActivatePluginRepeatedCallsStable] - PASS" << std::endl;
}

// ===========================================================================
// Group: UnSubscribeEvent
// ===========================================================================

/**
 * @brief UnSubscribeEvent() with a valid event name returns false.
 *
 * **Test Group ID:** ThunderUnSubscribeEvent_001@n
 */
TEST_F(ThunderInterfaceTest, UnSubscribeEventReturnsFalse)
{
    std::cout << "[ThunderUnSubscribeEvent.UnSubscribeEventReturnsFalse] - START" << std::endl;
    EXPECT_FALSE(iface->UnSubscribeEvent("onHdmiInputActive"));
    std::cout << "  UnSubscribeEvent(\"onHdmiInputActive\") = false" << std::endl;
    std::cout << "[ThunderUnSubscribeEvent.UnSubscribeEventReturnsFalse] - PASS" << std::endl;
}

/**
 * @brief UnSubscribeEvent() with empty string returns false.
 *
 * **Test Group ID:** ThunderUnSubscribeEvent_002@n
 */
TEST_F(ThunderInterfaceTest, UnSubscribeEventEmptyReturnsFalse)
{
    std::cout << "[ThunderUnSubscribeEvent.UnSubscribeEventEmptyReturnsFalse] - START" << std::endl;
    EXPECT_FALSE(iface->UnSubscribeEvent(""));
    std::cout << "  UnSubscribeEvent(\"\") = false" << std::endl;
    std::cout << "[ThunderUnSubscribeEvent.UnSubscribeEventEmptyReturnsFalse] - PASS" << std::endl;
}

// ===========================================================================
// Group: SetVideoRectangle
// ===========================================================================

/**
 * @brief SetVideoRectangle() with positive dimensions returns false.
 *
 * **Test Group ID:** ThunderSetVideoRectangle_001@n
 */
TEST_F(ThunderInterfaceTest, SetVideoRectangleReturnsFalse)
{
    std::cout << "[ThunderSetVideoRectangle.SetVideoRectangleReturnsFalse] - START" << std::endl;
    EXPECT_FALSE(iface->SetVideoRectangle(0, 0, 1920, 1080, "HDMI", VIDEOIN_SHIM));
    std::cout << "  SetVideoRectangle(0,0,1920,1080,HDMI,VIDEOIN_SHIM) = false" << std::endl;
    std::cout << "[ThunderSetVideoRectangle.SetVideoRectangleReturnsFalse] - PASS" << std::endl;
}

/**
 * @brief SetVideoRectangle() with zero dimensions returns false.
 *
 * **Test Group ID:** ThunderSetVideoRectangle_002@n
 */
TEST_F(ThunderInterfaceTest, SetVideoRectangleZeroDimensionsReturnsFalse)
{
    std::cout << "[ThunderSetVideoRectangle.SetVideoRectangleZeroDimensionsReturnsFalse] - START" << std::endl;
    EXPECT_FALSE(iface->SetVideoRectangle(0, 0, 0, 0, "", OTA_SHIM));
    std::cout << "  SetVideoRectangle(0,0,0,0) = false" << std::endl;
    std::cout << "[ThunderSetVideoRectangle.SetVideoRectangleZeroDimensionsReturnsFalse] - PASS" << std::endl;
}

/**
 * @brief SetVideoRectangle() does not throw for all shim types.
 *
 * **Test Group ID:** ThunderSetVideoRectangle_003@n
 */
TEST_F(ThunderInterfaceTest, SetVideoRectangleAllShimsDoNotThrow)
{
    std::cout << "[ThunderSetVideoRectangle.SetVideoRectangleAllShimsDoNotThrow] - START" << std::endl;
    for (auto shim : {VIDEOIN_SHIM, OTA_SHIM, RMF_SHIM})
        EXPECT_NO_THROW(iface->SetVideoRectangle(10, 20, 640, 480, "type", shim));
    std::cout << "  All 3 shim types completed without exception" << std::endl;
    std::cout << "[ThunderSetVideoRectangle.SetVideoRectangleAllShimsDoNotThrow] - PASS" << std::endl;
}

// ===========================================================================
// Group: SetPreferredAudioLanguages
// ===========================================================================

/**
 * @brief SetPreferredAudioLanguages() with non-empty data does not throw.
 *
 * **Test Group ID:** ThunderAudio_001@n
 */
TEST_F(ThunderInterfaceTest, SetPreferredAudioLanguagesDoesNotThrow)
{
    std::cout << "[ThunderAudio.SetPreferredAudioLanguagesDoesNotThrow] - START" << std::endl;
    PlayerPreferredAudioData data;
    data.preferredLanguagesString = "en";
    data.pluginPreferredLanguagesString = "en,fr";
    data.preferredRenditionString = "";
    data.pluginPreferredRenditionString = "";
    EXPECT_NO_THROW(iface->SetPreferredAudioLanguages(data, OTA_SHIM));
    std::cout << "  SetPreferredAudioLanguages(en) completed without exception" << std::endl;
    std::cout << "[ThunderAudio.SetPreferredAudioLanguagesDoesNotThrow] - PASS" << std::endl;
}

/**
 * @brief SetPreferredAudioLanguages() with empty data does not throw.
 *
 * **Test Group ID:** ThunderAudio_002@n
 */
TEST_F(ThunderInterfaceTest, SetPreferredAudioLanguagesEmptyDoesNotThrow)
{
    std::cout << "[ThunderAudio.SetPreferredAudioLanguagesEmptyDoesNotThrow] - START" << std::endl;
    PlayerPreferredAudioData data{};
    EXPECT_NO_THROW(iface->SetPreferredAudioLanguages(data, VIDEOIN_SHIM));
    std::cout << "  SetPreferredAudioLanguages(empty) completed without exception" << std::endl;
    std::cout << "[ThunderAudio.SetPreferredAudioLanguagesEmptyDoesNotThrow] - PASS" << std::endl;
}

// ===========================================================================
// Group: VideoIn APIs
// ===========================================================================

/**
 * @brief RegisterAllEventsVideoin() with real callbacks does not throw.
 *
 * **Test Group ID:** ThunderVideoIn_001@n
 */
TEST_F(ThunderInterfaceTest, RegisterAllEventsVideoinDoesNotThrow)
{
    std::cout << "[ThunderVideoIn.RegisterAllEventsVideoinDoesNotThrow] - START" << std::endl;
    auto sig = [](std::string s){ (void)s; };
    auto sta = [](std::string s){ (void)s; };
    EXPECT_NO_THROW(iface->RegisterAllEventsVideoin(sig, sta));
    std::cout << "  RegisterAllEventsVideoin() completed without exception" << std::endl;
    std::cout << "[ThunderVideoIn.RegisterAllEventsVideoinDoesNotThrow] - PASS" << std::endl;
}

/**
 * @brief UnRegisterAllEventsVideoin() does not throw.
 *
 * **Test Group ID:** ThunderVideoIn_002@n
 */
TEST_F(ThunderInterfaceTest, UnRegisterAllEventsVideoinDoesNotThrow)
{
    std::cout << "[ThunderVideoIn.UnRegisterAllEventsVideoinDoesNotThrow] - START" << std::endl;
    EXPECT_NO_THROW(iface->UnRegisterAllEventsVideoin());
    std::cout << "  UnRegisterAllEventsVideoin() completed without exception" << std::endl;
    std::cout << "[ThunderVideoIn.UnRegisterAllEventsVideoinDoesNotThrow] - PASS" << std::endl;
}

/**
 * @brief StartHelperVideoin() with positive port does not throw.
 *
 * **Test Group ID:** ThunderVideoIn_003@n
 */
TEST_F(ThunderInterfaceTest, StartHelperVideoinDoesNotThrow)
{
    std::cout << "[ThunderVideoIn.StartHelperVideoinDoesNotThrow] - START" << std::endl;
    EXPECT_NO_THROW(iface->StartHelperVideoin(1234, "HDMI"));
    std::cout << "  StartHelperVideoin(1234, HDMI) completed without exception" << std::endl;
    std::cout << "[ThunderVideoIn.StartHelperVideoinDoesNotThrow] - PASS" << std::endl;
}

/**
 * @brief StartHelperVideoin() with port=0 and empty type does not throw.
 *
 * **Test Group ID:** ThunderVideoIn_004@n
 */
TEST_F(ThunderInterfaceTest, StartHelperVideoinZeroPortDoesNotThrow)
{
    std::cout << "[ThunderVideoIn.StartHelperVideoinZeroPortDoesNotThrow] - START" << std::endl;
    EXPECT_NO_THROW(iface->StartHelperVideoin(0, ""));
    std::cout << "  StartHelperVideoin(0, \"\") completed without exception" << std::endl;
    std::cout << "[ThunderVideoIn.StartHelperVideoinZeroPortDoesNotThrow] - PASS" << std::endl;
}

/**
 * @brief StopHelperVideoin() does not throw.
 *
 * **Test Group ID:** ThunderVideoIn_005@n
 */
TEST_F(ThunderInterfaceTest, StopHelperVideoinDoesNotThrow)
{
    std::cout << "[ThunderVideoIn.StopHelperVideoinDoesNotThrow] - START" << std::endl;
    EXPECT_NO_THROW(iface->StopHelperVideoin("HDMI"));
    EXPECT_NO_THROW(iface->StopHelperVideoin(""));
    std::cout << "  StopHelperVideoin() (HDMI and empty) completed without exception" << std::endl;
    std::cout << "[ThunderVideoIn.StopHelperVideoinDoesNotThrow] - PASS" << std::endl;
}

// ===========================================================================
// Group: HDMIIN
// ===========================================================================

/**
 * @brief RegisterEventOnVideoStreamInfoUpdateHdmiin() does not throw.
 *
 * **Test Group ID:** ThunderHdmiIn_001@n
 */
TEST_F(ThunderInterfaceTest, RegisterEventOnVideoStreamInfoUpdateHdmiinDoesNotThrow)
{
    std::cout << "[ThunderHdmiIn.RegisterEventOnVideoStreamInfoUpdateHdmiinDoesNotThrow] - START" << std::endl;
    auto cb = [](PlayerVideoStreamInfoData d){ (void)d; };
    EXPECT_NO_THROW(iface->RegisterEventOnVideoStreamInfoUpdateHdmiin(cb));
    std::cout << "  RegisterEventOnVideoStreamInfoUpdateHdmiin() completed without exception" << std::endl;
    std::cout << "[ThunderHdmiIn.RegisterEventOnVideoStreamInfoUpdateHdmiinDoesNotThrow] - PASS" << std::endl;
}

// ===========================================================================
// Group: OTA APIs
// ===========================================================================

/**
 * @brief RegisterOnPlayerStatusOta() with real callback does not throw.
 *
 * **Test Group ID:** ThunderOTA_001@n
 */
TEST_F(ThunderInterfaceTest, RegisterOnPlayerStatusOtaDoesNotThrow)
{
    std::cout << "[ThunderOTA.RegisterOnPlayerStatusOtaDoesNotThrow] - START" << std::endl;
    auto cb = [](PlayerStatusData d){ (void)d; };
    EXPECT_NO_THROW(iface->RegisterOnPlayerStatusOta(cb));
    std::cout << "  RegisterOnPlayerStatusOta() completed without exception" << std::endl;
    std::cout << "[ThunderOTA.RegisterOnPlayerStatusOtaDoesNotThrow] - PASS" << std::endl;
}

/**
 * @brief ReleaseOta() does not throw.
 *
 * **Test Group ID:** ThunderOTA_002@n
 */
TEST_F(ThunderInterfaceTest, ReleaseOtaDoesNotThrow)
{
    std::cout << "[ThunderOTA.ReleaseOtaDoesNotThrow] - START" << std::endl;
    EXPECT_NO_THROW(iface->ReleaseOta());
    std::cout << "  ReleaseOta() completed without exception" << std::endl;
    std::cout << "[ThunderOTA.ReleaseOtaDoesNotThrow] - PASS" << std::endl;
}

/**
 * @brief StartOta() with valid strings does not throw.
 *
 * **Test Group ID:** ThunderOTA_003@n
 */
TEST_F(ThunderInterfaceTest, StartOtaDoesNotThrow)
{
    std::cout << "[ThunderOTA.StartOtaDoesNotThrow] - START" << std::endl;
    EXPECT_NO_THROW(iface->StartOta("http://test.url", "wayland", "en", "en", "primary", "primary"));
    std::cout << "  StartOta() completed without exception" << std::endl;
    std::cout << "[ThunderOTA.StartOtaDoesNotThrow] - PASS" << std::endl;
}

/**
 * @brief StartOta() with all empty strings does not throw.
 *
 * **Test Group ID:** ThunderOTA_004@n
 */
TEST_F(ThunderInterfaceTest, StartOtaEmptyStringsDoesNotThrow)
{
    std::cout << "[ThunderOTA.StartOtaEmptyStringsDoesNotThrow] - START" << std::endl;
    EXPECT_NO_THROW(iface->StartOta("", "", "", "", "", ""));
    std::cout << "  StartOta(all empty) completed without exception" << std::endl;
    std::cout << "[ThunderOTA.StartOtaEmptyStringsDoesNotThrow] - PASS" << std::endl;
}

/**
 * @brief StopOta() does not throw.
 *
 * **Test Group ID:** ThunderOTA_005@n
 */
TEST_F(ThunderInterfaceTest, StopOtaDoesNotThrow)
{
    std::cout << "[ThunderOTA.StopOtaDoesNotThrow] - START" << std::endl;
    EXPECT_NO_THROW(iface->StopOta());
    std::cout << "  StopOta() completed without exception" << std::endl;
    std::cout << "[ThunderOTA.StopOtaDoesNotThrow] - PASS" << std::endl;
}

/**
 * @brief GetAudioTracksOta() with empty vector returns empty string.
 *
 * **Test Group ID:** ThunderOTA_006@n
 */
TEST_F(ThunderInterfaceTest, GetAudioTracksOtaEmptyVectorReturnsEmpty)
{
    std::cout << "[ThunderOTA.GetAudioTracksOtaEmptyVectorReturnsEmpty] - START" << std::endl;
    std::vector<PlayerAudioData> v;
    EXPECT_TRUE(iface->GetAudioTracksOta(v).empty());
    std::cout << "  GetAudioTracksOta([]) = empty string" << std::endl;
    std::cout << "[ThunderOTA.GetAudioTracksOtaEmptyVectorReturnsEmpty] - PASS" << std::endl;
}

/**
 * @brief GetAudioTracksOta() with populated vector returns empty string (fake).
 *
 * **Test Group ID:** ThunderOTA_007@n
 */
TEST_F(ThunderInterfaceTest, GetAudioTracksOtaPopulatedVectorReturnsEmpty)
{
    std::cout << "[ThunderOTA.GetAudioTracksOtaPopulatedVectorReturnsEmpty] - START" << std::endl;
    std::vector<PlayerAudioData> v;
    v.emplace_back("en", "main", "English", "AC3", 1, "stereo");
    v.emplace_back("fr", "main", "French",  "AAC", 2, "stereo");
    EXPECT_TRUE(iface->GetAudioTracksOta(v).empty());
    std::cout << "  GetAudioTracksOta([2 tracks]) = empty string" << std::endl;
    std::cout << "[ThunderOTA.GetAudioTracksOtaPopulatedVectorReturnsEmpty] - PASS" << std::endl;
}

/**
 * @brief SetAudioTrackOta() returns empty string (fake).
 *
 * **Test Group ID:** ThunderOTA_008@n
 */
TEST_F(ThunderInterfaceTest, SetAudioTrackOtaReturnsEmpty)
{
    std::cout << "[ThunderOTA.SetAudioTrackOtaReturnsEmpty] - START" << std::endl;
    EXPECT_TRUE(iface->SetAudioTrackOta(0, 1).empty());
    std::cout << "  SetAudioTrackOta(0,1) = empty string" << std::endl;
    std::cout << "[ThunderOTA.SetAudioTrackOtaReturnsEmpty] - PASS" << std::endl;
}

/**
 * @brief SetAudioTrackOta() with negative indices returns empty string.
 *
 * **Test Group ID:** ThunderOTA_009@n
 */
TEST_F(ThunderInterfaceTest, SetAudioTrackOtaNegativeIndexReturnsEmpty)
{
    std::cout << "[ThunderOTA.SetAudioTrackOtaNegativeIndexReturnsEmpty] - START" << std::endl;
    EXPECT_TRUE(iface->SetAudioTrackOta(-1, -1).empty());
    std::cout << "  SetAudioTrackOta(-1,-1) = empty string" << std::endl;
    std::cout << "[ThunderOTA.SetAudioTrackOtaNegativeIndexReturnsEmpty] - PASS" << std::endl;
}

/**
 * @brief GetTextTracksOta() with empty vector returns false.
 *
 * **Test Group ID:** ThunderOTA_010@n
 */
TEST_F(ThunderInterfaceTest, GetTextTracksOtaEmptyVectorReturnsFalse)
{
    std::cout << "[ThunderOTA.GetTextTracksOtaEmptyVectorReturnsFalse] - START" << std::endl;
    std::vector<PlayerTextData> v;
    EXPECT_FALSE(iface->GetTextTracksOta(v));
    std::cout << "  GetTextTracksOta([]) = false" << std::endl;
    std::cout << "[ThunderOTA.GetTextTracksOtaEmptyVectorReturnsFalse] - PASS" << std::endl;
}

/**
 * @brief GetTextTracksOta() with populated vector returns false (fake).
 *
 * **Test Group ID:** ThunderOTA_011@n
 */
TEST_F(ThunderInterfaceTest, GetTextTracksOtaPopulatedVectorReturnsFalse)
{
    std::cout << "[ThunderOTA.GetTextTracksOtaPopulatedVectorReturnsFalse] - START" << std::endl;
    std::vector<PlayerTextData> v;
    v.emplace_back("subtitle", "en", 1, "CC1", "English", 100);
    EXPECT_FALSE(iface->GetTextTracksOta(v));
    std::cout << "  GetTextTracksOta([1 track]) = false" << std::endl;
    std::cout << "[ThunderOTA.GetTextTracksOtaPopulatedVectorReturnsFalse] - PASS" << std::endl;
}

/**
 * @brief DisableContentRestrictionsOta() does not throw with normal values.
 *
 * **Test Group ID:** ThunderOTA_012@n
 */
TEST_F(ThunderInterfaceTest, DisableContentRestrictionsOtaDoesNotThrow)
{
    std::cout << "[ThunderOTA.DisableContentRestrictionsOtaDoesNotThrow] - START" << std::endl;
    EXPECT_NO_THROW(iface->DisableContentRestrictionsOta(30L, 3600L, false));
    std::cout << "  DisableContentRestrictionsOta(30,3600,false) completed" << std::endl;
    std::cout << "[ThunderOTA.DisableContentRestrictionsOtaDoesNotThrow] - PASS" << std::endl;
}

/**
 * @brief DisableContentRestrictionsOta() with -1 grace does not throw.
 *
 * **Test Group ID:** ThunderOTA_013@n
 */
TEST_F(ThunderInterfaceTest, DisableContentRestrictionsUnlimitedGraceDoesNotThrow)
{
    std::cout << "[ThunderOTA.DisableContentRestrictionsUnlimitedGraceDoesNotThrow] - START" << std::endl;
    EXPECT_NO_THROW(iface->DisableContentRestrictionsOta(-1L, 0L, true));
    std::cout << "  DisableContentRestrictionsOta(-1,0,true) completed" << std::endl;
    std::cout << "[ThunderOTA.DisableContentRestrictionsUnlimitedGraceDoesNotThrow] - PASS" << std::endl;
}

/**
 * @brief EnableContentRestrictionsOta() does not throw.
 *
 * **Test Group ID:** ThunderOTA_014@n
 */
TEST_F(ThunderInterfaceTest, EnableContentRestrictionsOtaDoesNotThrow)
{
    std::cout << "[ThunderOTA.EnableContentRestrictionsOtaDoesNotThrow] - START" << std::endl;
    EXPECT_NO_THROW(iface->EnableContentRestrictionsOta());
    std::cout << "  EnableContentRestrictionsOta() completed" << std::endl;
    std::cout << "[ThunderOTA.EnableContentRestrictionsOtaDoesNotThrow] - PASS" << std::endl;
}

// ===========================================================================
// Group: RMF APIs
// ===========================================================================

/**
 * @brief InitRmf() returns false (fake).
 *
 * **Test Group ID:** ThunderRMF_001@n
 */
TEST_F(ThunderInterfaceTest, InitRmfReturnsFalse)
{
    std::cout << "[ThunderRMF.InitRmfReturnsFalse] - START" << std::endl;
    EXPECT_FALSE(iface->InitRmf());
    std::cout << "  InitRmf() = false" << std::endl;
    std::cout << "[ThunderRMF.InitRmfReturnsFalse] - PASS" << std::endl;
}

/**
 * @brief StartRmf() returns false (fake).
 *
 * **Test Group ID:** ThunderRMF_002@n
 */
TEST_F(ThunderInterfaceTest, StartRmfReturnsFalse)
{
    std::cout << "[ThunderRMF.StartRmfReturnsFalse] - START" << std::endl;
    auto cb = [](std::string s){ (void)s; };
    EXPECT_FALSE(iface->StartRmf("udp://localhost:5000", cb, cb));
    std::cout << "  StartRmf() = false" << std::endl;
    std::cout << "[ThunderRMF.StartRmfReturnsFalse] - PASS" << std::endl;
}

/**
 * @brief StartRmf() with empty URL does not throw.
 *
 * **Test Group ID:** ThunderRMF_003@n
 */
TEST_F(ThunderInterfaceTest, StartRmfEmptyUrlDoesNotThrow)
{
    std::cout << "[ThunderRMF.StartRmfEmptyUrlDoesNotThrow] - START" << std::endl;
    auto cb = [](std::string s){ (void)s; };
    EXPECT_NO_THROW(iface->StartRmf("", cb, cb));
    std::cout << "  StartRmf(\"\") completed without exception" << std::endl;
    std::cout << "[ThunderRMF.StartRmfEmptyUrlDoesNotThrow] - PASS" << std::endl;
}

/**
 * @brief StopRmf() does not throw.
 *
 * **Test Group ID:** ThunderRMF_004@n
 */
TEST_F(ThunderInterfaceTest, StopRmfDoesNotThrow)
{
    std::cout << "[ThunderRMF.StopRmfDoesNotThrow] - START" << std::endl;
    EXPECT_NO_THROW(iface->StopRmf());
    std::cout << "  StopRmf() completed without exception" << std::endl;
    std::cout << "[ThunderRMF.StopRmfDoesNotThrow] - PASS" << std::endl;
}

// ===========================================================================
// Group: Watermark APIs
// ===========================================================================

/**
 * @brief DeleteWatermark() returns false for positive layer ID.
 *
 * **Test Group ID:** ThunderWatermark_001@n
 */
TEST_F(ThunderInterfaceTest, DeleteWatermarkReturnsFalse)
{
    std::cout << "[ThunderWatermark.DeleteWatermarkReturnsFalse] - START" << std::endl;
    EXPECT_FALSE(iface->DeleteWatermark(5));
    EXPECT_FALSE(iface->DeleteWatermark(0));
    EXPECT_FALSE(iface->DeleteWatermark(INT_MAX));
    std::cout << "  DeleteWatermark(5/0/INT_MAX) all = false" << std::endl;
    std::cout << "[ThunderWatermark.DeleteWatermarkReturnsFalse] - PASS" << std::endl;
}

/**
 * @brief CreateWatermark() returns false for positive/zero layer ID.
 *
 * **Test Group ID:** ThunderWatermark_002@n
 */
TEST_F(ThunderInterfaceTest, CreateWatermarkReturnsFalse)
{
    std::cout << "[ThunderWatermark.CreateWatermarkReturnsFalse] - START" << std::endl;
    EXPECT_FALSE(iface->CreateWatermark(5));
    EXPECT_FALSE(iface->CreateWatermark(0));
    std::cout << "  CreateWatermark(5/0) = false" << std::endl;
    std::cout << "[ThunderWatermark.CreateWatermarkReturnsFalse] - PASS" << std::endl;
}

/**
 * @brief ShowWatermark() returns false for various opacity values.
 *
 * **Test Group ID:** ThunderWatermark_003@n
 */
TEST_F(ThunderInterfaceTest, ShowWatermarkReturnsFalse)
{
    std::cout << "[ThunderWatermark.ShowWatermarkReturnsFalse] - START" << std::endl;
    EXPECT_FALSE(iface->ShowWatermark(0));
    EXPECT_FALSE(iface->ShowWatermark(50));
    EXPECT_FALSE(iface->ShowWatermark(100));
    std::cout << "  ShowWatermark(0/50/100) all = false" << std::endl;
    std::cout << "[ThunderWatermark.ShowWatermarkReturnsFalse] - PASS" << std::endl;
}

/**
 * @brief HideWatermark() returns false.
 *
 * **Test Group ID:** ThunderWatermark_004@n
 */
TEST_F(ThunderInterfaceTest, HideWatermarkReturnsFalse)
{
    std::cout << "[ThunderWatermark.HideWatermarkReturnsFalse] - START" << std::endl;
    EXPECT_FALSE(iface->HideWatermark());
    std::cout << "  HideWatermark() = false" << std::endl;
    std::cout << "[ThunderWatermark.HideWatermarkReturnsFalse] - PASS" << std::endl;
}

/**
 * @brief UpdateWatermark() returns false for various arguments.
 *
 * **Test Group ID:** ThunderWatermark_005@n
 */
TEST_F(ThunderInterfaceTest, UpdateWatermarkReturnsFalse)
{
    std::cout << "[ThunderWatermark.UpdateWatermarkReturnsFalse] - START" << std::endl;
    EXPECT_FALSE(iface->UpdateWatermark(1, 12345, 1024));
    EXPECT_FALSE(iface->UpdateWatermark(0, 0, 0));
    std::cout << "  UpdateWatermark(valid/zeros) = false" << std::endl;
    std::cout << "[ThunderWatermark.UpdateWatermarkReturnsFalse] - PASS" << std::endl;
}

/**
 * @brief GetMetaDataWatermark() returns empty string.
 *
 * **Test Group ID:** ThunderWatermark_006@n
 */
TEST_F(ThunderInterfaceTest, GetMetaDataWatermarkReturnsEmpty)
{
    std::cout << "[ThunderWatermark.GetMetaDataWatermarkReturnsEmpty] - START" << std::endl;
    EXPECT_TRUE(iface->GetMetaDataWatermark().empty());
    std::cout << "  GetMetaDataWatermark() = empty string" << std::endl;
    std::cout << "[ThunderWatermark.GetMetaDataWatermarkReturnsEmpty] - PASS" << std::endl;
}

/**
 * @brief PersistentStoreSaveWatermark() returns false.
 *
 * **Test Group ID:** ThunderWatermark_007@n
 */
TEST_F(ThunderInterfaceTest, PersistentStoreSaveWatermarkReturnsFalse)
{
    std::cout << "[ThunderWatermark.PersistentStoreSaveWatermarkReturnsFalse] - START" << std::endl;
    EXPECT_FALSE(iface->PersistentStoreSaveWatermark("dGVzdA==", "meta"));
    EXPECT_FALSE(iface->PersistentStoreSaveWatermark("", ""));
    std::cout << "  PersistentStoreSaveWatermark(data/empty) = false" << std::endl;
    std::cout << "[ThunderWatermark.PersistentStoreSaveWatermarkReturnsFalse] - PASS" << std::endl;
}

/**
 * @brief PersistentStoreLoadWatermark() returns false for various layer IDs.
 *
 * **Test Group ID:** ThunderWatermark_008@n
 */
TEST_F(ThunderInterfaceTest, PersistentStoreLoadWatermarkReturnsFalse)
{
    std::cout << "[ThunderWatermark.PersistentStoreLoadWatermarkReturnsFalse] - START" << std::endl;
    EXPECT_FALSE(iface->PersistentStoreLoadWatermark(1));
    EXPECT_FALSE(iface->PersistentStoreLoadWatermark(0));
    std::cout << "  PersistentStoreLoadWatermark(1/0) = false" << std::endl;
    std::cout << "[ThunderWatermark.PersistentStoreLoadWatermarkReturnsFalse] - PASS" << std::endl;
}

// ===========================================================================
// Group: IsThunderAccess
// ===========================================================================

/**
 * @brief IsThunderAccess() returns false (FakeThunderAccess).
 *
 * **Test Group ID:** ThunderIsThunderAccess_001@n
 */
TEST_F(ThunderInterfaceTest, IsThunderAccessReturnsFalse)
{
    std::cout << "[ThunderIsThunderAccess.IsThunderAccessReturnsFalse] - START" << std::endl;
    EXPECT_FALSE(iface->IsThunderAccess());
    std::cout << "  IsThunderAccess() = false (FakeThunderAccess)" << std::endl;
    std::cout << "[ThunderIsThunderAccess.IsThunderAccessReturnsFalse] - PASS" << std::endl;
}

/**
 * @brief IsThunderAccess() returns false for all valid plugin enums.
 *
 * **Test Group ID:** ThunderIsThunderAccess_002@n
 */
TEST(ThunderIsThunderAccess, AllEnumsReturnFalse)
{
    std::cout << "[ThunderIsThunderAccess.AllEnumsReturnFalse] - START" << std::endl;
    for (auto plug : kAllPlugins)
    {
        PlayerThunderInterface pi(plug);
        EXPECT_FALSE(pi.IsThunderAccess());
    }
    std::cout << "  IsThunderAccess() = false for all 10 enums" << std::endl;
    std::cout << "[ThunderIsThunderAccess.AllEnumsReturnFalse] - PASS" << std::endl;
}

// ===========================================================================
// Group: Integration
// ===========================================================================

/**
 * @brief Full OTA workflow: Register → Start → GetAudio → SetAudio → GetText → Stop → Release.
 *
 * **Test Group ID:** ThunderIntegration_001@n
 */
TEST_F(ThunderInterfaceTest, OTAWorkflowSequence)
{
    std::cout << "[ThunderIntegration.OTAWorkflowSequence] - START" << std::endl;
    iface->RegisterOnPlayerStatusOta([](PlayerStatusData d){ (void)d; });
    iface->StartOta("http://test", "wl", "en", "en", "", "");
    std::vector<PlayerAudioData> aud;
    EXPECT_TRUE(iface->GetAudioTracksOta(aud).empty());
    EXPECT_TRUE(iface->SetAudioTrackOta(0, 0).empty());
    std::vector<PlayerTextData> txt;
    EXPECT_FALSE(iface->GetTextTracksOta(txt));
    iface->StopOta();
    iface->ReleaseOta();
    std::cout << "  OTA workflow sequence completed" << std::endl;
    std::cout << "[ThunderIntegration.OTAWorkflowSequence] - PASS" << std::endl;
}

/**
 * @brief Full Watermark workflow: Create→Show→Update→GetMeta→Save→Load→Hide→Delete.
 *
 * **Test Group ID:** ThunderIntegration_002@n
 */
TEST_F(ThunderInterfaceTest, WatermarkWorkflowSequence)
{
    std::cout << "[ThunderIntegration.WatermarkWorkflowSequence] - START" << std::endl;
    EXPECT_FALSE(iface->CreateWatermark(1));
    EXPECT_FALSE(iface->ShowWatermark(80));
    EXPECT_FALSE(iface->UpdateWatermark(1, 99, 512));
    EXPECT_TRUE(iface->GetMetaDataWatermark().empty());
    EXPECT_FALSE(iface->PersistentStoreSaveWatermark("data", "meta"));
    EXPECT_FALSE(iface->PersistentStoreLoadWatermark(1));
    EXPECT_FALSE(iface->HideWatermark());
    EXPECT_FALSE(iface->DeleteWatermark(1));
    std::cout << "  Watermark workflow sequence completed" << std::endl;
    std::cout << "[ThunderIntegration.WatermarkWorkflowSequence] - PASS" << std::endl;
}

/**
 * @brief Full VideoIn workflow: Register→Start→Stop→Unregister.
 *
 * **Test Group ID:** ThunderIntegration_003@n
 */
TEST_F(ThunderInterfaceTest, VideoInWorkflowSequence)
{
    std::cout << "[ThunderIntegration.VideoInWorkflowSequence] - START" << std::endl;
    auto sig = [](std::string s){ (void)s; };
    auto sta = [](std::string s){ (void)s; };
    iface->RegisterAllEventsVideoin(sig, sta);
    iface->StartHelperVideoin(1080, "HDMI");
    iface->StopHelperVideoin("HDMI");
    iface->UnRegisterAllEventsVideoin();
    std::cout << "  VideoIn workflow sequence completed" << std::endl;
    std::cout << "[ThunderIntegration.VideoInWorkflowSequence] - PASS" << std::endl;
}

/**
 * @brief Full RMF workflow: Init→Start→Stop.
 *
 * **Test Group ID:** ThunderIntegration_004@n
 */
TEST_F(ThunderInterfaceTest, RMFWorkflowSequence)
{
    std::cout << "[ThunderIntegration.RMFWorkflowSequence] - START" << std::endl;
    EXPECT_FALSE(iface->InitRmf());
    auto cb = [](std::string s){ (void)s; };
    EXPECT_FALSE(iface->StartRmf("udp://stream", cb, cb));
    iface->StopRmf();
    std::cout << "  RMF workflow sequence completed" << std::endl;
    std::cout << "[ThunderIntegration.RMFWorkflowSequence] - PASS" << std::endl;
}

/**
 * @brief Stress: 20 × 10 plugin instances each call ActivatePlugin + IsThunderAccess.
 *
 * **Test Group ID:** ThunderIntegration_005@n
 */
TEST(ThunderIntegration, Stress20InstancesPerPlugin)
{
    std::cout << "[ThunderIntegration.Stress20InstancesPerPlugin] - START" << std::endl;
    for (int rep = 0; rep < 20; ++rep)
    {
        for (auto plug : kAllPlugins)
        {
            PlayerThunderInterface pi(plug);
            EXPECT_FALSE(pi.ActivatePlugin());
            EXPECT_FALSE(pi.IsThunderAccess());
        }
    }
    std::cout << "  200 instances exercised safely" << std::endl;
    std::cout << "[ThunderIntegration.Stress20InstancesPerPlugin] - PASS" << std::endl;
}
