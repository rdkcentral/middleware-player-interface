# Linux Player-Interface Simulator Plan

## Goal

Build a lightweight Linux desktop simulator that accepts an unencrypted DASH
(`.mpd`) or HLS (`.m3u8`) URL, starts playback through the existing
`InterfacePlayerRDK` API, and provides a graphical control surface. The target
is the repository's existing Ubuntu/default SoC path. The plan does not change
player-interface business code, public APIs, or vendor implementations.

Subtitles, encrypted streams/DRM, and RDK hardware-specific behavior are out of
scope. The simulator must report unsupported input clearly rather than imply
that it played successfully.

## Current implementation status

- Working branch: `feature/Simulator`, created from `origin/develop`.
- Phase 1 scaffold and dependency inventory are implemented; Ubuntu configure,
  build, and dependency verification are pending.
- Ubuntu configure/build and runtime plugin checks are pending validation in the
  user's Ubuntu environment.

## Findings that shape the design

- On Ubuntu and macOS, `vendor/SocInterface.cpp` selects
  `DefaultSocInterface`; `CMakeLists.txt` also includes the default vendor
  source and excludes hardware-specific implementations on these platforms.
- The default implementation is already simulator-aware, sets Ubuntu H.264
  caps to access-unit/AVC format, and does not request Westeros or Rialto sinks.
- `InterfacePlayerRDK::ConfigurePipeline()` creates per-track `playbin`
  pipelines configured with `appsrc://`. The manifest URL is passed as setup
  metadata; it is not itself a request to play the manifest.
- The player accepts compressed/timestamped samples via `SendHelper(MediaSample&&, ...)`.
  Therefore a separate ingest/demux adapter is required between a manifest URL
  and the player interface.
- `pi-cli` is a manual API command tool, not a complete simulator. Its fragment
  injection call uses an older `SendHelper` signature and must be reconciled
  with the current API before it can serve as the backend.
- The interface creates a GStreamer bus watch. A GLib main loop is needed for
  bus callbacks and UI integration; an interactive command loop alone is not
  enough for a responsive GUI player. The interface attaches the watch to the
  default GLib main context and does not run its own `GMainLoop`; the UI event
  loop must also dispatch that context in the same process.
- `SendHelper()` converts sample PTS, DTS, and duration from seconds to
  nanoseconds. `mPtsOffset` is only used for subtitle timestamp restamping and
  should remain zero for this subtitle-free MVP.
- For the default Ubuntu path, `GetCaps(GST_FORMAT_VIDEO_ES_H264)` creates
  `video/x-h264` caps and adds `alignment=au, stream-format=avc`. AAC has
  separate ADTS (`GST_FORMAT_AUDIO_ES_AAC`) and raw
  (`GST_FORMAT_AUDIO_ES_AAC_RAW`) formats. These are sample-stream contracts,
  not a request to inject MP4 boxes into elementary-stream caps.
- The public API shown in this repository has no supported way to inject a
  UI/EGL window handle or replace the interface's video sink. Do not reach
  into `InterfacePlayerPriv` or alter production code to embed video.

## Recommended approach

Create a **test-only desktop application** under `test/pi-cli/`, built
on top of `playergstinterface`:

1. **SDL2 + EGL/OpenGL ES** for an explicit EGL graphical surface, URL input,
   controls, and the video-region layout. Pump the default GLib context from
   the UI event loop so GStreamer bus watches run on the same thread.
2. **Existing `InterfacePlayerRDK` API** for playback lifecycle, buffering,
   pause/resume, stop, position, and error callbacks.
3. **A separate GStreamer ingest pipeline** for URI handling and adaptive
   demux. It fetches MPD/HLS, selects supported unencrypted audio/video
   elementary streams, and delivers timestamped compressed samples to the
   interface through `SendHelper`.
4. **The interface's default Linux video sink** for rendering. First validate
   whether the installed GStreamer sink stack can provide EGL-accelerated
   output (for example via `glimagesink` selected by `autovideosink`) and
   whether the existing `SetVideoRectangle()` path can place it in the UI's
   reserved video area.

This reuses GStreamer already required by the interface and avoids integrating
a second standalone player such as VLC or mpv, which would bypass the API being
tested.

