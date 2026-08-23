#include "render_graph.h"
#include "core/debug/debugger.h"
#include <algorithm>
#include <queue>
#include <vulkan/vulkan_core.h>

namespace {
VkImageLayout slotLayout(AttachmentSlot slot) {
  switch (slot) {
  case AttachmentSlot::color:
    return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
  case AttachmentSlot::depth:
    return VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
  case AttachmentSlot::input:
    return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
  }
  return VK_IMAGE_LAYOUT_UNDEFINED;
}

VkPipelineStageFlags slotStage(AttachmentSlot slot) {
  switch (slot) {
  case AttachmentSlot::color:
    return VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  case AttachmentSlot::depth:
    return VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT |
           VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
  case AttachmentSlot::input:
    return VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
  }
  return VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
}

VkAccessFlags slotAccess(AttachmentSlot slot) {
  switch (slot) {
  case AttachmentSlot::color:
    return VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
  case AttachmentSlot::depth:
    return VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
  case AttachmentSlot::input:
    return VK_ACCESS_INPUT_ATTACHMENT_READ_BIT;
  }
  return VK_ACCESS_NONE;
}

VkAttachmentLoadOp toVkLoad(LoadOp loadOp) {
  switch (loadOp) {
  case LoadOp::clear:
    return VK_ATTACHMENT_LOAD_OP_CLEAR;
  case LoadOp::load:
    return VK_ATTACHMENT_LOAD_OP_LOAD;
  case LoadOp::dontCare:
    return VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  }
  return VK_ATTACHMENT_LOAD_OP_NONE;
}

VkAttachmentStoreOp toVkStore(StoreOp storeOp) {
  switch (storeOp) {
  case StoreOp::discard:
    return VK_ATTACHMENT_STORE_OP_DONT_CARE;
  case StoreOp::store:
    return VK_ATTACHMENT_STORE_OP_STORE;
  case StoreOp::readOnly:
    return VK_ATTACHMENT_STORE_OP_STORE;
  }
  return VK_ATTACHMENT_STORE_OP_NONE;
}

VkImageAspectFlags aspectFor(VkFormat format) {
  switch (format) {
  case VK_FORMAT_D16_UNORM:
  case VK_FORMAT_D32_SFLOAT:
    return VK_IMAGE_ASPECT_DEPTH_BIT;
  case VK_FORMAT_D16_UNORM_S8_UINT:
  case VK_FORMAT_D32_SFLOAT_S8_UINT:
    return VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
  default:
    return VK_IMAGE_ASPECT_COLOR_BIT;
  }
}

} // namespace

void RenderGraph::init(VkDevice device, VkExtent2D extent) {
  m_device = device;
  m_extent = extent;
  m_resources.clear();
  m_resources.push_back({}); // 0 号位：呈现目标（句柄固定为 0）
}

void RenderGraph::destroy() {
  for (auto &pass : m_passes) {
    if (pass.renderPass != VK_NULL_HANDLE) {
      vkDestroyRenderPass(m_device, pass.renderPass, nullptr);
      pass.renderPass = VK_NULL_HANDLE;
    }
  }
  for (auto& it : m_framebufferCache) {
    if (it.second != VK_NULL_HANDLE) {
      vkDestroyFramebuffer(m_device, it.second, nullptr);
    }
  }

  m_passes.clear();
  m_resources.clear();
  m_order.clear();
  m_presentImage = VK_NULL_HANDLE;
  m_presentView = VK_NULL_HANDLE;
  m_presentFormat = VK_FORMAT_UNDEFINED;
}

RenderGraphResource RenderGraph::registerExternal(VkImage image,
                                                  VkImageView view,
                                                  VkFormat format,
                                                  VkImageLayout initialLayout,
                                                  VkImageLayout finalLayout) {
  ResourceState state{};
  state.image = image;
  state.view = view;
  state.format = format;
  state.currentLayout = initialLayout;
  m_resources.push_back(state);
  return RenderGraphResource{static_cast<u32>(m_resources.size() - 1)};
}

RenderGraphResource RenderGraph::presentTarget() const {
  return RenderGraphResource{0};
}

void RenderGraph::setPresentTarget(VkImage image, VkImageView view,
                                   VkFormat format) {
  m_presentFormat = format;
  m_presentImage = image;
  m_presentView = view;
}

