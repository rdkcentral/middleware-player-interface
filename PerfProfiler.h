#ifndef PERF_PROFILER_H
#define PERF_PROFILER_H

#include <string>
#include <chrono>

// Forward declaration to avoid circular dependency
class MWConfig;

class ScopedTimer {
public:
    ScopedTimer(const std::string& funcName, const std::string& fileName, int line);
    ~ScopedTimer();

    /**
     * @brief Check if profiling is active (compile-time or runtime enabled)
     * @return true if profiling should be active
     */
    static bool IsProfilingActive();

private:
    std::string name;
    std::chrono::high_resolution_clock::time_point start;
};

// Macro helpers for generating a unique variable name per expansion
#define MW_PROFILE_CONCAT_IMPL(x, y) x##y
#define MW_PROFILE_CONCAT(x, y) MW_PROFILE_CONCAT_IMPL(x, y)

// Macro for easy integration - checks both compile-time flag and runtime config
#define MW_PROFILE_FUNCTION() \
    do { \
        if (ScopedTimer::IsProfilingActive()) { \
            ScopedTimer MW_PROFILE_CONCAT(timer_, __LINE__)(__FUNCTION__, __FILE__, __LINE__); \
        } \
    } while (0)

#endif // PERF_PROFILER_H