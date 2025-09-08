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

project(SocInterface)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

install(FILES SocInterface.h
PlayerSocInterfaceImpl.h
DESTINATION include)

set(VENDOR_SRC SocInterface.cpp)

add_library(SocInterface SHARED ${VENDOR_SRC})

#if simulator
if(CMAKE_SYSTEM_NAME STREQUAL Linux OR CMAKE_SYSTEM_NAME STREQUAL Darwin)
    add_subdirectory(default)
    set(SOCINTERFACE_DEPENDS ${SOCINTERFACE_DEPENDS} PlayerSocInterfaceImpl)
else()
    pkg_check_modules(SOC_IMPL REQUIRED libplayersocinterfaceimpl)
    set(SOCINTERFACE_DEPENDS ${SOCINTERFACE_DEPENDS} ${SOC_IMPL_LINK_LIBRARIES})
endif()


set_target_properties(SocInterface PROPERTIES PUBLIC_HEADER "SocInterface.h")

target_link_libraries(SocInterface ${SOCINTERFACE_DEPENDS})

# Install the library and its headers
install(TARGETS SocInterface
    DESTINATION lib
    PUBLIC_HEADER DESTINATION include)