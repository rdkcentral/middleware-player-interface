/*
 * If not stated otherwise in this file or this component's license file the
 * following copyright and licenses apply:
 *
 * Copyright 2026 RDK Management
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "backend/PlayerBackend.h"
#include "console/ConsolePlayer.h"

#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cerr << "Usage: pi-player-simulator <http(s)-stream-url>\n";
        return 2;
    }

    const std::string url(argv[1]);
    if (url.rfind("http://", 0) != 0 && url.rfind("https://", 0) != 0) {
        std::cerr << "Only HTTP(S) stream URLs are supported.\n";
        return 2;
    }

    PlayerBackend backend(url);
    ConsolePlayer console;
    return console.Run(backend);
}