### Important GUI/EGL boundary

EGL is a graphics API, not a UI toolkit. SDL2 can provide the window and input
events while EGL/OpenGL ES draws the simulator UI; GStreamer's `glimagesink`
may separately use EGL to render video. Using EGL for the controls does not
automatically make the interface's video appear inside the controls surface.

**Target UX:** one EGL-capable graphical surface with a video region and UI
controls/graphics around or over it; video pixels come from the
interface-owned GStreamer sink, not from a second player. The existing
`SetVideoRectangle(x, y, w, h)` API looks relevant, but its implementation only
sets a `rectangle` property when `enableRectPropertyCfg` is enabled and a video
sink is available. The default Ubuntu path and the selected generic sink may not
support that property or expose a native window handle through the public API.
Therefore Phase 3 must test whether the current public API can achieve the
single-surface layout without private internals or production-code changes.

If that test fails, video can still be rendered by the interface sink, but it
may appear in a separate sink-owned window; same-surface video then requires an
approved public sink/window integration point and is not guaranteed under the
no-interface-change constraint. EGL support itself also depends on the runtime
GStreamer plugin, display server, and driver.

## Component design

### 1. EGL application and lifecycle controller

- UI: URL entry, Play, Pause/Resume, Stop, position/duration, and status/error
  graphics on the EGL surface. Keep controls intentionally small; no playlist,
  subtitle, DRM, or advanced trick-play UI in the first version.
- Pump SDL events and the default GLib context on the UI thread. Do not draw
  directly from GStreamer streaming threads; marshal UI updates to the render
  thread.
- Controller owns the `InterfacePlayerRDK` instance and the ingest pipeline.
  Use explicit states such as `Idle`, `Preparing`, `Playing`, `Paused`,
  `Stopping`, `Ended`, and `Error`.
- On stop, URL replacement, window close, or failure: stop the ingest pipeline,
  prevent new sample callbacks, wait for in-flight callbacks to finish, stop
  and destroy the interface pipeline using its public API, then release the
  ingest pipeline and interface instance. Keep UI state changes on the render
  thread. Streaming-thread callbacks must marshal UI work to that thread; never
  assume the callback is on the UI thread.
- Dispatch the interface's bus watch through the default GLib main context from
  the UI event loop; do not create or block a second main loop. Validate calls that start/stop
  pipelines from UI callbacks and guard against callbacks arriving during
  teardown.

### 2. Manifest ingest and sample adapter

- Normalize local paths and HTTP(S) URLs to valid URIs; reject unsupported
  schemes and malformed input in the UI.
- Build an independent ingest pipeline with GStreamer's URI/adaptive-demux
  support (such as `uridecodebin3`/adaptive demux) and encoded-media output
  connected to appsinks. Constrain output to supported compressed caps so the
  ingest pipeline does **not** decode and render a second copy of the media.
- Handle dynamic audio/video pads and caps changes. Select one supported video
  track and, when present, one supported audio track. Ignore subtitle pads.
- Map negotiated caps to the interface's `GstStreamOutputFormat` values.
  Phase 1 supports only H.264 video (`video/x-h264`, AVC, access-unit aligned)
  and AAC audio (select ADTS or raw only when matching the interface enum and
  negotiated caps). Reject other codecs/profiles or formats with the detected
  caps and a clear UI message; add support only after validation.
- Copy or retain each appsink buffer safely in a `MediaSample`, preserving PTS,
  DTS, and duration in seconds (the interface converts these to nanoseconds).
  Leave subtitle-only `mPtsOffset` at zero. Transfer the sample through the
  sole current public `SendHelper(int, MediaSample&&, bool, bool&, bool&, bool&,
  bool&, bool&)` signature. Its `initFragment` argument is a processing-state
  signal; it does not mean that an MP4 initialization box can be passed as an
  H.264 access unit. Set init/discontinuity/segment flags only after verifying
  the required sequence against `SendHelper()` and a real fixture.
- Respect interface backpressure (`need-data`/`enough-data`) so the ingest side
  cannot grow unbounded queues. Configure a bounded appsink queue (initial
  target: at most 8 queued buffers per selected track, with dropping disabled)
  and pause/block upstream production when full; measure actual queue bytes and
  tune using representative streams. Keep per-track ordering and handle EOS,
  errors, seeks, and caps/period changes deliberately.
