#ifndef PERF_PROFILER_H
#define PERF_PROFILER_H

#include <string>
#include <chrono>

#ifdef ENABLE_MW_PROFILING

class ScopedTimer {
public:
    ScopedTimer(const std::string& funcName, const std::string& fileName, int line);
    ~ScopedTimer();

private:
    std::string name;
    std::chrono::high_resolution_clock::time_point start;
};

// Macro helpers for generating a unique variable name per expansion
#define MW_PROFILE_CONCAT_IMPL(x, y) x##y
#define MW_PROFILE_CONCAT(x, y) MW_PROFILE_CONCAT_IMPL(x, y)
// Macro for easy integration
#define MW_PROFILE_FUNCTION() ScopedTimer MW_PROFILE_CONCAT(timer_, __LINE__)(__FUNCTION__, __FILE__, __LINE__)
#else
#define MW_PROFILE_FUNCTION() do { } while (0)

#endif //ENABLE_MW_PROFILING
#endif // PERF_PROFILER_H