/*
 * If not stated otherwise in this file or this component's license file the
 * following copyright and licenses apply:
 *
 * Copyright 2020 RDK Management
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
 * @file OcdmGstSessionAdapter.h
 * @brief File holds operations on OCDM gst sessions
 */


#include <dlfcn.h>
#include <mutex>
#include <gst/gst.h>
#include "opencdmsessionadapter.h"
#include "PlayerLogManager.h"


/**
* @class OCDMGSTSessionAdapter
* @brief OpenCDM session adapter for GStreamer-based DRM sessions.
*
* This class extends OCDMSessionAdapter to provide GStreamer-specific
* decryption functionality for DRM-protected media streams, including
* SEI extraction and buffer decryption.
*/
class OCDMGSTSessionAdapter : public OCDMSessionAdapter
{
	/**
	* @brief Extracts SEI (Supplemental Enhancement Information) data from a GStreamer buffer.
	*
	* SEI messages typically contain metadata such as closed captions, HDR information,
	* or other auxiliary data embedded in the video stream. This function parses
	* the provided GstBuffer and extracts the SEI information as needed.
	*
	* @param buffer GstBuffer containing the encoded video data from which SEI is extracted.
	*/    
	void ExtractSEI( GstBuffer *buffer);
public:
	/**
	* @brief Constructor for OCDMGSTSessionAdapter.
	*
	* Initializes the OpenCDM GStreamer session adapter by calling the base
	* OCDMSessionAdapter constructor and attempts to dynamically load the
	* `opencdm_gstreamer_session_decrypt_buffer` function using dlsym.
	*
	* @param drmHelper Shared pointer to a DrmHelper instance used for DRM operations.
	* @param drmCallbacks Pointer to a DrmCallbacks structure for handling DRM events.
	*
	* If the dynamic symbol is found, a warning is logged indicating the
	* function is available; otherwise, a warning indicates it is not found.
	*/
	OCDMGSTSessionAdapter(DrmHelperPtr drmHelper,  DrmCallbacks *drmCallbacks) : OCDMSessionAdapter(drmHelper, drmCallbacks)
, OCDMGSTSessionDecrypt(nullptr)
	{
                const char* ocdmgstsessiondecrypt = "opencdm_gstreamer_session_decrypt_buffer";
                OCDMGSTSessionDecrypt = (OpenCDMError(*)(struct OpenCDMSession*, GstBuffer*, GstCaps*))dlsym(RTLD_DEFAULT, ocdmgstsessiondecrypt);
                if (OCDMGSTSessionDecrypt)
                    MW_LOG_WARN("Has opencdm_gstreamer_session_decrypt_buffer");
                else
                    MW_LOG_WARN("No opencdm_gstreamer_session_decrypt_buffer found");
	};
	~OCDMGSTSessionAdapter() {};
	/**
	* @brief Decrypt a GStreamer buffer using key and IV buffers.
	*
	* This method uses GStreamer-specific buffers for the key ID, IV, and
	* encrypted payload, optionally handling sub-sample decryption.
	*
	* @param keyIDBuffer GstBuffer containing the key ID.
	* @param ivBuffer GstBuffer containing the initialization vector (IV).
	* @param buffer GstBuffer containing the encrypted payload data.
	* @param subSampleCount Number of sub-samples.
	* @param subSamplesBuffer GstBuffer containing sub-sample information.
	* @param caps GstCaps associated with the buffer.
	* @return Integer status code: 0 for success, non-zero for failure.
	*/
	int decrypt(GstBuffer* keyIDBuffer, GstBuffer* ivBuffer, GstBuffer* buffer, unsigned subSampleCount, GstBuffer* subSamplesBuffer, GstCaps* caps);
	/**
	* @brief Decrypt raw payload data using a provided IV.
	*
	* This method works with raw byte arrays for IV and payload data and
	* outputs decrypted data to an opaque buffer pointer.
	*
	* @param f_pbIV Pointer to the initialization vector (IV).
	* @param f_cbIV Size of the IV in bytes.
	* @param payloadData Pointer to the encrypted payload data.
	* @param payloadDataSize Size of the encrypted payload data in bytes.
	* @param ppOpaqueData Pointer to store the decrypted opaque data.
	* @return Integer status code: 0 for success, non-zero for failure.
	*/
	int decrypt(const uint8_t *f_pbIV, uint32_t f_cbIV, const uint8_t *payloadData, uint32_t payloadDataSize, uint8_t **ppOpaqueData);
private:
	/**
	* @brief Function pointer to the OpenCDM GStreamer decryption function.
	*
	* This pointer is dynamically loaded using `dlsym` for the symbol
	* `opencdm_gstreamer_session_decrypt_buffer`. If available, it can be
	* used to decrypt a GstBuffer within an OpenCDM session.
	*
	* @note The function signature is:
	*       OpenCDMError (*)(struct OpenCDMSession*, GstBuffer*, GstCaps*)
	*/    
	OpenCDMError(*OCDMGSTSessionDecrypt)(struct OpenCDMSession*, GstBuffer*, GstCaps*);

};
