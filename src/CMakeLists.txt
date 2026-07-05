add_subdirectory(utils)
add_subdirectory(app)
add_subdirectory(core)
add_subdirectory(scene)
add_subdirectory(render)
add_subdirectory(input)
add_subdirectory(window)


add_executable(supervk
    main.cpp
)

find_package(Vulkan REQUIRED)

target_include_directories(supervk PRIVATE
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/thirdparty/glm
    ${PROJECT_SOURCE_DIR}/thirdparty/glfw/include
    ${PROJECT_SOURCE_DIR}/thirdparty/stb
)

target_link_libraries(supervk PRIVATE
    supervk_app
    supervk_core
    supervk_scene
    supervk_render
    supervk_input
    supervk_window
    supervk_utils
    
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
    $<TARGET_FILE_DIR:supervk>/assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${PROJECT_SOURCE_DIR}/assets
    $<TARGET_FILE_DIR:supervk>/assets
    COMMENT "Copying assets to build output"
)

