# Player Interface CLI Test Player

## Goal and constraints

Build a lightweight Linux console player under `test/pi-cli/` that exercises
Player Interface during real clear HLS and DASH playback. The simulator must
not change Player Interface production or vendor code.

- Use FFmpeg for manifest loading, HTTP(S) fetching, and demuxing.
- Keep media payloads in bounded heap-backed queues; never write manifests or
  media segments to local files.
- Pass compressed audio/video samples through Player Interface's
  `NeedData`/`EnoughData` and `SendHelper()` APIs.
- Leave decoding, A/V synchronization, and rendering to Player Interface.
- Do not create a second media pipeline or call media-pipeline APIs directly
  from pi-cli.
- Ignore DRM and subtitles initially. Start with H.264 video and AAC audio.
- Keep the CLI controls and diagnostics independent of the AAMP CLI; use its
  PI sample-injection flow only as architectural inspiration.

## Data flow

```text
HLS/DASH URL
    |
    v
FFmpeg libavformat
  manifest and segment HTTP(S) fetch
  adaptive demux to compressed audio/video packets
    |
    v
bounded heap queues <---- Player Interface NeedData / EnoughData
    |
    v
Player Interface SendHelper()
    |
    v
Player Interface decode, synchronization, and platform output
```

FFmpeg is an ingest/demux layer only; it does not decode or render. The
simulator does not write to a disk cache. H.264 Annex-B packets are converted
to four-byte length-prefixed access units with AVC codec configuration for
Player Interface's H.264 caps. AAC uses the demuxer's ADTS framing for
transport streams and raw AAC plus AudioSpecificConfig for containerized
streams.

## Implementation phases and gates

### Phase 1 — Standalone build

- Keep the CLI entry point and console parser under `test/pi-cli/`.
- Build against the installed Player Interface package and GLib.
- Add FFmpeg `libavformat`, `libavcodec`, and `libavutil` development modules.
- Add libxml2 for in-memory DASH manifest filtering.
- Do not add GStreamer factory checks or build a media pipeline in pi-cli.

**Gate:** configure and build on Ubuntu without changing the root Player
Interface build.

### Phase 2 — FFmpeg source and sample bridge

- Open HLS/DASH manifests using FFmpeg's network protocols and demuxers.
- Filter DASH manifests in memory to the lowest-bandwidth video representation
  and one non-commentary audio adaptation set before opening FFmpeg's DASH
  demuxer; that demuxer opens every advertised representation's initialization
  segment during manifest open.
- Select playback audio/video tracks before stream probing and ignore subtitle
  streams.
- Select supported H.264/AAC tracks; ignore subtitle streams.
- Keep packet data in bounded heap-backed per-track queues.
- Register Player Interface `NeedData` and `EnoughData` callbacks before
  configuring streams.
- Configure the Player Interface tracks from FFmpeg codec metadata, then feed
  samples with timestamps and discontinuity state through `SendHelper()`.
- Signal per-track end-of-stream after queued packets drain.
- Stop and interrupt FFmpeg network reads cleanly during teardown.
- Bound network reads with a timeout and log each startup/probe stage,
  including DASH manifest fetch and filtering.
- Log a bounded sample of packet timestamps, `NeedData`/`EnoughData` transitions,
  sample submission results, and playback position to diagnose stalled output.

**Gate:** logs show packet injection and Player Interface callbacks for one
clear HLS stream and one clear DASH stream. Confirm no media files are created.

### Phase 3 — Console controls

- Keep `play`, `pause`, `seek`, `trick`, `stop`, `status`, and `quit` in the
  console frontend.
- Coordinate seeks across FFmpeg demux and Player Interface flush operations.
- Preserve `NeedData`/`EnoughData` backpressure during pause, seek, and resume.
- Report FFmpeg and Player Interface failures explicitly.

**Gate:** on Ubuntu, verify pause/resume, seek, supported trick rates, EOS,
errors, and clean shutdown. Platform-specific trick playback is not assumed
until tested.

### Phase 4 — Playback validation and coverage

- Validate packet framing, timestamps, codec configuration, and A/V output on
  representative HLS MPEG-TS, HLS fMP4, and DASH fMP4 streams.
- Add support only for clear codecs/framing confirmed against Player Interface
  caps; reject unsupported codecs clearly.
- Use startup milestones to distinguish manifest probing, first packet
  arrival, Player Interface setup, first sample processing, and first frame.

**Gate:** build and playback checks on Ubuntu. Windows-only inspection cannot
confirm Linux sink behavior or end-to-end playback.
