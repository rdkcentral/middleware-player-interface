/*
 * If not stated otherwise in this file or this component's license file the
 * following copyright and licenses apply:
 *
 * Copyright 2025 RDK Management
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


/**
 * @file SocUtils.h
 * @brief Public APIs for core code to access vendor capabilities at runtime.
 *
 * This header defines the SocUtils namespace, which provides a set of utility functions
 * to query and interact with platform-specific (SoC) features. These functions abstract
 * hardware-level capabilities such as audio codec support, playback configuration,
 * latency correction, and rendering options, allowing the core application to adapt
 * dynamically to the underlying hardware.
 */
#ifndef SOC_UTILS_H
#define SOC_UTILS_H

/**
 * @namespace SocUtils
 * @brief Contains utility functions related to SoC (System on Chip) capabilities and configurations.
 */
namespace SocUtils
{
    /**
     * @brief Determines whether AppSrc should be used for progressive playback.
     * @return True if AppSrc is to be used, false otherwise.
     */
    bool UseAppSrcForProgressivePlayback(void);

    /**
     * @brief Checks if AC-4 audio format is supported by the system.
     * @return True if AC-4 is supported, false otherwise.
     */
    bool IsSupportedAC4(void);

    /**
     * @brief Determines whether the Westeros sink should be used for rendering.
     * @return True if Westeros sink is to be used, false otherwise.
     */
    bool UseWesterosSink(void);

    /**
     * @brief Checks if audio fragment synchronization is supported.
     * @return True if audio fragment sync is supported, false otherwise.
     */
    bool IsAudioFragmentSyncSupported(void);

    /**
     * @brief Enables or disables live latency correction.
     * @return True if live latency correction is enabled, false otherwise.
     */
    bool EnableLiveLatencyCorrection(void);

    /**
     * @brief Checks if AC-3 audio format is supported by the system.
     * @return True if AC-3 is supported, false otherwise.
     */
    bool IsSupportedAC3(void);

    /**
     * @brief Gets the number of frames required to be queued for smooth playback.
     * @return The number of required queued frames.
     */
    int RequiredQueuedFrames(void);

    /**
     * @brief Enables or disables PTS (Presentation Time Stamp) restamping.
     * @return True if PTS restamping is enabled, false otherwise.
     */
    bool EnablePTSRestamp(void);

    /**
     * @brief Resets the new segment event state.
     * @return True if the reset was successful, false otherwise.
     */
    bool ResetNewSegmentEvent(void);
}

#endif // SOC_UTILS_H

