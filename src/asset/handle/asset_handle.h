#pragma once

#include "asset/asset_type.h"

template <AssetType T> struct AssetHandle;

template <> struct AssetHandle<AssetType::mesh>;
template <> struct AssetHandle<AssetType::material>;
template <> struct AssetHandle<AssetType::texture>;
template <> struct AssetHandle<AssetType::shader>;
template <> struct AssetHandle<AssetType::animation>;
template <> struct AssetHandle<AssetType::scene>;
template <> struct AssetHandle<AssetType::skeleton>;

#include "asset_handle.inl"
