#pragma once

#include "core/define/compiler.h"
#include "core/define/types.h"

#include <vulkan/vulkan.h>

namespace image {
constexpr u32 defaultWidth = 0;
constexpr u32 defaultHeight = 0;
constexpr u32 defaultDepth = 1;
constexpr VkImageType defaultImageType = VK_IMAGE_TYPE_2D;
constexpr VkFormat defaultFormat = VK_FORMAT_R8G8B8A8_SRGB;
constexpr VkImageUsageFlags defaultUsage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT |
                                           VK_IMAGE_USAGE_TRANSFER_DST_BIT |
                                           VK_IMAGE_USAGE_SAMPLED_BIT;
constexpr u32 defaultMipLevels = 1;
constexpr u32 defaultArrayLayers = 1;
constexpr VkSampleCountFlagBits defaultSamples = VK_SAMPLE_COUNT_1_BIT;
constexpr VkImageTiling defaultTiling = VK_IMAGE_TILING_OPTIMAL;

struct ImageDesc {
  u32 width{defaultWidth};
  u32 height{defaultHeight};
  u32 depth{defaultDepth};
  VkImageType imageType{defaultImageType};
  VkFormat format{defaultFormat};
  VkImageUsageFlags usage{defaultUsage};
  u32 mipLevels{defaultMipLevels};
  u32 arrayLayers{defaultArrayLayers};
  VkSampleCountFlagBits samples{defaultSamples};
  VkImageTiling tiling{defaultTiling};
  u32 queueFamilyIndexCount{0};
};

class Image {
public:
  void init(const VkDevice &device, const VkPhysicalDevice &physicalDevice,
            const VkCommandPool &commandPool, const VkQueue &graphicsQueue,
            const ImageDesc &desc);
  void transitionLayout(VkImageLayout newLayout);
  void destroy();

  SV_FORCE_INLINE const VkImage &image() const { return m_image; }
  SV_FORCE_INLINE VkImage &image() { return m_image; }
  SV_FORCE_INLINE const VkImageView &imageView() const { return m_imageView; }
  SV_FORCE_INLINE VkImageView &imageView() { return m_imageView; }
  SV_FORCE_INLINE const VkFormat &format() const { return m_format; }
  SV_FORCE_INLINE VkFormat &format() { return m_format; }
  SV_FORCE_INLINE const VkExtent3D &extent() const { return m_extent; }
  SV_FORCE_INLINE VkExtent3D &extent() { return m_extent; }

  SV_FORCE_INLINE const VkImageLayout &layout() const { return m_layout; }
  SV_FORCE_INLINE VkImageLayout &layout() { return m_layout; }

private:
  VkDevice m_device{VK_NULL_HANDLE};
  VkCommandPool m_commandPool{VK_NULL_HANDLE};
  VkQueue m_graphicsQueue{VK_NULL_HANDLE};
  VkImage m_image{VK_NULL_HANDLE};
  VkImageView m_imageView{VK_NULL_HANDLE};
  VkDeviceMemory m_memory{VK_NULL_HANDLE};
  VkFormat m_format{VK_FORMAT_UNDEFINED};
  VkExtent3D m_extent{};
  VkImageLayout m_layout{VK_IMAGE_LAYOUT_UNDEFINED};
};
} // namespace image

using Image = image::Image;
using ImageDesc = image::ImageDesc;
