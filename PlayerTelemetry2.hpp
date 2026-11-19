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
#include <chrono>
#include <condition_variable>
#include <deque>
#include <functional>
#include <map>
#include <mutex>
#include <string>
#include <thread>
#include "PlayerLogManager.h"

/* ── Lifecycle events ─────────────────────────────────────────────────────── */
#define TELEMETRY_EVENT_INITIALIZED        "INTERFACE_INITIALIZED"      /**< Count marker: InterfacePlayerRDK constructor completed */
#define TELEMETRY_EVENT_INITIALIZED_INFO   "INTERFACE_INITIALIZED_INFO" /**< JSON marker: details of InterfacePlayerRDK construction */

/* ── Media / buffering events ─────────────────────────────────────────────── */
#define TELEMETRY_EVENT_BUFFERING_STARTED  "BUFFERING_STARTED"  /**< Pre-roll buffering begins */
#define TELEMETRY_EVENT_BUFFERING_ENDED    "BUFFERING_ENDED"    /**< Sufficient frames buffered; pipeline unpaused */

// RDK telemetry 2.0 is initialized once per process. A single process-wide PlayerTelemetryWorker
// owns the t2_init/t2_uninit lifecycle on its own thread; t2_uninit runs when the process exits.
// t2_init() itself cannot report failure (it returns void), so initialization problems
// are detected from the T2ERROR returned by the first t2_event_*() call.

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
                          unsigned maxSlowCalls = 2,
                          std::chrono::milliseconds cooldown = std::chrono::seconds(30),
                          std::chrono::milliseconds drainTimeout = std::chrono::seconds(1));
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
    const std::chrono::milliseconds mCooldown;
    const std::chrono::milliseconds mDrainTimeout;
    mutable std::mutex mMutex;
    std::condition_variable mCv;
    std::deque<Item> mQueue;
    unsigned mSlowCalls{0};
    bool mDisabled{false};
    std::chrono::steady_clock::time_point mDisabledAt{};
    size_t mDroppedWhileDisabled{0};
    bool mStop{false};
    std::chrono::steady_clock::time_point mDrainDeadline{};
    std::thread mThread;
};


class PlayerTelemetry2 {
private:
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
     * @brief send  - Queue the telemetry data for the telemetry bus by converting input map to json string
     * @param[in] markerName - Name of the marker
     * @param[in] intData - Map of int data
     * @param[in] stringData - Map of string data
     * @param[in] floatData - Map of float data
     * @return bool - true if the marker was queued, false otherwise
     */
    bool send(const std::string &markerName, const std::map<std::string, int>& intData, const std::map<std::string, std::string>& stringData, const std::map<std::string, float>& floatData);

    /**
     * @brief send  - Queue the telemetry data for the telemetry bus
     * @param[in] markerName - Name of the marker
     * @param[in] data - Data to be sent
     * @return bool - true if the marker was queued, false otherwise
     */
    bool send(const std::string &markerName, const char *data);

    /**
     * @brief sendEvent - Queue an increment of a count marker on the telemetry bus
     * @param[in] eventName - Name of the marker
     * @return bool - true if the marker was queued, false otherwise
     */
    bool sendEvent(const std::string &eventName);
};

#endif // __PLAYER_TELEMETRY_2_H__
