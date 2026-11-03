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
 * @file SecManagerThunderTests.cpp
 * @brief L1 coverage for SecManagerThunder - the SecManager JSON-RPC layer.
 *
 *        The existing ContentsecuritymanagerTests binary only compiles the
 *        ContentSecurityManager base class; SecManagerThunder.cpp itself is
 *        excluded because ThunderAccessPlayer needs WPEFramework.  This suite
 *        compiles SecManagerThunder.cpp against stub WPEFramework headers and
 *        a scripted FakeThunderAccessPlayer, giving deterministic control of
 *        every JSON-RPC call/response.
 *
 *        Coverage focus: the license-acquisition statusCode:reasonCode
 *        matrix - including the 200:1 (general failure, no retry) vs 200:3 /
 *        200:4 / 200:5 (retryable) distinction - session open vs update,
 *        base64 license decode, watermark RPC methods and event handlers.
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <deque>
#include <map>
#include <string>
#include <vector>
#include <cstring>

#include "ContentSecurityManager.h"
#include "SecManagerThunder.h"
#include "ContentSecurityManagerSession.h"
#include "PlayerScheduler.h"
#include "MockPlayerScheduler.h"
#include "_base64.h"

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

using ::testing::_;
using ::testing::DoAll;
using ::testing::Invoke;
using ::testing::Return;
using ::testing::NiceMock;

/* Test-double control block implemented in FakeThunderAccessPlayer.cpp */
struct ThunderCall
{
	std::string callsign;
	std::string method;
	JsonObject param;
};

struct FakeThunderControl
{
	bool activateResult;
	std::deque<std::pair<bool, JsonObject>> responses;
	std::pair<bool, JsonObject> defaultResponse;
	std::vector<ThunderCall> calls;
	std::map<std::string, std::function<void(const WPEFramework::Core::JSON::VariantContainer &)>> handlers;
	std::vector<std::string> unsubscribed;
	bool subscribeResult;
	void reset();
};
extern FakeThunderControl &g_FakeThunder;

namespace
{
	const char *kMoneyTrace[][2] = {{"moneyTrace", "trace123"}, {nullptr, nullptr}};

	JsonObject SuccessResponse(int64_t sessionId, const std::string &licenseB64)
	{
		JsonObject r;
		r["success"] = true;
		r["sessionId"] = sessionId;
		r["license"] = licenseB64;
		return r;
	}

	JsonObject FailureContext(int32_t cls, int32_t reason, int32_t business = 0)
	{
		JsonObject ctx;
		ctx["class"] = cls;
		ctx["reason"] = reason;
		ctx["businessStatus"] = business;
		JsonObject r;
		r["success"] = false;
		r["secManagerResultContext"] = ctx;
		return r;
	}

	std::string B64(const char *raw, size_t len)
	{
		char *enc = base64_Encode(reinterpret_cast<const unsigned char *>(raw), len);
		std::string s = enc ? enc : "";
		free(enc);
		return s;
	}

	const ThunderCall *LastCall()
	{
		return g_FakeThunder.calls.empty() ? nullptr : &g_FakeThunder.calls.back();
	}

	std::vector<ThunderCall> CallsTo(const std::string &method)
	{
		std::vector<ThunderCall> out;
		for (const auto &c : g_FakeThunder.calls)
		{
			if (c.method == method)
			{
				out.push_back(c);
			}
		}
		return out;
	}
}

class TestableSecManagerThunder : public SecManagerThunder
{
public:
	using SecManagerThunder::CreateWatermark;
	using SecManagerThunder::DeleteWatermark;
	using SecManagerThunder::GetWaterMarkPalette;
	using SecManagerThunder::AlwaysShowWatermarkOnTop;
	using SecManagerThunder::ModifyWatermarkPalette;
	using SecManagerThunder::RegisterAllEvents;
	using SecManagerThunder::UnRegisterAllEvents;
	using SecManagerThunder::UpdateWatermark;
	using SecManagerThunder::loadClutWatermark;
};

class SecManagerThunderTests : public ::testing::Test
{
protected:
	TestableSecManagerThunder *mSec = nullptr;

	void SetUp() override
	{
		g_FakeThunder.reset();
		mSec = new TestableSecManagerThunder();
	}

	void TearDown() override
	{
		delete mSec;
		mSec = nullptr;
		g_FakeThunder.reset();
	}

	/** Calls made after construction noise. */
	size_t BaseCallCount() { return g_FakeThunder.calls.size(); }

	JsonObject Acquire(bool &ret, ContentSecurityManagerSession &session,
					   int32_t &status, int32_t &reason, int32_t &business,
					   char **lic, size_t *licLen)
	{
		ret = mSec->AcquireLicenseOpenOrUpdate(
			"player", "app", "http://lic", kMoneyTrace, nullptr,
			"metadata", 8, "licreq", 6, "com.widevine.alpha",
			"stream", "token", 5,
			session, lic, licLen, &status, &reason, &business, false, 1);
		JsonObject p;
		if (!g_FakeThunder.calls.empty())
		{
			p = g_FakeThunder.calls.back().param;
		}
		return p;
	}
};

/* ================================================================== */
/* Construction / teardown                                            */
/* ================================================================== */

TEST_F(SecManagerThunderTests, CtorActivatesPluginAndClosesOldSessions)
{
	// ctor scripted responses were empty -> default success path just logs;
	// verify the startup RPCs happened
	auto closeCalls = CallsTo("closePlaybackSession");
	EXPECT_GE(closeCalls.size(), 1u);
	// first-instance cleanup uses sessionId 0
	EXPECT_EQ(closeCalls[0].param["sessionId"].Number(), 0);
	EXPECT_EQ(closeCalls[0].param["clientId"].String(), "player");
}

TEST_F(SecManagerThunderTests, CtorHidesWatermarkAtStartup)
{
	auto show = CallsTo("showWatermark");
	ASSERT_GE(show.size(), 1u);
	EXPECT_EQ(show[0].param["show"].Boolean(), false);
}

TEST_F(SecManagerThunderTests, CtorRegistersAllWatermarkEvents)
{
	EXPECT_NE(g_FakeThunder.handlers.find("onWatermarkSession"), g_FakeThunder.handlers.end());
	EXPECT_NE(g_FakeThunder.handlers.find("onAddWatermark"), g_FakeThunder.handlers.end());
	EXPECT_NE(g_FakeThunder.handlers.find("onUpdateWatermark"), g_FakeThunder.handlers.end());
	EXPECT_NE(g_FakeThunder.handlers.find("onRemoveWatermark"), g_FakeThunder.handlers.end());
	EXPECT_NE(g_FakeThunder.handlers.find("onDisplayWatermark"), g_FakeThunder.handlers.end());
}

/* ================================================================== */
/* AcquireLicense - input validation                                  */
/* ================================================================== */

TEST_F(SecManagerThunderTests, AcquireEmptyTokenAbortsNoRpc)
{
	size_t before = BaseCallCount();
	ContentSecurityManagerSession session;
	bool ret; int32_t st = -1, rc = -1, bs = -1;
	char *lic = nullptr; size_t licLen = 77;

	ret = mSec->AcquireLicenseOpenOrUpdate("player", "app", "u", kMoneyTrace, nullptr,
										   "meta", 4, "req", 3, "ks", "stream",
										   "", 0, session, &lic, &licLen, &st, &rc, &bs, false, 0);
	EXPECT_FALSE(ret);
	EXPECT_EQ(st, CONTENT_SECURITY_MANAGER_DRM_FAILURE);              /* 200 */
	EXPECT_EQ(rc, CONTENT_SECURITY_MANAGER_DRM_GEN_FAILURE);          /* 200:1 */
	EXPECT_EQ(g_FakeThunder.calls.size(), before);                    /* no RPC */
	EXPECT_EQ(lic, nullptr);
}

