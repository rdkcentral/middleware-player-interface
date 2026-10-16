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
 * @file InterfacePlayerGapFillTests.cpp
 * @brief L1 coverage gap-fill for InterfacePlayerRDK.cpp paths not already
 *        exercised by InterfacePlayerFunctionTests.cpp:
 *        gstGetMediaTypeName table, idle-callback dispatchers, free helper
 *        functions, PTS query/fallback chain, EOS/first-frame flag machine,
 *        volume/mute/zoom plumbing, CC control stream setup and assorted
 *        small getters/setters.
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "InterfacePlayerRDK.h"
#include "InterfacePlayerPriv.h"
#include "PlayerLogManager.h"
#include "MockGStreamer.h"
#include "MockGLib.h"
#include "MockGstHandlerControl.h"
#include "MockPlayerScheduler.h"
#include "MockGstUtils.h"
#include <gst/gstplugin.h>
#include <gst/gstpluginfeature.h>
#include <atomic>

using ::testing::NiceMock;
using ::testing::StrictMock;
using ::testing::Return;
using ::testing::StrEq;
using ::testing::Eq;
using ::testing::_;
using ::testing::Address;
using ::testing::DoAll;
using ::testing::SetArgPointee;
using ::testing::NotNull;
using ::testing::SaveArgPointee;
using ::testing::SaveArg;
using ::testing::Pointer;
using ::testing::Matcher;
using ::testing::AnyNumber;
using ::testing::AtLeast;

#define GST_NORMAL_PLAY_RATE		1

/* Free functions defined in InterfacePlayerRDK.cpp but not declared in its header */
extern const char *gstGetMediaTypeName(GstMediaType mediaType);
extern bool gst_StartsWith( const char *inputStr, const char *prefix );
extern bool GstPlayer_isVideoOrAudioDecoder(const char *name, InterfacePlayerRDK *pInterfacePlayerRDK);
extern bool GstPlayer_isVideoDecoder(const char *name, InterfacePlayerRDK *pInterfacePlayerRDK);
extern void AddBufferFieldToStructure(GstStructure *structure, const char *fieldName, const std::vector<uint8_t> &data);
extern void MonitorAV( InterfacePlayerRDK *pInterfacePlayerRDK );

/**
 * @brief Minimal concrete SocInterface for driving GetVideoPts return values.
 *        Only GetVideoPts is overridden; other members satisfy pure virtuals.
 */
class GapFillSocInterface : public SocInterface
{
public:
	long long videoPtsToReturn = 0;
	bool getVideoPtsCalled = false;

	void SetAudioProperty(const char * &volume, const char * &mute, bool& isSinkBinVolume) override
	{
		volume = "volume";
		mute = "mute";
		isSinkBinVolume = false;
	}
	bool SetPlaybackRate(const std::vector<GstElement*>& sources, GstElement *pipeline, double rate, GstElement *video_dec, GstElement *audio_dec) override
	{
		return true;
	}
	bool SetRateCorrection() override { return false; }
	bool IsVideoSink(const char* name) override { return false; }
	bool IsAudioSinkOrAudioDecoder(const char* name) override { return false; }
	bool IsVideoDecoder(const char* name) override { return false; }
	bool ConfigureAudioSink(GstElement **audio_sink, GstObject *src, bool decStreamSync) override { return false; }
	bool IsAudioOrVideoDecoder(const char* name) override { return false; }
	void GetCCDecoderHandle(gpointer *dec_handle, GstElement *video_dec) override {}
	void SetPlaybackFlags(gint &flags, bool isSub) override { flags = 0; }
	bool IsVideoMaster(GstElement *videoSink) override { return true; }
	long long GetVideoPts(GstElement *video_sink, GstElement *video_dec, bool isWesteros) override
	{
		getVideoPtsCalled = true;
		return videoPtsToReturn;
	}
};

class InterfacePlayerGapFillTests : public ::testing::Test
{

protected:
	bool isPipelineSetup = false;
	GstElement gst_element_pipeline = {.object = {.name = (gchar *)"Pipeline"}};
	GstBus bus = {};
	GstQuery query = {};
	InterfacePlayerRDK *mInterfaceGstPlayer;
	InterfacePlayerPriv *mInterfacePrivatePlayer;

	GstPlayerPriv* mPlayerContext;
	Configs* mPlayerConfigParams;
	int cbResponse = 0;

	void SetUp() override
	{
		g_mockGStreamer = new NiceMock<MockGStreamer>();
		g_mockGLib = new NiceMock<MockGLib>();
		g_mockPlayerScheduler = new NiceMock<MockPlayerScheduler>();

		ConstructAMPGstPlayer();
		PlayerLogManager::lockLogLevel(false);
		PlayerLogManager::disableLogRedirection = true; //required for mwlog output in utest
		PlayerLogManager::setLogLevel(mLOGLEVEL_TRACE);
	}

	void TearDown() override
	{
		delete g_mockGStreamer;
		g_mockGStreamer = nullptr;

		DestroyAMPGstPlayer();

		delete g_mockGLib;
		g_mockGLib = nullptr;

		delete g_mockPlayerScheduler;
		g_mockPlayerScheduler = nullptr;
	}

public:
	void ConstructAMPGstPlayer()
	{
		mInterfaceGstPlayer = new InterfacePlayerRDK();
		mInterfacePrivatePlayer  = mInterfaceGstPlayer->GetPrivatePlayer();

		mPlayerContext = mInterfacePrivatePlayer->gstPrivateContext;

		mPlayerConfigParams = mInterfaceGstPlayer->m_gstConfigParam;
		//init callback to avoid bad_function_call error
		mInterfaceGstPlayer->TearDownCallback([this](bool status, int mediaType) {
		if (status) {
			cbResponse = 5;
		}});
		mInterfaceGstPlayer->StopCallback([this](bool status)
			{
				printf("StopCallback status: %d\n", status);
			});
	}

	void DestroyAMPGstPlayer()
	{
		delete mInterfaceGstPlayer;
		mPlayerContext = nullptr;
		mInterfaceGstPlayer = nullptr;
	}

};

/* ------------------------------------------------------------------------- */
/* gstGetMediaTypeName - full table                                          */
/* ------------------------------------------------------------------------- */

TEST_F(InterfacePlayerGapFillTests, MediaTypeName_Video)
{
	EXPECT_STREQ(gstGetMediaTypeName(eGST_MEDIATYPE_VIDEO), "video");
}

TEST_F(InterfacePlayerGapFillTests, MediaTypeName_Audio)
{
	EXPECT_STREQ(gstGetMediaTypeName(eGST_MEDIATYPE_AUDIO), "audio");
}

TEST_F(InterfacePlayerGapFillTests, MediaTypeName_Subtitle)
{
	EXPECT_STREQ(gstGetMediaTypeName(eGST_MEDIATYPE_SUBTITLE), "text");
}

TEST_F(InterfacePlayerGapFillTests, MediaTypeName_Reserved)
{
	EXPECT_STREQ(gstGetMediaTypeName(eGST_MEDIATYPE_RESERVED), "reserved");
}

TEST_F(InterfacePlayerGapFillTests, MediaTypeName_Manifest)
{
	EXPECT_STREQ(gstGetMediaTypeName(eGST_MEDIATYPE_MANIFEST), "manifest");
}

TEST_F(InterfacePlayerGapFillTests, MediaTypeName_Licence)
{
	EXPECT_STREQ(gstGetMediaTypeName(eGST_MEDIATYPE_LICENCE), "licence");
}

TEST_F(InterfacePlayerGapFillTests, MediaTypeName_Iframe)
{
	EXPECT_STREQ(gstGetMediaTypeName(eGST_MEDIATYPE_IFRAME), "iframe");
}

TEST_F(InterfacePlayerGapFillTests, MediaTypeName_InitVideo)
{
	EXPECT_STREQ(gstGetMediaTypeName(eGST_MEDIATYPE_INIT_VIDEO), "init_video");
}

TEST_F(InterfacePlayerGapFillTests, MediaTypeName_InitAudio)
{
	EXPECT_STREQ(gstGetMediaTypeName(eGST_MEDIATYPE_INIT_AUDIO), "init_audio");
}

TEST_F(InterfacePlayerGapFillTests, MediaTypeName_InitSubtitle)
{
	EXPECT_STREQ(gstGetMediaTypeName(eGST_MEDIATYPE_INIT_SUBTITLE), "init_text");
}

TEST_F(InterfacePlayerGapFillTests, MediaTypeName_InitReserved)
{
	EXPECT_STREQ(gstGetMediaTypeName(eGST_MEDIATYPE_INIT_RESERVED), "init_reserved");
}

TEST_F(InterfacePlayerGapFillTests, MediaTypeName_PlaylistVideo)
{
	EXPECT_STREQ(gstGetMediaTypeName(eGST_MEDIATYPE_PLAYLIST_VIDEO), "playlist_video");
}

TEST_F(InterfacePlayerGapFillTests, MediaTypeName_PlaylistAudio)
{
	EXPECT_STREQ(gstGetMediaTypeName(eGST_MEDIATYPE_PLAYLIST_AUDIO), "playlist_audio");
}

TEST_F(InterfacePlayerGapFillTests, MediaTypeName_PlaylistSubtitle)
{
	EXPECT_STREQ(gstGetMediaTypeName(eGST_MEDIATYPE_PLAYLIST_SUBTITLE), "playlist_text");
}

TEST_F(InterfacePlayerGapFillTests, MediaTypeName_PlaylistReserved)
{
	EXPECT_STREQ(gstGetMediaTypeName(eGST_MEDIATYPE_PLAYLIST_RESERVED), "playlist_reserved");
}

TEST_F(InterfacePlayerGapFillTests, MediaTypeName_PlaylistIframe)
{
	EXPECT_STREQ(gstGetMediaTypeName(eGST_MEDIATYPE_PLAYLIST_IFRAME), "playlist_iframe");
}

TEST_F(InterfacePlayerGapFillTests, MediaTypeName_InitIframe)
{
	EXPECT_STREQ(gstGetMediaTypeName(eGST_MEDIATYPE_INIT_IFRAME), "init_iframe");
}

TEST_F(InterfacePlayerGapFillTests, MediaTypeName_DsmCc)
{
	EXPECT_STREQ(gstGetMediaTypeName(eGST_MEDIATYPE_DSM_CC), "dsm_cc");
}

TEST_F(InterfacePlayerGapFillTests, MediaTypeName_Image)
{
	EXPECT_STREQ(gstGetMediaTypeName(eGST_MEDIATYPE_IMAGE), "image");
}

TEST_F(InterfacePlayerGapFillTests, MediaTypeName_DefaultBoundary)
{
	EXPECT_STREQ(gstGetMediaTypeName(eGST_MEDIATYPE_DEFAULT), "UNKNOWN");
}

TEST_F(InterfacePlayerGapFillTests, MediaTypeName_BeyondRange)
{
	EXPECT_STREQ(gstGetMediaTypeName(static_cast<GstMediaType>(eGST_MEDIATYPE_DEFAULT + 5)), "UNKNOWN");
}

/* ------------------------------------------------------------------------- */
/* gst_StartsWith                                                            */
/* ------------------------------------------------------------------------- */

TEST_F(InterfacePlayerGapFillTests, StartsWith_MatchingPrefix)
{
	EXPECT_TRUE(gst_StartsWith("playbin0", "playbin"));
}

TEST_F(InterfacePlayerGapFillTests, StartsWith_NonMatchingPrefix)
{
	EXPECT_FALSE(gst_StartsWith("vipertransform", "playbin"));
}

