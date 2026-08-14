#!/usr/bin/env python3
"""Runs all benchmarks (Release) and prints each case's benchmark timing.

The binaries are executed directly so Google Benchmark prints its own
per-case report (ns/op, items_per_second) instead of CTest wall-clock times.
"""

import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
RELEASE_DIR = ROOT / "out" / "Release"

BENCHMARKS = [
    "benchmark_container_array",
    "benchmark_container_hashmap",
    "benchmark_container_ringbuffer",
    "benchmark_container_string",
    "benchmark_container_std",
    "benchmark_memory_heap",
    "benchmark_memory_linear",
    "benchmark_memory_pool",
    "benchmark_memory_stack",
    "benchmark_filesystem",
]


def find_binary(name):
    for sub in ("container", "memory", "filesystem"):
        candidate = RELEASE_DIR / "benchmark" / sub / name
        if candidate.is_file():
            return candidate
    for candidate in (RELEASE_DIR / "benchmark" / name,
                      ROOT / "out" / "benchmark" / name):
        if candidate.is_file():
            return candidate
    return None


def main():
    if not RELEASE_DIR.is_dir():
        sys.exit("out/Release does not exist, run first: python script/benchmark_build.py")

    # Fast by default; pass --full for longer, more stable runs.
    extra = [] if "--full" in sys.argv else ["--benchmark_min_time=0.1s"]

    for name in BENCHMARKS:
        exe = find_binary(name)
        if exe is None:
            print(f"[skip] {name}: binary not found (run python script/benchmark_build.py)")
            continue
        print(f"\n===== {name} =====")
        subprocess.run([str(exe), *extra], cwd=ROOT, check=True)


if __name__ == "__main__":
    main()