TEST_F(SecManagerThunderTests, AcquireNullTokenAbortsNoRpc)
{
	size_t before = BaseCallCount();
	ContentSecurityManagerSession session;
	bool ret; int32_t st, rc, bs; char *lic = nullptr; size_t licLen = 0;
	ret = mSec->AcquireLicenseOpenOrUpdate("player", "app", "u", kMoneyTrace, nullptr,
										   "meta", 4, "req", 3, "ks", "stream",
										   nullptr, 0, session, &lic, &licLen, &st, &rc, &bs, false, 0);
	EXPECT_FALSE(ret);
	EXPECT_EQ(st, 200);
	EXPECT_EQ(rc, 1);
	EXPECT_EQ(g_FakeThunder.calls.size(), before);
}

TEST_F(SecManagerThunderTests, AcquireEmptyMetadataAbortsNoRpc)
{
	size_t before = BaseCallCount();
	ContentSecurityManagerSession session;
	bool ret; int32_t st, rc, bs; char *lic = nullptr; size_t licLen = 0;
	ret = mSec->AcquireLicenseOpenOrUpdate("player", "app", "u", kMoneyTrace, nullptr,
										   "", 0, "req", 3, "ks", "stream",
										   "token", 5, session, &lic, &licLen, &st, &rc, &bs, false, 0);
	EXPECT_FALSE(ret);
	EXPECT_EQ(rc, CONTENT_SECURITY_MANAGER_DRM_GEN_FAILURE);
	EXPECT_EQ(g_FakeThunder.calls.size(), before);
}

TEST_F(SecManagerThunderTests, AcquireEmptyLicenseRequestAbortsNoRpc)
{
	size_t before = BaseCallCount();
	ContentSecurityManagerSession session;
	bool ret; int32_t st, rc, bs; char *lic = nullptr; size_t licLen = 0;
	ret = mSec->AcquireLicenseOpenOrUpdate("player", "app", "u", kMoneyTrace, nullptr,
										   "meta", 4, "", 0, "ks", "stream",
										   "token", 5, session, &lic, &licLen, &st, &rc, &bs, false, 0);
	EXPECT_FALSE(ret);
	EXPECT_EQ(st, 200);
	EXPECT_EQ(rc, 1);
	EXPECT_EQ(g_FakeThunder.calls.size(), before);
}

/* ================================================================== */
/* AcquireLicense - RPC dispatch                                      */
/* ================================================================== */

TEST_F(SecManagerThunderTests, AcquireInvalidSessionUsesOpenPlaybackSession)
{
	g_FakeThunder.responses.push_back({true, SuccessResponse(42, "")});
	ContentSecurityManagerSession session;
	bool ret; int32_t st, rc, bs; char *lic = nullptr; size_t licLen = 0;
	JsonObject p = Acquire(ret, session, st, rc, bs, &lic, &licLen);
	auto openCalls = CallsTo("openPlaybackSession");
	ASSERT_EQ(openCalls.size(), 1u);
	EXPECT_EQ(openCalls[0].callsign, SECMANAGER_CALL_SIGN);
	EXPECT_TRUE(CallsTo("updatePlaybackSession").empty());
}

TEST_F(SecManagerThunderTests, AcquireValidSessionUsesUpdatePlaybackSession)
{
	g_FakeThunder.responses.push_back({true, SuccessResponse(77, "")});
	ContentSecurityManagerSession session(77, 0);
	bool ret; int32_t st, rc, bs; char *lic = nullptr; size_t licLen = 0;
	Acquire(ret, session, st, rc, bs, &lic, &licLen);
	auto updCalls = CallsTo("updatePlaybackSession");
	ASSERT_EQ(updCalls.size(), 1u);
	EXPECT_EQ(updCalls[0].param["sessionId"].Number(), 77);
}

TEST_F(SecManagerThunderTests, AcquireParamFields)
{
	g_FakeThunder.responses.push_back({true, SuccessResponse(9, "")});
	ContentSecurityManagerSession session;
	bool ret; int32_t st, rc, bs; char *lic = nullptr; size_t licLen = 0;
	JsonObject p = Acquire(ret, session, st, rc, bs, &lic, &licLen);

	EXPECT_EQ(p["clientId"].String(), "player");
	EXPECT_EQ(p["keySystem"].String(), "com.widevine.alpha");
	EXPECT_EQ(p["mediaUsage"].String(), "stream");
	EXPECT_EQ(p["accessToken"].String(), "token");
	EXPECT_EQ(p["contentMetadata"].String(), "metadata");
	EXPECT_EQ(p["licenseRequest"].String(), "licreq");
}

TEST_F(SecManagerThunderTests, AcquireSessionConfigurationActive)
{
	g_FakeThunder.responses.push_back({true, SuccessResponse(9, "")});
	ContentSecurityManagerSession session;
	bool ret; int32_t st, rc, bs; char *lic = nullptr; size_t licLen = 0;
	Acquire(ret, session, st, rc, bs, &lic, &licLen);

	const ThunderCall *call = LastCall();
	JsonObject sc = call->param["sessionConfiguration"].Object();
	EXPECT_EQ(sc["distributedTraceType"].String(), "money");
	EXPECT_EQ(sc["distributedTraceId"].String(), "trace123");
	EXPECT_EQ(sc["sessionState"].String(), "active");
}

TEST_F(SecManagerThunderTests, AcquireSessionConfigurationMutedInactive)
{
	g_FakeThunder.responses.push_back({true, SuccessResponse(9, "")});
	ContentSecurityManagerSession session;
	bool ret; int32_t st, rc, bs; char *lic = nullptr; size_t licLen = 0;
	mSec->AcquireLicenseOpenOrUpdate("player", "app", "u", kMoneyTrace, nullptr,
									 "m", 1, "r", 1, "ks", "stream", "t", 1,
									 session, &lic, &licLen, &st, &rc, &bs,
									 /*isVideoMuted*/ true, 0);
	const ThunderCall *call = LastCall();
	JsonObject sc = call->param["sessionConfiguration"].Object();
	EXPECT_EQ(sc["sessionState"].String(), "inactive");
}

TEST_F(SecManagerThunderTests, AcquireAspectDimensionsHardcoded)
{
	g_FakeThunder.responses.push_back({true, SuccessResponse(9, "")});
	ContentSecurityManagerSession session;
	bool ret; int32_t st, rc, bs; char *lic = nullptr; size_t licLen = 0;
	Acquire(ret, session, st, rc, bs, &lic, &licLen);
	const ThunderCall *call = LastCall();
	JsonObject dims = call->param["contentAspectDimensions"].Object();
	EXPECT_EQ(dims["width"].Number(), 1920);
	EXPECT_EQ(dims["height"].Number(), 1080);
}

/* ================================================================== */
/* AcquireLicense - success path                                      */
/* ================================================================== */

