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
 * @file VanillaDrmHelper.h
 * @brief Handles the operation foe Vanilla DRM
 */

#ifndef VANILLADRMHELPER_H
#define VANILLADRMHELPER_H

#include "DrmHelper.h"
#include <string>
#include <vector>

/**
 * @class VanillaDrmHelper
 * @brief Provides a basic implementation of DrmHelper for clear (non-DRM) content.
 *
 * This class implements the DrmHelper interface for clear streams that do not
 * require any DRM decryption. It primarily acts as a placeholder for handling
 * generic AES or unprotected content.
 */
class VanillaDrmHelper : public DrmHelper
{
private:
	const std::string EMPTY_STRING;
	public:
	/**
	 * @brief Gets the DRM system ID.
	 *
	 * Since this helper represents a non-DRM use case, it returns an empty string.
	 *
	 * @return const std::string& Empty string representing no DRM system ID.
	 */
	virtual const std::string& ocdmSystemId() const override { return EMPTY_STRING; };
    /**
     * @brief Creates initialization data for DRM sessions.
     *
     * For Vanilla (non-DRM) content, this function performs no operation.
     *
     * @param[out] initData Vector to store initialization data.
     */
	virtual void createInitData(std::vector<uint8_t>& initData){};
	/**
	* @brief Parses the PSSH (Protection System Specific Header) data.
	*
	* Not applicable for non-DRM content; always returns false.
	*
	* @param[in] initData Pointer to the initialization data.
	* @param[in] initDataLen Length of the initialization data.
	* @return false Always returns false for Vanilla content.
	*/
	virtual bool parsePssh(const uint8_t* initData, uint32_t initDataLen) override { return false; };
	/**
	* @brief Checks whether the content can be decrypted without DRM.
	*
	* @return true Always true for clear content.
	*/
	virtual bool isClearDecrypt() { return true; };
	/**
	 * @brief Sets DRM-specific metadata.
	 *
	 * No operation for Vanilla (non-DRM) streams.
	 *
	 * @param[in] metaData DRM metadata string.
	 */
	virtual void setDrmMetaData(const std::string& metaData) override { }
    /**
     * @brief Gets the codec type associated with this DRM helper.
     *
     * @return int Codec type constant for Vanilla DRM (e.g., AES or clear).
     */
	virtual int getDrmCodecType() const override { return CODEC_TYPE; }
	/**
	 * @brief Retrieves the DRM key ID.
	 *
	 * For Vanilla content, clears the key vector since no keys are used.
	 *
	 * @param[out] keyID Vector to store key ID data.
	 */
	virtual void getKey(std::vector<uint8_t>& keyID) { keyID.clear(); };
	/**
	* @brief Checks whether the license is externally handled.
	*
	* @return true Always true for non-DRM content.
	*/
	virtual bool isExternalLicense() const override { return true; };
	/**
	* @brief Generates a DRM license request.
	*
	* No operation for Vanilla DRM helper.
	*
	* @param[in] challengeInfo DRM challenge data.
	* @param[out] licenseRequest License request to be filled.
	*/
	virtual void generateLicenseRequest(const ChallengeInfo& challengeInfo, LicenseRequest& licenseRequest) const override {};
	/**
	* @brief Retrieves the user-friendly DRM system name.
	*
	* @return const std::string& Name "Vanilla_AES".
	*/
	virtual const std::string& friendlyName() const override { return FRIENDLY_NAME; }
	/**
	 * @brief Creates DRM initialization data (const version).
	 *
	 * For Vanilla DRM, this function performs no operation.
	 *
	 * @param[out] initData Vector to store initialization data.
	 */
	virtual void createInitData(std::vector<uint8_t>& initData) const override {};
	/**
	* @brief Checks if clear decryption is supported (const version).
	*
	* @return true Always true for Vanilla DRM.
	*/
	virtual bool isClearDecrypt() const override { return true; };
	/**
	* @brief Retrieves the DRM key ID (const version).
	*
	* For Vanilla DRM, performs no operation.
	*
	* @param[out] keyID Vector to store key ID data.
	*/
	virtual void getKey(std::vector<uint8_t>& keyID) const override {};
	/**
	 * @brief Default constructor for VanillaDrmHelper.
	 *
	 * Initializes DRM info and sets friendly name and codec type.
	 */
	VanillaDrmHelper() : DrmHelper(DrmInfo {}), FRIENDLY_NAME("Vanilla_AES"), CODEC_TYPE(3), EMPTY_STRING() {}

private:
	const std::string FRIENDLY_NAME;
	const int CODEC_TYPE;

};

#endif /* VANILLADRMHELPER_H */

