#include "FakeTelemetry.h"
#include "telemetry_busmessage_sender.h"
#include <chrono>
#include <thread>

FakeTelemetryState &fakeTelemetry()
{
    static FakeTelemetryState state;
    return state;
}

static T2ERROR record(const char *marker, const std::string &value)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(fakeTelemetry().blockMs.load()));
    std::lock_guard<std::mutex> lock(fakeTelemetry().mutex);
    fakeTelemetry().markers.push_back(marker);
    fakeTelemetry().values.push_back(value);
    return T2ERROR_SUCCESS;
}

void t2_init(char *) {}
T2ERROR t2_event_s(const char *marker, const char *value) { return record(marker, value); }
T2ERROR t2_event_d(const char *marker, int value) { return record(marker, std::to_string(value)); }
void t2_uninit(void) {}
