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

#ifndef OpenCDMSessionAdapter_h
#define OpenCDMSessionAdapter_h

/**
 * @file opencdmsessionadapter.h
 * @brief Handles operation with OCDM session to handle DRM License data
 */

#include "PlayerExternalsInterface.h"
#include "DrmSession.h"
#include "DrmHelper.h"

#include "open_cdm.h"
#include "open_cdm_adapter.h"
#include <condition_variable>
#include "DrmCallbacks.h"

using namespace std;

/**
 * @class Event
 * @brief class to DRM Event handle
 */

class Event {
private:
	bool signalled; //TODO: added to handle the events fired before calling wait, need to recheck
	std::mutex lock;
	std::condition_variable condition;
public:
	Event() : signalled(false), lock(), condition() {
	}
	virtual ~Event() {
	}
	/**
	 * @brief Wait for a specified amount of time.
	 *
	 * This function blocks the calling thread for the given duration in milliseconds.
	 *
	 * @param waitTime Time to wait in milliseconds.
	 * @return True if the wait completed successfully, false otherwise.
	 */
	inline bool wait(const uint32_t waitTime)
	{
		bool ret = true;
		std::unique_lock<std::mutex> _lock(lock);
		if (!signalled) {
			if (waitTime == 0) {
				condition.wait(_lock);
			} else {
				if( std::cv_status::timeout == condition.wait_for(_lock,std::chrono::milliseconds(waitTime)) )
				{
					ret = false;
				}
			}
		}
		signalled = false;
		return ret;
	}

	/**
	* @brief Signal or notify waiting threads.
	*
	* This function is typically used in conjunction with a wait mechanism
	* to unblock threads that are waiting for a certain condition or event.
	*/
	inline void signal()
	{
		std::unique_lock<std::mutex> _lock(lock);
		signalled = true;
		condition.notify_all();
	}
};

/**
 * @class OCDMSessionAdapter
 * @brief Open CDM DRM session
 */
class OCDMSessionAdapter : public DrmSession
{
protected:
	std::mutex decryptMutex;

	KeyState m_eKeyState;

	OpenCDMSession* m_pOpenCDMSession;
#ifdef USE_THUNDER_OCDM_API_0_2
	struct OpenCDMSystem* m_pOpenCDMSystem;
#else
	struct OpenCDMAccessor* m_pOpenCDMSystem;
#endif
	OpenCDMSessionCallbacks m_OCDMSessionCallbacks;
	std::shared_ptr<PlayerExternalsInterface> m_pOutputProtection;

	std::string m_challenge;
	uint16_t m_challengeSize;

	std::string m_destUrl;
	KeyStatus m_keyStatus;
	bool m_keyStateIndeterminate;
	std::vector<uint8_t> m_keyStored;

	Event m_challengeReady;
	Event m_keyStatusReady;
	Event m_keyStatusWait;
	string m_sessionID;

	std::vector<uint8_t> m_keyId;

	DrmHelperPtr m_drmHelper;
	DrmCallbacks *m_drmCallbacks;

	/**
	* @brief Verify the output protection status of the device or playback path.
	*
	* This function checks whether the current playback or device configuration
	* meets the required output protection policies (e.g., HDCP, secure path).
	*
	* @return True if the output protection requirements are satisfied, false otherwise.
	*/
	bool verifyOutputProtection();
public:
	/**
	* @brief Process an OpenCDM challenge.
	*
	* This function handles the challenge received from the DRM system,
	* typically used to request a license or perform key negotiation.
	*
	* @param destUrl Destination URL to send the challenge to (license server).
	* @param challenge Pointer to the challenge data buffer.
	* @param challengeSize Size of the challenge data in bytes.
	*/
	void processOCDMChallenge(const char destUrl[], const uint8_t challenge[], const uint16_t challengeSize);
	/**
	* @brief Notify that DRM keys have been updated.
	*
	* This function should be called when the DRM system has updated or
	* renewed keys, so that dependent components can react accordingly.
	*/
	void keysUpdatedOCDM();
	/**
	* @brief Update a specific DRM key.
	*
	* This function provides a new key to the DRM session for decryption.
	*
	* @param key Pointer to the key data buffer.
	* @param keySize Size of the key data in bytes.
	*/
	void keyUpdateOCDM(const uint8_t key[], const uint8_t keySize);
	long long timeBeforeCallback;

private:
	void initDRMSystem();

public:
    	OCDMSessionAdapter(DrmHelperPtr drmHelper, DrmCallbacks *callbacks = nullptr);
	~OCDMSessionAdapter();
    	OCDMSessionAdapter(const OCDMSessionAdapter&) = delete;
	OCDMSessionAdapter& operator=(const OCDMSessionAdapter&) = delete;
	/**
	* @brief Generate a new DRM session using initialization data.
	*
	* This function initializes a DRM session with the provided initialization data
	* and optional custom data for license requests.
	*
	* @param f_pbInitData Pointer to the initialization data buffer.
	* @param f_cbInitData Size of the initialization data in bytes.
	* @param customData Custom data string that may be used in license requests.
	*/
	void generateDRMSession(const uint8_t *f_pbInitData,
		uint32_t f_cbInitData, std::string &customData) override; 
	/**
	* @brief Generate a key request for the DRM session.
	*
	* This function prepares a key request that can be sent to a license server.
	*
	* @param destinationURL URL of the license server.
	* @param timeout Timeout in milliseconds for the key request operation.
	* @return Pointer to a DrmData object containing the key request payload.
	*/
	DrmData * generateKeyRequest(string& destinationURL, uint32_t timeout) override;
	/**
	* @brief Process a DRM key received from the license server.
	*
	* This function applies the key to the DRM session for decryption.
	*
	* @param key Pointer to the DrmData object containing the received key.
	* @param timeout Timeout in milliseconds for processing the key.
	* @return Integer status code: 0 for success, non-zero for failure.
	*/
	int processDRMKey(DrmData* key, uint32_t timeout) override;
	/**
	* @brief Get the current state of the DRM session.
	*
	* @return Current KeyState of the DRM session (e.g., usable, expired, pending).
	*/
	KeyState getState() override;
	/**
	* @brief Clear the decryption context for the DRM session.
	*
	* This function releases any keys or session resources associated with
	* decryption to reset the session state.
	*/
	void clearDecryptContext() override;
#if defined(USE_OPENCDM_ADAPTER)
	/**
	* @brief Set the key ID for the DRM session.
	*
	* This function updates the DRM session with the specified key ID, which
	* will be used for decryption of protected content.
	*
	* @param keyId Vector containing the key ID bytes.
	*/
	void setKeyId(const std::vector<uint8_t>& keyId) override;
#endif
	/**
	* @brief Wait for the DRM session to reach a specific key state.
	*
	* This function blocks until the DRM session reaches the desired KeyState
	* or the timeout expires.
	*
	* @param state The desired KeyState to wait for.
	* @param timeout Maximum time to wait in milliseconds.
	* @return True if the desired state was reached within the timeout, false otherwise.
	*/
	bool waitForState(KeyState state, const uint32_t timeout) override;
};

#endif
