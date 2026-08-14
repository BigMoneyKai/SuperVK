set(FILESYSTEM_BENCHMARK_INCLUDES
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/thirdparty/benchmark/include
)

add_executable(benchmark_filesystem
    filesystem_bench.cpp
)
target_include_directories(benchmark_filesystem PRIVATE
    ${FILESYSTEM_BENCHMARK_INCLUDES}
)
target_link_libraries(benchmark_filesystem PRIVATE
    supervk_core_memory
    supervk_core_filesystem
    benchmark::benchmark_main
)
