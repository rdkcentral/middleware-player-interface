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
 * @file TtmlSubtecParser.h
 * @brief Defines the TtmlSubtecParser class for parsing TTML subtitles and interacting with the Subtec subtitle engine.
 */

#pragma once

#include "subtitleParser.h"
#include "playerisobmffbuffer.h"
#include "SubtecChannel.hpp"

/**
 * @class TtmlSubtecParser
 * @brief A parser for TTML subtitles that interacts with the Subtec subtitle engine.
 *
 * This class provides methods to initialize, process, and control TTML subtitle rendering.
 * It extends the SubtitleParser base class and implements specific functionality for TTML subtitles.
 */
class TtmlSubtecParser : public SubtitleParser
{
public:
    /**
     * @brief Constructs a TtmlSubtecParser instance for TTML subtitle parsing.
     *
     * Initializes the parser with the specified subtitle type and screen dimensions.
     * It sets up the communication channel with the Subtec subtitle engine, sends
     * initialization packets, and optionally resumes track downloads if a callback is set.
     *
     * @param[in] type   The MIME type of the subtitle (e.g., TTML). Expected values are enum values of SubtitleMimeType.
     * @param[in] width  The width of the video screen. Expected value: positive integer, defaults to 1920 if 0.
     * @param[in] height The height of the video screen. Expected value: positive integer, defaults to 1080 if 0.
     *
     * @throws std::runtime_error if the subtitle communication channel fails to initialize.
     */
    TtmlSubtecParser(SubtitleMimeType type, int width, int height);

    /// Deleted copy constructor to prevent copying.
    TtmlSubtecParser(const TtmlSubtecParser&) = delete;

    /// Deleted assignment operator to prevent copying.
    TtmlSubtecParser& operator=(const TtmlSubtecParser&) = delete;

    /**
     * @brief Initializes the TTML subtitle parser with a starting position and base PTS.
     *
     * Sends a timestamp packet to the subtitle channel based on the provided start position
     * in seconds. Also resets internal state flags and optionally resumes track downloads
     * if a callback is registered.
     *
     * @param[in] startPosSeconds The starting playback position in seconds. Expected value: non-negative double.
     * @param[in] basePTS         The base presentation timestamp (PTS) in microseconds. Expected value: non-negative unsigned long long.
     *
     * @return true Always returns true upon successful initialization.
     */
    bool init(double startPosSeconds, unsigned long long basePTS) override;

    /**
     * @brief Processes subtitle data from a TTML stream and sends it to the subtitle rendering channel.
     *
     * This function parses the input buffer as an ISO BMFF (MP4) stream. If the buffer is not an
     * initialization segment, it extracts the `mdat` box, parses the TTML data, and sends it to
     * the subtitle rendering channel. For linear content, it also calculates and sends a time offset
     * based on the first `begin` timestamp found in the TTML data.
     *
     * @param[in] buffer     Pointer to the input data buffer. Expected value: non-null pointer to valid TTML stream data.
     * @param[in] bufferLen  Length of the input buffer in bytes. Expected value: positive size_t value.
     * @param[in] position   Current playback position in seconds. Expected value: non-negative double.
     * @param[in] duration   Duration of the current segment in seconds. Expected value: non-negative double.
     *
     * @return true Always returns true after processing.
     */
    bool processData(const char* buffer, size_t bufferLen, double position, double duration) override;


    /**
     * @brief Closes the subtitle parser.
     *
     * @return true Always returns true upon successful closure.
     */
    bool close() override { return true; }

    /**
     * @brief Resets the subtitle parser's internal channel state.
     *
     * Sends a reset packet to the subtitle channel to clear any existing subtitle data
     * and prepare for fresh input. Typically used when restarting or reinitializing playback.
     */
    void reset() override;

    /**
     * @brief Sets the progress event offset.
     *
     * @param[in] offset The offset value to set.
     */
    void setProgressEventOffset(double offset) override {}

    /**
     * @brief Updates the subtitle parser with the current playback timestamp.
     *
     * Sends a timestamp packet to the subtitle channel to synchronize subtitle rendering
     * with the current playback position.
     *
     * @param[in] positionMs The current playback position in milliseconds. Expected value: non-negative unsigned long long.
     */
    void updateTimestamp(unsigned long long positionMs) override;

    /**
     * @brief Pauses or resumes subtitle rendering.
     *
     * Sends a pause or resume packet to the subtitle rendering channel based on the input flag.
     * This is typically used to synchronize subtitle behavior with media playback state.
     *
     * @param[in] pause If true, subtitles are paused; if false, subtitles are resumed.
     */
    void pause(bool pause) override;

    /**
     * @brief Mutes or unmutes subtitle rendering.
     *
     * Sends a mute or unmute packet to the subtitle rendering channel based on the input flag.
     * This is typically used to control subtitle visibility during playback.
     *
     * @param[in] mute If true, subtitles are muted (hidden); if false, subtitles are unmuted (shown).
     */
    void mute(bool mute) override;

    /**
     * @brief Sets the linear content flag for the subtitle parser.
     *
     * @param[in] isLinear If true, the content is linear; if false, it is not.
     */
    void isLinear(bool isLinear) override { m_isLinear = isLinear; }

protected:
    std::unique_ptr<SubtecChannel> m_channel; /**< Pointer to the subtitle communication channel. */
    bool m_isLinear = false; /**< Flag indicating whether the content is linear. */
    bool m_parsedFirstPacket = false; /**< Flag indicating whether the first packet has been parsed. */
    bool m_sentOffset = false; /**< Flag indicating whether the offset has been sent. */
    double m_firstBeginOffset = 0.0; /**< Offset value for the first "begin" timestamp. */
};
