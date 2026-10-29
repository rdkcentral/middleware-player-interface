#pragma once

#include "MediaSample.h"

#include <array>
#include <atomic>
#include <condition_variable>
#include <cstddef>
#include <cstdint>
#include <deque>
#include <functional>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

struct AVFormatContext;

class FfmpegDemuxSource {
public:
    using SendSampleCallback =
        std::function<bool(int, MediaSample&&, bool&)>;
    using EndOfStreamCallback = std::function<void(int)>;

    FfmpegDemuxSource();
    ~FfmpegDemuxSource();

    FfmpegDemuxSource(const FfmpegDemuxSource&) = delete;
    FfmpegDemuxSource& operator=(const FfmpegDemuxSource&) = delete;

    bool Open(const std::string& url);
    StreamCodecInfo GetCodecInfo() const;
    int GetMediaMode() const;
    long long GetDurationMilliseconds() const;
    bool HasTrack(int mediaType) const;
    bool Start(SendSampleCallback sendSample,
               EndOfStreamCallback endOfStream);
    void NeedData(int mediaType);
    void EnoughData(int mediaType);
    void SetFeedingEnabled(bool enabled);
    bool Seek(double positionSeconds);
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
        bool discontinuityNext = true;
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
    void DemuxLoop();
    void FeedLoop(Track& track);
    void MarkInputEos();
    void SetError(std::string error);
    bool InputReady() const;

    AVFormatContext* formatContext_ = nullptr;
    std::array<Track, 2> tracks_{};
    mutable std::mutex demuxMutex_;
    mutable std::mutex errorMutex_;
    std::string error_;
    std::string url_;
    int mediaMode_ = -1;
    long long durationMilliseconds_ = 0;
    std::atomic<bool> stopping_{false};
    std::atomic<bool> inputPaused_{false};
    std::atomic<bool> feedingEnabled_{true};
    std::atomic<uint64_t> seekGeneration_{0};
    std::mutex inputPauseMutex_;
    std::condition_variable inputPauseCondition_;
    std::thread demuxThread_;
    SendSampleCallback sendSample_;
    EndOfStreamCallback endOfStream_;
};
