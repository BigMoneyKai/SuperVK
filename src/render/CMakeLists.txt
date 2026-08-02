add_library(supervk_render STATIC)

add_subdirectory(base)
add_subdirectory(descriptor)
add_subdirectory(frame)
add_subdirectory(pipeline)
add_subdirectory(resource)

target_sources(supervk_render PRIVATE
    renderer.cpp
)

find_package(Vulkan REQUIRED)

target_include_directories(supervk_render PRIVATE
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/thirdparty/glm
    ${PROJECT_SOURCE_DIR}/thirdparty/glfw/include
    ${PROJECT_SOURCE_DIR}/thirdparty/stb

)

target_link_libraries(supervk_render PRIVATE
    supervk_core_debug

    supervk_render_base
    supervk_render_descriptor
    supervk_render_frame
    supervk_render_resource
    supervk_render_pipeline

    Vulkan::Vulkan
    glm
)
