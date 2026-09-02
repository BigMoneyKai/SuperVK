cmake_minimum_required(VERSION 3.14)

if(APPLE)
    set(CMAKE_C_COMPILER clang)
    set(CMAKE_CXX_COMPILER clang++)
elseif(WIN32)
    set(CMAKE_C_COMPILER cl)
    set(CMAKE_CXX_COMPILER cl)
    add_compile_options(/utf-8)

elseif(UNIX)
    set(CMAKE_C_COMPILER gcc)
    set(CMAKE_CXX_COMPILER g++)
else()
endif()

project(SuperVK VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(BENCHMARK_ENABLE_TESTING OFF)

# Set up environment variables for Vulkan
if(NOT DEFINED VULKAN_SDK)
    if(DEFINED ENV{VULKAN_SDK})
        set(VULKAN_SDK $ENV{VULKAN_SDK})
    endif()
endif()

# Find Vulkan before adding subdirectories
find_package(Vulkan REQUIRED)

# Add Vulkan include directories globally to ensure all modules can find headers
if(VULKAN_SDK)
    include_directories("${VULKAN_SDK}/include")
endif()

# Enable CTest at the top level so `ctest` works from the build root.
# test/ is added before benchmark/, so a plain serial `ctest` runs all
# unit tests first, then all benchmarks.
enable_testing()

set(CMAKE_EXE_LINKER_FLAGS_DEBUG
    "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fsanitize=address"
)

add_subdirectory(src)
add_subdirectory(thirdparty/glfw)

include(cmake/shaders.cmake)
include(cmake/install.cmake)
include(cmake/packaging.cmake)

add_subdirectory(test)
add_subdirectory(benchmark)

