#include <benchmark/benchmark.h>

#include "core/container/hashmap/hashmap.hpp"

// ---------------------------------------------------------------------------
// Insert — measure insertion of unique K,V pairs (amortized O(1) avg)
// Rehash triggers at load factor 0.5, capacity doubles (power-of-2)
// ---------------------------------------------------------------------------
static void BM_HashMap_Insert(benchmark::State &state) {
  const i64 n = state.range(0);
  for (auto _ : state) {
    HashMap<u64, u64> map(16);
    for (i64 i = 0; i < n; ++i) {
      map.insert(static_cast<u64>(i), static_cast<u64>(i));
    }
    benchmark::DoNotOptimize(map.size());
  }
  state.SetItemsProcessed(state.iterations() * n);
}
BENCHMARK(BM_HashMap_Insert)
    ->Arg(100)
    ->Arg(1000)
    ->Arg(10000)
    ->Arg(100000)
    ->Arg(1000000);

// ---------------------------------------------------------------------------
// Insert with pre-reserved capacity — no rehash during insertion
// ---------------------------------------------------------------------------
static void BM_HashMap_InsertReserved(benchmark::State &state) {
  const i64 n = state.range(0);
  for (auto _ : state) {
    // capacity rounds up to next power-of-2; we need cap >= n / 0.5 = 2n
    HashMap<u64, u64> map(static_cast<u64>(n) * 2);
    for (i64 i = 0; i < n; ++i) {
      map.insert(static_cast<u64>(i), static_cast<u64>(i));
    }
    benchmark::DoNotOptimize(map.size());
  }
  state.SetItemsProcessed(state.iterations() * n);
}
BENCHMARK(BM_HashMap_InsertReserved)
    ->Arg(100)
    ->Arg(1000)
    ->Arg(10000)
    ->Arg(100000)
    ->Arg(1000000);

// ---------------------------------------------------------------------------
// Find — lookup existing keys (cache-unfriendly probe sequence)
// ---------------------------------------------------------------------------
static void BM_HashMap_Find(benchmark::State &state) {
  const i64 n = state.range(0);
  HashMap<u64, u64> map(static_cast<u64>(n) * 2);
  for (i64 i = 0; i < n; ++i)
    map.insert(static_cast<u64>(i), static_cast<u64>(i));

  for (auto _ : state) {
    u64 sum = 0;
    for (i64 i = 0; i < n; ++i) {
      auto it = map.find(static_cast<u64>(i));
      sum += it.second();
    }
    benchmark::DoNotOptimize(sum);
  }
  state.SetItemsProcessed(state.iterations() * n);
}
BENCHMARK(BM_HashMap_Find)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000);

// ---------------------------------------------------------------------------
// Find missing — lookup non-existent keys (probing hits empty slots)
// ---------------------------------------------------------------------------
static void BM_HashMap_FindMissing(benchmark::State &state) {
  const i64 n = state.range(0);
  HashMap<u64, u64> map(static_cast<u64>(n) * 2);
  for (i64 i = 0; i < n; ++i)
    map.insert(static_cast<u64>(i), static_cast<u64>(i));

  for (auto _ : state) {
    u64 count = 0;
    for (i64 i = 0; i < n; ++i) {
      auto it = map.find(static_cast<u64>(i + n)); // all missing
      if (it != map.end())
        ++count;
    }
    benchmark::DoNotOptimize(count);
  }
  state.SetItemsProcessed(state.iterations() * n);
}
BENCHMARK(BM_HashMap_FindMissing)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000);

// ---------------------------------------------------------------------------
// Insert with pre-reserved capacity — no rehash during insertion
// NOTE: emplace uses variadic args to construct HashEntry<K,V> internally,
// but the implementation calls insert(Entry&&) which has no matching overload.
// Use insert() directly.
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// Erase — remove keys one by one
// ---------------------------------------------------------------------------
static void BM_HashMap_Erase(benchmark::State &state) {
  const i64 n = state.range(0);
  for (auto _ : state) {
    HashMap<u64, u64> map(static_cast<u64>(n) * 2);
    for (i64 i = 0; i < n; ++i)
      map.insert(static_cast<u64>(i), static_cast<u64>(i));
    for (i64 i = 0; i < n; ++i) {
      map.erase(static_cast<u64>(i));
    }
    benchmark::DoNotOptimize(map.size());
  }
  state.SetItemsProcessed(state.iterations() * n);
}
BENCHMARK(BM_HashMap_Erase)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000);

