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

#include "SubtecPacket.hpp"
#include "SubtecChannel.hpp"
#include "PacketSender.hpp"


class ClosedCaptionsPacket : public Packet
{
public:
   /**
	* @brief Constructs a Closed Captions data packet with PTS information.
	*
	* This constructor initializes and populates a Closed Captions (CC) data packet.
	* It appends header information such as packet type, counter, channel details,
	* and Presentation Timestamp (PTS), followed by the actual CC data bytes.
	*
	* @param[in] channelId  The identifier of the Closed Captions channel.
	* @param[in] counter    The packet sequence counter for tracking transmission order.
	* @param[in] ptsValue   The Presentation Timestamp (PTS) value associated with the packet.
	* @param[in] data       Pointer to the Closed Captions data buffer.
	* @param[in] dataLen    Length of the Closed Captions data buffer in bytes.
	*
	* @note This constructor includes PTS information in the generated packet
	*       (indicated by the fixed value `1` for PTS presence).
	*       The function assumes that the input data pointer is valid and accessible
	*       for the duration of the packet construction.
	*
	* @return A fully constructed Closed Captions data packet ready for transmission or processing.
    */
    ClosedCaptionsPacket(uint32_t channelId, uint32_t counter, uint32_t ptsValue, uint8_t* data, size_t dataLen)
    {
        appendType(Packet::PacketType::CC_DATA);
        append32(counter);
        append32( (uint32_t)(CC_DATA_HEADER_LEN+dataLen) );
        append32(channelId);
        append32(CC_CHANNEL_TYPE);
        append32(1); // pts presence
        append32(ptsValue);
        for(int i = 0; i<dataLen; ++i)
            getBuffer().push_back(data[i]);
    }

        
    /**
     * @brief Constructs a Closed Captions data packet without PTS information.
     *
     * This constructor initializes and builds a Closed Captions (CC) data packet
     * that does not include Presentation Timestamp (PTS) information. It appends
     * the required header fields such as packet type, counter, and channel details,
     * followed by the Closed Captions data bytes.
     *
     * @param[in] channelId  The identifier of the Closed Captions channel.
     * @param[in] counter    The packet sequence counter used for tracking order.
     * @param[in] data       Pointer to the Closed Captions data buffer.
     * @param[in] dataLen    Length of the Closed Captions data buffer in bytes.
     *
     * @note This constructor explicitly sets PTS presence to `0` and uses a
     *       placeholder PTS value of `0`, indicating that the packet does not
     *       contain timing information.
     *       The input data pointer must be valid and accessible during packet construction.
     *
     * @return A fully constructed Closed Captions data packet without PTS, ready for transmission or processing.
     */
    ClosedCaptionsPacket(uint32_t channelId, uint32_t counter, uint8_t* data, size_t dataLen)
    {
        appendType(Packet::PacketType::CC_DATA);
        append32(counter);
        append32( (uint32_t)(CC_DATA_HEADER_LEN+dataLen) );
        append32(channelId);
        append32(CC_CHANNEL_TYPE);
        append32(0); // pts presence
        append32(0); // no pts value
        for(int i = 0; i<dataLen; ++i)
            getBuffer().push_back(data[i]);
    }

private:
    static constexpr std::uint8_t CC_DATA_HEADER_LEN = 16;
    static constexpr std::uint8_t CC_CHANNEL_TYPE = 3;
};

