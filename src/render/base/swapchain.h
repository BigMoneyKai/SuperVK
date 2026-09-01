#pragma once

#include "core/define/compiler.h"
#include "core/define/types.h"

#include <vector>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

class Swapchain {
public:
  void init(const VkDevice &device, const VkPhysicalDevice &physicalDevice,
            const VkSurfaceKHR &surface, VkExtent2D windowExtent);
  void destroy();

  SV_FORCE_INLINE const std::vector<VkSurfaceFormatKHR> &formats() const {
    return m_formats;
  }
  SV_FORCE_INLINE VkFormat colorFormat() const { return m_format; }
  SV_FORCE_INLINE const VkSwapchainKHR &swapchain() const {
    return m_swapchain;
  }
  SV_FORCE_INLINE const VkExtent2D &extent() const { return m_extent; }
  SV_FORCE_INLINE const std::vector<VkImage> &swapchainImages() const {
    return m_swapchainImages;
  }
  SV_FORCE_INLINE const std::vector<VkImageView> &swapchainImageViews() const {
    return m_swapchainImageViews;
  }
  SV_FORCE_INLINE u32 imageCount() const {
    return static_cast<u32>(m_swapchainImages.size());
  }

  static constexpr VkExtent2D defaultExtent = {};

private:
  VkDevice m_device{VK_NULL_HANDLE};
  VkSwapchainKHR m_swapchain;
  VkSwapchainCreateInfoKHR m_swapchainCreateInfo{};
  VkSurfaceCapabilitiesKHR m_capabilities;
  VkExtent2D m_extent;
  VkFormat m_format{VK_FORMAT_UNDEFINED};
  std::vector<VkSurfaceFormatKHR> m_formats;
  std::vector<VkImage> m_swapchainImages;
  std::vector<VkImageView> m_swapchainImageViews;
};
