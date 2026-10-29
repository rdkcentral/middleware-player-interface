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

On Linux/X11, the UI locates the interface-owned `pi-player-simulator` video
window and places an X11-shaped SDL controls window over it. Only the header and
controls regions cover the video; the rest of the overlay window is clipped.
This keeps the interface-owned pipeline and sink while composing the UI in the
simulator. On desktops without X11/XWayland, controls remain in a separate
window.

Use the play/pause and stop buttons, or **Space** and **S**. Playback position
and duration are displayed in the timeline. Seeking is not connected: the
public direct-URI interface currently exposes no seek operation.

Startup timing milestones are printed to stderr with elapsed monotonic time,
for example `[PI-CLI] +1234ms configure-pipeline-returned`. They include
plugin initialization, pipeline configuration, pipeline state notifications,
the interface's first-frame notification, decoder first-frame callback when
available, and first advancing playback position. These events measure startup;
they do not accelerate stream loading.

## Dependencies checked by `build.sh`

- Installed `libplayergstinterface` pkg-config module
- `glib-2.0`, SDL2, SDL2_ttf, EGL, GLESv2, X11, and Xext development modules
- SDL2_ttf-compatible system font
- GStreamer `playbin`, `souphttpsrc`, HLS/DASH demuxers, `autovideosink`,
  and H.264/AAC decoders

`build.sh` installs missing Ubuntu packages when possible and reports any
runtime factories that remain unavailable.
