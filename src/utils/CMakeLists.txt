add_library(supervk_utils STATIC)

target_sources(supervk_utils PRIVATE
    utils.cpp
)

target_include_directories(supervk_utils PRIVATE
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/thirdparty/glfw/include
    ${PROJECT_SOURCE_DIR}/thirdparty/glm
)

target_link_libraries(supervk_utils PRIVATE
    glm
    glfw
)
