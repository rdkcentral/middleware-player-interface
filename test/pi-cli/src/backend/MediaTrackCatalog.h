#pragma once

#include <cstdint>
#include <string>
#include <vector>

struct AVFormatContext;

enum class MediaTrackType {
    Audio,
    Video
};

struct MediaTrackInfo {
    std::string id;
    MediaTrackType type = MediaTrackType::Video;
    std::string codec;
    std::string language;
    std::string role;
    uint64_t bitrate = 0;
    int width = 0;
    int height = 0;
    bool selected = false;
};

bool BuildDashTrackCatalog(const std::vector<uint8_t>& manifest,
                           std::vector<MediaTrackInfo>& tracks,
                           std::string& error);

std::vector<MediaTrackInfo> BuildAvFormatTrackCatalog(
    const AVFormatContext& format,
    int selectedVideoStream,
    int selectedAudioStream);
