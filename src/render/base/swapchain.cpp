#include "swapchain.h"
#include "core/debug/debugger.h"

#include <algorithm>
#include <stdexcept>

static VkSurfaceFormatKHR chooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats) {
    for(const auto& format : formats) {
        if(format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return format;
        }
    }
    return formats.front();
}

static VkPresentModeKHR choosePresentMode(const std::vector<VkPresentModeKHR>& presentModes) {
    for(const auto& presentMode : presentModes) {
        if(presentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return presentMode;
        }
    }
    return VK_PRESENT_MODE_FIFO_KHR;
}

void Swapchain::init(
    const VkDevice& device,
    const VkPhysicalDevice& physicalDevice,
    const VkSurfaceKHR& surface,
    VkExtent2D windowExtent
) {
    m_device = device;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &m_capabilities);
    u32 formatCount = 0;

    VK_CHECK_RESULT(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr));
    m_formats.resize(formatCount);
    VK_CHECK_RESULT(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, m_formats.data()));
    VkSurfaceFormatKHR surfaceFormat = chooseSurfaceFormat(m_formats);

    u32 presentModeCount = 0;
    VK_CHECK_RESULT(vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr));
    std::vector<VkPresentModeKHR> presentModes(presentModeCount);
    VK_CHECK_RESULT(vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, presentModes.data()));
    VkPresentModeKHR presentMode = choosePresentMode(presentModes);

    u32 imageCount = m_capabilities.minImageCount + 1;
    if(m_capabilities.maxImageCount > 0 && imageCount > m_capabilities.maxImageCount) {
        throw std::runtime_error("image count greater than maximum image count");
    }

    if(m_capabilities.currentExtent.width != UINT32_MAX) {
        m_extent = m_capabilities.currentExtent;
    } else {
        m_extent.width = std::clamp(
            windowExtent.width,
            m_capabilities.minImageExtent.width,
            m_capabilities.maxImageExtent.width
        );
        m_extent.height = std::clamp(
            windowExtent.height,
            m_capabilities.minImageExtent.height,
            m_capabilities.maxImageExtent.height
        );
    }

    m_swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    m_swapchainCreateInfo.surface = surface;
    m_swapchainCreateInfo.minImageCount = imageCount;
    m_swapchainCreateInfo.imageFormat = surfaceFormat.format;
    m_swapchainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
    m_swapchainCreateInfo.imageExtent = m_extent;
    m_swapchainCreateInfo.imageArrayLayers = 1;
    m_swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    m_swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    m_swapchainCreateInfo.preTransform = m_capabilities.currentTransform;
    m_swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    m_swapchainCreateInfo.presentMode = presentMode;
    m_swapchainCreateInfo.clipped = VK_TRUE;
    m_swapchainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

    VK_CHECK_RESULT(vkCreateSwapchainKHR(device, &m_swapchainCreateInfo, nullptr, &m_swapchain));
    m_format = surfaceFormat.format;

    uint32_t swapchainImageCount = 0;
    VK_CHECK_RESULT(vkGetSwapchainImagesKHR(device, m_swapchain, &swapchainImageCount, nullptr));
    m_swapchainImages.resize(swapchainImageCount);
    VK_CHECK_RESULT(vkGetSwapchainImagesKHR(device, m_swapchain, &swapchainImageCount, m_swapchainImages.data()));
    m_swapchainImageViews.resize(swapchainImageCount);

    for(uint32_t i = 0; i < m_swapchainImages.size(); i++) {
        VkImageViewCreateInfo imageViewCreateInfo{};
        imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        imageViewCreateInfo.image = m_swapchainImages[i];
        imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        imageViewCreateInfo.format = surfaceFormat.format;
        imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
        imageViewCreateInfo.subresourceRange.levelCount = 1;
        imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
        imageViewCreateInfo.subresourceRange.layerCount = 1;

        VK_CHECK_RESULT(vkCreateImageView(device, &imageViewCreateInfo, nullptr, &m_swapchainImageViews[i]));
    }
}

void Swapchain::destroy() {
    for(auto swapchainImageView : m_swapchainImageViews) {
        if(swapchainImageView != VK_NULL_HANDLE) {
            vkDestroyImageView(m_device, swapchainImageView, nullptr);
        }
    }
    if(m_swapchain != VK_NULL_HANDLE) {
        vkDestroySwapchainKHR(m_device, m_swapchain, nullptr);
        m_swapchain = VK_NULL_HANDLE;
    }
}
