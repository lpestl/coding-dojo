# Основы ООП

## 1. Что такое наследование, полиморфизм, инкапсуляция?

Это три базовых понятия ООП в C++. Их удобно понимать через одну модель: есть базовый класс `Animal`, а от него наследуются конкретные животные.

### 1. Наследование

Наследование — это механизм, позволяющий создать новый класс на основе существующего.

Например:

```cpp
class Animal {
public:
    void eat() {
        std::cout << "Eating\n";
    }
};

class Dog : public Animal {
public:
    void bark() {
        std::cout << "Woof!\n";
    }
};
```

Теперь `Dog` наследует методы `Animal`:

```cpp
Dog dog;

dog.eat();   // пришло из Animal
dog.bark();  // собственный метод Dog
```

Схематично:

```text
        Animal
           ↑
           |
          Dog
```

То есть:

> `Dog` является `Animal` и получает его доступные члены.

**`public` при наследовании**

```cpp
class Dog : public Animal
```

означает public inheritance.

Это соответствует отношению:

> Dog is-a Animal

Например:

```text
Animal
├── Dog
├── Cat
└── Bird
```

### 2. Инкапсуляция

Инкапсуляция — это объединение данных и методов для работы с ними внутри класса и контроль доступа к внутреннему состоянию объекта.

Например:

```cpp
class BankAccount {
private:
    double balance = 0;

public:
    void deposit(double amount) {
        if (amount > 0)
            balance += amount;
    }

    double getBalance() const {
        return balance;
    }
};
```

Пользователь класса не может напрямую изменить `balance`:

```cpp
BankAccount account;

account.balance = -1000; // ошибка
```

Вместо этого он использует публичный интерфейс:

```cpp
account.deposit(100);
std::cout << account.getBalance();
```

Смысл:

```text
          BankAccount
       ┌─────────────────┐
       │ private         │
       │ balance         │ ← скрыто
       │                 │
       │ public          │
       │ deposit()       │ ← интерфейс
       │ getBalance()    │ ← интерфейс
       └─────────────────┘
```

То есть инкапсуляция отвечает на вопрос:

> Что объект разрешает делать снаружи, а что скрывает внутри?

И здесь как раз важны `private`, `protected`, `public`.

### 3. Полиморфизм

Полиморфизм — возможность обращаться к объектам разных классов через общий интерфейс, при этом они могут выполнять действие по-разному.

Самый важный пример — `virtual`.

```cpp
class Animal {
public:
    virtual void makeSound() const {
        std::cout << "Some sound\n";
    }

    virtual ~Animal() = default;
};

class Dog : public Animal {
public:
    void makeSound() const override {
        std::cout << "Woof!\n";
    }
};

class Cat : public Animal {
public:
    void makeSound() const override {
        std::cout << "Meow!\n";
    }
};
```

Теперь:

```cpp
Dog dog;
Cat cat;

Animal& a1 = dog;
Animal& a2 = cat;

a1.makeSound(); // Woof!
a2.makeSound(); // Meow!
```

Хотя обе переменные имеют тип:

```cpp
Animal&
```

реально вызывается разная реализация.

```text
Animal&
   │
   ├──→ Dog → makeSound() → "Woof!"
   │
   └──→ Cat → makeSound() → "Meow!"
```

Вот это и есть runtime polymorphism.

### Как эти три понятия связаны

Можно посмотреть на них как на разные стороны ООП:

| Понятие | Смысл |
|---|---|
| Наследование | Создать класс на основе другого |
| Инкапсуляция | Скрыть внутреннюю реализацию и контролировать доступ |
| Полиморфизм | Один интерфейс → разное поведение |

Например:

```cpp
class Animal {
private:
    int age;

public:
    virtual void makeSound() const = 0;
};
```

Здесь одновременно:

- `private age` → инкапсуляция
- `Dog : public Animal` → наследование
- `virtual makeSound()` → полиморфизм

### Важный нюанс

Наследование не равно полиморфизм.

Можно иметь:

```cpp
class Dog : public Animal
{
};
```

и вообще не использовать `virtual`.

А полиморфизм в C++ бывает не только через наследование и `virtual`: существуют также compile-time polymorphism (перегрузка, шаблоны и т. д.).

Если говорить про собеседования, то обычно под «полиморфизмом» в контексте ООП прежде всего имеют в виду именно:

