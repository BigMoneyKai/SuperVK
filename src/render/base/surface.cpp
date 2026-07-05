#include "surface.h"
#include "debug/debugger.h"

void Surface::init(const VkInstance& instance, GLFWwindow* window) {
    m_instance = instance;
    VK_CHECK_RESULT(glfwCreateWindowSurface(instance, window, nullptr, &m_surface));

    DEBUG("Surface created");
}

void Surface::destroy() {
    vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
}

