add_library(supervk_render_descriptor STATIC)

target_sources(supervk_render_descriptor PRIVATE
    descriptor_manager.cpp
    descriptor_pool.cpp
    descriptor_set.cpp
    descriptor_set_layout.cpp
    descriptor_writer.cpp

)

find_package(Vulkan REQUIRED)

target_include_directories(supervk_render_descriptor PRIVATE
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/thirdparty/glm
)

target_link_libraries(supervk_render_descriptor PRIVATE
    Vulkan::Vulkan
    glm
)
