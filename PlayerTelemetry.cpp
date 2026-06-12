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
 * @file PlayerTelemetry.cpp
 * @brief Implementation of telemetry support for the player interface.
 */

#include "PlayerTelemetry.h"
#include "PlayerLogManager.h"

void PlayerTelemetry2::send(const std::string& marker,
                             const std::map<std::string, int>& iMap,
                             const std::map<std::string, std::string>& sMap,
                             const std::map<std::string, float>& fMap)
{
	MW_LOG_MIL("PlayerTelemetry2: %s", marker.c_str());
	for (const auto& kv : iMap)
	{
		MW_LOG_MIL("PlayerTelemetry2:  int[%s]=%d", kv.first.c_str(), kv.second);
	}
	for (const auto& kv : sMap)
	{
		MW_LOG_MIL("PlayerTelemetry2:  str[%s]=%s", kv.first.c_str(), kv.second.c_str());
	}
	for (const auto& kv : fMap)
	{
		MW_LOG_MIL("PlayerTelemetry2:  float[%s]=%f", kv.first.c_str(), kv.second);
	}
}
