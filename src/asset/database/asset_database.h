#pragma once

#include "defines.h"
#include "asset/handle/asset_handle.h"
#include "asset/asset_type.h"
#include "asset/assets/asset.h"
#include "asset/assets/mesh_asset.h"
#include "asset/assets/material_asset.h"
#include "asset/assets/texture_asset.h"
#include "asset/assets/shader_asset.h"
#include "asset/assets/animation_asset.h"
#include "asset/assets/skeleton_asset.h"
#include "asset/assets/scene_asset.h"
#include "core/container/hashmap/hashmap.hpp"

class AssetDatabase {
public:
    template<AssetType T>
    Asset* find(AssetHandle<T> handle);

    template<AssetType T>
    b32 contains(AssetHandle<T> handle);

    template<AssetType T>
    void insert(AssetHandle<T> handle, Asset* pAsset);

    template<AssetType T>
    void erase(AssetHandle<T> handle);

    void clear();

private:
    HashMap<AssetHandle<AssetType::Mesh>, MeshAsset*>           m_meshAssetDB{8};
    HashMap<AssetHandle<AssetType::Material>, MaterialAsset*>   m_materialAssetDB{8};
    HashMap<AssetHandle<AssetType::Texture>, TextureAsset*>     m_textureAssetDB{8};
    HashMap<AssetHandle<AssetType::Shader>, ShaderAsset*>       m_shaderAssetDB{8};
    HashMap<AssetHandle<AssetType::Scene>, SceneAsset*>         m_sceneAssetDB{8};
    HashMap<AssetHandle<AssetType::Animation>, AnimationAsset*> m_animationAssetDB{8};
    HashMap<AssetHandle<AssetType::Skeleton>, SkeletonAsset*>   m_skeletonAssetDB{8};
};

template<AssetType T>
Asset* AssetDatabase::find(AssetHandle<T> handle) {
    if      constexpr (T == AssetType::Mesh)      return m_meshAssetDB.contains(handle)      ? static_cast<Asset*>(m_meshAssetDB[handle])      : nullptr;
    else if constexpr (T == AssetType::Material)  return m_materialAssetDB.contains(handle)  ? static_cast<Asset*>(m_materialAssetDB[handle])  : nullptr;
    else if constexpr (T == AssetType::Texture)   return m_textureAssetDB.contains(handle)   ? static_cast<Asset*>(m_textureAssetDB[handle])   : nullptr;
    else if constexpr (T == AssetType::Shader)    return m_shaderAssetDB.contains(handle)    ? static_cast<Asset*>(m_shaderAssetDB[handle])    : nullptr;
    else if constexpr (T == AssetType::Animation) return m_animationAssetDB.contains(handle) ? static_cast<Asset*>(m_animationAssetDB[handle]) : nullptr;
    else if constexpr (T == AssetType::Scene)     return m_sceneAssetDB.contains(handle)     ? static_cast<Asset*>(m_sceneAssetDB[handle])     : nullptr;
    else if constexpr (T == AssetType::Skeleton)  return m_skeletonAssetDB.contains(handle)  ? static_cast<Asset*>(m_skeletonAssetDB[handle])  : nullptr;
    else return nullptr;
}

template<AssetType T>
b32 AssetDatabase::contains(AssetHandle<T> handle) {
    if      constexpr (T == AssetType::Mesh)      return m_meshAssetDB.contains(handle);
    else if constexpr (T == AssetType::Material)  return m_materialAssetDB.contains(handle);
    else if constexpr (T == AssetType::Texture)   return m_textureAssetDB.contains(handle);
    else if constexpr (T == AssetType::Shader)    return m_shaderAssetDB.contains(handle);
    else if constexpr (T == AssetType::Animation) return m_animationAssetDB.contains(handle);
    else if constexpr (T == AssetType::Scene)     return m_sceneAssetDB.contains(handle);
    else if constexpr (T == AssetType::Skeleton)  return m_skeletonAssetDB.contains(handle);
    else return false;
}

template<AssetType T>
void AssetDatabase::insert(AssetHandle<T> handle, Asset* pAsset) {
    if      constexpr (T == AssetType::Mesh)      m_meshAssetDB.insert(handle, static_cast<MeshAsset*>(pAsset));
    else if constexpr (T == AssetType::Material)  m_materialAssetDB.insert(handle, static_cast<MaterialAsset*>(pAsset));
    else if constexpr (T == AssetType::Texture)   m_textureAssetDB.insert(handle, static_cast<TextureAsset*>(pAsset));
    else if constexpr (T == AssetType::Shader)    m_shaderAssetDB.insert(handle, static_cast<ShaderAsset*>(pAsset));
    else if constexpr (T == AssetType::Animation) m_animationAssetDB.insert(handle, static_cast<AnimationAsset*>(pAsset));
    else if constexpr (T == AssetType::Scene)     m_sceneAssetDB.insert(handle, static_cast<SceneAsset*>(pAsset));
    else if constexpr (T == AssetType::Skeleton)  m_skeletonAssetDB.insert(handle, static_cast<SkeletonAsset*>(pAsset));
}

template<AssetType T>
void AssetDatabase::erase(AssetHandle<T> handle) {
    if      constexpr (T == AssetType::Mesh)      m_meshAssetDB.erase(handle);
    else if constexpr (T == AssetType::Material)  m_materialAssetDB.erase(handle);
    else if constexpr (T == AssetType::Texture)   m_textureAssetDB.erase(handle);
    else if constexpr (T == AssetType::Shader)    m_shaderAssetDB.erase(handle);
    else if constexpr (T == AssetType::Animation) m_animationAssetDB.erase(handle);
    else if constexpr (T == AssetType::Scene)     m_sceneAssetDB.erase(handle);
    else if constexpr (T == AssetType::Skeleton)  m_skeletonAssetDB.erase(handle);
}
