# Player Interface Simulator — Implementation Plan

## Goal

Create a small Linux test player under `test/pi-cli/` with an EGL-based UI. The
existing `InterfacePlayerRDK` GStreamer sink renders video; the UI draws the
controls and video layout. Start with clear DASH/HLS and ignore DRM/subtitles.

## Keep the build simple

- Build/install `playergstinterface` separately using its normal build.
- Build `test/pi-cli` independently using its own `CMakeLists.txt`.
- `pi-cli` consumes the installed headers and library through the existing
  `libplayergstinterface.pc` pkg-config file.
- Do not add `test/pi-cli` to the root CMake project. Do not change
  player-interface business code, vendor code, root CMake, or
  `install-middleware.sh` for the initial phases.
- `test/pi-cli` needs pkg-config modules `libplayergstinterface`, `sdl2`,
  `egl`, and `glesv2`. The interface pkg-config file supplies its GStreamer
  compile/link requirements.

The standalone Ubuntu build commands are maintained in
[`test/pi-cli/README.md`](./test/pi-cli/README.md).

## UI and video layout

SDL2 handles the window/input loop; EGL/OpenGL ES draws the UI surface. Video
comes from the interface-owned GStreamer sink, not a second player.

The existing `SetVideoRectangle()` API may position video in the UI surface,
but that depends on the default Ubuntu sink and its `rectangle` property. Test
this before building more UI around it. Do not access private player internals.
If the existing public API cannot render video in the surface, pause and agree
on a separate-window fallback or an API change before proceeding.

## Phases

### Phase 0 — Branch and clean test folder — complete

- Work on `feature/Simulator`, based on `develop`.
- Replace the old interactive `pi-cli` prototype under `test/pi-cli/`.
- Keep production player-interface code unchanged.

### Phase 1 — Standalone CMake and scaffold — complete

- CMake configures directly from `test/pi-cli/`; it finds the installed
  `playergstinterface` package using pkg-config.
- `test/pi-cli/build.sh` sets the local install-prefix paths, checks required
  pkg-config modules, installs missing Ubuntu build/UI packages when needed,
  then configures and builds the executable.
- The interface package remains a local prerequisite; the script does not
  replace building/installing `playergstinterface`.
- List required build/runtime dependencies in the README. The script reports
  dependency failures explicitly if automatic installation is unavailable or
  fails.
- Do not require `BUILD_PICLI`, the root CMake project, or changes to the
  install script.

**Test gate:** passed on Ubuntu. The standalone build found all four pkg-config
modules, built the executable, and running it printed
`pi-player-simulator scaffold`.

### Phase 2 — EGL UI — implemented; Ubuntu test pending

- Draw a URL field, Play/Pause/Stop controls, status, and a video rectangle.
- Use SDL_ttf with a system font instead of custom bitmap glyphs.
- SDL2_ttf and the system DejaVu Sans font provide readable UI text.
- Keep playback mocked in this phase.

**Test gate:** from `test/pi-cli`, run `./build.sh`, then run
`LD_LIBRARY_PATH="../../.libs/lib${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}" ./build/pi-player-simulator`.
Enter a URL, verify Play/Pause/Stop update the mocked status, resize the window,
and close it cleanly.

### Phase 3 — Connect the player interface

- Call the public `InterfacePlayerRDK` API and feed a short local clear
  H.264/AAC source.
- Verify startup, sample timestamps, sink output, `SetVideoRectangle()`, EOS,
  and teardown.

**Test gate:** successful local playback on the Ubuntu default SoC path; record
whether video renders in the EGL surface or needs the agreed fallback.

### Phase 4 — DASH/HLS input

- Add a GStreamer adaptive-demux ingest path that sends supported compressed
  samples to the interface through `SendHelper`.
- Validate HLS first, then DASH. Reject unsupported/encrypted media clearly.

**Test gate:** play one local clear fixture of each format on Ubuntu.

### Phase 5 — Playback controls and finish

- Connect controls/status to playback and handle pause, resume, stop, EOS,
  errors, and URL replacement.
- Add only small, redistributable test fixtures if needed.

**Test gate:** repeatable playback/control/stop tests pass and shutdown leaves
no queued samples or callbacks.

## Progressive testing rule

Every phase must be independently testable. Document its exact command and
expected result in the README, run that phase's check before continuing, and
stop to resolve failures before starting the next phase. Ubuntu/EGL results
must come from the user's Ubuntu environment; do not infer them from this
Windows workspace.

## References

- `InterfacePlayerRDK.cpp`, `InterfacePlayerRDK.h`, `GstUtils.cpp`
- [`appsrc`](https://gstreamer.freedesktop.org/documentation/app/appsrc.html)
- [`playbin`](https://gstreamer.freedesktop.org/documentation/playback/playbin.html)
- [GStreamer DASH demuxer](https://gstreamer.freedesktop.org/documentation/adaptivedemux2/dashdemux2.html)
- [GStreamer GL image sink](https://gstreamer.freedesktop.org/documentation/opengl/glimagesink.html)
