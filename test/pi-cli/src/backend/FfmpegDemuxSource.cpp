#include "FfmpegDemuxSource.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/error.h>
#include <libavutil/mathematics.h>
#include <libavutil/mem.h>
#include <libavutil/opt.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
}

#include <algorithm>
#include <chrono>
#include <cctype>
#include <climits>
#include <cmath>
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <exception>
#include <limits>
#include <sstream>
#include <system_error>

namespace {

constexpr size_t kMaximumQueuedBytes = 8 * 1024 * 1024;
constexpr size_t kMaximumQueuedPackets = 128;
constexpr size_t kMaximumPrimingPackets = 4096;
constexpr int kIoTimeoutSeconds = 30;
constexpr size_t kMaximumManifestBytes = 16 * 1024 * 1024;
constexpr int kManifestIoBufferSize = 32 * 1024;
// Configs::media uses these private HLS/DASH values in the installed API.
constexpr int kPlayerInterfaceHlsMediaMode = 0;
constexpr int kPlayerInterfaceDashMediaMode = 1;

struct ManifestMemory {
    const std::vector<uint8_t>* data = nullptr;
    size_t position = 0;
};

struct DashManifestSelection {
    std::vector<uint8_t> data;
    size_t removedAdaptationSets = 0;
    size_t removedVideoRepresentations = 0;
    bool keptVideo = false;
    bool keptAudio = false;
};

class ProgressWatchdog {
public:
    ProgressWatchdog(FfmpegDemuxSource::ProgressCallback progress,
                     std::string phase)
        : progress_(std::move(progress))
        , phase_(std::move(phase))
        , thread_(&ProgressWatchdog::Run, this)
    {
    }

    ~ProgressWatchdog()
    {
        {
            const std::lock_guard<std::mutex> lock(mutex_);
            stopped_ = true;
        }
        condition_.notify_all();
        if (thread_.joinable()) {
            thread_.join();
        }
    }

    ProgressWatchdog(const ProgressWatchdog&) = delete;
    ProgressWatchdog& operator=(const ProgressWatchdog&) = delete;

private:
    void Run()
    {
        std::unique_lock<std::mutex> lock(mutex_);
        int elapsedSeconds = 5;
        while (!condition_.wait_for(lock,
                                    std::chrono::seconds(5),
                                    [this] { return stopped_; })) {
            lock.unlock();
            if (progress_) {
                progress_(phase_ + "-still-waiting-" +
                          std::to_string(elapsedSeconds) + "s");
            }
            elapsedSeconds += 5;
            lock.lock();
        }
    }

