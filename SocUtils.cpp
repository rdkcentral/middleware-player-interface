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
 * @file SocUtils.cpp
 * @brief Implementation of utility functions for querying and configuring SoC (System on Chip) capabilities.
 *
 * This file defines the implementation of the SocUtils namespace functions, which provide
 * access to various hardware and platform-specific features such as audio codec support,
 * playback configuration, latency correction, and rendering options.
 *
 * These utilities rely on the SocInterface and InterfacePlayerRDK classes to query
 * platform-specific capabilities and behaviors.
 */

#include "SocUtils.h"
#include "SocInterface.h"
#include "InterfacePlayerRDK.h"
#include <assert.h>

namespace SocUtils
{
    /// Shared pointer to the SoC interface instance.
    static std::shared_ptr<SocInterface> socInterface = SocInterface::CreateSocInterface();

    /**
     * @brief Determines whether AppSrc should be used for progressive playback.
     * @return True if AppSrc is to be used, false otherwise.
     */
    bool UseAppSrcForProgressivePlayback(void)
    {
        return socInterface->UseAppSrc();
    }

    /**
     * @brief Checks if AC-4 audio format is supported by the system.
     * @return True if AC-4 is supported, false otherwise.
     */
    bool IsSupportedAC4(void)
    {
        bool disableAc = socInterface->IsSupportedAC4();
        return (disableAc || (!InterfacePlayerRDK::IsCodecSupported("ac-4")));
    }

    /**
     * @brief Checks if AC-3 audio format is supported by the system.
     * @return True if AC-3 is supported, false otherwise.
     */
    bool IsSupportedAC3(void)
    {
        return (!InterfacePlayerRDK::IsCodecSupported("ac-3"));
    }

    /**
     * @brief Determines whether the Westeros sink should be used for rendering.
     * @return True if Westeros sink is to be used, false otherwise.
     */
    bool UseWesterosSink(void)
    {
        return socInterface->UseWesterosSink();
    }

    /**
     * @brief Checks if audio fragment synchronization is supported.
     * @return True if audio fragment sync is supported, false otherwise.
     */
    bool IsAudioFragmentSyncSupported(void)
    {
        return socInterface->IsAudioFragmentSyncSupported();
    }

    /**
     * @brief Enables or disables live latency correction.
     * @return True if live latency correction is enabled, false otherwise.
     */
    bool EnableLiveLatencyCorrection(void)
    {
        return socInterface->EnableLiveLatencyCorrection();
    }

    /**
     * @brief Gets the number of frames required to be queued for smooth playback.
     * @return The number of required queued frames.
     */
    int RequiredQueuedFrames(void)
    {
        return socInterface->RequiredQueuedFrames();
    }

    /**
     * @brief Enables or disables PTS (Presentation Time Stamp) restamping.
     * @return True if PTS restamping is enabled, false otherwise.
     */
    bool EnablePTSRestamp(void)
    {
        return socInterface->EnablePTSRestamp();
    }

    /**
     * @brief Resets the new segment event state.
     * @return True if the reset was successful, false otherwise.
     */
    bool ResetNewSegmentEvent(void)
    {
        return socInterface->ResetNewSegmentEvent();
    }

}