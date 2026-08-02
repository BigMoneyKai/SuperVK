add_library(supervk_render_resource STATIC)

target_sources(supervk_render_resource PRIVATE
    stb_image_impl.cpp
    buffer.cpp
    buffer_manager.cpp
    image.cpp
    sampler.cpp
    texture.cpp
    texture_manager.cpp

)

find_package(Vulkan REQUIRED)

target_include_directories(supervk_render_resource PRIVATE
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/thirdparty/glm
    ${PROJECT_SOURCE_DIR}/thirdparty/stb
)

target_link_libraries(supervk_render_resource PRIVATE
    supervk_render_base
    supervk_utils

    Vulkan::Vulkan
    glm
)
