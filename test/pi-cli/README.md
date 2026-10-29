# Player Interface Simulator

`pi-player-simulator` is a standalone test application that consumes the
installed `playergstinterface` package. Playback is configured and controlled
through `InterfacePlayerRDK`; the simulator does not create its own GStreamer
pipeline or inject media samples.

The initial playback scope is clear HTTP(S) HLS and DASH. DRM and subtitles
are disabled.

## Ubuntu build

Build and install `playergstinterface` first. For the repository's usual
`.libs` prefix:

```sh
cd ~/Desktop/workspace/tuntime/middleware-player-interface
cmake -S . -B build -DCMAKE_PLATFORM_UBUNTU=ON -DCMAKE_INSTALL_PREFIX="$PWD/.libs"
cmake --build build --parallel
cmake --install build
```

Then build and run `pi-cli` as your regular user:

```sh
cd test/pi-cli
./build.sh
./run.sh "https://example.com/clear-stream.m3u8"
```

For DASH, pass a clear `.mpd` URL instead. The simulator starts playback
immediately from the command-line URL. There is no URL editor in the UI.
The launcher sets the player-interface runtime library path before starting
the simulator; running the binary directly does not inherit environment
variables set by `build.sh`.

If `playergstinterface` is installed to another prefix, set
`PLAYER_INTERFACE_PREFIX` before running `./build.sh` and `./run.sh`. The build
script configures the pkg-config paths, checks the required modules, and
installs missing Ubuntu build, UI, and GStreamer runtime packages through
`sudo apt-get`. Run the scripts without `sudo`.

The script does not build or install `playergstinterface`; that remains a
separate prerequisite. Extra CMake options can be passed to the script, for
example `./build.sh -DCMAKE_BUILD_TYPE=Debug`.

## Playback and controls

The backend sets `InterfacePlayerRDK` to its direct-URI playback mode and
passes the URL to `ConfigurePipeline()`. The interface's own `playbin` handles
HTTP source setup, HLS/DASH demuxing, decoding, and sink output. The simulator
does not make a parallel `uridecodebin`, `appsink`, or other media pipeline.

Use the overlay's play/pause and stop buttons, or **Space** and **S**.
Playback position and duration are displayed in the timeline. Seeking is not
connected: the public direct-URI interface currently exposes no seek operation.
The SDL/EGL controls and the interface-owned video sink may appear in separate
native windows; their z-order must be confirmed on Ubuntu and is not guaranteed
by this simulator.

## Dependencies checked by `build.sh`

- Installed `libplayergstinterface` pkg-config module
- `glib-2.0`, SDL2, SDL2_ttf, EGL, and GLESv2 development modules
- SDL2_ttf-compatible system font
- GStreamer `playbin`, `souphttpsrc`, HLS/DASH demuxers, `autovideosink`,
  and H.264/AAC decoders

`build.sh` installs missing Ubuntu packages when possible and reports any
runtime factories that remain unavailable.
