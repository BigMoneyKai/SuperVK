#pragma once

#include "defines.h"

#include "base/command_buffer.h"
#include "base/command_pool.h"
#include "base/device.h"
#include "base/instance.h"
#include "base/render_pass.h"
#include "base/surface.h"
#include "base/swapchain.h"

#include "descriptor/descriptor_manager.h"

#include "pipeline/pipeline_manager.h"

#include "frame/depth_resource.h"
#include "frame/frame_resource.h"
#include "frame/framebuffer.h"
#include "frame/sync.h"

#include "resource/buffer_manager.h"
#include "resource/texture_manager.h"

#include "window/window_manager.h"

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include <vector>

// Forward declaration
class Scene;

struct RendererDesc {
    WinMan* pWinMan{nullptr};
};

class Renderer {
public:
    void init(const RendererDesc& desc);
    void render(Scene& scene);
    void waitIdle();
    void destroy();

    SV_FORCE_INLINE const Device& device() const {
        return m_device;
    }
    SV_FORCE_INLINE Device& device() {
        return m_device;
    }

private:
    void drawFrame(Scene& scene);

private:
    // ------ target class members ------
    Instance m_instance;
    Device m_device;
    Surface m_surface;
    Swapchain m_swapchain;
    RenderPass m_renderPass;

    CommandPool m_commandPool;
    CommandBuffer m_commandBuffer;

    PipelineMan m_pipelineMan;
    DescriptorMan m_descriptorMan;
    BufferMan m_bufferMan;
    TextureMan m_textureMan;

    FrameResource m_frameResource;
    DepthResource m_depthResource;
    std::vector<Framebuffer> m_framebuffers;

    RendererDesc m_desc;
    u32 m_currFrame{0};
    u32 m_imageIndex{0};
};
