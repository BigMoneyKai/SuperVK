#pragma once

#include "defines.h"

#include <vulkan/vulkan.h>
#include <vector>

class Swapchain {
public:
    void init(
        const VkDevice& device,
        const VkPhysicalDevice& physicalDevice,
        const VkSurfaceKHR& surface,
        VkExtent2D windowExtent
    );
    void destroy(const VkDevice& device);

    SV_FORCE_INLINE const std::vector<VkSurfaceFormatKHR>& formats() const {
        return m_formats;
    }
    SV_FORCE_INLINE VkFormat colorFormat() const {
        return m_format;
    }
    SV_FORCE_INLINE const VkSwapchainKHR& swapchain() const {
        return m_swapchain;
    }
    SV_FORCE_INLINE const VkExtent2D& extent() const {
        return m_extent;
    }
    SV_FORCE_INLINE const std::vector<VkImage>& swapchainImages() const {
        return m_swapchainImages;
    }
    SV_FORCE_INLINE const std::vector<VkImageView>& swapchainImageViews() const {
        return m_swapchainImageViews;
    }

private:
    VkSwapchainKHR m_swapchain;
    VkSwapchainCreateInfoKHR m_swapchainCreateInfo{};
    VkSurfaceCapabilitiesKHR m_capabilities;
    VkExtent2D m_extent;
    VkFormat m_format{VK_FORMAT_UNDEFINED};
    std::vector<VkSurfaceFormatKHR> m_formats;
    std::vector<VkImage> m_swapchainImages;
    std::vector<VkImageView> m_swapchainImageViews;
    u32 m_imageCount{0};
};
