#include "PlayerBackend.h"

#include "FfmpegDemuxSource.h"
#include "InterfacePlayerRDK.h"

#include <glib.h>

#include <iostream>
#include <utility>

namespace {

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
    if (!source_->Open(url_)) {
        std::string error;
        source_->TakeError(error);
        statusMessage_ = error.empty() ? "Could not open the stream"
                                       : "FFmpeg ingest failed: " + error;
        LogStartupMilestone(statusMessage_);
        return false;
    }
    LogStartupMilestone("ffmpeg-manifest-and-stream-info-ready");

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
        source_->NeedData(mediaType);
    });
    player_->RegisterEnoughDataCb([this](int mediaType) {
        source_->EnoughData(mediaType);
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

    if (!source_->Start(
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
        source_->TakeError(error);
        statusMessage_ = error.empty() ? "Could not start FFmpeg ingest"
                                       : "FFmpeg ingest failed: " + error;
        LogStartupMilestone(statusMessage_);
        Stop();
        return false;
    }

    playing_ = true;
    statusMessage_ = "Opening stream";
    nextPositionQuery_ = std::chrono::steady_clock::now();
    nextDurationQuery_ = nextPositionQuery_;
    return true;
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
    if (player_ != nullptr && !stopped_ &&
        pipelinePlayingNotified_.load() && now >= nextPositionQuery_) {
        positionMs_ = player_->GetPositionMilliseconds();
        if (positionMs_ > 0 && !firstPositionReported_) {
            firstPositionReported_ = true;
            LogStartupMilestone("playback-position-advanced");
        }
        nextPositionQuery_ = now + std::chrono::milliseconds(250);

        if (now >= nextDurationQuery_) {
            durationMs_ = player_->GetDurationMilliseconds();
            if (durationMs_ <= 0 && source_ != nullptr) {
                durationMs_ = source_->GetDurationMilliseconds();
            }
            nextDurationQuery_ = now + std::chrono::seconds(1);
        }
    }
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

    source_->SetFeedingEnabled(true);
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

    source_->SetFeedingEnabled(false);
    if (!player_->Pause(true, false)) {
        source_->SetFeedingEnabled(true);
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

    const bool resumeFeeding = playing_;
    source_->SetFeedingEnabled(false);
    if (!source_->Seek(positionSeconds)) {
        source_->SetFeedingEnabled(resumeFeeding);
        statusMessage_ = "FFmpeg could not seek to the requested position";
        return false;
    }
    if (!player_->Flush(positionSeconds, 1, false, true)) {
        source_->SetFeedingEnabled(resumeFeeding);
        statusMessage_ = "Player Interface seek request failed";
        return false;
    }

    source_->SetFeedingEnabled(resumeFeeding);
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
    if (!IsSupportedTrickRate(rate)) {
        statusMessage_ = "Unsupported trick rate";
        return false;
    }

    const double currentPositionSeconds =
        static_cast<double>(player_->GetPositionMilliseconds()) / 1000.0;
    const bool resumeFeeding = playing_;
    source_->SetFeedingEnabled(false);
    if (!source_->Seek(currentPositionSeconds)) {
        source_->SetFeedingEnabled(resumeFeeding);
        statusMessage_ = "FFmpeg could not reposition for trick play";
        return false;
    }
    if (!player_->Flush(currentPositionSeconds, rate, false, false)) {
        source_->SetFeedingEnabled(resumeFeeding);
        statusMessage_ = "Player Interface trick-play request failed";
        return false;
    }

    source_->SetFeedingEnabled(resumeFeeding);
    statusMessage_ = rate == 1
        ? "Normal playback requested"
        : "Trick rate requested: " + std::to_string(rate) + "x";
    return true;
}

void PlayerBackend::Stop()
{
    if (stopped_) {
        return;
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
    status.positionMs = positionMs_;
    status.durationMs = durationMs_;
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
