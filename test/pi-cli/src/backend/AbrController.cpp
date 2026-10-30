#include "AbrController.h"

#include <algorithm>

namespace {

std::string TrackPeriodPrefix(const std::string& trackId)
{
    const size_t separator = trackId.find(':');
    if (separator == std::string::npos) {
        return {};
    }
    const size_t periodEnd = trackId.find(':', separator + 1);
    return periodEnd == std::string::npos
        ? std::string()
        : trackId.substr(0, periodEnd + 1);
}

uint64_t SafeVideoBudget(uint64_t bandwidth)
{
    return bandwidth / 100 * AbrController::kSafetyPercentage +
           bandwidth % 100 * AbrController::kSafetyPercentage / 100;
}

} // namespace

AbrDecision AbrController::Evaluate(
    const std::vector<MediaTrackInfo>& tracks,
    const MediaTrackSelection& selection,
    uint64_t measuredBandwidthBitsPerSecond)
{
    AbrDecision decision;
    decision.measuredBandwidthBitsPerSecond =
        measuredBandwidthBitsPerSecond;
    if (measuredBandwidthBitsPerSecond == 0) {
        return decision;
    }

    const std::string periodPrefix =
        TrackPeriodPrefix(selection.videoTrackId);
    std::vector<const MediaTrackInfo*> candidates;
    const MediaTrackInfo* current = nullptr;
    for (const MediaTrackInfo& track : tracks) {
        if (track.type == MediaTrackType::Audio &&
            track.id == selection.audioTrackId) {
            decision.reservedAudioBandwidthBitsPerSecond = track.bitrate;
        }
        if (track.type != MediaTrackType::Video ||
            track.id.rfind(periodPrefix, 0) != 0 ||
            track.bitrate == 0) {
            continue;
        }
        candidates.push_back(&track);
        if (track.id == selection.videoTrackId) {
            current = &track;
        }
    }
    if (current == nullptr || candidates.size() < 2) {
        return decision;
    }

    std::sort(candidates.begin(), candidates.end(),
              [](const MediaTrackInfo* left, const MediaTrackInfo* right) {
                  return left->bitrate < right->bitrate;
              });
    const uint64_t safeBandwidth = SafeVideoBudget(
        measuredBandwidthBitsPerSecond);
    decision.safeVideoBandwidthBitsPerSecond =
        safeBandwidth > decision.reservedAudioBandwidthBitsPerSecond
            ? safeBandwidth -
                  decision.reservedAudioBandwidthBitsPerSecond
            : 0;

    const MediaTrackInfo* target = candidates.front();
    for (const MediaTrackInfo* candidate : candidates) {
        if (candidate->bitrate <=
            decision.safeVideoBandwidthBitsPerSecond) {
            target = candidate;
        }
    }

    decision.valid = true;
    decision.currentTrackId = current->id;
    decision.targetTrackId = target->id;
    if (target->id == current->id ||
        (target->bitrate > current->bitrate &&
         static_cast<long double>(measuredBandwidthBitsPerSecond) <
             (static_cast<long double>(target->bitrate) +
              static_cast<long double>(
                  decision.reservedAudioBandwidthBitsPerSecond)) *
                 1.35L)) {
        Reset();
        decision.targetTrackId = current->id;
        return decision;
    }

    if (candidateTrackId_ == target->id) {
        ++candidateCount_;
    } else {
        candidateTrackId_ = target->id;
        candidateCount_ = 1;
    }
    decision.confirmationCount = candidateCount_;
    decision.shouldSwitch =
        candidateCount_ >= kRequiredConfirmations;
    return decision;
}

size_t AbrController::CountVideoRepresentations(
    const std::vector<MediaTrackInfo>& tracks,
    const std::string& currentTrackId) const
{
    const std::string periodPrefix = TrackPeriodPrefix(currentTrackId);
    return static_cast<size_t>(std::count_if(
        tracks.begin(),
        tracks.end(),
        [&periodPrefix](const MediaTrackInfo& track) {
            return track.type == MediaTrackType::Video &&
                   track.bitrate > 0 &&
                   track.id.rfind(periodPrefix, 0) == 0;
        }));
}

void AbrController::Reset()
{
    candidateTrackId_.clear();
    candidateCount_ = 0;
}
