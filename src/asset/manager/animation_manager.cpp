#include "asset/manager/animation_manager.h"

AnimationManager::Handle AnimationManager::load(const String& path) {
    Handle handle = m_assets.size();
    m_assets.emplace_back();
    m_assets[handle].type   = AssetType::animation;
    m_assets[handle].state  = AssetState::loaded;
    m_assets[handle].path   = path;
    return handle;
}

b32 AnimationManager::isValid(Handle handle) const {
    return handle != INVALID_HANDLE && handle < m_assets.size();
}

AnimationAsset& AnimationManager::get(Handle handle) {
    return m_assets[handle];
}

const AnimationAsset& AnimationManager::get(Handle handle) const {
    return m_assets[handle];
}

void AnimationManager::destroy() {
    m_assets.clear();
}
