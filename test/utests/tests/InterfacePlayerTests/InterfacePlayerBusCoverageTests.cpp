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
 * @file InterfacePlayerBusCoverageTests.cpp
 * @brief Coverage for the bus_message (async bus watch) and bus_sync_handler
 *        (sync bus handler) paths in InterfacePlayerRDK.cpp. The handlers are
 *        captured from the gst_bus_add_watch and gst_bus_set_sync_handler mocks
 *        and invoked with synthetic GstMessage objects.
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <cstring>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include "InterfacePlayerRDK.h"
#include "InterfacePlayerPriv.h"
#include "PlayerLogManager.h"
#include "MockGStreamer.h"
#include "MockGLib.h"
#include "MockPlayerScheduler.h"
#include "MockGstHandlerControl.h"

using ::testing::NiceMock;
using ::testing::Return;
using ::testing::StrEq;
using ::testing::Invoke;
using ::testing::DoAll;
using ::testing::SaveArg;
using ::testing::_;

namespace
{
/**
 * @brief SocInterface with switchable hooks used by the bus handlers.
 */
class BusCoverageSocInterface : public SocInterface
{
public:
	bool audioOnly = false;
	bool playbackRate = false;
	bool notifyVideoFirstFrame = false;
	bool simulatorFirstFrame = false;
	bool hasFirstAudioCb = true;
	GstPad *sourcePad = nullptr;
	bool handleErrors = false;
	bool configureAudioSinkResult = false;
	int configureAudioSinkCalls = 0;
	int discoverDecoderCalls = 0;
	int discoverSinkCalls = 0;
	int decodeErrorCalls = 0;
	int freerunCalls = 0;
	int ac4Calls = 0;
	int lastAc4TrackId = -1;

	bool AudioOnlyMode(GstElement *sinkbin) override { return audioOnly; }
	GstPad *GetSourcePad(GstElement *element) override { return sourcePad; }
	void SetAC4Tracks(GstElement *src, int trackId) override
	{
		ac4Calls++;
		lastAc4TrackId = trackId;
	}
	bool SetPlatformPlaybackRate() override { return playbackRate; }
	bool NotifyVideoFirstFrame() override { return notifyVideoFirstFrame; }
	bool IsSimulatorFirstFrame() override { return simulatorFirstFrame; }
	void DiscoverVideoDecoderProperties(GstElement *element) override { discoverDecoderCalls++; }
	void DiscoverVideoSinkProperties(GstElement *element) override { discoverSinkCalls++; }
	void SetDecodeError(GstObject *src) override { decodeErrorCalls++; }
	void SetFreerunThreshold(GstObject *src) override { freerunCalls++; }
	bool HasFirstAudioFrameCallback() override { return hasFirstAudioCb; }

	void SetAudioProperty(const char *&volume, const char *&mute, bool &isSinkBinVolume) override
	{
		volume = "volume";
		mute = "mute";
		isSinkBinVolume = false;
	}
	bool SetPlaybackRate(const std::vector<GstElement *> &sources, GstElement *pipeline, double rate, GstElement *video_dec, GstElement *audio_dec) override
	{
		return true;
	}
	bool SetRateCorrection() override { return false; }
	bool IsVideoSink(const char *name) override
	{
		return name && strcmp(name, "videosink") == 0;
	}
	bool IsAudioSinkOrAudioDecoder(const char *name) override
	{
		return name && (strcmp(name, "audiosink") == 0 || strcmp(name, "audiodec") == 0);
	}
	bool IsVideoDecoder(const char *name) override
	{
		return name && strcmp(name, "videodec") == 0;
	}
	bool ConfigureAudioSink(GstElement **audio_sink, GstObject *src, bool decStreamSync) override
	{
		configureAudioSinkCalls++;
		return configureAudioSinkResult;
	}
	bool IsAudioOrVideoDecoder(const char *name) override
	{
		return name && (strcmp(name, "videodec") == 0 || strcmp(name, "audiodec") == 0);
	}
	void GetCCDecoderHandle(gpointer *dec_handle, GstElement *video_dec) override {}
	void SetPlaybackFlags(gint &flags, bool isSub) override { flags = 0; }
	bool IsVideoMaster(GstElement *videoSink) override { return true; }
	bool IsVideoSinkHandleErrors() override { return handleErrors; }
	long long GetVideoPts(GstElement *video_sink, GstElement *video_dec, bool isWesteros) override
	{
		return 0;
	}
};
} // namespace

