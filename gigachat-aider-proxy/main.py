import os
from fastapi import FastAPI, HTTPException
from pydantic import BaseModel
from typing import List, Optional
import httpx

# --- Проверка наличия API-ключа ПЕРЕД запуском приложения ---
# Этот блок выполняется один раз при старте сервера.
GIGACHAT_API_KEY = os.getenv("GIGACHAT_API_KEY")
if not GIGACHAT_API_KEY:
    # Здесь отступов перед raise быть НЕ ДОЛЖНО.
    raise SystemExit("Ошибка: Переменная окружения GIGACHAT_API_KEY не установлена!")

app = FastAPI()

# --- Конфигурация ---
GIGACHAT_API_URL = "https://gigachat.api.sbercloud.ru/v1/chat/completions"

# Модели GigaChat, которые вы хотите разрешить использовать
ALLOWED_MODELS = ["GigaChat", "GigaChat-Evo-Light", "GigaChat-2", "GigaChat-Max", "GigaChat-3-Ultra"] 

# --- Модели данных Pydantic для валидации запроса от aider ---
class Message(BaseModel):
    role: str
    content: str

class ChatCompletionRequest(BaseModel):
    model: str
    messages: List[Message]
    temperature: Optional[float] = None
    max_tokens: Optional[int] = None

# --- Эндпоинт, имитирующий OpenAI API ---
@app.post("/v1/chat/completions")
async def chat_completion(request: ChatCompletionRequest):
    if request.model not in ALLOWED_MODELS:
        raise HTTPException(status_code=400, detail=f"Модель '{request.model}' не разрешена.")

    # Преобразуем формат сообщений для GigaChat API
    gigachat_payload = {
        "model": request.model,
        "messages": [{"role": msg.role, "text": msg.content} for msg in request.messages],
        "temperature": request.temperature or 0.7,
    }
    
    headers = {
        "Authorization": f"Bearer {GIGACHAT_API_KEY}",
        "Content-Type": "application/json",
        "Accept": "application/json"
    }

    async with httpx.AsyncClient() as client:
        try:
            response = await client.post(GIGACHAT_API_URL, json=gigachat_payload, headers=headers, timeout=60.0)
            response.raise_for_status()
            data = response.json()
            
            # Преобразуем ответ обратно в формат OpenAI API
            return {
                "id": data.get("id"),
                "object": "chat.completion",
                "created": data.get("created"),
                "model": request.model,
                "choices": [
                    {
                        "index": 0,
                        "message": {"role": "assistant", "content": data["choices"][0]["message"]["text"]},
                        "finish_reason": data["choices"][0].get("finish_reason"),
                    }
                ],
                "usage": data.get("usage"),
            }
        
        except httpx.HTTPStatusError as e:
            # Более детальная обработка ошибок от GigaChat API
            raise HTTPException(
                status_code=e.response.status_code,
                detail=f"Ошибка GigaChat API ({e.response.status_code}): {e.response.text}"
            )
        except Exception as e:
            raise HTTPException(status_code=500, detail=f"Внутренняя ошибка шлюза: {str(e)}")