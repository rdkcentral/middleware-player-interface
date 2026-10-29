cmake_minimum_required(VERSION 3.10)

project(pi-player-simulator LANGUAGES CXX)

find_package(PkgConfig REQUIRED)

pkg_check_modules(PLAYERGSTINTERFACE REQUIRED IMPORTED_TARGET libplayergstinterface)
pkg_check_modules(GLIB REQUIRED IMPORTED_TARGET glib-2.0)
pkg_check_modules(FFMPEG REQUIRED IMPORTED_TARGET
    libavformat
    libavcodec
    libavutil
)

add_executable(pi-player-simulator
    src/main.cpp
    src/backend/FfmpegDemuxSource.cpp
    src/backend/PlayerBackend.cpp
    src/console/ConsolePlayer.cpp
)

target_compile_features(pi-player-simulator PRIVATE cxx_std_17)
target_link_libraries(pi-player-simulator PRIVATE
    PkgConfig::PLAYERGSTINTERFACE
    PkgConfig::GLIB
    PkgConfig::FFMPEG
)

if(NOT UNIX)
    message(FATAL_ERROR "pi-player-simulator console input requires a Unix-like platform")
endif()

if(BUILD_WITH_COVERAGE)
    target_compile_options(pi-player-simulator PRIVATE --coverage)
    if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.13")
        target_link_options(pi-player-simulator PRIVATE --coverage)
    else()
        set_target_properties(pi-player-simulator PROPERTIES LINK_FLAGS "--coverage")
    endif()
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(pi-player-simulator PRIVATE -Wall -Wextra -Wpedantic)
endif()