class InterfacePlayerBusCoverageTests : public ::testing::Test
{
protected:
	InterfacePlayerRDK *mInterfaceGstPlayer = nullptr;
	InterfacePlayerPriv *mInterfacePrivatePlayer = nullptr;
	GstPlayerPriv *mPlayerContext = nullptr;
	std::shared_ptr<BusCoverageSocInterface> mSoc;

	GstElement mPipeline = {.object = {.name = (gchar *)"testPipeline"}};
	GstBus mBus = {.object = {.name = (gchar *)"testBus"}};
	GstTaskPool mTaskPool = {.object = {.name = (gchar *)"testTaskPool"}};
	GstQuery mQuery = {};
	GstBusFunc mBusWatch = nullptr;
	GstBusSyncHandler mSyncHandler = nullptr;

	GstElement mVideoSink = {.object = {.name = (gchar *)"videosink"}};
	GstElement mVideoDec = {.object = {.name = (gchar *)"videodec"}};
	GstElement mAudioDec = {.object = {.name = (gchar *)"audiodec"}};
	GstElement mAudioSink = {.object = {.name = (gchar *)"audiosink"}};
	GstElement mSubtitleSink = {.object = {.name = (gchar *)"subtitlesink"}};
	GstPad mSourcePad = {};

	std::map<std::string, GCallback> mSignalHandlers;
	GstState mOldState = GST_STATE_NULL;
	GstState mNewState = GST_STATE_NULL;

	int mBusEventCount = 0;
	MsgType mLastMsgType = MESSAGE_ERROR;
	std::string mLastMsg;
	std::string mLastDbgInfo;
	bool mLastSetPlaybackRate = false;
	bool mLastFirstBufferProcessed = false;
	bool mLastReceivedFirstFrame = false;
	std::vector<int> mFirstFrameTypes;

	void SetUp() override
	{
		g_mockGStreamer = new NiceMock<MockGStreamer>();
		g_mockGLib = new NiceMock<MockGLib>();
		g_mockPlayerScheduler = new NiceMock<MockPlayerScheduler>();
		// Handler gate: the fake GstHandlerControl reports disabled without a mock, which makes bus/signal handlers return early.
		g_mockGstHandlerControl = new NiceMock<MockGstHandlerControl>();
		ON_CALL(*g_mockGstHandlerControl, isEnabled()).WillByDefault(Return(true));

		ON_CALL(*g_mockGStreamer, gst_message_parse_state_changed(_, _, _, _))
			.WillByDefault(Invoke([this](GstMessage *, GstState *oldState, GstState *newState, GstState *pending) {
				*oldState = mOldState;
				*newState = mNewState;
				*pending = GST_STATE_VOID_PENDING;
			}));

		ON_CALL(*g_mockGLib, g_signal_connect_data(_, _, _, _, _, _))
			.WillByDefault(Invoke([this](gpointer, const gchar *name, GCallback cb, gpointer, GClosureNotify, GConnectFlags) -> gulong {
				mSignalHandlers[name] = cb;
				return 1;
			}));

		mInterfaceGstPlayer = new InterfacePlayerRDK();
		mInterfacePrivatePlayer = mInterfaceGstPlayer->GetPrivatePlayer();
		mPlayerContext = mInterfacePrivatePlayer->gstPrivateContext;

		mSoc = std::make_shared<BusCoverageSocInterface>();
		mInterfacePrivatePlayer->socInterface = mSoc;

		//init callbacks to avoid bad_function_call error
		mInterfaceGstPlayer->TearDownCallback([](bool, int) {});
		mInterfaceGstPlayer->StopCallback([](bool) {});
		mInterfaceGstPlayer->RegisterBusEvent([this](const BusEventData ev) {
			mBusEventCount++;
			mLastMsgType = ev.msgType;
			mLastMsg = ev.msg;
			mLastDbgInfo = ev.dbg_info;
			mLastSetPlaybackRate = ev.setPlaybackRate;
			mLastFirstBufferProcessed = ev.firstBufferProcessed;
			mLastReceivedFirstFrame = ev.receivedFirstFrame;
		});
		mInterfaceGstPlayer->FirstFrameCallback([this](int mediaType, bool, bool, bool &, bool &) {
			mFirstFrameTypes.push_back(mediaType);
		});

		PlayerLogManager::lockLogLevel(false);
		PlayerLogManager::disableLogRedirection = true; //required for mwlog output in utest
		PlayerLogManager::setLogLevel(mLOGLEVEL_TRACE);
	}

