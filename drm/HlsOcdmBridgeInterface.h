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

#ifndef _HLS_OCDM_BRIDGE_INTERFACE_H_
#define _HLS_OCDM_BRIDGE_INTERFACE_H_

/**
 * @file HlsOcdmBridgeInterface.h
 * @brief Handles OCDM bridge interface to validate DRM License
 */

#include "HlsDrmBase.h"

/**
 * @class FakeHlsOcdmBridge
 * @brief Fake OCDM bridge to handle DRM key
 */

class FakeHlsOcdmBridge : public HlsDrmBase
{
public:
	FakeHlsOcdmBridge(DrmSession * DrmSession){}

	virtual ~FakeHlsOcdmBridge(){}

	FakeHlsOcdmBridge(const FakeHlsOcdmBridge&) = delete;

	FakeHlsOcdmBridge& operator=(const FakeHlsOcdmBridge&) = delete;

	/*HlsDrmBase Methods*/

	/**
	* @brief Sets metadata for the DRM session.
	*
	* Default implementation returns an error.
	*
	* @param[in] metadata   Pointer to the metadata structure.
	* @param[in] trackType  Type of the track (e.g., audio, video, subtitles).
	*
	* @return eDRM_ERROR by default.
	*/
	virtual DrmReturn SetMetaData(void* metadata,int trackType) override {return DrmReturn::eDRM_ERROR;}
	
   /**
	* @brief Sets decryption information for the DRM session.
	*
	* Default implementation returns an error.
	*
	* @param[in] drmInfo             Pointer to a DrmInfo structure containing decryption info.
	* @param[in] acquireKeyWaitTime  Maximum time in milliseconds to wait for key acquisition.
	*
	* @return eDRM_ERROR by default.
	*/
	virtual DrmReturn SetDecryptInfo(const struct DrmInfo *drmInfo, int acquireKeyWaitTime) override {return DrmReturn::eDRM_ERROR;}
	/**
	* @brief Decrypts encrypted data for the DRM session.
	*
	* Default implementation returns an error.
	*
	* @param[in] bucketType         Identifier of the data bucket type.
	* @param[in] encryptedDataPtr   Pointer to the encrypted data buffer.
	* @param[in] encryptedDataLen   Length of the encrypted data buffer in bytes.
	* @param[in] timeInMs           Optional wait time in milliseconds (default: DECRYPT_WAIT_TIME_MS).
	*
	* @return eDRM_ERROR by default.
	*/
	virtual DrmReturn Decrypt(int bucketType, void *encryptedDataPtr, size_t encryptedDataLen, int timeInMs = DECRYPT_WAIT_TIME_MS) override {return DrmReturn::eDRM_ERROR;}
	/**
	* @brief Releases the DRM session.
	*
	* Default implementation does nothing.
	*/
	virtual void Release() override {}
	/**
	* @brief Cancels any pending key acquisition.
	*
	* Default implementation does nothing.
	*/
	virtual void CancelKeyWait() override {}
	/**
	* @brief Restores the DRM key state.
	*
	* Default implementation does nothing.
	*/
	virtual void RestoreKeyState() override {}
	/**
	* @brief Acquires a key for the DRM session.
	*
	* Default implementation does nothing.
	*
	* @param[in] metadata   Pointer to metadata used for key acquisition.
	* @param[in] trackType  Type of track for which the key is required.
	*/
	virtual void AcquireKey(void *metadata,int trackType) override {}	
	/**
	* @brief Returns the current DRM session state.
	*
	* Default implementation returns `eDRM_KEY_FAILED`.
	*
	* @return DRM session state as a `DRMState` enum value.
	*/
	virtual DRMState GetState() override {return DRMState::eDRM_KEY_FAILED;}

};

class HlsOcdmBridgeInterface
{

public:

    static HlsDrmBase* GetBridge(DrmSession * drmSession);

};



#endif // _HLS_OCDM_BRIDGE_INTERFACE_H_
