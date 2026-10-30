#include "DashResourceProxy.h"

extern "C" {
#include <libavformat/avio.h>
#include <libavutil/error.h>
#include <libavutil/opt.h>
}

#include <gio/gio.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

#include <algorithm>
#include <array>
#include <cerrno>
#include <chrono>
#include <cmath>
#include <condition_variable>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <iomanip>
#include <limits>
#include <mutex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <system_error>
#include <thread>
#include <unordered_map>
#include <utility>
#include <vector>

namespace {

constexpr size_t kPrefetchSegmentCount = 4;
constexpr size_t kPrefetchWorkerCount = 6;
constexpr size_t kAudioOnlyPrefetchWorkerCount = 4;
constexpr size_t kMaximumResourceBytes = 8 * 1024 * 1024;
constexpr size_t kMaximumCacheBytes = 32 * 1024 * 1024;
constexpr int kResourceIoTimeoutSeconds = 8;
constexpr size_t kMaximumHttpHeaderBytes = 16 * 1024;
constexpr int kHttpReadTimeoutSeconds = 10;

struct SegmentTemplateInfo {
    std::string remoteUrlTemplate;
    std::string variableToken;
    std::string localSuffix;
    bool usesTime = false;
    int numberWidth = 0;
    int64_t startNumber = 1;
    int64_t timescale = 1;
    int64_t duration = 0;
    int64_t presentationTimeOffset = 0;
    double periodStartSeconds = 0.0;
    bool hasSeekTiming = true;
    std::vector<int64_t> segmentValues;
    std::vector<int64_t> timelineValues;
};

struct PlannedTrack {
    xmlNodePtr segmentTemplate = nullptr;
    std::string routeId;
    std::string initUrl;
    SegmentTemplateInfo media;
};

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

std::string XmlNodeContent(xmlNodePtr node)
{
    xmlChar* value = xmlNodeGetContent(node);
    if (value == nullptr) {
        return {};
    }

    std::string result(reinterpret_cast<const char*>(value));
    xmlFree(value);
    return result;
}

std::string ReplaceToken(std::string value,
                         const std::string& token,
                         const std::string& replacement)
{
    if (token.empty()) {
        return value;
    }

    size_t position = 0;
    while ((position = value.find(token, position)) != std::string::npos) {
        value.replace(position, token.size(), replacement);
        position += replacement.size();
    }
    return value;
}

std::string ResolveHttpUrl(const std::string& baseUrl,
                           const std::string& reference)
{
    if (reference.empty()) {
        return baseUrl;
    }
    if (reference.find("://") != std::string::npos) {
        return reference;
    }

    const size_t schemeEnd = baseUrl.find("://");
    if (schemeEnd == std::string::npos) {
        return {};
    }
    const size_t authorityStart = schemeEnd + 3;
    const size_t authorityEnd = baseUrl.find_first_of("/?#", authorityStart);
    const std::string origin = baseUrl.substr(
        0,
        authorityEnd == std::string::npos ? baseUrl.size() : authorityEnd);

    if (reference.rfind("//", 0) == 0) {
        return baseUrl.substr(0, schemeEnd) + ":" + reference;
    }
    if (reference.front() == '/') {
        return origin + reference;
    }

    std::string basePath = authorityEnd == std::string::npos
        ? std::string("/")
        : baseUrl.substr(authorityEnd);
    const size_t queryOrFragment = basePath.find_first_of("?#");
    const std::string baseQuery = queryOrFragment == std::string::npos
        ? std::string()
        : basePath.substr(queryOrFragment);
    if (queryOrFragment != std::string::npos) {
        basePath.resize(queryOrFragment);
    }
    const size_t lastSlash = basePath.rfind('/');
    basePath.resize(lastSlash == std::string::npos ? 0 : lastSlash + 1);

    std::string resolved = origin + basePath + reference;
    if (reference.find('?') == std::string::npos &&
        reference.find('#') == std::string::npos) {
        const size_t query = baseQuery.find('?');
        if (query != std::string::npos) {
            resolved += baseQuery.substr(query);
        }
    }
    return resolved;
}

bool ParseInteger(const std::string& text, int64_t& value)
{
    if (text.empty()) {
        return false;
    }
    try {
        size_t parsed = 0;
        value = std::stoll(text, &parsed, 10);
        return parsed == text.size();
    } catch (const std::invalid_argument&) {
        return false;
    } catch (const std::out_of_range&) {
        return false;
    }
}

bool GetMediaPlaceholder(const std::string& media,
                         std::string& token,
                         bool& usesTime,
                         int& numberWidth)
{
    const size_t numberPosition = media.find("$Number");
    const size_t timePosition = media.find("$Time$");
    if (numberPosition != std::string::npos &&
        timePosition != std::string::npos) {
        return false;
    }

    if (numberPosition != std::string::npos) {
        const size_t end = media.find('$', numberPosition + 1);
        if (end == std::string::npos) {
            return false;
        }
        token = media.substr(numberPosition, end - numberPosition + 1);
        if (token == "$Number$") {
            numberWidth = 0;
        } else if (token.size() >= 12 &&
                   token.rfind("$Number%0", 0) == 0 &&
                   token.back() == '$' &&
                   token[token.size() - 2] == 'd') {
            int64_t width = 0;
            if (!ParseInteger(
                    token.substr(9, token.size() - 11), width) ||
                width <= 0 || width > 18) {
                return false;
            }
            numberWidth = static_cast<int>(width);
        } else {
            return false;
        }
        usesTime = false;
        return true;
    }

    if (timePosition != std::string::npos) {
        token = "$Time$";
        usesTime = true;
        numberWidth = 0;
        return true;
    }
    return false;
}

std::string FormatSegmentValue(int64_t value, int width)
{
    std::ostringstream result;
    if (width > 0) {
        result << std::setw(width) << std::setfill('0');
    }
    result << value;
    return result.str();
}

bool ParsePeriodStart(xmlNodePtr period, double& startSeconds)
{
    const std::string value = XmlProperty(period, "start");
    if (value.empty()) {
        startSeconds = 0.0;
        return true;
    }
    if (value.rfind("PT", 0) != 0) {
        return false;
    }

    startSeconds = 0.0;
    const char* cursor = value.c_str() + 2;
    while (*cursor != '\0') {
        char* end = nullptr;
        const double amount = std::strtod(cursor, &end);
        if (end == cursor || !std::isfinite(amount) || *end == '\0') {
            return false;
        }
        double multiplier = 0.0;
        switch (*end) {
        case 'H':
            multiplier = 3600.0;
            break;
        case 'M':
            multiplier = 60.0;
            break;
        case 'S':
            multiplier = 1.0;
            break;
        default:
            return false;
        }
        startSeconds += amount * multiplier;
        cursor = end + 1;
    }
    return std::isfinite(startSeconds);
}

std::string InheritedTemplateProperty(xmlNodePtr node, const char* name)
{
    for (; node != nullptr; node = node->parent) {
        for (xmlNodePtr child = node->children;
             child != nullptr;
             child = child->next) {
            if (child->type == XML_ELEMENT_NODE &&
                xmlStrEqual(child->name, BAD_CAST "SegmentTemplate")) {
                const std::string value = XmlProperty(child, name);
                if (!value.empty()) {
                    return value;
                }
            }
        }
    }
    return {};
}

xmlNodePtr FindInheritedSegmentTimeline(xmlNodePtr node)
{
    for (; node != nullptr; node = node->parent) {
        for (xmlNodePtr child = node->children;
             child != nullptr;
             child = child->next) {
            if (child->type != XML_ELEMENT_NODE ||
                !xmlStrEqual(child->name, BAD_CAST "SegmentTemplate")) {
                continue;
            }
            for (xmlNodePtr templateChild = child->children;
                 templateChild != nullptr;
                 templateChild = templateChild->next) {
                if (templateChild->type == XML_ELEMENT_NODE &&
                    xmlStrEqual(templateChild->name,
                                BAD_CAST "SegmentTimeline")) {
                    return templateChild;
                }
            }
        }
    }
    return nullptr;
}

bool BuildSegmentTimelineValues(xmlNodePtr representation,
                                std::vector<int64_t>& values)
{
    xmlNodePtr timeline = FindInheritedSegmentTimeline(representation);
    if (timeline == nullptr) {
        return false;
    }

    std::vector<int64_t> parsedValues;
    int64_t currentTime = 0;
    for (xmlNodePtr child = timeline->children;
         child != nullptr;
         child = child->next) {
        if (child->type != XML_ELEMENT_NODE ||
            !xmlStrEqual(child->name, BAD_CAST "S")) {
            continue;
        }
        const std::string start = XmlProperty(child, "t");
        const std::string duration = XmlProperty(child, "d");
        const std::string repeat = XmlProperty(child, "r");
        int64_t durationValue = 0;
        int64_t repeatValue = 0;
        if (!start.empty() && !ParseInteger(start, currentTime)) {
            return false;
        }
        if (!ParseInteger(duration, durationValue) || durationValue <= 0) {
            return false;
        }
        if (!repeat.empty() &&
            (!ParseInteger(repeat, repeatValue) || repeatValue < 0)) {
            return false;
        }

        if (repeatValue >= 10000 ||
            parsedValues.size() +
                    static_cast<size_t>(repeatValue + 1) >
                10000) {
            return false;
        }
        const size_t count = static_cast<size_t>(repeatValue + 1);
        for (size_t index = 0; index < count; ++index) {
            parsedValues.push_back(currentTime);
            if (currentTime >
                std::numeric_limits<int64_t>::max() - durationValue) {
                return false;
            }
            currentTime += durationValue;
        }
    }
    if (parsedValues.size() < kPrefetchSegmentCount) {
        return false;
    }
    values = std::move(parsedValues);
    return true;
}

bool BuildSegmentValues(xmlNodePtr representation,
                        bool usesTime,
                        int64_t startNumber,
                        std::vector<int64_t>& values)
{
    if (usesTime) {
        return BuildSegmentTimelineValues(representation, values);
    }

    for (size_t index = 0; index < kPrefetchSegmentCount; ++index) {
        if (startNumber >
            std::numeric_limits<int64_t>::max() -
                static_cast<int64_t>(index)) {
            return false;
        }
        values.push_back(startNumber + static_cast<int64_t>(index));
    }
    return true;
}

std::string ResolveDashBase(xmlNodePtr representation,
                            const std::string& manifestUrl)
{
    std::vector<xmlNodePtr> ancestors;
    for (xmlNodePtr node = representation;
         node != nullptr;
         node = node->parent) {
        if (node->type == XML_ELEMENT_NODE) {
            ancestors.push_back(node);
        }
    }
    std::reverse(ancestors.begin(), ancestors.end());

    std::string baseUrl = manifestUrl;
    for (xmlNodePtr ancestor : ancestors) {
        for (xmlNodePtr child = ancestor->children;
             child != nullptr;
             child = child->next) {
            if (child->type == XML_ELEMENT_NODE &&
                xmlStrEqual(child->name, BAD_CAST "BaseURL")) {
                const std::string baseReference = XmlNodeContent(child);
                if (!baseReference.empty()) {
                    baseUrl = ResolveHttpUrl(baseUrl, baseReference);
                }
                break;
            }
        }
        if (baseUrl.empty()) {
            return {};
        }
    }
    return baseUrl;
}

bool IsSupportedTrack(xmlNodePtr adaptationSet,
                      xmlNodePtr representation,
                      const char* type)
{
    std::string mediaType = XmlProperty(representation, "mimeType");
    if (mediaType.empty()) {
        mediaType = XmlProperty(adaptationSet, "mimeType");
    }
    if (mediaType.empty()) {
        mediaType = XmlProperty(adaptationSet, "contentType");
    }
    const std::string typeName(type, std::strlen(type) - 1);
    return mediaType.rfind(type, 0) == 0 || mediaType == typeName;
}

std::string ErrorText(int error)
{
    char buffer[AV_ERROR_MAX_STRING_SIZE] = {};
    av_strerror(error, buffer, sizeof(buffer));
    return buffer;
}

} // namespace

