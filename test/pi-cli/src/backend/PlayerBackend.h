#pragma once

#include <memory>
#include <string>
#include <chrono>
#include <atomic>
#include <mutex>

class InterfacePlayerRDK;
class FfmpegDemuxSource;
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

    bool Start();
    void PumpEvents();
    bool Play();
    bool Pause();
    bool Seek(double positionSeconds);
    bool SetTrickRate(int rate);
    void Stop();
    PlaybackStatus GetStatus() const;

private:
    void HandleBusEvent(const BusEventData& event);
    void LogStartupMilestone(const std::string& milestone);

    std::string url_;
    std::string statusMessage_ = "Starting playback";
    std::unique_ptr<FfmpegDemuxSource> source_;
    std::unique_ptr<InterfacePlayerRDK> player_;
    std::chrono::steady_clock::time_point startupAt_{};
    std::chrono::steady_clock::time_point nextPositionQuery_{};
    std::chrono::steady_clock::time_point nextDurationQuery_{};
    std::mutex startupLogMutex_;
    long long positionMs_ = 0;
    long durationMs_ = 0;
    std::atomic<bool> pipelinePlayingNotified_{false};
    std::atomic<bool> firstVideoFrameReported_{false};
    std::atomic<bool> firstVideoFrameDisplayedReported_{false};
    std::atomic<bool> firstVideoBufferReported_{false};
    std::atomic<bool> firstAudioBufferReported_{false};
    bool firstPositionReported_ = false;
    bool playing_ = false;
    bool stopped_ = false;
};
