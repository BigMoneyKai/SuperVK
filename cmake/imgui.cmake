add_library(imgui STATIC)

set(IMGUI_DIR
  ${PROJECT_SOURCE_DIR}/thirdparty/imgui
)

target_sources(imgui PRIVATE
  ${IMGUI_DIR}/imgui.cpp
  ${IMGUI_DIR}/imgui_draw.cpp
  ${IMGUI_DIR}/imgui_tables.cpp
  ${IMGUI_DIR}/imgui_widgets.cpp
  ${IMGUI_DIR}/imgui_demo.cpp

  ${IMGUI_DIR}/backends/imgui_impl_vulkan.cpp
  ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp

)

target_include_directories(imgui PUBLIC
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
)

target_link_libraries(imgui PUBLIC
  Vulkan::Vulkan 
  glfw
)
