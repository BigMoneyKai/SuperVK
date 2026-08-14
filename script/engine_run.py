#!/usr/bin/env python3
"""Runs the engine. Must run from the repository root (asset paths are relative)."""

import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
CONFIG = sys.argv[1] if len(sys.argv) > 1 and sys.argv[1] == "release" else "Debug"
EXE = ROOT / "out" / CONFIG / "src" / "supervk"

if not EXE.is_file():
    sys.exit(f"{EXE} does not exist, run first: python script/engine_build.py")

subprocess.run([str(EXE)], cwd=ROOT, check=True)
