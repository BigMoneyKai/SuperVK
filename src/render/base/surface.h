#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "defines.h"

class Surface {
public:
    void init(const VkInstance& instance, GLFWwindow* window);
    void destroy();

    SV_FORCE_INLINE const VkSurfaceKHR& surface() const {
        return m_surface;
    }
    SV_FORCE_INLINE VkSurfaceKHR surface() {
        return m_surface;
    }

private:
    VkInstance m_instance{VK_NULL_HANDLE};
    VkSurfaceKHR m_surface;

};
