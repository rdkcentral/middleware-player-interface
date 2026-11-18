/*
 * If not stated otherwise in this file or this component's license file the
 * following copyright and licenses apply:
 *
 * Copyright 2026 RDK Management
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

#include "PlayerTelemetry2.hpp"
#include <cjson/cJSON.h>
#include <telemetry_busmessage_sender.h>

#define PLAYER_T2_COMPONENT_NAME "mwplayer"

static const char *T2ErrorToString(T2ERROR err)
{
    switch (err)
    {
        case T2ERROR_SUCCESS:                 return "T2ERROR_SUCCESS";
        case T2ERROR_FAILURE:                 return "T2ERROR_FAILURE";
        case T2ERROR_INVALID_PROFILE:         return "T2ERROR_INVALID_PROFILE";
        case T2ERROR_PROFILE_NOT_FOUND:       return "T2ERROR_PROFILE_NOT_FOUND";
        case T2ERROR_PROFILE_NOT_SET:         return "T2ERROR_PROFILE_NOT_SET";
        case T2ERROR_MAX_PROFILES_REACHED:    return "T2ERROR_MAX_PROFILES_REACHED";
        case T2ERROR_MEMALLOC_FAILED:         return "T2ERROR_MEMALLOC_FAILED";
        case T2ERROR_INVALID_ARGS:            return "T2ERROR_INVALID_ARGS";
        case T2ERROR_INVALID_RESPONSE:        return "T2ERROR_INVALID_RESPONSE";
        case T2ERROR_INTERNAL_ERROR:          return "T2ERROR_INTERNAL_ERROR";
        case T2ERROR_NO_RBUS_METHOD_PROVIDER: return "T2ERROR_NO_RBUS_METHOD_PROVIDER";
        case T2ERROR_COMPONENT_NULL:          return "T2ERROR_COMPONENT_NULL";
        default:                              return "T2ERROR_UNKNOWN";
    }
}

Player_TelemetryInitializer::Player_TelemetryInitializer()
: m_Initialized(false), m_InitOnce()
{
}

bool Player_TelemetryInitializer::Init()
{
    std::call_once(m_InitOnce, [this]() {
        // t2_init() returns void and only records the component name; the T2 bus
        // connection is made lazily by the first t2_event_*() call, so connection
        // errors are only reported through the T2ERROR returned by those calls.
        t2_init((char *)PLAYER_T2_COMPONENT_NAME);
        m_Initialized = true;
        MW_LOG_MIL("t2_init called for component '%s'", PLAYER_T2_COMPONENT_NAME);
    });
    return m_Initialized;
}

bool Player_TelemetryInitializer::isInitialized() const
{
    return m_Initialized;
}

void Player_TelemetryInitializer::OnEventResult(const std::string &markerName, int result)
{
    T2ERROR err = (T2ERROR)result;
    if (T2ERROR_SUCCESS == err)
    {
        return;
    }
    MW_LOG_ERR("T2 event %s failed: %s (%d)", markerName.c_str(), T2ErrorToString(err), (int)err);
    if (T2ERROR_COMPONENT_NULL == err)
    {
        // The library has no component name, so t2_init() did not take effect.
        MW_LOG_ERR("t2_init('%s') is not in effect; telemetry disabled", PLAYER_T2_COMPONENT_NAME);
        m_Initialized = false;
    }
}

Player_TelemetryInitializer::~Player_TelemetryInitializer()
{
    if (m_Initialized)
    {
        t2_uninit();
        m_Initialized = false;
    }
}

Player_TelemetryInitializer PlayerTelemetry2::mInitializer;

PlayerTelemetry2::PlayerTelemetry2() : PlayerTelemetry2("")
{
}

PlayerTelemetry2::PlayerTelemetry2(const std::string &appName) : appName(appName)
{
    if (!mInitializer.Init())
    {
        MW_LOG_ERR("Telemetry initialization failed; markers will not be sent");
    }
}

bool PlayerTelemetry2::send(const std::string &markerName, const std::map<std::string, int>& intData, const std::map<std::string, std::string>& stringData, const std::map<std::string, float>& floatData)
{
    if (!mInitializer.isInitialized())
    {
        MW_LOG_ERR("[M] Telemetry not initialized, dropping marker %s", markerName.c_str());
        return false;
    }

    cJSON *root = cJSON_CreateObject();
    if (!root)
    {
        MW_LOG_ERR("[M] cJSON_CreateObject failed for marker %s", markerName.c_str());
        return false;
    }

    if (!appName.empty())
    {
        cJSON_AddStringToObject(root, "app", appName.c_str());
    }

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
    cJSON_Delete(root);
    if (!jsonString)
    {
        MW_LOG_ERR("[M] cJSON_PrintUnformatted failed for marker %s", markerName.c_str());
        return false;
    }

    bool bRet = send(markerName, jsonString);
    cJSON_free(jsonString);
    return bRet;
}

bool PlayerTelemetry2::send(const std::string &markerName, const char *data)
{
    if (!mInitializer.isInitialized() || NULL == data)
    {
        return false;
    }

    MW_LOG_INFO("[S] Marker Name: %s value: %s", markerName.c_str(), data);
    T2ERROR t2Error = t2_event_s((char *)markerName.c_str(), (char *)data);
    mInitializer.OnEventResult(markerName, t2Error);
    return (T2ERROR_SUCCESS == t2Error);
}

bool PlayerTelemetry2::sendEvent(const std::string &eventName)
{
    if (!mInitializer.isInitialized())
    {
        return false;
    }

    MW_LOG_MIL("[TELEMETRY] event=%s", eventName.c_str());
    T2ERROR t2Error = t2_event_d((char *)eventName.c_str(), 1);
    mInitializer.OnEventResult(eventName, t2Error);
    return (T2ERROR_SUCCESS == t2Error);
}