    FfmpegDemuxSource::ProgressCallback progress_;
    std::string phase_;
    std::mutex mutex_;
    std::condition_variable condition_;
    bool stopped_ = false;
    std::thread thread_;
};

std::string AvErrorString(int error)
{
    char buffer[AV_ERROR_MAX_STRING_SIZE] = {};
    av_strerror(error, buffer, sizeof(buffer));
    return buffer;
}

bool IsDashUrl(const std::string& url)
{
    const size_t pathEnd = url.find_first_of("?#");
    const size_t end = pathEnd == std::string::npos ? url.size() : pathEnd;
    if (end < 4) {
        return false;
    }

    return std::tolower(static_cast<unsigned char>(url[end - 4])) == '.' &&
           std::tolower(static_cast<unsigned char>(url[end - 3])) == 'm' &&
           std::tolower(static_cast<unsigned char>(url[end - 2])) == 'p' &&
           std::tolower(static_cast<unsigned char>(url[end - 1])) == 'd';
}

std::string XmlProperty(xmlNodePtr node, const char* name)
{
    xmlChar* value = xmlGetProp(node, BAD_CAST name);
    if (value == nullptr) {
        return {};
    }
    std::string result(reinterpret_cast<const char*>(value));
    xmlFree(value);
    return result;
}

bool HasRole(xmlNodePtr adaptationSet, const char* role)
{
    for (xmlNodePtr child = adaptationSet->children;
         child != nullptr;
         child = child->next) {
        if (child->type == XML_ELEMENT_NODE &&
            xmlStrEqual(child->name, BAD_CAST "Role")) {
            std::string value = XmlProperty(child, "value");
            std::transform(value.begin(), value.end(), value.begin(), [](char c) {
                return static_cast<char>(
                    std::tolower(static_cast<unsigned char>(c)));
            });
            if (value.find(role) != std::string::npos) {
                return true;
            }
        }
    }
    return false;
}

bool IsTrickMode(xmlNodePtr adaptationSet)
{
    for (xmlNodePtr child = adaptationSet->children;
         child != nullptr;
         child = child->next) {
        if (child->type != XML_ELEMENT_NODE ||
            (!xmlStrEqual(child->name, BAD_CAST "EssentialProperty") &&
             !xmlStrEqual(child->name, BAD_CAST "SupplementalProperty"))) {
            continue;
        }
        const std::string scheme = XmlProperty(child, "schemeIdUri");
        if (scheme.find("trickmode") != std::string::npos) {
            return true;
        }
    }
    return HasRole(adaptationSet, "trickmode");
}

std::string AdaptationSetMediaType(xmlNodePtr adaptationSet)
{
    std::string mediaType = XmlProperty(adaptationSet, "contentType");
    if (!mediaType.empty()) {
        return mediaType;
    }

    std::string mimeType = XmlProperty(adaptationSet, "mimeType");
    if (mimeType.empty()) {
        for (xmlNodePtr child = adaptationSet->children;
             child != nullptr;
             child = child->next) {
            if (child->type == XML_ELEMENT_NODE &&
                xmlStrEqual(child->name, BAD_CAST "Representation")) {
                mimeType = XmlProperty(child, "mimeType");
                if (!mimeType.empty()) {
                    break;
                }
            }
        }
    }

    if (mimeType.rfind("video/", 0) == 0) {
        return "video";
    }
    if (mimeType.rfind("audio/", 0) == 0) {
        return "audio";
    }
    return {};
}

void KeepFirstRepresentation(xmlNodePtr adaptationSet,
                             size_t& removedVideoRepresentations)
{
    bool keptRepresentation = false;
    for (xmlNodePtr child = adaptationSet->children; child != nullptr;) {
        xmlNodePtr next = child->next;
        if (child->type == XML_ELEMENT_NODE &&
            xmlStrEqual(child->name, BAD_CAST "Representation")) {
            if (keptRepresentation) {
                xmlUnlinkNode(child);
                xmlFreeNode(child);
                ++removedVideoRepresentations;
            } else {
                keptRepresentation = true;
            }
        }
        child = next;
    }
}

bool FilterDashManifest(const std::vector<uint8_t>& input,
                        DashManifestSelection& selection,
                        std::string& error)
{
    if (input.empty() || input.size() > static_cast<size_t>(INT_MAX)) {
        error = "DASH manifest is empty or too large to parse";
        return false;
    }

    xmlDocPtr document = xmlReadMemory(
        reinterpret_cast<const char*>(input.data()),
        static_cast<int>(input.size()),
        nullptr,
        nullptr,
        XML_PARSE_NONET | XML_PARSE_NOERROR | XML_PARSE_NOWARNING |
            XML_PARSE_COMPACT);
    if (document == nullptr) {
        const xmlError* parseError = xmlGetLastError();
        error = "Could not parse DASH manifest XML";
        if (parseError != nullptr && parseError->message != nullptr) {
            error += ": ";
            error += parseError->message;
        }
        return false;
    }

    xmlNodePtr root = xmlDocGetRootElement(document);
    if (root == nullptr || !xmlStrEqual(root->name, BAD_CAST "MPD")) {
        error = "The DASH URL did not return an MPD document";
        xmlFreeDoc(document);
        return false;
    }

    for (xmlNodePtr period = root->children;
         period != nullptr;
         period = period->next) {
        if (period->type != XML_ELEMENT_NODE ||
            !xmlStrEqual(period->name, BAD_CAST "Period")) {
            continue;
        }

        xmlNodePtr selectedVideo = nullptr;
        xmlNodePtr selectedAudio = nullptr;
        for (xmlNodePtr adaptationSet = period->children;
             adaptationSet != nullptr;
             adaptationSet = adaptationSet->next) {
            if (adaptationSet->type != XML_ELEMENT_NODE ||
                !xmlStrEqual(adaptationSet->name, BAD_CAST "AdaptationSet")) {
                continue;
            }

            const std::string mediaType =
                AdaptationSetMediaType(adaptationSet);
            if (mediaType == "video" && selectedVideo == nullptr &&
                !IsTrickMode(adaptationSet)) {
                selectedVideo = adaptationSet;
            } else if (mediaType == "audio" && selectedAudio == nullptr &&
                       !HasRole(adaptationSet, "commentary") &&
                       !HasRole(adaptationSet, "description")) {
                selectedAudio = adaptationSet;
            }
        }

        for (xmlNodePtr adaptationSet = period->children;
             adaptationSet != nullptr;) {
            xmlNodePtr next = adaptationSet->next;
            if (adaptationSet->type == XML_ELEMENT_NODE &&
                xmlStrEqual(adaptationSet->name, BAD_CAST "AdaptationSet")) {
                const std::string mediaType =
                    AdaptationSetMediaType(adaptationSet);
                const bool keep =
                    (mediaType == "video" &&
                     adaptationSet == selectedVideo) ||
                    (mediaType == "audio" &&
                     adaptationSet == selectedAudio);
                if (!keep) {
                    xmlUnlinkNode(adaptationSet);
                    xmlFreeNode(adaptationSet);
                    ++selection.removedAdaptationSets;
                } else if (mediaType == "video") {
                    selection.keptVideo = true;
                    KeepFirstRepresentation(
                        adaptationSet,
                        selection.removedVideoRepresentations);
                } else {
                    selection.keptAudio = true;
                }
            }
            adaptationSet = next;
        }
    }

    if (!selection.keptVideo && !selection.keptAudio) {
        error = "The MPD has no supported audio or video adaptation set";
        xmlFreeDoc(document);
        return false;
    }

    xmlChar* serialized = nullptr;
    int serializedSize = 0;
    xmlDocDumpMemoryEnc(document, &serialized, &serializedSize, "UTF-8");
    xmlFreeDoc(document);
    if (serialized == nullptr || serializedSize <= 0) {
        if (serialized != nullptr) {
            xmlFree(serialized);
        }
        error = "Could not serialize the filtered DASH manifest";
        return false;
    }

    selection.data.assign(serialized, serialized + serializedSize);
    xmlFree(serialized);
    return true;
}

int ReadManifestMemory(void* opaque, uint8_t* buffer, int bufferSize)
{
    auto* memory = static_cast<ManifestMemory*>(opaque);
    if (memory == nullptr || memory->data == nullptr || bufferSize <= 0) {
        return AVERROR(EINVAL);
    }
    if (memory->position >= memory->data->size()) {
        return AVERROR_EOF;
    }

    const size_t count = std::min(
        static_cast<size_t>(bufferSize),
        memory->data->size() - memory->position);
    std::memcpy(buffer, memory->data->data() + memory->position, count);
    memory->position += count;
    return static_cast<int>(count);
}

int64_t SeekManifestMemory(void* opaque, int64_t offset, int whence)
{
    auto* memory = static_cast<ManifestMemory*>(opaque);
    if (memory == nullptr || memory->data == nullptr) {
        return AVERROR(EINVAL);
    }
    whence &= ~AVSEEK_FORCE;
    if (whence == AVSEEK_SIZE) {
        return static_cast<int64_t>(memory->data->size());
    }

    int64_t base = 0;
    if (whence == SEEK_CUR) {
        base = static_cast<int64_t>(memory->position);
    } else if (whence == SEEK_END) {
        base = static_cast<int64_t>(memory->data->size());
    } else if (whence != SEEK_SET) {
        return AVERROR(EINVAL);
    }

    const int64_t size = static_cast<int64_t>(memory->data->size());
    if (offset < -base || offset > size - base) {
        return AVERROR(EINVAL);
    }
    const int64_t position = base + offset;
    if (position < 0 ||
        position > static_cast<int64_t>(memory->data->size())) {
        return AVERROR(EINVAL);
    }
    memory->position = static_cast<size_t>(position);
    return position;
}

} // namespace

FfmpegDemuxSource::FfmpegDemuxSource()
{
    tracks_[0].mediaType = eGST_MEDIATYPE_VIDEO;
    tracks_[1].mediaType = eGST_MEDIATYPE_AUDIO;
}

FfmpegDemuxSource::~FfmpegDemuxSource()
{
    Stop();
}

