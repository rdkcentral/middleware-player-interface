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
 * @file WebVTTSubtecParser.hpp
 * 
 * @brief Defines the WebVTTSubtecParser class, which handles parsing and rendering
 *        of WebVTT subtitle data using the SubtecChannel interface.
 */

#pragma once

#include "subtitleParser.h"
#include "SubtecChannel.hpp"

/**
 * @class WebVTTSubtecParser
 * @brief A parser for WebVTT subtitles that interfaces with SubtecChannel for rendering.
 *
 * This class extends SubtitleParser and provides implementations for initializing,
 * processing, and controlling subtitle playback, including mute, pause, and style settings.
 */
class WebVTTSubtecParser : public SubtitleParser
{
public:
    /**
     * @brief Constructs a WebVTTSubtecParser with the specified subtitle type and dimensions.
     * 
     * @param type   MIME type of the subtitle.
     * @param width  Width of the subtitle rendering area.
     * @param height Height of the subtitle rendering area.
     */
    WebVTTSubtecParser(SubtitleMimeType type, int width, int height);

    /// Deleted copy constructor.
    WebVTTSubtecParser(const WebVTTSubtecParser&) = delete;

    /// Deleted copy assignment operator.
    WebVTTSubtecParser& operator=(const WebVTTSubtecParser&) = delete;

    /**
     * @brief Initializes the parser with a start position and base PTS.
     * 
     * @param startPosSeconds Start position in seconds.
     * @param basePTS         Base presentation timestamp in milliseconds.
     * @return true if initialization is successful.
     */
    bool init(double startPosSeconds, unsigned long long basePTS) override;

    /**
     * @brief Processes subtitle data.
     * 
     * @param buffer     Pointer to the subtitle data buffer.
     * @param bufferLen  Length of the buffer.
     * @param position   Current playback position.
     * @param duration   Duration of the subtitle.
     * @return true if processing is successful.
     */
    bool processData(const char* buffer, size_t bufferLen, double position, double duration) override;

    /**
     * @brief Closes the parser.
     * 
     * @return true if closed successfully.
     */
    bool close() override { return true; }

    /**
     * @brief Resets the parser state.
     */
    void reset() override;

    /**
     * @brief Sets the progress event offset (not used in WebVTT).
     * 
     * @param offset Offset in seconds.
     */
    void setProgressEventOffset(double offset) override {}

    /**
     * @brief Updates the current playback timestamp.
     * 
     * @param positionMs Current playback position in milliseconds.
     */
    void updateTimestamp(unsigned long long positionMs) override;

    /**
     * @brief Pauses or resumes subtitle rendering.
     * 
     * @param pause true to pause, false to resume.
     */
    void pause(bool pause) override;

    /**
     * @brief Mutes or unmutes subtitle rendering.
     * 
     * @param mute true to mute, false to unmute.
     */
    void mute(bool mute) override;

    /**
     * @brief Sets text style attributes for subtitle rendering.
     * 
     * @param options A string containing style options.
     */
    void setTextStyle(const std::string &options) override;

protected:
    /// Pointer to the subtitle rendering channel.
    std::unique_ptr<SubtecChannel> m_channel;

private:
    /// Time offset in milliseconds.
    std::uint64_t time_offset_ms_ = 0;

    /// Start time in milliseconds.
    std::uint64_t start_ms_ = 0;
};
