"""Comprueba la logica C++ real de calidad sin grabar ni ejecutar la placa."""
from pathlib import Path
import argparse
import subprocess
import sys

project = Path(__file__).resolve().parent.parent
parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument("--compiler", type=Path, default=Path.home() / ".platformio" / "packages" / "toolchain-xtensa" / "bin" / "xtensa-lx106-elf-g++.exe")
args = parser.parse_args()
if not args.compiler.is_file():
    parser.error("No se encuentra el compilador; indicar --compiler RUTA")
result = subprocess.run([str(args.compiler), "-std=c++14", "-Wall", "-Wextra", "-Werror", "-Iinclude", "-fsyntax-only", "test/calidad_aire_test.cpp"], cwd=project)
if result.returncode == 0:
    print("PASS: 14 comprobaciones constexpr de calidad (paleta, referencia, limites y suavizado)")
sys.exit(result.returncode)
