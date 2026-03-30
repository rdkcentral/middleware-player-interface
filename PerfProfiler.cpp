#include "PerfProfiler.h"
#include <iostream>
#include <thread>
#include <mutex>
#include <unordered_map>
#include "PlayerLogManager.h"

#ifdef ENABLE_MW_PROFILING

ScopedTimer::ScopedTimer(const std::string& funcName, const std::string& fileName, int line)
    : name(funcName + " [" + fileName + ":" + std::to_string(line) + "]"),
      start(std::chrono::high_resolution_clock::now()) {}

ScopedTimer::~ScopedTimer() {
    auto end = std::chrono::high_resolution_clock::now();
    long long duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    // Throttle profiling logs to at most once every 5 seconds per API signature.
    static std::mutex sRateLimitMutex;
    static std::unordered_map<std::string, std::chrono::steady_clock::time_point> sLastLogTime;
    static constexpr auto kProfileLogInterval = std::chrono::seconds(5);

    const auto now = std::chrono::steady_clock::now();
    {
        std::lock_guard<std::mutex> lock(sRateLimitMutex);
        auto it = sLastLogTime.find(name);
        if (it != sLastLogTime.end() && (now - it->second) < kProfileLogInterval) {
            return;
        }
        sLastLogTime[name] = now;
    }

    MW_LOG_INFO( "[PERF] %s (Thread %zu) took %lld us",
        name.c_str(),
        std::hash<std::thread::id>{}(std::this_thread::get_id()),
        duration);
}

#endif // ENABLE_MW_PROFILING
