# SuperVK — AGENTS.md

## Build

```sh
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

- Compiler: `clang` (macOS), `gcc` (Linux). C++23 (`-std=c++23`).
- `compile_commands.json` generated at `build/`. clangd cache in `.cache/`.
- Executable output: `build/bin/supervk`.
- Shaders compile via `glslangValidator` → `shaders/spv/`. Run `cmake --build build --target shaders` to compile manually. SPIR-V and `resource/` are copied to the executable output dir post-build.

## Architecture

```
src/
├── main.cpp          # App entry: App → init/run/destroy
├── app/              # Orchestration (owns all managers + scene)
├── core/
│   ├── memory/       # Allocator base + Heap/Linear/Stack/Pool
│   ├── container/    # Array / HashMap / String / Queue / RingBuffer
│   ├── job/          # JobSystem with worker threads
│   ├── debug/        # DEBUG/INFO/WARNING/ERROR/FATAL macros, logger, VK_CHECK_RESULT
│   ├── io/           # ifstream/ofstream / imstream/omstream / ibstream/obstream / serializer
│   └── types/        # vertex.h
├── render/
│   ├── renderer.h    # Renderer orchestrator (owns all render managers)
│   ├── base/         # Instance, Device, Surface, Swapchain, RenderPass, Command*
│   ├── descriptor/   # DescriptorManager, pools, sets, layouts
│   ├── frame/        # FrameResource, DepthResource, Sync, Framebuffer
│   ├── pipeline/     # PipelineManager, Shader, Pipeline
│   └── resource/     # Buffer, Texture, Image, Sampler managers
├── scene/            # Camera, Light, Material, Mesh, Object, Scene
├── input/            # Keyboard + Mouse input via GLFW
├── window/           # WindowManager (GLFW)
├── platform/         # File, Thread, Memory, Time (platform abstraction)
├── asset/
│   ├── database/     # AssetDB, registry, dependency graph
│   ├── importer/     # GLTF/OBJ parsers + importer manager
│   └── cache/        # Cache manager, serializer
└── utils/            # Misc utilities
```

## Conventions

- **Types**: PascalCase (`Renderer`, `Array<T>`, `JobSystem`).
- **Member variables**: `m_` prefix (`m_device`, `m_handles`).
- **Functions**: camelCase (`init`, `destroy`, `drawFrame`).
- **Typedefs** (from `src/defines.h`): `u8/u16/u32/u64`, `i8/i16/i32/i64`, `f32/f64`, `b32/b8`, `addr`.
- **Headers**: `#pragma once` only (no include guards).
- **No STL containers** in engine code. Use `Array<T>`, `HashMap<K,V>`, `String`, `Queue`, `RingBuffer`. All allocator-aware, header-only.
- **No exceptions, no RTTI**.
- **`sv_` prefix** for macros: `SV_FORCE_INLINE`, `SV_RESTRICT`, `SV_ALIGN(x)`, `SV_BREAK()`, `SV_ASSERT(...)`.
- **`VK_CHECK_RESULT(func)`** wraps Vulkan calls with abort on failure (defined in `core/debug/debugger.h`).

## Gotchas

### Init ordering (render)

`DescriptorMan::init()` **must** complete before `PipelineMan::init()` — the pipeline needs the descriptor set layout. See `renderer.cpp:18-19`.

### Shaders

- Source: `shaders/glsl/*.{vert,frag,comp,geom,tesc,tese}`
- Compiled SPIR-V: `shaders/spv/*.spv`
- Loaded at runtime from `shaders/spv/basic.vert.spv` (relative to cwd / executable dir).
- Resource paths are relative: `resource/textures/red.png`, `resource/AlphaBlendModeTest/glTF-Binary/AlphaBlendModeTest.glb`.

### Build quirks

- `supervk_core` and `supervk_debug` are `INTERFACE` CMake targets (headers only, no compiled library).
- `Vulkan::Vulkan` is linked on every target regardless of actual Vulkan usage.
- Benchmark target `supervk_bench` (in `benchmark/`) is commented out in root `CMakeLists.txt`. Uncomment to enable.
- Submodules: `stb`, `glfw`, `glm`, `imgui`, `benchmark`. Run `git submodule update --init --recursive` after clone.

### Testing

No unit test framework. Only benchmarks (Google Benchmark) in `benchmark/`. Run with `build/bin/supervk_bench` (after enabling the target).

## Platform defines

From `src/defines.h`:
- `SV_PLATFORM_WINDOWS` / `SV_PLATFORM_APPLE` / `SV_PLATFORM_LINUX` / `SV_PLATFORM_ANDROID`
- `SV_COMPILER_MSVC` / `SV_COMPILER_CLANG` / `SV_COMPILER_GCC`
- `SV_RENDER_API_VULKAN` / `SV_RENDER_API_METAL` / `SV_RENDER_API_D3D11`

## Packaging

CPack configured in `cmake/packaging.cmake`: macOS → DragNDrop, Windows → ZIP, Linux → TGZ.
