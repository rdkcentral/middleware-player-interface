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
#ifndef _VERIMATRIX_DRM_HELPER_H
#define _VERIMATRIX_DRM_HELPER_H

#include <memory>

#include "DrmHelper.h"

/**
* @class VerimatrixHelper
* @brief Helper class for handling Verimatrix DRM operations.
*
* This class inherits from DrmHelper and provides functionality for
* creating init data, parsing PSSH, generating license requests, and
* handling DRM keys and metadata specific to Verimatrix DRM.
*/
class VerimatrixHelper: public DrmHelper
{
public:
	friend class VerimatrixHelperFactory;

	virtual const std::string& ocdmSystemId() const;
	/**
	* @brief Creates initialization data required by the DRM.
	* @param initData Vector to be filled with initialization data.
	*/
	void createInitData(std::vector<uint8_t>& initData) const;
	/**
	* @brief Parse a PSSH (Protection System Specific Header) buffer.
	* @param initData Pointer to the PSSH data.
	* @param initDataLen Length of the PSSH data.
	* @return True if parsing was successful, false otherwise.
	*/
	bool parsePssh(const uint8_t* initData, uint32_t initDataLen);
    /**
     * @brief Checks if the content can be decrypted without a license.
     * @return Always returns false for Verimatrix DRM.
     */
	bool isClearDecrypt() const { return false; }
    /**
     * @brief Indicates if the DRM uses an external license server.
     * @return Always returns true for Verimatrix DRM.
     */
	bool isExternalLicense() const { return true; };
    /**
     * @brief Retrieves the DRM key ID for the content.
     * @param keyID Vector to store the key ID.
     */
	void getKey(std::vector<uint8_t>& keyID) const;
    /**
     * @brief Get DRM metadata associated with the content.
     * @return Constant reference to the DRM metadata string.
     */
	const std::string& getDrmMetaData() const {return mContentMetadata;}
    /**
     * @brief Set DRM metadata for the content.
     * @param metaData Metadata string to set.
     */
	void setDrmMetaData(const std::string& metaData);
    /**
     * @brief Get the codec type used by the DRM.
     * @return Codec type as an integer.
     */
	virtual int getDrmCodecType() const { return CODEC_TYPE; }
    /**
     * @brief Generate a license request for the given challenge information.
     * @param challengeInfo Input challenge information.
     * @param licenseRequest Output license request structure.
     */
	void generateLicenseRequest(const ChallengeInfo& challengeInfo, LicenseRequest& licenseRequest) const;
    /**
     * @brief Transform and handle the license response from the license server.
     * @param licenseResponse Shared pointer to the license response data.
     */
	void transformLicenseResponse(std::shared_ptr<DrmData> licenseResponse) const;
    /**
     * @brief Get the friendly name of this DRM helper.
     * @return Constant reference to the friendly name string.
     */
	virtual const std::string& friendlyName() const override { return FRIENDLY_NAME; };

	VerimatrixHelper(const struct DrmInfo& drmInfo) : DrmHelper(drmInfo), FRIENDLY_NAME("Verimatrix"),
		CODEC_TYPE(1), VERIMATRIX_PSSH_DATA_POSITION(52),
		mInitData(), mKeyID(), mContentMetadata()
	{}

	~VerimatrixHelper() { }

private:
	static const std::string VERIMATRIX_OCDM_ID;
	const std::string FRIENDLY_NAME;
	const int CODEC_TYPE;
	const uint8_t VERIMATRIX_PSSH_DATA_POSITION;
	
	std::vector<uint8_t> mInitData;
	std::vector<uint8_t> mKeyID;
	std::string mContentMetadata;
};

class VerimatrixHelperFactory : public DrmHelperFactory
{
	/**
	* @brief Factory method to create a DRM helper instance.
	* @param drmInfo DRM information used to create the helper.
	* @return Shared pointer to a newly created DrmHelper instance.
	*/
	DrmHelperPtr createHelper(const struct DrmInfo& drmInfo) const;
	/**
	* @brief Append the system ID(s) supported by this DRM to a list.
	* @param systemIds Vector of strings to append the DRM system IDs to.
	*/
	void appendSystemId(std::vector<std::string>& systemIds) const;
	/**
	* @brief Check whether the given DRM info corresponds to a DRM-protected content.
	* @param drmInfo DRM information structure to check.
	* @return True if the content is DRM-protected, false otherwise.
	*/
	bool isDRM(const struct DrmInfo& drmInfo) const;
};


#endif //_VERIMATRIX_DRM_HELPER_H
