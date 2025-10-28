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
#define DEFAULT_AVSYNC_FREERUN_THRESHOLD_SECS 12	/**< Currently MAX FRAG DURATION + 2 per Realtek */
#define REQUIRED_QUEUED_FRAMES_REALTEK (3+1)

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
	return true;
}

void PlayerSocInterfaceImpl::SetVideoBufferSize(GstElement *sink, int size)
{
	g_object_set(sink, "buffer-size", (guint64)size, NULL);
	g_object_set(sink, "buffer-duration", 3000000000, NULL); //3000000000(ns), 3s
}

void PlayerSocInterfaceImpl::SetSinkAsync(GstElement *sink, gboolean status)
{
	gst_base_sink_set_async_enabled(GST_BASE_SINK(sink), status);
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
	return true;
}

bool PlayerSocInterfaceImpl::EnableLiveLatencyCorrection()
{
	return true;
}

int PlayerSocInterfaceImpl::RequiredQueuedFrames()
{
	return REQUIRED_QUEUED_FRAMES_REALTEK;
}

bool PlayerSocInterfaceImpl::EnablePTSRestamp()
{
	return false;
}

bool PlayerSocInterfaceImpl::IsFirstTuneWithWesteros()
{
	return true;
}

void PlayerSocInterfaceImpl::SetAudioProperty(const char * &volume, const char * &mute, bool& isSinkBinVolume)
{
	volume = "volume";
	mute = "mute";
	isSinkBinVolume = true;		/*volume/mute property should be applied on sinkbin*/
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
	if(!pipeline)
	{
		LOG_PIS("Failed to set playback rate");
		return false;
	}
	LOG_PIS("=send custom-instant-rate-change : %f ...", rate);
	GstStructure *structure = gst_structure_new("custom-instant-rate-change", "rate", G_TYPE_DOUBLE, rate, NULL);
	if(!structure)
	{
		LOG_PIS("Failed to create custom-instant-rate-change structure");
		return false;
	}
	/* The above statement creates a new GstStructure with the name
	   'custom-instant-rate-change' that has a member variable
	   'rate' of G_TYPE_DOUBLE and a value of rate i.e. second last parameter */
	GstEvent * rate_event = gst_event_new_custom(GST_EVENT_CUSTOM_DOWNSTREAM_OOB, structure);
	if (!rate_event)
	{
		LOG_PIS("Failed to create rate_event");
		gst_structure_free (structure);
		return false;
	}
	int ret = gst_element_send_event(pipeline, rate_event );
	if(!ret)
	{
		LOG_PIS("Rate change failed : %g [gst_element_send_event]", rate);
		return false;
	}
	LOG_PIS("Current rate: %g", rate);
	return true;
}

GstPad* PlayerSocInterfaceImpl::GetSourcePad(GstElement* element)
{
	return NULL;  // Return the pad, or NULL if not found
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
	return true;
}

bool PlayerSocInterfaceImpl::SetRateCorrection()
{
	return false;
}

bool PlayerSocInterfaceImpl::IsVideoSink(const char* name, bool isRialto)
{
	if(name)
		return (StartsWith(name, "westerossink") || StartsWith(name, "rtkv1sink") || (isRialto && StartsWith(name, "rialtomsevideosink") == true));
	else
		return false;
}

