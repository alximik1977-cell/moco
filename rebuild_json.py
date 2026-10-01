#!/usr/bin/env python3
"""
Добавляет в JSON функции, которые есть в C-файле, но отсутствуют в JSON.
"""
import json
import re
from pathlib import Path

C_FILE = "MH8114F_MH8115F_ES1212_flash-20260413-143030.bin_decompiled.c"
JSON_FILE = "MH8114F_MH8115F_ES1212_flash-20260413-143030.bin_functions.json"
OUT_FILE = "MH8114F_MH8115F_ES1212_flash-20260413-143030.bin_functions_enriched.json"


def parse_functions_from_c(c_text):
    """Парсит все функции из C-файла."""
    functions = {}
    # Паттерн для определения функции
    pattern = re.compile(
        r'(?m)^([\w\s\*]+?)\s+([A-Za-z0-9_]+)\s*\([^)]*\)\s*\{',
        re.MULTILINE
    )
    
    for m in pattern.finditer(c_text):
        ret_type = m.group(1).strip()
        func_name = m.group(2)
        start = m.start()
        
        # Ищем закрывающую скобку
        depth = 0
        i = m.end() - 1
        found_open = False
        while i < len(c_text):
            if c_text[i] == '{':
                depth += 1
                found_open = True
            elif c_text[i] == '}':
                depth -= 1
                if found_open and depth == 0:
                    end = i + 1
                    body = c_text[start:end]
                    
                    # Пытаемся извлечь адрес из имени
                    addr = "unknown"
                    addr_match = re.search(r'FUN_([0-9a-fA-F]{8})$', func_name)
                    if addr_match:
                        addr = addr_match.group(1)
                    
                    functions[func_name] = {
                        "name": func_name,
                        "address": addr,
                        "size": len(body),
                        "signature": f"{ret_type} {func_name}(...)",
                        "returnType": ret_type,
                        "callingConvention": "unknown",
                        "isExternal": False,
                        "isThunk": False,
                        "hasVarArgs": False,
                        "sourceType": "DECOMPILED",
                        "parameters": [],
                        "calls": [],  # Нужно парсить отдельно
                        "calledBy": []
                    }
                    break
            i += 1
    
    return functions


def enrich_json():
    print(f"📖 Читаю {C_FILE}...")
    with open(C_FILE, 'r', encoding='utf-8', errors='ignore') as f:
        c_text = f.read()
    
    print(f"🔧 Парсю функции из C-файла...")
    c_functions = parse_functions_from_c(c_text)
    print(f"   Найдено {len(c_functions)} функций в C-файле")
    
    print(f"📊 Загружаю {JSON_FILE}...")
    with open(JSON_FILE, 'r', encoding='utf-8') as f:
        data = json.load(f)
    
    # Определяем структуру
    if isinstance(data, dict) and 'functions' in data:
        json_functions = data['functions']
        is_dict = True
    else:
        json_functions = data if isinstance(data, list) else []
        is_dict = False
    
    print(f"   Найдено {len(json_functions)} функций в JSON")
    
    # Находим функции, которых нет в JSON
    json_names = {f['name'] for f in json_functions}
    missing = []
    
    for name, func_data in c_functions.items():
        if name not in json_names:
            missing.append(func_data)
    
    print(f"\n🔍 Найдено {len(missing)} функций в C-файле, но отсутствующих в JSON:")
    for f in missing[:20]:
        print(f"   • {f['name']} @ 0x{f['address']}")
    if len(missing) > 20:
        print(f"   ... и ещё {len(missing) - 20}")
    
    # Добавляем недостающие
    enriched = json_functions + missing
    
    # Сохраняем
    if is_dict:
        data['functions'] = enriched
    else:
        data = enriched
    
    with open(OUT_FILE, 'w', encoding='utf-8') as f:
        json.dump(data, f, indent=2, ensure_ascii=False)
    
    print(f"\n💾 Сохранено в {OUT_FILE}")
    print(f"   Всего функций: {len(enriched)}")
    print(f"   Добавлено из C-файла: {len(missing)}")


if __name__ == "__main__":
    enrich_json()
    