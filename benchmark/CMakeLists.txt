find_package(Vulkan REQUIRED)

add_executable(supervk_bench
    container_bench.cpp
)

target_include_directories(supervk_bench PRIVATE
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/thirdparty/glm
    ${PROJECT_SOURCE_DIR}/thirdparty/benchmark/include
)

target_link_libraries(supervk_bench PRIVATE
    supervk_core_container_array
    supervk_core_container_hashmap
    supervk_core_memory
    supervk_platform
    Vulkan::Vulkan
    glm
    benchmark::benchmark
)
