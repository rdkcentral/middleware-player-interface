# Player Interface Simulator Plan

## Goal

Build a lightweight Linux console test player under `test/pi-cli/` with direct
playback through `InterfacePlayerRDK`. The simulator must not construct a
separate GStreamer playback pipeline or modify the player-interface
implementation.

Initial stream scope: clear HTTP(S) HLS and DASH; DRM and subtitles are
disabled.

## Architecture

- Build/install `playergstinterface` separately using its normal build.
- Build `test/pi-cli` independently using its own CMake project and consume the
  installed library through `libplayergstinterface.pc`.
- Keep the CLI entry point small; put command parsing under `src/console/` and
  player-interface calls under `src/backend/`.
- Pass the command-line URL to `InterfacePlayerRDK::ConfigurePipeline()`.
  The interface's own `playbin` performs URI source setup, adaptive demux,
  decoding, and sink output. The simulator must not create another pipeline,
  use `uridecodebin`/`appsink`, or feed samples through `SendHelper`.
- Use `InterfacePlayerRDK` for play, pause, stop, seek/trick-play, position,
  duration, and bus errors.
- Do not modify player-interface production or vendor code.
- Let the interface-owned default GStreamer sink display video in its native
  window; the simulator does not create a second graphics window.

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
- Let `build.sh` set pkg-config/runtime paths and install missing Ubuntu build
  and GStreamer runtime dependencies.
- Do not require the root CMake project or `BUILD_PICLI`.

**Test gate:** standalone CMake configure/build and launch on Ubuntu.

### Phase 2 — Console controls — implemented; Ubuntu test pending

- Keep the entry point small and console command parsing in `src/console/`.
- Accept the stream URL only as `./build/pi-player-simulator <url>`.
- Support `play`, `pause`, `seek <seconds>`, `trick <rate>`, `stop`, `status`,
  `help`, and `quit`.
- Use the interface-owned GStreamer video sink window; do not create an SDL/EGL
  UI or an X11 overlay.

**Test gate:** launch with a URL; verify console commands, GStreamer bus
processing during input, video output, and clean shutdown on Ubuntu.

### Phase 3 — Interface-owned direct-URI playback — implemented; Ubuntu test pending

- Initialize GStreamer through `InterfacePlayerRDK`.
- Configure its own pipeline with the supplied URL in direct-URI mode.
- Let the interface-owned `playbin` process HLS/DASH; do not add a separate
  source or ingest pipeline.
- Route play, pause, stop, seek/trick-play, status, and bus errors through the
  interface API.

**Test gate:** play one clear HLS URL and one clear DASH URL on Ubuntu; verify
video/audio output, pause/resume, stop, EOS/error reporting, and teardown.

### Phase 4 — Playback command validation — validate before expanding

- Validate direct-URI seeking and trick rates through the public
  `InterfacePlayerRDK::Flush()` method; report if a sink or stream rejects a
  requested operation.
- Keep the interface-owned video sink in control of its native output window.

**Test gate:** verify seek targets and trick rates on a clear DASH stream, then
confirm stop and quit tear down the pipeline cleanly.

## Progressive testing rule

Every phase must have an exact command and expected result in
`test/pi-cli/README.md`. Run each phase's checks before continuing. Ubuntu
GStreamer output and playback controls must be confirmed on Ubuntu; they cannot
be inferred from the Windows development environment.