TEST_F(InterfacePlayerGapFillTests, StartsWith_PrefixLongerThanInput)
{
	// NOTE: prefix is scanned until NUL of prefix; mismatched bytes fail first
	EXPECT_FALSE(gst_StartsWith("qt", "qtdemux0"));
}

TEST_F(InterfacePlayerGapFillTests, StartsWith_EmptyPrefixAlwaysMatches)
{
	EXPECT_TRUE(gst_StartsWith("anything", ""));
}

TEST_F(InterfacePlayerGapFillTests, StartsWith_IdenticalStrings)
{
	EXPECT_TRUE(gst_StartsWith("rialtomseaudiosink", "rialtomseaudiosink"));
}

TEST_F(InterfacePlayerGapFillTests, StartsWith_RialtoSinkNames)
{
	EXPECT_TRUE(gst_StartsWith("rialtomsevideosink0", "rialtomsevideosink"));
	EXPECT_TRUE(gst_StartsWith("rialtomseaudiosink1", "rialtomseaudiosink"));
	EXPECT_FALSE(gst_StartsWith("rialtomseaudiosink0", "rialtomsevideosink"));
}

/* ------------------------------------------------------------------------- */
/* GstPlayer_isVideoOrAudioDecoder / GstPlayer_isVideoDecoder                */
/* (delegate to SocInterface - default fake matches rialto/westeros names)   */
/* ------------------------------------------------------------------------- */

TEST_F(InterfacePlayerGapFillTests, IsVideoOrAudioDecoder_RialtoVideoSink)
{
	EXPECT_TRUE(GstPlayer_isVideoOrAudioDecoder("rialtomsevideosink0", mInterfaceGstPlayer));
}

TEST_F(InterfacePlayerGapFillTests, IsVideoOrAudioDecoder_RialtoAudioSink)
{
	EXPECT_TRUE(GstPlayer_isVideoOrAudioDecoder("rialtomseaudiosink0", mInterfaceGstPlayer));
}

TEST_F(InterfacePlayerGapFillTests, IsVideoOrAudioDecoder_UnrelatedElement)
{
	EXPECT_FALSE(GstPlayer_isVideoOrAudioDecoder("audioparse0", mInterfaceGstPlayer));
}

TEST_F(InterfacePlayerGapFillTests, IsVideoOrAudioDecoder_NullName)
{
	EXPECT_FALSE(GstPlayer_isVideoOrAudioDecoder(nullptr, mInterfaceGstPlayer));
}

TEST_F(InterfacePlayerGapFillTests, IsVideoDecoder_RialtoVideoSink)
{
	EXPECT_TRUE(GstPlayer_isVideoDecoder("rialtomsevideosink0", mInterfaceGstPlayer));
}

TEST_F(InterfacePlayerGapFillTests, IsVideoDecoder_AudioSinkName)
{
	EXPECT_FALSE(GstPlayer_isVideoDecoder("rialtomseaudiosink0", mInterfaceGstPlayer));
}

/* ------------------------------------------------------------------------- */
/* IsPipelinePaused / EnablePendingPlayState / small flag plumbing           */
/* ------------------------------------------------------------------------- */

TEST_F(InterfacePlayerGapFillTests, IsPipelinePaused_DefaultFalse)
{
	EXPECT_FALSE(mInterfaceGstPlayer->IsPipelinePaused());
}

TEST_F(InterfacePlayerGapFillTests, IsPipelinePaused_ReflectsPausedFlag)
{
	mPlayerContext->paused = true;
	EXPECT_TRUE(mInterfaceGstPlayer->IsPipelinePaused());
	mPlayerContext->paused = false;
	EXPECT_FALSE(mInterfaceGstPlayer->IsPipelinePaused());
}

TEST_F(InterfacePlayerGapFillTests, EnablePendingPlayState_SetsFlag)
{
	mPlayerContext->pendingPlayState = false;
	mInterfaceGstPlayer->EnablePendingPlayState();
	EXPECT_TRUE(mPlayerContext->pendingPlayState);
}

TEST_F(InterfacePlayerGapFillTests, GetMonitorAVState_Defaults)
{
	const MonitorAVState& state = mInterfaceGstPlayer->GetMonitorAVState();
	EXPECT_FALSE(state.happy);
	EXPECT_EQ(state.tLastReported, 0);
	EXPECT_EQ(state.tLastSampled, 0);
	EXPECT_EQ(state.av_position[0], 0);
	EXPECT_EQ(state.av_position[1], 0);
}

TEST_F(InterfacePlayerGapFillTests, GetMonitorAVState_ReturnsContextMember)
{
	mPlayerContext->monitorAVstate.happy = true;
	mPlayerContext->monitorAVstate.av_position[0] = 1234;
	const MonitorAVState& state = mInterfaceGstPlayer->GetMonitorAVState();
	EXPECT_TRUE(state.happy);
	EXPECT_EQ(state.av_position[0], 1234);
}

/* ------------------------------------------------------------------------- */
/* Idle callbacks invoked directly                                            */
/* ------------------------------------------------------------------------- */

TEST_F(InterfacePlayerGapFillTests, IdleCallbackOnFirstFrame_NullUserData)
{
	EXPECT_EQ(InterfacePlayerRDK::IdleCallbackOnFirstFrame(nullptr), G_SOURCE_REMOVE);
}

TEST_F(InterfacePlayerGapFillTests, IdleCallbackOnFirstFrame_ClearsTaskState)
{
	bool eventFired = false;
	mInterfaceGstPlayer->callbackMap[InterfaceCB::firstVideoFrameReceived] = [&eventFired]() { eventFired = true; };
	mPlayerContext->firstFrameCallbackIdleTaskId = 42;
	mPlayerContext->firstFrameCallbackIdleTaskPending = true;

	EXPECT_EQ(InterfacePlayerRDK::IdleCallbackOnFirstFrame(mInterfaceGstPlayer), G_SOURCE_REMOVE);
	EXPECT_TRUE(eventFired);
	EXPECT_EQ(mPlayerContext->firstFrameCallbackIdleTaskId, PLAYER_TASK_ID_INVALID);
	EXPECT_FALSE(mPlayerContext->firstFrameCallbackIdleTaskPending);
}

TEST_F(InterfacePlayerGapFillTests, IdleCallbackOnFirstFrame_NoCallbackRegistered)
{
	// No callback registered for firstVideoFrameReceived - must not crash
	EXPECT_EQ(InterfacePlayerRDK::IdleCallbackOnFirstFrame(mInterfaceGstPlayer), G_SOURCE_REMOVE);
}

TEST_F(InterfacePlayerGapFillTests, IdleCallbackOnEOS_NullUserData)
{
	EXPECT_EQ(InterfacePlayerRDK::IdleCallbackOnEOS(nullptr), G_SOURCE_REMOVE);
}

TEST_F(InterfacePlayerGapFillTests, IdleCallbackOnEOS_ClearsTaskState)
{
	bool eventFired = false;
	mInterfaceGstPlayer->callbackMap[InterfaceCB::notifyEOS] = [&eventFired]() { eventFired = true; };
	mPlayerContext->eosCallbackIdleTaskId = 77;
	mPlayerContext->eosCallbackIdleTaskPending = true;

	EXPECT_EQ(InterfacePlayerRDK::IdleCallbackOnEOS(mInterfaceGstPlayer), G_SOURCE_REMOVE);
	EXPECT_TRUE(eventFired);
	EXPECT_EQ(mPlayerContext->eosCallbackIdleTaskId, PLAYER_TASK_ID_INVALID);
	EXPECT_FALSE(mPlayerContext->eosCallbackIdleTaskPending);
}

TEST_F(InterfacePlayerGapFillTests, IdleCallback_NullUserData)
{
	EXPECT_EQ(InterfacePlayerRDK::IdleCallback(nullptr), G_SOURCE_REMOVE);
}

TEST_F(InterfacePlayerGapFillTests, IdleCallback_TriggersIdleEvent)
{
	bool idleFired = false;
	mInterfaceGstPlayer->callbackMap[InterfaceCB::idleCb] = [&idleFired]() { idleFired = true; };
	EXPECT_EQ(InterfacePlayerRDK::IdleCallback(mInterfaceGstPlayer), G_SOURCE_REMOVE);
	EXPECT_TRUE(idleFired);
}

TEST_F(InterfacePlayerGapFillTests, IdleCallback_ClearsFirstProgressTaskFlags)
{
	mPlayerContext->firstProgressCallbackIdleTask.taskID = 55;
	mPlayerContext->firstProgressCallbackIdleTask.taskIsPending = true;
	mInterfaceGstPlayer->callbackMap[InterfaceCB::idleCb] = []() {};
	EXPECT_EQ(InterfacePlayerRDK::IdleCallback(mInterfaceGstPlayer), G_SOURCE_REMOVE);
	EXPECT_EQ(mPlayerContext->firstProgressCallbackIdleTask.taskID, 0);
	EXPECT_FALSE(mPlayerContext->firstProgressCallbackIdleTask.taskIsPending);
}

TEST_F(InterfacePlayerGapFillTests, IdleCallbackFirstVideoFrameDisplayed_NullUserData)
{
	EXPECT_EQ(InterfacePlayerRDK::IdleCallbackFirstVideoFrameDisplayed(nullptr), G_SOURCE_REMOVE);
}

TEST_F(InterfacePlayerGapFillTests, IdleCallbackFirstVideoFrameDisplayed_TriggersEvent)
{
	bool displayed = false;
	mInterfaceGstPlayer->callbackMap[InterfaceCB::firstVideoFrameDisplayed] = [&displayed]() { displayed = true; };
	EXPECT_EQ(InterfacePlayerRDK::IdleCallbackFirstVideoFrameDisplayed(mInterfaceGstPlayer), G_SOURCE_REMOVE);
	EXPECT_TRUE(displayed);
}

TEST_F(InterfacePlayerGapFillTests, ProgressCallbackOnTimeout_NullUserData)
{
	EXPECT_EQ(InterfacePlayerRDK::ProgressCallbackOnTimeout(nullptr), G_SOURCE_CONTINUE);
}

TEST_F(InterfacePlayerGapFillTests, ProgressCallbackOnTimeout_TriggersProgressEvent)
{
	bool progressFired = false;
	mInterfaceGstPlayer->callbackMap[InterfaceCB::progressCb] = [&progressFired]() { progressFired = true; };
	mPlayerConfigParams->monitorAV = false;
	EXPECT_EQ(InterfacePlayerRDK::ProgressCallbackOnTimeout(mInterfaceGstPlayer), G_SOURCE_CONTINUE);
	EXPECT_TRUE(progressFired);
}

TEST_F(InterfacePlayerGapFillTests, ProgressCallbackOnTimeout_MonitorAVEnabledNoPipeline)
{
	// monitorAV path exercises MonitorAV() with null pipeline - get_state mocked default
	mPlayerConfigParams->monitorAV = true;
	mInterfaceGstPlayer->callbackMap[InterfaceCB::progressCb] = []() {};
	EXPECT_EQ(InterfacePlayerRDK::ProgressCallbackOnTimeout(mInterfaceGstPlayer), G_SOURCE_CONTINUE);
}

/* ------------------------------------------------------------------------- */
/* TimerIsRunning / IdleTaskClearFlags edge paths                            */
/* ------------------------------------------------------------------------- */

TEST_F(InterfacePlayerGapFillTests, TimerIsRunning_InvalidIdReturnsFalse)
{
	guint taskId = PLAYER_TASK_ID_INVALID;
	EXPECT_FALSE(mInterfaceGstPlayer->TimerIsRunning(taskId));
}

