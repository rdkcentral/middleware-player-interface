/*
 * If not stated otherwise in this file or this component's license file the
 * following copyright and licenses apply:
 *
 * Copyright 2024 RDK Management
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

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "opencdmsessionadapter.h"
#include "MockDrmHelper.h"
#include "MockOpenCdm.h"
#include <cstring>
#include "_base64.h"

using ::testing::NiceMock;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::_;

std::shared_ptr<MockDrmHelper> drmHelper;

// For comparing memory buffers such as C-style arrays
MATCHER_P2(MemBufEq, buffer, elementCount, "")
{
	return std::memcmp(arg, buffer, elementCount * sizeof(buffer[0])) == 0;
}

class OCDMSessionAdapterTests : public ::testing::Test
{

protected:
	OCDMSessionAdapter *m_ocdmsessionadapter = nullptr;
	struct OpenCDMSystem *ocdmSystem = (OpenCDMSystem *)0x1234;
	std::string systemId = "com.microsoft.playready";

	void SetUp() override
	{
		drmHelper = std::make_shared<MockDrmHelper>();
		g_mockopencdm = new NiceMock<MockOpenCdm>();

		EXPECT_CALL(*drmHelper, ocdmSystemId()).WillOnce(ReturnRef(systemId));
		EXPECT_CALL(*g_mockopencdm, opencdm_create_system(MemBufEq(systemId.c_str(), systemId.length() + 1))).WillOnce(Return(ocdmSystem));

		m_ocdmsessionadapter = new OCDMSessionAdapter(drmHelper, nullptr);
	}

	void TearDown() override
	{
		delete m_ocdmsessionadapter;
		m_ocdmsessionadapter = nullptr;

		delete g_mockopencdm;
		g_mockopencdm = nullptr;

		drmHelper = nullptr;
	}

public:
};

TEST_F(OCDMSessionAdapterTests, generateDRMSession)
{
	uint8_t initData[] = {0, 1, 2, 3};
	uint8_t initDataLen = sizeof(initData);
	uint32_t f_cbInitData = 99;
	std::string customData = "Custom Data";
	const char *initDataType = "cenc";
	uint8_t initDataTypeLen = strlen(initDataType);

	EXPECT_CALL(*g_mockopencdm,
		opencdm_construct_session(
			ocdmSystem,
			LicenseType::Temporary,
			MemBufEq(initDataType, initDataTypeLen),
			MemBufEq(initData, initDataLen),
			f_cbInitData,
			MemBufEq(customData.c_str(), customData.length()),
			customData.length(),
			_, _, _))
		.WillOnce(Return(ERROR_NONE));

	m_ocdmsessionadapter->generateDRMSession(initData, f_cbInitData, customData);
}

// a second generateDRMSession() must tear down the old session and drop its state.
TEST_F(OCDMSessionAdapterTests, generateDRMSessionRecreateClosesOldSessionAndResetsState)
{
	uint8_t initData[] = {0, 1, 2, 3};
	std::string customData = "Custom Data";
	OpenCDMSession *oldSession = reinterpret_cast<OpenCDMSession *>(0x1111);
	OpenCDMSession *newSession = reinterpret_cast<OpenCDMSession *>(0x2222);
	const uint8_t challenge[] = {'c', 'h', 'a', 'l'};
	const uint8_t key[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};

	{
		::testing::InSequence seq;
		EXPECT_CALL(*g_mockopencdm, opencdm_construct_session(_, _, _, _, _, _, _, _, _, _))
			.WillOnce(::testing::DoAll(::testing::SetArgPointee<9>(oldSession), Return(ERROR_NONE)));
		EXPECT_CALL(*g_mockopencdm, opencdm_session_close(oldSession)).Times(1).WillOnce(Return(ERROR_NONE));
		EXPECT_CALL(*g_mockopencdm, opencdm_destruct_session(oldSession)).Times(1).WillOnce(Return(ERROR_NONE));
		EXPECT_CALL(*g_mockopencdm, opencdm_construct_session(_, _, _, _, _, _, _, _, _, _))
			.WillOnce(::testing::DoAll(::testing::SetArgPointee<9>(newSession), Return(ERROR_NONE)));
		// Closed by the adapter destructor in TearDown().
		EXPECT_CALL(*g_mockopencdm, opencdm_session_close(newSession)).Times(1).WillOnce(Return(ERROR_NONE));
		EXPECT_CALL(*g_mockopencdm, opencdm_destruct_session(newSession)).Times(1).WillOnce(Return(ERROR_NONE));
	}

	m_ocdmsessionadapter->generateDRMSession(initData, sizeof(initData), customData);

	// Seed state that belongs to the old session.
	m_ocdmsessionadapter->processOCDMChallenge("http://old", challenge, sizeof(challenge));
	m_ocdmsessionadapter->keyUpdateOCDM(key, sizeof(key));
	m_ocdmsessionadapter->keysUpdatedOCDM();
	ASSERT_EQ(1u, m_ocdmsessionadapter->getUsableKeys().size());

	m_ocdmsessionadapter->generateDRMSession(initData, sizeof(initData), customData);

	EXPECT_EQ(KEY_INIT, m_ocdmsessionadapter->getState());
	EXPECT_TRUE(m_ocdmsessionadapter->getUsableKeys().empty());

	// The old challenge signal must not be consumed by the replacement session.
	std::string url;
	DrmData *request = m_ocdmsessionadapter->generateKeyRequest(url, 10);
	EXPECT_EQ(nullptr, request);
	delete request;
}

// if the replacement session cannot be constructed, the old one is still released once.
TEST_F(OCDMSessionAdapterTests, generateDRMSessionRecreateFailureReportsCreateFailed)
{
	uint8_t initData[] = {0, 1, 2, 3};
	std::string customData = "Custom Data";
	OpenCDMSession *oldSession = reinterpret_cast<OpenCDMSession *>(0x1111);
	const uint8_t key[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};

	{
		::testing::InSequence seq;
		EXPECT_CALL(*g_mockopencdm, opencdm_construct_session(_, _, _, _, _, _, _, _, _, _))
			.WillOnce(::testing::DoAll(::testing::SetArgPointee<9>(oldSession), Return(ERROR_NONE)));
		EXPECT_CALL(*g_mockopencdm, opencdm_session_close(oldSession)).Times(1).WillOnce(Return(ERROR_NONE));
		EXPECT_CALL(*g_mockopencdm, opencdm_destruct_session(oldSession)).Times(1).WillOnce(Return(ERROR_NONE));
		EXPECT_CALL(*g_mockopencdm, opencdm_construct_session(_, _, _, _, _, _, _, _, _, _))
			.WillOnce(Return(ERROR_UNKNOWN));
	}

	m_ocdmsessionadapter->generateDRMSession(initData, sizeof(initData), customData);
	m_ocdmsessionadapter->keyUpdateOCDM(key, sizeof(key));

	m_ocdmsessionadapter->generateDRMSession(initData, sizeof(initData), customData);

	EXPECT_EQ(KEY_ERROR_SESSION_CREATE_FAILED, m_ocdmsessionadapter->getState());
	EXPECT_TRUE(m_ocdmsessionadapter->getUsableKeys().empty());
}