	void TearDown() override
	{
		delete mInterfaceGstPlayer;
		mInterfaceGstPlayer = nullptr;
		mPlayerContext = nullptr;

		delete g_mockGstHandlerControl;
		g_mockGstHandlerControl = nullptr;

		delete g_mockPlayerScheduler;
		g_mockPlayerScheduler = nullptr;

		delete g_mockGLib;
		g_mockGLib = nullptr;

		delete g_mockGStreamer;
		g_mockGStreamer = nullptr;
	}

	/* Creates the pipeline so that the bus watch and sync handler are captured. */
	void CreatePipelineForBus()
	{
		mPlayerContext->task_pool = &mTaskPool;
		EXPECT_CALL(*g_mockGStreamer, gst_pipeline_new(StrEq("testPipeline")))
			.WillOnce(Return(&mPipeline));
		EXPECT_CALL(*g_mockGStreamer, gst_pipeline_get_bus(GST_PIPELINE(&mPipeline)))
			.WillOnce(Return(&mBus));
		EXPECT_CALL(*g_mockGStreamer, gst_bus_add_watch(&mBus, _, _))
			.WillOnce(DoAll(SaveArg<1>(&mBusWatch), Return(1)));
		EXPECT_CALL(*g_mockGStreamer, gst_bus_set_sync_handler(&mBus, _, _, nullptr))
			.WillOnce(SaveArg<1>(&mSyncHandler));
		EXPECT_CALL(*g_mockGStreamer, gst_query_new_position(GST_FORMAT_TIME))
			.WillOnce(Return(&mQuery));

		ASSERT_TRUE(mInterfaceGstPlayer->CreatePipeline("testPipeline", 1));
		ASSERT_NE(mBusWatch, nullptr);
		ASSERT_NE(mSyncHandler, nullptr);
	}

	/* Delivers a STATE_CHANGED message through the captured async bus watch. */
	void SendBusStateChange(GstElement *src, GstState oldState, GstState newState)
	{
		mOldState = oldState;
		mNewState = newState;
		GstMessage msg = {};
		msg.type = GST_MESSAGE_STATE_CHANGED;
		msg.src = reinterpret_cast<GstObject *>(src);
		mBusWatch(&mBus, &msg, mInterfaceGstPlayer);
	}

	/* Delivers a STATE_CHANGED message through the captured sync handler. */
	GstBusSyncReply SendSyncStateChange(GstElement *src, GstState oldState, GstState newState)
	{
		mOldState = oldState;
		mNewState = newState;
		GstMessage msg = {};
		msg.type = GST_MESSAGE_STATE_CHANGED;
		msg.src = reinterpret_cast<GstObject *>(src);
		return mSyncHandler(&mBus, &msg, mInterfaceGstPlayer);
	}

	/* Delivers a non-state message through the async bus watch. */
	void SendBusMessage(GstMessageType type, GstObject *src)
	{
		GstMessage msg = {};
		msg.type = type;
		msg.src = src;
		mBusWatch(&mBus, &msg, mInterfaceGstPlayer);
	}

	/* Delivers a non-state message through the sync handler. */
	GstBusSyncReply SendSyncMessage(GstMessageType type, GstObject *src)
	{
		GstMessage msg = {};
		msg.type = type;
		msg.src = src;
		return mSyncHandler(&mBus, &msg, mInterfaceGstPlayer);
	}
};

// ---------------------------------------------------------------------------
// bus_message (async bus watch)
// ---------------------------------------------------------------------------

TEST_F(InterfacePlayerBusCoverageTests, BusMessage_StateChangeFromPipeline_ReportsStateChangeEvent)
{
	CreatePipelineForBus();
	mBusEventCount = 0;

	SendBusStateChange(&mPipeline, GST_STATE_READY, GST_STATE_READY);

	EXPECT_EQ(mBusEventCount, 1);
	EXPECT_EQ(mLastMsgType, MESSAGE_STATE_CHANGE);
	EXPECT_EQ(mLastMsg, "testPipeline");
	EXPECT_FALSE(mLastSetPlaybackRate);
}

TEST_F(InterfacePlayerBusCoverageTests, BusMessage_PlayingWithPlatformPlaybackRate_SetsPlaybackRateFlag)
{
	CreatePipelineForBus();
	mSoc->playbackRate = true;
	mBusEventCount = 0;

	SendBusStateChange(&mPipeline, GST_STATE_PAUSED, GST_STATE_PLAYING);

	EXPECT_EQ(mBusEventCount, 1);
	EXPECT_TRUE(mLastSetPlaybackRate);
}

