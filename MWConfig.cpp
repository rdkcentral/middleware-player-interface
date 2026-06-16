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
 * @file MWConfig.cpp
 * @brief Middleware Player Interface Configuration Implementation
 */

#include "MWConfig.h"
#include "PlayerLogManager.h"
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <algorithm>

MWConfig& MWConfig::GetInstance() {
    static MWConfig instance;
    return instance;
}

MWConfig::MWConfig() : mProfilingEnabled(false) {
    // Check compile-time flag to set initial state
#ifdef ENABLE_MW_PROFILING
    mProfilingEnabled = true;
    MW_LOG_INFO("[MWConfig] Profiling enabled at compile time (ENABLE_MW_PROFILING)");
#endif
}

void MWConfig::Initialize() {
    std::string cfgPath;

    // Try /opt first
    std::ifstream file("/opt/middleware-player-interface.cfg");
    if (!file.good()) {
        // Try HOME directory
        const char* homeDir = std::getenv("HOME");
        if (homeDir) {
            cfgPath = std::string(homeDir) + "/.middleware-player-interface.cfg";
            file.open(cfgPath);
        }
    } else {
        cfgPath = "/opt/middleware-player-interface.cfg";
    }

    if (file.good()) {
        LoadConfigFile(cfgPath);
    } else {
        MW_LOG_INFO("[MWConfig] No config file found (checked /opt and $HOME)");
    }
}

bool MWConfig::LoadConfigFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.good()) {
        MW_LOG_WARN("[MWConfig] Failed to open config file: %s", filePath.c_str());
        return false;
    }

    MW_LOG_INFO("[MWConfig] Loading config from: %s", filePath.c_str());

    std::string line;
    int lineNum = 0;
    while (std::getline(file, line)) {
        lineNum++;
        ParseConfigLine(line);
    }

    file.close();
    return true;
}

void MWConfig::ParseConfigLine(const std::string& line) {
    // Skip empty lines and comments
    if (line.empty() || line[0] == '#') {
        return;
    }

    // Trim leading/trailing whitespace
    size_t start = line.find_first_not_of(" \t\r\n");
    size_t end = line.find_last_not_of(" \t\r\n");
    if (start == std::string::npos) {
        return;
    }

    std::string trimmedLine = line.substr(start, end - start + 1);

    // Skip comments inline
    size_t commentPos = trimmedLine.find('#');
    if (commentPos != std::string::npos) {
        trimmedLine = trimmedLine.substr(0, commentPos);
        // Trim trailing whitespace again
        end = trimmedLine.find_last_not_of(" \t\r\n");
        if (end != std::string::npos) {
            trimmedLine = trimmedLine.substr(0, end + 1);
        }
    }

    // Parse key=value format
    size_t delimPos = trimmedLine.find('=');
    if (delimPos == std::string::npos) {
        return;
    }

    std::string key = trimmedLine.substr(0, delimPos);
    std::string value = trimmedLine.substr(delimPos + 1);

    // Trim key and value
    key.erase(std::remove_if(key.begin(), key.end(), ::isspace), key.end());
    value.erase(0, value.find_first_not_of(" \t"));
    value.erase(value.find_last_not_of(" \t") + 1);

    if (key.empty()) {
        return;
    }

    // Handle profiling setting
    if (key == "enableProfiling" || key == "profiling") {
        bool enabled = (value == "true" || value == "1" || value == "yes" || value == "on");
        SetProfilingEnabled(enabled);
        MW_LOG_INFO("[MWConfig] Profiling %s from config file",
                   enabled ? "ENABLED" : "DISABLED");
    } else {
        // Store generic config values
        SetConfigValue(key, value);
    }
}

bool MWConfig::IsProfilingEnabled() const {
    std::lock_guard<std::mutex> lock(mConfigMutex);
    return mProfilingEnabled;
}

void MWConfig::SetProfilingEnabled(bool enabled) {
    std::lock_guard<std::mutex> lock(mConfigMutex);
    mProfilingEnabled = enabled;
    mConfigMap["enableProfiling"] = enabled ? "true" : "false";
}

std::string MWConfig::GetConfigValue(const std::string& key, const std::string& defaultValue) const {
    std::lock_guard<std::mutex> lock(mConfigMutex);
    auto it = mConfigMap.find(key);
    if (it != mConfigMap.end()) {
        return it->second;
    }
    return defaultValue;
}

void MWConfig::SetConfigValue(const std::string& key, const std::string& value) {
    std::lock_guard<std::mutex> lock(mConfigMutex);
    mConfigMap[key] = value;
}
