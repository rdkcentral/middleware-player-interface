#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "ContentSecurityManager.h"
#include "SecManagerThunder.h"
#include "MockPlayerScheduler.h"
#include "PlayerLogManager.h"
#include <algorithm>
#include <cstdlib>
#include <map>
#include <vector>

namespace
{
const std::string sessionId = "24e22c0a-6bc7-4a2d-b547-88b2293c5a80";
struct RpcCall
{
	std::string method;
	std::string parameters;
};
std::vector<RpcCall> calls;
std::map<std::string, std::function<void(const JsonObject&)>> events;
std::function<void(const std::string&, JsonObject&)> respond;

void ExpectSessionString(const std::string& serialized, const std::string& expected)
{
	JsonObject parameters;
	ASSERT_TRUE(parameters.FromString(serialized));
	EXPECT_EQ(parameters["sessionId"].Content(), WPEFramework::Core::JSON::Variant::type::STRING);
	EXPECT_EQ(parameters["sessionId"].String(), expected);
	EXPECT_NE(serialized.find("\"sessionId\":\"" + expected + "\""), std::string::npos);
}
}

// Replace only the transport; the backend and Thunder JSON implementation are real.
ThunderAccessPlayer::ThunderAccessPlayer(std::string callsign)
	: remoteObject(nullptr), controllerObject(nullptr), pluginCallsign(std::move(callsign))
{
}
ThunderAccessPlayer::~ThunderAccessPlayer() = default;
bool ThunderAccessPlayer::ActivatePlugin() { return true; }
bool ThunderAccessPlayer::InvokeJSONRPC(std::string method, const JsonObject& parameters,
	JsonObject& result, const uint32_t)
{
	std::string serialized;
	parameters.ToString(serialized);
	calls.push_back({method, serialized});
	result.Clear();
	result["success"] = true;
	if (respond)
	{
		respond(method, result);
	}
	return true;
}
bool ThunderAccessPlayer::SubscribeEvent(std::string name, std::function<void(const JsonObject&)> handler)
{
	events[name] = std::move(handler);
	return true;
}
bool ThunderAccessPlayer::UnSubscribeEvent(std::string name)
{
	events.erase(name);
	return true;
}

TEST(SecManagerThunderStartup, CloseAllUsesEmptyString)
{
	calls.clear();
	SecManagerThunder manager;
	auto call = std::find_if(calls.begin(), calls.end(),
		[](const RpcCall& rpc) { return rpc.method == "closePlaybackSession"; });
	ASSERT_NE(call, calls.end());
	ExpectSessionString(call->parameters, "");
	EXPECT_EQ(JsonObject(call->parameters)["clientId"].String(), "player");
}

class SecManagerThunderTests : public testing::Test
{
protected:
	void SetUp() override
	{
		respond = nullptr;
		manager = static_cast<SecManagerThunder*>(ContentSecurityManager::GetInstance());
		calls.clear();
	}
	void TearDown() override
	{
		free(license);
		g_mockPlayerScheduler = nullptr;
		ContentSecurityManager::setWatermarkSessionEvent_CB(nullptr);
		respond = nullptr;
		ContentSecurityManager::DestroyInstance();
		calls.clear();
		events.clear();
		PlayerLogManager::mwLoglevel = originalLogLevel;
		PlayerLogManager::disableLogRedirection = originalLogRedirection;
	}
	bool Acquire(ContentSecurityManagerSession& session)
	{
		const char* trace[][2] = {{"moneyTrace", "trace"}};
		return manager->AcquireLicenseOpenOrUpdate("player", "app", "", trace, nullptr,
			"metadata", 8, "request", 7, "widevine", "stream", "token", 5, session,
			&license, &licenseLength, &status, &reason, &businessStatus, false, 0);
	}
	SecManagerThunder* manager = nullptr;
	char* license = nullptr;
	size_t licenseLength = 0;
	int32_t status = 0, reason = 0, businessStatus = 0;
	MW_LogLevel originalLogLevel = PlayerLogManager::mwLoglevel;
	bool originalLogRedirection = PlayerLogManager::disableLogRedirection;
};

TEST_F(SecManagerThunderTests, DebugLogsIdentifyStringSessionLifecycle)
{
	PlayerLogManager::mwLoglevel = mLOGLEVEL_DEBUG;
	PlayerLogManager::disableLogRedirection = true;
	respond = [](const std::string&, JsonObject& result)
	{
		result["sessionId"] = sessionId;
		result["license"] = "bGljZW5zZQ==";
	};
	testing::internal::CaptureStdout();
	bool acquired;
	{
		ContentSecurityManagerSession session;
		acquired = Acquire(session);
	}
	const std::string logs = testing::internal::GetCapturedStdout();
	EXPECT_TRUE(acquired);
	EXPECT_NE(logs.find("[SessionIdString] SecManager request method=openPlaybackSession"), std::string::npos);
	EXPECT_NE(logs.find("[SessionIdString] SecManager response method=openPlaybackSession sessionId='" + sessionId + "' type=string"), std::string::npos);
	EXPECT_NE(logs.find("[SessionIdString] MPI stored/reused string sessionId='" + sessionId + "'"), std::string::npos);
	EXPECT_NE(logs.find("[SessionIdString] MPI releasing DRM sessionId='" + sessionId + "'"), std::string::npos);
}

