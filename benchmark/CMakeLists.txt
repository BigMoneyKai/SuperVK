# All benchmark executables go to out/benchmark/
add_subdirectory(${PROJECT_SOURCE_DIR}/thirdparty/benchmark
                ${PROJECT_BINARY_DIR}/thirdparty/benchmark)

add_subdirectory(container)
add_subdirectory(memory)
add_subdirectory(filesystem)

# Register each benchmark executable as one CTest test. Defined after the
# unit tests (root CMakeLists adds test/ before benchmark/), so a plain
# serial `ctest` runs every unit test first, then every benchmark.
# Per-case timings are printed by running the binaries directly
# (see script/benchmark_run.py); ctest itself only reports pass/fail.
set(SV_BENCHMARK_TESTS
    benchmark_container_array
    benchmark_container_hashmap
    benchmark_container_ringbuffer
    benchmark_container_string
    benchmark_container_std
    benchmark_memory_heap
    benchmark_memory_linear
    benchmark_memory_pool
    benchmark_memory_stack
    benchmark_filesystem
)
foreach(sv_bench IN LISTS SV_BENCHMARK_TESTS)
    add_test(NAME ${sv_bench} COMMAND $<TARGET_FILE:${sv_bench}>)
    set_tests_properties(${sv_bench} PROPERTIES
        LABELS "benchmark"
        RUN_SERIAL TRUE
    )
endforeach()
