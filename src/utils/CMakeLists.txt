add_library(supervk_utils STATIC)

target_sources(supervk_utils PRIVATE
    utils.cpp
)

find_package(Vulkan REQUIRED)

target_include_directories(supervk_utils PRIVATE
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/thirdparty/glfw/include
    ${PROJECT_SOURCE_DIR}/thirdparty/glm
)

target_link_libraries(supervk_utils PRIVATE
    Vulkan::Vulkan
    glm
    glfw
)
