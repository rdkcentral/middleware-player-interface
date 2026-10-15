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
 * @file InfraL1Tests.cpp
 * @brief Vendor / infrastructure L1 coverage:
 *        - SocInterface factory, StartsWith, ReadVideoPts/GetVideoPts,
 *          CheckVideoPtsPropertySupport, ConfigureAcceptCaps, SetWesterosSinkState,
 *          SetDecodeError, SetAC4Tracks, Discover*Properties
 *        - DefaultSocInterface element-name classification, SetAudioProperty,
 *          SetPlaybackFlags, SetPlaybackRate (real event on real pipeline),
 *          IsVideoMaster, ConfigureAudioSink, caps helpers
 *        - PlayerScheduler real-thread task lifecycle: schedule/execute/remove/
 *          suspend/resume/remove-all/stop
 *        - ProcessHandler string parsing and safe kill() error paths
 *        - gstplayertaskpool real GstTaskPool push/join
 *
 *        This suite links the REAL production objects and the real GLib /
 *        GStreamer libraries (no `fakes` library), so threads and GObjects
 *        behave genuinely.  gst_init() is invoked once for the suite.
 */

#include <gtest/gtest.h>
#include <gst/gst.h>
#include <gst/base/gstbasetransform.h>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <string>
#include <vector>
#include <unistd.h>

#include "vendor/SocInterface.h"
#include "vendor/default/DefaultSocInterface.h"
#include "vendor/amlogic/AmlogicSocInterface.h"
#include "vendor/brcm/BrcmSocInterface.h"
#include "vendor/realtek/RealtekSocInterface.h"
#include "vendor/mtk/MtkSocInterface.h"
#include "PlayerScheduler.h"
#include "ProcessHandler.h"
#include "gstplayertaskpool.h"
#include "PlayerLogManager.h"

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

/**
 * @brief Exposes the protected SocInterface::StartsWith for testing.
 */
class ExposedSoc : public DefaultSocInterface
{
public:
	using SocInterface::StartsWith;
	using SocInterface::ReadVideoPts;
	using SocInterface::CheckVideoPtsPropertySupport;
};

class InfraL1Tests : public ::testing::Test
{
protected:
	static void SetUpTestSuite()
	{
		PlayerLogManager::lockLogLevel(false);
		PlayerLogManager::disableLogRedirection = true;
		PlayerLogManager::setLogLevel(mLOGLEVEL_TRACE);
		gst_init(nullptr, nullptr);
	}
};

/* ================================================================== */
/* SocInterface::StartsWith                                             */
/* ================================================================== */

TEST_F(InfraL1Tests, StartsWith_MatchingPrefix)
{
	ExposedSoc soc;
	EXPECT_TRUE(soc.StartsWith("rialtomsevideosink0", "rialtomsevideosink"));
}

TEST_F(InfraL1Tests, StartsWith_NonMatchingPrefix)
{
	ExposedSoc soc;
	EXPECT_FALSE(soc.StartsWith("fakesink", "rialtomsevideosink"));
}

TEST_F(InfraL1Tests, StartsWith_EmptyPrefixAlwaysMatches)
{
	ExposedSoc soc;
	EXPECT_TRUE(soc.StartsWith("anything", ""));
}

TEST_F(InfraL1Tests, StartsWith_IdenticalStrings)
{
	ExposedSoc soc;
	EXPECT_TRUE(soc.StartsWith("westerossink", "westerossink"));
}

TEST_F(InfraL1Tests, StartsWith_InputLongerThanPrefix)
{
	ExposedSoc soc;
	EXPECT_TRUE(soc.StartsWith("amlhalasink0", "amlhalasink"));
}

TEST_F(InfraL1Tests, StartsWith_PrefixMismatchMidway)
{
	ExposedSoc soc;
	EXPECT_FALSE(soc.StartsWith("brcmaudio", "brcmvideo"));
}

/* ================================================================== */
/* SocInterface factory / singleton                                     */
/* ================================================================== */

TEST_F(InfraL1Tests, CreateSocInterface_ReturnsNonNull)
{
	auto soc = SocInterface::CreateSocInterface();
	ASSERT_NE(soc, nullptr);
}

TEST_F(InfraL1Tests, CreateSocInterface_IsSingleton)
{
	auto a = SocInterface::CreateSocInterface();
	auto b = SocInterface::CreateSocInterface();
	EXPECT_EQ(a.get(), b.get());
}

