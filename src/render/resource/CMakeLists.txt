add_library(supervk_render_resource STATIC)

target_sources(supervk_render_resource PRIVATE
    buffer.cpp
    buffer_manager.cpp
    image.cpp
    sampler.cpp

)

find_package(Vulkan REQUIRED)

target_include_directories(supervk_render_resource PRIVATE
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/thirdparty/glm
)

target_link_libraries(supervk_render_resource PRIVATE
    Vulkan::Vulkan
    glm
)
