#include "asset/manager/mesh_manager.h"
#include "asset/loader/mesh_loader.h"
#include "core/debug/debugger.h"

void MeshManager::init(JobSystem* js, ImporterMan* imp) {
    m_jobSystem = js;
    m_importer  = imp;
}

MeshManager::Handle MeshManager::load(const String& path) {
    Handle handle = m_assets.size();
    m_assets.emplace_back();
    m_assets[handle].type   = AssetType::mesh;
    m_assets[handle].state  = AssetState::uploading;
    m_assets[handle].path   = path;

    auto* jd         = new LoadJobData;
    jd->self         = this;
    jd->handle       = handle;
    std::strncpy(jd->path, path.c_str(), sizeof(jd->path) - 1);
    jd->path[sizeof(jd->path) - 1] = '\0';
    m_jobSystem->submit(jobFunc, jd);

    return handle;
}

b32 MeshManager::isValid(Handle handle) const {
    return handle != INVALID_HANDLE && handle < m_assets.size();
}

MeshAsset& MeshManager::get(Handle handle) {
    return m_assets[handle];
}

const MeshAsset& MeshManager::get(Handle handle) const {
    return m_assets[handle];
}

void MeshManager::update() {
    for (auto& a : m_assets)
        if (a.state == AssetState::uploaded)
            a.state = AssetState::loaded;
}

void MeshManager::destroy() {
    m_assets.clear();
}

void MeshManager::jobFunc(void* data) {
    auto* jd = static_cast<LoadJobData*>(data);
    auto& out = jd->self->m_assets[jd->handle];
    MeshLoader loader;
    out.state = loader.load(*jd->self->m_importer, jd->path, out)
                    ? AssetState::uploaded
                    : AssetState::failed;
    delete jd;
}