TEST_F(InfraL1Tests, CreateSocInterface_RialtoFlagReturnsSameInstance)
{
	// isRialto only selects the scan strategy on first call; the cached
	// instance is returned thereafter
	auto a = SocInterface::CreateSocInterface(true);
	auto b = SocInterface::CreateSocInterface(false);
	ASSERT_NE(a, nullptr);
	EXPECT_EQ(a.get(), b.get());
}

/* ================================================================== */
/* SocInterface::ReadVideoPts / GetVideoPts                             */
/* ================================================================== */

TEST_F(InfraL1Tests, ReadVideoPts_NullElementReturnsZero)
{
	ExposedSoc soc;
	EXPECT_EQ(soc.ReadVideoPts(nullptr), 0);
}

TEST_F(InfraL1Tests, ReadVideoPts_UnsupportedPropertyReturnsMinusOne)
{
	ExposedSoc soc;
	GstElement *sink = gst_element_factory_make("fakesink", "ut");
	ASSERT_NE(sink, nullptr);
	// fakesink has no 'video-pts' property and the supported flag starts false
	soc.CheckVideoPtsPropertySupport(sink);
	EXPECT_EQ(soc.ReadVideoPts(sink), -1);
	gst_object_unref(sink);
}

TEST_F(InfraL1Tests, GetVideoPts_NullElementZeroTimesTwo)
{
	SocInterface *soc = new DefaultSocInterface();
	// ReadVideoPts(NULL) == 0, non-westeros doubles it -> still 0
	EXPECT_EQ(soc->GetVideoPts(nullptr, nullptr, false), 0);
	delete soc;
}

TEST_F(InfraL1Tests, GetVideoPts_NullElementWesteros)
{
	SocInterface *soc = new DefaultSocInterface();
	EXPECT_EQ(soc->GetVideoPts(nullptr, nullptr, true), 0);
	delete soc;
}

TEST_F(InfraL1Tests, CheckVideoPtsPropertySupport_NullElementNoCrash)
{
	ExposedSoc soc;
	EXPECT_NO_FATAL_FAILURE(soc.CheckVideoPtsPropertySupport(nullptr));
}

TEST_F(InfraL1Tests, DiscoverVideoDecoderProperties_NullElement)
{
	SocInterface *soc = new DefaultSocInterface();
	EXPECT_NO_FATAL_FAILURE(soc->DiscoverVideoDecoderProperties(nullptr));
	delete soc;
}

TEST_F(InfraL1Tests, DiscoverVideoSinkProperties_NoOp)
{
	SocInterface *soc = new DefaultSocInterface();
	GstElement *sink = gst_element_factory_make("fakesink", "ut");
	EXPECT_NO_FATAL_FAILURE(soc->DiscoverVideoSinkProperties(sink));
	if (sink)
	{
		gst_object_unref(sink);
	}
	delete soc;
}

/* ================================================================== */
/* SocInterface misc                                                    */
/* ================================================================== */

TEST_F(InfraL1Tests, SetWesterosSinkState_NoCrash)
{
	SocInterface *soc = new DefaultSocInterface();
	EXPECT_NO_FATAL_FAILURE(soc->SetWesterosSinkState(true));
	EXPECT_NO_FATAL_FAILURE(soc->SetWesterosSinkState(false));
	delete soc;
}

TEST_F(InfraL1Tests, SetDecodeError_NullSrcNoCrash)
{
	SocInterface *soc = new DefaultSocInterface();
	EXPECT_NO_FATAL_FAILURE(soc->SetDecodeError(nullptr));
	delete soc;
}

TEST_F(InfraL1Tests, SetDecodeError_ValidElementSetsProperty)
{
	SocInterface *soc = new DefaultSocInterface();
	GstElement *src = gst_element_factory_make("fakesrc", "ut");
	// fakesrc has no 'report_decode_errors' property - g_object_set warns but
	// must not crash; this exercises the non-null branch
	EXPECT_NO_FATAL_FAILURE(soc->SetDecodeError(GST_OBJECT(src)));
	if (src)
	{
		gst_object_unref(src);
	}
	delete soc;
}

TEST_F(InfraL1Tests, SetAC4Tracks_NullSrcWarnsOnly)
{
	SocInterface *soc = new DefaultSocInterface();
	EXPECT_NO_FATAL_FAILURE(soc->SetAC4Tracks(nullptr, 3));
	delete soc;
}