struct DashResourceProxy::Impl {
    struct ResourceState {
        std::shared_ptr<std::vector<uint8_t>> data;
        std::string routeId;
        int64_t segmentValue = -1;
        bool initial = false;
        bool complete = false;
        bool failed = false;
        std::string error;
    };

    struct MediaRoute {
        std::string urlTemplate;
        std::string variableToken;
        std::string localSuffix;
        std::vector<int64_t> segmentValues;
        std::vector<int64_t> timelineValues;
        bool usesTime = false;
        int numberWidth = 0;
        int64_t startNumber = 1;
        int64_t timescale = 1;
        int64_t duration = 0;
        int64_t presentationTimeOffset = 0;
        double periodStartSeconds = 0.0;
        bool hasSeekTiming = false;
    };

    struct DownloadJob {
        std::string path;
        std::string url;
        std::string routeId;
        int64_t segmentValue = -1;
        std::shared_ptr<ResourceState> state;
    };

    ~Impl()
    {
        Stop();
    }

    bool Start(std::vector<uint8_t>& manifest,
               const std::string& manifestUrl,
               DashResourceProxy::ProgressCallback callback,
               bool prefetchInitialSegments)
    {
        if (manifest.empty() ||
            manifest.size() > static_cast<size_t>(std::numeric_limits<int>::max())) {
            return false;
        }

        xmlDocPtr document = xmlReadMemory(
            reinterpret_cast<const char*>(manifest.data()),
            static_cast<int>(manifest.size()),
            nullptr,
            nullptr,
            XML_PARSE_NONET | XML_PARSE_NOERROR | XML_PARSE_NOWARNING |
                XML_PARSE_COMPACT);
        if (document == nullptr) {
            return false;
        }

        xmlNodePtr root = xmlDocGetRootElement(document);
        if (root == nullptr || !xmlStrEqual(root->name, BAD_CAST "MPD")) {
            xmlFreeDoc(document);
            return false;
        }
        std::vector<PlannedTrack> tracks;
        for (xmlNodePtr period = root->children;
             period != nullptr;
             period = period->next) {
            if (period->type != XML_ELEMENT_NODE ||
                !xmlStrEqual(period->name, BAD_CAST "Period")) {
                continue;
            }
            for (xmlNodePtr adaptationSet = period->children;
                 adaptationSet != nullptr;
                 adaptationSet = adaptationSet->next) {
                if (adaptationSet->type != XML_ELEMENT_NODE ||
                    !xmlStrEqual(adaptationSet->name,
                                 BAD_CAST "AdaptationSet")) {
                    continue;
                }
                for (xmlNodePtr representation = adaptationSet->children;
                     representation != nullptr;
                     representation = representation->next) {
                    if (representation->type != XML_ELEMENT_NODE ||
                        !xmlStrEqual(representation->name,
                                     BAD_CAST "Representation") ||
                        (!IsSupportedTrack(adaptationSet,
                                           representation,
                                           "video/") &&
                         !IsSupportedTrack(adaptationSet,
                                           representation,
                                           "audio/"))) {
                        continue;
                    }

                    xmlNodePtr segmentTemplate = nullptr;
                    for (xmlNodePtr child = representation->children;
                         child != nullptr;
                         child = child->next) {
                        if (child->type == XML_ELEMENT_NODE &&
                            xmlStrEqual(child->name,
                                        BAD_CAST "SegmentTemplate")) {
                            segmentTemplate = child;
                            break;
                        }
                    }
                    if (segmentTemplate == nullptr) {
                        xmlFreeDoc(document);
                        return false;
                    }

                    std::string initialization =
                        XmlProperty(segmentTemplate, "initialization");
                    std::string media =
                        XmlProperty(segmentTemplate, "media");
                    std::string representationId =
                        XmlProperty(representation, "id");
                    std::string bandwidth =
                        XmlProperty(representation, "bandwidth");
                    if (initialization.empty() || media.empty()) {
                        xmlFreeDoc(document);
                        return false;
                    }
                    initialization = ReplaceToken(
                        std::move(initialization),
                        "$RepresentationID$",
                        representationId);
                    initialization = ReplaceToken(
                        std::move(initialization),
                        "$Bandwidth$",
                        bandwidth);
                    media = ReplaceToken(
                        std::move(media),
                        "$RepresentationID$",
                        representationId);
                    media = ReplaceToken(
                        std::move(media),
                        "$Bandwidth$",
                        bandwidth);

                    PlannedTrack track;
                    track.segmentTemplate = segmentTemplate;
                    track.routeId = std::to_string(tracks.size());
                    track.media.hasSeekTiming =
                        ParsePeriodStart(period,
                                         track.media.periodStartSeconds);
                    const std::string baseUrl =
                        ResolveDashBase(representation, manifestUrl);
                    if (baseUrl.empty()) {
                        xmlFreeDoc(document);
                        return false;
                    }
                    track.initUrl = ResolveHttpUrl(baseUrl, initialization);
                    track.media.remoteUrlTemplate =
                        ResolveHttpUrl(baseUrl, media);
                    if (track.initUrl.empty() ||
                        track.media.remoteUrlTemplate.empty() ||
                        track.initUrl.find('$') != std::string::npos ||
                        !GetMediaPlaceholder(
                            track.media.remoteUrlTemplate,
                            track.media.variableToken,
                            track.media.usesTime,
                            track.media.numberWidth)) {
                        xmlFreeDoc(document);
                        return false;
                    }
                    const size_t variablePosition =
                        track.media.remoteUrlTemplate.find(
                            track.media.variableToken);
                    if (variablePosition == std::string::npos) {
                        xmlFreeDoc(document);
                        return false;
                    }
                    const size_t suffixStart =
                        variablePosition + track.media.variableToken.size();
                    const size_t suffixEnd =
                        track.media.remoteUrlTemplate.find_first_of(
                            "?#", suffixStart);
                    track.media.localSuffix =
                        track.media.remoteUrlTemplate.substr(
                            suffixStart,
                            suffixEnd == std::string::npos
                                ? std::string::npos
                                : suffixEnd - suffixStart);
                    if (track.media.localSuffix.find('/') !=
                        std::string::npos) {
                        xmlFreeDoc(document);
                        return false;
                    }

                    const std::string startNumber =
                        InheritedTemplateProperty(representation,
                                                  "startNumber");
                    if (!startNumber.empty() &&
                        !ParseInteger(startNumber, track.media.startNumber)) {
                        xmlFreeDoc(document);
                        return false;
                    }
                    const std::string timescale =
                        InheritedTemplateProperty(representation, "timescale");
                    if (!timescale.empty() &&
                        (!ParseInteger(timescale, track.media.timescale) ||
                         track.media.timescale <= 0)) {
                        track.media.hasSeekTiming = false;
                    }
                    const std::string duration =
                        InheritedTemplateProperty(representation, "duration");
                    if (!duration.empty() &&
                        (!ParseInteger(duration, track.media.duration) ||
                         track.media.duration <= 0)) {
                        track.media.hasSeekTiming = false;
                    }
                    const std::string presentationTimeOffset =
                        InheritedTemplateProperty(
                            representation,
                            "presentationTimeOffset");
                    if (!presentationTimeOffset.empty() &&
                        !ParseInteger(presentationTimeOffset,
                                      track.media.presentationTimeOffset)) {
                        track.media.hasSeekTiming = false;
                    }
                    if (!BuildSegmentValues(
                            representation,
                            track.media.usesTime,
                            track.media.startNumber,
                            track.media.segmentValues)) {
                        xmlFreeDoc(document);
                        return false;
                    }
                    if (!track.media.usesTime) {
                        BuildSegmentTimelineValues(
                            representation,
                            track.media.timelineValues);
                    }
                    if (track.media.usesTime &&
                        track.media.segmentValues.empty()) {
                        track.media.hasSeekTiming = false;
                    } else if (!track.media.usesTime &&
                               track.media.duration <= 0 &&
                               track.media.timelineValues.empty()) {
                        track.media.hasSeekTiming = false;
                    }
                    tracks.push_back(std::move(track));
                }
            }
        }

        bool hasVideo = false;
        bool hasAudio = false;
        for (xmlNodePtr period = root->children;
             period != nullptr;
             period = period->next) {
            if (period->type != XML_ELEMENT_NODE ||
                !xmlStrEqual(period->name, BAD_CAST "Period")) {
                continue;
            }
            for (xmlNodePtr adaptationSet = period->children;
                 adaptationSet != nullptr;
                 adaptationSet = adaptationSet->next) {
                if (adaptationSet->type != XML_ELEMENT_NODE ||
                    !xmlStrEqual(adaptationSet->name,
                                 BAD_CAST "AdaptationSet")) {
                    continue;
                }
                for (xmlNodePtr representation = adaptationSet->children;
                     representation != nullptr;
                     representation = representation->next) {
                    if (representation->type != XML_ELEMENT_NODE ||
                        !xmlStrEqual(representation->name,
                                     BAD_CAST "Representation")) {
                        continue;
                    }
                    hasVideo = hasVideo ||
                        IsSupportedTrack(adaptationSet,
                                         representation,
                                         "video/");
                    hasAudio = hasAudio ||
                        IsSupportedTrack(adaptationSet,
                                         representation,
                                         "audio/");
                }
            }
        }
        if (tracks.empty() || (hasVideo && hasAudio && tracks.size() < 2)) {
            xmlFreeDoc(document);
            return false;
        }

        progress = std::move(callback);
        if (!OpenListener()) {
            xmlFreeDoc(document);
            Stop();
            return false;
        }

        basePath = "/" + NewToken() + "/";
        const std::string localBase =
            "http://127.0.0.1:" + std::to_string(port) + basePath;
        for (PlannedTrack& track : tracks) {
            const std::string initPath = basePath + "i/" + track.routeId;
            const std::string mediaPath =
                basePath + "m/" + track.routeId + "/";
            staticRoutes.emplace(initPath, track.initUrl);
            MediaRoute route;
            route.urlTemplate = track.media.remoteUrlTemplate;
            route.variableToken = track.media.variableToken;
            route.localSuffix = track.media.localSuffix;
            route.segmentValues = track.media.segmentValues;
            route.timelineValues = track.media.timelineValues;
            route.usesTime = track.media.usesTime;
            route.numberWidth = track.media.numberWidth;
            route.startNumber = track.media.startNumber;
            route.timescale = track.media.timescale;
            route.duration = track.media.duration;
            route.presentationTimeOffset =
                track.media.presentationTimeOffset;
            route.periodStartSeconds = track.media.periodStartSeconds;
            route.hasSeekTiming = track.media.hasSeekTiming;
            mediaRoutes.emplace(track.routeId, std::move(route));

            const std::string localInitialization =
                localBase + "i/" + track.routeId;
            const std::string localMedia =
                localBase + "m/" + track.routeId + "/" +
                (track.media.usesTime ? "$Time$" : "$Number$") +
                track.media.localSuffix;
            if (xmlSetProp(track.segmentTemplate,
                           BAD_CAST "initialization",
                           BAD_CAST localInitialization.c_str()) == nullptr ||
                xmlSetProp(track.segmentTemplate,
                           BAD_CAST "media",
                           BAD_CAST localMedia.c_str()) == nullptr) {
                xmlFreeDoc(document);
                Stop();
                return false;
            }

            ScheduleResource(initPath,
                             track.initUrl,
                             track.routeId,
                             -1,
                             true);
            if (prefetchInitialSegments) {
                for (size_t index = 0;
                     index < std::min(kPrefetchSegmentCount,
                                      track.media.segmentValues.size());
                     ++index) {
                    const int64_t value =
                        track.media.usesTime
                            ? track.media.segmentValues[index]
                            : track.media.startNumber +
                                  static_cast<int64_t>(index);
                    const std::string path =
                        mediaPath + std::to_string(value);
                    const std::string url = ReplaceToken(
                        track.media.remoteUrlTemplate,
                        track.media.variableToken,
                        FormatSegmentValue(value, track.media.numberWidth));
                    ScheduleResource(path, url, track.routeId, value, true);
                }
            }
        }

        if (!StartWorkers(prefetchInitialSegments
                              ? kPrefetchWorkerCount
                              : kAudioOnlyPrefetchWorkerCount)) {
            xmlFreeDoc(document);
            Stop();
            return false;
        }
        try {
            acceptThread = std::thread(&Impl::ServerLoop, this);
        } catch (const std::system_error&) {
            xmlFreeDoc(document);
            Stop();
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
            Stop();
            return false;
        }
        manifest.assign(serialized, serialized + serializedSize);
        xmlFree(serialized);

        const auto prefetchStarted = std::chrono::steady_clock::now();
        const bool prefetchComplete = WaitForInitialPrefetch();
        const auto prefetchElapsed = std::chrono::duration_cast<
            std::chrono::milliseconds>(
            std::chrono::steady_clock::now() - prefetchStarted);
        std::ostringstream message;
        message << "ffmpeg-dash-parallel-prefetch-"
                << (prefetchComplete ? "complete" : "continuing")
                << ": resources=" << initialResourceCount
                << " cached=" << CachedResourceCount()
                << " bytes=" << CachedBytes()
                << " failed=" << FailedResourceCount()
                << (LastFailure().empty()
                        ? std::string()
                        : " last-error=" + LastFailure())
                << " elapsed-ms=" << prefetchElapsed.count();
        Report(message.str());
        return true;
    }

