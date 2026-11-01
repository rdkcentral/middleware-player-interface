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

#include "MediaTrackCatalog.h"

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

struct AbrDecision {
    bool valid = false;
    bool shouldSwitch = false;
    uint64_t measuredBandwidthBitsPerSecond = 0;
    uint64_t safeVideoBandwidthBitsPerSecond = 0;
    uint64_t reservedAudioBandwidthBitsPerSecond = 0;
    unsigned int confirmationCount = 0;
    std::string currentTrackId;
    std::string targetTrackId;
};

class AbrController {
public:
    static constexpr uint64_t kSafetyPercentage = 70;
    static constexpr unsigned int kRequiredConfirmations = 2;

    AbrDecision Evaluate(const std::vector<MediaTrackInfo>& tracks,
                         const MediaTrackSelection& selection,
                         uint64_t measuredBandwidthBitsPerSecond);
    size_t CountVideoRepresentations(
        const std::vector<MediaTrackInfo>& tracks,
        const std::string& currentTrackId) const;
    void Reset();

private:
    std::string candidateTrackId_;
    unsigned int candidateCount_ = 0;
};
