# Player Interface Simulator

`pi-cli` is a standalone test application. Build and install
`playergstinterface` first; this directory then consumes its installed headers
and library through `libplayergstinterface.pc`. It does not need the root
project's `BUILD_PICLI` option or a root CMake change.

## Ubuntu build

Install the player interface to a local prefix. Use any additional options
required by your normal Ubuntu build:

```sh
cd ~/Desktop/workspace/tuntime/middleware-player-interface
cmake -S . -B build -DCMAKE_PLATFORM_UBUNTU=ON -DCMAKE_INSTALL_PREFIX="$HOME/.local"
cmake --build build --parallel
cmake --install build
```

Then build `pi-cli` independently with its script:

```sh
cd test/pi-cli
PLAYER_INTERFACE_PREFIX="$HOME/.local" bash ./build.sh
LD_LIBRARY_PATH="$HOME/.local/lib${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}" ./build/pi-player-simulator
```

The script sets `PKG_CONFIG_PATH` and `LD_LIBRARY_PATH`, checks the required
pkg-config modules, then configures/builds the standalone app. The Phase 1
scaffold prints `pi-player-simulator scaffold`. You can pass extra CMake
arguments to the script, for example `bash ./build.sh -DCMAKE_BUILD_TYPE=Debug`.

## Dependencies

The standalone CMake checks for:

- `libplayergstinterface` via its installed pkg-config file (this supplies the
  interface headers, library, and GStreamer compile/link requirements)
- `sdl2`, `egl`, and `glesv2` for the planned graphical UI

Later playback phases also need runtime GStreamer factories for `playbin`,
`appsrc`, `appsink`, HLS/DASH demux, H.264/AAC, and optionally `glimagesink`.
Check them on Ubuntu with `gst-inspect-1.0 <factory>`. Their package installation
will be handled after the Ubuntu environment confirms the needed packages.

The script currently checks and reports missing development dependencies; it
does not install OS packages. Automatic dependency installation can be added
later after the Ubuntu package list is verified. No player-interface source,
root CMake, or install script changes are needed to build this standalone test
app.
