# If not stated otherwise in this file or this component's license file the
# following copyright and licenses apply:
#
# Copyright 2025 RDK Management
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

cmake_minimum_required(VERSION 3.5)

project(playertelemetry VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set default install prefix
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "/usr" CACHE PATH "Default install prefix" FORCE)
endif()

include(FindPkgConfig)
pkg_check_modules(LIBCJSON REQUIRED libcjson)
link_directories(${LIBCJSON_LIBRARY_DIRS})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../playerLogManager)
include_directories(${LIBCJSON_INCLUDE_DIRS})

set(PlayerTelemetry_SRC PlayerTelemetry2.cpp)

add_library(playertelemetry SHARED ${PlayerTelemetry_SRC})

target_compile_definitions(playertelemetry PUBLIC PLAYER_TELEMETRY_SUPPORT)

target_include_directories(playertelemetry PUBLIC
    ${LIBCJSON_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/../playerLogManager
)

target_link_libraries(playertelemetry PUBLIC
    ${LIBCJSON_LINK_LIBRARIES}
    playerlogmanager
)

set_target_properties(playertelemetry PROPERTIES
    PUBLIC_HEADER "PlayerTelemetry2.hpp"
)

# Configure pkg-config file
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/../pkgconfig/libplayertelemetry.pc.in"
    "${CMAKE_CURRENT_BINARY_DIR}/libplayertelemetry.pc"
    @ONLY
)

install(TARGETS playertelemetry
    DESTINATION lib
    PUBLIC_HEADER DESTINATION include
)

# Install pkg-config file
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/libplayertelemetry.pc"
    DESTINATION lib/pkgconfig)