- Call `ConfigurePipeline()` with formats matching the adapter's samples and
  the manifest URI required for setup. Do not assume that passing the URL alone
  starts playback; setup and sample feeding are separate steps.

**Feasibility gate:** Before building out the GUI, prove that a short clear
H.264/AAC DASH fixture can be demuxed into the exact sample framing/caps expected
by the default Ubuntu interface, then play through `SendHelper`. Repeat for HLS.
In particular, verify init-segment handling, `avc` access-unit framing, AAC
ADTS/raw expectations, timestamps, and discontinuity behavior. If the
interface's injection semantics require fragment-level data rather than
individual demuxed access units, adapt the ingest boundary to supply those
fragments; do not guess or patch production behavior.

The spike's written result must include: selected enum and negotiated caps for
each track; proof that buffers are access-unit payloads rather than container
boxes; one concrete init/first-sample/steady-state/discontinuity sequence with
flag values; sample PTS/DTS/duration units and a monotonicity check; success or
failure from `SendHelper`; and a playback/EOS result. For H.264 the expected
default caps are AVC access units. For AAC, use ADTS only when the adapter
actually receives ADTS; raw AAC requires a compatible decoder configuration
and must not be mislabeled.

### 3. Video output and EGL validation

- Use the interface's normal default Linux sink selection; do not make a
  separate GStreamer pipeline the visible player.
- Check plugin availability (`gst-inspect-1.0 playbin`, `appsrc`,
  `glimagesink`, and the applicable DASH/HLS demuxers) and run a real playback
  smoke test under the target desktop session.
- Prefer EGL/OpenGL output only where the installed GStreamer GL plugin,
  display server, and driver support it. Record the selected sink in simulator
  diagnostics only if it can be observed through public APIs; otherwise log
  available sink candidates and enable documented GStreamer debug logging.
  Provide a clear startup error or documented fallback when no usable video
  sink is available. Verify sink creation during playback rather than assuming
  that build-time plugin presence means runtime EGL works.
- Target the single EGL surface/video-region layout above. If the feasibility
  gate shows the existing sink cannot render into that region, document the
  separate sink-window fallback in the UI. Report sink/display errors in the
  control surface. Headless CI validates controller and sample flow separately;
  a display-enabled smoke test validates visible video, rectangle behavior, and
  EGL where available.

## Build and repository integration

- Keep the simulator as a test-only target under `test/pi-cli/` with its own
  `CMakeLists.txt`. Do not change the root/business CMake file in the initial
  phases. Later, integrate this subdirectory from the root CMake only for the
  Ubuntu platform (and keep it opt-in so normal product builds stay unchanged).
- Link to `playergstinterface`, GStreamer app/base/video/adaptive-demux
  dependencies already present where possible, and SDL2/EGL/OpenGL ES. Discover
  dependencies with `pkg-config`/CMake; fail configuration with an actionable
  message only when the optional simulator is explicitly enabled.
- Do not add GUI dependencies to the normal player-interface library or
  production install targets.
- Reuse existing logging and public callback registration patterns. Keep
  simulator-specific source and configuration under `test/pi-cli/`.
- Fix the test-only `pi-cli` API mismatch only if shared backend code or
  compilation requires it; do not change production interface behavior as
  part of that repair.

## Implementation plan (phased)

### Working agreement

- All implementation happens on a feature branch created from the current
  `develop` branch (`feature/Simulator`). Do not
  develop directly on `develop`.
- Work stays under `test/pi-cli/`. Remove the existing `pi-cli` attempt on the
  feature branch and replace it with the structure below; first review and
  preserve any useful behavior, but do not modify production player-interface
  sources, public headers, vendor code, or root/business CMake during the early
  phases.
- Each phase is a separate review/test checkpoint. Stop at its exit criteria,
  provide the exact Ubuntu build/run/test steps, and wait for Ubuntu validation
  before starting the next phase. Do not claim Linux/EGL validation from this
  Windows development environment.
- Every phase must be independently testable and leave the project in a
  runnable, understood state. Add phase-specific test commands, expected
  results, and failure diagnostics to `test/pi-cli/README.md`; run them before
  moving forward. A phase is not complete just because its code compiles.
