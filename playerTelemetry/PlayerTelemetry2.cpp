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
 * @file PlayerTelemetry2.cpp
 * @brief PlayerTelemetry2 class implementation
 */

#include "PlayerTelemetry2.hpp"

#ifndef PLAYER_SIMULATOR_BUILD
#include <telemetry_busmessage_sender.h>
#endif


PlayerTelemetryInitializer::PlayerTelemetryInitializer()
: m_Initialized(false)
{
}

void PlayerTelemetryInitializer::Init()
{
	if (false == m_Initialized)
	{
		m_Initialized = true;
#ifndef PLAYER_SIMULATOR_BUILD
		t2_init((char *)"player");
		MW_LOG_MIL("t2_init done");
#endif
	}
}

bool PlayerTelemetryInitializer::isInitialized() const
{
	return m_Initialized;
}

PlayerTelemetryInitializer::~PlayerTelemetryInitializer()
{
#ifndef PLAYER_SIMULATOR_BUILD
	t2_uninit();
	MW_LOG_MIL("t2_uninit done");
#endif
}


PlayerTelemetryInitializer PlayerTelemetry2::mInitializer;

PlayerTelemetry2::PlayerTelemetry2() : PlayerTelemetry2("")
{
}

PlayerTelemetry2::PlayerTelemetry2(const std::string &appName) : appName(appName)
{
	mInitializer.Init();
}

bool PlayerTelemetry2::send(const std::string &markerName,
                            const std::map<std::string, int>& intData,
                            const std::map<std::string, std::string>& stringData,
                            const std::map<std::string, float>& floatData)
{
	bool bRet = false;
	if (mInitializer.isInitialized())
	{
		cJSON *root = cJSON_CreateObject();

		cJSON_AddStringToObject(root, "app", appName.c_str());

		for (const auto& pair : intData)
		{
			cJSON_AddNumberToObject(root, pair.first.c_str(), pair.second);
		}

		for (const auto& pair : stringData)
		{
			cJSON_AddStringToObject(root, pair.first.c_str(), pair.second.c_str());
		}

		for (const auto& pair : floatData)
		{
			cJSON_AddNumberToObject(root, pair.first.c_str(), pair.second);
		}

		char *jsonString = cJSON_PrintUnformatted(root);

		MW_LOG_INFO("[M] Marker Name: %s value:%s", markerName.c_str(), jsonString);

#ifndef PLAYER_SIMULATOR_BUILD
		T2ERROR t2Error = t2_event_s((char *)markerName.c_str(), jsonString);

		if (T2ERROR_SUCCESS == t2Error)
		{
			bRet = true;
		}
		else
		{
			MW_LOG_ERR("t2_event_s map failed:%d", t2Error);
		}
#else
		bRet = true;
#endif
		cJSON_free(jsonString);
		cJSON_Delete(root);
	}

	return bRet;
}

bool PlayerTelemetry2::send(const std::string &markerName, const char *data)
{
	bool bRet = false;
	if (mInitializer.isInitialized() && nullptr != data)
	{
#ifndef PLAYER_SIMULATOR_BUILD
		T2ERROR t2Error = t2_event_s((char *)markerName.c_str(), (char *)data);

		if (T2ERROR_SUCCESS == t2Error)
		{
			bRet = true;
		}
		else
		{
			MW_LOG_ERR("t2_event_s string failed:%d", t2Error);
		}
#else
		bRet = true;
#endif
	}

	return bRet;
}