```cpp
Animal* animal = new Dog;
animal->makeSound(); // вызов Dog::makeSound()
```

при наличии `virtual`.

### Коротко для запоминания

- Наследование → "является"
- Инкапсуляция → "скрываем внутренности"
- Полиморфизм → "один интерфейс, разное поведение"

## 2. Зачем нужен виртуальный деструктор?

Виртуальный деструктор нужен, чтобы при удалении объекта через указатель на базовый класс корректно вызвался деструктор производного класса.

Это напрямую связано с полиморфизмом.

### Проблема

Представим:

```cpp
class Animal {
public:
    ~Animal() {
        std::cout << "Animal destroyed\n";
    }
};

class Dog : public Animal {
public:
    ~Dog() {
        std::cout << "Dog destroyed\n";
    }
};
```

Теперь:

```cpp
Animal* animal = new Dog();

delete animal;
```

На первый взгляд мы удаляем `Dog`, но указатель имеет тип `Animal*`.

Поскольку деструктор `Animal` не `virtual`, поведение программы здесь неопределено (UB).

### Правильно

Делаем деструктор базового класса виртуальным:

```cpp
class Animal {
public:
    virtual ~Animal() {
        std::cout << "Animal destroyed\n";
    }
};

class Dog : public Animal {
public:
    ~Dog() {
        std::cout << "Dog destroyed\n";
    }
};
```

Теперь:

```cpp
Animal* animal = new Dog();

delete animal;
```

вызовет:

```text
Dog::~Dog()
     ↓
Animal::~Animal()
```

То есть сначала уничтожается часть `Dog`, затем часть `Animal`.

### Почему это важно?

Производный класс может владеть ресурсами:

```cpp
class Dog : public Animal {
    int* data;

public:
    Dog() {
        data = new int[100];
    }

    ~Dog() {
        delete[] data;
    }
};
```

Если уничтожать `Dog` через `Animal*`, деструктор `Dog` должен быть вызван, иначе его ресурс может не освободиться.

Именно поэтому для полиморфного базового класса обычно пишут:

```cpp
class Animal {
public:
    virtual ~Animal() = default;
};
```

Это особенно важно в сочетании с RAII и управлением объектами через указатели.

### Хорошее правило для собеседования

Если класс предназначен для использования как полиморфный базовый класс, его деструктор обычно должен быть:

```cpp
virtual ~Base() = default;
```

Например:

```cpp
class Animal {
public:
    virtual void makeSound() = 0;
    virtual ~Animal() = default;
};
```

Здесь одновременно:

- `virtual makeSound()` → полиморфизм;
- `virtual ~Animal()` → корректное уничтожение через `Animal*` / `Animal&`-ориентированный интерфейс.

### Запомнить можно так

> Если собираешься делать `delete` производного объекта через указатель на базовый класс — базовый деструктор должен быть `virtual`.

## 3. Чем `virtual` отличается от `override`?

Разница очень простая:

- `virtual` — говорит: «этот метод может быть переопределён, и при вызове через базовый тип используй виртуальный dispatch».
- `override` — говорит компилятору: «я намеренно переопределяю виртуальный метод базового класса, проверь это».

### Пример

```cpp
class Animal {
public:
    virtual void makeSound() const {
        std::cout << "Some sound\n";
    }
};

class Dog : public Animal {
public:
    void makeSound() const override {
        std::cout << "Woof!\n";
    }
};
```

Здесь:

```cpp
virtual void makeSound()
```

в `Animal` включает виртуальный полиморфизм.

А:

```cpp
void makeSound() override
```

в `Dog` означает, что `Dog::makeSound()` переопределяет виртуальный метод `Animal::makeSound()`.

### Зачем нужен `override`?

Главное преимущество — защита от случайной ошибки.

Например:

```cpp
class Animal {
public:
    virtual void makeSound() const;
};

class Dog : public Animal {
public:
    void makeSound() override; // ошибка компиляции
};
```

Почему ошибка?

В базовом классе:

```cpp
makeSound() const
```

а в `Dog`:

```cpp
makeSound()
```

`const` пропущен.

Без `override` компилятор мог бы воспринять это как новый метод, а не как переопределение.

С `override` компилятор говорит:

> «Ты написал `override`, но подходящего виртуального метода для переопределения нет».

Это очень полезно.

