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

include(GoogleTest)

set(PLAYER_ROOT "../../../../")
set(UTESTS_ROOT "../../")
set(EXEC_NAME SecManagerThunderTests)

# SecManagerThunder.cpp needs WPEFramework headers for ThunderAccessPlayer.
# The stubs/ directory provides minimal stand-ins for <core/core.h>,
# <websocket/websocket.h> and Module.h (JsonObject, JSONRPC::LinkType, _T),
# and FakeThunderAccessPlayer.cpp implements the real ThunderAccessPlayer
# class with fully scripted behaviour.  USE_SECMANAGER selects the
# SecManagerThunder backend inside ContentSecurityManager::GetInstance().

add_definitions(-DUSE_SECMANAGER)

# Stub WPEFramework headers must win over any system-installed copies.
include_directories(BEFORE ${CMAKE_CURRENT_SOURCE_DIR}/stubs)
include_directories(${PLAYER_ROOT})
include_directories(${PLAYER_ROOT}/externals)
include_directories(${PLAYER_ROOT}/externals/contentsecuritymanager)
include_directories(${PLAYER_ROOT}/externals/contentsecuritymanager/IFirebolt)
include_directories(${PLAYER_ROOT}/playerLogManager)
include_directories(${PLAYER_ROOT}/playerJsonObject)
include_directories(${PLAYER_ROOT}/baseConversion)
include_directories(${PLAYER_ROOT}/vendor)
include_directories(${PLAYER_ROOT}/drm)
include_directories(${GTEST_INCLUDE_DIRS})
include_directories(${GMOCK_INCLUDE_DIRS})
include_directories(${GLIB_INCLUDE_DIRS})
include_directories(${GSTREAMER_INCLUDE_DIRS})
include_directories(${LIBCJSON_INCLUDE_DIRS})
include_directories(SYSTEM ${UTESTS_ROOT}/mocks)

set(TEST_SOURCES SecManagerThunderTests.cpp
	FakeThunderAccessPlayer.cpp
	TestMain.cpp)

set(PLAYER_SOURCES
	${PLAYER_ROOT}/externals/contentsecuritymanager/SecManagerThunder.cpp
	${PLAYER_ROOT}/externals/contentsecuritymanager/ContentSecurityManager.cpp
	${PLAYER_ROOT}/externals/contentsecuritymanager/ContentSecurityManagerSession.cpp
	${PLAYER_ROOT}/externals/contentsecuritymanager/PlayerMemoryUtils.cpp
	${PLAYER_ROOT}/externals/PlayerExternalUtils.cpp
	${PLAYER_ROOT}/playerLogManager/PlayerLogManager.cpp
	${PLAYER_ROOT}/baseConversion/_base64.cpp)

add_executable(${EXEC_NAME}
               ${TEST_SOURCES}
               ${PLAYER_SOURCES})

set_target_properties(${EXEC_NAME} PROPERTIES FOLDER "utests")

if (CMAKE_XCODE_BUILD_SYSTEM)
  # XCode schema target
  xcode_define_schema(${EXEC_NAME})
endif()

if (COVERAGE_ENABLED)
    player_utest_add_cov(${EXEC_NAME})
endif()

# `fakes` supplies FakePlayerScheduler, FakeContentProtectionFireBolt and the
# other support fakes; real PlayerScheduler.cpp is deliberately NOT compiled
# so there is no duplicate-symbol conflict.
target_link_libraries(${EXEC_NAME} fakes -lpthread ${GLIB_LINK_LIBRARIES} ${OS_LD_FLAGS}
                      ${GMOCK_LINK_LIBRARIES} ${GTEST_LINK_LIBRARIES})

player_utest_run_add(${EXEC_NAME})