    void Stop()
    {
        {
            std::lock_guard<std::mutex> lock(mutex);
            if (stopping) {
                return;
            }
            stopping = true;
            jobs.clear();
            condition.notify_all();
        }
        if (listener != nullptr) {
            g_socket_listener_close(listener);
        }
        if (acceptThread.joinable()) {
            acceptThread.join();
        }
        for (std::thread& worker : workers) {
            if (worker.joinable()) {
                worker.join();
            }
        }
        workers.clear();
        if (listener != nullptr) {
            g_object_unref(listener);
            listener = nullptr;
        }
    }

    void PrefetchForSeek(double positionSeconds)
    {
        PrefetchForSeekImpl(positionSeconds);
    }

    uint64_t GetEstimatedBandwidthBitsPerSecond() const
    {
        std::lock_guard<std::mutex> lock(mutex);
        return static_cast<uint64_t>(estimatedBandwidthBitsPerSecond);
    }

private:
    std::string NewToken() const
    {
        const auto now = std::chrono::steady_clock::now()
                             .time_since_epoch()
                             .count();
        std::ostringstream result;
        result << std::hex << now << "-" << std::hash<std::thread::id>{}(
            std::this_thread::get_id());
        return result.str();
    }

    bool OpenListener()
    {
        listener = g_socket_listener_new();
        GInetAddress* inetAddress =
            g_inet_address_new_loopback(G_SOCKET_FAMILY_IPV4);
        GSocketAddress* requestedAddress =
            g_inet_socket_address_new(inetAddress, 0);
        GSocketAddress* effectiveAddress = nullptr;
        GError* error = nullptr;
        const gboolean added = g_socket_listener_add_address(
            listener,
            requestedAddress,
            G_SOCKET_TYPE_STREAM,
            G_SOCKET_PROTOCOL_TCP,
            nullptr,
            &effectiveAddress,
            &error);
        g_object_unref(requestedAddress);
        g_object_unref(inetAddress);
        if (!added) {
            if (error != nullptr) {
                g_error_free(error);
            }
            return false;
        }
        port = g_inet_socket_address_get_port(
            G_INET_SOCKET_ADDRESS(effectiveAddress));
        g_object_unref(effectiveAddress);
        return port != 0;
    }