- Dependencies are documented and discovered by the new CMake files first.
  `install-middleware.sh` and root CMake integration are deferred until the
  integration phase.

### Progressive test gates

| Phase | Test before proceeding |
|---|---|
| 0 — Branch/replacement | Confirm the active branch is `feature/Simulator`, the old prototype is gone, and only intended `test/pi-cli/` files changed. |
| 1 — CMake/dependencies | On Ubuntu, configure with `BUILD_PICLI=ON`, build and run the scaffold, verify required pkg-config modules, and record available GStreamer factories. |
| 2 — EGL UI | Launch the UI, enter/edit a URL, resize the surface, observe the UI/video rectangle, then close cleanly. Record display/EGL diagnostics. |
| 3 — Player/sink | Play a known local clear H.264/AAC source through `InterfacePlayerRDK`; verify source setup, samples/timestamps, sink output, rectangle behavior, EOS, and teardown. |
| 4 — Manifest ingest | Test HLS first and DASH separately using local fixtures; confirm negotiated caps, sample flow, playback, and EOS for each before adding the other format. |
| 5 — Controls/reliability | Exercise Play/Pause/Resume/Stop, URL replacement, EOS, malformed/unsupported input, callback teardown, and bounded queues with repeatable tests. |
| 6 — Build integration | Configure/build with the simulator enabled and disabled on Ubuntu; verify non-Ubuntu/default product builds do not gain simulator dependencies. |
| 7 — Acceptance | Run the documented end-to-end Ubuntu checklist, including video-region/EGL verification or the approved fallback. |

If a gate fails, stop at that phase, capture the exact command, error/logs, and
environment details, fix or explicitly revise the plan, then rerun that phase's
checks before continuing.

### Target `test/pi-cli/` structure

```text
test/pi-cli/
  CMakeLists.txt
  README.md
  src/
    main.cpp
    app/
      PlayerApplication.h
      PlayerApplication.cpp
      PlayerController.h
      PlayerController.cpp
    ui/
      EglUi.h
      EglUi.cpp
    player/
      InterfacePlayerAdapter.h
      InterfacePlayerAdapter.cpp
    ingest/
      ManifestIngestPipeline.h
      ManifestIngestPipeline.cpp
      SampleAdapter.h
      SampleAdapter.cpp
  tests/
    unit/
    integration/
  fixtures/
    README.md
```

Keep the UI, player-interface adapter, and manifest ingest responsibilities
separate. `EglUi` owns the EGL surface and input/render loop; the controller owns
playback state; the adapter is the only layer that calls `InterfacePlayerRDK`;
the ingest/sample adapter turns manifest output into interface-compatible
`MediaSample`s.

### Dependencies to track

The new `test/pi-cli/CMakeLists.txt` will use `PkgConfig`/CMake checks and fail
with a clear message when an explicitly enabled simulator dependency is absent.
For now, do not add packages to `install-middleware.sh`.

| Dependency | Use | Initial handling |
|---|---|---|
| CMake, C++17 compiler, pkg-config | Configure/build | Use existing development setup |
| Existing `playergstinterface` build dependencies | Build the middleware library | Continue using the repository's current build/install setup |
| GStreamer core, app, base, and video development packages | Player API and appsink/appsrc sample bridge | Discover with pkg-config in the test CMake |
| GStreamer adaptive-demux runtime plugins for DASH and HLS | Manifest ingest | Check installed factories at startup; confirm exact Ubuntu package names during Ubuntu validation |
| GStreamer GL plugin and an EGL-capable display/driver | Sink rendering and EGL smoke test | Runtime capability check; not a hard compile dependency unless the UI directly links GL |
| SDL2, EGL, and OpenGL ES development packages | EGL surface, events, UI drawing | Discover with pkg-config/CMake in the test CMake |
| Optional GStreamer encoders/packagers | Generate small local DASH/HLS fixtures | Test-only; do not make a runtime player dependency |

After Ubuntu validation identifies the exact package names, add only the
simulator-specific packages to `install-middleware.sh` in the later integration
phase. Keep those dependencies out of the production library target.

### Phase 0 — Feature branch and legacy `pi-cli` replacement

