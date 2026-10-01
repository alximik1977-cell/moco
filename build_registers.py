#!/usr/bin/env python3
"""
Создаёт JSON с описанием регистров/переменных данных и функциями, которые к ним обращаются.
Поддерживает ЛЮБЫЕ имена: DAT_xxx, PTR_xxx, s_xxx, CAN_STATUS, IMMO_FLAGS_CONF_DAT_xxx, и т.д.

Источники данных:
  1. Глобальные объявления переменных (в начале C-файла и между функциями)
  2. Использование идентификаторов в телах функций
  3. Комментарии Ghidra (адреса, типы)
  4. 🔧 НОВОЕ: Все идентификаторы в телах функций, похожие на регистры
"""
import json
import re
from pathlib import Path
from collections import defaultdict

# ===== НАСТРОЙКИ =====
C_FILE = "MH8114F_MH8115F_ES1212_flash-20260413-143030.bin_decompiled.c"
FUNC_JSON = "MH8114F_MH8115F_ES1212_flash-20260413-143030.bin_functions.json"
OUT_FILE = "MH8114F_MH8115F_ES1212_flash-20260413-143030.bin_registers.json"

# Ключевые слова C и типы Ghidra
C_KEYWORDS = {
    'if', 'else', 'while', 'for', 'do', 'switch', 'case', 'break', 'continue',
    'return', 'goto', 'default', 'sizeof', 'typedef', 'struct', 'union', 'enum',
    'void', 'int', 'char', 'short', 'long', 'float', 'double', 'signed', 'unsigned',
    'const', 'volatile', 'static', 'extern', 'auto', 'register', 'inline',
    'undefined', 'undefined1', 'undefined2', 'undefined4', 'undefined8',
    'byte', 'ushort', 'uint', 'ulong', 'int8', 'int16', 'int32', 'int64',
    'uint8', 'uint16', 'uint32', 'uint64', 'bool', 'true', 'false', 'NULL',
    'wchar', 'char16_t', 'char32_t', 'size_t', 'ssize_t', 'ptrdiff_t',
}

# Префиксы функций (не данные)
NON_DATA_PREFIXES = ('FUN_',)

# 🔧 НОВОЕ: Служебные имена Ghidra — НЕ являются регистрами данных
GHIDRA_VIRTUAL_PREFIXES = (
    'extraout_', 'unaff_', 'in_stack_', 'Stack_', 'undef_',
)

GHIDRA_VIRTUAL_PATTERNS = (
    re.compile(r'^(u|i|b|c|f|d|p|s)Var\d+$'),
    re.compile(r'^(in|out)_[a-z]\w+$'),
    re.compile(r'^local_[a-zA-Z0-9_]+$'),
    re.compile(r'^(extra|unaff|undef)_[a-zA-Z0-9_]+$'),
)

# 🔧 НОВОЕ: Префиксы, указывающие на тип данных (расширенный список)
DATA_PREFIXES = {
    'DAT_': 'data',
    'PTR_': 'pointer',
    's_':   'string',
    'c_':   'char_const',
    'au_':  'array_unsigned',
    'a_':   'array',
    'u_':   'unsigned_data',
    'i_':   'signed_data',
    'f_':   'float_data',
}

# 🔧 НОВОЕ: Известные смысловые префиксы из прошивки
KNOWN_MEANINGFUL_PREFIXES = {
    'IMMO', 'CAN', 'UART', 'SPI', 'I2C', 'ADC', 'PWM', 'TIMER', 'DTC',
    'FUEL', 'IGN', 'INJ', 'LAMBDA', 'O2', 'MAP', 'MAF', 'TPS', 'IAT', 'ECT',
    'VSS', 'RPM', 'KNOCK', 'EGR', 'VVT', 'IDLE', 'AC', 'FAN', 'RELAY',
    'EEPROM', 'FLASH', 'RAM', 'ROM', 'DIAG', 'SECURITY', 'CONFIG',
    'STATUS', 'FLAG', 'FLAGS', 'STATE', 'MODE', 'COUNTER', 'TIMER',
    'BLOCK', 'NORM', 'INIT', 'WATCHDOG', 'UTIL', 'MAIN', 'MON',
    'A0', 'A2', 'A7', 'A8', 'B1', 'B2', 'C1', 'C3', 'M0', 'M1', 'M2',
    'N4', 'N16',
}