    bool StartWorkers(size_t workerCount)
    {
        try {
            for (size_t index = 0; index < workerCount; ++index) {
                workers.emplace_back(&Impl::WorkerLoop, this);
            }
        } catch (const std::system_error&) {
            return false;
        }
        return !workers.empty();
    }

    std::shared_ptr<ResourceState> ScheduleResource(
        const std::string& path,
        const std::string& url,
        const std::string& routeId,
        int64_t segmentValue,
        bool initial,
        bool prioritize = false)
    {
        std::lock_guard<std::mutex> lock(mutex);
        auto existing = resources.find(path);
        if (existing != resources.end() && !existing->second->failed) {
            if (prioritize && !existing->second->complete) {
                const auto queued = std::find_if(
                    jobs.begin(),
                    jobs.end(),
                    [&path](const DownloadJob& job) {
                        return job.path == path;
                    });
                if (queued != jobs.end()) {
                    DownloadJob job = std::move(*queued);
                    jobs.erase(queued);
                    jobs.push_front(std::move(job));
                    condition.notify_all();
                }
            }
            return existing->second;
        }

        auto state = std::make_shared<ResourceState>();
        state->routeId = routeId;
        state->segmentValue = segmentValue;
        state->initial = initial;
        resources[path] = state;
        DownloadJob job{path, url, routeId, segmentValue, state};
        if (prioritize) {
            jobs.push_front(std::move(job));
        } else {
            jobs.push_back(std::move(job));
        }
        if (initial) {
            ++initialPending;
            ++initialResourceCount;
        }
        condition.notify_one();
        return state;
    }