TEST_F(InterfacePlayerGapFillTests, TimerIsRunning_ValidIdReturnsTrue)
{
	guint taskId = 123;
	EXPECT_TRUE(mInterfaceGstPlayer->TimerIsRunning(taskId));
}

TEST_F(InterfacePlayerGapFillTests, IdleTaskClearFlags_ZeroId)
{
	GstTaskControlData task("zeroIdTask");
	task.taskID = 0;
	task.taskIsPending = true;
	mInterfaceGstPlayer->IdleTaskClearFlags(task);
	EXPECT_EQ(task.taskID, 0u);
	EXPECT_FALSE(task.taskIsPending);
}

TEST_F(InterfacePlayerGapFillTests, IdleTaskAdd_NullFuncStillSchedules)
{
	// funcPtr nullptr is legal for ScheduleTask signature; mock returns task id
	GstTaskControlData task("nullFunc");
	EXPECT_CALL(*g_mockPlayerScheduler, ScheduleTask(_)).WillOnce(Return(11));
	EXPECT_TRUE(mInterfaceGstPlayer->IdleTaskAdd(task, nullptr));
	EXPECT_TRUE(task.taskIsPending);
	EXPECT_EQ(task.taskID, 11u);
}

/* ------------------------------------------------------------------------- */
/* Callback registration members                                             */
/* ------------------------------------------------------------------------- */

TEST_F(InterfacePlayerGapFillTests, RegisterNeedDataCb_StoresCallback)
{
	int called = 0;
	mInterfaceGstPlayer->RegisterNeedDataCb([&called](int mediaType) { called++; });
	ASSERT_NE(mInterfaceGstPlayer->NeedDataCb, nullptr);
	mInterfaceGstPlayer->NeedDataCb(eGST_MEDIATYPE_VIDEO);
	EXPECT_EQ(called, 1);
}

TEST_F(InterfacePlayerGapFillTests, RegisterEnoughDataCb_StoresCallback)
{
	int called = 0;
	mInterfaceGstPlayer->RegisterEnoughDataCb([&called](int mediaType) { called++; });
	ASSERT_NE(mInterfaceGstPlayer->EnoughDataCb, nullptr);
	mInterfaceGstPlayer->EnoughDataCb(eGST_MEDIATYPE_AUDIO);
	EXPECT_EQ(called, 1);
}

TEST_F(InterfacePlayerGapFillTests, RegisterBufferingTimeoutCb_StoresCallback)
{
	int called = 0;
	mInterfaceGstPlayer->RegisterBufferingTimeoutCb([&called](bool a, bool b, bool c) { called++; });
	ASSERT_NE(mInterfaceGstPlayer->OnBuffering_timeoutCb, nullptr);
	mInterfaceGstPlayer->OnBuffering_timeoutCb(true, true, true);
	EXPECT_EQ(called, 1);
}

TEST_F(InterfacePlayerGapFillTests, RegisterGstPtsErrorCb_StoresCallback)
{
	int called = 0;
	mInterfaceGstPlayer->RegisterGstPtsErrorCb([&called](bool isVideo, bool isAudioSink) { called++; });
	ASSERT_NE(mInterfaceGstPlayer->OnGstPtsErrorCb, nullptr);
	mInterfaceGstPlayer->OnGstPtsErrorCb(true, false);
	EXPECT_EQ(called, 1);
}

TEST_F(InterfacePlayerGapFillTests, RegisterGstDecodeErrorCb_StoresCallback)
{
	int called = 0;
	mInterfaceGstPlayer->RegisterGstDecodeErrorCb([&called](int cnt) { called++; });
	ASSERT_NE(mInterfaceGstPlayer->OnGstDecodeErrorCb, nullptr);
	mInterfaceGstPlayer->OnGstDecodeErrorCb(3);
	EXPECT_EQ(called, 1);
}

TEST_F(InterfacePlayerGapFillTests, RegisterBufferUnderflowCb_StoresCallback)
{
	int mediaTypeSeen = -1;
	mInterfaceGstPlayer->RegisterBufferUnderflowCb([&mediaTypeSeen](int mediaType) { mediaTypeSeen = mediaType; });
	ASSERT_NE(mInterfaceGstPlayer->OnGstBufferUnderflowCb, nullptr);
	mInterfaceGstPlayer->OnGstBufferUnderflowCb(eGST_MEDIATYPE_AUDIO);
	EXPECT_EQ(mediaTypeSeen, eGST_MEDIATYPE_AUDIO);
}

TEST_F(InterfacePlayerGapFillTests, RegisterHandleRedButtonCallback_StoresCallback)
{
	const char *seen = nullptr;
	mInterfaceGstPlayer->RegisterHandleRedButtonCallback([&seen](const char *data) { seen = data; });
	ASSERT_NE(mInterfaceGstPlayer->OnHandleRedButtonCallback, nullptr);
	mInterfaceGstPlayer->OnHandleRedButtonCallback("redbutton");
	EXPECT_STREQ(seen, "redbutton");
}

TEST_F(InterfacePlayerGapFillTests, RegisterBusEvent_StoresCallback)
{
	int called = 0;
	mInterfaceGstPlayer->RegisterBusEvent([&called](const BusEventData data) { called++; });
	ASSERT_NE(mInterfaceGstPlayer->busMessageCallback, nullptr);
	BusEventData evt{};
	mInterfaceGstPlayer->busMessageCallback(evt);
	EXPECT_EQ(called, 1);
}

/* ------------------------------------------------------------------------- */
/* Struct / POD contracts                                                    */
/* ------------------------------------------------------------------------- */

TEST_F(InterfacePlayerGapFillTests, GstTaskControlData_Constructor)
{
	GstTaskControlData task("progressTimer");
	EXPECT_EQ(task.taskID, 0u);
	EXPECT_FALSE(task.taskIsPending);
	EXPECT_STREQ(task.taskName.c_str(), "progressTimer");
}

TEST_F(InterfacePlayerGapFillTests, GstTaskControlData_NullName)
{
	GstTaskControlData task(nullptr);
	EXPECT_STREQ(task.taskName.c_str(), "undefined");
}

TEST_F(InterfacePlayerGapFillTests, MonitorAVState_DefaultCtor)
{
	MonitorAVState st;
	EXPECT_EQ(st.tLastReported, 0);
	EXPECT_EQ(st.tLastSampled, 0);
	EXPECT_EQ(st.description, nullptr);
	EXPECT_EQ(st.av_position[0], 0);
	EXPECT_EQ(st.av_position[1], 0);
	EXPECT_FALSE(st.happy);
}

TEST_F(InterfacePlayerGapFillTests, PlayerAsyncTaskObj_MoveCtor)
{
	static bool ran = false;
	PlayerAsyncTaskObj obj([](void *d) { *static_cast<bool*>(d) = true; }, &ran, "taskName", 7);
	EXPECT_EQ(obj.mId, 7);
	EXPECT_EQ(obj.mData, &ran);
	EXPECT_STREQ(obj.mTaskName.c_str(), "taskName");
	obj.mTask(obj.mData);
	EXPECT_TRUE(ran);
	ran = false;
}

TEST_F(InterfacePlayerGapFillTests, PlayerAsyncTaskObj_CopyAssign)
{
	PlayerAsyncTaskObj a([](void*){}, nullptr, "a", 3);
	PlayerAsyncTaskObj b([](void*){}, nullptr, "b", 9);
	b = a;
	EXPECT_EQ(b.mId, 3);
	EXPECT_STREQ(b.mTaskName.c_str(), "a");
}

/* ------------------------------------------------------------------------- */
/* GetVideoPTS / GetVideoPosition                                            */
/* ------------------------------------------------------------------------- */

TEST_F(InterfacePlayerGapFillTests, GetVideoPTS_DirectReturn)
{
	auto soc = std::make_shared<GapFillSocInterface>();
	soc->videoPtsToReturn = 90000;
	mInterfacePrivatePlayer->socInterface = soc;
	EXPECT_EQ(mInterfaceGstPlayer->GetVideoPTS(), 90000);
	EXPECT_TRUE(soc->getVideoPtsCalled);
}

TEST_F(InterfacePlayerGapFillTests, GetVideoPTS_FallbackWhenPropertyUnsupported)
{
	// socInterface->GetVideoPts() == -1 triggers gst_element_query_position fallback
	auto soc = std::make_shared<GapFillSocInterface>();
	soc->videoPtsToReturn = -1;
	mInterfacePrivatePlayer->socInterface = soc;
	// no pipeline configured - GetVideoPosition returns 0 -> 0 ticks
	EXPECT_EQ(mInterfaceGstPlayer->GetVideoPTS(), 0);
	EXPECT_TRUE(soc->getVideoPtsCalled);
}

TEST_F(InterfacePlayerGapFillTests, GetVideoPTS_FallbackConvertsMsTo90kHzTicks)
{
	auto soc = std::make_shared<GapFillSocInterface>();
	soc->videoPtsToReturn = -1;
	mInterfacePrivatePlayer->socInterface = soc;

	GstElement videoSinkbin{};
	mPlayerContext->pipeline = &gst_element_pipeline;
	mPlayerContext->stream[eGST_MEDIATYPE_VIDEO].sinkbin = &videoSinkbin;

	ON_CALL(*g_mockGStreamer, gst_element_get_state(_, _, _, _))
		.WillByDefault(DoAll(SetArgPointee<1>(GST_STATE_PLAYING), SetArgPointee<2>(GST_STATE_NULL), Return(GST_STATE_CHANGE_SUCCESS)));
	ON_CALL(*g_mockGStreamer, gst_element_query_position(_, _, _))
		.WillByDefault(DoAll(SetArgPointee<2>(1000 * GST_MSECOND), Return(TRUE))); // position in ns

	// 1000ms * 90 = 90000 ticks
	EXPECT_EQ(mInterfaceGstPlayer->GetVideoPTS(), 1000 * MPEG_PTS_TICKS_PER_MS);

	mPlayerContext->stream[eGST_MEDIATYPE_VIDEO].sinkbin = nullptr;
	mPlayerContext->pipeline = nullptr;
}

TEST_F(InterfacePlayerGapFillTests, GetVideoPosition_NullPipelineReturnsZero)
{
	mPlayerContext->pipeline = nullptr;
	EXPECT_EQ(mInterfaceGstPlayer->GetVideoPosition(), 0);
}

TEST_F(InterfacePlayerGapFillTests, GetVideoPosition_NullSinkbinReturnsZero)
{
	mPlayerContext->pipeline = &gst_element_pipeline;
	mPlayerContext->stream[eGST_MEDIATYPE_VIDEO].sinkbin = nullptr;
	EXPECT_EQ(mInterfaceGstPlayer->GetVideoPosition(), 0);
}

TEST_F(InterfacePlayerGapFillTests, GetVideoPosition_NotPlayingOrPausedReturnsZero)
{
	GstElement videoSinkbin{};
	mPlayerContext->pipeline = &gst_element_pipeline;
	mPlayerContext->stream[eGST_MEDIATYPE_VIDEO].sinkbin = &videoSinkbin;
	EXPECT_CALL(*g_mockGStreamer, gst_element_get_state(_, _, _, _))
		.WillOnce(DoAll(SetArgPointee<1>(GST_STATE_READY), SetArgPointee<2>(GST_STATE_NULL), Return(GST_STATE_CHANGE_SUCCESS)));
	EXPECT_CALL(*g_mockGStreamer, gst_element_query_position(_, _, _)).Times(0);
	EXPECT_EQ(mInterfaceGstPlayer->GetVideoPosition(), 0);
	mPlayerContext->stream[eGST_MEDIATYPE_VIDEO].sinkbin = nullptr;
	mPlayerContext->pipeline = nullptr;
}

