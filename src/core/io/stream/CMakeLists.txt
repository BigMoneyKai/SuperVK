add_library(supervk_core_io_stream INTERFACE)

find_package(Vulkan REQUIRED)

target_include_directories(supervk_core_io_stream INTERFACE
    ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(supervk_core_io_stream INTERFACE
    supervk_platform

    Vulkan::Vulkan
)