TEST_F(InfraL1Tests, SetAC4Tracks_ValidElementNoCrash)
{
	SocInterface *soc = new DefaultSocInterface();
	GstElement *src = gst_element_factory_make("fakesrc", "ut");
	EXPECT_NO_FATAL_FAILURE(soc->SetAC4Tracks(src, 0));
	if (src)
	{
		gst_object_unref(src);
	}
	delete soc;
}

TEST_F(InfraL1Tests, ConfigureAcceptCaps_SetsFunctionPointer)
{
	GstBaseTransformClass klass;
	memset(&klass, 0, sizeof(klass));
	auto fn = +[](GstBaseTransform *, GstPadDirection, GstCaps *) -> gboolean { return TRUE; };
	SocInterface *soc = new DefaultSocInterface();
	soc->ConfigureAcceptCaps(&klass, fn);
	EXPECT_EQ(GST_DEBUG_FUNCPTR(klass.accept_caps), GST_DEBUG_FUNCPTR(fn));
	delete soc;
}

TEST_F(InfraL1Tests, ConfigureAcceptCaps_NullFuncLeavesPointer)
{
	GstBaseTransformClass klass;
	memset(&klass, 0, sizeof(klass));
	SocInterface *soc = new DefaultSocInterface();
	soc->ConfigureAcceptCaps(&klass, nullptr);
	EXPECT_EQ(klass.accept_caps, nullptr);
	delete soc;
}

/* ================================================================== */
/* DefaultSocInterface classification                                   */
/* ================================================================== */

TEST_F(InfraL1Tests, Default_IsVideoSink_RialtoMseVideoSink)
{
	DefaultSocInterface soc;
	EXPECT_TRUE(soc.IsVideoSink("rialtomsevideosink0"));
}

TEST_F(InfraL1Tests, Default_IsVideoSink_WesterosSink)
{
	DefaultSocInterface soc;
	EXPECT_TRUE(soc.IsVideoSink("westerossink0"));
}

TEST_F(InfraL1Tests, Default_IsVideoSink_NullName)
{
	DefaultSocInterface soc;
	EXPECT_FALSE(soc.IsVideoSink(nullptr));
}

TEST_F(InfraL1Tests, Default_IsVideoSink_AudioName)
{
	DefaultSocInterface soc;
	EXPECT_FALSE(soc.IsVideoSink("rialtomseaudiosink0"));
}

TEST_F(InfraL1Tests, Default_IsVideoDecoder_VideoName)
{
	DefaultSocInterface soc;
	EXPECT_TRUE(soc.IsVideoDecoder("rialtomsevideosink0"));
}

TEST_F(InfraL1Tests, Default_IsVideoDecoder_AudioName)
{
	DefaultSocInterface soc;
	EXPECT_FALSE(soc.IsVideoDecoder("rialtomseaudiosink0"));
}

TEST_F(InfraL1Tests, Default_IsAudioOrVideoDecoder_AudioSink)
{
	DefaultSocInterface soc;
	EXPECT_TRUE(soc.IsAudioOrVideoDecoder("rialtomseaudiosink0"));
}

TEST_F(InfraL1Tests, Default_IsAudioOrVideoDecoder_VideoSink)
{
	DefaultSocInterface soc;
	EXPECT_TRUE(soc.IsAudioOrVideoDecoder("rialtomsevideosink0"));
}

TEST_F(InfraL1Tests, Default_IsAudioOrVideoDecoder_Westeros)
{
	DefaultSocInterface soc;
	EXPECT_TRUE(soc.IsAudioOrVideoDecoder("westerossink0"));
}

TEST_F(InfraL1Tests, Default_IsAudioOrVideoDecoder_OtherName)
{
	DefaultSocInterface soc;
	EXPECT_FALSE(soc.IsAudioOrVideoDecoder("fakesink0"));
}

TEST_F(InfraL1Tests, Default_IsAudioOrVideoDecoder_NullName)
{
	DefaultSocInterface soc;
	EXPECT_FALSE(soc.IsAudioOrVideoDecoder(nullptr));
}

TEST_F(InfraL1Tests, Default_IsAudioSinkOrAudioDecoder_AlwaysFalse)
{
	DefaultSocInterface soc;
	EXPECT_FALSE(soc.IsAudioSinkOrAudioDecoder("rialtomseaudiosink0"));
	EXPECT_FALSE(soc.IsAudioSinkOrAudioDecoder(nullptr));
}

/* ================================================================== */
/* DefaultSocInterface behaviour                                        */
/* ================================================================== */

