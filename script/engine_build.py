#!/usr/bin/env python3
"""Builds the engine executable. Debug by default; pass 'release' for Release."""

import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
CONFIG = sys.argv[1] if len(sys.argv) > 1 and sys.argv[1] == "release" else "Debug"
BUILD_DIR = ROOT / "out" / CONFIG


def run(cmd):
    print(f"+ {' '.join(cmd)}")
    subprocess.run(cmd, cwd=ROOT, check=True)


def main():
    run(["cmake", "-S", str(ROOT), "-B", str(BUILD_DIR), "-G", "Ninja",
         f"-DCMAKE_BUILD_TYPE={CONFIG}"])
    run(["cmake", "--build", str(BUILD_DIR), "--target", "supervk"])


if __name__ == "__main__":
    main()
