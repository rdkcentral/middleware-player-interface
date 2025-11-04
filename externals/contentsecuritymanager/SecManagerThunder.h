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
 * @file SecManagerThunder.h
 * @brief Class to communicate with SecManager Thunder plugin
 */

#ifndef __SECMANAGER_THUNDER_H__
#define __SECMANAGER_THUNDER_H__

#include <mutex>
#include <sys/time.h>
#include "ContentSecurityManagerSession.h"
#include "PlayerScheduler.h"
#include "PlayerMemoryUtils.h"
#include "ThunderAccessPlayer.h"
#include <inttypes.h>
#include <memory>
#include <list>
#include <map>
#include <vector>

#define SECMANAGER_CALL_SIGN "org.rdk.SecManager.1"
#define WATERMARK_PLUGIN_CALLSIGN "org.rdk.Watermark.1"
//#define RDKSHELL_CALLSIGN "org.rdk.RDKShell.1"   //need to be used instead of WATERMARK_PLUGIN_CALLSIGN if RDK Shell is used for rendering watermark

/**
 * @class SecManagerThunder
 * @brief Class to get License from Sec Manager Thunder
 */
class SecManagerThunder : public ContentSecurityManager
{
public:
        /**
         * @brief Sets DRM session state (e.g., active/inactive)
         * @param sessionId DRM session ID
         * @param active Whether the session should be marked active
         * @return true on success, false otherwise
         */
        bool SetDrmSessionState(int64_t sessionId, bool active) override;
        /**
         * @brief Closes an existing DRM session
         * @param sessionId DRM session ID
         * @return true if closed successfully
         */
        void CloseDrmSession(int64_t sessionId) override;
        /* Run AcquireLicenseOpenOrUpdate is the old AcquireLicense code
         * It is used by AcquireLicense() to for opening sessions & for calling update when this is required*/
        bool AcquireLicenseOpenOrUpdate( std::string clientId, std::string appId, const char* licenseUrl, const char* moneyTraceMetadata[][2],
                                                const char* accessAttributes[][2], const char* contentMetadata, size_t contentMetadataLen,
                                                const char* licenseRequest, size_t licenseRequestLen, const char* keySystemId,
                                                const char* mediaUsage, const char* accessToken, size_t accessTokenLen,
                                                ContentSecurityManagerSession &session,
                                                char** licenseResponse, size_t* licenseResponseLength,
                                                int32_t* statusCode, int32_t* reasonCode, int32_t*  businessStatus, bool isVideoMuted, int sleepTime) override;
        /**
         * @brief Sends update license challenge to existing session
         * @param sessionId DRM session ID
         * @param licenseRequest Challenge string
         * @param initData Initialization data
         * @param[out] response Response from Firebolt
         * @return true on success
         */
        /**
         * @brief Sets playback state for watermark alignment
         * @param sessionId Session ID
         * @param speed Playback rate (1.0 = normal)
         * @param position Current position in seconds
         * @return true if command succeeded
         */
        bool SetPlaybackPosition(int64_t sessionId, float speed, int32_t position) override;
        /**
         * @brief Enables or disables visual watermark
         * @param show Show/hide flag
         * @param sessionId Session context (optional)
         */
        void ShowWatermark(bool show);
	/**
	 * @fn setWindowSize
	 *
	 * @param[in] sessionId - session id
	 * @param[in] video_width - video width 
	 * @param[in] video_height - video height 
	 */
	bool setWindowSize(int64_t sessionId, int64_t video_width, int64_t video_height);

