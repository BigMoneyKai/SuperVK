add_library(supervk_core_except STATIC)

target_include_directories(supervk_core_except
    PRIVATE
      ${PROJECT_SOURCE_DIR}/src
)

target_sources(supervk_core_except PRIVATE
    exception_str.cpp
    memory_exception.cpp
)

target_link_libraries(supervk_core_except PRIVATE
    supervk_core_container_string

)
