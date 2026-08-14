#!/usr/bin/env python3
"""Builds unit tests (Debug, with ASan)."""

import subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
BUILD_DIR = ROOT / "out" / "Debug"


def run(cmd):
    print(f"+ {' '.join(cmd)}")
    subprocess.run(cmd, cwd=ROOT, check=True)


def main():
    run(["cmake", "-S", str(ROOT), "-B", str(BUILD_DIR), "-G", "Ninja",
         "-DCMAKE_BUILD_TYPE=Debug"])
    run(["cmake", "--build", str(BUILD_DIR)])


if __name__ == "__main__":
    main()
