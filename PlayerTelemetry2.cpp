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

#include "PlayerTelemetry2.hpp"
#include <cjson/cJSON.h>
#include <telemetry_busmessage_sender.h>

#define PLAYER_T2_COMPONENT_NAME "mwplayer"

static const char *T2ErrorToString(T2ERROR err)
{
    switch (err)
    {
        case T2ERROR_SUCCESS:                 return "T2ERROR_SUCCESS";
        case T2ERROR_FAILURE:                 return "T2ERROR_FAILURE";
        case T2ERROR_INVALID_PROFILE:         return "T2ERROR_INVALID_PROFILE";
        case T2ERROR_PROFILE_NOT_FOUND:       return "T2ERROR_PROFILE_NOT_FOUND";
        case T2ERROR_PROFILE_NOT_SET:         return "T2ERROR_PROFILE_NOT_SET";
        case T2ERROR_MAX_PROFILES_REACHED:    return "T2ERROR_MAX_PROFILES_REACHED";
        case T2ERROR_MEMALLOC_FAILED:         return "T2ERROR_MEMALLOC_FAILED";
        case T2ERROR_INVALID_ARGS:            return "T2ERROR_INVALID_ARGS";
        case T2ERROR_INVALID_RESPONSE:        return "T2ERROR_INVALID_RESPONSE";
        case T2ERROR_INTERNAL_ERROR:          return "T2ERROR_INTERNAL_ERROR";
        case T2ERROR_NO_RBUS_METHOD_PROVIDER: return "T2ERROR_NO_RBUS_METHOD_PROVIDER";
        case T2ERROR_COMPONENT_NULL:          return "T2ERROR_COMPONENT_NULL";
        default:                              return "T2ERROR_UNKNOWN";
    }
}

PlayerTelemetryWorker::PlayerTelemetryWorker(Sender sender, Hook init, Hook uninit, size_t maxQueueSize,
                                             std::chrono::milliseconds slowCallThreshold, unsigned maxSlowCalls,
                                             std::chrono::milliseconds cooldown, std::chrono::milliseconds drainTimeout)
    : mSender(std::move(sender)), mInit(std::move(init)), mUninit(std::move(uninit)), mMaxQueueSize(maxQueueSize),
      mSlowCallThreshold(slowCallThreshold), mMaxSlowCalls(maxSlowCalls), mCooldown(cooldown),
      mDrainTimeout(drainTimeout)
{
    mThread = std::thread(&PlayerTelemetryWorker::run, this);
}

PlayerTelemetryWorker::~PlayerTelemetryWorker()
{
    {
        std::lock_guard<std::mutex> lock(mMutex);
        mStop = true;
        mDrainDeadline = std::chrono::steady_clock::now() + mDrainTimeout;
    }
    mCv.notify_all();
    if (mThread.joinable())
    {
        mThread.join();
    }
}

bool PlayerTelemetryWorker::enqueue(Item item)
{
    {
        std::lock_guard<std::mutex> lock(mMutex);
        if (mStop)
        {
            return false;
        }
        if (mDisabled)
        {
            if (std::chrono::steady_clock::now() - mDisabledAt < mCooldown)
            {
                ++mDroppedWhileDisabled;
                return false;
            }
            // Cooldown elapsed: half-open, allow sending again and reset the slow-call count
            MW_LOG_MIL("[M] Telemetry re-enabled after cooldown; %zu markers dropped while disabled",
                       mDroppedWhileDisabled);
            mDisabled = false;
            mDroppedWhileDisabled = 0;
            mSlowCalls = 0;
        }
        if (mQueue.size() >= mMaxQueueSize)
        {
            MW_LOG_WARN("[M] Telemetry queue full, dropping marker %s", item.marker.c_str());
            return false;
        }
        mQueue.push_back(std::move(item));
    }
    mCv.notify_one();
    return true;
}

bool PlayerTelemetryWorker::isDisabled() const
{
    std::lock_guard<std::mutex> lock(mMutex);
    return mDisabled;
}

void PlayerTelemetryWorker::run()
{
    if (mInit)
    {
        mInit();
    }
    std::unique_lock<std::mutex> lock(mMutex);
    while (true)
    {
        mCv.wait(lock, [this] { return mStop || !mQueue.empty(); });
        // On shutdown, deliver markers already accepted, but never past the drain deadline
        if (mStop && !mQueue.empty() && std::chrono::steady_clock::now() >= mDrainDeadline)
        {
            MW_LOG_WARN("[M] Telemetry drain timeout on shutdown; dropping %zu queued markers", mQueue.size());
            mQueue.clear();
        }
        if (mQueue.empty())
        {
            break;
        }
        Item item = std::move(mQueue.front());
        mQueue.pop_front();
        lock.unlock();

        const auto start = std::chrono::steady_clock::now();
        mSender(item);
        const auto elapsed =
            std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start);

        lock.lock();
        if (elapsed > mSlowCallThreshold)
        {
            MW_LOG_WARN("[M] Telemetry marker %s took %lld ms", item.marker.c_str(),
                        static_cast<long long>(elapsed.count()));
            if (++mSlowCalls >= mMaxSlowCalls)
            {
                MW_LOG_ERR("[M] Telemetry disabled for %lld s after %u consecutive slow calls; dropping %zu queued markers",
                           static_cast<long long>(std::chrono::duration_cast<std::chrono::seconds>(mCooldown).count()),
                           mSlowCalls, mQueue.size());
                mDisabled = true;
                mDisabledAt = std::chrono::steady_clock::now();
                mQueue.clear();
            }
        }
        else
        {
            mSlowCalls = 0;
        }
    }
    lock.unlock();
    if (mUninit)
    {
        mUninit();
    }
}