def is_ghidra_virtual(name):
    """Проверяет, является ли имя служебным именем Ghidra."""
    for prefix in GHIDRA_VIRTUAL_PREFIXES:
        if name.startswith(prefix):
            return True
    for pattern in GHIDRA_VIRTUAL_PATTERNS:
        if pattern.match(name):
            return True
    return False


def looks_like_register(name):
    """
    🔧 НОВОЕ: Определяет, похоже ли имя на регистр/переменную данных.
    Это эвристика, которая отсеивает случайные идентификаторы.
    """
    if not name or len(name) < 3:
        return False
    
    # Ключевые слова
    if name in C_KEYWORDS:
        return False
    
    # Служебные имена Ghidra
    if is_ghidra_virtual(name):
        return False
    
    # Имена функций
    if any(name.startswith(p) for p in NON_DATA_PREFIXES):
        return False
    
    # Стандартные префиксы данных
    if any(name.startswith(p) for p in DATA_PREFIXES.keys()):
        return True
    
    # 🔧 НОВОЕ: Содержит DAT_, PTR_ в середине (составные имена)
    if re.search(r'_(DAT|PTR|s|c)_', name):
        return True
    
    # 🔧 НОВОЕ: Содержит известный смысловой префикс
    parts = name.split('_')
    if any(p.upper() in KNOWN_MEANINGFUL_PREFIXES for p in parts):
        return True
    
    # 🔧 НОВОЕ: Имя в верхнем регистре с подчёркиваниями (типичный стиль для регистров)
    if name.isupper() and '_' in name and len(name) > 5:
        return True
    
    # 🔧 НОВОЕ: Имя содержит hex-адрес (8 цифр)
    if re.search(r'[0-9a-fA-F]{8}', name):
        return True
    
    # 🔧 НОВОЕ: Имя начинается с заглавной буквы и содержит подчёркивания
    if name[0].isupper() and '_' in name and len(parts) >= 2:
        return True
    
    return False


def load_functions_meta():
    """Загружает метаданные функций из JSON."""
    if not Path(FUNC_JSON).exists():
        return {}
    with open(FUNC_JSON, 'r', encoding='utf-8') as f:
        data = json.load(f)
    funcs = data.get('functions', data) if isinstance(data, dict) else data
    return {f['name']: f for f in funcs}