TEST_F(SecManagerThunderTests, AcquireSuccessDecodesLicense)
{
	const char raw[] = "LICENSE-BYTES";
	std::string b64 = B64(raw, sizeof(raw) - 1);
	ASSERT_FALSE(b64.empty());
	g_FakeThunder.responses.push_back({true, SuccessResponse(55, b64)});

	ContentSecurityManagerSession session;
	bool ret; int32_t st = -1, rc = -1, bs = -1;
	char *lic = nullptr; size_t licLen = 0;
	Acquire(ret, session, st, rc, bs, &lic, &licLen);

	EXPECT_TRUE(ret);
	ASSERT_NE(lic, nullptr);
	EXPECT_EQ(licLen, sizeof(raw) - 1);
	EXPECT_EQ(std::memcmp(lic, raw, licLen), 0);
	free(lic);
}

TEST_F(SecManagerThunderTests, AcquireSuccessUpdatesInvalidSession)
{
	std::string b64 = B64("x", 1);
	g_FakeThunder.responses.push_back({true, SuccessResponse(4242, b64)});
	ContentSecurityManagerSession session;
	EXPECT_FALSE(session.isSessionValid());
	bool ret; int32_t st, rc, bs; char *lic = nullptr; size_t licLen = 0;
	Acquire(ret, session, st, rc, bs, &lic, &licLen);
	EXPECT_TRUE(ret);
	EXPECT_TRUE(session.isSessionValid());
	EXPECT_EQ(session.getSessionID(), 4242);
	free(lic);
}

TEST_F(SecManagerThunderTests, AcquireSuccessKeepsExistingSession)
{
	std::string b64 = B64("x", 1);
	g_FakeThunder.responses.push_back({true, SuccessResponse(999, b64)});
	ContentSecurityManagerSession session(7, 0);
	bool ret; int32_t st, rc, bs; char *lic = nullptr; size_t licLen = 0;
	Acquire(ret, session, st, rc, bs, &lic, &licLen);
	// existing session is NOT replaced even though response carries a new id
	EXPECT_EQ(session.getSessionID(), 7);
	free(lic);
}

TEST_F(SecManagerThunderTests, AcquireSuccessFalseNoLicenseFails)
{
	JsonObject r;
	r["success"] = false;
	r["secManagerResultContext"] = [] {
		JsonObject c; c["class"] = 200; c["reason"] = 1; c["businessStatus"] = 0; return c;
	}();
	g_FakeThunder.responses.push_back({true, r});

	ContentSecurityManagerSession session;
	bool ret; int32_t st, rc, bs; char *lic = nullptr; size_t licLen = 0;
	Acquire(ret, session, st, rc, bs, &lic, &licLen);
	EXPECT_FALSE(ret);
	EXPECT_EQ(st, 200);
	EXPECT_EQ(rc, 1);
	EXPECT_EQ(lic, nullptr);
}

TEST_F(SecManagerThunderTests, AcquireRpcTransportFailureKeepsDefaults)
{
	// rpcResult == false, empty response object -> default 200:1 preserved
	g_FakeThunder.responses.push_back({false, JsonObject{}});

	ContentSecurityManagerSession session;
	bool ret; int32_t st, rc, bs; char *lic = nullptr; size_t licLen = 0;
	Acquire(ret, session, st, rc, bs, &lic, &licLen);
	EXPECT_FALSE(ret);
	EXPECT_EQ(st, CONTENT_SECURITY_MANAGER_DRM_FAILURE);
	EXPECT_EQ(rc, CONTENT_SECURITY_MANAGER_DRM_GEN_FAILURE);
}

TEST_F(SecManagerThunderTests, AcquireSuccessEmptyLicenseStringFails)
{
	g_FakeThunder.responses.push_back({true, SuccessResponse(5, "")});

	ContentSecurityManagerSession session;
	bool ret; int32_t st, rc, bs; char *lic = nullptr; size_t licLen = 0;
	Acquire(ret, session, st, rc, bs, &lic, &licLen);
	EXPECT_FALSE(ret);
	EXPECT_EQ(lic, nullptr);
}

TEST_F(SecManagerThunderTests, AcquireSuccessGarbageBase64Fails)
{
	// '!!!' is not valid base64 -> base64_Decode yields nothing
	g_FakeThunder.responses.push_back({true, SuccessResponse(5, "!!!")});

	ContentSecurityManagerSession session;
	bool ret; int32_t st, rc, bs; char *lic = nullptr; size_t licLen = 0;
	Acquire(ret, session, st, rc, bs, &lic, &licLen);
	EXPECT_FALSE(ret);
	EXPECT_EQ(lic, nullptr);
}

/* ================================================================== */
/* statusCode:reasonCode matrix - the 200:x retry semantics           */
/*                                                                    */
/* MAX_LICENSE_REQUEST_ATTEMPTS == 2, so a retryable failure produces */
/* exactly 2 JSON-RPC calls before giving up.                         */
/* ================================================================== */

TEST_F(SecManagerThunderTests, Err_200_1_GeneralFailureNoRetry)
{
	g_FakeThunder.responses.push_back({true, FailureContext(200, 1)});
	size_t before = BaseCallCount();
	ContentSecurityManagerSession session;
	bool ret; int32_t st, rc, bs; char *lic = nullptr; size_t licLen = 0;
	Acquire(ret, session, st, rc, bs, &lic, &licLen);
	EXPECT_FALSE(ret);
	EXPECT_EQ(st, 200);
	EXPECT_EQ(rc, 1);
	// 200:1 = general DRM failure -> NOT retryable -> single call
	EXPECT_EQ(g_FakeThunder.calls.size() - before, 1u);
}

TEST_F(SecManagerThunderTests, Err_200_3_TimeoutRetries)
{
	g_FakeThunder.responses.push_back({true, FailureContext(200, 3)});
	g_FakeThunder.responses.push_back({true, FailureContext(200, 3)});
	size_t before = BaseCallCount();
	ContentSecurityManagerSession session;
	bool ret; int32_t st, rc, bs; char *lic = nullptr; size_t licLen = 0;
	Acquire(ret, session, st, rc, bs, &lic, &licLen);
	EXPECT_FALSE(ret);
	EXPECT_EQ(st, 200);
	EXPECT_EQ(rc, 3);
	// 200:3 retryable -> MAX_LICENSE_REQUEST_ATTEMPTS calls
	EXPECT_EQ(g_FakeThunder.calls.size() - before, (size_t)MAX_LICENSE_REQUEST_ATTEMPTS);
}

TEST_F(SecManagerThunderTests, Err_200_4_ConnectionFailureRetries)
{
	g_FakeThunder.responses.push_back({true, FailureContext(200, 4)});
	g_FakeThunder.responses.push_back({true, FailureContext(200, 4)});
	size_t before = BaseCallCount();
	ContentSecurityManagerSession session;
	bool ret; int32_t st, rc, bs; char *lic = nullptr; size_t licLen = 0;
	Acquire(ret, session, st, rc, bs, &lic, &licLen);
	EXPECT_FALSE(ret);
	EXPECT_EQ(st, 200);
	EXPECT_EQ(rc, 4);
	// 200:4 = network connection failure -> retried (vs 200:1 which is not)
	EXPECT_EQ(g_FakeThunder.calls.size() - before, (size_t)MAX_LICENSE_REQUEST_ATTEMPTS);
}

