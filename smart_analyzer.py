#!/usr/bin/env python3
"""
Умный анализатор прошивки с автоматическим извлечением контекста.
GigaChat сам запрашивает нужные функции и регистры через маркеры.

Особенности:
  - BFS-поиск путей между функциями
  - Автоподтягивание регистров из тел функций
  - Умный поиск регистров по составным именам Ghidra
  - Поддержка имён функций с нормализацией Ghidra
  - Парсер естественных запросов "покажи путь от X до Y"
  - Подробный анализ регистров

Использование:
  python smart_analyzer.py                                          # интерактивный
  python smart_analyzer.py --model GigaChat-3-Lightning "Вопрос"    # одноразовый
  python smart_analyzer.py --path START END                         # найти путь
  python smart_analyzer.py --list-models                            # список моделей
"""
import json
import re
import sys
import requests
import argparse
from pathlib import Path
from collections import deque, defaultdict

# ===== НАСТРОЙКИ =====
PROXY_URL = "http://127.0.0.1:8000/v1/chat/completions"
C_FILE = "MH8114F_MH8115F_ES1212_flash-20260413-143030.bin_decompiled.c"
FUNC_JSON = "MH8114F_MH8115F_ES1212_flash-20260413-143030.bin_functions.json"
REG_JSON = "MH8114F_MH8115F_ES1212_flash-20260413-143030.bin_registers.json"

CONFIG = {
    "model": "GigaChat-3-Ultra",
    "max_fetch_rounds": 5,
    "max_context_chars": 80000,
    "auto_attach_regs": True,
    "max_regs_per_function": 15,
}

AVAILABLE_MODELS = [
    "GigaChat-3-Ultra",
    "GigaChat-3-Pro",
    "GigaChat-3-Lightning",
    "GigaChat-2-Max",
    "GigaChat-2-Pro",
    "GigaChat-2",
]
# =====================


# ─── Утилиты для работы с именами регистров ───

def extract_address_from_name(name):
    """
    Извлекает адрес из составного имени регистра Ghidra.
    
    Примеры:
      IMMO_FLAGS_CONF_DAT_fff84540 → fff84540
      CAN_STATUS_DAT_fff88200      → fff88200
      DAT_fff88344                 → fff88344
      PTR_08001234                 → 08001234
      s_hello_world                → None (нет адреса)
    """
    if not name:
        return None
    
    # Паттерн: DAT_/PTR_/s_ + 8 hex цифр (может быть в конце или в середине)
    m = re.search(r'(?:DAT|PTR|s|c|au|a|u|i|f)_([0-9a-fA-F]{8})(?:$|_)', name)
    if m:
        return m.group(1).lower()
    
    # Паттерн: просто 8 hex цифр в имени
    m = re.search(r'(?<![0-9a-fA-F])([0-9a-fA-F]{8})(?![0-9a-fA-F])', name)
    if m:
        return m.group(1).lower()
    
    return None


def extract_prefix_from_name(name):
    """
    Извлекает смысловой префикс из составного имени регистра.
    
    Примеры:
      IMMO_FLAGS_CONF_DAT_fff84540 → IMMO_FLAGS_CONF
      CAN_STATUS_DAT_fff88200      → CAN_STATUS
      DAT_fff88344                 → None (нет префикса)
    """
    if not name:
        return None
    
    # Ищем DAT_/PTR_/s_ и берём всё, что до него
    m = re.match(r'^(.+?)_(?:DAT|PTR|s|c|au|a|u|i|f)_[0-9a-fA-F]{8}$', name)
    if m:
        return m.group(1)
    
    return None


def generate_name_variants(name):
    """
    Генерирует все варианты имени регистра для поиска.
    
    Для составного имени IMMO_FLAGS_CONF_DAT_fff84540 вернёт:
      - IMMO_FLAGS_CONF_DAT_fff84540 (точное)
      - DAT_fff84540 (оригинальное Ghidra-имя)
      - fff84540 (только адрес)
      - IMMO_FLAGS_CONF (только префикс)
    """
    variants = [name]
    
    addr = extract_address_from_name(name)
    if addr:
        # Добавляем варианты с адресом
        variants.append(f"DAT_{addr}")
        variants.append(f"PTR_{addr}")
        variants.append(f"s_{addr}")
        variants.append(addr)
        variants.append(f"0x{addr}")
    
    prefix = extract_prefix_from_name(name)
    if prefix:
        variants.append(prefix)
    
    # Убираем дубликаты, сохраняя порядок
    seen = set()
    unique = []
    for v in variants:
        v_lower = v.lower()
        if v_lower not in seen:
            seen.add(v_lower)
            unique.append(v)
    
    return unique


# ─── Глобальная база данных ───

