#pragma once

#include "MediaTrackCatalog.h"
#include "MediaSample.h"

#include <array>
#include <atomic>
#include <condition_variable>
#include <cstddef>
#include <cstdint>
#include <deque>
#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

struct AVFormatContext;
class DashResourceProxy;

class FfmpegDemuxSource {
public:
    using ProgressCallback = std::function<void(const std::string&)>;
    using SendSampleCallback =
        std::function<bool(int, MediaSample&&, bool&)>;
    using EndOfStreamCallback = std::function<void(int)>;

    FfmpegDemuxSource();
    ~FfmpegDemuxSource();

    FfmpegDemuxSource(const FfmpegDemuxSource&) = delete;
    FfmpegDemuxSource& operator=(const FfmpegDemuxSource&) = delete;

    bool Open(const std::string& url,
              ProgressCallback progress,
              const MediaTrackSelection& selection = {},
              bool audioOnly = false,
              const std::vector<uint8_t>* manifestSnapshot = nullptr,
              const std::string& manifestBaseUrl = {},
              const std::atomic<bool>* cancellation = nullptr);
    StreamCodecInfo GetCodecInfo() const;
    std::vector<MediaTrackInfo> GetTrackCatalog() const;
    std::vector<uint8_t> GetDashManifestSnapshot() const;
    std::string GetDashManifestBaseUrl() const;
    uint64_t GetEstimatedBandwidthBitsPerSecond() const;
    bool IsDashMedia() const;
    int GetMediaMode() const;
    long long GetDurationMilliseconds() const;
    bool HasTrack(int mediaType) const;
    bool Start(SendSampleCallback sendSample,
               EndOfStreamCallback endOfStream);
    void NeedData(int mediaType);
    void EnoughData(int mediaType);
    void SetTrackEnabled(int mediaType, bool enabled);
    void SetFeedingEnabled(bool enabled);
    bool PrepareSeek(double positionSeconds);
    bool Seek(double positionSeconds,
              bool markDiscontinuity = true,
              bool seekForward = false);
    void SetDiscardBeforePosition(int mediaType, double positionSeconds);
    bool TakeError(std::string& error);
    void Stop();

private:
    struct QueuedPacket {
        std::vector<uint8_t> data;
        double pts = 0.0;
        double dts = 0.0;
        double duration = 0.0;
        bool discontinuity = false;
    };

    struct Track {
        int mediaType = -1;
        int streamIndex = -1;
        GstStreamOutputFormat format = GST_FORMAT_INVALID;
        int width = 0;
        int height = 0;
        int sampleRate = 0;
        int channels = 0;
        double timeBaseSeconds = 0.0;
        double fallbackDuration = 0.0;
        double lastTimestamp = 0.0;
        int nalLengthSize = 4;
        bool inputAnnexB = false;
        bool ready = false;
        bool hasTimestamp = false;
        bool needData = false;
        bool inputEos = false;
        bool eosNotified = false;
        bool sending = false;
        double discardBeforeSeconds = -1.0;
        std::atomic<bool> enabled{true};
        bool demuxEnabled = true;
        bool discontinuityNext = true;
        uint64_t packetsPrepared = 0;
        uint64_t samplesSubmitted = 0;
        uint32_t needDataLogCount = 0;
        uint32_t enoughDataLogCount = 0;
        std::vector<uint8_t> codecData;
        std::vector<std::vector<uint8_t>> sequenceParameterSets;
        std::vector<std::vector<uint8_t>> pictureParameterSets;
        std::deque<QueuedPacket> packets;
        size_t queuedBytes = 0;
        std::mutex mutex;
        std::condition_variable condition;
        std::thread feederThread;
    };

    static int InterruptCallback(void* opaque);
    static bool IsAdts(const uint8_t* data, size_t size);
    static bool FindStartCode(const uint8_t* data,
                              size_t size,
                              size_t from,
                              size_t& position,
                              size_t& length);
    static bool BuildAvcConfiguration(
        const std::vector<std::vector<uint8_t>>& sequenceParameterSets,
        const std::vector<std::vector<uint8_t>>& pictureParameterSets,
        std::vector<uint8_t>& configuration);

    Track* FindTrack(int mediaType);
    const Track* FindTrack(int mediaType) const;
    bool InitializeTrack(Track& track);
    bool PrimeTracks();
    bool ReadAndQueuePacket(bool priming);
    bool PreparePacket(Track& track,
                       const struct AVPacket* packet,
                       QueuedPacket& output);
    bool NormalizeH264(Track& track,
                       const uint8_t* data,
                       size_t size,
                       std::vector<uint8_t>& output);
    bool CollectH264Configuration(Track& track,
                                  const uint8_t* data,
                                  size_t size,
                                  bool annexB);
    bool QueuePacket(Track& track, QueuedPacket&& packet);
    void ApplyTrackDiscard();
    void BeginTimedIo();
    bool EndTimedIo();
    void DemuxLoop();
    void FeedLoop(Track& track);
    void MarkInputEos();
    void ReportProgress(const std::string& milestone) const;
    void SetError(std::string error);
    bool InputReady() const;

    AVFormatContext* formatContext_ = nullptr;
    std::unique_ptr<DashResourceProxy> dashResourceProxy_;
    std::array<Track, 2> tracks_{};
    mutable std::mutex demuxMutex_;
    mutable std::mutex errorMutex_;
    std::string error_;
    std::string url_;
    std::vector<uint8_t> dashManifestSnapshot_;
    std::string dashManifestBaseUrl_;
    std::vector<MediaTrackInfo> trackCatalog_;
    int mediaMode_ = -1;
    long long durationMilliseconds_ = 0;
    std::atomic<bool> stopping_{false};
    const std::atomic<bool>* cancellation_ = nullptr;
    std::atomic<bool> inputPaused_{false};
    std::atomic<bool> feedingEnabled_{true};
    std::atomic<uint64_t> seekGeneration_{0};
    std::atomic<int64_t> ioDeadlineNanoseconds_{0};
    std::atomic<bool> ioTimedOut_{false};
    std::mutex inputPauseMutex_;
    std::condition_variable inputPauseCondition_;
    std::thread demuxThread_;
    ProgressCallback progress_;
    SendSampleCallback sendSample_;
    EndOfStreamCallback endOfStream_;
};
