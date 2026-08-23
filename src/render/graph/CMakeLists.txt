add_library(supervk_render_graph STATIC)

target_sources(supervk_render_graph PRIVATE
    render_graph.cpp
)

find_package(Vulkan REQUIRED)

target_include_directories(supervk_render_graph PRIVATE
    ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(supervk_render_graph PRIVATE
    supervk_core_debug
    Vulkan::Vulkan
)
