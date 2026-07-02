add_library(supervk_input STATIC)

target_sources(supervk_input PRIVATE
    input_manager.cpp
    mouse.cpp
    keyboard.cpp
)

target_include_directories(supervk_input PRIVATE
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/thirdparty/glm
    ${PROJECT_SOURCE_DIR}/thirdparty/glfw/include
)

target_link_libraries(supervk_input PRIVATE
    glm
    glfw
)