TEST_F(InterfacePlayerBusCoverageTests, BusMessage_PausedOnStartWithFrameStepSupport_SendsStepAndNotifiesVideo)
{
	CreatePipelineForBus();
	GParamSpec paramSpec = {};
	ON_CALL(*g_mockGLib, g_object_class_find_property(_, _)).WillByDefault(Return(&paramSpec));
	mPlayerContext->video_sink = &mVideoSink;
	mPlayerContext->pauseOnStartPlayback = true;
	mPlayerContext->usingRialtoSink = true;

	EXPECT_CALL(*g_mockGStreamer, gst_element_send_event(_, _)).WillOnce(Return(TRUE));

	SendBusStateChange(&mPipeline, GST_STATE_READY, GST_STATE_PAUSED);

	EXPECT_FALSE(mPlayerContext->pauseOnStartPlayback);
	EXPECT_TRUE(mPlayerContext->firstVideoFrameReceived);
	ASSERT_EQ(mFirstFrameTypes.size(), 1u);
	EXPECT_EQ(mFirstFrameTypes[0], eGST_MEDIATYPE_VIDEO);
}

TEST_F(InterfacePlayerBusCoverageTests, BusMessage_PausedOnStartWithoutFrameStepSupport_NotifiesVideoWithoutStep)
{
	CreatePipelineForBus();
	ON_CALL(*g_mockGLib, g_object_class_find_property(_, _)).WillByDefault(Return(nullptr));
	mPlayerContext->pauseOnStartPlayback = true;
	mPlayerContext->usingRialtoSink = true;

	EXPECT_CALL(*g_mockGStreamer, gst_element_send_event(_, _)).Times(0);

	SendBusStateChange(&mPipeline, GST_STATE_READY, GST_STATE_PAUSED);

	EXPECT_FALSE(mPlayerContext->pauseOnStartPlayback);
	ASSERT_EQ(mFirstFrameTypes.size(), 1u);
	EXPECT_EQ(mFirstFrameTypes[0], eGST_MEDIATYPE_VIDEO);
}

TEST_F(InterfacePlayerBusCoverageTests, BusMessage_PlayingAudioOnlyMode_NotifiesAudioFirstFrame)
{
	CreatePipelineForBus();
	mInterfaceGstPlayer->m_gstConfigParam->audioOnlyMode = true;
	mSoc->audioOnly = true;
	mPlayerContext->NumberOfTracks = 1;
	mPlayerContext->firstAudioFrameReceived = false;

	SendBusStateChange(&mPipeline, GST_STATE_PAUSED, GST_STATE_PLAYING);

	EXPECT_TRUE(mPlayerContext->firstAudioFrameReceived);
	ASSERT_EQ(mFirstFrameTypes.size(), 1u);
	EXPECT_EQ(mFirstFrameTypes[0], eGST_MEDIATYPE_AUDIO);
}

TEST_F(InterfacePlayerBusCoverageTests, BusMessage_PlayingProgressive_MarksFirstBufferProcessed)
{
	CreatePipelineForBus();
	mInterfaceGstPlayer->m_gstConfigParam->media = eGST_MEDIAFORMAT_PROGRESSIVE;
	mPlayerContext->stream[eGST_MEDIATYPE_VIDEO].firstBufferProcessed = false;
	mBusEventCount = 0;

	SendBusStateChange(&mPipeline, GST_STATE_PAUSED, GST_STATE_PLAYING);

	EXPECT_EQ(mBusEventCount, 1);
	EXPECT_TRUE(mLastFirstBufferProcessed);
}

TEST_F(InterfacePlayerBusCoverageTests, BusMessage_PlayingRialtoSink_MarksVideoAndAudioFirstFrames)
{
	CreatePipelineForBus();
	mPlayerContext->usingRialtoSink = true;
	mPlayerContext->firstVideoFrameReceived = false;
	mPlayerContext->firstAudioFrameReceived = false;

	SendBusStateChange(&mPipeline, GST_STATE_PAUSED, GST_STATE_PLAYING);

	EXPECT_TRUE(mPlayerContext->firstVideoFrameReceived);
	EXPECT_TRUE(mPlayerContext->firstAudioFrameReceived);
}

