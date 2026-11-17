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
 * @file InterfacePlayerCallbackCoverageTests.cpp
 * @brief Coverage for GStreamer callback paths in InterfacePlayerRDK.cpp.
 *        Callbacks are captured from the g_signal_connect_data, gst_bus_add_watch
 *        and g_timeout_add mocks and then invoked directly:
 *        appsrc need-data / enough-data / seek-data handlers, bus_message
 *        state-change wiring (underflow, pts-error, decode-error), underflow
 *        handling including the EOS PTS checker, and the bus EOS branch.
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <cstring>
#include <map>
#include <memory>
#include <string>
#include "InterfacePlayerRDK.h"
#include "InterfacePlayerPriv.h"
#include "PlayerLogManager.h"
#include "DemuxDataTypes.h"
#include "MockGStreamer.h"
#include "MockGLib.h"
#include "MockPlayerScheduler.h"
#include "MockGstUtils.h"
#include "MockGstHandlerControl.h"

using ::testing::NiceMock;
using ::testing::Return;
using ::testing::StrEq;
using ::testing::_;
using ::testing::DoAll;
using ::testing::Invoke;
using ::testing::SaveArg;

/* Free functions defined in InterfacePlayerRDK.cpp but not declared in its header */
extern const char *CipherTypeToString(CipherType type);
extern bool GstPlayer_isAudioSinkOrAudioDecoder(const char *name, InterfacePlayerRDK *pInterfacePlayerRDK);

namespace
{
/**
 * @brief SocInterface whose element classification is fixed for these tests:
 *        "videosink" is a video sink, "videodec" a video decoder,
 *        "audiosink"/"audiodec" audio elements. Error handling mode is switchable.
 */
class CallbackCoverageSocInterface : public SocInterface
{
public:
	bool handleErrors = false;
	long long videoPtsToReturn = 0;

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
	bool IsVideoSink(const char* name) override
	{
		return name && strcmp(name, "videosink") == 0;
	}
	bool IsAudioSinkOrAudioDecoder(const char* name) override
	{
		return name && (strcmp(name, "audiosink") == 0 || strcmp(name, "audiodec") == 0);
	}
	bool IsVideoDecoder(const char* name) override
	{
		return name && strcmp(name, "videodec") == 0;
	}
	bool ConfigureAudioSink(GstElement **audio_sink, GstObject *src, bool decStreamSync) override { return false; }
	bool IsAudioOrVideoDecoder(const char* name) override
	{
		return name && (strcmp(name, "videodec") == 0 || strcmp(name, "audiodec") == 0);
	}
	void GetCCDecoderHandle(gpointer *dec_handle, GstElement *video_dec) override {}
	void SetPlaybackFlags(gint &flags, bool isSub) override { flags = 0; }
	bool IsVideoMaster(GstElement *videoSink) override { return true; }
	bool IsVideoSinkHandleErrors() override { return handleErrors; }
	long long GetVideoPts(GstElement *video_sink, GstElement *video_dec, bool isWesteros) override
	{
		return videoPtsToReturn;
	}
};
} // namespace

class InterfacePlayerCallbackCoverageTests : public ::testing::Test
{
protected:
	InterfacePlayerRDK *mInterfaceGstPlayer = nullptr;
	InterfacePlayerPriv *mInterfacePrivatePlayer = nullptr;
	GstPlayerPriv *mPlayerContext = nullptr;
	Configs *mPlayerConfigParams = nullptr;
	std::shared_ptr<CallbackCoverageSocInterface> mSoc;

	GstElement mPipeline = {.object = {.name = (gchar *)"testPipeline"}};
	GstBus mBus = {.object = {.name = (gchar *)"testBus"}};
	GstTaskPool mTaskPool = {.object = {.name = (gchar *)"testTaskPool"}};
	GstQuery mQuery = {};
	GstBusFunc mBusWatch = nullptr;
	GstBusSyncHandler mSyncHandler = nullptr;

	GstElement mVideoSink = {.object = {.name = (gchar *)"videosink"}};
	GstElement mVideoDec = {.object = {.name = (gchar *)"videodec"}};
	GstElement mAudioDec = {.object = {.name = (gchar *)"audiodec"}};
	GstElement mAppSrcVideo = {};
	GstElement mAppSrcAudio = {};
	GstCaps mCaps = {};

	std::map<std::string, GCallback> mSignalHandlers;
	GSourceFunc mTimerFunc = nullptr;
	gpointer mTimerData = nullptr;
	GstState mOldState = GST_STATE_NULL;
	GstState mNewState = GST_STATE_NULL;

