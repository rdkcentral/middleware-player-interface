#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
REPO_DIR="$(cd -- "${SCRIPT_DIR}/../.." && pwd)"
if ((EUID == 0)); then
    printf 'Do not run this build script with sudo; run it as your regular user.\n' >&2
    exit 1
fi

if [[ -n "${PLAYER_INTERFACE_PREFIX:-}" ]]; then
    PLAYER_INTERFACE_PREFIX="${PLAYER_INTERFACE_PREFIX}"
elif [[ -f "${REPO_DIR}/.libs/lib/pkgconfig/libplayergstinterface.pc" ]]; then
    PLAYER_INTERFACE_PREFIX="${REPO_DIR}/.libs"
else
    PLAYER_INTERFACE_PREFIX="${HOME}/.local"
fi
BUILD_DIR="${PI_CLI_BUILD_DIR:-${SCRIPT_DIR}/build}"

export PKG_CONFIG_PATH="${PLAYER_INTERFACE_PREFIX}/lib/pkgconfig${PKG_CONFIG_PATH:+:${PKG_CONFIG_PATH}}"
export LD_LIBRARY_PATH="${PLAYER_INTERFACE_PREFIX}/lib${LD_LIBRARY_PATH:+:${LD_LIBRARY_PATH}}"

system_packages=()
add_system_package() {
    local package="$1"
    local existing
    for existing in "${system_packages[@]}"; do
        if [[ "${existing}" == "${package}" ]]; then
            return
        fi
    done
    system_packages+=("${package}")
}

install_system_packages() {
    if ((${#system_packages[@]} == 0)); then
        return
    fi
    if ! command -v apt-get >/dev/null 2>&1 ||
       ! command -v sudo >/dev/null 2>&1; then
        printf 'Automatic dependency installation requires apt-get and sudo.\n' >&2
        return 1
    fi
    printf 'Installing Ubuntu packages: %s\n' "${system_packages[*]}"
    if ! sudo -v; then
        printf 'Could not obtain sudo permission to install dependencies.\n' >&2
        return 1
    fi
    sudo apt-get install -y --no-install-recommends "${system_packages[@]}"
    system_packages=()
}

command -v pkg-config >/dev/null 2>&1 || add_system_package pkg-config
command -v cmake >/dev/null 2>&1 || add_system_package cmake
command -v c++ >/dev/null 2>&1 || add_system_package build-essential
install_system_packages

if ! pkg-config --exists libplayergstinterface; then
    printf 'Missing pkg-config module: libplayergstinterface\n' >&2
    printf 'Install playergstinterface to %s before building pi-cli.\n' \
        "${PLAYER_INTERFACE_PREFIX}" >&2
    exit 1
fi

missing_modules=()
for module in glib-2.0 sdl2 SDL2_ttf egl glesv2; do
    if ! pkg-config --exists "${module}"; then
        missing_modules+=("${module}")
        case "${module}" in
            glib-2.0) add_system_package libglib2.0-dev ;;
            sdl2) add_system_package libsdl2-dev ;;
            SDL2_ttf) add_system_package libsdl2-ttf-dev ;;
            egl) add_system_package libegl1-mesa-dev ;;
            glesv2) add_system_package libgles2-mesa-dev ;;
        esac
    fi
done

font_paths=(
    /usr/share/fonts/truetype/dejavu/DejaVuSans.ttf
    /usr/share/fonts/truetype/liberation2/LiberationSans-Regular.ttf
    /usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf
)
font_found=false
for font_path in "${font_paths[@]}"; do
    if [[ -f "${font_path}" ]]; then
        font_found=true
        break
    fi
done
if [[ "${font_found}" != true ]]; then
    add_system_package fonts-dejavu-core
fi

if ((${#missing_modules[@]} > 0)) || [[ "${font_found}" != true ]]; then
    install_system_packages
fi

runtime_factories=(
    playbin
    souphttpsrc
    hlsdemux
    dashdemux
    autovideosink
    avdec_h264
    avdec_aac
)
runtime_packages=(
    gstreamer1.0-tools
    gstreamer1.0-plugins-base
    gstreamer1.0-plugins-good
    gstreamer1.0-plugins-bad
    gstreamer1.0-libav
)
missing_factories=()
if ! command -v gst-inspect-1.0 >/dev/null 2>&1; then
    missing_factories=("${runtime_factories[@]}")
else
    for factory in "${runtime_factories[@]}"; do
        if ! gst-inspect-1.0 "${factory}" >/dev/null 2>&1; then
            missing_factories+=("${factory}")
        fi
    done
fi
if ((${#missing_factories[@]} > 0)); then
    printf 'Installing GStreamer runtime packages for: %s\n' \
        "${missing_factories[*]}"
    for package in "${runtime_packages[@]}"; do
        add_system_package "${package}"
    done
    install_system_packages
fi

still_missing=()
for module in glib-2.0 sdl2 SDL2_ttf egl glesv2; do
    if ! pkg-config --exists "${module}"; then
        still_missing+=("${module}")
    fi
done
font_found=false
for font_path in "${font_paths[@]}"; do
    if [[ -f "${font_path}" ]]; then
        font_found=true
        break
    fi
done
if ((${#still_missing[@]} > 0)); then
    printf 'Dependencies still missing after package installation: %s\n' \
        "${still_missing[*]}" >&2
    exit 1
fi
if [[ "${font_found}" != true ]]; then
    printf 'Could not find a supported font after installing fonts-dejavu-core.\n' >&2
    exit 1
fi
if ! command -v gst-inspect-1.0 >/dev/null 2>&1; then
    printf 'gst-inspect-1.0 is still missing after installing GStreamer packages.\n' >&2
    exit 1
fi
still_missing_factories=()
for factory in "${runtime_factories[@]}"; do
    if ! gst-inspect-1.0 "${factory}" >/dev/null 2>&1; then
        still_missing_factories+=("${factory}")
    fi
done
if ((${#still_missing_factories[@]} > 0)); then
    printf 'GStreamer factories still unavailable: %s\n' \
        "${still_missing_factories[*]}" >&2
    exit 1
fi

cmake -S "${SCRIPT_DIR}" -B "${BUILD_DIR}" "$@"
cmake --build "${BUILD_DIR}" --parallel
