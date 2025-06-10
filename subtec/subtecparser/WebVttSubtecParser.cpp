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

#include "WebVttSubtecParser.hpp"
#include "TextStyleAttributes.h"

/**
 * @brief Constructs a WebVTTSubtecParser object.
 * 
 * Initializes the subtitle parser and sets up the communication channel for WebVTT subtitles.
 * 
 * @param type   The MIME type of the subtitle.
 * @param width  Width of the subtitle rendering area.
 * @param height Height of the subtitle rendering area.
 * 
 * @throws std::runtime_error if communication initialization fails.
 */
WebVTTSubtecParser::WebVTTSubtecParser(SubtitleMimeType type, int width, int height)
    : SubtitleParser(type, width, height), m_channel(nullptr)
{
    m_channel = SubtecChannel::SubtecChannelFactory(SubtecChannel::ChannelType::WEBVTT);
    if (!m_channel->InitComms())
    {
        MW_LOG_INFO("Init failed - subtitle parsing disabled");
        throw std::runtime_error("PacketSender init failed");
    }

    MW_LOG_INFO("Sending RESET ALL");
    m_channel->SendResetAllPacket();

    int screenWidth = 1920, screenHeight = 1080;
    if (width != 0 && height != 0)
    {
        screenWidth = width;
        screenHeight = height;
    }

    m_channel->SendMutePacket();
    m_channel->SendSelectionPacket(screenWidth, screenHeight);
}

/**
 * @brief Updates the current playback timestamp.
 * 
 * @param positionMs Current playback position in milliseconds.
 */
void WebVTTSubtecParser::updateTimestamp(unsigned long long positionMs)
{
    MW_LOG_INFO("positionMs %lld", positionMs);
    m_channel->SendTimestampPacket(positionMs);
}

/**
 * @brief Resets the subtitle channel and mutes output.
 */
void WebVTTSubtecParser::reset()
{
    m_channel->SendMutePacket();
    m_channel->SendResetChannelPacket();
}

/**
 * @brief Initializes the parser with a start position and base PTS.
 * 
 * @param startPosSeconds Start position in seconds.
 * @param basePTS         Base presentation timestamp in milliseconds.
 * 
 * @return true if initialization is successful.
 */
bool WebVTTSubtecParser::init(double startPosSeconds, unsigned long long basePTS)
{
    MW_LOG_INFO("startPos %f basePTS %lld", startPosSeconds, basePTS);

    m_channel->SendTimestampPacket(static_cast<uint64_t>(startPosSeconds * 1000.0));
    if (playerResumeTrackDownloads_CB)
    {
        playerResumeTrackDownloads_CB();
    }

    return true;
}

/**
 * @brief Processes subtitle data and sends it to the rendering channel.
 * 
 * @param buffer     Pointer to the subtitle data buffer.
 * @param bufferLen  Length of the buffer.
 * @param position   Current playback position (not used).
 * @param duration   Duration of the subtitle (not used).
 * 
 * @return true if processing is successful.
 */
bool WebVTTSubtecParser::processData(const char* buffer, size_t bufferLen, double position, double duration)
{
    std::string str(const_cast<const char*>(buffer), bufferLen);
    std::vector<uint8_t> data(str.begin(), str.end());

    m_channel->SendDataPacket(std::move(data), 0);

    return true;
}

/**
 * @brief Mutes or unmutes subtitle rendering.
 * 
 * @param mute true to mute, false to unmute.
 */
void WebVTTSubtecParser::mute(bool mute)
{
    if (mute)
        m_channel->SendMutePacket();
    else
        m_channel->SendUnmutePacket();
}

/**
 * @brief Pauses or resumes subtitle rendering.
 * 
 * @param pause true to pause, false to resume.
 */
void WebVTTSubtecParser::pause(bool pause)
{
    if (pause)
        mSendResumePacket();
}

/**
 * @brief Sets text style attributes for subtitle rendering.
 * 
 * @param options A string containing style options.
 */
void WebVTTSubtecParser::setTextStyle(const std::string &options)
{
    TextStyleAttributes textStyleAttributes;
    uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    int ccType = 0; // Value not used by WebVTT

    if (!textStyleAttributes.getAttributes(options, attributesValues, attributesMask))
    {
        if (attributesMask)
        {
            m_channel->SendCCSetAttributePacket(ccType, attributesMask, attributesValues);
        }
    }
}
