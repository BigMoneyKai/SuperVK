#include "asset/manager/shader_manager.h"
#include "asset/loader/shader_loader.h"
#include "core/debug/debugger.h"

void ShaderManager::init(JobSystem* js) {
    m_jobSystem = js;
}

ShaderManager::Handle ShaderManager::load(const String& path) {
    Handle handle = m_assets.size();
    m_assets.emplace_back();
    m_assets[handle].type   = AssetType::Shader;
    m_assets[handle].state  = AssetState::Uploading;
    m_assets[handle].path   = path;

    auto* jd         = new LoadJobData;
    jd->self         = this;
    jd->handle       = handle;
    std::strncpy(jd->path, path.c_str(), sizeof(jd->path) - 1);
    jd->path[sizeof(jd->path) - 1] = '\0';
    m_jobSystem->submit(jobFunc, jd);

    return handle;
}

b32 ShaderManager::isValid(Handle handle) const {
    return handle != INVALID_HANDLE && handle < m_assets.size();
}

ShaderAsset& ShaderManager::get(Handle handle) {
    return m_assets[handle];
}

const ShaderAsset& ShaderManager::get(Handle handle) const {
    return m_assets[handle];
}

void ShaderManager::update() {
    for (auto& a : m_assets)
        if (a.state == AssetState::Uploaded)
            a.state = AssetState::Loaded;
}

void ShaderManager::destroy() {
    m_assets.clear();
}

void ShaderManager::jobFunc(void* data) {
    auto* jd = static_cast<LoadJobData*>(data);
    auto& out = jd->self->m_assets[jd->handle];
    ShaderLoader loader;
    out.state = loader.load(jd->path, out)
                   ? AssetState::Uploaded
                   : AssetState::Failed;
    delete jd;
}
