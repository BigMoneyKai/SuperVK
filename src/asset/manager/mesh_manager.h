#pragma once

#include "asset/asset_state.h"
#include "asset/asset_type.h"
#include "asset/assets/mesh_asset.h"
#include "asset/handle/asset_handle.h"
#include "asset/importer/importer_manager.h"
#include "core/container/array/array.hpp"
#include "core/container/string/string.hpp"
#include "core/job/job_system.h"
#include <cstring>

class MeshManager {
public:
  using Handle = u64;
  static constexpr Handle INVALID_HANDLE = ~0ull;

  void init(JobSystem *js, ImporterMan *imp);
  Handle load(const String &path);
  void destroy();

  b32 isValid(Handle handle) const;
  MeshAsset &get(Handle handle);
  const MeshAsset &get(Handle handle) const;

  void update();

private:
  struct LoadJobData {
    MeshManager *self;
    char path[256];
    Handle handle;
  };
  static void jobFunc(void *data);

  JobSystem *m_jobSystem{nullptr};
  ImporterMan *m_importer{nullptr};
  Array<MeshAsset> m_assets;
};
