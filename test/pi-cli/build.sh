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

missing_modules=()
for module in libplayergstinterface sdl2 SDL2_ttf egl glesv2; do
    if ! pkg-config --exists "${module}"; then
        missing_modules+=("${module}")
    fi
done

if ((${#missing_modules[@]} > 0)); then
    printf 'Missing pkg-config modules: %s\n' "${missing_modules[*]}" >&2
    if [[ " ${missing_modules[*]} " == *" libplayergstinterface "* ]]; then
        printf 'Could not find libplayergstinterface in %s/lib/pkgconfig.\n' \
            "${PLAYER_INTERFACE_PREFIX}" >&2
        printf 'Install the player-interface package to that prefix, or set PLAYER_INTERFACE_PREFIX to its install prefix.\n' >&2
    fi
    if [[ " ${missing_modules[*]} " == *" sdl2 "* || \
          " ${missing_modules[*]} " == *" egl "* || \
          " ${missing_modules[*]} " == *" glesv2 "* ]]; then
        printf 'Install the missing SDL2/EGL development packages.\n' >&2
    fi
    if [[ " ${missing_modules[*]} " == *" SDL2_ttf "* ]]; then
        printf 'Install the SDL2_ttf development package (Ubuntu: libsdl2-ttf-dev).\n' >&2
    fi
    exit 1
fi

cmake -S "${SCRIPT_DIR}" -B "${BUILD_DIR}" "$@"
cmake --build "${BUILD_DIR}" --parallel
