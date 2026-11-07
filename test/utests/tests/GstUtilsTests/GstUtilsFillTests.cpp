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
 * @file GstUtilsFillTests.cpp
 * @brief Gap-fill coverage for GstUtils.cpp:
 *        - GetCaps() per-format branch matrix (media type + first field)
 *        - UNKNOWN/INVALID formats produce no caps call
 *        - CreateGstBufferWithData() allocation/map failure paths
 *
 *        Conventions match GstUtilsTests.cpp: g_mockGStreamer drives the
 *        faked gst_* entry points.  gst_caps_new_simple is forwarded to the
 *        mock with signature (media_type, fieldname, GType, int, void*).
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "GstUtils.h"
#include "MockGStreamer.h"

using ::testing::_;
using ::testing::IsNull;
using ::testing::Return;
using ::testing::StrEq;

class GstUtilsFillTests : public ::testing::Test
{
protected:
	GstCaps mDummyCaps{};

	void SetUp() override
	{
		g_mockGStreamer = new MockGStreamer();
	}

	void TearDown() override
	{
		delete g_mockGStreamer;
		g_mockGStreamer = nullptr;
	}
};

/* ================================================================== */
/* GetCaps - elementary stream formats                                */
/* ================================================================== */

TEST_F(GstUtilsFillTests, GetCaps_MpegTs)
{
	EXPECT_CALL(*g_mockGStreamer,
				gst_caps_new_simple(StrEq("video/mpegts"), StrEq("systemstream"),
									G_TYPE_BOOLEAN, TRUE, _))
		.WillOnce(Return(&mDummyCaps));
	EXPECT_EQ(GetCaps(GST_FORMAT_MPEGTS), &mDummyCaps);
}

TEST_F(GstUtilsFillTests, GetCaps_IsoBmff)
{
	EXPECT_CALL(*g_mockGStreamer,
				gst_caps_new_simple(StrEq("video/quicktime"), IsNull(), _, _, _))
		.WillOnce(Return(&mDummyCaps));
	EXPECT_EQ(GetCaps(GST_FORMAT_ISO_BMFF), &mDummyCaps);
}

TEST_F(GstUtilsFillTests, GetCaps_AudioMp3)
{
	EXPECT_CALL(*g_mockGStreamer,
				gst_caps_new_simple(StrEq("audio/mpeg"), StrEq("mpegversion"),
									G_TYPE_INT, 1, _))
		.WillOnce(Return(&mDummyCaps));
	EXPECT_EQ(GetCaps(GST_FORMAT_AUDIO_ES_MP3), &mDummyCaps);
}

TEST_F(GstUtilsFillTests, GetCaps_AudioAac)
{
	EXPECT_CALL(*g_mockGStreamer,
				gst_caps_new_simple(StrEq("audio/mpeg"), StrEq("mpegversion"),
									G_TYPE_INT, 2, _))
		.WillOnce(Return(&mDummyCaps));
	EXPECT_EQ(GetCaps(GST_FORMAT_AUDIO_ES_AAC), &mDummyCaps);
}

TEST_F(GstUtilsFillTests, GetCaps_AudioAacRaw)
{
	EXPECT_CALL(*g_mockGStreamer,
				gst_caps_new_simple(StrEq("audio/mpeg"), StrEq("mpegversion"),
									G_TYPE_INT, 4, _))
		.WillOnce(Return(&mDummyCaps));
	EXPECT_EQ(GetCaps(GST_FORMAT_AUDIO_ES_AAC_RAW), &mDummyCaps);
}

TEST_F(GstUtilsFillTests, GetCaps_AudioAc3)
{
	EXPECT_CALL(*g_mockGStreamer,
				gst_caps_new_simple(StrEq("audio/x-ac3"), IsNull(), _, _, _))
		.WillOnce(Return(&mDummyCaps));
	EXPECT_EQ(GetCaps(GST_FORMAT_AUDIO_ES_AC3), &mDummyCaps);
}

TEST_F(GstUtilsFillTests, GetCaps_AudioAc4)
{
	EXPECT_CALL(*g_mockGStreamer,
				gst_caps_new_simple(StrEq("audio/x-ac4"), IsNull(), _, _, _))
		.WillOnce(Return(&mDummyCaps));
	EXPECT_EQ(GetCaps(GST_FORMAT_AUDIO_ES_AC4), &mDummyCaps);
}

TEST_F(GstUtilsFillTests, GetCaps_AudioEc3)
{
	EXPECT_CALL(*g_mockGStreamer,
				gst_caps_new_simple(StrEq("audio/x-eac3"), IsNull(), _, _, _))
		.WillOnce(Return(&mDummyCaps));
	EXPECT_EQ(GetCaps(GST_FORMAT_AUDIO_ES_EC3), &mDummyCaps);
}

