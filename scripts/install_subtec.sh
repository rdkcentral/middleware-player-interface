#!/usr/bin/env bash

function subtec_install_fn() {

    # Need PLAYER_DIR passed in so we can find patch files
    if [ -z "${1}" ] ; then
        echo "Middleware directory parameter is empty, can not find patch files."
        return 1
    fi
    # Need LOCAL_DEPS_BUILD_DIR passed in so we can patch the location of glib
    if [ -z "${2}" ] ; then
        echo "Dependency directory parameter is empty, can not patch subtec-app CMakeLists.txt"
        return 1
    fi

    echo "Cloning subtec-app..."
    do_clone_fn "https://code.rdkcentral.com/r/components/generic/subtec-app"
    git -C subtec-app checkout a95f7591fff3fb8777781dfdc76d95fc0a1c382b

    echo
    echo "Cloning websocket-ipplayer2-utils..."
    do_clone_fn https://code.rdkcentral.com/r/components/generic/websocket-ipplayer2-utils subtec-app/websocket-ipplayer2-utils
    git -C subtec-app/websocket-ipplayer2-utils checkout 2287fea4d1af0a632aed5f1b8bfba8babbdade1f


    pushd subtec-app
    echo "Patching subtec-app from ${1}"
    git apply -p1 ${1}/OSX/patches/subttxrend-app-xkbcommon.patch
    git apply -p1 ${1}/OSX/patches/subttxrend-app-packet.patch
    git apply -p1 ${1}/OSX/patches/websocket-ipplayer2-link.patch --directory websocket-ipplayer2-utils
    git apply -p1 ${1}/OSX/patches/websocket-ipplayer2-typescpp.patch --directory websocket-ipplayer2-utils
    cp ${1}/OSX/patches/RDKLogoBlack.png subttxrend-gfx/quartzcpp/assets/RDKLogo.png
    git apply -p1 ${1}/OSX/patches/subttxrend-app-ubuntu_24_04_build.patch
    git apply -p1 ${1}/OSX/patches/websocket-ipplayer2-ubuntu_24_04_build.patch --directory websocket-ipplayer2-utils

    echo "Patching subtec-app CMakeLists.txt (use gdbus-codegen)"
    if [[ "$OSTYPE" == "darwin"* ]] ; then
        SED_ARG="''"     # macOS sed: -i ''
    else
        SED_ARG=""       # GNU sed: -i
    fi

    # Setup gdbus-codegen based on OS (standard D-Bus XML -> C generator). Documentation: https://developer.gnome.org/gio/stable/gdbus-codegen.html
    if [[ "$OSTYPE" == "darwin"* ]] ; then
        # macOS: Use Homebrew GLib gdbus-codegen
        if ! command -v brew >/dev/null 2>&1; then
            echo "ERROR: Homebrew (brew) is not installed or not in PATH."
            echo "Install Homebrew and then run: brew install glib"
            popd >/dev/null
            return 1
        fi

        if ! GLIB_PREFIX="$(brew --prefix glib 2>/dev/null)"; then
            echo "ERROR: Failed to determine Homebrew prefix for glib."
            echo "Run: brew install glib"
            popd >/dev/null
            return 1
        fi

        if [ -z "${GLIB_PREFIX}" ]; then
            echo "ERROR: Homebrew reported an empty prefix for glib."
            echo "Run: brew install glib"
            popd >/dev/null
            return 1
        fi

        export GDBUS_CODEGEN="${GLIB_PREFIX}/bin/gdbus-codegen"
        export PATH="${GLIB_PREFIX}/bin:$PATH"
        if [ ! -x "${GDBUS_CODEGEN}" ]; then
            echo "ERROR: gdbus-codegen not found at ${GDBUS_CODEGEN}"
            echo "Run: brew install glib"
            popd >/dev/null
            return 1
        fi
    else
        # Linux: Install and use system gdbus-codegen
        if ! command -v gdbus-codegen >/dev/null 2>&1; then
            echo "Installing gdbus-codegen via package manager..."
            if command -v apt-get >/dev/null 2>&1; then
                sudo apt-get update && sudo apt-get install -y libglib2.0-dev
            elif command -v yum >/dev/null 2>&1; then
                sudo yum install -y glib2-devel
            elif command -v dnf >/dev/null 2>&1; then
                sudo dnf install -y glib2-devel
            elif command -v pacman >/dev/null 2>&1; then
                sudo pacman -S glib2
            else
                echo "ERROR: Could not detect package manager. Please install libglib2.0-dev or equivalent manually."
                popd >/dev/null
                return 1
            fi
        fi

        if ! command -v gdbus-codegen >/dev/null 2>&1; then
            echo "ERROR: gdbus-codegen not found after installation."
            popd >/dev/null
            return 1
        fi

        export GDBUS_CODEGEN="gdbus-codegen"
    fi

    # Escape path for safe sed replacement
    GDBUS_ESCAPED="$(printf '%s\n' "${GDBUS_CODEGEN}" | sed 's/[\/&]/\\&/g')"

    # Replace the COMMAND lines to use the absolute Homebrew path
    sed -i ${SED_ARG} "s:COMMAND gdbus-codegen --interface-prefix com.libertyglobal.rdk --generate-c-code SubtitleDbusInterface:COMMAND ${GDBUS_ESCAPED} --interface-prefix com.libertyglobal.rdk --generate-c-code SubtitleDbusInterface:g" subttxrend-dbus/CMakeLists.txt

    sed -i ${SED_ARG} "s:COMMAND gdbus-codegen --interface-prefix com.libertyglobal.rdk --generate-c-code TeletextDbusInterface:COMMAND ${GDBUS_ESCAPED} --interface-prefix com.libertyglobal.rdk --generate-c-code TeletextDbusInterface:g" subttxrend-dbus/CMakeLists.txt

    # Also handle case where it was previously patched to a local glib/build path
    sed -i ${SED_ARG} "s:COMMAND .*glib/build/gio/gdbus-2.0/codegen/gdbus-codegen:COMMAND ${GDBUS_ESCAPED}:g" subttxrend-dbus/CMakeLists.txt

    echo "subtec-app source prepared"
    popd >/dev/null || return 1
}

function subtec_install_build_fn() {

    cd "$LOCAL_DEPS_BUILD_DIR" || return 1

    # OPTION_CLEAN == true
    if [ "$1" = true ] ; then
        echo "subtec clean"
        rm -rf subtec-app
    fi

    # Install
    if [ -d "subtec-app" ]; then
        echo "subtec-app is already installed"
        INSTALL_STATUS_ARR+=("subtec-app was already installed.")
    else
        # Tell installer where DEPs are so cmake can be patched
        subtec_install_fn "${PLAYER_DIR}" "${LOCAL_DEPS_BUILD_DIR}" || return 1
    fi

    # Build
    cd subtec-app/subttxrend-app/x86_builder/ || return 1

    if [ ! -d build/install ] ; then
        PKG_CONFIG_PATH=/usr/local/opt/libffi/lib/pkgconfig:/usr/local/ssl/lib/pkgconfig:/opt/homebrew/lib/pkgconfig:$PKG_CONFIG_PATH ./build.sh fast

        if [ -f ./build/install/usr/local/bin/subttxrend-app ]; then
            echo "subtec-app has been built."
            INSTALL_STATUS_ARR+=("subtec-app has been built.")
        else
            echo "subtec-app build has failed."
            return 1
        fi
    fi
}
