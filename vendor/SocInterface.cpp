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

#include "SocInterface.h"


/**< Static variable for singleton */
std::shared_ptr<SocInterface> SocInterface::s_pSocInterface = nullptr;


SocInterface::SocInterface()
{
	m_pPlayerSocInterfaceImpl = std::shared_ptr<PlayerSocInterfaceImpl>(new PlayerSocInterfaceImpl());
}

SocInterface::~SocInterface()
{
	m_pPlayerSocInterfaceImpl = nullptr;
	s_pSocInterface = nullptr;
}

std::shared_ptr<SocInterface> SocInterface::CreateSocInterface()
{
	if( !s_pSocInterface)
	{
		s_pSocInterface = std::shared_ptr<SocInterface>(new SocInterface());
	}
	return s_pSocInterface;
}

void SocInterface::SetWesterosSinkState(bool status)
{
	m_pPlayerSocInterfaceImpl->SetWesterosSinkState(status);
}

bool SocInterface::IsPlaybackQualityFromSink()
{
	return m_pPlayerSocInterfaceImpl->IsPlaybackQualityFromSink();
}

void SocInterface::SetVideoBufferSize(GstElement *sink, int size)
{
	m_pPlayerSocInterfaceImpl->SetVideoBufferSize(sink, size);
}

void SocInterface::SetSinkAsync(GstElement *sink, gboolean status)
{
	m_pPlayerSocInterfaceImpl->SetSinkAsync(sink, status);
}

bool SocInterface::UseAppSrc()
{
	return m_pPlayerSocInterfaceImpl->UseAppSrc();
}

bool SocInterface::IsSupportedAC4()
{
	return m_pPlayerSocInterfaceImpl->IsSupportedAC4();
}

bool SocInterface::UseWesterosSink()
{
	return m_pPlayerSocInterfaceImpl->UseWesterosSink();
}

bool SocInterface::IsAudioFragmentSyncSupported()
{
	return m_pPlayerSocInterfaceImpl->IsAudioFragmentSyncSupported();
}

bool SocInterface::EnableLiveLatencyCorrection()
{
	return m_pPlayerSocInterfaceImpl->EnableLiveLatencyCorrection();
}

int SocInterface::RequiredQueuedFrames()
{
	return m_pPlayerSocInterfaceImpl->RequiredQueuedFrames();
}

bool SocInterface::EnablePTSRestamp()
{
	return m_pPlayerSocInterfaceImpl->EnablePTSRestamp();
}

bool SocInterface::IsFirstTuneWithWesteros()
{
	return m_pPlayerSocInterfaceImpl->IsFirstTuneWithWesteros();
}

void SocInterface::SetAudioProperty(const char * &volume, const char * &mute, bool& isSinkBinVolume)
{
	m_pPlayerSocInterfaceImpl->SetAudioProperty(volume, mute, isSinkBinVolume);
}

void SocInterface::SetSeamlessSwitch(GstElement* object,  gboolean value)
{
	m_pPlayerSocInterfaceImpl->SetSeamlessSwitch(object, value);
}

bool SocInterface::AudioOnlyMode(GstElement *sinkbin)
{
	return m_pPlayerSocInterfaceImpl->AudioOnlyMode(sinkbin);
}

bool SocInterface::SetPlaybackRate(const std::vector<GstElement*>& sources, GstElement *pipeline, double rate, GstElement *video_dec, GstElement *audio_dec)
{
	return m_pPlayerSocInterfaceImpl->SetPlaybackRate(sources, pipeline, rate, video_dec, audio_dec);
}

GstPad* SocInterface::GetSourcePad(GstElement* element)
{
	return m_pPlayerSocInterfaceImpl->GetSourcePad(element);
}

GstElement* SocInterface::GetVideoSink(GstElement* sinkbin)
{
	return m_pPlayerSocInterfaceImpl->GetVideoSink(sinkbin);
}

void SocInterface::SetAC4Tracks(GstElement *src, int trackId)
{
	m_pPlayerSocInterfaceImpl->SetAC4Tracks(src, trackId);
}

bool SocInterface::SetPlatformPlaybackRate()
{
	return m_pPlayerSocInterfaceImpl->SetPlatformPlaybackRate();
}

bool SocInterface::SetRateCorrection()
{
	return m_pPlayerSocInterfaceImpl->SetRateCorrection();
}

bool SocInterface::IsVideoSink(const char* name, bool isRialto)
{
	return m_pPlayerSocInterfaceImpl->IsVideoSink(name, isRialto);
}