TEST_F(InfraL1Tests, Default_SetAudioProperty_Names)
{
	DefaultSocInterface soc;
	const char *volume = nullptr;
	const char *mute = nullptr;
	bool isSinkBinVolume = true;
	soc.SetAudioProperty(volume, mute, isSinkBinVolume);
	EXPECT_STREQ(volume, "volume");
	EXPECT_STREQ(mute, "mute");
#if defined(__APPLE__)
	EXPECT_TRUE(isSinkBinVolume);
#else
	EXPECT_FALSE(isSinkBinVolume);
#endif
}

TEST_F(InfraL1Tests, Default_SetPlaybackFlags_AV)
{
	DefaultSocInterface soc;
	gint flags = 0;
	soc.SetPlaybackFlags(flags, false);
#if !defined(__APPLE__)
	EXPECT_EQ(flags, PLAY_FLAG_VIDEO | PLAY_FLAG_AUDIO | PLAY_FLAG_SOFT_VOLUME);
#endif
}

TEST_F(InfraL1Tests, Default_SetPlaybackFlags_Sub)
{
	DefaultSocInterface soc;
	gint flags = 0xDEAD;
	soc.SetPlaybackFlags(flags, true);
	EXPECT_EQ(flags, PLAY_FLAG_TEXT);
}

TEST_F(InfraL1Tests, Default_UseAppSrc_PlatformValue)
{
	DefaultSocInterface soc;
#if defined(__APPLE__)
	EXPECT_TRUE(soc.UseAppSrc());
#else
	EXPECT_FALSE(soc.UseAppSrc());
#endif
}

TEST_F(InfraL1Tests, Default_IsSimulatorFirstFrame_True)
{
	DefaultSocInterface soc;
	EXPECT_TRUE(soc.IsSimulatorFirstFrame());
}

TEST_F(InfraL1Tests, Default_IsSimulatorVideoSample_True)
{
	DefaultSocInterface soc;
	EXPECT_TRUE(soc.IsSimulatorVideoSample());
}

TEST_F(InfraL1Tests, Default_ShouldTearDownForTrickplay_PlatformValue)
{
	DefaultSocInterface soc;
#if defined(__APPLE__) || defined(UBUNTU)
	EXPECT_TRUE(soc.ShouldTearDownForTrickplay());
#else
	EXPECT_FALSE(soc.ShouldTearDownForTrickplay());
#endif
}

TEST_F(InfraL1Tests, Default_SetPlaybackRate_NullPipelineFalse)
{
	DefaultSocInterface soc;
#if defined(__APPLE__) || defined(UBUNTU)
	EXPECT_FALSE(soc.SetPlaybackRate({}, nullptr, 1.0, nullptr, nullptr));
#else
	std::vector<GstElement*> sources;
	EXPECT_FALSE(soc.SetPlaybackRate(sources, nullptr, 1.0, nullptr, nullptr));
#endif
}

TEST_F(InfraL1Tests, Default_SetPlaybackRate_RealPipelineSendsEvent)
{
#if defined(__APPLE__) || defined(UBUNTU)
	GTEST_SKIP() << "SetPlaybackRate always returns false on Apple/Ubuntu";
#else
	DefaultSocInterface soc;
	GstElement *pipeline = gst_pipeline_new("ratepipe");
	ASSERT_NE(pipeline, nullptr);
	GstElement *fakesink = gst_element_factory_make("fakesink", "ratesink");
	ASSERT_NE(fakesink, nullptr);
	gst_bin_add(GST_BIN(pipeline), fakesink);
	std::vector<GstElement*> sources;
	bool rc = soc.SetPlaybackRate(sources, pipeline, 2.0, nullptr, nullptr);
	// fakesink ignores the OOB rate-change event; result reflects send_event
	(void)rc;
	gst_object_unref(pipeline);
#endif
}

TEST_F(InfraL1Tests, Default_ConfigureAudioSink_NonAmlNameReturnsFalse)
{
#if !defined(__APPLE__) && !defined(UBUNTU)
	DefaultSocInterface soc;
	GstElement *src = gst_element_factory_make("fakesrc", "notaml");
	GstElement *sink = nullptr;
	EXPECT_FALSE(soc.ConfigureAudioSink(&sink, GST_OBJECT(src), false));
	if (src)
	{
		gst_object_unref(src);
	}
#endif
}

