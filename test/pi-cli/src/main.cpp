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
    backend.Start();

    ConsolePlayer console;
    return console.Run(backend);
}
