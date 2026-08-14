set(MEMORY_BENCHMARK_INCLUDES
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/thirdparty/benchmark/include
)

set(MEMORY_BENCHMARK_LIBS
    supervk_core_memory
    supervk_core_container
    benchmark::benchmark_main
)

# --- Heap Allocator benchmarks ---
add_executable(benchmark_memory_heap
    heap_allocator_bench.cpp
)
target_include_directories(benchmark_memory_heap PRIVATE
    ${MEMORY_BENCHMARK_INCLUDES}
)
target_link_libraries(benchmark_memory_heap PRIVATE
    ${MEMORY_BENCHMARK_LIBS}
)

# --- Linear Allocator benchmarks ---
add_executable(benchmark_memory_linear
    linear_allocator_bench.cpp
)
target_include_directories(benchmark_memory_linear PRIVATE
    ${MEMORY_BENCHMARK_INCLUDES}
)
target_link_libraries(benchmark_memory_linear PRIVATE
    ${MEMORY_BENCHMARK_LIBS}
)

# --- Stack Allocator benchmarks ---
add_executable(benchmark_memory_stack
    stack_allocator_bench.cpp
)
target_include_directories(benchmark_memory_stack PRIVATE
    ${MEMORY_BENCHMARK_INCLUDES}
)
target_link_libraries(benchmark_memory_stack PRIVATE
    ${MEMORY_BENCHMARK_LIBS}
)

# --- Pool Allocator benchmarks ---
add_executable(benchmark_memory_pool
    pool_allocator_bench.cpp
)
target_include_directories(benchmark_memory_pool PRIVATE
    ${MEMORY_BENCHMARK_INCLUDES}
)
target_link_libraries(benchmark_memory_pool PRIVATE
    ${MEMORY_BENCHMARK_LIBS}
)