bool PlayerSocInterfaceImpl::IsAudioSinkOrAudioDecoder(const char* name, bool isRialto)
{
	if(name)
	{
		return (StartsWith(name, "rtkaudiosink")
				|| StartsWith(name, "alsasink")
				|| StartsWith(name, "fakesink")
				|| (isRialto && StartsWith(name, "rialtomseaudiosink") == true));
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
		return (StartsWith(name, "omxwmvdec") || StartsWith(name, "omxh26")
				|| StartsWith(name, "omxav1dec") || StartsWith(name, "omxvp") || StartsWith(name, "omxmpeg"));
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

	if (srcName && (StartsWith(srcName, "rtkaudiosink")
				|| StartsWith(srcName, "alsasink")
				|| StartsWith(srcName, "fakesink")))
	{
		gst_object_replace(reinterpret_cast<GstObject **>(audio_sink), src);
		status = true;
	}

	return status;
}

bool PlayerSocInterfaceImpl::IsAudioOrVideoDecoder(const char* name, bool isRialto)
{
	bool AudioOrVideoDecoder = false;
	if(name)
	{
		if(StartsWith(name, "omx"))
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
	bool bAsyncModify = false;
	if (audio_sink)
	{
		if (rate > 1 || rate < 0 || isSeeking)
		{
			LOG_PIS("Disable async for audio stream at trickplay");
			if (gst_base_sink_is_async_enabled(GST_BASE_SINK(audio_sink)) == TRUE)
			{
				gst_base_sink_set_async_enabled(GST_BASE_SINK(audio_sink), FALSE);
				bAsyncModify = true;
			}
		}
	}
	return bAsyncModify;
}

void PlayerSocInterfaceImpl::GetCCDecoderHandle(gpointer *dec_handle, GstElement *video_dec)
{
	*dec_handle = video_dec; // Realtek directly returns the decoder element
}

bool PlayerSocInterfaceImpl::ResetTrickUTC()
{
	return true;
}

long long PlayerSocInterfaceImpl::GetVideoPts(GstElement *video_sink, GstElement *video_dec, bool isWesteros)
{
	gint64 currentPTS = 0;
	GstElement *element = nullptr;
	if(video_sink)
	{
		element = video_sink;
	}

	if (element)
	{
		g_object_get(element, "video-pts", &currentPTS, NULL);	/* Gets the 'video-pts' from the element into the currentPTS */
	}
	return (long long)currentPTS;
}

bool PlayerSocInterfaceImpl::NotifyVideoFirstFrame()
{
	return true;
}

void PlayerSocInterfaceImpl::SetDecodeError(GstObject* src)
{
}

void PlayerSocInterfaceImpl::SetFreerunThreshold(GstObject* src)
{
	if(src)
	{
		g_object_set(src, "freerun-threshold", DEFAULT_AVSYNC_FREERUN_THRESHOLD_SECS, NULL);
	}
}

bool PlayerSocInterfaceImpl::RequiredElementSetup()
{
	return true;
}

void PlayerSocInterfaceImpl::SetAudioRoutingProperties(GstElement *source)
{
	if ((strstr(GST_ELEMENT_NAME(source), "omxaacdec") != NULL) ||
			(strstr(GST_ELEMENT_NAME(source), "omxac3dec") != NULL) ||
			(strstr(GST_ELEMENT_NAME(source), "omxeac3dec") != NULL) ||
			(strstr(GST_ELEMENT_NAME(source), "omxmp3dec") != NULL) ||
			(strstr(GST_ELEMENT_NAME(source), "omxvorbisdec") != NULL) ||
			(strstr(GST_ELEMENT_NAME(source), "omxac4dec") != NULL))
	{
		g_object_set(source, "audio-tunnel-mode", FALSE, NULL );
		LOG_PIS("callback_element_added audio-tunnel-mode FALSE");
		g_object_set(source, "aux-audio", TRUE, NULL );
		LOG_PIS("callback_element_added aux-audio TRUE");
	}	
}

bool PlayerSocInterfaceImpl::HasFirstAudioFrameCallback()
{
	return false;
}

bool PlayerSocInterfaceImpl::IsVideoSinkHandleErrors()
{
	return true;
}

void PlayerSocInterfaceImpl::SetPlaybackFlags(gint &flags,  bool isSub)
{
	flags = PLAY_FLAG_VIDEO | PLAY_FLAG_AUDIO | PLAY_FLAG_NATIVE_AUDIO | PLAY_FLAG_NATIVE_VIDEO;

	flags = PLAY_FLAG_VIDEO | PLAY_FLAG_AUDIO |  PLAY_FLAG_NATIVE_AUDIO | PLAY_FLAG_NATIVE_VIDEO | PLAY_FLAG_SOFT_VOLUME;
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
	gst_caps_set_simple (caps, "enable-fastplayback", G_TYPE_STRING, "true", NULL);
}

void PlayerSocInterfaceImpl::SetHevcCaps(GstCaps *caps)
{
	gst_caps_set_simple (caps, "enable-fastplayback", G_TYPE_STRING, "true", NULL);
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
