#pragma once

#include <GLFW/glfw3.h>
#include "defines.h"

#include <string>

constexpr u32 default_width = 1280;
constexpr u32 default_height = 720;

enum DisplayMode {
    FULLSCREEN,
    BORDERLESS,
    WINDOWED,
};

class WinMan {
public:
    WinMan();
    void init(const char* title, DisplayMode display);
    void destroy();

    SV_INLINE const GLFWwindow* window() const {
        return m_window;
    }
    SV_INLINE GLFWwindow* window() {
        return m_window;
    }

    SV_INLINE const u32& width() const {
        return m_width;
    }
    SV_INLINE u32& width() {
        return m_width;
    }

    SV_INLINE const u32& height() const {
        return m_height;
    }
    SV_INLINE u32& height() {
        return m_height;
    }

    SV_INLINE const b32& headless() const {
        return m_headless;
    }
    SV_INLINE b32& headless() {
        return m_headless;
    }

private:
    GLFWwindow* m_window;
    u32 m_width;
    u32 m_height;
    b32 m_headless;

    const GLFWvidmode* m_mode;
    GLFWmonitor* m_monitor;

};