TEST_F(GstUtilsFillTests, GetCaps_AudioAtmos)
{
	EXPECT_CALL(*g_mockGStreamer,
				gst_caps_new_simple(StrEq("audio/x-eac3"), IsNull(), _, _, _))
		.WillOnce(Return(&mDummyCaps));
	EXPECT_EQ(GetCaps(GST_FORMAT_AUDIO_ES_ATMOS), &mDummyCaps);
}

/* ================================================================== */
/* GetCaps - video formats (also exercise the SocInterface caps hooks) */
/* ================================================================== */

TEST_F(GstUtilsFillTests, GetCaps_VideoH264)
{
	EXPECT_CALL(*g_mockGStreamer,
				gst_caps_new_simple(StrEq("video/x-h264"), IsNull(), _, _, _))
		.WillOnce(Return(&mDummyCaps));
	EXPECT_EQ(GetCaps(GST_FORMAT_VIDEO_ES_H264), &mDummyCaps);
}

TEST_F(GstUtilsFillTests, GetCaps_VideoHevc)
{
	EXPECT_CALL(*g_mockGStreamer,
				gst_caps_new_simple(StrEq("video/x-h265"), IsNull(), _, _, _))
		.WillOnce(Return(&mDummyCaps));
	EXPECT_EQ(GetCaps(GST_FORMAT_VIDEO_ES_HEVC), &mDummyCaps);
}

TEST_F(GstUtilsFillTests, GetCaps_VideoMpeg2)
{
	EXPECT_CALL(*g_mockGStreamer,
				gst_caps_new_simple(StrEq("video/mpeg"), StrEq("mpegversion"),
									G_TYPE_INT, 2, _))
		.WillOnce(Return(&mDummyCaps));
	EXPECT_EQ(GetCaps(GST_FORMAT_VIDEO_ES_MPEG2), &mDummyCaps);
}

/* ================================================================== */
/* GetCaps - subtitle formats                                         */
/* ================================================================== */

TEST_F(GstUtilsFillTests, GetCaps_SubtitleTtml)
{
	EXPECT_CALL(*g_mockGStreamer,
				gst_caps_new_simple(StrEq("application/ttml+xml"), IsNull(), _, _, _))
		.WillOnce(Return(&mDummyCaps));
	EXPECT_EQ(GetCaps(GST_FORMAT_SUBTITLE_TTML), &mDummyCaps);
}

TEST_F(GstUtilsFillTests, GetCaps_SubtitleWebVtt)
{
	EXPECT_CALL(*g_mockGStreamer,
				gst_caps_new_simple(StrEq("text/vtt"), IsNull(), _, _, _))
		.WillOnce(Return(&mDummyCaps));
	EXPECT_EQ(GetCaps(GST_FORMAT_SUBTITLE_WEBVTT), &mDummyCaps);
}

TEST_F(GstUtilsFillTests, GetCaps_SubtitleMp4)
{
	EXPECT_CALL(*g_mockGStreamer,
				gst_caps_new_simple(StrEq("application/mp4"), IsNull(), _, _, _))
		.WillOnce(Return(&mDummyCaps));
	EXPECT_EQ(GetCaps(GST_FORMAT_SUBTITLE_MP4), &mDummyCaps);
}

/* ================================================================== */
/* GetCaps - invalid formats                                          */
/* ================================================================== */

TEST_F(GstUtilsFillTests, GetCaps_UnknownFormatNoCapsCall)
{
	EXPECT_CALL(*g_mockGStreamer, gst_caps_new_simple(_, _, _, _, _)).Times(0);
	EXPECT_EQ(GetCaps(GST_FORMAT_UNKNOWN), nullptr);
}

TEST_F(GstUtilsFillTests, GetCaps_InvalidFormatNoCapsCall)
{
	EXPECT_CALL(*g_mockGStreamer, gst_caps_new_simple(_, _, _, _, _)).Times(0);
	EXPECT_EQ(GetCaps(GST_FORMAT_INVALID), nullptr);
}

/* ================================================================== */
/* CreateGstBufferWithData                                            */
/* ================================================================== */

TEST_F(GstUtilsFillTests, CreateBuffer_MapFailureReturnsNull)
{
	// FakeGStreamer::gst_buffer_map always returns FALSE, so the function
	// unrefs and returns NULL once allocation succeeds.
	const char payload[] = {0x01, 0x02, 0x03, 0x04};
	GstBuffer *buf = CreateGstBufferWithData(payload, sizeof(payload));
	EXPECT_EQ(buf, nullptr);
}

TEST_F(GstUtilsFillTests, CreateBuffer_NullDataStillAttemptsAllocation)
{
	// NULL data pointer is not rejected up-front; the map failure path
	// still returns NULL under the fake.
	EXPECT_EQ(CreateGstBufferWithData(nullptr, 16), nullptr);
}

TEST_F(GstUtilsFillTests, CreateBuffer_ZeroSizeReturnsNull)
{
	const char payload[] = {static_cast<char>(0xAA)};
	EXPECT_EQ(CreateGstBufferWithData(payload, 0), nullptr);
}