TEST_F(SecManagerThunderTests, Err_200_5_BusyRetries)
{
	g_FakeThunder.responses.push_back({true, FailureContext(200, 5)});
	g_FakeThunder.responses.push_back({true, FailureContext(200, 5)});
	size_t before = BaseCallCount();
	ContentSecurityManagerSession session;
	bool ret; int32_t st, rc, bs; char *lic = nullptr; size_t licLen = 0;
	Acquire(ret, session, st, rc, bs, &lic, &licLen);
	EXPECT_FALSE(ret);
	EXPECT_EQ(rc, 5);
	EXPECT_EQ(g_FakeThunder.calls.size() - before, (size_t)MAX_LICENSE_REQUEST_ATTEMPTS);
}

TEST_F(SecManagerThunderTests, Err_200_2_NoPlaybackSessionNoRetry)
{
	g_FakeThunder.responses.push_back({true, FailureContext(200, 2)});
	size_t before = BaseCallCount();
	ContentSecurityManagerSession session;
	bool ret; int32_t st, rc, bs; char *lic = nullptr; size_t licLen = 0;
	Acquire(ret, session, st, rc, bs, &lic, &licLen);
	EXPECT_FALSE(ret);
	EXPECT_EQ(rc, 2);
	EXPECT_EQ(g_FakeThunder.calls.size() - before, 1u);
}

TEST_F(SecManagerThunderTests, Err_200_8_TokenExpiredNoRetry)
{
	g_FakeThunder.responses.push_back({true, FailureContext(200, 8)});
	size_t before = BaseCallCount();
	ContentSecurityManagerSession session;
	bool ret; int32_t st, rc, bs; char *lic = nullptr; size_t licLen = 0;
	Acquire(ret, session, st, rc, bs, &lic, &licLen);
	EXPECT_FALSE(ret);
	EXPECT_EQ(rc, 8);
	EXPECT_EQ(g_FakeThunder.calls.size() - before, 1u);
}

TEST_F(SecManagerThunderTests, Err_300_4_WmConnectionFailureRetries)
{
	g_FakeThunder.responses.push_back({true, FailureContext(300, 4)});
	g_FakeThunder.responses.push_back({true, FailureContext(300, 4)});
	size_t before = BaseCallCount();
	ContentSecurityManagerSession session;
	bool ret; int32_t st, rc, bs; char *lic = nullptr; size_t licLen = 0;
	Acquire(ret, session, st, rc, bs, &lic, &licLen);
	EXPECT_FALSE(ret);
	EXPECT_EQ(st, 300);
	EXPECT_EQ(rc, 4);
	// watermark class 300 also retries on network reasons
	EXPECT_EQ(g_FakeThunder.calls.size() - before, (size_t)MAX_LICENSE_REQUEST_ATTEMPTS);
}

TEST_F(SecManagerThunderTests, Err_100_4_ApiClassNoRetry)
{
	// same reason 4 but class 100 -> retry gate rejects
	g_FakeThunder.responses.push_back({true, FailureContext(100, 4)});
	size_t before = BaseCallCount();
	ContentSecurityManagerSession session;
	bool ret; int32_t st, rc, bs; char *lic = nullptr; size_t licLen = 0;
	Acquire(ret, session, st, rc, bs, &lic, &licLen);
	EXPECT_FALSE(ret);
	EXPECT_EQ(st, 100);
	EXPECT_EQ(g_FakeThunder.calls.size() - before, 1u);
}

TEST_F(SecManagerThunderTests, Err_400_x_SecClientNoRetry)
{
	g_FakeThunder.responses.push_back({true, FailureContext(400, 5)});
	size_t before = BaseCallCount();
	ContentSecurityManagerSession session;
	bool ret; int32_t st, rc, bs; char *lic = nullptr; size_t licLen = 0;
	Acquire(ret, session, st, rc, bs, &lic, &licLen);
	EXPECT_FALSE(ret);
	EXPECT_EQ(st, 400);
	EXPECT_EQ(g_FakeThunder.calls.size() - before, 1u);
}

TEST_F(SecManagerThunderTests, Err_NoResultContextDefaultsTo200_1NoRetry)
{
	JsonObject r;
	r["success"] = false; // no secManagerResultContext -> defaults 200:1 remain
	g_FakeThunder.responses.push_back({true, r});
	size_t before = BaseCallCount();
	ContentSecurityManagerSession session;
	bool ret; int32_t st, rc, bs; char *lic = nullptr; size_t licLen = 0;
	Acquire(ret, session, st, rc, bs, &lic, &licLen);
	EXPECT_FALSE(ret);
	EXPECT_EQ(st, 200);
	EXPECT_EQ(rc, 1);
	EXPECT_EQ(g_FakeThunder.calls.size() - before, 1u);
}

TEST_F(SecManagerThunderTests, RetryThenSuccessReturnsLicense)
{
	g_FakeThunder.responses.push_back({true, FailureContext(200, 4)});
	std::string b64 = B64("OK", 2);
	g_FakeThunder.responses.push_back({true, SuccessResponse(66, b64)});

	size_t before = BaseCallCount();
	ContentSecurityManagerSession session;
	bool ret; int32_t st, rc, bs; char *lic = nullptr; size_t licLen = 0;
	Acquire(ret, session, st, rc, bs, &lic, &licLen);
	EXPECT_TRUE(ret);
	EXPECT_EQ(g_FakeThunder.calls.size() - before, 2u);
	ASSERT_NE(lic, nullptr);
	EXPECT_EQ(std::memcmp(lic, "OK", 2), 0);
	free(lic);
}

TEST_F(SecManagerThunderTests, BusinessStatusPropagatedFromContext)
{
	g_FakeThunder.responses.push_back({true, FailureContext(200, 1, 4711)});
	ContentSecurityManagerSession session;
	bool ret; int32_t st, rc, bs = -1; char *lic = nullptr; size_t licLen = 0;
	Acquire(ret, session, st, rc, bs, &lic, &licLen);
	EXPECT_FALSE(ret);
	EXPECT_EQ(bs, 4711);
}

TEST_F(SecManagerThunderTests, MissingReasonKeepsDefault1)
{
	JsonObject ctx;
	ctx["class"] = 200; ctx["businessStatus"] = 3;
	JsonObject r;
	r["success"] = false;
	r["secManagerResultContext"] = ctx;
	g_FakeThunder.responses.push_back({true, r});
	ContentSecurityManagerSession session;
	bool ret; int32_t st, rc, bs; char *lic = nullptr; size_t licLen = 0;
	Acquire(ret, session, st, rc, bs, &lic, &licLen);
	EXPECT_FALSE(ret);
	EXPECT_EQ(st, 200);
	EXPECT_EQ(rc, CONTENT_SECURITY_MANAGER_DRM_GEN_FAILURE); // unchanged default
	EXPECT_EQ(bs, 3);
}

/* ================================================================== */
/* ContentSecurityManager::AcquireLicense wrapper                     */
/* ================================================================== */

TEST_F(SecManagerThunderTests, PublicAcquireSameHashUsesSetActiveNotOpen)
{
	// Session valid + matching input hash -> UpdateSessionState path, not
	// openPlaybackSession / updatePlaybackSession
	const char *mt[][2] = {{"m", "t"}, {nullptr, nullptr}};
	auto hash = ContentSecurityManager::getInputSummaryHash(
		mt, "meta", 4, "req", "ks", "stream", "tok", false);
	ContentSecurityManagerSession session(314, hash);

	JsonObject ok;
	ok["success"] = true;
	g_FakeThunder.responses.push_back({true, ok});

	bool ret; int32_t st, rc, bs; char *lic = nullptr; size_t licLen = 0;
	ret = mSec->AcquireLicense("player", "app", "u", mt, nullptr,
							   "meta", 4, "req", 3, "ks", "stream",
							   "tok", 3, session, &lic, &licLen,
							   &st, &rc, &bs, false, 0);
	EXPECT_TRUE(ret);
	EXPECT_EQ(CallsTo("setPlaybackSessionState").size(), 1u);
	EXPECT_TRUE(CallsTo("openPlaybackSession").empty());
	EXPECT_TRUE(CallsTo("updatePlaybackSession").empty());
}