bool FfmpegDemuxSource::Open(const std::string& url,
                             ProgressCallback progress)
{
    if (url.empty()) {
        SetError("A stream URL is required");
        return false;
    }

    url_ = url;
    progress_ = std::move(progress);
    stopping_ = false;
    inputPaused_ = false;
    feedingEnabled_ = true;

    static std::once_flag networkInitialization;
    static int networkInitializationResult = 0;
    std::call_once(networkInitialization, [] {
        networkInitializationResult = avformat_network_init();
    });
    if (networkInitializationResult < 0) {
        SetError("FFmpeg network initialization failed: " +
                 AvErrorString(networkInitializationResult));
        return false;
    }

    formatContext_ = avformat_alloc_context();
    if (formatContext_ == nullptr) {
        SetError("Could not allocate an FFmpeg input context");
        return false;
    }
    formatContext_->interrupt_callback.callback =
        &FfmpegDemuxSource::InterruptCallback;
    formatContext_->interrupt_callback.opaque = this;

    DashManifestSelection dashManifest;
    AVIOContext* customManifestIo = nullptr;
    ManifestMemory manifestMemory;
    std::string dashManifestUrl = url_;
    if (IsDashUrl(url_)) {
        std::vector<uint8_t> manifestBytes;
        AVIOContext* manifestInput = nullptr;
        AVDictionary* manifestOptions = nullptr;
        av_dict_set(&manifestOptions,
                    "rw_timeout",
                    std::to_string(kIoTimeoutSeconds * 1000000).c_str(),
                    0);

        ReportProgress("ffmpeg-dash-manifest-fetch-begin");
        int fetchResult = 0;
        BeginTimedIo();
        {
            ProgressWatchdog watchdog(progress_,
                                      "ffmpeg-dash-manifest-fetch");
            fetchResult = avio_open2(
                &manifestInput,
                url_.c_str(),
                AVIO_FLAG_READ,
                &formatContext_->interrupt_callback,
                &manifestOptions);
            if (fetchResult >= 0) {
                uint8_t* resolvedUrl = nullptr;
                if (av_opt_get(manifestInput,
                               "location",
                               AV_OPT_SEARCH_CHILDREN,
                               &resolvedUrl) >= 0 &&
                    resolvedUrl != nullptr &&
                    resolvedUrl[0] != '\0') {
                    dashManifestUrl =
                        reinterpret_cast<const char*>(resolvedUrl);
                }
                av_free(resolvedUrl);

                std::array<uint8_t, 8192> buffer{};
                while (true) {
                    const int bytesRead = avio_read(
                        manifestInput,
                        buffer.data(),
                        static_cast<int>(buffer.size()));
                    if (bytesRead == AVERROR_EOF || bytesRead == 0) {
                        break;
                    }
                    if (bytesRead < 0) {
                        fetchResult = bytesRead;
                        break;
                    }
                    if (manifestBytes.size() + static_cast<size_t>(bytesRead) >
                        kMaximumManifestBytes) {
                        fetchResult = AVERROR(EMSGSIZE);
                        break;
                    }
                    manifestBytes.insert(
                        manifestBytes.end(),
                        buffer.begin(),
                        buffer.begin() + bytesRead);
                }
            }
        }
        if (manifestInput != nullptr) {
            avio_closep(&manifestInput);
        }
        const bool manifestFetchTimedOut = EndTimedIo();
        av_dict_free(&manifestOptions);
        if (fetchResult < 0) {
            if (manifestFetchTimedOut) {
                SetError("DASH manifest fetch exceeded " +
                         std::to_string(kIoTimeoutSeconds) + " seconds");
            } else if (fetchResult == AVERROR(EMSGSIZE)) {
                SetError("DASH manifest exceeds the 16 MiB in-memory limit");
            } else {
                SetError("Could not fetch DASH manifest: " +
                         AvErrorString(fetchResult));
            }
            Stop();
            return false;
        }
        ReportProgress("ffmpeg-dash-manifest-fetch-complete: " +
                       std::to_string(manifestBytes.size()) + " bytes");

        std::string filterError;
        if (!FilterDashManifest(manifestBytes, dashManifest, filterError)) {
            SetError(std::move(filterError));
            Stop();
            return false;
        }
        std::ostringstream selectionLog;
        selectionLog << "ffmpeg-dash-manifest-filtered: video="
                     << (dashManifest.keptVideo ? "yes" : "no")
                     << " audio=" << (dashManifest.keptAudio ? "yes" : "no")
                     << " removed-adaptation-sets="
                     << dashManifest.removedAdaptationSets
                     << " removed-video-representations="
                     << dashManifest.removedVideoRepresentations;
        ReportProgress(selectionLog.str());

        manifestMemory.data = &dashManifest.data;
        uint8_t* ioBuffer = static_cast<uint8_t*>(
            av_malloc(kManifestIoBufferSize));
        if (ioBuffer == nullptr) {
            SetError("Could not allocate the in-memory DASH manifest buffer");
            Stop();
            return false;
        }
        customManifestIo = avio_alloc_context(
            ioBuffer,
            kManifestIoBufferSize,
            0,
            &manifestMemory,
            &ReadManifestMemory,
            nullptr,
            &SeekManifestMemory);
        if (customManifestIo == nullptr) {
            av_free(ioBuffer);
            SetError("Could not create an in-memory DASH manifest input");
            Stop();
            return false;
        }
        customManifestIo->seekable = AVIO_SEEKABLE_NORMAL;
    }

    AVDictionary* options = nullptr;
    av_dict_set(&options, "probesize", "2000000", 0);
    av_dict_set(&options, "analyzeduration", "5000000", 0);
    av_dict_set(&options,
                "rw_timeout",
                std::to_string(kIoTimeoutSeconds * 1000000).c_str(),
                0);
    ReportProgress("ffmpeg-format-open-begin");
    int result = 0;
    BeginTimedIo();
    {
        ProgressWatchdog watchdog(progress_, "ffmpeg-format-open");
        if (customManifestIo != nullptr) {
            const AVInputFormat* dashFormat =
                av_find_input_format("dash");
            if (dashFormat == nullptr) {
                result = AVERROR_DEMUXER_NOT_FOUND;
            } else {
                formatContext_->pb = customManifestIo;
                formatContext_->flags |= AVFMT_FLAG_CUSTOM_IO;
                result = avformat_open_input(
                    &formatContext_,
                    dashManifestUrl.c_str(),
                    dashFormat,
                    &options);
            }
        } else {
            result = avformat_open_input(&formatContext_,
                                         url_.c_str(),
                                         nullptr,
                                         &options);
        }
    }
    if (customManifestIo != nullptr) {
        if (formatContext_ != nullptr &&
            formatContext_->pb == customManifestIo) {
            formatContext_->pb = nullptr;
        }
        avio_context_free(&customManifestIo);
    }
    const bool openTimedOut = EndTimedIo();
    av_dict_free(&options);
    if (result < 0) {
        SetError(openTimedOut
                     ? "FFmpeg format open exceeded " +
                           std::to_string(kIoTimeoutSeconds) + " seconds"
                     : "Could not open stream URL: " + AvErrorString(result));
        Stop();
        return false;
    }
    const std::string inputFormat = formatContext_->iformat != nullptr
        ? formatContext_->iformat->name
        : "";
    ReportProgress("ffmpeg-format-open-complete: " +
                   (inputFormat.empty() ? std::string("unknown")
                                        : inputFormat));

    int videoIndex = av_find_best_stream(formatContext_,
                                         AVMEDIA_TYPE_VIDEO,
                                         -1,
                                         -1,
                                         nullptr,
                                         0);
    int audioIndex = av_find_best_stream(formatContext_,
                                         AVMEDIA_TYPE_AUDIO,
                                         -1,
                                         videoIndex >= 0 ? videoIndex : -1,
                                         nullptr,
                                         0);
    const bool selectedBeforeProbe = videoIndex >= 0 || audioIndex >= 0;
    if (selectedBeforeProbe) {
        for (unsigned int index = 0;
             index < formatContext_->nb_streams;
             ++index) {
            AVStream* stream = formatContext_->streams[index];
            const bool unselectedVideo =
                videoIndex >= 0 &&
                stream->codecpar->codec_type == AVMEDIA_TYPE_VIDEO &&
                static_cast<int>(index) != videoIndex;
            const bool unselectedAudio =
                audioIndex >= 0 &&
                stream->codecpar->codec_type == AVMEDIA_TYPE_AUDIO &&
                static_cast<int>(index) != audioIndex;
            const bool unsupportedStream =
                stream->codecpar->codec_type == AVMEDIA_TYPE_SUBTITLE ||
                stream->codecpar->codec_type == AVMEDIA_TYPE_DATA;
            if (unselectedVideo || unselectedAudio || unsupportedStream) {
                stream->discard = AVDISCARD_ALL;
            }
        }
        std::ostringstream streamSelection;
        streamSelection << "ffmpeg-selected-streams-before-probe: video="
                        << videoIndex << " audio=" << audioIndex
                        << " total=" << formatContext_->nb_streams;
        ReportProgress(streamSelection.str());
    } else {
        ReportProgress("ffmpeg-streams-not-yet-discoverable; probing all");
    }

    ReportProgress("ffmpeg-stream-info-probe-begin");
    BeginTimedIo();
    {
        ProgressWatchdog watchdog(progress_, "ffmpeg-stream-info-probe");
        result = avformat_find_stream_info(formatContext_, nullptr);
    }
    const bool streamInfoTimedOut = EndTimedIo();
    if (result < 0) {
        SetError(streamInfoTimedOut
                     ? "FFmpeg stream-info probe exceeded " +
                           std::to_string(kIoTimeoutSeconds) + " seconds"
                     : "Could not read stream information: " +
                           AvErrorString(result));
        Stop();
        return false;
    }
    ReportProgress("ffmpeg-stream-info-probe-complete");

    if (inputFormat.find("hls") != std::string::npos) {
        mediaMode_ = kPlayerInterfaceHlsMediaMode;
    } else if (inputFormat.find("dash") != std::string::npos) {
        mediaMode_ = kPlayerInterfaceDashMediaMode;
    } else {
        SetError("Only HLS and DASH manifests are supported; FFmpeg opened " +
                 (inputFormat.empty() ? std::string("an unknown format")
                                      : inputFormat));
        Stop();
        return false;
    }

    if (videoIndex < 0) {
        videoIndex = av_find_best_stream(formatContext_,
                                         AVMEDIA_TYPE_VIDEO,
                                         -1,
                                         -1,
                                         nullptr,
                                         0);
    }
    if (audioIndex < 0) {
        audioIndex = av_find_best_stream(formatContext_,
                                         AVMEDIA_TYPE_AUDIO,
                                         -1,
                                         videoIndex >= 0 ? videoIndex : -1,
                                         nullptr,
                                         0);
    }
    if (videoIndex < 0 && audioIndex < 0) {
        SetError("The manifest has no supported audio or video streams");
        Stop();
        return false;
    }

    ReportProgress("ffmpeg-track-codec-info-begin");
    tracks_[0].streamIndex = videoIndex >= 0 ? videoIndex : -1;
    tracks_[1].streamIndex = audioIndex >= 0 ? audioIndex : -1;
    if (videoIndex >= 0 && !InitializeTrack(tracks_[0])) {
        Stop();
        return false;
    }
    if (audioIndex >= 0 && !InitializeTrack(tracks_[1])) {
        Stop();
        return false;
    }
    ReportProgress("ffmpeg-track-codec-info-complete");

    if (formatContext_->duration != AV_NOPTS_VALUE &&
        formatContext_->duration > 0) {
        durationMilliseconds_ = formatContext_->duration / 1000;
    }

    ReportProgress("ffmpeg-initial-packet-probe-begin");
    bool primed = false;
    {
        ProgressWatchdog watchdog(progress_, "ffmpeg-initial-packet-probe");
        primed = PrimeTracks();
    }
    if (!primed) {
        if (error_.empty()) {
            SetError("Could not obtain codec configuration and initial samples");
        }
        Stop();
        return false;
    }
    ReportProgress("ffmpeg-initial-packet-probe-complete");

    return true;
}

