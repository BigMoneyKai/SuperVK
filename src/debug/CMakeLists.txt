add_library(supervk_debug INTERFACE)

target_include_directories(supervk_debug
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(supervk_debug INTERFACE
    supervk_utils
)