TEST_F(InterfacePlayerBusCoverageTests, BusMessage_PlayingWesterosFirstTune_ClearsFlagAndNotifiesVideo)
{
	CreatePipelineForBus();
	mPlayerContext->usingRialtoSink = false;
	mPlayerContext->firstTuneWithWesterosSinkOff = true;
	mSoc->notifyVideoFirstFrame = true;

	SendBusStateChange(&mPipeline, GST_STATE_PAUSED, GST_STATE_PLAYING);

	EXPECT_FALSE(mPlayerContext->firstTuneWithWesterosSinkOff);
	EXPECT_TRUE(mPlayerContext->firstVideoFrameReceived);
}

TEST_F(InterfacePlayerBusCoverageTests, BusMessage_PlayingSimulatorFirstFrame_ReportsReceivedFirstFrame)
{
	CreatePipelineForBus();
	mSoc->simulatorFirstFrame = true;
	mPlayerContext->firstFrameReceived = false;
	mBusEventCount = 0;

	SendBusStateChange(&mPipeline, GST_STATE_PAUSED, GST_STATE_PLAYING);

	EXPECT_EQ(mBusEventCount, 1);
	EXPECT_TRUE(mLastReceivedFirstFrame);
}

TEST_F(InterfacePlayerBusCoverageTests, BusMessage_PlayingWithGstLoggingAndFirstFrameRequired_CompletesStateChange)
{
	CreatePipelineForBus();
	mInterfaceGstPlayer->m_gstConfigParam->gstLogging = true;
	mInterfaceGstPlayer->mFirstFrameRequired = true;
	mBusEventCount = 0;

	SendBusStateChange(&mPipeline, GST_STATE_PAUSED, GST_STATE_PLAYING);

	EXPECT_EQ(mBusEventCount, 1);
}

TEST_F(InterfacePlayerBusCoverageTests, BusMessage_PlayingWithSubtitlePending_UnmutesSubtitleSink)
{
	CreatePipelineForBus();
	mPlayerContext->subtitle_sink = &mSubtitleSink;
	mPlayerContext->setSubtitlePending = true;
	mPlayerContext->subtitleMuted = false;

	SendBusStateChange(&mPipeline, GST_STATE_PAUSED, GST_STATE_PLAYING);

	EXPECT_FALSE(mPlayerContext->setSubtitlePending);
}

TEST_F(InterfacePlayerBusCoverageTests, BusMessage_SourceElementNullToReady_CompletesSourcePadPath)
{
	// The fake gst_pad_add_probe returns 0 without calling the mock, so this test
	// asserts only that the source-element path completes and reports the event.
	CreatePipelineForBus();
	mSoc->sourcePad = &mSourcePad;
	mBusEventCount = 0;

	GstElement source = {.object = {.name = (gchar *)"sourceElement"}};
	SendBusStateChange(&source, GST_STATE_NULL, GST_STATE_READY);

	EXPECT_EQ(mBusEventCount, 1);
}

TEST_F(InterfacePlayerBusCoverageTests, BusMessage_RialtoMseVideoSinkNullToReady_ConnectsUnderflowCallback)
{
	CreatePipelineForBus();
	mPlayerContext->usingRialtoSink = false;
	mSoc->handleErrors = false;

	GstElement sink = {.object = {.name = (gchar *)"rialtomsevideosink"}};
	SendBusStateChange(&sink, GST_STATE_NULL, GST_STATE_READY);

	EXPECT_EQ(mSignalHandlers.count("buffer-underflow-callback"), 1u);
}

TEST_F(InterfacePlayerBusCoverageTests, BusMessage_VideoDecoderNullToReadyWithoutHandleErrors_ConnectsDecodeErrorCallback)
{
	CreatePipelineForBus();
	mSoc->handleErrors = false;
	mPlayerContext->usingRialtoSink = false;

	SendBusStateChange(&mVideoDec, GST_STATE_NULL, GST_STATE_READY);

	EXPECT_EQ(mSignalHandlers.count("decode-error-callback"), 1u);
	EXPECT_EQ(mSignalHandlers.count("buffer-underflow-callback"), 1u);
}