class ClosedCaptionsActiveTypePacket: public Packet
{
public:
    enum class CEA : uint32_t
    {
        type_608 = 0,
        type_708 = 1
    };
    /**
    * @brief Constructs a Closed Captions Active Type packet.
    *
    * This constructor creates and initializes a Closed Captions (CC) Active Type packet,
    * which specifies the active CEA (Closed Captioning standard) type and service number.
    * The packet includes metadata such as packet type, counter, and channel information,
    * followed by auxiliary identifiers representing the caption type and service.
    *
    * @param[in] channelId  The identifier of the Closed Captions channel.
    * @param[in] counter    The packet sequence counter used for tracking transmission order.
    * @param[in] type       The Closed Captions standard type (e.g., CEA-608, CEA-708).
    * @param[in] service    The service number associated with the caption type.
    *
    * @note The function converts the provided CEA type to its corresponding
    *       integral representation before appending it to the packet.
    *
    * @return A fully constructed Closed Captions Active Type packet ready for transmission or configuration.
    */
    ClosedCaptionsActiveTypePacket(uint32_t channelId, uint32_t counter, CEA type, int service)
    {
        auto to_integral = [](CEA e) -> uint32_t
        {
            return static_cast<uint32_t>(e);
        };

        appendType(Packet::PacketType::SUBTITLE_SELECTION);
        append32(counter);
        append32(CC_SELECTION_LEN);
        append32(channelId);
        append32(CC_USERDATA_SUBTITLE_TYPE);
        append32(to_integral(type)); //aux id 1
        append32(service);//aux id 2


    }


private:
    static constexpr std::uint8_t CC_SELECTION_LEN = 16;
    static constexpr std::uint8_t CC_USERDATA_SUBTITLE_TYPE = 3;
};

class ClosedCaptionsChannel : public SubtecChannel
{
public:
    ClosedCaptionsChannel() {}
    
    /**
    * @brief Sends a data packet with the specified Presentation Timestamp (PTS).
    *
    * This function transmits a data packet along with an associated PTS value,
    * which can be used for synchronizing playback or timing operations.
    *
    * @param[in] ptsValue  The Presentation Timestamp (PTS) value in milliseconds (or relevant time unit).
    * @param[in] data      Pointer to the buffer containing the data to be sent.
    * @param[in] dataLen   Length of the data buffer in bytes.
    *
    * @note The function assumes that the provided data buffer is valid and accessible
    *       for the duration of the send operation.
    *
    * @return None.
    */
    void SendDataPacketWithPTS(uint32_t ptsValue, uint8_t* data, size_t dataLen)
    {
        std::unique_lock<std::mutex> lock(mChannelMtx);
        PacketSender::Instance()->SendPacket(std::unique_ptr<ClosedCaptionsPacket>(new ClosedCaptionsPacket(m_channelId, m_counter++, ptsValue, data, dataLen)));
    }
     
   /**
    * @brief S
    ends a data packet without a Presentation Timestamp (PTS).
    *
    * This function transmits a data packet that does not include an associated
    * PTS value. It is typically used for sending data streams or packets
    * where timing or synchronization information is not required.
    *
    * @param[in] data     Pointer to the buffer containing the data to be sent.
    * @param[in] dataLen  Length of the data buffer in bytes.
    *
    * @note The function assumes that the provided data buffer is valid and
    *       accessible for the duration of the send operation.
    *
    * @return None.
    */
    void SendDataPacketNoPTS(uint8_t* data, size_t dataLen)
    {
        std::unique_lock<std::mutex> lock(mChannelMtx);
        PacketSender::Instance()->SendPacket(std::unique_ptr<ClosedCaptionsPacket>(new ClosedCaptionsPacket(m_channelId, m_counter++, data, dataLen)));
    }
    /**
    * @brief Sends an active type packet for Closed Captions.
    *
    * This function sends a Closed Captions Active Type packet that specifies
    * the caption format (CEA type) and the channel to which it belongs.
    * It is used to indicate or configure the active caption type being transmitted.
    *
    * @param[in] type     The Closed Captions active type (e.g., CEA-608, CEA-708).
    * @param[in] channel  The channel number associated with the active caption type.
    *
    * @note Ensure that the specified caption type and channel are valid before calling this function.
    *
    * @return None.
    */
    void SendActiveTypePacket(ClosedCaptionsActiveTypePacket::CEA type, unsigned int channel)
    {
        std::unique_lock<std::mutex> lock(mChannelMtx);
        PacketSender::Instance()->SendPacket(std::unique_ptr<ClosedCaptionsActiveTypePacket>(new ClosedCaptionsActiveTypePacket(m_channelId, m_counter++, type, channel)));
    }
};
