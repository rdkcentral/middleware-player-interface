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

#pragma once

/**
 * @file PlayerTelemetry.h
 * @brief Lightweight telemetry emission utility for the middleware player interface.
 *
 * Provides PlayerTelemetry::sendEvent() overloads that accept an event name and
 * an optional key/value payload map.  Events are emitted to the platform logging
 * sink so that they can be forwarded by the surrounding RDK telemetry framework.
 *
 * The implementation deliberately avoids a hard dependency on any particular
 * telemetry back-end.  Integration with T2 or another platform service can be
 * added by extending or replacing the body of sendEvent() without changing any
 * call sites.
 *
 * When PLAYER_TELEMETRY_SUPPORT is NOT defined at compile time every call to
 * sendEvent() is compiled away to a no-op so there is zero overhead and no
 * dependency on <map> in non-telemetry builds.  Enable telemetry by passing
 * -DPLAYER_TELEMETRY_SUPPORT (or setting CMAKE_PLAYER_TELEMETRY_SUPPORT) at
 * build time.
 */

#ifdef PLAYER_TELEMETRY_SUPPORT

#include <string>
#include <map>
#include "PlayerLogManager.h"

/**
 * @class PlayerTelemetry
 * @brief Static helper for emitting named telemetry events with optional payload data.
 *
 * Active implementation compiled when PLAYER_TELEMETRY_SUPPORT is defined.
 */
class PlayerTelemetry
{
public:
    /**
     * @brief Emit a telemetry event with no additional payload.
     * @param[in] eventName  One of the TELEMETRY_EVENT_* markers from TelemetryMarkers.h.
     */
    static void sendEvent(const std::string& eventName)
    {
        MW_LOG_MIL("[TELEMETRY] event=%s", eventName.c_str());
    }

    /**
     * @brief Emit a telemetry event with a structured key/value payload.
     * @param[in] eventName  One of the TELEMETRY_EVENT_* markers from TelemetryMarkers.h.
     * @param[in] payload    Additional context data to attach to the event.
     */
    static void sendEvent(const std::string& eventName,
                          const std::map<std::string, std::string>& payload)
    {
        std::string fields;
        for (const auto& kv : payload)
        {
            if (!fields.empty())
            {
                fields += ' ';
            }
            fields += kv.first + '=' + kv.second;
        }
        MW_LOG_MIL("[TELEMETRY] event=%s %s", eventName.c_str(), fields.c_str());
    }

private:
    PlayerTelemetry() = delete;
};

#else /* PLAYER_TELEMETRY_SUPPORT not defined */

#include <string>
#include <map>

/**
 * @class PlayerTelemetry
 * @brief No-op stub compiled when PLAYER_TELEMETRY_SUPPORT is not defined.
 *
 * All methods are empty inline functions so the compiler eliminates them
 * entirely.  Call sites require no #ifdef guards.
 */
class PlayerTelemetry
{
public:
    static void sendEvent(const std::string& /*eventName*/) {}
    static void sendEvent(const std::string& /*eventName*/,
                          const std::map<std::string, std::string>& /*payload*/) {}

private:
    PlayerTelemetry() = delete;
};

#endif /* PLAYER_TELEMETRY_SUPPORT */