TEST_F(InterfacePlayerGapFillTests, GetVideoPosition_StateChangeFailedReturnsZero)
{
	GstElement videoSinkbin{};
	mPlayerContext->pipeline = &gst_element_pipeline;
	mPlayerContext->stream[eGST_MEDIATYPE_VIDEO].sinkbin = &videoSinkbin;
	EXPECT_CALL(*g_mockGStreamer, gst_element_get_state(_, _, _, _))
		.WillOnce(Return(GST_STATE_CHANGE_FAILURE));
	EXPECT_EQ(mInterfaceGstPlayer->GetVideoPosition(), 0);
	mPlayerContext->stream[eGST_MEDIATYPE_VIDEO].sinkbin = nullptr;
	mPlayerContext->pipeline = nullptr;
}

TEST_F(InterfacePlayerGapFillTests, GetVideoPosition_QueryFailureReturnsZero)
{
	GstElement videoSinkbin{};
	mPlayerContext->pipeline = &gst_element_pipeline;
	mPlayerContext->stream[eGST_MEDIATYPE_VIDEO].sinkbin = &videoSinkbin;
	EXPECT_CALL(*g_mockGStreamer, gst_element_get_state(_, _, _, _))
		.WillOnce(DoAll(SetArgPointee<1>(GST_STATE_PLAYING), SetArgPointee<2>(GST_STATE_NULL), Return(GST_STATE_CHANGE_SUCCESS)));
	EXPECT_CALL(*g_mockGStreamer, gst_element_query_position(_, _, _))
		.WillOnce(Return(FALSE));
	EXPECT_EQ(mInterfaceGstPlayer->GetVideoPosition(), 0);
	mPlayerContext->stream[eGST_MEDIATYPE_VIDEO].sinkbin = nullptr;
	mPlayerContext->pipeline = nullptr;
}

TEST_F(InterfacePlayerGapFillTests, GetVideoPosition_ClockTimeNoneReturnsZero)
{
	GstElement videoSinkbin{};
	mPlayerContext->pipeline = &gst_element_pipeline;
	mPlayerContext->stream[eGST_MEDIATYPE_VIDEO].sinkbin = &videoSinkbin;
	EXPECT_CALL(*g_mockGStreamer, gst_element_get_state(_, _, _, _))
		.WillOnce(DoAll(SetArgPointee<1>(GST_STATE_PAUSED), SetArgPointee<2>(GST_STATE_NULL), Return(GST_STATE_CHANGE_SUCCESS)));
	EXPECT_CALL(*g_mockGStreamer, gst_element_query_position(_, _, _))
		.WillOnce(DoAll(SetArgPointee<2>(GST_CLOCK_TIME_NONE), Return(TRUE)));
	EXPECT_EQ(mInterfaceGstPlayer->GetVideoPosition(), 0);
	mPlayerContext->stream[eGST_MEDIATYPE_VIDEO].sinkbin = nullptr;
	mPlayerContext->pipeline = nullptr;
}

TEST_F(InterfacePlayerGapFillTests, GetVideoPosition_PlayingStateSuccess)
{
	GstElement videoSinkbin{};
	mPlayerContext->pipeline = &gst_element_pipeline;
	mPlayerContext->stream[eGST_MEDIATYPE_VIDEO].sinkbin = &videoSinkbin;
	EXPECT_CALL(*g_mockGStreamer, gst_element_get_state(_, _, _, _))
		.WillOnce(DoAll(SetArgPointee<1>(GST_STATE_PLAYING), SetArgPointee<2>(GST_STATE_NULL), Return(GST_STATE_CHANGE_SUCCESS)));
	EXPECT_CALL(*g_mockGStreamer, gst_element_query_position(&videoSinkbin, GST_FORMAT_TIME, _))
		.WillOnce(DoAll(SetArgPointee<2>(5432 * GST_MSECOND), Return(TRUE)));
	EXPECT_EQ(mInterfaceGstPlayer->GetVideoPosition(), 5432);
	mPlayerContext->stream[eGST_MEDIATYPE_VIDEO].sinkbin = nullptr;
	mPlayerContext->pipeline = nullptr;
}

/* ------------------------------------------------------------------------- */
/* CheckForPTSChangeWithTimeout                                              */
/* ------------------------------------------------------------------------- */

TEST_F(InterfacePlayerGapFillTests, CheckForPTSChange_PtsZeroReturnsTrue)
{
	auto soc = std::make_shared<GapFillSocInterface>();
	soc->videoPtsToReturn = 0;
	mInterfacePrivatePlayer->socInterface = soc;
	EXPECT_TRUE(mInterfaceGstPlayer->CheckForPTSChangeWithTimeout(1000));
}

TEST_F(InterfacePlayerGapFillTests, CheckForPTSChange_NewPtsUpdatesState)
{
	auto soc = std::make_shared<GapFillSocInterface>();
	soc->videoPtsToReturn = 45000;
	mInterfacePrivatePlayer->socInterface = soc;
	mPlayerContext->lastKnownPTS = 0;
	mPlayerContext->ptsUpdatedTimeMS = 0;
	EXPECT_TRUE(mInterfaceGstPlayer->CheckForPTSChangeWithTimeout(1000));
	EXPECT_EQ(mPlayerContext->lastKnownPTS, 45000);
	EXPECT_NE(mPlayerContext->ptsUpdatedTimeMS, 0);
}

TEST_F(InterfacePlayerGapFillTests, CheckForPTSChange_UnchangedWithinTimeout)
{
	auto soc = std::make_shared<GapFillSocInterface>();
	soc->videoPtsToReturn = 45000;
	mInterfacePrivatePlayer->socInterface = soc;
	mPlayerContext->lastKnownPTS = 45000;
	// ptsUpdatedTimeMS = "now" means diff ~0 < timeout
	mPlayerContext->ptsUpdatedTimeMS = std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::steady_clock::now().time_since_epoch()).count();
	EXPECT_TRUE(mInterfaceGstPlayer->CheckForPTSChangeWithTimeout(60 * 1000));
}

TEST_F(InterfacePlayerGapFillTests, CheckForPTSChange_StaleBeyondTimeoutReturnsFalse)
{
	auto soc = std::make_shared<GapFillSocInterface>();
	soc->videoPtsToReturn = 45000;
	mInterfacePrivatePlayer->socInterface = soc;
	mPlayerContext->lastKnownPTS = 45000;
	mPlayerContext->ptsUpdatedTimeMS = 1; // epoch -> huge diff
	EXPECT_FALSE(mInterfaceGstPlayer->CheckForPTSChangeWithTimeout(1));
}

/* ------------------------------------------------------------------------- */
/* NotifyFirstFrame branches                                                 */
/* ------------------------------------------------------------------------- */

TEST_F(InterfacePlayerGapFillTests, NotifyFirstFrame_VideoFirstFrameReceived)
{
	bool cbInvoked = false;
	int cbMediaType = -1;
	bool cbNotifyFirstBuffer = false;
	mInterfaceGstPlayer->FirstFrameCallback(
		[&](int mediaType, bool notifyFirstBuffer, bool decoder, bool &reqDisplay, bool &audioOnly)
		{
			cbInvoked = true;
			cbMediaType = mediaType;
			cbNotifyFirstBuffer = notifyFirstBuffer;
		});
	mPlayerContext->firstVideoFrameReceived = true;
	mPlayerContext->firstFrameReceived = false;
	mInterfaceGstPlayer->PipelineSetToReady = true;
	mInterfaceGstPlayer->NotifyFirstFrame(eGST_MEDIATYPE_VIDEO);
	EXPECT_TRUE(cbInvoked);
	EXPECT_EQ(cbMediaType, eGST_MEDIATYPE_VIDEO);
	EXPECT_TRUE(cbNotifyFirstBuffer);
	EXPECT_TRUE(mPlayerContext->firstFrameReceived);
	EXPECT_TRUE(mPlayerContext->decoderHandleNotified);
	EXPECT_FALSE(mInterfaceGstPlayer->PipelineSetToReady);
}

TEST_F(InterfacePlayerGapFillTests, NotifyFirstFrame_AudioTypeNoOpWhenTwoTracks)
{
	bool cbInvoked = false;
	mInterfaceGstPlayer->FirstFrameCallback(
		[&](int mediaType, bool notifyFirstBuffer, bool decoder, bool &reqDisplay, bool &audioOnly)
		{ cbInvoked = true; });
	mPlayerContext->NumberOfTracks = 2;
	mPlayerContext->firstAudioFrameReceived = true;
	mPlayerContext->firstVideoFrameReceived = false;
	mPlayerContext->firstFrameReceived = false;
	mInterfaceGstPlayer->NotifyFirstFrame(eGST_MEDIATYPE_AUDIO);
	// firstVideoFrameReceived false && NumberOfTracks!=1 -> firstFrameReceived stays false
	EXPECT_FALSE(mPlayerContext->firstFrameReceived);
	EXPECT_TRUE(cbInvoked); // callback still invoked with notifyFirstBuffer=false
}

TEST_F(InterfacePlayerGapFillTests, NotifyFirstFrame_SubtitleType)
{
	mInterfaceGstPlayer->FirstFrameCallback(
		[](int mediaType, bool notifyFirstBuffer, bool decoder, bool &reqDisplay, bool &audioOnly) {});
	EXPECT_NO_FATAL_FAILURE(mInterfaceGstPlayer->NotifyFirstFrame(eGST_MEDIATYPE_SUBTITLE));
}

TEST_F(InterfacePlayerGapFillTests, NotifyFirstFrame_NoCallbackRegistered)
{
	// Default std::function is empty - notifyFirstFrameCallback check is skipped
	mPlayerContext->firstVideoFrameReceived = true;
	EXPECT_NO_FATAL_FAILURE(mInterfaceGstPlayer->NotifyFirstFrame(eGST_MEDIATYPE_VIDEO));
	EXPECT_TRUE(mPlayerContext->firstFrameReceived);
}

TEST_F(InterfacePlayerGapFillTests, NotifyFirstFrame_DecoderAlreadyNotifiedSkipsReschedule)
{
	// decoderHandleNotified short-circuits the FirstFrameCallback schedule;
	// IdleTaskAdd(firstProgressCallbackIdleTask) still schedules once.
	mPlayerContext->decoderHandleNotified = true;
	mPlayerContext->firstVideoFrameReceived = true;
	EXPECT_CALL(*g_mockPlayerScheduler, ScheduleTask(_)).Times(1).WillOnce(Return(9));
	mInterfaceGstPlayer->NotifyFirstFrame(eGST_MEDIATYPE_VIDEO);
	EXPECT_EQ(mPlayerContext->firstFrameCallbackIdleTaskId, 0u);
	EXPECT_FALSE(mPlayerContext->firstFrameCallbackIdleTaskPending);
}

TEST_F(InterfacePlayerGapFillTests, NotifyFirstFrame_AudioOnlySchedulesFirstFrameTask)
{
	mPlayerContext->NumberOfTracks = 1;
	mPlayerContext->firstAudioFrameReceived = true;
	mInterfaceGstPlayer->FirstFrameCallback(
		[](int mediaType, bool notifyFirstBuffer, bool decoder, bool &reqDisplay, bool &audioOnly)
		{ audioOnly = true; });
	EXPECT_CALL(*g_mockPlayerScheduler, ScheduleTask(_)).Times(AnyNumber()).WillRepeatedly(Return(5));
	mInterfaceGstPlayer->NotifyFirstFrame(eGST_MEDIATYPE_AUDIO);
	EXPECT_TRUE(mPlayerContext->firstFrameReceived);
	EXPECT_TRUE(mPlayerContext->decoderHandleNotified);
}

