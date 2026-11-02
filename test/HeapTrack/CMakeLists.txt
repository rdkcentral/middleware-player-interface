cmake_minimum_required(VERSION 3.10)
project(heaptrack C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif()

add_library(heaptrack SHARED
    src/ht_api.cpp
    src/ht_bootstrap.c
    src/ht_interceptor.cpp
    src/ht_context.cpp
    src/ht_registry.cpp
    src/ht_unwind.cpp
    src/ht_stackdepot.cpp
    src/ht_elf.cpp
    src/ht_symbolizer.cpp
    src/ht_leak.cpp
    src/ht_report.cpp
    src/ht_sink.cpp
)

target_include_directories(heaptrack
    PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_options(heaptrack PRIVATE
    -Wall -Wextra -fvisibility=hidden -fasynchronous-unwind-tables
)

target_link_libraries(heaptrack PRIVATE dl pthread)

install(TARGETS heaptrack LIBRARY DESTINATION lib)
install(FILES include/heaptrack.h include/heaptrack.hpp DESTINATION include)

option(HEAPTRACK_BUILD_TESTS "Build heaptrack test programs" ON)
if(HEAPTRACK_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
