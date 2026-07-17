#pragma once

#include "defines.h"
#include "asset_handle.h"

// ---------- Mesh ----------
template<>
struct AssetHandle<AssetType::Mesh> {
    static constexpr AssetType kType = AssetType::Mesh;
    u64 id = UINT32_MAX;  // UINT32_MAX 代表无效句柄

    AssetHandle() = default;
    explicit AssetHandle(u64 id) : id(id) {}

    b32 IsValid() const { return id != UINT32_MAX; }
    static AssetHandle Invalid() { return AssetHandle{}; }

    b32 operator==(const AssetHandle& other) const { return id == other.id; }
    b32 operator!=(const AssetHandle& other) const { return id != other.id; }
};

// ---------- Material ----------
template<>
struct AssetHandle<AssetType::Material> {
    static constexpr AssetType kType = AssetType::Material;
    u64 id = UINT32_MAX;

    AssetHandle() = default;
    explicit AssetHandle(u64 id) : id(id) {}

    b32 IsValid() const { return id != UINT32_MAX; }
    static AssetHandle Invalid() { return AssetHandle{}; }

    b32 operator==(const AssetHandle& other) const { return id == other.id; }
    b32 operator!=(const AssetHandle& other) const { return id != other.id; }
};

// ---------- Texture ----------
template<>
struct AssetHandle<AssetType::Texture> {
    static constexpr AssetType kType = AssetType::Texture;
    u64 id = UINT32_MAX;

    AssetHandle() = default;
    explicit AssetHandle(u64 id) : id(id) {}

    b32 IsValid() const { return id != UINT32_MAX; }
    static AssetHandle Invalid() { return AssetHandle{}; }

    b32 operator==(const AssetHandle& other) const { return id == other.id; }
    b32 operator!=(const AssetHandle& other) const { return id != other.id; }
};

// ---------- Shader ----------
template<>
struct AssetHandle<AssetType::Shader> {
    static constexpr AssetType kType = AssetType::Shader;
    u64 id = UINT32_MAX;

    AssetHandle() = default;
    explicit AssetHandle(u64 id) : id(id) {}

    b32 IsValid() const { return id != UINT32_MAX; }
    static AssetHandle Invalid() { return AssetHandle{}; }

    b32 operator==(const AssetHandle& other) const { return id == other.id; }
    b32 operator!=(const AssetHandle& other) const { return id != other.id; }
};

// ---------- Animation ----------
template<>
struct AssetHandle<AssetType::Animation> {
    static constexpr AssetType kType = AssetType::Animation;
    u64 id = UINT32_MAX;

    AssetHandle() = default;
    explicit AssetHandle(u64 id) : id(id) {}

    b32 IsValid() const { return id != UINT32_MAX; }
    static AssetHandle Invalid() { return AssetHandle{}; }

    b32 operator==(const AssetHandle& other) const { return id == other.id; }
    b32 operator!=(const AssetHandle& other) const { return id != other.id; }
};

// ---------- Scene ----------
template<>
struct AssetHandle<AssetType::Scene> {
    static constexpr AssetType kType = AssetType::Scene;
    u64 id = UINT32_MAX;

    AssetHandle() = default;
    explicit AssetHandle(u64 id) : id(id) {}

    b32 IsValid() const { return id != UINT32_MAX; }
    static AssetHandle Invalid() { return AssetHandle{}; }

    b32 operator==(const AssetHandle& other) const { return id == other.id; }
    b32 operator!=(const AssetHandle& other) const { return id != other.id; }
};

// ---------- Skeleton ----------
template<>
struct AssetHandle<AssetType::Skeleton> {
    static constexpr AssetType kType = AssetType::Skeleton;
    u64 id = UINT32_MAX;

    AssetHandle() = default;
    explicit AssetHandle(u64 id) : id(id) {}

    b32 IsValid() const { return id != UINT32_MAX; }
    static AssetHandle Invalid() { return AssetHandle{}; }

    b32 operator==(const AssetHandle& other) const { return id == other.id; }
    b32 operator!=(const AssetHandle& other) const { return id != other.id; }
};

