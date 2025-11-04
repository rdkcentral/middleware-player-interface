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
    * @brief Retrieves the singleton instance of the CCDataController.
    *
    * This method returns the global instance of the CCDataController class.
    * If the instance does not exist, it will be created.
    *
    * @return Pointer to the CCDataController singleton instance.
    */
    static CCDataController* Instance();
    /**
     * @brief Callback function to handle closed caption data received from the decoder.
     *
     * This function is triggered when new closed caption data is available for processing.
     *
     * @param decoderIndex   Index of the decoder that generated the caption data.
     * @param eType          Type of closed caption data (e.g., CEA-608, CEA-708).
     * @param ccData         Pointer to the raw closed caption data buffer.
     * @param dataLength     Length of the closed caption data in bytes.
     * @param sequenceNumber Sequence number used to identify caption data order.
     * @param localPts       Local presentation timestamp associated with the caption data.
     */
    void closedCaptionDataCb (int decoderIndex, VL_CC_DATA_TYPE eType, unsigned char* ccData,
                                    unsigned dataLength, int sequenceNumber, long long localPts);
    /**
    * @brief Callback function to handle closed caption decoder events.
    *
    * This function is called when a closed caption decoder event occurs,
    * such as start, stop, or error notifications.
    *
    * @param decoderIndex Index of the decoder generating the event.
    * @param event        Event identifier or type.
    */
    void closedCaptionDecodeCb(int decoderIndex, int event);
    /**
    * @brief Sends a mute command for closed captions.
    *
    * This function notifies the system to mute closed caption output.
    */
    void sendMute();
    /**
    * @brief Sends an unmute command for closed captions.
    *
    * This function notifies the system to resume closed caption output.
    */
    void sendUnmute();
    /**
    * @brief Sends a pause command for closed captions.
    *
    * This function pauses closed caption rendering or decoding.
    */
    void sendPause();
    /**
    * @brief Sends a resume command for closed captions.
    *
    * This function resumes closed caption rendering or decoding after a pause.
    */
    void sendResume();
    /**
    * @brief Sends a reset packet to reset the closed caption channel.
    *
    * This function resets all caption-related parameters and clears buffers.
    */
    void sendResetChannelPacket();
    /**
    * @brief Sends a command to set closed caption display attributes.
    *
    * @param attrib  Pointer to the structure containing caption display attributes.
    * @param type    Attribute type identifier.
    * @param ccType  Closed caption type (e.g., analog or digital).
    */
    void sendCCSetAttribute(gsw_CcAttributes * attrib, short type, gsw_CcType ccType);
    /**
    * @brief Sets the current digital closed caption channel.
    *
    * @param channel Digital channel number to be set for closed captions.
    */
    void ccSetDigitalChannel(unsigned int channel);
    /**
    * @brief Sets the current analog closed caption channel.
    *
    * @param channel Analog channel number to be set for closed captions.
    */
    void ccSetAnalogChannel(unsigned int channel);
    /**
    * @brief Retrieves the current closed caption display attributes.
    *
    * @param attrib  Pointer to the structure that will store the current attributes.
    * @param ccType  Closed caption type (e.g., analog or digital).
    */
    void ccGetAttributes(gsw_CcAttributes * attrib, gsw_CcType ccType);

private:
    CCDataController();
    CCDataController(const CCDataController&) = delete;
    CCDataController(CCDataController&&) = delete;
    ClosedCaptionsChannel channel;

    gsw_CcAttributes currentAttributes;
};
/**
 * @brief Callback function to handle closed caption decoder events.
 *
 * This function is invoked when a closed caption decoder event occurs,
 * such as initialization, data availability, error, or stop notification.
 *
 * @param context       Pointer to the user-defined context or instance data.
 * @param decoderIndex  Index of the decoder generating the event.
 * @param event         Event identifier or type (e.g., start, stop, error).
 */
void closedCaptionDecodeCb(void *context, int decoderIndex, int event);
/**
 * @brief Callback function to handle closed caption data received from the decoder.
 *
 * This function is triggered when new closed caption data is available for processing.
 * It extracts and processes caption information for display or further decoding.
 *
 * @param context         Pointer to the user-defined context or instance data.
 * @param decoderIndex    Index of the decoder that generated the caption data.
 * @param eType           Type of closed caption data (e.g., CEA-608, CEA-708).
 * @param ccData          Pointer to the raw closed caption data buffer.
 * @param dataLength      Length of the closed caption data in bytes.
 * @param sequenceNumber  Sequence number used to maintain caption data order.
 * @param localPts        Local presentation timestamp associated with the caption data.
 */
void closedCaptionDataCb (void *context, int decoderIndex, VL_CC_DATA_TYPE eType, unsigned char* ccData,
                                unsigned dataLength, int sequenceNumber, long long localPts);


} // subtecConnector

#endif //__CC_DATA_CONTROLLER_H__
