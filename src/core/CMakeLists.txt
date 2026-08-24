add_library(supervk_core INTERFACE)

add_subdirectory(container)
add_subdirectory(debug)
add_subdirectory(except)
add_subdirectory(event)
add_subdirectory(filesystem)
add_subdirectory(io)
add_subdirectory(job)
add_subdirectory(memory)

find_package(Vulkan REQUIRED)

target_include_directories(supervk_core INTERFACE
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/thirdparty
)

target_link_libraries(supervk_core INTERFACE
    supervk_core_memory
    supervk_core_container
    supervk_core_io

    Vulkan::Vulkan
    glm
)