// ---------------------------------------------------------------------------
// Operator[] — access/insert via subscript
// ---------------------------------------------------------------------------
static void BM_HashMap_OperatorBracket(benchmark::State &state) {
  const i64 n = state.range(0);
  HashMap<u64, u64> map(static_cast<u64>(n) * 2);
  for (i64 i = 0; i < n; ++i)
    map.insert(static_cast<u64>(i), static_cast<u64>(i));

  for (auto _ : state) {
    u64 sum = 0;
    for (i64 i = 0; i < n; ++i) {
      sum += map[static_cast<u64>(i)];
    }
    benchmark::DoNotOptimize(sum);
  }
  state.SetItemsProcessed(state.iterations() * n);
}
BENCHMARK(BM_HashMap_OperatorBracket)
    ->Arg(100)
    ->Arg(1000)
    ->Arg(10000)
    ->Arg(100000);

// ---------------------------------------------------------------------------
// Contains — membership test
// ---------------------------------------------------------------------------
static void BM_HashMap_Contains(benchmark::State &state) {
  const i64 n = state.range(0);
  HashMap<u64, u64> map(static_cast<u64>(n) * 2);
  for (i64 i = 0; i < n; ++i)
    map.insert(static_cast<u64>(i), static_cast<u64>(i));

  for (auto _ : state) {
    u64 count = 0;
    for (i64 i = 0; i < n; ++i) {
      if (map.contains(static_cast<u64>(i)))
        ++count;
    }
    benchmark::DoNotOptimize(count);
  }
  state.SetItemsProcessed(state.iterations() * n);
}
BENCHMARK(BM_HashMap_Contains)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000);

// ---------------------------------------------------------------------------
// Iterate — walk all occupied entries (skips empty slots in probe array)
// ---------------------------------------------------------------------------
static void BM_HashMap_Iterate(benchmark::State &state) {
  const i64 n = state.range(0);
  HashMap<u64, u64> map(static_cast<u64>(n) * 2);
  for (i64 i = 0; i < n; ++i)
    map.insert(static_cast<u64>(i), static_cast<u64>(i));

  for (auto _ : state) {
    u64 sum = 0;
    for (auto &entry : map) {
      sum += entry.key;
      sum += entry.val;
    }
    benchmark::DoNotOptimize(sum);
  }
  state.SetItemsProcessed(state.iterations() * n);
}
BENCHMARK(BM_HashMap_Iterate)
    ->Arg(100)
    ->Arg(1000)
    ->Arg(10000)
    ->Arg(100000)
    ->Arg(1000000);

// ---------------------------------------------------------------------------
// Rehash — forced resizing of a populated map
// ---------------------------------------------------------------------------
static void BM_HashMap_Rehash(benchmark::State &state) {
  const i64 n = state.range(0);

  for (auto _ : state) {
    HashMap<u64, u64> map(n * 2);

    for (i64 i = 0; i < n; ++i)
      map.insert(i, i);

    state.PauseTiming();
    benchmark::DoNotOptimize(map);
    state.ResumeTiming();

    map.rehash(map.capacity() * 2);

    benchmark::DoNotOptimize(map.size());
  }
  state.SetItemsProcessed(state.iterations() * n);
}
BENCHMARK(BM_HashMap_Rehash)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000);

// ---------------------------------------------------------------------------
// String keys — measure hash + comparison cost on string keys
// ---------------------------------------------------------------------------
static void BM_HashMap_StringKeys(benchmark::State &state) {
  const i64 n = state.range(0);
  // Pre-build key strings
  Array<String> keys;
  keys.reserve(static_cast<u64>(n));
  for (i64 i = 0; i < n; ++i) {
    char buf[32];
    snprintf(buf, sizeof(buf), "key_%lld", static_cast<long long>(i));
    keys.emplace_back(buf);
  }

  for (auto _ : state) {
    HashMap<String, u64> map(static_cast<u64>(n) * 2);
    for (i64 i = 0; i < n; ++i) {
      map.insert(keys[static_cast<u64>(i)], static_cast<u64>(i));
    }
    benchmark::DoNotOptimize(map.size());
  }
  state.SetItemsProcessed(state.iterations() * n);
}
BENCHMARK(BM_HashMap_StringKeys)->Arg(100)->Arg(1000)->Arg(10000);
