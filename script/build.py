#!/usr/bin/env python3
"""One-click build: configures and builds Debug (engine + unit tests) and
Release (benchmarks) by default.

Usage:
  python script/build.py            # build Debug + Release
  python script/build.py debug      # build Debug only
  python script/build.py release    # build Release only
  python script/build.py clean      # remove out/Debug and out/Release
"""

import shutil
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
DEBUG_DIR = ROOT / "out" / "Debug"
RELEASE_DIR = ROOT / "out" / "Release"


def run(cmd):
    print(f"+ {' '.join(cmd)}")
    subprocess.run(cmd, cwd=ROOT, check=True)


def configure_and_build(build_dir):
    build_type = build_dir.name  # "Debug" / "Release"
    run(["cmake", "-S", str(ROOT), "-B", str(build_dir), "-G", "Ninja",
         f"-DCMAKE_BUILD_TYPE={build_type}"])
    run(["cmake", "--build", str(build_dir)])


def clean():
    for d in (DEBUG_DIR, RELEASE_DIR):
        if d.is_dir():
            print(f"removing {d}")
            shutil.rmtree(d)


def main():
    what = sys.argv[1] if len(sys.argv) > 1 else "all"
    if what == "clean":
        clean()
        return
    if what in ("all", "debug", "engine", "test"):
        print("=== Debug build (engine + unit tests) ===")
        configure_and_build(DEBUG_DIR)
    if what in ("all", "release", "benchmark"):
        print("=== Release build (benchmarks) ===")
        configure_and_build(RELEASE_DIR)


if __name__ == "__main__":
    main()
