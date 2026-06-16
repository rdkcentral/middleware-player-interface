#include "PerfProfiler.h"
#include "MWConfig.h"
#include <thread>
#include <mutex>
#include <unordered_map>
#include "PlayerLogManager.h"

bool ScopedTimer::IsProfilingActive() {
#ifdef ENABLE_MW_PROFILING
    // Check both compile-time flag and runtime config
    return MWConfig::GetInstance().IsProfilingEnabled();
#else
    // If compile-time flag is not set, check runtime config
    // This allows profiling to be enabled at runtime even without compile-time flag
    return MWConfig::GetInstance().IsProfilingEnabled();
#endif
}

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
