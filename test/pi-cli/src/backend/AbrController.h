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