StreamCodecInfo FfmpegDemuxSource::GetCodecInfo() const
{
    StreamCodecInfo info;
    for (const Track& track : tracks_) {
        if (track.streamIndex < 0) {
            continue;
        }

        MediaCodecInfo* codecInfo =
            track.mediaType == eGST_MEDIATYPE_VIDEO
                ? &info.video
                : &info.audio;
        codecInfo->mCodecFormat = track.format;
        codecInfo->mCodecData = track.codecData;
        codecInfo->mIsEncrypted = false;
        codecInfo->mNaluLengthPrefixed =
            track.mediaType == eGST_MEDIATYPE_VIDEO;
        if (track.mediaType == eGST_MEDIATYPE_VIDEO) {
            codecInfo->mInfo.video.mWidth =
                static_cast<uint16_t>(track.width);
            codecInfo->mInfo.video.mHeight =
                static_cast<uint16_t>(track.height);
        } else {
            codecInfo->mInfo.audio.mChannelCount =
                static_cast<uint16_t>(track.channels);
            codecInfo->mInfo.audio.mSampleRate =
                static_cast<uint16_t>(track.sampleRate);
        }
    }
    return info;
}

int FfmpegDemuxSource::GetMediaMode() const
{
    return mediaMode_;
}

long long FfmpegDemuxSource::GetDurationMilliseconds() const
{
    return durationMilliseconds_;
}

bool FfmpegDemuxSource::HasTrack(int mediaType) const
{
    return FindTrack(mediaType) != nullptr;
}

