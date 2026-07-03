#pragma once

#include <vulkan/vulkan.h>

#include "defines.h"

#include <vector>

class Device {
public:
    void init(const VkInstance& instance, const VkSurfaceKHR& surface);
    void destroy();

    SV_FORCE_INLINE const VkDevice& device() const {
        return m_device;
    }
    SV_FORCE_INLINE const VkPhysicalDevice& physicalDevice() const {
        return m_physicalDevice;
    }
    SV_FORCE_INLINE const VkQueue& graphicsQueue() const {
        return m_graphicsQueue;
    }
    SV_FORCE_INLINE const VkQueue& presentQueue() const {
        return m_presentQueue;
    }
    SV_FORCE_INLINE u32 graphicsQueueFamilyIndex() const {
        return m_graphicsQueueFamilyIndex;
    }
    SV_FORCE_INLINE const VkFormat& depthFormat() const {
        return m_depthFormat;
    }
    SV_FORCE_INLINE VkFormat depthFormat() {
        return m_depthFormat;
    }

private:
    void initDevice(const VkInstance& instance, const VkSurfaceKHR& surface);
    void initQueue(const VkSurfaceKHR& surface);
    void initDeviceExtensions();
    bool isDeviceSuitable(const VkPhysicalDevice& device, const VkSurfaceKHR& surface);
    void createDevice();
    VkFormat findSupportedFormat(
        const std::vector<VkFormat>& candidates, 
        VkImageTiling tiling, 
        VkFormatFeatureFlags features
    ) const;
    VkFormat findDepthFormat() const;


private:
    VkDeviceCreateInfo m_deviceCreateInfo{};
    VkPhysicalDevice m_physicalDevice;
    VkDevice m_device{VK_NULL_HANDLE};
    VkPhysicalDeviceProperties m_physicalDeviceProperties{};
    VkPhysicalDeviceFeatures m_physicalDeviceFeatures{};

    VkDeviceQueueCreateInfo m_queueCreateInfo{};
    VkQueue m_graphicsQueue{VK_NULL_HANDLE};
    VkQueue m_presentQueue{VK_NULL_HANDLE};
    u32 m_graphicsQueueFamilyIndex{UINT32_MAX};

    VkFormat m_depthFormat;

    std::vector<const char*> m_deviceExtensions;

    
};
