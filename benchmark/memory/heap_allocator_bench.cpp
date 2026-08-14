#include <benchmark/benchmark.h>

#include "core/memory/heap_allocator.h"

// ---------------------------------------------------------------------------
// HeapAllocator::allocate — raw allocation from the OS/page allocator
// Each call goes through platform_aligned_alloc with 40-byte header overhead
// ---------------------------------------------------------------------------
static void BM_Heap_Allocate(benchmark::State& state) {
    const i64 size = state.range(0);
    HeapAllocator& heap = HeapAllocator::instance();

    for (auto _ : state) {
        void* ptr = heap.allocate(static_cast<u64>(size));
        benchmark::DoNotOptimize(ptr);
        heap.deallocate(ptr);
    }
    state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_Heap_Allocate)
    ->Arg(1)
    ->Arg(16)
    ->Arg(64)
    ->Arg(256)
    ->Arg(1024)
    ->Arg(4096)
    ->Arg(65536);

// ---------------------------------------------------------------------------
// HeapAllocator allocate + deallocate pair — measures full lifecycle
// ---------------------------------------------------------------------------
static void BM_Heap_AllocateDeallocate(benchmark::State& state) {
    const i64 n = state.range(0);
    HeapAllocator& heap = HeapAllocator::instance();

    for (auto _ : state) {
        void* ptrs[256];
        for (i64 i = 0; i < n; ++i) {
            ptrs[i] = heap.allocate(64);
        }
        for (i64 i = 0; i < n; ++i) {
            heap.deallocate(ptrs[i]);
        }
    }
    state.SetItemsProcessed(state.iterations() * n);
}
BENCHMARK(BM_Heap_AllocateDeallocate)
    ->Arg(1)
    ->Arg(10)
    ->Arg(100);

// ---------------------------------------------------------------------------
// Various allocation sizes — measure size impact on allocation cost
// ---------------------------------------------------------------------------
static void BM_Heap_AllocateVariousSizes(benchmark::State& state) {
    HeapAllocator& heap = HeapAllocator::instance();
    // Cycle through a mix of common allocation sizes
    static const u64 sizes[] = {8, 16, 32, 64, 128, 256, 512, 1024};

    for (auto _ : state) {
        void* ptrs[8];
        for (i32 i = 0; i < 8; ++i) {
            ptrs[i] = heap.allocate(sizes[i]);
        }
        for (i32 i = 0; i < 8; ++i) {
            heap.deallocate(ptrs[i]);
        }
    }
    state.SetItemsProcessed(state.iterations() * 8);
}
BENCHMARK(BM_Heap_AllocateVariousSizes);
