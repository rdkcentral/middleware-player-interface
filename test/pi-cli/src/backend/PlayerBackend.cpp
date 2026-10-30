#include "PlayerBackend.h"

#include "FfmpegDemuxSource.h"
#include "InterfacePlayerRDK.h"

#include <glib.h>

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <system_error>
#include <utility>

namespace {

constexpr int kVideoAppSrcQueueBytes = 15 * 1024 * 1024;
constexpr int kAudioAppSrcQueueBytes = 1536 * 1024;

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

    source_ = std::make_unique<FfmpegDemuxSource>();
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
    latestPositionMsForAudioSwitch_ = positionMs_;
    nextPositionQuery_ = std::chrono::steady_clock::now();
    nextDurationQuery_ = nextPositionQuery_;
    nextProgressLog_ = nextPositionQuery_;
    nextAbrEvaluation_ = nextPositionQuery_ + std::chrono::seconds(3);
    return true;
}

bool PlayerBackend::StartSource()
{
    return source_ != nullptr && StartSource(*source_);
}

bool PlayerBackend::StartSource(FfmpegDemuxSource& source)
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
            [this](int mediaType) {
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
        if (!audioSwitchInProgress_ && activeAudioSource_ != nullptr) {
            activeAudioSource_->NeedData(mediaType);
        }
        return;
    }
    if (source_ != nullptr) {
        source_->NeedData(mediaType);
    }
}

void PlayerBackend::RouteEnoughData(int mediaType)
{
    if (mediaType == eGST_MEDIATYPE_AUDIO) {
        const std::lock_guard<std::mutex> lock(audioRoutingMutex_);
        if (!audioSwitchInProgress_ && activeAudioSource_ != nullptr) {
            activeAudioSource_->EnoughData(mediaType);
        }
        return;
    }
    if (source_ != nullptr) {
        source_->EnoughData(mediaType);
    }
}

void PlayerBackend::SetActiveAudioSource(FfmpegDemuxSource* source)
{
    const std::lock_guard<std::mutex> lock(audioRoutingMutex_);
    activeAudioSource_ = source;
}

void PlayerBackend::ConfigurePlayerForSource(FfmpegDemuxSource& source)
{
    LogStartupMilestone("player-interface-selection-reconfigure-begin");
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
    LogStartupMilestone("player-interface-selection-reconfigure-complete");
}

void PlayerBackend::CompleteAudioSwitchIfReady()
{
    std::unique_ptr<FfmpegDemuxSource> candidate;
    std::string error;
    double positionSeconds = 0.0;
    {
        const std::lock_guard<std::mutex> lock(audioSwitchMutex_);
        if (!audioSwitchReady_) {
            return;
        }
        candidate = std::move(preparedAudioSource_);
        error = std::move(audioSwitchError_);
        positionSeconds = preparedAudioPositionSeconds_;
        audioSwitchReady_ = false;
    }
    if (audioSwitchThread_.joinable()) {
        audioSwitchThread_.join();
    }

    if (stopped_ || player_ == nullptr) {
        if (candidate != nullptr) {
            candidate->Stop();
        }
        audioSwitchInProgress_ = false;
        previousAudioSource_ = nullptr;
        return;
    }
    if (candidate == nullptr) {
        RestoreAudioAfterSwitchFailure(
            error.empty() ? "Could not prepare the requested audio track"
                          : error);
        return;
    }
    if (!candidate->HasTrack(eGST_MEDIATYPE_AUDIO)) {
        candidate->Stop();
        RestoreAudioAfterSwitchFailure(
            "The requested DASH adaptation has no audio stream");
        return;
    }

    StreamCodecInfo codecCaps = candidate->GetCodecInfo();
    player_->SetStreamCaps(eGST_MEDIATYPE_AUDIO,
                           std::move(codecCaps.audio));
    player_->FlushTrack(
        eGST_MEDIATYPE_AUDIO, positionSeconds, 0.0, 0.0);
    if (!StartSource(*candidate)) {
        const std::string startError = statusMessage_;
        candidate->Stop();
        RestoreAudioAfterSwitchFailure(startError);
        return;
    }

    std::unique_ptr<FfmpegDemuxSource> previousAudioOnly =
        std::move(audioOnlySource_);
    audioOnlySource_ = std::move(candidate);
    SetActiveAudioSource(audioOnlySource_.get());
    selection_.audioTrackId = requestedAudioTrackId_;
    previousAudioSource_ = nullptr;
    audioSwitchInProgress_ = false;
    cancelAudioSwitch_ = false;
    audioOnlySource_->NeedData(eGST_MEDIATYPE_AUDIO);

    if (previousAudioOnly != nullptr) {
        previousAudioOnly->Stop();
    }
    statusMessage_ = "Audio track switch complete: " +
                     selection_.audioTrackId;
    LogStartupMilestone("audio-track-switch-complete: track=" +
                        selection_.audioTrackId +
                        " video-position-ms=" +
                        std::to_string(positionMs_));
}

