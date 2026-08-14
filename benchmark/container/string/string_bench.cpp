#include <benchmark/benchmark.h>

#include "core/container/string/string.hpp"
#include "core/container/string/string_view.hpp"

// ---------------------------------------------------------------------------
// Construct from C-string — measures alloc + copy
// ---------------------------------------------------------------------------
static void BM_String_Construct(benchmark::State& state) {
    const i64 n = state.range(0);
    char* buf = new char[static_cast<u64>(n) + 1];
    memset(buf, 'x', static_cast<u64>(n));
    buf[n] = '\0';

    for (auto _ : state) {
        String s(buf);
        benchmark::DoNotOptimize(s.c_str());
    }
    delete[] buf;
    state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_String_Construct)
    ->Arg(8)     // SSO
    ->Arg(23)    // SSO boundary
    ->Arg(64)    // heap
    ->Arg(256)   // heap
    ->Arg(1024); // heap

// ---------------------------------------------------------------------------
// Append — measure growth via operator+=
// ---------------------------------------------------------------------------
static void BM_String_Append(benchmark::State& state) {
    const i64 n = state.range(0);
    for (auto _ : state) {
        String s;
        for (i64 i = 0; i < n; ++i) {
            s.append("a");
        }
        benchmark::DoNotOptimize(s.c_str());
    }
    state.SetItemsProcessed(state.iterations() * n);
}
BENCHMARK(BM_String_Append)
    ->Arg(10)
    ->Arg(100)
    ->Arg(1000)
    ->Arg(10000);

// ---------------------------------------------------------------------------
// Insert — insert characters at various positions
// ---------------------------------------------------------------------------
static void BM_String_InsertFront(benchmark::State& state) {
    const i64 n = state.range(0);
    for (auto _ : state) {
        String s;
        for (i64 i = 0; i < n; ++i) {
            s.insert(0, "a");
        }
        benchmark::DoNotOptimize(s.c_str());
    }
    state.SetItemsProcessed(state.iterations() * n);
}
BENCHMARK(BM_String_InsertFront)->Arg(10)->Arg(100)->Arg(1000);

static void BM_String_InsertMiddle(benchmark::State& state) {
    const i64 n = state.range(0);
    for (auto _ : state) {
        String s;
        for (i64 i = 0; i < n; ++i) {
            s.insert(s.size() / 2, "abc");
        }
        benchmark::DoNotOptimize(s.c_str());
    }
    state.SetItemsProcessed(state.iterations() * n);
}
BENCHMARK(BM_String_InsertMiddle)->Arg(10)->Arg(100)->Arg(500);

// ---------------------------------------------------------------------------
// Erase — remove characters from various positions
// ---------------------------------------------------------------------------
static void BM_String_Erase(benchmark::State& state) {
    const i64 n = state.range(0);
    for (auto _ : state) {
        String s(static_cast<u64>(n), 'x');
        s.erase(0, s.size());
        benchmark::DoNotOptimize(s.size());
    }
    state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_String_Erase)->Arg(100)->Arg(1000)->Arg(10000);

// ---------------------------------------------------------------------------
// Find — substring search
// ---------------------------------------------------------------------------
static void BM_String_Find(benchmark::State& state) {
    const i64 n = state.range(0);
    String s(static_cast<u64>(n), 'a');
    s.push_back('!'); // unique char at the end
    s.insert(0, "needle");

    for (auto _ : state) {
        u64 pos = s.find("needle");
        benchmark::DoNotOptimize(pos);
    }
    state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_String_Find)->Arg(100)->Arg(1000)->Arg(10000);

// ---------------------------------------------------------------------------
// Replace — character and substring replacement
// ---------------------------------------------------------------------------
static void BM_String_ReplaceChar(benchmark::State& state) {
    const i64 n = state.range(0);
    String s(static_cast<u64>(n), 'x');

    for (auto _ : state) {
        String copy = s;
        copy.replace('x', 'y');
        benchmark::DoNotOptimize(copy.c_str());
    }
    state.SetItemsProcessed(state.iterations() * n);
}
BENCHMARK(BM_String_ReplaceChar)->Arg(100)->Arg(1000)->Arg(10000);

// ---------------------------------------------------------------------------
// Substr — extract substring
// ---------------------------------------------------------------------------
static void BM_String_Substr(benchmark::State& state) {
    const i64 n = state.range(0);
    String s(static_cast<u64>(n), 'x');

    for (auto _ : state) {
        String sub = s.substr(n / 4, n / 2);
        benchmark::DoNotOptimize(sub.c_str());
    }
    state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_String_Substr)->Arg(100)->Arg(1000)->Arg(10000);

// ---------------------------------------------------------------------------
// SSO vs Heap — compare operations inside and outside SSO boundary
// ---------------------------------------------------------------------------
static void BM_String_SSOBoundary(benchmark::State& state) {
    const i64 n = state.range(0);
    for (auto _ : state) {
        String s(static_cast<u64>(n), 'a');
        benchmark::DoNotOptimize(s.c_str());
    }
    state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_String_SSOBoundary)
    ->Arg(23)   // max SSO (23 chars + null)
    ->Arg(24);  // heap allocation triggered

// ---------------------------------------------------------------------------
// Compare — string equality check
// ---------------------------------------------------------------------------
static void BM_String_Compare(benchmark::State& state) {
    const i64 n = state.range(0);
    String a(static_cast<u64>(n), 'x');
    String b(static_cast<u64>(n), 'x');

    for (auto _ : state) {
        b32 eq = (a.compare(b) == 0);
        benchmark::DoNotOptimize(eq);
    }
    state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_String_Compare)->Arg(16)->Arg(64)->Arg(256)->Arg(1024);

// ---------------------------------------------------------------------------
// StartsWith / EndsWith — prefix/suffix checks
// ---------------------------------------------------------------------------
static void BM_String_StartsWith(benchmark::State& state) {
    const i64 n = state.range(0);
    String s(static_cast<u64>(n), 'x');
    char prefix[32];
    memset(prefix, 'x', 16);
    prefix[16] = '\0';

    for (auto _ : state) {
        b32 ok = s.starts_with(prefix);
        benchmark::DoNotOptimize(ok);
    }
    state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_String_StartsWith)->Arg(100)->Arg(1000)->Arg(10000);

// ---------------------------------------------------------------------------
// StringView — non-owning find operations
// ---------------------------------------------------------------------------
static void BM_StringView_Find(benchmark::State& state) {
    const i64 n = state.range(0);
    String s(static_cast<u64>(n), 'x');
    s.push_back('!');
    s.insert(0, "target");
    StringView sv(s);

    for (auto _ : state) {
        u64 pos = sv.find("target");
        benchmark::DoNotOptimize(pos);
    }
    state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_StringView_Find)->Arg(100)->Arg(1000)->Arg(10000);

static void BM_StringView_StartsWith(benchmark::State& state) {
    const i64 n = state.range(0);
    String s(static_cast<u64>(n), 'x');
    StringView sv(s);

    for (auto _ : state) {
        b32 ok = sv.starts_with('x');
        benchmark::DoNotOptimize(ok);
    }
    state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_StringView_StartsWith)->Arg(100)->Arg(1000)->Arg(10000);
