#pragma once

#include <memory>
#include <string>
#include <chrono>

class InterfacePlayerRDK;
struct BusEventData;

struct PlaybackStatus {
    std::string message;
    bool playing = false;
    bool stopped = false;
    long long positionMs = 0;
    long durationMs = 0;
};

class PlayerBackend {
public:
    explicit PlayerBackend(std::string url);
    ~PlayerBackend();

    PlayerBackend(const PlayerBackend&) = delete;
    PlayerBackend& operator=(const PlayerBackend&) = delete;

    void Start();
    void PumpEvents();
    void TogglePlayback();
    void Stop();
    PlaybackStatus GetStatus() const;

private:
    void HandleBusEvent(const BusEventData& event);

    std::string url_;
    std::string statusMessage_ = "Starting playback";
    std::unique_ptr<InterfacePlayerRDK> player_;
    std::chrono::steady_clock::time_point nextPositionQuery_{};
    long long positionMs_ = 0;
    long durationMs_ = 0;
    bool playing_ = false;
    bool stopped_ = false;
};
