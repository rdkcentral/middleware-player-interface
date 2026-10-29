#pragma once

#include <memory>
#include <string>
#include <chrono>
#include <atomic>
#include <mutex>

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
    void LogStartupMilestone(const char* milestone);

    std::string url_;
    std::string statusMessage_ = "Starting playback";
    std::unique_ptr<InterfacePlayerRDK> player_;
    std::chrono::steady_clock::time_point startupAt_{};
    std::chrono::steady_clock::time_point nextPositionQuery_{};
    std::chrono::steady_clock::time_point nextDurationQuery_{};
    std::mutex startupLogMutex_;
    long long positionMs_ = 0;
    long durationMs_ = 0;
    std::atomic<bool> pipelinePlayingNotified_{false};
    std::atomic<bool> firstVideoFrameReported_{false};
    bool firstPositionReported_ = false;
    bool playing_ = false;
    bool stopped_ = false;
};
