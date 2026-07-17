add_library(supervk_asset_loader STATIC)

target_sources(supervk_asset_loader PRIVATE
    loader.cpp
    mesh_loader.cpp
    texture_loader.cpp
    material_loader.cpp
    shader_loader.cpp
    animation_loader.cpp
    skeleton_loader.cpp
    scene_loader.cpp
)

find_package(Vulkan REQUIRED)

target_include_directories(supervk_asset_loader PRIVATE
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/thirdparty/glm
    ${PROJECT_SOURCE_DIR}/thirdparty/stb
)

target_link_libraries(supervk_asset_loader PRIVATE
    supervk_asset_assets
    supervk_asset_parser
    supervk_core_container_array
    supervk_core_container_string

    Vulkan::Vulkan
    glm
)
