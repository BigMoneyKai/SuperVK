#pragma once

#include "defines.h"

#include <vulkan/vulkan.h>
#include <vector>

static constexpr u32 MAX_FRAMES_IN_FLIGHT = 3;

class Synchronizer {
public:
    void init();
    void destroy(const VkDevice& device);

    SV_FORCE_INLINE VkSemaphore imageAvailableSemaphore(u32 frame) {
        return m_imageAvailableSemaphores[frame];
    }
    SV_FORCE_INLINE VkSemaphore renderFinishedSemaphore(u32 frame) {
        return m_renderFinishedSemaphores[frame];
    }
    SV_FORCE_INLINE VkFence inFlightFence(u32 frame) {
        return m_inFlightFences[frame];
    }

private:
    std::vector<VkSemaphore> m_imageAvailableSemaphores;
    std::vector<VkSemaphore> m_renderFinishedSemaphores;
    std::vector<VkFence> m_inFlightFences;

};
