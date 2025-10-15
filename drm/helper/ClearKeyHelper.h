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
#ifndef _CLEARKEY_HELPER_H
#define _CLEARKEY_HELPER_H

/**
 * @file ClearKeyHelper.cpp
 * @brief Implemented Clear key helper functions
 */

#include <memory>

#include "DrmHelper.h"

/**
 * @class ClearKeyHelper
 * @brief Class handles the clear key license operations
 */
class ClearKeyHelper : public DrmHelper
{
public:
	const std::string& ocdmSystemId() const override;
	/**
	* @brief Creates the initialization data for the DRM session.
	*
	* This method populates the provided vector with the initialization
	* data required to start or configure the DRM session.
	*
	* @param[out] initData  Vector to store the initialization data.
	*
	* @return None.
	*/
	void createInitData(std::vector<uint8_t>& initData) const override;
	/**
	* @brief Parses a PSSH (Protection System Specific Header) box.
	*
	* This method reads and interprets the PSSH data to extract information
	* required for DRM initialization or license acquisition.
	*
	* @param[in] initData     Pointer to the PSSH data buffer.
	* @param[in] initDataLen  Length of the PSSH data buffer in bytes.
	*
	* @return True if parsing succeeds, false otherwise.
	*/
	bool parsePssh(const uint8_t* initData, uint32_t initDataLen) override;
	/**
	* @brief Checks if the DRM session uses clear content decryption.
	*
	* This method indicates whether the DRM session performs decryption
	* without encrypted content (i.e., clear decrypt mode).
	*
	* @return True if clear decryption is used, false otherwise.
	*/
	bool isClearDecrypt() const override { return true; }
	/**
	* @brief Retrieves the DRM key identifier.
	*
	* This method populates the provided vector with the key ID associated
	* with the DRM session.
	*
	* @param[out] keyID  Vector to store the DRM key identifier.
	*
	* @return None.
	*/
	void getKey(std::vector<uint8_t>& keyID) const override;
	/**
	* @brief Returns the codec type associated with the DRM session.
	*
	* This method provides the codec type used for the DRM-protected content.
	*
	* @return Codec type as an integer constant (`CODEC_TYPE`).
	*/
	virtual int getDrmCodecType() const override { return CODEC_TYPE; }
	/**
	* @brief Generates a license request for DRM-protected content.
	*
	* This method fills the provided `LicenseRequest` structure based on
	* the input `ChallengeInfo`. It is used to request a license from
	* the DRM server.
	*
	* @param[in]  challengeInfo   Challenge information from the DRM session.
	* @param[out] licenseRequest  License request to be populated.
	*
	* @return None.
	*/
	void generateLicenseRequest(const ChallengeInfo& challengeInfo, LicenseRequest& licenseRequest) const override;
	/**
	* @brief Transforms a DRM license response.
	*
	* This method processes the license response received from the DRM
	* server and updates internal DRM session data accordingly.
	*
	* @param[in] licenseResponse  Shared pointer to the DRM license response data.
	*
	* @return None.
	*/
	void transformLicenseResponse(std::shared_ptr<DrmData> licenseResponse) const override;
	/**
	* @brief Returns a human-readable name for the DRM session.
	*
	* This method provides a friendly name for display or logging purposes.
	*
	* @return Reference to a constant string containing the friendly name (`FRIENDLY_NAME`).
	*/
	virtual const std::string& friendlyName() const override { return FRIENDLY_NAME; };

	ClearKeyHelper(const struct DrmInfo& drmInfo) : DrmHelper(drmInfo), mInitData(), mKeyID(), mPsshStr(),
		CLEARKEY_KEY_ID("1"), FRIENDLY_NAME("Clearkey"), CODEC_TYPE(0), CLEARKEY_DASH_KEY_ID_OFFSET(32u),
		CLEARKEY_DASH_KEY_ID_LEN(16u), KEY_ID_OFFSET(12), KEY_PAYLOAD_OFFSET(14), BASE_16(16)
	{}

	~ClearKeyHelper() { }

private:
	static const std::string CLEARKEY_OCDM_ID;
	const std::string CLEARKEY_KEY_ID;
	const std::string FRIENDLY_NAME;
	const int CODEC_TYPE;
	const size_t CLEARKEY_DASH_KEY_ID_OFFSET; // Offset in the PSSH to find the key ID for DASH
	const size_t CLEARKEY_DASH_KEY_ID_LEN; // Length of the key ID for DASH
	const int KEY_ID_OFFSET;
	const int KEY_PAYLOAD_OFFSET;
	const int BASE_16;

	std::string mPsshStr;
	std::vector<uint8_t> mInitData;
	std::vector<uint8_t> mKeyID;
};

/**
 * @class ClearKeyHelperFactory
 * @brief Helper Factory class to maintain Player DRM data
 */

class ClearKeyHelperFactory : public DrmHelperFactory
{
public:
	static const int CLEARKEY_WEIGHTING = DEFAULT_WEIGHTING * 2;
	/**
	* @brief Creates a DRM helper object based on the provided DRM information.
	*
	* This method instantiates and returns a `DrmHelper` object configured
	* using the given `DrmInfo` structure. The helper can then be used to
	* perform DRM operations such as key acquisition or decryption.
	*
	* @param[in] drmInfo  DRM information structure used to configure the helper.
	*
	* @return Shared pointer to a `DrmHelper` instance.
	*/
	DrmHelperPtr createHelper(const struct DrmInfo& drmInfo) const;
	/**
	* @brief Appends the system IDs supported by this DRM implementation.
	*
	* This method adds the DRM system identifiers to the provided vector.
	* These system IDs can be used to determine compatibility with content
	* or license servers.
	*
	* @param[out] systemIds  Vector to which the system IDs will be appended.
	*
	* @return None.
	*/
	void appendSystemId(std::vector<std::string>& systemIds) const;
	/**
	* @brief Checks whether the provided DRM information corresponds to a DRM-protected session.
	*
	* This method evaluates the given `DrmInfo` structure and determines
	* if it represents a DRM-protected stream or content.
	*
	* @param[in] drmInfo  DRM information structure to check.
	*
	* @return True if the content is DRM-protected, false otherwise.
	*/
	bool isDRM(const struct DrmInfo& drmInfo) const;

public:
	ClearKeyHelperFactory() : DrmHelperFactory(CLEARKEY_WEIGHTING) {};
};

#endif //_CLEARKEY_HELPER_H
