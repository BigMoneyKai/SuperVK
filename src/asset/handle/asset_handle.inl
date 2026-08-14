#pragma once

#include "asset_handle.h"

// ---------- Mesh ----------
template <> struct AssetHandle<AssetType::mesh> {
  static constexpr AssetType kType = AssetType::mesh;
  u64 id = UINT32_MAX; // UINT32_MAX means invalid handle

  AssetHandle() = default;
  explicit AssetHandle(u64 id) : id(id) {}

  b32 IsValid() const { return id != UINT32_MAX; }
  static AssetHandle Invalid() { return AssetHandle{}; }

  b32 operator==(const AssetHandle &other) const { return id == other.id; }
  b32 operator!=(const AssetHandle &other) const { return id != other.id; }
};

// ---------- Material ----------
template <> struct AssetHandle<AssetType::material> {
  static constexpr AssetType kType = AssetType::material;
  u64 id = UINT32_MAX;

  AssetHandle() = default;
  explicit AssetHandle(u64 id) : id(id) {}

  b32 IsValid() const { return id != UINT32_MAX; }
  static AssetHandle Invalid() { return AssetHandle{}; }

  b32 operator==(const AssetHandle &other) const { return id == other.id; }
  b32 operator!=(const AssetHandle &other) const { return id != other.id; }
};

// ---------- Texture ----------
template <> struct AssetHandle<AssetType::texture> {
  static constexpr AssetType kType = AssetType::texture;
  u64 id = UINT32_MAX;

  AssetHandle() = default;
  explicit AssetHandle(u64 id) : id(id) {}

  b32 IsValid() const { return id != UINT32_MAX; }
  static AssetHandle Invalid() { return AssetHandle{}; }

  b32 operator==(const AssetHandle &other) const { return id == other.id; }
  b32 operator!=(const AssetHandle &other) const { return id != other.id; }
};

// ---------- Shader ----------
template <> struct AssetHandle<AssetType::shader> {
  static constexpr AssetType kType = AssetType::shader;
  u64 id = UINT32_MAX;

  AssetHandle() = default;
  explicit AssetHandle(u64 id) : id(id) {}

  b32 IsValid() const { return id != UINT32_MAX; }
  static AssetHandle Invalid() { return AssetHandle{}; }

  b32 operator==(const AssetHandle &other) const { return id == other.id; }
  b32 operator!=(const AssetHandle &other) const { return id != other.id; }
};

// ---------- Animation ----------
template <> struct AssetHandle<AssetType::animation> {
  static constexpr AssetType kType = AssetType::animation;
  u64 id = UINT32_MAX;

  AssetHandle() = default;
  explicit AssetHandle(u64 id) : id(id) {}

  b32 IsValid() const { return id != UINT32_MAX; }
  static AssetHandle Invalid() { return AssetHandle{}; }

  b32 operator==(const AssetHandle &other) const { return id == other.id; }
  b32 operator!=(const AssetHandle &other) const { return id != other.id; }
};

// ---------- Scene ----------
template <> struct AssetHandle<AssetType::scene> {
  static constexpr AssetType kType = AssetType::scene;
  u64 id = UINT32_MAX;

  AssetHandle() = default;
  explicit AssetHandle(u64 id) : id(id) {}

  b32 IsValid() const { return id != UINT32_MAX; }
  static AssetHandle Invalid() { return AssetHandle{}; }

  b32 operator==(const AssetHandle &other) const { return id == other.id; }
  b32 operator!=(const AssetHandle &other) const { return id != other.id; }
};

// ---------- Skeleton ----------
template <> struct AssetHandle<AssetType::skeleton> {
  static constexpr AssetType kType = AssetType::skeleton;
  u64 id = UINT32_MAX;

  AssetHandle() = default;
  explicit AssetHandle(u64 id) : id(id) {}

  b32 IsValid() const { return id != UINT32_MAX; }
  static AssetHandle Invalid() { return AssetHandle{}; }

  b32 operator==(const AssetHandle &other) const { return id == other.id; }
  b32 operator!=(const AssetHandle &other) const { return id != other.id; }
};
