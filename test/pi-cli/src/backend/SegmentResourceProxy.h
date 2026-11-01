/*
 * SPDX-License-Identifier: Apache-2.0
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
