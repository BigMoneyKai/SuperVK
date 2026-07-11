add_library(supervk_core_memory STATIC)

target_sources(supervk_core_memory PRIVATE
    linear_allocator.cpp
    stack_allocator.cpp
    pool_allocator.cpp
    heap_allocator.cpp
    allocator.cpp
)

find_package(Vulkan REQUIRED)

target_include_directories(supervk_core_memory PRIVATE
    ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(supervk_core_memory PRIVATE
    supervk_platform
    supervk_debug
    Vulkan::Vulkan
)