TEST_F(SecManagerThunderTests, PublicAcquireDifferentHashOpensSession)
{
	const char *mt[][2] = {{"m", "t"}, {nullptr, nullptr}};
	ContentSecurityManagerSession session(555, 0); // hash won't match
	std::string b64 = B64("L", 1);
	g_FakeThunder.responses.push_back({true, SuccessResponse(555, b64)});

	bool ret; int32_t st, rc, bs; char *lic = nullptr; size_t licLen = 0;
	ret = mSec->AcquireLicense("player", "app", "u", mt, nullptr,
							   "meta", 4, "req", 3, "ks", "stream",
							   "tok", 3, session, &lic, &licLen,
							   &st, &rc, &bs, false, 0);
	EXPECT_EQ(CallsTo("updatePlaybackSession").size(), 1u); // session was valid -> update path
	free(lic);
}

TEST_F(SecManagerThunderTests, PublicAcquireInvalidSessionOpensSession)
{
	const char *mt[][2] = {{"m", "t"}, {nullptr, nullptr}};
	ContentSecurityManagerSession session;
	std::string b64 = B64("L", 1);
	g_FakeThunder.responses.push_back({true, SuccessResponse(101, b64)});

	bool ret; int32_t st, rc, bs; char *lic = nullptr; size_t licLen = 0;
	ret = mSec->AcquireLicense("player", "app", "u", mt, nullptr,
							   "meta", 4, "req", 3, "ks", "stream",
							   "tok", 3, session, &lic, &licLen,
							   &st, &rc, &bs, false, 0);
	EXPECT_TRUE(ret);
	EXPECT_EQ(CallsTo("openPlaybackSession").size(), 1u);
	free(lic);
}

/* ================================================================== */
/* SetDrmSessionState                                                 */
/* ================================================================== */

TEST_F(SecManagerThunderTests, SetDrmSessionStateActiveParam)
{
	JsonObject ok;
	ok["success"] = true;
	g_FakeThunder.responses.push_back({true, ok});
	EXPECT_TRUE(mSec->SetDrmSessionState(42, true));
	const ThunderCall *c = LastCall();
	EXPECT_EQ(c->method, "setPlaybackSessionState");
	EXPECT_EQ(c->param["sessionId"].Number(), 42);
	EXPECT_EQ(c->param["sessionState"].String(), "active");
	EXPECT_EQ(c->param["clientId"].String(), "player");
}

TEST_F(SecManagerThunderTests, SetDrmSessionStateInactiveParam)
{
	JsonObject ok;
	ok["success"] = true;
	g_FakeThunder.responses.push_back({true, ok});
	EXPECT_TRUE(mSec->SetDrmSessionState(43, false));
	EXPECT_EQ(LastCall()->param["sessionState"].String(), "inactive");
}

TEST_F(SecManagerThunderTests, SetDrmSessionStateRpcFailureReturnsFalse)
{
	g_FakeThunder.responses.push_back({false, JsonObject{}});
	EXPECT_FALSE(mSec->SetDrmSessionState(44, true));
}

TEST_F(SecManagerThunderTests, SetDrmSessionStateSuccessFalseReturnsFalse)
{
	JsonObject r;
	r["success"] = false;
	g_FakeThunder.responses.push_back({true, r});
	EXPECT_FALSE(mSec->SetDrmSessionState(45, true));
}

TEST_F(SecManagerThunderTests, SetDrmSessionStateMissingSuccessReturnsFalse)
{
	g_FakeThunder.responses.push_back({true, JsonObject{}}); // no "success" label
	EXPECT_FALSE(mSec->SetDrmSessionState(46, true));
}

/* ================================================================== */
/* CloseDrmSession                                                    */
/* ================================================================== */

TEST_F(SecManagerThunderTests, CloseDrmSessionSendsParams)
{
	JsonObject ok;
	ok["success"] = true;
	g_FakeThunder.responses.push_back({true, ok});
	mSec->CloseDrmSession(88);
	auto c = CallsTo("closePlaybackSession");
	ASSERT_GE(c.size(), 1u);
	EXPECT_EQ(c.back().param["sessionId"].Number(), 88);
	EXPECT_EQ(c.back().param["clientId"].String(), "player");
}

TEST_F(SecManagerThunderTests, CloseDrmSessionRpcFailureTolerated)
{
	g_FakeThunder.responses.push_back({false, JsonObject{}});
	EXPECT_NO_FATAL_FAILURE(mSec->CloseDrmSession(89));
}

TEST_F(SecManagerThunderTests, CloseDrmSessionSuccessFalseTolerated)
{
	JsonObject r;
	r["success"] = false;
	g_FakeThunder.responses.push_back({true, r});
	EXPECT_NO_FATAL_FAILURE(mSec->CloseDrmSession(90));
}

/* ================================================================== */
/* setWindowSize / SetPlaybackPosition                                */
/* ================================================================== */

TEST_F(SecManagerThunderTests, SetWindowSizeSendsParams)
{
	JsonObject ok;
	ok["success"] = true;
	g_FakeThunder.responses.push_back({true, ok});
	EXPECT_TRUE(mSec->setWindowSize(12, 1920, 1080));
	const ThunderCall *c = LastCall();
	EXPECT_EQ(c->method, "setVideoWindowSize");
	EXPECT_EQ(c->param["sessionId"].Number(), 12);
	EXPECT_EQ(c->param["videoWidth"].Number(), 1920);
	EXPECT_EQ(c->param["videoHeight"].Number(), 1080);
}

TEST_F(SecManagerThunderTests, SetWindowSizeRpcFailReturnsFalse)
{
	g_FakeThunder.responses.push_back({false, JsonObject{}});
	EXPECT_FALSE(mSec->setWindowSize(1, 2, 3));
}

TEST_F(SecManagerThunderTests, SetWindowSizeSuccessFalseReturnsFalse)
{
	JsonObject r;
	r["success"] = false;
	g_FakeThunder.responses.push_back({true, r});
	EXPECT_FALSE(mSec->setWindowSize(1, 2, 3));
}

TEST_F(SecManagerThunderTests, SetPlaybackPositionSendsParams)
{
	JsonObject ok;
	ok["success"] = true;
	g_FakeThunder.responses.push_back({true, ok});
	mSec->SetPlaybackPosition(21, 2.0f, 12345);
	const ThunderCall *c = LastCall();
	EXPECT_EQ(c->method, "setPlaybackSpeedState");
	EXPECT_EQ(c->param["sessionId"].Number(), 21);
	EXPECT_EQ(c->param["playbackSpeed"].Number(), 2);
	EXPECT_EQ(c->param["playbackPosition"].Number(), 12345);
}

TEST_F(SecManagerThunderTests, SetPlaybackPositionRpcFailHandled)
{
	g_FakeThunder.responses.push_back({false, JsonObject{}});
	EXPECT_NO_FATAL_FAILURE(mSec->SetPlaybackPosition(22, 1.0f, 0));
}