	int mBusEventCount = 0;
	int mUnderflowCount = 0;
	int mLastUnderflowType = -1;
	int mPtsErrorCount = 0;
	bool mLastPtsIsVideo = false;
	bool mLastPtsIsAudio = false;
	int mDecodeErrorCount = 0;
	int mLastDecodeErrorCount = -1;
	int mNeedDataCount = 0;
	int mLastNeedDataType = -1;
	int mEnoughDataCount = 0;
	int mLastEnoughDataType = -1;

	void SetUp() override
	{
		g_mockGStreamer = new NiceMock<MockGStreamer>();
		g_mockGLib = new NiceMock<MockGLib>();
		g_mockPlayerScheduler = new NiceMock<MockPlayerScheduler>();
		g_mockGstUtils = new NiceMock<MockGstUtils>();
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

		ON_CALL(*g_mockGLib, g_timeout_add(_, _, _))
			.WillByDefault(Invoke([this](guint, GSourceFunc func, gpointer data) -> guint {
				mTimerFunc = func;
				mTimerData = data;
				return 7;
			}));

		mInterfaceGstPlayer = new InterfacePlayerRDK();
		mInterfacePrivatePlayer = mInterfaceGstPlayer->GetPrivatePlayer();
		mPlayerContext = mInterfacePrivatePlayer->gstPrivateContext;
		mPlayerConfigParams = mInterfaceGstPlayer->m_gstConfigParam;

		mSoc = std::make_shared<CallbackCoverageSocInterface>();
		mInterfacePrivatePlayer->socInterface = mSoc;

		//init callbacks to avoid bad_function_call error
		mInterfaceGstPlayer->TearDownCallback([](bool, int) {});
		mInterfaceGstPlayer->StopCallback([](bool) {});
		mInterfaceGstPlayer->RegisterBusEvent([this](auto &&) { ++mBusEventCount; });
		mInterfaceGstPlayer->RegisterBufferUnderflowCb([this](int type) {
			++mUnderflowCount;
			mLastUnderflowType = type;
		});
		mInterfaceGstPlayer->RegisterGstPtsErrorCb([this](bool isVideo, bool isAudioSink) {
			++mPtsErrorCount;
			mLastPtsIsVideo = isVideo;
			mLastPtsIsAudio = isAudioSink;
		});
		mInterfaceGstPlayer->RegisterGstDecodeErrorCb([this](int count) {
			++mDecodeErrorCount;
			mLastDecodeErrorCount = count;
		});
		mInterfaceGstPlayer->RegisterNeedDataCb([this](int type) {
			++mNeedDataCount;
			mLastNeedDataType = type;
		});
		mInterfaceGstPlayer->RegisterEnoughDataCb([this](int type) {
			++mEnoughDataCount;
			mLastEnoughDataType = type;
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

		delete g_mockGstUtils;
		g_mockGstUtils = nullptr;

		delete g_mockGStreamer;
		g_mockGStreamer = nullptr;

		delete g_mockGLib;
		g_mockGLib = nullptr;

		delete g_mockPlayerScheduler;
		g_mockPlayerScheduler = nullptr;
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
	}

	/* Delivers a STATE_CHANGED message from the element through the captured bus watch. */
	void SendStateChange(GstElement *src, GstState oldState, GstState newState)
	{
		mOldState = oldState;
		mNewState = newState;
		GstMessage msg = {};
		msg.type = GST_MESSAGE_STATE_CHANGED;
		msg.src = reinterpret_cast<GstObject *>(src);
		mBusWatch(&mBus, &msg, mInterfaceGstPlayer);
	}

	/* Creates the pipeline and registers the video sink handlers (underflow and PTS error). */
	void ConnectVideoSinkSignals()
	{
		mSoc->handleErrors = true;
		CreatePipelineForBus();
		SendStateChange(&mVideoSink, GST_STATE_NULL, GST_STATE_READY);
	}

	/* Invokes a captured GStreamer element signal handler (underflow, pts-error, decode-error). */
	void InvokeElementSignal(const char *signal, GstElement *elem)
	{
		auto it = mSignalHandlers.find(signal);
		ASSERT_NE(it, mSignalHandlers.end()) << "signal not connected: " << signal;
		reinterpret_cast<void (*)(GstElement *, guint, gpointer, InterfacePlayerRDK *)>(it->second)(elem, 0, nullptr, mInterfaceGstPlayer);
	}

	/* Runs InitializeSourceForPlayer for the given appsrc so its handlers get captured. */
	void InitializeAppSrc(GstElement *src, GstMediaType type)
	{
		mPlayerConfigParams->videoBufBytes = 500;
		mPlayerConfigParams->audioBufBytes = 500;
		mPlayerContext->stream[type].format = GST_FORMAT_ISO_BMFF;
		mPlayerContext->stream[type].source = src;
		EXPECT_CALL(*g_mockGstUtils, GetCaps(_)).WillRepeatedly(Return(&mCaps));
		mInterfaceGstPlayer->InitializeSourceForPlayer(mInterfaceGstPlayer, src, type);
	}
};

/* ------------------------------------------------------------------------- */
/* Direct helpers                                                            */
/* ------------------------------------------------------------------------- */

TEST_F(InterfacePlayerCallbackCoverageTests, CipherTypeToString_AllKnownValues)
{
	EXPECT_STREQ(CipherTypeToString(CIPHER_TYPE_CENC), "cenc");
	EXPECT_STREQ(CipherTypeToString(CIPHER_TYPE_CBC1), "cbc1");
	EXPECT_STREQ(CipherTypeToString(CIPHER_TYPE_CENS), "cens");
	EXPECT_STREQ(CipherTypeToString(CIPHER_TYPE_CBCS), "cbcs");
	EXPECT_STREQ(CipherTypeToString(CIPHER_TYPE_NONE), "none");
}

TEST_F(InterfacePlayerCallbackCoverageTests, CipherTypeToString_UnknownValue)
{
	EXPECT_STREQ(CipherTypeToString(static_cast<CipherType>(99)), "unknown");
}

TEST_F(InterfacePlayerCallbackCoverageTests, IsAudioSinkOrAudioDecoder_DelegatesToSoc)
{
	EXPECT_TRUE(GstPlayer_isAudioSinkOrAudioDecoder("audiosink", mInterfaceGstPlayer));
	EXPECT_FALSE(GstPlayer_isAudioSinkOrAudioDecoder("videosink", mInterfaceGstPlayer));
}

/* ------------------------------------------------------------------------- */
/* appsrc signal handlers                                                    */
/* ------------------------------------------------------------------------- */

TEST_F(InterfacePlayerCallbackCoverageTests, InitializeSourceForPlayer_ConnectsAppSrcSignals)
{
	InitializeAppSrc(&mAppSrcVideo, eGST_MEDIATYPE_VIDEO);

	EXPECT_EQ(mSignalHandlers.count("need-data"), 1u);
	EXPECT_EQ(mSignalHandlers.count("enough-data"), 1u);
	EXPECT_EQ(mSignalHandlers.count("seek-data"), 1u);
	EXPECT_TRUE(mPlayerContext->stream[eGST_MEDIATYPE_VIDEO].sourceConfigured);
}

TEST_F(InterfacePlayerCallbackCoverageTests, NeedData_RegisteredVideoSource_InvokesNeedDataCb)
{
	InitializeAppSrc(&mAppSrcVideo, eGST_MEDIATYPE_VIDEO);

	reinterpret_cast<void (*)(void *, guint, void *)>(mSignalHandlers["need-data"])(&mAppSrcVideo, 0, mInterfaceGstPlayer);

	EXPECT_EQ(mNeedDataCount, 1);
	EXPECT_EQ(mLastNeedDataType, static_cast<int>(eGST_MEDIATYPE_VIDEO));
}

TEST_F(InterfacePlayerCallbackCoverageTests, NeedData_UnmappedSource_NoCallback)
{
	InitializeAppSrc(&mAppSrcVideo, eGST_MEDIATYPE_VIDEO);
	GstElement unknownSrc = {};

	reinterpret_cast<void (*)(void *, guint, void *)>(mSignalHandlers["need-data"])(&unknownSrc, 0, mInterfaceGstPlayer);

	EXPECT_EQ(mNeedDataCount, 0);
}

TEST_F(InterfacePlayerCallbackCoverageTests, EnoughData_RegisteredAudioSource_InvokesEnoughDataCb)
{
	InitializeAppSrc(&mAppSrcAudio, eGST_MEDIATYPE_AUDIO);

	reinterpret_cast<void (*)(GstElement *, void *)>(mSignalHandlers["enough-data"])(&mAppSrcAudio, mInterfaceGstPlayer);

	EXPECT_EQ(mEnoughDataCount, 1);
	EXPECT_EQ(mLastEnoughDataType, static_cast<int>(eGST_MEDIATYPE_AUDIO));
}

TEST_F(InterfacePlayerCallbackCoverageTests, EnoughData_PauseInjectorActive_NoCallback)
{
	InitializeAppSrc(&mAppSrcAudio, eGST_MEDIATYPE_AUDIO);
	mInterfaceGstPlayer->mPauseInjector = true;

	reinterpret_cast<void (*)(GstElement *, void *)>(mSignalHandlers["enough-data"])(&mAppSrcAudio, mInterfaceGstPlayer);

	EXPECT_EQ(mEnoughDataCount, 0);
}

TEST_F(InterfacePlayerCallbackCoverageTests, SeekData_ReturnsTrue)
{
	InitializeAppSrc(&mAppSrcVideo, eGST_MEDIATYPE_VIDEO);

	gboolean ret = reinterpret_cast<gboolean (*)(void *, guint64, void *)>(mSignalHandlers["seek-data"])(&mAppSrcVideo, 1000, mInterfaceGstPlayer);

	EXPECT_TRUE(ret);
}

/* ------------------------------------------------------------------------- */
/* bus_message state-change wiring                                           */
/* ------------------------------------------------------------------------- */

TEST_F(InterfacePlayerCallbackCoverageTests, BusStateChange_VideoSink_ConnectsUnderflowAndPtsHandlers)
{
	ConnectVideoSinkSignals();

	EXPECT_EQ(mSignalHandlers.count("buffer-underflow-callback"), 1u);
	EXPECT_EQ(mSignalHandlers.count("pts-error-callback"), 1u);
	EXPECT_EQ(mSignalHandlers.count("decode-error-callback"), 0u);
	EXPECT_EQ(mBusEventCount, 1);
}

TEST_F(InterfacePlayerCallbackCoverageTests, BusStateChange_VideoDecoder_ConnectsDecodeErrorHandler)
{
	mSoc->handleErrors = false;
	CreatePipelineForBus();

	SendStateChange(&mVideoDec, GST_STATE_NULL, GST_STATE_READY);

	EXPECT_EQ(mSignalHandlers.count("buffer-underflow-callback"), 1u);
	EXPECT_EQ(mSignalHandlers.count("pts-error-callback"), 1u);
	EXPECT_EQ(mSignalHandlers.count("decode-error-callback"), 1u);
}

TEST_F(InterfacePlayerCallbackCoverageTests, BusStateChange_AudioDecoder_ConnectsPtsErrorWithoutDecodeError)
{
	mSoc->handleErrors = false;
	CreatePipelineForBus();

	SendStateChange(&mAudioDec, GST_STATE_NULL, GST_STATE_READY);

	EXPECT_EQ(mSignalHandlers.count("pts-error-callback"), 1u);
	EXPECT_EQ(mSignalHandlers.count("decode-error-callback"), 0u);
}

TEST_F(InterfacePlayerCallbackCoverageTests, BusEos_NotifiesBusEventCallback)
{
	CreatePipelineForBus();

	GstMessage msg = {};
	msg.type = GST_MESSAGE_EOS;
	mBusWatch(&mBus, &msg, mInterfaceGstPlayer);

	EXPECT_EQ(mBusEventCount, 1);
}

/* ------------------------------------------------------------------------- */
/* Underflow callback                                                        */
/* ------------------------------------------------------------------------- */

TEST_F(InterfacePlayerCallbackCoverageTests, Underflow_VideoSink_MarksBufferUnderrunAndNotifies)
{
	ConnectVideoSinkSignals();

	InvokeElementSignal("buffer-underflow-callback", &mVideoSink);

	EXPECT_TRUE(mPlayerContext->stream[eGST_MEDIATYPE_VIDEO].bufferUnderrun);
	EXPECT_EQ(mUnderflowCount, 1);
	EXPECT_EQ(mLastUnderflowType, static_cast<int>(eGST_MEDIATYPE_VIDEO));
}

TEST_F(InterfacePlayerCallbackCoverageTests, Underflow_DisableUnderflowConfig_IgnoresEvent)
{
	ConnectVideoSinkSignals();
	mPlayerConfigParams->disableUnderflow = true;

	InvokeElementSignal("buffer-underflow-callback", &mVideoSink);

	EXPECT_FALSE(mPlayerContext->stream[eGST_MEDIATYPE_VIDEO].bufferUnderrun);
	EXPECT_EQ(mUnderflowCount, 0);
}

TEST_F(InterfacePlayerCallbackCoverageTests, Underflow_UnmappedElement_IsIgnored)
{
	ConnectVideoSinkSignals();
	GstElement otherSink = {.object = {.name = (gchar *)"unknownsink"}};

	InvokeElementSignal("buffer-underflow-callback", &otherSink);

	EXPECT_FALSE(mPlayerContext->stream[eGST_MEDIATYPE_VIDEO].bufferUnderrun);
	EXPECT_EQ(mUnderflowCount, 0);
}

TEST_F(InterfacePlayerCallbackCoverageTests, Underflow_EosReachedAtNormalRate_SchedulesPtsCheckAndDetectsEos)
{
	ConnectVideoSinkSignals();
	mSoc->videoPtsToReturn = 1234;
	mPlayerContext->stream[eGST_MEDIATYPE_VIDEO].eosReached = true;
	mPlayerContext->rate = 1;

	InvokeElementSignal("buffer-underflow-callback", &mVideoSink);

	EXPECT_EQ(mPlayerContext->ptsCheckForEosOnUnderflowIdleTaskId, 7u);
	ASSERT_NE(mTimerFunc, nullptr);
	EXPECT_EQ(mPlayerContext->lastKnownPTS, 1234);

	/* PTS unchanged since underflow, so the checker reports EOS and clears its task id */
	EXPECT_EQ(mTimerFunc(mTimerData), G_SOURCE_REMOVE);
	EXPECT_EQ(mPlayerContext->ptsCheckForEosOnUnderflowIdleTaskId, 0u);
}

TEST_F(InterfacePlayerCallbackCoverageTests, Underflow_EosReachedTimerAlreadyScheduled_DoesNotReschedule)
{
	ConnectVideoSinkSignals();
	mPlayerContext->stream[eGST_MEDIATYPE_VIDEO].eosReached = true;
	mPlayerContext->rate = 1;
	mPlayerContext->ptsCheckForEosOnUnderflowIdleTaskId = 5;

	InvokeElementSignal("buffer-underflow-callback", &mVideoSink);

	EXPECT_TRUE(mTimerFunc == nullptr);
	EXPECT_EQ(mPlayerContext->ptsCheckForEosOnUnderflowIdleTaskId, 5u);
}

/* ------------------------------------------------------------------------- */
/* PTS error and decode error callbacks                                      */
/* ------------------------------------------------------------------------- */

TEST_F(InterfacePlayerCallbackCoverageTests, PtsError_VideoDecoder_ReportsVideoFlag)
{
	mSoc->handleErrors = false;
	CreatePipelineForBus();
	SendStateChange(&mVideoDec, GST_STATE_NULL, GST_STATE_READY);

	InvokeElementSignal("pts-error-callback", &mVideoDec);

	EXPECT_EQ(mPtsErrorCount, 1);
	EXPECT_TRUE(mLastPtsIsVideo);
	EXPECT_FALSE(mLastPtsIsAudio);
}

TEST_F(InterfacePlayerCallbackCoverageTests, PtsError_AudioDecoder_ReportsAudioFlag)
{
	mSoc->handleErrors = false;
	CreatePipelineForBus();
	SendStateChange(&mAudioDec, GST_STATE_NULL, GST_STATE_READY);

	InvokeElementSignal("pts-error-callback", &mAudioDec);

	EXPECT_EQ(mPtsErrorCount, 1);
	EXPECT_FALSE(mLastPtsIsVideo);
	EXPECT_TRUE(mLastPtsIsAudio);
}

TEST_F(InterfacePlayerCallbackCoverageTests, DecodeError_ThrottlesCallbackWithinInterval)
{
	mSoc->handleErrors = false;
	CreatePipelineForBus();
	SendStateChange(&mVideoDec, GST_STATE_NULL, GST_STATE_READY);
	mPlayerContext->decodeErrorMsgTimeMS = 0;

	/* First error is outside the throttle interval and is reported */
	InvokeElementSignal("decode-error-callback", &mVideoDec);
	EXPECT_EQ(mDecodeErrorCount, 1);
	EXPECT_EQ(mLastDecodeErrorCount, 1);

	/* Second error arrives immediately and is suppressed, count keeps accumulating */
	InvokeElementSignal("decode-error-callback", &mVideoDec);
	EXPECT_EQ(mDecodeErrorCount, 1);
	EXPECT_EQ(mPlayerContext->decodeErrorCBCount, 1);
}
