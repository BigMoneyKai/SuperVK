add_library(supervk_window STATIC)

target_sources(supervk_window PRIVATE
    window_manager.cpp
)

find_package(Vulkan REQUIRED)

target_include_directories(supervk_window PRIVATE
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/thirdparty/glfw/include
    ${PROJECT_SOURCE_DIR}/thirdparty/glm
)

target_link_libraries(supervk_window PRIVATE
    Vulkan::Vulkan
    glm
    glfw
)
