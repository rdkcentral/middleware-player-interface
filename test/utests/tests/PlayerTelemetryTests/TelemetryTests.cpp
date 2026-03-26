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

#include <gtest/gtest.h>
#include <cjson/cJSON.h>
#include "PlayerTelemetry2.hpp"

class PlayerTelemetryTest : public ::testing::Test {
protected:

	void SetUp() override {
	}

	void TearDown() override {
	}

public:
	PlayerTelemetry2 telemetry;
};


TEST_F(PlayerTelemetryTest, Send_StringData)
{
	std::string markername = "VideoStartTime";
	std::string data = "{\"type\":\"IP_PLAYER_TUNETIME\",\"ver\":5,\"bld\":\"1.0\"}";
	EXPECT_EQ(true, telemetry.send(markername, data.c_str()));
}

TEST_F(PlayerTelemetryTest, Send_NullData)
{
	std::string markername = "VideoStartTime";
	EXPECT_EQ(false, telemetry.send(markername, nullptr));
}

TEST_F(PlayerTelemetryTest, Send_MapData_VideoStartFailure)
{
	std::string markername = "VideoStartFailure";
	std::map<std::string, int> intData;
	intData["err"] = 10;
	intData["cat"] = 10;
	intData["cls"] = 0;
	std::map<std::string, std::string> stringData;
	stringData["desc"] = "manifest request failed";
	std::map<std::string, float> floatData;
	floatData["pos"] = 0.0f;
	EXPECT_EQ(true, telemetry.send(markername, intData, stringData, floatData));
}

TEST_F(PlayerTelemetryTest, Send_MapData_VideoBufferingStart)
{
	std::string markername = "VideoBufferingStart";
	std::map<std::string, int> intData;
	intData["buffer"] = 1;
	std::map<std::string, std::string> stringData;
	stringData["type"] = "underrun";
	std::map<std::string, float> floatData;
	floatData["pos"] = 10.5f;
	EXPECT_EQ(true, telemetry.send(markername, intData, stringData, floatData));
}

TEST_F(PlayerTelemetryTest, Send_MapData_VideoBufferingEnd)
{
	std::string markername = "VideoBufferingEnd";
	std::map<std::string, int> intData;
	intData["buffer"] = 0;
	std::map<std::string, std::string> stringData;
	stringData["type"] = "underrun";
	std::map<std::string, float> floatData;
	floatData["dur"] = 2.1f;
	EXPECT_EQ(true, telemetry.send(markername, intData, stringData, floatData));
}

TEST_F(PlayerTelemetryTest, Send_MapData_VideoBitrateChange)
{
	std::string markername = "VideoBitrateChange";
	std::map<std::string, int> intData;
	intData["bit"] = 1650064;
	intData["wdh"] = 640;
	intData["hth"] = 266;
	std::map<std::string, std::string> stringData;
	stringData["desc"] = "Reset to default bitrate due to tune";
	std::map<std::string, float> floatData;
	floatData["frt"] = 25.0f;
	floatData["pos"] = 102.0f;
	EXPECT_EQ(true, telemetry.send(markername, intData, stringData, floatData));
}

TEST_F(PlayerTelemetryTest, Send_MapData_EmptyMaps)
{
	std::string markername = "VideoPlaybackSuccess";
	std::map<std::string, int> intData;
	std::map<std::string, std::string> stringData;
	std::map<std::string, float> floatData;
	EXPECT_EQ(true, telemetry.send(markername, intData, stringData, floatData));
}
