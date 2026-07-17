add_library(supervk_debug INTERFACE)

target_include_directories(supervk_debug
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(supervk_debug PRIVATE
    logger.cpp
    except/exception_str.cpp
)

target_link_libraries(supervk_debug INTERFACE
    supervk_utils
)
