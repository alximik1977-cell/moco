#!/usr/bin/env python3
"""
Умный анализатор прошивки с автоматическим извлечением контекста.
GigaChat сам запрашивает нужные функции и регистры через маркеры.

Особенности:
  - Поддержка имён функций, начинающихся с цифры (Ghidra-нормализация)
  - Поддержка регистров с произвольными именами
  - Автоматическая докачка контекста по запросу модели
"""
import json
import re
import sys
import requests
from pathlib import Path

# ===== НАСТРОЙКИ =====
PROXY_URL = "http://127.0.0.1:8000/v1/chat/completions"
MODEL = "GigaChat-Max"
C_FILE = "MH8114F_MH8115F_ES1212_flash-20260413-143030.bin_decompiled.c"
FUNC_JSON = "MH8114F_MH8115F_ES1212_flash-20260413-143030.bin_functions.json"
REG_JSON = "MH8114F_MH8115F_ES1212_flash-20260413-143030.bin_registers.json"
MAX_FETCH_ROUNDS = 5
MAX_CONTEXT_CHARS = 80000
# =====================


# ─── Ghidra-нормализация имён ───

def ghidra_name_variants(name):
    """
    Генерирует все варианты имени функции, которые Ghidra может использовать в C-файле.
    
    Ghidra нормализует имена, начинающиеся с цифры:
      - Добавляет '_' в начало
      - Убирает ведущие нули из первой группы цифр
    
    Примеры:
      0000_0_TBR_1_FUN_00005da0 → _000_0_TBR_1_FUN_00005da0
      0000_TBR_DEADLOOP_FUN_00001038 → _TBR_DEADLOOP_FUN_00001038
      0000_INIT_VARS_FUN_0000106e → _INIT_VARS_FUN_0000106e
    """
    variants = [name]
    
    # Вариант 1: просто '_' в начале
    if name and name[0].isdigit():
        variants.append('_' + name)
    
    # Вариант 2: '_' в начале + удаление ведущих нулей из первой группы цифр
    if name and name[0].isdigit():
        # Находим первую группу цифр и убираем ведущие нули
        m = re.match(r'^(0+)(.*)', name)
        if m:
            zeros = m.group(1)
            rest = m.group(2)
            # Убираем все нули, если после них не цифра
            if rest and not rest[0].isdigit():
                variants.append('_' + rest)
            else:
                # Убираем все нули кроме одного
                variants.append('_' + rest.lstrip('0') or '0' + rest)
    
    # Вариант 3: если имя уже начинается с '_' и дальше цифры — пробуем без '_'
    if name.startswith('_') and len(name) > 1 and name[1].isdigit():
        variants.append(name[1:])
    
    # Вариант 4: поиск по суффиксу FUN_XXXXXXXX (самый надёжный fallback)
    m = re.search(r'FUN_([0-9a-fA-F]{8})$', name)
    if m:
        variants.append(f'__FUN_{m.group(1)}__')  # маркер для особого поиска
    
    # Убираем дубликаты
    seen = set()
    unique = []
    for v in variants:
        if v not in seen:
            seen.add(v)
            unique.append(v)
    
    return unique


