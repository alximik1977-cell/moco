#!/usr/bin/env python3
"""
Расширенный поиск связей между функциями, когда прямой путь не найден.
Ищет через:
  1. Общие вызовы (обе функции вызывают одну и ту же)
  2. Общие строковые константы
  3. Общие адреса/регистры в теле функции
  4. Функции-посредники (A → X → ... → Y → B)
"""
import json
import re
from collections import defaultdict
from pathlib import Path

JSON_FILE = "MH8114F_MH8115F_ES1212_flash-20260413-143030.bin_functions.json"
C_FILE = "MH8114F_MH8115F_ES1212_flash-20260413-143030.bin_decompiled.c"

def load_data():
    with open(JSON_FILE, 'r', encoding='utf-8') as f:
        funcs = json.load(f)
    by_name = {f['name']: f for f in funcs}
    by_addr = {f['address']: f for f in funcs}
    
    callees = defaultdict(set)
    callers = defaultdict(set)
    for f in funcs:
        for called_addr in f.get('calls', []):
            called = by_addr.get(called_addr)
            if called:
                callees[f['name']].add(called['name'])
                callers[called['name']].add(f['name'])
    
    return funcs, by_name, by_addr, callees, callers

def get_function_body(name, c_text):
    pattern = re.compile(
        rf'(?m)^[a-zA-Z_][\w\s\*]*\s+{re.escape(name)}\s*\([^)]*\)\s*\{{',
        re.MULTILINE
    )
    m = pattern.search(c_text)
    if not m:
        return ""
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
    return ""

def find_common_callees(start, end, callees, depth=3):
    """Ищем функции, которые вызываются и из start, и из end (с учётом глубины)."""
    def get_all_callees(name, d, visited):
        if d > depth or name in visited:
            return set()
        visited.add(name)
        result = set()
        for c in callees.get(name, []):
            result.add(c)
            result |= get_all_callees(c, d+1, visited)
        return result
    
    start_set = get_all_callees(start, 0, set())
    end_set = get_all_callees(end, 0, set())
    common = start_set & end_set
    return common

def find_bridges(start, end, callees, callers, max_bridge_depth=2):
    """Ищем функции-мосты: start → X → ... → Y → end."""
    # Все достижимые из start
    def reachable_from(name, depth):
        result = {name: 0}
        queue = [(name, 0)]
        while queue:
            cur, d = queue.pop(0)
            if d >= depth:
                continue
            for c in callees.get(cur, []):
                if c not in result:
                    result[c] = d + 1
                    queue.append((c, d + 1))
        return result
    
    # Все, откуда достижим end (через обратный граф)
    def reachable_to(name, depth):
        result = {name: 0}
        queue = [(name, 0)]
        while queue:
            cur, d = queue.pop(0)
            if d >= depth:
                continue
            for c in callers.get(cur, []):
                if c not in result:
                    result[c] = d + 1
                    queue.append((c, d + 1))
        return result
    
    fwd = reachable_from(start, max_bridge_depth)
    bwd = reachable_to(end, max_bridge_depth)
    
    bridges = []
    for name in set(fwd) & set(bwd):
        if name != start and name != end:
            bridges.append((name, fwd[name], bwd[name]))
    
    bridges.sort(key=lambda x: x[1] + x[2])
    return bridges

def find_common_strings(start, end, c_text):
    """Ищем общие строковые константы в телах функций."""
    body_start = get_function_body(start, c_text)
    body_end = get_function_body(end, c_text)
    
    str_pattern = re.compile(r'"([^"\\]|\\.)*"')
    strs_start = set(str_pattern.findall(body_start))
    strs_end = set(str_pattern.findall(body_end))
    
    return strs_start & strs_end

def find_common_addresses(start, end, c_text):
    """Ищем общие hex-адреса (0x...) в телах функций."""
    body_start = get_function_body(start, c_text)
    body_end = get_function_body(end, c_text)
    
    addr_pattern = re.compile(r'0x[0-9a-fA-F]{4,}')
    addrs_start = set(addr_pattern.findall(body_start))
    addrs_end = set(addr_pattern.findall(body_end))
    
    return addrs_start & addrs_end

def find_by_name_pattern(pattern):
    """Ищем функции по паттерну в имени."""
    funcs, by_name, _, _, _ = load_data()
    matches = [f for f in funcs if re.search(pattern, f['name'], re.IGNORECASE)]
    return matches

def main():
    import sys
    if len(sys.argv) < 2:
        print("""
Использование:
  python find_link.py --analyze <start> <end>     Полный анализ связей
  python find_link.py --bridges <start> <end>     Функции-мосты
  python find_link.py --search <pattern>          Поиск функций по имени
  python find_link.py --body <name>               Показать тело функции
""")
        return
    
    cmd = sys.argv[1]
    funcs, by_name, by_addr, callees, callers = load_data()
    
    with open(C_FILE, 'r', encoding='utf-8', errors='ignore') as f:
        c_text = f.read()
    
    if cmd == "--analyze":
        start, end = sys.argv[2], sys.argv[3]
        print(f"🔗 Анализ связей: {start} ↔ {end}\n")
        
        print(f"📤 Прямые вызовы {start}: {len(callees[start])}")
        for c in list(callees[start])[:15]:
            print(f"   → {c}")
        if len(callees[start]) > 15:
            print(f"   ... и ещё {len(callees[start])-15}")
        
        print(f"\n📥 Кто вызывает {end}: {len(callers[end])}")
        for c in list(callers[end])[:15]:
            print(f"   ← {c}")
        if len(callers[end]) > 15:
            print(f"   ... и ещё {len(callers[end])-15}")
        
        print(f"\n🎯 Общие callees (глубина 3):")
        common = find_common_callees(start, end, callees, 3)
        if common:
            for c in list(common)[:20]:
                print(f"   • {c}")
        else:
            print("   (нет)")
        
        print(f"\n🔤 Общие строки:")
        strs = find_common_strings(start, end, c_text)
        if strs:
            for s in list(strs)[:10]:
                print(f"   • \"{s}\"")
        else:
            print("   (нет)")
        
        print(f"\n📍 Общие адреса (0x...):")
        addrs = find_common_addresses(start, end, c_text)
        if addrs:
            for a in list(addrs)[:15]:
                print(f"   • {a}")
        else:
            print("   (нет)")
    
    elif cmd == "--bridges":
        start, end = sys.argv[2], sys.argv[3]
        print(f"🌉 Функции-мосты между {start} и {end}:\n")
        bridges = find_bridges(start, end, callees, callers, max_bridge_depth=4)
        if bridges:
            for name, d_fwd, d_bwd in bridges[:30]:
                print(f"   {start} →[{d_fwd}]→ {name} →[{d_bwd}]→ {end}")
        else:
            print("   Мосты не найдены. Увеличьте глубину или ищите через данные.")
    
    elif cmd == "--search":
        pattern = sys.argv[2]
        matches = find_by_name_pattern(pattern)
        print(f"🔍 Найдено {len(matches)} функций:")
        for f in matches[:50]:
            print(f"   {f['name']:50s} @ 0x{f['address']}")
    
    elif cmd == "--body":
        name = sys.argv[2]
        body = get_function_body(name, c_text)
        if body:
            print(body)
        else:
            print(f"❌ Тело функции {name} не найдено")

if __name__ == "__main__":
    main()