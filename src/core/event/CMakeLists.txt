add_library(supervk_core_event STATIC)

target_include_directories(supervk_core_event
    PRIVATE
      ${PROJECT_SOURCE_DIR}/src
)

target_sources(supervk_core_event PRIVATE
    event_system.cpp
)

target_link_libraries(supervk_core_event PRIVATE
    supervk_core_debug
)
