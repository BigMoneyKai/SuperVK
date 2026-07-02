add_library(supervk_core STATIC)

target_sources(supervk_core PRIVATE
    parser/obj/objparser.cpp
    parser/gltf/gltfparser.cpp
    parser/gltf/glbparser.cpp
    parser/parser_manager.cpp
)

find_package(Vulkan REQUIRED)

target_include_directories(supervk_core PRIVATE
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/thirdparty/glm
)

target_link_libraries(supervk_core PRIVATE
    supervk_scene

    Vulkan::Vulkan
    glm
)
