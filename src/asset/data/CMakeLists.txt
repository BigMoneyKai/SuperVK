add_library(supervk_asset_data INTERFACE)

target_include_directories(supervk_asset_data INTERFACE
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/thirdparty/glm
)

target_link_libraries(supervk_asset_data INTERFACE
    glm
)
