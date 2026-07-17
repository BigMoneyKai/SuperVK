#include "asset/manager/skeleton_manager.h"

SkeletonManager::Handle SkeletonManager::load(const String& path) {
    Handle handle = m_assets.size();
    m_assets.emplace_back();
    m_assets[handle].type   = AssetType::Skeleton;
    m_assets[handle].state  = AssetState::Loaded;
    m_assets[handle].path   = path;
    return handle;
}

b32 SkeletonManager::isValid(Handle handle) const {
    return handle != INVALID_HANDLE && handle < m_assets.size();
}

SkeletonAsset& SkeletonManager::get(Handle handle) {
    return m_assets[handle];
}

const SkeletonAsset& SkeletonManager::get(Handle handle) const {
    return m_assets[handle];
}

void SkeletonManager::destroy() {
    m_assets.clear();
}
