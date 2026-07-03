#include "sync.h"

void Synchronizer::init(u32 swapchainImageCount) {
    m_imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    m_renderFinishedSemaphores.resize(swapchainImageCount);
    m_inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
}

void Synchronizer::destroy(const VkDevice& device) {
    for(auto imageAvailableSemaphore : m_imageAvailableSemaphores) {
        vkDestroySemaphore(device, imageAvailableSemaphore, nullptr);
    }
    for(auto renderFinishedSemaphore : m_renderFinishedSemaphores) {
        vkDestroySemaphore(device, renderFinishedSemaphore, nullptr);
    }
    for(auto inFlightFence : m_inFlightFences) {
        vkDestroyFence(device, inFlightFence, nullptr);
    }
    m_imageAvailableSemaphores.clear();
    m_renderFinishedSemaphores.clear();
    m_inFlightFences.clear();
}
