#include "app/app.h"
#include "core/debug/debugger.h"
#include "core/define/types.h"
#include "platform/time.h"
#include "scene/camera.h"

#include <algorithm>

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
    const f32 dt = std::clamp(static_cast<f32>((now - last) * 0.001), 0.0f, 0.05f);
    last = now;

    update(dt);
    m_assetMan.update();
    m_renderer.render(m_scene);
  }
}

void App::destroy() {
  m_renderer.waitIdle();

  m_scene.destroy();
  m_renderer.destroy();
  m_assetMan.destroy();
  m_jobSystem.destroy();
  m_inputMan.destroy();
  m_winMan.destroy();
}

void App::update(f32 dt) {
  m_inputMan.newFrame();

  CameraInput camInput;
  Mouse &mouse = m_inputMan.getMouse();
  Keyboard &kb = m_inputMan.getKeyboard();

  camInput.deltaTime = dt;
  camInput.mouseDelta = mouse.delta();
  camInput.scrollWheel = mouse.wheel();
  camInput.moveForward = kb.isDown(Key::keyW);
  camInput.moveBackward = kb.isDown(Key::keyS);
  camInput.moveLeft = kb.isDown(Key::keyA);
  camInput.moveRight = kb.isDown(Key::keyD);
  camInput.moveUp = kb.isDown(Key::keySpace);
  camInput.moveDown = kb.isDown(Key::keyLShift);

  m_scene.camera().update(camInput);
}