namespace
{
// Cleared when a t2_event_*() call reports that t2_init() did not take effect.
std::atomic<bool> gTelemetryUsable{true};

PlayerTelemetryWorker &getTelemetryWorker()
{
    static PlayerTelemetryWorker worker(
        [](const PlayerTelemetryWorker::Item &item)
        {
            char *marker = const_cast<char *>(item.marker.c_str());
            T2ERROR t2Error = item.isEvent ? t2_event_d(marker, 1)
                                           : t2_event_s(marker, const_cast<char *>(item.value.c_str()));
            if (T2ERROR_SUCCESS != t2Error)
            {
                MW_LOG_ERR("[M] T2 event %s failed: %s (%d)", item.marker.c_str(), T2ErrorToString(t2Error), (int)t2Error);
                if (T2ERROR_COMPONENT_NULL == t2Error)
                {
                    // The library has no component name, so t2_init() did not take effect.
                    MW_LOG_ERR("t2_init('%s') is not in effect; telemetry disabled", PLAYER_T2_COMPONENT_NAME);
                    gTelemetryUsable = false;
                }
                return false;
            }
            return true;
        },
        []()
        {
            // t2_init() returns void and only records the component name; the T2 bus
            // connection is made lazily by the first t2_event_*() call, so connection
            // errors are only reported through the T2ERROR returned by those calls.
            t2_init(const_cast<char *>(PLAYER_T2_COMPONENT_NAME));
            MW_LOG_MIL("t2_init called for component '%s'", PLAYER_T2_COMPONENT_NAME);
        },
        []()
        {
            t2_uninit();
            MW_LOG_MIL("t2_uninit done");
        });
    return worker;
}

bool enqueueTelemetry(PlayerTelemetryWorker::Item item)
{
    if (!gTelemetryUsable)
    {
        return false;
    }
    return getTelemetryWorker().enqueue(std::move(item));
}
} // namespace

PlayerTelemetry2::PlayerTelemetry2() : PlayerTelemetry2("")
{
}

PlayerTelemetry2::PlayerTelemetry2(const std::string &appName) : appName(appName)
{
}

bool PlayerTelemetry2::send(const std::string &markerName, const std::map<std::string, int>& intData, const std::map<std::string, std::string>& stringData, const std::map<std::string, float>& floatData)
{
    if (!gTelemetryUsable)
    {
        MW_LOG_ERR("[M] Telemetry not usable, dropping marker %s", markerName.c_str());
        return false;
    }

    cJSON *root = cJSON_CreateObject();
    if (!root)
    {
        MW_LOG_ERR("[M] cJSON_CreateObject failed for marker %s", markerName.c_str());
        return false;
    }

    if (!appName.empty())
    {
        cJSON_AddStringToObject(root, "app", appName.c_str());
    }

    for (const auto& pair : intData)
    {
        cJSON_AddNumberToObject(root, pair.first.c_str(), pair.second);
    }

    for (const auto& pair : stringData)
    {
        cJSON_AddStringToObject(root, pair.first.c_str(), pair.second.c_str());
    }

    for (const auto& pair : floatData)
    {
        cJSON_AddNumberToObject(root, pair.first.c_str(), pair.second);
    }

    char *jsonString = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);
    if (!jsonString)
    {
        MW_LOG_ERR("[M] cJSON_PrintUnformatted failed for marker %s", markerName.c_str());
        return false;
    }

    bool bRet = send(markerName, jsonString);
    cJSON_free(jsonString);
    return bRet;
}

bool PlayerTelemetry2::send(const std::string &markerName, const char *data)
{
    if (NULL == data)
    {
        return false;
    }

    MW_LOG_INFO("[S] Marker Name: %s value: %s", markerName.c_str(), data);
    return enqueueTelemetry({markerName, data, false});
}

bool PlayerTelemetry2::sendEvent(const std::string &eventName)
{
    MW_LOG_MIL("[TELEMETRY] event=%s", eventName.c_str());
    return enqueueTelemetry({eventName, "", true});
}
