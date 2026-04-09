#include "SocInterfaceImpl.h"
#include <cstring>

#define DEFAULT_AVSYNC_FREERUN_THRESHOLD_SECS 12	/**< Currently MAX FRAG DURATION + 2 per Realtek */
#define REQUIRED_QUEUED_FRAMES_REALTEK (3+1)

// Helper function for string prefix matching
static inline bool StartsWith(const char* str, const char* prefix)
{
	if (!str || !prefix)
		return false;
	return strncmp(str, prefix, strlen(prefix)) == 0;
}

SocInterfaceImpl::SocInterfaceImpl()
{
}

std::shared_ptr<SocInterfaceImpl> SocInterfaceImpl::CreateSocInterfaceImpl()
{
	return std::shared_ptr<SocInterfaceImpl>();
}

SocPlatformType SocInterfaceImpl::GetSocPlatformType()
{
	return SocPlatformType::SOC_PLATFORM_REALTEK;
}

void SocInterfaceImpl::SvpGetContext(void **svpCtx, int flags)
{
}

void SocInterfaceImpl::SvpFreeContext(void *svpCtx)
{
}

bool SocInterfaceImpl::IsPlaybackQualityFromSink()
{
	return true;
}

void SocInterfaceImpl::SetVideoBufferSize(GstElement *sink, int size)
{
	g_object_set(sink, "buffer-size", (guint64)size, NULL);
	g_object_set(sink, "buffer-duration", 3000000000, NULL); //3000000000(ns), 3s
}

void SocInterfaceImpl::SetSinkAsync(GstElement *sink, gboolean status)
{
	gst_base_sink_set_async_enabled(GST_BASE_SINK(sink), status);
}

void SocInterfaceImpl::ConfigureAcceptCaps(GstBaseTransformClass* base_transform_class, AcceptCapsFunc accept_caps_func)
{
}

bool SocInterfaceImpl::IsTransformCapsRequired() const
{
	return false;
}

bool SocInterfaceImpl::IsDecryptRequired() const
{
	return false;
}

bool SocInterfaceImpl::UseAppSrc()
{
	return false;
}

bool SocInterfaceImpl::UseWesterosSink()
{
	return false;
}

bool SocInterfaceImpl::IsAudioFragmentSyncSupported()
{
	return true;
}

bool SocInterfaceImpl::EnableLiveLatencyCorrection()
{
	return true;
}

int SocInterfaceImpl::RequiredQueuedFrames()
{
	return REQUIRED_QUEUED_FRAMES_REALTEK;
}

bool SocInterfaceImpl::EnablePTSRestamp()
{
	return false;
}

bool SocInterfaceImpl::IsFirstTuneWithWesteros()
{
	return true;
}

void SocInterfaceImpl::SetAudioProperty(const char * &volume, const char * &mute, bool& isSinkBinVolume)
{
	volume = "volume";
	mute = "mute";
	isSinkBinVolume = true;		/*volume/mute property should be applied on sinkbin*/
}

void SocInterfaceImpl::SetSeamlessSwitch(GstElement* object, gboolean value)
{
}

bool SocInterfaceImpl::AudioOnlyMode(GstElement *sinkbin)
{
	return false;
}

bool SocInterfaceImpl::SetPlaybackRate(const std::vector<GstElement*>& sources, GstElement *pipeline, double rate, GstElement *video_dec, GstElement *audio_dec)
{
	//SOC specific code will  be executed for rialto sink at default soc 
	if(!pipeline)
	{
		// MW_LOG_ERR("Failed to set playback rate");
		return false;
	}
	// MW_LOG_MIL("=send custom-instant-rate-change : %f ...", rate);
	GstStructure *structure = gst_structure_new("custom-instant-rate-change", "rate", G_TYPE_DOUBLE, rate, NULL);
	if(!structure)
	{
		// MW_LOG_ERR("Failed to create custom-instant-rate-change structure");
		return false;
	}
	/* The above statement creates a new GstStructure with the name
	   'custom-instant-rate-change' that has a member variable
	   'rate' of G_TYPE_DOUBLE and a value of rate i.e. second last parameter */
	GstEvent * rate_event = gst_event_new_custom(GST_EVENT_CUSTOM_DOWNSTREAM_OOB, structure);
	if (!rate_event)
	{
		// MW_LOG_ERR("Failed to create rate_event");
		gst_structure_free (structure);
		return false;
	}
	int ret = gst_element_send_event(pipeline, rate_event );
	if(!ret)
	{
		// MW_LOG_ERR("Rate change failed : %g [gst_element_send_event]", rate);
		return false;
	}
	// MW_LOG_MIL("Current rate: %g", rate);
	return true;
}

GstPad* SocInterfaceImpl::GetSourcePad(GstElement* element)
{
	return nullptr;
}

GstElement* SocInterfaceImpl::GetVideoSink(GstElement* sinkbin)
{
	GstElement* vidsink = nullptr;
	if(!sinkbin)
	{
		// MW_LOG_ERR("Invalid SinkBin");
	}
	else if(mUsingWesterosSink)
	{
		// MW_LOG_INFO("using westerossink");
		vidsink = gst_element_factory_make("westerossink", NULL);
		g_object_set(sinkbin, "video-sink", vidsink, NULL);
	}
	return vidsink;
}

