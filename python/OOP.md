# ООП в Python

## 1. Что такое наследование, полиморфизм, инкапсуляция?

Три базовых понятия ООП.

### Наследование

Новый класс получает атрибуты и методы родителя.

```python
class Animal:
    def eat(self):
        print("Eating")

class Dog(Animal):
    def bark(self):
        print("Woof!")

dog = Dog()
dog.eat()    # из Animal
dog.bark()   # собственный метод
```

### Инкапсуляция

Скрытие внутреннего состояния и управление доступом через интерфейс. В Python она
реализуется **соглашением**, а не жёстко: `_x` — «не трогай», `__x` — name mangling.

```python
class BankAccount:
    def __init__(self):
        self._balance = 0   # приватное по соглашению

    def deposit(self, amount):
        if amount > 0:
            self._balance += amount

    @property
    def balance(self):
        return self._balance
```

### Полиморфизм

Разные классы реализуют один интерфейс по-своему. В Python — «утиная типизация»:
важно наличие метода, а не тип.

```python
class Dog:
    def make_sound(self):
        return "Woof!"

class Cat:
    def make_sound(self):
        return "Meow!"

def speak(animal):
    print(animal.make_sound())

speak(Dog())   # Woof!
speak(Cat())   # Meow!
```

## 2. Что такое self?

`self` — ссылка на текущий экземпляр класса. Это **первый параметр метода**, и Python
подставляет его автоматически при вызове.

```python
class Point:
    def __init__(self, x, y):
        self.x = x   # self.x — атрибут экземпляра
        self.y = y

    def move(self, dx, dy):
        self.x += dx

p = Point(1, 2)
p.move(10, 20)   # эквивалентно Point.move(p, 10, 20)
```

`self` — просто имя по соглашению; можно назвать иначе, но не нужно. Через `self`
экземпляр хранит своё состояние и передаёт себя в методы.

## 3. Чем @classmethod, @staticmethod отличаются от обычного метода?

| Метод | Первый аргумент | Что получает | Когда использовать |
|---|---|---|---|
| instance method | `self` | экземпляр | работа с состоянием экземпляра |
| `@classmethod` | `cls` | класс | фабрики, доступ к атрибутам класса, полиморфизм на уровне класса |
| `@staticmethod` | ничего | — | утилитарная функция, логически связанная с классом |

```python
class Date:
    def __init__(self, year, month, day):
        self.year, self.month, self.day = year, month, day

    @classmethod
    def from_string(cls, s):
        year, month, day = s.split("-")
        return cls(int(year), int(month), int(day))   # использует cls

    @staticmethod
    def is_valid(year):
        return 0 <= year <= 9999

Date.from_string("2024-01-15")
Date.is_valid(2024)
```

`classmethod` полезен, когда нужно создать объект подкласса из родительского метода —
`cls` указывает на реальный класс.

## 4. Чем __init__ отличается от __new__?

- `__new__` — создаёт объект (вызывается первым), статический метод, принимает `cls`
  и возвращает экземпляр.
- `__init__` — инициализирует уже созданный объект, ничего не возвращает (кроме `None`).

```python
class A:
    def __new__(cls, *args, **kwargs):
        print("new")
        return super().__new__(cls)

    def __init__(self):
        print("init")

a = A()
# new
# init
```

`__new__` нужен редко: при наследовании от immutable-типов (`int`, `str`, `tuple`) или
для реализации синглтонов.

```python
class UpperStr(str):
    def __new__(cls, value):
        return super().__new__(cls, value.upper())

UpperStr("hi")   # 'HI'
```

## 5. Какие магические методы важно знать?

| Метод | Назначение |
|---|---|
| `__init__` | инициализация |
| `__repr__` / `__str__` | строковое представление |
| `__eq__` / `__ne__` | сравнение |
| `__lt__`, `__le__`, `__gt__`, `__ge__` | упорядочивание |
| `__hash__` | хеш (нужен для dict/set) |
| `__len__` | `len(obj)` |
| `__getitem__` / `__setitem__` | доступ по индексу/ключу |
| `__iter__` / `__next__` | итерация |
| `__call__` | вызов как функции |
| `__enter__` / `__exit__` | контекстный менеджер |
| `__add__`, `__mul__` и др. | арифметика |
| `__contains__` | оператор `in` |

Пример:

```python
class Vector:
    def __init__(self, x, y):
        self.x, self.y = x, y

    def __repr__(self):
        return f"Vector({self.x}, {self.y})"

    def __eq__(self, other):
        return self.x == other.x and self.y == other.y

    def __add__(self, other):
        return Vector(self.x + other.x, self.y + other.y)

Vector(1, 2) + Vector(3, 4)   # Vector(4, 6)
```