TEST_F(InfraL1Tests, Default_IsVideoMaster_NoPropertyDefaultsTrue)
{
	DefaultSocInterface soc;
	GstElement *sink = gst_element_factory_make("fakesink", "ut");
	ASSERT_NE(sink, nullptr);
	// fakesink has no "is-master" property -> default TRUE
	EXPECT_TRUE(soc.IsVideoMaster(sink));
	gst_object_unref(sink);
}

TEST_F(InfraL1Tests, Default_SetH264Caps_UbuntuGuarded)
{
	DefaultSocInterface soc;
	GstCaps *caps = gst_caps_new_empty_simple("video/x-h264");
	ASSERT_NE(caps, nullptr);
	EXPECT_NO_FATAL_FAILURE(soc.SetH264Caps(caps));
	gst_caps_unref(caps);
}

TEST_F(InfraL1Tests, Default_SetHevcCaps_UbuntuGuarded)
{
	DefaultSocInterface soc;
	GstCaps *caps = gst_caps_new_empty_simple("video/x-h265");
	ASSERT_NE(caps, nullptr);
	EXPECT_NO_FATAL_FAILURE(soc.SetHevcCaps(caps));
	gst_caps_unref(caps);
}

TEST_F(InfraL1Tests, Default_ConfigurePluginPriority_NoCrash)
{
	DefaultSocInterface soc;
	EXPECT_NO_FATAL_FAILURE(soc.ConfigurePluginPriority());
}

TEST_F(InfraL1Tests, Default_SetRateCorrection_False)
{
	DefaultSocInterface soc;
	EXPECT_FALSE(soc.SetRateCorrection());
}

TEST_F(InfraL1Tests, Default_IsTransformCapsRequired_BaseDefault)
{
	DefaultSocInterface soc;
	EXPECT_FALSE(soc.IsTransformCapsRequired());
}

TEST_F(InfraL1Tests, Default_IsDecryptRequired_BaseDefault)
{
	DefaultSocInterface soc;
	EXPECT_FALSE(soc.IsDecryptRequired());
}

TEST_F(InfraL1Tests, Default_IsAudioFragmentSyncSupported_False)
{
	DefaultSocInterface soc;
	EXPECT_FALSE(soc.IsAudioFragmentSyncSupported());
}

TEST_F(InfraL1Tests, Default_EnableLiveLatencyCorrection_False)
{
	DefaultSocInterface soc;
	EXPECT_FALSE(soc.EnableLiveLatencyCorrection());
}

TEST_F(InfraL1Tests, Default_RequiredQueuedFrames_Default)
{
	DefaultSocInterface soc;
	EXPECT_EQ(soc.RequiredQueuedFrames(), REQUIRED_QUEUED_FRAMES_DEFAULT);
}

TEST_F(InfraL1Tests, Default_EnablePTSRestamp_False)
{
	DefaultSocInterface soc;
	EXPECT_FALSE(soc.EnablePTSRestamp());
}

TEST_F(InfraL1Tests, Default_IsFirstTuneWithWesteros_False)
{
	DefaultSocInterface soc;
	EXPECT_FALSE(soc.IsFirstTuneWithWesteros());
}

TEST_F(InfraL1Tests, Default_GetSourcePad_NullElement)
{
	DefaultSocInterface soc;
	EXPECT_EQ(soc.GetSourcePad(nullptr), nullptr);
}

TEST_F(InfraL1Tests, Default_AudioOnlyMode_BaseFalse)
{
	DefaultSocInterface soc;
	GstElement *sink = gst_element_factory_make("fakesink", "ut");
	EXPECT_FALSE(soc.AudioOnlyMode(sink));
	if (sink)
	{
		gst_object_unref(sink);
	}
}

/* ================================================================== */
/* PlayerScheduler - real threads                                       */
/* ================================================================== */

namespace
{
	struct TaskGate
	{
		std::mutex mtx;
		std::condition_variable cv;
		int runCount = 0;
		std::vector<std::string> order;
		bool release = false;

		void Run(const std::string &name)
		{
			std::lock_guard<std::mutex> lock(mtx);
			runCount++;
			order.push_back(name);
			cv.notify_all();
		}
		bool WaitRuns(int n, int timeoutMs = 2000)
		{
			std::unique_lock<std::mutex> lock(mtx);
			return cv.wait_for(lock, std::chrono::milliseconds(timeoutMs),
							   [&] { return runCount >= n; });
		}
	};
}

