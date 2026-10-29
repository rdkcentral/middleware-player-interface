#include "PlayerBackend.h"

#include "InterfacePlayerRDK.h"

#include <glib.h>

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
    InterfacePlayerRDK::InitializePlayerGstreamerPlugins();
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

    player_->RegisterBusEvent([this](const BusEventData event) {
        HandleBusEvent(event);
    });

    StreamCodecInfo codecInfo;
    codecInfo.video.mCodecFormat = GST_FORMAT_UNKNOWN;
    player_->ConfigurePipeline(
        std::move(codecInfo),
        false,
        false,
        false,
        0,
        1,
        "pi-player-simulator",
        -1,
        false,
        url_);

    playing_ = true;
    statusMessage_ = "Opening stream";
    nextPositionQuery_ = std::chrono::steady_clock::now();
}

void PlayerBackend::PumpEvents()
{
    g_main_context_iteration(nullptr, FALSE);

    const auto now = std::chrono::steady_clock::now();
    if (player_ != nullptr && !stopped_ && now >= nextPositionQuery_) {
        positionMs_ = player_->GetPositionMilliseconds();
        durationMs_ = player_->GetDurationMilliseconds();
        nextPositionQuery_ = now + std::chrono::milliseconds(250);
    }
}

void PlayerBackend::TogglePlayback()
{
    if (player_ == nullptr || stopped_) {
        return;
    }

    const bool pause = playing_;
    if (!player_->Pause(pause, false)) {
        statusMessage_ = pause ? "Could not pause playback"
                               : "Could not resume playback";
        return;
    }

    playing_ = !pause;
    statusMessage_ = playing_ ? "Playing" : "Paused";
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
        statusMessage_ = event.msg.empty() ? "Playback error" : event.msg;
        playing_ = false;
        break;
    case MESSAGE_WARNING:
        statusMessage_ = event.msg.empty() ? "Playback warning"
                                           : "Warning: " + event.msg;
        break;
    case MESSAGE_EOS:
        statusMessage_ = "End of stream";
        playing_ = false;
        break;
    case MESSAGE_APPLICATION:
    case MESSAGE_STATE_CHANGE:
    case MESSAGE_CLOCK_LOST:
        break;
    }
}
