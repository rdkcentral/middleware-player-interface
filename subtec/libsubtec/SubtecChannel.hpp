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

#include <memory>
#include <vector>
#include <mutex>
#include <algorithm>
#include "SubtecAttribute.hpp"

namespace player_utils
{
    template<typename T, typename ...Args>
    std::unique_ptr<T> make_unique(Args&& ...args)
    {
        return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    }
}

class SubtecChannelManager
{
public:
    /**
    * @brief Returns the singleton instance of the SubtecChannelManager.
    *
    * This static method provides access to a single shared instance of
    * the SubtecChannelManager. It ensures that only one instance exists
    * throughout the application's lifecycle.
    *
    * @return Pointer to the SubtecChannelManager instance.
    */
    static SubtecChannelManager *getInstance()
    {
        static SubtecChannelManager instance;
        return &instance;
    }
    /**
    * @brief Retrieves the next available channel ID.
    *
    * This function returns the current channel ID and then increments
    * the internal counter for future allocations. It is typically used
    * to assign unique identifiers to new channels.
    *
    * @return The next available channel ID.
    */
    int getNextChannelId() { return m_nextChannelId++; }
protected:
    SubtecChannelManager() :  m_nextChannelId(0) {}
private:
    uint32_t m_nextChannelId;
};

class SubtecChannel
{

protected:
    template<typename PacketType, typename ...Args>
    void sendPacket(Args && ...args);
public:
    enum class ChannelType
    {
        TTML,
        WEBVTT,
        CC
    };

    SubtecChannel() : m_counter(0), m_channelId(0), mChannelMtx()
    {
        m_channelId = SubtecChannelManager::getInstance()->getNextChannelId();
    }

    static std::unique_ptr<SubtecChannel> SubtecChannelFactory(ChannelType type);
    /**
    * @brief Initializes communication with default settings.
    *
    * This static method initializes the communication interface used for
    * sending and receiving packets. It sets up necessary resources and
    * prepares the communication channel for operation.
    *
    * @return True if initialization succeeds, false otherwise.
    */
    static bool InitComms();
    /**
    * @brief Initializes communication using a specified socket path.
    *
    * This overload of InitComms() sets up the communication interface using
    * the provided UNIX domain socket path for packet transmission.
    *
    * @param[in] socket_path  Path to the socket used for communication.
    *
    * @return True if initialization succeeds, false otherwise.
    */
    static bool InitComms(const char* socket_path);
    /**
    * @brief Sends a packet to reset all Closed Captions channels.
    *
    * This function transmits a control packet instructing all active
    * Closed Captions channels to reset their state.
    *
    * @return None.
    */
    void SendResetAllPacket();
    /**
    * @brief Sends a packet to reset the current Closed Captions channel.
    *
    * This function transmits a control packet that resets the specific
    * channel currently in use.
    *
    * @return None.
    */
    void SendResetChannelPacket();
    /**
    * @brief Sends a pause control packet.
    *
    * This function transmits a control packet instructing the receiver
    * to pause Closed Captions data processing or playback.
    *
    * @return None.
    */
    void SendPausePacket();
    /**
     * @brief Sends a resume control packet.
     *
     * This function transmits a control packet that resumes Closed Captions
     * data processing or playback after a pause.
     *
     * @return None.
     */
    void SendResumePacket();
    /**
    * @brief Sends a mute control packet.
    *
    * This function transmits a control packet instructing the receiver
    * to mute Closed Captions output.
    *
    * @return None.
    */
    void SendMutePacket();
    /**
    * @brief Sends an unmute control packet.
    *
    * This function transmits a control packet instructing the receiver
    * to restore Closed Captions output after being muted.
    *
    * @return None.
    */
    void SendUnmutePacket();
    /**
    * @brief Sends a packet to set Closed Captions attributes.
    *
    * This function transmits a packet containing attribute configuration
    * for a specific Closed Captions type. Attributes may include visual
    * properties such as font size, position, or color.
    *
    * @param[in] ccType           Closed Captions type identifier (e.g., CEA-608, CEA-708).
    * @param[in] attribType       Attribute type identifier.
    * @param[in] attributesValues Structure containing the attribute values to be applied.
    *
    * @return None.
    */
    void SendCCSetAttributePacket(std::uint32_t ccType, std::uint32_t attribType, const attributesType &attributesValues);
    /**
    * @brief Sends a packet with selection parameters.
    *
    * This virtual method transmits a selection packet containing display
    * parameters such as width and height. Derived classes may override
    * this function to implement specific behavior.
    *
    * @param[in] width   Display width in pixels.
    * @param[in] height  Display height in pixels.
    *
    * @return None.
    */
    virtual void SendSelectionPacket(uint32_t width, uint32_t height){};
    /**
    * @brief Sends a Closed Captions data packet.
    *
    * This virtual method transmits a data packet containing Closed Captions
    * content. The optional time offset can be used to align playback timing.
    *
    * @param[in] data             The Closed Captions data to send.
    * @param[in] time_offset_ms   Optional time offset in milliseconds (default: 0).
    *
    * @return None.
    */
    virtual void SendDataPacket(std::vector<uint8_t> &&data, std::int64_t time_offset_ms = 0){};
    /**
    * @brief Sends a packet containing a timestamp.
    *
    * This virtual method transmits a timestamp packet used for synchronization
    * or timing alignment in Closed Captions playback.
    *
    * @param[in] timestampMs  The timestamp in milliseconds.
    *
    * @return None.
    */
    virtual void SendTimestampPacket(uint64_t timestampMs){};

    virtual ~SubtecChannel() = 0;
    
protected:
    uint32_t m_channelId;
    uint32_t m_counter;
    std::mutex mChannelMtx;
};
