add_library(supervk_scene STATIC)

target_sources(supervk_scene PRIVATE
    camera.cpp
    light.cpp
    material.cpp
    mesh.cpp
    object.cpp
    scene.cpp
)

find_package(Vulkan REQUIRED)

target_include_directories(supervk_scene PRIVATE
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/thirdparty/glm
)

target_link_libraries(supervk_scene PRIVATE
    supervk_render_resource

    Vulkan::Vulkan
    glm
)
