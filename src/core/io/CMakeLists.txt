add_library(supervk_core_io STATIC
    file/ifstream.cpp
    file/ofstream.cpp
    memory/imstream.cpp
    memory/omstream.cpp
    buffer/ibstream.cpp
    buffer/obstream.cpp
    binary/binary_reader.cpp
    binary/binary_writer.cpp
    console/icstream.cpp
    console/ocstream.cpp
    text/text_reader.cpp
    text/text_writer.cpp
)

find_package(Vulkan REQUIRED)

target_include_directories(supervk_core_io PRIVATE
    ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(supervk_core_io PRIVATE
    supervk_platform
    supervk_core_memory
    supervk_core_container

    Vulkan::Vulkan
)
