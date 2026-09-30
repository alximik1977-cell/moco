#!/usr/bin/env python3
"""
Поиск пути в графе вызовов декомпилированной прошивки.
Работает с новым форматом JSON (корень — объект с полем 'functions').
"""
import json
import sys
from collections import deque, defaultdict
from pathlib import Path
import re

JSON_FILE = "MH8114F_MH8115F_ES1212_flash-20260413-143030.bin_functions.json"
C_FILE = "MH8114F_MH8115F_ES1212_flash-20260413-143030.bin_decompiled.c"

def load_data():
    with open(JSON_FILE, 'r', encoding='utf-8') as f:
        data = json.load(f)
    
    funcs = data['functions']
    by_name = {f['name']: f for f in funcs}
    by_addr = {f['address']: f for f in funcs}
    
    # Прямой граф: callees[A] = [B, C] означает A вызывает B и C
    callees = defaultdict(list)
    # Обратный граф: callers[B] = [A] означает B вызывается из A
    callers = defaultdict(list)
    
    for f in funcs:
        name = f['name']
        # Используем поле 'calls' для прямого графа
        for called_addr in f.get('calls', []):
            # Адрес может быть в формате "00000800" или "0000_TBR_..."
            # Нужно найти функцию по адресу
            called = by_addr.get(called_addr)
            if called:
                callees[name].append(called['name'])
                callers[called['name']].append(name)
        
        # Также используем поле 'calledBy' для обратного графа (если есть)
        for caller_name in f.get('calledBy', []):
            if caller_name in by_name and caller_name not in callers[name]:
                callers[name].append(caller_name)
    
    return funcs, by_name, by_addr, callees, callers

def find_path_bfs(start, end, graph, max_depth=20):
    """BFS поиск кратчайшего пути в графе."""
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

def find_function_in_c(func_name, c_text):
    """Извлекает тело функции из C-файла."""
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

def cmd_path(start, end, max_depth=20):
    """Поиск пути от start до end через прямые вызовы."""
    funcs, by_name, by_addr, callees, callers = load_data()
    
    if start not in by_name:
        print(f"❌ Функция '{start}' не найдена")
        return
    if end not in by_name:
        print(f"❌ Функция '{end}' не найдена")
        return
    
    print(f"🔍 Ищу путь от {start} → {end} (прямые вызовы, глубина ≤ {max_depth})...")
    path = find_path_bfs(start, end, callees, max_depth)
    
    if not path:
        print("❌ Путь не найден через прямые вызовы")
        print("💡 Попробую через обратный граф от целевой функции...")
        path_back = find_path_bfs(end, start, callers, max_depth)
        if path_back:
            path = list(reversed(path_back))
            print(f"✅ Найден путь (обратный поиск):")
        else:
            print("❌ Путь не найден ни в одном направлении")
            return
    else:
        print(f"✅ Найден путь длиной {len(path)}:")
    
    print(f"\n{'='*70}")
    for i, name in enumerate(path):
        f = by_name[name]
        size = f.get('size', '?')
        print(f"[{i:2d}] 0x{f['address']}  (size={size})  {name}")
    print(f"{'='*70}\n")
    
    Path("call_path.txt").write_text("\n".join(path), encoding='utf-8')
    print(f"📝 Путь сохранён в call_path.txt ({len(path)} функций)")
    return path

def cmd_callers(name, depth=3):
    """Кто вызывает данную функцию (использует calledBy из JSON)."""
    funcs, by_name, by_addr, callees, callers = load_data()
    
    if name not in by_name:
        print(f"❌ Функция '{name}' не найдена")
        return
    
    f = by_name[name]
    # Сначала покажем прямое calledBy из JSON
    called_by = f.get('calledBy', [])
    print(f"📞 Кто вызывает {name} (из JSON calledBy): {len(called_by)}")
    for caller in called_by[:20]:
        cf = by_name.get(caller)
        if cf:
            print(f"   ← {caller} @ 0x{cf['address']}")
    
    if depth > 1:
        print(f"\n📞 Рекурсивно (до глубины {depth}):")
        def show(n, d, visited):
            if d > depth or n in visited:
                return
            visited.add(n)
            f = by_name.get(n)
            if not f:
                return
            print("  " * d + f"├─ {n} @ 0x{f['address']}")
            for caller in callers.get(n, []):
                show(caller, d + 1, visited)
        
        show(name, 0, set())

def cmd_callees(name, depth=3):
    """Кого вызывает данная функция."""
    funcs, by_name, by_addr, callees, callers = load_data()
    
    if name not in by_name:
        print(f"❌ Функция '{name}' не найдена")
        return
    
    def show(n, d, visited):
        if d > depth or n in visited:
            return
        visited.add(n)
        f = by_name.get(n)
        if not f:
            return
        print("  " * d + f"├─ {n} @ 0x{f['address']}")
        for callee in callees.get(n, []):
            show(callee, d + 1, visited)
    
    print(f"📤 Кого вызывает {name} (до глубины {depth}):")
    show(name, 0, set())