def parse_c_file(c_text):
    """Парсит C-файл: глобальные объявления, функции, адреса из комментариев."""
    result = {
        'globals': {},
        'functions': {},
        'address_comments': {}
    }
    
    lines = c_text.split('\n')
    
    # 1. Адреса из комментариев Ghidra: /* NAME @ 0xADDRESS */
    addr_pattern = re.compile(r'/\*\s*([A-Za-z0-9_]+)\s*@\s*([0-9a-fA-F]+)\s*\*/')
    for i, line in enumerate(lines):
        m = addr_pattern.search(line)
        if m:
            name = m.group(1)
            addr = m.group(2)
            result['address_comments'][name] = addr
    
    # 2. Парсим функции и глобальные объявления
    i = 0
    while i < len(lines):
        line = lines[i]
        stripped = line.strip()
        
        if not stripped or stripped.startswith('//') or stripped.startswith('/*'):
            i += 1
            continue
        
        # Определение функции
        func_pattern = re.compile(
            r'^([\w\s\*]+?)\s+([A-Za-z0-9_]+)\s*\([^)]*\)\s*\{?\s*$'
        )
        m = func_pattern.match(line)
        if m:
            ret_type = m.group(1).strip()
            name = m.group(2)
            
            if '(' in line and ')' in line:
                start_line = i
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
                                body = '\n'.join(lines[start_line:j+1])
                                result['functions'][name] = {
                                    'start': start_line,
                                    'end': j,
                                    'body': body,
                                    'ret_type': ret_type
                                }
                                i = j
                                break
                    j += 1
                    if found_open and depth == 0:
                        break
                i += 1
                continue
        
        # Глобальное объявление переменной
        # 🔧 ИСПРАВЛЕНО: более широкий паттерн, принимает имена с цифрами в начале
        global_pattern = re.compile(
            r'^\s*([\w\s\*]+?)\s+([A-Za-z_][A-Za-z0-9_]*)\s*'
            r'(?:\[\s*(\d*)\s*\])?\s*'
            r'(?:=\s*[^;]+)?\s*;\s*(?://.*)?$'
        )
        m = global_pattern.match(line)
        if m:
            var_type = m.group(1).strip()
            var_name = m.group(2)
            array_size = m.group(3)
            
            if var_name in C_KEYWORDS:
                i += 1
                continue
            if any(var_name.startswith(p) for p in NON_DATA_PREFIXES):
                i += 1
                continue
            if is_ghidra_virtual(var_name):
                i += 1
                continue
            
            size = guess_type_size(var_type)
            addr = result['address_comments'].get(var_name)
            
            result['globals'][var_name] = {
                'type': var_type,
                'size': size,
                'address': addr,
                'line': i,
                'is_array': array_size is not None,
                'array_size': int(array_size) if array_size and array_size.isdigit() else None,
                'is_pointer': '*' in var_type,
            }
        
        i += 1
    
    return result


def guess_type_size(type_str):
    """Определяет размер типа по его имени."""
    type_str = type_str.lower().strip()
    
    size_map = {
        'undefined1': 1, 'byte': 1, 'char': 1, 'uint8': 1, 'int8': 1,
        'undefined2': 2, 'ushort': 2, 'short': 2, 'uint16': 2, 'int16': 2, 'wchar': 2,
        'undefined4': 4, 'uint': 4, 'int': 4, 'ulong': 4, 'long': 4, 
        'float': 4, 'uint32': 4, 'int32': 4,
        'undefined8': 8, 'ulonglong': 8, 'longlong': 8, 'double': 8,
        'uint64': 8, 'int64': 8,
    }
    
    for t, s in size_map.items():
        if t in type_str:
            return s
    
    if '*' in type_str:
        return 4
    
    return 4


def find_identifiers_in_body(body, known_globals, known_functions):
    """
    🔧 ИСПРАВЛЕНО: Находит ВСЕ идентификаторы, похожие на регистры.
    Больше не ограничивается префиксами DAT_, PTR_ и т.д.
    """
    local_vars = set()
    
    # Локальные переменные Ghidra
    local_pattern = re.compile(
        r'(?:^|\n)\s*(?:[\w\s\*]+?)\s+(local_\w+|uVar\d+|iVar\d+|bVar\d+|cVar\d+|fVar\d+|dVar\d+|pVar\d+|sVar\d+)'
    )
    for m in local_pattern.finditer(body):
        local_vars.add(m.group(1))
    
    # Параметры функции
    sig_match = re.match(r'^[\w\s\*]+\s+\w+\s*\(([^)]*)\)', body)
    if sig_match:
        params_str = sig_match.group(1)
        for param in re.findall(r'\b([A-Za-z_]\w*)\b', params_str):
            if param not in C_KEYWORDS:
                local_vars.add(param)
    
    identifier_pattern = re.compile(r'\b([A-Za-z_][A-Za-z0-9_]*)\b')
    found = set()
    
    for m in identifier_pattern.finditer(body):
        name = m.group(1)
        
        if name in C_KEYWORDS:
            continue
        if name in local_vars:
            continue
        if name in known_functions:
            continue
        if is_ghidra_virtual(name):
            continue
        # Вызов функции — не регистр
        if m.end() < len(body) and body[m.end():m.end()+1] == '(':
            continue
        
        # 🔧 ИСПРАВЛЕНО: используем эвристику вместо жёсткого списка префиксов
        if name in known_globals or looks_like_register(name):
            found.add(name)
    
    return found


