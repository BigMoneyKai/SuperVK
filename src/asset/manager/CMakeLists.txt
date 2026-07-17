add_library(supervk_asset_manager STATIC)

target_sources(supervk_asset_manager PRIVATE
    mesh_manager.cpp
    texture_manager.cpp
    material_manager.cpp
    shader_manager.cpp
    animation_manager.cpp
    skeleton_manager.cpp
    scene_manager.cpp
)

find_package(Vulkan REQUIRED)

target_include_directories(supervk_asset_manager PRIVATE
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/thirdparty/glm
    ${PROJECT_SOURCE_DIR}/thirdparty/stb
)

target_link_libraries(supervk_asset_manager PRIVATE
    supervk_asset_assets
    supervk_asset_loader
    supervk_core_container_array
    supervk_core_container_string
    supervk_core_job
    supervk_debug

    Vulkan::Vulkan
    glm
)
