#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <vector>

class DashResourceProxy {
public:
    using ProgressCallback = std::function<void(const std::string&)>;

    DashResourceProxy();
    ~DashResourceProxy();

    DashResourceProxy(const DashResourceProxy&) = delete;
    DashResourceProxy& operator=(const DashResourceProxy&) = delete;

    bool Start(std::vector<uint8_t>& manifest,
               const std::string& manifestUrl,
               ProgressCallback progress);
    void PrefetchForSeek(double positionSeconds);
    void Stop();

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};
