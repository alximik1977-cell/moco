#!/usr/bin/env python3
"""
Автоматически расширяет контекст пути:
- Для каждой функции на пути извлекает все вызываемые функции (1 уровень)
- Для каждой функции находит все используемые регистры
- Добавляет описания регистров из registers.json
"""
import json
import re
import sys
from pathlib import Path
from collections import OrderedDict

C_FILE = "MH8114F_MH8115F_ES1212_flash-20260413-143030.bin_decompiled.c"
FUNC_JSON = "MH8114F_MH8115F_ES1212_flash-20260413-143030.bin_functions.json"
REG_JSON = "MH8114F_MH8115F_ES1212_flash-20260413-143030.bin_registers.json"


def load_json(path):
    with open(path, 'r', encoding='utf-8') as f:
        return json.load(f)


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


def expand(path_file, output_file, include_callees=True, include_regs=True):
    path = Path(path_file).read_text().strip().split("\n")
    
    func_data = load_json(FUNC_JSON)
    funcs = func_data.get('functions', func_data) if isinstance(func_data, dict) else func_data
    by_name = {f['name']: f for f in funcs}
    
    regs_data = load_json(REG_JSON) if Path(REG_JSON).exists() else {}
    regs = regs_data.get('registers', []) if isinstance(regs_data, dict) else []
    reg_by_name = {r['name']: r for r in regs}
    
    with open(C_FILE, 'r', encoding='utf-8', errors='ignore') as f:
        c_text = f.read()
    
    # Собираем все функции для извлечения
    all_funcs = OrderedDict()  # name -> (source, depth)
    for name in path:
        all_funcs[name] = ("path", 0)
    
    if include_callees:
        for name in path:
            info = by_name.get(name)
            if info:
                for called in info.get('calls', []):
                    if called not in all_funcs and called in by_name:
                        all_funcs[called] = ("callee_of_" + name, 1)
    
    # Собираем все регистры
    all_regs = OrderedDict()
    dat_pattern = re.compile(r'\b(DAT_[0-9a-fA-F]{8})\b')
    
    for name in list(all_funcs.keys()):
        body = find_function_body(name, c_text)
        if body:
            for m in dat_pattern.finditer(body):
                reg_name = m.group(1)
                if reg_name not in all_regs:
                    all_regs[reg_name] = name  # кто использует
    
    result = []
    result.append("// ═══════════════════════════════════════════════════════════")
    result.append("// РАСШИРЕННЫЙ КОНТЕКСТ ПУТИ")
    result.append(f"// Основной путь: {' → '.join(path)}")
    result.append(f"// Всего функций: {len(all_funcs)} (основных: {len(path)}, вызываемых: {len(all_funcs)-len(path)})")
    result.append(f"// Всего регистров: {len(all_regs)}")
    result.append("// ═══════════════════════════════════════════════════════════")
    result.append("")
    
    # Секция 1: Основной путь
    result.append("// ═══════════════════════════════════════════════════════════")
    result.append("// СЕКЦИЯ 1: ОСНОВНОЙ ПУТЬ ВЫЗОВОВ")
    result.append("// ═══════════════════════════════════════════════════════════")
    result.append("")
    
    for i, name in enumerate(path):
        info = by_name.get(name)
        body = find_function_body(name, c_text)
        
        result.append(f"// ═══ [{i+1}/{len(path)}] {name} @ 0x{info['address'] if info else '?'} ═══")
        if info:
            result.append(f"// Сигнатура: {info['signature']}")
            if info.get('calls'):
                result.append(f"// Вызывает: {', '.join(info['calls'])}")
        if body:
            result.append(body)
        else:
            result.append(f"// ⚠️ Тело не найдено")
        result.append("")
    
    # Секция 2: Вызываемые функции
    if include_callees:
        callees_only = {k: v for k, v in all_funcs.items() if v[1] == 1}
        if callees_only:
            result.append("// ═══════════════════════════════════════════════════════════")
            result.append("// СЕКЦИЯ 2: ФУНКЦИИ, ВЫЗЫВАЕМЫЕ ИЗ ОСНОВНОГО ПУТИ")
            result.append("// ═══════════════════════════════════════════════════════════")
            result.append("")
            
            for name, (source, _) in callees_only.items():
                info = by_name.get(name)
                body = find_function_body(name, c_text)
                
                result.append(f"// ─── {name} @ 0x{info['address'] if info else '?'} (вызывается из {source}) ───")
                if info:
                    result.append(f"// Сигнатура: {info['signature']}")
                
                if body:
                    lines = body.split('\n')
                    if len(lines) > 80:
                        result.append('\n'.join(lines[:80]))
                        result.append(f"// ... (обрезано, ещё {len(lines)-80} строк. Используйте: /run python fetch_func.py {name})")
                    else:
                        result.append(body)
                else:
                    result.append(f"// ⚠️ Тело не найдено")
                result.append("")
    
    # Секция 3: Регистры
    if include_regs and all_regs:
        result.append("// ═══════════════════════════════════════════════════════════")
        result.append("// СЕКЦИЯ 3: РЕГИСТРЫ ДАННЫХ, ИСПОЛЬЗУЕМЫЕ НА ПУТИ")
        result.append("// ═══════════════════════════════════════════════════════════")
        result.append("")
        
        for reg_name, used_by in all_regs.items():
            reg_info = reg_by_name.get(reg_name, {})
            addr = reg_info.get('address', reg_name.split('_')[1])
            size = reg_info.get('size', '?')
            rtype = reg_info.get('type', '?')
            access = ', '.join(reg_info.get('access', ['?']))
            region = reg_info.get('region', '?')
            
            result.append(f"// {reg_name} @ 0x{addr} | {size} байт | {rtype} | [{access}] | {region}")
            result.append(f"//   Используется в: {used_by}")
            
            # Показать 2-3 примера использования
            body = find_function_body(used_by, c_text)
            if body:
                lines = body.split('\n')
                examples = [l.strip() for l in lines if reg_name in l][:3]
                for ex in examples:
                    result.append(f"//   Пример: {ex}")
            result.append("")
    
    output = "\n".join(result)
    Path(output_file).write_text(output, encoding='utf-8')
    
    print(f"✅ Расширенный контекст → {output_file}")
    print(f"📏 Размер: {len(output)} символов, {len(output.splitlines())} строк")
    print(f"📊 Функций: {len(all_funcs)}, Регистров: {len(all_regs)}")
    
    if len(output) > 200000:
        print(f"\n⚠️  Файл большой ({len(output)//1024} КБ). GigaChat может терять контекст.")
        print("💡 Используйте /run команды для дозапроса данных.")


if __name__ == "__main__":
    path_file = sys.argv[1] if len(sys.argv) > 1 else "call_path.txt"
    output_file = sys.argv[2] if len(sys.argv) > 2 else "context_expanded.c"
    expand(path_file, output_file)
    