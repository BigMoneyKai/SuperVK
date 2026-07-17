#include "asset/database/asset_registry.h"

b32 AssetRegistry::contains(const String& path) {
    return m_meshRegistry.contains(path) ||
           m_materialRegistry.contains(path) ||
           m_shaderRegistry.contains(path) ||
           m_textureRegistry.contains(path) ||
           m_sceneRegistry.contains(path) ||
           m_animationRegistry.contains(path) ||
           m_skeletonRegistry.contains(path);
}

void AssetRegistry::clear() {
    m_meshRegistry.clear();
    m_materialRegistry.clear();
    m_shaderRegistry.clear();
    m_textureRegistry.clear();
    m_sceneRegistry.clear();
    m_animationRegistry.clear();
    m_skeletonRegistry.clear();
}
