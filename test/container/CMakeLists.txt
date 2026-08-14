# Common include directories for all container tests
set(CONTAINER_TEST_INCLUDES
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/thirdparty/glm
    ${PROJECT_SOURCE_DIR}/thirdparty/stb
    ${PROJECT_SOURCE_DIR}/thirdparty/test/googletest/include/
)

# Common link libraries
# NOTE: We avoid supervk_core because it references a broken "glm" library target.
# Instead, link the concrete libraries the containers depend on at runtime.
set(CONTAINER_TEST_LIBS
    supervk_core_container
    supervk_core_memory
    gtest_main
)

# --- Array tests ---
add_executable(test_container_array
    array/array_test.cpp
    array/array_stress_test.cpp
)
target_include_directories(test_container_array PRIVATE ${CONTAINER_TEST_INCLUDES})
target_link_libraries(test_container_array PRIVATE ${CONTAINER_TEST_LIBS})

# --- String tests ---
add_executable(test_container_string
    string/string_test.cpp
)
target_include_directories(test_container_string PRIVATE ${CONTAINER_TEST_INCLUDES})
target_link_libraries(test_container_string PRIVATE ${CONTAINER_TEST_LIBS})

# --- StringView tests ---
add_executable(test_container_string_view
    string/string_view_test.cpp
)
target_include_directories(test_container_string_view PRIVATE ${CONTAINER_TEST_INCLUDES})
target_link_libraries(test_container_string_view PRIVATE ${CONTAINER_TEST_LIBS})

# --- HashMap tests ---
add_executable(test_container_hashmap
    hashmap/hashmap_test.cpp
    hashmap/hashmap_stress_test.cpp
)
target_include_directories(test_container_hashmap PRIVATE ${CONTAINER_TEST_INCLUDES})
target_link_libraries(test_container_hashmap PRIVATE ${CONTAINER_TEST_LIBS})

# --- Iterator tests ---
add_executable(test_container_iterator
    iterator/iterator_test.cpp
)
target_include_directories(test_container_iterator PRIVATE ${CONTAINER_TEST_INCLUDES})
target_link_libraries(test_container_iterator PRIVATE ${CONTAINER_TEST_LIBS})

# --- RingBuffer tests ---
add_executable(test_container_ring_buffer
    ring_buffer/ring_buffer_test.cpp
    ring_buffer/ring_buffer_stress_test.cpp
)
target_include_directories(test_container_ring_buffer PRIVATE ${CONTAINER_TEST_INCLUDES})
target_link_libraries(test_container_ring_buffer PRIVATE ${CONTAINER_TEST_LIBS})

# --- general entry ---
add_executable(test_container
    array/array_test.cpp
    array/array_stress_test.cpp
    string/string_test.cpp
    string/string_view_test.cpp
    hashmap/hashmap_test.cpp
    hashmap/hashmap_stress_test.cpp
    iterator/iterator_test.cpp
    ring_buffer/ring_buffer_test.cpp
    ring_buffer/ring_buffer_stress_test.cpp
)
target_include_directories(test_container PRIVATE ${CONTAINER_TEST_INCLUDES})
target_link_libraries(test_container PRIVATE ${CONTAINER_TEST_LIBS})

# Register all tests with CTest
include(GoogleTest)
gtest_discover_tests(test_container)