void SocInterfaceImpl::SetAC4Tracks(GstElement *src, int trackId)
{
}

bool SocInterfaceImpl::SetPlatformPlaybackRate()
{
	return true;
}

bool SocInterfaceImpl::SetRateCorrection()
{
	return false;
}

bool SocInterfaceImpl::IsVideoSink(const char* name)
{
	return name && (
					StartsWith(name, "westerossink") ||
					StartsWith(name, "rtkv1sink") );
}

bool SocInterfaceImpl::IsAudioSinkOrAudioDecoder(const char* name)
{
	return name && (
					StartsWith(name, "rtkaudiosink") ||
					StartsWith(name, "alsasink") ||
					StartsWith(name, "fakesink") );
}

bool SocInterfaceImpl::IsVideoDecoder(const char* name)
{
	return name && (
					StartsWith(name, "omxwmvdec") ||
					StartsWith(name, "omxh26") ||
					StartsWith(name, "omxav1dec") ||
					StartsWith(name, "omxvp") ||
					StartsWith(name, "omxmpeg") );
}

bool SocInterfaceImpl::ConfigureAudioSink(GstElement **audio_sink, GstObject *src, bool decStreamSync)
{
	if (!audio_sink || !src)
	{
		// MW_LOG_ERR("ConfigureAudioSink: Invalid input parameters");
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

bool SocInterfaceImpl::IsAudioOrVideoDecoder(const char* name)
{
	return name && StartsWith(name, "omx");
}

bool SocInterfaceImpl::DisableAsyncAudio(GstElement *audio_sink, int rate, bool isSeeking)
{
	bool bAsyncModify = false;
	if (audio_sink)
	{
		if (rate > 1 || rate < 0 || isSeeking)
		{
			// MW_LOG_MIL("Disable async for audio stream at trickplay");
			if (gst_base_sink_is_async_enabled(GST_BASE_SINK(audio_sink)) == TRUE)
			{
				gst_base_sink_set_async_enabled(GST_BASE_SINK(audio_sink), FALSE);
				bAsyncModify = true;
			}
		}
	}
	return bAsyncModify;
}

void SocInterfaceImpl::GetCCDecoderHandle(gpointer *dec_handle, GstElement *video_dec)
{
	*dec_handle = video_dec; // Realtek directly returns the decoder element
}

bool SocInterfaceImpl::ResetTrickUTC()
{
	return true;
}

long long SocInterfaceImpl::GetVideoPts(GstElement *video_sink, GstElement *video_dec, bool isWesteros)
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

bool SocInterfaceImpl::NotifyVideoFirstFrame()
{
	return true;
}

void SocInterfaceImpl::SetDecodeError(GstObject* src)
{
}

void SocInterfaceImpl::SetFreerunThreshold(GstObject* src)
{
	if(src)
	{
		g_object_set(src, "freerun-threshold", DEFAULT_AVSYNC_FREERUN_THRESHOLD_SECS, NULL);
	}
}

bool SocInterfaceImpl::RequiredElementSetup()
{
	return true;
}

bool SocInterfaceImpl::HasFirstAudioFrameCallback()
{
	return false;
}

bool SocInterfaceImpl::IsVideoSinkHandleErrors()
{
	return true;
}

void SocInterfaceImpl::SetPlaybackFlags(gint &flags, bool isSub)
{
	flags = PLAY_FLAG_VIDEO | PLAY_FLAG_AUDIO | PLAY_FLAG_NATIVE_AUDIO | PLAY_FLAG_NATIVE_VIDEO;

	flags = PLAY_FLAG_VIDEO | PLAY_FLAG_AUDIO |  PLAY_FLAG_NATIVE_AUDIO | PLAY_FLAG_NATIVE_VIDEO | PLAY_FLAG_SOFT_VOLUME;
	if(isSub)
	{
		flags = PLAY_FLAG_TEXT;
	}
}

bool SocInterfaceImpl::IsSimulatorFirstFrame()
{
	return false;
}

bool SocInterfaceImpl::IsSimulatorSink()
{
	return false;
}

void SocInterfaceImpl::ConfigurePluginPriority()
{
}

bool SocInterfaceImpl::ShouldTearDownForTrickplay()
{
	return false;
}

bool SocInterfaceImpl::IsSimulatorVideoSample()
{
	return false;
}

void SocInterfaceImpl::SetH264Caps(GstCaps *caps)
{
	gst_caps_set_simple (caps, "enable-fastplayback", G_TYPE_STRING, "true", NULL);
}

void SocInterfaceImpl::SetHevcCaps(GstCaps *caps)
{
	gst_caps_set_simple (caps, "enable-fastplayback", G_TYPE_STRING, "true", NULL);
}

bool SocInterfaceImpl::ResetNewSegmentEvent()
{
	return false;
}

bool SocInterfaceImpl::IsVideoMaster(GstElement *videoSink)
{
	return true;
}