1. Confirm the working tree is safe to branch from; preserve unrelated changes.
2. Create `feature/Simulator` from `develop`.
3. Inventory the current `test/pi-cli` files and the existing root CMake hook.
4. On the feature branch, remove the old command-line prototype files and
   create the directory structure above. Keep the `pi-cli` folder as requested;
   the graphical executable may use a clearer target name such as
   `pi-player-simulator`.

**Exit criteria:** feature branch is based on `develop`; legacy prototype is
replaced only in `test/pi-cli/`; no production files or root CMake have changed.

### Phase 1 — Standalone test CMake and dependency inventory

1. Implement the independent `test/pi-cli/CMakeLists.txt` for the simulator
   sources and dependency discovery.
2. Initially build it through the repository's existing opt-in `BUILD_PICLI`
   root hook, without editing root/business CMake. Do not edit
   `install-middleware.sh` yet.
3. Record required pkg-config module names, runtime plugin factories, Ubuntu
   package names still to verify, and useful `gst-inspect-1.0` checks in the
   `test/pi-cli/README.md`.
4. Run configure/build checks in the user's Ubuntu environment; report missing
   packages explicitly rather than adding undocumented install steps.

**Exit criteria:** clean Ubuntu configure/build of the empty application
scaffold; dependency list is concrete, and no new dependency has leaked into the
production library or install script.

### Phase 2 — EGL surface and UI-only prototype

1. Create the SDL2 window and EGL/OpenGL ES context; draw a minimal background,
   URL entry, Play/Pause/Stop controls, status, and a visible video rectangle.
2. Implement keyboard/text input and the UI event loop. Dispatch the default
   GLib context regularly from this loop so existing GStreamer bus watches can
   run once the player is connected.
3. Keep this phase independent of playback; use placeholder status and a test
   rectangle to validate resizing, input, and shutdown.

**Exit criteria:** user confirms on Ubuntu that the UI surface opens, accepts a
URL, draws controls/rectangle, resizes cleanly, and closes without hanging.

### Phase 3 — Player-interface and sink/rectangle feasibility

1. Connect the controller to the existing `InterfacePlayerRDK` public API.
2. Use a short local H.264/AAC source first; verify the default SoC path, source
   setup, current `MediaSample`/`SendHelper` signature, PTS/DTS/duration units,
   backpressure, EOS, and teardown.
3. Call `SetVideoRectangle()` and verify whether the selected default Ubuntu
   sink actually renders in the EGL UI's video region. Check
   `enableRectPropertyCfg` and sink property support; do not assume the method
   implies a native-window handle or same-surface composition.
4. Do not access private player internals. If the public API cannot render into
   the UI surface, report the exact blocker and get a decision on the documented
   separate sink-window fallback before proceeding with the visual integration.

**Exit criteria:** successful player lifecycle with local media and an explicit
Ubuntu result for same-surface video-region support. No production interface
changes.

### Phase 4 — Manifest ingest: HLS, then DASH

1. Implement URI validation and the independent adaptive-demux ingest pipeline.
2. Start with one clear HLS fixture; inspect negotiated pads/caps and feed only
   supported H.264/AAC access units through `SendHelper`.
3. Add clear DASH after the HLS sample path is proven. Preserve timestamps,
   per-track ordering, discontinuity handling, and bounded backpressure.
4. Ignore subtitle tracks and reject encrypted or unsupported caps with a
   visible explanation.

**Exit criteria:** one local clear HLS fixture and one local clear DASH fixture
play through the interface on Ubuntu without network dependency or production
code changes.

### Phase 5 — Playback controls, robustness, and fixtures

1. Wire Play/Pause/Resume/Stop, URL replacement, position, EOS, and error state
   to the controller and UI.
2. Test teardown while callbacks are pending, buffering/network failure,
   malformed URLs/manifests, and unsupported/encrypted content.
3. Prefer short generated color-bar/sine-wave assets or explicitly
   redistributable media under `test/pi-cli/fixtures/`; record provenance and
   license, and target a small total fixture budget (for example 5 MB).
4. Run paced playback for at least 30 seconds (or repeated short fixtures);
   assert the configured per-track queue bound is respected, no samples are
   silently dropped, and stop/EOS releases queued data.