/* ================================================================== */
/* getSessionToken                                                    */
/* ================================================================== */

TEST_F(SecManagerThunderTests, GetSessionTokenSuccess)
{
	JsonObject r;
	r["token"] = "tok-abc";
	g_FakeThunder.responses.push_back({true, r});
	std::string token;
	EXPECT_TRUE(mSec->getSessionToken(token));
	EXPECT_EQ(token, "tok-abc");
	EXPECT_EQ(LastCall()->callsign, AUTH_SERVICE_CALL_SIGN);
	EXPECT_EQ(LastCall()->method, "getSessionToken");
}

TEST_F(SecManagerThunderTests, GetSessionTokenRpcFail)
{
	g_FakeThunder.responses.push_back({false, JsonObject{}});
	std::string token = "unchanged";
	EXPECT_FALSE(mSec->getSessionToken(token));
	EXPECT_EQ(token, "unchanged");
}

/* ================================================================== */
/* Watermark RPC methods                                              */
/* ================================================================== */

TEST_F(SecManagerThunderTests, ShowWatermarkTrueParam)
{
	JsonObject ok;
	ok["success"] = true;
	g_FakeThunder.responses.push_back({true, ok});
	mSec->ShowWatermark(true);
	EXPECT_EQ(LastCall()->method, "showWatermark");
	EXPECT_EQ(LastCall()->param["show"].Boolean(), true);
	EXPECT_EQ(LastCall()->callsign, WATERMARK_PLUGIN_CALLSIGN);
}

TEST_F(SecManagerThunderTests, ShowWatermarkFalseParam)
{
	JsonObject ok;
	ok["success"] = true;
	g_FakeThunder.responses.push_back({true, ok});
	mSec->ShowWatermark(false);
	EXPECT_EQ(LastCall()->param["show"].Boolean(), false);
}

TEST_F(SecManagerThunderTests, AlwaysShowWatermarkOnTopSendsRpc)
{
	JsonObject ok;
	ok["success"] = true;
	g_FakeThunder.responses.push_back({true, ok});
	mSec->AlwaysShowWatermarkOnTop(true);
	EXPECT_EQ(LastCall()->method, "alwaysShowWatermarkOnTop");
	EXPECT_EQ(LastCall()->param["show"].Boolean(), true);
}

TEST_F(SecManagerThunderTests, CreateWatermarkSendsIdAndZorder)
{
	JsonObject ok;
	ok["success"] = true;
	g_FakeThunder.responses.push_back({true, ok});
	mSec->CreateWatermark(7, 3);
	EXPECT_EQ(LastCall()->method, "createWatermark");
	EXPECT_EQ(LastCall()->param["id"].Number(), 7);
	EXPECT_EQ(LastCall()->param["zorder"].Number(), 3);
}

TEST_F(SecManagerThunderTests, DeleteWatermarkSendsId)
{
	JsonObject ok;
	ok["success"] = true;
	g_FakeThunder.responses.push_back({true, ok});
	mSec->DeleteWatermark(9);
	EXPECT_EQ(LastCall()->method, "deleteWatermark");
	EXPECT_EQ(LastCall()->param["id"].Number(), 9);
}

TEST_F(SecManagerThunderTests, ModifyWatermarkPaletteParams)
{
	JsonObject ok;
	ok["success"] = true;
	g_FakeThunder.responses.push_back({true, ok});
	mSec->ModifyWatermarkPalette(4, 10, 20);
	EXPECT_EQ(LastCall()->method, "modifyPalettedWatermark");
	EXPECT_EQ(LastCall()->param["id"].Number(), 4);
	EXPECT_EQ(LastCall()->param["clutKey"].Number(), 10);
	EXPECT_EQ(LastCall()->param["imageKey"].Number(), 20);
}

TEST_F(SecManagerThunderTests, GetWaterMarkPaletteSendsId)
{
	JsonObject ok;
	ok["success"] = false;
	g_FakeThunder.responses.push_back({true, ok});
	mSec->GetWaterMarkPalette(100, 6);
	EXPECT_EQ(LastCall()->method, "getPalettedWatermark");
	EXPECT_EQ(LastCall()->param["id"].Number(), 6);
}

TEST_F(SecManagerThunderTests, UpdateWatermarkSendsUpdateRpc)
{
	JsonObject ok;
	ok["success"] = true;
	g_FakeThunder.responses.push_back({true, ok});
	mSec->UpdateWatermark(8, 512, 2048);
	const ThunderCall *c = LastCall();
	EXPECT_EQ(c->method, "updateWatermark");
	EXPECT_EQ(c->param["id"].Number(), 8);
	EXPECT_EQ(c->param["key"].Number(), 512);
	EXPECT_EQ(c->param["size"].Number(), 2048);
}

TEST_F(SecManagerThunderTests, LoadClutWatermarkParams)
{
	JsonObject ok;
	ok["success"] = true;
	g_FakeThunder.responses.push_back({true, ok});
	bool rc = mSec->loadClutWatermark(5, 6, 11, 22, 256, "rgba", 640, 480, 1.5f);
	(void)rc;
	const ThunderCall *c = LastCall();
	EXPECT_EQ(c->method, "loadClutWatermark");
	EXPECT_EQ(c->param["sessionId"].Number(), 5);
	EXPECT_EQ(c->param["graphicId"].Number(), 6);
	EXPECT_EQ(c->param["watermarkClutBufferKey"].Number(), 11);
	EXPECT_EQ(c->param["watermarkImageBufferKey"].Number(), 22);
	EXPECT_EQ(c->param["clutPaletteSize"].Number(), 256);
	EXPECT_EQ(c->param["clutPaletteFormat"].String(), "rgba");
	EXPECT_EQ(c->param["watermarkWidth"].Number(), 640);
	EXPECT_EQ(c->param["watermarkHeight"].Number(), 480);
	EXPECT_EQ(c->param["aspectRatio"].String(), std::to_string(1.5f));
}

TEST_F(SecManagerThunderTests, LoadClutNullFormatTolerated)
{
	JsonObject ok;
	ok["success"] = true;
	g_FakeThunder.responses.push_back({true, ok});
	EXPECT_NO_FATAL_FAILURE(
		mSec->loadClutWatermark(1, 2, 3, 4, 0, nullptr, 0, 0, 0.0f));
}

/* ================================================================== */
/* Event handlers driven through the recorded subscriptions           */
/* ================================================================== */

TEST_F(SecManagerThunderTests, WatermarkSessionHandlerInvokesCallback)
{
	bool called = false;
	uint32_t sid = 0, ctx = 0;
	std::string wsys;
	ContentSecurityManager::setWatermarkSessionEvent_CB(
		[&](uint32_t s, uint32_t c, const std::string &w)
		{ called = true; sid = s; ctx = c; wsys = w; });

	JsonObject params;
	params["sessionId"] = 77;
	params["conditionContext"] = 3;
	params["watermarkingSystem"] = "Widevine";
	g_FakeThunder.handlers["onWatermarkSession"](params);

	EXPECT_TRUE(called);
	EXPECT_EQ(sid, 77u);
	EXPECT_EQ(ctx, 3u);
	EXPECT_EQ(wsys, "Widevine");
	ContentSecurityManager::setWatermarkSessionEvent_CB(nullptr);
}

