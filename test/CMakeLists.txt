# Build Google Test from thirdparty sources.
# googletest expects GOOGLETEST_VERSION to be set externally.
set(GOOGLETEST_VERSION 1.14.0)
add_subdirectory(${PROJECT_SOURCE_DIR}/thirdparty/test/googletest
                 ${PROJECT_BINARY_DIR}/thirdparty/test/googletest)

# glm is header-only; the CMake target is needed because supervk_utils
# references "glm" as a link dependency.
add_subdirectory(${PROJECT_SOURCE_DIR}/thirdparty/glm/glm
                 ${PROJECT_BINARY_DIR}/thirdparty/glm)

add_subdirectory(container)
add_subdirectory(memory)
