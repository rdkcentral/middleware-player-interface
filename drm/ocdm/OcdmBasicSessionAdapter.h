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
 * @file OcdmBasicSessionAdapter.cpp
 * @brief Handles operations on OCDM session
 */
#include "opencdmsessionadapter.h"
#include "DrmHelper.h"

/**
 * @class OCDMBasicSessionAdapter
 * @brief OCDM session Adapter
 */
class OCDMBasicSessionAdapter : public OCDMSessionAdapter
{
public:
	/**
	* @brief Constructor for OCDMBasicSessionAdapter.
	* @param drmHelper Shared pointer to a DrmHelper instance.
	* @param drmCallbacks Pointer to a DrmCallbacks structure.
	*/
	OCDMBasicSessionAdapter(DrmHelperPtr drmHelper, DrmCallbacks *drmCallbacks)
	: OCDMSessionAdapter(drmHelper, drmCallbacks)
	{};
	/**
	* @brief Destructor for OCDMBasicSessionAdapter.
	*/
	~OCDMBasicSessionAdapter() {};
	/**
	* @brief Decrypt payload data using the provided IV.
	* @param f_pbIV Pointer to the initialization vector (IV).
	* @param f_cbIV Size of the IV in bytes.
	* @param payloadData Pointer to the encrypted payload data.
	* @param payloadDataSize Size of the encrypted payload data in bytes.
	* @param ppOpaqueData Pointer to store the decrypted opaque data.
	* @return Integer status code (e.g., 0 for success, non-zero for failure).
	*/
	int decrypt(const uint8_t *f_pbIV, uint32_t f_cbIV, const uint8_t *payloadData, uint32_t payloadDataSize, uint8_t **ppOpaqueData);
};