**Exit criteria:** user can operate the complete UI and observe useful
playback/error status; deterministic fixture tests pass on Ubuntu.

### Phase 6 — Deferred root-CMake and dependency-install integration

Only after Phases 1–5 pass on Ubuntu:

1. Add the simulator-specific packages to `install-middleware.sh`, based on
   package names verified in the target Ubuntu environment.
2. Update the root CMake to include `test/pi-cli/` only for the Ubuntu platform
   and only when the simulator is explicitly enabled, leaving other platforms
   and default product builds unchanged.
3. Re-run a clean Ubuntu configure/build with the option both enabled and
   disabled; verify the normal library build does not acquire SDL/EGL/GUI
   dependencies.

**Exit criteria:** Ubuntu opt-in builds the simulator; default and non-Ubuntu
builds remain unaffected.

### Phase 7 — Final Ubuntu acceptance

- Verify the EGL UI surface, sink output, and video rectangle (or the approved
  fallback) on the target Ubuntu desktop/display driver.
- Play clear HLS and DASH fixtures through the existing player interface.
- Verify controls, position/status, EOS, malformed input, unsupported DRM and
  subtitles, and clean shutdown.
- Update `test/pi-cli/README.md` with exact Ubuntu dependencies, configure,
  build, run, and test commands, plus known video-surface limitations.

## Risks and mitigations

| Risk | Mitigation |
|---|---|
| MPD/HLS demux output may not match the interface's expected buffer framing or initialization semantics. | Make the Phase 4 sample/framing spike a hard gate; use local fixtures and inspect negotiated caps/buffer flags. |
| The public `SendHelper` API accepts samples but does not document the full upstream demux contract. | Validate behavior against the implementation and existing format caps; do not infer fragment semantics from the manifest URL. |
| GStreamer adaptive-demuxer availability varies by distro/version. | Check installed plugins at startup/build time; record minimum tested GStreamer version and supported demuxers. |
| `autovideosink` may select a non-EGL sink or fail in headless CI. | Separate control UI from video output, report available sink candidates (the selected sink may not be exposed through the public interface), use a display-enabled smoke test for EGL, and keep headless controller tests distinct. |
| The default sink may not render into the EGL surface's video region. | Test `SetVideoRectangle()` and the selected sink on Ubuntu; do not use private internals. If unsupported, document the separate sink-window fallback or raise an explicit API-change decision. |
| Public streams are unstable or inaccessible from CI. | Prefer small local clear fixtures for deterministic tests; make remote URL checks optional. |

## Acceptance criteria

- Production player-interface sources, public headers, and vendor implementations
  remain unchanged.
- Optional simulator builds against the existing library and defaults to the
  Ubuntu/default SoC implementation.
- An EGL URL/control interface starts and stops playback for validated
  clear DASH and HLS inputs; the single-surface video-region layout is accepted
  only if Phase 3 proves it works with the existing public API.
- Audio/video samples enter through the existing `InterfacePlayerRDK` public
  API with correct caps, timestamps, initialization ordering, and backpressure.
- Playback state, errors, EOS, and unsupported encrypted/subtitle cases are
  visible to the user.
- EGL/video-sink requirements and the verified video-region or separate-window
  behavior are documented for the target Linux desktop environment.

## References

- Repository architecture: `vendor/SocInterface.cpp`,
  `vendor/default/DefaultSocInterface.cpp`, `InterfacePlayerRDK.cpp`,
  `InterfacePlayerRDK.h`, `GstUtils.cpp`, and `test/pi-cli/`.
- GStreamer [`appsrc`](https://gstreamer.freedesktop.org/documentation/app/appsrc.html)
  and [`playbin`](https://gstreamer.freedesktop.org/documentation/playback/playbin.html)
  documentation.
- GStreamer [adaptive DASH demuxer](https://gstreamer.freedesktop.org/documentation/adaptivedemux2/dashdemux2.html)
  documentation.
- [SDL2 video API](https://wiki.libsdl.org/SDL2/CategoryVideo) and
  [Khronos EGL registry](https://registry.khronos.org/EGL/).
- GStreamer [GL image sink](https://gstreamer.freedesktop.org/documentation/opengl/glimagesink.html)
  documentation.
