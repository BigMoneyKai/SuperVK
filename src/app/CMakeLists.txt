add_library(supervk_app STATIC)

target_sources(supervk_app PRIVATE
    app.cpp
)

find_package(Vulkan REQUIRED)

target_include_directories(supervk_app PRIVATE
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/thirdparty/glfw/include
    ${PROJECT_SOURCE_DIR}/thirdparty/glm
    ${PROJECT_SOURCE_DIR}/thirdparty/stb
)

target_link_libraries(supervk_app PRIVATE
    supervk_render
    supervk_core_job
    supervk_asset_parser
    supervk_asset

    Vulkan::Vulkan
    glm
    glfw
)
