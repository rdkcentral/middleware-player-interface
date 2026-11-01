# Player Interface CLI Simulator

`pi-player-simulator` is a console-driven harness for exercising
`InterfacePlayerRDK` with clear HLS and DASH streams. FFmpeg fetches and
demuxes compressed media; Player Interface remains responsible for decoding,
audio/video synchronization, and output.

The simulator does not create a second GStreamer pipeline, decode media, or
write manifests, segments, or samples to disk. Its output uses the sink
configured by Player Interface; pi-cli does not create a separate GUI or EGL
window.

## Design and source layout

```text
test/pi-cli/
  CMakeLists.txt              Build definition and native dependencies
  build.sh                    Dependency checks and CMake build
  run.sh                      Runtime library path setup and launch
  src/
    main.cpp                  URL validation and application entry point
    console/
      ConsolePlayer.h/.cpp    Console command loop and command parsing
    backend/
      PlayerBackend.h/.cpp    Player Interface setup, playback state,
                              track switching, seek, trick play, and ABR
      StreamDemuxSource.h/.cpp FFmpeg input, demux, bounded packet queues,
                               timestamps, and sample delivery
      SegmentResourceProxy.h/.cpp In-memory DASH segment prefetch/cache proxy
      MediaTrackCatalog.h/.cpp DASH/FFmpeg track discovery and selection
      AbrController.h/.cpp    Bandwidth-based video representation decisions
```

`PlayerBackend` configures `InterfacePlayerRDK` and connects its
`NeedData`/`EnoughData` callbacks to the active FFmpeg source. Per-track FFmpeg
queues are bounded to 8 MiB and 128 packets. Player Interface appsrc queues
are configured to 15 MiB for video and 8 KiB for audio.

## Playback flow

1. `main.cpp` accepts one HTTP(S) stream URL and starts `ConsolePlayer`.
2. `StreamDemuxSource` opens the HLS or DASH input and prepares compressed
   H.264 video and AAC audio packets. For DASH, the track catalog reads MPD
   metadata and the source filters the manifest in memory before FFmpeg opens
   it. The default selection is one regular video adaptation and one
   non-commentary audio adaptation.
3. For supported DASH `SegmentTemplate` layouts using `$Number$` or `$Time$`,
   `SegmentResourceProxy` prefetches segments over loopback HTTP and keeps its
   cache in memory (maximum 32 MiB). Unsupported layouts fall back to FFmpeg's
   direct HTTP fetching. HLS is fetched by FFmpeg.
4. Player Interface requests data through `NeedData`; the source submits
   compressed samples and timestamps using `SendHelper()`. `EnoughData`
   applies backpressure. FFmpeg Annex-B H.264 packets are converted to
   length-prefixed AVC access units; AAC keeps the framing and codec
   configuration appropriate to its input.
5. Console commands call the same backend methods used for playback controls.
   Seeking coordinates the FFmpeg demux position with a Player Interface
   flush; track and quality changes prepare replacement sources and hand them
   off without creating another playback pipeline.

`AbrController` evaluates completed DASH segment transfer rates and uses a
70% bandwidth safety margin with two consecutive decisions before switching.
Trick play uses a DASH video adaptation marked with a trick-mode descriptor;
the simulator does not synthesize trick frames from the normal video track.

## FFmpeg responsibilities

FFmpeg is the input, network, and demux layer. pi-cli uses `libavformat` to:

- Open HTTP(S) HLS/DASH inputs and read manifests and media segments. HLS uses
  FFmpeg's HLS demuxer. For supported DASH manifests, pi-cli first filters the
  MPD in memory and can route selected segment requests through
  `SegmentResourceProxy`.
- Probe streams and read codec parameters, time bases, duration, and track
  metadata with `avformat_find_stream_info`.
- Demux compressed packets with `av_read_frame`, preserving packet timestamps,
  duration, and key-frame flags for the Player Interface sample bridge.
- Seek with `avformat_seek_file` and reset FFmpeg's demux state with
  `avformat_flush` when the user requests a seek or a source switch.

