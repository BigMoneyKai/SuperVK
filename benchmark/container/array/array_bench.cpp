#include <benchmark/benchmark.h>

#include "core/container/array/array.hpp"

// ---------------------------------------------------------------------------
// push_back — measure insertion at the tail (amortized O(1), 1.5x growth)
// ---------------------------------------------------------------------------
static void BM_Array_PushBack(benchmark::State& state) {
    const i64 n = state.range(0);
    for (auto _ : state) {
        Array<u64> arr;
        for (i64 i = 0; i < n; ++i) {
            arr.push_back(static_cast<u64>(i));
        }
        benchmark::DoNotOptimize(arr.data());
    }
    state.SetItemsProcessed(state.iterations() * n);
}
BENCHMARK(BM_Array_PushBack)
    ->Arg(100)
    ->Arg(1000)
    ->Arg(10000)
    ->Arg(100000)
    ->Arg(1000000);

// ---------------------------------------------------------------------------
// push_back with pre-reserved capacity — isolates the copy cost
// ---------------------------------------------------------------------------
static void BM_Array_PushBackReserved(benchmark::State& state) {
    const i64 n = state.range(0);
    for (auto _ : state) {
        Array<u64> arr;
        arr.reserve(static_cast<u64>(n));
        for (i64 i = 0; i < n; ++i) {
            arr.push_back(static_cast<u64>(i));
        }
        benchmark::DoNotOptimize(arr.data());
    }
    state.SetItemsProcessed(state.iterations() * n);
}
BENCHMARK(BM_Array_PushBackReserved)
    ->Arg(100)
    ->Arg(1000)
    ->Arg(10000)
    ->Arg(100000)
    ->Arg(1000000);

// ---------------------------------------------------------------------------
// emplace_back — in-place construction vs push_back copy
// ---------------------------------------------------------------------------
static void BM_Array_EmplaceBack(benchmark::State& state) {
    const i64 n = state.range(0);
    for (auto _ : state) {
        Array<u64> arr;
        for (i64 i = 0; i < n; ++i) {
            arr.emplace_back(static_cast<u64>(i));
        }
        benchmark::DoNotOptimize(arr.data());
    }
    state.SetItemsProcessed(state.iterations() * n);
}
BENCHMARK(BM_Array_EmplaceBack)
    ->Arg(100)
    ->Arg(1000)
    ->Arg(10000)
    ->Arg(100000)
    ->Arg(1000000);

// ---------------------------------------------------------------------------
// operator[] — random access pattern (cache-friendly sequential)
// ---------------------------------------------------------------------------
static void BM_Array_Access(benchmark::State& state) {
    const i64 n = state.range(0);
    Array<u64> arr;
    arr.reserve(static_cast<u64>(n));
    for (i64 i = 0; i < n; ++i) arr.push_back(static_cast<u64>(i));

    for (auto _ : state) {
        u64 sum = 0;
        for (i64 i = 0; i < n; ++i) {
            sum += arr[static_cast<u64>(i)];
        }
        benchmark::DoNotOptimize(sum);
    }
    state.SetItemsProcessed(state.iterations() * n);
}
BENCHMARK(BM_Array_Access)
    ->Arg(1000)
    ->Arg(10000)
    ->Arg(100000);

// ---------------------------------------------------------------------------
// Iterate — range-based for / iterator traversal
// ---------------------------------------------------------------------------
static void BM_Array_Iterate(benchmark::State& state) {
    const i64 n = state.range(0);
    Array<u64> arr;
    arr.reserve(static_cast<u64>(n));
    for (i64 i = 0; i < n; ++i) arr.push_back(static_cast<u64>(i));

    for (auto _ : state) {
        u64 sum = 0;
        for (auto& v : arr) {
            sum += v;
        }
        benchmark::DoNotOptimize(sum);
    }
    state.SetItemsProcessed(state.iterations() * n);
}
BENCHMARK(BM_Array_Iterate)
    ->Arg(1000)
    ->Arg(10000)
    ->Arg(100000);

