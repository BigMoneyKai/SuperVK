#include <benchmark/benchmark.h>

#include "core/container/ring_buffer/ring_buffer.hpp"

// ---------------------------------------------------------------------------
// push_back — append to tail (O(1), mask-based index)
// ---------------------------------------------------------------------------
static void BM_RingBuffer_PushBack(benchmark::State& state) {
    const i64 n = state.range(0);
    for (auto _ : state) {
        RingBuffer<u64> rb(static_cast<u64>(n));
        for (i64 i = 0; i < n; ++i) {
            rb.push_back(static_cast<u64>(i));
        }
        benchmark::DoNotOptimize(rb.size());
    }
    state.SetItemsProcessed(state.iterations() * n);
}
BENCHMARK(BM_RingBuffer_PushBack)
    ->Arg(100)
    ->Arg(1000)
    ->Arg(10000)
    ->Arg(100000);

// ---------------------------------------------------------------------------
// pop_front — O(1), just advances head
// ---------------------------------------------------------------------------
static void BM_RingBuffer_PopFront(benchmark::State& state) {
    const i64 n = state.range(0);
    for (auto _ : state) {
        RingBuffer<u64> rb(static_cast<u64>(n));
        for (i64 i = 0; i < n; ++i) rb.push_back(static_cast<u64>(i));
        for (i64 i = 0; i < n; ++i) {
            rb.pop_front();
        }
        benchmark::DoNotOptimize(rb.size());
    }
    state.SetItemsProcessed(state.iterations() * n);
}
BENCHMARK(BM_RingBuffer_PopFront)
    ->Arg(100)
    ->Arg(1000)
    ->Arg(10000)
    ->Arg(100000);

// ---------------------------------------------------------------------------
// FIFO push/pop interleaved — the defining ring-buffer workload
// push + pop in lockstep → O(1) per op, buffer size stays ~1
// ---------------------------------------------------------------------------
static void BM_RingBuffer_FifoPushPop(benchmark::State& state) {
    const i64 n = state.range(0);
    for (auto _ : state) {
        RingBuffer<u64> rb(16);
        for (i64 i = 0; i < n; ++i) {
            rb.push_back(static_cast<u64>(i));
            rb.pop_front();
        }
        benchmark::DoNotOptimize(rb.size());
    }
    state.SetItemsProcessed(state.iterations() * n);
}
BENCHMARK(BM_RingBuffer_FifoPushPop)
    ->Arg(100)
    ->Arg(1000)
    ->Arg(10000)
    ->Arg(100000);

// ---------------------------------------------------------------------------
// FIFO with steady-state queue depth — simulates command queue workload
// ---------------------------------------------------------------------------
static void BM_RingBuffer_FifoBatching(benchmark::State& state) {
    const i64 n = state.range(0);
    for (auto _ : state) {
        RingBuffer<u64> rb(256);
        // fill half
        for (i32 i = 0; i < 128; ++i) rb.push_back(i);
        // batch: push 8, pop 8, repeat n times
        for (i64 i = 0; i < n; ++i) {
            for (i32 j = 0; j < 8; ++j) rb.push_back(static_cast<u64>(i));
            for (i32 j = 0; j < 8; ++j) rb.pop_front();
        }
        benchmark::DoNotOptimize(rb.size());
    }
    state.SetItemsProcessed(state.iterations() * n * 8);
}
BENCHMARK(BM_RingBuffer_FifoBatching)
    ->Arg(100)
    ->Arg(1000)
    ->Arg(10000);

// ---------------------------------------------------------------------------
// front / back — O(1) access to ends
// ---------------------------------------------------------------------------
static void BM_RingBuffer_FrontBack(benchmark::State& state) {
    RingBuffer<u64> rb(1024);
    for (i32 i = 0; i < 512; ++i) rb.push_back(i);

    for (auto _ : state) {
        u64 sum = rb.front() + rb.back();
        benchmark::DoNotOptimize(sum);
    }
    state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_RingBuffer_FrontBack);

// ---------------------------------------------------------------------------
// operator[] — logical-index random access
// ---------------------------------------------------------------------------
static void BM_RingBuffer_Access(benchmark::State& state) {
    const i64 n = state.range(0);
    RingBuffer<u64> rb(static_cast<u64>(n));
    for (i64 i = 0; i < n; ++i) rb.push_back(static_cast<u64>(i));

    for (auto _ : state) {
        u64 sum = 0;
        for (i64 i = 0; i < n; ++i) {
            sum += rb[static_cast<u64>(i)];
        }
        benchmark::DoNotOptimize(sum);
    }
    state.SetItemsProcessed(state.iterations() * n);
}
BENCHMARK(BM_RingBuffer_Access)
    ->Arg(1000)
    ->Arg(10000)
    ->Arg(100000);

// ---------------------------------------------------------------------------
// Forward iteration — range-based for over (potentially wrapped) elements
// ---------------------------------------------------------------------------
static void BM_RingBuffer_Iterate(benchmark::State& state) {
    const i64 n = state.range(0);
    RingBuffer<u64> rb(static_cast<u64>(n));
    for (i64 i = 0; i < n; ++i) rb.push_back(static_cast<u64>(i));

    for (auto _ : state) {
        u64 sum = 0;
        for (auto& v : rb) sum += v;
        benchmark::DoNotOptimize(sum);
    }
    state.SetItemsProcessed(state.iterations() * n);
}
BENCHMARK(BM_RingBuffer_Iterate)
    ->Arg(1000)
    ->Arg(10000)
    ->Arg(100000);

// ---------------------------------------------------------------------------
// Reverse iteration — tests rbegin()/rend() with wrapped buffer
// ---------------------------------------------------------------------------
static void BM_RingBuffer_ReverseIterate(benchmark::State& state) {
    const i64 n = state.range(0);
    RingBuffer<u64> rb(static_cast<u64>(n));
    for (i64 i = 0; i < n; ++i) rb.push_back(static_cast<u64>(i));

    for (auto _ : state) {
      u64 sum = 0;
      for (auto it = rb.rbegin(); it != rb.rend(); ++it)
        sum += (*it);
      benchmark::DoNotOptimize(sum);
    }
    state.SetItemsProcessed(state.iterations() * n);
}
BENCHMARK(BM_RingBuffer_ReverseIterate)
    ->Arg(1000)
    ->Arg(10000)
    ->Arg(100000);

// ---------------------------------------------------------------------------
// Iterate after wrap — fill, drain half, refill, then iterate
// This ensures the head > tail wrap case is exercised
// ---------------------------------------------------------------------------
static void BM_RingBuffer_IterateWrapped(benchmark::State& state) {
    const i64 n = state.range(0);
    for (auto _ : state) {
        RingBuffer<u64> rb(static_cast<u64>(n));
        // fill
        for (i64 i = 0; i < n; ++i) rb.push_back(static_cast<u64>(i));
        // drain half — forces head to advance, tail stays at end
        for (i64 i = 0; i < n / 2; ++i) rb.pop_front();
        // refill — tail wraps around
        for (i64 i = 0; i < n / 2; ++i) rb.push_back(static_cast<u64>(i));

        u64 sum = 0;
        for (auto& v : rb) sum += v;
        benchmark::DoNotOptimize(sum);
    }
    state.SetItemsProcessed(state.iterations() * n);
}
BENCHMARK(BM_RingBuffer_IterateWrapped)
    ->Arg(1000)
    ->Arg(10000)
    ->Arg(100000);
