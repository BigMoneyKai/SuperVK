#include "surface.h"
#include "debug/debug.h"

void Surface::init(const VkInstance& instance, GLFWwindow* window) {
    VK_CHECK_RESULT(glfwCreateWindowSurface(instance, window, nullptr, &m_surface));

    DEBUG("Surface created");
}

void Surface::destroy(const VkInstance& instance) {
    vkDestroySurfaceKHR(instance, m_surface, nullptr);
}

