# Player Interface Simulator

This directory is being rebuilt as an opt-in Linux test application for
`InterfacePlayerRDK`. The current target is only a build scaffold; UI, media
ingest, and playback are scheduled for later phases.

## Phase 1 dependencies

### Build-time pkg-config modules

| Module | Purpose |
|---|---|
| `gstreamer-1.0` | GStreamer core |
| `gstreamer-app-1.0` | `appsrc`/`appsink` sample bridge |
| `gstreamer-video-1.0` | Video sink and overlay-related APIs |
| `sdl2` | Window creation and UI input/events |
| `egl` | EGL surface/context |
| `glesv2` | OpenGL ES rendering |

The middleware library's existing build dependencies continue to come from the
repository's normal build setup. No packages are added to
`install-middleware.sh` in this phase.

### Runtime GStreamer factories to verify on Ubuntu

The application will need the following factories in later phases:

- `playbin`, `appsrc`, and `appsink`
- `hlsdemux`
- `dashdemux` or `dashdemux2`, depending on the installed GStreamer version
- `glimagesink` if EGL-backed video output is available
- H.264 and AAC parsers/decoders compatible with the selected clear test media

The exact Ubuntu package names and minimum tested GStreamer version are not
assumed here. Verify them in the target Ubuntu environment before adding
packages to the installer.

## Configure and build on Ubuntu

From the repository root, add these options to the normal Ubuntu configure
command:

```sh
cmake -S . -B build -DCMAKE_PLATFORM_UBUNTU=ON -DBUILD_PICLI=ON
cmake --build build --target pi-player-simulator
```

If the repository's normal Ubuntu build needs additional `-D` options, retain
them and add `-DBUILD_PICLI=ON`. The root `BUILD_PICLI` hook is used as-is;
there is no root CMake or installer change in this phase.

Useful environment checks:

```sh
pkg-config --modversion gstreamer-1.0 gstreamer-app-1.0 gstreamer-video-1.0 sdl2 egl glesv2
gst-inspect-1.0 playbin
gst-inspect-1.0 appsrc
gst-inspect-1.0 appsink
gst-inspect-1.0 hlsdemux
gst-inspect-1.0 glimagesink
```

For DASH, check the installed factory name with `gst-inspect-1.0 dashdemux` and
`gst-inspect-1.0 dashdemux2`; at least one must be available before DASH ingest
is implemented.

## Current phase status

- Phase 1: independent test CMake, scaffold target, and dependency inventory.
- Not yet implemented: EGL UI, URL ingest, DASH/HLS playback, and sink/video
  rectangle integration.
- Ubuntu configure/build and package verification must be performed in the
  target Ubuntu environment; this Windows workspace is not used to claim Linux
  or EGL validation.