### `virtual` можно не писать в наследнике

Если метод уже виртуальный в базовом классе:

```cpp
class Animal {
public:
    virtual void makeSound();
};

class Dog : public Animal {
public:
    void makeSound() override;
};
```

`Dog::makeSound()` остаётся виртуальным, даже если мы не написали `virtual`.

Можно написать:

```cpp
virtual void makeSound() override;
```

но обычно это избыточно.

Предпочтительнее:

```cpp
void makeSound() override;
```

### Главное различие

| | `virtual` | `override` |
|---|---|---|
| Что делает? | Включает/обозначает виртуальный метод | Проверяет переопределение |
| Где обычно пишется? | В базовом классе | В производном |
| Нужен для полиморфизма? | Да, для виртуального dispatch | Нет, это проверка |
| Проверяет, что метод действительно переопределяет базовый? | Нет | Да |

### Запомнить

- `virtual` → «метод виртуальный»
- `override` → «я переопределяю виртуальный метод»

И практически хорошая привычка: если переопределяешь виртуальный метод — почти всегда ставь `override`.

## 4. Что такое pure virtual function?

Pure virtual function (чисто виртуальная функция) — это виртуальный метод, для которого базовый класс не предоставляет обязательной реализации, а производные классы должны её переопределить.

### Синтаксис

```cpp
virtual void makeSound() = 0;
```

`= 0` здесь и означает pure virtual.

### Пример

```cpp
class Animal {
public:
    virtual void makeSound() = 0;
};
```

Теперь `Animal` — абстрактный класс.

Нельзя написать:

```cpp
Animal animal; // ошибка
```

Потому что непонятно, какой именно `makeSound()` должен выполнять `Animal`.

Но можно создать производный класс:

```cpp
class Dog : public Animal {
public:
    void makeSound() override {
        std::cout << "Woof!\n";
    }
};
```

И использовать:

```cpp
Dog dog;
dog.makeSound(); // Woof!
```

### Зачем это нужно?

Pure virtual function позволяет задать интерфейс, но оставить реализацию производным классам.

Например:

```cpp
class Shape {
public:
    virtual double area() const = 0;
    virtual ~Shape() = default;
};
```

Мы говорим:

> «Любая фигура должна уметь вычислять площадь, но я не знаю, как именно».

Круг:

```cpp
class Circle : public Shape {
public:
    double area() const override {
        return 3.14 * radius * radius;
    }

private:
    double radius = 5;
};
```

Прямоугольник:

```cpp
class Rectangle : public Shape {
public:
    double area() const override {
        return width * height;
    }

private:
    double width = 10;
    double height = 20;
};
```

Теперь можно работать через общий интерфейс:

```cpp
Shape& shape = circle;

std::cout << shape.area();
```

### Pure virtual ≠ просто virtual

```cpp
class A {
public:
    virtual void foo() {
        std::cout << "A";
    }
};
```

Это обычная виртуальная функция. У неё есть реализация.

А:

```cpp
class A {
public:
    virtual void foo() = 0;
};
```

Это pure virtual function. Она делает класс абстрактным.

### Связь с предыдущими понятиями

Получается такая цепочка:

```text
virtual
   ↓
runtime polymorphism
   ↓
pure virtual function
   ↓
абстрактный класс
   ↓
общий интерфейс для производных классов
```

Типичный C++-интерфейс выглядит примерно так:

```cpp
class Animal {
public:
    virtual void makeSound() const = 0;
    virtual ~Animal() = default;
};
```

Здесь:

- `virtual` → участвует в полиморфизме;
- `= 0` → функция чисто виртуальная;
- класс становится абстрактным;
- `virtual ~Animal()` → безопасное удаление производного объекта через `Animal*`.

Для собеседования: pure virtual function — это виртуальная функция, объявленная с `= 0`, которая делает класс абстрактным и задаёт обязательный интерфейс для производных классов.

## 5. Можно ли вызвать виртуальную функцию из конструктора?

Да, синтаксически вызвать можно, но есть очень важный нюанс:

> Во время конструктора виртуальный вызов не ведёт себя как обычный полиморфный вызов.

### Рассмотрим

```cpp
class Base {
public:
    Base() {
        foo();
    }

    virtual void foo() {
        std::cout << "Base\n";
    }
};

class Derived : public Base {
public:
    void foo() override {
        std::cout << "Derived\n";
    }
};
```

