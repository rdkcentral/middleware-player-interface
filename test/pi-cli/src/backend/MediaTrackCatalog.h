/*
 * If not stated otherwise in this file or this component's license file the
 * following copyright and licenses apply:
 *
 * Copyright 2026 RDK Management
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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

struct MediaTrackSelection {
    std::string audioTrackId;
    std::string videoTrackId;
};

bool BuildDashTrackCatalog(const std::vector<uint8_t>& manifest,
                           std::vector<MediaTrackInfo>& tracks,
                           std::string& error);

std::vector<MediaTrackInfo> BuildAvFormatTrackCatalog(
    const AVFormatContext& format,
    int selectedVideoStream,
    int selectedAudioStream);
