/*
 * If not stated otherwise in this file or this component's license file the
 * following copyright and licenses apply:
 *
 * Copyright 2018 RDK Management
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
 * @file  subtitleParser.h
 * 
 * @brief This file provides interfaces for a subtitle parser in Player
 *
 */

#ifndef __SUBTITLE_PARSER_H__
#define __SUBTITLE_PARSER_H__

#include <string>
#include <functional>
#include "vttCue.h"
#include "PlayerLogManager.h"
#include <glib.h>
#include <iostream>
#include <sstream>

/**
* \enum       SubtitleMimeType
* \brief      Subtitle data types
*/
typedef enum
{
	eSUB_TYPE_WEBVTT,
	eSUB_TYPE_MP4,
	eSUB_TYPE_TTML,
	eSUB_TYPE_UNKNOWN
} SubtitleMimeType;

/**
 * @brief Structure holding the player callback.
 */
struct PlayerCallbacks {
    std::function<void()> resumeTrackDownloads_CB; /**< Callback to resume track downloads */
    std::function<void()> stopTrackDownloads_CB;   /**< Callback to stop track downloads */
    std::function<void(VTTCue*)> sendVTTCueData_CB; /**< Callback to send VTT cue data */
    std::function<void(long long&, double&)> getPlayerPositions_CB; /**< Callback to get player positions */
};

/**
* \class       SubtitleParser
* \brief       Subtitle parser class
*
* This is the base class for a subtitle parser impl in Player
*/
class SubtitleParser
{
public:

	SubtitleParser(SubtitleMimeType type, int width, int height) : mType(type),
				mHeight(height), mWidth(width)
	{
	}

	/// Copy Constructor
	SubtitleParser(const SubtitleParser&) = delete;

	virtual ~SubtitleParser()
	{
		UnRegisterCallback();
	}

	/// Assignment operator Overloading
	SubtitleParser& operator=(const SubtitleParser&) = delete;

	/**
	 * @brief Initializes the subtitle parser.
	 * 
	 * @param startPosSeconds Start position in seconds.
	 * @param basePTS Base presentation timestamp in milliseconds.
	 * @return true if initialization is successful, false otherwise.
	 */
	virtual bool init(double startPosSeconds, unsigned long long basePTS) { return false; }

	/**
	 * @brief Processes subtitle data.
	 * 
	 * @param buffer Pointer to the subtitle data buffer.
	 * @param bufferLen Length of the buffer.
	 * @param position Current playback position in seconds.
	 * @param duration Duration of the subtitle in seconds.
	 * @return true if processing is successful.
	 */
	virtual bool processData(const char* buffer, size_t bufferLen, double position, double duration) = 0;

	/**
	 * @brief Closes the subtitle parser and releases any allocated resources.
	 * 
	 * @return true if closed successfully.
	 */
	virtual bool close() = 0;

	/**
	 * @brief Resets the subtitle parser to its initial state.
	 */
	virtual void reset() = 0;

	/**
	 * @brief Sets the offset for progress events.
	 * 
	 * @param offset Offset in seconds.
	 */
	virtual void setProgressEventOffset(double offset) = 0;

	/**
	 * @brief Updates the current playback timestamp.
	 * 
	 * @param positionMs Current playback position in milliseconds.
	 */
	virtual void updateTimestamp(unsigned long long positionMs) = 0;

	/**
	 * @brief Pauses or resumes subtitle rendering.
	 * 
	 * @param pause true to pause, false to resume.
	 */
	virtual void pause(bool pause) {}

	/**
	 * @brief Mutes or unmutes subtitle rendering.
	 * 
	 * @param mute true to mute, false to unmute.
	 */
	virtual void mute(bool mute) {}

	/**
	 * @brief Sets whether the playback is linear.
	 * 
	 * @param isLinear true if playback is linear, false otherwise.
	 */
	virtual void isLinear(bool isLinear) {}

	/**
	 * @brief Sets text style attributes for subtitle rendering.
	 * 
	 * @param options A string containing style options.
	 */
	virtual void setTextStyle(const std::string &options) {}

	
	/**
	 * @brief Registers callback functions for player-subtitle interaction.
	 * 
	 * This method stores function pointers provided via the PlayerCallbacks structure.
	 * These callbacks are used to interact with the player for operations such as:
	 * - Resuming track downloads
	 * - Getting current playback positions
	 * - Stopping track downloads
	 * - Sending VTT cue data
	 * 
	 * @param playerCallBack A structure containing the callback functions to register.
	 */
	void RegisterCallback(const PlayerCallbacks& playerCallBack)
	{
		playerResumeTrackDownloads_CB = playerCallBack.resumeTrackDownloads_CB;
		playerGetPositions_CB = playerCallBack.getPlayerPositions_CB;
		playerStopTrackDownloads_CB = playerCallBack.stopTrackDownloads_CB;
		playerSendVTTCueData_CB = playerCallBack.sendVTTCueData_CB;
	}

	/**
	 * @brief Unregisters all previously registered player callbacks.
	 * 
	 * This method clears all stored callback function pointers, effectively disabling
	 * player-subtitle interactions.
	 */
	void UnRegisterCallback( )
	{
		playerResumeTrackDownloads_CB = NULL;
		playerGetPositions_CB = NULL;
		playerStopTrackDownloads_CB = NULL;
		playerSendVTTCueData_CB = NULL;
	}
protected:

	/**
	 * @brief MIME type of the subtitle stream (e.g., WebVTT, TTML).
	 */
	SubtitleMimeType mType;

	/**
	 * @brief Height of the subtitle rendering area in pixels.
	 */
	int mHeight;

	/**
	 * @brief Width of the subtitle rendering area in pixels.
	 */
	int mWidth;

	/**
	 * @brief Callback to resume subtitle track downloads.
	 */
	std::function<void()> playerResumeTrackDownloads_CB;

	/**
	 * @brief Callback to stop subtitle track downloads.
	 */
	std::function<void()> playerStopTrackDownloads_CB;

	/**
	 * @brief Callback to retrieve the current playback position.
	 * 
	 * The callback provides a reference to a timestamp (in milliseconds) and a playback rate (as a double).
	 */
	std::function<void(long long&, double&)> playerGetPositions_CB;

	/**
	 * @brief Callback to send parsed VTT cue data to the player.
	 * 
	 * @note The VTTCue pointer should point to a valid cue object to be rendered.
	 */
	std::function<void(VTTCue*)> playerSendVTTCueData_CB;

};

#endif /* __SUBTITLE_PARSER_H__ */
