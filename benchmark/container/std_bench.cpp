#include <benchmark/benchmark.h>

#include <string>
#include <unordered_map>
#include <vector>

#include "core/container/array/array.hpp"
#include "core/container/hashmap/hashmap.hpp"
#include "core/container/string/string.hpp"

// Side-by-side comparison of the custom containers vs the C++ standard
// library. Both sides use default (auto-growing) behavior.

// ============================================================================
// Array vs std::vector
// ============================================================================

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
    ->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Arg(1000000);

static void BM_StdVector_PushBack(benchmark::State& state) {
    const i64 n = state.range(0);
    for (auto _ : state) {
        std::vector<u64> v;
        for (i64 i = 0; i < n; ++i) {
            v.push_back(static_cast<u64>(i));
        }
        benchmark::DoNotOptimize(v.data());
    }
    state.SetItemsProcessed(state.iterations() * n);
}
BENCHMARK(BM_StdVector_PushBack)
    ->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Arg(1000000);

static void BM_Array_InsertMiddle(benchmark::State& state) {
    const i64 n = state.range(0);
    for (auto _ : state) {
        Array<u64> arr;
        for (i64 i = 0; i < n; ++i) {
            arr.push_back(static_cast<u64>(i));
        }
        for (i64 i = 0; i < 100; ++i) {
            arr.insert(arr.size() / 2, 0);
        }
        benchmark::DoNotOptimize(arr.data());
    }
    state.SetItemsProcessed(state.iterations() * 100);
}
BENCHMARK(BM_Array_InsertMiddle)->Arg(100)->Arg(1000)->Arg(10000);

static void BM_StdVector_InsertMiddle(benchmark::State& state) {
    const i64 n = state.range(0);
    for (auto _ : state) {
        std::vector<u64> v;
        for (i64 i = 0; i < n; ++i) {
            v.push_back(static_cast<u64>(i));
        }
        for (i64 i = 0; i < 100; ++i) {
            v.insert(v.begin() + static_cast<std::ptrdiff_t>(v.size() / 2), 0);
        }
        benchmark::DoNotOptimize(v.data());
    }
    state.SetItemsProcessed(state.iterations() * 100);
}
BENCHMARK(BM_StdVector_InsertMiddle)->Arg(100)->Arg(1000)->Arg(10000);

static void BM_Array_EraseFront(benchmark::State& state) {
    const i64 n = state.range(0);
    for (auto _ : state) {
        Array<u64> arr;
        for (i64 i = 0; i < n; ++i) {
            arr.push_back(static_cast<u64>(i));
        }
        for (i64 i = 0; i < 100; ++i) {
            arr.erase(0);
        }
        benchmark::DoNotOptimize(arr.data());
    }
    state.SetItemsProcessed(state.iterations() * 100);
}
BENCHMARK(BM_Array_EraseFront)->Arg(100)->Arg(1000)->Arg(10000);

static void BM_StdVector_EraseFront(benchmark::State& state) {
    const i64 n = state.range(0);
    for (auto _ : state) {
        std::vector<u64> v;
        for (i64 i = 0; i < n; ++i) {
            v.push_back(static_cast<u64>(i));
        }
        for (i64 i = 0; i < 100; ++i) {
            v.erase(v.begin());
        }
        benchmark::DoNotOptimize(v.data());
    }
    state.SetItemsProcessed(state.iterations() * 100);
}
BENCHMARK(BM_StdVector_EraseFront)->Arg(100)->Arg(1000)->Arg(10000);

static void BM_Array_Iterate(benchmark::State& state) {
    const i64 n = state.range(0);
    Array<u64> arr;
    for (i64 i = 0; i < n; ++i) {
        arr.push_back(static_cast<u64>(i));
    }
    u64 sum = 0;
    for (auto _ : state) {
        for (u64 v : arr) {
            sum += v;
        }
    }
    benchmark::DoNotOptimize(sum);
    state.SetItemsProcessed(state.iterations() * n);
}
BENCHMARK(BM_Array_Iterate)->Arg(1000)->Arg(10000)->Arg(100000)->Arg(1000000);

static void BM_StdVector_Iterate(benchmark::State& state) {
    const i64 n = state.range(0);
    std::vector<u64> v;
    for (i64 i = 0; i < n; ++i) {
        v.push_back(static_cast<u64>(i));
    }
    u64 sum = 0;
    for (auto _ : state) {
        for (u64 x : v) {
            sum += x;
        }
    }
    benchmark::DoNotOptimize(sum);
    state.SetItemsProcessed(state.iterations() * n);
}
BENCHMARK(BM_StdVector_Iterate)->Arg(1000)->Arg(10000)->Arg(100000)->Arg(1000000);

