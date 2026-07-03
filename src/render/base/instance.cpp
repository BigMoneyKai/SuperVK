#include "defines.h"
#include "instance.h"
#include "debug/debugger.h"

#include <GLFW/glfw3.h>

#include <cstring>

void Instance::init() {

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "SuperVK";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "SuperVK Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

#ifndef NDEBUG
    b32 validationLayerEnabled = enable_validation_layer();
#else
    b32 validationLayerEnabled = SV_FALSE;
#endif
    enable_extensions();

    u32 instanceFlags = 0;
#if defined(__APPLE__)
    instanceFlags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
#endif

    VkInstanceCreateInfo instanceCreateInfo{};
    instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCreateInfo.pApplicationInfo = &appInfo;
    if(validationLayerEnabled) {
        instanceCreateInfo.enabledLayerCount = static_cast<u32>(m_validationLayers.size());
        instanceCreateInfo.ppEnabledLayerNames = m_validationLayers.data();
    } else {
        instanceCreateInfo.enabledLayerCount = 0;
    }
    instanceCreateInfo.enabledExtensionCount = static_cast<u32>(m_extensions.size());
    instanceCreateInfo.ppEnabledExtensionNames = m_extensions.data();
    instanceCreateInfo.flags = instanceFlags;

    VK_CHECK_RESULT(vkCreateInstance(&instanceCreateInfo, nullptr, &m_instance));
}

void Instance::destroy() {
    vkDestroyInstance(m_instance, nullptr);
}

b32 Instance::enable_validation_layer() {
    u32 layerCount = 0;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
    b32 layerFound = SV_FALSE;
    for(const char* layerName : m_validationLayers) {
        for(const auto& layerProperties : availableLayers) {
            if(strcmp(layerProperties.layerName, layerName) == 0) {
                layerFound = SV_TRUE;
                break;
            }
        }
    }
    if(!layerFound) {
        WARNING("Validation layer not found in SDK, please install it or config it");
    } else {
        DEBUG("Validation layer enabled");
    }
    return layerFound;
}

void Instance::enable_extensions() {
    u32 glfwExtensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    m_extensions.assign(glfwExtensions, glfwExtensions + glfwExtensionCount);
#if defined(__APPLE__)
    m_extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
    m_extensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
#endif
}
