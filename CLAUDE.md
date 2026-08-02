# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build

```sh
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

- Compiler: `clang` (macOS), `gcc` (Linux). C++23 (`-std=c++23`).
- `compile_commands.json` generated at the build dir root (symlinked from project root).
- Executable output: `build/bin/supervk`.
- Shaders compile via `glslangValidator` → `shaders/spv/`. Run `cmake --build build --target shaders` to compile manually. SPIR-V and `resource/` are copied to the executable output dir post-build.
- Debug builds enable `-fsanitize=address` on both compile and link.
- Submodules: `stb`, `glfw`, `glm`, `imgui`, `benchmark` in `thirdparty/`. Run `git submodule update --init --recursive` after clone.

## Architecture

```
src/
├── main.cpp          # 3 lines: #include "entry.h" → MAIN_ENTRY()
├── app/              # App owns all managers + Scene; init → run → destroy
├── core/
│   ├── memory/       # Allocator base + Heap/Linear/Stack/Pool
│   ├── container/    # Array / HashMap / String / StringView / Queue
│   ├── job/          # JobSystem with work-stealing worker threads
│   ├── debug/        # TRACE/DEBUG/INFO/WARNING/ERROR/FATAL macros, Logger, VK_CHECK_RESULT
│   ├── io/           # ifstream/ofstream / imstream/omstream / ibstream/obstream / serializer
│   └── types/        # vertex.h
├── render/
│   ├── renderer.h    # Renderer owns all Vulkan objects; single-pipeline, single-descriptor-set
│   ├── base/         # Instance, Device, Surface, Swapchain, RenderPass, CommandPool/Buffer
│   ├── descriptor/   # DescriptorMan: one layout, one pool, one set, 4 UBOs + 1 sampler
│   ├── frame/        # FrameResource (3 frames in flight), DepthResource, Framebuffer, Sync
│   ├── pipeline/     # PipelineMan: one vert + one frag shader → one graphics pipeline
│   └── resource/     # Buffer, Texture, Image, Sampler managers
├── scene/            # Camera, Light, Material, Mesh, Object, Scene (no hierarchy)
├── input/            # Keyboard (WASD only) + Mouse via GLFW callbacks
├── window/           # WinMan wrapping GLFWwindow
├── platform/         # File, Thread, Memory, Time (platform abstraction layer)
├── asset/
│   ├── database/     # AssetDB, registry, dependency graph (scaffolded, not wired in)
│   ├── importer/     # GLTF/GLB/OBJ parsers + ImporterMan
│   ├── cache/        # Cache manager, serializer (scaffolded, not wired in)
│   └── manager/      # Per-type asset managers (Mesh, Texture, Material, Shader, etc.)
└── utils/            # Misc utilities
```

## Key Architectural Patterns

### Renderer: single-global-state, single draw call

The renderer is deliberately minimal. One graphics pipeline, one descriptor set, one mesh per scene. 3 frames in flight with fence+semaphore gating. The descriptor set layout binds 4 UBOs (camera, object, light, material at bindings 0–3) and one combined image sampler (binding 4). `drawFrame` uploads UBOs inside the render pass via host-visible buffers, binds the single pipeline and set, and issues one `vkCmdDrawIndexed`.

**Critical init ordering:** `DescriptorMan::init()` must complete before `PipelineMan::init()` — the pipeline needs the descriptor set layout. See `renderer.cpp`.

**Frame indexing asymmetry:** `m_imageAvailableSemaphores` and `m_inFlightFences` are indexed by frame-in-flight (0..2), but `m_renderFinishedSemaphores` are indexed by swapchain image index. This is correct — each swapchain image needs its own render-finished semaphore because `vkAcquireNextImageKHR` may skip frames.

**Memory model:** All GPU buffers use host-visible + host-coherent memory. No device-local memory, no transfer queue, no staging-based upload path. `Buffer::update(data)` does map/memcpy/unmap. This keeps things simple but is not performant for large assets.

### Asset pipeline: async load, manual GPU promotion

1. `AssetMan::loadMesh(path)` → appends `MeshAsset` to array, submits a job to `JobSystem`
2. Worker thread runs `MeshLoader::load()` → `ImporterMan::parse()` dispatches to ObjParser/GltfParser/GlbParser
3. Parser fills a CPU-side `scene::Mesh` (std::vector vertices/indices/positions/texcoords/normals)
4. Main thread detects `Uploaded` state in `AssetMan::update()`, promotes to `Loaded`
5. `App::init()` copies mesh data into `Scene::mesh()`, then `Scene::init()` creates GPU buffers

Asset handles are raw `u64` array indices. `AssetDatabase`, `AssetRegistry`, `CacheMan`, and `DependencyGraph` exist as headers but are not wired into the managers — the pipeline currently operates directly on `Array<Asset>`.

### Containers: allocator-aware, move-only, header-only

All containers (`Array<T>`, `HashMap<K,V>`, `String`, `Queue<T>`, `StringView`) are header-only templates with `.hpp`/`.inl` split. Every container takes `Allocator*` in its constructor (defaults to `default_allocator()`). Copy is deleted; move is allowed. `String` uses SSO (23-char inline buffer, 40-byte total size). `HashMap` uses open addressing with SIMD group probing (SwissTable-inspired): 1-byte fingerprints, Robin Hood insertion, parallel arrays for keys/vals/hashes/probe-distances.

Containers must never log — only `SV_ASSERT` and `SV_STATIC_ASSERT` (per `src/core/container/README.md`).

### Allocators: composable backing-allocator chain

- **HeapAllocator** — singleton, wraps `platform_aligned_alloc/free`. The root.
- **LinearAllocator** — bump allocator, `deallocate` is a no-op, `reset()` rewinds. Use for per-frame scratch.
- **StackAllocator** — bump with LIFO markers. `reset_to(Marker)` for scoped cleanup.
- **PoolAllocator** — fixed-size free-list. Use for many same-sized objects.

All except HeapAllocator take a `backing` allocator (defaulting to heap) and allocate their buffer from it in one call. Debug builds prepend a guard-header to every allocation; release builds strip it.

## Conventions

- **Types**: PascalCase (`Renderer`, `Array<T>`, `JobSystem`)
- **Member variables**: `m_` prefix (`m_device`, `m_handles`)
- **Functions**: camelCase (`init`, `destroy`, `drawFrame`)
- **Typedefs** (from `src/defines.h`): `u8/u16/u32/u64`, `i8/i16/i32/i64`, `f32/f64`, `b32/b8`, `addr`
- **Headers**: `#pragma once` only (no include guards)
- **No STL containers** in engine code. Use `Array<T>`, `HashMap<K,V>`, `String`, `Queue<T>`, `RingBuffer`
- **No exceptions, no RTTI**
- **`sv_` prefix** for macros: `SV_FORCE_INLINE`, `SV_RESTRICT`, `SV_ALIGN(x)`, `SV_BREAK()`, `SV_ASSERT(...)`
- **`VK_CHECK_RESULT(func)`** wraps Vulkan calls with abort on failure; compiles to `(func)` in release
- **Logging macros**: `TRACE`/`DEBUG`/`INFO`/`WARNING`/`ERROR`/`FATAL(catag, fmt, ...)` using `std::format`; `TRACE` compiles out without `TRACE_MODE`, `DEBUG` compiles out under `NDEBUG`
- **Log categories**: `LogCatag::Asset`, `::Render`, `::Vulkan`, `::Platform`, `::Scene`, `::Input`, `::Window`, `::JobSystem`, `::Memory`, `::Container`, `::IO`
- **Vulkan wrappers**: every class follows `init`/`destroy` + `SV_FORCE_INLINE` accessors, stores its `VkCreateInfo` and owning `VkDevice`. No RAII destructors, no move semantics.

