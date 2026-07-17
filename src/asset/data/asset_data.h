#pragma once

#include "asset/asset_type.h"

template<AssetType T> struct AssetData;

template<>
struct AssetData<AssetType::Mesh>;
template<>
struct AssetData<AssetType::Material>;
template<>
struct AssetData<AssetType::Texture>;
template<>
struct AssetData<AssetType::Shader>;
template<>
struct AssetData<AssetType::Animation>;
template<>
struct AssetData<AssetType::Scene>;
template<>
struct AssetData<AssetType::Skeleton>;

#include "asset_data.inl"
