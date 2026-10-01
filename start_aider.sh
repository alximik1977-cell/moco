#!/bin/bash

export CURL_CA_BUNDLE=""
export REQUESTS_CA_BUNDLE=""
export SSL_CERT_FILE=""
export HTTPX_SSL_VERIFY=false
export NODE_TLS_REJECT_UNAUTHORIZED=0

TOKEN=$(python -c "
from gigachat import GigaChat
giga = GigaChat(
    credentials='MDFhMGQ3MGQtNWQ5MS03ZDYxLTgzODYtN2Y2MDJiZjViYzkzOjNlNGMyNmI2LTZlMDMtNDQ2Ni1hNTI0LTMzYjc0YWJmOTUwOQ==',
    scope='GIGACHAT_API_PERS',
    verify_ssl_certs=False
)
print(giga.get_token().access_token)
")

export OPENAI_API_BASE="https://api.giga.chat/v1/models"
export OPENAI_API_KEY="$TOKEN"

aider \
  --openai-api-base https://api.giga.chat/v1 \
  --model openai/GigaChat-2 \
  --no-verify-ssl \
  --no-show-model
  