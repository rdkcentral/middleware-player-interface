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

#include "PlayerSocInterfaceImpl.h"
#include "PlayerLogManager.h"

#define REQUIRED_QUEUED_FRAMES_DEFAULT (5+1)

/**
 * @brief Checks if the input string starts with the given prefix.
 *
 * @param inputStr The input string to check.
 * @param prefix The prefix to check for.
 *
 * @return True if the input string starts with the prefix, false otherwise.
 */
bool StartsWith( const char *inputStr, const char *prefix )
{
	bool rc = true;
	while( *prefix )
	{
		if( *inputStr++ != *prefix++ )
		{
			rc = false;
			break;
		}
	}
	return rc;
}

PlayerSocInterfaceImpl::PlayerSocInterfaceImpl()
{
}

PlayerSocInterfaceImpl::~PlayerSocInterfaceImpl()
{
}

void PlayerSocInterfaceImpl::SetWesterosSinkState(bool status)
{
	mUsingWesterosSink = status;
}

bool PlayerSocInterfaceImpl::IsPlaybackQualityFromSink()
{
	return false;
}

void PlayerSocInterfaceImpl::SetVideoBufferSize(GstElement *sink, int size)
{
}

void PlayerSocInterfaceImpl::SetSinkAsync(GstElement *sink, gboolean status)
{
}

bool PlayerSocInterfaceImpl::UseAppSrc()
{
#if defined (__APPLE__)
	return true;
#endif
	return false;
}

bool PlayerSocInterfaceImpl::IsSupportedAC4()
{
	return false;
}

bool PlayerSocInterfaceImpl::UseWesterosSink()
{
	return false;
}

bool PlayerSocInterfaceImpl::IsAudioFragmentSyncSupported()
{
	return false;
}

bool PlayerSocInterfaceImpl::EnableLiveLatencyCorrection()
{
	return false;
}

int PlayerSocInterfaceImpl::RequiredQueuedFrames()
{
	return REQUIRED_QUEUED_FRAMES_DEFAULT;
}

bool PlayerSocInterfaceImpl::EnablePTSRestamp()
{
	return false;
}

bool PlayerSocInterfaceImpl::IsFirstTuneWithWesteros()
{
	return false;
}

void PlayerSocInterfaceImpl::SetAudioProperty(const char * &volume, const char * &mute, bool& isSinkBinVolume)
{
	isSinkBinVolume = false;
	volume = "volume";
	mute = "mute";
#if defined(__APPLE__)
	isSinkBinVolume = true;
#endif
}

void PlayerSocInterfaceImpl::SetSeamlessSwitch(GstElement* object,  gboolean value)
{
}

bool PlayerSocInterfaceImpl::AudioOnlyMode(GstElement *sinkbin)
{
	return false;
}

bool PlayerSocInterfaceImpl::SetPlaybackRate(const std::vector<GstElement*>& sources, GstElement *pipeline, double rate, GstElement *video_dec, GstElement *audio_dec)
{
	return false;
}

GstPad* PlayerSocInterfaceImpl::GetSourcePad(GstElement* element)
{
	return NULL;
}

GstElement* PlayerSocInterfaceImpl::GetVideoSink(GstElement* sinkbin)
{
	return nullptr;
}

void PlayerSocInterfaceImpl::SetAC4Tracks(GstElement *src, int trackId)
{
	MW_LOG_INFO("Selecting AC4 Track Id : %d", trackId);
	g_object_set(src, "ac4-presentation-group-index", trackId, NULL);
}

bool PlayerSocInterfaceImpl::SetPlatformPlaybackRate()
{
	return false;
}

bool PlayerSocInterfaceImpl::SetRateCorrection()
{
	return false;
}

bool PlayerSocInterfaceImpl::IsVideoSink(const char* name, bool isRialto)
{
	return (mUsingWesterosSink && StartsWith(name, "westerossink") == true);
}

bool PlayerSocInterfaceImpl::IsAudioSinkOrAudioDecoder(const char* name, bool isRialto)
{
	return false;
}

bool PlayerSocInterfaceImpl::IsVideoDecoder(const char* name, bool isRialto)
{
	if(mUsingWesterosSink)
	{
		return StartsWith(name, "westerossink");
	}
	else if (isRialto)
	{
		return StartsWith(name, "rialtomsevideosink");
	}
	return false;
}

bool PlayerSocInterfaceImpl::ConfigureAudioSink(GstElement **audio_sink, GstObject *src, bool decStreamSync)
{
	bool status = false;
	if (StartsWith(GST_OBJECT_NAME(src), "amlhalasink") == true)
	{
			gst_object_replace((GstObject **)audio_sink, src);
			g_object_set(audio_sink, "disable-xrun", TRUE, NULL);
			status = true;
	}
	return status;
}