bool FfmpegDemuxSource::Start(SendSampleCallback sendSample,
                              EndOfStreamCallback endOfStream)
{
    if (formatContext_ == nullptr || !sendSample || !endOfStream ||
        stopping_) {
        SetError("The FFmpeg source is not ready to start");
        return false;
    }

    sendSample_ = std::move(sendSample);
    endOfStream_ = std::move(endOfStream);
    try {
        for (Track& track : tracks_) {
            if (track.streamIndex >= 0) {
                track.feederThread =
                    std::thread(&FfmpegDemuxSource::FeedLoop, this,
                                std::ref(track));
            }
        }
        demuxThread_ = std::thread(&FfmpegDemuxSource::DemuxLoop, this);
    } catch (const std::system_error& error) {
        SetError("Could not start FFmpeg worker threads: " +
                 std::string(error.what()));
        Stop();
        return false;
    }
    return true;
}

void FfmpegDemuxSource::NeedData(int mediaType)
{
    Track* track = FindTrack(mediaType);
    if (track == nullptr) {
        SetError("Player Interface requested data for an unconfigured track");
        return;
    }

    {
        const std::lock_guard<std::mutex> lock(track->mutex);
        track->needData = true;
    }
    track->condition.notify_all();
}

void FfmpegDemuxSource::EnoughData(int mediaType)
{
    Track* track = FindTrack(mediaType);
    if (track == nullptr) {
        SetError("Player Interface stopped data for an unconfigured track");
        return;
    }

    {
        const std::lock_guard<std::mutex> lock(track->mutex);
        track->needData = false;
    }
    track->condition.notify_all();
}

void FfmpegDemuxSource::SetFeedingEnabled(bool enabled)
{
    feedingEnabled_ = enabled;
    for (Track& track : tracks_) {
        track.condition.notify_all();
    }
    if (enabled) {
        return;
    }

    for (Track& track : tracks_) {
        if (track.streamIndex < 0) {
            continue;
        }
        std::unique_lock<std::mutex> lock(track.mutex);
        track.condition.wait(lock, [&track, this] {
            return !track.sending || stopping_;
        });
    }
}

bool FfmpegDemuxSource::Seek(double positionSeconds)
{
    if (formatContext_ == nullptr || !std::isfinite(positionSeconds) ||
        positionSeconds < 0.0 || positionSeconds >
            static_cast<double>(std::numeric_limits<int64_t>::max()) /
                AV_TIME_BASE) {
        return false;
    }

    inputPaused_ = true;
    inputPauseCondition_.notify_all();
    for (Track& track : tracks_) {
        std::lock_guard<std::mutex> lock(track.mutex);
        track.packets.clear();
        track.queuedBytes = 0;
        track.inputEos = false;
        track.eosNotified = false;
        track.discontinuityNext = true;
        track.hasTimestamp = false;
        track.condition.notify_all();
    }

    const int64_t target =
        static_cast<int64_t>(positionSeconds * AV_TIME_BASE);
    int result = 0;
    {
        const std::lock_guard<std::mutex> lock(demuxMutex_);
        result = avformat_seek_file(formatContext_,
                                    -1,
                                    std::numeric_limits<int64_t>::min(),
                                    target,
                                    std::numeric_limits<int64_t>::max(),
                                    AVSEEK_FLAG_BACKWARD);
        if (result >= 0) {
            avformat_flush(formatContext_);
        }
    }

    if (result < 0) {
        inputPaused_ = false;
        ++seekGeneration_;
        inputPauseCondition_.notify_all();
        return false;
    }

    for (Track& track : tracks_) {
        std::lock_guard<std::mutex> lock(track.mutex);
        track.packets.clear();
        track.queuedBytes = 0;
        track.inputEos = false;
        track.eosNotified = false;
        track.discontinuityNext = true;
        track.hasTimestamp = false;
        track.condition.notify_all();
    }
    inputPaused_ = false;
    ++seekGeneration_;
    inputPauseCondition_.notify_all();
    return true;
}

bool FfmpegDemuxSource::TakeError(std::string& error)
{
    const std::lock_guard<std::mutex> lock(errorMutex_);
    if (error_.empty()) {
        return false;
    }
    error = std::move(error_);
    error_.clear();
    return true;
}

void FfmpegDemuxSource::Stop()
{
    stopping_ = true;
    inputPaused_ = true;
    inputPauseCondition_.notify_all();
    for (Track& track : tracks_) {
        track.condition.notify_all();
    }

    if (demuxThread_.joinable()) {
        demuxThread_.join();
    }
    for (Track& track : tracks_) {
        if (track.feederThread.joinable()) {
            track.feederThread.join();
        }
        std::lock_guard<std::mutex> lock(track.mutex);
        track.packets.clear();
        track.queuedBytes = 0;
    }

    if (formatContext_ != nullptr) {
        const std::lock_guard<std::mutex> lock(demuxMutex_);
        avformat_close_input(&formatContext_);
    }
}

int FfmpegDemuxSource::InterruptCallback(void* opaque)
{
    const auto* source = static_cast<const FfmpegDemuxSource*>(opaque);
    if (source->stopping_) {
        return 1;
    }

    const int64_t deadline = source->ioDeadlineNanoseconds_;
    if (deadline > 0) {
        const int64_t now = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::steady_clock::now().time_since_epoch()).count();
        if (now >= deadline) {
            source->ioTimedOut_ = true;
            return 1;
        }
    }
    return 0;
}

bool FfmpegDemuxSource::IsAdts(const uint8_t* data, size_t size)
{
    return data != nullptr && size >= 7 && data[0] == 0xff &&
           (data[1] & 0xf6) == 0xf0;
}

bool FfmpegDemuxSource::FindStartCode(const uint8_t* data,
                                      size_t size,
                                      size_t from,
                                      size_t& position,
                                      size_t& length)
{
    if (data == nullptr || size < 3 || from >= size) {
        return false;
    }
    for (size_t index = from; index + 2 < size; ++index) {
        if (data[index] == 0 && data[index + 1] == 0) {
            if (index + 3 < size && data[index + 2] == 0 &&
                data[index + 3] == 1) {
                position = index;
                length = 4;
                return true;
            }
            if (data[index + 2] == 1) {
                position = index;
                length = 3;
                return true;
            }
        }
    }
    return false;
}

