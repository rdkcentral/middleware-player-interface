#include "ConsolePlayer.h"

#include "../backend/PlayerBackend.h"

#include <cerrno>
#include <cmath>
#include <cstddef>
#include <iostream>
#include <poll.h>
#include <sstream>
#include <string>
#include <unistd.h>

namespace {

void PrintHelp()
{
    std::cout
        << "Commands:\n"
        << "  play                 Resume playback\n"
        << "  pause                Pause playback\n"
        << "  seek <seconds>       Seek to a position in seconds\n"
        << "  trick <rate>         Set trick rate: 1x, 2x, 4x, 6x, 12x, or 32x\n"
        << "  list audio           List available audio tracks\n"
        << "  list video           List available video tracks\n"
        << "  setaudio <number>    Switch to an audio track from list audio\n"
        << "  setresolution <WxH|Hp> Select video quality (for example 1280x720)\n"
        << "  abr <on|off>         Enable or disable automatic bitrate selection\n"
        << "  stop                 Stop playback\n"
        << "  status               Show playback status and position\n"
        << "  help                 Show this help\n"
        << "  quit                 Stop and exit\n";
}

void PrintStatus(const PlayerBackend& backend)
{
    const PlaybackStatus status = backend.GetStatus();
    std::cout << status.message << " | position "
              << static_cast<double>(status.positionMs) / 1000.0
              << " s";
    if (status.durationMs > 0) {
        std::cout << " / "
                  << static_cast<double>(status.durationMs) / 1000.0
                  << " s";
    }
    std::cout << " | ABR "
              << (status.autoBitrateEnabled ? "on" : "off")
              << " bandwidth="
              << status.estimatedBandwidthBitsPerSecond << "bps";
    std::cout << '\n';
}

void PrintTracks(const PlayerBackend& backend, MediaTrackType type)
{
    const std::vector<MediaTrackInfo> tracks = backend.GetTrackCatalog();
    bool foundTrack = false;
    std::size_t index = 0;
    for (const MediaTrackInfo& track : tracks) {
        if (track.type != type) {
            continue;
        }
        foundTrack = true;
        std::cout << ++index << (track.selected ? " [selected]" : "")
                  << " codec="
                  << (track.codec.empty() ? "unknown" : track.codec);
        if (!track.language.empty()) {
            std::cout << " language=" << track.language;
        }
        if (!track.role.empty()) {
            std::cout << " role=" << track.role;
        }
        if (track.bitrate > 0) {
            std::cout << " bitrate=" << track.bitrate;
        }
        if (track.width > 0 && track.height > 0) {
            std::cout << " resolution=" << track.width << 'x' << track.height;
        }
        std::cout << '\n';
    }
    if (!foundTrack) {
        std::cout << "No " << (type == MediaTrackType::Audio ? "audio"
                                                               : "video")
                  << " track metadata is available.\n";
    }
}

bool ParseSeekSeconds(const std::string& value, double& seconds)
{
    std::istringstream input(value);
    input >> seconds;
    input >> std::ws;
    return input.eof() && std::isfinite(seconds) && seconds >= 0.0;
}

bool ParseTrickRate(const std::string& value, int& rate)
{
    if (value == "1x") {
        rate = 1;
    } else if (value == "2x") {
        rate = 2;
    } else if (value == "4x") {
        rate = 4;
    } else if (value == "6x") {
        rate = 6;
    } else if (value == "12x") {
        rate = 12;
    } else if (value == "32x") {
        rate = 32;
    } else {
        return false;
    }
    return true;
}

bool ParsePositiveInt(const std::string& value, int& result)
{
    std::istringstream input(value);
    input >> result;
    input >> std::ws;
    return input.eof() && result > 0;
}

bool ParseResolution(const std::string& value, int& width, int& height)
{
    if (value.empty()) {
        return false;
    }

    const char suffix = value.back();
    if (suffix == 'p' || suffix == 'P') {
        const std::string heightValue = value.substr(0, value.size() - 1);
        width = 0;
        return ParsePositiveInt(heightValue, height);
    }

    const std::size_t separator = value.find_first_of("xX");
    if (separator == std::string::npos ||
        value.find_first_of("xX", separator + 1) != std::string::npos) {
        return false;
    }
    return ParsePositiveInt(value.substr(0, separator), width) &&
           ParsePositiveInt(value.substr(separator + 1), height);
}

bool ExecuteCommand(const std::string& line, PlayerBackend& backend)
{
    std::istringstream input(line);
    std::string command;
    std::string argument;
    std::string extra;
    if (!(input >> command)) {
        return true;
    }

    if (command == "quit" || command == "q" || command == "exit") {
        return false;
    }
    if (command == "help") {
        if (input >> extra) {
            std::cout << "Usage: help\n";
        } else {
            PrintHelp();
        }
        return true;
    }
    if (command == "status") {
        if (input >> extra) {
            std::cout << "Usage: status\n";
        } else {
            PrintStatus(backend);
        }
        return true;
    }
    if (command == "list") {
        if (!(input >> argument) || (input >> extra) ||
            (argument != "audio" && argument != "video")) {
            std::cout << "Usage: list <audio|video>\n";
            return true;
        }
        PrintTracks(
            backend,
            argument == "audio" ? MediaTrackType::Audio
                                : MediaTrackType::Video);
        return true;
    }
    if (command == "setaudio") {
        int index = 0;
        if (!(input >> argument) || (input >> extra) ||
            !ParsePositiveInt(argument, index)) {
            std::cout << "Usage: setaudio <positive-track-number>\n";
            return true;
        }
        backend.SelectAudioTrack(static_cast<std::size_t>(index));
        PrintStatus(backend);
        return true;
    }
    if (command == "setresolution") {
        int width = 0;
        int height = 0;
        if (!(input >> argument) || (input >> extra) ||
            !ParseResolution(argument, width, height)) {
            std::cout << "Usage: setresolution <width>x<height>|<height>p\n";
            return true;
        }
        backend.SelectVideoResolution(width, height);
        PrintStatus(backend);
        return true;
    }
    if (command == "abr") {
        if (!(input >> argument) || (input >> extra) ||
            (argument != "on" && argument != "off")) {
            std::cout << "Usage: abr <on|off>\n";
            return true;
        }
        backend.SetAutoBitrate(argument == "on");
        PrintStatus(backend);
        return true;
    }
    if (command == "play" || command == "pause" || command == "stop") {
        if (input >> extra) {
            std::cout << "Command takes no arguments.\n";
            return true;
        }
        if (command == "play") {
            backend.Play();
        } else if (command == "pause") {
            backend.Pause();
        } else {
            backend.Stop();
        }
        PrintStatus(backend);
        return true;
    }
    if (command == "seek") {
        double seconds = 0.0;
        if (!(input >> argument) || (input >> extra) ||
            !ParseSeekSeconds(argument, seconds)) {
            std::cout << "Usage: seek <non-negative-seconds>\n";
            return true;
        }
        backend.Seek(seconds);
        PrintStatus(backend);
        return true;
    }
    if (command == "trick") {
        int rate = 0;
        if (!(input >> argument) || (input >> extra) ||
            !ParseTrickRate(argument, rate)) {
            std::cout << "Usage: trick <1x|2x|4x|6x|12x|32x>\n";
            return true;
        }
        backend.SetTrickRate(rate);
        PrintStatus(backend);
        return true;
    }

    std::cout << "Unknown command: " << command << " (type 'help')\n";
    return true;
}

} // namespace

int ConsolePlayer::Run(PlayerBackend& backend)
{
    std::cout << "Video output is provided by Player Interface.\n";
    PrintHelp();
    std::cout << "pi-cli> " << std::flush;

    bool running = true;
    int exitCode = 0;
    while (running) {
        backend.PumpEvents();

        pollfd inputFd{STDIN_FILENO, POLLIN, 0};
        const int pollResult = poll(&inputFd, 1, 50);
        if (pollResult < 0) {
            if (errno == EINTR) {
                continue;
            }
            std::cerr << "Could not read console input: errno "
                      << errno << '\n';
            exitCode = 1;
            break;
        }
        if (pollResult == 0) {
            continue;
        }
        if ((inputFd.revents & POLLIN) != 0) {
            std::string line;
            if (!std::getline(std::cin, line)) {
                running = false;
                continue;
            }
            running = ExecuteCommand(line, backend);
            if (running) {
                std::cout << "pi-cli> " << std::flush;
            }
        } else if ((inputFd.revents & (POLLERR | POLLHUP | POLLNVAL)) != 0) {
            running = false;
        }
    }

    backend.Stop();
    return exitCode;
}
