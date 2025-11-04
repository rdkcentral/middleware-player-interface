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

#ifndef _HLS_OCDM_BRIDGE_H_
#define _HLS_OCDM_BRIDGE_H_

/**
 * @file HlsOcdmBridge.h
 * @brief Handles OCDM bridge to validate DRM License
 */

#include "HlsDrmBase.h"

/**
 * @class HlsOcdmBridge
 * @brief OCDM bridge to handle DRM key
 */

class HlsOcdmBridge : public HlsDrmBase
{
	DRMState m_drmState;

	const DrmInfo* m_drmInfo;
	DrmSession* m_drmSession;
	std::mutex m_Mutex;
public:
	HlsOcdmBridge(DrmSession * DrmSession);

	~HlsOcdmBridge();

	HlsOcdmBridge(const HlsOcdmBridge&) = delete;

	HlsOcdmBridge& operator=(const HlsOcdmBridge&) = delete;

	/*HlsDrmBase Methods*/
	/**
	* @brief Sets metadata for the DRM session.
	*
	* This overridden method sets metadata associated with a given track type.
	* The default implementation returns success.
	*
	* @param[in] metadata   Pointer to the metadata structure.
	* @param[in] trackType  Type of the track (e.g., audio, video, subtitles).
	*
	* @return eDRM_SUCCESS by default.
	*/
	virtual DrmReturn SetMetaData(void* metadata,int trackType) override {return eDRM_SUCCESS;};
	/**
	* @brief Sets decryption information for the DRM session.
	*
	* This method provides DRM-specific decryption information and may wait
	* for key acquisition up to the specified timeout.
	*
	* @param[in] drmInfo             Pointer to a DrmInfo structure containing decryption info.
	* @param[in] acquireKeyWaitTime  Maximum time in milliseconds to wait for key acquisition.
	*
	* @return DRM status code indicating success or failure.
	*/
	virtual DrmReturn SetDecryptInfo(const struct DrmInfo *drmInfo, int acquireKeyWaitTime) override;
	/**
	* @brief Decrypts encrypted data for the DRM session.
	*
	* This method decrypts the provided data bucket using the current DRM keys.
	*
	* @param[in] bucketType         Identifier of the data bucket type.
	* @param[in] encryptedDataPtr   Pointer to the encrypted data buffer.
	* @param[in] encryptedDataLen   Length of the encrypted data buffer in bytes.
	* @param[in] timeInMs           Optional wait time in milliseconds (default: DECRYPT_WAIT_TIME_MS).
	*
	* @return DRM status code indicating success or failure.
	*/
	virtual DrmReturn Decrypt(int bucketType, void *encryptedDataPtr, size_t encryptedDataLen, int timeInMs = DECRYPT_WAIT_TIME_MS) override;

	/**
	* @brief Releases the DRM session.
	*
	* This method cleans up resources and terminates the DRM session.
	*
	* @return None.
	*/
	virtual void Release() override;
	/**
	* @brief Cancels any pending key acquisition.
	*
	* This method interrupts any ongoing key wait operations.
	*
	* @return None.
	*/
	virtual void CancelKeyWait() override;
	/**
	* @brief Restores the DRM key state.
	*
	* This method can be used to reapply previously acquired DRM keys.
	* Default implementation does nothing.
	*
	* @return None.
	*/
	virtual void RestoreKeyState() override {};
	/**
	* @brief Acquires a key for the DRM session.
	*
	* This method initiates key acquisition using the provided metadata
	* and track type. Default implementation does nothing.
	*
	* @param[in] metadata   Pointer to metadata used for key acquisition.
	* @param[in] trackType  Type of track for which the key is required.
	*
	* @return None.
	*/
	virtual void AcquireKey(void *metadata,int trackType) override {};
	
	/**
	* @brief Returns the current DRM session state.
	*
	* This method provides the current state of the DRM session.
	*
	* @return The DRM session state as a `DRMState` enum value.
	*/
	virtual DRMState GetState() override {return m_drmState;}

};

#endif // _HLS_OCDM_BRIDGE_H_
