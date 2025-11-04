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

#pragma once

#include <string>
#include <cstdint>
#include <memory>
#include <vector>
#include <array>
#include <limits>
#include <mutex>
#include "SubtecAttribute.hpp"

class Packet
{
public:
    Packet() : m_buffer(), m_counter(std::numeric_limits<std::uint32_t>::max()) {}
    Packet(std::uint32_t counter) : m_buffer(), m_counter(counter) {}
    /**
    * @brief Retrieves the packet type from the internal buffer.
    *
    * This function reads the first 4 bytes of the packet buffer and
    * computes the packet type as a 32-bit unsigned integer. If the buffer
    * contains fewer than 4 bytes, the function returns 0.
    *
    * @note The type is calculated by combining the first four bytes of the buffer
    *       using little-endian byte order.
    *
    * @return The packet type as a `uint32_t`. Returns 0 if the buffer is too small.
    */
    const uint32_t getType()
    {
        uint32_t type = 0;

        if (getBuffer().size() >= 4)
        {
            std::vector<std::uint8_t> buffer = getBuffer();
            for (int i = 0; i < 4; i++)
            {
                type += (buffer[i] << (i*8)) & 0xFF;
            }
        }
        return type;
    }
    /**
    * @brief Returns the internal buffer of the packet.
    *
    * This function provides read-only access to the packet's internal
    * byte buffer. The returned reference allows inspection of the packet
    * data without making a copy.
    *
    * @return A constant reference to the internal `std::vector<uint8_t>` buffer.
    */
    const std::vector<uint8_t>& getBytes()
    {
        return m_buffer;
    }
    /**
    * @brief Retrieves the packet counter.
    *
    * This function returns the sequence counter associated with the packet,
    * which is typically used to track the order of packet transmission
    * or reception.
    *
    * @return The packet counter as a `std::uint32_t`.
    */
    const std::uint32_t getCounter()
    {
        return m_counter;
    }
    /**
    * @brief Converts a packet type value to a human-readable string.
    *
    * This static function maps a numeric packet type identifier to its
    * corresponding descriptive string. It is useful for logging, debugging,
    * or displaying packet type information in a readable format.
    *
    * @param[in] type  The packet type value as a `uint32_t`.
    *
    * @return A `std::string` representing the name or description of the packet type.
    */
    static std::string getTypeString(uint32_t type)
    {
        std::string ret;
        PacketType pktType = static_cast<PacketType>(type);

        switch(pktType)
        {
        case PacketType::PES_DATA:
            ret = "PES_DATA";
            break;
        case PacketType::TIMESTAMP:
            ret = "TIMESTAMP";
            break;
        case PacketType::RESET_ALL:
            ret = "RESET_ALL";
            break;
        case PacketType::RESET_CHANNEL:
            ret = "RESET_CHANNEL";
            break;
        case PacketType::SUBTITLE_SELECTION:
            ret = "SUBTITLE_SELECTION";
            break;
        case PacketType::TELETEXT_SELECTION:
            ret = "TELETEXT_SELECTION";
            break;
        case PacketType::TTML_SELECTION:
            ret = "TTML_SELECTION";
            break;
        case PacketType::TTML_DATA:
            ret = "TTML_DATA";
            break;
        case PacketType::TTML_TIMESTAMP:
            ret = "TTML_TIMESTAMP";
            break;
        case PacketType::WEBVTT_SELECTION:
            ret = "WEBVTT_SELECTION";
            break;
        case PacketType::WEBVTT_DATA:
            ret = "WEBVTT_DATA";
            break;
        case PacketType::WEBVTT_TIMESTAMP:
            ret = "WEBVTT_TIMESTAMP";
            break;
        case PacketType::CC_DATA :
            ret = "CC_DATA";
            break;
        case PacketType::PAUSE :
            ret = "PAUSE";
            break;
        case PacketType::RESUME :
            ret = "RESUME";
            break;
        case PacketType::MUTE :
            ret = "MUTE";
            break;
        case PacketType::UNMUTE :
            ret = "UNMUTE";
            break;
        case PacketType::CC_SET_ATTRIBUTE:
            ret = "CC_SET_ATTRIBUTE";
            break;
        case PacketType::INVALID:
            ret = "INVALID";
            break;
        default:
            ret = "UNKNOWN";
            break;
        }

        return ret;
    }


protected:
    std::vector<uint8_t>& getBuffer() { return m_buffer; }

    enum class PacketType : std::uint32_t
    {
        ZERO,
        PES_DATA,
        TIMESTAMP,
        RESET_ALL,
        RESET_CHANNEL,
        SUBTITLE_SELECTION,
        TELETEXT_SELECTION,
        TTML_SELECTION,
        TTML_DATA,
        TTML_TIMESTAMP,
        CC_DATA,
        PAUSE,
        RESUME,
        MUTE,
        UNMUTE,
        WEBVTT_SELECTION,
        WEBVTT_DATA,
        WEBVTT_TIMESTAMP,
        CC_SET_ATTRIBUTE,

        INVALID = 0xFFFFFFFF,
    };

