#!/bin/bash
export OPENAI_API_KEY="sk-dummy"
  
  aider \
  --openai-api-base http://127.0.0.1:8000/v1 \
  --model openai/GigaChat-3-Ultra \
  --openai-api-key sk-dummy \
  --no-stream \
  --edit-format diff \
  --no-show-model-warnings 
  