def find_function_in_c(func_name, c_text):
    """
    Ищет функцию в C-файле, пробуя все варианты имени Ghidra.
    Возвращает (найденное_имя, тело_функции) или (None, None).
    """
    variants = ghidra_name_variants(func_name)
    
    patterns_template = [
        # тип имя(параметры) {
        lambda name: rf'(?m)^[\w\s\*]+\s+{re.escape(name)}\s*\([^)]*\)\s*\{{',
        # имя(параметры) {
        lambda name: rf'(?m)^{re.escape(name)}\s*\([^)]*\)\s*\{{',
        # с переносом строки перед {
        lambda name: rf'(?m)^[\w\s\*]+\s+{re.escape(name)}\s*\([^)]*\)\s*\n\s*\{{',
        # undefined имя(void)
        lambda name: rf'(?m)^undefined\s+{re.escape(name)}\s*\([^)]*\)\s*\{{',
        # любой тип + имя + (
        lambda name: rf'(?m)^[\w\s\*]+\s+{re.escape(name)}\s*\(',
        # имя + перенос + (
        lambda name: rf'(?m){re.escape(name)}\s*\n\s*\([^)]*\)\s*\{{',
        # просто имя + (
        lambda name: rf'(?m){re.escape(name)}\s*\(',
    ]
    
    # Перебираем все варианты имён × все паттерны
    for name in variants:
        # Особый случай: поиск по FUN_XXXXXXXX
        if name.startswith('__FUN_') and name.endswith('__'):
            fun_addr = name[6:-2]
            pattern = re.compile(rf'FUN_{fun_addr}\s*\(')
            m = pattern.search(c_text)
            if m:
                # Нашли упоминание, теперь ищем определение этой функции
                # Ищем строку с определением: тип FUN_XXXX(...) {
                line_start = c_text.rfind('\n', 0, m.start()) + 1
                line_end = c_text.find('\n', m.end())
                if line_end == -1:
                    line_end = len(c_text)
                line = c_text[line_start:line_end]
                # Извлекаем реальное имя из этой строки
                real_name_match = re.search(r'([A-Za-z0-9_]+FUN_' + fun_addr + r')', line)
                if real_name_match:
                    real_name = real_name_match.group(1)
                    # Рекурсивно ищем тело по реальному имени
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
    """Загружает JSON с автоопределением структуры."""
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


def load_c_file():
    if not Path(C_FILE).exists():
        return ""
    with open(C_FILE, 'r', encoding='utf-8', errors='ignore') as f:
        return f.read()


# ─── Извлечение функций ───

def fetch_function(func_name, c_text, funcs_list):
    """Извлекает полную информацию о функции."""
    result = []
    
    # Ищем функцию с учётом Ghidra-нормализации
    found_name, body = find_function_in_c(func_name, c_text)
    
    # Метаданные из JSON
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
        result.append(f"// Соглашение о вызове: {func_meta.get('callingConvention', '?')}")
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


# ─── Индекс регистров ───

class RegisterIndex:
    """Индекс регистров для быстрого поиска по имени, адресу, подстроке."""
    def __init__(self, regs_list):
        self.regs = regs_list or []
        self.by_name = {}
        self.by_addr = {}
        self.by_addr_int = {}
        self.by_lower = {}
        
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
    
    def find(self, query):
        """Ищет регистр по запросу."""
        if not query:
            return []
        
        query_stripped = query.strip()
        query_lower = query_stripped.lower()
        
        # 1. Точное совпадение
        if query_stripped in self.by_name:
            return [self.by_name[query_stripped]]
        
        # 2. Без регистра
        if query_lower in self.by_lower:
            return [self.by_lower[query_lower]]
        
        # 3. По адресу (hex)
        addr_query = query_stripped.lower().lstrip('0x')
        if addr_query in self.by_addr:
            return [self.by_addr[addr_query]]
        
        # 4. По адресу (целое число)
        try:
            addr_int = int(query_stripped, 0)
            if addr_int in self.by_addr_int:
                return [self.by_addr_int[addr_int]]
        except ValueError:
            pass
        
        # 5. Подстрока в имени
        matches = [r for r in self.regs if query_lower in r.get('name', '').lower()]
        if matches:
            return matches[:10]
        
        # 6. По адресу как подстроке
        if addr_query and len(addr_query) >= 4:
            matches = [r for r in self.regs 
                      if addr_query in r.get('address', '').lower()]
            if matches:
                return matches[:10]
        
        return []
    
    def format(self, reg):
        """Форматирует регистр в читаемый вид."""
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
    """Извлекает информацию о регистре по произвольному запросу."""
    if not reg_index:
        return f"// ═══ Регистр: {query} ═══\n// ⚠️ База регистров не загружена"
    
    matches = reg_index.find(query)
    
    if not matches:
        result = [f"// ═══ Регистр: {query} ═══"]
        result.append(f"// ⚠️ Регистр не найден в базе данных")
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


