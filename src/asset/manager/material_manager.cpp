#include "asset/manager/material_manager.h"

MaterialManager::Handle MaterialManager::load(const String& path) {
    Handle handle = m_assets.size();
    m_assets.emplace_back();
    m_assets[handle].type   = AssetType::material;
    m_assets[handle].state  = AssetState::loaded;
    m_assets[handle].path   = path;
    return handle;
}

b32 MaterialManager::isValid(Handle handle) const {
    return handle != INVALID_HANDLE && handle < m_assets.size();
}

MaterialAsset& MaterialManager::get(Handle handle) {
    return m_assets[handle];
}

const MaterialAsset& MaterialManager::get(Handle handle) const {
    return m_assets[handle];
}

void MaterialManager::destroy() {
    m_assets.clear();
}
