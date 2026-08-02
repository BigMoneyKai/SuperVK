#pragma once

#include "asset/asset_manager.h"
#include "core/job/job_system.h"
#include "defines.h"
#include "input/input_manager.h"
#include "render/renderer.h"
#include "scene/scene.h"
#include "window/window_manager.h"

#include <GLFW/glfw3.h>

SV_INLINE constexpr u64 defaultThreadCount = 8;
SV_INLINE constexpr const char *defaultTitle = "SuperVK";
SV_INLINE constexpr DisplayMode defaultDisplayMode = WINDOWED;

struct AppInfo {
  const char *version{"1.0.0"};
  const char *name{"SuperVK"};
};

class App {
public:
  void init(const char *title = defaultTitle,
            DisplayMode mode = defaultDisplayMode,
            u64 threadCount = defaultThreadCount);
  void run();
  void destroy();

private:
  void update();

private:
  WinMan m_winMan;
  InputMan m_inputMan;
  Scene m_scene;
  Renderer m_renderer;
  JobSystem m_jobSystem;
  AssetMan m_assetMan;

  AssetMan::MeshHandle m_meshHandle{AssetMan::INVALID_HANDLE};
  AssetMan::TextureHandle m_textureHandle{AssetMan::INVALID_HANDLE};

  AppInfo m_appInfo{};
};
