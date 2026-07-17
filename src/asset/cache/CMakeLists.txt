add_library(supervk_asset_cache STATIC)

target_sources(supervk_asset_cache PRIVATE
    cache_manager.cpp
)

find_package(Vulkan REQUIRED)

target_include_directories(supervk_asset_cache PRIVATE
    ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(supervk_asset_cache PRIVATE
    supervk_core_container_array
    supervk_core_container_string

    Vulkan::Vulkan
)