def fetch_related_functions(func_name, funcs_list):
    """Извлекает связанные функции."""
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
    """Ищет маркеры в ответе GigaChat."""
    markers = []
    pattern = re.compile(r'<<FETCH:(func|reg|related):([^>\s]+)>>')
    for m in pattern.finditer(text):
        markers.append((m.group(1), m.group(2)))
    return markers


def clean_markers(text):
    """Удаляет маркеры из текста ответа."""
    return re.sub(r'<<FETCH:(func|reg|related):[^>\s]+>>\s*', '', text).strip()


# ─── Коммуникация с GigaChat ───

def chat_with_gigachat(messages):
    """Отправляет запрос в GigaChat через прокси."""
    payload = {
        "model": MODEL,
        "messages": messages,
        "temperature": 0.3,
    }
    
    try:
        resp = requests.post(PROXY_URL, json=payload, verify=False, timeout=180)
        resp.raise_for_status()
        data = resp.json()
        return data["choices"][0]["message"]["content"]
    except Exception as e:
        return f"[ОШИБКА: {e}]"


# ─── Системный промпт ───

SYSTEM_PROMPT = """Ты — эксперт по reverse engineering прошивок микроконтроллеров SuperH (SH2/SH2A).
Ты анализируешь декомпилированный код прошивки MH8114F/MH8115F (контроллер ЭБУ автомобиля).

## Важно о функциях

Некоторые функции могут отсутствовать в JSON-файле с метаданными, но присутствовать 
в декомпилированном C-файле. Скрипт автоматически извлекает их тело из C-файла.

Имена функций в C-файле могут отличаться от имён в JSON из-за нормализации Ghidra:
  - Если имя начинается с цифры, Ghidra добавляет '_' в начало и убирает ведущие нули
  - Пример: `0000_0_TBR_1_FUN_00005da0` в C-файле → `_000_0_TBR_1_FUN_00005da0`
  - Скрипт автоматически находит все варианты — просто используй имя из JSON

## Важно о регистрах данных

Регистры данных могут иметь разные имена:
  - Стандартные: `DAT_fff88344`, `DAT_08001234`
  - Осмысленные: `CAN_STATUS`, `uart_config`, `g_engine_state`
  - По адресам: `0xfff88344`, `fff88344`

При запросе регистра используй ТО ИМЯ, которое видишь в коде.

## Твои возможности

Если для анализа нужна дополнительная информация, вставь маркеры в свой ответ:

- `<<FETCH:func:ИМЯ_ФУНКЦИИ>>` — запросить тело функции и её метаданные
- `<<FETCH:reg:ИМЯ_ИЛИ_АДРЕС>>` — запросить информацию о регистре/переменной
- `<<FETCH:related:ИМЯ_ФУНКЦИИ>>` — запросить список связанных функций

## Правила использования маркеров

1. Вставляй маркеры в конце ответа, после основного текста
2. За один раз запрашивай не более 5 маркеров
3. Запрашивай только то, что реально нужно для ответа
4. Если у тебя уже достаточно информации — НЕ используй маркеры
5. Для регистров используй ТОЧНОЕ имя из кода

## Примеры

Пользователь: "Проанализируй функцию BLOCK_IM_FUN_000151da"

Твой ответ:
"Для анализа мне нужно увидеть тело этой функции.
<<FETCH:func:BLOCK_IM_FUN_000151da>>
<<FETCH:related:BLOCK_IM_FUN_000151da>>"

Пользователь: "Что за регистр DAT_fff88344?"

Твой ответ:
"Запрошу информацию о регистре.
<<FETCH:reg:DAT_fff88344>>"

## Формат ответов

- Отвечай на русском языке
- Используй техническую терминологию
- Для каждой функции указывай: назначение, используемые регистры, условия переходов
- Предлагай осмысленные имена вместо FUN_xxxxxxxx и DAT_xxxxxxxx
- Если видишь паттерны (инициализация, обработчик прерывания, конечный автомат) — указывай их
"""