class FirmwareDB:
    """База данных прошивки: функции, регистры, графы вызовов."""
    def __init__(self):
        self.c_text = ""
        self.funcs_list = []
        self.regs_list = []
        self.funcs_by_name = {}
        self.callees = {}
        self.callers = {}
        self.reg_index = None
        self.loaded = False
        
        self.reg_to_funcs = {}
        self.func_to_regs = {}
    
    def load(self):
        """Загрузить все данные."""
        if self.loaded:
            return
        
        print("📦 Загрузка базы данных прошивки...")
        
        if Path(C_FILE).exists():
            with open(C_FILE, 'r', encoding='utf-8', errors='ignore') as f:
                self.c_text = f.read()
            print(f"   ✅ C-файл: {len(self.c_text)} символов")
        
        funcs_data, _ = load_json(FUNC_JSON)
        if funcs_data:
            self.funcs_list = funcs_data
            self.funcs_by_name = {f['name']: f for f in funcs_data}
            
            for f in self.funcs_list:
                name = f['name']
                self.callees[name] = [c for c in f.get('calls', []) if c in self.funcs_by_name]
                self.callers[name] = [c for c in f.get('calledBy', []) if c in self.funcs_by_name]
            
            print(f"   ✅ Функции: {len(self.funcs_list)}")
            print(f"   ✅ Граф вызовов построен")
        
        regs_data, _ = load_json(REG_JSON)
        if regs_data:
            self.regs_list = regs_data
            self.reg_index = RegisterIndex(regs_data)
            print(f"   ✅ Регистры: {len(self.regs_list)} (индексировано)")
            
            self._build_cross_indexes()
        
        self.loaded = True
    
    def _build_cross_indexes(self):
        """Строит индексы связей регистров и функций."""
        self.reg_to_funcs = {}
        self.func_to_regs = {}
        
        for reg in self.regs_list:
            name = reg.get('name', '')
            refs = reg.get('references', [])
            self.reg_to_funcs[name] = set(refs)
            
            for func_name in refs:
                if func_name not in self.func_to_regs:
                    self.func_to_regs[func_name] = set()
                self.func_to_regs[func_name].add(name)
        
        print(f"   ✅ Перекрёстные индексы: {len(self.reg_to_funcs)} регистров ↔ {len(self.func_to_regs)} функций")
    
    def find_function_name(self, query):
        """Ищет функцию по подстроке, точному имени или адресу."""
        if not query:
            return None
        
        query = query.strip()
        
        if query in self.funcs_by_name:
            return query
        
        addr_match = re.search(r'FUN_([0-9a-fA-F]{8})', query)
        if addr_match:
            addr = addr_match.group(1).lower()
            for name in self.funcs_by_name:
                if name.lower().endswith(f'fun_{addr}'):
                    return name
        
        if query.startswith('0x') or query.startswith('0X'):
            try:
                addr_int = int(query, 16)
                addr_hex = f"{addr_int:08x}"
                for name in self.funcs_by_name:
                    if name.lower().endswith(f'fun_{addr_hex}'):
                        return name
            except ValueError:
                pass
        
        query_lower = query.lower()
        matches = [name for name in self.funcs_by_name if query_lower in name.lower()]
        
        if len(matches) == 1:
            return matches[0]
        elif len(matches) > 1:
            exact = [m for m in matches if query_lower == m.lower()]
            if exact:
                return exact[0]
            return matches[0]
        
        return None
    
    def get_regs_for_function(self, func_name):
        """Возвращает список регистров, используемых в функции."""
        return sorted(self.func_to_regs.get(func_name, set()))
    
    def get_funcs_for_register(self, reg_name):
        """Возвращает список функций, использующих регистр."""
        return sorted(self.reg_to_funcs.get(reg_name, set()))


DB = FirmwareDB()


# ─── BFS-поиск пути ───

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


def find_path(start_name, end_name, max_depth=20):
    """Ищет путь между двумя функциями."""
    start = DB.find_function_name(start_name)
    end = DB.find_function_name(end_name)
    
    if not start:
        return None, f"❌ Функция '{start_name}' не найдена"
    if not end:
        return None, f"❌ Функция '{end_name}' не найдена"
    
    path = find_path_bfs(start, end, DB.callees, max_depth)
    if path:
        return path, None
    
    path_back = find_path_bfs(end, start, DB.callers, max_depth)
    if path_back:
        return list(reversed(path_back)), None
    
    return None, f"❌ Путь между '{start}' и '{end}' не найден (глубина ≤ {max_depth})"


def extract_registers_from_body(body):
    """Извлекает все имена регистров из тела функции."""
    if not DB.reg_index:
        return []
    
    identifiers = set(re.findall(r'\b([A-Za-z_][A-Za-z0-9_]*)\b', body))
    
    found_regs = []
    for ident in identifiers:
        matches = DB.reg_index.find(ident)
        if matches:
            for m in matches:
                if m.get('name') not in found_regs:
                    found_regs.append(m.get('name'))
    
    return found_regs


def extract_path_context(path, include_regs=True):
    """Извлекает все функции из пути в контекст для GigaChat."""
    result = []
    result.append(f"// ═══ Путь вызовов ({len(path)} функций) ═══")
    result.append(f"// Путь: {' → '.join(path[:3])} → ... → {path[-1]}")
    result.append("")
    
    all_regs_on_path = set()
    for name in path:
        regs = DB.get_regs_for_function(name)
        all_regs_on_path.update(regs)
    
    if include_regs and all_regs_on_path:
        result.append(f"// ═══ Регистры, используемые на пути ({len(all_regs_on_path)} шт.) ═══")
        sorted_regs = sorted(
            all_regs_on_path,
            key=lambda r: next((x.get('reference_count', 0) for x in DB.regs_list if x.get('name') == r), 0),
            reverse=True
        )
        for reg_name in sorted_regs[:CONFIG["max_regs_per_function"] * 2]:
            reg_data = next((r for r in DB.regs_list if r.get('name') == reg_name), None)
            if reg_data:
                result.append(format_register_compact(reg_data))
        result.append("")
    
    for i, name in enumerate(path):
        func_data = fetch_function(name, DB.c_text, DB.funcs_list, include_regs=include_regs)
        result.append(f"// ─── [{i+1}/{len(path)}] ───")
        result.append(func_data)
        result.append("")
    
    return "\n".join(result)


def format_register_compact(reg):
    """Компактное форматирование регистра для списка."""
    refs = reg.get('references', [])
    refs_str = ', '.join(refs[:3])
    if len(refs) > 3:
        refs_str += f" +{len(refs)-3}"
    
    return (f"//   {reg.get('name', '?'):40s} @ 0x{reg.get('address', '?'):>10s}  "
            f"[{','.join(reg.get('access', [])):10s}]  "
            f"({reg.get('reference_count', 0)} refs)  "
            f"kind={reg.get('kind', '?')}")


# ─── Парсер естественных запросов ───

def parse_path_request(text):
    """Парсит запросы типа 'покажи путь от X до Y'."""
    patterns = [
        r'(?:покажи|показать|найди|найти|построй|построить)\s+(?:путь|маршрут|цепочку)\s+(?:от|из)\s+([^\s]+(?:\s+[^\s]+)*?)\s+(?:до|к)\s+([^\s]+(?:\s+[^\s]+)*)',
        r'путь\s+(?:от|из)\s+([^\s]+(?:\s+[^\s]+)*?)\s+(?:до|к)\s+([^\s]+(?:\s+[^\s]+)*)',
        r'от\s+([A-Za-z0-9_]+)\s+(?:до|к)\s+([A-Za-z0-9_]+)',
    ]
    
    for pattern in patterns:
        m = re.search(pattern, text, re.IGNORECASE)
        if m:
            start = m.group(1).strip()
            end = m.group(2).strip()
            start = re.sub(r'\s+(функци[ияюи]|функции)\s*$', '', start, flags=re.IGNORECASE)
            end = re.sub(r'\s+(функци[ияюи]|функции)\s*$', '', end, flags=re.IGNORECASE)
            return start, end
    
    return None, None


# ─── Ghidra-нормализация имён функций ───

