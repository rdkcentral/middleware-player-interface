# Player Interface Simulator

`pi-cli` is a standalone test application. Build and install
`playergstinterface` first; this directory then consumes its installed headers
and library through `libplayergstinterface.pc`. It does not need the root
project's `BUILD_PICLI` option or a root CMake change.

## Ubuntu build

Install the player interface to a local prefix. If you use the repository's
usual `.libs` install prefix, the simulator script detects it automatically.
Otherwise, use any additional options required by your normal Ubuntu build.
For example, to install under `.libs`:

```sh
cd ~/Desktop/workspace/tuntime/middleware-player-interface
cmake -S . -B build -DCMAKE_PLATFORM_UBUNTU=ON -DCMAKE_INSTALL_PREFIX="$PWD/.libs"
cmake --build build --parallel
cmake --install build
```

Then build `pi-cli` independently as your regular user (do not use `sudo`):

```sh
cd test/pi-cli
./build.sh
LD_LIBRARY_PATH="../../.libs/lib${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}" ./build/pi-player-simulator
```

When the interface was installed to another prefix, set `PLAYER_INTERFACE_PREFIX`
to that prefix when running `./build.sh`, and use its `lib` directory in
`LD_LIBRARY_PATH` when launching the executable. The script sets
`PKG_CONFIG_PATH` and `LD_LIBRARY_PATH`, checks the required pkg-config modules,
then configures/builds the standalone app. You can pass extra CMake arguments
to the script, for example
`./build.sh -DCMAKE_BUILD_TYPE=Debug`.

## Dependencies

The standalone CMake checks for:

- `libplayergstinterface` via its installed pkg-config file (this supplies the
  interface headers, library, and GStreamer compile/link requirements)
- `sdl2`, `SDL2_ttf`, `egl`, and `glesv2` for the SDL window, readable text,
  and OpenGL ES UI

Later playback phases also need runtime GStreamer factories for `playbin`,
`appsrc`, `appsink`, HLS/DASH demux, H.264/AAC, and optionally `glimagesink`.
Check them on Ubuntu with `gst-inspect-1.0 <factory>`. Their package installation
will be handled after the Ubuntu environment confirms the needed packages.

On Ubuntu, `build.sh` checks for the build tools, SDL2/EGL development modules,
and a supported system font. If any are missing, it installs the corresponding
APT packages using `sudo` (and may prompt for your password). The locally built
`playergstinterface` package is still a prerequisite and must be installed
separately. No player-interface source, root CMake, or install script changes
are needed to build this standalone test app.

## Phase 2 UI test

Run the app using the commands in the Ubuntu build section. The window provides an editable URL field, mocked Play/Pause/Stop controls, a
status line, and a video placeholder. Text is rendered with SDL_ttf and the
system DejaVu Sans font. Type a URL, click the controls to verify the status,
resize the window, and close it. Playback is not connected in this phase.
