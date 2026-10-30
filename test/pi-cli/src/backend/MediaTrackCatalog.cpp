#include "MediaTrackCatalog.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/dict.h>
}

#include <libxml/parser.h>
#include <libxml/tree.h>

#include <algorithm>
#include <cerrno>
#include <cctype>
#include <cstdlib>
#include <limits>
#include <utility>

namespace {

std::string XmlProperty(xmlNodePtr node, const char* name)
{
    if (node == nullptr) {
        return {};
    }
    xmlChar* value = xmlGetProp(node, BAD_CAST name);
    if (value == nullptr) {
        return {};
    }

    std::string result(reinterpret_cast<const char*>(value));
    xmlFree(value);
    return result;
}

bool IsElement(xmlNodePtr node, const char* name)
{
    return node->type == XML_ELEMENT_NODE &&
           xmlStrEqual(node->name, BAD_CAST name);
}

std::string MediaType(xmlNodePtr adaptationSet)
{
    const std::string contentType =
        XmlProperty(adaptationSet, "contentType");
    if (contentType == "audio" || contentType == "video") {
        return contentType;
    }

    std::string mimeType = XmlProperty(adaptationSet, "mimeType");
    if (mimeType.empty()) {
        for (xmlNodePtr child = adaptationSet->children;
             child != nullptr;
             child = child->next) {
            if (IsElement(child, "Representation")) {
                mimeType = XmlProperty(child, "mimeType");
                if (!mimeType.empty()) {
                    break;
                }
            }
        }
    }
    if (mimeType.rfind("audio/", 0) == 0) {
        return "audio";
    }
    if (mimeType.rfind("video/", 0) == 0) {
        return "video";
    }
    return {};
}

bool HasRole(xmlNodePtr adaptationSet, const char* target)
{
    for (xmlNodePtr child = adaptationSet->children;
         child != nullptr;
         child = child->next) {
        if (!IsElement(child, "Role")) {
            continue;
        }
        std::string role = XmlProperty(child, "value");
        std::transform(role.begin(), role.end(), role.begin(), [](char value) {
            return static_cast<char>(
                std::tolower(static_cast<unsigned char>(value)));
        });
        if (role.find(target) != std::string::npos) {
            return true;
        }
    }
    return false;
}

bool IsTrickMode(xmlNodePtr adaptationSet)
{
    for (xmlNodePtr child = adaptationSet->children;
         child != nullptr;
         child = child->next) {
        if (!IsElement(child, "EssentialProperty") &&
            !IsElement(child, "SupplementalProperty")) {
            continue;
        }
        if (XmlProperty(child, "schemeIdUri").find("trickmode") !=
            std::string::npos) {
            return true;
        }
    }
    return HasRole(adaptationSet, "trickmode");
}

uint64_t UnsignedProperty(xmlNodePtr node, const char* name)
{
    const std::string text = XmlProperty(node, name);
    if (text.empty()) {
        return 0;
    }
    char* end = nullptr;
    errno = 0;
    const unsigned long long value =
        std::strtoull(text.c_str(), &end, 10);
    if (errno != 0 || end == text.c_str() || end == nullptr || *end != '\0') {
        return 0;
    }
    return static_cast<uint64_t>(value);
}

std::string InheritedProperty(xmlNodePtr node,
                              xmlNodePtr parent,
                              const char* name)
{
    std::string value = XmlProperty(node, name);
    return value.empty() ? XmlProperty(parent, name) : value;
}

std::string TrackId(const char* type,
                   size_t periodIndex,
                   size_t ordinal,
                   const std::string& manifestId)
{
    return std::string(type) + ":" + std::to_string(periodIndex) + ":" +
           (manifestId.empty() ? std::to_string(ordinal) : manifestId);
}

std::string CodecName(AVCodecID codecId)
{
    const char* name = avcodec_get_name(codecId);
    return name == nullptr ? std::string() : std::string(name);
}

} // namespace