    void WorkerLoop()
    {
        while (true) {
            DownloadJob job;
            {
                std::unique_lock<std::mutex> lock(mutex);
                condition.wait(lock, [this] {
                    return stopping || !jobs.empty();
                });
                if (stopping) {
                    return;
                }
                job = std::move(jobs.front());
                jobs.pop_front();
            }

            std::vector<uint8_t> bytes;
            std::string error;
            const auto fetchStarted = std::chrono::steady_clock::now();
            const bool success = Fetch(job.url, bytes, error);
            const auto fetchElapsed = std::chrono::steady_clock::now() -
                                      fetchStarted;
            const size_t fetchedBytes = bytes.size();
            std::string failureMessage;
            {
                std::lock_guard<std::mutex> lock(mutex);
                job.state->complete = true;
                if (success && cachedBytes + bytes.size() <=
                                   kMaximumCacheBytes) {
                    job.state->data =
                        std::make_shared<std::vector<uint8_t>>(
                            std::move(bytes));
                    cachedBytes += job.state->data->size();
                } else {
                    job.state->failed = true;
                    job.state->error = success
                        ? "DASH resource cache reached its 32 MiB limit"
                        : std::move(error);
                    ++failedResourceCount;
                    lastFailure = job.state->error;
                    failureMessage = job.state->error;
                }
                if (job.state->initial && initialPending > 0) {
                    --initialPending;
                }
                condition.notify_all();
            }
            if (success && job.segmentValue >= 0) {
                RecordBandwidthSample(fetchedBytes, fetchElapsed);
            }
            if (!failureMessage.empty() && !job.state->initial) {
                Report("ffmpeg-dash-prefetch-resource-error: route=" +
                       (job.routeId.empty() ? std::string("init")
                                            : job.routeId) +
                       " segment=" + std::to_string(job.segmentValue) +
                       " reason=" + failureMessage);
            }
        }
    }