def ghidra_name_variants(name):
    """Генерирует варианты имени функции для поиска в C-файле."""
    variants = [name]
    
    if name and name[0].isdigit():
        variants.append('_' + name)
        m = re.match(r'^(0+)(.*)', name)
        if m:
            rest = m.group(2)
            if rest and not rest[0].isdigit():
                variants.append('_' + rest)
            else:
                variants.append('_' + (rest.lstrip('0') or '0' + rest))
    
    if name.startswith('_') and len(name) > 1 and name[1].isdigit():
        variants.append(name[1:])
    
    m = re.search(r'FUN_([0-9a-fA-F]{8})$', name)
    if m:
        variants.append(f'__FUN_{m.group(1)}__')
    
    seen = set()
    unique = []
    for v in variants:
        if v not in seen:
            seen.add(v)
            unique.append(v)
    
    return unique


def find_function_in_c(func_name, c_text):
    """Ищет функцию в C-файле, пробуя все варианты имени Ghidra."""
    variants = ghidra_name_variants(func_name)
    
    patterns_template = [
        lambda name: rf'(?m)^[\w\s\*]+\s+{re.escape(name)}\s*\([^)]*\)\s*\{{',
        lambda name: rf'(?m)^{re.escape(name)}\s*\([^)]*\)\s*\{{',
        lambda name: rf'(?m)^[\w\s\*]+\s+{re.escape(name)}\s*\([^)]*\)\s*\n\s*\{{',
        lambda name: rf'(?m)^undefined\s+{re.escape(name)}\s*\([^)]*\)\s*\{{',
        lambda name: rf'(?m)^[\w\s\*]+\s+{re.escape(name)}\s*\(',
        lambda name: rf'(?m){re.escape(name)}\s*\n\s*\([^)]*\)\s*\{{',
        lambda name: rf'(?m){re.escape(name)}\s*\(',
    ]
    
    for name in variants:
        if name.startswith('__FUN_') and name.endswith('__'):
            fun_addr = name[6:-2]
            pattern = re.compile(rf'FUN_{fun_addr}\s*\(')
            m = pattern.search(c_text)
            if m:
                line_start = c_text.rfind('\n', 0, m.start()) + 1
                line_end = c_text.find('\n', m.end())
                if line_end == -1:
                    line_end = len(c_text)
                line = c_text[line_start:line_end]
                real_name_match = re.search(r'([A-Za-z0-9_]+FUN_' + fun_addr + r')', line)
                if real_name_match:
                    real_name = real_name_match.group(1)
                    _, body = find_function_in_c(real_name, c_text)
                    if body:
                        return real_name, body
            continue
        
        for pattern_fn in patterns_template:
            pattern = pattern_fn(name)
            m = re.search(pattern, c_text)
            if m:
                start = m.start()
                brace_pos = c_text.find('{', m.end() - 1)
                if brace_pos == -1 or brace_pos > m.end() + 500:
                    continue
                
                depth = 0
                i = brace_pos
                while i < len(c_text):
                    if c_text[i] == '{':
                        depth += 1
                    elif c_text[i] == '}':
                        depth -= 1
                        if depth == 0:
                            return name, c_text[start:i+1]
                    i += 1
    
    return None, None


# ─── Загрузка данных ───

def load_json(path):
    if not Path(path).exists():
        return None, None
    with open(path, 'r', encoding='utf-8') as f:
        data = json.load(f)
    if isinstance(data, dict):
        for key in ['functions', 'registers', 'data']:
            if key in data and isinstance(data[key], list):
                return data[key], data
        return list(data.values()), data
    return data, {"raw": True}


# ─── Извлечение функций ───

def fetch_function(func_name, c_text, funcs_list, include_regs=True):
    """Извлекает полную информацию о функции с автоподтягиванием регистров."""
    result = []
    
    found_name, body = find_function_in_c(func_name, c_text)
    
    func_meta = None
    if funcs_list:
        for f in funcs_list:
            if f.get('name') == func_name:
                func_meta = f
                break
    
    if func_meta:
        result.append(f"// ═══ Функция: {func_name} @ 0x{func_meta.get('address', '?')} ═══")
        if found_name and found_name != func_name:
            result.append(f"// В C-файле имя: {found_name}")
        result.append(f"// Сигнатура: {func_meta.get('signature', '?')}")
        result.append(f"// Размер: {func_meta.get('size', '?')} байт")
        result.append(f"// Тип возврата: {func_meta.get('returnType', '?')}")
        if func_meta.get('parameters'):
            params = ', '.join(
                f"{p.get('type', '?')} {p.get('name', '?')}"
                for p in func_meta['parameters']
            )
            result.append(f"// Параметры: {params}")
        if func_meta.get('calls'):
            result.append(f"// Вызывает: {', '.join(func_meta['calls'])}")
        if func_meta.get('calledBy'):
            result.append(f"// Вызывается из: {', '.join(func_meta['calledBy'][:10])}")
    else:
        result.append(f"// ═══ Функция: {func_name} ═══")
        result.append(f"// ⚠️ Метаданные отсутствуют в JSON")
        addr_match = re.search(r'FUN_([0-9a-fA-F]{8})$', func_name)
        if addr_match:
            result.append(f"// Адрес (из имени): 0x{addr_match.group(1)}")
    
    if include_regs and DB.reg_index:
        regs = DB.get_regs_for_function(func_name)
        if regs:
            result.append(f"//")
            result.append(f"// 📊 Регистры, используемые в функции ({len(regs)} шт.):")
            for reg_name in regs[:CONFIG["max_regs_per_function"]]:
                reg_data = next((r for r in DB.regs_list if r.get('name') == reg_name), None)
                if reg_data:
                    result.append(format_register_compact(reg_data))
            if len(regs) > CONFIG["max_regs_per_function"]:
                result.append(f"//   ... и ещё {len(regs) - CONFIG['max_regs_per_function']}")
    
    if body:
        result.append("")
        result.append(body)
    else:
        result.append("")
        result.append(f"// ⚠️ Тело функции не найдено в C-файле")
        if func_meta and func_meta.get('calls'):
            result.append(f"// Известные вызовы из JSON: {', '.join(func_meta['calls'])}")
        result.append(f"void {func_name}(void) {{ /* тело не найдено */ }}")
    
    return "\n".join(result)


# ─── Индекс регистров (с поддержкой составных имён) ───