bool BuildDashTrackCatalog(const std::vector<uint8_t>& manifest,
                           std::vector<MediaTrackInfo>& tracks,
                           std::string& error)
{
    if (manifest.empty() ||
        manifest.size() > static_cast<size_t>(std::numeric_limits<int>::max())) {
        error = "DASH manifest is empty or too large to catalog";
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
        error = "Could not parse DASH manifest for track discovery";
        return false;
    }

    xmlNodePtr root = xmlDocGetRootElement(document);
    if (root == nullptr || !xmlStrEqual(root->name, BAD_CAST "MPD")) {
        xmlFreeDoc(document);
        error = "DASH track discovery did not receive an MPD document";
        return false;
    }

    size_t periodIndex = 0;
    size_t audioOrdinal = 0;
    size_t videoOrdinal = 0;
    for (xmlNodePtr period = root->children;
         period != nullptr;
         period = period->next) {
        if (!IsElement(period, "Period")) {
            continue;
        }

        xmlNodePtr defaultVideoSet = nullptr;
        xmlNodePtr defaultAudioSet = nullptr;
        size_t selectedVideoTrackIndex =
            std::numeric_limits<size_t>::max();
        for (xmlNodePtr adaptationSet = period->children;
             adaptationSet != nullptr;
             adaptationSet = adaptationSet->next) {
            if (!IsElement(adaptationSet, "AdaptationSet")) {
                continue;
            }
            const std::string mediaType = MediaType(adaptationSet);
            if (mediaType == "video" && defaultVideoSet == nullptr &&
                !IsTrickMode(adaptationSet)) {
                defaultVideoSet = adaptationSet;
            } else if (mediaType == "audio" && defaultAudioSet == nullptr &&
                       !HasRole(adaptationSet, "commentary") &&
                       !HasRole(adaptationSet, "description")) {
                defaultAudioSet = adaptationSet;
            }
        }

        for (xmlNodePtr adaptationSet = period->children;
             adaptationSet != nullptr;
             adaptationSet = adaptationSet->next) {
            if (!IsElement(adaptationSet, "AdaptationSet")) {
                continue;
            }
            const std::string mediaType = MediaType(adaptationSet);
            if (mediaType == "audio" &&
                !HasRole(adaptationSet, "commentary") &&
                !HasRole(adaptationSet, "description")) {
                xmlNodePtr representation = nullptr;
                for (xmlNodePtr child = adaptationSet->children;
                     child != nullptr;
                     child = child->next) {
                    if (IsElement(child, "Representation")) {
                        representation = child;
                        break;
                    }
                }
                MediaTrackInfo track;
                track.id = TrackId(
                    "audio",
                    periodIndex,
                    audioOrdinal++,
                    XmlProperty(adaptationSet, "id"));
                track.type = MediaTrackType::Audio;
                track.codec = InheritedProperty(
                    representation, adaptationSet, "codecs");
                track.language = InheritedProperty(
                    adaptationSet, representation, "lang");
                track.role = HasRole(adaptationSet, "main")
                    ? "main"
                    : (HasRole(adaptationSet, "alternate")
                           ? "alternate"
                           : std::string());
                if (representation != nullptr) {
                    track.bitrate =
                        UnsignedProperty(representation, "bandwidth");
                }
                track.selected = adaptationSet == defaultAudioSet;
                tracks.push_back(std::move(track));
            } else if (mediaType == "video" && !IsTrickMode(adaptationSet)) {
                for (xmlNodePtr representation = adaptationSet->children;
                     representation != nullptr;
                     representation = representation->next) {
                    if (!IsElement(representation, "Representation")) {
                        continue;
                    }
                    MediaTrackInfo track;
                    track.id = TrackId(
                        "video",
                        periodIndex,
                        videoOrdinal++,
                        XmlProperty(representation, "id"));
                    track.type = MediaTrackType::Video;
                    track.codec = InheritedProperty(
                        representation, adaptationSet, "codecs");
                    track.bitrate =
                        UnsignedProperty(representation, "bandwidth");
                    track.width = static_cast<int>(
                        UnsignedProperty(representation, "width"));
                    track.height = static_cast<int>(
                        UnsignedProperty(representation, "height"));
                    const size_t trackIndex = tracks.size();
                    track.selected = false;
                    tracks.push_back(std::move(track));
                    if (adaptationSet == defaultVideoSet &&
                        selectedVideoTrackIndex ==
                            std::numeric_limits<size_t>::max()) {
                        selectedVideoTrackIndex = trackIndex;
                    }
                }
            }
        }

        if (selectedVideoTrackIndex !=
            std::numeric_limits<size_t>::max()) {
            tracks[selectedVideoTrackIndex].selected = true;
        }
        ++periodIndex;
    }

    xmlFreeDoc(document);
    if (tracks.empty()) {
        error = "DASH manifest contains no audio or video tracks";
        return false;
    }
    return true;
}

std::vector<MediaTrackInfo> BuildAvFormatTrackCatalog(
    const AVFormatContext& format,
    int selectedVideoStream,
    int selectedAudioStream)
{
    std::vector<MediaTrackInfo> tracks;
    for (unsigned int index = 0; index < format.nb_streams; ++index) {
        const AVStream* stream = format.streams[index];
        const AVCodecParameters* parameters = stream->codecpar;
        if (parameters->codec_type != AVMEDIA_TYPE_AUDIO &&
            parameters->codec_type != AVMEDIA_TYPE_VIDEO) {
            continue;
        }

        MediaTrackInfo track;
        track.id = "stream:" + std::to_string(index);
        track.type = parameters->codec_type == AVMEDIA_TYPE_AUDIO
            ? MediaTrackType::Audio
            : MediaTrackType::Video;
        track.codec = CodecName(parameters->codec_id);
        track.bitrate = parameters->bit_rate > 0
            ? static_cast<uint64_t>(parameters->bit_rate)
            : 0;
        track.width = parameters->width;
        track.height = parameters->height;
        const AVDictionaryEntry* language =
            av_dict_get(stream->metadata, "language", nullptr, 0);
        if (language != nullptr && language->value != nullptr) {
            track.language = language->value;
        }
        track.selected = parameters->codec_type == AVMEDIA_TYPE_VIDEO
            ? static_cast<int>(index) == selectedVideoStream
            : static_cast<int>(index) == selectedAudioStream;
        tracks.push_back(std::move(track));
    }
    return tracks;
}
