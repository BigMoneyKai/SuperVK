add_library(supervk_core_container_hashmap INTERFACE)

find_package(Vulkan REQUIRED)

target_include_directories(supervk_core_container_hashmap INTERFACE
    ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(supervk_core_container_hashmap INTERFACE
    supervk_core_container_array
    supervk_core_container_iterator
    supervk_core_container_string

    Vulkan::Vulkan
)