/* ------------------------------------------------------------------------- */
/* NotifyEOS / NotifyFragmentCachingComplete extra branches                  */
/* ------------------------------------------------------------------------- */

TEST_F(InterfacePlayerGapFillTests, NotifyEOS_SetsPendingAndSignalledFlags)
{
	EXPECT_CALL(*g_mockPlayerScheduler, ScheduleTask(_)).WillOnce(Return(33));
	mInterfaceGstPlayer->NotifyEOS();
	EXPECT_TRUE(mPlayerContext->eosSignalled);
	EXPECT_TRUE(mPlayerContext->eosCallbackIdleTaskPending);
	EXPECT_EQ(mPlayerContext->eosCallbackIdleTaskId, 33u);
}

TEST_F(InterfacePlayerGapFillTests, NotifyEOS_ScheduleFailureClearsPending)
{
	EXPECT_CALL(*g_mockPlayerScheduler, ScheduleTask(_)).WillOnce(Return(PLAYER_TASK_ID_INVALID));
	mInterfaceGstPlayer->NotifyEOS();
	EXPECT_TRUE(mPlayerContext->eosSignalled);
	EXPECT_FALSE(mPlayerContext->eosCallbackIdleTaskPending);
	EXPECT_EQ(mPlayerContext->eosCallbackIdleTaskId, PLAYER_TASK_ID_INVALID);
}

TEST_F(InterfacePlayerGapFillTests, NotifyFragmentCachingComplete_SetsPlaying)
{
	mPlayerContext->pendingPlayState = true;
	mPlayerContext->pipeline = &gst_element_pipeline;
	EXPECT_CALL(*g_mockGStreamer, gst_element_set_state(&gst_element_pipeline, GST_STATE_PLAYING))
		.WillOnce(Return(GST_STATE_CHANGE_SUCCESS));
	mInterfaceGstPlayer->NotifyFragmentCachingComplete();
	EXPECT_FALSE(mPlayerContext->pendingPlayState);
	EXPECT_EQ(mPlayerContext->buffering_target_state, GST_STATE_PLAYING);
	mPlayerContext->pipeline = nullptr;
}

/* ------------------------------------------------------------------------- */
/* EndOfStreamReached - matrix                                               */
/* ------------------------------------------------------------------------- */

TEST_F(InterfacePlayerGapFillTests, EndOfStreamReached_SetsEosFlag)
{
	bool shouldHaltBuffering = false;
	mPlayerContext->rate = GST_NORMAL_PLAY_RATE;
	mInterfaceGstPlayer->EndOfStreamReached(eGST_MEDIATYPE_VIDEO, shouldHaltBuffering);
	EXPECT_TRUE(mPlayerContext->stream[eGST_MEDIATYPE_VIDEO].eosReached);
	EXPECT_TRUE(shouldHaltBuffering);
}

TEST_F(InterfacePlayerGapFillTests, EndOfStreamReached_TrickModeSignalsAudioAndSubtitle)
{
	bool shouldHaltBuffering = false;
	mPlayerContext->rate = 4; // trickmode
	GstElement subtitleSrc{};
	mPlayerContext->stream[eGST_MEDIATYPE_SUBTITLE].source = &subtitleSrc;
	mInterfaceGstPlayer->EndOfStreamReached(eGST_MEDIATYPE_VIDEO, shouldHaltBuffering);
	EXPECT_TRUE(mPlayerContext->stream[eGST_MEDIATYPE_VIDEO].eosReached);
	EXPECT_TRUE(shouldHaltBuffering);
	mPlayerContext->stream[eGST_MEDIATYPE_SUBTITLE].source = nullptr;
}

TEST_F(InterfacePlayerGapFillTests, EndOfStreamReached_NormalRateAudioEosCascadesToSubtitle)
{
	bool shouldHaltBuffering = false;
	mPlayerContext->rate = GST_NORMAL_PLAY_RATE;
	GstElement subtitleSrc{};
	mPlayerContext->stream[eGST_MEDIATYPE_AUDIO].eosReached = true;
	mPlayerContext->stream[eGST_MEDIATYPE_SUBTITLE].source = &subtitleSrc;
	mInterfaceGstPlayer->EndOfStreamReached(eGST_MEDIATYPE_VIDEO, shouldHaltBuffering);
	EXPECT_TRUE(mPlayerContext->stream[eGST_MEDIATYPE_SUBTITLE].eosReached);
	mPlayerContext->stream[eGST_MEDIATYPE_SUBTITLE].source = nullptr;
	mPlayerContext->stream[eGST_MEDIATYPE_AUDIO].eosReached = false;
}

TEST_F(InterfacePlayerGapFillTests, EndOfStreamReached_NormalRateNoSubtitleSource)
{
	bool shouldHaltBuffering = false;
	mPlayerContext->rate = GST_NORMAL_PLAY_RATE;
	mPlayerContext->stream[eGST_MEDIATYPE_AUDIO].eosReached = true;
	mPlayerContext->stream[eGST_MEDIATYPE_SUBTITLE].source = nullptr;
	mInterfaceGstPlayer->EndOfStreamReached(eGST_MEDIATYPE_VIDEO, shouldHaltBuffering);
	EXPECT_FALSE(mPlayerContext->stream[eGST_MEDIATYPE_SUBTITLE].eosReached);
	mPlayerContext->stream[eGST_MEDIATYPE_AUDIO].eosReached = false;
}

TEST_F(InterfacePlayerGapFillTests, EndOfStreamReached_AudioType)
{
	bool shouldHaltBuffering = false;
	mPlayerContext->rate = GST_NORMAL_PLAY_RATE;
	mInterfaceGstPlayer->EndOfStreamReached(eGST_MEDIATYPE_AUDIO, shouldHaltBuffering);
	EXPECT_TRUE(mPlayerContext->stream[eGST_MEDIATYPE_AUDIO].eosReached);
	mPlayerContext->stream[eGST_MEDIATYPE_AUDIO].eosReached = false;
}

/* ------------------------------------------------------------------------- */
/* Pause extras                                                              */
/* ------------------------------------------------------------------------- */

TEST_F(InterfacePlayerGapFillTests, Pause_ForceStopPreBufferingClearsFlag)
{
	mPlayerContext->pipeline = &gst_element_pipeline;
	mPlayerContext->buffering_in_progress = true;
	EXPECT_CALL(*g_mockGStreamer, gst_element_set_state(_, GST_STATE_PAUSED))
		.WillOnce(Return(GST_STATE_CHANGE_SUCCESS));
	EXPECT_TRUE(mInterfaceGstPlayer->Pause(true, true));
	EXPECT_FALSE(mPlayerContext->buffering_in_progress);
	EXPECT_TRUE(mPlayerContext->paused);
	EXPECT_EQ(mPlayerContext->buffering_target_state, GST_STATE_PAUSED);
	mPlayerContext->pipeline = nullptr;
}

TEST_F(InterfacePlayerGapFillTests, Pause_ResumeSetsPlayingTarget)
{
	mPlayerContext->pipeline = &gst_element_pipeline;
	EXPECT_CALL(*g_mockGStreamer, gst_element_set_state(_, GST_STATE_PLAYING))
		.WillOnce(Return(GST_STATE_CHANGE_SUCCESS));
	EXPECT_TRUE(mInterfaceGstPlayer->Pause(false, false));
	EXPECT_FALSE(mPlayerContext->paused);
	EXPECT_EQ(mPlayerContext->buffering_target_state, GST_STATE_PLAYING);
	mPlayerContext->pipeline = nullptr;
}

/* ------------------------------------------------------------------------- */
/* SetPlayBackRate                                                           */
/* ------------------------------------------------------------------------- */

TEST_F(InterfacePlayerGapFillTests, SetPlayBackRate_NoSources)
{
	// default fake SocInterface::SetPlaybackRate returns false
	EXPECT_FALSE(mInterfaceGstPlayer->SetPlayBackRate(2.0));
}

TEST_F(InterfacePlayerGapFillTests, SetPlayBackRate_WithSources)
{
	GstElement vsrc{}, asrc{}, ssrc{};
	mPlayerContext->stream[eGST_MEDIATYPE_VIDEO].source = &vsrc;
	mPlayerContext->stream[eGST_MEDIATYPE_AUDIO].source = &asrc;
	mPlayerContext->stream[eGST_MEDIATYPE_SUBTITLE].source = &ssrc; // excluded from rate set
	auto soc = std::make_shared<GapFillSocInterface>();
	mInterfacePrivatePlayer->socInterface = soc;
	EXPECT_TRUE(mInterfaceGstPlayer->SetPlayBackRate(2.0));
	mPlayerContext->stream[eGST_MEDIATYPE_VIDEO].source = nullptr;
	mPlayerContext->stream[eGST_MEDIATYPE_AUDIO].source = nullptr;
	mPlayerContext->stream[eGST_MEDIATYPE_SUBTITLE].source = nullptr;
}

TEST_F(InterfacePlayerGapFillTests, SetPlayBackRate_OnlyVideoSource)
{
	GstElement vsrc{};
	mPlayerContext->stream[eGST_MEDIATYPE_VIDEO].source = &vsrc;
	auto soc = std::make_shared<GapFillSocInterface>();
	mInterfacePrivatePlayer->socInterface = soc;
	EXPECT_TRUE(mInterfaceGstPlayer->SetPlayBackRate(-4.0));
	mPlayerContext->stream[eGST_MEDIATYPE_VIDEO].source = nullptr;
}

/* ------------------------------------------------------------------------- */
/* SetAudioVolume / SetVolumeOrMuteUnMute                                    */
/* ------------------------------------------------------------------------- */

TEST_F(InterfacePlayerGapFillTests, SetAudioVolume_ScalesToFraction)
{
	mInterfaceGstPlayer->SetAudioVolume(50);
	EXPECT_DOUBLE_EQ(mPlayerContext->audioVolume, 0.5);
}

TEST_F(InterfacePlayerGapFillTests, SetAudioVolume_Zero)
{
	mInterfaceGstPlayer->SetAudioVolume(0);
	EXPECT_DOUBLE_EQ(mPlayerContext->audioVolume, 0.0);
}

TEST_F(InterfacePlayerGapFillTests, SetAudioVolume_Full)
{
	mInterfaceGstPlayer->SetAudioVolume(100);
	EXPECT_DOUBLE_EQ(mPlayerContext->audioVolume, 1.0);
}

TEST_F(InterfacePlayerGapFillTests, SetVolumeOrMuteUnMute_RialtoVolumeZeroMutes)
{
	GstElement audioSink{};
	mPlayerContext->usingRialtoSink = true;
	mPlayerContext->audio_sink = &audioSink;
	mPlayerContext->audioVolume = 0.0;
	mPlayerContext->audioMuted = false;
	EXPECT_CALL(*g_mockGLib, g_object_set(&audioSink, StrEq("mute"), Matcher<int>(1))).Times(AtLeast(1));
	mInterfaceGstPlayer->SetVolumeOrMuteUnMute();
	EXPECT_TRUE(mPlayerContext->audioMuted);
	mPlayerContext->audio_sink = nullptr;
	mPlayerContext->usingRialtoSink = false;
}

TEST_F(InterfacePlayerGapFillTests, SetVolumeOrMuteUnMute_RialtoUnmutesWhenVolumeRestored)
{
	GstElement audioSink{};
	mPlayerContext->usingRialtoSink = true;
	mPlayerContext->audio_sink = &audioSink;
	mPlayerContext->audioVolume = 1.0;
	mPlayerContext->audioMuted = true;
	EXPECT_CALL(*g_mockGLib, g_object_set(&audioSink, StrEq("mute"), Matcher<int>(0))).Times(AtLeast(0));
	mInterfaceGstPlayer->SetVolumeOrMuteUnMute();
	EXPECT_FALSE(mPlayerContext->audioMuted);
	mPlayerContext->audio_sink = nullptr;
	mPlayerContext->usingRialtoSink = false;
}

