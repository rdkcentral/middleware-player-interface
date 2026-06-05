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
 * Provides a TelemetryPayload builder and PlayerTelemetry::sendEvent() overloads.
 * TelemetryPayload::add() accepts string, integer, and floating-point values so
 * call sites do not need manual std::to_string() conversions.
 *
 * Usage:
 * @code
 *   TelemetryPayload payload;
 *   payload.add("systemId", systemId);
 *   payload.add("retryCount", retryCount);   // int — no std::to_string needed
 *   PlayerTelemetry::sendEvent(TELEMETRY_EVENT_DRM_HELPER_NOT_FOUND, payload);
 * @endcode
 *
 * When PLAYER_TELEMETRY_SUPPORT is NOT defined at compile time every call is
 * compiled away to a no-op — zero overhead, no dependency on <map> or logging
 * headers in non-telemetry builds.  Enable telemetry by passing
 * -DPLAYER_TELEMETRY_SUPPORT (or setting CMAKE_PLAYER_TELEMETRY_SUPPORT) at
 * build time.
 */

#ifdef PLAYER_TELEMETRY_SUPPORT

#include <string>
#include <map>
#include "PlayerLogManager.h"

/**
 * @class TelemetryPayload
 * @brief Key/value container for telemetry event context data.
 *
 * Accepts string, integer, and floating-point values via the add() method,
 * converting numeric types to their string representation automatically so
 * that call sites do not need explicit std::to_string() calls.
 */
class TelemetryPayload
{
public:
    /** @brief Add a string field. */
    TelemetryPayload& add(const std::string& key, const std::string& value)
    {
        m_fields[key] = value;
        return *this;
    }

    /** @brief Add an integer field (converted to decimal string). */
    TelemetryPayload& add(const std::string& key, int value)
    {
        m_fields[key] = std::to_string(value);
        return *this;
    }

    /** @brief Add a long integer field (converted to decimal string). */
    TelemetryPayload& add(const std::string& key, long value)
    {
        m_fields[key] = std::to_string(value);
        return *this;
    }

    /** @brief Add a float field (converted to decimal string). */
    TelemetryPayload& add(const std::string& key, float value)
    {
        m_fields[key] = std::to_string(value);
        return *this;
    }

    /** @brief Add a double field (converted to decimal string). */
    TelemetryPayload& add(const std::string& key, double value)
    {
        m_fields[key] = std::to_string(value);
        return *this;
    }

    /** @brief Read-only access to the internal map for sendEvent(). */
    const std::map<std::string, std::string>& fields() const { return m_fields; }

private:
    std::map<std::string, std::string> m_fields;
};

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
	    t2_event_d(&marker[0], 1);
    }

    /**
     * @brief Emit a telemetry event with a structured key/value payload.
     * @param[in] eventName  One of the TELEMETRY_EVENT_* markers from TelemetryMarkers.h.
     * @param[in] payload    Additional context data built with TelemetryPayload::add().
     */
    static void sendEvent(const std::string& eventName, const TelemetryPayload& payload)
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
        MW_LOG_MIL("[TELEMETRY] event=%s %s", eventName.c_str(), fields.c_str());
    }

private:
    PlayerTelemetry() = delete;
};

#else /* PLAYER_TELEMETRY_SUPPORT not defined */

#include <string>

/**
 * @class TelemetryPayload
 * @brief No-op stub compiled when PLAYER_TELEMETRY_SUPPORT is not defined.
 *
 * All add() methods are empty inline functions eliminated by the compiler.
 * Call sites compile without change and require no #ifdef guards.
 */
class TelemetryPayload
{
public:
    TelemetryPayload& add(const std::string& /*key*/, const std::string& /*value*/) { return *this; }
    TelemetryPayload& add(const std::string& /*key*/, int /*value*/)                { return *this; }
    TelemetryPayload& add(const std::string& /*key*/, long /*value*/)               { return *this; }
    TelemetryPayload& add(const std::string& /*key*/, float /*value*/)              { return *this; }
    TelemetryPayload& add(const std::string& /*key*/, double /*value*/)             { return *this; }
};

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
    static void sendEvent(const std::string& /*eventName*/, const TelemetryPayload& /*payload*/) {}

private:
    PlayerTelemetry() = delete;
};

#endif /* PLAYER_TELEMETRY_SUPPORT */
