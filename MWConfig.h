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
 * @file MWConfig.h
 * @brief Middleware Player Interface Configuration Management
 */

#ifndef MWCONFIG_H
#define MWCONFIG_H

#include <string>
#include <map>
#include <mutex>

/**
 * @class MWConfig
 * @brief Centralized middleware configuration management for runtime settings
 */
class MWConfig {
public:
    /**
     * @brief Get singleton instance of MWConfig
     * @return Reference to MWConfig instance
     */
    static MWConfig& GetInstance();

    /**
     * @brief Initialize config from file
     * Tries to read middleware-player-interface.cfg from /opt or HOME directory
     */
    void Initialize();

    /**
     * @brief Check if profiling is enabled at runtime
     * @return true if profiling enabled, false otherwise
     */
    bool IsProfilingEnabled() const;

    /**
     * @brief Enable or disable profiling at runtime
     * @param[in] enabled - true to enable, false to disable
     */
    void SetProfilingEnabled(bool enabled);

    /**
     * @brief Load configuration from a file
     * @param[in] filePath - path to config file
     * @return true if file loaded successfully, false otherwise
     */
    bool LoadConfigFile(const std::string& filePath);

    /**
     * @brief Get config value (generic getter for extensibility)
     * @param[in] key - configuration key
     * @param[in] defaultValue - default value if key not found
     * @return configuration value
     */
    std::string GetConfigValue(const std::string& key, const std::string& defaultValue = "") const;

    /**
     * @brief Set config value (generic setter for extensibility)
     * @param[in] key - configuration key
     * @param[in] value - configuration value
     */
    void SetConfigValue(const std::string& key, const std::string& value);

private:
    /**
     * @brief Private constructor for singleton pattern
     */
    MWConfig();

    /**
     * @brief Delete copy constructor
     */
    MWConfig(const MWConfig&) = delete;

    /**
     * @brief Delete assignment operator
     */
    MWConfig& operator=(const MWConfig&) = delete;

    /**
     * @brief Parse configuration line (key=value format)
     * @param[in] line - configuration line to parse
     */
    void ParseConfigLine(const std::string& line);

    // Configuration storage
    std::map<std::string, std::string> mConfigMap;
    mutable std::mutex mConfigMutex;
    bool mProfilingEnabled;
};

#endif // MWCONFIG_H