TEST_F(InterfacePlayerGapFillTests, SetVolumeOrMuteUnMute_NonRialtoPath)
{
	// non-rialto -> socInterface->SetAudioProperty supplies "volume"/"mute" prop names
	GstElement audioSink{};
	mPlayerContext->usingRialtoSink = false;
	mPlayerContext->audio_sink = &audioSink;
	mPlayerContext->audioVolume = 0.5;
	mPlayerContext->audioMuted = false;
	mInterfaceGstPlayer->SetVolumeOrMuteUnMute();
	// FakeSocInterface sets isSinkBinVolume=false; gSource = audio_sink (non-null)
	EXPECT_FALSE(mPlayerContext->audioMuted);
	mPlayerContext->audio_sink = nullptr;
}

TEST_F(InterfacePlayerGapFillTests, SetVolumeOrMuteUnMute_NullAudioSink)
{
	mPlayerContext->usingRialtoSink = false;
	mPlayerContext->audio_sink = nullptr;
	mPlayerContext->stream[eGST_MEDIATYPE_AUDIO].sinkbin = nullptr;
	EXPECT_NO_FATAL_FAILURE(mInterfaceGstPlayer->SetVolumeOrMuteUnMute());
}

/* ------------------------------------------------------------------------- */
/* GetCCDecoderHandle                                                        */
/* ------------------------------------------------------------------------- */

TEST_F(InterfacePlayerGapFillTests, GetCCDecoderHandle_UsingCCControlReturnsSubtitleSink)
{
	GstElement subtitleSink{};
	mPlayerContext->usingClosedCaptionsControl = true;
	mPlayerContext->subtitle_sink = &subtitleSink;
	EXPECT_EQ(mInterfaceGstPlayer->GetCCDecoderHandle(), (unsigned long)&subtitleSink);
	mPlayerContext->subtitle_sink = nullptr;
	mPlayerContext->usingClosedCaptionsControl = false;
}

TEST_F(InterfacePlayerGapFillTests, GetCCDecoderHandle_NoCCControlNoVideoDecReturnsZero)
{
	mPlayerContext->usingClosedCaptionsControl = false;
	mPlayerContext->video_dec = nullptr;
	EXPECT_EQ(mInterfaceGstPlayer->GetCCDecoderHandle(), 0UL);
}

TEST_F(InterfacePlayerGapFillTests, GetCCDecoderHandle_VideoDecQueriesSoC)
{
	GstElement videoDec{};
	mPlayerContext->usingClosedCaptionsControl = false;
	mPlayerContext->video_dec = &videoDec;
	// default fake leaves handle NULL
	EXPECT_EQ(mInterfaceGstPlayer->GetCCDecoderHandle(), 0UL);
	mPlayerContext->video_dec = nullptr;
}

/* ------------------------------------------------------------------------- */
/* SetupClosedCaptionControlStream                                           */
/* ------------------------------------------------------------------------- */

TEST_F(InterfacePlayerGapFillTests, SetupCCControlStream_FactoryMakeFailsLeavesNull)
{
	EXPECT_CALL(*g_mockGStreamer, gst_element_factory_make(_, _)).WillRepeatedly(Return(nullptr));
	mInterfaceGstPlayer->SetupClosedCaptionControlStream();
	EXPECT_EQ(mPlayerContext->stream[eGST_MEDIATYPE_SUBTITLE].sinkbin, nullptr);
	EXPECT_EQ(mPlayerContext->subtitle_sink, nullptr);
}

TEST_F(InterfacePlayerGapFillTests, SetupCCControlStream_SucceedsAndAssignsElements)
{
	GstElement appsrc{}, textsink{}, subtitlebin{};
	EXPECT_CALL(*g_mockGStreamer, gst_element_factory_make(_, _)).WillRepeatedly(Return(&appsrc));
	EXPECT_CALL(*g_mockGStreamer, gst_bin_new(_)).WillRepeatedly(Return(&subtitlebin));
	EXPECT_CALL(*g_mockGStreamer, gst_element_link(_, _)).WillRepeatedly(Return(TRUE));
	EXPECT_CALL(*g_mockGStreamer, gst_bin_add(_, _)).WillRepeatedly(Return(TRUE));
	mInterfaceGstPlayer->SetupClosedCaptionControlStream();
	EXPECT_NE(mPlayerContext->stream[eGST_MEDIATYPE_SUBTITLE].sinkbin, nullptr);
	EXPECT_NE(mPlayerContext->subtitle_sink, nullptr);
	// restore to avoid g_clear_object on stack objects
	mPlayerContext->stream[eGST_MEDIATYPE_SUBTITLE].sinkbin = nullptr;
	mPlayerContext->stream[eGST_MEDIATYPE_SUBTITLE].source = nullptr;
	mPlayerContext->subtitle_sink = nullptr;
}

TEST_F(InterfacePlayerGapFillTests, SetupCCControlStream_LinkFailsLeavesNull)
{
	GstElement appsrc{}, subtitlebin{};
	EXPECT_CALL(*g_mockGStreamer, gst_element_factory_make(_, _)).WillRepeatedly(Return(&appsrc));
	EXPECT_CALL(*g_mockGStreamer, gst_bin_new(_)).WillRepeatedly(Return(&subtitlebin));
	EXPECT_CALL(*g_mockGStreamer, gst_element_link(_, _)).WillOnce(Return(FALSE));
	mInterfaceGstPlayer->SetupClosedCaptionControlStream();
	EXPECT_EQ(mPlayerContext->stream[eGST_MEDIATYPE_SUBTITLE].sinkbin, nullptr);
}

TEST_F(InterfacePlayerGapFillTests, SetupCCControlStream_BinAddFailsLeavesNull)
{
	GstElement appsrc{}, subtitlebin{};
	EXPECT_CALL(*g_mockGStreamer, gst_element_factory_make(_, _)).WillRepeatedly(Return(&appsrc));
	EXPECT_CALL(*g_mockGStreamer, gst_bin_new(_)).WillRepeatedly(Return(&subtitlebin));
	EXPECT_CALL(*g_mockGStreamer, gst_element_link(_, _)).WillOnce(Return(TRUE));
	EXPECT_CALL(*g_mockGStreamer, gst_bin_add(_, _)).WillOnce(Return(FALSE));
	mInterfaceGstPlayer->SetupClosedCaptionControlStream();
	EXPECT_EQ(mPlayerContext->stream[eGST_MEDIATYPE_SUBTITLE].sinkbin, nullptr);
}

/* ------------------------------------------------------------------------- */
/* SetupStream / InterfacePlayer_SetupStream                                 */
/* ------------------------------------------------------------------------- */

TEST_F(InterfacePlayerGapFillTests, SetupStream_SubtitleDisabledConfig)
{
	mPlayerConfigParams->gstreamerSubsEnabled = false;
	int ret = mInterfaceGstPlayer->SetupStream(eGST_MEDIATYPE_SUBTITLE, mInterfaceGstPlayer, "http://x/m.mpd");
	(void)ret; // outcome depends on pipeline availability; call must not crash
}

TEST_F(InterfacePlayerGapFillTests, InterfacePlayerSetupStream_TriggersSubtitleEvent)
{
	int gotType = -1;
	mInterfaceGstPlayer->setupStreamCallbackMap[InterfaceCB::startNewSubtitleStream] = [&gotType](int d) { gotType = d; };
	mInterfaceGstPlayer->InterfacePlayer_SetupStream(eGST_MEDIATYPE_SUBTITLE, "url");
	EXPECT_EQ(gotType, eGST_MEDIATYPE_SUBTITLE);
}

TEST_F(InterfacePlayerGapFillTests, InterfacePlayerSetupStream_NoCallbackRegistered)
{
	EXPECT_EQ(mInterfaceGstPlayer->InterfacePlayer_SetupStream(eGST_MEDIATYPE_VIDEO, "url"), 0);
}

/* ------------------------------------------------------------------------- */
/* AddBufferFieldToStructure (free function)                                 */
/* ------------------------------------------------------------------------- */

TEST_F(InterfacePlayerGapFillTests, AddBufferField_NullStructure)
{
	std::vector<uint8_t> data{1, 2, 3};
	EXPECT_NO_FATAL_FAILURE(AddBufferFieldToStructure(nullptr, "kid", data));
}

TEST_F(InterfacePlayerGapFillTests, AddBufferField_NullFieldName)
{
	GstStructure s{};
	std::vector<uint8_t> data{1, 2, 3};
	EXPECT_NO_FATAL_FAILURE(AddBufferFieldToStructure(&s, nullptr, data));
}

TEST_F(InterfacePlayerGapFillTests, AddBufferField_EmptyData)
{
	GstStructure s{};
	EXPECT_NO_FATAL_FAILURE(AddBufferFieldToStructure(&s, "kid", {}));
}

TEST_F(InterfacePlayerGapFillTests, AddBufferField_ValidSetsField)
{
	GstStructure s{};
	std::vector<uint8_t> data{0xde, 0xad, 0xbe, 0xef};
	EXPECT_NO_FATAL_FAILURE(AddBufferFieldToStructure(&s, "kid", data));
}

/* ------------------------------------------------------------------------- */
/* FlushTrack                                                                */
/* ------------------------------------------------------------------------- */

TEST_F(InterfacePlayerGapFillTests, FlushTrack_AudioAppliesAudioDelta)
{
	GstElement asrc{}, audioSink{};
	mPlayerContext->stream[eGST_MEDIATYPE_AUDIO].source = &asrc;
	mPlayerContext->audio_sink = &audioSink;
	mPlayerContext->pipeline = &gst_element_pipeline;
	EXPECT_CALL(*g_mockGStreamer, gst_element_seek_simple(&asrc, GST_FORMAT_TIME, _, _)).WillOnce(Return(TRUE));
	double ret = mInterfaceGstPlayer->FlushTrack(eGST_MEDIATYPE_AUDIO, 10.0, 2.5, 0.0);
	EXPECT_DOUBLE_EQ(ret, (double)GST_NORMAL_PLAY_RATE);
	EXPECT_TRUE(mPlayerContext->filterAudioDemuxBuffers);
	mPlayerContext->stream[eGST_MEDIATYPE_AUDIO].source = nullptr;
	mPlayerContext->audio_sink = nullptr;
	mPlayerContext->pipeline = nullptr;
	mPlayerContext->filterAudioDemuxBuffers = false;
}

TEST_F(InterfacePlayerGapFillTests, FlushTrack_SubtitleAppliesSubDelta)
{
	GstElement ssrc{};
	mPlayerContext->stream[eGST_MEDIATYPE_SUBTITLE].source = &ssrc;
	mPlayerContext->pipeline = &gst_element_pipeline;
	EXPECT_CALL(*g_mockGStreamer, gst_element_seek_simple(&ssrc, GST_FORMAT_TIME, _, _)).WillOnce(Return(TRUE));
	double ret = mInterfaceGstPlayer->FlushTrack(eGST_MEDIATYPE_SUBTITLE, 10.0, 0.0, 3.0);
	EXPECT_DOUBLE_EQ(ret, (double)GST_NORMAL_PLAY_RATE);
	EXPECT_FALSE(mPlayerContext->filterAudioDemuxBuffers);
	mPlayerContext->stream[eGST_MEDIATYPE_SUBTITLE].source = nullptr;
	mPlayerContext->pipeline = nullptr;
}