    void RecordBandwidthSample(
        size_t bytes,
        std::chrono::steady_clock::duration elapsed)
    {
        const auto elapsedNanoseconds =
            std::chrono::duration_cast<std::chrono::nanoseconds>(
                elapsed).count();
        if (bytes == 0 || elapsedNanoseconds <= 0) {
            return;
        }

        const double sample =
            static_cast<double>(bytes) * 8.0e9 /
            static_cast<double>(elapsedNanoseconds);
        uint64_t estimate = 0;
        {
            std::lock_guard<std::mutex> lock(mutex);
            estimatedBandwidthBitsPerSecond =
                estimatedBandwidthBitsPerSecond == 0
                    ? sample
                    : estimatedBandwidthBitsPerSecond * 0.75 + sample * 0.25;
            estimate = static_cast<uint64_t>(
                estimatedBandwidthBitsPerSecond);
        }
        Report("ffmpeg-dash-bandwidth-sample: bits-per-second=" +
               std::to_string(static_cast<uint64_t>(sample)) +
               " estimate-bps=" + std::to_string(estimate) +
               " bytes=" + std::to_string(bytes));
    }

    bool Fetch(const std::string& url,
               std::vector<uint8_t>& bytes,
               std::string& error)
    {
        AVIOContext* input = nullptr;
        AVDictionary* options = nullptr;
        av_dict_set(&options,
                    "rw_timeout",
                    std::to_string(
                        kResourceIoTimeoutSeconds * 1000000).c_str(),
                    0);
        int result = avio_open2(
            &input, url.c_str(), AVIO_FLAG_READ, nullptr, &options);
        av_dict_free(&options);
        if (result < 0) {
            error = ErrorText(result);
            return false;
        }

        std::array<uint8_t, 8192> buffer{};
        while (true) {
            const int count = avio_read(input,
                                        buffer.data(),
                                        static_cast<int>(buffer.size()));
            if (count == AVERROR_EOF || count == 0) {
                break;
            }
            if (count < 0) {
                result = count;
                break;
            }
            if (bytes.size() + static_cast<size_t>(count) >
                kMaximumResourceBytes) {
                result = AVERROR(EMSGSIZE);
                break;
            }
            bytes.insert(bytes.end(), buffer.begin(), buffer.begin() + count);
        }
        avio_closep(&input);
        if (result < 0) {
            bytes.clear();
            error = ErrorText(result);
            return false;
        }
        if (bytes.empty()) {
            error = "DASH resource was empty";
            return false;
        }
        return true;
    }

    bool WaitForInitialPrefetch()
    {
        std::unique_lock<std::mutex> lock(mutex);
        return condition.wait_for(
            lock,
            std::chrono::seconds(kResourceIoTimeoutSeconds + 2),
            [this] { return initialPending == 0 || stopping; }) &&
            initialPending == 0;
    }

    size_t CachedResourceCount() const
    {
        std::lock_guard<std::mutex> lock(mutex);
        size_t count = 0;
        for (const auto& entry : resources) {
            if (entry.second->data != nullptr) {
                ++count;
            }
        }
        return count;
    }

    size_t CachedBytes() const
    {
        std::lock_guard<std::mutex> lock(mutex);
        return cachedBytes;
    }

    size_t FailedResourceCount() const
    {
        std::lock_guard<std::mutex> lock(mutex);
        return failedResourceCount;
    }

    std::string LastFailure() const
    {
        std::lock_guard<std::mutex> lock(mutex);
        return lastFailure;
    }

    void PrefetchForSeekImpl(double positionSeconds)
    {
        if (!std::isfinite(positionSeconds) || positionSeconds < 0.0) {
            return;
        }

        std::vector<std::pair<std::string, MediaRoute>> routes;
        {
            std::lock_guard<std::mutex> lock(mutex);
            if (stopping) {
                return;
            }
            routes.reserve(mediaRoutes.size());
            for (const auto& entry : mediaRoutes) {
                routes.emplace_back(entry.first, entry.second);
            }
        }

        for (const auto& entry : routes) {
            const std::string& routeId = entry.first;
            const MediaRoute& route = entry.second;
            if (!route.hasSeekTiming) {
                Report("ffmpeg-dash-seek-prefetch-skipped: route=" +
                       routeId +
                       " reason=segment-timing-unavailable");
                continue;
            }

            size_t segmentIndex = 0;
            const std::vector<int64_t>& timelineValues =
                route.usesTime ? route.segmentValues
                               : route.timelineValues;
            if (!timelineValues.empty()) {
                const long double ticks =
                    (std::max(0.0, positionSeconds -
                                      route.periodStartSeconds) *
                         static_cast<long double>(route.timescale)) +
                    route.presentationTimeOffset;
                if (ticks > std::numeric_limits<int64_t>::max()) {
                    continue;
                }
                const int64_t targetTime =
                    static_cast<int64_t>(std::floor(ticks));
                const auto next = std::upper_bound(
                    timelineValues.begin(),
                    timelineValues.end(),
                    targetTime);
                if (next != timelineValues.begin()) {
                    segmentIndex = static_cast<size_t>(
                        next - timelineValues.begin() - 1);
                }
            } else if (!route.usesTime && route.duration > 0) {
                const long double index =
                    (std::max(0.0, positionSeconds -
                                      route.periodStartSeconds) *
                     static_cast<long double>(route.timescale)) /
                    route.duration +
                    static_cast<long double>(
                        route.presentationTimeOffset) /
                        route.duration;
                if (index > std::numeric_limits<int64_t>::max()) {
                    continue;
                }
                segmentIndex =
                    static_cast<size_t>(std::floor(index));
            } else {
                Report("ffmpeg-dash-seek-prefetch-skipped: route=" +
                       routeId +
                       " reason=segment-timeline-unavailable");
                continue;
            }

            size_t scheduled = 0;
            int64_t firstValue = -1;
            int64_t lastValue = -1;
            for (size_t remaining = kPrefetchSegmentCount;
                 remaining > 0;
                 --remaining) {
                const size_t offset = remaining - 1;
                if (segmentIndex >
                    std::numeric_limits<size_t>::max() - offset) {
                    break;
                }
                const size_t index = segmentIndex + offset;
                int64_t segmentValue = 0;
                if (route.usesTime) {
                    if (index >= route.segmentValues.size()) {
                        break;
                    }
                    segmentValue = route.segmentValues[index];
                } else {
                    if (index > static_cast<size_t>(
                                    std::numeric_limits<int64_t>::max()) ||
                        route.startNumber >
                            std::numeric_limits<int64_t>::max() -
                                static_cast<int64_t>(index)) {
                        break;
                    }
                    segmentValue =
                        route.startNumber + static_cast<int64_t>(index);
                }

                const std::string path =
                    basePath + "m/" + routeId + "/" +
                    std::to_string(segmentValue);
                const std::string url = ReplaceToken(
                    route.urlTemplate,
                    route.variableToken,
                    FormatSegmentValue(segmentValue, route.numberWidth));
                ScheduleResource(path,
                                 url,
                                 routeId,
                                 segmentValue,
                                 false,
                                 true);
                if (scheduled == 0) {
                    firstValue = segmentValue;
                }
                lastValue = segmentValue;
                ++scheduled;
            }

            if (scheduled > 0) {
                Report("ffmpeg-dash-seek-prefetch-scheduled: position=" +
                       std::to_string(positionSeconds) +
                       " route=" + routeId +
                       " first-segment=" + std::to_string(firstValue) +
                       " last-segment=" + std::to_string(lastValue));
            }
        }
    }