TEST_F(InterfacePlayerBusCoverageTests, BusMessage_ClockLostNonDash_ResetsPipelineToPausedAndPlaying)
{
	CreatePipelineForBus();
	mInterfaceGstPlayer->m_gstConfigParam->media = eGST_MEDIAFORMAT_PROGRESSIVE;

	EXPECT_CALL(*g_mockGStreamer, gst_element_set_state(_, GST_STATE_PAUSED)).WillOnce(Return(GST_STATE_CHANGE_SUCCESS));
	EXPECT_CALL(*g_mockGStreamer, gst_element_set_state(_, GST_STATE_PLAYING)).WillOnce(Return(GST_STATE_CHANGE_SUCCESS));

	SendBusMessage(GST_MESSAGE_CLOCK_LOST, GST_OBJECT(&mPipeline));
}

TEST_F(InterfacePlayerBusCoverageTests, BusMessage_ClockLostDash_SkipsPipelineStateReset)
{
	CreatePipelineForBus();
	mInterfaceGstPlayer->m_gstConfigParam->media = eGST_MEDIAFORMAT_DASH;

	EXPECT_CALL(*g_mockGStreamer, gst_element_set_state(_, _)).Times(0);

	SendBusMessage(GST_MESSAGE_CLOCK_LOST, GST_OBJECT(&mPipeline));
}

TEST_F(InterfacePlayerBusCoverageTests, BusMessage_InformationalMessageTypes_DoNotReportEvents)
{
	CreatePipelineForBus();
	mBusEventCount = 0;
	GstElement elem = {.object = {.name = (gchar *)"someElement"}};

	SendBusMessage(GST_MESSAGE_QOS, GST_OBJECT(&elem));
	SendBusMessage(GST_MESSAGE_RESET_TIME, GST_OBJECT(&elem));
	SendBusMessage(GST_MESSAGE_NEW_CLOCK, GST_OBJECT(&elem));
	SendBusMessage(GST_MESSAGE_TAG, GST_OBJECT(&elem));
	SendBusMessage(GST_MESSAGE_DURATION, GST_OBJECT(&elem));
	SendBusMessage(GST_MESSAGE_LATENCY, GST_OBJECT(&elem));
	SendBusMessage(GST_MESSAGE_ELEMENT, GST_OBJECT(&elem));
	SendBusMessage(GST_MESSAGE_STREAM_STATUS, GST_OBJECT(&elem));

	EXPECT_EQ(mBusEventCount, 0);
}

TEST_F(InterfacePlayerBusCoverageTests, BusMessage_Application_ReportsApplicationEvent)
{
	CreatePipelineForBus();
	mBusEventCount = 0;

	SendBusMessage(GST_MESSAGE_APPLICATION, GST_OBJECT(&mPipeline));

	EXPECT_EQ(mBusEventCount, 1);
	EXPECT_EQ(mLastMsgType, MESSAGE_APPLICATION);
}

TEST_F(InterfacePlayerBusCoverageTests, BusMessage_UnsupportedMessageType_ReturnsTrue)
{
	CreatePipelineForBus();

	GstMessage msg = {};
	msg.type = GST_MESSAGE_BUFFERING;
	msg.src = GST_OBJECT(&mPipeline);

	EXPECT_EQ(mBusWatch(&mBus, &msg, mInterfaceGstPlayer), TRUE);
}

// ---------------------------------------------------------------------------
// bus_sync_handler (synchronous bus handler)
// ---------------------------------------------------------------------------

TEST_F(InterfacePlayerBusCoverageTests, SyncHandler_PipelineStateChange_UpdatesPipelineState)
{
	CreatePipelineForBus();

	EXPECT_EQ(SendSyncStateChange(&mPipeline, GST_STATE_READY, GST_STATE_PLAYING), GST_BUS_PASS);
	EXPECT_EQ(mPlayerContext->pipelineState, GST_STATE_PLAYING);
}

TEST_F(InterfacePlayerBusCoverageTests, SyncHandler_StateChangeWithGstLogging_ConfiguresAudioSinkForOtherElements)
{
	CreatePipelineForBus();
	mInterfaceGstPlayer->m_gstConfigParam->gstLogging = true;
	GstElement other = {.object = {.name = (gchar *)"sourceElement"}};

	EXPECT_EQ(SendSyncStateChange(&other, GST_STATE_READY, GST_STATE_PAUSED), GST_BUS_PASS);
	EXPECT_EQ(mSoc->configureAudioSinkCalls, 1);
}

TEST_F(InterfacePlayerBusCoverageTests, SyncHandler_VideoSinkPausedFromReadyRialto_DiscoversSinkAndSetsRectangle)
{
	CreatePipelineForBus();
	mPlayerContext->usingRialtoSink = true;
	mInterfaceGstPlayer->m_gstConfigParam->enableRectPropertyCfg = true;

	EXPECT_EQ(SendSyncStateChange(&mVideoSink, GST_STATE_READY, GST_STATE_PAUSED), GST_BUS_PASS);
	EXPECT_EQ(mSoc->discoverSinkCalls, 1);
}

