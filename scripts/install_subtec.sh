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

    echo "Patching subtec-app CMakeLists.txt (use Homebrew gdbus-codegen)"
    if [[ "$OSTYPE" == "darwin"* ]] ; then
        SED_ARG="''"     # macOS sed: -i ''
    else
        SED_ARG=""       # GNU sed: -i
    fi

    # Use Homebrew GLib gdbus-codegen (standard DBus XML -> C generator) [1](https://github.com/taglib/taglib/releases)[2](https://reviews.llvm.org/D154997)
    export GDBUS_CODEGEN="$(brew --prefix glib)/bin/gdbus-codegen"
    export PATH="$(brew --prefix glib)/bin:$PATH"

    if [ ! -x "${GDBUS_CODEGEN}" ]; then
        echo "ERROR: gdbus-codegen not found at ${GDBUS_CODEGEN}"
        echo "Run: brew install glib"
        popd >/dev/null
        return 1
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

