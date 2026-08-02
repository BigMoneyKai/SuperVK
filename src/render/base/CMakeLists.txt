add_library(supervk_render_base STATIC)

target_sources(supervk_render_base PRIVATE
    command_buffer.cpp
    command_pool.cpp
    device.cpp
    instance.cpp
    render_pass.cpp
    surface.cpp
    swapchain.cpp

)

find_package(Vulkan REQUIRED)

target_include_directories(supervk_render_base PRIVATE
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/thirdparty/glm
    ${PROJECT_SOURCE_DIR}/thirdparty/glfw/include
)

target_link_libraries(supervk_render_base PRIVATE
    supervk_core_debug

    Vulkan::Vulkan
    glm
    glfw
)