// ---------------------------------------------------------------------------
// Insert at front, middle, back — measures memmove cost
// ---------------------------------------------------------------------------
static void BM_Array_InsertFront(benchmark::State& state) {
    const i64 n = state.range(0);
    for (auto _ : state) {
        Array<u64> arr;
        for (i64 i = 0; i < n; ++i) {
            arr.insert(0, static_cast<u64>(i));
        }
        benchmark::DoNotOptimize(arr.data());
    }
    state.SetItemsProcessed(state.iterations() * n);
}
BENCHMARK(BM_Array_InsertFront)->Arg(100)->Arg(1000)->Arg(10000);

static void BM_Array_InsertMiddle(benchmark::State& state) {
    const i64 n = state.range(0);
    for (auto _ : state) {
        Array<u64> arr;
        for (i64 i = 0; i < n; ++i) {
            arr.insert(arr.size() / 2, static_cast<u64>(i));
        }
        benchmark::DoNotOptimize(arr.data());
    }
    state.SetItemsProcessed(state.iterations() * n);
}
BENCHMARK(BM_Array_InsertMiddle)->Arg(100)->Arg(1000)->Arg(10000);

// ---------------------------------------------------------------------------
// Erase from front, middle, back — measures memmove cost
// ---------------------------------------------------------------------------
static void BM_Array_EraseFront(benchmark::State& state) {
    const i64 n = state.range(0);
    for (auto _ : state) {
        Array<u64> arr;
        arr.reserve(static_cast<u64>(n));
        for (i64 i = 0; i < n; ++i) arr.push_back(static_cast<u64>(i));
        for (i64 i = 0; i < n; ++i) {
            arr.erase(0);
        }
        benchmark::DoNotOptimize(arr.data());
    }
    state.SetItemsProcessed(state.iterations() * n);
}
BENCHMARK(BM_Array_EraseFront)->Arg(100)->Arg(1000)->Arg(10000);

static void BM_Array_EraseBack(benchmark::State& state) {
    const i64 n = state.range(0);
    for (auto _ : state) {
        Array<u64> arr;
        arr.reserve(static_cast<u64>(n));
        for (i64 i = 0; i < n; ++i) arr.push_back(static_cast<u64>(i));
        for (i64 i = 0; i < n; ++i) {
            arr.erase(arr.size() - 1);
        }
        benchmark::DoNotOptimize(arr.data());
    }
    state.SetItemsProcessed(state.iterations() * n);
}
BENCHMARK(BM_Array_EraseBack)->Arg(100)->Arg(1000)->Arg(10000);

// ---------------------------------------------------------------------------
// Clear + reuse — measures clear overhead and subsequent re-push
// ---------------------------------------------------------------------------
static void BM_Array_Clear(benchmark::State& state) {
    const i64 n = state.range(0);
    for (auto _ : state) {
        Array<u64> arr;
        arr.reserve(static_cast<u64>(n));
        for (i64 i = 0; i < n; ++i) arr.push_back(static_cast<u64>(i));
        arr.clear();
        benchmark::DoNotOptimize(arr.data());
    }
    state.SetItemsProcessed(state.iterations() * n);
}
BENCHMARK(BM_Array_Clear)->Arg(1000)->Arg(10000)->Arg(100000);

// ---------------------------------------------------------------------------
// Reserve — measure allocation + growth overhead
// ---------------------------------------------------------------------------
static void BM_Array_Reserve(benchmark::State& state) {
    const i64 n = state.range(0);
    for (auto _ : state) {
        Array<u64> arr;
        arr.reserve(static_cast<u64>(n));
        benchmark::DoNotOptimize(arr.data());
    }
    state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_Array_Reserve)->Arg(1000)->Arg(10000)->Arg(100000)->Arg(1000000);
