#include "PerfProfiler.h"
#include <iostream>
#include <thread>

ScopedTimer::ScopedTimer(const std::string& funcName, const std::string& fileName, int line)
    : name(funcName + " [" + fileName + ":" + std::to_string(line) + "]"),
      start(std::chrono::high_resolution_clock::now()) {}

ScopedTimer::~ScopedTimer() {
    auto end = std::chrono::high_resolution_clock::now();
    long long duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
	MW_LOG_INFO(MW_LOG_PERF, "[PERF] %s (Thread %zu) took %lld us",
        name.c_str(),
        std::hash<std::thread::id>{}(std::this_thread::get_id()),
        duration
    );
}

