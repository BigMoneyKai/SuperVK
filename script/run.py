#!/usr/bin/env python3
"""One-click run: runs all unit tests (Debug) first, then all benchmarks
(Release).

Usage:
  python script/run.py               # unit tests + benchmarks
  python script/run.py test          # unit tests only
  python script/run.py benchmark     # benchmarks only
  python script/run.py engine        # launch the engine window
"""

import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
DEBUG_DIR = ROOT / "out" / "Debug"
RELEASE_DIR = ROOT / "out" / "Release"


def run(cmd):
    print(f"+ {' '.join(cmd)}")
    subprocess.run(cmd, cwd=ROOT, check=True)


def main():
    what = sys.argv[1] if len(sys.argv) > 1 else "all"
    if what in ("all", "test", "tests"):
        if not DEBUG_DIR.is_dir():
            sys.exit("out/Debug does not exist, run first: python script/build.py")
        print("=== Unit tests (Debug) ===")
        run(["ctest", "--test-dir", str(DEBUG_DIR), "-LE", "benchmark",
             "-j8", "--output-on-failure"])
    if what in ("all", "benchmark", "benchmarks"):
        if not RELEASE_DIR.is_dir():
            sys.exit("out/Release does not exist, run first: python script/build.py")
        print("=== Benchmark (Release) ===")
        run([sys.executable, str(ROOT / "script" / "benchmark_run.py")])
    if what == "engine":
        exe = DEBUG_DIR / "src" / "supervk"
        if not exe.is_file():
            sys.exit(f"{exe} does not exist, run first: python script/build.py")
        print("=== Engine (Debug) ===")
        subprocess.run([str(exe)], cwd=ROOT, check=True)


if __name__ == "__main__":
    main()
