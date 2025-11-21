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
 * @file CCDataController.h
 *
 * @brief Impl of subtec communication layer
 *
 */

#ifndef __CC_DATA_CONTROLLER_H__
#define __CC_DATA_CONTROLLER_H__

#include <ClosedCaptionsPacket.hpp>

#include "ccDataReader.h"

#include "SubtecConnector.h"

/**
 * @brief Print logs to console / log file
 * @param[in] format - printf style string
 * @return void
 */
extern void logprintf(int playerId,const char* levelstr,const char* file, int line,const char *format, ...);

namespace subtecConnector
{

/**
 * @brief Controller for CCdata
 */
class CCDataController
{
public:
    /**
     * @brief Get the singleton instance of CCDataController.
     *
     * @return Pointer to the CCDataController instance.
     */
    static CCDataController* Instance();

    /**
     * @brief Callback for receiving closed caption data.
     *
     * @param decoderIndex Index of the decoder.
     * @param eType Closed caption data type.
     * @param ccData Pointer to closed caption data buffer.
     * @param dataLength Length of the closed caption data.
     * @param sequenceNumber Sequence number of the data packet.
     * @param localPts Local presentation timestamp.
     */
    void closedCaptionDataCb (int decoderIndex, VL_CC_DATA_TYPE eType, unsigned char* ccData,
                                    unsigned dataLength, int sequenceNumber, long long localPts);

    /**
     * @brief Callback for closed caption decode events.
     *
     * @param decoderIndex Index of the decoder.
     * @param event Event code.
     */
    void closedCaptionDecodeCb(int decoderIndex, int event);

    /**
     * @brief Send mute command for closed captions.
     */
    void sendMute();

    /**
     * @brief Send unmute command for closed captions.
     */
    void sendUnmute();

    /**
     * @brief Send pause command for closed captions.
     */
    void sendPause();

    /**
     * @brief Send resume command for closed captions.
     */
    void sendResume();

    /**
     * @brief Send reset channel packet for closed captions.
     */
    void sendResetChannelPacket();

    /**
     * @brief Send command to set closed caption attributes.
     *
     * @param attrib Pointer to attributes structure.
     * @param type Attribute type.
     * @param ccType Closed caption type.
     */
    void sendCCSetAttribute(gsw_CcAttributes * attrib, short type, gsw_CcType ccType);

    /**
     * @brief Set the digital closed caption channel.
     *
     * @param channel Channel number.
     */
    void ccSetDigitalChannel(unsigned int channel);

    /**
     * @brief Set the analog closed caption channel.
     *
     * @param channel Channel number.
     */
    void ccSetAnalogChannel(unsigned int channel);

    /**
     * @brief Get closed caption attributes.
     *
     * @param attrib Pointer to attributes structure to fill.
     * @param ccType Closed caption type.
     */
    void ccGetAttributes(gsw_CcAttributes * attrib, gsw_CcType ccType);

private:
    /**
     * @brief Default constructor for CCDataController.
     */
    CCDataController();

    /**
     * @brief Deleted copy constructor to prevent copying.
     */
    CCDataController(const CCDataController&) = delete;

    /**
     * @brief Deleted move constructor to prevent moving.
     */
    CCDataController(CCDataController&&) = delete;

    /**
     * @brief Closed captions channel handler.
     */
    ClosedCaptionsChannel channel;

    /**
     * @brief Stores the current closed caption attributes.
     */
    gsw_CcAttributes currentAttributes;
};

/**
 * @brief Callback for closed caption decode events (C-style).
 *
 * @param context Pointer to user context.
 * @param decoderIndex Index of the decoder.
 * @param event Event code.
 */
void closedCaptionDecodeCb(void *context, int decoderIndex, int event);

/**
 * @brief Callback for receiving closed caption data (C-style).
 *
 * @param context Pointer to user context.
 * @param decoderIndex Index of the decoder.
 * @param eType Closed caption data type.
 * @param ccData Pointer to closed caption data buffer.
 * @param dataLength Length of the closed caption data.
 * @param sequenceNumber Sequence number of the data packet.
 * @param localPts Local presentation timestamp.
 */
void closedCaptionDataCb (void *context, int decoderIndex, VL_CC_DATA_TYPE eType, unsigned char* ccData,
                                unsigned dataLength, int sequenceNumber, long long localPts);


} // subtecConnector

#endif //__CC_DATA_CONTROLLER_H__
