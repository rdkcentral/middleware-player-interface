#include "PlayerTelemetry2.hpp"
#include <fstream>

#include <telemetry_busmessage_sender.h>

PlayerTelemetryWorker::PlayerTelemetryWorker(Sender sender, Hook init, Hook uninit, size_t maxQueueSize,
                                             std::chrono::milliseconds slowCallThreshold, unsigned maxSlowCalls)
    : mSender(std::move(sender)), mInit(std::move(init)), mUninit(std::move(uninit)), mMaxQueueSize(maxQueueSize),
      mSlowCallThreshold(slowCallThreshold), mMaxSlowCalls(maxSlowCalls)
{
    mThread = std::thread(&PlayerTelemetryWorker::run, this);
}

PlayerTelemetryWorker::~PlayerTelemetryWorker()
{
    {
        std::lock_guard<std::mutex> lock(mMutex);
        mStop = true;
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
        if (mDisabled || mStop)
        {
            return false;
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
        if (mStop)
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
                MW_LOG_ERR("[M] Telemetry disabled after %u consecutive slow calls; dropping %zu queued markers",
                           mSlowCalls, mQueue.size());
                mDisabled = true;
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
PlayerTelemetryWorker &getTelemetryWorker()
{
    static PlayerTelemetryWorker worker(
        [](const PlayerTelemetryWorker::Item &item)
        {
            T2ERROR t2Error = item.isEvent ? t2_event_d(item.marker.c_str(), 1)
                                           : t2_event_s(item.marker.c_str(), item.value.c_str());
            if (T2ERROR_SUCCESS != t2Error)
            {
                MW_LOG_ERR("[M] Telemetry event %s failed: %d", item.marker.c_str(), t2Error);
                return false;
            }
            return true;
        },
        []()
        {
            t2_init(const_cast<char *>("mwplayer"));
            MW_LOG_MIL("t2_init done ");
        },
        []()
        {
            t2_uninit();
            MW_LOG_MIL("t2_uninit done ");
        });
    return worker;
}
} // namespace

PlayerTelemetry2::PlayerTelemetry2() : PlayerTelemetry2("")
{
}

PlayerTelemetry2::PlayerTelemetry2(const std::string &appName) : appName(appName)
{
}

bool PlayerTelemetry2::send( const std::string &markerName, const std::map<std::string, int>& intData, const std::map<std::string, std::string>& stringData, const std::map<std::string, float>& floatData ) {
    bool bRet = false;

    // Log entry and initializer status

    {

        cJSON *root = cJSON_CreateObject();
        if (root == NULL)
        {
            MW_LOG_ERR("Failed to create cJSON root object for telemetry event: %s", markerName.c_str());
            return false;
        }
        if(!root)
        {
            MW_LOG_ERR("[M] cJSON_CreateObject failed");
            return false;
        }


     //   cJSON_AddStringToObject(root, "app", appName.c_str());

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
        if(!jsonString)
        {
            MW_LOG_ERR("[M] cJSON_PrintUnformatted failed");
            cJSON_Delete(root);
            return false;
        }

        MW_LOG_INFO("[M] Marker Name: %s | JSON: %s", markerName.c_str(), jsonString);

        bRet = getTelemetryWorker().enqueue({markerName, jsonString, false});

        cJSON_free(jsonString);
        cJSON_Delete(root);
    }

    return bRet;
}

bool PlayerTelemetry2::send( const std::string &markerName, const char *  data)
{
    bool bRet = false;
    if(NULL != data)
    {
        MW_LOG_INFO("[S] Marker Name: %s value:%s", markerName.c_str(),data );
        bRet = getTelemetryWorker().enqueue({markerName, data, false});
    }
    return bRet;
}
void PlayerTelemetry2::sendEvent(const std::string& eventName)
{
    MW_LOG_MIL("[TELEMETRY] event=%s", eventName.c_str());
    getTelemetryWorker().enqueue({eventName, "", true});
}
