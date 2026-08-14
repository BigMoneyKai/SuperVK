#include <benchmark/benchmark.h>

#include "core/memory/linear_allocator.h"

// ---------------------------------------------------------------------------
// LinearAllocator::allocate — bump-pointer allocation, O(1), no free list
// Returns nullptr on overflow; deallocate is a no-op
// ---------------------------------------------------------------------------

// 4 MiB backing buffer shared across iterations (reset between runs)
static constexpr u64 LINEAR_BUFFER_SIZE = 4 * 1024 * 1024;

static void BM_Linear_Allocate(benchmark::State& state) {
    const i64 allocSize = state.range(0);
    LinearAllocator linear(LINEAR_BUFFER_SIZE);

    const i64 maxIters = static_cast<i64>(LINEAR_BUFFER_SIZE) / (allocSize + 48); // +48 for header overhead

    for (auto _ : state) {
        for (i64 i = 0; i < maxIters; ++i) {
            void* ptr = linear.allocate(static_cast<u64>(allocSize));
            benchmark::DoNotOptimize(ptr);
        }
        linear.reset();
    }
    state.SetItemsProcessed(state.iterations() * maxIters);
}
BENCHMARK(BM_Linear_Allocate)
    ->Arg(8)
    ->Arg(16)
    ->Arg(64)
    ->Arg(256)
    ->Arg(1024);

// ---------------------------------------------------------------------------
// LinearAllocator::reset — O(1) cursor reset, frees all allocations at once
// ---------------------------------------------------------------------------
static void BM_Linear_Reset(benchmark::State& state) {
    const i64 n = state.range(0);
    LinearAllocator linear(LINEAR_BUFFER_SIZE);

    for (auto _ : state) {
        for (i64 i = 0; i < n; ++i) {
            linear.allocate(64);
        }
        linear.reset();
    }
    state.SetItemsProcessed(state.iterations() * n);
}
BENCHMARK(BM_Linear_Reset)
    ->Arg(100)
    ->Arg(1000)
    ->Arg(10000);

// ---------------------------------------------------------------------------
// LinearAllocator::allocate with varying sizes — real-world per-frame pattern
// ---------------------------------------------------------------------------
static void BM_Linear_AllocateMixed(benchmark::State& state) {
    static const u64 sizes[] = {16, 64, 256, 8, 128, 32, 512, 24, 96, 48};
    LinearAllocator linear(LINEAR_BUFFER_SIZE);

    const i64 cycles = static_cast<i64>(LINEAR_BUFFER_SIZE) / (48 * 10 + 48);

    for (auto _ : state) {
        for (i64 c = 0; c < cycles; ++c) {
            for (i32 i = 0; i < 10; ++i) {
                void* ptr = linear.allocate(sizes[i]);
                benchmark::DoNotOptimize(ptr);
            }
        }
        linear.reset();
    }
    state.SetItemsProcessed(state.iterations() * cycles * 10);
}
BENCHMARK(BM_Linear_AllocateMixed);