class RegisterIndex:
    """
    Индекс регистров для быстрого поиска.
    Поддерживает составные имена Ghidra: PREFIX_DAT_XXXXXXXX.
    """
    def __init__(self, regs_list):
        self.regs = regs_list or []
        self.by_name = {}
        self.by_addr = {}
        self.by_addr_int = {}
        self.by_lower = {}
        
        # 🔧 НОВОЕ: индекс по адресной части (для составных имён)
        self.by_addr_part = {}  # "fff84540" -> [reg1, reg2, ...]
        # 🔧 НОВОЕ: индекс по префиксу (для поиска по смысловой части)
        self.by_prefix = {}     # "IMMO_FLAGS_CONF" -> [reg1, ...]
        
        for r in self.regs:
            name = r.get('name', '')
            addr = r.get('address', '')
            
            self.by_name[name] = r
            self.by_lower[name.lower()] = r
            
            if addr and addr != "unknown":
                addr_clean = addr.lower().lstrip('0x').lstrip('0') or '0'
                self.by_addr[addr_clean] = r
                try:
                    self.by_addr_int[int(addr, 16)] = r
                except:
                    pass
            
            # 🔧 НОВОЕ: извлекаем адресную часть и префикс из имени
            addr_part = extract_address_from_name(name)
            if addr_part:
                if addr_part not in self.by_addr_part:
                    self.by_addr_part[addr_part] = []
                self.by_addr_part[addr_part].append(r)
            
            prefix = extract_prefix_from_name(name)
            if prefix:
                prefix_lower = prefix.lower()
                if prefix_lower not in self.by_prefix:
                    self.by_prefix[prefix_lower] = []
                self.by_prefix[prefix_lower].append(r)
    
    def find(self, query):
        """
        Ищет регистр по запросу. Поддерживает:
          - точное имя: DAT_fff84540
          - составное имя: IMMO_FLAGS_CONF_DAT_fff84540
          - адрес: 0xfff84540, fff84540
          - префикс: IMMO_FLAGS_CONF
          - подстроку: immo, can, uart
        """
        if not query:
            return []
        
        query_stripped = query.strip()
        query_lower = query_stripped.lower()
        
        # 1. Точное совпадение
        if query_stripped in self.by_name:
            return [self.by_name[query_stripped]]
        
        # 2. Совпадение без регистра
        if query_lower in self.by_lower:
            return [self.by_lower[query_lower]]
        
        # 🔧 3. НОВОЕ: пробуем все варианты имени (для составных имён)
        variants = generate_name_variants(query_stripped)
        for variant in variants[1:]:  # пропускаем первый (уже проверили)
            variant_lower = variant.lower()
            if variant_lower in self.by_lower:
                return [self.by_lower[variant_lower]]
        
        # 4. По адресу (hex)
        addr_query = query_stripped.lower().lstrip('0x')
        if addr_query in self.by_addr:
            return [self.by_addr[addr_query]]
        
        # 🔧 5. НОВОЕ: по адресной части (если запрос содержит 8 hex цифр)
        addr_part = extract_address_from_name(query_stripped)
        if addr_part and addr_part in self.by_addr_part:
            return self.by_addr_part[addr_part]
        
        # 🔧 6. НОВОЕ: по префиксу (если запрос совпадает с префиксом регистра)
        if query_lower in self.by_prefix:
            return self.by_prefix[query_lower]
        
        # 7. По адресу (целое число)
        try:
            addr_int = int(query_stripped, 0)
            if addr_int in self.by_addr_int:
                return [self.by_addr_int[addr_int]]
        except ValueError:
            pass
        
        # 8. Подстрока в имени
        matches = [r for r in self.regs if query_lower in r.get('name', '').lower()]
        if matches:
            return matches[:10]
        
        # 9. По адресу как подстроке
        if addr_query and len(addr_query) >= 4:
            matches = [r for r in self.regs 
                      if addr_query in r.get('address', '').lower()]
            if matches:
                return matches[:10]
        
        return []
    
    def format(self, reg):
        result = []
        result.append(f"// ═══ Регистр: {reg.get('name', '?')} ═══")
        result.append(f"// Адрес: 0x{reg.get('address', '?')}")
        result.append(f"// Размер: {reg.get('size', '?')} байт")
        result.append(f"// Тип: {reg.get('type', '?')}")
        result.append(f"// Вид: {reg.get('kind', '?')}")
        result.append(f"// Доступ: {', '.join(reg.get('access', []))}")
        result.append(f"// Регион: {reg.get('region', '?')}")
        if reg.get('is_array'):
            result.append(f"// Массив: да, размер {reg.get('array_size', '?')}")
        if reg.get('is_pointer'):
            result.append(f"// Указатель: да")
        
        refs = reg.get('references', [])
        if refs:
            result.append(f"// Используется в функциях ({reg.get('reference_count', len(refs))} шт.):")
            for ref in refs[:15]:
                result.append(f"//   - {ref}")
        
        return "\n".join(result)


# ─── Извлечение регистров ───

def fetch_register(query, reg_index, c_text=""):
    if not reg_index:
        return f"// ═══ Регистр: {query} ═══\n// ⚠️ База регистров не загружена"
    
    matches = reg_index.find(query)
    
    if not matches:
        result = [f"// ═══ Регистр: {query} ═══"]
        result.append(f"// ⚠️ Регистр не найден в базе данных")
        # 🔧 НОВОЕ: показываем варианты имени для отладки
        variants = generate_name_variants(query)
        if len(variants) > 1:
            result.append(f"// Варианты имени для поиска: {', '.join(variants[1:])}")
        if c_text:
            pattern = re.compile(rf'\b{re.escape(query)}\b')
            count = len(pattern.findall(c_text))
            if count > 0:
                result.append(f"// Упоминаний в C-файле: {count}")
        return "\n".join(result)
    
    if len(matches) > 1:
        result = [f"// ═══ Найдено {len(matches)} регистров по запросу '{query}' ═══"]
        for r in matches[:5]:
            result.append("")
            result.append(reg_index.format(r))
        if len(matches) > 5:
            result.append(f"\n// ... и ещё {len(matches) - 5}")
        return "\n".join(result)
    
    return reg_index.format(matches[0])


