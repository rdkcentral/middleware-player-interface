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

#include <cstddef>
#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <vector>

class SegmentResourceProxy {
public:
    using ProgressCallback = std::function<void(const std::string&)>;

    SegmentResourceProxy();
    ~SegmentResourceProxy();

    SegmentResourceProxy(const SegmentResourceProxy&) = delete;
    SegmentResourceProxy& operator=(const SegmentResourceProxy&) = delete;

    bool Start(std::vector<uint8_t>& manifest,
               const std::string& manifestUrl,
               ProgressCallback progress,
               std::size_t initialSegmentCount = 4);
    void PrefetchForSeek(double positionSeconds);
    uint64_t GetEstimatedBandwidthBitsPerSecond() const;
    void Stop();

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};
