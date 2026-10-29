#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
PLAYER_INTERFACE_PREFIX="${PLAYER_INTERFACE_PREFIX:-${HOME}/.local}"
BUILD_DIR="${PI_CLI_BUILD_DIR:-${SCRIPT_DIR}/build}"

export PKG_CONFIG_PATH="${PLAYER_INTERFACE_PREFIX}/lib/pkgconfig${PKG_CONFIG_PATH:+:${PKG_CONFIG_PATH}}"
export LD_LIBRARY_PATH="${PLAYER_INTERFACE_PREFIX}/lib${LD_LIBRARY_PATH:+:${LD_LIBRARY_PATH}}"

missing_modules=()
for module in libplayergstinterface sdl2 egl glesv2; do
    if ! pkg-config --exists "${module}"; then
        missing_modules+=("${module}")
    fi
done

if ((${#missing_modules[@]} > 0)); then
    printf 'Missing pkg-config modules: %s\n' "${missing_modules[*]}" >&2
    printf 'Install playergstinterface to %s and install the SDL2/EGL development packages.\n' \
        "${PLAYER_INTERFACE_PREFIX}" >&2
    exit 1
fi

cmake -S "${SCRIPT_DIR}" -B "${BUILD_DIR}" "$@"
cmake --build "${BUILD_DIR}" --parallel
