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
	return false;
}

bool PlayerSocInterfaceImpl::IsSupportedAC4()
{
	return false;
}

bool PlayerSocInterfaceImpl::UseWesterosSink()
{
	return true;
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
	/* Avoid mute property setting for AMLOGIC as use of "mute" property on pipeline is impacting all other players */
	/* Using "stream-volume" property of audio-sink for setting volume and mute for AMLOGIC platform */
	volume = "stream-volume";
	isSinkBinVolume = false;
}

void PlayerSocInterfaceImpl::SetSeamlessSwitch(GstElement* object,  gboolean value)
{
	LOG_PIS("AMLOGIC:setting seamless property");
	g_object_set(sink, "seamless-switch", value, NULL);
}

bool PlayerSocInterfaceImpl::AudioOnlyMode(GstElement *sinkbin)
{
	gint n_audio;
	bool firstFrameReceived = false;
	g_object_get(sinkbin, "n-audio", &n_audio, NULL);

	if(n_audio > 0)
	{
		firstFrameReceived = true;
	}
	return firstFrameReceived;
}

bool PlayerSocInterfaceImpl::SetPlaybackRate(const std::vector<GstElement*>& sources, GstElement *pipeline, double rate, GstElement *video_dec, GstElement *audio_dec)
{
	bool status = false;
	/*for gst version 1.18.0 we need to apply rate into audio/video source pad*/
	for (GstElement* source : sources)
	{
		if(source)
		{
			GstPad* sourceEleSrcPad = gst_element_get_static_pad(GST_ELEMENT(source), "src");
			if(!sourceEleSrcPad)
			{
				LOG_PIS("failed to get static pad retrying");
				continue;
			}
			/*
			 gboolean ret = gst_pad_send_event(sourceEleSrcPad, gst_event_new_seek (rate, GST_FORMAT_TIME,
			 static_cast<GstSeekFlags>(GST_SEEK_FLAG_INSTANT_RATE_CHANGE),
			 GST_SEEK_TYPE_NONE,0, GST_SEEK_TYPE_NONE, 0));
			 gst_object_unref(sourceEleSrcPad);
			 */
			GstEvent* seek_event = gst_event_new_seek(rate, GST_FORMAT_TIME, static_cast<GstSeekFlags>(GST_SEEK_FLAG_INSTANT_RATE_CHANGE), GST_SEEK_TYPE_NONE, 0, GST_SEEK_TYPE_NONE, 0);
			if (!seek_event)
			{
				LOG_PIS("Failed to create seek event");
				gst_object_unref(sourceEleSrcPad);
				continue;
			}
			gboolean ret = gst_pad_send_event(sourceEleSrcPad, seek_event);
			gst_object_unref(sourceEleSrcPad);
			if(ret)
			{
				status = true;
			}
			else
			{
				LOG_PIS("Vendor: failed to send the rate event to src pad");
			}
		}
	}
	LOG_PIS("Current rate: %g", rate);

	return status;
}

GstPad* PlayerSocInterfaceImpl::GetSourcePad(GstElement* element)
{
	GstPad* sourceEleSrcPad = gst_element_get_static_pad(source, "src");
	return sourceEleSrcPad;  // Return the pad, or NULL if not found
}

GstElement* PlayerSocInterfaceImpl::GetVideoSink(GstElement* sinkbin)
{
	GstElement* vidsink = nullptr;
	if(!sinkbin)
	{
		LOG_PIS("Invalid SinkBin");
	}
	else if(mUsingWesterosSink)
	{
		LOG_PIS("using westerossink");
		vidsink = gst_element_factory_make("westerossink", NULL);
		g_object_set(sinkbin, "video-sink", vidsink, NULL);
	}
	return vidsink;
}

