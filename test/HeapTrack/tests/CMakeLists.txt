set(HT_TESTS
    test_basic
    test_realloc
    test_cpp
    test_threads
)

foreach(t ${HT_TESTS})
    add_executable(${t} ${t}.cpp)
    target_link_libraries(${t} PRIVATE heaptrack pthread)
    target_compile_options(${t} PRIVATE -Wall -Wextra -fasynchronous-unwind-tables -fno-omit-frame-pointer)
    add_test(NAME ${t} COMMAND ${t})
    set_tests_properties(${t} PROPERTIES ENVIRONMENT "HEAPTRACK_REPORT_AT_EXIT=0")
endforeach()

add_executable(test_c_api test_c_api.c)
target_link_libraries(test_c_api PRIVATE heaptrack)
target_compile_options(test_c_api PRIVATE -Wall -Wextra -fasynchronous-unwind-tables -fno-omit-frame-pointer)
add_test(NAME test_c_api COMMAND test_c_api)
set_tests_properties(test_c_api PROPERTIES ENVIRONMENT "HEAPTRACK_REPORT_AT_EXIT=0")
