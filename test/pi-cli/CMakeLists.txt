cmake_minimum_required(VERSION 3.10)

project(pi-player-simulator LANGUAGES CXX)

if(NOT TARGET playergstinterface)
    message(FATAL_ERROR
        "pi-player-simulator must be configured from the middleware-player-interface root "
        "with -DBUILD_PICLI=ON")
endif()

find_package(PkgConfig REQUIRED)

pkg_check_modules(GSTREAMER REQUIRED IMPORTED_TARGET
    gstreamer-1.0
    gstreamer-app-1.0
    gstreamer-video-1.0
)
pkg_check_modules(SDL2 REQUIRED IMPORTED_TARGET sdl2)
pkg_check_modules(EGL REQUIRED IMPORTED_TARGET egl)
pkg_check_modules(GLESV2 REQUIRED IMPORTED_TARGET glesv2)

add_executable(pi-player-simulator
    src/main.cpp
)

target_compile_features(pi-player-simulator PRIVATE cxx_std_17)
target_link_libraries(pi-player-simulator PRIVATE
    playergstinterface
    PkgConfig::GSTREAMER
    PkgConfig::SDL2
    PkgConfig::EGL
    PkgConfig::GLESV2
)

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
