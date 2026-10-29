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
for module in glib-2.0 gio-2.0 libavformat libavcodec libavutil libxml-2.0; do
    if ! pkg-config --exists "${module}"; then
        missing_modules+=("${module}")
        case "${module}" in
            glib-2.0|gio-2.0) add_system_package libglib2.0-dev ;;
            libavformat) add_system_package libavformat-dev ;;
            libavcodec) add_system_package libavcodec-dev ;;
            libavutil) add_system_package libavutil-dev ;;
            libxml-2.0) add_system_package libxml2-dev ;;
        esac
    fi
done

if ((${#missing_modules[@]} > 0)); then
    install_system_packages
fi

still_missing=()
for module in glib-2.0 gio-2.0 libavformat libavcodec libavutil libxml-2.0; do
    if ! pkg-config --exists "${module}"; then
        still_missing+=("${module}")
    fi
done
if ((${#still_missing[@]} > 0)); then
    printf 'Dependencies still missing after package installation: %s\n' \
        "${still_missing[*]}" >&2
    exit 1
fi

cmake -S "${SCRIPT_DIR}" -B "${BUILD_DIR}" "$@"
cmake --build "${BUILD_DIR}" --parallel