void PlayerBackend::RestoreAudioAfterSwitchFailure(
    const std::string& error)
{
    FfmpegDemuxSource* previousAudioSource = previousAudioSource_;
    previousAudioSource_ = nullptr;
    audioSwitchInProgress_ = false;
    cancelAudioSwitch_ = false;
    if (previousAudioSource != nullptr && !stopped_) {
        previousAudioSource->SetTrackEnabled(eGST_MEDIATYPE_AUDIO, true);
        SetActiveAudioSource(previousAudioSource);
        const double currentPositionSeconds =
            static_cast<double>(player_->GetPositionMilliseconds()) / 1000.0;
        player_->FlushTrack(
            eGST_MEDIATYPE_AUDIO, currentPositionSeconds, 0.0, 0.0);
        previousAudioSource->NeedData(eGST_MEDIATYPE_AUDIO);
    } else {
        SetActiveAudioSource(nullptr);
    }
    statusMessage_ = "Audio track switch failed: " + error;
    LogStartupMilestone(statusMessage_);
}

void PlayerBackend::SetAllSourcesFeedingEnabled(bool enabled)
{
    if (source_ != nullptr) {
        source_->SetFeedingEnabled(enabled);
    }
    if (audioOnlySource_ != nullptr &&
        audioOnlySource_.get() != source_.get()) {
        audioOnlySource_->SetFeedingEnabled(enabled);
    }
}

