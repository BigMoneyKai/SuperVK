#!/usr/bin/env python3
"""Runs all unit tests (Debug, excluding benchmarks)."""

import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
BUILD_DIR = ROOT / "out" / "Debug"

if not BUILD_DIR.is_dir():
    sys.exit("out/Debug does not exist, run first: python script/test_build.py")

subprocess.run(
    ["ctest", "--test-dir", str(BUILD_DIR), "-LE", "benchmark",
        "-j8", "--output-on-failure"],
    cwd=ROOT,
    check=True,
)
