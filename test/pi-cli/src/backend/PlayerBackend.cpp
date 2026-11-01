/*
 * If not stated otherwise in this file or this component's license file the
 * following copyright and licenses apply:
 *
 * Copyright 2026 RDK Management
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

#include "PlayerBackend.h"

#include "StreamDemuxSource.h"
#include "InterfacePlayerPriv.h"
#include "InterfacePlayerRDK.h"

#include <glib.h>

#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <system_error>
#include <thread>
#include <utility>

namespace {

constexpr int kVideoAppSrcQueueBytes = 15 * 1024 * 1024;
constexpr int kAudioAppSrcQueueBytes = 8 * 1024;

std::chrono::milliseconds AudioQueueDrainAllowance(uint64_t bitrate)
{
    if (bitrate == 0) {
        return std::chrono::milliseconds(1500);
    }

    const uint64_t queueDurationMilliseconds =
        (static_cast<uint64_t>(kAudioAppSrcQueueBytes) * 8 * 1000 +
         bitrate - 1) /
        bitrate;
    return std::chrono::milliseconds(
        std::min<uint64_t>(queueDurationMilliseconds + 250, 1500));
}

bool IsSupportedTrickRate(int rate)
{
    return rate == 1 || rate == 2 || rate == 4 || rate == 6 ||
           rate == 12 || rate == 32;
}

} // namespace

PlayerBackend::PlayerBackend(std::string url)
    : url_(std::move(url))
{
}

PlayerBackend::~PlayerBackend()
{
    Stop();
}

bool PlayerBackend::Start()
{
    startupAt_ = std::chrono::steady_clock::now();
    LogStartupMilestone("start");
    LogStartupMilestone("player-interface-plugin-initialization-begin");
    InterfacePlayerRDK::InitializePlayerGstreamerPlugins();
    LogStartupMilestone("player-interface-plugin-initialization-complete");

    source_ = std::make_unique<StreamDemuxSource>();
    LogStartupMilestone("ffmpeg-manifest-open-begin");
    if (!source_->Open(url_, [this](const std::string& milestone) {
            LogStartupMilestone(milestone);
        })) {
        std::string error;
        source_->TakeError(error);
        statusMessage_ = error.empty() ? "Could not open the stream"
                                       : "FFmpeg ingest failed: " + error;
        LogStartupMilestone(statusMessage_);
        return false;
    }
    activeAudioSource_ = source_->HasTrack(eGST_MEDIATYPE_AUDIO)
        ? source_.get()
        : nullptr;
    SetActiveVideoSource(source_.get());
    LogStartupMilestone("ffmpeg-manifest-and-stream-info-ready");
    for (const MediaTrackInfo& track : source_->GetTrackCatalog()) {
        if (!track.selected) {
            continue;
        }
        if (track.type == MediaTrackType::Audio) {
            selection_.audioTrackId = track.id;
        } else {
            selection_.videoTrackId = track.id;
        }
    }

    player_ = std::make_unique<InterfacePlayerRDK>();
    player_->SetPlayerName("pi-player-simulator");

    Configs* config = player_->m_gstConfigParam;
    config->media = source_->GetMediaMode();
    config->appSrcForProgressivePlayback = false;
    config->gstreamerSubsEnabled = false;
    config->tcpServerSink = false;
    config->useWesterosSink = false;
    config->useRialtoSink = false;
    config->gstreamerBufferingBeforePlay = false;
    config->monitorAV = true;
    config->monitorAvsyncThresholdPositiveMs = 500;
    config->monitorAvsyncThresholdNegativeMs = -500;
    config->monitorAvJumpThresholdMs = 5000;
    config->videoBufBytes = kVideoAppSrcQueueBytes;
    config->audioBufBytes = kAudioAppSrcQueueBytes;
    LogStartupMilestone(
        "player-interface-appsrc-buffer-limits: video=" +
        std::to_string(kVideoAppSrcQueueBytes) + " audio=" +
        std::to_string(kAudioAppSrcQueueBytes));

    player_->TearDownCallback([](bool, int) {});
    player_->StopCallback([](bool) {});
    player_->setupStreamCallbackMap[InterfaceCB::startNewSubtitleStream] =
        [](int) {};
    player_->callbackMap[InterfaceCB::idleCb] = [] {};
    player_->callbackMap[InterfaceCB::progressCb] = [] {};
    player_->callbackMap[InterfaceCB::firstVideoFrameReceived] = [] {};
    player_->callbackMap[InterfaceCB::notifyEOS] = [] {};
    player_->callbackMap[InterfaceCB::firstVideoFrameDisplayed] = [this] {
        if (!firstVideoFrameDisplayedReported_.exchange(true)) {
            LogStartupMilestone(
                "interface-first-video-frame-displayed-callback");
        }
    };
    player_->RegisterNeedDataCb([this](int mediaType) {
        RouteNeedData(mediaType);
    });
    player_->RegisterEnoughDataCb([this](int mediaType) {
        RouteEnoughData(mediaType);
    });
    player_->FirstFrameCallback(
        [this](int mediaType,
               bool,
               bool,
               bool&,
               bool&) {
            if (mediaType == eGST_MEDIATYPE_VIDEO &&
                !firstVideoFrameReported_.exchange(true)) {
                LogStartupMilestone("player-interface-first-video-frame");
            }
        });
    player_->RegisterBusEvent([this](const BusEventData event) {
        HandleBusEvent(event);
    });

    StreamCodecInfo codecInfo = source_->GetCodecInfo();
    LogStartupMilestone("configure-player-interface-begin");
    player_->ConfigurePipeline(
        std::move(codecInfo),
        false,
        false,
        false,
        0,
        1,
        "pi-player-simulator",
        -1,
        true,
        url_);
    LogStartupMilestone("configure-player-interface-returned");

    StreamCodecInfo codecCaps = source_->GetCodecInfo();
    if (source_->HasTrack(eGST_MEDIATYPE_VIDEO)) {
        player_->SetStreamCaps(eGST_MEDIATYPE_VIDEO,
                               std::move(codecCaps.video));
    }
    if (source_->HasTrack(eGST_MEDIATYPE_AUDIO)) {
        player_->SetStreamCaps(eGST_MEDIATYPE_AUDIO,
                               std::move(codecCaps.audio));
    }
    LogStartupMilestone("player-interface-stream-caps-set");

    if (!StartSource()) {
        Stop();
        return false;
    }

    playing_ = true;
    statusMessage_ = "Opening stream";
    latestVideoTimestampMsForAudioSwitch_ =
        GetVideoTimestampMilliseconds();
    nextPositionQuery_ = std::chrono::steady_clock::now();
    nextDurationQuery_ = nextPositionQuery_;
    nextProgressLog_ = nextPositionQuery_;
    nextAbrEvaluation_ = nextPositionQuery_ + std::chrono::seconds(3);
    nextAudioPrefetch_ = nextPositionQuery_;
    StartAudioTrackPrewarming();
    return true;
}

void PlayerBackend::StartAudioTrackPrewarming()
{
    if (source_ == nullptr || trickModeSource_ != nullptr || stopped_ ||
        audioSwitchInProgress_ ||
        audioWarmupRunning_ || trickWarmupRunning_ ||
        pendingTrickRate_ > 1 || !source_->IsDashMedia()) {
        return;
    }
    {
        const std::lock_guard<std::mutex> lock(trickWarmupMutex_);
        if (preparedTrickModeSource_ != nullptr) {
            return;
        }
    }
    if (audioWarmupThread_.joinable()) {
        audioWarmupThread_.join();
    }

    const std::vector<uint8_t> manifestSnapshot =
        source_->GetDashManifestSnapshot();
    if (manifestSnapshot.empty()) {
        return;
    }

    const std::vector<MediaTrackInfo> trackCatalog =
        source_->GetTrackCatalog();
    std::vector<MediaTrackInfo> audioTracks;
    audioTracks.reserve(trackCatalog.size());
    const MediaTrackInfo* preferredTrack = nullptr;
    const MediaTrackInfo* fallbackTrack = nullptr;
    for (const MediaTrackInfo& track : trackCatalog) {
        if (track.type == MediaTrackType::Audio &&
            track.id != selection_.audioTrackId) {
            audioTracks.push_back(track);
            if (track.id == preferredAudioPrewarmId_ &&
                track.id != audioTrackPrewarmAttemptedId_) {
                preferredTrack = &audioTracks.back();
            } else if (track.id != audioTrackPrewarmAttemptedId_ &&
                       fallbackTrack == nullptr) {
                fallbackTrack = &audioTracks.back();
            }
        }
    }
    const MediaTrackInfo* selectedTrack =
        preferredTrack != nullptr ? preferredTrack : fallbackTrack;
    if (selectedTrack == nullptr) {
        return;
    }
    const MediaTrackInfo nextTrack = *selectedTrack;

    std::vector<std::unique_ptr<StreamDemuxSource>> stalePreparedSources;
    std::vector<std::string> stalePreparedTrackIds;
    bool prepared = false;
    bool cacheOccupied = false;
    const bool replacingCache =
        nextTrack.id == preferredAudioPrewarmId_;
    {
        const std::lock_guard<std::mutex> lock(audioSwitchMutex_);
        for (auto entry = preparedAudioSources_.begin();
             entry != preparedAudioSources_.end();) {
            if (entry->first == nextTrack.id) {
                prepared = true;
                ++entry;
            } else if (replacingCache) {
                stalePreparedTrackIds.push_back(entry->first);
                stalePreparedSources.push_back(std::move(entry->second));
                entry = preparedAudioSources_.erase(entry);
            } else {
                cacheOccupied = true;
                ++entry;
            }
        }
    }
    for (size_t index = 0; index < stalePreparedSources.size(); ++index) {
        LogStartupMilestone("audio-track-prewarm-evict: track=" +
                            stalePreparedTrackIds[index] +
                            " preferred=" + nextTrack.id);
        QueueAudioSourceCleanup(std::move(stalePreparedSources[index]));
    }
    if (prepared || cacheOccupied) {
        return;
    }
    if (replacingCache) {
        LogStartupMilestone("audio-track-prewarm-priority: track=" +
                            nextTrack.id);
    }

    const std::string manifestBaseUrl =
        source_->GetDashManifestBaseUrl();
    const MediaTrackSelection baseSelection = selection_;
    cancelAudioWarmup_ = false;
    audioTrackPrewarmAttemptedId_ = nextTrack.id;
    audioWarmupRunning_ = true;
    try {
        audioWarmupThread_ = std::thread(
            [this,
             track = nextTrack,
             manifestSnapshot,
             manifestBaseUrl,
             baseSelection]() mutable {
                MediaTrackSelection selection = baseSelection;
                selection.audioTrackId = track.id;
                auto candidate = std::make_unique<StreamDemuxSource>();
                const bool opened = candidate->Open(
                    url_,
                    [this](const std::string& milestone) {
                        LogStartupMilestone(milestone);
                    },
                    selection,
                    true,
                    &manifestSnapshot,
                    manifestBaseUrl,
                    &cancelAudioWarmup_);
                if (!opened) {
                    std::string error;
                    candidate->TakeError(error);
                    if (!cancelAudioWarmup_) {
                        LogStartupMilestone(
                            "audio-track-prewarm-failed: track=" +
                            track.id + " error=" + error);
                    }
                } else if (cancelAudioWarmup_) {
                    candidate->Stop();
                } else {
                    const double positionSeconds =
                        static_cast<double>(
                            latestVideoTimestampMsForAudioSwitch_.load()) /
                        1000.0;
                    candidate->PrepareSeek(positionSeconds);
                    const std::lock_guard<std::mutex> lock(
                        audioSwitchMutex_);
                    if (cancelAudioWarmup_) {
                        candidate->Stop();
                    } else {
                        preparedAudioSources_[track.id] =
                            std::move(candidate);
                        LogStartupMilestone(
                            "audio-track-prewarm-ready: track=" + track.id);
                    }
                }
                audioWarmupRunning_ = false;
                LogStartupMilestone("audio-track-prewarming-complete");
            });
    } catch (const std::system_error& error) {
        audioWarmupRunning_ = false;
        LogStartupMilestone(
            "audio-track-prewarming-unavailable: " +
            std::string(error.what()));
    }
}

void PlayerBackend::StartTrickModePrewarming()
{
    if (source_ == nullptr || player_ == nullptr || stopped_ ||
        trickModeSource_ != nullptr || audioWarmupRunning_ ||
        trickWarmupRunning_ || trickWarmupAttempted_ ||
        (!playing_ && pendingTrickRate_ == 0) ||
        (!firstVideoFrameDisplayedReported_ && pendingTrickRate_ == 0) ||
        !source_->CanUseTrickMode()) {
        return;
    }

    if (trickWarmupThread_.joinable()) {
        trickWarmupThread_.join();
    }

    const std::vector<uint8_t> manifestSnapshot =
        source_->GetDashManifestSnapshot();
    const std::string manifestBaseUrl =
        source_->GetDashManifestBaseUrl();
    if (manifestSnapshot.empty()) {
        trickWarmupAttempted_ = true;
        std::lock_guard<std::mutex> lock(trickWarmupMutex_);
        trickWarmupError_ =
            "The DASH manifest is unavailable for trick-mode preparation";
        LogStartupMilestone("trick-mode-prewarm-failed: " +
                            trickWarmupError_);
        return;
    }

    trickWarmupAttempted_ = true;
    cancelTrickWarmup_ = false;
    trickWarmupRunning_ = true;
    {
        const std::lock_guard<std::mutex> lock(trickWarmupMutex_);
        trickWarmupError_.clear();
    }
    LogStartupMilestone("trick-mode-prewarm-begin");
    try {
        trickWarmupThread_ = std::thread(
            [this, manifestSnapshot, manifestBaseUrl]() mutable {
                auto candidate = std::make_unique<StreamDemuxSource>();
                std::string error;
                const bool opened = candidate->Open(
                    url_,
                    [this](const std::string& milestone) {
                        LogStartupMilestone(milestone);
                    },
                    {},
                    false,
                    &manifestSnapshot,
                    manifestBaseUrl,
                    &cancelTrickWarmup_,
                    true);
                if (!opened) {
                    candidate->TakeError(error);
                } else if (cancelTrickWarmup_) {
                    candidate->Stop();
                } else {
                    const double positionSeconds =
                        static_cast<double>(
                            latestVideoTimestampMsForAudioSwitch_.load()) /
                        1000.0;
                    if (!candidate->PrepareSeek(positionSeconds)) {
                        error =
                            "Could not prefetch the DASH trick-mode track";
                    }
                }

                std::unique_ptr<StreamDemuxSource> discarded;
                {
                    const std::lock_guard<std::mutex> lock(
                        trickWarmupMutex_);
                    if (cancelTrickWarmup_) {
                        discarded = std::move(candidate);
                    } else if (!error.empty()) {
                        trickWarmupError_ = error;
                    } else {
                        preparedTrickModeSource_ = std::move(candidate);
                    }
                }
                if (discarded != nullptr) {
                    discarded->Stop();
                }
                trickWarmupRunning_ = false;
                if (!cancelTrickWarmup_) {
                    LogStartupMilestone(
                        error.empty()
                            ? "trick-mode-prewarm-ready"
                            : "trick-mode-prewarm-failed: " + error);
                }
            });
    } catch (const std::system_error& error) {
        trickWarmupRunning_ = false;
        std::lock_guard<std::mutex> lock(trickWarmupMutex_);
        trickWarmupError_ =
            "Could not start trick-mode preparation: " +
            std::string(error.what());
        LogStartupMilestone("trick-mode-prewarm-failed: " +
                            trickWarmupError_);
    }
}

void PlayerBackend::StopTrickModePrewarming()
{
    cancelTrickWarmup_ = true;
    if (trickWarmupThread_.joinable()) {
        trickWarmupThread_.join();
    }

    std::unique_ptr<StreamDemuxSource> preparedSource;
    {
        const std::lock_guard<std::mutex> lock(trickWarmupMutex_);
        preparedSource = std::move(preparedTrickModeSource_);
    }
    if (preparedSource != nullptr) {
        preparedSource->Stop();
    }
}

bool PlayerBackend::StartSource()
{
    return source_ != nullptr && StartSource(*source_);
}

bool PlayerBackend::StartSource(StreamDemuxSource& source)
{
    if (!source.Start(
            [this](int mediaType, MediaSample&& sample,
                   bool& discontinuity) {
                bool notifyFirstBufferProcessed = false;
                bool sendNewSegmentEvent = discontinuity;
                bool resetTrickUtc = false;
                bool firstBufferPushed = false;
                const bool accepted = player_->SendHelper(
                    mediaType,
                    std::move(sample),
                    false,
                    discontinuity,
                    notifyFirstBufferProcessed,
                    sendNewSegmentEvent,
                    resetTrickUtc,
                    firstBufferPushed);
                std::atomic<bool>* firstBufferReported =
                    mediaType == eGST_MEDIATYPE_VIDEO
                        ? &firstVideoBufferReported_
                        : &firstAudioBufferReported_;
                if (notifyFirstBufferProcessed &&
                    !firstBufferReported->exchange(true)) {
                    LogStartupMilestone(
                        mediaType == eGST_MEDIATYPE_VIDEO
                            ? "player-interface-first-video-buffer-processed"
                            : "player-interface-first-audio-buffer-processed");
                }
                if (resetTrickUtc) {
                    LogStartupMilestone("player-interface-trick-timestamp-reset");
                }
                return accepted;
            },
            [this, &source](int mediaType, uint64_t seekGeneration) {
                if (seekGeneration != source.GetSeekGeneration()) {
                    LogStartupMilestone(
                        "player-interface-stale-source-eos-ignored");
                    return;
                }
                pendingPlayerEos_ = true;
                bool shouldHaltBuffering = false;
                player_->EndOfStreamReached(mediaType, shouldHaltBuffering);
            })) {
        std::string error;
        source.TakeError(error);
        statusMessage_ = error.empty() ? "Could not start FFmpeg ingest"
                                       : "FFmpeg ingest failed: " + error;
        LogStartupMilestone(statusMessage_);
        return false;
    }
    return true;
}

void PlayerBackend::RouteNeedData(int mediaType)
{
    if (mediaType == eGST_MEDIATYPE_AUDIO) {
        const std::lock_guard<std::mutex> lock(audioRoutingMutex_);
        if (activeAudioSource_ != nullptr) {
            activeAudioSource_->NeedData(mediaType);
        }
        return;
    }
    const std::lock_guard<std::mutex> lock(videoRoutingMutex_);
    if (activeVideoSource_ != nullptr) {
        activeVideoSource_->NeedData(mediaType);
    }
}

void PlayerBackend::RouteEnoughData(int mediaType)
{
    if (mediaType == eGST_MEDIATYPE_AUDIO) {
        const std::lock_guard<std::mutex> lock(audioRoutingMutex_);
        if (activeAudioSource_ != nullptr) {
            activeAudioSource_->EnoughData(mediaType);
        }
        return;
    }
    const std::lock_guard<std::mutex> lock(videoRoutingMutex_);
    if (activeVideoSource_ != nullptr) {
        activeVideoSource_->EnoughData(mediaType);
    }
}

void PlayerBackend::SetActiveAudioSource(StreamDemuxSource* source)
{
    const std::lock_guard<std::mutex> lock(audioRoutingMutex_);
    activeAudioSource_ = source;
}

void PlayerBackend::SetActiveVideoSource(StreamDemuxSource* source)
{
    const std::lock_guard<std::mutex> lock(videoRoutingMutex_);
    activeVideoSource_ = source;
}

long long PlayerBackend::GetVideoTimestampMilliseconds() const
{
    if (player_ == nullptr) {
        return 0;
    }
    if (trickRate_ > 1) {
        return GetPlaybackPositionMilliseconds();
    }
    if (trickPlayPositionRebasePending_) {
        return positionMs_;
    }

    const long long videoPts = player_->GetVideoPTS();
    return videoPts > 0 ? videoPts / 90 + positionOffsetMs_
                        : GetPlaybackPositionMilliseconds();
}

long long PlayerBackend::GetPlaybackPositionMilliseconds() const
{
    if (player_ == nullptr) {
        return positionMs_;
    }
    if (trickPlayPositionRebasePending_) {
        return positionMs_;
    }

    const long long position = player_->GetPositionMilliseconds();
    if (position > 0 && trickRate_ > 1) {
        return trickPlayPositionBaselineMs_ +
               (position - trickPlayPositionBaselineMs_) * trickRate_ +
               positionOffsetMs_;
    }
    return position > 0 ? position + positionOffsetMs_ : positionMs_;
}

void PlayerBackend::LogVideoDiagnostics(const std::string& stage)
{
    if (player_ == nullptr) {
        return;
    }

    long long interfacePositionMilliseconds =
        player_->GetPositionMilliseconds();
    if (interfacePositionMilliseconds > 0 && trickRate_ > 1) {
        interfacePositionMilliseconds =
            trickPlayPositionBaselineMs_ +
            (interfacePositionMilliseconds -
             trickPlayPositionBaselineMs_) * trickRate_;
    }
    const long long videoSinkPositionMilliseconds =
        player_->GetVideoPosition();
    const long long videoPts90k = player_->GetVideoPTS();
    const MonitorAVState& monitorState = player_->GetMonitorAVState();
    const GstPlaybackQualityStruct* quality =
        player_->GetVideoPlaybackQuality();

    std::string diagnostic =
        "audio-switch-video-diagnostic: stage=" + stage +
        " playing=" + (playing_ ? "yes" : "no") +
        " pipeline-playing-notified=" +
        (pipelinePlayingNotified_.load() ? "yes" : "no") +
        " interface-position-ms=" +
        std::to_string(interfacePositionMilliseconds) +
        " adjusted-position-ms=" + std::to_string(positionMs_) +
        " video-sink-position-ms=" +
        std::to_string(videoSinkPositionMilliseconds) +
        " video-pts-90khz=" + std::to_string(videoPts90k);
    if (videoPts90k > 0) {
        diagnostic += " video-pts-ms=" + std::to_string(videoPts90k / 90);
    }
    diagnostic +=
        " monitor-av-state=" +
        std::string(monitorState.description == nullptr
                        ? "unavailable"
                        : monitorState.description) +
        " monitor-video-ms=" +
        std::to_string(monitorState.av_position[eGST_MEDIATYPE_VIDEO]) +
        " monitor-audio-ms=" +
        std::to_string(monitorState.av_position[eGST_MEDIATYPE_AUDIO]) +
        " monitor-happy=" + (monitorState.happy ? "yes" : "no");
    if (quality != nullptr) {
        diagnostic += " frames-rendered=" +
                      std::to_string(quality->rendered) +
                      " frames-dropped=" +
                      std::to_string(quality->dropped);
    } else {
        diagnostic += " frame-stats=unavailable";
    }
    LogStartupMilestone(diagnostic);
}

void PlayerBackend::LogTrickPlayPipelineState(const std::string& stage)
{
    if (player_ == nullptr) {
        return;
    }

    InterfacePlayerPriv* privatePlayer = player_->GetPrivatePlayer();
    GstPlayerPriv* context =
        privatePlayer == nullptr ? nullptr : privatePlayer->gstPrivateContext;
    if (context == nullptr || context->pipeline == nullptr) {
        LogStartupMilestone(
            "trick-play-pipeline-state: stage=" + stage +
            " pipeline=unavailable");
        return;
    }

    GstState current = GST_STATE_NULL;
    GstState pending = GST_STATE_VOID_PENDING;
    const GstStateChangeReturn result =
        gst_element_get_state(context->pipeline, &current, &pending, 0);

    auto readQueuedFrames = [](GstElement* decoder, gint& frames) {
        if (decoder != nullptr &&
            g_object_class_find_property(
                G_OBJECT_GET_CLASS(decoder), "queued_frames") != nullptr) {
            guint queuedFrames = 0;
            g_object_get(decoder, "queued_frames", &queuedFrames, nullptr);
            frames = static_cast<gint>(queuedFrames);
        }
    };
    auto readAppSrcBytes = [](GstElement* source, guint64& bytes) {
        if (source != nullptr &&
            g_object_class_find_property(
                G_OBJECT_GET_CLASS(source), "current-level-bytes") != nullptr) {
            g_object_get(source, "current-level-bytes", &bytes, nullptr);
        }
    };
    auto elementState = [](const char* label, GstElement* element) {
        if (element == nullptr) {
            return std::string(" ") + label + "=unavailable";
        }
        GstState elementCurrent = GST_STATE_NULL;
        GstState elementPending = GST_STATE_VOID_PENDING;
        const GstStateChangeReturn elementResult =
            gst_element_get_state(
                element, &elementCurrent, &elementPending, 0);
        return std::string(" ") + label + "=" +
               gst_element_state_get_name(elementCurrent) + "/" +
               gst_element_state_get_name(elementPending) + "(" +
               std::to_string(elementResult) + ")";
    };

    gint videoQueuedFrames = -1;
    gint audioQueuedFrames = -1;
    guint64 videoAppSrcBytes = 0;
    guint64 audioAppSrcBytes = 0;
    readQueuedFrames(context->video_dec, videoQueuedFrames);
    readQueuedFrames(context->audio_dec, audioQueuedFrames);
    readAppSrcBytes(context->stream[eGST_MEDIATYPE_VIDEO].source,
                    videoAppSrcBytes);
    readAppSrcBytes(context->stream[eGST_MEDIATYPE_AUDIO].source,
                    audioAppSrcBytes);

    std::string diagnostic =
        "trick-play-pipeline-state: stage=" + stage +
        " current=" + gst_element_state_get_name(current) +
        " pending=" + gst_element_state_get_name(pending) +
        " state-result=" + std::to_string(result) +
        " paused-flag=" + (context->paused ? "yes" : "no") +
        " pending-play=" + (context->pendingPlayState ? "yes" : "no") +
        " buffering=" +
        (context->buffering_in_progress ? "yes" : "no") +
        " video-appsrc-bytes=" + std::to_string(videoAppSrcBytes) +
        " audio-appsrc-bytes=" + std::to_string(audioAppSrcBytes) +
        " video-decoder-queued-frames=" +
        std::to_string(videoQueuedFrames) +
        " audio-decoder-queued-frames=" +
        std::to_string(audioQueuedFrames) +
        " first-video-buffer=" +
        (context->stream[eGST_MEDIATYPE_VIDEO].firstBufferProcessed
             ? "yes"
             : "no") +
        " first-audio-buffer=" +
        (context->stream[eGST_MEDIATYPE_AUDIO].firstBufferProcessed
             ? "yes"
             : "no") +
        " first-video-frame=" +
        (context->firstVideoFrameReceived ? "yes" : "no") +
        " first-audio-frame=" +
        (context->firstAudioFrameReceived ? "yes" : "no") +
        " video-eos=" +
        (context->stream[eGST_MEDIATYPE_VIDEO].eosReached ? "yes" : "no") +
        " audio-eos=" +
        (context->stream[eGST_MEDIATYPE_AUDIO].eosReached ? "yes" : "no");
    diagnostic += elementState("video-dec", context->video_dec);
    diagnostic += elementState("video-sink", context->video_sink);
    diagnostic += elementState("video-sinkbin",
                               context->stream[eGST_MEDIATYPE_VIDEO].sinkbin);
    diagnostic += elementState("audio-dec", context->audio_dec);
    diagnostic += elementState("audio-sink", context->audio_sink);
    diagnostic += elementState("audio-sinkbin",
                               context->stream[eGST_MEDIATYPE_AUDIO].sinkbin);
    LogStartupMilestone(diagnostic);
}

void PlayerBackend::ConfigurePlayerForSource(StreamDemuxSource& source,
                                             double positionSeconds)
{
    LogStartupMilestone("player-interface-selection-reconfigure-begin");
    if (!source.HasTrack(eGST_MEDIATYPE_AUDIO)) {
        player_->TearDownStream(eGST_MEDIATYPE_AUDIO);
        LogStartupMilestone(
            "player-interface-video-only-audio-branch-torn-down");
    }
    StreamCodecInfo codecInfo = source.GetCodecInfo();
    // Flush EOS-signals the audio appsrc, so rebuild that stream before reuse.
    player_->ConfigurePipeline(
        std::move(codecInfo),
        true,
        true,
        false,
        0,
        1,
        "pi-player-simulator",
        -1,
        true,
        url_);

    StreamCodecInfo codecCaps = source.GetCodecInfo();
    if (source.HasTrack(eGST_MEDIATYPE_VIDEO)) {
        player_->SetStreamCaps(eGST_MEDIATYPE_VIDEO,
                               std::move(codecCaps.video));
    }
    if (source.HasTrack(eGST_MEDIATYPE_AUDIO)) {
        player_->SetStreamCaps(eGST_MEDIATYPE_AUDIO,
                               std::move(codecCaps.audio));
    }
    if (activeAudioSource_ != nullptr &&
        activeAudioSource_ != &source) {
        StreamCodecInfo audioCodecInfo = activeAudioSource_->GetCodecInfo();
        player_->SetStreamCaps(eGST_MEDIATYPE_AUDIO,
                               std::move(audioCodecInfo.audio));
    }
    // ConfigurePipeline rebuilds the pipeline with a zero-based segment, and
    // Flush() already consumed the pending seek. Re-arm it so the first buffer
    // starts the segment at the resume position instead of waiting for PTS.
    player_->SetSeekPosition(positionSeconds);
    LogStartupMilestone(
        "player-interface-selection-segment-armed: position=" +
        std::to_string(positionSeconds));
    LogStartupMilestone("player-interface-selection-reconfigure-complete");
}

void PlayerBackend::CompleteAudioSwitchIfReady()
{
    std::unique_ptr<StreamDemuxSource> candidate;
    std::string error;
    double positionSeconds = 0.0;
    bool useMainSource = false;
    bool beginDrain = false;
    const auto now = std::chrono::steady_clock::now();
    {
        const std::lock_guard<std::mutex> lock(audioSwitchMutex_);
        if (!audioSwitchReady_) {
            return;
        }
        if (!audioSwitchError_.empty()) {
            candidate = std::move(preparedAudioSource_);
            error = std::move(audioSwitchError_);
            useMainSource = audioSwitchUseMainSource_;
            audioSwitchReady_ = false;
            audioSwitchUseMainSource_ = false;
            audioSwitchDrainStarted_ = false;
        } else if (!audioSwitchDrainStarted_) {
            if (!playing_ || source_ == nullptr) {
                return;
            }
            uint64_t currentAudioBitrate = 0;
            for (const MediaTrackInfo& track : source_->GetTrackCatalog()) {
                if (track.type == MediaTrackType::Audio &&
                    track.id == selection_.audioTrackId) {
                    currentAudioBitrate = track.bitrate;
                    break;
                }
            }
            audioSwitchDrainStarted_ = true;
            audioSwitchDrainRemaining_ =
                AudioQueueDrainAllowance(currentAudioBitrate);
            audioSwitchDrainLastUpdate_ = now;
            useMainSource = audioSwitchUseMainSource_;
            beginDrain = true;
        } else {
            if (playing_) {
                const auto elapsed =
                    std::chrono::duration_cast<std::chrono::milliseconds>(
                        now - audioSwitchDrainLastUpdate_);
                audioSwitchDrainRemaining_ =
                    elapsed >= audioSwitchDrainRemaining_
                        ? std::chrono::milliseconds::zero()
                        : audioSwitchDrainRemaining_ - elapsed;
            }
            audioSwitchDrainLastUpdate_ = now;
            if (audioSwitchDrainRemaining_ > std::chrono::milliseconds::zero()) {
                return;
            }
            candidate = std::move(preparedAudioSource_);
            error = std::move(audioSwitchError_);
            positionSeconds = preparedAudioPositionSeconds_;
            useMainSource = audioSwitchUseMainSource_;
            audioSwitchReady_ = false;
            audioSwitchUseMainSource_ = false;
            audioSwitchDrainStarted_ = false;
        }
    }
    if (audioSwitchThread_.joinable()) {
        audioSwitchThread_.join();
    }

    if (beginDrain) {
        if (stopped_ || player_ == nullptr) {
            audioSwitchInProgress_ = false;
            previousAudioSource_ = nullptr;
            return;
        }

        if (!useMainSource &&
            (preparedAudioSource_ == nullptr ||
             !preparedAudioSource_->HasTrack(eGST_MEDIATYPE_AUDIO))) {
            std::unique_ptr<StreamDemuxSource> failedCandidate;
            {
                const std::lock_guard<std::mutex> lock(audioSwitchMutex_);
                failedCandidate = std::move(preparedAudioSource_);
                audioSwitchReady_ = false;
                audioSwitchUseMainSource_ = false;
                audioSwitchDrainStarted_ = false;
            }
            if (failedCandidate != nullptr) {
                failedCandidate->Stop();
            }
            RestoreAudioAfterSwitchFailure(
                "The requested DASH adaptation has no audio stream");
            return;
        }

        const long long currentVideoTimestampMilliseconds =
            GetVideoTimestampMilliseconds();
        if (currentVideoTimestampMilliseconds > 0) {
            positionSeconds =
                static_cast<double>(currentVideoTimestampMilliseconds) / 1000.0;
            latestVideoTimestampMsForAudioSwitch_ =
                currentVideoTimestampMilliseconds;
        }
        LogVideoDiagnostics("before-audio-drain");
        SetActiveAudioSource(nullptr);
        StreamDemuxSource* previousAudioSource = previousAudioSource_;
        if (previousAudioSource != nullptr) {
            previousAudioSource->SetTrackEnabled(eGST_MEDIATYPE_AUDIO, false);
        }
        if (useMainSource) {
            if (source_ == nullptr ||
                !source_->HasTrack(eGST_MEDIATYPE_AUDIO)) {
                {
                    const std::lock_guard<std::mutex> lock(audioSwitchMutex_);
                    audioSwitchReady_ = false;
                    audioSwitchUseMainSource_ = false;
                    audioSwitchDrainStarted_ = false;
                    audioSwitchDrainRemaining_ =
                        std::chrono::milliseconds::zero();
                }
                RestoreAudioAfterSwitchFailure(
                    "The original audio source is no longer available");
                return;
            }
            source_->SetDiscardBeforePosition(eGST_MEDIATYPE_AUDIO,
                                              positionSeconds);
        } else {
            preparedAudioSource_->SetDiscardBeforePosition(
                eGST_MEDIATYPE_AUDIO, positionSeconds);
            if (!StartSource(*preparedAudioSource_)) {
                const std::string startError = statusMessage_;
                std::unique_ptr<StreamDemuxSource> failedCandidate;
                {
                    const std::lock_guard<std::mutex> lock(audioSwitchMutex_);
                    failedCandidate = std::move(preparedAudioSource_);
                    audioSwitchReady_ = false;
                    audioSwitchUseMainSource_ = false;
                    audioSwitchDrainStarted_ = false;
                }
                if (failedCandidate != nullptr) {
                    failedCandidate->Stop();
                }
                RestoreAudioAfterSwitchFailure(startError);
                return;
            }
        }

        LogStartupMilestone(
            "audio-track-switch-draining-old-audio: appsrc-bytes=" +
            std::to_string(kAudioAppSrcQueueBytes) +
            " drain-ms=" +
            std::to_string(audioSwitchDrainRemaining_.count()));
        statusMessage_ = "Switching audio; draining queued audio";
        return;
    }

    if (stopped_ || player_ == nullptr) {
        if (candidate != nullptr) {
            candidate->Stop();
        }
        audioSwitchInProgress_ = false;
        previousAudioSource_ = nullptr;
        return;
    }
    if (!useMainSource && candidate == nullptr) {
        RestoreAudioAfterSwitchFailure(
            error.empty() ? "Could not prepare the requested audio track"
                          : error);
        return;
    }
    if (useMainSource &&
        (source_ == nullptr || !source_->HasTrack(eGST_MEDIATYPE_AUDIO))) {
        RestoreAudioAfterSwitchFailure(
            "The original audio source is no longer available");
        return;
    }
    if (!useMainSource && !candidate->HasTrack(eGST_MEDIATYPE_AUDIO)) {
        candidate->Stop();
        RestoreAudioAfterSwitchFailure(
            "The requested DASH adaptation has no audio stream");
        return;
    }

    const long long currentVideoTimestampMilliseconds =
        GetVideoTimestampMilliseconds();
    if (currentVideoTimestampMilliseconds > 0) {
        positionSeconds =
            static_cast<double>(currentVideoTimestampMilliseconds) / 1000.0;
        latestVideoTimestampMsForAudioSwitch_ =
            currentVideoTimestampMilliseconds;
    }
    const long long currentPositionMilliseconds =
        GetPlaybackPositionMilliseconds();
    if (currentPositionMilliseconds > 0) {
        positionMs_ = currentPositionMilliseconds;
    }
    audioSwitchPositionBaselineMs_ = positionMs_;
    if (useMainSource) {
        source_->SetDiscardBeforePosition(eGST_MEDIATYPE_AUDIO,
                                          positionSeconds);
    } else {
        candidate->SetDiscardBeforePosition(eGST_MEDIATYPE_AUDIO,
                                           positionSeconds);
    }

    LogVideoDiagnostics("before-audio-handoff");

    StreamCodecInfo codecCaps = useMainSource
        ? source_->GetCodecInfo()
        : candidate->GetCodecInfo();
    player_->SetStreamCaps(eGST_MEDIATYPE_AUDIO,
                           std::move(codecCaps.audio));
    if (useMainSource) {
        source_->SetTrackEnabled(eGST_MEDIATYPE_AUDIO, true);
    }
    LogVideoDiagnostics("after-audio-caps");

    std::unique_ptr<StreamDemuxSource> previousAudioOnly =
        std::move(audioOnlySource_);
    const std::string previousAudioTrackId = selection_.audioTrackId;
    if (useMainSource) {
        SetActiveAudioSource(source_.get());
    } else {
        audioOnlySource_ = std::move(candidate);
        SetActiveAudioSource(audioOnlySource_.get());
    }
    selection_.audioTrackId = requestedAudioTrackId_;
    if (previousAudioSource_ != nullptr &&
        previousAudioSource_ != activeAudioSource_) {
        preferredAudioPrewarmId_ = previousAudioTrackId;
        if (audioWarmupRunning_ &&
            audioTrackPrewarmAttemptedId_ != preferredAudioPrewarmId_) {
            cancelAudioWarmup_ = true;
        } else if (!audioWarmupRunning_ &&
                   audioTrackPrewarmAttemptedId_ ==
                       preferredAudioPrewarmId_) {
            audioTrackPrewarmAttemptedId_.clear();
        }
    }
    if (selection_.audioTrackId == preferredAudioPrewarmId_) {
        preferredAudioPrewarmId_.clear();
    }
    previousAudioSource_ = nullptr;
    audioSwitchInProgress_ = false;
    cancelAudioSwitch_ = false;
    audioSwitchDrainRemaining_ = std::chrono::milliseconds::zero();
    StreamDemuxSource* activeAudioSource = activeAudioSource_;
    if (activeAudioSource != nullptr) {
        activeAudioSource->NeedData(eGST_MEDIATYPE_AUDIO);
    }
    LogVideoDiagnostics("after-audio-source-start");
    const auto diagnosticsStartedAt = std::chrono::steady_clock::now();
    nextAudioSwitchDiagnostic_ = diagnosticsStartedAt;
    audioSwitchDiagnosticsUntil_ =
        diagnosticsStartedAt + std::chrono::seconds(5);

    if (previousAudioOnly != nullptr &&
        previousAudioOnly.get() != activeAudioSource) {
        QueueAudioSourceCleanup(std::move(previousAudioOnly));
    }
    statusMessage_ = "Audio track switch complete: " +
                     selection_.audioTrackId;
    LogStartupMilestone("audio-track-switch-complete: track=" +
                        selection_.audioTrackId +
                        " handoff-video-pts-ms=" +
                        std::to_string(static_cast<long long>(
                            positionSeconds * 1000.0)) +
                        " video-position-ms=" +
                        std::to_string(positionMs_) +
                        " elapsed-ms=" +
                        std::to_string(
                            std::chrono::duration_cast<
                                std::chrono::milliseconds>(
                                std::chrono::steady_clock::now() -
                                audioSwitchStartedAt_)
                                .count()));
}

void PlayerBackend::QueueAudioSourceCleanup(
    std::unique_ptr<StreamDemuxSource> source)
{
    if (source == nullptr) {
        return;
    }

    for (auto task = audioSourceCleanupTasks_.begin();
         task != audioSourceCleanupTasks_.end();) {
        if (!task->done->load()) {
            ++task;
            continue;
        }
        if (task->thread.joinable()) {
            task->thread.join();
        }
        task = audioSourceCleanupTasks_.erase(task);
    }

    audioSourceCleanupTasks_.reserve(audioSourceCleanupTasks_.size() + 1);
    auto done = std::make_shared<std::atomic<bool>>(false);
    std::shared_ptr<StreamDemuxSource> retired(std::move(source));
    AudioSourceCleanupTask task;
    task.done = done;
    try {
        task.thread = std::thread([retired, done] {
            retired->Stop();
            done->store(true);
        });
    } catch (const std::system_error& error) {
        LogStartupMilestone(
            "audio-source-cleanup-thread-failed: " +
            std::string(error.what()));
        retired->Stop();
        return;
    }
    audioSourceCleanupTasks_.push_back(std::move(task));
    LogStartupMilestone("audio-source-cleanup-started");
}

void PlayerBackend::RestoreAudioAfterSwitchFailure(
    const std::string& error,
    bool playerFlushed)
{
    StreamDemuxSource* previousAudioSource = previousAudioSource_;
    previousAudioSource_ = nullptr;
    audioSwitchInProgress_ = false;
    cancelAudioSwitch_ = false;
    bool playbackResumed = true;
    if (previousAudioSource != nullptr && !stopped_) {
        previousAudioSource->SetTrackEnabled(eGST_MEDIATYPE_AUDIO, true);
        SetActiveAudioSource(previousAudioSource);
        if (playerFlushed && player_ != nullptr) {
            StreamCodecInfo codecCaps = previousAudioSource->GetCodecInfo();
            player_->SetStreamCaps(eGST_MEDIATYPE_AUDIO,
                                   std::move(codecCaps.audio));
            long long currentPositionMilliseconds =
                GetVideoTimestampMilliseconds();
            if (currentPositionMilliseconds <= 0) {
                currentPositionMilliseconds = positionMs_;
            }
            const double currentPositionSeconds =
                static_cast<double>(currentPositionMilliseconds) / 1000.0;
            player_->FlushTrack(
                eGST_MEDIATYPE_AUDIO, currentPositionSeconds, 0.0, 0.0);
        }
        previousAudioSource->NeedData(eGST_MEDIATYPE_AUDIO);
        if (playerFlushed && playing_ && player_ != nullptr) {
            playbackResumed = player_->Pause(false, false);
        }
    } else if (previousAudioSource == nullptr) {
        SetActiveAudioSource(nullptr);
    }
    statusMessage_ = "Audio track switch failed: " + error;
    if (!playbackResumed) {
        statusMessage_ += "; Player Interface did not resume after audio flush";
    }
    audioSwitchDrainStarted_ = false;
    audioSwitchDrainRemaining_ = std::chrono::milliseconds::zero();
    LogStartupMilestone(statusMessage_);
}

void PlayerBackend::SetAllSourcesFeedingEnabled(bool enabled)
{
    if (source_ != nullptr) {
        source_->SetFeedingEnabled(
            enabled && trickModeSource_ == nullptr);
    }
    if (audioOnlySource_ != nullptr &&
        audioOnlySource_.get() != source_.get()) {
        audioOnlySource_->SetFeedingEnabled(
            enabled && trickModeSource_ == nullptr);
    }
    if (trickModeSource_ != nullptr) {
        trickModeSource_->SetFeedingEnabled(enabled);
    }
}

void PlayerBackend::PumpEvents()
{
    g_main_context_iteration(nullptr, FALSE);

    StreamDemuxSource* playbackSource =
        trickModeSource_ != nullptr ? trickModeSource_.get() : source_.get();
    if (playbackSource != nullptr) {
        std::string error;
        if (playbackSource->TakeError(error)) {
            const std::string failure = "FFmpeg ingest failed: " + error;
            statusMessage_ = failure;
            playing_ = false;
            LogStartupMilestone(statusMessage_);
            playbackSource->Stop();
            StopTrickModePrewarming();
            if (player_ != nullptr && !stopped_) {
                player_->Stop(true);
                stopped_ = true;
            }
            statusMessage_ = failure;
        }
    }

    const auto now = std::chrono::steady_clock::now();
    EvaluateAutoBitrate(now);
    if (trickModeSource_ == nullptr &&
        audioOnlySource_ != nullptr && !audioSwitchInProgress_) {
        std::string error;
        if (audioOnlySource_->TakeError(error)) {
            const std::string failedTrackId = selection_.audioTrackId;
            if (!failedTrackId.empty() &&
                failedTrackId != audioTrackRecoveryAttemptedId_ &&
                SelectAudioTrackId(failedTrackId, true)) {
                audioTrackRecoveryAttemptedId_ = failedTrackId;
                statusMessage_ =
                    "Audio track source failed; retrying track " +
                    failedTrackId + ": " + error;
            } else {
                SetActiveAudioSource(nullptr);
                audioOnlySource_->Stop();
                audioOnlySource_.reset();
                statusMessage_ =
                    "Audio track source failed after retry; continuing "
                    "without audio: " +
                    error;
            }
            LogStartupMilestone(statusMessage_);
        }
    }
    if (player_ != nullptr && !stopped_ && playing_ &&
        now >= nextPositionQuery_) {
        if (trickRate_ > 1 && now >= nextTrickPipelineStateLog_) {
            LogTrickPlayPipelineState("periodic");
            nextTrickPipelineStateLog_ = now + std::chrono::seconds(1);
        }
        const long long interfacePositionMilliseconds =
            player_->GetPositionMilliseconds();
        if (interfacePositionMilliseconds > 0) {
            if (audioSwitchPositionRebasePending_) {
                const long long adjustedPositionMilliseconds =
                    interfacePositionMilliseconds + positionOffsetMs_;
                if (adjustedPositionMilliseconds <
                    audioSwitchPositionBaselineMs_) {
                    positionOffsetMs_ +=
                        audioSwitchPositionBaselineMs_ -
                        adjustedPositionMilliseconds;
                    LogStartupMilestone(
                        "audio-switch-position-rebased: interface-position-ms=" +
                        std::to_string(interfacePositionMilliseconds) +
                        " playback-position-ms=" +
                        std::to_string(audioSwitchPositionBaselineMs_) +
                        " offset-ms=" +
                        std::to_string(positionOffsetMs_));
                }
                audioSwitchPositionRebasePending_ = false;
            }
            if (trickPlayPositionRebasePending_) {
                // FFmpeg may seek back to an earlier keyframe before the
                // pipeline reports its new, relative position.
                constexpr long long kTrickPlayRebaseToleranceMs = 2000;
                const long long adjustedPositionMilliseconds =
                    interfacePositionMilliseconds + positionOffsetMs_;
                if (adjustedPositionMilliseconds <
                    trickPlayPositionBaselineMs_ -
                        kTrickPlayRebaseToleranceMs) {
                    positionOffsetMs_ = trickPlayPositionBaselineMs_;
                    trickPlayPositionRebasePending_ = false;
                    LogStartupMilestone(
                        "trick-play-position-rebased: interface-position-ms=" +
                        std::to_string(interfacePositionMilliseconds) +
                        " playback-position-ms=" +
                        std::to_string(trickPlayPositionBaselineMs_) +
                        " offset-ms=" +
                        std::to_string(positionOffsetMs_));
                } else if (
                    adjustedPositionMilliseconds >
                    trickPlayPositionBaselineMs_ +
                        kTrickPlayRebaseToleranceMs) {
                    trickPlayPositionRebasePending_ = false;
                }
            }
            if (!trickPlayPositionRebasePending_) {
                positionMs_ = trickRate_ > 1
                    ? trickPlayPositionBaselineMs_ +
                          (interfacePositionMilliseconds -
                           trickPlayPositionBaselineMs_) *
                              trickRate_ +
                          positionOffsetMs_
                    : interfacePositionMilliseconds + positionOffsetMs_;
            }
        }
        const long long videoTimestampMilliseconds =
            GetVideoTimestampMilliseconds();
        if (videoTimestampMilliseconds > 0) {
            latestVideoTimestampMsForAudioSwitch_ =
                videoTimestampMilliseconds;
        }
        if (positionMs_ > 0 && !firstPositionReported_) {
            firstPositionReported_ = true;
            LogStartupMilestone("playback-position-advanced");
        }
        nextPositionQuery_ = now + std::chrono::milliseconds(250);

        if (now >= nextDurationQuery_) {
            if (durationMs_ <= 0 && source_ != nullptr) {
                durationMs_ = source_->GetDurationMilliseconds();
            }
            if (durationMs_ <= 0) {
                durationMs_ = player_->GetDurationMilliseconds();
            }
            nextDurationQuery_ = now + std::chrono::seconds(1);
        }
        if (now >= nextProgressLog_) {
            std::string progress =
                "playback-progress: position-ms=" +
                std::to_string(positionMs_) +
                " duration-ms=" + std::to_string(durationMs_);
            if (videoTimestampMilliseconds > 0) {
                progress += " video-pts-ms=" +
                            std::to_string(videoTimestampMilliseconds);
            }
            const GstPlaybackQualityStruct* quality =
                player_->GetVideoPlaybackQuality();
            if (quality != nullptr) {
                progress += " frames-rendered=" +
                            std::to_string(quality->rendered) +
                            " frames-dropped=" +
                            std::to_string(quality->dropped);
            } else {
                progress += " frame-stats=unavailable";
            }
            LogStartupMilestone(progress);
            nextProgressLog_ = now + std::chrono::seconds(5);
        }
    }
    const long long sourceDurationMilliseconds =
        source_ == nullptr ? 0 : source_->GetDurationMilliseconds();
    const bool nearEndOfStream =
        sourceDurationMilliseconds > 0 &&
        positionMs_ >= sourceDurationMilliseconds - 2000;
    if (trickModeSource_ == nullptr && playing_ &&
        source_ != nullptr && player_ != nullptr && !nearEndOfStream &&
        now >= nextAudioPrefetch_) {
        const double positionSeconds =
            static_cast<double>(
                latestVideoTimestampMsForAudioSwitch_.load()) /
            1000.0;
        const std::lock_guard<std::mutex> lock(audioSwitchMutex_);
        for (auto& entry : preparedAudioSources_) {
            entry.second->PrepareSeek(positionSeconds);
        }
        nextAudioPrefetch_ = now + std::chrono::seconds(1);
    }
    CompleteAudioSwitchIfReady();
    if (!audioSwitchInProgress_ && !pendingAudioTrackId_.empty()) {
        std::string pendingTrackId = std::move(pendingAudioTrackId_);
        pendingAudioTrackId_.clear();
        if (pendingTrackId == selection_.audioTrackId) {
            LogStartupMilestone(
                "audio-track-switch-queued-target-already-selected: track=" +
                pendingTrackId);
        } else {
            LogStartupMilestone(
                "audio-track-switch-dequeued: track=" + pendingTrackId);
            SelectAudioTrackId(pendingTrackId);
        }
    }
    if (player_ != nullptr &&
        now >= nextAudioSwitchDiagnostic_ &&
        now < audioSwitchDiagnosticsUntil_) {
        LogVideoDiagnostics("post-switch");
        nextAudioSwitchDiagnostic_ = now + std::chrono::milliseconds(500);
    }
    if (!audioWarmupRunning_) {
        if (!nearEndOfStream || pendingTrickRate_ > 1) {
            StartTrickModePrewarming();
        }
        if (playing_ && !nearEndOfStream && !trickWarmupRunning_ &&
            pendingTrickRate_ == 0) {
            StartAudioTrackPrewarming();
        }
    }

    if (trickModeSource_ == nullptr && source_ != nullptr &&
        player_ != nullptr && !stopped_ && playing_ &&
        !nearEndOfStream &&
        now >= nextTrickPrefetch_) {
        const long long currentPositionMilliseconds =
            GetPlaybackPositionMilliseconds();
        const double currentPositionSeconds =
            static_cast<double>(
                std::max(0LL, currentPositionMilliseconds)) /
            1000.0;
        const std::lock_guard<std::mutex> lock(trickWarmupMutex_);
        if (preparedTrickModeSource_ != nullptr) {
            preparedTrickModeSource_->PrepareSeek(currentPositionSeconds);
        }
        nextTrickPrefetch_ = now + std::chrono::seconds(1);
    }

    if (pendingTrickRate_ > 1 && trickModeSource_ == nullptr) {
        std::unique_ptr<StreamDemuxSource> candidate;
        std::string warmupError;
        {
            const std::lock_guard<std::mutex> lock(trickWarmupMutex_);
            candidate = std::move(preparedTrickModeSource_);
            warmupError = trickWarmupError_;
        }
        if (candidate != nullptr) {
            const int requestedRate = pendingTrickRate_;
            pendingTrickRate_ = 0;
            long long currentPositionMilliseconds =
                GetPlaybackPositionMilliseconds();
            if (currentPositionMilliseconds <= 0) {
                currentPositionMilliseconds = positionMs_;
            }
            const bool entered = EnterTrickMode(
                requestedRate,
                static_cast<double>(
                    std::max(0LL, currentPositionMilliseconds)) /
                    1000.0,
                playing_,
                std::move(candidate));
            if (!entered) {
                trickWarmupAttempted_ = true;
                const std::lock_guard<std::mutex> lock(trickWarmupMutex_);
                trickWarmupError_ = statusMessage_;
            }
        } else if (!warmupError.empty()) {
            pendingTrickRate_ = 0;
            statusMessage_ =
                "Could not prepare DASH trick-mode video track: " +
                warmupError;
            LogStartupMilestone(statusMessage_);
        }
    }
}

bool PlayerBackend::Play()
{
    if (player_ == nullptr || stopped_) {
        statusMessage_ = "Playback has stopped; restart the simulator to play again";
        return false;
    }

    if (trickModeSource_ != nullptr) {
        const double currentPositionSeconds =
            static_cast<double>(GetPlaybackPositionMilliseconds()) / 1000.0;
        if (!RestoreNormalPlayback(currentPositionSeconds, true)) {
            return false;
        }
        statusMessage_ = "Playing normal audio/video";
        return true;
    }

    pendingTrickRate_ = 0;
    SetAllSourcesFeedingEnabled(true);
    if (source_ != nullptr &&
        source_->HasTrack(eGST_MEDIATYPE_VIDEO)) {
        source_->NeedData(eGST_MEDIATYPE_VIDEO);
    }
    if (activeAudioSource_ != nullptr) {
        activeAudioSource_->NeedData(eGST_MEDIATYPE_AUDIO);
    }
    if (!player_->Pause(false, false)) {
        SetAllSourcesFeedingEnabled(false);
        statusMessage_ = "Could not resume playback";
        return false;
    }

    playing_ = true;
    statusMessage_ = "Playing";
    return true;
}

bool PlayerBackend::Pause()
{
    if (player_ == nullptr || stopped_) {
        statusMessage_ = "Playback has stopped";
        return false;
    }

    SetAllSourcesFeedingEnabled(false);
    if (!player_->Pause(true, false)) {
        SetAllSourcesFeedingEnabled(true);
        statusMessage_ = "Could not pause playback";
        return false;
    }

    playing_ = false;
    statusMessage_ = "Paused";
    return true;
}

bool PlayerBackend::Seek(double positionSeconds)
{
    if (player_ == nullptr || stopped_) {
        statusMessage_ = "Cannot seek: playback has stopped";
        return false;
    }
    if (audioSwitchInProgress_) {
        statusMessage_ = "Cannot seek while an audio track switch is preparing";
        return false;
    }

    const bool recoveringFromEndOfStream = endOfStream_;
    const bool resumeFeeding = playing_ || recoveringFromEndOfStream;
    const double requestedPositionSeconds = positionSeconds;
    bool seekClampedToEnd = false;
    const long long durationMilliseconds =
        source_ == nullptr ? 0 : source_->GetDurationMilliseconds();
    if (durationMilliseconds > 0 &&
        positionSeconds >=
            static_cast<double>(durationMilliseconds) / 1000.0) {
        constexpr double kFinalPlaybackWindowSeconds = 2.0;
        const double durationSeconds =
            static_cast<double>(durationMilliseconds) / 1000.0;
        positionSeconds =
            std::max(0.0, durationSeconds - kFinalPlaybackWindowSeconds);
        seekClampedToEnd = true;
        pendingTrickRate_ = 0;
    }

    if (trickModeSource_ != nullptr) {
        const long long previousPositionMilliseconds =
            GetPlaybackPositionMilliseconds();
        const double previousPositionSeconds =
            previousPositionMilliseconds > 0
                ? static_cast<double>(previousPositionMilliseconds) / 1000.0
                : 0.0;
        StreamDemuxSource* audioSource = normalAudioSourceDuringTrick_;
        if (!trickModeSource_->PrepareSeek(positionSeconds) ||
            !source_->PrepareSeek(positionSeconds) ||
            (audioSource != nullptr && audioSource != source_.get() &&
             !audioSource->PrepareSeek(positionSeconds))) {
            statusMessage_ = "FFmpeg cannot seek to the requested position";
            return false;
        }

        SetAllSourcesFeedingEnabled(false);
        if (!player_->Flush(positionSeconds, 1, false, false) ||
            !trickModeSource_->SetTrickPlaybackRate(
                trickRate_, positionSeconds) ||
            !trickModeSource_->Seek(positionSeconds, true, true) ||
            !source_->Seek(positionSeconds) ||
            (audioSource != nullptr && audioSource != source_.get() &&
             !audioSource->Seek(positionSeconds))) {
            const bool restored =
                trickModeSource_->SetTrickPlaybackRate(
                    trickRate_, previousPositionSeconds) &&
                trickModeSource_->Seek(
                    previousPositionSeconds, true, true) &&
                source_->Seek(previousPositionSeconds) &&
                (audioSource == nullptr ||
                 audioSource == source_.get() ||
                 audioSource->Seek(previousPositionSeconds)) &&
                player_->Flush(previousPositionSeconds, 1, false, false) &&
                player_->Pause(!resumeFeeding, false);
            SetAllSourcesFeedingEnabled(resumeFeeding && restored);
            statusMessage_ = restored
                ? "FFmpeg could not seek; trick playback restored"
                : "FFmpeg could not seek and trick playback recovery failed";
            return false;
        }
        pendingPlayerEos_ = false;
        player_->ResetEOSSignalledFlag();
        endOfStream_ = false;
        if (!player_->Pause(!resumeFeeding, false)) {
            SetAllSourcesFeedingEnabled(false);
            playing_ = false;
            statusMessage_ =
                "Could not restore trick-play state after seeking";
            return false;
        }

        positionMs_ = static_cast<long long>(positionSeconds * 1000.0);
        positionOffsetMs_ = 0;
        trickPlayPositionBaselineMs_ = positionMs_;
        trickPlayPositionRebasePending_ = false;
        audioSwitchPositionRebasePending_ = false;
        latestVideoTimestampMsForAudioSwitch_ = positionMs_;
        SetAllSourcesFeedingEnabled(resumeFeeding);
        if (resumeFeeding) {
            trickModeSource_->NeedData(eGST_MEDIATYPE_VIDEO);
        }
        if (recoveringFromEndOfStream) {
            if (!player_->Pause(false, false)) {
                SetAllSourcesFeedingEnabled(false);
                playing_ = false;
                statusMessage_ =
                    "Could not resume trick playback after seeking from EOS";
                return false;
            }
        }
        playing_ = resumeFeeding;
        statusMessage_ = seekClampedToEnd
            ? "Seek beyond duration; playing final segment from " +
                  std::to_string(positionSeconds) + " s"
            : "Seek requested: " +
                  std::to_string(positionSeconds) + " s";
        if (seekClampedToEnd) {
            LogStartupMilestone(
                "seek-beyond-duration-clamped: requested-seconds=" +
                std::to_string(requestedPositionSeconds) +
                " target-seconds=" + std::to_string(positionSeconds));
        }
        return true;
    }

    StreamDemuxSource* audioSource =
        activeAudioSource_ != source_.get() ? activeAudioSource_ : nullptr;
    const long long previousPositionMilliseconds =
        GetPlaybackPositionMilliseconds();
    const double previousPositionSeconds =
        previousPositionMilliseconds > 0
            ? static_cast<double>(previousPositionMilliseconds) / 1000.0
            : 0.0;
    if (!source_->PrepareSeek(positionSeconds) ||
        (audioSource != nullptr &&
         !audioSource->PrepareSeek(positionSeconds))) {
        statusMessage_ = "FFmpeg cannot seek to the requested position";
        return false;
    }
    SetAllSourcesFeedingEnabled(false);
    LogStartupMilestone("player-interface-seek-flush-begin");
    if (!player_->Flush(positionSeconds, 1, false, true)) {
        SetAllSourcesFeedingEnabled(resumeFeeding);
        statusMessage_ = "Player Interface seek request failed";
        return false;
    }
    positionMs_ = static_cast<long long>(positionSeconds * 1000.0);
    positionOffsetMs_ = 0;
    audioSwitchPositionRebasePending_ = false;
    trickPlayPositionRebasePending_ = false;
    const long long videoTimestampMilliseconds =
        GetVideoTimestampMilliseconds();
    latestVideoTimestampMsForAudioSwitch_ =
        videoTimestampMilliseconds > 0
            ? videoTimestampMilliseconds
            : positionMs_;
    LogStartupMilestone("player-interface-seek-flush-complete");

    LogStartupMilestone("ffmpeg-seek-begin");
    const bool sourceSeeked = source_->Seek(positionSeconds);
    const bool audioSeeked =
        audioSource == nullptr || audioSource->Seek(positionSeconds);
    if (!sourceSeeked || !audioSeeked) {
        const bool sourceRestored = source_->Seek(previousPositionSeconds);
        const bool audioRestored =
            audioSource == nullptr ||
            audioSource->Seek(previousPositionSeconds);
        const bool pipelineRestored =
            sourceRestored && audioRestored &&
            player_->Flush(previousPositionSeconds, 1, false, true);
        if (pipelineRestored) {
            SetAllSourcesFeedingEnabled(resumeFeeding);
            positionMs_ = static_cast<long long>(
                previousPositionSeconds * 1000.0);
            positionOffsetMs_ = 0;
            audioSwitchPositionRebasePending_ = false;
            trickPlayPositionRebasePending_ = false;
            const long long videoTimestampMilliseconds =
                GetVideoTimestampMilliseconds();
            latestVideoTimestampMsForAudioSwitch_ =
                videoTimestampMilliseconds > 0
                    ? videoTimestampMilliseconds
                    : positionMs_;
            statusMessage_ =
                "FFmpeg could not seek; playback restored to its prior position";
        } else {
            SetAllSourcesFeedingEnabled(false);
            playing_ = false;
            statusMessage_ =
                "FFmpeg could not seek and playback recovery failed";
        }
        return false;
    }
    LogStartupMilestone("ffmpeg-seek-complete");

    latestVideoTimestampMsForAudioSwitch_ = positionMs_;
    pendingPlayerEos_ = false;
    player_->ResetEOSSignalledFlag();
    endOfStream_ = false;
    playing_ = resumeFeeding;
    SetAllSourcesFeedingEnabled(resumeFeeding);
    if (recoveringFromEndOfStream) {
        source_->NeedData(eGST_MEDIATYPE_VIDEO);
        if (activeAudioSource_ != nullptr) {
            activeAudioSource_->NeedData(eGST_MEDIATYPE_AUDIO);
        }
        if (!player_->Pause(false, false)) {
            SetAllSourcesFeedingEnabled(false);
            statusMessage_ = "Could not resume playback after seeking from EOS";
            return false;
        }
    }
    statusMessage_ = seekClampedToEnd
        ? "Seek beyond duration; playing final segment from " +
              std::to_string(positionSeconds) + " s"
        : "Seek requested: " +
              std::to_string(positionSeconds) + " s";
    if (seekClampedToEnd) {
        LogStartupMilestone(
            "seek-beyond-duration-clamped: requested-seconds=" +
            std::to_string(requestedPositionSeconds) +
            " target-seconds=" + std::to_string(positionSeconds));
    }
    return true;
}

bool PlayerBackend::EnterTrickMode(int rate,
                                  double positionSeconds,
                                  bool resumeFeeding,
                                  std::unique_ptr<StreamDemuxSource> candidate)
{
    if (audioWarmupThread_.joinable()) {
        cancelAudioWarmup_ = true;
        audioWarmupThread_.join();
    }

    if (trickModeSource_ != nullptr) {
        SetAllSourcesFeedingEnabled(false);
        if (!trickModeSource_->SetTrickPlaybackRate(rate, positionSeconds) ||
            !trickModeSource_->PrepareSeek(positionSeconds) ||
            !trickModeSource_->Seek(positionSeconds, true, true) ||
            !player_->Flush(positionSeconds, 1, false, false)) {
            bool recovered =
                trickModeSource_->SetTrickPlaybackRate(
                    trickRate_, positionSeconds) &&
                trickModeSource_->PrepareSeek(positionSeconds) &&
                trickModeSource_->Seek(positionSeconds, true, true) &&
                player_->Flush(positionSeconds, 1, false, false);
            if (recovered && resumeFeeding) {
                trickModeSource_->SetFeedingEnabled(true);
                trickModeSource_->NeedData(eGST_MEDIATYPE_VIDEO);
            }
            if (recovered) {
                recovered = player_->Pause(!resumeFeeding, false);
            }
            SetAllSourcesFeedingEnabled(resumeFeeding && recovered);
            if (resumeFeeding && recovered) {
                trickModeSource_->NeedData(eGST_MEDIATYPE_VIDEO);
            } else if (!recovered) {
                playing_ = false;
            }
            statusMessage_ = recovered
                ? "Could not reposition the DASH trick-mode track"
                : "Trick-mode reposition failed and recovery failed";
            return false;
        }
    } else {
        if (trickWarmupThread_.joinable()) {
            trickWarmupThread_.join();
        }
        if (candidate == nullptr) {
            statusMessage_ =
                "The DASH trick-mode track is still being prepared";
            return false;
        }
        if (resumeFeeding) {
            const long long currentPosition =
                GetPlaybackPositionMilliseconds();
            if (currentPosition > 0) {
                positionSeconds =
                    static_cast<double>(currentPosition) / 1000.0;
            }
        }
        if (!candidate->HasTrack(eGST_MEDIATYPE_VIDEO) ||
            !candidate->SetTrickPlaybackRate(rate, positionSeconds) ||
            !candidate->PrepareSeek(positionSeconds) ||
            !candidate->Seek(positionSeconds, true, true)) {
            std::string error;
            candidate->TakeError(error);
            statusMessage_ = error.empty()
                ? "Could not position the DASH trick-mode video track"
                : "Could not prepare DASH trick-mode video track: " + error;
            candidate->Stop();
            LogStartupMilestone(statusMessage_);
            return false;
        }

        candidate->SetFeedingEnabled(false);
        if (!StartSource(*candidate)) {
            candidate->Stop();
            return false;
        }

        SetAllSourcesFeedingEnabled(false);
        LogTrickPlayPipelineState("before-trick-mode-reconfigure");
        if (!player_->Flush(positionSeconds, 1, false, true)) {
            candidate->Stop();
            const bool stateRestored =
                player_->Pause(!resumeFeeding, false);
            SetAllSourcesFeedingEnabled(resumeFeeding && stateRestored);
            if (!stateRestored) {
                playing_ = false;
            }
            statusMessage_ = "Player Interface could not flush for trick mode";
            return false;
        }

        normalAudioSourceDuringTrick_ = activeAudioSource_;
        trickModeSource_ = std::move(candidate);
        SetActiveAudioSource(nullptr);
        SetActiveVideoSource(trickModeSource_.get());
        ConfigurePlayerForSource(*trickModeSource_, positionSeconds);
        LogStartupMilestone(
            "trick-mode-track-selected: video-only dedicated DASH adaptation");
    }

    LogStartupMilestone(
        "trick-play-pipeline-flush: interface-rate=1 synthesized-rate=" +
        std::to_string(rate));
    LogTrickPlayPipelineState("after-flush");

    audioSwitchPositionRebasePending_ = false;
    positionOffsetMs_ = 0;
    positionMs_ = static_cast<long long>(positionSeconds * 1000.0);
    trickPlayPositionBaselineMs_ = positionMs_;
    trickPlayPositionRebasePending_ = false;
    trickRate_ = rate;
    nextTrickPipelineStateLog_ = std::chrono::steady_clock::now();
    player_->SetAudioVolume(0);
    player_->SetVolumeOrMuteUnMute();
    SetAllSourcesFeedingEnabled(resumeFeeding);
    if (resumeFeeding) {
        trickModeSource_->NeedData(eGST_MEDIATYPE_VIDEO);
        LogStartupMilestone("trick-play-injection-rearmed: video-only");
    }
    const bool playRequested = player_->Pause(!resumeFeeding, false);
    if (!playRequested) {
        SetAllSourcesFeedingEnabled(false);
        const std::string reconfigureError =
            "Player Interface could not enter DASH trick mode";
        LogStartupMilestone(reconfigureError);
        RestoreNormalPlayback(positionSeconds, resumeFeeding);
        statusMessage_ = reconfigureError;
        return false;
    }
    if (resumeFeeding) {
        LogStartupMilestone(
            std::string("trick-play-pipeline-play-request=") +
            (playRequested ? "accepted" : "rejected"));
    }
    playing_ = resumeFeeding;
    LogTrickPlayPipelineState("after-play-state-request");
    LogStartupMilestone(
        std::string("trick-play-player-state-preserved: state=") +
        (resumeFeeding ? "playing" : "paused"));
    statusMessage_ = "Trick rate requested: " + std::to_string(rate) + "x";
    return true;
}

bool PlayerBackend::RestoreNormalPlayback(double positionSeconds,
                                          bool resumeFeeding)
{
    StreamDemuxSource* audioSource = normalAudioSourceDuringTrick_;
    if (source_ == nullptr ||
        !source_->PrepareSeek(positionSeconds) ||
        (audioSource != nullptr && audioSource != source_.get() &&
         !audioSource->PrepareSeek(positionSeconds))) {
        statusMessage_ = "FFmpeg cannot reposition normal playback";
        return false;
    }
    if (!source_->Seek(positionSeconds) ||
        (audioSource != nullptr && audioSource != source_.get() &&
         !audioSource->Seek(positionSeconds))) {
        statusMessage_ = "FFmpeg could not restore normal playback position";
        return false;
    }
    if (trickModeSource_ != nullptr &&
        (!trickModeSource_->SetTrickPlaybackRate(
             trickRate_, positionSeconds) ||
         !trickModeSource_->PrepareSeek(positionSeconds) ||
         !trickModeSource_->Seek(positionSeconds, true, true))) {
        statusMessage_ = "Could not preserve trick-mode recovery position";
        return false;
    }

    SetAllSourcesFeedingEnabled(false);
    if (!player_->Flush(positionSeconds, 1, false, true)) {
        const bool stateRestored =
            player_->Pause(!resumeFeeding, false);
        SetAllSourcesFeedingEnabled(resumeFeeding && stateRestored);
        if (!stateRestored) {
            playing_ = false;
        }
        statusMessage_ = "Player Interface could not flush for normal playback";
        return false;
    }

    StreamDemuxSource* trickSource = trickModeSource_.get();
    SetActiveVideoSource(source_.get());
    SetActiveAudioSource(audioSource);
    firstVideoFrameDisplayedReported_ = false;
    ConfigurePlayerForSource(*source_, positionSeconds);
    if (resumeFeeding) {
        source_->SetFeedingEnabled(true);
        if (audioSource != nullptr && audioSource != source_.get()) {
            audioSource->SetFeedingEnabled(true);
        }
        if (source_->HasTrack(eGST_MEDIATYPE_VIDEO)) {
            source_->NeedData(eGST_MEDIATYPE_VIDEO);
        }
        if (audioSource != nullptr) {
            audioSource->NeedData(eGST_MEDIATYPE_AUDIO);
        }
    }
    if (!player_->Pause(!resumeFeeding, false)) {
        SetAllSourcesFeedingEnabled(false);
        SetActiveVideoSource(trickSource);
        SetActiveAudioSource(nullptr);
        ConfigurePlayerForSource(*trickSource, positionSeconds);
        if (resumeFeeding && trickModeSource_ != nullptr) {
            trickModeSource_->SetFeedingEnabled(true);
            trickModeSource_->NeedData(eGST_MEDIATYPE_VIDEO);
        }
        const bool trickStateRestored =
            player_->Pause(!resumeFeeding, false);
        SetAllSourcesFeedingEnabled(
            resumeFeeding && trickStateRestored);
        playing_ = trickStateRestored && resumeFeeding;
        statusMessage_ =
            trickStateRestored
                ? "Normal playback restore failed; trick mode resumed"
                : "Normal playback restore failed and trick mode recovery failed";
        return false;
    }

    if (trickModeSource_ != nullptr) {
        trickModeSource_->SetFeedingEnabled(false);
        trickModeSource_->Stop();
        trickModeSource_.reset();
    }
    normalAudioSourceDuringTrick_ = nullptr;
    trickRate_ = 1;
    trickWarmupAttempted_ = false;
    {
        const std::lock_guard<std::mutex> lock(trickWarmupMutex_);
        trickWarmupError_.clear();
    }
    trickPlayPositionRebasePending_ = false;
    audioSwitchPositionRebasePending_ = false;
    positionMs_ = static_cast<long long>(positionSeconds * 1000.0);
    positionOffsetMs_ = 0;
    trickPlayPositionBaselineMs_ = positionMs_;
    player_->SetAudioVolume(100);
    player_->SetVolumeOrMuteUnMute();
    SetAllSourcesFeedingEnabled(resumeFeeding);
    playing_ = resumeFeeding;
    if (resumeFeeding) {
        source_->NeedData(eGST_MEDIATYPE_VIDEO);
        if (activeAudioSource_ != nullptr) {
            activeAudioSource_->NeedData(eGST_MEDIATYPE_AUDIO);
        }
    }
    latestVideoTimestampMsForAudioSwitch_ =
        static_cast<long long>(positionSeconds * 1000.0);
    LogStartupMilestone("trick-mode-exit-restored-normal-audio-video");
    statusMessage_ = "Normal playback requested";
    return true;
}

bool PlayerBackend::SetTrickRate(int rate)
{
    if (player_ == nullptr || source_ == nullptr || stopped_) {
        statusMessage_ = "Cannot change trick rate: playback has stopped";
        return false;
    }
    if (audioSwitchInProgress_) {
        statusMessage_ =
            "Cannot change trick rate while an audio switch is preparing";
        return false;
    }
    if (!IsSupportedTrickRate(rate)) {
        statusMessage_ = "Unsupported trick rate";
        return false;
    }
    if (rate == 1) {
        if (trickModeSource_ == nullptr) {
            pendingTrickRate_ = 0;
            statusMessage_ = "Normal playback requested";
            return true;
        }
        const double currentPositionSeconds =
            static_cast<double>(GetPlaybackPositionMilliseconds()) / 1000.0;
        const bool restored =
            RestoreNormalPlayback(currentPositionSeconds, playing_);
        if (restored) {
            trickWarmupAttempted_ = false;
        }
        return restored;
    }
    if (!source_->CanUseTrickMode()) {
        statusMessage_ =
            "Trick play requires a finite static DASH VOD with video";
        return false;
    }

    const double currentPositionSeconds =
        static_cast<double>(GetPlaybackPositionMilliseconds()) / 1000.0;
    if (trickModeSource_ != nullptr) {
        return EnterTrickMode(
            rate, currentPositionSeconds, playing_, nullptr);
    }

    std::unique_ptr<StreamDemuxSource> candidate;
    std::string warmupError;
    {
        const std::lock_guard<std::mutex> lock(trickWarmupMutex_);
        candidate = std::move(preparedTrickModeSource_);
        warmupError = trickWarmupError_;
    }
    if (candidate != nullptr) {
        pendingTrickRate_ = 0;
        const bool entered = EnterTrickMode(
            rate,
            currentPositionSeconds,
            playing_,
            std::move(candidate));
        if (!entered) {
            trickWarmupAttempted_ = true;
            const std::lock_guard<std::mutex> lock(trickWarmupMutex_);
            trickWarmupError_ = statusMessage_;
        }
        return entered;
    }
    if (!warmupError.empty()) {
        statusMessage_ =
            "Could not prepare DASH trick-mode video track: " +
            warmupError;
        return false;
    }

    pendingTrickRate_ = rate;
    if (audioWarmupRunning_) {
        cancelAudioWarmup_ = true;
    }
    StartTrickModePrewarming();
    {
        const std::lock_guard<std::mutex> lock(trickWarmupMutex_);
        if (!trickWarmupError_.empty()) {
            pendingTrickRate_ = 0;
            statusMessage_ =
                "Could not prepare DASH trick-mode video track: " +
                trickWarmupError_;
            return false;
        }
    }
    statusMessage_ =
        "Preparing DASH trick-mode track; normal playback continues";
    LogStartupMilestone("trick-mode-request-queued: rate=" +
                        std::to_string(rate));
    return true;
}

bool PlayerBackend::SelectAudioTrack(std::size_t oneBasedIndex)
{
    if (source_ == nullptr || stopped_) {
        statusMessage_ = "Cannot switch audio: playback has stopped";
        return false;
    }
    if (oneBasedIndex == 0) {
        statusMessage_ = "Audio track numbers start at 1; use list audio";
        return false;
    }

    const std::vector<MediaTrackInfo> tracks = source_->GetTrackCatalog();
    std::size_t audioIndex = 0;
    for (const MediaTrackInfo& track : tracks) {
        if (track.type != MediaTrackType::Audio) {
            continue;
        }
        if (++audioIndex == oneBasedIndex) {
            return SelectAudioTrackId(track.id);
        }
    }
    statusMessage_ = "Audio track number is out of range; use list audio";
    return false;
}

bool PlayerBackend::SelectAudioTrackId(const std::string& trackId,
                                       bool allowCurrentSelection)
{
    if (source_ == nullptr || stopped_) {
        statusMessage_ = "Cannot switch audio: playback has stopped";
        return false;
    }
    if (trickModeSource_ != nullptr) {
        statusMessage_ = "Cannot switch audio during trick play";
        return false;
    }
    if (!source_->IsDashMedia()) {
        statusMessage_ = "Manual audio switching currently requires DASH";
        return false;
    }
    if (audioSwitchInProgress_) {
        pendingAudioTrackId_ = trackId;
        statusMessage_ = "Audio switch queued (latest request wins): " + trackId;
        LogStartupMilestone(
            "audio-track-switch-queued: track=" + trackId +
            " latest-request-wins");
        return true;
    }
    if (!allowCurrentSelection) {
        audioTrackRecoveryAttemptedId_.clear();
    }
    if (!allowCurrentSelection && selection_.audioTrackId == trackId) {
        statusMessage_ = "Audio track is already selected: " + trackId;
        return true;
    }

    MediaTrackSelection nextSelection = selection_;
    nextSelection.audioTrackId = trackId;
    StreamDemuxSource* previousAudioSource = activeAudioSource_;
    if (previousAudioSource == nullptr) {
        statusMessage_ = "No active audio source is available to switch";
        return false;
    }

    long long currentPositionMilliseconds =
        GetVideoTimestampMilliseconds();
    if (currentPositionMilliseconds <= 0) {
        currentPositionMilliseconds = positionMs_;
    }
    const double currentPositionSeconds =
        static_cast<double>(currentPositionMilliseconds) / 1000.0;
    previousAudioSource_ = previousAudioSource;
    requestedAudioTrackId_ = trackId;
    cancelAudioSwitch_ = false;
    audioSwitchInProgress_ = true;
    audioSwitchDrainStarted_ = false;
    audioSwitchDrainRemaining_ = std::chrono::milliseconds::zero();
    audioSwitchStartedAt_ = std::chrono::steady_clock::now();
    latestVideoTimestampMsForAudioSwitch_ = currentPositionMilliseconds;
    LogStartupMilestone("audio-track-switch-prepare-begin: track=" + trackId);

    std::unique_ptr<StreamDemuxSource> prewarmedSource;
    {
        const std::lock_guard<std::mutex> lock(audioSwitchMutex_);
        const auto prepared = preparedAudioSources_.find(trackId);
        if (prepared != preparedAudioSources_.end()) {
            prewarmedSource = std::move(prepared->second);
            preparedAudioSources_.erase(prepared);
        }
        audioSwitchReady_ = false;
        audioSwitchUseMainSource_ = false;
        preparedAudioSource_.reset();
        audioSwitchError_.clear();
        preparedAudioPositionSeconds_ = currentPositionSeconds;
    }

    std::vector<uint8_t> manifestSnapshot =
        source_->GetDashManifestSnapshot();
    const std::string manifestBaseUrl =
        source_->GetDashManifestBaseUrl();

    try {
        audioSwitchThread_ = std::thread(
            [this,
             selection = std::move(nextSelection),
             manifestSnapshot = std::move(manifestSnapshot),
             manifestBaseUrl,
             candidate = std::move(prewarmedSource)]() mutable {
                std::string error;
                double preparedPositionSeconds = 0.0;
                bool positioned = candidate != nullptr;
                if (!positioned) {
                    candidate = std::make_unique<StreamDemuxSource>();
                    positioned = candidate->Open(
                        url_,
                        [this](const std::string& milestone) {
                            LogStartupMilestone(milestone);
                        },
                        selection,
                        true,
                        manifestSnapshot.empty() ? nullptr
                                                 : &manifestSnapshot,
                        manifestBaseUrl,
                        &cancelAudioSwitch_);
                    if (!positioned) {
                        candidate->TakeError(error);
                    }
                }

                if (positioned) {
                    positioned = false;
                    for (unsigned int attempt = 0; attempt < 4; ++attempt) {
                        if (cancelAudioSwitch_) {
                            break;
                        }
                        const long long targetMilliseconds =
                            latestVideoTimestampMsForAudioSwitch_.load();
                        const double targetSeconds =
                            static_cast<double>(targetMilliseconds) / 1000.0;
                        candidate->SetDiscardBeforePosition(
                            eGST_MEDIATYPE_AUDIO, targetSeconds);
                        if (!candidate->PrepareSeek(targetSeconds) ||
                            !candidate->Seek(targetSeconds, false)) {
                            candidate->TakeError(error);
                            break;
                        }

                        preparedPositionSeconds = targetSeconds;
                        const long long latestMilliseconds =
                            latestVideoTimestampMsForAudioSwitch_.load();
                        const long long positionDelta =
                            std::llabs(latestMilliseconds -
                                       targetMilliseconds);
                        if (positionDelta <= 3000) {
                            positioned = true;
                            break;
                        }
                    }
                    if (!positioned && error.empty() &&
                        !cancelAudioSwitch_) {
                        error =
                            "Playback position kept advancing while the "
                            "audio track was being prepared";
                    }
                }

                if (cancelAudioSwitch_) {
                    candidate->Stop();
                    return;
                }

                if (!positioned && error.empty()) {
                    error = "Could not position the requested audio track";
                }
                {
                    const std::lock_guard<std::mutex> lock(audioSwitchMutex_);
                    if (positioned) {
                        preparedAudioSource_ = std::move(candidate);
                    } else {
                        if (candidate != nullptr) {
                            candidate->Stop();
                        }
                    }
                    audioSwitchError_ = std::move(error);
                    preparedAudioPositionSeconds_ = preparedPositionSeconds;
                    audioSwitchReady_ = true;
                    audioSwitchUseMainSource_ = false;
                }
            });
    } catch (const std::system_error& error) {
        cancelAudioSwitch_ = true;
        audioSwitchInProgress_ = false;
        RestoreAudioAfterSwitchFailure(
            "Could not start audio-track preparation: " +
            std::string(error.what()));
        return false;
    }

    statusMessage_ = "Switching audio to track: " + trackId +
                     " (video playback continues)";
    return true;
}

bool PlayerBackend::SelectVideoResolution(int width, int height)
{
    if (source_ == nullptr || stopped_) {
        statusMessage_ = "Cannot switch video quality: playback has stopped";
        return false;
    }
    if (!source_->IsDashMedia()) {
        statusMessage_ = "Manual quality switching currently requires DASH";
        return false;
    }
    if (width < 0 || height <= 0) {
        statusMessage_ = "Invalid video resolution";
        return false;
    }

    const std::vector<MediaTrackInfo> tracks = source_->GetTrackCatalog();
    const MediaTrackInfo* match = nullptr;
    for (const MediaTrackInfo& track : tracks) {
        if (track.type != MediaTrackType::Video ||
            track.height != height ||
            (width > 0 && track.width != width)) {
            continue;
        }
        if (match == nullptr || track.bitrate > match->bitrate) {
            match = &track;
        }
    }
    if (match == nullptr) {
        statusMessage_ = "No video representation matches " +
                         std::to_string(width == 0 ? height : width) +
                         (width == 0 ? "p" : "x" + std::to_string(height));
        return false;
    }

    return SelectVideoTrackId(match->id);
}

bool PlayerBackend::SelectVideoTrackId(const std::string& trackId)
{
    if (source_ == nullptr || stopped_) {
        statusMessage_ = "Cannot switch video quality: playback has stopped";
        return false;
    }
    if (trickModeSource_ != nullptr) {
        statusMessage_ = "Cannot switch video quality during trick play";
        return false;
    }
    autoBitrateEnabled_ = false;
    abrController_.Reset();
    if (selection_.videoTrackId == trackId) {
        statusMessage_ = "Video representation is already selected: " +
                         trackId + "; automatic bitrate disabled";
        return true;
    }

    MediaTrackSelection nextSelection = selection_;
    nextSelection.videoTrackId = trackId;
    const double positionSeconds =
        static_cast<double>(GetPlaybackPositionMilliseconds()) / 1000.0;
    return ApplySelection(
        nextSelection, positionSeconds, "manual-video-quality-switch");
}

bool PlayerBackend::SetAutoBitrate(bool enabled)
{
    if (source_ == nullptr || stopped_) {
        statusMessage_ = "Cannot change ABR mode: playback has stopped";
        return false;
    }
    if (enabled && audioSwitchInProgress_) {
        statusMessage_ =
            "Cannot enable ABR while an audio track switch is preparing";
        return false;
    }
    if (!enabled) {
        autoBitrateEnabled_ = false;
        abrController_.Reset();
        statusMessage_ = "Automatic bitrate disabled";
        LogStartupMilestone("abr-disabled");
        return true;
    }
    if (!source_->IsDashMedia()) {
        statusMessage_ = "Automatic bitrate selection currently requires DASH";
        return false;
    }

    const size_t selectableRepresentations =
        abrController_.CountVideoRepresentations(
            source_->GetTrackCatalog(), selection_.videoTrackId);
    if (selectableRepresentations < 2) {
        statusMessage_ =
            "Automatic bitrate requires at least two DASH video representations";
        return false;
    }

    autoBitrateEnabled_ = true;
    abrController_.Reset();
    nextAbrEvaluation_ = std::chrono::steady_clock::now();
    statusMessage_ = "Automatic bitrate enabled";
    LogStartupMilestone("abr-enabled: safety-factor=0.70");
    return true;
}

bool PlayerBackend::ApplySelection(const MediaTrackSelection& selection,
                                   double positionSeconds,
                                   const std::string& reason)
{
    if (trickModeSource_ != nullptr) {
        statusMessage_ = "Cannot change tracks during trick play";
        return false;
    }
    if (audioSwitchInProgress_) {
        statusMessage_ =
            "Cannot change video selection while an audio switch is preparing";
        return false;
    }

    const bool resumeFeeding = playing_;
    auto candidate = std::make_unique<StreamDemuxSource>();
    const std::vector<uint8_t> manifestSnapshot =
        source_->GetDashManifestSnapshot();
    const std::string manifestBaseUrl =
        source_->GetDashManifestBaseUrl();
    LogStartupMilestone(
        reason + "-prepare-begin: audio-track=" + selection.audioTrackId +
        " video-track=" + selection.videoTrackId);
    const auto candidateOpenStarted = std::chrono::steady_clock::now();
    if (!candidate->Open(
            url_,
            [this](const std::string& milestone) {
                LogStartupMilestone(milestone);
            },
            selection,
            false,
            manifestSnapshot.empty() ? nullptr : &manifestSnapshot,
            manifestBaseUrl)) {
        std::string error;
        candidate->TakeError(error);
        statusMessage_ = error.empty()
            ? "Could not open the requested DASH selection"
            : "Could not prepare DASH selection: " + error;
        LogStartupMilestone(statusMessage_);
        return false;
    }
    const double candidateOpenSeconds =
        std::chrono::duration<double>(
            std::chrono::steady_clock::now() - candidateOpenStarted)
            .count();

    // Prepare ahead of playback, then wait for it to reach the switch point so
    // the old stream keeps playing and the new one does not rewind.
    double leadSeconds =
        std::min(10.0, std::max(2.0, candidateOpenSeconds + 2.0));
    LogStartupMilestone(
        reason + "-switch-lookahead: candidate-open-ms=" +
        std::to_string(static_cast<long long>(candidateOpenSeconds * 1000.0)) +
        " lead-seconds=" + std::to_string(leadSeconds));
    for (unsigned int attempt = 0; attempt < 3; ++attempt) {
        if (resumeFeeding) {
            const double latestPosition =
                static_cast<double>(GetPlaybackPositionMilliseconds()) /
                1000.0;
            positionSeconds =
                std::max(positionSeconds, latestPosition + leadSeconds);
        }
        const auto prepareStart = std::chrono::steady_clock::now();
        if (candidate->PrepareSeek(positionSeconds) &&
            candidate->Seek(positionSeconds, false, true)) {
            candidate->SetDiscardBeforePosition(
                eGST_MEDIATYPE_AUDIO, positionSeconds);
            candidate->SetDiscardBeforePosition(
                eGST_MEDIATYPE_VIDEO, positionSeconds);
            if (!resumeFeeding) {
                break;
            }
            const double prepareSeconds =
                std::chrono::duration<double>(
                    std::chrono::steady_clock::now() - prepareStart).count();
            leadSeconds = std::max(2.0, prepareSeconds * 1.5 + 1.0);
            double latestPosition =
                static_cast<double>(GetPlaybackPositionMilliseconds()) /
                1000.0;
            if (attempt < 2 && latestPosition >= positionSeconds - 0.05) {
                continue;
            }
            const auto deadline =
                std::chrono::steady_clock::now() +
                std::chrono::duration<double>(
                    std::min(12.0, std::max(0.0, positionSeconds -
                                                     latestPosition) + 2.0));
            while (latestPosition < positionSeconds - 0.05 &&
                   std::chrono::steady_clock::now() < deadline) {
                std::this_thread::sleep_for(std::chrono::milliseconds(20));
                latestPosition =
                    static_cast<double>(GetPlaybackPositionMilliseconds()) /
                    1000.0;
            }
            LogStartupMilestone(
                reason + "-switch-point-reached: target=" +
                std::to_string(positionSeconds) + " playback=" +
                std::to_string(latestPosition) + " attempts=" +
                std::to_string(attempt + 1));
            break;
        }

        std::string error;
        candidate->TakeError(error);
        statusMessage_ = error.empty()
            ? "Could not position the requested DASH selection"
            : "Could not position DASH selection: " + error;
        LogStartupMilestone(statusMessage_);
        return false;
    }

    StreamDemuxSource* previousAudioSource = activeAudioSource_;
    SetAllSourcesFeedingEnabled(false);
    if (!player_->Flush(positionSeconds, 1, false, true)) {
        SetAllSourcesFeedingEnabled(resumeFeeding);
        statusMessage_ = "Player Interface flush failed during " + reason;
        return false;
    }

    std::unique_ptr<StreamDemuxSource> previous = std::move(source_);
    source_ = std::move(candidate);
    SetActiveVideoSource(source_.get());
    SetActiveAudioSource(source_->HasTrack(eGST_MEDIATYPE_AUDIO)
                             ? source_.get()
                             : nullptr);
    source_->SetFeedingEnabled(false);
    ConfigurePlayerForSource(*source_, positionSeconds);
    const bool sourceStarted = StartSource();
    const bool playbackStateRestored =
        sourceStarted && player_->Pause(!resumeFeeding, false);
    LogStartupMilestone(
        reason + "-player-state-restored: target=" +
        (resumeFeeding ? "playing" : "paused") +
        " success=" + (playbackStateRestored ? "yes" : "no"));
    if (!playbackStateRestored) {
        const std::string switchError = sourceStarted
            ? "Could not restore Player Interface state after " + reason
            : statusMessage_;
        source_->Stop();
        source_.reset();
        source_ = std::move(previous);
        SetActiveVideoSource(source_.get());
        SetActiveAudioSource(previousAudioSource);
        const bool previousSourcePositioned =
            source_->Seek(positionSeconds) &&
            (previousAudioSource == nullptr ||
             previousAudioSource == source_.get() ||
             previousAudioSource->Seek(positionSeconds)) &&
            player_->Flush(positionSeconds, 1, false, true);
        if (previousSourcePositioned) {
            ConfigurePlayerForSource(*source_, positionSeconds);
        }
        const bool previousPlayerStateRestored =
            previousSourcePositioned &&
            (resumeFeeding || player_->Pause(true, false));
        if (previousPlayerStateRestored) {
            SetAllSourcesFeedingEnabled(resumeFeeding);
            statusMessage_ = "Selection failed; previous playback restored: " +
                             switchError;
        } else {
            SetAllSourcesFeedingEnabled(false);
            playing_ = false;
            statusMessage_ = "Selection failed and previous playback recovery failed: " +
                             switchError;
        }
        LogStartupMilestone(statusMessage_);
        return false;
    }

    source_->SetFeedingEnabled(resumeFeeding);
    if (audioOnlySource_ != nullptr) {
        audioOnlySource_->Stop();
        audioOnlySource_.reset();
    }
    SetActiveAudioSource(source_->HasTrack(eGST_MEDIATYPE_AUDIO)
                             ? source_.get()
                             : nullptr);
    LogStartupMilestone(reason + "-previous-source-stop-begin");
    previous->Stop();
    LogStartupMilestone(reason + "-previous-source-stop-complete");
    selection_ = selection;
    positionMs_ = static_cast<long long>(positionSeconds * 1000.0);
    positionOffsetMs_ = 0;
    audioSwitchPositionRebasePending_ = false;
    trickPlayPositionRebasePending_ = false;
    const long long videoTimestampMilliseconds =
        GetVideoTimestampMilliseconds();
    latestVideoTimestampMsForAudioSwitch_ =
        videoTimestampMilliseconds > 0
            ? videoTimestampMilliseconds
            : positionMs_;
    const long long newDuration = source_->GetDurationMilliseconds();
    if (newDuration > 0) {
        durationMs_ = newDuration;
    }
    statusMessage_ = reason + " complete: audio=" +
                     selection_.audioTrackId + " video=" +
                     selection_.videoTrackId + " at " +
                     std::to_string(positionSeconds) + " s";
    LogStartupMilestone(
        reason + "-complete: audio-track=" + selection_.audioTrackId +
        " video-track=" + selection_.videoTrackId +
        " position-seconds=" + std::to_string(positionSeconds));
    return true;
}

void PlayerBackend::EvaluateAutoBitrate(
    std::chrono::steady_clock::time_point now)
{
    if (!autoBitrateEnabled_ || trickModeSource_ != nullptr ||
        !playing_ || source_ == nullptr ||
        audioSwitchInProgress_ ||
        now < nextAbrEvaluation_) {
        return;
    }
    nextAbrEvaluation_ = now + std::chrono::seconds(3);

    const uint64_t measured =
        source_->GetEstimatedBandwidthBitsPerSecond();
    if (measured > 0) {
        lastBandwidthEstimateBitsPerSecond_ = measured;
    }
    const uint64_t bandwidth = lastBandwidthEstimateBitsPerSecond_;
    const AbrDecision decision = abrController_.Evaluate(
        source_->GetTrackCatalog(), selection_, bandwidth);
    if (!decision.valid) {
        if (bandwidth == 0) {
            return;
        }
        LogStartupMilestone(
            "abr-decision-skipped: current-or-alternate-representation-unavailable");
        return;
    }
    LogStartupMilestone(
        "abr-decision: bandwidth-bps=" +
        std::to_string(decision.measuredBandwidthBitsPerSecond) +
        " safe-video-bps=" +
        std::to_string(decision.safeVideoBandwidthBitsPerSecond) +
        " audio-reserved-bps=" +
        std::to_string(decision.reservedAudioBandwidthBitsPerSecond) +
        " current=" + decision.currentTrackId +
        " target=" + decision.targetTrackId +
        " confirmation=" + std::to_string(decision.confirmationCount));
    if (!decision.shouldSwitch) {
        return;
    }

    MediaTrackSelection nextSelection = selection_;
    nextSelection.videoTrackId = decision.targetTrackId;
    const double positionSeconds =
        static_cast<double>(GetPlaybackPositionMilliseconds()) / 1000.0;
    ApplySelection(nextSelection, positionSeconds, "abr-video-switch");
    abrController_.Reset();
}

std::vector<MediaTrackInfo> PlayerBackend::GetTrackCatalog() const
{
    if (source_ == nullptr) {
        return {};
    }

    std::vector<MediaTrackInfo> tracks = source_->GetTrackCatalog();
    for (MediaTrackInfo& track : tracks) {
        if (track.type == MediaTrackType::Audio) {
            track.selected = track.id == selection_.audioTrackId;
        } else if (track.type == MediaTrackType::Video) {
            track.selected = track.id == selection_.videoTrackId;
        }
    }
    return tracks;
}

void PlayerBackend::Stop()
{
    if (stopped_) {
        return;
    }

    cancelAudioSwitch_ = true;
    cancelAudioWarmup_ = true;
    if (audioSwitchThread_.joinable()) {
        audioSwitchThread_.join();
    }
    if (audioWarmupThread_.joinable()) {
        audioWarmupThread_.join();
    }
    StopTrickModePrewarming();
    {
        const std::lock_guard<std::mutex> lock(audioSwitchMutex_);
        preparedAudioSource_.reset();
        preparedAudioSources_.clear();
        audioSwitchReady_ = false;
        audioSwitchUseMainSource_ = false;
        audioSwitchDrainStarted_ = false;
        audioSwitchDrainRemaining_ = std::chrono::milliseconds::zero();
    }
    audioSwitchInProgress_ = false;
    previousAudioSource_ = nullptr;
    pendingAudioTrackId_.clear();
    SetActiveAudioSource(nullptr);
    SetActiveVideoSource(nullptr);

    if (trickModeSource_ != nullptr) {
        trickModeSource_->SetFeedingEnabled(false);
        trickModeSource_->Stop();
        trickModeSource_.reset();
    }
    normalAudioSourceDuringTrick_ = nullptr;
    if (audioOnlySource_ != nullptr) {
        audioOnlySource_->SetFeedingEnabled(false);
        audioOnlySource_->Stop();
        audioOnlySource_.reset();
    }
    if (source_ != nullptr) {
        source_->SetFeedingEnabled(false);
        source_->Stop();
    }
    for (AudioSourceCleanupTask& cleanup : audioSourceCleanupTasks_) {
        if (cleanup.thread.joinable()) {
            cleanup.thread.join();
        }
    }
    audioSourceCleanupTasks_.clear();
    if (player_ != nullptr) {
        player_->Stop(true);
    }
    stopped_ = true;
    playing_ = false;
    statusMessage_ = "Stopped";
}

PlaybackStatus PlayerBackend::GetStatus() const
{
    PlaybackStatus status;
    status.message = statusMessage_;
    status.playing = playing_;
    status.stopped = stopped_;
    status.autoBitrateEnabled = autoBitrateEnabled_;
    status.positionMs = positionMs_;
    status.durationMs = durationMs_;
    const uint64_t measuredBandwidth =
        source_ == nullptr
            ? 0
            : source_->GetEstimatedBandwidthBitsPerSecond();
    status.estimatedBandwidthBitsPerSecond = measuredBandwidth > 0
        ? measuredBandwidth
        : lastBandwidthEstimateBitsPerSecond_;
    return status;
}

void PlayerBackend::HandleBusEvent(const BusEventData& event)
{
    switch (event.msgType) {
    case MESSAGE_ERROR:
        LogStartupMilestone("player-interface-error: " + event.msg +
                            " | debug: " + event.dbg_info);
        statusMessage_ = event.msg.empty() ? "Playback error" : event.msg;
        pendingPlayerEos_ = false;
        endOfStream_ = false;
        playing_ = false;
        break;
    case MESSAGE_WARNING:
        LogStartupMilestone("player-interface-warning: " + event.msg +
                            " | debug: " + event.dbg_info);
        statusMessage_ = event.msg.empty() ? "Playback warning"
                                           : "Warning: " + event.msg;
        break;
    case MESSAGE_EOS:
        if (!pendingPlayerEos_.exchange(false)) {
            LogStartupMilestone("player-interface-stale-eos-ignored");
            break;
        }
        LogStartupMilestone("player-interface-eos");
        statusMessage_ = "End of stream";
        endOfStream_ = true;
        playing_ = false;
        break;
    case MESSAGE_APPLICATION:
    case MESSAGE_CLOCK_LOST:
        break;
    case MESSAGE_STATE_CHANGE:
        if (event.firstBufferProcessed &&
            !pipelinePlayingNotified_.exchange(true)) {
            LogStartupMilestone("player-interface-entered-playing");
        }
        break;
    }
}

void PlayerBackend::LogStartupMilestone(const std::string& milestone)
{
    const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - startupAt_).count();
    const std::lock_guard<std::mutex> lock(startupLogMutex_);
    std::clog << "[PI-CLI] +" << elapsed << "ms " << milestone << std::endl;
}
