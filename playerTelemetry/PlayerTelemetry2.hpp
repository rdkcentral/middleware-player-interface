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
 * @file PlayerTelemetry2.hpp
 * @brief Supporting class to provide telemetry support to Player middleware
 */

#ifndef __PLAYER_TELEMETRY_2_H__
#define __PLAYER_TELEMETRY_2_H__

#ifdef PLAYER_TELEMETRY_SUPPORT

#include <cjson/cJSON.h>
#include <iostream>
#include <string>
#include <map>
#include "PlayerLogManager.h"

// Note that RDK telemetry 2.0 support is per process basis,
// this class is created to take care of un initialization of telemetry but having object as global variable
// when process goes down, destructor of this class will be called and it will uninitialize the telemetry.

/**
 * @class PlayerTelemetryInitializer
 * @brief Manages the lifecycle of the T2 telemetry library initialization
 */
class PlayerTelemetryInitializer {
private:
	bool m_Initialized = false;
public:
	/**
	 * @brief Constructor
	 */
	PlayerTelemetryInitializer();

	/**
	 * @brief Initialize the telemetry library (idempotent)
	 */
	void Init();

	/**
	 * @brief Check if telemetry has been initialized
	 * @return true if initialized, false otherwise
	 */
	bool isInitialized() const;

	/**
	 * @brief Destructor - uninitializes the telemetry library
	 */
	~PlayerTelemetryInitializer();
};


/**
 * @class PlayerTelemetry2
 * @brief Provides object-based telemetry event sending via RDK T2 (Telemetry 2.0)
 *
 * Instead of calling telemetry send functions directly, create a PlayerTelemetry2
 * object and call send() to dispatch telemetry events.
 */
class PlayerTelemetry2 {
private:
	static PlayerTelemetryInitializer mInitializer;
	std::string appName;

public:
	/**
	 * @brief Constructor with no application name
	 */
	PlayerTelemetry2();

	/**
	 * @brief Constructor
	 * @param[in] appName - Name of the application
	 */
	PlayerTelemetry2(const std::string &appName);

	/**
	 * @brief Send telemetry data to the telemetry bus by converting input maps to a JSON string
	 * @param[in] markerName - Name of the marker/event
	 * @param[in] intData    - Map of integer key-value pairs
	 * @param[in] stringData - Map of string key-value pairs
	 * @param[in] floatData  - Map of float key-value pairs
	 * @return true if the event was sent successfully, false otherwise
	 */
	bool send(const std::string &markerName,
	          const std::map<std::string, int>& intData,
	          const std::map<std::string, std::string>& stringData,
	          const std::map<std::string, float>& floatData);

	/**
	 * @brief Send telemetry data to the telemetry bus
	 * @param[in] markerName - Name of the marker/event
	 * @param[in] data       - Raw data string to send
	 * @return true if the event was sent successfully, false otherwise
	 */
	bool send(const std::string &markerName, const char *data);
};

#endif // PLAYER_TELEMETRY_SUPPORT

#endif // __PLAYER_TELEMETRY_2_H__
