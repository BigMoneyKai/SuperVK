#pragma once

#include "asset/asset_type.h"

template<AssetType T> struct AssetData;

template<>
struct AssetData<AssetType::mesh>;
template<>
struct AssetData<AssetType::material>;
template<>
struct AssetData<AssetType::texture>;
template<>
struct AssetData<AssetType::shader>;
template<>
struct AssetData<AssetType::animation>;
template<>
struct AssetData<AssetType::scene>;
template<>
struct AssetData<AssetType::skeleton>;

#include "asset_data.inl"
