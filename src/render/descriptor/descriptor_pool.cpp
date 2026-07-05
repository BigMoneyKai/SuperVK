#include "descriptor_pool.h"
#include "debug/debugger.h"

void DescriptorPool::init(const VkDevice& device, const std::vector<VkDescriptorPoolSize>& poolSizes, u32 maxSets) {
    m_device = device;
    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<u32>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = maxSets;

    VK_CHECK_RESULT(vkCreateDescriptorPool(device, &poolInfo, nullptr, &m_pool));
}

void DescriptorPool::destroy() {
    if(m_pool != VK_NULL_HANDLE) {
        vkDestroyDescriptorPool(m_device, m_pool, nullptr);
        m_pool = VK_NULL_HANDLE;
    }
}

VkDescriptorSet DescriptorPool::allocate(const VkDescriptorSetLayout& layout) {
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = m_pool;
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = &layout;

    VkDescriptorSet descriptorSet{};
    VK_CHECK_RESULT(vkAllocateDescriptorSets(m_device, &allocInfo, &descriptorSet));
    return descriptorSet;
}
