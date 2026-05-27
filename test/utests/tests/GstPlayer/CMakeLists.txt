include(GoogleTest)
find_package(PkgConfig REQUIRED)

set(PLAYER_ROOT "../../../../")
set(UTESTS_ROOT "../../")
set(EXEC_NAME PlayerGstPlayer)

# --- GStreamer (common for Linux + macOS) ---
pkg_check_modules(GSTREAMER REQUIRED gstreamer-1.0)
pkg_check_modules(GSTREAMERAPP REQUIRED gstreamer-app-1.0)
pkg_check_modules(GSTREAMERBASE REQUIRED gstreamer-base-1.0)

# --- Includes ---
include_directories(${PLAYER_ROOT})
include_directories(${PLAYER_ROOT}/playerLogManager)
include_directories(${PLAYER_ROOT}/subtec/subtecparser)
include_directories(${PLAYER_ROOT}/subtec/libsubtec)
include_directories(${PLAYER_ROOT}/vendor)
include_directories(${UTESTS_ROOT}/fakes)
include_directories(${GTEST_INCLUDE_DIRS})
include_directories(${GMOCK_INCLUDE_DIRS})
include_directories(${GLIB_INCLUDE_DIRS})
include_directories(${GSTREAMER_INCLUDE_DIRS})
include_directories(${GSTREAMERAPP_INCLUDE_DIRS})
include_directories(${GSTREAMERBASE_INCLUDE_DIRS})
include_directories(${LIBCJSON_INCLUDE_DIRS})
include_directories(SYSTEM ${UTESTS_ROOT}/mocks)
include_directories(${UTESTS_ROOT}/mocks)
include_directories(${PLAYER_ROOT}/externals)
include_directories(${PLAYER_ROOT}/../test/gstTestHarness)
include_directories(${PLAYER_ROOT}/mp4demux)

message(STATUS "GSTREAMER_INCLUDE_DIRS=${GSTREAMER_INCLUDE_DIRS}")

# --- Sources ---
set(TEST_SOURCES
    PauseOnPlaybackTests.cpp
    FunctionalTests.cpp
    GstPlayerTests.cpp
)

set(PLAYER_SOURCES
    ${PLAYER_ROOT}/InterfacePlayerRDK.cpp
    ${PLAYER_ROOT}/GstUtils.cpp
    ${PLAYER_ROOT}/playerLogManager/PlayerLogManager.cpp
    ${PLAYER_ROOT}/externals/PlayerExternalsInterface.cpp
    ${PLAYER_ROOT}/externals/PlayerExternalUtils.cpp
)

set(FAKE_SOURCES
    ${UTESTS_ROOT}/fakes/FakeGStreamer.cpp
    ${UTESTS_ROOT}/fakes/FakeGLib.cpp
)

# --- Target ---
add_executable(${EXEC_NAME}
    ${TEST_SOURCES}
    ${PLAYER_SOURCES}
    ${FAKE_SOURCES}
)

set_target_properties(${EXEC_NAME} PROPERTIES FOLDER "utests")

if (CMAKE_XCODE_BUILD_SYSTEM)
  xcode_define_schema(${EXEC_NAME})
endif()

if (COVERAGE_ENABLED)
  player_utest_add_cov(${EXEC_NAME})
endif()

# --- Link dirs (important for macOS) ---
link_directories(
  ${GSTREAMER_LIBRARY_DIRS}
  ${GSTREAMERAPP_LIBRARY_DIRS}
  ${GSTREAMERBASE_LIBRARY_DIRS}
)

# --- Link libraries (common for Linux + macOS) ---
target_link_libraries(${EXEC_NAME} PRIVATE
  fakes
  pthread
  ${GLIB_LINK_LIBRARIES}
  ${OS_LD_FLAGS}
  ${GMOCK_LINK_LIBRARIES}
  ${GTEST_LINK_LIBRARIES}
  ${GSTREAMERAPP_LINK_LIBRARIES}
  ${GSTREAMERBASE_LINK_LIBRARIES}
  ${GSTREAMER_LINK_LIBRARIES}
)

# --- RPATH (fixes macOS runtime issues) ---
set_target_properties(${EXEC_NAME} PROPERTIES
  BUILD_RPATH "${GSTREAMER_LIBRARY_DIRS}"
)

player_utest_run_add(${EXEC_NAME})
