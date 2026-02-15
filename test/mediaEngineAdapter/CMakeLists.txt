cmake_minimum_required(VERSION 3.10)
project(APIMiddleware)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(Threads REQUIRED)

# Check if dependencies exist in third_party/


# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR})              # ← Source root
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/third_party)  # ← Third party libs

add_library(mongoose STATIC third_party/mongoose.c)

# Custom target to auto-generate API server
add_custom_command(
    OUTPUT api_server.cpp
    COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/api_generator.py ${CMAKE_CURRENT_SOURCE_DIR}/api_interface.h
    DEPENDS api_interface.h api_generator.py
    COMMENT "Auto-generating API server from interface"
)

# Build the server
add_executable(server 
    api_server.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/business_logic.cpp    # ← Full path to source
)

# Link the server executable to the playergstinterface shared library
target_link_libraries(server PRIVATE playergstinterface mongoose)

# Ensure Threads::Threads is linked to the server executable
target_link_libraries(server PRIVATE Threads::Threads)

# Install the server executable to the bin directory
install(TARGETS server DESTINATION bin)

