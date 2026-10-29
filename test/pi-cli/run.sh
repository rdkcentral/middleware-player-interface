#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
REPO_DIR="$(cd -- "${SCRIPT_DIR}/../.." && pwd)"
if [[ -n "${PLAYER_INTERFACE_PREFIX:-}" ]]; then
    PLAYER_INTERFACE_PREFIX="${PLAYER_INTERFACE_PREFIX}"
elif [[ -f "${REPO_DIR}/.libs/lib/pkgconfig/libplayergstinterface.pc" ]]; then
    PLAYER_INTERFACE_PREFIX="${REPO_DIR}/.libs"
else
    PLAYER_INTERFACE_PREFIX="${HOME}/.local"
fi
BUILD_DIR="${PI_CLI_BUILD_DIR:-${SCRIPT_DIR}/build}"
EXECUTABLE="${BUILD_DIR}/pi-player-simulator"

if [[ ! -x "${EXECUTABLE}" ]]; then
    printf 'Simulator executable not found: %s\nRun ./build.sh first.\n' \
        "${EXECUTABLE}" >&2
    exit 1
fi

export LD_LIBRARY_PATH="${PLAYER_INTERFACE_PREFIX}/lib${LD_LIBRARY_PATH:+:${LD_LIBRARY_PATH}}"
if [[ -n "${PI_CLI_GST_DEBUG:-}" ]]; then
    export GST_DEBUG="${PI_CLI_GST_DEBUG}"
    export GST_DEBUG_NO_COLOR=1
fi
exec "${EXECUTABLE}" "$@"
