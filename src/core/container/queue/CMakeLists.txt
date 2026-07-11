add_library(supervk_core_container_queue INTERFACE)

find_package(Vulkan REQUIRED)

target_include_directories(supervk_core_container_queue INTERFACE
    ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(supervk_core_container_queue INTERFACE
    supervk_core_container_iterator
    supervk_platform
    supervk_core_memory
    Vulkan::Vulkan
)