void PlayerSocInterfaceImpl::SetAC4Tracks(GstElement *src, int trackId)
{
	LOG_PIS("Selecting AC4 Track Id : %d", trackId);
	if(src)
	{
		g_object_set(src, "ac4-presentation-group-index", trackId, NULL);
	}
	else
	{
		LOG_PIS("No valid src to set ac4-presentation-group-index");
	}
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
	if (name == nullptr)
	{
		return false;
	}

	// Check for Westeros sink
	if (mUsingWesterosSink && StartsWith(name, "westerossink"))
	{
		return true;
	}

	// Check for Rialto sink
	if (isRialto && StartsWith(name, "rialtomsevideosink"))
	{
		return true;
	}

	return false;
}

bool PlayerSocInterfaceImpl::IsAudioSinkOrAudioDecoder(const char* name, bool isRialto)
{
	if(name)
	{
		return StartsWith(name, "amlhalasink");
	}
	else
	{
		return false;
	}
}

bool PlayerSocInterfaceImpl::IsVideoDecoder(const char* name, bool isRialto)
{
	if(name)
	{
		if(mUsingWesterosSink)
		{
			return StartsWith(name, "westerossink");
		}

		else if (isRialto)
		{
			return StartsWith(name, "rialtomsevideosink");
		}
	}
	return false;
}

bool PlayerSocInterfaceImpl::ConfigureAudioSink(GstElement **audio_sink, GstObject *src, bool decStreamSync)
{
	if (!audio_sink || !src)
	{
		LOG_PIS("ConfigureAudioSink: Invalid input parameters");
		return false;
	}

	bool status = false;
	const char* srcName = GST_OBJECT_NAME(src);
	if (srcName && StartsWith(srcName, "amlhalasink"))
	{
		gst_object_replace(reinterpret_cast<GstObject **>(audio_sink), src);

		if (*audio_sink)  // Ensure audio_sink was set correctly
		{
			g_object_set(*audio_sink, "disable-xrun", TRUE, NULL);
			status = true;
		}
	}
	return status;
}

bool PlayerSocInterfaceImpl::IsAudioOrVideoDecoder(const char* name, bool isRialto)
{
	bool AudioOrVideoDecoder = false;
	if(name)
	{
		if(mUsingWesterosSink && StartsWith(name, "westerossink"))
		{
			AudioOrVideoDecoder = true;
		}
		else if(isRialto && StartsWith(name, "rialtomse"))
		{
			AudioOrVideoDecoder = true;
		}
	}
	return AudioOrVideoDecoder;
}

bool PlayerSocInterfaceImpl::DisableAsyncAudio(GstElement *audio_sink, int rate, bool isSeeking)
{
	return false;
}

void PlayerSocInterfaceImpl::GetCCDecoderHandle(gpointer *dec_handle, GstElement *video_dec)
{
	if (video_dec)
	{
		g_object_get(video_dec, "videodecoder", dec_handle, NULL);
	}
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
	flags = PLAY_FLAG_VIDEO | PLAY_FLAG_AUDIO | PLAY_FLAG_NATIVE_AUDIO | PLAY_FLAG_NATIVE_VIDEO;
	flags = PLAY_FLAG_VIDEO | PLAY_FLAG_AUDIO | PLAY_FLAG_SOFT_VOLUME;
	if(isSub)
	{
		flags = PLAY_FLAG_TEXT;
	}
}

bool PlayerSocInterfaceImpl::IsSimulatorFirstFrame()
{
	return false;
}

bool PlayerSocInterfaceImpl::IsSimulatorSink()
{
	return false;
}

void PlayerSocInterfaceImpl::ConfigurePluginPriority()
{
}

bool PlayerSocInterfaceImpl::ShouldTearDownForTrickplay()
{
	return false;
}

bool PlayerSocInterfaceImpl::IsSimulatorVideoSample()
{
	return false;
}

void PlayerSocInterfaceImpl::SetH264Caps(GstCaps *caps)
{
}

void PlayerSocInterfaceImpl::SetHevcCaps(GstCaps *caps)
{
}

bool PlayerSocInterfaceImpl::ResetNewSegmentEvent()
{
	return true;
}

bool PlayerSocInterfaceImpl::IsPlatformSegmentReady()
{
	return true;

}

bool PlayerSocInterfaceImpl::IsVideoMaster()
{
	return false;
}
