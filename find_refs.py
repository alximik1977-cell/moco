#!/usr/bin/env python3
"""Ищет упоминания функций в C-файле любым способом."""
import re
import sys
from pathlib import Path
from collections import defaultdict

C_FILE = "MH8114F_MH8115F_ES1212_flash-20260413-143030.bin_decompiled.c"

def find_references(func_name):
    """Находит все строки, где упоминается функция."""
    with open(C_FILE, 'r', encoding='utf-8', errors='ignore') as f:
        lines = f.readlines()
    
    results = []
    # Паттерны упоминания:
    # 1. Вызов: func_name(...)
    # 2. Указатель: &func_name, func_name без скобок
    # 3. Адрес в массиве: func_name,
    # 4. Комментарий: // func_name
    pattern = re.compile(r'\b' + re.escape(func_name) + r'\b')
    
    for i, line in enumerate(lines, 1):
        if pattern.search(line):
            results.append((i, line.rstrip()))
            print(f"Line {i}: {line.rstrip()}")
    print(f"Found {len(results)} references")
    
    return results

def find_function_def(func_name):
    """Находит определение функции."""
    with open(C_FILE, 'r', encoding='utf-8', errors='ignore') as f:
        text = f.read()
    
    pattern = re.compile(
        rf'(?m)^[a-zA-Z_][\w\s\*]*\s+{re.escape(func_name)}\s*\([^)]*\)\s*\{{',
        re.MULTILINE
    )
    m = pattern.search(text)
    if m:
        # Считаем номер строки
        line_no = text[:m.start()].count('\n') + 1
        return line_no
    return None

def find_address_references(address):
    """Ищет упоминания адреса в C-файле."""
    addr_clean = address.lstrip('0').lower()
    patterns = [
        f'0x{address}',
        f'0x{address.lower()}',
        f'0x{address.upper()}',
        f'0x0*{addr_clean}',  # с ведущими нулями
    ]
    
    with open(C_FILE, 'r', encoding='utf-8', errors='ignore') as f:
        lines = f.readlines()
    
    results = []
    for i, line in enumerate(lines, 1):
        for p in patterns:
            if p in line:
                results.append((i, line.rstrip()))
                break
    return results

def find_pointer_tables():
    """Ищет массивы указателей на функции (типичные таблицы переходов)."""
    with open(C_FILE, 'r', encoding='utf-8', errors='ignore') as f:
        text = f.read()
    
    # Паттерн: тип (*имя[])(параметры) = {...}
    # или: тип *имя[] = {FUN_..., FUN_..., ...}
    pattern = re.compile(
        r'(?m)^[\w\s\*]+\(\s*\*\s*(\w+)\s*\[\s*\d*\s*\]\s*\)\s*\([^)]*\)\s*=\s*\{([^}]{10,500})\}',
        re.MULTILINE
    )
    
    tables = []
    for m in pattern.finditer(text):
        name = m.group(1)
        content = m.group(2)
        line_no = text[:m.start()].count('\n') + 1
        # Считаем количество указателей
        funcs = re.findall(r'FUN_[0-9a-fA-F]+|\w+_FUN_[0-9a-fA-F]+', content)
        tables.append({
            'name': name,
            'line': line_no,
            'funcs': funcs,
            'preview': content[:200]
        })
    
    return tables

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("""
Использование:
  python find_refs.py --refs <func_name>          Все упоминания функции
  python find_refs.py --addr <address>            Упоминания адреса
  python find_refs.py --tables                    Найти таблицы указателей
  python find_refs.py --nearby <func_name> [N]    Функции рядом (±N байт)
""")
        sys.exit(1)
    
    cmd = sys.argv[1]
    
    if cmd == "--refs":
        name = sys.argv[2]
        refs = find_references(name)
        defn = find_function_def(name)
        
        print(f"🔍 Упоминания {name}:")
        if defn:
            print(f"   📌 Определение на строке {defn}")
        print(f"   Всего ссылок: {len(refs)}\n")
        
        for line_no, line in refs[:30]:
            marker = "📌" if line_no == defn else "  "
            print(f"{marker} {line_no:6d}: {line}")
        
        if len(refs) > 30:
            print(f"   ... и ещё {len(refs)-30}")
    
    elif cmd == "--addr":
        addr = sys.argv[2]
        refs = find_address_references(addr)
        print(f"📍 Упоминания адреса 0x{addr}: {len(refs)}")
        for line_no, line in refs[:20]:
            print(f"   {line_no:6d}: {line}")
    
    elif cmd == "--tables":
        tables = find_pointer_tables()
        print(f"📊 Найдено таблиц указателей: {len(tables)}\n")
        for t in tables:
            print(f"📋 {t['name']} (строка {t['line']})")
            print(f"   Функций: {len(t['funcs'])}")
            if t['funcs']:
                print(f"   Первые: {', '.join(t['funcs'][:5])}")
            print(f"   {t['preview']}")
            print()
    
    elif cmd == "--nearby":
        name = sys.argv[2]
        radius = int(sys.argv[3]) if len(sys.argv) > 3 else 0x100
        
        import json
        with open('MH8114F_MH8115F_ES1212_flash-20260413-143030.bin_functions.json') as f:
            funcs = json.load(f)
        
        target = next((f for f in funcs if f['name'] == name), None)
        if not target:
            print(f"❌ Функция {name} не найдена")
            sys.exit(1)
        
        target_addr = int(target['address'], 16)
        print(f"🎯 Функции рядом с {name} @ 0x{target['address']} (±0x{radius:x}):\n")
        
        nearby = []
        for f in funcs:
            addr = int(f['address'], 16)
            if abs(addr - target_addr) <= radius:
                nearby.append((addr, f['name']))
        
        nearby.sort()
        for addr, name in nearby:
            marker = "👉" if addr == target_addr else "  "
            print(f"{marker} 0x{addr:08x}  {name}")
            
