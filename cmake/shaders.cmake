set(GLSL_DIR ${PROJECT_SOURCE_DIR}/shaders/glsl)
set(SPV_DIR  ${PROJECT_SOURCE_DIR}/shaders/spv)
find_program(GLSLANG_VALIDATOR glslangValidator REQUIRED)
file(GLOB GLSL_SOURCES
    CONFIGURE_DEPENDS
    "${GLSL_DIR}/*.vert"
    "${GLSL_DIR}/*.frag"
    "${GLSL_DIR}/*.comp"
    "${GLSL_DIR}/*.geom"
    "${GLSL_DIR}/*.tesc"
    "${GLSL_DIR}/*.tese"
)

file(MAKE_DIRECTORY ${SPV_DIR})

set(SPIRV_BINARY_FILES)

foreach(GLSL ${GLSL_SOURCES})
    get_filename_component(FILE_NAME ${GLSL} NAME)
    set(SPIRV "${SPV_DIR}/${FILE_NAME}.spv")

    add_custom_command(
        OUTPUT ${SPIRV}
        COMMAND ${GLSLANG_VALIDATOR}
                -V
                ${GLSL}
                -o ${SPIRV}
        DEPENDS ${GLSL}
        COMMENT "Compiling ${FILE_NAME}"
        VERBATIM
    )

    list(APPEND SPIRV_BINARY_FILES ${SPIRV})
endforeach()

add_custom_target(shaders
    DEPENDS ${SPIRV_BINARY_FILES}
)

add_dependencies(supervk shaders)
