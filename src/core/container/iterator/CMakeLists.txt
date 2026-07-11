add_library(supervk_core_container_iterator INTERFACE)

find_package(Vulkan REQUIRED)

target_include_directories(supervk_core_container_iterator INTERFACE
    ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(supervk_core_container_iterator INTERFACE
    supervk_platform
)
