add_library(supervk_render_pipeline STATIC)

target_sources(supervk_render_pipeline PRIVATE
    shader.cpp
    pipeline.cpp
    pipeline_manager.cpp

)

find_package(Vulkan REQUIRED)

target_include_directories(supervk_render_pipeline PRIVATE
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/thirdparty/glm
)

target_link_libraries(supervk_render_pipeline PRIVATE
    Vulkan::Vulkan
    glm
)