def fetch_register_detailed(query):
    """Подробное описание регистра с анализом использования."""
    if not DB.reg_index:
        return "❌ База регистров не загружена"
    
    matches = DB.reg_index.find(query)
    if not matches:
        # 🔧 НОВОЕ: показываем варианты имени для отладки
        variants = generate_name_variants(query)
        result = [f"❌ Регистр '{query}' не найден"]
        result.append(f"\n🔍 Варианты имени, которые пробовались:")
        for v in variants:
            result.append(f"   • {v}")
        result.append(f"\n💡 Попробуйте:")
        result.append(f"   • /search-reg {query.split('_')[0]}  — поиск по части имени")
        addr = extract_address_from_name(query)
        if addr:
            result.append(f"   • /search-reg {addr}  — поиск по адресу")
        return "\n".join(result)
    
    # Если найдено несколько — показываем все
    if len(matches) > 1:
        result = [f"📋 Найдено {len(matches)} регистров по запросу '{query}':\n"]
        for reg in matches:
            result.append(fetch_register_detailed_single(reg))
            result.append("")
        return "\n".join(result)
    
    return fetch_register_detailed_single(matches[0])


def fetch_register_detailed_single(reg):
    """Подробное описание одного регистра."""
    result = []
    result.append(f"╔═══ Подробное описание регистра: {reg.get('name', '?')} ═══")
    result.append(f"║ Адрес:        0x{reg.get('address', '?')}")
    result.append(f"║ Размер:       {reg.get('size', '?')} байт")
    result.append(f"║ Тип:          {reg.get('type', '?')}")
    result.append(f"║ Вид:          {reg.get('kind', '?')}")
    result.append(f"║ Доступ:       {', '.join(reg.get('access', []))}")
    result.append(f"║ Регион памяти:{reg.get('region', '?')}")
    if reg.get('is_array'):
        result.append(f"║ Массив:       да, размер {reg.get('array_size', '?')}")
    if reg.get('is_pointer'):
        result.append(f"║ Указатель:    да")
    
    # 🔧 НОВОЕ: разбор составного имени
    prefix = extract_prefix_from_name(reg.get('name', ''))
    if prefix:
        result.append(f"║")
        result.append(f"║ Осмысленное имя (префикс): {prefix}")
    
    result.append(f"║")
    
    refs = reg.get('references', [])
    result.append(f"║ Используется в {reg.get('reference_count', len(refs))} функциях:")
    for ref in refs[:20]:
        func = DB.funcs_by_name.get(ref, {})
        addr = func.get('address', '?')
        result.append(f"║   • {ref:45s} @ 0x{addr}")
    if len(refs) > 20:
        result.append(f"║   ... и ещё {len(refs) - 20}")
    
    result.append(f"║")
    result.append(f"║ Анализ использования:")
    
    groups = defaultdict(list)
    for ref in refs:
        parts = ref.split('_')
        if len(parts) > 1 and parts[0] in ('FUN', 'DAT', 'A0', 'A2', 'A7', 'A8', 
                                            'B1', 'B2', 'C1', 'C3', 'CAN', 'M0', 
                                            'M1', 'M2', 'N4', 'N16', 'DIAG', 'MAIN',
                                            'MON', 'BLOCK', 'NORM', 'INIT', 'WATCHDOG',
                                            'UTIL', 'DTC', 'FUEL', 'IMMO'):
            groups[parts[0]].append(ref)
        else:
            groups['other'].append(ref)
    
    for group, funcs in sorted(groups.items(), key=lambda x: -len(x[1])):
        result.append(f"║   [{group:10s}] {len(funcs)} функций")
    
    result.append(f"╚{'═'*60}")
    
    return "\n".join(result)


def fetch_related_functions(func_name, funcs_list):
    result = []
    func_meta = None
    if funcs_list:
        for f in funcs_list:
            if f.get('name') == func_name:
                func_meta = f
                break
    
    if not func_meta:
        return ""
    
    related = set()
    for c in func_meta.get('calls', []):
        related.add(c)
    for c in func_meta.get('calledBy', []):
        related.add(c)
    
    if related:
        result.append(f"// ═══ Связанные функции для {func_name} ═══")
        for r in sorted(related)[:10]:
            rm = next((f for f in funcs_list if f.get('name') == r), None)
            if rm:
                result.append(f"// {r} @ 0x{rm.get('address', '?')} — {rm.get('signature', '?')}")
    
    return "\n".join(result)


# ─── Парсинг маркеров ───

def parse_fetch_markers(text):
    markers = []
    pattern = re.compile(r'<<FETCH:(func|reg|related):([^>\s]+)>>')
    for m in pattern.finditer(text):
        markers.append((m.group(1), m.group(2)))
    return markers


def clean_markers(text):
    return re.sub(r'<<FETCH:(func|reg|related):[^>\s]+>>\s*', '', text).strip()


# ─── Коммуникация с GigaChat ───

def chat_with_gigachat(messages):
    """Отправляет запрос в GigaChat через прокси."""
    payload = {
        "model": CONFIG["model"],
        "messages": messages,
        "temperature": 0.3,
    }
    
    try:
        resp = requests.post(PROXY_URL, json=payload, verify=False, timeout=300)
        resp.raise_for_status()
        data = resp.json()
        return data["choices"][0]["message"]["content"]
    except Exception as e:
        return f"[ОШИБКА: {e}]"


# ─── Системный промпт ───

SYSTEM_PROMPT = """Ты — эксперт по reverse engineering прошивок микроконтроллеров SuperH (SH2/SH2A).
Ты анализируешь декомпилированный код прошивки MH8114F/MH8115F (контроллер ЭБУ автомобиля).

## ВАЖНО: Поиск путей между функциями

Если пользователь просит "покажи путь от функции X до функции Y",
НЕ ПЫТАЙСЯ искать путь сама через маркеры. Скрипт УЖЕ нашёл путь через BFS
и передал тебе готовые тела всех функций на пути в контексте.

## ВАЖНО: Работа с регистрами

В контексте тебе передаются описания регистров. Имена регистров могут быть:
  - Стандартные: `DAT_fff88344`
  - Составные: `IMMO_FLAGS_CONF_DAT_fff84540` (префикс + оригинальное имя)
  - Осмысленные: `CAN_STATUS`, `uart_config`

Для каждого регистра указано:
  - Адрес (0x...)
  - Размер и тип
  - Вид (data, status_register, peripheral_data, counter, buffer и т.д.)
  - Тип доступа (read, write, read_write)
  - Регион памяти (peripheral, peripheral_mirror, flash_rom и т.д.)
  - Список функций, которые его используют

При запросе регистра используй ТО ИМЯ, которое видишь в коде.
Скрипт автоматически найдёт регистр по любому варианту имени.

Если видишь регистры с адресами в диапазоне 0xFFF8xxxx — это периферия МК.
Если видишь регистры в диапазоне 0x0000xxxx — это flash ROM (константы).

## Важно о функциях

Имена функций в C-файле могут отличаться от имён в JSON из-за нормализации Ghidra.
Скрипт автоматически находит все варианты.

## Твои возможности

Если для анализа нужна дополнительная информация, вставь маркеры:

- `<<FETCH:func:ИМЯ_ФУНКЦИИ>>` — запросить тело функции
- `<<FETCH:reg:ИМЯ_ИЛИ_АДРЕС>>` — запросить информацию о регистре
- `<<FETCH:related:ИМЯ_ФУНКЦИИ>>` — запросить связанные функции

## Правила использования маркеров

1. Вставляй маркеры в конце ответа
2. За один раз не более 5 маркеров
3. Запрашивай только то, что реально нужно
4. Если у тебя уже достаточно информации — НЕ используй маркеры

## Формат ответов

- Отвечай на русском языке
- Используй техническую терминологию
- Предлагай осмысленные имена вместо FUN_xxxxxxxx и DAT_xxxxxxxx
- При анализе функции упоминай используемые регистры и их назначение
"""


