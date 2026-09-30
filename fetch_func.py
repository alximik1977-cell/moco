#!/usr/bin/env python3
"""
Извлекает функцию из декомпилированного C-файла по имени.
Используется как инструмент для Aider/GigaChat.
"""
import json
import re
import sys
from pathlib import Path

C_FILE = "MH8114F_MH8115F_ES1212_flash-20260413-143030.bin_decompiled.c"
JSON_FILE = "MH8114F_MH8115F_ES1212_flash-20260413-143030.bin_functions.json"


def load_functions_json():
    with open(JSON_FILE, 'r', encoding='utf-8') as f:
        data = json.load(f)
    if isinstance(data, dict) and 'functions' in data:
        return data['functions']
    return data if isinstance(data, list) else []


def find_function_body(func_name, c_text):
    pattern = re.compile(
        rf'(?m)^[\w\s\*]+\s+{re.escape(func_name)}\s*\([^)]*\)\s*\{{',
        re.MULTILINE
    )
    m = pattern.search(c_text)
    if not m:
        return None
    start = m.start()
    depth = 0
    i = m.end() - 1
    while i < len(c_text):
        if c_text[i] == '{':
            depth += 1
        elif c_text[i] == '}':
            depth -= 1
            if depth == 0:
                return c_text[start:i+1]
        i += 1
    return None


def get_c_text():
    with open(C_FILE, 'r', encoding='utf-8', errors='ignore') as f:
        return f.read()


def cmd_fetch(name):
    """Извлечь одну функцию."""
    funcs = load_functions_json()
    by_name = {f['name']: f for f in funcs}
    
    if name not in by_name:
        # Поиск по частичному совпадению
        matches = [f for f in funcs if name.lower() in f['name'].lower()]
        if matches:
            print(f"❌ Точное имя '{name}' не найдено. Возможно, вы имели в виду:")
            for m in matches[:10]:
                print(f"   {m['name']} @ 0x{m['address']}")
        else:
            print(f"❌ Функция '{name}' не найдена")
        return
    
    info = by_name[name]
    c_text = get_c_text()
    body = find_function_body(name, c_text)
    
    print(f"// ═══ {name} @ 0x{info['address']} ═══")
    print(f"// Сигнатура: {info['signature']}")
    print(f"// Размер: {info.get('size', '?')} байт")
    if info.get('calls'):
        print(f"// Вызывает: {', '.join(info['calls'])}")
    if info.get('calledBy'):
        print(f"// Вызывается из: {', '.join(info['calledBy'][:10])}")
    print()
    if body:
        print(body)
    else:
        print(f"// ⚠️ Тело функции не найдено в C-файле")


def cmd_callees(name):
    """Показать все функции, вызываемые из данной."""
    funcs = load_functions_json()
    by_name = {f['name']: f for f in funcs}
    
    if name not in by_name:
        print(f"❌ Функция '{name}' не найдена")
        return
    
    info = by_name[name]
    calls = info.get('calls', [])
    
    print(f"// Функции, вызываемые из {name} ({len(calls)}):")
    c_text = get_c_text()
    
    for called_name in calls:
        called_info = by_name.get(called_name)
        if called_info:
            print(f"\n// ─── {called_name} @ 0x{called_info['address']} ───")
            print(f"// Сигнатура: {called_info['signature']}")
            body = find_function_body(called_name, c_text)
            if body:
                # Ограничиваем вывод 50 строками
                lines = body.split('\n')
                if len(lines) > 50:
                    print('\n'.join(lines[:50]))
                    print(f"// ... (ещё {len(lines)-50} строк, используйте fetch_func.py {called_name} для полного кода)")
                else:
                    print(body)
            else:
                print(f"// ⚠️ Тело не найдено")


def cmd_callers(name):
    """Показать все функции, вызывающие данную."""
    funcs = load_functions_json()
    by_name = {f['name']: f for f in funcs}
    
    if name not in by_name:
        print(f"❌ Функция '{name}' не найдена")
        return
    
    info = by_name[name]
    callers = info.get('calledBy', [])
    
    print(f"// Функции, вызывающие {name} ({len(callers)}):")
    for caller_name in callers:
        caller_info = by_name.get(caller_name)
        if caller_info:
            print(f"//   {caller_name} @ 0x{caller_info['address']}")
        else:
            print(f"//   {caller_name} (не найдена в индексе)")


def cmd_search(pattern):
    """Поиск функций по паттерну."""
    funcs = load_functions_json()
    matches = [f for f in funcs if pattern.lower() in f['name'].lower()]
    
    print(f"// Найдено {len(matches)} функций по паттерну '{pattern}':")
    for f in matches[:30]:
        calls_count = len(f.get('calls', []))
        called_count = len(f.get('calledBy', []))
        print(f"//   {f['name']:50s} @ 0x{f['address']}  (calls:{calls_count} calledBy:{called_count})")
    if len(matches) > 30:
        print(f"//   ... и ещё {len(matches)-30}")


if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Использование:")
        print("  python fetch_func.py <имя>              Извлечь функцию")
        print("  python fetch_func.py --callees <имя>    Кого вызывает")
        print("  python fetch_func.py --callers <имя>    Кто вызывает")
        print("  python fetch_func.py --search <паттерн> Поиск по имени")
        sys.exit(1)
    
    if sys.argv[1] == "--callees":
        cmd_callees(sys.argv[2])
    elif sys.argv[1] == "--callers":
        cmd_callers(sys.argv[2])
    elif sys.argv[1] == "--search":
        cmd_search(sys.argv[2])
    else:
        cmd_fetch(sys.argv[1])
        