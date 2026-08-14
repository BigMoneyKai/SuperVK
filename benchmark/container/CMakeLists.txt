set(CONTAINER_BENCHMARK_INCLUDES
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/thirdparty/glm
    ${PROJECT_SOURCE_DIR}/thirdparty/stb
    ${PROJECT_SOURCE_DIR}/thirdparty/benchmark/include
)

set(CONTAINER_BENCHMARK_LIBS
    supervk_core_container
    supervk_core_memory
    benchmark::benchmark_main
)

# --- Array benchmarks ---
add_executable(benchmark_container_array
    array/array_bench.cpp
)
target_include_directories(benchmark_container_array PRIVATE
    ${CONTAINER_BENCHMARK_INCLUDES}
)
target_link_libraries(benchmark_container_array PRIVATE
    ${CONTAINER_BENCHMARK_LIBS}
)

# --- HashMap benchmarks ---
add_executable(benchmark_container_hashmap
    hashmap/hashmap_bench.cpp
)
target_include_directories(benchmark_container_hashmap PRIVATE
    ${CONTAINER_BENCHMARK_INCLUDES}
)
target_link_libraries(benchmark_container_hashmap PRIVATE
    ${CONTAINER_BENCHMARK_LIBS}
)

# --- String benchmarks ---
add_executable(benchmark_container_string
    string/string_bench.cpp
)
target_include_directories(benchmark_container_string PRIVATE
    ${CONTAINER_BENCHMARK_INCLUDES}
)
target_link_libraries(benchmark_container_string PRIVATE
    ${CONTAINER_BENCHMARK_LIBS}
)

# --- RingBuffer benchmarks ---
add_executable(benchmark_container_ringbuffer
    ring_buffer/ring_buffer_bench.cpp
)
target_include_directories(benchmark_container_ringbuffer PRIVATE
    ${CONTAINER_BENCHMARK_INCLUDES}
)
target_link_libraries(benchmark_container_ringbuffer PRIVATE
    ${CONTAINER_BENCHMARK_LIBS}
)

# --- Std library comparison benchmarks ---
add_executable(benchmark_container_std
    std_bench.cpp
)
target_include_directories(benchmark_container_std PRIVATE
    ${CONTAINER_BENCHMARK_INCLUDES}
)
target_link_libraries(benchmark_container_std PRIVATE
    ${CONTAINER_BENCHMARK_LIBS}
)
