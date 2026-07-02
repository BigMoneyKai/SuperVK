cmake_minimum_required(VERSION 3.25)

project(SuperVK VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(src)
add_subdirectory(thirdparty/glm)
add_subdirectory(thirdparty/glfw)

include(cmake/shaders.cmake)
include(cmake/install.cmake)
include(cmake/packaging.cmake)

add_compile_options(
    -fsanitize=address
    -fno-omit-frame-pointer
)

add_link_options(
    -fsanitize=address
)