bool PlayerSocInterfaceImpl::IsAudioOrVideoDecoder(const char* name, bool isRialto)
{
	bool AudioOrVideoDecoder = false;
	if(mUsingWesterosSink && StartsWith(name, "westerossink"))
	{
		AudioOrVideoDecoder = true;
	}
	else if(isRialto && StartsWith(name, "rialtomse"))
	{
		AudioOrVideoDecoder = true;
	}
	return AudioOrVideoDecoder;
}

bool PlayerSocInterfaceImpl::DisableAsyncAudio(GstElement *audio_sink, int rate, bool isSeeking)
{
	return false;
}

void PlayerSocInterfaceImpl::GetCCDecoderHandle(gpointer *dec_handle, GstElement *video_dec)
{

}

bool PlayerSocInterfaceImpl::ResetTrickUTC()
{
	return false;
}

long long PlayerSocInterfaceImpl::GetVideoPts(GstElement *video_sink, GstElement *video_dec, bool isWesteros)
{
	gint64 currentPTS = 0;
	if(video_dec)
	{
		g_object_get(video_dec, "video-pts", &currentPTS, NULL);
		if(!isWesteros)
		{
			currentPTS *= 2;
		}
	}
	return (long long)currentPTS;
}

bool PlayerSocInterfaceImpl::NotifyVideoFirstFrame()
{
	return false;
}

void PlayerSocInterfaceImpl::SetDecodeError(GstObject* src)
{
	if(src)
	{
		g_object_set(src, "report_decode_errors", TRUE, NULL);
	}
}

void PlayerSocInterfaceImpl::SetFreerunThreshold(GstObject* src)
{

}

bool PlayerSocInterfaceImpl::RequiredElementSetup()
{
	return false;
}

void PlayerSocInterfaceImpl::SetAudioRoutingProperties(GstElement *source)
{
	
}

bool PlayerSocInterfaceImpl::HasFirstAudioFrameCallback()
{
	return true;
}

bool PlayerSocInterfaceImpl::IsVideoSinkHandleErrors()
{
	return false;
}

void PlayerSocInterfaceImpl::SetPlaybackFlags(gint &flags,  bool isSub)
{
#if defined(__APPLE__)
	// on OSX, just use working defaults
	// note that if PLAY_FLAG_DEINTERLACE is not included, video freezes on first frame
#else
	flags = PLAY_FLAG_VIDEO | PLAY_FLAG_AUDIO | PLAY_FLAG_SOFT_VOLUME;
#endif
	if(isSub)
	{
		flags = PLAY_FLAG_TEXT;
	}
}

bool PlayerSocInterfaceImpl::IsSimulatorFirstFrame()
{
	return true;
}

bool PlayerSocInterfaceImpl::IsSimulatorSink()
{
#if !defined(UBUNTU)
	return false;
#endif
	return true;
}

void PlayerSocInterfaceImpl::ConfigurePluginPriority()
{
#ifdef UBUNTU
	GstPluginFeature* pluginFeature = gst_registry_lookup_feature(gst_registry_get(), "pulsesink");
	if (pluginFeature != NULL)
	{
		MW_LOG_INFO("InterfacePlayerRDK: pulsesink plugin priority set to GST_RANK_SECONDARY");
		gst_plugin_feature_set_rank(pluginFeature, GST_RANK_SECONDARY);
		gst_object_unref(pluginFeature);
	}
#endif
}

bool PlayerSocInterfaceImpl::ShouldTearDownForTrickplay()
{
#if defined(__APPLE__) || defined(UBUNTU)
	return true;
#endif
	return false;
}

bool PlayerSocInterfaceImpl::IsSimulatorVideoSample()
{
#if defined(__APPLE__)
	return true;
#endif
	return true;
}

void PlayerSocInterfaceImpl::SetH264Caps(GstCaps *caps)
{
#ifdef UBUNTU
	// below required on Ubuntu - harmless on OSX, but breaks RPI
	gst_caps_set_simple (caps,
			"alignment", G_TYPE_STRING, "au",
			"stream-format", G_TYPE_STRING, "avc",
			NULL);
#endif
}

void PlayerSocInterfaceImpl::SetHevcCaps(GstCaps *caps)
{
#ifdef UBUNTU
	// below required on Ubuntu - harmless on OSX, but breaks RPI
	gst_caps_set_simple(caps,
			"alignment", G_TYPE_STRING, "au",
			"stream-format", G_TYPE_STRING, "hev1",
			NULL);
#endif
}

bool PlayerSocInterfaceImpl::ResetNewSegmentEvent()
{
	return false;
}

bool PlayerSocInterfaceImpl::IsPlatformSegmentReady()
{
	return false;

}

bool PlayerSocInterfaceImpl::IsVideoMaster()
{
	return true;
}