void RenderGraph::addPass(const char *name, const PassDesc &desc) {
  m_passes.push_back({name, desc});
}

void RenderGraph::process() {
  for (auto &r : m_resources)
    r.usages.clear();
  m_resources[0].currentLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  for (auto &p : m_passes)
    p.dependencies.clear();

  for (u32 p = 0; p < m_passes.size(); ++p) {
    const auto &cfg = m_passes[p].desc;
    for (const auto &a : cfg.colorAttachments)
      addUsage(a.resource, p, AttachmentSlot::color);
    if (cfg.depthAttachment.resource.isValid())
      addUsage(cfg.depthAttachment.resource, p, AttachmentSlot::depth);
    for (const auto &a : cfg.inputAttachments)
      addUsage(a.resource, p, AttachmentSlot::input);
  }

  // write->read, write->write
  for (auto &r : m_resources) {
    const Usage *lastWrite = nullptr;
    for (const auto &u : r.usages) {
      if (u.slot != AttachmentSlot::input || lastWrite != nullptr) {
        if (lastWrite)
          addEdge(lastWrite->pass, u.pass);
      }
      if (u.slot != AttachmentSlot::input)
        lastWrite = &u;
    }
  }

  // 3) 拓扑排序 → 推导 barrier → 生成 render pass（render pass 只建一次，缓存）
  topoSort();
  buildTransitions();
  for (auto &p : m_passes)
    if (p.renderPass == VK_NULL_HANDLE)
      buildRenderPass(p);
}

void RenderGraph::execute(VkCommandBuffer cmd) {
  for (u32 idx : m_order) {
    Pass &pass = m_passes[idx];

    // 进入本 pass 前：先完成所有资源过渡
    for (const auto &t : pass.transitions) {
      transitionImage(cmd, t);
      m_resources[t.resource].currentLayout = t.newLayout;
    }

    ensureFramebuffer(pass);

    VkRenderPassBeginInfo rpbi{};
    rpbi.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    rpbi.renderPass = pass.renderPass;
    rpbi.framebuffer = pass.framebuffer;
    rpbi.renderArea.offset = {0, 0};
    rpbi.renderArea.extent = m_extent;
    rpbi.clearValueCount = static_cast<u32>(pass.clearValues.size());
    rpbi.pClearValues = pass.clearValues.data();

    vkCmdBeginRenderPass(cmd, &rpbi, VK_SUBPASS_CONTENTS_INLINE);
    if (pass.desc.drawList)
      pass.desc.drawList(cmd);
    vkCmdEndRenderPass(cmd);
  }

  // 全部 pass 结束后：呈现目标切到 PRESENT，交给交换链
  auto &present = m_resources[0];
  if (present.currentLayout != VK_IMAGE_LAYOUT_PRESENT_SRC_KHR) {
    transitionImage(cmd, 0, present.currentLayout,
                    VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
                    VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                    VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
                    VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, VK_ACCESS_NONE);
    present.currentLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
  }
}

VkRenderPass RenderGraph::renderPassOf(u32 index) const {
  return m_passes[index].renderPass;
}

void RenderGraph::addUsage(RenderGraphResource resource, u32 pass,
                           AttachmentSlot slot) {
  m_resources[resource.id].usages.push_back({pass, slot});
}

void RenderGraph::addEdge(u32 from, u32 to) {
  return m_passes[from].dependencies.push_back(to);
}

