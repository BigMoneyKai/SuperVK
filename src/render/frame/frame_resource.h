#pragma once

#include "defines.h"

#include <vulkan/vulkan.h>
#include <vector>

#include "sync.h"

class FrameResource {
public:
    void init(u32 swapchainImageCount);
    void createSyncPrimitives(const VkDevice& device);
    void destroy(const VkDevice& device);

    SV_FORCE_INLINE u32 framebufferCount() const {
        return m_framebufferCount;
    }
    SV_FORCE_INLINE VkSemaphore imageAvailableSemaphore(u32 frame) const {
        return m_imageAvailableSemaphores.at(frame);
    }
    SV_FORCE_INLINE VkSemaphore renderFinishedSemaphore(u32 imageIndex) const {
        return m_renderFinishedSemaphores.at(imageIndex);
    }
    SV_FORCE_INLINE VkFence inFlightFence(u32 frame) const {
        return m_inFlightFences.at(frame);
    }

private:
    u32 m_framebufferCount{3};
    u32 m_swapchainImageCount{0};
    std::vector<VkSemaphore> m_imageAvailableSemaphores;
    std::vector<VkSemaphore> m_renderFinishedSemaphores;
    std::vector<VkFence> m_inFlightFences;
};
