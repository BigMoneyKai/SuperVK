add_executable(supervk_test_memory
    heap_allocator_test.cpp
    linear_allocator_test.cpp
    pool_allocator_test.cpp
    stack_allocator_test.cpp
    allocator_stress_test.cpp
)

target_include_directories(supervk_test_memory PRIVATE
    ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(supervk_test_memory PRIVATE
    supervk_core_memory
    gtest_main
)

include(GoogleTest)
gtest_discover_tests(supervk_test_memory)
