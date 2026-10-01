#!/usr/bin/env python3
"""
Универсальный загрузчик JSON — находит список функций в любой структуре.
"""
import json
from pathlib import Path

def load_functions(json_file):
    """Загружает JSON и находит список функций в любой структуре."""
    with open(json_file, 'r', encoding='utf-8') as f:
        data = json.load(f)
    
    # Если это уже список — проверяем первый элемент
    if isinstance(data, list):
        if data and isinstance(data[0], dict) and 'name' in data[0]:
            print(f"✅ Структура: список из {len(data)} функций")
            return data
        else:
            print(f"❌ Список, но элементы не похожи на функции")
            return None
    
    # Если это словарь — ищем список функций
    if isinstance(data, dict):
        # Ищем ключ "functions" или похожий
        for key in ['functions', 'data', 'items', 'results']:
            if key in data and isinstance(data[key], list):
                print(f"✅ Структура: dict['{key}'] — список из {len(data[key])} элементов")
                return data[key]
        
        # Ищем любой список словарей с 'name'
        for key, value in data.items():
            if isinstance(value, list) and value and isinstance(value[0], dict) and 'name' in value[0]:
                print(f"✅ Структура: dict['{key}'] — список функций ({len(value)} шт.)")
                return value
        
        # Может быть, словарь адрес → функция
        first_val = next(iter(data.values()), None)
        if isinstance(first_val, dict) and 'name' in first_val:
            print(f"✅ Структура: dict адрес → функция ({len(data)} шт.)")
            return list(data.values())
        
        print(f"❌ Не удалось найти список функций")
        print(f"   Ключи верхнего уровня: {list(data.keys())}")
        return None
    
    print(f"❌ Неожиданный тип: {type(data)}")
    return None

if __name__ == "__main__":
    funcs = load_functions('MH8114F_MH8115F_ES1212_flash-20260413-143030.bin_functions.json')
    if funcs:
        print(f"\nПример первой функции:")
        print(json.dumps(funcs[0], indent=2, ensure_ascii=False))
        