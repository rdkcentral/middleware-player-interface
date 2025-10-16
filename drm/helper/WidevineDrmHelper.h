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
#ifndef _WIDEVINE_DRM_HELPER_H
#define _WIDEVINE_DRM_HELPER_H

/**
 * @file WidevineDrmHelper.h
 * @brief Handles the operation for Wide vine DRM operation
 */

#include <memory>

#include "DrmHelper.h"

/**
 * @class WidevineDrmHelper
 * @brief DRM helper class for Widevine-protected content.
 *
 * This class inherits from DrmHelper and provides functionality to handle
 * Widevine DRM operations including init data creation, PSSH parsing,
 * key retrieval, license requests, and DRM metadata management.
 */

class WidevineDrmHelper: public DrmHelper
{
public:
	friend class WidevineDrmHelperFactory;
	/**
     * @brief Get the OC-DM system ID for Widevine DRM.
     * @return Constant reference to the system ID string.
     */
	virtual const std::string& ocdmSystemId() const override;
    /**
     * @brief Create initialization data required by Widevine DRM.
     * @param initData Vector to be filled with initialization data.
     */
	void createInitData(std::vector<uint8_t>& initData) const override;
    /**
     * @brief Parse a PSSH (Protection System Specific Header) buffer.
     * @param initData Pointer to the PSSH data.
     * @param initDataLen Length of the PSSH data.
     * @return True if parsing is successful, false otherwise.
     */
	bool parsePssh(const uint8_t* initData, uint32_t initDataLen) override;
    /**
     * @brief Checks if content can be decrypted without a license.
     * @return Always false for Widevine DRM.
     */
	bool isClearDecrypt() const override { return false; }
    /**
     * @brief Indicates whether an external license server is used.
     * @return Always false for Widevine DRM.
     */
	bool isExternalLicense() const override { return false; };
    /**
     * @brief Retrieve the DRM key ID for the content.
     * @param keyID Vector to store the key ID.
     */
	void getKey(std::vector<uint8_t>& keyID) const override;
    /**
     * @brief Retrieve multiple DRM keys for the content.
     * @param keyIDs Map of key slot to key ID vectors.
     */
	void getKeys(std::map<int, std::vector<uint8_t>>& keyIDs) const override;
    /**
     * @brief Get DRM metadata associated with the content.
     * @return Constant reference to the DRM metadata string.
     */
	const std::string& getDrmMetaData() const override {return mContentMetadata;}
    /**
     * @brief Set DRM metadata for the content.
     * @param metaData Metadata string to set.
     */
	void setDrmMetaData(const std::string& metaData) override;
    /**
     * @brief Set the default key ID using CENC data.
     * @param cencData CENC-formatted string containing key information.
     */
	void setDefaultKeyID(const std::string& cencData) override;
	/**
	* @brief Get the codec type used by the DRM.
	* @return Codec type as an integer.
	*/
	virtual int getDrmCodecType() const override { return CODEC_TYPE; }
	/**
	* @brief Generate a license request based on the provided challenge information.
	* @param challengeInfo Input challenge information.
	* @param licenseRequest Output license request structure.
	*/
	void generateLicenseRequest(const ChallengeInfo& challengeInfo, LicenseRequest& licenseRequest) const override;
	/**
     * @brief Get the friendly name of this DRM helper.
     * @return Constant reference to the friendly name string.
     */
	virtual const std::string& friendlyName() const override { return FRIENDLY_NAME; };

	WidevineDrmHelper(const struct DrmInfo& drmInfo) : DrmHelper(drmInfo), FRIENDLY_NAME("Widevine"),
		CODEC_TYPE(1), mInitData(), mKeyID(), mKeyIDs(), mContentMetadata(), mDefaultKeySlot(-1)
	{}

	~WidevineDrmHelper() { }

private:
	static const std::string WIDEVINE_OCDM_ID;
	const std::string FRIENDLY_NAME;
	const int CODEC_TYPE;
	std::vector<uint8_t> mInitData;
	std::vector<uint8_t> mKeyID;
	std::map<int,std::vector<uint8_t>> mKeyIDs;
	std::string mContentMetadata;
	int mDefaultKeySlot;
};

/**
 * @class WidevineDrmHelperFactory
 * @brief Helps to handle widevine DRM
 */
class WidevineDrmHelperFactory : public DrmHelperFactory
{
	/**
	* @brief Factory method to create a DRM helper instance for the given DRM info.
	* @param drmInfo DRM information used to create the helper.
	* @return Shared pointer to a newly created DrmHelper instance.
	*/
	DrmHelperPtr createHelper(const struct DrmInfo& drmInfo) const;
	/**
	* @brief Append the system ID(s) supported by this DRM to the provided list.
	* @param systemIds Vector of strings to append the DRM system IDs to.
	*/
	void appendSystemId(std::vector<std::string>& systemIds) const;
	/**
	* @brief Check whether the given DRM info corresponds to DRM-protected content.
	* @param drmInfo DRM information structure to check.
	* @return True if the content is DRM-protected, false otherwise.
	*/
	bool isDRM(const struct DrmInfo& drmInfo) const;
};


#endif //_WIDEVINE_DRM_HELPER_H
