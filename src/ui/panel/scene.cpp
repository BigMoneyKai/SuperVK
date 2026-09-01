#include "ui/panel/scene.h"

namespace UI {
void Scene::init(const PanelInfo &info) { Panel::init(info); }

void Scene::draw(const ImRect &bounds) {
  // 透明占位窗口：3D 场景在底层渲染，这里只圈出视口矩形
  if (!beginPanel(bounds))
    return;
  endPanel();
}

void Scene::destroy() {}
} // namespace UI
