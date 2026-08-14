add_library(supervk_core_debug STATIC)

target_include_directories(supervk_core_debug
    PRIVATE
      ${PROJECT_SOURCE_DIR}/src
)

target_sources(supervk_core_debug PRIVATE
    logger.cpp
    log_tag.cpp
)

target_link_libraries(supervk_core_debug PRIVATE
    supervk_utils
    supervk_platform
    supervk_core_container_string
    supervk_core_container_ringbuffer
    supervk_core_io

)
