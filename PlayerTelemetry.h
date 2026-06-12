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

#ifndef PLAYER_TELEMETRY_H
#define PLAYER_TELEMETRY_H

/**
 * @file PlayerTelemetry.h
 * @brief Telemetry support for the player interface.
 */

#include <map>
#include <string>

/**
 * @brief Class for sending player telemetry events with typed key-value maps.
 */
class PlayerTelemetry2
{
public:
	/**
	 * @brief Sends a telemetry event with associated data maps.
	 *
	 * @param[in] marker  The telemetry event name/marker.
	 * @param[in] iMap    Integer key-value pairs associated with the event.
	 * @param[in] sMap    String key-value pairs associated with the event.
	 * @param[in] fMap    Float key-value pairs associated with the event.
	 */
	void send(const std::string& marker,
	          const std::map<std::string, int>& iMap,
	          const std::map<std::string, std::string>& sMap,
	          const std::map<std::string, float>& fMap);
};

#endif // PLAYER_TELEMETRY_H