// ============================================================================
// HashMap vs std::unordered_map
// ============================================================================

static void BM_HashMap_Insert(benchmark::State& state) {
    const i64 n = state.range(0);
    for (auto _ : state) {
        HashMap<u64, u64> map(16);
        for (i64 i = 0; i < n; ++i) {
            map.insert(static_cast<u64>(i), 1);
        }
        benchmark::DoNotOptimize(map.size());
    }
    state.SetItemsProcessed(state.iterations() * n);
}
BENCHMARK(BM_HashMap_Insert)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000);

static void BM_StdUnorderedMap_Insert(benchmark::State& state) {
    const i64 n = state.range(0);
    for (auto _ : state) {
        std::unordered_map<u64, u64> map;
        for (i64 i = 0; i < n; ++i) {
            map.emplace(static_cast<u64>(i), 1);
        }
        benchmark::DoNotOptimize(map.size());
    }
    state.SetItemsProcessed(state.iterations() * n);
}
BENCHMARK(BM_StdUnorderedMap_Insert)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000);

static void BM_HashMap_Contains(benchmark::State& state) {
    const i64 n = state.range(0);
    HashMap<u64, u64> map(16);
    for (i64 i = 0; i < n; ++i) {
        map.insert(static_cast<u64>(i), static_cast<u64>(i));
    }
    u64 found = 0;
    for (auto _ : state) {
        for (i64 i = 0; i < n; ++i) {
            found += map.contains(static_cast<u64>(i)) ? 1 : 0;
        }
    }
    benchmark::DoNotOptimize(found);
    state.SetItemsProcessed(state.iterations() * n);
}
BENCHMARK(BM_HashMap_Contains)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000);

static void BM_StdUnorderedMap_Contains(benchmark::State& state) {
    const i64 n = state.range(0);
    std::unordered_map<u64, u64> map(16);
    for (i64 i = 0; i < n; ++i) {
        map.emplace(static_cast<u64>(i), static_cast<u64>(i));
    }
    u64 found = 0;
    for (auto _ : state) {
        for (i64 i = 0; i < n; ++i) {
            found += map.count(static_cast<u64>(i)) ? 1 : 0;
        }
    }
    benchmark::DoNotOptimize(found);
    state.SetItemsProcessed(state.iterations() * n);
}
BENCHMARK(BM_StdUnorderedMap_Contains)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000);

// ============================================================================
// String vs std::string
// ============================================================================

static void BM_String_Construct(benchmark::State& state) {
    const i64 len = state.range(0);
    const std::string base(static_cast<size_t>(len), 'x');
    for (auto _ : state) {
        String s(base.c_str());
        benchmark::DoNotOptimize(s.c_str());
    }
    state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_String_Construct)->Arg(8)->Arg(23)->Arg(24)->Arg(64)->Arg(256)->Arg(1024);

static void BM_StdString_Construct(benchmark::State& state) {
    const i64 len = state.range(0);
    const std::string base(static_cast<size_t>(len), 'x');
    for (auto _ : state) {
        std::string s(base);
        benchmark::DoNotOptimize(s.data());
    }
    state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_StdString_Construct)->Arg(8)->Arg(23)->Arg(24)->Arg(64)->Arg(256)->Arg(1024);

static void BM_String_Append(benchmark::State& state) {
    const i64 n = state.range(0);
    for (auto _ : state) {
        String s;
        for (i64 i = 0; i < n; ++i) {
            s += "a";
        }
        benchmark::DoNotOptimize(s.c_str());
    }
    state.SetItemsProcessed(state.iterations() * n);
}
BENCHMARK(BM_String_Append)->Arg(10)->Arg(100)->Arg(1000)->Arg(10000);

static void BM_StdString_Append(benchmark::State& state) {
    const i64 n = state.range(0);
    for (auto _ : state) {
        std::string s;
        for (i64 i = 0; i < n; ++i) {
            s += 'a';
        }
        benchmark::DoNotOptimize(s.data());
    }
    state.SetItemsProcessed(state.iterations() * n);
}
BENCHMARK(BM_StdString_Append)->Arg(10)->Arg(100)->Arg(1000)->Arg(10000);
