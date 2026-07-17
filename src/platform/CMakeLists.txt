add_library(supervk_platform STATIC)

target_sources(supervk_platform PRIVATE
    console.cpp
    file.cpp
    thread.cpp
    memory.cpp
    time.cpp
    socket.cpp
)

target_include_directories(supervk_platform PRIVATE
    ${PROJECT_SOURCE_DIR}/src
)


