# name | source | text the report must contain for the test to pass
set(HT_TESTS
    "heaptrack_test|test.cpp|Decoder::allocBuffers"
    "test_basic|test_basic.cpp|OUTLIVED_SCOPE"
    "test_realloc|test_realloc.cpp|realloc_api"
    "test_cpp|test_cpp.cpp|OVERWRITTEN"
    "test_threads|test_threads.cpp|Worker::consume"
    "test_c_api|test_c_api.c|parse_manifest"
)

foreach(spec ${HT_TESTS})
    string(REPLACE "|" ";" parts ${spec})
    list(GET parts 0 name)
    list(GET parts 1 src)
    list(GET parts 2 expect)

    add_executable(${name} ${src})
    target_link_libraries(${name} PRIVATE heaptrack pthread)
    target_compile_options(${name} PRIVATE
        -Wall -Wextra -fasynchronous-unwind-tables -fno-omit-frame-pointer)

    add_test(NAME ${name} COMMAND ${name})
    set_tests_properties(${name} PROPERTIES
        ENVIRONMENT "HEAPTRACK_REPORT_AT_EXIT=0"
        PASS_REGULAR_EXPRESSION "${expect}")
endforeach()
