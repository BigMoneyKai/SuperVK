#pragma once

// .dll/.so
#if defined(_MSC_VER)
#if defined(SV_EXPORT)
#define SVAPI __declspec(dllexport)
#elif defined(SV_IMPORT)
#define SVAPI __declspec(dllimport)
#else
#define SVAPI
#endif
#elif defined(__GNUC__) || defined(__clang__)
#define SVAPI __attribute__((visibility("default")))
#else
#define SVAPI
#endif

// API choice
#if defined(SV_PLATFORM_WINDOWS)
#define SV_RENDER_API_D3D11
#define SV_RENDER_API_VULKAN
#elif defined(SV_PLATFORM_APPLE)
#define SV_RENDER_API_METAL
#define SV_RENDER_API_VULKAN
#elif defined(SV_PLATFORM_LINUX)
#define SV_RENDER_API_VULKAN
#else
#define SV_RENDER_API_NO_SUPPORT
#endif