# ─── Основной цикл ───

def run_analysis(initial_context="", user_question=""):
    """Запускает анализ с автодокачкой контекста."""
    print(f"🤖 Используемая модель: {CONFIG['model']}")
    
    messages = [{"role": "system", "content": SYSTEM_PROMPT}]
    
    if initial_context:
        messages.append({
            "role": "user",
            "content": f"Вот начальный контекст для анализа:\n\n{initial_context}\n\n---\n\n{user_question}"
        })
    else:
        messages.append({"role": "user", "content": user_question})
    
    for round_num in range(CONFIG["max_fetch_rounds"] + 1):
        print(f"\n{'='*70}")
        if round_num == 0:
            print("🤖 Отправляю запрос в GigaChat...")
        else:
            print(f"🔄 Раунд автодокачки {round_num}/{CONFIG['max_fetch_rounds']}...")
        
        response = chat_with_gigachat(messages)
        markers = parse_fetch_markers(response)
        clean_response = clean_markers(response)
        
        if not markers:
            print(f"\n{'='*70}")
            print("📋 ФИНАЛЬНЫЙ ОТВЕТ:")
            print(f"{'='*70}")
            print(clean_response)
            print(f"{'='*70}")
            break
        
        if clean_response.strip():
            print(f"\n💬 GigaChat: {clean_response[:500]}...")
        
        print(f"\n📥 GigaChat запросил {len(markers)} элементов:")
        
        fetched_data = []
        for fetch_type, fetch_name in markers:
            print(f"   🔍 {fetch_type}: {fetch_name}")
            
            if fetch_type == "func":
                data = fetch_function(fetch_name, DB.c_text, DB.funcs_list)
                fetched_data.append(data)
                
            elif fetch_type == "reg":
                data = fetch_register(fetch_name, DB.reg_index, DB.c_text)
                fetched_data.append(data)
                
            elif fetch_type == "related":
                data = fetch_related_functions(fetch_name, DB.funcs_list)
                if data:
                    fetched_data.append(data)
                func_meta = DB.funcs_by_name.get(fetch_name)
                if func_meta:
                    for related_name in func_meta.get('calls', [])[:3]:
                        rdata = fetch_function(related_name, DB.c_text, DB.funcs_list)
                        fetched_data.append(rdata)
        
        fetched_text = "\n\n".join(fetched_data)
        
        total_context = sum(len(m.get("content", "")) for m in messages)
        if total_context + len(fetched_text) > CONFIG["max_context_chars"]:
            print(f"\n⚠️  Лимит контекста достигнут ({total_context}/{CONFIG['max_context_chars']} символов)")
            print(f"\n{'='*70}")
            print("📋 ОТВЕТ (контекст переполнен):")
            print(f"{'='*70}")
            print(clean_response)
            break
        
        messages.append({
            "role": "assistant",
            "content": clean_response + "\n\n[Запрашиваю дополнительные данные...]"
        })
        messages.append({
            "role": "user",
            "content": f"Вот запрошенные тобой данные:\n\n{fetched_text}\n\n"
                       f"Продолжи анализ на основе этих данных. "
                       f"Если нужно ещё что-то — запроси через маркеры."
        })
    else:
        print(f"\n⚠️  Достигнут лимит раундов ({CONFIG['max_fetch_rounds']})")
        print("📋 ПОСЛЕДНИЙ ОТВЕТ:")
        print(clean_response)


def handle_path_command(start, end):
    """Обрабатывает команду поиска пути."""
    print(f"\n🔍 Ищу путь от '{start}' до '{end}'...")
    
    path, error = find_path(start, end)
    
    if error:
        print(error)
        return None
    
    print(f"✅ Путь найден (длина {len(path)}):\n")
    for i, name in enumerate(path):
        func = DB.funcs_by_name.get(name, {})
        addr = func.get('address', '?')
        regs = DB.get_regs_for_function(name)
        print(f"[{i:2d}] 0x{addr}  {name:50s}  ({len(regs)} regs)")
    
    print(f"\n📦 Извлекаю тела функций и регистры...")
    context = extract_path_context(path, include_regs=CONFIG["auto_attach_regs"])
    print(f"✅ Контекст готов ({len(context)} символов)")
    
    return context


def handle_regs_in_command(func_name):
    """Показывает все регистры, используемые в функции."""
    resolved = DB.find_function_name(func_name)
    if not resolved:
        print(f"❌ Функция '{func_name}' не найдена")
        return None
    
    regs = DB.get_regs_for_function(resolved)
    if not regs:
        print(f"ℹ️  Функция {resolved} не использует известных регистров")
        return None
    
    print(f"\n📊 Регистры, используемые в {resolved} ({len(regs)} шт.):\n")
    
    result = [f"// ═══ Регистры функции {resolved} ({len(regs)} шт.) ═══"]
    
    sorted_regs = sorted(
        regs,
        key=lambda r: next((x.get('reference_count', 0) for x in DB.regs_list if x.get('name') == r), 0),
        reverse=True
    )
    
    for reg_name in sorted_regs:
        reg_data = next((r for r in DB.regs_list if r.get('name') == reg_name), None)
        if reg_data:
            line = format_register_compact(reg_data)
            print(line.replace('// ', ''))
            result.append(line)
    
    return "\n".join(result)


