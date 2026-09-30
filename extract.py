#!/usr/bin/env python3
"""Извлекает функции из декомпилированного .c файла по JSON-индексу."""
import json
import re
import sys
from pathlib import Path

C_FILE = "MH8114F_MH8115F_ES1212_flash-20260413-143030.bin_decompiled.c"
JSON_FILE = "MH8114F_MH8115F_ES1212_flash-20260413-143030.bin_functions.json"

def load_index():
    with open(JSON_FILE, 'r', encoding='utf-8') as f:
        funcs = json.load(f)
    # Индекс по имени и по адресу
    by_name = {f['name']: f for f in funcs}
    by_addr = {f['address']: f for f in funcs}
    return funcs, by_name, by_addr

def find_function_in_c(func_name, c_text):
    """Находит функцию в C-файле по имени (декомпилятор Ghidra обычно пишет имя функции в начале)."""
    # Паттерн: тип_возврата имя_функции(параметры) { ... }
    # Учитываем вложенные скобки
    pattern = re.compile(
        rf'(?m)^[a-zA-Z_][\w\s\*]*\s+{re.escape(func_name)}\s*\([^)]*\)\s*\{{',
        re.MULTILINE
    )
    m = pattern.search(c_text)
    if not m:
        return None, None, None
    
    start = m.start()
    # Ищем закрывающую скобку с учётом вложенности
    depth = 0
    i = m.end() - 1
    while i < len(c_text):
        if c_text[i] == '{':
            depth += 1
        elif c_text[i] == '}':
            depth -= 1
            if depth == 0:
                end = i + 1
                return start, end, c_text[start:end]
        i += 1
    return None, None, None

def extract(func_name, include_calls=True, max_depth=1):
    funcs, by_name, by_addr = load_index()
    
    if func_name not in by_name:
        print(f"❌ Функция '{func_name}' не найдена в индексе")
        print(f"💡 Попробуйте: python extract.py --search {func_name}")
        return
    
    with open(C_FILE, 'r', encoding='utf-8', errors='ignore') as f:
        c_text = f.read()
    
    result = []
    visited = set()
    
    def extract_one(name, depth=0):
        if name in visited or depth > max_depth:
            return
        visited.add(name)
        
        func_info = by_name.get(name)
        if not func_info:
            return
        
        start, end, body = find_function_in_c(name, c_text)
        if not body:
            result.append(f"// ⚠️ Функция {name} (адрес {func_info['address']}) не найдена в .c файле\n")
            return
        
        result.append(f"// ═══ {name} @ 0x{func_info['address']} ═══")
        result.append(f"// Сигнатура: {func_info['signature']}")
        if func_info.get('calls'):
            result.append(f"// Вызывает: {', '.join(func_info['calls'])}")
        result.append(body)
        result.append("")
        
        # Рекурсивно подтягиваем вызываемые функции
        if include_calls and func_info.get('calls'):
            for called_addr in func_info['calls']:
                called_func = by_addr.get(called_addr)
                if called_func:
                    extract_one(called_func['name'], depth + 1)
    
    extract_one(func_name)
    
    output = "\n".join(result)
    out_file = f"context_{func_name}.c"
    Path(out_file).write_text(output, encoding='utf-8')
    print(f"✅ Извлечено {len(visited)} функций → {out_file}")
    print(f"📏 Размер: {len(output)} символов, ~{len(output.split())} слов")

def search(pattern):
    """Поиск функций по паттерну в имени."""
    funcs, _, _ = load_index()
    matches = [f for f in funcs if pattern.lower() in f['name'].lower()]
    print(f"🔍 Найдено {len(matches)} функций:")
    for f in matches[:30]:
        print(f"  {f['name']:40s} @ 0x{f['address']}")

def list_calls(func_name):
    """Показывает дерево вызовов функции."""
    funcs, by_name, by_addr = load_index()
    if func_name not in by_name:
        print(f"❌ Функция не найдена")
        return
    
    def show(name, indent=0):
        f = by_name.get(name)
        if not f:
            return
        print("  " * indent + f"├─ {name} @ 0x{f['address']}")
        if indent < 3:  # ограничение глубины
            for addr in f.get('calls', []):
                called = by_addr.get(addr)
                if called:
                    show(called['name'], indent + 1)
    
    show(func_name)

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Использование:")
        print("  python extract.py <имя_функции>           # извлечь функцию + вызовы")
        print("  python extract.py <имя> --no-calls        # только эта функция")
        print("  python extract.py <имя> --depth 2         # глубина рекурсии вызовов")
        print("  python extract.py --search <паттерн>      # поиск по имени")
        print("  python extract.py --calls <имя>           # дерево вызовов")
        sys.exit(1)
    
    if sys.argv[1] == "--search":
        search(sys.argv[2])
    elif sys.argv[1] == "--calls":
        list_calls(sys.argv[2])
    else:
        name = sys.argv[1]
        include_calls = "--no-calls" not in sys.argv
        depth = 1
        if "--depth" in sys.argv:
            depth = int(sys.argv[sys.argv.index("--depth") + 1])
        extract(name, include_calls=include_calls, max_depth=depth)
        