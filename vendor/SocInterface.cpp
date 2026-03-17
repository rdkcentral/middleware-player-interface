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

#include <assert.h>
#include "SocInterface.h"
#include "vendor/default/DefaultSocInterface.h"
#include "PlayerLogManager.h"
#if defined(PLAYER_SOC_INTERFACE_IMPL)
#include "SocInterfaceImpl.h"
#endif


/**
 * @brief Creates an instance of the SoC-specific interface based on the detected platform.
 *
 * @return A pointer to the created SocInterface object, or nullptr on failure.
 */
std::shared_ptr<SocInterface> SocInterface::CreateSocInterface()
{
	static std::shared_ptr<SocInterface> socInterface;
	if( !socInterface)
	{		
#if defined(PLAYER_SOC_INTERFACE_IMPL)
		socInterface = std::make_shared<SocInterfaceImpl>();
#else
		socInterface = std::make_shared<DefaultSocInterface>();
#endif
	}
	return socInterface;
}

/**
 * @brief Get video PTS.
 *
 * Retrieves the current video presentation timestamp (PTS).
 *
 * @param video_sink The video sink element (unused)
 * @param video_dec The video decoder element.
 * @param isWesteros A flag for Westeros logic.
 *
 * @return Video PTS in nanoseconds, or -1 on error.
 */
long long SocInterface::GetVideoPts(GstElement *video_sink, GstElement *video_dec, bool isWesteros)
{
	gint64 currentPTS = 0;
	if(video_dec)
	{
		g_object_get(video_dec, "video-pts", &currentPTS, NULL);
		if(!isWesteros)
		{
			currentPTS *= 2;
		}
	}
	return (long long)currentPTS;
}

/**
 * @brief Set decode error on source.
 *
 * Sets a decode error flag on the given source object.
 *
 * @param src The source object.
 */
void SocInterface::SetDecodeError(GstObject* src)
{
	if(src)
	{
		g_object_set(src, "report_decode_errors", TRUE, NULL);
	}
}

/**
 * @brief Sets the state of Westeros Sink usage.
 *
 * This function updates the internal flag to indicate whether
 * Westeros Sink is being used. It does not enable or disable
 * Westeros Sink itself, but merely informs the SocInterface
 * about its status.
 *
 * @param status Set to `true` if Westeros Sink is enabled, `false` otherwise.
 */
void SocInterface::SetWesterosSinkState(bool status)
{
	mUsingWesterosSink = status;
}

/**
 * @brief Configure Capability Acceptance for GStreamer Transform
 *
 * Sets up the accept_caps function pointer for a GStreamer base transform class.
 * This allows the transform element to decide whether it can accept a given set of capabilities (caps),
 * which is essential for negotiating media formats during pipeline setup.
 *
 * @param base_transform_class Pointer to the GStreamer base transform class to configure.
 * @param accept_caps_func Function used to determine if the transform accepts specific caps.
 */
void SocInterface::ConfigureAcceptCaps(GstBaseTransformClass* base_transform_class ,
													AcceptCapsFunc accept_caps_func) {
    if (accept_caps_func) {
        base_transform_class->accept_caps = GST_DEBUG_FUNCPTR(accept_caps_func);
    }
}

/**
 * @brief Set AC4 tracks.
 * @param src Source element.
 * @param trackId Track ID.
 */
void SocInterface::SetAC4Tracks(GstElement *src, int trackId)
{
	MW_LOG_INFO("Selecting AC4 Track Id : %d", trackId);
	if(src)
	{
		g_object_set(src, "ac4-presentation-group-index", trackId, NULL);
	}
	else
	{
		MW_LOG_ERR("No valid src to set ac4-presentation-group-index");
	}
}
