#include "PlayerBackend.h"

#include "InterfacePlayerRDK.h"

#include <glib.h>

#include <iostream>
#include <utility>

namespace {

constexpr int kDirectUriMediaMode = 2;

} // namespace

PlayerBackend::PlayerBackend(std::string url)
    : url_(std::move(url))
{
}

PlayerBackend::~PlayerBackend()
{
    Stop();
}

void PlayerBackend::Start()
{
    startupAt_ = std::chrono::steady_clock::now();
    LogStartupMilestone("start");
    LogStartupMilestone("plugin-initialization-begin");
    InterfacePlayerRDK::InitializePlayerGstreamerPlugins();
    LogStartupMilestone("plugin-initialization-complete");
    player_ = std::make_unique<InterfacePlayerRDK>();
    player_->SetPlayerName("pi-player-simulator");

    Configs* config = player_->m_gstConfigParam;
    // The corresponding media-format enum is private in the installed package.
    config->media = kDirectUriMediaMode;
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
        pipelinePlayingNotified_.store(true);
        LogStartupMilestone("interface-playing-notification");
    };
    player_->FirstFrameCallback(
        [this](int mediaType,
               bool,
               bool,
               bool&,
               bool&) {
            if (mediaType == eGST_MEDIATYPE_VIDEO &&
                !firstVideoFrameReported_.exchange(true)) {
                LogStartupMilestone("decoded-video-first-frame-callback");
            }
        });
    player_->RegisterBusEvent([this](const BusEventData event) {
        HandleBusEvent(event);
    });

    StreamCodecInfo codecInfo;
    codecInfo.video.mCodecFormat = GST_FORMAT_UNKNOWN;
    LogStartupMilestone("configure-pipeline-begin");
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
    LogStartupMilestone("configure-pipeline-returned");

    playing_ = true;
    statusMessage_ = "Opening stream";
    nextPositionQuery_ = std::chrono::steady_clock::now();
    nextDurationQuery_ = nextPositionQuery_;
}

void PlayerBackend::PumpEvents()
{
    g_main_context_iteration(nullptr, FALSE);

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

    if (!player_->Pause(true, false)) {
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

    if (!player_->Flush(positionSeconds, 1, false, true)) {
        statusMessage_ = "Seek request failed";
        return false;
    }

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

    const double currentPositionSeconds =
        static_cast<double>(player_->GetPositionMilliseconds()) / 1000.0;
    if (!player_->Flush(currentPositionSeconds, rate, false, false)) {
        statusMessage_ = "Trick-play request failed";
        return false;
    }

    statusMessage_ = rate == 1
        ? "Normal playback requested"
        : "Trick rate requested: " + std::to_string(rate) + "x";
    return true;
}

void PlayerBackend::Stop()
{
    if (player_ == nullptr || stopped_) {
        return;
    }

    player_->Stop(true);
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
        LogStartupMilestone("gstreamer-error");
        statusMessage_ = event.msg.empty() ? "Playback error" : event.msg;
        playing_ = false;
        break;
    case MESSAGE_WARNING:
        LogStartupMilestone("gstreamer-warning");
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
        if (event.msg == "pi-player-simulator") {
            LogStartupMilestone("pipeline-state-change-notification");
        }
        break;
    }
}

void PlayerBackend::LogStartupMilestone(const char* milestone)
{
    const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - startupAt_).count();
    const std::lock_guard<std::mutex> lock(startupLogMutex_);
    std::clog << "[PI-CLI] +" << elapsed << "ms " << milestone << std::endl;
}