    std::vector<uint8_t> m_buffer;
    std::uint32_t m_counter;
    /**
    * @brief Appends a 32-bit unsigned integer to the internal buffer in little-endian order.
    *
    * This function splits the given 32-bit value into four bytes and appends them
    * sequentially to the packet's internal byte buffer (`m_buffer`). The least
    * significant byte is added first, followed by more significant bytes.
    *
    * @param[in] value  The 32-bit unsigned integer to append.
    *
    * @return None.
    */
    void append32(std::uint32_t value)
    {
        m_buffer.push_back((static_cast<std::uint8_t>((value >> 0)) & 0xFF));
        m_buffer.push_back((static_cast<std::uint8_t>((value >> 8)) & 0xFF));
        m_buffer.push_back((static_cast<std::uint8_t>((value >> 16)) & 0xFF));
        m_buffer.push_back((static_cast<std::uint8_t>((value >> 24)) & 0xFF));
    }
    /**
    * @brief Appends a 64-bit signed integer to the internal buffer in little-endian order.
    *
    * This function splits the given 64-bit value into two 32-bit halves and appends
    * them sequentially to the packet's internal buffer (`m_buffer`) using `append32()`.
    * The least significant 32 bits are appended first, followed by the most significant 32 bits.
    *
    * @param[in] value  The 64-bit signed integer to append.
    *
    * @return None.
    */
    void append64(std::int64_t value)
    {
        append32((static_cast<std::int32_t>((value >> 0)) & 0xFFFFFFFF));
        append32((static_cast<std::int32_t>((value >> 32)) & 0xFFFFFFFF));
    }
    /**
    * @brief Appends a packet type to the internal buffer.
    *
    * This function converts the `PacketType` enum to its underlying integral
    * representation and appends it to the packet's internal buffer (`m_buffer`)
    * using `append32()`.
    *
    * @param[in] type  The packet type to append, represented as a `PacketType` enum.
    *
    * @return None.
    */
    void appendType(PacketType type)
    {
        append32(static_cast<std::underlying_type<PacketType>::type>(type));
    }
};

using PacketPtr = std::unique_ptr<Packet>;

class DummyPacket : public Packet
{
public:
    DummyPacket() : Packet() {}
};

/**
 * Pause packet.
 */
class PausePacket : public Packet
{
public:

    /**
     * Constructor.
     *
     * @param counter
     *      Packet counter.
     */
    PausePacket(std::uint32_t channelId,
                std::uint32_t counter) : Packet(counter)
    {
        appendType(PacketType::PAUSE);
        append32(counter);
        append32(4);
        append32(channelId);
    }
};

/**
 * Resume packet.
 */
class ResumePacket : public Packet
{
public:

    /**
     * Constructor.
     *
     * @param counter
     *      Packet counter.
     */
    ResumePacket(std::uint32_t channelId,
                 std::uint32_t counter) : Packet(counter)
    {
        appendType(PacketType::RESUME);
        append32(counter);
        append32(4);
        append32(channelId);
    }
};

/**
 * Mute packet.
 */
class MutePacket : public Packet
{
public:

    /**
     * Constructor.
     *
     * @param counter
     *      Packet counter.
     */
    MutePacket(std::uint32_t channelId,
               std::uint32_t counter) : Packet(counter)
    {
        appendType(PacketType::MUTE);
        append32(counter);
        append32(4);
        append32(channelId);
    }
};

/**
 * Mute packet.
 */
class UnmutePacket : public Packet
{
public:

    /**
     * Constructor.
     *
     * @param counter
     *      Packet counter.
     */
    UnmutePacket(std::uint32_t channelId,
                 std::uint32_t counter) : Packet(counter)
    {
        appendType(PacketType::UNMUTE);
        append32(counter);
        append32(4);
        append32(channelId);
    }
};

/**
 * Reset all data packet.
 */

class ResetAllPacket : public Packet
{
public:

    /**
     * Constructor.
     *
     * @param counter
     *      Packet counter.
     */
    ResetAllPacket() : Packet(0)
    {
        appendType(PacketType::RESET_ALL);
        append32(0);
        append32(0);
    }
};

/**
 * Reset all data packet.
 */

class ResetChannelPacket : public Packet
{
public:

    /**
     * Constructor.
     *
     * @param counter
     *      Packet counter.
     */
    ResetChannelPacket(std::uint32_t channelId,
                       std::uint32_t counter) : Packet(counter)
    {
        appendType(PacketType::RESET_CHANNEL);
        append32(counter);
        append32(4);
        append32(channelId);
    }
};


/*

field           size    value       description
type            4       18          message type
counter         4       0..n
size            4       68          specifies size of transferred "data" that includes channelId, ccType, attribType and attributes payload

data:

channelId           4                   Specifies channel on which data for subtitles is transmitted.
ccType              4       {0,1}       0 - analog, 1 - digital
attribType          4                   bitmask specifying which attribs are set

attributes payload: -
1.  FONT_COLOR          4       0..n
2.  BACKGROUND_COLOR    4       0..n
3.  FONT_OPACITY        4       0..n
4.  BACKGROUND_OPACITY  4       0..n
5.  FONT_STYLE          4       0..n
6.  FONT_SIZE           4       0..n
7.  FONT_ITALIC         4       0..n
8.  FONT_UNDERLINE      4       0..n
9.  BORDER_TYPE         4       0..n
10. BORDER_COLOR        4       0..n
11. WIN_COLOR           4       0..n
12. WIN_OPACITY         4       0..n
13. EDGE_TYPE           4       0..n
14. EDGE_COLOR          4       0..n

When adding/removing an attribute to the above list, update the definition in the file SubtecAttribute.hpp

*/

class CCSetAttributePacket : public Packet
{
public:

    /**
     * Constructor.
     *
     * @param counter
     *      Packet counter.
     */
    CCSetAttributePacket(std::uint32_t channelId,
                         std::uint32_t counter,
                         std::uint32_t ccType,
                         std::uint32_t attribType,
                         const attributesType &attributesValues) : Packet(counter)
    {
        appendType(PacketType::CC_SET_ATTRIBUTE);
        append32(counter);
        append32((NUMBER_OF_ATTRIBUTES+3)*4);
        append32(channelId);
        append32(ccType);
        append32(attribType);

        for(const auto value : attributesValues)
            append32(value);
    }
};