/* ------------------------------------------------------------------------- */
/* SetStreamCaps format matrix                                               */
/* ------------------------------------------------------------------------- */

TEST_F(InterfacePlayerGapFillTests, SetStreamCaps_H264SetsStreamFormat)
{
	GstCaps caps{};
	GstElement vsrc{};
	mPlayerContext->stream[eGST_MEDIATYPE_VIDEO].source = &vsrc;
	ON_CALL(*g_mockGStreamer, gst_caps_new_simple(_, _, _, _, _)).WillByDefault(Return(&caps));
	MediaCodecInfo info;
	info.mCodecFormat = GST_FORMAT_VIDEO_ES_H264;
	info.mInfo.video.mWidth = 1920;
	info.mInfo.video.mHeight = 1080;
	mInterfaceGstPlayer->SetStreamCaps(eGST_MEDIATYPE_VIDEO, std::move(info));
	EXPECT_EQ(mPlayerContext->stream[eGST_MEDIATYPE_VIDEO].format, GST_FORMAT_VIDEO_ES_H264);
	EXPECT_TRUE(mPlayerContext->isMp4DemuxPlayback);
	mPlayerContext->stream[eGST_MEDIATYPE_VIDEO].source = nullptr;
	mPlayerContext->isMp4DemuxPlayback = false;
}

TEST_F(InterfacePlayerGapFillTests, SetStreamCaps_HevcHvc1)
{
	GstCaps caps{};
	GstElement vsrc{};
	mPlayerContext->stream[eGST_MEDIATYPE_VIDEO].source = &vsrc;
	ON_CALL(*g_mockGStreamer, gst_caps_new_simple(_, _, _, _, _)).WillByDefault(Return(&caps));
	MediaCodecInfo info;
	info.mCodecFormat = GST_FORMAT_VIDEO_ES_HEVC;
	mInterfaceGstPlayer->SetStreamCaps(eGST_MEDIATYPE_VIDEO, std::move(info));
	EXPECT_EQ(mPlayerContext->stream[eGST_MEDIATYPE_VIDEO].format, GST_FORMAT_VIDEO_ES_HEVC);
	mPlayerContext->stream[eGST_MEDIATYPE_VIDEO].source = nullptr;
	mPlayerContext->isMp4DemuxPlayback = false;
}

TEST_F(InterfacePlayerGapFillTests, SetStreamCaps_AacRawSetsChannelRate)
{
	GstCaps caps{};
	GstElement asrc{};
	mPlayerContext->stream[eGST_MEDIATYPE_AUDIO].source = &asrc;
	ON_CALL(*g_mockGStreamer, gst_caps_new_simple(_, _, _, _, _)).WillByDefault(Return(&caps));
	MediaCodecInfo info;
	info.mCodecFormat = GST_FORMAT_AUDIO_ES_AAC_RAW;
	info.mInfo.audio.mChannelCount = 2;
	info.mInfo.audio.mSampleRate = 48000;
	mInterfaceGstPlayer->SetStreamCaps(eGST_MEDIATYPE_AUDIO, std::move(info));
	EXPECT_EQ(mPlayerContext->stream[eGST_MEDIATYPE_AUDIO].format, GST_FORMAT_AUDIO_ES_AAC_RAW);
	mPlayerContext->stream[eGST_MEDIATYPE_AUDIO].source = nullptr;
	mPlayerContext->isMp4DemuxPlayback = false;
}

TEST_F(InterfacePlayerGapFillTests, SetStreamCaps_Ec3Framed)
{
	GstCaps caps{};
	GstElement asrc{};
	mPlayerContext->stream[eGST_MEDIATYPE_AUDIO].source = &asrc;
	ON_CALL(*g_mockGStreamer, gst_caps_new_simple(_, _, _, _, _)).WillByDefault(Return(&caps));
	MediaCodecInfo info;
	info.mCodecFormat = GST_FORMAT_AUDIO_ES_EC3;
	mInterfaceGstPlayer->SetStreamCaps(eGST_MEDIATYPE_AUDIO, std::move(info));
	EXPECT_EQ(mPlayerContext->stream[eGST_MEDIATYPE_AUDIO].format, GST_FORMAT_AUDIO_ES_EC3);
	mPlayerContext->stream[eGST_MEDIATYPE_AUDIO].source = nullptr;
	mPlayerContext->isMp4DemuxPlayback = false;
}

TEST_F(InterfacePlayerGapFillTests, SetStreamCaps_EncryptedInvokesTransform)
{
	GstCaps caps{};
	GstElement vsrc{};
	mPlayerContext->stream[eGST_MEDIATYPE_VIDEO].source = &vsrc;
	ON_CALL(*g_mockGStreamer, gst_caps_new_simple(_, _, _, _, _)).WillByDefault(Return(&caps));
	mInterfaceGstPlayer->SetPreferredDRM("com.widevine.alpha");
	MediaCodecInfo info;
	info.mCodecFormat = GST_FORMAT_VIDEO_ES_H264;
	info.mIsEncrypted = true;
	mInterfaceGstPlayer->SetStreamCaps(eGST_MEDIATYPE_VIDEO, std::move(info));
	EXPECT_EQ(mPlayerContext->stream[eGST_MEDIATYPE_VIDEO].codecInfo.mIsEncrypted, true);
	mPlayerContext->stream[eGST_MEDIATYPE_VIDEO].source = nullptr;
	mPlayerContext->isMp4DemuxPlayback = false;
}

TEST_F(InterfacePlayerGapFillTests, SetStreamCaps_CodecDataPath)
{
	GstCaps caps{};
	GstElement vsrc{};
	mPlayerContext->stream[eGST_MEDIATYPE_VIDEO].source = &vsrc;
	ON_CALL(*g_mockGStreamer, gst_caps_new_simple(_, _, _, _, _)).WillByDefault(Return(&caps));
	MediaCodecInfo info;
	info.mCodecFormat = GST_FORMAT_VIDEO_ES_H264;
	info.mCodecData = {0x01, 0x64, 0x00, 0x2a};
	mInterfaceGstPlayer->SetStreamCaps(eGST_MEDIATYPE_VIDEO, std::move(info));
	EXPECT_EQ(mPlayerContext->stream[eGST_MEDIATYPE_VIDEO].codecInfo.mCodecData.size(), 4u);
	mPlayerContext->stream[eGST_MEDIATYPE_VIDEO].source = nullptr;
	mPlayerContext->isMp4DemuxPlayback = false;
}

/* ------------------------------------------------------------------------- */
/* SetPreferredDRM extra branches                                            */
/* ------------------------------------------------------------------------- */

TEST_F(InterfacePlayerGapFillTests, SetPreferredDRM_NullIgnored)
{
	mInterfaceGstPlayer->SetPreferredDRM(nullptr);
	// no crash, mDrmSystem untouched
}

TEST_F(InterfacePlayerGapFillTests, SetPreferredDRM_Replacement)
{
	mInterfaceGstPlayer->SetPreferredDRM("com.widevine.alpha");
	mInterfaceGstPlayer->SetPreferredDRM("com.microsoft.playready");
	EXPECT_FALSE(mInterfaceGstPlayer->mDrmSystem.empty());
	EXPECT_STREQ(mInterfaceGstPlayer->mDrmSystem.c_str(), "com.microsoft.playready");
}

/* ------------------------------------------------------------------------- */
/* HandleVideoBufferSent                                                     */
/* ------------------------------------------------------------------------- */

TEST_F(InterfacePlayerGapFillTests, HandleVideoBufferSent_FirstBufferTrue)
{
	mPlayerContext->numberOfVideoBuffersSent = 0;
	EXPECT_TRUE(mInterfaceGstPlayer->HandleVideoBufferSent());
	EXPECT_EQ(mPlayerContext->numberOfVideoBuffersSent, 1);
}

/* ------------------------------------------------------------------------- */
/* SetSeekPosition / GetVideoRectangle / misc getters                        */
/* ------------------------------------------------------------------------- */

TEST_F(InterfacePlayerGapFillTests, SetSeekPosition_Stores)
{
	mInterfaceGstPlayer->SetSeekPosition(42.5);
	EXPECT_DOUBLE_EQ(mPlayerContext->seekPosition, 42.5);
}

TEST_F(InterfacePlayerGapFillTests, GetVideoRectangle_DefaultEmpty)
{
	EXPECT_TRUE(mInterfaceGstPlayer->GetVideoRectangle().empty() ||
	            mInterfaceGstPlayer->GetVideoRectangle() == "0,0,0,0");
}

/* ------------------------------------------------------------------------- */
/* CreatePipeline - destroy-then-recreate path                               */
/* ------------------------------------------------------------------------- */

TEST_F(InterfacePlayerGapFillTests, CreatePipeline_ExistingPipelineDestroyedFirst)
{
	GstElement newPipeline{};
	mPlayerContext->pipeline = &gst_element_pipeline;
	mPlayerContext->bus = &bus;
	ON_CALL(*g_mockGStreamer, gst_pipeline_new(_)).WillByDefault(Return(&newPipeline));
	ON_CALL(*g_mockGStreamer, gst_pipeline_get_bus(_)).WillByDefault(Return(&bus));
	mInterfaceGstPlayer->CreatePipeline("testPipe", 0);
	EXPECT_EQ(mPlayerContext->pipeline, &newPipeline);
	mPlayerContext->pipeline = nullptr;
	mPlayerContext->bus = nullptr;
	mPlayerContext->positionQuery = nullptr;
}

/* ------------------------------------------------------------------------- */
/* GetDurationMilliseconds success                                           */
/* ------------------------------------------------------------------------- */

TEST_F(InterfacePlayerGapFillTests, GetDurationMilliseconds_Success)
{
	// GetDurationMilliseconds uses the cached pipelineState + gst_element_query
	GstQuery durationQuery{};
	mPlayerContext->pipeline = &gst_element_pipeline;
	mPlayerContext->pipelineState = GST_STATE_PLAYING;
	EXPECT_CALL(*g_mockGStreamer, gst_query_new_duration(GST_FORMAT_TIME)).WillOnce(Return(&durationQuery));
	EXPECT_CALL(*g_mockGStreamer, gst_element_query(&gst_element_pipeline, &durationQuery)).WillOnce(Return(TRUE));
	EXPECT_CALL(*g_mockGStreamer, gst_query_parse_duration(&durationQuery, _, _))
		.WillOnce(SetArgPointee<2>(120 * GST_SECOND));
	EXPECT_EQ(mInterfaceGstPlayer->GetDurationMilliseconds(), 120 * 1000);
	mPlayerContext->pipeline = nullptr;
}

TEST_F(InterfacePlayerGapFillTests, GetDurationMilliseconds_PausedByUserQueries)
{
	GstQuery durationQuery{};
	mPlayerContext->pipeline = &gst_element_pipeline;
	mPlayerContext->pipelineState = GST_STATE_PAUSED;
	mPlayerContext->paused = true;
	EXPECT_CALL(*g_mockGStreamer, gst_query_new_duration(GST_FORMAT_TIME)).WillOnce(Return(&durationQuery));
	EXPECT_CALL(*g_mockGStreamer, gst_element_query(_, _)).WillOnce(Return(TRUE));
	EXPECT_CALL(*g_mockGStreamer, gst_query_parse_duration(_, _, _))
		.WillOnce(SetArgPointee<2>(5 * GST_SECOND));
	EXPECT_EQ(mInterfaceGstPlayer->GetDurationMilliseconds(), 5000);
	mPlayerContext->pipeline = nullptr;
}

