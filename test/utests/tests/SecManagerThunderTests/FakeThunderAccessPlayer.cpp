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
 * @file FakeThunderAccessPlayer.cpp
 * @brief Test-double for ThunderAccessPlayer used by the SecManagerThunder
 *        L1 suite.  Compiles the REAL ThunderAccessPlayer.h (against the
 *        stub WPEFramework headers) and implements every method with
 *        scripted behaviour controlled by g_FakeThunder.
 */

#include "ThunderAccessPlayer.h"

#include <deque>
#include <map>
#include <vector>

/**
 * @brief Recorded JSON-RPC invocation.
 */
struct ThunderCall
{
	std::string callsign;
	std::string method;
	JsonObject param;
};

/**
 * @brief Scripted control block - reset by the fixture before every test.
 */
struct FakeThunderControl
{
	/** Return value for ActivatePlugin(). */
	bool activateResult;
	/** Queue of (rpcResult, response) pairs; popped per InvokeJSONRPC call. */
	std::deque<std::pair<bool, JsonObject>> responses;
	/** Response used once the queue is empty. */
	std::pair<bool, JsonObject> defaultResponse;
	/** All recorded invocations, in order. */
	std::vector<ThunderCall> calls;
	/** Event handlers registered via SubscribeEvent, keyed by event name. */
	std::map<std::string, std::function<void(const WPEFramework::Core::JSON::VariantContainer &)>> handlers;
	/** Event names unsubscribed via UnSubscribeEvent. */
	std::vector<std::string> unsubscribed;
	/** Return value for SubscribeEvent. */
	bool subscribeResult;
	void reset();
};

void FakeThunderControl::reset()
{
	activateResult = true;
	responses.clear();
	defaultResponse = {true, JsonObject{}};
	calls.clear();
	handlers.clear();
	unsubscribed.clear();
	subscribeResult = true;
}

extern FakeThunderControl &g_FakeThunder;
// Deliberately leaked: ContentSecurityManagerSession static SessionManager
// objects can fire ReleaseSession->InvokeJSONRPC during static destruction,
// so the control block must outlive every other static in the binary.
FakeThunderControl &g_FakeThunder = *(new FakeThunderControl());

ThunderAccessPlayer::ThunderAccessPlayer(const std::string& callsign)
	: remoteObject(nullptr), controllerObject(nullptr), pluginCallsign(callsign)
{
}

ThunderAccessPlayer::~ThunderAccessPlayer() = default;

bool ThunderAccessPlayer::ActivatePlugin()
{
	return g_FakeThunder.activateResult;
}

bool ThunderAccessPlayer::InvokeJSONRPC(const std::string& method, const JsonObject &param,
										JsonObject &result, const uint32_t waitTime)
{
	(void)waitTime;
	g_FakeThunder.calls.push_back(ThunderCall{pluginCallsign, method, param});

	if (!g_FakeThunder.responses.empty())
	{
		auto front = g_FakeThunder.responses.front();
		g_FakeThunder.responses.pop_front();
		result = front.second;
		return front.first;
	}

	result = g_FakeThunder.defaultResponse.second;
	return g_FakeThunder.defaultResponse.first;
}

bool ThunderAccessPlayer::SubscribeEvent(
	const std::string& eventName,
	std::function<void(const WPEFramework::Core::JSON::VariantContainer &)> functionHandler)
{
	if (g_FakeThunder.subscribeResult)
	{
		g_FakeThunder.handlers[eventName] = std::move(functionHandler);
	}
	return g_FakeThunder.subscribeResult;
}

bool ThunderAccessPlayer::UnSubscribeEvent(const std::string& eventName)
{
	g_FakeThunder.unsubscribed.push_back(eventName);
	g_FakeThunder.handlers.erase(eventName);
	return true;
}
