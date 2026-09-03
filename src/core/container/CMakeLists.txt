add_library(supervk_core_container INTERFACE)

add_subdirectory(array)
add_subdirectory(hashmap)
add_subdirectory(ring_buffer)
add_subdirectory(string)

target_include_directories(supervk_core_container
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