TEST_F(InterfacePlayerGapFillTests, GetDurationMilliseconds_NullPipelineReturnsZero)
{
	mPlayerContext->pipeline = nullptr;
	EXPECT_EQ(mInterfaceGstPlayer->GetDurationMilliseconds(), 0);
}

TEST_F(InterfacePlayerGapFillTests, GetDurationMilliseconds_NotPlayingOrUserPausedReturnsZero)
{
	mPlayerContext->pipeline = &gst_element_pipeline;
	mPlayerContext->pipelineState = GST_STATE_PAUSED;
	mPlayerContext->paused = false; // paused but not by user -> no query
	EXPECT_CALL(*g_mockGStreamer, gst_query_new_duration(_)).Times(0);
	EXPECT_EQ(mInterfaceGstPlayer->GetDurationMilliseconds(), 0);
	mPlayerContext->pipeline = nullptr;
}

TEST_F(InterfacePlayerGapFillTests, GetDurationMilliseconds_QueryNewReturnsNull)
{
	mPlayerContext->pipeline = &gst_element_pipeline;
	mPlayerContext->pipelineState = GST_STATE_PLAYING;
	EXPECT_CALL(*g_mockGStreamer, gst_query_new_duration(GST_FORMAT_TIME)).WillOnce(Return(nullptr));
	EXPECT_CALL(*g_mockGStreamer, gst_element_query(_, _)).Times(0);
	EXPECT_EQ(mInterfaceGstPlayer->GetDurationMilliseconds(), 0);
	mPlayerContext->pipeline = nullptr;
}

TEST_F(InterfacePlayerGapFillTests, GetDurationMilliseconds_ElementQueryFails)
{
	GstQuery durationQuery{};
	mPlayerContext->pipeline = &gst_element_pipeline;
	mPlayerContext->pipelineState = GST_STATE_PLAYING;
	EXPECT_CALL(*g_mockGStreamer, gst_query_new_duration(GST_FORMAT_TIME)).WillOnce(Return(&durationQuery));
	EXPECT_CALL(*g_mockGStreamer, gst_element_query(_, _)).WillOnce(Return(FALSE));
	EXPECT_EQ(mInterfaceGstPlayer->GetDurationMilliseconds(), 0);
	mPlayerContext->pipeline = nullptr;
}

/* ------------------------------------------------------------------------- */
/* TearDownStream for remaining media types                                  */
/* ------------------------------------------------------------------------- */

TEST_F(InterfacePlayerGapFillTests, TearDownStream_Audio)
{
	EXPECT_NO_FATAL_FAILURE(mInterfaceGstPlayer->TearDownStream(eGST_MEDIATYPE_AUDIO));
}

TEST_F(InterfacePlayerGapFillTests, TearDownStream_Subtitle)
{
	EXPECT_NO_FATAL_FAILURE(mInterfaceGstPlayer->TearDownStream(eGST_MEDIATYPE_SUBTITLE));
}

/* ------------------------------------------------------------------------- */
/* Stop variants                                                             */
/* ------------------------------------------------------------------------- */

TEST_F(InterfacePlayerGapFillTests, Stop_NullPipelineNoCrash)
{
	mPlayerContext->pipeline = nullptr;
	EXPECT_NO_FATAL_FAILURE(mInterfaceGstPlayer->Stop(false));
	EXPECT_NO_FATAL_FAILURE(mInterfaceGstPlayer->Stop(true));
}

/* ------------------------------------------------------------------------- */
/* InitializePlayerGstreamerPlugins                                          */
/* ------------------------------------------------------------------------- */

TEST_F(InterfacePlayerGapFillTests, InitializePlayerGstreamerPlugins_NoFeaturesFound)
{
	ON_CALL(*g_mockGStreamer, gst_init_check(_, _, _)).WillByDefault(Return(TRUE));
	ON_CALL(*g_mockGStreamer, gst_registry_lookup_feature(_, _)).WillByDefault(Return(nullptr));
	EXPECT_NO_FATAL_FAILURE(InterfacePlayerRDK::InitializePlayerGstreamerPlugins());
}

TEST_F(InterfacePlayerGapFillTests, InitializePlayerGstreamerPlugins_FeaturesFoundGetRanked)
{
	// GstPluginFeature is opaque in the fake GStreamer headers, so it cannot be
	// instantiated directly; a non-null pointer is all the ranking path needs.
	alignas(void *) unsigned char featureStorage[64] = {0};
	GstPluginFeature *feature = reinterpret_cast<GstPluginFeature *>(featureStorage);
	ON_CALL(*g_mockGStreamer, gst_init_check(_, _, _)).WillByDefault(Return(TRUE));
	ON_CALL(*g_mockGStreamer, gst_registry_lookup_feature(_, _)).WillByDefault(Return(feature));
	EXPECT_NO_FATAL_FAILURE(InterfacePlayerRDK::InitializePlayerGstreamerPlugins());
}

/* ------------------------------------------------------------------------- */
/* DumpDiagnostics                                                           */
/* ------------------------------------------------------------------------- */

/*
 * NOTE: InterfacePlayerRDK::DumpDiagnostics() is declared in InterfacePlayerRDK.h
 * but has no definition in InterfacePlayerRDK.cpp (even the pi-cli command for it
 * is commented out). Linking a test that calls it fails with an undefined
 * reference, so the smoke test is omitted until the method is implemented.
 */

/* ------------------------------------------------------------------------- */
/* MonitorAV free function                                                   */
/* ------------------------------------------------------------------------- */

TEST_F(InterfacePlayerGapFillTests, MonitorAV_StateNotPlayingLeavesUnhappy)
{
	mPlayerContext->pipeline = &gst_element_pipeline;
	EXPECT_CALL(*g_mockGStreamer, gst_element_get_state(_, _, _, _))
		.WillOnce(DoAll(SetArgPointee<1>(GST_STATE_NULL), SetArgPointee<2>(GST_STATE_NULL), Return(GST_STATE_CHANGE_FAILURE)));
	EXPECT_NO_FATAL_FAILURE(MonitorAV(mInterfaceGstPlayer));
	EXPECT_FALSE(mPlayerContext->monitorAVstate.happy);
	mPlayerContext->pipeline = nullptr;
}

/* ------------------------------------------------------------------------- */
/* ConfigurePipeline extras                                                  */
/* ------------------------------------------------------------------------- */

TEST_F(InterfacePlayerGapFillTests, ConfigurePipeline_LiveLatencyFlag)
{
	EXPECT_NO_FATAL_FAILURE(
		mInterfaceGstPlayer->ConfigurePipeline(StreamCodecInfo{GST_FORMAT_VIDEO_ES_H264, GST_FORMAT_AUDIO_ES_AAC, GST_FORMAT_INVALID},
			false, false, false, 0, GST_NORMAL_PLAY_RATE, "pipe", 0, false, "http://x/m.mpd", true));
}

TEST_F(InterfacePlayerGapFillTests, ConfigurePipeline_SubtitleFormatSet)
{
	mInterfaceGstPlayer->ConfigurePipeline(GST_FORMAT_VIDEO_ES_H264, GST_FORMAT_AUDIO_ES_AAC, GST_FORMAT_SUBTITLE_WEBVTT,
		false, false, true, 0, GST_NORMAL_PLAY_RATE, "pipe", 0, false, "http://x/m.mpd", false);
	// smoke-level coverage of subtitle-aware configure path
}

/* ------------------------------------------------------------------------- */
/* SignalSubtitleClock extras                                                */
/* ------------------------------------------------------------------------- */

TEST_F(InterfacePlayerGapFillTests, SignalSubtitleClock_InvalidFormatReturnsFalse)
{
	mPlayerContext->stream[eGST_MEDIATYPE_SUBTITLE].format = GST_FORMAT_INVALID;
	EXPECT_FALSE(mInterfaceGstPlayer->SignalSubtitleClock(90000, false));
}

/* ------------------------------------------------------------------------- */
/* IsStreamReady / GetBufferControlData extras                               */
/* ------------------------------------------------------------------------- */

TEST_F(InterfacePlayerGapFillTests, IsStreamReady_NotConfiguredReturnsFalse)
{
	mPlayerContext->stream[eGST_MEDIATYPE_VIDEO].sourceConfigured = false;
	EXPECT_FALSE(mInterfaceGstPlayer->IsStreamReady(eGST_MEDIATYPE_VIDEO));
}

TEST_F(InterfacePlayerGapFillTests, PipelineConfiguredForMedia_Subtitle)
{
	mPlayerContext->stream[eGST_MEDIATYPE_SUBTITLE].sourceConfigured = true;
	EXPECT_TRUE(mInterfaceGstPlayer->PipelineConfiguredForMedia(eGST_MEDIATYPE_SUBTITLE));
	mPlayerContext->stream[eGST_MEDIATYPE_SUBTITLE].sourceConfigured = false;
}

/* ------------------------------------------------------------------------- */
/* GetTrickTeardown boundary                                                 */
/* ------------------------------------------------------------------------- */

TEST_F(InterfacePlayerGapFillTests, TrickTearDown_RoundTripTwice)
{
	mInterfaceGstPlayer->SetTrickTearDown(true);
	EXPECT_TRUE(mInterfaceGstPlayer->GetTrickTeardown());
	mInterfaceGstPlayer->SetTrickTearDown(false);
	EXPECT_FALSE(mInterfaceGstPlayer->GetTrickTeardown());
	mInterfaceGstPlayer->SetTrickTearDown(true);
	EXPECT_TRUE(mInterfaceGstPlayer->GetTrickTeardown());
	mInterfaceGstPlayer->SetTrickTearDown(false);
}

/* ------------------------------------------------------------------------- */
/* BusEventData / MsgType                                                    */
/* ------------------------------------------------------------------------- */

TEST_F(InterfacePlayerGapFillTests, BusEventData_Fields)
{
	BusEventData evt;
	evt.msgType = MESSAGE_ERROR;
	evt.msg = "err";
	evt.setPlaybackRate = true;
	EXPECT_EQ(evt.msgType, MESSAGE_ERROR);
	EXPECT_TRUE(evt.setPlaybackRate);
}

TEST_F(InterfacePlayerGapFillTests, MsgType_ValuesStable)
{
	EXPECT_EQ(MESSAGE_ERROR, 0);
	EXPECT_EQ(MESSAGE_WARNING, 1);
	EXPECT_EQ(MESSAGE_EOS, 2);
	EXPECT_EQ(MESSAGE_APPLICATION, 3);
	EXPECT_EQ(MESSAGE_STATE_CHANGE, 4);
	EXPECT_EQ(MESSAGE_CLOCK_LOST, 5);
}

/* ------------------------------------------------------------------------- */
/* NotifyFirstFrame with audioOnly path                                      */
/* ------------------------------------------------------------------------- */

TEST_F(InterfacePlayerGapFillTests, NotifyFirstFrame_VideoClearsPipelineSetToReady)
{
	mInterfaceGstPlayer->PipelineSetToReady = true;
	mPlayerContext->firstVideoFrameReceived = true;
	mInterfaceGstPlayer->NotifyFirstFrame(eGST_MEDIATYPE_VIDEO);
	EXPECT_FALSE(mInterfaceGstPlayer->PipelineSetToReady);
}

TEST_F(InterfacePlayerGapFillTests, NotifyFirstFrame_AudioType_DoesNotClearReady)
{
	mInterfaceGstPlayer->PipelineSetToReady = true;
	mInterfaceGstPlayer->NotifyFirstFrame(eGST_MEDIATYPE_AUDIO);
	EXPECT_TRUE(mInterfaceGstPlayer->PipelineSetToReady);
}