def cmd_extract_path(path_file=None, path=None):
    """Извлекает все функции из пути в один контекст-файл."""
    funcs, by_name, by_addr, callees, callers = load_data()
    
    if path is None:
        if path_file is None:
            path_file = "call_path.txt"
        if not Path(path_file).exists():
            print(f"❌ Файл {path_file} не найден. Сначала выполните --path")
            return
        path = Path(path_file).read_text().strip().split("\n")
    
    with open(C_FILE, 'r', encoding='utf-8', errors='ignore') as f:
        c_text = f.read()
    
    result = []
    result.append(f"// ═══ Call path: {' → '.join(path[:3])} → ... → {path[-1]} ═══")
    result.append(f"// Всего функций: {len(path)}")
    result.append("")
    
    total_size = 0
    for i, name in enumerate(path):
        f = by_name.get(name)
        if not f:
            result.append(f"// ⚠️ {name} не найдена в индексе\n")
            continue
        
        body = find_function_in_c(name, c_text)
        if not body:
            result.append(f"// ⚠️ {name} @ 0x{f['address']} — тело не найдено в .c\n")
            continue
        
        result.append(f"// ═══ [{i+1}/{len(path)}] {name} @ 0x{f['address']} ═══")
        result.append(f"// Сигнатура: {f['signature']}")
        result.append(f"// Размер: {f.get('size', '?')} байт")
        if f.get('calls'):
            called_names = []
            for addr in f['calls']:
                cf = by_addr.get(addr)
                if cf:
                    called_names.append(cf['name'])
            if called_names:
                result.append(f"// Вызывает: {', '.join(called_names)}")
        result.append(body)
        result.append("")
        total_size += len(body)
    
    output = "\n".join(result)
    out_file = "context_path.c"
    Path(out_file).write_text(output, encoding='utf-8')
    
    print(f"✅ Извлечено {len(path)} функций → {out_file}")
    print(f"📏 Размер контекста: {len(output)} символов (~{len(output.split())} слов)")
    print(f"📊 Размер тел функций: {total_size} символов")
    
    if len(output) > 100000:
        print(f"\n⚠️  Контекст большой ({len(output)//1024} КБ).")
        print("💡 Рекомендую разбить на части:")
        print("   python find_path.py --extract-part 0 5   # функции 0-5")
        print("   python find_path.py --extract-part 6 10  # функции 6-10")

def cmd_extract_part(start_idx, end_idx):
    """Извлекает часть пути (для больших путей)."""
    if not Path("call_path.txt").exists():
        print("❌ Сначала выполните --path")
        return
    
    path = Path("call_path.txt").read_text().strip().split("\n")
    part = path[start_idx:end_idx+1]
    
    funcs, by_name, by_addr, callees, callers = load_data()
    with open(C_FILE, 'r', encoding='utf-8', errors='ignore') as f:
        c_text = f.read()
    
    result = [f"// Part of path: functions {start_idx}-{end_idx} from {path[0]} to {path[-1]}", ""]
    
    for i, name in enumerate(part):
        real_idx = start_idx + i
        f = by_name.get(name)
        if not f:
            continue
        body = find_function_in_c(name, c_text)
        if not body:
            continue
        result.append(f"// ═══ [{real_idx}] {name} @ 0x{f['address']} ═══")
        result.append(body)
        result.append("")
    
    out_file = f"context_path_part_{start_idx}_{end_idx}.c"
    Path(out_file).write_text("\n".join(result), encoding='utf-8')
    print(f"✅ Извлечено {len(part)} функций → {out_file}")

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("""
Использование:
  python find_path.py --path <start> <end> [max_depth]   Найти путь
  python find_path.py --callers <name> [depth]           Кто вызывает
  python find_path.py --callees <name> [depth]           Кого вызывает
  python find_path.py --extract                          Извлечь весь путь
  python find_path.py --extract-part <start> <end>       Извлечь часть пути
""")
        sys.exit(1)
    
    cmd = sys.argv[1]
    
    if cmd == "--path":
        start = sys.argv[2]
        end = sys.argv[3]
        depth = int(sys.argv[4]) if len(sys.argv) > 4 else 20
        cmd_path(start, end, depth)
    elif cmd == "--callers":
        name = sys.argv[2]
        depth = int(sys.argv[3]) if len(sys.argv) > 3 else 3
        cmd_callers(name, depth)
    elif cmd == "--callees":
        name = sys.argv[2]
        depth = int(sys.argv[3]) if len(sys.argv) > 3 else 3
        cmd_callees(name, depth)
    elif cmd == "--extract":
        cmd_extract_path()
    elif cmd == "--extract-part":
        cmd_extract_part(int(sys.argv[2]), int(sys.argv[3]))
        