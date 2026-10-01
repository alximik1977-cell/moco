#!/usr/bin/env python3
"""
Находит main функцию (даже если имя начинается с цифры)
и строит путь до M_2_MAIN_APP_CYCLE2_FUN_00011852.
"""
import re
import sys
from pathlib import Path
from collections import defaultdict

C_FILE = "MH8114F_MH8115F_ES1212_flash-20260413-143030.bin_decompiled.c"
JSON_FILE = "MH8114F_MH8115F_ES1212_flash-20260413-143030.bin_functions.json"

def parse_functions():
    """Парсит все функции из C-файла."""
    with open(C_FILE, 'r', encoding='utf-8', errors='ignore') as f:
        lines = f.readlines()
    
    functions = []
    i = 0
    while i < len(lines):
        line = lines[i]
        # Паттерн: тип имя(параметры) {
        # Имя может начинаться с цифры (Ghidra генерирует такие имена)
        m = re.match(r'^([\w\s\*]+?)\s+([A-Za-z0-9_]+)\s*\([^)]*\)\s*\{?\s*$', line)
        if m:
            ret_type = m.group(1).strip()
            func_name = m.group(2)
            start_line = i + 1
            
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

def extract_calls(body):
    """Извлекает все вызовы функций из тела."""
    keywords = {'if', 'while', 'for', 'switch', 'return', 'sizeof', 'else', 'do'}
    pattern = re.compile(r'\b([A-Za-z0-9_]+)\s*\(')
    calls = []
    for m in pattern.finditer(body):
        name = m.group(1)
        if name not in keywords:
            calls.append(name)
    return list(dict.fromkeys(calls))  # уникальные с сохранением порядка

def find_main(functions):
    """Находит main функцию по адресу 0x00005da0."""
    for f in functions:
        if '00005da0' in f['name'] or 'TBR' in f['name']:
            return f
    # Альтернатива: ищем по строке 4367
    for f in functions:
        if f['start'] <= 4367 <= f['end']:
            return f
    return None

def build_call_graph(functions):
    """Строит граф вызовов из C-файла."""
    callees = defaultdict(set)
    callers = defaultdict(set)
    
    for f in functions:
        calls = extract_calls(f['body'])
        for c in calls:
            callees[f['name']].add(c)
            callers[c].add(f['name'])
    
    return callees, callers

def find_path_bfs(start, end, graph, max_depth=15):
    """BFS поиск пути."""
    from collections import deque
    
    if start == end:
        return [start]
    
    queue = deque([(start, [start])])
    visited = {start}
    
    while queue:
        current, path = queue.popleft()
        if len(path) > max_depth:
            continue
        
        for neighbor in graph.get(current, []):
            if neighbor in visited:
                continue
            new_path = path + [neighbor]
            if neighbor == end:
                return new_path
            visited.add(neighbor)
            queue.append((neighbor, new_path))
    
    return None

def cmd_find_path():
    """Находит путь от main до главного цикла."""
    functions = parse_functions()
    callees, callers = build_call_graph(functions)
    
    main_func = find_main(functions)
    if not main_func:
        print("❌ Main функция не найдена")
        return
    
    target = "M_2_MAIN_APP_CYCLE2_FUN_00011852"
    
    print(f"🔍 Ищу путь от {main_func['name']} → {target}\n")
    
    path = find_path_bfs(main_func['name'], target, callees, max_depth=10)
    
    if path:
        print(f"✅ Путь найден (длина {len(path)}):\n")
        for i, name in enumerate(path):
            func = next((f for f in functions if f['name'] == name), None)
            if func:
                print(f"[{i:2d}] строки {func['start']:5d}-{func['end']:5d}  {name}")
            else:
                print(f"[{i:2d}] {name} (не найдена в C-файле)")
        
        # Сохраняем путь
        Path("path_to_main_cycle.txt").write_text("\n".join(path), encoding='utf-8')
        print(f"\n📝 Путь сохранён в path_to_main_cycle.txt")
    else:
        print("❌ Путь не найден")
        print("\n💡 Попробую обратный поиск от цели...")
        path_back = find_path_bfs(target, main_func['name'], callers, max_depth=10)
        if path_back:
            path = list(reversed(path_back))
            print(f"✅ Путь найден (обратный поиск):")
            for i, name in enumerate(path):
                print(f"[{i:2d}] {name}")

def cmd_show_main():
    """Показывает тело main."""
    functions = parse_functions()
    main_func = find_main(functions)
    
    if not main_func:
        print("❌ Main не найдена")
        return
    
    print(f"📋 Main функция: {main_func['name']}")
    print(f"   Строки: {main_func['start']}-{main_func['end']}")
    print(f"\n{main_func['body'][:2000]}")
    
    calls = extract_calls(main_func['body'])
    print(f"\n📤 Вызовы из main ({len(calls)}):")
    for c in calls[:20]:
        print(f"   → {c}")

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("""
Использование:
  python find_main.py --path          Найти путь до главного цикла
  python find_main.py --show          Показать тело main
""")
        sys.exit(1)
    
    cmd = sys.argv[1]
    
    if cmd == "--path":
        cmd_find_path()
    elif cmd == "--show":
        cmd_show_main()
        