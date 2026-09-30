/**
 * @file PlayerTelemetry2.hpp
 * @brief Supporting class to provide telemetry support to Player
 */

#ifndef __PLAYER_TELEMETRY_2_H__
#define __PLAYER_TELEMETRY_2_H__

#include <cjson/cJSON.h>
#include <iostream>
#include <string>
#include <map>
#include "PlayerLogManager.h"


/* ── Lifecycle events ─────────────────────────────────────────────────────── */
#define TELEMETRY_EVENT_INITIALIZED        "INTERFACE_INITIALIZED" /**< InterfacePlayerRDK constructor completed */

/* ── Media / buffering events ─────────────────────────────────────────────── */
#define TELEMETRY_EVENT_BUFFERING_STARTED  "BUFFERING_STARTED"  /**< Pre-roll buffering begins */
#define TELEMETRY_EVENT_BUFFERING_ENDED    "BUFFERING_ENDED"    /**< Sufficient frames buffered; pipeline unpaused */
// RDK telemetry 2.0 is initialized once per process.
// This helper class encapsulates the initialization/uninitialization logic and is intended to be used
// via a global/static instance: its constructor initializes telemetry at process startup, and its
// destructor automatically uninitializes telemetry when the process shuts down.

class Player_TelemetryInitializer {
private:
    bool m_Initialized = false;
public:
    Player_TelemetryInitializer();
    void Init();
    bool isInitialized() const; 
    ~Player_TelemetryInitializer();
};


class PlayerTelemetry2 {
private:
    static Player_TelemetryInitializer mInitializer;
    std::string appName;
    
public:
    /**
     * @brief Constructor
     * @param[in] NONE
     */
    PlayerTelemetry2();
    
    /**
     * @brief Constructor
     * @param[in] appName - Name of the application
     */
    PlayerTelemetry2(const std::string &appName);
    
    /**
     *  
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
    bool send(const std::string &markerName, const char *  data);

    void sendEvent(const std::string& eventName);
};

#endif // PLAYER_TELEMETRY_2_H