    void PruneRoute(const std::string& routeId, int64_t keepFrom)
    {
        std::lock_guard<std::mutex> lock(mutex);
        for (auto entry = resources.begin(); entry != resources.end();) {
            const std::shared_ptr<ResourceState>& state = entry->second;
            if (state->routeId == routeId && state->segmentValue >= 0 &&
                state->segmentValue < keepFrom && state->complete) {
                if (state->data != nullptr) {
                    cachedBytes -= state->data->size();
                }
                entry = resources.erase(entry);
            } else {
                ++entry;
            }
        }
    }

    bool GetResource(const std::string& path,
                     std::shared_ptr<ResourceState>& state)
    {
        std::string url;
        std::string routeId;
        int64_t segmentValue = -1;
        {
            std::lock_guard<std::mutex> lock(mutex);
            const auto staticRoute = staticRoutes.find(path);
            if (staticRoute != staticRoutes.end()) {
                url = staticRoute->second;
            } else {
                const std::string mediaPrefix = basePath + "m/";
                if (path.rfind(mediaPrefix, 0) != 0) {
                    return false;
                }
                const size_t routeStart = mediaPrefix.size();
                const size_t separator = path.find('/', routeStart);
                if (separator == std::string::npos) {
                    return false;
                }
                routeId = path.substr(routeStart, separator - routeStart);
                const auto route = mediaRoutes.find(routeId);
                if (route == mediaRoutes.end()) {
                    return false;
                }
                std::string valueText = path.substr(separator + 1);
                if (!route->second.localSuffix.empty()) {
                    if (valueText.size() < route->second.localSuffix.size() ||
                        valueText.compare(
                            valueText.size() -
                                route->second.localSuffix.size(),
                            route->second.localSuffix.size(),
                            route->second.localSuffix) != 0) {
                        return false;
                    }
                    valueText.resize(
                        valueText.size() -
                        route->second.localSuffix.size());
                }
                if (!ParseInteger(valueText, segmentValue)) {
                    return false;
                }
                const std::string replacement =
                    FormatSegmentValue(segmentValue,
                                       route->second.numberWidth);
                url = ReplaceToken(route->second.urlTemplate,
                                   route->second.variableToken,
                                   replacement);
            }
        }

        if (url.empty()) {
            return false;
        }

        const std::string cachePath = routeId.empty()
            ? path
            : basePath + "m/" + routeId + "/" +
                  std::to_string(segmentValue);
        state = ScheduleResource(
            cachePath, url, routeId, segmentValue, false, true);
        if (!routeId.empty()) {
            const auto route = mediaRoutes.find(routeId);
            if (route != mediaRoutes.end()) {
                int64_t nextValue = -1;
                if (route->second.usesTime) {
                    const auto value = std::find(
                        route->second.segmentValues.begin(),
                        route->second.segmentValues.end(),
                        segmentValue);
                    if (value != route->second.segmentValues.end()) {
                        const size_t index = static_cast<size_t>(
                            value - route->second.segmentValues.begin());
                        if (index + kPrefetchSegmentCount <
                            route->second.segmentValues.size()) {
                            nextValue = route->second.segmentValues[
                                index + kPrefetchSegmentCount];
                        }
                    }
                } else if (segmentValue <=
                           std::numeric_limits<int64_t>::max() -
                               static_cast<int64_t>(
                                   kPrefetchSegmentCount)) {
                    nextValue = segmentValue +
                        static_cast<int64_t>(kPrefetchSegmentCount);
                }
                if (nextValue >= 0) {
                    const std::string nextPath =
                        basePath + "m/" + routeId + "/" +
                        std::to_string(nextValue);
                    const std::string nextUrl = ReplaceToken(
                        route->second.urlTemplate,
                        route->second.variableToken,
                        FormatSegmentValue(nextValue,
                                           route->second.numberWidth));
                    ScheduleResource(nextPath,
                                     nextUrl,
                                     routeId,
                                     nextValue,
                                     false);
                    PruneRoute(routeId, segmentValue - 2);
                }
            }
        }

        const auto requestStarted = std::chrono::steady_clock::now();
        std::unique_lock<std::mutex> lock(mutex);
        const bool availableBeforeWait =
            state->complete && !state->failed && state->data != nullptr;
        const bool completed = condition.wait_for(
            lock,
            std::chrono::seconds(kResourceIoTimeoutSeconds + 2),
            [this, &state] { return state->complete || stopping; });
        const bool available =
            completed && state->complete && !state->failed &&
            state->data != nullptr;
        const size_t bytes = available ? state->data->size() : 0;
        const std::string error = state->error;
        lock.unlock();
        if (!availableBeforeWait && available) {
            const auto waitMilliseconds = std::chrono::duration_cast<
                std::chrono::milliseconds>(
                std::chrono::steady_clock::now() - requestStarted);
            Report("ffmpeg-dash-proxy-cache-wait: route=" +
                   (routeId.empty() ? std::string("init") : routeId) +
                   " segment=" + std::to_string(segmentValue) +
                   " wait-ms=" + std::to_string(waitMilliseconds.count()) +
                   " bytes=" + std::to_string(bytes));
        } else if (completed && !available) {
            Report("ffmpeg-dash-proxy-resource-error: route=" +
                   (routeId.empty() ? std::string("init") : routeId) +
                   " segment=" + std::to_string(segmentValue) +
                   " reason=" + error);
        }
        return available;
    }