bool FfmpegDemuxSource::BuildAvcConfiguration(
    const std::vector<std::vector<uint8_t>>& sequenceParameterSets,
    const std::vector<std::vector<uint8_t>>& pictureParameterSets,
    std::vector<uint8_t>& configuration)
{
    if (sequenceParameterSets.empty() || pictureParameterSets.empty() ||
        sequenceParameterSets.size() > 31 ||
        pictureParameterSets.size() > 255) {
        return false;
    }
    const std::vector<uint8_t>& firstSps = sequenceParameterSets.front();
    if (firstSps.size() < 4) {
        return false;
    }

    std::vector<uint8_t> result{
        1,
        firstSps[1],
        firstSps[2],
        firstSps[3],
        0xff,
        static_cast<uint8_t>(0xe0 | sequenceParameterSets.size())
    };
    auto appendParameterSet = [&result](const std::vector<uint8_t>& set) {
        if (set.empty() || set.size() > std::numeric_limits<uint16_t>::max()) {
            return false;
        }
        result.push_back(static_cast<uint8_t>((set.size() >> 8) & 0xff));
        result.push_back(static_cast<uint8_t>(set.size() & 0xff));
        result.insert(result.end(), set.begin(), set.end());
        return true;
    };

    for (const auto& sps : sequenceParameterSets) {
        if (!appendParameterSet(sps)) {
            return false;
        }
    }
    result.push_back(static_cast<uint8_t>(pictureParameterSets.size()));
    for (const auto& pps : pictureParameterSets) {
        if (!appendParameterSet(pps)) {
            return false;
        }
    }
    configuration = std::move(result);
    return true;
}

FfmpegDemuxSource::Track* FfmpegDemuxSource::FindTrack(int mediaType)
{
    for (Track& track : tracks_) {
        if (track.streamIndex >= 0 && track.mediaType == mediaType) {
            return &track;
        }
    }
    return nullptr;
}

const FfmpegDemuxSource::Track*
FfmpegDemuxSource::FindTrack(int mediaType) const
{
    for (const Track& track : tracks_) {
        if (track.streamIndex >= 0 && track.mediaType == mediaType) {
            return &track;
        }
    }
    return nullptr;
}

bool FfmpegDemuxSource::InitializeTrack(Track& track)
{
    AVStream* stream = formatContext_->streams[track.streamIndex];
    AVCodecParameters* parameters = stream->codecpar;
    track.timeBaseSeconds = av_q2d(stream->time_base);
    if (track.timeBaseSeconds <= 0.0) {
        SetError("FFmpeg returned an invalid track time base");
        return false;
    }

    if (track.mediaType == eGST_MEDIATYPE_VIDEO) {
        if (parameters->codec_id != AV_CODEC_ID_H264) {
            SetError("Only H.264 video elementary streams are supported");
            return false;
        }
        track.format = GST_FORMAT_VIDEO_ES_H264;
        track.width = parameters->width;
        track.height = parameters->height;
        AVRational frameRate = stream->avg_frame_rate;
        if (frameRate.num <= 0 || frameRate.den <= 0) {
            frameRate = stream->r_frame_rate;
        }
        if (frameRate.num > 0 && frameRate.den > 0) {
            track.fallbackDuration =
                static_cast<double>(frameRate.den) / frameRate.num;
        }

        if (parameters->extradata_size > 0 &&
            parameters->extradata != nullptr) {
            const uint8_t* data = parameters->extradata;
            const size_t size =
                static_cast<size_t>(parameters->extradata_size);
            if (data[0] == 1 && size >= 7) {
                track.codecData.assign(data, data + size);
                track.nalLengthSize = (data[4] & 0x03) + 1;
                track.codecData[4] =
                    static_cast<uint8_t>((track.codecData[4] & 0xfc) | 0x03);
                track.inputAnnexB = false;
            } else {
                track.inputAnnexB = true;
                CollectH264Configuration(track, data, size, true);
            }
        } else {
            track.inputAnnexB = true;
        }
    } else {
        if (parameters->codec_id != AV_CODEC_ID_AAC) {
            SetError("Only AAC audio elementary streams are supported");
            return false;
        }
        track.sampleRate = parameters->sample_rate;
#if LIBAVUTIL_VERSION_MAJOR >= 57
        track.channels = parameters->ch_layout.nb_channels;
#else
        track.channels = parameters->channels;
#endif
        if (track.sampleRate > 0) {
            track.fallbackDuration = 1024.0 / track.sampleRate;
        }
        if (parameters->extradata_size > 0 &&
            parameters->extradata != nullptr &&
            !IsAdts(parameters->extradata,
                    static_cast<size_t>(parameters->extradata_size))) {
            track.format = GST_FORMAT_AUDIO_ES_AAC_RAW;
            track.codecData.assign(
                parameters->extradata,
                parameters->extradata + parameters->extradata_size);
        }
    }
    return true;
}

bool FfmpegDemuxSource::PrimeTracks()
{
    for (size_t count = 0; count < kMaximumPrimingPackets; ++count) {
        if (InputReady()) {
            return true;
        }
        if (!ReadAndQueuePacket(true)) {
            break;
        }
    }
    return InputReady();
}

bool FfmpegDemuxSource::ReadAndQueuePacket(bool priming)
{
    std::lock_guard<std::mutex> demuxLock(demuxMutex_);
    if (stopping_) {
        return false;
    }
    if (inputPaused_) {
        return true;
    }

    AVPacket* packet = av_packet_alloc();
    if (packet == nullptr) {
        SetError("Could not allocate an FFmpeg packet");
        return false;
    }

    BeginTimedIo();
    const int result = av_read_frame(formatContext_, packet);
    const bool readTimedOut = EndTimedIo();
    if (result < 0) {
        av_packet_free(&packet);
        if (readTimedOut) {
            SetError("FFmpeg media packet read exceeded " +
                     std::to_string(kIoTimeoutSeconds) + " seconds");
            return false;
        }
        if (result == AVERROR(EAGAIN)) {
            return true;
        }
        if (result == AVERROR_EOF) {
            MarkInputEos();
            return false;
        }
        if (!stopping_) {
            SetError("FFmpeg could not read the next media packet: " +
                     AvErrorString(result));
            MarkInputEos();
        }
        return false;
    }

    Track* track = nullptr;
    for (Track& candidate : tracks_) {
        if (candidate.streamIndex == packet->stream_index) {
            track = &candidate;
            break;
        }
    }

    bool queued = true;
    if (track != nullptr) {
        QueuedPacket prepared;
        if (!PreparePacket(*track, packet, prepared)) {
            queued = false;
            if (!stopping_) {
                SetError("FFmpeg produced an unsupported or malformed media "
                         "packet");
                MarkInputEos();
            }
        } else if (!QueuePacket(*track, std::move(prepared))) {
            queued = !stopping_ && inputPaused_;
        }
    }
    av_packet_free(&packet);

    if (!queued && priming && !stopping_) {
        return false;
    }
    return queued || (!stopping_ && inputPaused_);
}