TEST_F(InterfacePlayerBusCoverageTests, SyncHandler_VideoSinkPausedFromReadyWesteros_DiscoversSinkAndSetsZoom)
{
	CreatePipelineForBus();
	mPlayerContext->usingRialtoSink = false;
	mPlayerContext->using_westerossink = true;
	mInterfaceGstPlayer->m_gstConfigParam->enableRectPropertyCfg = false;

	EXPECT_EQ(SendSyncStateChange(&mVideoSink, GST_STATE_READY, GST_STATE_PAUSED), GST_BUS_PASS);
	EXPECT_EQ(mSoc->discoverSinkCalls, 1);
}

TEST_F(InterfacePlayerBusCoverageTests, SyncHandler_VideoSinkPausedFromReadyDefault_DiscoversSinkAndSetsRectangle)
{
	CreatePipelineForBus();
	mPlayerContext->usingRialtoSink = false;
	mPlayerContext->using_westerossink = false;
	mInterfaceGstPlayer->m_gstConfigParam->enableRectPropertyCfg = false;

	EXPECT_EQ(SendSyncStateChange(&mVideoSink, GST_STATE_READY, GST_STATE_PAUSED), GST_BUS_PASS);
	EXPECT_EQ(mSoc->discoverSinkCalls, 1);
}

TEST_F(InterfacePlayerBusCoverageTests, SyncHandler_RialtoMseAudioSinkPausedFromReady_SkipsConfigureAudioSink)
{
	CreatePipelineForBus();
	GstElement audio = {.object = {.name = (gchar *)"rialtomseaudiosink"}};

	EXPECT_EQ(SendSyncStateChange(&audio, GST_STATE_READY, GST_STATE_PAUSED), GST_BUS_PASS);
	EXPECT_EQ(mSoc->configureAudioSinkCalls, 0);
}

TEST_F(InterfacePlayerBusCoverageTests, SyncHandler_AudioSinkPausedFromReadyConfigured_ConfiguresAudioSink)
{
	CreatePipelineForBus();
	mSoc->configureAudioSinkResult = true;

	EXPECT_EQ(SendSyncStateChange(&mAudioSink, GST_STATE_READY, GST_STATE_PAUSED), GST_BUS_PASS);
	EXPECT_EQ(mSoc->configureAudioSinkCalls, 1);
}

TEST_F(InterfacePlayerBusCoverageTests, SyncHandler_VideoDecoderNullToReady_DiscoversDecoderAndConnectsFirstFrame)
{
	CreatePipelineForBus();

	EXPECT_EQ(SendSyncStateChange(&mVideoDec, GST_STATE_NULL, GST_STATE_READY), GST_BUS_PASS);
	EXPECT_EQ(mSoc->discoverDecoderCalls, 1);
	EXPECT_EQ(mSoc->decodeErrorCalls, 1);
	EXPECT_EQ(mSignalHandlers.count("first-video-frame-callback"), 1u);
}

TEST_F(InterfacePlayerBusCoverageTests, SyncHandler_AudioDecoderNullToReadyWithAc4Track_ConnectsFirstAudioFrameAndSetsTrack)
{
	CreatePipelineForBus();
	mSoc->hasFirstAudioCb = true;
	mPlayerContext->stream[eGST_MEDIATYPE_AUDIO].trackId = 3;

	EXPECT_EQ(SendSyncStateChange(&mAudioDec, GST_STATE_NULL, GST_STATE_READY), GST_BUS_PASS);
	EXPECT_EQ(mSignalHandlers.count("first-audio-frame-callback"), 1u);
	EXPECT_EQ(mSoc->ac4Calls, 1);
	EXPECT_EQ(mSoc->lastAc4TrackId, 3);
}

TEST_F(InterfacePlayerBusCoverageTests, SyncHandler_AudioDecoderNullToReadyNoFirstFrameCbNoTrack_SkipsSignalAndAc4)
{
	CreatePipelineForBus();
	mSoc->hasFirstAudioCb = false;
	mPlayerContext->stream[eGST_MEDIATYPE_AUDIO].trackId = -1;

	EXPECT_EQ(SendSyncStateChange(&mAudioDec, GST_STATE_NULL, GST_STATE_READY), GST_BUS_PASS);
	EXPECT_EQ(mSignalHandlers.count("first-audio-frame-callback"), 0u);
	EXPECT_EQ(mSoc->ac4Calls, 0);
}

