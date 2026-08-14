#pragma once

#include "asset/asset_type.h"
#include "asset/handle/asset_handle.h"
#include "core/container/hashmap/hashmap.hpp"
#include "core/container/string/string.hpp"

class AssetRegistry {
public:
  b32 contains(const String &path);

  template <AssetType T> AssetHandle<T> find(const String &path);

  template <AssetType T> void insert(const String &path, AssetHandle<T> handle);

  template <AssetType T> void erase(const String &path);

  void clear();

private:
  HashMap<String, AssetHandle<AssetType::mesh>> m_meshRegistry{8};
  HashMap<String, AssetHandle<AssetType::material>> m_materialRegistry{8};
  HashMap<String, AssetHandle<AssetType::shader>> m_shaderRegistry{8};
  HashMap<String, AssetHandle<AssetType::texture>> m_textureRegistry{8};
  HashMap<String, AssetHandle<AssetType::scene>> m_sceneRegistry{8};
  HashMap<String, AssetHandle<AssetType::animation>> m_animationRegistry{8};
  HashMap<String, AssetHandle<AssetType::skeleton>> m_skeletonRegistry{8};
};

template <AssetType T> AssetHandle<T> AssetRegistry::find(const String &path) {
  if constexpr (T == AssetType::mesh)
    return m_meshRegistry.contains(path) ? m_meshRegistry[path]
                                         : AssetHandle<T>{};
  else if constexpr (T == AssetType::material)
    return m_materialRegistry.contains(path) ? m_materialRegistry[path]
                                             : AssetHandle<T>{};
  else if constexpr (T == AssetType::shader)
    return m_shaderRegistry.contains(path) ? m_shaderRegistry[path]
                                           : AssetHandle<T>{};
  else if constexpr (T == AssetType::texture)
    return m_textureRegistry.contains(path) ? m_textureRegistry[path]
                                            : AssetHandle<T>{};
  else if constexpr (T == AssetType::scene)
    return m_sceneRegistry.contains(path) ? m_sceneRegistry[path]
                                          : AssetHandle<T>{};
  else if constexpr (T == AssetType::animation)
    return m_animationRegistry.contains(path) ? m_animationRegistry[path]
                                              : AssetHandle<T>{};
  else if constexpr (T == AssetType::skeleton)
    return m_skeletonRegistry.contains(path) ? m_skeletonRegistry[path]
                                             : AssetHandle<T>{};
  else
    return AssetHandle<T>{};
}

template <AssetType T>
void AssetRegistry::insert(const String &path, AssetHandle<T> handle) {
  if constexpr (T == AssetType::mesh)
    m_meshRegistry.insert(path, handle);
  else if constexpr (T == AssetType::material)
    m_materialRegistry.insert(path, handle);
  else if constexpr (T == AssetType::shader)
    m_shaderRegistry.insert(path, handle);
  else if constexpr (T == AssetType::texture)
    m_textureRegistry.insert(path, handle);
  else if constexpr (T == AssetType::scene)
    m_sceneRegistry.insert(path, handle);
  else if constexpr (T == AssetType::animation)
    m_animationRegistry.insert(path, handle);
  else if constexpr (T == AssetType::skeleton)
    m_skeletonRegistry.insert(path, handle);
}

template <AssetType T> void AssetRegistry::erase(const String &path) {
  if constexpr (T == AssetType::mesh)
    m_meshRegistry.erase(path);
  else if constexpr (T == AssetType::material)
    m_materialRegistry.erase(path);
  else if constexpr (T == AssetType::shader)
    m_shaderRegistry.erase(path);
  else if constexpr (T == AssetType::texture)
    m_textureRegistry.erase(path);
  else if constexpr (T == AssetType::scene)
    m_sceneRegistry.erase(path);
  else if constexpr (T == AssetType::animation)
    m_animationRegistry.erase(path);
  else if constexpr (T == AssetType::skeleton)
    m_skeletonRegistry.erase(path);
}
