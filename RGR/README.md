# Нагрузочное тестирование веб-сервиса (многопоточная имитация пользователей)

Программа на языке C для нагрузочного тестирования HTTP API. Поддерживает многопоточную имитацию пользователей, авторизацию (регистрация + логин + JWT-токен), сбор метрик времени ответа (среднее, p95, p99), логирование и конфигурацию через KEY=VALUE файл.

## Требования к системе

- Операционная система: Linux (Ubuntu/Debian) или WSL
- Компилятор: GCC с поддержкой C99
- Библиотеки: libcurl, libcjson, libcmocka

## Установка зависимостей

sudo apt update
sudo apt install gcc libcurl4-openssl-dev libcjson-dev libcmocka-dev

## Сборка проекта

mkdir build
cd build
cmake ..
make

После сборки в папке build появятся:
- load_tester - основная программа
- tests - UNIT-тесты
- config - конфигурационный файл (скопируется автоматически)
- endpoints.json - файл с описанием эндпоинтов (скопируется автоматически)

## Конфигурация

### Файл config (KEY=VALUE)

Отредактируйте файл config в папке build:

THREADS=10
TIME=20
ENDPOINTS=5
LOGGING_ENABLED=1

Параметры:
- THREADS - количество потоков (пользователей) от 1 до 10000
- TIME - время работы каждого потока в секундах от 1 до 3600
- ENDPOINTS - количество эндпоинтов в endpoints.json (должно быть не меньше 3)
- LOGGING_ENABLED - 1 (включить логирование) или 0 (выключить)

Если файл config отсутствует, программа использует аргументы командной строки:
./load_tester THREADS TIME ENDPOINTS

Пример: ./load_tester 10 60 5

### Файл endpoints.json (описание эндпоинтов)

Отредактируйте файл endpoints.json в папке build.

ПЕРВЫЕ ДВА ЭНДПОИНТА ОБЯЗАТЕЛЬНО ДОЛЖНЫ НАЗЫВАТЬСЯ register И login

Пример файла:

{
    "parameters": {
        "host": "http://ваш_сервер:порт"
    },
    "network": {
        "connect_timeout_ms": 1000,
        "timeout_ms": 5000
    },
    "requests": {
        "register": {
            "method": "POST",
            "path": "/auth/register/",
            "auth_required": 0,
            "query": {},
            "headers": {
                "Content-Type": "application/json"
            },
            "body": {
                "username": "str",
                "email": "str",
                "password": "str"
            },
            "behavior": {
                "delay_ms": 1000
            }
        },
        "login": {
            "method": "POST",
            "path": "/auth/login/",
            "auth_required": 0,
            "query": {},
            "headers": {
                "Content-Type": "application/json"
            },
            "body": {
                "username_or_email": "str",
                "password": "str"
            },
            "behavior": {
                "delay_ms": 1000
            }
        },
        "send_message": {
            "method": "POST",
            "path": "/messages/send/",
            "auth_required": 1,
            "query": {},
            "headers": {
                "Content-Type": "application/json"
            },
            "body": {
                "recipient_username": "str",
                "text": "str"
            },
            "behavior": {
                "weight": 60,
                "delay_ms": 1000
            }
        }
    }
}

Описание полей:
- parameters.host - базовый URL сервера (без слеша в конце)
- network.connect_timeout_ms - таймаут подключения в миллисекундах
- network.timeout_ms - общий таймаут запроса в миллисекундах
- requests - объект с эндпоинтами
- method - HTTP-метод (GET, POST, PUT, DELETE)
- path - путь к эндпоинту
- auth_required - 1 (нужен токен) или 0 (не нужен)
- query - GET-параметры
- headers - HTTP-заголовки
- body - тело запроса
- behavior.weight - вес эндпоинта (чем больше, тем чаще выбирается)
- behavior.delay_ms - задержка после запроса в миллисекундах

Специальные значения в body и query:
- "str" - заменяется на userX (где X - ID потока)
- "int" - заменяется на ID потока (число)

Это позволяет каждому потоку создавать уникальные данные.

## Запуск

cd build
./load_tester

Пример вывода:

[INFO]: Thread 3, HTTP 200, time 2.10 ms
[INFO]: Thread 5, HTTP 200, time 2.11 ms

========== РЕЗУЛЬТАТЫ НАГРУЗОЧНОГО ТЕСТА ==========
Потоков: 10
Длительность: 20 с
Всего запросов: 180
Успешных: 180 (100.00%)
HTTP ошибок: 0 (0.00%)
Ошибок соединения: 0 (0.00%)

--- Время ответа (успешные запросы) ---
Среднее: 242.97 мс
p95: 2105.01 мс
p99: 2107.56 мс
==================================================

## Запуск UNIT-тестов

cd build
./tests

Ожидаемый вывод:
[==========] Running 6 test(s).
[ RUN      ] test_sred_time_normal
[       OK ] test_sred_time_normal
[==========] 6 test(s) run.
[  PASSED  ] 6 test(s).

## Логирование

При LOGGING_ENABLED=1 программа создаёт файл log.txt в папке build:
- [INFO] - успешные запросы
- [WARNING] - HTTP-ошибки (4xx, 5xx)
- [ERROR] - ошибки соединения (curl)

## Обработка ошибок

Программа проверяет:
- Существование файлов config и endpoints.json
- Корректность JSON-синтаксиса
- Наличие обязательных эндпоинтов register и login
- Деление на ноль при расчёте метрик

При любой ошибке выводится сообщение с пояснением.

## Выполненные требования

- Решение задачи на C - выполнено
- Проверка входных данных - выполнена
- Сборка через CMake - выполнена
- UNIT-тесты (CMocka) - выполнены
- Логирование в файл и консоль - выполнено
- Конфигурация KEY=VALUE - выполнена
- Многопоточность (pthread) - выполнено

## Автор

Студент группы ИКС-531 Кудрявцев А.В.