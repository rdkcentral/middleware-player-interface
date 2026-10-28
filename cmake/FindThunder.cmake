# If not stated otherwise in this file or this component's license file the
# following copyright and licenses apply:
#
# Copyright 2020 RDK Management
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

# - Try to find Thunder
# Once done this will define
#  THUNDER_FOUND - System has Thunder
#  THUNDER_INCLUDE_DIRS - The Thunder include directories
#  THUNDER_LIBRARIES - The libraries needed to use Thunder
#
# Be extremely careful! THUNDER_PLUGINS_INCLUDE_DIRS and THUNDER_LIBRARIES is already defined in
# Thunder/Source/plugins!!
# So here we purposely left one underscore away

find_package(PkgConfig)
pkg_check_modules(PC_THUNDER ThunderCore)

if(PC_THUNDER_FOUND)
    if(THUNDER_FIND_VERSION AND PC_THUNDER_VERSION)
        if ("${THUNDER_FIND_VERSION}" VERSION_GREATER "${PC_THUNDER_VERSION}")
            message(WARNING "Incorrect version, found ${PC_THUNDER_VERSION}, need at least ${THUNDER_FIND_VERSION}, please install correct version ${THUNDER_FIND_VERSION}")
            set(THUNDER_FOUND_TEXT "Found incorrect version")
            unset(PC_THUNDER_FOUND)
        endif()
    endif()
else()
    set(THUNDER_FOUND_TEXT "Not found")
endif()

if(PC_THUNDER_FOUND)
    find_path(
        THUNDER_INCLUDE_DIRS
        NAMES plugins/plugins.h
        HINTS ${PC_THUNDER_INCLUDEDIR} ${PC_THUNDER_INCLUDE_DIRS})

if (USE_THUNDER_R4)
       set(THUNDER_LIBS ThunderPlugins ThunderCore ThunderCOM ThunderWebSocket)
else()
    set(THUNDER_LIBS ThunderPlugins ThunderCore ThunderTracing ThunderProtocols)
endif()

# Add ThunderSecurityUtil if DISABLE_SECURITY_TOKEN is not enabled
if (NOT DISABLE_SECURITY_TOKEN)
    list(APPEND THUNDER_LIBS ThunderSecurityUtil)
endif()

    set(THUNDER_LIBRARY )
    foreach(LIB ${THUNDER_LIBS})
        find_library(THUNDER_LIBRARY_${LIB} NAMES ${LIB}
            HINTS ${PC_THUNDER_LIBDIR} ${PC_THUNDER_LIBRARY_DIRS})
        list(APPEND THUNDER_LIBRARY ${THUNDER_LIBRARY_${LIB}})
    endforeach()

    if("${THUNDER_INCLUDE_DIRS}" STREQUAL "" OR "${THUNDER_LIBRARY}" STREQUAL "")
        set(THUNDER_FOUND_TEXT "Not found")
    else()
        set(THUNDER_FOUND_TEXT "Found")
    endif()
else()
    set(THUNDER_FOUND_TEXT "Not found")
endif()

if (THUNDER_VERBOSE_BUILD)
    message(STATUS "Thunder   : ${THUNDER_FOUND_TEXT}")
    message(STATUS "  version      : ${PC_THUNDER_VERSION}")
    message(STATUS "  cflags       : ${PC_THUNDER_CFLAGS}")
    message(STATUS "  cflags other : ${PC_THUNDER_CFLAGS_OTHER}")
    message(STATUS "  include dirs : ${PC_THUNDER_INCLUDE_DIRS} ${PC_THUNDER_INCLUDEDIR}")
    message(STATUS "  lib dirs     : ${PC_THUNDER_LIBRARY_DIRS} ${PC_THUNDER_LIBDIR}")
    message(STATUS "  include dirs : ${THUNDER_INCLUDE_DIRS}")
    message(STATUS "  libs         : ${THUNDER_LIBRARY}")
endif()

set(THUNDER_DEFINITIONS ${PC_THUNDER_PLUGINS_CFLAGS_OTHER})
set(THUNDER_INCLUDE_DIR ${THUNDER_INCLUDE_DIRS})
set(THUNDER_LIBRARIES ${THUNDER_LIBRARY})
set(THUNDER_LIBRARY_DIRS ${PC_THUNDER_LIBRARY_DIRS})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(THUNDER DEFAULT_MSG
    THUNDER_LIBRARIES
    THUNDER_INCLUDE_DIRS)

if(THUNDER_FOUND)
else()
    message(WARNING "Could not find Thunder")
endif()

mark_as_advanced(
    THUNDER_DEFINITIONS
    THUNDER_INCLUDE_DIRS
    THUNDER_LIBRARIES
    THUNDER_LIBRARY_ThunderPlugins
    THUNDER_LIBRARY_ThunderCore
    THUNDER_LIBRARY_ThunderTracing
    THUNDER_LIBRARY_ThunderProtocols)
