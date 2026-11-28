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

/**
 * @file PlayerTelemetry2.hpp
 * @brief Supporting class to provide telemetry support to Player
 */

#ifndef __PLAYER_TELEMETRY_2_H__
#define __PLAYER_TELEMETRY_2_H__

#include <atomic>
#include <map>
#include <mutex>
#include <string>
#include "PlayerLogManager.h"

/* ── Lifecycle events ─────────────────────────────────────────────────────── */
#define TELEMETRY_EVENT_INITIALIZED        "INTERFACE_INITIALIZED"      /**< Count marker: InterfacePlayerRDK constructor completed */
#define TELEMETRY_EVENT_INITIALIZED_INFO   "INTERFACE_INITIALIZED_INFO" /**< JSON marker: details of InterfacePlayerRDK construction */

/* ── Media / buffering events ─────────────────────────────────────────────── */
#define TELEMETRY_EVENT_BUFFERING_STARTED  "BUFFERING_STARTED"  /**< Pre-roll buffering begins */
#define TELEMETRY_EVENT_BUFFERING_ENDED    "BUFFERING_ENDED"    /**< Sufficient frames buffered; pipeline unpaused */

// RDK telemetry 2.0 is initialized once per process. A single static instance of this
// class owns the t2_init/t2_uninit lifecycle; t2_uninit runs when the process exits.
// t2_init() itself cannot report failure (it returns void), so initialization problems
// are detected from the T2ERROR returned by the first t2_event_*() call.
class Player_TelemetryInitializer {
private:
    std::atomic<bool> m_Initialized;
    std::once_flag m_InitOnce;
public:
    Player_TelemetryInitializer();
    /**
     * @brief Calls t2_init() once per process
     * @return bool - true if telemetry is usable, false otherwise
     */
    bool Init();
    bool isInitialized() const;
    /**
     * @brief Logs a failed t2_event_*() result and disables telemetry if t2_init() did not take effect
     * @param[in] markerName - Name of the marker that was sent
     * @param[in] result - T2ERROR returned by t2_event_*()
     */
    void OnEventResult(const std::string &markerName, int result);
    ~Player_TelemetryInitializer();
};


class PlayerTelemetry2 {
private:
    static Player_TelemetryInitializer mInitializer;
    std::string appName;

public:
    /**
     * @brief Constructor
     */
    PlayerTelemetry2();

    /**
     * @brief Constructor
     * @param[in] appName - Name of the application
     */
    PlayerTelemetry2(const std::string &appName);

    /**
     * @brief send  - Send the telemetry data to the telemetry bus by converting input map to json string
     * @param[in] markerName - Name of the marker
     * @param[in] intData - Map of int data
     * @param[in] stringData - Map of string data
     * @param[in] floatData - Map of float data
     * @return bool - true if success, false otherwise
     */
    bool send(const std::string &markerName, const std::map<std::string, int>& intData, const std::map<std::string, std::string>& stringData, const std::map<std::string, float>& floatData);

    /**
     * @brief send  - Send the telemetry data to the telemetry bus
     * @param[in] markerName - Name of the marker
     * @param[in] data - Data to be sent
     * @return bool - true if success, false otherwise
     */
    bool send(const std::string &markerName, const char *data);

    /**
     * @brief sendEvent - Increment a count marker on the telemetry bus
     * @param[in] eventName - Name of the marker
     * @return bool - true if success, false otherwise
     */
    bool sendEvent(const std::string &eventName);
};

#endif // __PLAYER_TELEMETRY_2_H__
