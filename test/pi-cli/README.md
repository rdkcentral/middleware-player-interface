# Player Interface CLI Test Player

`pi-player-simulator` exercises `InterfacePlayerRDK` with real clear HLS and
DASH playback. FFmpeg fetches manifests and media over HTTP(S), demuxes
compressed audio/video packets in memory, and pi-cli passes those samples to
Player Interface through `SendHelper()`. Player Interface remains responsible
for decoding, synchronization, and rendering.

pi-cli does not create a playback pipeline, decode media, or write manifests,
segments, or samples to local files. FFmpeg packet queues are heap-backed and
bounded. Player Interface `NeedData` and `EnoughData` callbacks control sample
injection; its appsrc queues are capped at 512 KiB for video and 128 KiB for
audio so those callbacks provide backpressure. For DASH, pi-cli filters the MPD
in memory to the lowest-bandwidth
video representation and one non-commentary audio adaptation set before FFmpeg
opens the stream. This avoids opening every representation's initialization
segment and reduces software decode/render load on the default desktop sink.
HLS track selection and network reads are bounded by timeouts. DRM and
subtitles are out of scope.

The initial codec scope is H.264 video and AAC audio. FFmpeg's Annex-B H.264
packets are converted to the AVC length-prefixed form expected by Player
Interface's elementary-stream caps. AAC packets retain their FFmpeg framing:
ADTS for transport-stream inputs and raw AAC with codec configuration for
containerized inputs.

## Ubuntu build and run

Build and install `playergstinterface` first. For the repository's usual
`.libs` prefix:

```sh
cd ~/Desktop/workspace/player/middleware-player-interface
cmake -S . -B build -DCMAKE_PLATFORM_UBUNTU=ON -DCMAKE_INSTALL_PREFIX="$PWD/.libs"
cmake --build build --parallel
cmake --install build
```

Then build and run pi-cli as your regular user:

```sh
cd test/pi-cli
./build.sh
./run.sh "https://example.com/clear-stream.m3u8"
```

For DASH, pass a clear `.mpd` URL. The player-interface-owned output is used;
pi-cli does not open a second graphics window. `run.sh` configures the player
interface runtime library path. Running the binary directly does not inherit
that setting.

If Player Interface is installed to another prefix, set
`PLAYER_INTERFACE_PREFIX` before running both scripts. `build.sh` checks for
the Player Interface, GLib, FFmpeg, and libxml2 development modules, and
installs missing Ubuntu build dependencies through `sudo apt-get`. Run scripts
without `sudo`. The script does not build or install Player Interface itself.

## Console controls

```text
play
pause
seek 45
trick 2x
trick 4x
trick 6x
trick 12x
trick 32x
trick 1x
stop
status
quit
```

`seek` takes a target in seconds. The command repositions FFmpeg and flushes
Player Interface. Trick-rate commands flush Player Interface and reposition
the source at the current playback time. Actual trick playback depends on the
Player Interface platform implementation and stream support.

## Startup diagnostics

Timing milestones use monotonic elapsed time and are printed to stderr, for
example `[PI-CLI] +1234ms ffmpeg-manifest-and-stream-info-ready`. They cover
DASH manifest fetch/filter, FFmpeg open, stream selection/probing, initial packet arrival, Player
Interface setup, first sample processing, first decoded frame, and advancing
playback position. The first five packet timestamps, feed-control transitions,
sample submission results, and periodic playback position are also logged to
diagnose stalls. Network reads time out instead of waiting indefinitely.
Player Interface bus errors and FFmpeg ingest failures are reported with their
details.

## Validation

Run `./build.sh`, then try one clear HLS and one clear DASH stream on Ubuntu.
Verify audio/video output, pause/resume, seek, EOS/error reporting, and clean
shutdown. This Windows workspace cannot establish Linux hardware output or
runtime behavior.
