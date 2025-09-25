/*
 * If not stated otherwise in this file or this component's license file the
 * following copyright and licenses apply:
 *
 * Copyright 2023 RDK Management
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
#include "GstUtils.h"
#include "InterfacePlayerPriv.h"


/**
 * @brief Sets a flag indicating that pipeline transition to PLAYING state is pending
 */
const char *gstGetMediaTypeName(GstMediaType mediaType)
{
	static const char *name[] =
	{
		"video",//eMEDIATYPE_VIDEO
		"audio",//eMEDIATYPE_AUDIO
		"text",//eMEDIATYPE_SUBTITLE
		"aux_audio",//eMEDIATYPE_AUX_AUDIO
		"manifest",//eMEDIATYPE_MANIFEST
		"licence",//eMEDIATYPE_LICENCE
		"iframe",//eMEDIATYPE_IFRAME
		"init_video",//eMEDIATYPE_INIT_VIDEO
		"init_audio",//eMEDIATYPE_INIT_AUDIO
		"init_text",//eMEDIATYPE_INIT_SUBTITLE
		"init_aux_audio",//eMEDIATYPE_INIT_AUX_AUDIO
		"playlist_video",//eMEDIATYPE_PLAYLIST_VIDEO
		"playlist_audio",//eMEDIATYPE_PLAYLIST_AUDIO
		"playlist_text",//eMEDIATYPE_PLAYLIST_SUBTITLE
		"playlist_aux_audio",//eMEDIATYPE_PLAYLIST_AUX_AUDIO
		"playlist_iframe",//eMEDIATYPE_PLAYLIST_IFRAME
		"init_iframe",//eMEDIATYPE_INIT_IFRAME
		"dsm_cc",//eMEDIATYPE_DSM_CC
		"image",//eMEDIATYPE_IMAGE
	};
	if( mediaType < eGST_MEDIATYPE_DEFAULT )
	{
		return name[mediaType];
	}
	else
	{
		return "UNKNOWN";
	}
}
