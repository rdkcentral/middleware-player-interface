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
 * @file vttCue.h
 *
 * @brief Provides data structure to hold a WebVTT cue data
 *
 */

#ifndef __VTT_CUE_H__
#define __VTT_CUE_H__

#include <string>

/**
 * @struct VTTCue
 * @brief Represents a single WebVTT subtitle cue.
 * 
 * A VTTCue contains the timing, text, and optional settings for a subtitle entry.
 */
struct VTTCue
{
    /**
     * @brief Constructs a VTTCue with the given parameters.
     * 
     * @param startTime Start time of the cue in seconds.
     * @param duration Duration of the cue in seconds.
     * @param text Subtitle text content.
     * @param settings Optional cue settings (e.g., position, alignment).
     */
    VTTCue(double startTime, double duration, std::string text, std::string settings)
        : mStart(startTime), mDuration(duration), mText(text), mSettings(settings) {}

    /**
     * @brief Start time of the cue in seconds.
     */
    double mStart;

    /**
     * @brief Duration of the cue in seconds.
     */
    double mDuration;

    /**
     * @brief Text content of the subtitle cue.
     */
    std::string mText;

    /**
     * @brief Optional settings string for cue rendering (e.g., alignment, position).
     */
    std::string mSettings;
};
#endif /* __VTT_CUE_H__ */
