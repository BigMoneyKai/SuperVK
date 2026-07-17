add_library(supervk_asset_database STATIC
    asset_database.cpp
    asset_registry.cpp
)

find_package(Vulkan REQUIRED)

target_include_directories(supervk_asset_database PRIVATE
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/thirdparty/glm
    ${PROJECT_SOURCE_DIR}/thirdparty/stb
)

target_link_libraries(supervk_asset_database PRIVATE
    supervk_core_container_hashmap
    supervk_core_container_string

    Vulkan::Vulkan
    glm
)
