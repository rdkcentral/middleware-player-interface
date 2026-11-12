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
#include <chrono>
#include <condition_variable>
#include <deque>
#include <functional>
#include <mutex>
#include <thread>
#include "PlayerLogManager.h"


/* ── Lifecycle events ─────────────────────────────────────────────────────── */
#define TELEMETRY_EVENT_INITIALIZED        "INTERFACE_INITIALIZED" /**< InterfacePlayerRDK constructor completed */

/* ── Media / buffering events ─────────────────────────────────────────────── */
#define TELEMETRY_EVENT_BUFFERING_STARTED  "BUFFERING_STARTED"  /**< Pre-roll buffering begins */
#define TELEMETRY_EVENT_BUFFERING_ENDED    "BUFFERING_ENDED"    /**< Sufficient frames buffered; pipeline unpaused */
// Note that RDK telemetry 2.0 support is per process basic, 
// this class is created to take care of uninitialization of telemetry but having object as global variable 
// when process goes down, destructor of this class will be called and it will uninitialize the telemetry. 

/**
 * @class PlayerTelemetryWorker
 * @brief Sends telemetry markers from a dedicated thread, so that callers (tune thread, GLib callbacks)
 *        never block on the Telemetry 2.0 library. Stops sending after consecutive slow calls,
 *        e.g. when the T2 message bus cannot be reached and every call waits for its connect timeout.
 */
class PlayerTelemetryWorker
{
public:
    struct Item
    {
        std::string marker;
        std::string value;
        bool isEvent{false};
    };
    using Sender = std::function<bool(const Item &)>;
    using Hook = std::function<void()>;

    PlayerTelemetryWorker(Sender sender, Hook init, Hook uninit, size_t maxQueueSize = 64,
                          std::chrono::milliseconds slowCallThreshold = std::chrono::milliseconds(500),
                          unsigned maxSlowCalls = 2);
    ~PlayerTelemetryWorker();
    PlayerTelemetryWorker(const PlayerTelemetryWorker &) = delete;
    PlayerTelemetryWorker &operator=(const PlayerTelemetryWorker &) = delete;

    /**
     * @brief Queues an item for sending
     * @return false if the worker is disabled or the queue is full
     */
    bool enqueue(Item item);

    /**
     * @brief True once sending has been stopped because of consecutive slow calls
     */
    bool isDisabled() const;

private:
    void run();

    Sender mSender;
    Hook mInit;
    Hook mUninit;
    const size_t mMaxQueueSize;
    const std::chrono::milliseconds mSlowCallThreshold;
    const unsigned mMaxSlowCalls;
    mutable std::mutex mMutex;
    std::condition_variable mCv;
    std::deque<Item> mQueue;
    unsigned mSlowCalls{0};
    bool mDisabled{false};
    bool mStop{false};
    std::thread mThread;
};


class PlayerTelemetry2 {
private:
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
