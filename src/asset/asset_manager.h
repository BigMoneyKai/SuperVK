#pragma once

#include "defines.h"

#include "asset/handle/asset_handle.h"

#include "asset/database/asset_database.h"
#include "asset/database/asset_registry.h"

#include "asset/importer/importer_manager.h"

#include "core/container/array/array.hpp"
#include "core/job/job_system.h"

#include "asset/manager/mesh_manager.h"
#include "asset/manager/texture_manager.h"
#include "asset/manager/material_manager.h"
#include "asset/manager/shader_manager.h"
#include "asset/manager/animation_manager.h"
#include "asset/manager/skeleton_manager.h"
#include "asset/manager/scene_manager.h"

#include "asset/assets/mesh_asset.h"
#include "asset/assets/texture_asset.h"
#include "asset/assets/material_asset.h"
#include "asset/assets/shader_asset.h"
#include "asset/assets/animation_asset.h"
#include "asset/assets/skeleton_asset.h"
#include "asset/assets/scene_asset.h"

class AssetMan {
public:
    using Handle = u64;
    static constexpr Handle INVALID_HANDLE = ~0ull;

    using MeshHandle      = MeshManager::Handle;
    using TextureHandle   = TextureManager::Handle;
    using MaterialHandle  = MaterialManager::Handle;
    using ShaderHandle    = ShaderManager::Handle;
    using AnimationHandle = AnimationManager::Handle;
    using SkeletonHandle  = SkeletonManager::Handle;
    using SceneHandle     = SceneManager::Handle;

    void init(JobSystem* js);
    void destroy();
    void update();

    MeshHandle      loadMesh(const String& path);
    b32             isValidMesh(MeshHandle handle) const;
    MeshAsset&      mesh(MeshHandle handle);
    const MeshAsset& mesh(MeshHandle handle) const;

    TextureHandle   loadTexture(const String& path);
    b32             isValidTexture(TextureHandle handle) const;
    TextureAsset&   texture(TextureHandle handle);
    const TextureAsset& texture(TextureHandle handle) const;

    MaterialHandle  loadMaterial(const String& path);
    b32             isValidMaterial(MaterialHandle handle) const;
    MaterialAsset&  material(MaterialHandle handle);
    const MaterialAsset& material(MaterialHandle handle) const;

    ShaderHandle    loadShader(const String& path);
    b32             isValidShader(ShaderHandle handle) const;
    ShaderAsset&    shader(ShaderHandle handle);
    const ShaderAsset& shader(ShaderHandle handle) const;

    AnimationHandle loadAnimation(const String& path);
    b32             isValidAnimation(AnimationHandle handle) const;
    AnimationAsset& animation(AnimationHandle handle);
    const AnimationAsset& animation(AnimationHandle handle) const;

    SkeletonHandle  loadSkeleton(const String& path);
    b32             isValidSkeleton(SkeletonHandle handle) const;
    SkeletonAsset&  skeleton(SkeletonHandle handle);
    const SkeletonAsset& skeleton(SkeletonHandle handle) const;

    SceneHandle     loadScene(const String& path);
    b32             isValidScene(SceneHandle handle) const;
    SceneAsset&     scene(SceneHandle handle);
    const SceneAsset& scene(SceneHandle handle) const;

private:
    JobSystem*      m_jobSystem{nullptr};
    ImporterMan     m_importerMan;
    AssetDatabase   m_assetDatabase;
    AssetRegistry   m_assetRegistry;

    MeshManager      m_meshMan;
    TextureManager   m_textureMan;
    MaterialManager  m_materialMan;
    ShaderManager    m_shaderMan;
    AnimationManager m_animationMan;
    SkeletonManager  m_skeletonMan;
    SceneManager     m_sceneMan;
};