TEST_F(SecManagerThunderTests, PlaybackRequestsPreserveStringIds)
{
	for (const auto& id : {sessionId, std::string("9223372036854775808")})
	{
		calls.clear();
		EXPECT_TRUE(manager->SetDrmSessionState(id, true));
		manager->CloseDrmSession(id);
		EXPECT_TRUE(manager->setWindowSize(id, 1920, 1080));
		EXPECT_TRUE(manager->SetPlaybackPosition(id, 1.0f, 42));
		ASSERT_EQ(calls.size(), 4u);
		EXPECT_EQ(calls[0].method, "setPlaybackSessionState");
		EXPECT_EQ(calls[1].method, "closePlaybackSession");
		EXPECT_EQ(calls[2].method, "setVideoWindowSize");
		EXPECT_EQ(calls[3].method, "setPlaybackSpeedState");
		for (const auto& call : calls)
		{
			ExpectSessionString(call.parameters, id);
		}
	}
}

TEST_F(SecManagerThunderTests, EmptyIndividualSessionDoesNotInvokeRpc)
{
	EXPECT_FALSE(manager->SetDrmSessionState("", true));
	manager->CloseDrmSession("");
	EXPECT_FALSE(manager->setWindowSize("", 1920, 1080));
	EXPECT_FALSE(manager->SetPlaybackPosition("", 1.0f, 42));
	EXPECT_TRUE(calls.empty());
}

TEST_F(SecManagerThunderTests, OpenResponsePreservesUuid)
{
	respond = [](const std::string&, JsonObject& result)
	{
		result["sessionId"] = sessionId;
		result["license"] = "bGljZW5zZQ==";
	};
	ContentSecurityManagerSession session;
	ASSERT_TRUE(Acquire(session));
	EXPECT_EQ(session.getSessionID(), sessionId);
	ASSERT_EQ(calls.size(), 1u);
	EXPECT_EQ(calls[0].method, "openPlaybackSession");
	EXPECT_FALSE(JsonObject(calls[0].parameters).HasLabel("sessionId"));
	ASSERT_NE(license, nullptr);
	EXPECT_EQ(std::string(license, licenseLength), "license");
}

TEST_F(SecManagerThunderTests, UpdatePreservesUuid)
{
	respond = [](const std::string&, JsonObject& result)
	{
		result["sessionId"] = sessionId;
		result["license"] = "bGljZW5zZQ==";
	};
	ContentSecurityManagerSession session(sessionId, 0);
	ASSERT_TRUE(Acquire(session));
	ASSERT_EQ(calls.size(), 1u);
	EXPECT_EQ(calls[0].method, "updatePlaybackSession");
	ExpectSessionString(calls[0].parameters, sessionId);
	EXPECT_EQ(session.getSessionID(), sessionId);
}

TEST_F(SecManagerThunderTests, RetryReusesStringSession)
{
	respond = [](const std::string& method, JsonObject& result)
	{
		result["sessionId"] = sessionId;
		if (method == "openPlaybackSession")
		{
			result["license"] = "";
			JsonObject context;
			context["class"] = CONTENT_SECURITY_MANAGER_DRM_FAILURE;
			context["reason"] = CONTENT_SECURITY_MANAGER_SERVICE_BUSY;
			result["secManagerResultContext"] = context;
		}
		else
		{
			result["license"] = "bGljZW5zZQ==";
		}
	};
	ContentSecurityManagerSession session;
	ASSERT_TRUE(Acquire(session));
	ASSERT_EQ(calls.size(), 2u);
	EXPECT_EQ(calls[0].method, "openPlaybackSession");
	EXPECT_EQ(calls[1].method, "updatePlaybackSession");
	ExpectSessionString(calls[1].parameters, sessionId);
}

TEST_F(SecManagerThunderTests, SuccessfulOpenWithEmptyIdIsRejected)
{
	respond = [](const std::string&, JsonObject& result)
	{
		result["sessionId"] = "";
		result["license"] = "bGljZW5zZQ==";
	};
	ContentSecurityManagerSession session;
	EXPECT_FALSE(Acquire(session));
	EXPECT_FALSE(session.isSessionValid());
	EXPECT_EQ(license, nullptr);
	EXPECT_EQ(status, CONTENT_SECURITY_MANAGER_DRM_FAILURE);
	EXPECT_EQ(reason, CONTENT_SECURITY_MANAGER_DRM_GEN_FAILURE);
}