    void ServerLoop()
    {
        while (true) {
            GError* error = nullptr;
            GSocketConnection* connection = g_socket_listener_accept(
                listener, nullptr, nullptr, &error);
            if (connection == nullptr) {
                if (error != nullptr) {
                    g_error_free(error);
                }
                std::lock_guard<std::mutex> lock(mutex);
                if (stopping) {
                    return;
                }
                continue;
            }
            GSocket* socket = g_socket_connection_get_socket(connection);
            g_socket_set_timeout(socket, kHttpReadTimeoutSeconds);
            HandleConnection(connection);
            g_object_unref(connection);
        }
    }

    void HandleConnection(GSocketConnection* connection)
    {
        GInputStream* input =
            g_io_stream_get_input_stream(G_IO_STREAM(connection));
        GOutputStream* output =
            g_io_stream_get_output_stream(G_IO_STREAM(connection));
        std::string request;
        std::array<char, 2048> buffer{};
        while (request.size() < kMaximumHttpHeaderBytes &&
               request.find("\r\n\r\n") == std::string::npos) {
            GError* error = nullptr;
            const gssize count = g_input_stream_read(
                input, buffer.data(), buffer.size(), nullptr, &error);
            if (count <= 0) {
                if (error != nullptr) {
                    g_error_free(error);
                }
                return;
            }
            request.append(buffer.data(), static_cast<size_t>(count));
            if (error != nullptr) {
                g_error_free(error);
            }
        }

        const size_t lineEnd = request.find("\r\n");
        const size_t firstSpace = request.find(' ');
        const size_t secondSpace = firstSpace == std::string::npos
            ? std::string::npos
            : request.find(' ', firstSpace + 1);
        if (lineEnd == std::string::npos || firstSpace == std::string::npos ||
            secondSpace == std::string::npos || secondSpace > lineEnd) {
            SendError(output, 400, "Bad Request");
            return;
        }
        const std::string method = request.substr(0, firstSpace);
        std::string path =
            request.substr(firstSpace + 1, secondSpace - firstSpace - 1);
        const size_t query = path.find('?');
        if (query != std::string::npos) {
            path.resize(query);
        }
        if (method != "GET" && method != "HEAD") {
            SendError(output, 405, "Method Not Allowed");
            return;
        }

        std::shared_ptr<ResourceState> state;
        if (!GetResource(path, state)) {
            Report("ffmpeg-dash-proxy-request-failed: path=" + path);
            SendError(output, 502, "Bad Gateway");
            return;
        }

        const std::string header =
            "HTTP/1.1 200 OK\r\nContent-Type: application/octet-stream\r\n"
            "Content-Length: " + std::to_string(state->data->size()) +
            "\r\nConnection: close\r\n\r\n";
        gsize written = 0;
        GError* error = nullptr;
        if (!g_output_stream_write_all(output,
                                       header.data(),
                                       header.size(),
                                       &written,
                                       nullptr,
                                       &error)) {
            if (error != nullptr) {
                g_error_free(error);
            }
            return;
        }
        if (method == "GET" && !state->data->empty()) {
            g_output_stream_write_all(output,
                                      state->data->data(),
                                      state->data->size(),
                                      &written,
                                      nullptr,
                                      &error);
        }
        if (error != nullptr) {
            g_error_free(error);
        }
    }

    void SendError(GOutputStream* output, int status, const char* reason)
    {
        const std::string header =
            "HTTP/1.1 " + std::to_string(status) + " " + reason +
            "\r\nContent-Length: 0\r\nConnection: close\r\n\r\n";
        gsize written = 0;
        GError* error = nullptr;
        g_output_stream_write_all(output,
                                  header.data(),
                                  header.size(),
                                  &written,
                                  nullptr,
                                  &error);
        if (error != nullptr) {
            g_error_free(error);
        }
    }

    void Report(const std::string& message) const
    {
        if (progress) {
            const std::lock_guard<std::mutex> lock(progressMutex);
            progress(message);
        }
    }

    GSocketListener* listener = nullptr;
    guint16 port = 0;
    std::string basePath;
    DashResourceProxy::ProgressCallback progress;
    std::thread acceptThread;
    std::vector<std::thread> workers;
    mutable std::mutex mutex;
    mutable std::mutex progressMutex;
    std::condition_variable condition;
    bool stopping = false;
    size_t cachedBytes = 0;
    size_t initialPending = 0;
    size_t initialResourceCount = 0;
    size_t failedResourceCount = 0;
    double estimatedBandwidthBitsPerSecond = 0.0;
    std::string lastFailure;
    std::deque<DownloadJob> jobs;
    std::unordered_map<std::string, std::shared_ptr<ResourceState>> resources;
    std::unordered_map<std::string, std::string> staticRoutes;
    std::unordered_map<std::string, MediaRoute> mediaRoutes;
};

DashResourceProxy::DashResourceProxy()
    : impl_(std::make_unique<Impl>())
{
}

DashResourceProxy::~DashResourceProxy() = default;

bool DashResourceProxy::Start(std::vector<uint8_t>& manifest,
                              const std::string& manifestUrl,
                              ProgressCallback progress,
                              bool prefetchInitialSegments)
{
    return impl_->Start(manifest,
                        manifestUrl,
                        std::move(progress),
                        prefetchInitialSegments);
}

void DashResourceProxy::PrefetchForSeek(double positionSeconds)
{
    impl_->PrefetchForSeek(positionSeconds);
}

uint64_t DashResourceProxy::GetEstimatedBandwidthBitsPerSecond() const
{
    return impl_->GetEstimatedBandwidthBitsPerSecond();
}

void DashResourceProxy::Stop()
{
    if (impl_ != nullptr) {
        impl_->Stop();
    }
}