Важно: переопределив `__eq__`, нужно переопределить и `__hash__` (иначе объект станет
нехешируемым), а лучше использовать `@dataclass` или `functools.total_ordering`.

## 6. Что такое MRO и как решается diamond problem?

**MRO (Method Resolution Order)** — порядок, в котором Python ищет методы по цепочке
наследования. Порядок можно посмотреть через `Class.__mro__`.

```python
class A:
    def method(self):
        return "A"

class B(A):
    def method(self):
        return "B"

class C(A):
    def method(self):
        return "C"

class D(B, C):
    pass

D.__mro__
# (D, B, C, A, object)
```

**Diamond problem** — когда класс наследуется от двух классов с общим предком. Python
решает его алгоритмом **C3 linearization**: линеаризация строится так, что порядок
родителей сохраняется, а общий предок оказывается в конце.

`super()` идёт **по MRO**, а не по «родителю напрямую», поэтому при diamond-наследовании
общий метод предка вызывается ровно один раз.

```python
class A:
    def method(self):
        print("A")

class B(A):
    def method(self):
        print("B")
        super().method()

class C(A):
    def method(self):
        print("C")
        super().method()

class D(B, C):
    def method(self):
        print("D")
        super().method()

D().method()
# D
# B
# C
# A
```

## 7. Что делает super()?

`super()` возвращает прокси-объект, который позволяет вызвать метод следующего класса
в цепочке MRO.

```python
class Parent:
    def __init__(self, name):
        self.name = name

class Child(Parent):
    def __init__(self, name, age):
        super().__init__(name)   # вызвать __init__ родителя
        self.age = age
```

Ключевые моменты:

- `super()` идёт по MRO, что важно при множественном наследовании.
- В Python 3 аргументы можно опускать: `super().__init__(...)`.
- `super()` в методе без явного указания класса использует текущие `cls`/`self`.

## 8. Что такое ABC и @abstractmethod?

**ABC (Abstract Base Class)** — класс, от которого нельзя создать экземпляр и который
задаёт интерфейс через абстрактные методы.

```python
from abc import ABC, abstractmethod

class Shape(ABC):
    @abstractmethod
    def area(self):
        ...

    @abstractmethod
    def perimeter(self):
        ...

class Circle(Shape):
    def __init__(self, r):
        self.r = r

    def area(self):
        return 3.14 * self.r ** 2

    def perimeter(self):
        return 2 * 3.14 * self.r

Shape()       # ❌ TypeError: Can't instantiate abstract class
Circle(5)     # ✅
```

Если подкласс не реализует все абстрактные методы, он сам остаётся абстрактным. Это
способ явно описать контракт интерфейса (в отличие от чисто «утиной типизации»).

## 9. Что такое property и зачем нужны геттеры/сеттеры?

`property` позволяет обращаться к атрибуту как к полю, но при этом выполнять код
(геттер/сеттер/делитер).

```python
class Temperature:
    def __init__(self, celsius=0):
        self._celsius = celsius

    @property
    def celsius(self):
        return self._celsius

    @celsius.setter
    def celsius(self, value):
        if value < -273.15:
            raise ValueError("ниже абсолютного нуля")
        self._celsius = value

t = Temperature()
t.celsius = 25        # вызов setter
print(t.celsius)      # вызов getter
t.celsius = -500      # ValueError
```

Зачем это нужно:

- добавить валидацию/логику при чтении и записи;
- сохранить API «как поле» — клиентский код не меняется при переходе от обычного
  атрибута к вычисляемому.

В Python «геттеры и сеттеры» не пишут заранее — их добавляют, когда появляется реальная
необходимость (в отличие от Java).

## 10. Что такое __slots__?

`__slots__` фиксирует набор атрибутов экземпляра, отключая автоматический `__dict__`.

```python
class Point:
    __slots__ = ("x", "y")

    def __init__(self, x, y):
        self.x = x
        self.y = y

p = Point(1, 2)
p.z = 3   # ❌ AttributeError: 'Point' object has no attribute 'z'
```

Плюсы:

- экономия памяти (нет словаря на каждый экземпляр);
- чуть более быстрый доступ к атрибутам;
- защита от опечаток в названиях атрибутов.

Минусы и ограничения:

- нельзя добавлять произвольные атрибуты;
- не работает со слабыми ссылками, если не добавить `__weakref__` в слоты;
- сложнее с наследованием (подкласс без `__slots__` снова получит `__dict__`).


