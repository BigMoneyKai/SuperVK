#pragma once

#include "core/define/compiler.h"
#include "core/define/types.h"

#include <functional>
#include <unordered_map>
#include <vector>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

enum class LoadOp : u8 { clear, load, dontCare };
enum class StoreOp : u8 { store, discard, readOnly };
enum class AttachmentSlot { color, depth, input };

struct RenderGraphResource {
  u32 id = 0xFFFFFFFFu;
  SV_FORCE_INLINE b32 isValid() const { return id != 0xFFFFFFFFu; };
};

struct AttachmentBinding {
  RenderGraphResource resource;
  LoadOp loadOp = LoadOp::dontCare;
  StoreOp storeOp = StoreOp::readOnly;
};

using DrawList = std::function<void(VkCommandBuffer)>;

struct PassDesc {
  std::vector<AttachmentBinding> colorAttachments;
  AttachmentBinding depthAttachment;
  std::vector<AttachmentBinding> inputAttachments;
  DrawList drawList;
};

class RenderGraph {
private:
  struct Usage {
    u32 pass;
    AttachmentSlot slot;
  };
  struct ResourceState {
    VkImage image{VK_NULL_HANDLE};
    VkImageView view{VK_NULL_HANDLE};
    VkFormat format{};
    VkImageLayout currentLayout{VK_IMAGE_LAYOUT_UNDEFINED};
    std::vector<Usage> usages;
  };
  struct Transition {
    u32 resource;
    VkImageLayout oldLayout;
    VkImageLayout newLayout;
    VkPipelineStageFlags dstStage;
    VkAccessFlags dstAccess;
  };
  struct Pass {
    const char *name{nullptr};
    PassDesc desc;
    std::vector<u32> dependencies; // 出边（writer → reader/writer）
    VkRenderPass renderPass{VK_NULL_HANDLE};
    VkFramebuffer framebuffer{VK_NULL_HANDLE};
    VkImage framebufferImage{VK_NULL_HANDLE}; // framebuffer 绑的是哪张呈现图
    std::vector<Transition> transitions;      // 进入本 pass 前要插的 barrier
    std::vector<VkClearValue> clearValues;
  };

public:
  void init(VkDevice device, VkExtent2D extent);
  void destroy();

  RenderGraphResource registerExternal(VkImage image, VkImageView view,
                                       VkFormat format,
                                       VkImageLayout initialLayout,
                                       VkImageLayout finalLayout);
  RenderGraphResource presentTarget() const;
  void setPresentTarget(VkImage image, VkImageView view, VkFormat format);
  // resize 后更新渲染尺寸（framebuffer 缓存需先 destroyFramebuffer）
  void setExtent(VkExtent2D extent);
  // 外部资源重建后（如 depth），按 id 更新 image/view
  void updateExternalResource(u32 id, VkImage image, VkImageView view,
                              VkFormat format);

  void ensureFramebuffer(Pass &pass);
  void addPass(const char *name, const PassDesc &desc);

  void process();
  void execute(VkCommandBuffer cmd);

  void destroyFramebuffer();

  VkRenderPass renderPassOf(u32 index) const;
  SV_FORCE_INLINE u32 passCount() const {
    return static_cast<u32>(m_passes.size());
  }

private:
  void addUsage(RenderGraphResource resource, u32 pass, AttachmentSlot slot);
  void addEdge(u32 from, u32 to);
  void topoSort();
  void buildRenderPass(Pass &pass);
  void buildTransitions();
  void transitionImage(VkCommandBuffer cmd, const Transition &t);
  void transitionImage(VkCommandBuffer cmd, u32 resource,
                       VkImageLayout oldLayout, VkImageLayout newLayout,
                       VkPipelineStageFlags srcStage, VkAccessFlags srcAccess,
                       VkPipelineStageFlags dstStage, VkAccessFlags dstAccess);

  SV_FORCE_INLINE VkImage imageOf(u32 id) const {
    return id == 0 ? m_presentImage : m_resources[id].image;
  }
  SV_FORCE_INLINE VkImageView viewOf(u32 id) const {
    return id == 0 ? m_presentView : m_resources[id].view;
  }
  SV_FORCE_INLINE VkFormat formatOf(u32 id) const {
    return id == 0 ? m_presentFormat : m_resources[id].format;
  }

  VkDevice m_device{VK_NULL_HANDLE};
  VkExtent2D m_extent{};
  std::vector<ResourceState> m_resources;
  std::vector<Pass> m_passes;
  std::vector<u32> m_order;
  std::unordered_map<VkImage, VkFramebuffer> m_framebufferCache;
  VkImage m_presentImage{VK_NULL_HANDLE};
  VkImageView m_presentView{VK_NULL_HANDLE};
  VkFormat m_presentFormat{VK_FORMAT_UNDEFINED};
};