bool FfmpegDemuxSource::PreparePacket(Track& track,
                                      const AVPacket* packet,
                                      QueuedPacket& output)
{
    if (packet == nullptr || packet->data == nullptr || packet->size <= 0) {
        return false;
    }

    if (track.mediaType == eGST_MEDIATYPE_VIDEO) {
        if (!NormalizeH264(track,
                           packet->data,
                           static_cast<size_t>(packet->size),
                           output.data)) {
            return false;
        }
    } else {
        const bool adts = IsAdts(packet->data,
                                 static_cast<size_t>(packet->size));
        if (!track.ready) {
            if (adts) {
                track.format = GST_FORMAT_AUDIO_ES_AAC;
                track.codecData.clear();
                if (track.channels == 0) {
                    track.channels =
                        ((packet->data[2] & 0x01) << 2) |
                        ((packet->data[3] >> 6) & 0x03);
                }
                if (track.sampleRate == 0) {
                    static constexpr int sampleRates[] = {
                        96000, 88200, 64000, 48000, 44100, 32000,
                        24000, 22050, 16000, 12000, 11025, 8000, 7350
                    };
                    const int index = (packet->data[2] >> 2) & 0x0f;
                    if (index < static_cast<int>(
                                    sizeof(sampleRates) / sizeof(sampleRates[0]))) {
                        track.sampleRate = sampleRates[index];
                        track.fallbackDuration = 1024.0 / track.sampleRate;
                    }
                }
            } else if (track.format != GST_FORMAT_AUDIO_ES_AAC_RAW ||
                       track.codecData.empty()) {
                return false;
            }
        } else if ((track.format == GST_FORMAT_AUDIO_ES_AAC) != adts) {
            return false;
        }
        output.data.assign(packet->data, packet->data + packet->size);
    }

    AVStream* stream = formatContext_->streams[track.streamIndex];
    const double timeBase = av_q2d(stream->time_base);
    double duration = packet->duration > 0
        ? packet->duration * timeBase
        : track.fallbackDuration;
    const int64_t pts = packet->pts;
    const int64_t dts = packet->dts;
    double ptsSeconds = pts != AV_NOPTS_VALUE
        ? pts * timeBase
        : (dts != AV_NOPTS_VALUE
               ? dts * timeBase
               : (track.hasTimestamp
                      ? track.lastTimestamp + duration
                      : 0.0));
    double dtsSeconds = dts != AV_NOPTS_VALUE ? dts * timeBase : ptsSeconds;
    if (formatContext_->start_time != AV_NOPTS_VALUE) {
        const double startTime =
            static_cast<double>(formatContext_->start_time) / AV_TIME_BASE;
        ptsSeconds -= startTime;
        dtsSeconds -= startTime;
    }
    ptsSeconds = std::max(0.0, ptsSeconds);
    dtsSeconds = std::max(0.0, dtsSeconds);
    track.lastTimestamp = ptsSeconds;
    track.hasTimestamp = true;

    output.pts = ptsSeconds;
    output.dts = dtsSeconds;
    output.duration = std::max(0.0, duration);
    track.ready = track.mediaType != eGST_MEDIATYPE_VIDEO ||
                  !track.codecData.empty();
    if (track.mediaType == eGST_MEDIATYPE_AUDIO) {
        track.ready = track.format == GST_FORMAT_AUDIO_ES_AAC ||
                      track.format == GST_FORMAT_AUDIO_ES_AAC_RAW;
    }
    return !output.data.empty();
}

bool FfmpegDemuxSource::NormalizeH264(Track& track,
                                      const uint8_t* data,
                                      size_t size,
                                      std::vector<uint8_t>& output)
{
    if (data == nullptr || size == 0) {
        return false;
    }

    size_t cursor = 0;
    size_t naluCount = 0;
    auto processNalu = [&](const uint8_t* nalu, size_t naluSize) {
        if (naluSize == 0 ||
            naluSize > std::numeric_limits<uint32_t>::max()) {
            return false;
        }
        const uint8_t type = nalu[0] & 0x1f;
        if (type == 7 && track.codecData.empty()) {
            track.sequenceParameterSets.emplace_back(nalu, nalu + naluSize);
        } else if (type == 8 && track.codecData.empty()) {
            track.pictureParameterSets.emplace_back(nalu, nalu + naluSize);
        }
        const uint32_t length = static_cast<uint32_t>(naluSize);
        output.push_back(static_cast<uint8_t>((length >> 24) & 0xff));
        output.push_back(static_cast<uint8_t>((length >> 16) & 0xff));
        output.push_back(static_cast<uint8_t>((length >> 8) & 0xff));
        output.push_back(static_cast<uint8_t>(length & 0xff));
        output.insert(output.end(), nalu, nalu + naluSize);
        ++naluCount;
        return true;
    };

    if (track.inputAnnexB) {
        size_t startPosition = 0;
        size_t startLength = 0;
        if (!FindStartCode(data, size, 0, startPosition, startLength)) {
            return false;
        }
        cursor = startPosition + startLength;
        while (cursor < size) {
            size_t nextPosition = 0;
            size_t nextLength = 0;
            const bool hasNext =
                FindStartCode(data, size, cursor, nextPosition, nextLength);
            size_t naluEnd = hasNext ? nextPosition : size;
            while (naluEnd > cursor && data[naluEnd - 1] == 0) {
                --naluEnd;
            }
            if (!processNalu(data + cursor, naluEnd - cursor)) {
                return false;
            }
            if (!hasNext) {
                break;
            }
            cursor = nextPosition + nextLength;
        }
    } else {
        while (cursor < size) {
            if (size - cursor < static_cast<size_t>(track.nalLengthSize)) {
                return false;
            }
            uint32_t naluSize = 0;
            for (int byte = 0; byte < track.nalLengthSize; ++byte) {
                naluSize = (naluSize << 8) | data[cursor++];
            }
            if (naluSize == 0 || naluSize > size - cursor ||
                !processNalu(data + cursor, naluSize)) {
                return false;
            }
            cursor += naluSize;
        }
    }

    if (naluCount == 0) {
        return false;
    }
    if (track.codecData.empty()) {
        BuildAvcConfiguration(track.sequenceParameterSets,
                              track.pictureParameterSets,
                              track.codecData);
    }
    return true;
}

