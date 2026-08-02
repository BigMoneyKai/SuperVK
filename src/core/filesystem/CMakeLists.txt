add_library(supervk_core_file STATIC)

target_include_directories(supervk_core_file PRIVATE
    ${PROJECT_SOURCE_DIR}/src
)

target_sources(supervk_core_file PRIVATE
    file_path.cpp
    file_system.cpp
    file.cpp
    directory.cpp
)

target_link_libraries(supervk_core_file PRIVATE
    supervk_core_io
    supervk_core_container

    supervk_utils
)