# ─── Основной цикл ───

def run_analysis(initial_context="", user_question=""):
    """Запускает анализ с автодокачкой контекста."""
    print("📦 Загрузка базы данных прошивки...")
    c_text = load_c_file()
    funcs_list, funcs_data = load_json(FUNC_JSON) if Path(FUNC_JSON).exists() else (None, None)
    regs_list, regs_data = load_json(REG_JSON) if Path(REG_JSON).exists() else (None, None)
    
    reg_index = RegisterIndex(regs_list)
    
    if funcs_list:
        print(f"   ✅ Функции: {len(funcs_list)}")
    if regs_list:
        print(f"   ✅ Регистры: {len(regs_list)} (индексировано)")
    if c_text:
        print(f"   ✅ C-файл: {len(c_text)} символов")
    
    messages = [{"role": "system", "content": SYSTEM_PROMPT}]
    
    if initial_context:
        messages.append({
            "role": "user",
            "content": f"Вот начальный контекст для анализа:\n\n{initial_context}\n\n---\n\n{user_question}"
        })
    else:
        messages.append({"role": "user", "content": user_question})
    
    for round_num in range(MAX_FETCH_ROUNDS + 1):
        print(f"\n{'='*70}")
        if round_num == 0:
            print("🤖 Отправляю запрос в GigaChat...")
        else:
            print(f"🔄 Раунд автодокачки {round_num}/{MAX_FETCH_ROUNDS}...")
        
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
                data = fetch_function(fetch_name, c_text, funcs_list)
                fetched_data.append(data)
                
            elif fetch_type == "reg":
                data = fetch_register(fetch_name, reg_index, c_text)
                fetched_data.append(data)
                
            elif fetch_type == "related":
                data = fetch_related_functions(fetch_name, funcs_list)
                if data:
                    fetched_data.append(data)
                func_meta = next((f for f in (funcs_list or []) if f.get('name') == fetch_name), None)
                if func_meta:
                    for related_name in func_meta.get('calls', [])[:3]:
                        rdata = fetch_function(related_name, c_text, funcs_list)
                        fetched_data.append(rdata)
        
        fetched_text = "\n\n".join(fetched_data)
        
        total_context = sum(len(m.get("content", "")) for m in messages)
        if total_context + len(fetched_text) > MAX_CONTEXT_CHARS:
            print(f"\n⚠️  Лимит контекста достигнут ({total_context}/{MAX_CONTEXT_CHARS} символов)")
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
        print(f"\n⚠️  Достигнут лимит раундов ({MAX_FETCH_ROUNDS})")
        print("📋 ПОСЛЕДНИЙ ОТВЕТ:")
        print(clean_response)


