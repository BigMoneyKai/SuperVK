add_library(supervk_asset_parser STATIC)

target_sources(supervk_asset_parser PRIVATE
    importer_manager.cpp
    obj/objparser.cpp
    gltf/gltfparser.cpp
    gltf/glbparser.cpp
)

find_package(Vulkan REQUIRED)

target_include_directories(supervk_asset_parser PRIVATE
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/thirdparty/glm
)

target_link_libraries(supervk_asset_parser PRIVATE
    supervk_scene
    supervk_debug

    Vulkan::Vulkan
    glm
)