TEST_F(SecManagerThunderTests, SuccessfulOpenWithMissingIdIsRejected)
{
	respond = [](const std::string&, JsonObject& result)
	{
		result["license"] = "bGljZW5zZQ==";
	};
	ContentSecurityManagerSession session;
	EXPECT_FALSE(Acquire(session));
	EXPECT_FALSE(session.isSessionValid());
	EXPECT_EQ(license, nullptr);
	EXPECT_EQ(status, CONTENT_SECURITY_MANAGER_DRM_FAILURE);
	EXPECT_EQ(reason, CONTENT_SECURITY_MANAGER_DRM_GEN_FAILURE);
}

TEST_F(SecManagerThunderTests, SuccessfulOpenWithNonStringIdIsRejected)
{
	for (const std::string value : {"null", "123", "true"})
	{
		respond = [value](const std::string&, JsonObject& result)
		{
			ASSERT_TRUE(result.FromString("{\"success\":true,\"sessionId\":" + value +
				",\"license\":\"bGljZW5zZQ==\"}"));
		};
		ContentSecurityManagerSession session;
		EXPECT_FALSE(Acquire(session));
		EXPECT_FALSE(session.isSessionValid());
		EXPECT_EQ(license, nullptr);
		EXPECT_EQ(status, CONTENT_SECURITY_MANAGER_DRM_FAILURE);
		EXPECT_EQ(reason, CONTENT_SECURITY_MANAGER_DRM_GEN_FAILURE);
	}
}

TEST_F(SecManagerThunderTests, WatermarkCallbackPreservesUuid)
{
	int count = 0;
	ContentSecurityManager::setWatermarkSessionEvent_CB(
		[&](const std::string& id, uint32_t condition, const std::string& system)
		{
			++count;
			EXPECT_EQ(id, sessionId);
			EXPECT_EQ(condition, 7u);
			EXPECT_EQ(system, "provider");
		});
	JsonObject parameters;
	ASSERT_TRUE(parameters.FromString("{\"sessionId\":\"" + sessionId +
		"\",\"conditionContext\":7,\"watermarkingSystem\":\"provider\"}"));
	ASSERT_EQ(events.count("onWatermarkSession"), 1u);
	events.at("onWatermarkSession")(parameters);
	EXPECT_EQ(count, 1);
	parameters["sessionId"] = "";
	events.at("onWatermarkSession")(parameters);
	EXPECT_EQ(count, 1);
	parameters["sessionId"] = 123;
	events.at("onWatermarkSession")(parameters);
	EXPECT_EQ(count, 1);
	JsonObject missingId;
	events.at("onWatermarkSession")(missingId);
	EXPECT_EQ(count, 1);
}

TEST_F(SecManagerThunderTests, InvalidAddWatermarkDoesNotScheduleTasks)
{
	testing::StrictMock<MockPlayerScheduler> scheduler;
	g_mockPlayerScheduler = &scheduler;
	JsonObject parameters;
	ASSERT_EQ(events.count("onAddWatermark"), 1u);
	events.at("onAddWatermark")(parameters);
	parameters["sessionId"] = "";
	events.at("onAddWatermark")(parameters);
	parameters["sessionId"] = 123;
	events.at("onAddWatermark")(parameters);
	EXPECT_TRUE(calls.empty());
}

TEST_F(SecManagerThunderTests, DeferredPaletteRequestPreservesUuid)
{
	testing::StrictMock<MockPlayerScheduler> scheduler;
	g_mockPlayerScheduler = &scheduler;
	std::vector<PlayerAsyncTaskObj> tasks;
	EXPECT_CALL(scheduler, ScheduleTask(testing::_)).Times(2)
		.WillRepeatedly(testing::Invoke([&](PlayerAsyncTaskObj task)
		{
			tasks.push_back(std::move(task));
			return static_cast<int>(tasks.size());
		}));
	respond = [](const std::string& method, JsonObject& result)
	{
		if (method == "getPalettedWatermark")
		{
			result["clutKey"] = 11;
			result["clutSize"] = 16;
			result["imageKey"] = 12;
			result["imageWidth"] = 1920;
			result["imageHeight"] = 1080;
		}
	};
	{
		JsonObject parameters;
		parameters["sessionId"] = sessionId;
		parameters["graphicId"] = 2;
		parameters["zIndex"] = 3;
		parameters["graphicImageBufferKey"] = 4;
		parameters["graphicImageSize"] = 5;
		parameters["adjustVisibilityRequired"] = true;
		ASSERT_EQ(events.count("onAddWatermark"), 1u);
		events.at("onAddWatermark")(parameters);
	}
	ASSERT_EQ(tasks.size(), 2u);
	for (const auto& task : tasks)
	{
		task.mTask(task.mData);
	}
	auto call = std::find_if(calls.begin(), calls.end(),
		[](const RpcCall& rpc) { return rpc.method == "loadClutWatermark"; });
	ASSERT_NE(call, calls.end());
	ExpectSessionString(call->parameters, sessionId);
	JsonObject parameters(call->parameters);
	EXPECT_EQ(parameters["graphicId"].Number(), 2);
	EXPECT_EQ(parameters["watermarkClutBufferKey"].Number(), 11);
	EXPECT_EQ(parameters["watermarkImageBufferKey"].Number(), 12);
}
