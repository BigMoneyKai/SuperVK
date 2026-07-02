#pragma once

#include "render/renderer.h"
#include "window/window_manager.h"
#include "input/input_manager.h"
#include "scene/scene.h"
#include "core/parser/parser_manager.h"

#include <GLFW/glfw3.h>

struct AppInfo {
    const char* version{"1.0.0"};
    const char* name{"SuperVK"};
};

class App {
public:
    void init(const char* title, DisplayMode mode);
    void run();
    void destroy();

private:
    void update();

private:
    WinMan m_winMan;
    InputMan m_inputMan;
    ParserMan m_parserMan;
    Scene m_scene;
    Renderer m_renderer;

    AppInfo m_appInfo{};
};