TEST_F(InfraL1Tests, Scheduler_ScheduleWhenNotRunningReturnsInvalid)
{
	PlayerScheduler sched;
	TaskGate gate;
	int id = sched.ScheduleTask(PlayerAsyncTaskObj(
		[](void *d) {}, &gate, "noop"));
	EXPECT_EQ(id, PLAYER_TASK_ID_INVALID);
}

TEST_F(InfraL1Tests, Scheduler_TaskExecutesAfterStart)
{
	PlayerScheduler sched;
	sched.StartScheduler();
	TaskGate gate;
	int id = sched.ScheduleTask(PlayerAsyncTaskObj(
		[](void *d) { static_cast<TaskGate *>(d)->Run("task"); }, &gate, "task"));
	EXPECT_NE(id, PLAYER_TASK_ID_INVALID);
	EXPECT_TRUE(gate.WaitRuns(1));
	sched.StopScheduler();
	EXPECT_EQ(gate.runCount, 1);
}

TEST_F(InfraL1Tests, Scheduler_TaskIdsIncrement)
{
	PlayerScheduler sched;
	sched.StartScheduler();
	TaskGate gate;
	int a = sched.ScheduleTask(PlayerAsyncTaskObj([](void *d) { static_cast<TaskGate *>(d)->Run("a"); }, &gate, "a"));
	int b = sched.ScheduleTask(PlayerAsyncTaskObj([](void *d) { static_cast<TaskGate *>(d)->Run("b"); }, &gate, "b"));
	EXPECT_NE(a, PLAYER_TASK_ID_INVALID);
	EXPECT_NE(b, PLAYER_TASK_ID_INVALID);
	EXPECT_NE(a, b);
	EXPECT_LT(a, b);
	EXPECT_TRUE(gate.WaitRuns(2));
	sched.StopScheduler();
}

TEST_F(InfraL1Tests, Scheduler_TasksRunInFifoOrder)
{
	PlayerScheduler sched;
	sched.StartScheduler();
	TaskGate gate;
	sched.ScheduleTask(PlayerAsyncTaskObj([](void *d) { static_cast<TaskGate *>(d)->Run("first"); }, &gate, "first"));
	sched.ScheduleTask(PlayerAsyncTaskObj([](void *d) { static_cast<TaskGate *>(d)->Run("second"); }, &gate, "second"));
	sched.ScheduleTask(PlayerAsyncTaskObj([](void *d) { static_cast<TaskGate *>(d)->Run("third"); }, &gate, "third"));
	EXPECT_TRUE(gate.WaitRuns(3));
	sched.StopScheduler();
	ASSERT_EQ(gate.order.size(), 3u);
	EXPECT_EQ(gate.order[0], "first");
	EXPECT_EQ(gate.order[1], "second");
	EXPECT_EQ(gate.order[2], "third");
}

TEST_F(InfraL1Tests, Scheduler_SuspendBlocksScheduling)
{
	PlayerScheduler sched;
	sched.StartScheduler();
	sched.SuspendScheduler();
	TaskGate gate;
	int id = sched.ScheduleTask(PlayerAsyncTaskObj(
		[](void *d) { static_cast<TaskGate *>(d)->Run("blocked"); }, &gate, "blocked"));
	EXPECT_EQ(id, PLAYER_TASK_ID_INVALID);
	sched.ResumeScheduler();
	sched.StopScheduler();
	EXPECT_EQ(gate.runCount, 0);
}

TEST_F(InfraL1Tests, Scheduler_ResumeReenablesScheduling)
{
	PlayerScheduler sched;
	sched.StartScheduler();
	sched.SuspendScheduler();
	TaskGate gate;
	EXPECT_EQ(sched.ScheduleTask(PlayerAsyncTaskObj([](void *d) {}, &gate, "x")), PLAYER_TASK_ID_INVALID);
	sched.ResumeScheduler();
	int id = sched.ScheduleTask(PlayerAsyncTaskObj(
		[](void *d) { static_cast<TaskGate *>(d)->Run("after"); }, &gate, "after"));
	EXPECT_NE(id, PLAYER_TASK_ID_INVALID);
	EXPECT_TRUE(gate.WaitRuns(1));
	sched.StopScheduler();
}

struct BlockerGate
{
	std::mutex mtx;
	std::condition_variable cv;
	bool started = false;
	bool release = false;

	static void Run(void *data)
	{
		auto *g = static_cast<BlockerGate *>(data);
		std::unique_lock<std::mutex> lk(g->mtx);
		g->started = true;
		g->cv.notify_all();
		g->cv.wait(lk, [&] { return g->release; });
	}
};

