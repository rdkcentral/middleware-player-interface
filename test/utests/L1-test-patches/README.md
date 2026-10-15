# L1 Unit-Test Coverage Patches — middleware-player-interface

**Base commit:** `3c8cee996a15be2147fd5662f36e799857a5690e`
(`develop` tip at time of authoring — "Merge pull request #274 from
rdkcentral/feature/vpaamp-1341")

Six sequential `git format-patch` files adding ~440 L1 gtest cases to
`test/utests/`. Apply in numeric order; each patch was generated against
the tree produced by the previous one.

```
cd middleware-player-interface
git checkout 3c8cee996a15be2147fd5662f36e799857a5690e   # or a later develop
git am 0001-*.patch 0002-*.patch 0003-*.patch 0004-*.patch 0005-*.patch 0006-*.patch
```

`git apply` also works; apply order matters only because patches 2, 3, 5
and 6 each add their own `add_subdirectory(...)` line to
`test/utests/tests/CMakeLists.txt` at the same location.

## Patch contents

| # | Patch | Suite | ~Tests |
|---|-------|-------|--------|
| 0001 | InterfacePlayerRDK gap-fill | `InterfacePlayerTests` (+ `InterfacePlayerGapFillTests.cpp`) | 130 |
| 0002 | gst-plugins dedicated | `GstPluginL1Tests` (new dir) | 70 |
| 0003 | mp4demux.hpp dedicated | `Mp4DemuxTests` (new dir) | 50 |
| 0004 | thin-suite fills | GstUtils, GstHandlerControl, OcdmBasicSessionAdapter, Subtitle | 59 |
| 0005 | DRM error/timeout matrix | `SecManagerThunderTests` (new dir) | 83 |
| 0006 | vendor/infra | `InfraTests` (new dir) | 50 |

### 0001 — InterfacePlayerRDK gap-fill (~130 tests)
New file `InterfacePlayerGapFillTests.cpp` compiled into the existing
`InterfacePlayerTest` binary. Covers previously uncalled regions of
`InterfacePlayerRDK.cpp`: `GetDurationMilliseconds` (cached
`pipelineState` + `gst_element_query`, nanosecond values),
`GetVideoPosition` (live `gst_element_get_state`), `NotifyFirstFrame`,
`NotifyEOS`, `SetVolumeOrMuteUnMute` dispatch (mute→int, volume→double
through the existing fake), `SetupStream`/`SetupClosedCaptionControlStream`
5-arg call contract, and playback-state/error paths.

### 0002 — gst-plugins dedicated (~70 tests)
New `GstPluginL1Tests` executable driving `gstsubtecmp4transform` and
`gstvipertransform` through `GstBaseTransform` vfuncs:
- `transform_caps` TTML↔MP4 mapping, `viper_ttml_format` preservation
- `transform_ip` `ftyp`/`mdat` scan, payload copy/resize/drop flow values
- `convertTime`, `findTag`, non-TTML passthrough, TTML begin-time offsets
- multi-XML-document ("harmonic UHD") buffer splitting with PTS/duration/offset
- flush-event state reset

**Known source defect pinned, not fixed:** `findBoxOffsetAndLength`
(`gst-plugins/gst_subtec_mp4_transform.cpp`) sets `boxLength = boxSize`
including the 8-byte box header while `boxOffset` points at the payload,
so `transform_ip` over-reads 8 bytes past the `mdat` payload. The tests
append a trailing `free` box so the over-read stays in-bounds and
deterministic; they assert current behaviour. Fix the source and the
assertions change.

### 0003 — mp4demux.hpp dedicated (~50 tests)
New `Mp4DemuxTests` executable. The shared `fakes` library is
**deliberately not linked** — `FakeGStreamer::gst_buffer_map` returns
`FALSE` without populating `GstMapInfo`, which would make the demuxer
copy from uninitialised memory. The suite links real GStreamer/GLib
(initialised once) so parser-emitted `GstBuffer`/`GstEvent`/`GstCaps`/
`GstStructure` objects are inspected for real; a local
`gst_app_src_set_caps` stub captures output caps. Synthetic MP4 input
always wraps sample bytes in a valid `mdat` box — trailing raw zero
bytes parse as a zero-size box and would loop the box walk.

### 0004 — thin-suite fills (~59 tests)
- `GstUtilsFillTests.cpp` (21): `GetCaps` format branch matrix —
  MPEG-TS, ISO BMFF, MP3, AAC/raw-AAC, AC3/EC3/Atmos, AC4, H.264,
  HEVC, MPEG-2, TTML, WebVTT, MP4, unknown, invalid.
- `GstHandlerControlFillTests.cpp` (20): enable/disable, `ScopeHelper`
  lifecycle and move semantics, handler completion, immediate and
  negative timeouts, concurrent waiters.
- `OcdmBasicDecryptFillTests.cpp` (8): error-code passthrough,
  `terminateEarly` on decrypt failure, HDCP gate skipping the memory
  system, zero-size payload, IV propagation, serialised repeat calls.
- `VttCueFillTests.cpp` (10): `VTTCue` field assignment across
  boundary inputs.

**Not extended, by design:**
- `DrmSecureClient` — the suite is compiled only when
  `SEC_CLIENT_HEADERS` are present; they are absent on this target, so
  the entire test binary is `#if`-ed out. No always-on coverage is
  possible without vendor headers.
- `PluginsTests` — the four decryptor plugins exist only as
  `GTEST_SKIP` stubs; the plugin `.cpp` files are `static`-heavy and
  are not compiled into that binary. Patch 0002 covers the plugin code
  that is actually compilable.

### 0005 — SecManagerThunder DRM error/timeout matrix (~83 tests)
New `SecManagerThunderTests` executable with local stubs for the
Thunder/WebSocket/core headers (unavailable on host builds) and
`FakeThunderAccessPlayer` capturing JSON-RPC calls and event handlers.

Retry-gate matrix (`MAX_LICENSE_REQUEST_ATTEMPTS == 2`):
- **Not retried (1 call):** 200:1, 200:2, 200:8, 200:9; wrong-class
  100:3, 100:4, 400:x; missing resultContext (defaults 200:1).
- **Retried (2 calls):** 200:3 (timeout), 200:4 (connection failure),
  200:5 (busy); 300:3, 300:4, 300:5 (watermark class, same reasons).
- This is the **200:1 vs 200:4** distinction: same status class, and
  the reason code alone decides retryability.
- Retry-then-success, retry exhaustion (queued responses unconsumed),
  per-`Acquire` budget reset, businessStatus propagation.

Also: acquire-parameter validation and field values, base64 license
decode (real `base64_Decode`, not the `NULL`-returning fake), session
open vs update by hash, `setDrmSessionState`/`closePlaybackSession`/
`setVideoWindowSize`/`setPlaybackSpeedState` params and failure
tolerance, all watermark RPCs, `RegisterAllEvents`/`UnRegisterAllEvents`,
scheduler-queued handlers, and the `ContentSecurityManager` base-class
wrappers (`UpdateSessionState`, `ReleaseSession`,
`setVideoWindowSize`, `setPlaybackSpeedState`, `DestroyInstance`).

### 0006 — vendor/infra (~50 tests)
New `InfraTests` executable:
- `SocInterface`: `StartsWith` prefix matching (protected → exposed via
  test subclass; it dereferences arguments unconditionally so no null
  cases), `ReadVideoPts`, `CheckVideoPtsPropertySupport`.
- `ProcessHandler`.
- `PlayerScheduler`: `PLAYER_TASK_ID_INVALID` when not running,
  lockout skip, ID increment and wrap at
  `PLAYER_SCHEDULER_ID_MAX_VALUE`, suspend/resume.
- `InfraTests/stubs/gst_svp_meta.h` — local stub for the platform SVP
  header unavailable on the host (vendor SVP methods are also
  `USE_SVP`-conditional).

## Verification status

Authored on a Windows host without the RDK build toolchain —
**these tests have not been compiled or run here.** First build on an
RDK-capable host may surface minor compile fixes (mock arity,
include paths); the suites were written against the actual headers,
mock signatures, and existing-suite conventions of the base commit.

Known harness constraints respected:
- `FakeBase64::base64_Encode` returns `NULL` — suites needing real
  base64 (0005) link real code instead.
- `FakePlayerScheduler` is a no-op — 0006 links the real scheduler.
- `FakeGStreamer::gst_buffer_map` returns `FALSE` — 0003 links real
  GStreamer rather than the fakes library.
