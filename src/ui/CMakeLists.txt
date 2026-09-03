add_library(supervk_ui STATIC
    layout.cpp
    manager.cpp
    panel.cpp
    settings.cpp

    panel/bottom_bar.cpp
    panel/console.cpp
    panel/file_zone.cpp
    panel/inspector.cpp
    panel/scene.cpp
    panel/settings_panel.cpp
    panel/side_bar.cpp
    panel/stats.cpp
    panel/top_bar.cpp
    panel/splitter.cpp

    settings/color.cpp
    settings/font.cpp
    settings/icon.cpp
    settings/interaction.cpp
    settings/style.cpp
 "panel/inspector.cpp" "panel/inspector.h")

target_include_directories(supervk_ui PUBLIC
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/thirdparty/imgui
)

target_link_libraries(supervk_ui PUBLIC
    imgui
)
