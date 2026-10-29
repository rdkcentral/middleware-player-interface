#pragma once

#include <string>

class PlayerBackend;

class PlayerUi {
public:
    int Run(PlayerBackend& backend, const std::string& url);
};
