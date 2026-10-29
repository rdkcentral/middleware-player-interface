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
immediately from the command-line URL. Video appears in the GStreamer sink's
own window; pi-cli does not create a second graphics window.
The launcher sets the player-interface runtime library path before starting
the simulator; running the binary directly does not inherit environment
variables set by `build.sh`.

If `playergstinterface` is installed to another prefix, set
`PLAYER_INTERFACE_PREFIX` before running `./build.sh` and `./run.sh`. The build
script configures the pkg-config paths, checks the required modules, and
installs missing Ubuntu build and GStreamer runtime packages through
`sudo apt-get`. Run the scripts without `sudo`.

The script does not build or install `playergstinterface`; that remains a
separate prerequisite. Extra CMake options can be passed to the script, for
example `./build.sh -DCMAKE_BUILD_TYPE=Debug`.

## Playback and controls

The backend sets `InterfacePlayerRDK` to its direct-URI playback mode and
passes the URL to `ConfigurePipeline()`. The interface's own `playbin` handles
HTTP source setup, HLS/DASH demuxing, decoding, and sink output. The simulator
does not make a parallel `uridecodebin`, `appsink`, or other media pipeline.

## Console controls

Type commands in the terminal while the GStreamer video window is playing:

```text
play
pause
seek 45
trick 2x
trick 4x
trick 6x
trick 12x
trick 32x
trick 1x
stop
status
quit
```

`seek` takes a target position in seconds. `trick 1x` returns to normal speed.
The console pumps the GLib main context while waiting for input so the
interface-owned GStreamer bus callbacks continue to run. Seek and trick
requests use `InterfacePlayerRDK::Flush()`; the stream/sink must support the
requested operation, so check GStreamer logs if playback does not respond.

Startup timing milestones are printed to stderr with elapsed monotonic time,
for example `[PI-CLI] +1234ms configure-pipeline-returned`. They include plugin
initialization, pipeline configuration, the pipeline's PLAYING transition,
the decoder's first-frame callback when available, and first advancing
playback position. GStreamer errors and warnings include their details.

To diagnose slow HLS/DASH startup, enable targeted source/demux logs and save
the combined output:

```sh
PI_CLI_GST_DEBUG='playbin:4,uridecodebin:4,dashdemux:6,souphttpsrc:6,qtdemux:4' \
  ./run.sh "https://example.com/clear-stream.mpd" 2>&1 | tee startup.log
```

`PI_CLI_GST_DEBUG` is optional; without it the normal log volume is unchanged.
Compare the elapsed time between `configure-pipeline-returned`,
`pipeline-entered-playing`, `decoded-video-first-frame-callback`, and
`playback-position-advanced`. The category logs help locate delays in manifest
and segment requests or demuxing; these diagnostics measure startup and do not
themselves speed it up.

## Dependencies checked by `build.sh`

- Installed `libplayergstinterface` pkg-config module
- `glib-2.0` development module
- GStreamer `playbin`, `souphttpsrc`, HLS/DASH demuxers, `autovideosink`,
  `xvimagesink`, and H.264/AAC decoders

`build.sh` installs missing Ubuntu packages when possible and reports any
runtime factories that remain unavailable.