bool FfmpegDemuxSource::CollectH264Configuration(
    Track& track,
    const uint8_t* data,
    size_t size,
    bool annexB)
{
    if (data == nullptr || size == 0) {
        return false;
    }

    auto collect = [&track](const uint8_t* nalu, size_t naluSize) {
        if (naluSize == 0) {
            return false;
        }
        const uint8_t type = nalu[0] & 0x1f;
        if (type == 7) {
            track.sequenceParameterSets.emplace_back(nalu, nalu + naluSize);
        } else if (type == 8) {
            track.pictureParameterSets.emplace_back(nalu, nalu + naluSize);
        }
        return true;
    };

    size_t cursor = 0;
    if (annexB) {
        size_t startPosition = 0;
        size_t startLength = 0;
        if (!FindStartCode(data, size, 0, startPosition, startLength)) {
            return false;
        }
        cursor = startPosition + startLength;
        while (cursor < size) {
            size_t nextPosition = 0;
            size_t nextLength = 0;
            const bool hasNext =
                FindStartCode(data, size, cursor, nextPosition, nextLength);
            size_t naluEnd = hasNext ? nextPosition : size;
            while (naluEnd > cursor && data[naluEnd - 1] == 0) {
                --naluEnd;
            }
            if (!collect(data + cursor, naluEnd - cursor)) {
                return false;
            }
            if (!hasNext) {
                break;
            }
            cursor = nextPosition + nextLength;
        }
    } else {
        while (cursor < size) {
            if (size - cursor < static_cast<size_t>(track.nalLengthSize)) {
                return false;
            }
            uint32_t naluSize = 0;
            for (int byte = 0; byte < track.nalLengthSize; ++byte) {
                naluSize = (naluSize << 8) | data[cursor++];
            }
            if (naluSize == 0 || naluSize > size - cursor ||
                !collect(data + cursor, naluSize)) {
                return false;
            }
            cursor += naluSize;
        }
    }

    return BuildAvcConfiguration(track.sequenceParameterSets,
                                 track.pictureParameterSets,
                                 track.codecData);
}

bool FfmpegDemuxSource::QueuePacket(Track& track, QueuedPacket&& packet)
{
    std::unique_lock<std::mutex> lock(track.mutex);
    track.condition.wait(lock, [&] {
        return stopping_ || inputPaused_ ||
               (track.packets.size() < kMaximumQueuedPackets &&
                (track.queuedBytes == 0 ||
                 packet.data.size() <=
                     kMaximumQueuedBytes -
                         std::min(track.queuedBytes, kMaximumQueuedBytes)));
    });
    if (stopping_ || inputPaused_) {
        return false;
    }

    packet.discontinuity = track.discontinuityNext;
    track.discontinuityNext = false;
    track.queuedBytes += packet.data.size();
    track.packets.push_back(std::move(packet));
    lock.unlock();
    track.condition.notify_all();
    return true;
}

void FfmpegDemuxSource::DemuxLoop()
{
    while (!stopping_) {
        {
            std::unique_lock<std::mutex> lock(inputPauseMutex_);
            inputPauseCondition_.wait(lock, [this] {
                return stopping_ || !inputPaused_;
            });
        }
        if (stopping_) {
            break;
        }
        const uint64_t generation = seekGeneration_;
        if (!ReadAndQueuePacket(false)) {
            if (stopping_) {
                break;
            }
            std::unique_lock<std::mutex> lock(inputPauseMutex_);
            inputPauseCondition_.wait(lock, [this, generation] {
                return stopping_ || seekGeneration_ != generation;
            });
        }
    }
}

void FfmpegDemuxSource::FeedLoop(Track& track)
{
    while (!stopping_) {
        QueuedPacket packet;
        bool signalEos = false;
        {
            std::unique_lock<std::mutex> lock(track.mutex);
            track.condition.wait(lock, [&] {
                return stopping_ ||
                       (feedingEnabled_ && track.needData &&
                        !track.packets.empty()) ||
                       (track.inputEos && track.packets.empty() &&
                        !track.eosNotified);
            });
            if (stopping_) {
                return;
            }
            if (track.inputEos && track.packets.empty() &&
                !track.eosNotified) {
                track.eosNotified = true;
                signalEos = true;
            } else if (feedingEnabled_ && track.needData &&
                       !track.packets.empty()) {
                packet = std::move(track.packets.front());
                track.packets.pop_front();
                track.queuedBytes -= packet.data.size();
                track.sending = true;
            }
        }
        track.condition.notify_all();

        if (signalEos) {
            endOfStream_(track.mediaType);
            continue;
        }
        if (packet.data.empty()) {
            continue;
        }

        bool accepted = false;
        try {
            MediaSample sample(std::move(packet.data),
                               packet.pts,
                               packet.dts,
                               packet.duration);
            accepted = sendSample_(track.mediaType,
                                   std::move(sample),
                                   packet.discontinuity);
        } catch (const std::exception& error) {
            SetError("Could not submit a media sample: " +
                     std::string(error.what()));
        }

        {
            const std::lock_guard<std::mutex> lock(track.mutex);
            track.sending = false;
        }
        track.condition.notify_all();

        if (!accepted) {
            if (!stopping_) {
                SetError("Player Interface rejected a media sample");
            }
            return;
        }
    }
}

void FfmpegDemuxSource::MarkInputEos()
{
    for (Track& track : tracks_) {
        if (track.streamIndex < 0) {
            continue;
        }
        {
            const std::lock_guard<std::mutex> lock(track.mutex);
            track.inputEos = true;
        }
        track.condition.notify_all();
    }
}

void FfmpegDemuxSource::ReportProgress(
    const std::string& milestone) const
{
    if (progress_) {
        progress_(milestone);
    }
}

void FfmpegDemuxSource::BeginTimedIo()
{
    const int64_t now = std::chrono::duration_cast<std::chrono::nanoseconds>(
        std::chrono::steady_clock::now().time_since_epoch()).count();
    const int64_t timeout =
        std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::seconds(kIoTimeoutSeconds)).count();
    ioTimedOut_ = false;
    ioDeadlineNanoseconds_ = now + timeout;
}

bool FfmpegDemuxSource::EndTimedIo()
{
    ioDeadlineNanoseconds_ = 0;
    return ioTimedOut_.exchange(false);
}

void FfmpegDemuxSource::SetError(std::string error)
{
    {
        const std::lock_guard<std::mutex> lock(errorMutex_);
        if (error_.empty()) {
            error_ = std::move(error);
        }
    }
    stopping_ = true;
    inputPaused_ = false;
    inputPauseCondition_.notify_all();
    for (Track& track : tracks_) {
        track.condition.notify_all();
    }
}

bool FfmpegDemuxSource::InputReady() const
{
    for (const Track& track : tracks_) {
        if (track.streamIndex < 0) {
            continue;
        }
        if (!track.ready ||
            (track.mediaType == eGST_MEDIATYPE_VIDEO &&
             track.codecData.empty())) {
            return false;
        }
    }
    return true;
}
