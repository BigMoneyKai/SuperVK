#include "app/app.h"
#include "GLFW/glfw3.h"
#include "core/debug/debugger.h"
#include "core/define/types.h"
#include "platform/time.h"
#include "scene/camera.h"
#include "ui/manager.h"

#include <algorithm>
#include <thirdparty/imgui/imgui.h>
#include <vulkan/vulkan_core.h>

namespace App {

static b32 waitForMeshLoad(JobSystem &js, AssetMan &am,
                           AssetMan::MeshHandle handle) {
  while (true) {
    js.wait_all();
    am.update();
    if (am.isValidMesh(handle) && am.mesh(handle).state == AssetState::loaded)
      return true;
    if (am.isValidMesh(handle) && am.mesh(handle).state == AssetState::failed)
      return false;
  }
}

void App::init(const char *title, DisplayMode mode, u64 threadCount) {
  m_jobSystem.init(static_cast<u32>(threadCount));
  m_winMan.init(title, mode);
  m_inputMan.init(m_winMan.window());
  m_renderer.init({&m_winMan});

  UI::UIInitInfo uiInitInfo{};
  uiInitInfo.window = m_winMan.window();
  uiInitInfo.instance = m_renderer.instance();
  uiInitInfo.physicalDevice = m_renderer.device().physicalDevice();
  uiInitInfo.device = m_renderer.device().device();
  uiInitInfo.queueFamily = m_renderer.device().graphicsQueueFamilyIndex();
  uiInitInfo.queue = m_renderer.device().graphicsQueue();
  uiInitInfo.renderPass = m_renderer.renderPass();
  uiInitInfo.imageCount = m_renderer.imageCount();
  m_uiMan.init(uiInitInfo);

  m_renderer.setUiRenderHook(
      [](VkCommandBuffer cmd, void *userData) {
        static_cast<UI::Man *>(userData)->update(cmd);
      },
      &m_uiMan);

  m_assetMan.init(&m_jobSystem);

  m_meshHandle = m_assetMan.loadMesh(
    "resource/AlphaBlendModeTest/glTF-Binary/AlphaBlendModeTest.glb");

  if (!waitForMeshLoad(m_jobSystem, m_assetMan, m_meshHandle)) {
    FATAL(LogCatag::asset, "App: failed to load initial mesh");
  }

  mesh::Mesh &src = m_assetMan.mesh(m_meshHandle).mesh;
  mesh::Mesh &dst = m_scene.mesh();
  dst.vertices() = std::move(src.vertices());
  dst.indices() = std::move(src.indices());
  dst.positions() = std::move(src.positions());
  dst.texcoords() = std::move(src.texcoords());
  dst.normals() = std::move(src.normals());

  m_scene.init(m_renderer.device().device(),
               m_renderer.device().physicalDevice(), m_winMan.width(),
               m_winMan.height());
}

void App::run() {
  f64 last = timer_now_ms();
  while (!glfwWindowShouldClose(m_winMan.window())) {
    glfwPollEvents();
    const f64 now = timer_now_ms();
    const f32 dt =
      std::clamp(static_cast<f32>((now - last) * 0.001), 0.0f, 0.05f);
    last = now;

    update(dt);
    m_assetMan.update();

    // 把 UI 的 Scene 视口矩形换算成 framebuffer 像素，交给渲染器
    const ImRect vp = m_uiMan.viewportRect();
    // 唯一基准：swapchain extent（renderer 的坐标空间），不是 glfw framebuffer
    const VkExtent2D ext = m_renderer.swapchainExtent();
    const ImVec2 ds = ImGui::GetIO().DisplaySize;
    const f32 scaleX = ds.x > 0.f ? static_cast<f32>(ext.width) / ds.x : 1.f;
    const f32 scaleY = ds.y > 0.f ? static_cast<f32>(ext.height) / ds.y : 1.f;
    m_renderer.setViewportRect(
      static_cast<i32>(vp.Min.x * scaleX), static_cast<i32>(vp.Min.y * scaleY),
      static_cast<i32>((vp.Min.x + vp.GetWidth()) * scaleX),
      static_cast<i32>((vp.Min.y + vp.GetHeight()) * scaleY));

    m_renderer.render(m_scene);
  }
}

void App::destroy() {
  m_renderer.waitIdle();

  m_scene.destroy();
  m_uiMan.destroy();
  m_renderer.destroy();
  m_assetMan.destroy();
  m_jobSystem.destroy();
  m_inputMan.destroy();
  m_winMan.destroy();
}

void App::update(f32 dt) {
  m_uiMan.newFrame();
  m_inputMan.newFrame();

  CameraInput camInput;
  Mouse &mouse = m_inputMan.getMouse();
  Keyboard &kb = m_inputMan.getKeyboard();

  camInput.deltaTime = dt;
  if (!m_uiMan.captureMouseNeeded()) {
    camInput.mouseDelta = mouse.delta();
    camInput.scrollWheel = mouse.wheel();
    camInput.OrbitEnabled = mouse.isDown(Button::button2);
  }
  camInput.moveForward = kb.isDown(Key::keyW);
  camInput.moveBackward = kb.isDown(Key::keyS);
  camInput.moveLeft = kb.isDown(Key::keyA);
  camInput.moveRight = kb.isDown(Key::keyD);
  camInput.moveUp = kb.isDown(Key::keySpace);
  camInput.moveDown = kb.isDown(Key::keyLShift);

  // 相机 aspect 跟随视口（而不是整个窗口）
  const ImRect vp = m_uiMan.viewportRect();
  if (vp.GetHeight() > 0.f)
    m_scene.camera().setAspect(vp.GetWidth() / vp.GetHeight());

  m_scene.camera().update(camInput);
  m_uiMan.endFrame();
}
} // namespace App