def handle_regs_stats():
    """Показывает статистику по базе регистров."""
    if not DB.regs_list:
        print("❌ База регистров не загружена")
        return
    
    print(f"\n📊 Статистика по базе регистров ({len(DB.regs_list)} шт.):\n")
    
    by_region = defaultdict(int)
    for r in DB.regs_list:
        by_region[r.get('region', 'unknown')] += 1
    
    print("🗺️  По регионам памяти:")
    for region, count in sorted(by_region.items(), key=lambda x: -x[1]):
        print(f"   {region:25s}: {count}")
    
    by_kind = defaultdict(int)
    for r in DB.regs_list:
        by_kind[r.get('kind', 'unknown')] += 1
    
    print("\n📦 По видам:")
    for kind, count in sorted(by_kind.items(), key=lambda x: -x[1]):
        print(f"   {kind:25s}: {count}")
    
    by_access = defaultdict(int)
    for r in DB.regs_list:
        acc = tuple(sorted(r.get('access', [])))
        by_access[','.join(acc)] += 1
    
    print("\n🔀 По типу доступа:")
    for acc, count in sorted(by_access.items(), key=lambda x: -x[1]):
        print(f"   {acc:25s}: {count}")
    
    # 🔧 НОВОЕ: статистика по составным именам
    with_prefix = sum(1 for r in DB.regs_list if extract_prefix_from_name(r.get('name', '')))
    print(f"\n🏷️  Составные имена (с префиксом): {with_prefix}")
    
    print("\n🔝 Топ-20 самых используемых регистров:")
    sorted_regs = sorted(DB.regs_list, key=lambda r: r.get('reference_count', 0), reverse=True)
    for r in sorted_regs[:20]:
        print(f"   {r.get('name'):40s} @ 0x{r.get('address', '?'):>10s}  ({r.get('reference_count', 0):3d} refs)  [{r.get('kind', '?')}]")


def handle_regs_by_region():
    """Группирует регистры по регионам памяти."""
    if not DB.regs_list:
        print("❌ База регистров не загружена")
        return
    
    by_region = defaultdict(list)
    for r in DB.regs_list:
        by_region[r.get('region', 'unknown')].append(r)
    
    print(f"\n🗺️  Регистры по регионам памяти:\n")
    
    for region in sorted(by_region.keys()):
        regs = by_region[region]
        print(f"\n📍 {region} ({len(regs)} регистров):")
        sorted_regs = sorted(regs, key=lambda r: r.get('reference_count', 0), reverse=True)
        for r in sorted_regs[:10]:
            print(f"   {r.get('name'):40s} @ 0x{r.get('address', '?'):>10s}  ({r.get('reference_count', 0)} refs)")
        if len(sorted_regs) > 10:
            print(f"   ... и ещё {len(sorted_regs) - 10}")


def process_chat_query(messages, context_parts, user_input):
    """Обрабатывает обычный чат-запрос с автодокачкой."""
    full_context = "\n\n".join(context_parts) if context_parts else ""
    if full_context:
        query = f"Контекст из прошивки:\n\n{full_context}\n\n---\n\nВопрос: {user_input}"
    else:
        query = user_input
    
    messages.append({"role": "user", "content": query})
    
    for round_num in range(CONFIG["max_fetch_rounds"] + 1):
        if round_num == 0:
            print("\n🤖 Анализирую...")
        else:
            print(f"🔄 Автодокачка раунд {round_num}...")
        
        response = chat_with_gigachat(messages)
        markers = parse_fetch_markers(response)
        clean_response = clean_markers(response)
        
        if not markers:
            print(f"\n📋 {clean_response}")
            messages.append({"role": "assistant", "content": clean_response})
            return
        
        print(f"   📥 Запрошено: {', '.join(f'{t}:{n}' for t, n in markers)}")
        
        fetched = []
        for fetch_type, fetch_name in markers:
            if fetch_type == "func":
                fetched.append(fetch_function(fetch_name, DB.c_text, DB.funcs_list))
            elif fetch_type == "reg":
                fetched.append(fetch_register(fetch_name, DB.reg_index, DB.c_text))
            elif fetch_type == "related":
                fetched.append(fetch_related_functions(fetch_name, DB.funcs_list))
        
        fetched_text = "\n\n".join(fetched)
        
        messages.append({"role": "assistant", "content": clean_response})
        messages.append({
            "role": "user",
            "content": f"Запрошенные данные:\n\n{fetched_text}\n\nПродолжи анализ."
        })
    
    print(f"\n📋 {clean_response}")