Создаём:

```cpp
Derived d;
```

Можно ожидать:

```text
Derived
```

Но фактически будет:

```text
Base
```

### Почему?

Потому что когда выполняется конструктор `Base`, объект `Derived` ещё не сконструирован полностью.

Порядок такой:

```text
создание Derived
       ↓
конструктор Base
       ↓
конструктор Derived
       ↓
объект полностью создан
```

Во время `Base::Base()` объект рассматривается как `Base`, поэтому виртуальный вызов идёт в `Base::foo()`.

### Ещё нагляднее

```cpp
class Base {
public:
    Base() {
        foo();  // Base::foo()
    }

    virtual void foo() {
        std::cout << "Base\n";
    }
};

class Derived : public Base {
public:
    Derived() {
        foo();  // Derived::foo()
    }

    void foo() override {
        std::cout << "Derived\n";
    }
};
```

При:

```cpp
Derived d;
```

получим:

```text
Base
Derived
```

Потому что:

```text
Base constructor → Base::foo()
Derived constructor → Derived::foo()
```

### Почему это опасно?

Представим:

```cpp
class Base {
public:
    Base() {
        init();
    }

    virtual void init() = 0;
};
```

Не стоит рассчитывать, что во время конструктора `Base` будет вызвана реализация `Derived`.

Производная часть объекта ещё не инициализирована.

### А что с деструктором?

Практически то же самое правило работает в обратную сторону.

```cpp
class Base {
public:
    virtual ~Base() {
        foo();
    }

    virtual void foo() {
        std::cout << "Base\n";
    }
};

class Derived : public Base {
public:
    ~Derived() {
        foo();
    }

    void foo() override {
        std::cout << "Derived\n";
    }
};
```

При уничтожении:

```text
Derived destructor → Derived::foo()
        ↓
Base destructor → Base::foo()
```

Потому что сначала уничтожается `Derived`, затем базовая часть.

### Главное для собеседования

В конструкторах и деструкторах виртуальные вызовы не работают полиморфно.

В конструкторе вызывается версия текущего конструируемого класса, а в деструкторе — версия текущего разрушаемого класса.

Поэтому обычно не следует вызывать виртуальные методы из конструкторов и деструкторов, если вы ожидаете вызов переопределённой версии в наследнике.

## 6. Чем композиция отличается от наследования?

Главная разница — в отношении между объектами:

- Наследование → «является» (is-a)
- Композиция → «содержит / имеет» (has-a)

### 1. Наследование

Один класс является разновидностью другого:

```cpp
class Animal {
public:
    void eat() {
        std::cout << "Eating\n";
    }
};

class Dog : public Animal {
};
```

Здесь:

```text
Dog
 ↑
Animal
```

Можно сказать:

> Dog is an Animal

`Dog` получает интерфейс и поведение `Animal`.

### 2. Композиция

Один объект содержит другой объект как своё поле.

Например:

```cpp
class Engine {
public:
    void start() {
        std::cout << "Engine started\n";
    }
};

class Car {
private:
    Engine engine;

public:
    void start() {
        engine.start();
    }
};
```

Здесь:

```text
Car
 │
 └── Engine
```

То есть:

> Car has an Engine

`Car` не является `Engine`. Он содержит `Engine`.

### Главное различие

| Наследование | Композиция |
|---|---|
| is-a | has-a |
| `Dog : Animal` | `Car` содержит `Engine` |
| Получаем интерфейс базового класса | Используем объект как поле |
| Часто создаёт сильную связь | Обычно более гибкая связь |
| Может использовать полиморфизм | Не требует наследования |

### Почему часто предпочитают композицию?

Представим:

```cpp
class Car : public Engine {
};
```

Это странно:

```text
Car is an Engine ❌
```

Логичнее:

```cpp
class Car {
    Engine engine;
};
```

```text
Car has an Engine ✅
```

### Композиция позволяет менять компоненты

Например:

```cpp
class Engine {
public:
    virtual void start() = 0;
    virtual ~Engine() = default;
};

class ElectricEngine : public Engine {
public:
    void start() override {
        std::cout << "Electric engine\n";
    }
};

class GasEngine : public Engine {
public:
    void start() override {
        std::cout << "Gas engine\n";
    }
};
```

`Car` может работать с любым двигателем:

