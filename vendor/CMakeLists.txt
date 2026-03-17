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
project(playersocinterface VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)

# Set default install prefix
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "/usr" CACHE PATH "Default install prefix" FORCE)
endif()

find_package(PkgConfig REQUIRED)
pkg_check_modules(GST REQUIRED gstreamer-plugins-base-1.0)
pkg_check_modules(GSTREAMER REQUIRED gstreamer-1.0)
pkg_check_modules(GSTREAMERBASE REQUIRED gstreamer-app-1.0)

include_directories(${GST_INCLUDE_DIRS} ${GSTREAMER_INCLUDE_DIRS} ${GSTREAMERBASE_INCLUDE_DIRS} ${GSTREAMERVIDEO_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/default)

set(VENDOR_INTERFACE_SOURCES
    SocInterface.cpp
    default/DefaultSocInterface.cpp)

set(VENDOR_INTERFACE_DEPENDS
    ${GST_LINK_LIBRARIES}
    ${GSTREAMER_LINK_LIBRARIES}
    ${GSTREAMERBASE_LINK_LIBRARIES})

add_library(${PROJECT_NAME} SHARED ${VENDOR_INTERFACE_SOURCES})

# Set public headers for the library
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER 
  "SocInterface.h;SocInterfaceImpl.h;default/DefaultSocInterface.h")

target_link_libraries(${PROJECT_NAME} PRIVATE ${VENDOR_INTERFACE_DEPENDS})

if(PLAYERSOCINTERFACEIMPL_FOUND)
    message(STATUS "playersocinterfaceimpl found")
    set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_DEFINITIONS "-DPLAYER_SOC_INTERFACE_IMPL")
    target_include_directories(${PROJECT_NAME} PRIVATE ${PLAYERSOCINTERFACEIMPL_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${PLAYERSOCINTERFACEIMPL_LINK_LIBRARIES})
endif()


# Install the library and its headers
install(TARGETS ${PROJECT_NAME}
    DESTINATION lib
    PUBLIC_HEADER DESTINATION include)