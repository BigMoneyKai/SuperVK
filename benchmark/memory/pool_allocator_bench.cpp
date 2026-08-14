#include <benchmark/benchmark.h>

#include "core/memory/pool_allocator.h"

// ---------------------------------------------------------------------------
// PoolAllocator — fixed-size free-list allocator
// All chunks are the same size; alloc/dealloc are O(1) free-list ops.
// Returns nullptr when the free list is exhausted (no growth).
// ---------------------------------------------------------------------------

static constexpr u64 CHUNK_SIZE = 64;
static constexpr u64 CHUNK_COUNT = 64 * 1024; // 64K chunks = 4M entries
static constexpr u64 CHUNK_ALIGN = 16;

// ---------------------------------------------------------------------------
// allocate — O(1) pop from free list
// ---------------------------------------------------------------------------
static void BM_Pool_Allocate(benchmark::State& state) {
    const i64 n = state.range(0);
    PoolAllocator pool(CHUNK_SIZE, static_cast<u64>(n), CHUNK_ALIGN);

    for (auto _ : state) {
        void* ptrs[8192];
        for (i64 i = 0; i < n; ++i) {
            ptrs[i] = pool.allocate(CHUNK_SIZE);
            benchmark::DoNotOptimize(ptrs[i]);
        }
        // Return to free list for next iteration
        for (i64 i = 0; i < n; ++i) {
            pool.deallocate(ptrs[i]);
        }
    }
    state.SetItemsProcessed(state.iterations() * n);
}
BENCHMARK(BM_Pool_Allocate)
    ->Arg(100)
    ->Arg(1000)
    ->Arg(8192);

// ---------------------------------------------------------------------------
// allocate + deallocate interleaved — O(1) push/pop to/from free list
// ---------------------------------------------------------------------------
static void BM_Pool_AllocateDeallocate(benchmark::State& state) {
    const i64 n = state.range(0);
    PoolAllocator pool(CHUNK_SIZE, static_cast<u64>(n) * 2, CHUNK_ALIGN);

    for (auto _ : state) {
        for (i64 i = 0; i < n; ++i) {
            void* ptr = pool.allocate(CHUNK_SIZE);
            pool.deallocate(ptr);
        }
    }
    state.SetItemsProcessed(state.iterations() * n);
}
BENCHMARK(BM_Pool_AllocateDeallocate)
    ->Arg(100)
    ->Arg(1000)
    ->Arg(10000);

// ---------------------------------------------------------------------------
// Full-allocation then deallocate — measures bulk allocation + bulk free
// ---------------------------------------------------------------------------
static void BM_Pool_BulkAllocDealloc(benchmark::State& state) {
    const i64 n = state.range(0);
    PoolAllocator pool(CHUNK_SIZE, static_cast<u64>(n), CHUNK_ALIGN);

    void* ptrs[65536];

    for (auto _ : state) {
        for (i64 i = 0; i < n; ++i) {
            ptrs[i] = pool.allocate(CHUNK_SIZE);
        }
        for (i64 i = 0; i < n; ++i) {
            pool.deallocate(ptrs[i]);
        }
    }
    state.SetItemsProcessed(state.iterations() * n);
}
BENCHMARK(BM_Pool_BulkAllocDealloc)
    ->Arg(100)
    ->Arg(1000)
    ->Arg(10000);
