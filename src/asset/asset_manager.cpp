#include "asset/asset_manager.h"

void AssetMan::init(JobSystem* js) {
    m_jobSystem = js;
    m_importerMan.init();

    m_meshMan.init(m_jobSystem, &m_importerMan);
    m_textureMan.init(m_jobSystem);
    m_shaderMan.init(m_jobSystem);
}

void AssetMan::destroy() {
    m_meshMan.destroy();
    m_textureMan.destroy();
    m_materialMan.destroy();
    m_shaderMan.destroy();
    m_animationMan.destroy();
    m_skeletonMan.destroy();
    m_sceneMan.destroy();

    m_assetDatabase.clear();
    m_assetRegistry.clear();
    m_importerMan.destroy();
}

void AssetMan::update() {
    m_meshMan.update();
    m_textureMan.update();
    m_shaderMan.update();
}

AssetMan::MeshHandle AssetMan::loadMesh(const String& path) {
    return m_meshMan.load(path);
}

b32 AssetMan::isValidMesh(MeshHandle handle) const {
    return m_meshMan.isValid(handle);
}

MeshAsset& AssetMan::mesh(MeshHandle handle) {
    return m_meshMan.get(handle);
}

const MeshAsset& AssetMan::mesh(MeshHandle handle) const {
    return m_meshMan.get(handle);
}

AssetMan::TextureHandle AssetMan::loadTexture(const String& path) {
    return m_textureMan.load(path);
}

b32 AssetMan::isValidTexture(TextureHandle handle) const {
    return m_textureMan.isValid(handle);
}

TextureAsset& AssetMan::texture(TextureHandle handle) {
    return m_textureMan.get(handle);
}

const TextureAsset& AssetMan::texture(TextureHandle handle) const {
    return m_textureMan.get(handle);
}

AssetMan::MaterialHandle AssetMan::loadMaterial(const String& path) {
    return m_materialMan.load(path);
}

b32 AssetMan::isValidMaterial(MaterialHandle handle) const {
    return m_materialMan.isValid(handle);
}

MaterialAsset& AssetMan::material(MaterialHandle handle) {
    return m_materialMan.get(handle);
}

const MaterialAsset& AssetMan::material(MaterialHandle handle) const {
    return m_materialMan.get(handle);
}

AssetMan::ShaderHandle AssetMan::loadShader(const String& path) {
    return m_shaderMan.load(path);
}

b32 AssetMan::isValidShader(ShaderHandle handle) const {
    return m_shaderMan.isValid(handle);
}

ShaderAsset& AssetMan::shader(ShaderHandle handle) {
    return m_shaderMan.get(handle);
}

const ShaderAsset& AssetMan::shader(ShaderHandle handle) const {
    return m_shaderMan.get(handle);
}

AssetMan::AnimationHandle AssetMan::loadAnimation(const String& path) {
    return m_animationMan.load(path);
}

b32 AssetMan::isValidAnimation(AnimationHandle handle) const {
    return m_animationMan.isValid(handle);
}

AnimationAsset& AssetMan::animation(AnimationHandle handle) {
    return m_animationMan.get(handle);
}

const AnimationAsset& AssetMan::animation(AnimationHandle handle) const {
    return m_animationMan.get(handle);
}

AssetMan::SkeletonHandle AssetMan::loadSkeleton(const String& path) {
    return m_skeletonMan.load(path);
}

b32 AssetMan::isValidSkeleton(SkeletonHandle handle) const {
    return m_skeletonMan.isValid(handle);
}

SkeletonAsset& AssetMan::skeleton(SkeletonHandle handle) {
    return m_skeletonMan.get(handle);
}

const SkeletonAsset& AssetMan::skeleton(SkeletonHandle handle) const {
    return m_skeletonMan.get(handle);
}

AssetMan::SceneHandle AssetMan::loadScene(const String& path) {
    return m_sceneMan.load(path);
}

b32 AssetMan::isValidScene(SceneHandle handle) const {
    return m_sceneMan.isValid(handle);
}

SceneAsset& AssetMan::scene(SceneHandle handle) {
    return m_sceneMan.get(handle);
}

const SceneAsset& AssetMan::scene(SceneHandle handle) const {
    return m_sceneMan.get(handle);
}
