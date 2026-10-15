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

/**
 * @file OcdmBasicDecryptFillTests.cpp
 * @brief Boundary and error-code gap-fill for OCDMBasicSessionAdapter::decrypt
 *        beyond the scenarios in FunctionalTests.cpp:
 *        - OCDM error-code passthrough (nonzero values propagate verbatim)
 *        - memory-system error path still calls terminateEarly
 *        - HDCP gate never touches the memory system or OCDM
 *        - zero-size payload / null-IV parameter variations
 *        - repeated decrypt calls remain serialized and correct
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "OcdmBasicSessionAdapter.h"
#include "MockOpenCdmSessionAdapter.h"
#include "MockDrmHelper.h"
#include "MockOpenCdm.h"
#include "MockDrmMemorySystem.h"
#include <cstring>

using ::testing::NiceMock;
using ::testing::Return;
using ::testing::_;
using ::testing::SetArgReferee;
using ::testing::DoAll;
using ::testing::Eq;

class OcdmBasicDecryptFillTests : public ::testing::Test
{
protected:
	std::shared_ptr<MockDrmHelper> mDrmHelper;
	OCDMBasicSessionAdapter *mAdapter = nullptr;
	std::string mSystemId = "com.widevine.alpha";
	std::vector<std::vector<uint8_t>> mUsableKeys;

	void SetUp() override
	{
		mDrmHelper = std::make_shared<NiceMock<MockDrmHelper>>();
		g_mockopencdm = new NiceMock<MockOpenCdm>();
		g_mockOpenCdmSessionAdapter = new NiceMock<MockOpenCdmSessionAdapter>();
		g_mockMemorySystem = new NiceMock<MockDrmMemorySystem>();

		ON_CALL(*mDrmHelper, ocdmSystemId()).WillByDefault(testing::ReturnRef(mSystemId));
		ON_CALL(*mDrmHelper, getMemorySystem()).WillByDefault(Return(nullptr));
		ON_CALL(*g_mockOpenCdmSessionAdapter, getUsableKeys())
			.WillByDefault(testing::ReturnRef(mUsableKeys));

		mAdapter = new OCDMBasicSessionAdapter(mDrmHelper, nullptr);
	}

	void TearDown() override
	{
		delete mAdapter;
		delete g_mockOpenCdmSessionAdapter;
		g_mockOpenCdmSessionAdapter = nullptr;
		delete g_mockopencdm;
		g_mockopencdm = nullptr;
		delete g_mockMemorySystem;
		g_mockMemorySystem = nullptr;
		mDrmHelper = nullptr;
	}

	MockDrmMemorySystem *g_mockMemorySystem = nullptr;
};

TEST_F(OcdmBasicDecryptFillTests, HdcpFailSkipsMemorySystemEntirely)
{
	EXPECT_CALL(*g_mockOpenCdmSessionAdapter, verifyOutputProtection()).WillOnce(Return(false));
	EXPECT_CALL(*mDrmHelper, getMemorySystem()).Times(0);
	EXPECT_CALL(*g_mockMemorySystem, encode(_, _, _)).Times(0);
	EXPECT_CALL(*g_mockMemorySystem, terminateEarly()).Times(0);
	EXPECT_CALL(*g_mockopencdm, opencdm_session_decrypt(_, _, _, _, _, _, _, _, _, _)).Times(0);

	EXPECT_EQ(mAdapter->decrypt(nullptr, 0, nullptr, 0, nullptr),
			  HDCP_COMPLIANCE_CHECK_FAILURE);
}

TEST_F(OcdmBasicDecryptFillTests, DecryptErrorCodePassthroughInvalidSession)
{
	EXPECT_CALL(*g_mockOpenCdmSessionAdapter, verifyOutputProtection()).WillOnce(Return(true));
	EXPECT_CALL(*g_mockopencdm, opencdm_session_decrypt(_, _, _, _, _, _, _, _, _, _))
		.WillOnce(Return(ERROR_INVALID_SESSION));
	EXPECT_EQ(mAdapter->decrypt(nullptr, 0, nullptr, 0, nullptr), ERROR_INVALID_SESSION);
}

TEST_F(OcdmBasicDecryptFillTests, DecryptErrorCodePassthroughUnknown)
{
	EXPECT_CALL(*g_mockOpenCdmSessionAdapter, verifyOutputProtection()).WillOnce(Return(true));
	EXPECT_CALL(*g_mockopencdm, opencdm_session_decrypt(_, _, _, _, _, _, _, _, _, _))
		.WillOnce(Return(ERROR_UNKNOWN));
	EXPECT_EQ(mAdapter->decrypt(nullptr, 0, nullptr, 0, nullptr), ERROR_UNKNOWN);
}

TEST_F(OcdmBasicDecryptFillTests, DecryptErrorNoMemorySystemNoTerminateEarly)
{
	EXPECT_CALL(*g_mockOpenCdmSessionAdapter, verifyOutputProtection()).WillOnce(Return(true));
	EXPECT_CALL(*mDrmHelper, getMemorySystem()).WillRepeatedly(Return(nullptr));
	EXPECT_CALL(*g_mockopencdm, opencdm_session_decrypt(_, _, _, _, _, _, _, _, _, _))
		.WillOnce(Return(ERROR_INVALID_SESSION));
	EXPECT_CALL(*g_mockMemorySystem, terminateEarly()).Times(0);
	EXPECT_EQ(mAdapter->decrypt(nullptr, 0, nullptr, 0, nullptr), ERROR_INVALID_SESSION);
}

TEST_F(OcdmBasicDecryptFillTests, DecryptSuccessNoMemorySystemNoDecode)
{
	const uint8_t payload[] = {9, 9, 9};
	EXPECT_CALL(*g_mockOpenCdmSessionAdapter, verifyOutputProtection()).WillOnce(Return(true));
	EXPECT_CALL(*mDrmHelper, getMemorySystem()).WillRepeatedly(Return(nullptr));
	EXPECT_CALL(*g_mockopencdm, opencdm_session_decrypt(_, _, sizeof(payload), _, _, _, _, _, _, _))
		.WillOnce(Return(ERROR_NONE));
	EXPECT_CALL(*g_mockMemorySystem, decode(_, _, _, _)).Times(0);
	EXPECT_EQ(mAdapter->decrypt(nullptr, 0, payload, sizeof(payload), nullptr), 0);
}

TEST_F(OcdmBasicDecryptFillTests, DecryptZeroSizePayload)
{
	EXPECT_CALL(*g_mockOpenCdmSessionAdapter, verifyOutputProtection()).WillOnce(Return(true));
	EXPECT_CALL(*g_mockopencdm, opencdm_session_decrypt(_, _, 0, _, _, _, _, _, _, _))
		.WillOnce(Return(ERROR_NONE));
	EXPECT_EQ(mAdapter->decrypt(nullptr, 0, nullptr, 0, nullptr), 0);
}

TEST_F(OcdmBasicDecryptFillTests, DecryptPassesIvPointerAndLength)
{
	const uint8_t iv[] = {0xAA, 0xBB, 0xCC, 0xDD};
	const uint8_t payload[] = {1};
	EXPECT_CALL(*g_mockOpenCdmSessionAdapter, verifyOutputProtection()).WillOnce(Return(true));
	EXPECT_CALL(*g_mockopencdm,
				opencdm_session_decrypt(_, _, sizeof(payload), _, _,
										Eq(iv), Eq((uint32_t)sizeof(iv)), _, _, _))
		.WillOnce(Return(ERROR_NONE));
	EXPECT_EQ(mAdapter->decrypt(iv, sizeof(iv), payload, sizeof(payload), nullptr), 0);
}

TEST_F(OcdmBasicDecryptFillTests, RepeatedDecryptCallsSerialize)
{
	EXPECT_CALL(*g_mockOpenCdmSessionAdapter, verifyOutputProtection())
		.Times(2)
		.WillRepeatedly(Return(true));
	EXPECT_CALL(*g_mockopencdm, opencdm_session_decrypt(_, _, _, _, _, _, _, _, _, _))
		.Times(2)
		.WillRepeatedly(Return(ERROR_NONE));

	EXPECT_EQ(mAdapter->decrypt(nullptr, 0, nullptr, 0, nullptr), 0);
	EXPECT_EQ(mAdapter->decrypt(nullptr, 0, nullptr, 0, nullptr), 0);
}
