#ifndef FAKE_TELEMETRY_H
#define FAKE_TELEMETRY_H

#include <atomic>
#include <mutex>
#include <string>
#include <vector>

struct FakeTelemetryState
{
    std::atomic<int> blockMs{0};
    std::mutex mutex;
    std::vector<std::string> markers;
    std::vector<std::string> values;
};

FakeTelemetryState &fakeTelemetry();

#endif // FAKE_TELEMETRY_H
