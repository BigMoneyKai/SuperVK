add_library(supervk_core_container_string INTERFACE)

target_include_directories(supervk_core_container_string INTERFACE
    ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(supervk_core_container_string INTERFACE
    supervk_platform
    supervk_core_memory
)
