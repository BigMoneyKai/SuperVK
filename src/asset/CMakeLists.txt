add_library(supervk_asset STATIC
    asset_manager.cpp
)

add_subdirectory(database)
add_subdirectory(importer)
add_subdirectory(assets)
add_subdirectory(manager)
add_subdirectory(loader)
add_subdirectory(data)
add_subdirectory(cache)
add_subdirectory(handle)

find_package(Vulkan REQUIRED)

target_include_directories(supervk_asset PRIVATE
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/thirdparty/glm
    ${PROJECT_SOURCE_DIR}/thirdparty/stb
)

target_link_libraries(supervk_asset PRIVATE
    supervk_asset_database
    supervk_asset_parser
    supervk_asset_assets
    supervk_asset_manager
    supervk_asset_loader
    supervk_asset_cache
    supervk_asset_data
    supervk_asset_handle
    supervk_core_container_ringbuffer
    supervk_core_container_string
    supervk_core_job

    Vulkan::Vulkan
    glm
)