TEST_F(SecManagerThunderTests, WatermarkSessionHandlerNoCallbackNoCrash)
{
	ContentSecurityManager::setWatermarkSessionEvent_CB(nullptr);
	JsonObject params;
	params["sessionId"] = 1;
	params["conditionContext"] = 0;
	EXPECT_NO_FATAL_FAILURE(g_FakeThunder.handlers["onWatermarkSession"](params));
}

TEST_F(SecManagerThunderTests, AddWatermarkHandlerSchedulesCreateAndUpdate)
{
	NiceMock<MockPlayerScheduler> mockSched;
	g_mockPlayerScheduler = &mockSched;
	std::vector<PlayerAsyncTaskObj> tasks;
	EXPECT_CALL(mockSched, ScheduleTask(_))
		.WillRepeatedly(DoAll(Invoke([&](PlayerAsyncTaskObj o) { tasks.push_back(o); }),
							  Return(1)));

	JsonObject params;
	params["graphicId"] = 15;
	params["zIndex"] = 2;
	params["graphicImageBufferKey"] = 700;
	params["graphicImageSize"] = 1024;
	params["adjustVisibilityRequired"] = false;
	params["sessionId"] = 33;
	g_FakeThunder.handlers["onAddWatermark"](params);

	EXPECT_EQ(tasks.size(), 2u); // CreateWatermark + UpdateWatermark tasks
	g_mockPlayerScheduler = nullptr;
}

TEST_F(SecManagerThunderTests, AddWatermarkTaskExecutesCreateWatermarkRpc)
{
	NiceMock<MockPlayerScheduler> mockSched;
	g_mockPlayerScheduler = &mockSched;
	std::vector<PlayerAsyncTaskObj> tasks;
	EXPECT_CALL(mockSched, ScheduleTask(_))
		.WillRepeatedly(DoAll(Invoke([&](PlayerAsyncTaskObj o) { tasks.push_back(o); }),
							  Return(1)));

	JsonObject params;
	params["graphicId"] = 15;
	params["zIndex"] = 2;
	params["graphicImageBufferKey"] = 700;
	params["graphicImageSize"] = 1024;
	params["adjustVisibilityRequired"] = false;
	params["sessionId"] = 33;
	g_FakeThunder.handlers["onAddWatermark"](params);
	g_mockPlayerScheduler = nullptr;

	ASSERT_GE(tasks.size(), 1u);
	size_t before = BaseCallCount();
	tasks[0].mTask(tasks[0].mData); // runs CreateWatermark(15,2) on the singleton
	EXPECT_GT(g_FakeThunder.calls.size(), before);
	bool sawCreate = !CallsTo("createWatermark").empty();
	EXPECT_TRUE(sawCreate);
}

TEST_F(SecManagerThunderTests, UpdateWatermarkHandlerSchedulesTask)
{
	NiceMock<MockPlayerScheduler> mockSched;
	g_mockPlayerScheduler = &mockSched;
	int calls = 0;
	EXPECT_CALL(mockSched, ScheduleTask(_)).WillRepeatedly(
		DoAll(Invoke([&](PlayerAsyncTaskObj) { ++calls; }), Return(1)));

	JsonObject params;
	params["graphicId"] = 4;
	params["watermarkClutBufferKey"] = 10;
	params["watermarkImageBufferKey"] = 20;
	g_FakeThunder.handlers["onUpdateWatermark"](params);
	EXPECT_EQ(calls, 1);
	g_mockPlayerScheduler = nullptr;
}

TEST_F(SecManagerThunderTests, RemoveWatermarkHandlerSchedulesDelete)
{
	NiceMock<MockPlayerScheduler> mockSched;
	g_mockPlayerScheduler = &mockSched;
	int calls = 0;
	EXPECT_CALL(mockSched, ScheduleTask(_)).WillRepeatedly(
		DoAll(Invoke([&](PlayerAsyncTaskObj) { ++calls; }), Return(1)));

	JsonObject params;
	params["graphicId"] = 4;
	g_FakeThunder.handlers["onRemoveWatermark"](params);
	EXPECT_EQ(calls, 1);
	g_mockPlayerScheduler = nullptr;
}

TEST_F(SecManagerThunderTests, ShowWatermarkHandlerShowPathSchedulesTask)
{
	NiceMock<MockPlayerScheduler> mockSched;
	g_mockPlayerScheduler = &mockSched;
	std::vector<PlayerAsyncTaskObj> tasks;
	EXPECT_CALL(mockSched, ScheduleTask(_))
		.WillRepeatedly(DoAll(Invoke([&](PlayerAsyncTaskObj o) { tasks.push_back(o); }),
							  Return(1)));

	JsonObject params;
	params["hideWatermark"] = false;
	g_FakeThunder.handlers["onDisplayWatermark"](params);
	EXPECT_EQ(tasks.size(), 1u);
	g_mockPlayerScheduler = nullptr;
}

TEST_F(SecManagerThunderTests, ShowWatermarkHandlerHidePathSchedulesTask)
{
	NiceMock<MockPlayerScheduler> mockSched;
	g_mockPlayerScheduler = &mockSched;
	std::vector<PlayerAsyncTaskObj> tasks;
	EXPECT_CALL(mockSched, ScheduleTask(_))
		.WillRepeatedly(DoAll(Invoke([&](PlayerAsyncTaskObj o) { tasks.push_back(o); }),
							  Return(1)));

	JsonObject params;
	params["hideWatermark"] = true;
	g_FakeThunder.handlers["onDisplayWatermark"](params);
	EXPECT_EQ(tasks.size(), 1u);
	g_mockPlayerScheduler = nullptr;
}

/* ================================================================== */
/* Misc                                                               */
/* ================================================================== */

TEST_F(SecManagerThunderTests, GetSchedulerStatusTrueAfterCtor)
{
	EXPECT_TRUE(mSec->getSchedulerStatus());
}

TEST_F(SecManagerThunderTests, GetInstanceReturnsSecManagerThunder)
{
	auto *inst = ContentSecurityManager::GetInstance();
	ASSERT_NE(inst, nullptr);
	EXPECT_NE(dynamic_cast<SecManagerThunder *>(inst), nullptr);
}

TEST_F(SecManagerThunderTests, RegisterUnregisterAllEvents)
{
	mSec->UnRegisterAllEvents();
	EXPECT_TRUE(g_FakeThunder.handlers.empty());
	for (const char *evt : {"onWatermarkSession", "onAddWatermark",
						  "onUpdateWatermark", "onRemoveWatermark", "onDisplayWatermark"})
	{
		EXPECT_NE(std::find(g_FakeThunder.unsubscribed.begin(),
							g_FakeThunder.unsubscribed.end(), evt),
				  g_FakeThunder.unsubscribed.end()) << evt;
	}

	mSec->RegisterAllEvents();
	EXPECT_EQ(g_FakeThunder.handlers.size(), 5u);
}

/* ================================================================== */
/* Retry-gate boundaries: class must be 200 OR 300 AND reason must be */
/* 3/4/5. These pin that neither side alone is sufficient.            */
/* ================================================================== */

TEST_F(SecManagerThunderTests, Err_100_3_TimeoutReasonWrongClassNoRetry)
{
	g_FakeThunder.responses.push_back({true, FailureContext(100, 3)});
	size_t before = BaseCallCount();
	ContentSecurityManagerSession session;
	bool ret; int32_t st, rc, bs; char *lic = nullptr; size_t licLen = 0;
	Acquire(ret, session, st, rc, bs, &lic, &licLen);
	EXPECT_FALSE(ret);
	EXPECT_EQ(st, 100);
	EXPECT_EQ(rc, 3);
	EXPECT_EQ(g_FakeThunder.calls.size() - before, 1u);
}