bool SocInterface::IsAudioSinkOrAudioDecoder(const char* name, bool isRialto)
{
	return m_pPlayerSocInterfaceImpl->IsAudioSinkOrAudioDecoder(name, isRialto);
}

bool SocInterface::IsVideoDecoder(const char* name, bool isRialto)
{
	return m_pPlayerSocInterfaceImpl->IsVideoDecoder(name, isRialto);
}

bool SocInterface::ConfigureAudioSink(GstElement **audio_sink, GstObject *src, bool decStreamSync)
{
	return m_pPlayerSocInterfaceImpl->ConfigureAudioSink(audio_sink, src, decStreamSync);
}

bool SocInterface::IsAudioOrVideoDecoder(const char* name, bool isRialto)
{
	return m_pPlayerSocInterfaceImpl->IsAudioOrVideoDecoder(name, isRialto);
}

bool SocInterface::DisableAsyncAudio(GstElement *audio_sink, int rate, bool isSeeking)
{
	return m_pPlayerSocInterfaceImpl->DisableAsyncAudio(audio_sink, rate, isSeeking);
}

void SocInterface::GetCCDecoderHandle(gpointer *dec_handle, GstElement *video_dec)
{
	m_pPlayerSocInterfaceImpl->GetCCDecoderHandle(dec_handle, video_dec);
}

bool SocInterface::ResetTrickUTC()
{
	return m_pPlayerSocInterfaceImpl->ResetTrickUTC();
}

long long SocInterface::GetVideoPts(GstElement *video_sink, GstElement *video_dec, bool isWesteros)
{
	return m_pPlayerSocInterfaceImpl->GetVideoPts(video_sink, video_dec, isWesteros);
}

bool SocInterface::NotifyVideoFirstFrame()
{
	return m_pPlayerSocInterfaceImpl->NotifyVideoFirstFrame();
}

void SocInterface::SetDecodeError(GstObject* src)
{
	m_pPlayerSocInterfaceImpl->SetDecodeError(src);
}

void SocInterface::SetFreerunThreshold(GstObject* src)
{
	m_pPlayerSocInterfaceImpl->SetFreerunThreshold(src);
}

bool SocInterface::RequiredElementSetup()
{
	return m_pPlayerSocInterfaceImpl->RequiredElementSetup();
}

void SocInterface::SetAudioRoutingProperties(GstElement *source)
{
	m_pPlayerSocInterfaceImpl->SetAudioRoutingProperties(source);
}

bool SocInterface::HasFirstAudioFrameCallback()
{
	return m_pPlayerSocInterfaceImpl->HasFirstAudioFrameCallback();
}

bool SocInterface::IsVideoSinkHandleErrors()
{
	return m_pPlayerSocInterfaceImpl->IsVideoSinkHandleErrors();
}

void SocInterface::SetPlaybackFlags(gint &flags, bool isSub)
{
	m_pPlayerSocInterfaceImpl->SetPlaybackFlags(flags, isSub);
}

bool SocInterface::IsSimulatorFirstFrame()
{
	return m_pPlayerSocInterfaceImpl->IsSimulatorFirstFrame();
}

bool SocInterface::IsSimulatorSink()
{
	return m_pPlayerSocInterfaceImpl->IsSimulatorSink();
}

void SocInterface::ConfigurePluginPriority()
{
	m_pPlayerSocInterfaceImpl->ConfigurePluginPriority();
}

bool SocInterface::ShouldTearDownForTrickplay()
{
	return m_pPlayerSocInterfaceImpl->ShouldTearDownForTrickplay();
}

bool SocInterface::IsSimulatorVideoSample()
{
	return m_pPlayerSocInterfaceImpl->IsSimulatorVideoSample();
}

void SocInterface::SetH264Caps(GstCaps *caps)
{
	m_pPlayerSocInterfaceImpl->SetH264Caps(caps);
}

void SocInterface::SetHevcCaps(GstCaps *caps)
{
	m_pPlayerSocInterfaceImpl->SetHevcCaps(caps);
}

bool SocInterface::ResetNewSegmentEvent()
{
	return m_pPlayerSocInterfaceImpl->ResetNewSegmentEvent();
}

bool SocInterface::IsPlatformSegmentReady()
{
	return m_pPlayerSocInterfaceImpl->IsPlatformSegmentReady();
}

bool SocInterface::IsVideoMaster()
{
	return m_pPlayerSocInterfaceImpl->IsVideoMaster();
}