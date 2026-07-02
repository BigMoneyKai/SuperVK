#pragma once

#include "defines.h"

#include <vulkan/vulkan.h>
#include <vector>

class FrameResource {
public:
    void init();
    void createSyncPrimitives(const VkDevice& device);
    void destroy(const VkDevice& device);

    SV_FORCE_INLINE u32 framebufferCount() const {
        return m_framebufferCount;
    }
    SV_FORCE_INLINE VkSemaphore imageAvailableSemaphore(u32 index) const {
        return m_imageAvailableSemaphores.at(index);
    }
    SV_FORCE_INLINE VkSemaphore renderFinishedSemaphore(u32 index) const {
        return m_renderFinishedSemaphores.at(index);
    }
    SV_FORCE_INLINE VkFence inFlightFence(u32 index) const {
        return m_inFlightFences.at(index);
    }

private:
    u32 m_framebufferCount{3};
    std::vector<VkSemaphore> m_imageAvailableSemaphores;
    std::vector<VkSemaphore> m_renderFinishedSemaphores;
    std::vector<VkFence> m_inFlightFences;
};
