"""Comprueba la logica C++ real de configuracion sin grabar ni ejecutar la placa."""
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
result = subprocess.run([str(args.compiler), "-std=c++14", "-Wall", "-Wextra", "-Werror", "-Iinclude", "-fsyntax-only", "test/iluminacion_airbit_test.cpp"], cwd=project)
if result.returncode == 0:
    print("PASS: 9 comprobaciones constexpr de iluminacion e intensidad de calidad (limites, CRC, escalado, color y memoria)")
sys.exit(result.returncode)
