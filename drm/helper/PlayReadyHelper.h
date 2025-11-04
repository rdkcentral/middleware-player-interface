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
#ifndef _PLAYREADY_HELPER_H
#define _PLAYREADY_HELPER_H

/**
 * @file PlayReadyHelper.h
 * @brief Handles the operation for Play ready DRM operations
 */

#include <memory>

#include "DrmHelper.h"
#include "PlayerLogManager.h"
/** 
 * @class PlayReadyHelper
 * @brief Handles the operation for Play ready DRM operations
 */

class PlayReadyHelper : public DrmHelper
{
public:
	friend class PlayReadyHelperFactory;

	const std::string& ocdmSystemId() const override;

	/**
	* @brief Creates the initialization data required for the DRM session.
	*
	* Populates the provided vector with initialization data that can be
	* used to configure or start the DRM session.
	*
	* @param[out] initData  Vector to store the initialization data.
	*/
	void createInitData(std::vector<uint8_t>& initData) const override;
	/**
	* @brief Parses a PSSH (Protection System Specific Header) box.
	*
	* Extracts DRM-specific information from the provided PSSH data.
	*
	* @param[in] initData     Pointer to the PSSH data buffer.
	* @param[in] initDataLen  Length of the PSSH data buffer in bytes.
	*
	* @return True if parsing succeeds, false otherwise.
	*/
	bool parsePssh(const uint8_t* initData, uint32_t initDataLen) override;
	/**
	* @brief Indicates whether the DRM session uses clear content decryption.
	*
	* @return False, as this implementation does not use clear decryption.
	*/
	bool isClearDecrypt() const override { return false; }
	/**
	* @brief Checks whether HDCP 2.2 output protection is required.
	*
	* @return True if output protection is enabled, false otherwise.
	*/
	bool isHdcp22Required() const override { return bOutputProtectionEnabled; }
	/**
	* @brief Sets DRM metadata for the current session.
	*
	* @param[in] metaData  Metadata string associated with the DRM session.
	*/
	void setDrmMetaData( const std::string& metaData ) override;
	/**
	* @brief Retrieves the DRM key identifier.
	*
	* Populates the provided vector with the key ID associated with the
	* current DRM session.
	*
	* @param[out] keyID  Vector to store the DRM key identifier.
	*/
	void getKey(std::vector<uint8_t>& keyID) const override;
	/**
	* @brief Returns the codec type associated with the DRM session.
	*
	* @return Integer representing the codec type (`CODEC_TYPE`).
	*/
	virtual int getDrmCodecType() const override { return CODEC_TYPE; }

	/**
	* @brief Generates a license request for DRM-protected content.
	*
	* Fills the `LicenseRequest` structure based on the provided `ChallengeInfo`.
	*
	* @param[in]  challengeInfo   Challenge information from the DRM session.
	* @param[out] licenseRequest  License request to be populated.
	*/
	void generateLicenseRequest(const ChallengeInfo& challengeInfo, LicenseRequest& licenseRequest) const override;
	/**
	* @brief Retrieves the DRM metadata for the current session.
	*
	* @return Constant reference to the metadata string (`mContentMetaData`).
	*/
	const std::string& getDrmMetaData() const override {return mContentMetaData;}
	/**
	* @brief Returns a friendly name for the DRM session.
	*
	* @return Constant reference to the friendly name (`FRIENDLY_NAME`).
	*/
	virtual const std::string& friendlyName() const override { return FRIENDLY_NAME; };

	PlayReadyHelper(const struct DrmInfo& drmInfo) : DrmHelper(drmInfo), FRIENDLY_NAME("PlayReady"), CODEC_TYPE(2),
		mPsshStr(),
		mInitData(), mKeyID(), mContentMetaData(), mStrInitDataFormated()
	{}

	~PlayReadyHelper() {}

private:
	std::string extractMetaData();
	std::string extractKeyID();
	std::string findSubstr(std::string &data, std::string start, std::string end); /**< move to utils */
	
	static const std::string PLAYREADY_OCDM_ID;
	static const size_t PLAYREADY_DECODED_KEY_ID_LEN; // Expected size of base64 decoded key ID from the PSSH
	static const size_t PLAYREADY_KEY_ID_LEN; // PlayReady ID key length. A NULL character is included at the end
	const std::string FRIENDLY_NAME;
	const int CODEC_TYPE;

	std::string mPsshStr;
	std::vector<uint8_t> mInitData;
	std::vector<uint8_t> mKeyID;
	std::string mContentMetaData;
	std::string mStrInitDataFormated;           /**< String init data after clean up*/
};

/**
 * @class PlayReadyHelperFactory
 * @brief Handles operations to support play ready DRM
 */
class PlayReadyHelperFactory : public DrmHelperFactory
{
public:
	/**
	* @brief Creates a DRM helper object using the provided DRM information.
	*
	* This method instantiates and returns a `DrmHelper` object configured
	* based on the given `DrmInfo`. The helper can then be used for
	* license acquisition, decryption, or other DRM operations.
	*
	* @param[in] drmInfo  DRM information used to configure the helper.
	*
	* @return Shared pointer to a `DrmHelper` instance.
	*/
	DrmHelperPtr createHelper(const struct DrmInfo& drmInfo) const;
	/**
	* @brief Appends supported DRM system IDs to the provided vector.
	*
	* This method adds the DRM system identifiers that are supported by
	* this implementation to the given vector. Useful for determining
	* compatibility with content or license servers.
	*
	* @param[out] systemIds  Vector to store the supported DRM system IDs.
	*/
	void appendSystemId(std::vector<std::string>& systemIds) const;
	/**
	* @brief Determines whether the provided DRM information corresponds to DRM-protected content.
	*
	* This method checks the given `DrmInfo` structure to see if the content
	* is protected by a DRM system.
	*
	* @param[in] drmInfo  DRM information to check.
	*
	* @return True if the content is DRM-protected, false otherwise.
	*/
	bool isDRM(const struct DrmInfo& drmInfo) const;
};

#endif //_PLAYREADY_HELPER_H
