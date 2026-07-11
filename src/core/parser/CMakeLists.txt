add_library(supervk_core_parser STATIC)

target_sources(supervk_core_parser PRIVATE
    gltf/glbparser.cpp
    gltf/gltfparser.cpp
    obj/objparser.cpp
    parser_manager.cpp
)

find_package(Vulkan REQUIRED)

target_include_directories(supervk_core_parser PRIVATE
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/thirdparty/glm
)

target_link_libraries(supervk_core_parser PRIVATE
    supervk_scene
    supervk_debug

    Vulkan::Vulkan
    glm
)
