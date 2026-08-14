#pragma once

#include "buffer.h"
#include "core/define/compiler.h"
#include "image.h"
#include "sampler.h"

#include <stb_image.h>
#include <vulkan/vulkan.h>

class Texture {
public:
  void init(const VkDevice &device, const VkPhysicalDevice &physicalDevice,
            const VkCommandPool &commandPool, const VkQueue &graphicsQueue,
            const char *path);
  void destroy();

  SV_FORCE_INLINE const VkImage &image() const { return m_image.image(); }
  SV_FORCE_INLINE VkImage &image() { return m_image.image(); }

  SV_FORCE_INLINE const VkImageView &imageView() const {
    return m_image.imageView();
  }
  SV_FORCE_INLINE VkImageView &imageView() { return m_image.imageView(); }

  SV_FORCE_INLINE const VkImageLayout &imageLayout() const {
    return m_image.layout();
  }
  SV_FORCE_INLINE VkImageLayout &imageLayout() { return m_image.layout(); }

  SV_FORCE_INLINE const VkSampler &sampler() const {
    return m_sampler.sampler();
  }
  SV_FORCE_INLINE VkSampler &sampler() { return m_sampler.sampler(); }

private:
  void copyBufferToImage(const VkExtent3D &extent);

private:
  stbi_uc *m_pixels{nullptr};
  Image m_image{};
  Sampler m_sampler{};

  Buffer m_stagingBuffer{};

  VkDevice m_device{VK_NULL_HANDLE};
  VkPhysicalDevice m_physicalDevice{VK_NULL_HANDLE};
  VkCommandPool m_commandPool{VK_NULL_HANDLE};
  VkQueue m_graphicsQueue{VK_NULL_HANDLE};
};
