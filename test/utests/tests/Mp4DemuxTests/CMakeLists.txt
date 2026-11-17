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
set(EXEC_NAME Mp4DemuxL1Tests)

# mp4demux.hpp lives under the repo root; it is header-only so no production
# sources need compiling.  The suite is hermetic - it deliberately does NOT
# link the shared `fakes` library.  Real GStreamer/GLib are used so that the
# GstBuffer/GstEvent/GstCaps/GstStructure objects produced by the parser can be
# inspected with the real API after a one-time gst_init().

include_directories(${PLAYER_ROOT})
include_directories(${GTEST_INCLUDE_DIRS})
include_directories(${GMOCK_INCLUDE_DIRS})
include_directories(${GLIB_INCLUDE_DIRS})
include_directories(${GSTREAMER_INCLUDE_DIRS})
include_directories(${GSTREAMERBASE_INCLUDE_DIRS})

set(TEST_SOURCES Mp4DemuxL1Tests.cpp)

add_executable(${EXEC_NAME}
               ${TEST_SOURCES})

set_target_properties(${EXEC_NAME} PROPERTIES FOLDER "utests")

if (CMAKE_XCODE_BUILD_SYSTEM)
  # XCode schema target
  xcode_define_schema(${EXEC_NAME})
endif()

if (COVERAGE_ENABLED)
    player_utest_add_cov(${EXEC_NAME})
endif()

target_link_libraries(${EXEC_NAME} PRIVATE ${GLIB_LINK_LIBRARIES} ${OS_LD_FLAGS}
                      ${GMOCK_LINK_LIBRARIES} ${GTEST_LINK_LIBRARIES}
                      ${GSTREAMERBASE_LINK_LIBRARIES} ${GSTREAMER_LINK_LIBRARIES} -lpthread)

player_utest_run_add(${EXEC_NAME})