void PlayerBackend::PumpEvents()
{
    g_main_context_iteration(nullptr, FALSE);

    if (source_ != nullptr) {
        std::string error;
        if (source_->TakeError(error)) {
            const std::string failure = "FFmpeg ingest failed: " + error;
            statusMessage_ = failure;
            playing_ = false;
            LogStartupMilestone(statusMessage_);
            source_->Stop();
            if (player_ != nullptr && !stopped_) {
                player_->Stop(true);
                stopped_ = true;
            }
            statusMessage_ = failure;
        }
    }

    const auto now = std::chrono::steady_clock::now();
    EvaluateAutoBitrate(now);
    if (audioOnlySource_ != nullptr && !audioSwitchInProgress_) {
        std::string error;
        if (audioOnlySource_->TakeError(error)) {
            SetActiveAudioSource(nullptr);
            audioOnlySource_->Stop();
            audioOnlySource_.reset();
            if (player_ != nullptr && !stopped_) {
                player_->FlushTrack(
                    eGST_MEDIATYPE_AUDIO,
                    static_cast<double>(positionMs_) / 1000.0,
                    0.0,
                    0.0);
            }
            statusMessage_ =
                "Audio track source failed; video playback continues: " +
                error;
            LogStartupMilestone(statusMessage_);
        }
    }
    if (player_ != nullptr && !stopped_ && playing_ &&
        now >= nextPositionQuery_) {
        positionMs_ = player_->GetPositionMilliseconds();
        latestPositionMsForAudioSwitch_ = positionMs_;
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
    CompleteAudioSwitchIfReady();
}

bool PlayerBackend::Play()
{
    if (player_ == nullptr || stopped_) {
        statusMessage_ = "Playback has stopped; restart the simulator to play again";
        return false;
    }

    if (!player_->Pause(false, false)) {
        statusMessage_ = "Could not resume playback";
        return false;
    }

    SetAllSourcesFeedingEnabled(true);
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

    const bool resumeFeeding = playing_;
    FfmpegDemuxSource* audioSource =
        activeAudioSource_ != source_.get() ? activeAudioSource_ : nullptr;
    const long long previousPositionMilliseconds =
        player_->GetPositionMilliseconds();
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
    latestPositionMsForAudioSwitch_ = positionMs_;
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
            latestPositionMsForAudioSwitch_ = positionMs_;
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

    SetAllSourcesFeedingEnabled(resumeFeeding);
    statusMessage_ = "Seek requested: " +
                     std::to_string(positionSeconds) + " s";
    return true;
}

bool PlayerBackend::SetTrickRate(int rate)
{
    if (player_ == nullptr || stopped_) {
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

    const double currentPositionSeconds =
        static_cast<double>(player_->GetPositionMilliseconds()) / 1000.0;
    const bool resumeFeeding = playing_;
    FfmpegDemuxSource* audioSource =
        activeAudioSource_ != source_.get() ? activeAudioSource_ : nullptr;
    if (!source_->PrepareSeek(currentPositionSeconds) ||
        (audioSource != nullptr &&
         !audioSource->PrepareSeek(currentPositionSeconds))) {
        statusMessage_ = "FFmpeg cannot reposition for trick play";
        return false;
    }
    SetAllSourcesFeedingEnabled(false);
    if (!source_->Seek(currentPositionSeconds) ||
        (audioSource != nullptr &&
         !audioSource->Seek(currentPositionSeconds))) {
        SetAllSourcesFeedingEnabled(resumeFeeding);
        statusMessage_ = "FFmpeg could not reposition for trick play";
        return false;
    }
    if (!player_->Flush(currentPositionSeconds, rate, false, false)) {
        SetAllSourcesFeedingEnabled(resumeFeeding);
        statusMessage_ = "Player Interface trick-play request failed";
        return false;
    }

    SetAllSourcesFeedingEnabled(resumeFeeding);
    statusMessage_ = rate == 1
        ? "Normal playback requested"
        : "Trick rate requested: " + std::to_string(rate) + "x";
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

bool PlayerBackend::SelectAudioTrackId(const std::string& trackId)
{
    if (source_ == nullptr || stopped_) {
        statusMessage_ = "Cannot switch audio: playback has stopped";
        return false;
    }
    if (!source_->IsDashMedia()) {
        statusMessage_ = "Manual audio switching currently requires DASH";
        return false;
    }
    if (selection_.audioTrackId == trackId) {
        statusMessage_ = "Audio track is already selected: " + trackId;
        return true;
    }
    if (audioSwitchInProgress_) {
        statusMessage_ = "An audio track switch is already in progress";
        return false;
    }

    MediaTrackSelection nextSelection = selection_;
    nextSelection.audioTrackId = trackId;
    FfmpegDemuxSource* previousAudioSource = activeAudioSource_;
    if (previousAudioSource == nullptr) {
        statusMessage_ = "No active audio source is available to switch";
        return false;
    }

    const long long currentPositionMilliseconds =
        player_->GetPositionMilliseconds();
    const double currentPositionSeconds =
        static_cast<double>(currentPositionMilliseconds) / 1000.0;
    SetActiveAudioSource(nullptr);
    previousAudioSource->SetTrackEnabled(eGST_MEDIATYPE_AUDIO, false);
    player_->FlushTrack(
        eGST_MEDIATYPE_AUDIO, currentPositionSeconds, 0.0, 0.0);

    {
        const std::lock_guard<std::mutex> lock(audioSwitchMutex_);
        audioSwitchReady_ = false;
        preparedAudioSource_.reset();
        audioSwitchError_.clear();
        preparedAudioPositionSeconds_ = currentPositionSeconds;
    }
    previousAudioSource_ = previousAudioSource;
    requestedAudioTrackId_ = trackId;
    cancelAudioSwitch_ = false;
    audioSwitchInProgress_ = true;
    latestPositionMsForAudioSwitch_ = currentPositionMilliseconds;
    LogStartupMilestone("audio-track-switch-prepare-begin: track=" + trackId);

    try {
        audioSwitchThread_ = std::thread(
            [this, selection = std::move(nextSelection)]() mutable {
                auto candidate = std::make_unique<FfmpegDemuxSource>();
                std::string error;
                double preparedPositionSeconds = 0.0;
                bool positioned = candidate->Open(
                    url_,
                    [this](const std::string& milestone) {
                        LogStartupMilestone(milestone);
                    },
                    selection,
                    true);
                if (!positioned) {
                    candidate->TakeError(error);
                }

                if (positioned) {
                    positioned = false;
                    for (unsigned int attempt = 0; attempt < 4; ++attempt) {
                        if (cancelAudioSwitch_) {
                            break;
                        }
                        const long long targetMilliseconds =
                            latestPositionMsForAudioSwitch_.load();
                        const double targetSeconds =
                            static_cast<double>(targetMilliseconds) / 1000.0;
                        if (!candidate->PrepareSeek(targetSeconds) ||
                            !candidate->Seek(targetSeconds)) {
                            candidate->TakeError(error);
                            break;
                        }

                        preparedPositionSeconds = targetSeconds;
                        const long long latestMilliseconds =
                            latestPositionMsForAudioSwitch_.load();
                        const long long positionDelta =
                            std::llabs(latestMilliseconds -
                                       targetMilliseconds);
                        if (positionDelta <= 1000) {
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
                        candidate->Stop();
                    }
                    audioSwitchError_ = std::move(error);
                    preparedAudioPositionSeconds_ = preparedPositionSeconds;
                    audioSwitchReady_ = true;
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
        static_cast<double>(player_->GetPositionMilliseconds()) / 1000.0;
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
    if (audioSwitchInProgress_) {
        statusMessage_ =
            "Cannot change video selection while an audio switch is preparing";
        return false;
    }

    const bool resumeFeeding = playing_;
    auto candidate = std::make_unique<FfmpegDemuxSource>();
    LogStartupMilestone(
        reason + "-prepare-begin: audio-track=" + selection.audioTrackId +
        " video-track=" + selection.videoTrackId);
    if (!candidate->Open(
            url_,
            [this](const std::string& milestone) {
                LogStartupMilestone(milestone);
            },
            selection)) {
        std::string error;
        candidate->TakeError(error);
        statusMessage_ = error.empty()
            ? "Could not open the requested DASH selection"
            : "Could not prepare DASH selection: " + error;
        LogStartupMilestone(statusMessage_);
        return false;
    }

    for (unsigned int attempt = 0; attempt < 2; ++attempt) {
        if (resumeFeeding) {
            const double latestPosition =
                static_cast<double>(player_->GetPositionMilliseconds()) /
                1000.0;
            if (latestPosition > positionSeconds + 1.0) {
                positionSeconds = latestPosition;
            }
        }
        if (candidate->PrepareSeek(positionSeconds) &&
            candidate->Seek(positionSeconds)) {
            const double latestPosition = resumeFeeding
                ? static_cast<double>(player_->GetPositionMilliseconds()) /
                      1000.0
                : positionSeconds;
            if (attempt == 0 && latestPosition > positionSeconds + 1.0) {
                positionSeconds = latestPosition;
                continue;
            }
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

    FfmpegDemuxSource* previousAudioSource = activeAudioSource_;
    SetAllSourcesFeedingEnabled(false);
    if (!player_->Flush(positionSeconds, 1, false, true)) {
        SetAllSourcesFeedingEnabled(resumeFeeding);
        statusMessage_ = "Player Interface flush failed during " + reason;
        return false;
    }

    std::unique_ptr<FfmpegDemuxSource> previous = std::move(source_);
    source_ = std::move(candidate);
    SetActiveAudioSource(source_->HasTrack(eGST_MEDIATYPE_AUDIO)
                             ? source_.get()
                             : nullptr);
    source_->SetFeedingEnabled(false);
    ConfigurePlayerForSource(*source_);
    const bool sourceStarted = StartSource();
    const bool playbackStateRestored =
        sourceStarted &&
        (resumeFeeding || player_->Pause(true, false));
    if (!playbackStateRestored) {
        const std::string switchError = sourceStarted
            ? "Could not restore Player Interface state after " + reason
            : statusMessage_;
        source_->Stop();
        source_.reset();
        source_ = std::move(previous);
        SetActiveAudioSource(previousAudioSource);
        const bool previousSourcePositioned =
            source_->Seek(positionSeconds) &&
            (previousAudioSource == nullptr ||
             previousAudioSource == source_.get() ||
             previousAudioSource->Seek(positionSeconds)) &&
            player_->Flush(positionSeconds, 1, false, true);
        if (previousSourcePositioned) {
            ConfigurePlayerForSource(*source_);
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
    previous->Stop();
    selection_ = selection;
    positionMs_ = static_cast<long long>(positionSeconds * 1000.0);
    latestPositionMsForAudioSwitch_ = positionMs_;
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
        " video-track=" + selection_.videoTrackId);
    return true;
}

void PlayerBackend::EvaluateAutoBitrate(
    std::chrono::steady_clock::time_point now)
{
    if (!autoBitrateEnabled_ || !playing_ || source_ == nullptr ||
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
        static_cast<double>(player_->GetPositionMilliseconds()) / 1000.0;
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
    if (audioSwitchThread_.joinable()) {
        audioSwitchThread_.join();
    }
    {
        const std::lock_guard<std::mutex> lock(audioSwitchMutex_);
        preparedAudioSource_.reset();
        audioSwitchReady_ = false;
    }
    audioSwitchInProgress_ = false;
    previousAudioSource_ = nullptr;
    SetActiveAudioSource(nullptr);

    if (audioOnlySource_ != nullptr) {
        audioOnlySource_->SetFeedingEnabled(false);
        audioOnlySource_->Stop();
        audioOnlySource_.reset();
    }
    if (source_ != nullptr) {
        source_->SetFeedingEnabled(false);
        source_->Stop();
    }
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
        playing_ = false;
        break;
    case MESSAGE_WARNING:
        LogStartupMilestone("player-interface-warning: " + event.msg +
                            " | debug: " + event.dbg_info);
        statusMessage_ = event.msg.empty() ? "Playback warning"
                                           : "Warning: " + event.msg;
        break;
    case MESSAGE_EOS:
        LogStartupMilestone("player-interface-eos");
        statusMessage_ = "End of stream";
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
