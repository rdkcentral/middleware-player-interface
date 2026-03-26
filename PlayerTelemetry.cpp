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

/**
 * @file PlayerTelemetry.cpp
 * @brief Implementation of PlayerTelemetry::sendEvent() using the Telemetry 2.0 framework.
 *
 * Compiled only when PLAYER_TELEMETRY_SUPPORT is defined.  Each event is
 * forwarded to t2_event_s() so that it is picked up by the platform telemetry
 * back-end.
 */

#ifdef PLAYER_TELEMETRY_SUPPORT

#include "PlayerTelemetry2.hpp"
#include <telemetry2_marker.h>

#include "PlayerTelemetry.h"

/**
 * @brief Emit a telemetry event with no additional payload.
 */
void PlayerTelemetry::sendEvent(const std::string& eventName)
{
    /* t2_event_s() is a C API that takes char* but does not modify the string. */
    t2_event_s(const_cast<char*>(eventName.c_str()), const_cast<char*>("1"));
}

/**
 * @brief Emit a telemetry event with a structured key/value payload.
 *
 * The payload fields are serialised as a single space-separated
 * "key=value" string and forwarded as the t2_event_s value argument.
 */
void PlayerTelemetry::sendEvent(const std::string& eventName, const TelemetryPayload& payload)
{
    std::string fields;
    for (const auto& kv : payload.fields())
    {
        if (!fields.empty())
        {
            fields += ' ';
        }
        fields += kv.first + '=' + kv.second;
    }
    /* t2_event_s() is a C API that takes char* but does not modify the strings. */
    t2_event_s(const_cast<char*>(eventName.c_str()), const_cast<char*>(fields.c_str()));
}

#endif /* PLAYER_TELEMETRY_SUPPORT */