TEST_F(SecManagerThunderTests, Err_300_3_WmTimeoutRetries)
{
	g_FakeThunder.responses.push_back({true, FailureContext(300, 3)});
	g_FakeThunder.responses.push_back({true, FailureContext(300, 3)});
	size_t before = BaseCallCount();
	ContentSecurityManagerSession session;
	bool ret; int32_t st, rc, bs; char *lic = nullptr; size_t licLen = 0;
	Acquire(ret, session, st, rc, bs, &lic, &licLen);
	EXPECT_FALSE(ret);
	EXPECT_EQ(st, 300);
	EXPECT_EQ(rc, 3);
	EXPECT_EQ(g_FakeThunder.calls.size() - before, (size_t)MAX_LICENSE_REQUEST_ATTEMPTS);
}

TEST_F(SecManagerThunderTests, Err_300_5_WmBusyRetries)
{
	g_FakeThunder.responses.push_back({true, FailureContext(300, 5)});
	g_FakeThunder.responses.push_back({true, FailureContext(300, 5)});
	size_t before = BaseCallCount();
	ContentSecurityManagerSession session;
	bool ret; int32_t st, rc, bs; char *lic = nullptr; size_t licLen = 0;
	Acquire(ret, session, st, rc, bs, &lic, &licLen);
	EXPECT_FALSE(ret);
	EXPECT_EQ(rc, 5);
	EXPECT_EQ(g_FakeThunder.calls.size() - before, (size_t)MAX_LICENSE_REQUEST_ATTEMPTS);
}

TEST_F(SecManagerThunderTests, Err_200_9_DeviceTokenExpiredNoRetry)
{
	g_FakeThunder.responses.push_back({true, FailureContext(200, 9)});
	size_t before = BaseCallCount();
	ContentSecurityManagerSession session;
	bool ret; int32_t st, rc, bs; char *lic = nullptr; size_t licLen = 0;
	Acquire(ret, session, st, rc, bs, &lic, &licLen);
	EXPECT_FALSE(ret);
	EXPECT_EQ(rc, 9);
	EXPECT_EQ(g_FakeThunder.calls.size() - before, 1u);
}

TEST_F(SecManagerThunderTests, RetryExhaustionLeavesQueuedResponses)
{
	// Only MAX_LICENSE_REQUEST_ATTEMPTS calls are made; further queued
	// responses are never consumed.
	g_FakeThunder.responses.push_back({true, FailureContext(200, 4)});
	g_FakeThunder.responses.push_back({true, FailureContext(200, 4)});
	g_FakeThunder.responses.push_back({true, FailureContext(200, 4)});
	size_t before = BaseCallCount();
	ContentSecurityManagerSession session;
	bool ret; int32_t st, rc, bs; char *lic = nullptr; size_t licLen = 0;
	Acquire(ret, session, st, rc, bs, &lic, &licLen);
	EXPECT_FALSE(ret);
	EXPECT_EQ(g_FakeThunder.calls.size() - before, (size_t)MAX_LICENSE_REQUEST_ATTEMPTS);
	EXPECT_EQ(g_FakeThunder.responses.size(), 1u);
}

TEST_F(SecManagerThunderTests, RetryCountResetsAcrossAcquireCalls)
{
	// A fresh Acquire must get the full retry budget again even though a
	// previous Acquire exhausted it.
	g_FakeThunder.responses.push_back({true, FailureContext(200, 3)});
	g_FakeThunder.responses.push_back({true, FailureContext(200, 3)});
	ContentSecurityManagerSession session;
	bool ret; int32_t st, rc, bs; char *lic = nullptr; size_t licLen = 0;
	Acquire(ret, session, st, rc, bs, &lic, &licLen);
	EXPECT_FALSE(ret);

	g_FakeThunder.responses.push_back({true, FailureContext(200, 3)});
	std::string b64 = B64("OK", 2);
	g_FakeThunder.responses.push_back({true, SuccessResponse(77, b64)});
	size_t before = BaseCallCount();
	Acquire(ret, session, st, rc, bs, &lic, &licLen);
	EXPECT_TRUE(ret);
	EXPECT_EQ(g_FakeThunder.calls.size() - before, 2u);
	if (lic) free(lic);
}

/* ================================================================== */
/* ContentSecurityManager base-class wrappers -> virtual dispatch     */
/* ================================================================== */

TEST_F(SecManagerThunderTests, UpdateSessionStateWrapsSetDrmSessionState)
{
	JsonObject ok;
	ok["success"] = true;
	g_FakeThunder.responses.push_back({true, ok});
	EXPECT_TRUE(mSec->UpdateSessionState(51, true));
	const ThunderCall *c = LastCall();
	EXPECT_EQ(c->method, "setPlaybackSessionState");
	EXPECT_EQ(c->param["sessionId"].Number(), 51);
	EXPECT_EQ(c->param["sessionState"].String(), "active");
}

TEST_F(SecManagerThunderTests, UpdateSessionStateRpcFailReturnsFalse)
{
	g_FakeThunder.responses.push_back({false, JsonObject{}});
	EXPECT_FALSE(mSec->UpdateSessionState(52, false));
}

TEST_F(SecManagerThunderTests, ReleaseSessionWrapsCloseDrmSession)
{
	JsonObject ok;
	ok["success"] = true;
	g_FakeThunder.responses.push_back({true, ok});
	mSec->ReleaseSession(93);
	auto c = CallsTo("closePlaybackSession");
	ASSERT_GE(c.size(), 1u);
	EXPECT_EQ(c.back().param["sessionId"].Number(), 93);
}

TEST_F(SecManagerThunderTests, SetVideoWindowSizeWrapperSendsParams)
{
	JsonObject ok;
	ok["success"] = true;
	g_FakeThunder.responses.push_back({true, ok});
	EXPECT_TRUE(mSec->setVideoWindowSize(14, 1280, 720));
	const ThunderCall *c = LastCall();
	EXPECT_EQ(c->method, "setVideoWindowSize");
	EXPECT_EQ(c->param["sessionId"].Number(), 14);
	EXPECT_EQ(c->param["videoWidth"].Number(), 1280);
	EXPECT_EQ(c->param["videoHeight"].Number(), 720);
}

TEST_F(SecManagerThunderTests, SetPlaybackSpeedStateWrapperSendsParams)
{
	JsonObject ok;
	ok["success"] = true;
	g_FakeThunder.responses.push_back({true, ok});
	EXPECT_TRUE(mSec->setPlaybackSpeedState(25, 4, 999));
	const ThunderCall *c = LastCall();
	EXPECT_EQ(c->method, "setPlaybackSpeedState");
	EXPECT_EQ(c->param["sessionId"].Number(), 25);
	EXPECT_EQ(c->param["playbackSpeed"].Number(), 4);
	EXPECT_EQ(c->param["playbackPosition"].Number(), 999);
}

TEST_F(SecManagerThunderTests, DestroyInstanceThenGetInstanceReconstructs)
{
	ContentSecurityManager::DestroyInstance();
	auto *inst = ContentSecurityManager::GetInstance();
	ASSERT_NE(inst, nullptr);
	EXPECT_NE(dynamic_cast<SecManagerThunder *>(inst), nullptr);
}