The DASH filtering and track catalog are simulator code using libxml2, not
FFmpeg APIs. The optional loopback proxy is also simulator code: for supported
`SegmentTemplate` manifests, it prefetches selected initialization/media
segments and serves them from a bounded in-memory cache. Other layouts fall
back to FFmpeg's own HTTP fetching.

FFmpeg does **not** decode or render the media. pi-cli does not use FFmpeg
decoder APIs, filters, or an FFmpeg audio/video output. It passes compressed
packets onward. The simulator normalizes H.264 Annex-B NAL units into
length-prefixed AVC access units and supplies codec configuration; AAC
framing/configuration is passed through in the form required by the input.
Player Interface performs the decode, synchronization, and output.

## Player Interface API coverage

The simulator exercises the APIs needed for a clear-stream integration path;
it is not a complete Player Interface API conformance suite.

| API area | APIs and behavior exercised by pi-cli |
|---|---|
| Initialization and pipeline setup | `InitializePlayerGstreamerPlugins`, `SetPlayerName`, the `ConfigurePipeline(StreamCodecInfo&&, ...)` overload, and `SetStreamCaps`. It also sets `m_gstConfigParam` fields for media type, sink selection, monitoring, and appsrc queue sizes. |
| Data-flow callbacks | `RegisterNeedDataCb` and `RegisterEnoughDataCb` route Player Interface backpressure to the active FFmpeg source. |
| Sample and EOS delivery | `SendHelper` submits compressed samples, timestamps, and discontinuity/segment flags. `EndOfStreamReached` signals per-track EOS after queued packets drain. |
| Playback and seek state | `Pause` implements play/pause and state restoration. `Flush` is used for seeks and pipeline transitions; `ResetEOSSignalledFlag` clears EOS state after a successful seek. |
| Stream changes | `SetStreamCaps`, `TearDownStream`, and `SetSeekPosition` support stream reconfiguration. `FlushTrack` is used for audio-switch recovery, not as the normal audio handoff path. |
| Shutdown | `Stop` stops the Player Interface pipeline. |
| Events and first-frame callbacks | `RegisterBusEvent` handles bus errors, warnings, EOS, and state changes. `FirstFrameCallback` and the first-video-frame-displayed callback provide startup diagnostics. Several unrelated callbacks are registered as no-ops; subtitle setup is disabled. |
| Position and diagnostics | `GetPositionMilliseconds`, `GetDurationMilliseconds`, `GetVideoPTS`, `GetVideoPosition`, `GetVideoPlaybackQuality`, and `GetMonitorAVState` report playback progress and A/V state. |
| Trick-play audio behavior | `SetAudioVolume` and `SetVolumeOrMuteUnMute` mute and restore audio during video-only trick play. |

For trick play, pi-cli selects a DASH trick-mode adaptation and adjusts the
injected packet timestamps while keeping the Player Interface pipeline rate
at normal speed. This deliberately tests the stream-selection approach:
`SetPlayBackRate` delegates rate control to the SoC-specific implementation
for the currently configured pipeline and sources; it does not select the
DASH trick-mode adaptation or make FFmpeg demux that track. Combining that
pipeline-rate change with pi-cli's timestamp scaling would mix two rate
control strategies and could apply the speed change twice. Therefore
`SetPlayBackRate` is not covered as pi-cli's trick-play mechanism; it should
be validated separately when testing Player Interface's native rate-control
path.

Diagnostics also call `GetPrivatePlayer` and inspect private GStreamer
pipeline/element state. This is implementation-coupled instrumentation, not
coverage of a stable public control API.

### Coverage boundaries

Coverage is **partial and functional**, not a percentage of the Player
Interface API. There is no API-coverage instrumentation or automated
pi-cli test suite: the documented HLS/DASH playback checks are manual. A
successful run exercises the listed integration paths only; it does not
validate every method, callback, failure path, sink, or platform backend.

The harness does not exercise DRM/protection, subtitles/closed captions,
text styling, live-latency controls, device/vendor-specific APIs, or the
unregistered buffering/decode/PTS error callbacks. It observes Player
Interface bus failures but does not inject every failure condition.

## Ubuntu build and run