	/**
	 * @fn getSchedulerStatus
	 *
	 * @return bool - true if scheduler is running, false otherwise
	 */
	bool getSchedulerStatus();
        /**
         * @fn SecManagerThunder
         */
        SecManagerThunder();
        /**
         * @fn ~SecManagerThunder
         */
        ~SecManagerThunder();
        /**
         * @brief Copy constructor disabled
         */
        SecManagerThunder(const SecManagerThunder&) = delete;
        /**
         * @brief assignment operator disabled
         */
        SecManagerThunder* operator=(const SecManagerThunder&) = delete;
protected:
	/*Event Handlers*/
	/**
	* @brief Handles a request to configure or manage a watermark session.
	*
	* This function processes a JSON object containing session-related parameters
	* for watermark management, such as session ID or global settings.
	*
	* @param parameters A JSON object containing the watermark session configuration.
	*/
	void watermarkSessionHandler(const JsonObject& parameters);
	/**
	* @brief Handles a request to add a new watermark based on JSON parameters.
	*
	* This function parses the provided JSON object to extract watermark
	* properties such as graphic ID, z-index, position, and display attributes,
	* and then creates the watermark accordingly.
	*
	* @param parameters A JSON object containing the configuration parameters for the new watermark.
	*/
	void addWatermarkHandler(const JsonObject& parameters);
	/**
	* @brief Handles a request to update an existing watermark based on JSON parameters.
	*
	* This function parses the JSON object to extract updated properties for an
	* existing watermark, such as new image data, palette, or display position.
	*
	* @param parameters A JSON object containing the updated watermark parameters.
	*/
	void updateWatermarkHandler(const JsonObject& parameters);
	/**
	* @brief Handles a request to remove a watermark based on JSON parameters.
	*
	* This function parses the JSON object to identify the watermark to remove
	* and deletes it from the display.
	*
	* @param parameters A JSON object containing the identifier of the watermark to remove.
	*/
	void removeWatermarkHandler(const JsonObject& parameters);
	/**
	* @brief Handles a request to show a watermark based on JSON parameters.
	*
	* This function parses the provided JSON object to extract watermark-related
	* settings such as graphic ID, z-index, position, or other display properties,
	* and then displays the watermark accordingly.
	*
	* @param parameters A JSON object containing the configuration parameters for the watermark.
	*/
	void showWatermarkHandler(const JsonObject& parameters);
	/**
	* @brief Loads a watermark with a specific color lookup table (CLUT) and image data.
	*
	* This function initializes or updates a watermark using the provided CLUT buffer
	* and image buffer. It supports specifying the palette format, size, and
	* display dimensions including aspect ratio.
	*
	* @param sessionId             The session identifier associated with the watermark.
	* @param graphicId             The identifier of the watermark to load or update.
	* @param watermarkClutBufferKey Key or pointer to the CLUT buffer.
	* @param watermarkImageBufferKey Key or pointer to the image buffer for the watermark.
	* @param clutPaletteSize       Size of the CLUT palette in bytes.
	* @param clutPaletteFormat     Format of the CLUT palette (e.g., "ARGB", "RGBA").
	* @param watermarkWidth        Width of the watermark in pixels.
	* @param watermarkHeight       Height of the watermark in pixels.
	* @param aspectRatio           Aspect ratio of the watermark (width / height).
	*
	* @return bool Returns true if the watermark was successfully loaded; false otherwise.
	*/
    bool loadClutWatermark(int64_t sessionId, int64_t graphicId, int64_t watermarkClutBufferKey, int64_t watermarkImageBufferKey, int64_t clutPaletteSize, const char* clutPaletteFormat, int64_t watermarkWidth, int64_t watermarkHeight, float aspectRatio);
	/**
	* @brief Creates a new watermark on the display.
	*
	* This function adds a watermark with the specified graphic ID and stacking
	* order (z-index) on top of the video or UI layer.
	*
	* @param graphicId The identifier to assign to the new watermark.
	* @param zIndex    The stacking order for the watermark; higher values
	*                  appear above lower ones.
	*/
	void CreateWatermark(int graphicId, int zIndex);
	/**
	 * @brief Updates the content of an existing watermark.
	 *
	 * This function modifies the watermark identified by the given graphic ID
	 * using new symbol or image data.
	 *
	 * @param graphicId The identifier of the watermark to update.
	 * @param smKey     The key or pointer to the new symbol/image data.
	 * @param smSize    The size of the symbol/image data in bytes.
	 */
	void UpdateWatermark(int graphicId, int smKey, int smSize);
	/**
	 * @brief Retrieves the color palette of a watermark.
	 *
	 * This function fetches the color lookup table (CLUT) and image mapping
	 * information for a specific watermark identified by its session ID and graphic ID.
	 *
	 * @param sessionId The session identifier associated with the watermark.
	 * @param graphicId The identifier of the watermark whose palette is being queried.
	 */
	void GetWaterMarkPalette(int sessionId, int graphicId);
	/**
	* @brief Modifies the color palette of a watermark.
	*
	* This function updates the color lookup table (CLUT) and image mapping
	* for a specific watermark identified by its graphic ID.
	*
	* @param graphicId The identifier of the watermark to modify.
	* @param clutKey   The new color lookup table (CLUT) key to apply.
	* @param imageKey  The new image key to apply to the watermark.
	*/
	void ModifyWatermarkPalette(int graphicId, int clutKey, int imageKey);
	/**
	* @brief Deletes a watermark from the display.
	*
	* This function removes the watermark identified by the given graphic ID
	* from the video or UI overlay.
	*
	* @param graphicId The identifier of the watermark to delete.
	*/
	void DeleteWatermark(int graphicId);
	/**
	* @brief Configures whether the watermark should always appear on top.
	*
	* This function sets the display behavior of the watermark overlay. When enabled,
	* the watermark will remain visible above all other video content, regardless of
	* other UI or overlay elements.
	*
	* @param show Set to true to always show the watermark on top; false to allow it
	*             to be occluded by other elements.
	*/
	void AlwaysShowWatermarkOnTop(bool show);
	/**
	* @brief Registers all required events with the system or component.
	*
	* This function sets up all necessary event handlers or callbacks so that
	* the system can respond to events appropriately. It should typically be
	* called during initialization or when event handling needs to be restored.
	*/
	void RegisterAllEvents ();
	/**
	* @brief Unregisters all previously registered events.
	*
	* This function removes all event handlers or callbacks that were
	* registered with the system or component. After calling this function,
	* no events will trigger until new handlers are registered.
	*/
	void UnRegisterAllEvents ();

	ThunderAccessPlayer mSecManagerObj;       /**< ThunderAccessPlayer object for communicating with SecManager*/
	ThunderAccessPlayer mWatermarkPluginObj;  /**< ThunderAccessPlayer object for communicating with Watermark Plugin Obj*/
	std::mutex mSecMutex;    	        /**< Lock for accessing mSecManagerObj*/
	std::mutex mWatMutex;		        /**< Lock for accessing mWatermarkPluginObj*/
	std::mutex mSpeedStateMutex;		/**< mutex for setPlaybackSpeedState()*/
	std::list<std::string> mRegisteredEvents;
	bool mSchedulerStarted;
};

#endif /* __SECMANAGER_THUNDER_H__ */
