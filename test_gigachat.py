from gigachat import GigaChat

giga = GigaChat(
    credentials="MDFhMGQ3MGQtNWQ5MS03ZDYxLTgzODYtN2Y2MDJiZjViYzkzOjNlNGMyNmI2LTZlMDMtNDQ2Ni1hNTI0LTMzYjc0YWJmOTUwOQ==",
    scope="GIGACHAT_API_PERS",
    verify_ssl_certs=False
)

response = giga.chat("Привет, какая ты модель?")
print(response.choices[0].message.content)