TEST_F(InterfacePlayerBusCoverageTests, SyncHandler_RtkAudioSinkNullToReadyWithoutFirstFrameCb_ConnectsFirstAudioFrame)
{
	CreatePipelineForBus();
	mSoc->hasFirstAudioCb = false;
	GstElement sink = {.object = {.name = (gchar *)"rtkaudiosink"}};

	EXPECT_EQ(SendSyncStateChange(&sink, GST_STATE_NULL, GST_STATE_READY), GST_BUS_PASS);
	EXPECT_EQ(mSignalHandlers.count("first-audio-frame"), 1u);
}

TEST_F(InterfacePlayerBusCoverageTests, SyncHandler_VideoSinkNullToReadyWithSeiTimecode_ConnectsTimecodeAndFreerun)
{
	CreatePipelineForBus();
	mPlayerContext->usingRialtoSink = false;
	mPlayerContext->enableSEITimeCode = true;

	EXPECT_EQ(SendSyncStateChange(&mVideoSink, GST_STATE_NULL, GST_STATE_READY), GST_BUS_PASS);
	EXPECT_EQ(mSignalHandlers.count("timecode-callback"), 1u);
	EXPECT_EQ(mSoc->freerunCalls, 1);
}

TEST_F(InterfacePlayerBusCoverageTests, SyncHandler_VideoSinkNullToReadyRialto_SkipsFreerunThreshold)
{
	CreatePipelineForBus();
	mPlayerContext->usingRialtoSink = true;
	mPlayerContext->enableSEITimeCode = false;

	EXPECT_EQ(SendSyncStateChange(&mVideoSink, GST_STATE_NULL, GST_STATE_READY), GST_BUS_PASS);
	EXPECT_EQ(mSoc->freerunCalls, 0);
}

TEST_F(InterfacePlayerBusCoverageTests, SyncHandler_DrmDecryptorNullToReady_PassesMessage)
{
	CreatePipelineForBus();
	GstElement decryptor = {.object = {.name = (gchar *)"playreadydecryptor"}};

	EXPECT_EQ(SendSyncStateChange(&decryptor, GST_STATE_NULL, GST_STATE_READY), GST_BUS_PASS);
}

TEST_F(InterfacePlayerBusCoverageTests, SyncHandler_AsyncDoneWhileBuffering_StartsBufferingTimer)
{
	CreatePipelineForBus();
	mPlayerContext->buffering_in_progress = TRUE;
	mPlayerContext->bufferingTimeoutTimerId = 99;

	EXPECT_EQ(SendSyncMessage(GST_MESSAGE_ASYNC_DONE, GST_OBJECT(&mPipeline)), GST_BUS_PASS);
	// The fake g_timeout_add_full returns 0, so the stored timer id is replaced with 0.
	EXPECT_EQ(static_cast<long long>(mPlayerContext->bufferingTimeoutTimerId), 0LL);
}

TEST_F(InterfacePlayerBusCoverageTests, SyncHandler_AsyncDoneNotBuffering_LeavesTimerUnchanged)
{
	CreatePipelineForBus();
	mPlayerContext->buffering_in_progress = FALSE;
	mPlayerContext->bufferingTimeoutTimerId = 99;

	EXPECT_EQ(SendSyncMessage(GST_MESSAGE_ASYNC_DONE, GST_OBJECT(&mPipeline)), GST_BUS_PASS);
	EXPECT_EQ(static_cast<long long>(mPlayerContext->bufferingTimeoutTimerId), 99LL);
}

TEST_F(InterfacePlayerBusCoverageTests, SyncHandler_StreamStatusWithoutTaskPool_PassesMessage)
{
	CreatePipelineForBus();
	mPlayerContext->task_pool = nullptr;

	EXPECT_EQ(SendSyncMessage(GST_MESSAGE_STREAM_STATUS, GST_OBJECT(&mPipeline)), GST_BUS_PASS);
}

TEST_F(InterfacePlayerBusCoverageTests, SyncHandler_UnhandledMessageType_PassesMessage)
{
	CreatePipelineForBus();

	EXPECT_EQ(SendSyncMessage(GST_MESSAGE_TAG, GST_OBJECT(&mPipeline)), GST_BUS_PASS);
}
