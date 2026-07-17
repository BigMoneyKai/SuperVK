#include "command_buffer.h"
#include "core/debug/debugger.h"

void CommandBuffer::init(const VkDevice& device, VkCommandPool pool, u32 count) {
    m_device = device;
    m_pool = pool;
    m_commandBuffers.resize(count);

    m_commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    m_commandBufferAllocateInfo.commandPool = pool;
    m_commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    m_commandBufferAllocateInfo.commandBufferCount = count;

    VK_CHECK_RESULT(vkAllocateCommandBuffers(
        device,
        &m_commandBufferAllocateInfo,
        m_commandBuffers.data()
    ));
}

void CommandBuffer::destroy() {
    if(!m_commandBuffers.empty() && m_pool != VK_NULL_HANDLE) {
        vkFreeCommandBuffers(
            m_device,
            m_pool,
            static_cast<u32>(m_commandBuffers.size()),
            m_commandBuffers.data()
        );
    }
    m_commandBuffers.clear();
    m_pool = VK_NULL_HANDLE;
}