Build and install `playergstinterface` first. From the
`middleware-player-interface` repository root, use the repository's usual
`.libs` prefix:

```sh
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
trick 2
list audio
list video
setaudio 2
setres 720
abr on
trick 4
trick 6
trick 12
trick 32
trick 1
stop
status
quit
```

`seek` takes a target in seconds. The command repositions FFmpeg and flushes
Player Interface. Trick-rate commands select the DASH `AdaptationSet` marked
with the MPD trick-mode descriptor, configure a video-only pipeline, and
compress the trick track's timestamps to the requested rate. The MPD must
provide a finite, static trick-mode video adaptation; otherwise the command
reports that trick play is unavailable. The trick track is opened and
prefetched in the background; if it is not ready when requested, normal
playback continues until preparation finishes. Audio-track, video-quality,
and ABR changes are suspended during trick play. `play` exits trick mode and
resumes normal audio/video at the current media position; `trick 1` performs
the same transition while preserving the current paused/playing state.
While trick play is active, pi-cli logs the GStreamer and per-element states,
appsrc queue levels, decoder queue depths, and first-buffer/frame indicators
once per second. It also logs each selected video packet's H.264 NAL types,
IDR presence, and input/output timestamps.
`list audio` and `list video` report one-based track numbers and metadata
discovered from the manifest or FFmpeg input context. For clear static DASH
streams, `pi-cli` prepares alternate audio-only sources in the background and
keeps one alternate source's four-segment lookahead near the current playback
position. Current audio continues during preparation; the audio branch is
flushed only at handoff, and stale replacement packets are discarded so the
video timeline is not reset. This prewarming adds audio-segment network traffic
for alternate tracks. A track that is not ready yet falls back to on-demand
preparation, so network or server delays can still exceed two seconds.
`setres` takes a height such as `360`, `720`, or `1080` and selects the
highest-bitrate representation at that height. The legacy `setresolution`
command remains available for exact sizes (`1280x720`) or heights (`720p`).
`abr on` enables bandwidth-based video selection; `abr off` returns to the
currently selected representation. A manual resolution selection disables
ABR. Video selection rebuilds the FFmpeg demux source and reconfigures the
Player Interface pipeline at the current position. Automatic adaptation uses
completed DASH media-segment transfer rates, a 70% safety margin, and two
consecutive decisions before switching. Manual audio/video selection and ABR
currently require DASH.

## Known limitations

- The console frontend is Unix-only. The scripts target Ubuntu; this is not a
  Windows player.
- Only clear HTTP(S) HLS and DASH playback is in scope. DRM/encrypted content
  and subtitles are unsupported.
- Supported elementary streams are H.264 video and AAC audio.
- Manual audio selection, video quality selection, and ABR require DASH.
- Trick play requires a finite, static DASH trick-mode video adaptation and
  supports rates `2`, `4`, `6`, `12`, and `32`.
- The in-memory DASH proxy only handles the supported `SegmentTemplate`
  patterns described above. Other layouts use FFmpeg's direct fetching and
  may have different startup or seek performance.
- The Player Interface output sink must be usable in the target environment.
  pi-cli itself does not provide a desktop UI or select a graphics backend.
- Track preparation, network performance, and available representations vary
  by stream/server; switches can take longer on uncached or slow streams.

## Startup diagnostics

Timing milestones use monotonic elapsed time and are printed to stderr, for
example `[PI-CLI] +1234ms ffmpeg-manifest-and-stream-info-ready`. They cover
DASH manifest fetch/filter, FFmpeg open, stream selection/probing, initial packet arrival, Player
Interface setup, first sample processing, first decoded frame, and advancing
playback position. The first five packet timestamps, feed-control transitions,
sample submission results, periodic playback position, and rendered/dropped
frame counts are also logged to diagnose stalls. Network reads time out instead
of waiting indefinitely.
Player Interface bus errors and FFmpeg ingest failures are reported with their
details.

## Validation

Run `./build.sh`, then try one clear HLS and one clear DASH stream on Ubuntu.
Verify audio/video output, pause/resume, seek, EOS/error reporting, and clean
shutdown. This Windows workspace cannot establish Linux hardware output or
runtime behavior.
