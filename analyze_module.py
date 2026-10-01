#!/usr/bin/env python3
"""Извлекает все функции, связанные с определённым модулем/периферией."""
import json
import sys

JSON_FILE = "MH8114F_MH8115F_ES1212_flash-20260413-143030.bin_functions.json"

with open(JSON_FILE) as f:
    funcs = json.load(f)

module = sys.argv[1]  # например "UART" или "FLASH"

# Находим все функции, содержащие module в имени
related = [f for f in funcs if module.upper() in f['name'].upper()]
print(f"Найдено {len(related)} функций, связанных с {module}:")
for f in related:
    print(f"  {f['name']}")

# Извлекаем все
for f in related:
    print(f"\n📦 Извлекаю {f['name']}...")
    import subprocess
    subprocess.run(["python", "extract.py", f['name'], "--no-calls"])
    