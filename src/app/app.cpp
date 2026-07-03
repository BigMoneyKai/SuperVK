#include "app/app.h"
#include "scene/camera.h"

void App::init(const char* title, DisplayMode mode) {
    m_winMan.init(title, mode);
    m_inputMan.init(m_winMan.window());
    m_renderer.init({&m_winMan});
    m_parserMan.init();

    // Load mesh data BEFORE scene init — Mesh::init() needs non-empty vertices/indices
    // to create GPU buffers, otherwise it will FATAL.
    m_parserMan.parse("assets/objects/heart/Heart.obj", &m_scene.mesh());

    m_scene.init(
        m_renderer.device().device(),
        m_renderer.device().physicalDevice(),
        m_winMan.width(),
        m_winMan.height()
    );
}

void App::run() {
    while(!glfwWindowShouldClose(m_winMan.window())) {
        glfwPollEvents();
        update();
        m_renderer.render(m_scene);
    }
}

void App::destroy() {
    m_renderer.waitIdle();
    m_scene.destroy(m_renderer.device().device());
    m_renderer.destroy();
    m_inputMan.destroy();
    m_winMan.destroy();
}

void App::update() {
    m_inputMan.newFrame();

    // ---- assemble camera input from mouse + keyboard ----
    CameraInput camInput;
    Mouse& mouse = m_inputMan.getMouse();
    Keyboard& kb = m_inputMan.getKeyboard();

    camInput.mouseDelta   = mouse.delta();
    camInput.scrollWheel  = mouse.wheel();
    camInput.orbitEnabled = mouse.isDown(BUTTON_2);  // right mouse button
    camInput.moveForward  = kb.isDown(KEY_W);
    camInput.moveBackward = kb.isDown(KEY_S);
    camInput.moveLeft     = kb.isDown(KEY_A);
    camInput.moveRight    = kb.isDown(KEY_D);

    m_scene.camera().update(camInput);
}
