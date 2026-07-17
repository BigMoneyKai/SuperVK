add_library(supervk_asset_assets STATIC)

target_sources(supervk_asset_assets PRIVATE
    asset.cpp
    mesh_asset.cpp
    texture_asset.cpp
    material_asset.cpp
    shader_asset.cpp
    animation_asset.cpp
    skeleton_asset.cpp
    scene_asset.cpp
)

find_package(Vulkan REQUIRED)

target_include_directories(supervk_asset_assets PRIVATE
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/thirdparty/glm
)

target_link_libraries(supervk_asset_assets PRIVATE
    supervk_scene
    supervk_core_container_string
    supervk_core_container_array
    Vulkan::Vulkan
    glm
)
