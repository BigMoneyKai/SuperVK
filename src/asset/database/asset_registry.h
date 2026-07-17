#pragma once

#include "defines.h"
#include "core/container/hashmap/hashmap.hpp"
#include "core/container/string/string.hpp"
#include "asset/handle/asset_handle.h"
#include "asset/asset_type.h"

class AssetRegistry {
public:
    b32 contains(const String& path);

    template<AssetType T>
    AssetHandle<T> find(const String& path);

    template<AssetType T>
    void insert(const String& path, AssetHandle<T> handle);

    template<AssetType T>
    void erase(const String& path);

    void clear();

private:
    HashMap<String, AssetHandle<AssetType::Mesh>>       m_meshRegistry{8};
    HashMap<String, AssetHandle<AssetType::Material>>   m_materialRegistry{8};
    HashMap<String, AssetHandle<AssetType::Shader>>     m_shaderRegistry{8};
    HashMap<String, AssetHandle<AssetType::Texture>>    m_textureRegistry{8};
    HashMap<String, AssetHandle<AssetType::Scene>>      m_sceneRegistry{8};
    HashMap<String, AssetHandle<AssetType::Animation>>  m_animationRegistry{8};
    HashMap<String, AssetHandle<AssetType::Skeleton>>   m_skeletonRegistry{8};
};

template<AssetType T>
AssetHandle<T> AssetRegistry::find(const String& path) {
    if      constexpr (T == AssetType::Mesh)      return m_meshRegistry.contains(path)      ? m_meshRegistry[path]      : AssetHandle<T>{};
    else if constexpr (T == AssetType::Material)  return m_materialRegistry.contains(path)  ? m_materialRegistry[path]  : AssetHandle<T>{};
    else if constexpr (T == AssetType::Shader)    return m_shaderRegistry.contains(path)    ? m_shaderRegistry[path]    : AssetHandle<T>{};
    else if constexpr (T == AssetType::Texture)   return m_textureRegistry.contains(path)   ? m_textureRegistry[path]   : AssetHandle<T>{};
    else if constexpr (T == AssetType::Scene)     return m_sceneRegistry.contains(path)     ? m_sceneRegistry[path]     : AssetHandle<T>{};
    else if constexpr (T == AssetType::Animation) return m_animationRegistry.contains(path) ? m_animationRegistry[path] : AssetHandle<T>{};
    else if constexpr (T == AssetType::Skeleton)  return m_skeletonRegistry.contains(path)  ? m_skeletonRegistry[path]  : AssetHandle<T>{};
    else return AssetHandle<T>{};
}

template<AssetType T>
void AssetRegistry::insert(const String& path, AssetHandle<T> handle) {
    if      constexpr (T == AssetType::Mesh)      m_meshRegistry.insert(path, handle);
    else if constexpr (T == AssetType::Material)  m_materialRegistry.insert(path, handle);
    else if constexpr (T == AssetType::Shader)    m_shaderRegistry.insert(path, handle);
    else if constexpr (T == AssetType::Texture)   m_textureRegistry.insert(path, handle);
    else if constexpr (T == AssetType::Scene)     m_sceneRegistry.insert(path, handle);
    else if constexpr (T == AssetType::Animation) m_animationRegistry.insert(path, handle);
    else if constexpr (T == AssetType::Skeleton)  m_skeletonRegistry.insert(path, handle);
}

template<AssetType T>
void AssetRegistry::erase(const String& path) {
    if      constexpr (T == AssetType::Mesh)      m_meshRegistry.erase(path);
    else if constexpr (T == AssetType::Material)  m_materialRegistry.erase(path);
    else if constexpr (T == AssetType::Shader)    m_shaderRegistry.erase(path);
    else if constexpr (T == AssetType::Texture)   m_textureRegistry.erase(path);
    else if constexpr (T == AssetType::Scene)     m_sceneRegistry.erase(path);
    else if constexpr (T == AssetType::Animation) m_animationRegistry.erase(path);
    else if constexpr (T == AssetType::Skeleton)  m_skeletonRegistry.erase(path);
}