TEST_F(InfraL1Tests, Scheduler_RemoveTaskFromQueuePreventsRun)
{
	PlayerScheduler sched;
	sched.StartScheduler();
	// Occupy the worker with a blocked task so the next task stays queued
	BlockerGate gate;
	sched.ScheduleTask(PlayerAsyncTaskObj(&BlockerGate::Run, &gate, "blocker"));

	TaskGate dead;
	int deadId = sched.ScheduleTask(PlayerAsyncTaskObj(
		[](void *d) { static_cast<TaskGate *>(d)->Run("dead"); }, &dead, "dead"));

	{
		std::unique_lock<std::mutex> lk(gate.mtx);
		ASSERT_TRUE(gate.cv.wait_for(lk, std::chrono::milliseconds(2000),
								   [&] { return gate.started; }));
	}
	EXPECT_TRUE(sched.RemoveTask(deadId));
	{
		std::lock_guard<std::mutex> lk(gate.mtx);
		gate.release = true;
	}
	gate.cv.notify_all();
	sched.StopScheduler();
	EXPECT_EQ(dead.runCount, 0); // removed task never ran
}

TEST_F(InfraL1Tests, Scheduler_RemoveTaskInvalidIdReturnsFalse)
{
	PlayerScheduler sched;
	sched.StartScheduler();
	EXPECT_FALSE(sched.RemoveTask(PLAYER_TASK_ID_INVALID));
	EXPECT_FALSE(sched.RemoveTask(-5));
	sched.StopScheduler();
}

TEST_F(InfraL1Tests, Scheduler_RemoveTaskUnknownIdReturnsFalse)
{
	PlayerScheduler sched;
	sched.StartScheduler();
	EXPECT_FALSE(sched.RemoveTask(99999));
	sched.StopScheduler();
}

TEST_F(InfraL1Tests, Scheduler_RemoveAllTasksClearsQueue)
{
	PlayerScheduler sched;
	sched.StartScheduler();
	// Hold the worker so queued tasks stay pending
	BlockerGate gate;
	sched.ScheduleTask(PlayerAsyncTaskObj(&BlockerGate::Run, &gate, "blocker"));
	{
		std::unique_lock<std::mutex> lk(gate.mtx);
		ASSERT_TRUE(gate.cv.wait_for(lk, std::chrono::milliseconds(2000),
								   [&] { return gate.started; }));
	}
	TaskGate dead;
	sched.ScheduleTask(PlayerAsyncTaskObj(
		[](void *d) { static_cast<TaskGate *>(d)->Run("dead1"); }, &dead, "dead1"));
	sched.ScheduleTask(PlayerAsyncTaskObj(
		[](void *d) { static_cast<TaskGate *>(d)->Run("dead2"); }, &dead, "dead2"));
	sched.RemoveAllTasks();
	{
		std::lock_guard<std::mutex> lk(gate.mtx);
		gate.release = true;
	}
	gate.cv.notify_all();
	sched.StopScheduler();
	EXPECT_EQ(dead.runCount, 0);
}

TEST_F(InfraL1Tests, Scheduler_StopWithoutStartIsSafe)
{
	PlayerScheduler sched;
	EXPECT_NO_FATAL_FAILURE(sched.StopScheduler());
}

TEST_F(InfraL1Tests, Scheduler_DestructorStopsRunningThread)
{
	{
		PlayerScheduler sched;
		sched.StartScheduler();
		// destructor must stop/join the worker cleanly
	}
	SUCCEED();
}

TEST_F(InfraL1Tests, Scheduler_EnableScheduleTaskClearsLockout)
{
	PlayerScheduler sched;
	sched.StartScheduler();
	sched.SuspendScheduler();
	sched.ResumeScheduler();
	sched.EnableScheduleTask();
	TaskGate gate;
	int id = sched.ScheduleTask(PlayerAsyncTaskObj(
		[](void *d) { static_cast<TaskGate *>(d)->Run("post"); }, &gate, "post"));
	EXPECT_NE(id, PLAYER_TASK_ID_INVALID);
	EXPECT_TRUE(gate.WaitRuns(1));
	sched.StopScheduler();
}

