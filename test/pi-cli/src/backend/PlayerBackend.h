#pragma once

#include "AbrController.h"
#include "MediaTrackCatalog.h"

#include <memory>
#include <string>
#include <chrono>
#include <atomic>
#include <cstddef>
#include <cstdint>
#include <mutex>
#include <vector>

class InterfacePlayerRDK;
class FfmpegDemuxSource;
struct BusEventData;

struct PlaybackStatus {
    std::string message;
    bool playing = false;
    bool stopped = false;
    bool autoBitrateEnabled = false;
    long long positionMs = 0;
    long durationMs = 0;
    uint64_t estimatedBandwidthBitsPerSecond = 0;
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
    bool SelectAudioTrack(std::size_t oneBasedIndex);
    bool SelectVideoResolution(int width, int height);
    bool SetAutoBitrate(bool enabled);
    std::vector<MediaTrackInfo> GetTrackCatalog() const;
    void Stop();
    PlaybackStatus GetStatus() const;

private:
    void HandleBusEvent(const BusEventData& event);
    void LogStartupMilestone(const std::string& milestone);
    bool StartSource();
    bool ApplySelection(const MediaTrackSelection& selection,
                        double positionSeconds,
                        const std::string& reason);
    bool SelectAudioTrackId(const std::string& trackId);
    bool SelectVideoTrackId(const std::string& trackId);
    void EvaluateAutoBitrate(
        std::chrono::steady_clock::time_point now);

    std::string url_;
    MediaTrackSelection selection_;
    std::string statusMessage_ = "Starting playback";
    std::unique_ptr<FfmpegDemuxSource> source_;
    std::unique_ptr<InterfacePlayerRDK> player_;
    std::chrono::steady_clock::time_point startupAt_{};
    std::chrono::steady_clock::time_point nextPositionQuery_{};
    std::chrono::steady_clock::time_point nextDurationQuery_{};
    std::chrono::steady_clock::time_point nextProgressLog_{};
    std::chrono::steady_clock::time_point nextAbrEvaluation_{};
    std::mutex startupLogMutex_;
    long long positionMs_ = 0;
    long durationMs_ = 0;
    uint64_t lastBandwidthEstimateBitsPerSecond_ = 0;
    AbrController abrController_;
    std::atomic<bool> pipelinePlayingNotified_{false};
    std::atomic<bool> firstVideoFrameReported_{false};
    std::atomic<bool> firstVideoFrameDisplayedReported_{false};
    std::atomic<bool> firstVideoBufferReported_{false};
    std::atomic<bool> firstAudioBufferReported_{false};
    bool firstPositionReported_ = false;
    bool autoBitrateEnabled_ = false;
    bool playing_ = false;
    bool stopped_ = false;
};
