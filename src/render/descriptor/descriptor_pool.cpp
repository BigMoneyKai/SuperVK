#include "descriptor_pool.h"
#include "debug/debugger.h"

void DescriptorPool::init(const VkDevice& device, const std::vector<VkDescriptorPoolSize>& poolSizes, u32 maxSets) {
    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<u32>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = maxSets;

    VK_CHECK_RESULT(vkCreateDescriptorPool(device, &poolInfo, nullptr, &m_pool));
}

void DescriptorPool::destroy(const VkDevice& device) {
    if(m_pool != VK_NULL_HANDLE) {
        vkDestroyDescriptorPool(device, m_pool, nullptr);
        m_pool = VK_NULL_HANDLE;
    }
}

VkDescriptorSet DescriptorPool::allocate(const VkDevice& device, const VkDescriptorSetLayout& layout) {
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = m_pool;
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = &layout;

    VkDescriptorSet descriptorSet{};
    VK_CHECK_RESULT(vkAllocateDescriptorSets(device, &allocInfo, &descriptorSet));
    return descriptorSet;
}