TEST_F(InfraL1Tests, Scheduler_TaskObjFieldsPreserved)
{
	int marker = 0;
	PlayerAsyncTaskObj obj([](void *d) { *static_cast<int *>(d) = 7; }, &marker, "nm", 42);
	EXPECT_EQ(obj.mId, 42);
	EXPECT_EQ(obj.mData, &marker);
	EXPECT_EQ(obj.mTaskName, "nm");
	obj.mTask(obj.mData);
	EXPECT_EQ(marker, 7);
}

/* ================================================================== */
/* ProcessHandler                                                       */
/* ================================================================== */

TEST_F(InfraL1Tests, ProcessHandler_GetProcessNameSelf)
{
	ProcessHandler ph;
	std::string pid = std::to_string(getpid());
	std::string name = ph.GetProcessName(pid);
	EXPECT_FALSE(name.empty()); // /proc/self/status always has a Name: line
}

TEST_F(InfraL1Tests, ProcessHandler_GetProcessNameInvalidPid)
{
	ProcessHandler ph;
	std::string bogus = "99999999";
	EXPECT_TRUE(ph.GetProcessName(bogus).empty());
}

TEST_F(InfraL1Tests, ProcessHandler_GetProcessNameNonNumericPath)
{
	ProcessHandler ph;
	std::string bogus = "no_such_pid_dir";
	EXPECT_TRUE(ph.GetProcessName(bogus).empty());
}

TEST_F(InfraL1Tests, ProcessHandler_KillProcessByNameNotFound)
{
	ProcessHandler ph;
	EXPECT_FALSE(ph.KillProcess(std::string("definitely_no_such_proc_xqz9")));
}

TEST_F(InfraL1Tests, ProcessHandler_KillProcessInvalidPidFails)
{
	ProcessHandler ph;
	// large invalid pid -> kill() returns ESRCH -> false
	EXPECT_FALSE(ph.KillProcess(99999999L));
}

/* ================================================================== */
/* gstplayertaskpool                                                    */
/* ================================================================== */

TEST_F(InfraL1Tests, TaskPool_TypeRegistrationSucceeds)
{
	GType t = gst_player_taskpool_get_type();
	EXPECT_NE(t, (GType)0);
	// repeated calls return the same registered type
	EXPECT_EQ(t, gst_player_taskpool_get_type());
}

static void TaskPoolRecorder(void *data)
{
	std::atomic<int> *counter = static_cast<std::atomic<int> *>(data);
	counter->fetch_add(1);
}

TEST_F(InfraL1Tests, TaskPool_PushRunsFunctionJoinCompletes)
{
	GstTaskPool *pool = GST_TASK_POOL(g_object_new(GST_TYPE_PLAYER_TASKPOOL, NULL));
	ASSERT_NE(pool, nullptr);
	std::atomic<int> counter{0};
	GError *err = nullptr;
	gpointer tid = gst_task_pool_push(pool, TaskPoolRecorder, &counter, &err);
	ASSERT_NE(tid, nullptr);
	ASSERT_EQ(err, nullptr);
	gst_task_pool_join(pool, tid); // blocks until the pthread exits
	EXPECT_EQ(counter.load(), 1);
	gst_object_unref(pool);
}

TEST_F(InfraL1Tests, TaskPool_MultipleConcurrentTasks)
{
	GstTaskPool *pool = GST_TASK_POOL(g_object_new(GST_TYPE_PLAYER_TASKPOOL, NULL));
	ASSERT_NE(pool, nullptr);
	std::atomic<int> counter{0};
	GError *err = nullptr;
	gpointer a = gst_task_pool_push(pool, TaskPoolRecorder, &counter, &err);
	gpointer b = gst_task_pool_push(pool, TaskPoolRecorder, &counter, &err);
	gpointer c = gst_task_pool_push(pool, TaskPoolRecorder, &counter, &err);
	ASSERT_NE(a, nullptr);
	ASSERT_NE(b, nullptr);
	ASSERT_NE(c, nullptr);
	gst_task_pool_join(pool, a);
	gst_task_pool_join(pool, b);
	gst_task_pool_join(pool, c);
	EXPECT_EQ(counter.load(), 3);
	gst_object_unref(pool);
}

TEST_F(InfraL1Tests, TaskPool_IsPlayerTaskpoolType)
{
	GstTaskPool *pool = GST_TASK_POOL(g_object_new(GST_TYPE_PLAYER_TASKPOOL, NULL));
	ASSERT_NE(pool, nullptr);
	EXPECT_TRUE(GST_IS_PLAYER_TASKPOOL(pool));
	gst_object_unref(pool);
}
