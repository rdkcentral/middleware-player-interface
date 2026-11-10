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
using ::testing::DoAll;
using ::testing::Invoke;
using ::testing::SaveArg;
using ::testing::SaveArgPointee;

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

	((*g_mockopencdm).gmock_opencdm_construct_session(ocdmSystem, LicenseType::Temporary, MemBufEq(initDataType, initDataTypeLen), MemBufEq(initData, initDataLen), f_cbInitData, MemBufEq(customData.c_str(), customData.length()), customData.length(),_,_,_))(::testing::internal::GetWithoutMatchers(), nullptr) .InternalExpectedAt("/home/rekha/RDK/latest/l1_Final/aamp/middleware/test/utests/tests/OCDMSessionAdapter/FunctionalTests.cpp", 91, "*g_mockopencdm", "opencdm_construct_session(ocdmSystem, LicenseType::Temporary, MemBufEq(initDataType, initDataTypeLen), MemBufEq(initData, initDataLen), f_cbInitData, MemBufEq(customData.c_str(), customData.length()), customData.length(),_,_,_)").WillOnce(Return(ERROR_NONE));

	m_ocdmsessionadapter->generateDRMSession(initData, f_cbInitData, customData);
}

class TestableOCDMSessionAdapter : public OCDMSessionAdapter
{
public:
	using OCDMSessionAdapter::OCDMSessionAdapter;

	void setStateAndKeyStatus(KeyState state, KeyStatus keyStatus)
	{
		m_eKeyState = state;
		m_keyStatus = keyStatus;
	}
};

class MockDrmCallbacks : public DrmCallbacks
{
public:
	MOCK_METHOD(void, Individualization, (const std::string &payload), (override));
	MOCK_METHOD(void, LicenseRenewal, (DrmHelperPtr drmHelper, void *userData), (override));
	MOCK_METHOD(void, DrmSessionError, (DrmSession *session, const std::string &message), (override));
};

class OCDMSessionAdapterErrorCallbackTests : public ::testing::Test
{
protected:
	struct OpenCDMSystem *ocdmSystem = (OpenCDMSystem *)0x1234;
	std::string systemId = "com.microsoft.playready";
	MockDrmCallbacks m_drmCallbacks;
	std::unique_ptr<TestableOCDMSessionAdapter> m_adapter;
	OpenCDMSessionCallbacks m_registeredCallbacks{};
	void *m_registeredUserData = nullptr;

	void SetUp() override
	{
		drmHelper = std::make_shared<MockDrmHelper>();
		g_mockopencdm = new NiceMock<MockOpenCdm>();

		EXPECT_CALL(*drmHelper, ocdmSystemId()).WillOnce(ReturnRef(systemId));
		EXPECT_CALL(*g_mockopencdm, opencdm_create_system(MemBufEq(systemId.c_str(), systemId.length() + 1)))
			.WillOnce(Return(ocdmSystem));

		m_adapter = std::make_unique<TestableOCDMSessionAdapter>(drmHelper, &m_drmCallbacks);
	}

	void TearDown() override
	{
		m_adapter.reset();
		delete g_mockopencdm;
		g_mockopencdm = nullptr;
		drmHelper = nullptr;
	}

	void registerErrorCallback()
	{
		const uint8_t initData[] = {0, 1, 2, 3};
		std::string customData = "Custom Data";
		EXPECT_CALL(*g_mockopencdm, opencdm_construct_session(_, _, _, _, _, _, _, _, _, _))
			.WillOnce(DoAll(SaveArgPointee<7>(&m_registeredCallbacks), SaveArg<8>(&m_registeredUserData), Return(ERROR_NONE)));

		m_adapter->generateDRMSession(initData, sizeof(initData), customData);
		ASSERT_NE(nullptr, m_registeredCallbacks.error_message_callback);
		EXPECT_EQ(static_cast<void *>(m_adapter.get()), m_registeredUserData);
	}

	void invokeRegisteredErrorCallback(const char message[])
	{
		m_registeredCallbacks.error_message_callback(nullptr, m_registeredUserData, message);
	}
};

TEST_F(OCDMSessionAdapterErrorCallbackTests, ErrorCallbackForwardsAdapterAndMessage)
{
	registerErrorCallback();
	const std::string message = "Rialto Session Server crashed.";
	EXPECT_CALL(m_drmCallbacks, DrmSessionError(m_adapter.get(), message));

	invokeRegisteredErrorCallback(message.c_str());
}

TEST_F(OCDMSessionAdapterErrorCallbackTests, ErrorCallbackConvertsNullMessageToEmptyString)
{
	registerErrorCallback();
	EXPECT_CALL(m_drmCallbacks, DrmSessionError(m_adapter.get(), ""));

	invokeRegisteredErrorCallback(nullptr);
}

TEST_F(OCDMSessionAdapterErrorCallbackTests, InternalErrorTransitionsReadySessionBeforeNotifyingApplication)
{
	registerErrorCallback();
	m_adapter->setStateAndKeyStatus(KEY_READY, InternalError);
	const std::string message = "Rialto Session Server crashed.";
	EXPECT_CALL(m_drmCallbacks, DrmSessionError(m_adapter.get(), message))
		.WillOnce(Invoke([](DrmSession *session, const std::string &) {
			EXPECT_EQ(KEY_ERROR, session->getState());
		}));

	invokeRegisteredErrorCallback(message.c_str());
	EXPECT_EQ(KEY_ERROR, m_adapter->getState());
}

TEST_F(OCDMSessionAdapterErrorCallbackTests, UsableReadySessionRemainsReadyWhenErrorIsForwarded)
{
	registerErrorCallback();
	m_adapter->setStateAndKeyStatus(KEY_READY, Usable);
	const std::string message = "Rialto Session Server crashed.";
	EXPECT_CALL(m_drmCallbacks, DrmSessionError(m_adapter.get(), message))
		.WillOnce(Invoke([](DrmSession *session, const std::string &) {
			EXPECT_EQ(KEY_READY, session->getState());
		}));

	invokeRegisteredErrorCallback(message.c_str());
	EXPECT_EQ(KEY_READY, m_adapter->getState());
}
