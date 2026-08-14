#include "asset/manager/scene_manager.h"

SceneManager::Handle SceneManager::load(const String& path) {
    Handle handle = m_assets.size();
    m_assets.emplace_back();
    m_assets[handle].type   = AssetType::scene;
    m_assets[handle].state  = AssetState::loaded;
    m_assets[handle].path   = path;
    return handle;
}

b32 SceneManager::isValid(Handle handle) const {
    return handle != INVALID_HANDLE && handle < m_assets.size();
}

SceneAsset& SceneManager::get(Handle handle) {
    return m_assets[handle];
}

const SceneAsset& SceneManager::get(Handle handle) const {
    return m_assets[handle];
}

void SceneManager::destroy() {
    m_assets.clear();
}
