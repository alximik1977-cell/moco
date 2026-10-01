#!/usr/bin/env python3
"""
Находит, какой функции принадлежит строка в декомпилированном C-файле.
Также извлекает все вызовы из тела функции (даже те, что не в JSON).
"""
import re
import sys
from pathlib import Path

C_FILE = "MH8114F_MH8115F_ES1212_flash-20260413-143030.bin_decompiled.c"

def parse_functions():
    """Парсит все функции из C-файла с их диапазонами строк."""
    with open(C_FILE, 'r', encoding='utf-8', errors='ignore') as f:
        lines = f.readlines()
    
    functions = []
    i = 0
    while i < len(lines):
        line = lines[i]
        # Ищем определение функции: тип имя(параметры) {
        # Ghidra обычно пишет: undefined8 FUN_xxxxxxxx(void)
        # или: void FUN_xxxxxxxx(uint param_1, ...)
        m = re.match(r'^(\w+(?:\s*\*)*\s+)(\w+)\s*\([^)]*\)\s*\{?\s*$', line)
        if m:
            ret_type = m.group(1).strip()
            func_name = m.group(2)
            start_line = i + 1  # 1-based
            
            # Ищем закрывающую скобку
            depth = 0
            j = i
            found_open = False
            while j < len(lines):
                for ch in lines[j]:
                    if ch == '{':
                        depth += 1
                        found_open = True
                    elif ch == '}':
                        depth -= 1
                        if found_open and depth == 0:
                            end_line = j + 1
                            body = ''.join(lines[i:j+1])
                            functions.append({
                                'name': func_name,
                                'ret': ret_type,
                                'start': start_line,
                                'end': end_line,
                                'body': body
                            })
                            i = j
                            break
                j += 1
                if found_open and depth == 0:
                    break
        i += 1
    
    return functions

def find_owner(line_no, functions):
    """Находит функцию, которой принадлежит строка."""
    for f in functions:
        if f['start'] <= line_no <= f['end']:
            return f
    return None

def extract_calls_from_body(body):
    """Извлекает все вызовы функций из тела (по паттерну FUNC_NAME(...))."""
    # Паттерн: имя_функции(
    # Исключаем ключевые слова C
    keywords = {'if', 'while', 'for', 'switch', 'return', 'sizeof', 'else'}
    pattern = re.compile(r'\b([A-Za-z_]\w*)\s*\(')
    calls = []
    for m in pattern.finditer(body):
        name = m.group(1)
        if name not in keywords and not name.startswith('FUN_') is False:
            calls.append(name)
    # Уникальные с сохранением порядка
    seen = set()
    unique = []
    for c in calls:
        if c not in seen:
            seen.add(c)
            unique.append(c)
    return unique

def cmd_owner(line_no):
    """Найти функцию-владельца строки."""
    functions = parse_functions()
    owner = find_owner(line_no, functions)
    if not owner:
        print(f"❌ Строка {line_no} не принадлежит ни одной функции")
        return
    
    print(f"📌 Строка {line_no} принадлежит функции:")
    print(f"   Имя: {owner['name']}")
    print(f"   Строки: {owner['start']}-{owner['end']}")
    print(f"   Размер: {owner['end'] - owner['start'] + 1} строк")
    
    calls = extract_calls_from_body(owner['body'])
    print(f"\n📤 Вызовы из этой функции ({len(calls)}):")
    for c in calls[:30]:
        print(f"   → {c}")
    if len(calls) > 30:
        print(f"   ... и ещё {len(calls)-30}")
    
    return owner

def cmd_body_at(line_no, context=10):
    """Показать строки вокруг указанной."""
    with open(C_FILE, 'r', encoding='utf-8', errors='ignore') as f:
        lines = f.readlines()
    
    start = max(0, line_no - context - 1)
    end = min(len(lines), line_no + context)
    
    print(f"📖 Строки {start+1}-{end}:")
    for i in range(start, end):
        marker = "👉" if i + 1 == line_no else "  "
        print(f"{marker} {i+1:6d}: {lines[i].rstrip()}")

def cmd_main_body():
    """Показать тело main функции."""
    functions = parse_functions()
    for f in functions:
        if f['name'] == '0000_0_TBR_1_FUN_00005da0':
            print(f"📋 Тело main (строки {f['start']}-{f['end']}):\n")
            print(f['body'])
            
            calls = extract_calls_from_body(f['body'])
            print(f"\n📤 Вызовы из main ({len(calls)}):")
            for c in calls:
                print(f"   → {c}")
            return
    print("❌ main не найдена")

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("""
Использование:
  python find_owner.py --owner <line>           Чья это строка?
  python find_owner.py --body <line> [context]  Показать строки вокруг
  python find_owner.py --main                   Тело main функции
""")
        sys.exit(1)
    
    cmd = sys.argv[1]
    
    if cmd == "--owner":
        line_no = int(sys.argv[2])
        cmd_owner(line_no)
    elif cmd == "--body":
        line_no = int(sys.argv[2])
        context = int(sys.argv[3]) if len(sys.argv) > 3 else 10
        cmd_body_at(line_no, context)
    elif cmd == "--main":
        cmd_main_body()
        