def analyze_access(name, body):
    """Определяет тип доступа: read, write или read+write."""
    access = set()
    
    if re.search(rf'\b{re.escape(name)}\s*=[^=]', body):
        access.add('write')
    if re.search(rf'\b{re.escape(name)}\s*(\+|-|\*|/|&|\||\^)=', body):
        access.add('write')
        access.add('read')
    if re.search(rf'\b{re.escape(name)}\s*(\+\+|--)', body):
        access.add('write')
        access.add('read')
    if re.search(rf'[^=!<>]=[^=]*\b{re.escape(name)}\b', body):
        access.add('read')
    if re.search(rf'(?:if|while|return|,)\s*[^=]*\b{re.escape(name)}\b', body):
        access.add('read')
    if re.search(rf'[\+\-\*/&\|\^<>]\s*[^=]*\b{re.escape(name)}\b', body):
        access.add('read')
    if re.search(rf'\b{re.escape(name)}\b\s*[\+\-\*/&\|\^<>]', body):
        access.add('read')
    if re.search(rf'\(\s*[^)]*\b{re.escape(name)}\b[^)]*\)', body):
        access.add('read')
    if re.search(rf'&\s*{re.escape(name)}\b', body):
        access.add('read')
    if re.search(rf'\*\s*{re.escape(name)}\b', body):
        access.add('read')
    
    if not access:
        if re.search(rf'\b{re.escape(name)}\b', body):
            access.add('read')
    
    return sorted(access) if access else ['unknown']


def classify_region(addr_int, name):
    """Классифицирует регистр по адресу (для SuperH)."""
    if addr_int is None:
        if name.startswith('s_'):
            return 'string_literal'
        if name.startswith('PTR_'):
            return 'pointer_table'
        return 'unknown'
    
    if 0xFFF80000 <= addr_int <= 0xFFFFFFFF:
        return 'peripheral_mirror'
    elif 0xFF000000 <= addr_int <= 0xFFEFFFFF:
        return 'peripheral'
    elif 0x08000000 <= addr_int <= 0x0FFFFFFF:
        return 'external_ram'
    elif 0x00000000 <= addr_int <= 0x000FFFFF:
        return 'flash_rom'
    elif 0x20000000 <= addr_int <= 0x2FFFFFFF:
        return 'internal_ram'
    else:
        return 'data'


def classify_by_name(name):
    """Определяет тип регистра по имени."""
    for prefix, kind in DATA_PREFIXES.items():
        if name.startswith(prefix):
            return kind
    
    name_lower = name.lower()
    if any(p in name_lower for p in ['status', 'stat', 'sts', 'flag']):
        return 'status_register'
    if any(p in name_lower for p in ['config', 'cfg', 'setting']):
        return 'configuration'
    if any(p in name_lower for p in ['counter', 'cnt', 'count']):
        return 'counter'
    if any(p in name_lower for p in ['buffer', 'buf', 'fifo']):
        return 'buffer'
    if any(p in name_lower for p in ['table', 'tbl', 'map']):
        return 'table'
    if any(p in name_lower for p in ['state', 'mode', 'phase']):
        return 'state_variable'
    if any(p in name_lower for p in ['timer', 'tmr']):
        return 'timer'
    if any(p in name_lower for p in ['immo', 'immobilizer', 'security']):
        return 'security'
    if any(p in name_lower for p in ['fuel', 'inject', 'inj']):
        return 'fuel_system'
    if any(p in name_lower for p in ['ign', 'ignition']):
        return 'ignition'
    if any(p in name_lower for p in ['can']):
        return 'can_bus'
    if any(p in name_lower for p in ['uart']):
        return 'uart'
    if any(p in name_lower for p in ['adc']):
        return 'adc'
    if any(p in name_lower for p in ['pwm']):
        return 'pwm'
    if any(p in name_lower for p in ['lambda', 'o2']):
        return 'lambda_control'
    if any(p in name_lower for p in ['dtc', 'diag', 'diagnostic']):
        return 'diagnostics'
    if any(p in name_lower for p in ['can', 'uart', 'spi', 'i2c', 'adc', 'pwm']):
        return 'peripheral_data'
    if name.startswith('g_') or name.startswith('g'):
        return 'global_variable'
    
    return 'general_data'


