add_library(supervk_core_container INTERFACE)

target_include_directories(supervk_core_container
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(supervk_core_container INTERFACE
    supervk_core_memory
    supervk_platform
)
