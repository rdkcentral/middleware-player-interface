cmake_minimum_required(VERSION 3.10)
project(APIMiddleware)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(Threads REQUIRED)

# Check if dependencies exist in third_party/
set(HTTPLIB_PATH "${CMAKE_SOURCE_DIR}/third_party/httplib.h")
set(JSON_PATH "${CMAKE_SOURCE_DIR}/third_party/json.hpp")

if(NOT EXISTS ${HTTPLIB_PATH})
    message(FATAL_ERROR 
        "httplib.h not found!\n"
        "Please download it to third_party/:\n"
        "  mkdir -p third_party\n"
        "  curl -L -o third_party/httplib.h https://raw.githubusercontent.com/yhirose/cpp-httplib/master/httplib.h"
    )
endif()

if(NOT EXISTS ${JSON_PATH})
    message(FATAL_ERROR 
        "json.hpp not found!\n"
        "Please download it to third_party/:\n"
        "  mkdir -p third_party\n"
        "  curl -L -o third_party/json.hpp https://raw.githubusercontent.com/nlohmann/json/develop/single_include/nlohmann/json.hpp"
    )
endif()

message(STATUS "Found httplib.h")
message(STATUS "Found json.hpp")

# Include directories
include_directories(${CMAKE_SOURCE_DIR})              # ← Source root
include_directories(${CMAKE_SOURCE_DIR}/third_party)  # ← Third party libs

# Custom target to auto-generate API server
add_custom_command(
    OUTPUT api_server.cpp
    COMMAND python3 ${CMAKE_SOURCE_DIR}/api_generator.py ${CMAKE_SOURCE_DIR}/api_interface.h
    DEPENDS api_interface.h api_generator.py
    COMMENT "Auto-generating API server from interface"
)

# Build the server
add_executable(server 
    api_server.cpp
    ${CMAKE_SOURCE_DIR}/business_logic.cpp    # ← Full path to source
)

# Link the server executable to the playergstinterface shared library
target_link_libraries(server PRIVATE playergstinterface)

# Ensure Threads::Threads is linked to the server executable
target_link_libraries(server PRIVATE Threads::Threads)

# Install the server executable to the bin directory
install(TARGETS server DESTINATION bin)

