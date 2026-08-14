#pragma once

#include <vector>
#include <vulkan/vulkan.h>

#include "sync.h"

class FrameResource {
public:
  void init(const VkDevice &device, u32 swapchainImageCount);
  void createSyncPrimitives();
  void destroy();

  SV_FORCE_INLINE u32 framebufferCount() const { return m_framebufferCount; }
  SV_FORCE_INLINE VkSemaphore imageAvailableSemaphore(u32 frame) const {
    return m_imageAvailableSemaphores.at(frame);
  }
  SV_FORCE_INLINE VkSemaphore renderFinishedSemaphore(u32 imageIndex) const {
    return m_renderFinishedSemaphores.at(imageIndex);
  }
  SV_FORCE_INLINE VkFence inFlightFence(u32 frame) const {
    return m_inFlightFences.at(frame);
  }
  SV_FORCE_INLINE VkFence imageInFlight(u32 imageIndex) const {
    return m_imagesInFlight.at(imageIndex);
  }
  SV_FORCE_INLINE void setImageInFlight(u32 imageIndex, VkFence fence) {
    m_imagesInFlight.at(imageIndex) = fence;
  }

private:
  VkDevice m_device{VK_NULL_HANDLE};
  u32 m_framebufferCount{MAX_FRAMES_IN_FLIGHT};
  u32 m_swapchainImageCount{0};
  std::vector<VkSemaphore> m_imageAvailableSemaphores;
  std::vector<VkSemaphore> m_renderFinishedSemaphores;
  std::vector<VkFence> m_inFlightFences;
  std::vector<VkFence> m_imagesInFlight;
};
