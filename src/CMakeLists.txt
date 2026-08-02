add_subdirectory(app)
add_subdirectory(core)
add_subdirectory(scene)
add_subdirectory(render)
add_subdirectory(input)
add_subdirectory(window)
add_subdirectory(platform)
add_subdirectory(utils)
add_subdirectory(asset)

add_executable(supervk
    main.cpp
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_DEBUG_BINARY_DIR}/bin)

# remove sanitizer when releasing
set(CMAKE_CXX_FLAGS_DEBUG
    "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address -fno-omit-frame-pointer"
)

set(CMAKE_EXE_LINKER_FLAGS_DEBUG
    "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fsanitize=address"
)

find_package(Vulkan REQUIRED)

target_include_directories(supervk PRIVATE
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/thirdparty/glm
    ${PROJECT_SOURCE_DIR}/thirdparty/glfw/include
    ${PROJECT_SOURCE_DIR}/thirdparty/stb
    ${PROJECT_SOURCE_DIR}/thirdparty/benchmark
)

target_link_libraries(supervk PRIVATE
    supervk_app
    supervk_core
    supervk_scene
    supervk_render
    supervk_input
    supervk_window
    supervk_utils
    supervk_platform
    supervk_asset_parser
    supervk_asset

    Vulkan::Vulkan
)

# Copy SPIR-V shaders to the executable output directory so they can
# be loaded at runtime via relative paths (e.g. "shaders/spv/basic.vert.spv")
add_custom_command(TARGET supervk POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
    $<TARGET_FILE_DIR:supervk>/shaders/spv
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${PROJECT_SOURCE_DIR}/shaders/spv
    $<TARGET_FILE_DIR:supervk>/shaders/spv
    COMMENT "Copying SPIR-V shaders to build output"
)

add_custom_command(TARGET supervk POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
    $<TARGET_FILE_DIR:supervk>/resource
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${PROJECT_SOURCE_DIR}/resource
    $<TARGET_FILE_DIR:supervk>/resource
    COMMENT "Copying assets to build output"
)

