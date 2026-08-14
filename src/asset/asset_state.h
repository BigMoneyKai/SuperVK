#pragma once

#include "core/define/types.h"

enum class AssetState : u8 {
  // in static resource
  unloaded,

  // cpu operation
  uploading,
  uploaded,

  // gpu operation
  loading,
  loaded,

  unloading,
  failed

};
