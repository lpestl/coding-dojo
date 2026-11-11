## 1. Tasks

1. [Find problem](TaskLiveCode01/README.md)

## 2. Questions

### 2.1 Python basics

- [Чем `list` отличается от `tuple`?](PythonBasic.md#1-чем-list-отличается-от-tuple)
- [Что такое `mutable` и `immutable` типы?](PythonBasic.md#2-что-такое-mutable-и-immutable-типы)
- [Чем оператор `is` отличается от `==`?](PythonBasic.md#3-чем-оператор-is-отличается-от)
- [Что такое `*args` и `**kwargs`?](PythonBasic.md#4-что-такое-args-и-kwargs)
- [Что такое итератор и итерируемый объект?](PythonBasic.md#5-что-такое-итератор-и-итерируемый-объект)
- [Почему изменяемый объект нельзя использовать как значение по умолчанию?](PythonBasic.md#6-почему-изменяемый-объект-нельзя-использовать-как-значение-по-умолчанию)
- [Что такое срезы (slicing)?](PythonBasic.md#7-что-такое-срезы-slicing)
- [Как работает контекстный менеджер `with`?](PythonBasic.md#8-как-работает-контекстный-менеджер-with)
- [Что такое `global` и `nonlocal`?](PythonBasic.md#9-что-такое-global-и-nonlocal)
- [Способы форматирования строк: `f-string`, `format`, `%`](PythonBasic.md#10-способы-форматирования-строк-f-string-format)

### 2.2 ООП

- [Что такое наследование, полиморфизм, инкапсуляция?](OOP.md#1-что-такое-наследование-полиморфизм-инкапсуляция)
- [Что такое `self`?](OOP.md#2-что-такое-self)
- [Чем `@classmethod`, `@staticmethod` отличаются от обычного метода?](OOP.md#3-чем-classmethod-staticmethod-отличаются-от-обычного-метода)
- [Чем `__init__` отличается от `__new__`?](OOP.md#4-чем-init-отличается-от-new)
- [Какие магические методы важно знать?](OOP.md#5-какие-магические-методы-важно-знать)
- [Что такое MRO и как решается diamond problem?](OOP.md#6-что-такое-mro-и-как-решается-diamond-problem)
- [Что делает `super()`?](OOP.md#7-что-делает-super)
- [Что такое ABC и `@abstractmethod`?](OOP.md#8-что-такое-abc-и-abstractmethod)
- [Что такое `property` и зачем нужны геттеры/сеттеры?](OOP.md#9-что-такое-property-и-зачем-нужны-геттерысеттеры)
- [Что такое `__slots__`?](OOP.md#10-что-такое-slots)

### 2.3 Стандартная библиотека и структуры данных

- [Как устроены `list`, `dict`, `set`, `tuple`?](Stdlib.md#1-как-устроены-list-dict-set-tuple)
- [Какова сложность основных операций?](Stdlib.md#2-какова-сложность-основных-операций)
- [Как работает `dict` изнутри?](Stdlib.md#3-как-работает-dict-изнутри)
- [Что полезного есть в модуле `collections`?](Stdlib.md#4-что-полезного-есть-в-модуле-collections)
- [Чем list comprehension отличается от generator expression?](Stdlib.md#5-чем-list-comprehension-отличается-от-generator-expression)
- [Что даёт модуль `functools`?](Stdlib.md#6-что-даёт-модуль-functools)
- [Как работают `enumerate`, `zip`, `map`, `filter`?](Stdlib.md#7-как-работают-enumerate-zip-map-filter)

### 2.4 Продвинутый Python

- [Что такое декораторы и как они работают?](AdvancedPython.md#1-что-такое-декораторы-и-как-они-работают)
- [Что такое генераторы и `yield`?](AdvancedPython.md#2-что-такое-генераторы-и-yield)
- [Что такое GIL?](AdvancedPython.md#3-что-такое-gil)
- [Как работает управление памятью в Python?](AdvancedPython.md#4-как-работает-управление-памятью-в-python)
- [Что такое `dataclasses`?](AdvancedPython.md#5-что-такое-dataclasses)
- [Зачем нужны type hints?](AdvancedPython.md#6-зачем-нужны-type-hints)
- [Что такое метаклассы?](AdvancedPython.md#7-что-такое-метаклассы)

### 2.5 Асинхронность и многопоточность

- [Threading vs multiprocessing vs asyncio: когда что?](AsyncConcurrency.md#1-threading-vs-multiprocessing-vs-asyncio-когда-что)
- [Как GIL влияет на многопоточность?](AsyncConcurrency.md#2-как-gil-влияет-на-многопоточность)
- [Что такое race condition и deadlock?](AsyncConcurrency.md#3-что-такое-race-condition-и-deadlock)
- [Как работает `asyncio` и event loop?](AsyncConcurrency.md#4-как-работает-asyncio-и-event-loop)
- [Чем `asyncio.Lock` отличается от `threading.Lock`?](AsyncConcurrency.md#5-чем-asynciolock-отличается-от-threadinglock)
- [Что такое `concurrent.futures`?](AsyncConcurrency.md#6-что-такое-concurrentfutures)

## 3. Что особенно хорошо выучить перед Python интервью

Если времени немного, приоритет можно расставить следующим образом.

### Очень важно

1. Mutable vs immutable, `list` vs `tuple`.
2. `is` vs `==`.
3. `*args` / `**kwargs`, распаковка.
4. Итераторы и генераторы, `yield`.
5. Декораторы и `functools.wraps`.
6. ООП: `self`, `super`, MRO, `@classmethod`/`@staticmethod`/`@property`.
7. Магические методы: `__init__`, `__repr__`, `__eq__`, `__hash__`.
8. `dict` и его сложность, `collections`.
9. GIL и разница threading/multiprocessing/asyncio.
10. Контекстные менеджеры, `with`.
11. Управление памятью, reference counting.
12. Comprehensions и generator expressions.

### Для Middle/Senior

- метаклассы и `__new__`;
- дескрипторы (`__get__`/`__set__`) и `property`;
- `asyncio` на глубоком уровне (event loop, tasks, cancellation);
- потоки и GIL, race condition, deadlock, синхронизация;
- `asyncio.Lock` vs `threading.Lock`, reentrancy;
- weak references, `__slots__`, copy vs deepcopy;
- `functools.lru_cache`, `partial`, `singledispatch`;
- `itertools` и ленивые вычисления;
- дизайн-паттерны и SOLID;
- тестирование (pytest, mocking, fixtures).

### Как обычно выглядит глубокий вопрос

Очень частый формат собеседования — не просто:

> Что такое генератор?

а цепочка вопросов:

```text
Что такое генератор?
        ↓
Чем он отличается от списка?
        ↓
Что такое итератор и итерируемый объект?
        ↓
Что делает yield?
        ↓
Как работает ленивое вычисление?
        ↓
Что произойдёт, если перебрать генератор дважды?
```

Именно такие цепочки стоит потренировать: они гораздо лучше показывают глубину понимания Python, чем заучивание определений.
