#!/usr/bin/env python3
"""
Локальный прокси для GigaChat с автоматическим обновлением токена.
Aider подключается к этому прокси, прокси сам следит за токеном.
"""
import json
import time
import threading
import requests
from flask import Flask, request, Response, jsonify
from gigachat import GigaChat

# ===== НАСТРОЙКИ =====
AUTH_KEY = "MDFhMGQ3MGQtNWQ5MS03ZDYxLTgzODYtN2Y2MDJiZjViYzkzOjNlNGMyNmI2LTZlMDMtNDQ2Ni1hNTI0LTMzYjc0YWJmOTUwOQ=="  # ← замените на ваш ключ из личного кабинета Сбера
SCOPE = "GIGACHAT_API_PERS"  # или GIGACHAT_API_CORP
#GIGACHAT_API = "https://gigachat.devices.sberbank.ru/api/v1"
GIGACHAT_API = "https://api.giga.chat/v1"
LISTEN_PORT = 8000
TOKEN_REFRESH_SECONDS = 25 * 60  # обновлять токен каждые 25 минут (живёт 30)
# =====================

app = Flask(__name__)

# Глобальное состояние
state = {
    "token": None,
    "token_expires": 0,
    "lock": threading.Lock(),
}


def refresh_token():
    """Получить свежий токен GigaChat."""
    print(f"[{time.strftime('%H:%M:%S')}] Обновление токена GigaChat...")
    try:
        giga = GigaChat(
            credentials=AUTH_KEY,
            scope=SCOPE,
            verify_ssl_certs=False,
        )
        token_obj = giga.get_token()
        with state["lock"]:
            state["token"] = token_obj.access_token
            # Токен живёт ~30 минут, обновим через 25
            state["token_expires"] = time.time() + TOKEN_REFRESH_SECONDS
        print(f"[{time.strftime('%H:%M:%S')}] ✅ Токен обновлён (первые 40 символов: {token_obj.access_token[:40]}...)")
        return True
    except Exception as e:
        print(f"[{time.strftime('%H:%M:%S')}] ❌ Ошибка обновления токена: {e}")
        return False


def get_token():
    """Получить валидный токен, при необходимости обновить."""
    with state["lock"]:
        if state["token"] is None or time.time() > state["token_expires"] - 60:
            pass  # нужно обновить
        else:
            return state["token"]
    # Обновляем вне lock, чтобы не блокировать надолго
    refresh_token()
    return state["token"]


def token_refresher_loop():
    """Фоновый поток для регулярного обновления токена."""
    while True:
        time.sleep(TOKEN_REFRESH_SECONDS)
        refresh_token()


def proxy_request_to_gigachat(endpoint, payload, stream=False):
    """Переслать запрос в GigaChat."""
    token = get_token()
    if not token:
        return {"error": "Failed to obtain GigaChat token"}, 500
    
    url = f"{GIGACHAT_API}/{endpoint}"
    headers = {
        "Authorization": f"Bearer {token}",
        "Content-Type": "application/json",
        "Accept": "application/json",
    }
    
    try:
        resp = requests.post(
            url,
            headers=headers,
            json=payload,
            stream=stream,
            verify=False,
            timeout=180,
        )
        
        # Если токен истёк (401) — пробуем обновить и повторить
        if resp.status_code == 401:
            print(f"[{time.strftime('%H:%M:%S')}] ⚠️  Токен отклонён (401), обновляю...")
            refresh_token()
            token = get_token()
            headers["Authorization"] = f"Bearer {token}"
            resp = requests.post(
                url,
                headers=headers,
                json=payload,
                stream=stream,
                verify=False,
                timeout=180,
            )
        
        return resp
    except Exception as e:
        return {"error": str(e)}, 500


@app.route("/v1/chat/completions", methods=["POST"])
def chat_completions():
    """Проксировать /v1/chat/completions в GigaChat."""
    payload = request.get_json()
    
    # Удаляем параметры, которые GigaChat не понимает
    for key in ["frequency_penalty", "presence_penalty", "seed", "logit_bias"]:
        payload.pop(key, None)
    
    stream = payload.get("stream", False)
    
    resp = proxy_request_to_gigachat("chat/completions", payload, stream=stream)
    
    if isinstance(resp, tuple):
        return jsonify(resp[0]), resp[1]
    
    if stream:
        def generate():
            for chunk in resp.iter_lines():
                if chunk:
                    yield chunk + b"\n"
        return Response(generate(), mimetype="text/event-stream")
    else:
        return Response(
            resp.content,
            status=resp.status_code,
            mimetype="application/json",
        )


@app.route("/v1/models", methods=["GET"])
def list_models():
    """Вернуть список моделей."""
    return jsonify({
        "object": "list",
        "data": [
            {"id": "GigaChat-Max", "object": "model", "owned_by": "sber"},
            {"id": "GigaChat-Plus", "object": "model", "owned_by": "sber"},
            {"id": "GigaChat-Pro", "object": "model", "owned_by": "sber"},
            {"id": "GigaChat-2-Max", "object": "model", "owned_by": "sber"},
        ]
    })


@app.route("/health", methods=["GET"])
def health():
    """Проверка работоспособности."""
    return jsonify({
        "status": "ok",
        "token_valid": state["token"] is not None and time.time() < state["token_expires"],
        "token_expires_in": max(0, int(state["token_expires"] - time.time())),
    })


if __name__ == "__main__":
    import urllib3
    urllib3.disable_warnings(urllib3.exceptions.InsecureRequestWarning)
    
    print("🚀 Запуск GigaChat прокси...")
    print(f"📡 Aider подключайте к: http://127.0.0.1:{LISTEN_PORT}/v1")
    
    # Первичное получение токена
    if not refresh_token():
        print("❌ Не удалось получить начальный токен. Проверьте AUTH_KEY.")
        exit(1)
    
    # Фоновый поток обновления
    t = threading.Thread(target=token_refresher_loop, daemon=True)
    t.start()
    
    # Запуск Flask
    app.run(host="127.0.0.1", port=LISTEN_PORT, threaded=True)