```cpp
class Car {
private:
    std::unique_ptr<Engine> engine;

public:
    Car(std::unique_ptr<Engine> engine)
        : engine(std::move(engine)) {}

    void start() {
        engine->start();
    }
};
```

Теперь:

```cpp
Car electricCar(std::make_unique<ElectricEngine>());
Car gasCar(std::make_unique<GasEngine>());
```

Получаем:

```text
             Car
              │
       ┌──────┴──────┐
       ↓             ↓
ElectricEngine   GasEngine
```

Это сочетает композицию + полиморфизм.

### Наследование vs композиция на практике

Допустим, у нас есть:

```cpp
Dog
```

Если вопрос:

> «Собака является животным?»

Да → наследование.

```cpp
class Dog : public Animal {};
```

Если вопрос:

> «У машины есть двигатель?»

Да → композиция.

```cpp
class Car {
    Engine engine;
};
```

### Хорошее правило для собеседования

Если отношение is-a — рассматриваем наследование. Если has-a — композицию.

И ещё практическое правило:

> Предпочитай композицию наследованию, когда наследование не выражает настоящее отношение «является».

Это связано с тем, что композиция обычно даёт меньше связанности и больше гибкости.

## 7. Что такое object slicing?

Object slicing — это ситуация, когда объект производного класса копируют в объект базового класса по значению, и производная часть объекта «обрезается».

Это особенно важно в контексте наследования и полиморфизма.

### Пример

```cpp
class Animal {
public:
    int age = 5;
};

class Dog : public Animal {
public:
    int bones = 10;
};
```

Создадим `Dog`:

```cpp
Dog dog;
```

Внутри него есть:

```text
Dog
┌─────────────┐
│ Animal      │
│ age = 5     │
├─────────────┤
│ Dog         │
│ bones = 10  │
└─────────────┘
```

Теперь:

```cpp
Animal animal = dog;
```

Происходит slicing:

```text
Dog
┌─────────────┐
│ age = 5     │ ← копируется
├─────────────┤
│ bones = 10  │ ← теряется
└─────────────┘
        ↓
Animal
┌─────────────┐
│ age = 5     │
└─────────────┘
```

`animal` — это уже отдельный объект `Animal`. Часть `Dog` в нём не существует.

### Почему это проблема с полиморфизмом?

Допустим:

```cpp
class Animal {
public:
    virtual void makeSound() const {
        std::cout << "Animal\n";
    }

    virtual ~Animal() = default;
};

class Dog : public Animal {
public:
    void makeSound() const override {
        std::cout << "Woof\n";
    }
};
```

Если сделать:

```cpp
Dog dog;

Animal animal = dog;

animal.makeSound();
```

получим:

```text
Animal
```

а не:

```text
Woof
```

Потому что `dog` был скопирован в новый объект `Animal`. Это уже не `Dog`.

### Как избежать slicing?

**1. Использовать ссылку**

```cpp
Dog dog;

Animal& animal = dog;

animal.makeSound(); // Woof
```

Здесь копирования нет.

```text
Animal& ──────→ Dog
```

**2. Использовать указатель**

```cpp
Dog dog;

Animal* animal = &dog;

animal->makeSound(); // Woof
```

**3. Использовать smart pointer**

Например:

```cpp
std::unique_ptr<Animal> animal =
    std::make_unique<Dog>();

animal->makeSound(); // Woof
```

Здесь особенно хорошо сочетаются наши предыдущие темы:

- наследование;
- `virtual`;
- полиморфизм;
- `unique_ptr`;
- RAII.

### Ещё один частый случай — передача функции

Плохо:

```cpp
void process(Animal animal) {
    animal.makeSound();
}
```

Если вызвать:

```cpp
Dog dog;
process(dog);
```

произойдёт slicing.

Лучше:

```cpp
void process(const Animal& animal) {
    animal.makeSound();
}
```

Теперь:

```cpp
Dog dog;
process(dog); // Woof
```

### Как запомнить

```text
Dog dog;
     ↓
Animal animal = dog;
     ↓
     SLICING
     ↓
Dog → Animal
      ↑
производная часть потеряна
```

Ключевая мысль:

> Object slicing происходит, когда производный объект копируется в базовый объект по значению.

Чтобы сохранить полиморфизм, обычно используют ссылку или указатель, а не базовый объект по значению.
