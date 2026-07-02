#include "device.h"
#include "instance.h"
#include "debug/debug.h"
#include "utils/utils.h"

#include <stdlib.h>

void Device::init(const VkInstance& instance, const VkSurfaceKHR& surface) {
    initDevice(instance);
    initDeviceExtensions();
    initQueue(surface);
    createDevice();

    m_depthFormat = findDepthFormat();
}

void Device::destroy() {
    if(m_device != VK_NULL_HANDLE) {
        vkDestroyDevice(m_device, nullptr);
        m_device = VK_NULL_HANDLE;
    }

}

void Device::initDevice(const VkInstance& instance) {
    m_deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    m_deviceCreateInfo.queueCreateInfoCount = 1;
    m_deviceCreateInfo.pQueueCreateInfos = &m_queueCreateInfo;
    m_deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(m_deviceExtensions.size());
    m_deviceCreateInfo.ppEnabledExtensionNames = m_deviceExtensions.data();
    u32 deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
    if(deviceCount == 0) {
        FATAL("No physical device available");
    }
    if(deviceCount == 1)
        DEBUG("1 physical device found");
    else {
        DEBUG("%d physical device found", deviceCount);
    }

    b32 discreteGPUFound = SV_FALSE;
    b32 integratedGPUFound = SV_FALSE;
    for(const auto& device : devices) {
        vkGetPhysicalDeviceProperties(device, &m_physicalDeviceProperties);
        vkGetPhysicalDeviceFeatures(device, &m_physicalDeviceFeatures);

        if(m_physicalDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
            m_physicalDevice = device;
            discreteGPUFound = SV_TRUE;
        }
        if(m_physicalDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU) {
            if(!discreteGPUFound) {
                m_physicalDevice = device;
            }
            integratedGPUFound = SV_TRUE;
        }
    }
    if(!discreteGPUFound && !integratedGPUFound) {
        FATAL("Failed to find either discrete GPU or integrated GPU");
    } else if(discreteGPUFound) {
        DEBUG("Found discrete GPU(No integrated GPU available): %s", m_physicalDeviceProperties.deviceName);
    } else if(integratedGPUFound) {
        DEBUG("Found integrated GPU(No discrete GPU available): %s", m_physicalDeviceProperties.deviceName);
    } else {
        DEBUG(
            "Found discrete GPU and integrated GPU, using discrete GPU: %s",
            m_physicalDeviceProperties.deviceName
        );
    }

}
void Device::initQueue(const VkSurfaceKHR& surface) {
    u32 queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueFamilyCount, queueFamilies.data());
    m_graphicsQueueFamilyIndex = UINT32_MAX;

    for(uint32_t i = 0; i < queueFamilies.size(); i++) {
        VkBool32 presentSupport = VK_FALSE;
        
        vkGetPhysicalDeviceSurfaceSupportKHR(m_physicalDevice, i, surface, &presentSupport);
        if((queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) && presentSupport) {
            m_graphicsQueueFamilyIndex = i;
            break;
        }
    }
    if(m_graphicsQueueFamilyIndex == UINT32_MAX) {
        FATAL("Failed to find graphics queue family to present and render");
    }
    DEBUG("Graphics queue family found, index: %d", m_graphicsQueueFamilyIndex);

}

void Device::initDeviceExtensions() {
    m_deviceExtensions.clear();
    m_deviceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
#if defined(__APPLE__)
    m_deviceExtensions.push_back(
        "VK_KHR_portability_subset"
    );
#endif
}

void Device::createDevice() {
    f32 queuePriority = 1.0f;
    m_queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    m_queueCreateInfo.queueFamilyIndex = m_graphicsQueueFamilyIndex;
    m_queueCreateInfo.queueCount = 1;
    m_queueCreateInfo.pQueuePriorities = &queuePriority;

    m_deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    m_deviceCreateInfo.queueCreateInfoCount = 1;
    m_deviceCreateInfo.pQueueCreateInfos = &m_queueCreateInfo;
    m_deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(m_deviceExtensions.size());
    m_deviceCreateInfo.ppEnabledExtensionNames = m_deviceExtensions.data();

    VK_CHECK_RESULT(vkCreateDevice(m_physicalDevice, &m_deviceCreateInfo, nullptr, &m_device));
    vkGetDeviceQueue(m_device, m_graphicsQueueFamilyIndex, 0, &m_graphicsQueue);
    m_presentQueue = m_graphicsQueue;
    DEBUG("Lo=gical device created");
}

VkFormat Device::findSupportedFormat(
    const std::vector<VkFormat>& candidates,
    VkImageTiling tiling,
    VkFormatFeatureFlags features
) const {
    for(auto format : candidates) {
        VkFormatProperties formatProps;
        
        vkGetPhysicalDeviceFormatProperties(
            m_physicalDevice,
            format,
            &formatProps
        );

        if(tiling == VK_IMAGE_TILING_LINEAR && (formatProps.linearTilingFeatures & features) == features) {
            return format;
        }
        if(tiling == VK_IMAGE_TILING_OPTIMAL && (formatProps.optimalTilingFeatures & features) == features) {
            return format;
        }
    }
    FATAL("No supported depth format");
    return VK_FORMAT_UNDEFINED;
}

VkFormat Device::findDepthFormat() const {
    return findSupportedFormat(
        {
            VK_FORMAT_D32_SFLOAT,
            VK_FORMAT_D32_SFLOAT_S8_UINT,
            VK_FORMAT_D24_UNORM_S8_UINT
        },
        VK_IMAGE_TILING_OPTIMAL,
        VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
    );
}

