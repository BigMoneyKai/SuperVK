#pragma once

#include "defines.h"

#include "asset/asset_type.h"

template<AssetType T>
struct AssetHandle;

template<>
struct AssetHandle<AssetType::Mesh>;
template<>
struct AssetHandle<AssetType::Material>;
template<>
struct AssetHandle<AssetType::Texture>;
template<>
struct AssetHandle<AssetType::Shader>;
template<>
struct AssetHandle<AssetType::Animation>;
template<>
struct AssetHandle<AssetType::Scene>;
template<>
struct AssetHandle<AssetType::Skeleton>;

#include "asset_handle.inl"
