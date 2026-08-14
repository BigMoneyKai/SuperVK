#include <benchmark/benchmark.h>

#include "core/memory/stack_allocator.h"

// ---------------------------------------------------------------------------
// StackAllocator — LIFO bump allocator with individual deallocate support
// Each deallocate rewinds the cursor to the freed allocation's position.
// Only correct when freed in strict reverse-allocation order.
// ---------------------------------------------------------------------------

static constexpr u64 STACK_BUFFER_SIZE = 4 * 1024 * 1024;

// ---------------------------------------------------------------------------
// allocate — bump-pointer O(1), same as Linear but tracks per-allocation state
// ---------------------------------------------------------------------------
static void BM_Stack_Allocate(benchmark::State& state) {
    const i64 allocSize = state.range(0);
    StackAllocator stack(STACK_BUFFER_SIZE);

    const i64 maxIters = static_cast<i64>(STACK_BUFFER_SIZE) / (allocSize + 48);

    for (auto _ : state) {
        for (i64 i = 0; i < maxIters; ++i) {
            void* ptr = stack.allocate(static_cast<u64>(allocSize));
            benchmark::DoNotOptimize(ptr);
        }
        stack.reset();
    }
    state.SetItemsProcessed(state.iterations() * maxIters);
}
BENCHMARK(BM_Stack_Allocate)
    ->Arg(8)
    ->Arg(64)
    ->Arg(256)
    ->Arg(1024);

// ---------------------------------------------------------------------------
// allocate + deallocate (LIFO) — measures paired alloc/dealloc in stack order
// This is the intended usage pattern: push and pop in reverse order
// ---------------------------------------------------------------------------
static void BM_Stack_AllocateDeallocate(benchmark::State& state) {
    const i64 n = state.range(0);
    StackAllocator stack(STACK_BUFFER_SIZE);

    for (auto _ : state) {
        void* ptrs[256];
        for (i64 i = 0; i < n; ++i) {
            ptrs[i] = stack.allocate(64);
        }
        // Deallocate in reverse order (LIFO)
        for (i64 i = n - 1; i >= 0; --i) {
            stack.deallocate(ptrs[i]);
        }
    }
    state.SetItemsProcessed(state.iterations() * n);
}
BENCHMARK(BM_Stack_AllocateDeallocate)
    ->Arg(1)
    ->Arg(10)
    ->Arg(100)
    ->Arg(256);

// ---------------------------------------------------------------------------
// reset — complete reset of the stack, O(1)
// ---------------------------------------------------------------------------
static void BM_Stack_Reset(benchmark::State& state) {
    const i64 n = state.range(0);
    StackAllocator stack(STACK_BUFFER_SIZE);

    for (auto _ : state) {
        for (i64 i = 0; i < n; ++i) {
            stack.allocate(64);
        }
        stack.reset();
    }
    state.SetItemsProcessed(state.iterations() * n);
}
BENCHMARK(BM_Stack_Reset)
    ->Arg(100)
    ->Arg(1000)
    ->Arg(10000);

// ---------------------------------------------------------------------------
// reset_to(Marker) — partial rollback, measures marker-based scoped cleanup
// ---------------------------------------------------------------------------
static void BM_Stack_ResetToMarker(benchmark::State& state) {
    StackAllocator stack(STACK_BUFFER_SIZE);

    for (auto _ : state) {
        // Allocate, capture marker, allocate more, then rollback
        stack.allocate(64);
        Marker m{stack.used()};
        stack.allocate(128);
        stack.allocate(256);
        stack.reset_to(m);
    }
    state.SetItemsProcessed(state.iterations() * 3);
}
BENCHMARK(BM_Stack_ResetToMarker);
