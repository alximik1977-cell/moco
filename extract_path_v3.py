#!/usr/bin/env python3
"""
Извлекает путь вызовов из обновлённого JSON.
Структура: {"functions": [{name, address, calls: [имена], calledBy: [имена], ...}]}

Использование:
  python extract_path_v3.py --path <start> <end>        Найти путь
  python extract_path_v3.py --extract [output.c]        Извлечь путь в файл (по умолчанию context_path.c)
"""
import json
import re
import sys
from pathlib import Path
from collections import deque

C_FILE = "MH8114F_MH8115F_ES1212_flash-20260413-143030.bin_decompiled.c"
JSON_FILE = "MH8114F_MH8115F_ES1212_flash-20260413-143030.bin_functions.json"


def load_functions():
    """Загружает JSON с автоопределением структуры."""
    with open(JSON_FILE, 'r', encoding='utf-8') as f:
        data = json.load(f)
    
    if isinstance(data, list):
        return data
    if isinstance(data, dict):
        for key in ['functions', 'data', 'items']:
            if key in data and isinstance(data[key], list):
                return data[key]
        first_val = next(iter(data.values()), None)
        if isinstance(first_val, dict) and 'name' in first_val:
            return list(data.values())
    raise ValueError("Не удалось найти список функций")


def build_graph(funcs):
    """Строит графы вызовов из полей calls и calledBy."""
    by_name = {f['name']: f for f in funcs}
    callees = {}
    callers = {}
    
    for f in funcs:
        name = f['name']
        callees[name] = [c for c in f.get('calls', []) if c in by_name]
        callers[name] = [c for c in f.get('calledBy', []) if c in by_name]
    
    return by_name, callees, callers


def find_path_bfs(start, end, graph, max_depth=20):
    """BFS поиск кратчайшего пути."""
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


def find_function_body(func_name, c_text):
    """Находит тело функции в C-файле."""
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


def cmd_path(start, end):
    """Поиск пути от start до end."""
    funcs = load_functions()
    by_name, callees, callers = build_graph(funcs)
    
    if start not in by_name:
        print(f"❌ Функция '{start}' не найдена")
        return
    if end not in by_name:
        print(f"❌ Функция '{end}' не найдена")
        return
    
    print(f"🔍 Ищу путь от {start} → {end}...\n")
    
    # Прямой поиск
    path = find_path_bfs(start, end, callees, 20)
    if not path:
        print("❌ Прямой путь не найден. Пробую обратный...")
        path_back = find_path_bfs(end, start, callers, 20)
        if path_back:
            path = list(reversed(path_back))
        else:
            print("❌ Путь не найден")
            return
    
    print(f"✅ Путь найден (длина {len(path)}):\n")
    for i, name in enumerate(path):
        f = by_name[name]
        print(f"[{i:2d}] 0x{f['address']}  {name}")
    
    Path("call_path.txt").write_text("\n".join(path), encoding='utf-8')
    print(f"\n📝 Сохранено в call_path.txt")


def cmd_extract(output_file="context_path.c"):
    """Извлекает все функции из пути в контекст-файл."""
    if not Path("call_path.txt").exists():
        print("❌ Сначала выполните --path")
        return
    
    path = Path("call_path.txt").read_text().strip().split("\n")
    funcs = load_functions()
    by_name, callees, callers = build_graph(funcs)
    
    with open(C_FILE, 'r', encoding='utf-8', errors='ignore') as f:
        c_text = f.read()
    
    result = []
    result.append("// ═══ Путь вызовов от entry point до цели ═══")
    result.append(f"// Всего функций: {len(path)}")
    result.append(f"// Путь: {' → '.join(path)}")
    result.append("")
    
    extracted = 0
    for i, name in enumerate(path):
        func_info = by_name.get(name)
        if not func_info:
            result.append(f"// ⚠️ {name} не найдена в JSON\n")
            continue
        
        body = find_function_body(name, c_text)
        
        result.append(f"// ═══ [{i+1}/{len(path)}] {name} @ 0x{func_info['address']} ═══")
        result.append(f"// Сигнатура: {func_info['signature']}")
        result.append(f"// Размер: {func_info.get('size', '?')} байт")
        if func_info.get('calls'):
            result.append(f"// Вызывает: {', '.join(func_info['calls'])}")
        if func_info.get('calledBy'):
            result.append(f"// Вызывается из: {', '.join(func_info['calledBy'][:5])}")
        
        if body:
            result.append(body)
            extracted += 1
        else:
            result.append(f"// ⚠️ Тело функции не найдено в C-файле")
            result.append(f"void {name}(void) {{ /* тело не найдено */ }}")
        
        result.append("")
    
    output = "\n".join(result)
    Path(output_file).write_text(output, encoding='utf-8')
    
    print(f"✅ Извлечено {extracted} из {len(path)} функций → {output_file}")
    print(f"📏 Размер: {len(output)} символов, {len(output.splitlines())} строк")


if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("""
Использование:
  python extract_path_v3.py --path <start> <end>        Найти путь
  python extract_path_v3.py --extract [output.c]        Извлечь путь в файл
                                                         (по умолчанию context_path.c)

Примеры:
  python extract_path_v3.py --path 0000_0_TBR_1_FUN_00005da0 NORM_START2_FUN_00015816
  python extract_path_v3.py --extract norm_start2_path.c
  
  python extract_path_v3.py --path 0000_0_TBR_1_FUN_00005da0 FUEL_Injectors_FUN_000183dc
  python extract_path_v3.py --extract fuel_injectors_path.c
""")
        sys.exit(1)
    
    cmd = sys.argv[1]
    if cmd == "--path":
        if len(sys.argv) < 4:
            print("❌ Для --path нужно указать две функции: <start> <end>")
            sys.exit(1)
        cmd_path(sys.argv[2], sys.argv[3])
    elif cmd == "--extract":
        output_file = sys.argv[2] if len(sys.argv) > 2 else "context_path.c"
        cmd_extract(output_file)
    else:
        print(f"❌ Неизвестная команда: {cmd}")
        sys.exit(1)
        