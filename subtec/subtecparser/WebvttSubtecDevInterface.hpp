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
 * @file  WebvttSubtecDevInterface.hpp
 * 
 * @brief This file provides interfaces for a subtitle parser in the Player.
 *        It defines the WebVTT subtitle device interface used to manage subtitle
 *        rendering, cue data handling, and playback state updates.
 */

#ifndef __WEBVTT_SUBTEC_DEV_INTERFACE_HPP__
#define __WEBVTT_SUBTEC_DEV_INTERFACE_HPP__

#include "SubtecChannel.hpp"

/**
 * @class WebvttSubtecDevInterface
 * @brief Interface class for handling WebVTT subtitle rendering and control.
 *
 * This class provides methods to initialize subtitle rendering, send cue data,
 * update playback timestamps, and control playback states such as pause and mute.
 */
class WebvttSubtecDevInterface
{
public:
    /**
     * @brief Constructor to initialize the subtitle interface with display dimensions.
     * 
     * @param width  Width of the subtitle rendering area.
     * @param height Height of the subtitle rendering area.
     */
    WebvttSubtecDevInterface(int width, int height);

    /**
     * @brief Destructor.
     */
    ~WebvttSubtecDevInterface(){}

    /**
     * @brief Initializes the subtitle interface with a base presentation timestamp.
     * 
     * @param basePTS Base presentation timestamp in milliseconds.
     * @return true if initialization is successful, false otherwise.
     */
    bool init(unsigned long long basePTS);

    /**
     * @brief Resets the subtitle interface, clearing any internal state.
     */
    void reset();

    /**
     * @brief Sends cue data in TTML format to the subtitle renderer.
     * 
     * @param ttml TTML-formatted subtitle cue data.
     */
    void sendCueData(const std::string& ttml);

    /**
     * @brief Updates the current playback timestamp.
     * 
     * @param positionMs Current playback position in milliseconds.
     */
    void updateTimestamp(unsigned long long positionMs);

    /**
     * @brief Pauses or resumes subtitle rendering.
     * 
     * @param pause true to pause, false to resume.
     */
    void pause(bool pause);

    /**
     * @brief Mutes or unmutes subtitle rendering.
     * 
     * @param mute true to mute, false to unmute.
     */
    void mute(bool mute);

protected:
    /**
     * @brief Pointer to the subtitle rendering channel.
     */
    std::unique_ptr<SubtecChannel> m_channel;
};

#endif /* __WEBVTT_SUBTEC_DEV_INTERFACE_HPP__ */
