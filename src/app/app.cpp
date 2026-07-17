#include "app/app.h"
#include "scene/camera.h"
#include "core/memory/allocator.h"
#include "core/debug/debugger.h"

static b32 waitForMeshLoad(JobSystem& js, AssetMan& am, AssetMan::MeshHandle handle) {
    while (true) {
        js.wait_all();
        am.update();
        if (am.isValidMesh(handle) && am.mesh(handle).state == AssetState::Loaded)
            return true;
        if (am.isValidMesh(handle) && am.mesh(handle).state == AssetState::Failed)
            return false;
    }
}

void App::init(const char* title, DisplayMode mode, u64 threadCount) {
    m_jobSystem.init(static_cast<u32>(threadCount));
    m_winMan.init(title, mode);
    m_inputMan.init(m_winMan.window());
    m_renderer.init({&m_winMan});

    m_assetMan.init(&m_jobSystem);

    m_meshHandle = m_assetMan.loadMesh("resource/AlphaBlendModeTest/glTF-Binary/AlphaBlendModeTest.glb");

    if (!waitForMeshLoad(m_jobSystem, m_assetMan, m_meshHandle)) {
        FATAL("App: failed to load initial mesh");
    }

    mesh::Mesh& src = m_assetMan.mesh(m_meshHandle).mesh;
    mesh::Mesh& dst = m_scene.mesh();
    dst.vertices()  = std::move(src.vertices());
    dst.indices()   = std::move(src.indices());
    dst.positions() = std::move(src.positions());
    dst.texcoords() = std::move(src.texcoords());
    dst.normals()   = std::move(src.normals());

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

void App::update() {
    m_inputMan.newFrame();

    CameraInput camInput;
    Mouse& mouse = m_inputMan.getMouse();
    Keyboard& kb = m_inputMan.getKeyboard();

    camInput.mouseDelta   = mouse.delta();
    camInput.scrollWheel  = mouse.wheel();
    camInput.orbitEnabled = mouse.isDown(BUTTON_2);
    camInput.moveForward  = kb.isDown(KEY_W);
    camInput.moveBackward = kb.isDown(KEY_S);
    camInput.moveLeft     = kb.isDown(KEY_A);
    camInput.moveRight    = kb.isDown(KEY_D);

    m_scene.camera().update(camInput);
}