def interactive_mode():
    """Интерактивный режим работы."""
    print("=" * 70)
    print("🔧 УМНЫЙ АНАЛИЗАТОР ПРОШИВКИ MH8114F/MH8115F")
    print("=" * 70)
    print()
    print("Команды:")
    print("  /path <start> <end>  — загрузить путь вызовов")
    print("  /func <name>         — добавить функцию в контекст")
    print("  /reg <query>         — добавить регистр (имя, адрес, подстрока)")
    print("  /search-reg <query>  — поиск регистров по подстроке")
    print("  /clear               — очистить историю")
    print("  /quit                — выход")
    print()
    print("Или просто задайте вопрос о прошивке.")
    print("GigaChat сам запросит нужные функции и регистры.")
    print("=" * 70)
    
    c_text = load_c_file()
    funcs_list, _ = load_json(FUNC_JSON) if Path(FUNC_JSON).exists() else (None, None)
    regs_list, _ = load_json(REG_JSON) if Path(REG_JSON).exists() else (None, None)
    reg_index = RegisterIndex(regs_list)
    
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
        
        elif user_input.startswith("/path "):
            parts = user_input.split()
            if len(parts) < 3:
                print("❌ Использование: /path <start_func> <end_func>")
                continue
            start, end = parts[1], parts[2]
            import subprocess
            result = subprocess.run(
                ["python", "extract_path_v3.py", "--path", start, end],
                capture_output=True, text=True
            )
            print(result.stdout)
            if result.returncode == 0:
                subprocess.run(
                    ["python", "extract_path_v3.py", "--extract", "auto_context.c"],
                    capture_output=True, text=True
                )
                if Path("auto_context.c").exists():
                    ctx = Path("auto_context.c").read_text(encoding='utf-8')
                    context_parts.append(ctx)
                    print(f"✅ Путь загружен ({len(ctx)} символов)")
            continue
        
        elif user_input.startswith("/func "):
            func_name = user_input.split(maxsplit=1)[1].strip()
            data = fetch_function(func_name, c_text, funcs_list)
            context_parts.append(data)
            print(f"✅ Функция {func_name} добавлена в контекст")
            continue
        
        elif user_input.startswith("/reg "):
            query = user_input.split(maxsplit=1)[1].strip()
            data = fetch_register(query, reg_index, c_text)
            context_parts.append(data)
            print(f"✅ Регистр(ы) по запросу '{query}' добавлены в контекст")
            continue
        
        elif user_input.startswith("/search-reg "):
            query = user_input.split(maxsplit=1)[1].strip()
            matches = reg_index.find(query)
            if matches:
                print(f"🔍 Найдено {len(matches)} регистров:")
                for r in matches[:20]:
                    refs = r.get('reference_count', 0)
                    print(f"   • {r.get('name'):30s} @ 0x{r.get('address', '?'):10s}  ({refs} refs)")
            else:
                print(f"❌ Регистры по запросу '{query}' не найдены")
            continue
        
        # Формируем запрос
        full_context = "\n\n".join(context_parts) if context_parts else ""
        if full_context:
            query = f"Контекст из прошивки:\n\n{full_context}\n\n---\n\nВопрос: {user_input}"
        else:
            query = user_input
        
        messages.append({"role": "user", "content": query})
        
        # Цикл с автодокачкой
        for round_num in range(MAX_FETCH_ROUNDS + 1):
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
                break
            
            print(f"   📥 Запрошено: {', '.join(f'{t}:{n}' for t, n in markers)}")
            
            fetched = []
            for fetch_type, fetch_name in markers:
                if fetch_type == "func":
                    fetched.append(fetch_function(fetch_name, c_text, funcs_list))
                elif fetch_type == "reg":
                    fetched.append(fetch_register(fetch_name, reg_index, c_text))
                elif fetch_type == "related":
                    fetched.append(fetch_related_functions(fetch_name, funcs_list))
            
            fetched_text = "\n\n".join(fetched)
            
            messages.append({"role": "assistant", "content": clean_response})
            messages.append({
                "role": "user",
                "content": f"Запрошенные данные:\n\n{fetched_text}\n\nПродолжи анализ."
            })
        else:
            print(f"\n📋 {clean_response}")


if __name__ == "__main__":
    if len(sys.argv) > 1:
        question = " ".join(sys.argv[1:])
        
        initial_context = ""
        if Path("context_path.c").exists():
            initial_context = Path("context_path.c").read_text(encoding='utf-8')
            print(f"📦 Загружен контекст из context_path.c ({len(initial_context)} символов)")
        
        run_analysis(initial_context, question)
    else:
        interactive_mode()
        