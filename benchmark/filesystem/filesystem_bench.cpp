#include <benchmark/benchmark.h>

#include "core/filesystem/file_path.h"
#include "platform/file.h"

// ---------------------------------------------------------------------------
// Filesystem benchmarks — measure FilePath and low-level platform file I/O
// ---------------------------------------------------------------------------

// FilePath — construction from C-string
static void BM_FilePath_Construct(benchmark::State& state) {
    for (auto _ : state) {
        FilePath path("assets/textures/albedo.png");
        benchmark::DoNotOptimize(path);
    }
    state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_FilePath_Construct);

// FilePath — file_name() / extension() / stem() extraction
static void BM_FilePath_Components(benchmark::State& state) {
    FilePath path("assets/meshes/sponza.obj");
    for (auto _ : state) {
        StringView name = path.file_name();
        StringView stem = path.stem();
        StringView ext  = path.extension();
        benchmark::DoNotOptimize(name.data());
        benchmark::DoNotOptimize(stem.data());
        benchmark::DoNotOptimize(ext.data());
    }
    state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_FilePath_Components);

// FilePath — parent() traversal
static void BM_FilePath_Parent(benchmark::State& state) {
    for (auto _ : state) {
        FilePath path("a/b/c/d/e/f/g/h/file.txt");
        // Walk up to root (8 levels)
        FilePath p = path.parent();   // a/b/c/d/e/f/g/h
        p = p.parent();               // a/b/c/d/e/f/g
        p = p.parent();               // a/b/c/d/e/f
        p = p.parent();               // a/b/c/d/e
        p = p.parent();               // a/b/c/d
        p = p.parent();               // a/b/c
        p = p.parent();               // a/b
        p = p.parent();               // a
        benchmark::DoNotOptimize(p);
    }
    state.SetItemsProcessed(state.iterations() * 8);
}
BENCHMARK(BM_FilePath_Parent);

// FilePath — append()
static void BM_FilePath_Append(benchmark::State& state) {
    for (auto _ : state) {
        FilePath path("assets");
        path.append(FilePath("textures"));
        path.append(FilePath("albedo.png"));
        benchmark::DoNotOptimize(path);
    }
    state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_FilePath_Append);

// ---------------------------------------------------------------------------
// Platform-level file I/O — using the C-style file_* API
// ---------------------------------------------------------------------------

// file_open + file_close — measure open/close overhead
static void BM_File_OpenClose(benchmark::State& state) {
    for (auto _ : state) {
        FileHandle* f = file_open("CMakeLists.txt", FileMode::read);
        benchmark::DoNotOptimize(f);
        file_close(f);
    }
    state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_File_OpenClose);

// file_read — sequential read in varying chunk sizes
static void BM_File_Read(benchmark::State& state) {
    const i64 chunkSize = state.range(0);
    FileHandle* f = file_open("CMakeLists.txt", FileMode::read);
    u64 totalSize = file_size(f);

    u8 buf[4096];
    for (auto _ : state) {
        file_seek(f, 0, FileSeekOrigin::begin);
        u64 total = 0;
        while (total < totalSize) {
            u64 bytes = file_read(f, buf, static_cast<u64>(chunkSize) > 4096 ? 4096 : static_cast<u64>(chunkSize));
            if (bytes == 0) break;
            total += bytes;
        }
        benchmark::DoNotOptimize(total);
    }
    file_close(f);
    state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_File_Read)->Arg(1)->Arg(16)->Arg(256)->Arg(4096);

// file_write — sequential write benchmark
static void BM_File_Write(benchmark::State& state) {
    const i64 size = state.range(0);
    u8* data = new u8[static_cast<u64>(size)];
    memset(data, 0xAB, static_cast<u64>(size));

    for (auto _ : state) {
        FileHandle* f = file_open("/tmp/supervk_bench_write.tmp", FileMode::write);
        u64 written = file_write(f, data, static_cast<u64>(size));
        benchmark::DoNotOptimize(written);
        file_close(f);
    }
    delete[] data;
    file_close(nullptr); // ensure cleanup
    state.SetBytesProcessed(state.iterations() * size);
}
BENCHMARK(BM_File_Write)->Arg(1024)->Arg(65536)->Arg(1048576);

// fs_file_exists — stat-based existence check
static void BM_File_Exists(benchmark::State& state) {
    for (auto _ : state) {
        i32 exists = fs_file_exists("CMakeLists.txt");
        benchmark::DoNotOptimize(exists);
    }
    state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_File_Exists);

// fs_read_file — one-shot full file read into memory
static void BM_File_ReadAll(benchmark::State& state) {
    for (auto _ : state) {
        void* data = nullptr;
        u64 size = 0;
        i32 ok = fs_read_file("CMakeLists.txt", &data, &size);
        benchmark::DoNotOptimize(ok);
        benchmark::DoNotOptimize(size);
        // Note: fs_read_file allocates memory; need to free
        if (data) {
            delete[] static_cast<u8*>(data);
        }
    }
    state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_File_ReadAll);
