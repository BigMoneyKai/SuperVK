add_library(supervk_render_frame STATIC)

target_sources(supervk_render_frame PRIVATE
    depth_resource.cpp
    framebuffer.cpp
    frame_resource.cpp
    sync.cpp

)

find_package(Vulkan REQUIRED)

target_include_directories(supervk_render_frame PRIVATE
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/thirdparty/glm
)

target_link_libraries(supervk_render_frame PRIVATE
    Vulkan::Vulkan
    glm
)
