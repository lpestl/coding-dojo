# Асинхронность и многопоточность

## 1. Threading vs multiprocessing vs asyncio: когда что?

Три разных способа выполнять несколько задач «одновременно»:

| Подход | Модуль | Для чего | GIL | Параллелизм |
|---|---|---|---|---|
| Threading | `threading` | I/O-bound | ограничивает | нет (в CPython) |
| Multiprocessing | `multiprocessing` | CPU-bound | нет (отдельные процессы) | да |
| Asyncio | `asyncio` | I/O-bound, много соединений | не мешает (однопоточно) | нет |

```python
# Threading — ожидание сети/диска
import threading

def download(url):
    ...

t = threading.Thread(target=download, args=(url,))
t.start()
t.join()
```

```python
# Multiprocessing — тяжёлые вычисления
import multiprocessing

def compute(x):
    return x * x

with multiprocessing.Pool() as pool:
    results = pool.map(compute, range(1000))
```

```python
# Asyncio — много одновременных I/O-операций
import asyncio

async def fetch(url):
    ...

async def main():
    await asyncio.gather(fetch(u1), fetch(u2), fetch(u3))
```

Выбор:

- **CPU-bound** → `multiprocessing`.
- **I/O-bound, много коротких ожиданий** → `asyncio`.
- **I/O-bound, с блокирующими библиотеками** → `threading`.

## 2. Как GIL влияет на многопоточность?

GIL разрешает исполнять только один поток Python одновременно, поэтому потоки **не дают
прироста** для CPU-bound задач — наоборот, переключение контекста замедляет.

```python
import threading

counter = 0

def increment():
    global counter
    for _ in range(1_000_000):
        counter += 1

threads = [threading.Thread(target=increment) for _ in range(4)]
for t in threads: t.start()
for t in threads: t.join()

print(counter)   # ❌ не 4_000_000, и результат непредсказуем
```

Здесь две проблемы:

1. **GIL** — потоки не исполняются параллельно для чистого Python-кода.
2. **Race condition** — `counter += 1` не атомарна, поэтому результат меньше ожидаемого.

Для I/O-bound поток полезен: пока один поток ждёт сеть/диск, GIL освобождается и другой
поток продолжает работать.

## 3. Что такое race condition и deadlock?

**Race condition** — результат зависит от порядка выполнения потоков; возникает при
несинхронизированном доступе к разделяемым данным.

```python
# небезопасно
counter += 1   # три операции: чтение, +1, запись
```

Решение — синхронизация через `Lock`:

```python
lock = threading.Lock()

with lock:
    counter += 1
```

**Deadlock** — взаимная блокировка: поток A ждёт ресурс, занятый потоком B, а B ждёт
ресурс, занятый A.

```python
lock_a = threading.Lock()
lock_b = threading.Lock()

def t1():
    with lock_a:
        with lock_b:   # ждёт b
            ...

def t2():
    with lock_b:
        with lock_a:   # ждёт a
            ...
# t1 держит a и ждёт b, t2 держит b и ждёт a → deadlock
```

Профилактика: захватывать блокировки всегда в одном порядке, не держать их дольше
нужного, использовать `RLock` там, где нужен повторный захват тем же потоком.

## 4. Как работает asyncio и event loop?

`asyncio` — **однопоточная** модель конкурентности на базе **event loop** (цикла
событий). Корутины кооперативно «уступают» управление в точках `await`, а event loop
переключается между ними, пока они ждут I/O.

```python
import asyncio

async def task(name, delay):
    await asyncio.sleep(delay)
    print(name)

async def main():
    await asyncio.gather(
        task("A", 2),
        task("B", 1),
    )

asyncio.run(main())
```

Ключевые понятия:

- **Корутина** — `async def`; при вызове не выполняется, а возвращает объект.
- **Task** — обёртка, планируемая в event loop: `asyncio.create_task(...)`.
- **`await`** — точка, где корутина может приостановиться и отдать управление.
- **`gather`** — запуск нескольких корутин и ожидание всех.

```python
async def main():
    t = asyncio.create_task(task("A", 2))   # планируем
    await t                                  # ждём
```

Важно: блокирующий вызов внутри корутины блокирует весь event loop. Для CPU-работы
используют `await asyncio.to_thread(...)` или `loop.run_in_executor(...)`.

## 5. Чем asyncio.Lock отличается от threading.Lock?

Оба защищают разделяемый ресурс, но работают в разных моделях конкурентности.

| | `threading.Lock` | `asyncio.Lock` |
|---|---|---|
| Модель | потоки (многопоточность) | корутины (однопоточность) |
| Ожидание | блокирует поток | `await` — не блокирует event loop |
| Reentrant | нет | нет |
| Использование | `with lock:` | `async with lock:` |

`asyncio.Lock` — **не реентерабельный** (как и `threading.Lock`). Если одна корутина
захватила блокировку и попытается захватить её повторно (напрямую или через вызываемый
`await`), она будет ждать сама себя вечно — **deadlock**:

```python
lock = asyncio.Lock()

async def outer():
    async with lock:
        await inner()          # inner снова пытается взять lock → зависание

async def inner():
    async with lock:
        ...
```

Для повторного захвата тем же контекстом есть `asyncio`-примитивы, но обычно правильное
решение — не захватывать блокировку дважды: выносить защищаемую секцию в отдельную
функцию, которую вызывают **без** повторного `async with lock`.

## 6. Что такое concurrent.futures?

`concurrent.futures` — высокоуровневый интерфейс для запуска задач в пуле потоков или
процессов: `ThreadPoolExecutor` и `ProcessPoolExecutor`.

```python
from concurrent.futures import ThreadPoolExecutor

def work(x):
    return x * x

with ThreadPoolExecutor(max_workers=4) as executor:
    futures = [executor.submit(work, i) for i in range(10)]
    results = [f.result() for f in futures]
```

Или через `map`:

```python
with ThreadPoolExecutor() as executor:
    results = list(executor.map(work, range(10)))
```

Выбор:

- `ThreadPoolExecutor` — I/O-bound, потоки.
- `ProcessPoolExecutor` — CPU-bound, процессы (обходят GIL).

`concurrent.futures` можно использовать и вместе с `asyncio`:

```python
import asyncio

async def main():
    loop = asyncio.get_running_loop()
    with ThreadPoolExecutor() as pool:
        result = await loop.run_in_executor(pool, blocking_func, arg)
```