def interactive_mode():
    """Интерактивный режим работы."""
    DB.load()
    
    print("=" * 70)
    print(f"🔧 УМНЫЙ АНАЛИЗАТОР ПРОШИВКИ MH8114F/MH8115F")
    print(f"🤖 Модель: {CONFIG['model']}")
    print("=" * 70)
    print()
    print("Команды:")
    print("  /path <start> <end>     — найти путь между функциями (BFS)")
    print("  /func <name>            — добавить функцию в контекст")
    print("  /reg <query>            — добавить регистр в контекст")
    print("  /describe-reg <query>   — подробное описание регистра")
    print("  /regs-in <func>         — регистры, используемые в функции")
    print("  /regs-stats             — статистика по базе регистров")
    print("  /regs-by-region         — регистры по регионам памяти")
    print("  /search-func <query>    — поиск функций по подстроке")
    print("  /search-reg <query>     — поиск регистров по подстроке")
    print("  /model [name]           — показать или сменить модель")
    print("  /clear                  — очистить историю")
    print("  /quit                   — выход")
    print()
    print("Или задайте вопрос на естественном языке.")
    print("=" * 70)
    
    context_parts = []
    messages = [{"role": "system", "content": SYSTEM_PROMPT}]
    
    while True:
        try:
            user_input = input("\n🧑 > ").strip()
        except (EOFError, KeyboardInterrupt):
            print("\n👋 До свидания!")
            break
        
        if not user_input:
            continue
        
        if user_input in ("/quit", "/exit"):
            print("👋 До свидания!")
            break
        
        elif user_input == "/clear":
            messages = [{"role": "system", "content": SYSTEM_PROMPT}]
            context_parts = []
            print("🧹 История очищена.")
            continue
        
        elif user_input == "/model" or user_input.startswith("/model "):
            parts = user_input.split(maxsplit=1)
            if len(parts) == 1:
                print(f"🤖 Текущая модель: {CONFIG['model']}")
                print(f"📋 Доступные модели:")
                for m in AVAILABLE_MODELS:
                    marker = " 👈" if m == CONFIG['model'] else ""
                    print(f"   • {m}{marker}")
            else:
                new_model = parts[1]
                if new_model in AVAILABLE_MODELS:
                    CONFIG["model"] = new_model
                    print(f"✅ Модель изменена на: {CONFIG['model']}")
                else:
                    print(f"❌ Модель '{new_model}' не найдена")
            continue
        
        elif user_input.startswith("/path "):
            parts = user_input.split(maxsplit=2)
            if len(parts) < 3:
                print("❌ Использование: /path <start_func> <end_func>")
                continue
            start, end = parts[1], parts[2]
            context = handle_path_command(start, end)
            if context:
                context_parts.append(context)
            continue
        
        elif user_input.startswith("/func "):
            func_name = user_input.split(maxsplit=1)[1].strip()
            data = fetch_function(func_name, DB.c_text, DB.funcs_list)
            context_parts.append(data)
            print(f"✅ Функция {func_name} добавлена в контекст")
            continue
        
        elif user_input.startswith("/reg "):
            query = user_input.split(maxsplit=1)[1].strip()
            data = fetch_register(query, DB.reg_index, DB.c_text)
            context_parts.append(data)
            print(f"✅ Регистр(ы) добавлены в контекст")
            continue
        
        elif user_input.startswith("/describe-reg "):
            query = user_input.split(maxsplit=1)[1].strip()
            print(fetch_register_detailed(query))
            continue
        
        elif user_input.startswith("/regs-in "):
            func_name = user_input.split(maxsplit=1)[1].strip()
            data = handle_regs_in_command(func_name)
            if data:
                context_parts.append(data)
            continue
        
        elif user_input == "/regs-stats":
            handle_regs_stats()
            continue
        
        elif user_input == "/regs-by-region":
            handle_regs_by_region()
            continue
        
        elif user_input.startswith("/search-func "):
            query = user_input.split(maxsplit=1)[1].strip()
            matches = [name for name in DB.funcs_by_name if query.lower() in name.lower()]
            if matches:
                print(f"🔍 Найдено {len(matches)} функций:")
                for name in matches[:20]:
                    func = DB.funcs_by_name[name]
                    regs = DB.get_regs_for_function(name)
                    print(f"   • {name:50s} @ 0x{func.get('address', '?')}  ({len(regs)} regs)")
            else:
                print(f"❌ Функции по запросу '{query}' не найдены")
            continue
        
        elif user_input.startswith("/search-reg "):
            query = user_input.split(maxsplit=1)[1].strip()
            matches = DB.reg_index.find(query)
            if matches:
                print(f"🔍 Найдено {len(matches)} регистров:")
                for r in matches[:20]:
                    refs = r.get('reference_count', 0)
                    print(f"   • {r.get('name'):40s} @ 0x{r.get('address', '?'):>10s}  ({refs} refs)  [{r.get('kind', '?')}]")
            else:
                print(f"❌ Регистры не найдены")
                # 🔧 НОВОЕ: показываем варианты имени
                variants = generate_name_variants(query)
                if len(variants) > 1:
                    print(f"💡 Варианты имени для поиска: {', '.join(variants[1:])}")
            continue
        
        # Парсер естественных запросов "путь от X до Y"
        start, end = parse_path_request(user_input)
        if start and end:
            print(f"\n🔍 Распознан запрос пути: '{start}' → '{end}'")
            context = handle_path_command(start, end)
            if context:
                context_parts.append(context)
                query = "Проанализируй путь вызовов. Для каждой функции объясни назначение, используемые регистры и условия перехода к следующей."
                process_chat_query(messages, context_parts, query)
            continue
        
        # Обычный вопрос
        process_chat_query(messages, context_parts, user_input)


def parse_args():
    """Парсит аргументы командной строки."""
    parser = argparse.ArgumentParser(
        description="Умный анализатор прошивки с автодокачкой контекста",
        formatter_class=argparse.RawDescriptionHelpFormatter,
    )
    parser.add_argument("--model", "-m", default="GigaChat-3-Ultra",
                       choices=AVAILABLE_MODELS,
                       help="Модель GigaChat")
    parser.add_argument("--list-models", action="store_true",
                       help="Показать список моделей")
    parser.add_argument("--path", nargs=2, metavar=("START", "END"),
                       help="Найти путь между функциями")
    parser.add_argument("--regs-stats", action="store_true",
                       help="Показать статистику по регистрам")
    parser.add_argument("--regs-in", metavar="FUNC",
                       help="Показать регистры, используемые в функции")
    parser.add_argument("--describe-reg", metavar="REG",
                       help="Подробное описание регистра")
    parser.add_argument("--no-auto-regs", action="store_true",
                       help="Отключить автоподтягивание регистров")
    parser.add_argument("--max-rounds", type=int, default=CONFIG["max_fetch_rounds"])
    parser.add_argument("--max-context", type=int, default=CONFIG["max_context_chars"])
    parser.add_argument("question", nargs="*", help="Вопрос для анализа")
    return parser.parse_args()


if __name__ == "__main__":
    args = parse_args()
    
    if args.list_models:
        print("📋 Доступные модели GigaChat:")
        for m in AVAILABLE_MODELS:
            print(f"   • {m}")
        sys.exit(0)
    
    CONFIG["model"] = args.model
    CONFIG["max_fetch_rounds"] = args.max_rounds
    CONFIG["max_context_chars"] = args.max_context
    if args.no_auto_regs:
        CONFIG["auto_attach_regs"] = False
    
    DB.load()
    
    if args.regs_stats:
        handle_regs_stats()
        sys.exit(0)
    
    if args.regs_in:
        handle_regs_in_command(args.regs_in)
        sys.exit(0)
    
    if args.describe_reg:
        print(fetch_register_detailed(args.describe_reg))
        sys.exit(0)
    
    if args.path:
        start, end = args.path
        context = handle_path_command(start, end)
        if context:
            if args.question:
                question = " ".join(args.question)
                run_analysis(context, question)
            else:
                run_analysis(context, "Проанализируй этот путь вызовов.")
    elif args.question:
        question = " ".join(args.question)
        run_analysis("", question)
    else:
        interactive_mode()
        