void RenderGraph::topoSort() {
  std::vector<u32> indegree(m_passes.size(), 0);
  for (const auto &pass : m_passes)
    for (u32 to : pass.dependencies)
      ++indegree[to];

  std::priority_queue<u32, std::vector<u32>, std::greater<u32>> ready;
  for (u32 i = 0; i < m_passes.size(); ++i)
    if (indegree[i] == 0)
      ready.push(i);

  m_order.clear();
  while (!ready.empty()) {
    u32 p = ready.top();
    ready.pop();
    m_order.push_back(p);
    for (u32 to : m_passes[p].dependencies)
      if (--indegree[to] == 0)
        ready.push(to);
  }
  if (m_order.size() != m_passes.size())
    FATAL(LogCatag::render, "Render graph cycle detected");
}
void RenderGraph::buildTransitions() {
  for (auto &p : m_passes)
    p.transitions.clear();

  // 每个 pass 在拓扑序里的名次，用于把资源的用法排成"实际执行顺序"
  std::vector<u32> rank(m_passes.size());
  for (u32 i = 0; i < m_order.size(); ++i)
    rank[m_order[i]] = i;

  for (u32 id = 0; id < m_resources.size(); ++id) {
    auto &r = m_resources[id];
    if (r.usages.empty())
      continue;

    std::vector<Usage> usages = r.usages;
    std::stable_sort(usages.begin(), usages.end(),
                     [&rank](const Usage &a, const Usage &b) {
                       return rank[a.pass] < rank[b.pass];
                     });

    VkImageLayout prevLayout = r.currentLayout;
    bool prevWrite = false;
    for (const auto &u : usages) {
      VkImageLayout target = slotLayout(u.slot);
      if (prevWrite || prevLayout != target) {
        m_passes[u.pass].transitions.push_back(Transition{
            id, prevLayout, target, slotStage(u.slot), slotAccess(u.slot)});
      }
      prevLayout = target;
      prevWrite = (u.slot != AttachmentSlot::input);
    }
  }
}

void RenderGraph::buildRenderPass(Pass &pass) {
  const auto &cfg = pass.desc;
  if (cfg.colorAttachments.empty() && !cfg.depthAttachment.resource.isValid())
    FATAL(LogCatag::render, "Pass '%s' has no attachments", pass.name);

  std::vector<VkAttachmentDescription> descs;
  std::vector<VkAttachmentReference> colorRefs;
  std::vector<VkAttachmentReference> inputRefs;
  VkAttachmentReference depthRef{};
  bool hasDepth = cfg.depthAttachment.resource.isValid();

  // color -> depth -> input
  u32 index = 0;
  for (const auto &a : cfg.colorAttachments) {
    VkAttachmentDescription d{};
    d.format = formatOf(a.resource.id);
    d.samples = VK_SAMPLE_COUNT_1_BIT;
    d.loadOp = toVkLoad(a.loadOp);
    d.storeOp = toVkStore(a.storeOp);
    d.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    d.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    d.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    d.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    descs.push_back(d);
    colorRefs.push_back({index++, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL});
  }
  if (hasDepth) {
    VkAttachmentDescription d{};
    d.format = formatOf(cfg.depthAttachment.resource.id);
    d.samples = VK_SAMPLE_COUNT_1_BIT;
    d.loadOp = toVkLoad(cfg.depthAttachment.loadOp);
    d.storeOp = toVkStore(cfg.depthAttachment.storeOp);
    d.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    d.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    d.initialLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    d.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    descs.push_back(d);
    depthRef = {index++, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL};
  }
  for (const auto &a : cfg.inputAttachments) {
    VkAttachmentDescription d{};
    d.format = formatOf(a.resource.id);
    d.samples = VK_SAMPLE_COUNT_1_BIT;
    d.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
    d.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    d.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    d.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    d.initialLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    d.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    descs.push_back(d);
    inputRefs.push_back({index++, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL});
  }

  VkSubpassDescription subpass{};
  subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
  subpass.colorAttachmentCount = static_cast<u32>(colorRefs.size());
  subpass.pColorAttachments = colorRefs.data();
  subpass.pDepthStencilAttachment = hasDepth ? &depthRef : nullptr;
  subpass.inputAttachmentCount = static_cast<u32>(inputRefs.size());
  subpass.pInputAttachments = inputRefs.data();

  VkSubpassDependency deps[2]{};
  deps[0].srcSubpass = VK_SUBPASS_EXTERNAL;
  deps[0].dstSubpass = 0;
  deps[0].srcStageMask = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
  deps[0].srcAccessMask = VK_ACCESS_NONE;
  deps[1].srcSubpass = 0;
  deps[1].dstSubpass = VK_SUBPASS_EXTERNAL;
  deps[1].dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
  deps[1].dstAccessMask = VK_ACCESS_NONE;
  for (const auto &a : cfg.colorAttachments) {
    deps[0].dstStageMask |= VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    deps[0].dstAccessMask |= VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    deps[1].srcStageMask |= VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    deps[1].srcAccessMask |= VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
  }
  if (hasDepth) {
    deps[0].dstStageMask |= VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT |
                            VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
    deps[0].dstAccessMask |= VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    deps[1].srcStageMask |= VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT |
                            VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
    deps[1].srcAccessMask |= VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
  }
  for (const auto &r : cfg.inputAttachments) {
    deps[0].dstStageMask |= VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    deps[0].dstAccessMask |= VK_ACCESS_INPUT_ATTACHMENT_READ_BIT;
    deps[1].srcStageMask |= VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    deps[1].srcAccessMask |= VK_ACCESS_INPUT_ATTACHMENT_READ_BIT;
  }

  VkRenderPassCreateInfo ci{};
  ci.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  ci.attachmentCount = static_cast<u32>(descs.size());
  ci.pAttachments = descs.data();
  ci.subpassCount = 1;
  ci.pSubpasses = &subpass;
  ci.dependencyCount = 2;
  ci.pDependencies = deps;
  VK_CHECK_RESULT(vkCreateRenderPass(m_device, &ci, nullptr, &pass.renderPass));

  pass.clearValues.resize(descs.size());
  u32 ci2 = 0;
  for (const auto &a : cfg.colorAttachments)
    pass.clearValues[ci2++] =
        a.loadOp == LoadOp::clear
            ? VkClearValue{.color = {{0.0f, 0.0f, 0.0f, 1.0f}}}
            : VkClearValue{.color = {{0, 0, 0, 0}}};
  if (hasDepth)
    pass.clearValues[ci2++] = cfg.depthAttachment.loadOp == LoadOp::clear
                                  ? VkClearValue{.depthStencil = {1.0f, 0}}
                                  : VkClearValue{.depthStencil = {0, 0}};
}

