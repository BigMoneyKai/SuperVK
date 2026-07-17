#include "frame_resource.h"
#include "core/debug/debugger.h"
#include "render/base/device.h"

void FrameResource::init(const VkDevice& device, u32 swapchainImageCount) {
    m_device = device;
    m_swapchainImageCount = swapchainImageCount;
    m_imageAvailableSemaphores.resize(m_framebufferCount);
    m_renderFinishedSemaphores.resize(m_swapchainImageCount);
    m_inFlightFences.resize(m_framebufferCount);
}

void FrameResource::createSyncPrimitives() {
    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for(u32 i = 0; i < m_framebufferCount; ++i) {
        if(vkCreateSemaphore(m_device, &semaphoreInfo, nullptr, &m_imageAvailableSemaphores[i]) != VK_SUCCESS) {
            FATAL("Failed to create image available semaphore");
        }

        if(vkCreateFence(m_device, &fenceInfo, nullptr, &m_inFlightFences[i]) != VK_SUCCESS) {
            FATAL("Failed to create in-flight fence");
        }
    }
    for(u32 i = 0; i < m_swapchainImageCount; ++i) {
        if(vkCreateSemaphore(m_device, &semaphoreInfo, nullptr, &m_renderFinishedSemaphores[i]) != VK_SUCCESS) {
            FATAL("Failed to create render finished semaphore");
        }
    }
}

void FrameResource::destroy() {
    for(auto semaphore : m_imageAvailableSemaphores) {
        if(semaphore != VK_NULL_HANDLE) {
            vkDestroySemaphore(m_device, semaphore, nullptr);
        }
    }
    for(auto semaphore : m_renderFinishedSemaphores) {
        if(semaphore != VK_NULL_HANDLE) {
            vkDestroySemaphore(m_device, semaphore, nullptr);
        }
    }
    for(auto fence : m_inFlightFences) {
        if(fence != VK_NULL_HANDLE) {
            vkDestroyFence(m_device, fence, nullptr);
        }
    }
    m_imageAvailableSemaphores.clear();
    m_renderFinishedSemaphores.clear();
    m_inFlightFences.clear();
}
