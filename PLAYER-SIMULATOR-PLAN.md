# Player Interface Simulator Plan

## Goal

Build a lightweight Linux test player under `test/pi-cli/` with an EGL-based
control UI and direct playback through `InterfacePlayerRDK`. The simulator must
not construct a separate GStreamer playback pipeline or modify the
player-interface implementation.

Initial stream scope: clear HTTP(S) HLS and DASH; DRM and subtitles are
disabled.

## Architecture

- Build/install `playergstinterface` separately using its normal build.
- Build `test/pi-cli` independently using its own CMake project and consume the
  installed library through `libplayergstinterface.pc`.
- Keep the CLI entry point small; put SDL/EGL rendering under `src/ui/` and
  player-interface calls under `src/backend/`.
- Pass the command-line URL to `InterfacePlayerRDK::ConfigurePipeline()`.
  The interface's own `playbin` performs URI source setup, adaptive demux,
  decoding, and sink output. The simulator must not create another pipeline,
  use `uridecodebin`/`appsink`, or feed samples through `SendHelper`.
- Use `InterfacePlayerRDK` for pause, stop, position, duration, and bus errors.
- Do not modify player-interface production or vendor code.
- The default Ubuntu video sink may use a native window separate from SDL/EGL.
  Video/control z-order is an Ubuntu validation item, not an assumed feature.

`Configs::media` is an integer in the installed public API; the media-format
enum itself is private. The simulator selects the direct-URI/progressive mode
and disables its appsrc option so the URL is handled by the interface-owned
`playbin`.

## Phases and test gates

### Phase 0 — Branch and clean test folder — complete

- Work on `feature/Simulator`, based on `develop`.
- Keep the simulator isolated under `test/pi-cli/`.
- Keep production player-interface code unchanged.

### Phase 1 — Standalone build and scaffold — complete

- Configure/build directly from `test/pi-cli/`.
- Use the installed `playergstinterface` pkg-config module.
- Let `build.sh` set pkg-config/runtime paths and install missing Ubuntu build,
  UI, font, and GStreamer runtime dependencies.
- Do not require the root CMake project or `BUILD_PICLI`.

**Test gate:** standalone CMake configure/build and launch on Ubuntu.

### Phase 2 — EGL UI — implemented; visual test pending

- Keep the entry point small and UI code in `src/ui/`.
- Accept the stream URL only as `./build/pi-player-simulator <url>`.
- Render play/pause, stop, status, and a playback timeline in an overlay.
- Use SDL2_ttf with a system font and hide controls after inactivity while
  playing.

**Test gate:** launch with a URL; verify the EGL renderer, controls, keyboard
shortcuts, resize behavior, and clean close on Ubuntu.

### Phase 3 — Interface-owned direct-URI playback — implemented; Ubuntu test pending

- Initialize GStreamer through `InterfacePlayerRDK`.
- Configure its own pipeline with the supplied URL in direct-URI mode.
- Let the interface-owned `playbin` process HLS/DASH; do not add a separate
  source or ingest pipeline.
- Route pause, stop, status, and bus errors through the interface API.

**Test gate:** play one clear HLS URL and one clear DASH URL on Ubuntu; verify
video/audio output, pause/resume, stop, EOS/error reporting, and teardown.

### Phase 4 — Video/UI composition and seeking — validate before expanding

- Record whether the default Ubuntu sink can be layered with the SDL/EGL window.
- Do not use private player internals or alter player-interface production code
  to force z-order.
- The current public direct-URI API has no seek operation; the timeline is
  display-only until a supported public seek path is available.

**Test gate:** document actual native-window behavior and confirm no UI change
breaks playback or shutdown.

## Progressive testing rule

Every phase must have an exact command and expected result in
`test/pi-cli/README.md`. Run each phase's checks before continuing. Ubuntu
GStreamer/EGL rendering and z-order must be confirmed on Ubuntu; they cannot be
inferred from the Windows development environment.