void RenderGraph::ensureFramebuffer(Pass &pass) {
  if (pass.framebuffer != VK_NULL_HANDLE &&
      pass.framebufferImage == m_presentImage)
    return;

  std::vector<VkImageView> views;
  for (const auto &a : pass.desc.colorAttachments)
    views.push_back(viewOf(a.resource.id));
  if (pass.desc.depthAttachment.resource.isValid())
    views.push_back(viewOf(pass.desc.depthAttachment.resource.id));
  for (const auto &a : pass.desc.inputAttachments)
    views.push_back(viewOf(a.resource.id));

  // Use the current present image as the key for framebuffer cache
  auto it = m_framebufferCache.find(m_presentImage);
  if (it == m_framebufferCache.end()) {
    VkFramebuffer fb = VK_NULL_HANDLE;
    VkFramebufferCreateInfo ci{};
    ci.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    ci.renderPass = pass.renderPass;
    ci.attachmentCount = static_cast<u32>(views.size());
    ci.pAttachments = views.data();
    ci.width = m_extent.width;
    ci.height = m_extent.height;
    ci.layers = 1;
    VK_CHECK_RESULT(vkCreateFramebuffer(m_device, &ci, nullptr, &fb));
    m_framebufferCache.insert({m_presentImage, fb});
    pass.framebuffer = fb;
  } else {
    pass.framebuffer = it->second;
  }
  pass.framebufferImage = m_presentImage;
}

void RenderGraph::transitionImage(VkCommandBuffer cmd, const Transition &t) {
  transitionImage(cmd, t.resource, t.oldLayout, t.newLayout,
                  VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
                  VK_ACCESS_MEMORY_WRITE_BIT, t.dstStage, t.dstAccess);
}

void RenderGraph::transitionImage(VkCommandBuffer cmd, u32 resource,
                                  VkImageLayout oldLayout,
                                  VkImageLayout newLayout,
                                  VkPipelineStageFlags srcStage,
                                  VkAccessFlags srcAccess,
                                  VkPipelineStageFlags dstStage,
                                  VkAccessFlags dstAccess) {
  VkImageMemoryBarrier barrier{};
  barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
  barrier.oldLayout = oldLayout;
  barrier.newLayout = newLayout;
  barrier.srcAccessMask = srcAccess;
  barrier.dstAccessMask = dstAccess;
  barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  barrier.image = imageOf(resource);
  barrier.subresourceRange = {aspectFor(formatOf(resource)), 0, 1, 0, 1};

  vkCmdPipelineBarrier(cmd, srcStage, dstStage, 0, 0, nullptr, 0, nullptr, 1,
                       &barrier);
}
