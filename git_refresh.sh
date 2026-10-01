#!/bin/bash
# Удалить из git-индекса все отсутствующие файлы
git rm --cached auto_context.c call_path.txt context_path.c context_expanded.c test_gigachat.py 2>/dev/null

# Закоммитить изменения (опционально)
git commit -m "Удалить временные файлы анализа из git"