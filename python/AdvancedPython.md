# Продвинутый Python

## 1. Что такое декораторы и как они работают?

Декоратор — функция, которая принимает другую функцию и возвращает новую (обёртку),
добавляя поведение без изменения исходного кода.

```python
def my_decorator(func):
    def wrapper(*args, **kwargs):
        print("до вызова")
        result = func(*args, **kwargs)
        print("после вызова")
        return result
    return wrapper

@my_decorator
def greet(name):
    print(f"Привет, {name}!")

greet("Alice")
# до вызова
# Привет, Alice!
# после вызова
```

Запись `@my_decorator` эквивалентна:

```python
greet = my_decorator(greet)
```

Декораторы могут принимать аргументы — тогда это «фабрика декораторов»:

```python
def repeat(times):
    def decorator(func):
        def wrapper(*args, **kwargs):
            for _ in range(times):
                func(*args, **kwargs)
        return wrapper
    return decorator

@repeat(3)
def hi():
    print("hi")

hi()   # hi hi hi
```

Важно использовать `functools.wraps`, чтобы сохранить имя и docstring функции:

```python
from functools import wraps

def logged(func):
    @wraps(func)
    def wrapper(*args, **kwargs):
        print(f"вызов {func.__name__}")
        return func(*args, **kwargs)
    return wrapper
```

## 2. Что такое генераторы и yield?

Генератор — функция с `yield`, которая возвращает **ленивый итератор**. При вызове
`next()` выполнение идёт до `yield`, отдаёт значение и «замирает» до следующего вызова.

```python
def countdown(n):
    while n > 0:
        yield n
        n -= 1

g = countdown(3)
next(g)   # 3
next(g)   # 2
next(g)   # 1
next(g)   # StopIteration
```

Преимущества генераторов:

- **Память** — элементы вычисляются по одному, не хранятся целиком.
- **Ленивость** — можно работать с бесконечными последовательностями.
- **Поток** — удобны для обработки больших данных построчно.

```python
def infinite():
    i = 0
    while True:
        yield i
        i += 1

g = infinite()
next(g)   # 0
next(g)   # 1
```

`yield` может и **получать** значения через `send()`, а также использоваться в
генераторных выражениях `(x for x in ...)`.

## 3. Что такое GIL?

**GIL (Global Interpreter Lock)** — глобальная блокировка интерпретатора, которая
позволяет исполнять **только один байткод Python в каждый момент времени** в одном
процессе, даже на многоядерном процессоре.

Следствия:

- Многопоточность в CPython **не даёт** параллелизма для CPU-bound задач.
- Потоки полезны для I/O-bound задач (ожидание сети/диска освобождает GIL).
- Для CPU-bound параллелизма используют `multiprocessing` (отдельные процессы).

```text
Поток A ──┐
          ├── GIL ──→ один поток Python исполняется за раз
Поток B ──┘
```

Важно понимать: GIL — особенность **CPython** (референсной реализации). Jython, PyPy
(частично) и другие реализации могут вести себя иначе. Python 3.13+ движется в сторону
ослабления GIL (экспериментальный free-threaded режим).

## 4. Как работает управление памятью в Python?

Три уровня:

1. **Reference counting** — каждый объект хранит счётчик ссылок. Когда счётчик падает
   до нуля, память немедленно освобождается.

```python
import sys
a = [1, 2, 3]
sys.getrefcount(a)   # количество ссылок
```

2. **Garbage collector (GC)** — обрабатывает **циклические ссылки**, которые reference
   counting не может собрать (объекты ссылаются друг на друга).

```python
import gc

a = []
b = [a]
a.append(b)   # цикл: a → b → a
del a, b
gc.collect()  # сборщик уберёт цикл
```

3. **Пул памяти** — CPython использует собственный аллокатор (pymalloc) для мелких
   объектов и арены памяти, чтобы не обращаться к ОС слишком часто.

Ключевая идея: освобождение по reference counting происходит сразу и детерминированно,
но циклические ссылки убирает только GC.

## 5. Что такое dataclasses?

`dataclasses` автоматически генерирует `__init__`, `__repr__`, `__eq__` и другие методы
по объявленным полям — избавляет от шаблонного кода.

```python
from dataclasses import dataclass

@dataclass
class Point:
    x: int
    y: int

p = Point(1, 2)
print(p)        # Point(x=1, y=2)
Point(1, 2) == Point(1, 2)   # True
```

Полезные параметры:

- `frozen=True` — делает экземпляры неизменяемыми (и хешируемыми).
- `order=True` — добавляет операторы сравнения.
- `field()` — настройка полей (значение по умолчанию, исключение из `__repr__` и др.).

```python
@dataclass(frozen=True)
class Point:
    x: int
    y: int

hash(Point(1, 2))   # теперь хешируем
```

## 6. Зачем нужны type hints?

Type hints — аннотации типов, которые **не проверяются во время выполнения**, но дают
возможность статического анализа (mypy, pyright, IDE).

```python
def add(a: int, b: int) -> int:
    return a + b

def greet(names: list[str]) -> None:
    for n in names:
        print(n)
```

Преимущества:

- раннее обнаружение ошибок через статические анализаторы;
- автодополнение и подсказки в IDE;
- самодокументирование кода;
- упрощение рефакторинга.

Важно помнить: аннотации — это подсказки, а не гарантии. Python их не навязывает:

```python
add("a", "b")   # отработает, но статический анализатор укажет на ошибку
```

## 7. Что такое метаклассы?

Метакласс — это «класс класса». Как класс описывает, как создаются объекты, так
метакласс описывает, как создаются классы. По умолчанию метакласс всех классов — `type`.

```python
# type создаёт класс динамически
MyClass = type("MyClass", (), {"x": 10})
obj = MyClass()
obj.x   # 10
```

Собственный метакласс наследуется от `type` и переопределяет `__new__`/`__init__`,
которые вызываются при создании класса:

```python
class Meta(type):
    def __new__(cls, name, bases, dct):
        dct["greeting"] = "hello"
        return super().__new__(cls, name, bases, dct)

class A(metaclass=Meta):
    pass

A.greeting   # 'hello'
```

Метаклассы — мощный, но редко нужный инструмент. Обычно их заменяют на декораторы
классов или `__init_subclass__`, что проще и читабельнее. Метаклассы применяют в
библиотеках и фреймворках (например, в ORM для создания полей таблиц).