def build_registers_json():
    """Основная функция — строит JSON с регистрами."""
    print(f"📖 Читаю {C_FILE}...")
    with open(C_FILE, 'r', encoding='utf-8', errors='ignore') as f:
        c_text = f.read()
    
    print(f"🔧 Парсю C-файл...")
    parsed = parse_c_file(c_text)
    print(f"   ✅ Найдено глобальных переменных: {len(parsed['globals'])}")
    print(f"   ✅ Найдено функций: {len(parsed['functions'])}")
    print(f"   ✅ Найдено адресов в комментариях: {len(parsed['address_comments'])}")
    
    func_meta = load_functions_meta()
    known_func_names = set(parsed['functions'].keys()) | set(func_meta.keys())
    known_globals = set(parsed['globals'].keys())
    
    print(f"🔍 Анализирую использование регистров в функциях...")
    registers = defaultdict(lambda: {
        'refs': set(),
        'access': set(),
        'bodies': [],
    })
    
    filtered_count = 0
    new_regs_from_body = 0  # 🔧 НОВОЕ: счётчик регистров, найденных только в телах
    
    for func_name, func_data in parsed['functions'].items():
        body = func_data['body']
        
        for m in re.finditer(r'\b(extraout_\w+|unaff_\w+|local_\w+|[uibcf dps]Var\d+|in_\w+|out_\w+)\b', body):
            filtered_count += 1
        
        found = find_identifiers_in_body(body, known_globals, known_func_names)
        
        for reg_name in found:
            registers[reg_name]['refs'].add(func_name)
            registers[reg_name]['bodies'].append(body)
            access = analyze_access(reg_name, body)
            registers[reg_name]['access'].update(access)
            
            # 🔧 НОВОЕ: считаем регистры, которых нет в глобальных объявлениях
            if reg_name not in known_globals:
                new_regs_from_body += 1
    
    print(f"   🗑️  Отфильтровано служебных имён Ghidra: {filtered_count}")
    print(f"   🆕 Найдено регистров только в телах функций: {new_regs_from_body}")
    print(f"✅ Найдено {len(registers)} уникальных регистров/переменных")
    
    # Формируем выходной JSON
    result = {
        "program": "MH8114F_MH8115F_ES1212_flash-20260413-143030.bin",
        "architecture": "SuperH",
        "total_registers": len(registers),
        "registers": []
    }
    
    for name, data in sorted(registers.items()):
        decl = parsed['globals'].get(name, {})
        
        addr = decl.get('address') or parsed['address_comments'].get(name)
        
        # 🔧 НОВОЕ: если адреса нет, пытаемся извлечь из имени
        if not addr:
            m = re.search(r'(?:DAT|PTR|s|c|au|a|u|i|f)_([0-9a-fA-F]{8})', name)
            if m:
                addr = m.group(1)
        
        addr_int = None
        if addr:
            try:
                addr_int = int(addr, 16)
            except:
                pass
        
        size = decl.get('size', 4)
        type_name = decl.get('type', 'unknown')
        is_array = decl.get('is_array', False)
        array_size = decl.get('array_size')
        is_pointer = decl.get('is_pointer', False)
        
        if is_array and array_size:
            size = size * array_size
        
        region = classify_region(addr_int, name)
        kind = classify_by_name(name)
        
        refs_list = sorted(data['refs'])
        access_list = sorted(data['access'])
        
        reg_entry = {
            "name": name,
            "address": addr or "unknown",
            "address_int": addr_int,
            "size": size,
            "type": type_name,
            "kind": kind,
            "access": access_list,
            "region": region,
            "is_array": is_array,
            "array_size": array_size,
            "is_pointer": is_pointer,
            "references": refs_list,
            "reference_count": len(refs_list)
        }
        
        result["registers"].append(reg_entry)
    
    result["registers"].sort(key=lambda r: r["reference_count"], reverse=True)
    
    result["statistics"] = {
        "total_registers": len(result["registers"]),
        "with_address": sum(1 for r in result["registers"] if r["address"] != "unknown"),
        "without_address": sum(1 for r in result["registers"] if r["address"] == "unknown"),
        "filtered_ghidra_virtual": filtered_count,
        "found_only_in_bodies": new_regs_from_body,
        "by_kind": {},
        "by_region": {},
        "by_access": {"read": 0, "write": 0, "read_write": 0, "unknown": 0},
        "arrays": sum(1 for r in result["registers"] if r["is_array"]),
        "pointers": sum(1 for r in result["registers"] if r["is_pointer"]),
        "most_referenced": result["registers"][:20]
    }
    
    for r in result["registers"]:
        kind = r["kind"]
        result["statistics"]["by_kind"][kind] = \
            result["statistics"]["by_kind"].get(kind, 0) + 1
        
        region = r["region"]
        result["statistics"]["by_region"][region] = \
            result["statistics"]["by_region"].get(region, 0) + 1
        
        acc = tuple(sorted(r["access"]))
        if acc == ("read",):
            result["statistics"]["by_access"]["read"] += 1
        elif acc == ("write",):
            result["statistics"]["by_access"]["write"] += 1
        elif "read" in acc and "write" in acc:
            result["statistics"]["by_access"]["read_write"] += 1
        else:
            result["statistics"]["by_access"]["unknown"] += 1
    
    with open(OUT_FILE, 'w', encoding='utf-8') as f:
        json.dump(result, f, indent=2, ensure_ascii=False)
    
    print(f"\n💾 Сохранено в {OUT_FILE}")
    print(f"\n📊 Статистика:")
    print(f"   Всего регистров/переменных: {result['statistics']['total_registers']}")
    print(f"   Отфильтровано служебных имён: {result['statistics']['filtered_ghidra_virtual']}")
    print(f"   Найдено только в телах функций: {result['statistics']['found_only_in_bodies']}")
    print(f"   С известным адресом: {result['statistics']['with_address']}")
    print(f"   Без адреса: {result['statistics']['without_address']}")
    print(f"   Массивов: {result['statistics']['arrays']}")
    print(f"   Указателей: {result['statistics']['pointers']}")
    
    print(f"\n📦 По типу:")
    for kind, count in sorted(result['statistics']['by_kind'].items(), key=lambda x: -x[1]):
        print(f"   {kind:25s}: {count}")
    
    print(f"\n🗺️  По регионам памяти:")
    for region, count in sorted(result['statistics']['by_region'].items(), key=lambda x: -x[1]):
        print(f"   {region:25s}: {count}")
    
    print(f"\n🔀 По типу доступа:")
    for acc, count in result['statistics']['by_access'].items():
        print(f"   {acc:25s}: {count}")
    
    print(f"\n🔝 Топ-20 самых используемых:")
    for r in result['statistics']['most_referenced']:
        addr_str = f"0x{r['address']:>10s}" if r['address'] != 'unknown' else '         ?'
        print(f"   {r['name']:40s} {addr_str}  ({r['reference_count']:3d} refs)  [{','.join(r['access'])}]  [{r['kind']}]")


if __name__ == "__main__":
    build_registers_json()
    