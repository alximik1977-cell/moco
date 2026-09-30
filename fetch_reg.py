#!/usr/bin/env python3
"""
Извлекает информацию о регистрах данных (DAT_xxxxxxxx).
Используется как инструмент для Aider/GigaChat.
"""
import json
import re
import sys
from pathlib import Path

C_FILE = "MH8114F_MH8115F_ES1212_flash-20260413-143030.bin_decompiled.c"
REG_JSON = "MH8114F_MH8115F_ES1212_flash-20260413-143030.bin_registers.json"
FUNC_JSON = "MH8114F_MH8115F_ES1212_flash-20260413-143030.bin_functions.json"


def load_registers():
    if not Path(REG_JSON).exists():
        return None
    with open(REG_JSON, 'r', encoding='utf-8') as f:
        data = json.load(f)
    return data.get('registers', [])


def load_functions():
    with open(FUNC_JSON, 'r', encoding='utf-8') as f:
        data = json.load(f)
    if isinstance(data, dict) and 'functions' in data:
        return data['functions']
    return data if isinstance(data, list) else []


def find_usages_in_c(reg_name, c_text, context_lines=3):
    """Находит все строки с использованием регистра в C-файле."""
    lines = c_text.split('\n')
    pattern = re.compile(r'\b' + re.escape(reg_name) + r'\b')
    results = []
    
    for i, line in enumerate(lines):
        if pattern.search(line):
            start = max(0, i - context_lines)
            end = min(len(lines), i + context_lines + 1)
            results.append({
                'line': i + 1,
                'context': '\n'.join(lines[start:end])
            })
    
    return results


def cmd_fetch(reg_name):
    """Извлечь информацию о регистре."""
    regs = load_registers()
    
    if regs:
        # Ищем в JSON регистров
        match = next((r for r in regs if r['name'] == reg_name), None)
        if match:
            print(f"// ═══ Регистр {reg_name} ═══")
            print(f"// Адрес: 0x{match['address']}")
            print(f"// Размер: {match.get('size', '?')} байт")
            print(f"// Тип: {match.get('type', '?')}")
            print(f"// Доступ: {', '.join(match.get('access', []))}")
            print(f"// Регион: {match.get('region', '?')}")
            print(f"// Используется в {match.get('reference_count', '?')} функциях:")
            for ref in match.get('references', [])[:15]:
                print(f"//   → {ref}")
            print()
        else:
            print(f"// ⚠️ Регистр {reg_name} не найден в registers.json")
    else:
        print(f"// ⚠️ Файл registers.json не найден")
    
    # Показываем использования в C-файле
    with open(C_FILE, 'r', encoding='utf-8', errors='ignore') as f:
        c_text = f.read()
    
    usages = find_usages_in_c(reg_name, c_text)
    if usages:
        print(f"// Использование в коде ({len(usages)} упоминаний):")
        for u in usages[:10]:
            print(f"// --- строка {u['line']} ---")
            for line in u['context'].split('\n'):
                print(f"//   {line}")
            print()
        if len(usages) > 10:
            print(f"// ... и ещё {len(usages)-10} упоминаний")


def cmd_by_addr(addr):
    """Найти регистр по адресу."""
    addr_clean = addr.lower().lstrip('0x')
    regs = load_registers()
    
    if not regs:
        print("⚠️ registers.json не найден")
        return
    
    matches = [r for r in regs if r['address'].lower().lstrip('0') == addr_clean.lstrip('0')]
    
    if matches:
        for m in matches:
            cmd_fetch(m['name'])
    else:
        # Ищем в C-файле по адресу
        with open(C_FILE, 'r', encoding='utf-8', errors='ignore') as f:
            c_text = f.read()
        
        pattern = re.compile(rf'DAT_[0-9a-fA-F]*{re.escape(addr_clean)}', re.IGNORECASE)
        found = set(pattern.findall(c_text))
        
        if found:
            print(f"// Найдены регистры с адресом, содержащим {addr}:")
            for name in found:
                print(f"//   {name}")
        else:
            print(f"❌ Регистр с адресом 0x{addr} не найден")


def cmd_used_in(func_name):
    """Найти все регистры, используемые в функции."""
    with open(C_FILE, 'r', encoding='utf-8', errors='ignore') as f:
        c_text = f.read()
    
    # Находим тело функции
    pattern = re.compile(
        rf'(?m)^[\w\s\*]+\s+{re.escape(func_name)}\s*\([^)]*\)\s*\{{',
        re.MULTILINE
    )
    m = pattern.search(c_text)
    if not m:
        print(f"❌ Функция {func_name} не найдена")
        return
    
    start = m.start()
    depth = 0
    i = m.end() - 1
    while i < len(c_text):
        if c_text[i] == '{':
            depth += 1
        elif c_text[i] == '}':
            depth -= 1
            if depth == 0:
                body = c_text[start:i+1]
                break
        i += 1
    else:
        print(f"❌ Не удалось найти конец функции")
        return
    
    # Ищем все DAT_ в теле
    dat_pattern = re.compile(r'\b(DAT_[0-9a-fA-F]{8})\b')
    regs_found = sorted(set(dat_pattern.findall(body)))
    
    print(f"// Регистры, используемые в {func_name} ({len(regs_found)}):")
    
    regs = load_registers()
    reg_by_name = {r['name']: r for r in regs} if regs else {}
    
    for reg_name in regs_found:
        info = reg_by_name.get(reg_name, {})
        addr = info.get('address', reg_name.split('_')[1])
        access = ', '.join(info.get('access', ['?']))
        region = info.get('region', '?')
        print(f"//   {reg_name:20s} @ 0x{addr}  [{access}]  ({region})")


if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Использование:")
        print("  python fetch_reg.py <DAT_xxxxxxxx>           Информация о регистре")
        print("  python fetch_reg.py --addr <xxxxxxxx>        Поиск по адресу")
        print("  python fetch_reg.py --used-in <func_name>    Регистры в функции")
        sys.exit(1)
    
    if sys.argv[1] == "--addr":
        cmd_by_addr(sys.argv[2])
    elif sys.argv[1] == "--used-in":
        cmd_used_in(sys.argv[2])
    else:
        cmd_fetch(sys.argv[1])
        