## CMake Target Structure

- `supervk` — main executable, links all subsystem libraries + `Vulkan::Vulkan`
- `supervk_core` — `INTERFACE` (headers only: containers + memory + debug + job + io)
- `supervk_app`, `supervk_render`, `supervk_scene`, `supervk_input`, `supervk_window`, `supervk_platform`, `supervk_utils` — static libraries
- `supervk_asset`, `supervk_asset_parser` — static libraries for asset subsystem
- `supervk_bench` — benchmark target (commented out in root `CMakeLists.txt`)

## Shaders

- Source: `shaders/glsl/*.{vert,frag,comp,geom,tesc,tese}`
- Compiled SPIR-V: `shaders/spv/*.spv`
- Loaded at runtime from `shaders/spv/basic.vert.spv` (relative to executable dir; copied post-build)

## Platform Defines

From `src/defines.h`:
- `SV_PLATFORM_WINDOWS` / `SV_PLATFORM_APPLE` (`SV_PLATFORM_MAC` / `SV_PLATFORM_IOS`) / `SV_PLATFORM_LINUX` / `SV_PLATFORM_ANDROID`
- `SV_COMPILER_MSVC` / `SV_COMPILER_CLANG` / `SV_COMPILER_GCC`
- `SV_RENDER_API_VULKAN` / `SV_RENDER_API_METAL` / `SV_RENDER_API_D3D11`

## What's Scaffolded vs Implemented

**Fully working:** Renderer (Vulkan init → draw → present), all containers, allocator hierarchy, job system, logger, GLTF/GLB/OBJ importers, async mesh loading, window/input.

**Scaffolded but not wired in:** `AssetDatabase`, `AssetRegistry`, `CacheMan`, `DependencyGraph`, `BufferMan` (empty class), `RingBuffer` (empty directory), `SceneAsset`/`SceneManager` (headers exist but `App` manually copies mesh data).

**No swapchain resize/recreation handling.** On `VK_ERROR_OUT_OF_DATE_KHR` or `VK_SUBOPTIMAL_KHR`, `drawFrame` only logs a warning and returns.

## Testing

No unit test framework. Only benchmarks (Google Benchmark) in `benchmark/`. Run with `build/bin/supervk_bench` after uncommenting `add_subdirectory(benchmark)` in root `CMakeLists.txt` and rebuilding. The `test/` directory is empty.
