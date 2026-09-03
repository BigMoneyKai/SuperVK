add_library(supervk_core_container_array INTERFACE)

find_package(Vulkan REQUIRED)

target_include_directories(supervk_core_container_array INTERFACE
    ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(supervk_core_container_array INTERFACE
    supervk_platform
    supervk_core_memory

    Vulkan::Vulkan
)
