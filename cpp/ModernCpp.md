# Modern C++

## 1. Move semantics

Move semantics — это механизм C++, который позволяет передавать ресурсы от одного объекта к другому без дорогостоящего копирования.

Особенно важен для объектов, которые владеют ресурсами:

- динамическая память;
- большие буферы;
- файлы;
- sockets;
- mutex/handles;
- другие ресурсы, которыми управляет объект.

Главные инструменты:

- `T&&`
- `std::move()`
- move constructor
- move assignment operator

### 1. Сначала проблема: копирование

Представим класс, который владеет массивом:

```cpp
class Buffer {
private:
    int* data;
    size_t size;

public:
    Buffer(size_t n)
        : data(new int[n]), size(n) {}

    ~Buffer() {
        delete[] data;
    }
};
```

Создадим:

```cpp
Buffer a(1'000'000);
Buffer b = a;
```

Если сделать нормальное копирование, нам нужно:

```text
a
 │
 ▼
┌──────────────────┐
│ 1 000 000 ints   │
└──────────────────┘

        COPY

        ↓

b
 │
 ▼
┌──────────────────┐
│ 1 000 000 ints   │
└──────────────────┘
```

То есть нужно выделить новый миллион элементов и скопировать данные.

Это может быть дорого.

### 2. Но иногда копировать вообще не нужно

Представим:

```cpp
Buffer createBuffer() {
    Buffer temp(1'000'000);
    return temp;
}

Buffer b = createBuffer();
```

В некотором смысле нам не нужна копия `temp`.

После `createBuffer()` объект `temp` больше не нужен.

Можно сделать:

```text
temp
 │
 ▼
┌──────────────────┐
│ большой буфер     │
└──────────────────┘

        MOVE

        ↓

b
 │
 ▼
┌──────────────────┐
│ тот же буфер      │
└──────────────────┘
```

То есть вместо копирования самого ресурса мы можем передать владение ресурсом.

Это и есть идея move semantics.

### 3. Copy vs Move

Очень важное различие.

**Copy**

Создаём новый ресурс:

```text
source ──→ Resource A

destination ──→ Resource B
```

**Move**

Передаём существующий ресурс:

```text
source ──→ Resource A
              ↑
              │
destination ──┘
```

После move исходный объект остаётся существовать, но его состояние становится valid but unspecified.

### 4. Move constructor

Для этого существует move constructor:

```cpp
Buffer(Buffer&& other)
```

Обрати внимание:

```cpp
Buffer&
```

— ссылка на lvalue.

А:

```cpp
Buffer&&
```

— ссылка на rvalue.

Например:

```cpp
Buffer(Buffer&& other)
    : data(other.data),
      size(other.size)
{
    other.data = nullptr;
    other.size = 0;
}
```

Что произошло?

До:

```text
other
  │
  ▼
┌──────────────┐
│ data ────────┼────→ [ BIG BUFFER ]
└──────────────┘
```

После:

```text
other
  │
  ▼
data = nullptr


new object
  │
  ▼
┌──────────────┐
│ data ────────┼────→ [ BIG BUFFER ]
└──────────────┘
```

Данные не копировались.

Мы просто передали указатель.

### 5. Почему нужно обнулить other.data?

Потому что теперь ресурс принадлежит новому объекту.

Если оставить:

```cpp
other.data = old_pointer;
```

получится:

```text
object A ──┐
           ├──→ same memory
object B ──┘
```

И оба деструктора попробуют:

```cpp
delete[] data;
```

Получим проблему — повторное освобождение памяти, то есть undefined behavior.

Поэтому после move:

```cpp
other.data = nullptr;
```

### 6. std::move

Теперь самая известная часть:

```cpp
std::move()
```

Важно:

`std::move()` сам ничего не перемещает.

Он просто говорит компилятору:

«Рассматривай этот объект как rvalue; его ресурсы можно попытаться переместить».

Например:

```cpp
Buffer a(100);

Buffer b = std::move(a);
```

Условно:

```text
a
 │
 └──→ [ buffer ]

std::move(a)
       ↓
     xvalue
       ↓
move constructor
       ↓
b
 │
 └──→ [ buffer ]
```

### 7. Почему нельзя просто написать Buffer b = a?

Потому что:

```cpp
Buffer b = a;
```

`a` — это lvalue.

Поэтому выбирается copy constructor:

```cpp
Buffer(const Buffer& other);
```

А:

```cpp
Buffer b = std::move(a);
```

создаёт xvalue, поэтому может быть выбран:

```cpp
Buffer(Buffer&& other);
```

То есть move constructor.

### 8. Откуда вообще взялись &&?

Мы уже разбирали категории value:

- lvalue
- prvalue
- xvalue

И теперь они становятся очень полезными.

Упрощённо:

```cpp
T&   → принимает lvalue
T&&  → принимает rvalue
```

Например:

```cpp
void foo(const Buffer& x);
void foo(Buffer&& x);
```

Тогда:

```cpp
Buffer b;

foo(b);
```

вызовет:

```cpp
foo(const Buffer&)
```

А:

```cpp
foo(std::move(b));
```

может вызвать:

```cpp
foo(Buffer&&)
```

### 9. Move assignment

Есть не только move constructor, но и move assignment operator.

Конструктор:

```cpp
Buffer b = std::move(a);
```

Создаёт новый объект.

А здесь:

```cpp
b = std::move(a);
```

`b` уже существует.

Поэтому используется:

```cpp
Buffer& operator=(Buffer&& other);
```

Например:

```cpp
Buffer& operator=(Buffer&& other)
{
    if (this != &other) {
        delete[] data;

        data = other.data;
        size = other.size;

        other.data = nullptr;
        other.size = 0;
    }

    return *this;
}
```

### 10. Четыре специальных метода

В контексте resource-owning класса важно знать:

- Destructor
- Copy constructor
- Copy assignment
- Move constructor
- Move assignment

Например:

```cpp
class Buffer {
public:
    ~Buffer();

    Buffer(const Buffer& other);
    Buffer& operator=(const Buffer& other);

    Buffer(Buffer&& other);
    Buffer& operator=(Buffer&& other);
};
```

Можно представить:

```text
                 Object
                   │
        ┌──────────┴──────────┐
        │                     │
      Copy                    Move
        │                     │
   ┌────┴────┐           ┌────┴────┐
   │         │           │         │
construct  assign     construct  assign
```

### 11. Очень важная разница

**Move constructor**

```cpp
Buffer b = std::move(a);
```

`b` ещё не существует.

```text
a ──resource──→ b
```

**Move assignment**

```cpp
b = std::move(a);
```

`b` уже существует.

```text
b ──old resource──→ DELETE

a ──resource──────→ b
```

### 12. Что происходит со std::string?

Это один из самых простых способов увидеть move semantics в реальном C++.

```cpp
std::string a = "Hello";

std::string b = std::move(a);
```

Вместо потенциального копирования большого внутреннего буфера строка может передать владение своим внутренним ресурсом.

После этого:

`b` содержит "Hello".

А состояние:

`a` не следует предполагать конкретным. Оно valid but unspecified.

То есть нельзя рассуждать:

«После move a всегда пустая».

Это часто бывает так, но полагаться на конкретное состояние нельзя, если документация конкретного типа этого не гарантирует.

### 13. Move semantics и unique_ptr

Здесь move semantics особенно очевидна.

```cpp
auto p1 = std::make_unique<int>(42);
```

`unique_ptr` нельзя копировать:

```cpp
auto p2 = p1; // ERROR
```

Потому что тогда было бы два владельца.

Но можно переместить:

```cpp
auto p2 = std::move(p1);
```

Получаем:

До:

```text
p1 ─────→ [42]


После:

p1 ─────→ nullptr

p2 ─────→ [42]
```

То есть:

move передал ownership.

Это очень важная связь с тем, что мы уже разбирали про RAII и unique_ptr.

### 14. А shared_ptr?

С shared_ptr ситуация интереснее.

```cpp
auto p1 = std::make_shared<int>(42);

auto p2 = p1;
```

Это copy:

```text
p1 ──┐
     ├──→ [42]
p2 ──┘
```

reference count увеличился.

А:

```cpp
auto p2 = std::move(p1);
```

передаёт сам shared_ptr без необходимости увеличивать/уменьшать ownership так, как при обычном копировании.

Упрощённо:

До:

```text
p1 ──→ control block ←→ object


После move:

p1 ──→ nullptr

p2 ──→ control block ←→ object
```

### 15. Почему move особенно полезен с контейнерами?

Например:

```cpp
std::vector<std::string> names;

names.push_back("Alice");
```

Когда vector расширяется, ему иногда приходится выделить новый большой буфер и перенести элементы.

Если тип поддерживает эффективный move:

```text
старый vector

[string A]
[string B]
[string C]

        MOVE

          ↓

новый vector

[string A]
[string B]
[string C]
```

Для объектов, владеющих большими ресурсами, это может быть значительно дешевле копирования.

### 16. noexcept и move constructor

Очень важный момент для собеседования.

Move constructor часто объявляют:

```cpp
Buffer(Buffer&& other) noexcept;
```

Почему noexcept важен?

Например, std::vector при reallocation должен решить:

- копировать элементы?
- или перемещать?

Если move constructor может бросить исключение, контейнеру иногда безопаснее использовать copy constructor, если он доступен.

Поэтому стандартные типы обычно стараются предоставлять noexcept move operations.

Например:

```cpp
std::vector<T>
```

может использовать move более охотно, если он noexcept.

### 17. Move не всегда быстрее copy

Важно не воспринимать:

`move = всегда быстро`

как абсолютную истину.

Например:

```cpp
int x = 10;
int y = std::move(x);
```

Здесь никакого особенного выигрыша нет.

Для маленьких типов:

- int
- double
- char

копирование и так дешёвое.

Move особенно полезен для объектов, которые владеют ресурсами:

- std::string
- std::vector
- std::unique_ptr
- std::fstream

и пользовательских resource-owning классов.

### 18. Связь с std::forward

Мы уже разбирали perfect forwarding:

```cpp
template <typename T>
void wrapper(T&& value)
{
    foo(std::forward<T>(value));
}
```

Теперь разница становится гораздо понятнее:

**std::move**

```cpp
std::move(x)
```

говорит:

Я хочу рассматривать x как объект, из которого можно перемещать.

**std::forward**

```cpp
std::forward<T>(x)
```

говорит:

Сохрани исходную категорию аргумента.

То есть:

```text
std::move
    ↓
принудительно превращает в xvalue


std::forward
    ↓
сохраняет исходную value category
```

### 19. Важный момент: std::move может привести к неожиданному результату

Например:

```cpp
std::string a = "Hello";

std::string b = std::move(a);

std::cout << a;
```

Не стоит рассчитывать, что здесь будет "Hello".

После:

```cpp
std::move(a)
```

`a` был использован как источник для move.

Он всё ещё валидный объект, но его содержимое находится в unspecified state.

### 20. Как думать о move semantics

Полезная ментальная модель:

**Copy**

«Создай мне ещё один такой же ресурс».

```text
A ──→ Resource 1

B ──→ Resource 2
```

**Move**

«Мне больше не нужен мой ресурс — забери его себе».

```text
A ──→ nothing

B ──→ Resource 1
```

Именно поэтому move semantics отлично сочетается с ownership.

### 🧠 Шпаргалка для собеседования

| Понятие | Смысл |
|---------|-------|
| `T&&` | rvalue reference |
| Move constructor | создаёт объект, забирая ресурс у другого |
| Move assignment | передаёт ресурс существующему объекту |
| `std::move(x)` | превращает x в xvalue |
| `std::forward<T>(x)` | сохраняет исходную value category |
| `noexcept` | важно для эффективного move в контейнерах |
| moved-from object | остаётся valid, но state обычно unspecified |
| Copy | дублирует ресурс |
| Move | передаёт ресурс |

Самая важная цепочка:

```text
        Move semantics
              │
      ┌───────┴────────┐
      ↓                ↓
  rvalue (&&)      ownership
      │                │
      ↓                ↓
 std::move()      transfer resource
      │                │
      └───────┬────────┘
              ↓
      move constructor
      move assignment
```

И если свести всё к одной фразе:

Move semantics позволяет не копировать ресурс, а передавать его от одного объекта другому, когда исходный объект больше не нужен.

## 2. Rule of 0/3/5

Rule of 0/3/5 — это очень важное правило Modern C++, которое напрямую связано с RAII, ownership и move semantics.

Главная идея:

Если класс управляет ресурсом, нужно правильно определить специальные методы. Но в современном C++ лучше вообще не управлять ресурсом вручную — тогда получается Rule of 0.

### 1. Какие специальные методы имеются в виду?

Речь идёт о пяти special member functions:

```cpp
~T();                              // destructor

T(const T&);                       // copy constructor
T& operator=(const T&);            // copy assignment

T(T&&);                            // move constructor
T& operator=(T&&);                 // move assignment
```

Условно:

```text
                 Special members
                       │
        ┌──────────────┼──────────────┐
        ↓              ↓              ↓
     Destructor       Copy           Move
                     /   \           /   \
                   ctor  =          ctor   =
```

И отсюда появляются правила 0 / 3 / 5.

### 2. Rule of 0 — лучший вариант

Rule of 0:

Если класс не управляет ресурсом напрямую, не нужно вручную объявлять ни один из пяти special member functions.

Например:

```cpp
class Person {
private:
    std::string name;
    std::vector<int> scores;

public:
    Person(std::string name)
        : name(std::move(name)) {}
};
```

Здесь мы не пишем:

```cpp
~Person();
Person(const Person&);
Person& operator=(const Person&);
Person(Person&&);
Person& operator=(Person&&);
```

Почему?

Потому что:

```text
Person
 ├── std::string
 └── std::vector
```

сами правильно управляют своими ресурсами.

`std::string` знает, как себя копировать и перемещать.

`std::vector` тоже.

Поэтому компилятор может автоматически сгенерировать подходящие операции для Person.

### 3. Почему это называется Rule of 0?

Потому что ты пишешь:

`0 special member functions`

То есть:

```cpp
class Person {
    std::string name;
    std::vector<int> scores;
};
```

И всё.

Это обычно предпочтительный подход.

### 4. Rule of 3

Теперь представим старый C++-стиль с ручным управлением памятью:

```cpp
class Buffer {
private:
    int* data;
    size_t size;

public:
    Buffer(size_t n)
        : data(new int[n]), size(n) {}

    ~Buffer() {
        delete[] data;
    }
};
```

Проблема возникает при копировании:

```cpp
Buffer a(100);
Buffer b = a;
```

Если компилятор автоматически скопирует data:

```text
a.data ─────┐
            ├──→ [ memory ]
b.data ─────┘
```

Теперь два объекта считают себя владельцами одной памяти.

При уничтожении:

```cpp
a → delete[]
b → delete[]
```

💥 Double delete → UB.

### 5. Поэтому нужен copy constructor

Нужно сделать глубокое копирование:

```cpp
Buffer(const Buffer& other)
    : data(new int[other.size]),
      size(other.size)
{
    std::copy(
        other.data,
        other.data + size,
        data
    );
}
```

Теперь:

```text
a.data ──→ [ memory A ]

b.data ──→ [ memory B ]
```

### 6. Но есть ещё copy assignment

Например:

```cpp
Buffer a(100);
Buffer b(200);

b = a;
```

Это уже не constructor.

`b` уже существует.

Поэтому нужен:

```cpp
Buffer& operator=(const Buffer& other);
```

Например:

```cpp
Buffer& operator=(const Buffer& other)
{
    if (this != &other) {
        delete[] data;

        size = other.size;
        data = new int[size];

        std::copy(
            other.data,
            other.data + size,
            data
        );
    }

    return *this;
}
```

### 7. Отсюда Rule of 3

Если классу пришлось вручную определить один из этих трёх:

1. destructor
2. copy constructor
3. copy assignment operator

то, скорее всего, нужно определить все три.

Это и есть:

Rule of 3

```text
              Rule of 3
                  │
        ┌─────────┼─────────┐
        ↓         ↓         ↓
   destructor   copy ctor   copy =
```

Почему?

Потому что если ты вручную управляешь ресурсом, стандартные copy operations, скорее всего, уже недостаточны.

### 8. Но появился C++11

В C++11 появились:

```cpp
T(T&&);
T& operator=(T&&);
```

То есть move constructor и move assignment.

И появилась:

Rule of 5

Если класс вручную управляет ресурсом и тебе приходится определять special members, нужно учитывать уже пять:

```text
                 Rule of 5
                     │
       ┌─────────────┼─────────────┐
       ↓             ↓             ↓
   Destructor       Copy          Move
                  /     \        /     \
                ctor     =      ctor     =
```

То есть:

```cpp
class Buffer {
public:
    ~Buffer();

    Buffer(const Buffer&);
    Buffer& operator=(const Buffer&);

    Buffer(Buffer&&) noexcept;
    Buffer& operator=(Buffer&&) noexcept;
};
```

### 9. Почему move нужен?

Вернёмся к нашему Buffer.

Допустим:

```cpp
Buffer a(1'000'000);
Buffer b = std::move(a);
```

Без move пришлось бы копировать:

```text
1 000 000 элементов
        ↓
     COPY
        ↓
новый buffer
```

С move можно просто передать владение:

```text
a
 │
 └──→ [ BIG BUFFER ]

        MOVE

b
 │
 └──→ [ BIG BUFFER ]

a.data = nullptr
```

То есть:

`copy` → копируем ресурс
`move` → передаём ресурс

### 10. Полная реализация Rule of 5

Упрощённый пример:

```cpp
class Buffer {
private:
    int* data;
    size_t size;

public:
    // Constructor
    explicit Buffer(size_t n)
        : data(new int[n]), size(n) {}

    // Destructor
    ~Buffer() {
        delete[] data;
    }

    // Copy constructor
    Buffer(const Buffer& other)
        : data(new int[other.size]),
          size(other.size)
    {
        std::copy(
            other.data,
            other.data + size,
            data
        );
    }

    // Copy assignment
    Buffer& operator=(const Buffer& other)
    {
        if (this != &other) {
            delete[] data;

            size = other.size;
            data = new int[size];

            std::copy(
                other.data,
                other.data + size,
                data
            );
        }

        return *this;
    }

    // Move constructor
    Buffer(Buffer&& other) noexcept
        : data(other.data),
          size(other.size)
    {
        other.data = nullptr;
        other.size = 0;
    }

    // Move assignment
    Buffer& operator=(Buffer&& other) noexcept
    {
        if (this != &other) {
            delete[] data;

            data = other.data;
            size = other.size;

            other.data = nullptr;
            other.size = 0;
        }

        return *this;
    }
};
```

Это классический пример Rule of 5.

### 11. Но в Modern C++ мы обычно не пишем такое

И вот здесь самое важное.

Вместо:

```cpp
int* data;
```

используем:

```cpp
std::vector<int> data;
```

Получаем:

```cpp
class Buffer {
private:
    std::vector<int> data;

public:
    explicit Buffer(size_t n)
        : data(n) {}
};
```

И всё.

Не нужны:

```text
destructor          ❌
copy constructor    ❌
copy assignment     ❌
move constructor    ❌
move assignment     ❌
```

Потому что std::vector уже умеет всё это делать.

Это:

Rule of 0

### 12. Связь с RAII

Вот почему мы сначала изучали RAII.

Плохой дизайн:

```text
MyClass
   │
   └── raw pointer
          │
          └── manually manage resource
```

↓

приходится писать:

`Rule of 3 / Rule of 5`

Хороший современный дизайн:

```text
MyClass
   │
   └── std::vector / unique_ptr / string
             │
             └── RAII
```

↓

можно использовать:

`Rule of 0`

То есть:

Rule of 0 — это естественное следствие хорошего RAII-дизайна.

### 13. Очень важная связь с unique_ptr

Например:

```cpp
class User {
private:
    std::unique_ptr<int> data;
};
```

Мы не пишем вручную destructor:

```cpp
~User() {
    delete ...
}
```

`unique_ptr` сам освободит память.

Но есть нюанс: unique_ptr не копируется.

Поэтому:

```cpp
User a;
User b = a;              // ❌
User b = std::move(a);   // ✅
```

Move operation для User может быть сгенерирована автоматически, если остальные условия для этого выполнены.

### 14. Важная ловушка: пользовательский destructor

Здесь начинается то, что часто спрашивают на собеседованиях.

Если написать:

```cpp
class MyClass {
public:
    ~MyClass() {}
};
```

это уже влияет на автоматическую генерацию move operations.

Упрощённо:

```text
написал destructor
        ↓
автоматический move может не появиться
```

Поэтому нельзя просто думать:

«Компилятор всегда автоматически сделает мне move constructor».

Нет.

Правила implicit generation special members довольно строгие.

### 15. = default

Если тебе нужно явно сказать:

«Используй стандартную автоматически сгенерированную реализацию»

можно написать:

```cpp
class MyClass {
public:
    MyClass() = default;
    ~MyClass() = default;

    MyClass(const MyClass&) = default;
    MyClass& operator=(const MyClass&) = default;

    MyClass(MyClass&&) = default;
    MyClass& operator=(MyClass&&) = default;
};
```

Но в обычном Rule-of-0 классе чаще вообще ничего писать не нужно.

### 16. = delete

А иногда наоборот нужно запретить операцию.

Например, unique_ptr нельзя копировать:

```cpp
class UniqueResource {
public:
    UniqueResource(const UniqueResource&) = delete;
    UniqueResource& operator=(const UniqueResource&) = delete;

    UniqueResource(UniqueResource&&) = default;
    UniqueResource& operator=(UniqueResource&&) = default;
};
```

То есть:

```text
copy → ❌
move → ✅
```

### 17. Как запомнить 0 / 3 / 5

Очень простая схема:

```text
                 Управляешь ресурсом вручную?
                           │
                    ┌──────┴──────┐
                   НЕТ            ДА
                    │              │
                    ▼              ▼
              Rule of 0      Нужно думать
                                   │
                            ┌──────┴──────┐
                            ↓             ↓
                         C++98         C++11+
                            │             │
                            ▼             ▼
                      Rule of 3       Rule of 5
```

**Rule of 0**

Используй RAII-типы:

- std::string
- std::vector
- std::unique_ptr
- std::shared_ptr

и не пиши special members.

**Rule of 3**

Если пишешь вручную:

```cpp
~T()
T(const T&)
operator=(const T&)
```

нужно рассмотреть все три.

**Rule of 5**

В Modern C++ добавляются:

```cpp
T(T&&)
operator=(T&&)
```

### 🧠 Для собеседования

Если спросят:

«Что такое Rule of 0/3/5?»

Хороший короткий ответ:

- **Rule of 0** — если класс не управляет ресурсом напрямую, лучше не определять special member functions и использовать RAII-типы.
- **Rule of 3** — если класс вручную управляет ресурсом и определяет destructor, copy constructor или copy assignment, обычно нужно определить все три.
- **Rule of 5** — в C++11 к этим трём добавились move constructor и move assignment, поэтому resource-owning класс обычно должен учитывать все пять.

И самая полезная мысль:

Rule of 5 — уметь написать.
Rule of 0 — стремиться использовать.

То есть в Modern C++ хороший ответ чаще выглядит не как огромный Buffer с пятью методами, а как:

```cpp
class Buffer {
    std::vector<int> data;
};
```

RAII → Rule of 0 → меньше ручного управления → меньше ошибок.

## 3. Lambda expressions

Lambda expression — это способ создать небольшую функцию прямо в месте её использования.

Вместо:

```cpp
bool isEven(int x) {
    return x % 2 == 0;
}
```

можно написать:

```cpp
[](int x) {
    return x % 2 == 0;
}
```

Такая функция называется лямбдой.

### 1. Базовый синтаксис

Общий вид:

```cpp
[capture](parameters) -> return_type {
    // body
}
```

Например:

```cpp
[](int x) -> bool {
    return x > 10;
}
```

Разберём:

```text
[]        → capture list
(int x)   → параметры
-> bool   → возвращаемый тип
{ ... }   → тело
```

Возвращаемый тип часто можно не писать:

```cpp
[](int x) {
    return x > 10;
}
```

Компилятор сам выведет bool.

### 2. Зачем вообще нужны lambda?

Очень часто нам нужна маленькая функция только в одном месте.

Например, отсортировать числа:

```cpp
std::vector<int> v = {5, 1, 4, 2, 3};

std::sort(v.begin(), v.end());
```

А если хотим сортировать по убыванию:

```cpp
std::sort(
    v.begin(),
    v.end(),
    [](int a, int b) {
        return a > b;
    }
);
```

Вместо создания отдельной функции:

```cpp
bool greater(int a, int b) {
    return a > b;
}
```

мы просто написали функцию там, где она нужна.

### 3. Lambda можно сохранить в переменную

Например:

```cpp
auto square = [](int x) {
    return x * x;
};
```

Теперь:

```cpp
std::cout << square(5);
```

Получим:

```text
25
```

То есть:

```text
lambda
   ↓
объект
   ↓
square
   ↓
square(5)
```

И здесь есть важный момент:

Lambda в C++ — это фактически объект некоторого уникального безымянного типа, у которого есть operator().

### 4. Lambda — это объект

Например:

```cpp
auto add = [](int a, int b) {
    return a + b;
};
```

Концептуально компилятор создаёт что-то вроде:

```cpp
class SomeUniqueType {
public:
    int operator()(int a, int b) const {
        return a + b;
    }
};
```

И:

```cpp
auto add = [](int a, int b) {
    return a + b;
};
```

примерно соответствует:

```cpp
SomeUniqueType add;
```

А:

```cpp
add(2, 3);
```

вызывает:

```cpp
add.operator()(2, 3);
```

Конкретное имя и детали типа компилятор скрывает.

### 5. Самая важная часть — capture

Теперь представим:

```cpp
int multiplier = 10;

auto multiply = [](int x) {
    return x * multiplier; // ERROR
};
```

Lambda не видит локальную переменную multiplier автоматически.

Нужно сказать:

«Захвати эту переменную».

```cpp
int multiplier = 10;

auto multiply = [multiplier](int x) {
    return x * multiplier;
};
```

Теперь:

```cpp
multiply(5);
```

→ 50.

### 6. [ ] — capture list

Именно квадратные скобки:

```cpp
[multiplier]
```

определяют, какие внешние переменные lambda захватывает.

Основные варианты:

```text
[]          ничего не захватывать
[x]         захватить x по значению
[&x]        захватить x по ссылке
[=]         все используемые внешние переменные по значению
[&]         все используемые внешние переменные по ссылке
```

### 7. Захват по значению

```cpp
int x = 10;

auto lambda = [x]() {
    std::cout << x;
};
```

Lambda получает свою копию x.

Условно:

```text
x в функции
   │
   │ copy
   ▼
lambda object
   │
   └── x = 10
```

Если потом:

```cpp
x = 20;
```

то внутри lambda по-прежнему:

```text
10
```

### 8. Захват по ссылке

```cpp
int x = 10;

auto lambda = [&x]() {
    std::cout << x;
};
```

Теперь lambda хранит ссылку на существующий x.

```text
lambda
   │
   └────────→ x
```

Поэтому:

```cpp
x = 20;
lambda();
```

выведет:

```text
20
```

И lambda может изменить x:

```cpp
auto lambda = [&x]() {
    x = 100;
};
```

### 9. [=] и [&]

Вместо перечисления переменных можно написать:

```cpp
[=]
```

Все используемые внешние переменные захватываются по значению.

Например:

```cpp
int a = 10;
int b = 20;

auto lambda = [=]() {
    return a + b;
};
```

Или:

```cpp
[&]
```

Все используемые внешние переменные захватываются по ссылке:

```cpp
int a = 10;
int b = 20;

auto lambda = [&]() {
    a = 100;
    b = 200;
};
```

### 10. Очень важный момент: const

Посмотри:

```cpp
int x = 10;

auto lambda = [x]() {
    x = 20; // ERROR
};
```

Почему?

Потому что lambda по умолчанию имеет operator() const.

То есть захваченная по значению переменная внутри lambda фактически недоступна для изменения.

Если нужно изменить копию, используется:

```cpp
mutable
```

```cpp
int x = 10;

auto lambda = [x]() mutable {
    x = 20;
    std::cout << x;
};
```

После вызова:

```cpp
lambda();
```

внутри lambda будет 20.

Но внешний x всё ещё:

```text
10
```

Потому что это была копия.

### 11. [&] и lifetime

Здесь появляется важная связь с тем, что мы изучали про dangling references.

Например:

```cpp
std::function<int()> createLambda()
{
    int x = 42;

    return [&x]() {
        return x;
    };
}
```

После выхода из createLambda():

```text
x
↓
уничтожен
```

А lambda всё ещё содержит ссылку на него:

```text
lambda ─────→ x ❌
              dangling reference
```

Вызов:

```cpp
auto f = createLambda();
f(); // UB
```

Поэтому при захвате по ссылке нужно следить за lifetime объекта.

### 12. Lambda и std::sort

Один из самых частых вариантов использования:

```cpp
std::vector<int> numbers = {
    10, 2, 50, 3, 1
};

std::sort(
    numbers.begin(),
    numbers.end(),
    [](int a, int b) {
        return a < b;
    }
);
```

Lambda здесь является comparator.

sort вызывает её примерно так:

```text
sort
 │
 ├── lambda(10, 2)
 ├── lambda(2, 50)
 ├── lambda(50, 3)
 ├── ...
```

### 13. Lambda и std::find_if

Например, найти первое чётное число:

```cpp
std::vector<int> numbers = {
    1, 3, 7, 8, 11
};

auto it = std::find_if(
    numbers.begin(),
    numbers.end(),
    [](int x) {
        return x % 2 == 0;
    }
);
```

Получим iterator на:

```text
8
```

Здесь lambda выступает как predicate — функция, возвращающая условие.

### 14. Lambda с захватом

Например:

```cpp
int minValue = 10;

auto isGreater = [minValue](int x) {
    return x > minValue;
};
```

Можно использовать:

```cpp
std::find_if(
    numbers.begin(),
    numbers.end(),
    isGreater
);
```

Здесь особенно хорошо видно преимущество lambda:

```text
          minValue
             │
             ▼
lambda ─────────────→ predicate
```

Lambda может «нести с собой» состояние.

### 15. Lambda без capture — почти как обычная функция

Например:

```cpp
auto square = [](int x) {
    return x * x;
};
```

Она ничего не захватывает:

```cpp
[]
```

Такая lambda может преобразовываться в обычный function pointer:

```cpp
int (*func)(int) = [](int x) {
    return x * x;
};
```

Но lambda с capture:

```cpp
int multiplier = 10;

auto f = [multiplier](int x) {
    return x * multiplier;
};
```

обычно уже не может быть преобразована в обычный function pointer.

Потому что ей нужно хранить состояние:

```text
lambda object
 ├── multiplier = 10
 └── operator()
```

### 16. Lambda + std::function

Можно хранить lambda в:

```cpp
std::function
```

Например:

```cpp
std::function<int(int)> square =
    [](int x) {
        return x * x;
    };
```

Теперь:

```cpp
std::cout << square(5);
```

→ 25.

Но std::function — это уже type-erasure-обёртка и может иметь дополнительные runtime/инфраструктурные расходы.

Поэтому:

```cpp
auto lambda = ...;
```

часто предпочтительнее, если конкретный тип можно вывести.

### 17. Generic lambda

В Modern C++ можно сделать lambda с:

```cpp
auto
```

в параметрах:

```cpp
auto print = [](const auto& value) {
    std::cout << value << '\n';
};
```

Теперь:

```cpp
print(42);
print(3.14);
print(std::string("Hello"));
```

Одна lambda работает с разными типами.

Концептуально это похоже на шаблон:

```cpp
template <typename T>
void print(const T& value) {
    std::cout << value << '\n';
}
```

То есть generic lambda фактически позволяет получить templated operator().

### 18. C++20: template lambda

Можно писать ещё явнее:

```cpp
auto print = []<typename T>(const T& value) {
    std::cout << value << '\n';
};
```

Это уже явно template-параметр lambda.

### 19. Init capture

Modern C++ позволяет захватывать выражение:

```cpp
auto ptr = std::make_unique<int>(42);

auto lambda = [p = std::move(ptr)]() {
    std::cout << *p;
};
```

Это очень интересный пример.

Мы уже знаем:

```cpp
std::move(ptr)
```

передаёт ownership.

Получаем:

```text
до:

ptr ─────→ [42]


после:

ptr ─────→ nullptr

lambda
  │
  └────→ [42]
```

То есть lambda сама стала владельцем ресурса.

Это часто используется для асинхронного кода и callback'ов.

### 20. Lambda и this

В методе класса:

```cpp
class User {
private:
    int age = 30;

public:
    void foo() {
        auto lambda = [this]() {
            std::cout << age;
        };

        lambda();
    }
};
```

`[this]` захватывает указатель this.

Концептуально:

```text
lambda
  │
  └──→ this
          │
          └──→ User
```

Поэтому внутри lambda можно обращаться к полям:

```cpp
age
```

или явно:

```cpp
this->age
```

### 21. В C++20 можно захватить *this

Есть разница между:

```cpp
[this]
```

и:

```cpp
[*this]
```

`[this]` захватывает указатель.

`[*this]` — копию объекта.

Это особенно важно для lifetime.

Условно:

```text
[this]

lambda ──→ original object


[*this]

lambda ──→ copy of object
```

### 22. Lambda — это не просто «анонимная функция»

Это хорошая упрощённая формулировка, но технически точнее:

Lambda expression создаёт объект closure-типа, у которого есть operator().

Например:

```cpp
int x = 10;

auto f = [x](int y) {
    return x + y;
};
```

Условно:

```cpp
class Closure {
    int x;

public:
    int operator()(int y) const {
        return x + y;
    }
};
```

То есть capture превращается в данные объекта lambda.

Это очень полезно понимать.

### 🧠 Главное для собеседования

Запомни синтаксис:

```cpp
[capture](parameters) -> return_type {
    body
};
```

Например:

```cpp
[x, &y](int value) {
    return x + y + value;
};
```

**Capture**

| Запись | Значение |
|--------|----------|
| `[]` | ничего |
| `[x]` | x по значению |
| `[&x]` | x по ссылке |
| `[=]` | используемые переменные по значению |
| `[&]` | используемые переменные по ссылке |
| `[this]` | указатель this |
| `[*this]` | копия объекта |
| `[p = std::move(ptr)]` | init capture |

**Что важно помнить**

```text
Lambda
  │
  ├── capture → состояние
  │
  ├── parameters → входные данные
  │
  └── operator() → поведение
```

И особенно полезно связать Lambda с предыдущими темами:

```text
Lambda
  │
  ├── capture by value
  │       ↓
  │     copy
  │
  ├── capture by reference
  │       ↓
  │     lifetime / dangling reference
  │
  ├── [p = std::move(ptr)]
  │       ↓
  │     move semantics / ownership
  │
  └── generic lambda
          ↓
        templates
```

Самая короткая формулировка для собеседования:

Lambda expression — это синтаксис для создания callable-объекта с operator(), который может захватывать внешние переменные по значению или по ссылке. Чаще всего lambda используется как локальный callback, comparator или predicate.

## 4. `auto`, `decltype`

`auto` и `decltype` в Modern C++ оба связаны с выводом типов, но делают это по-разному:

- `auto` — выводит тип переменной из инициализатора.
- `decltype` — определяет тип выражения по специальным правилам.

### 1. auto

Самый простой пример:

```cpp
auto x = 42;
```

Компилятор понимает:

```cpp
int x = 42;
```

Другие примеры:

```cpp
auto a = 3.14;       // double
auto b = 'A';        // char
auto c = true;       // bool
auto d = "Hello";    // const char*
```

То есть:

```text
auto x = expression;
          ↓
      определить тип
          ↓
          T
```

### 2. Зачем нужен auto?

Особенно полезен для сложных типов.

Например:

```cpp
std::vector<std::pair<std::string, int>> users;
```

Итератор может иметь очень длинный тип:

```cpp
std::vector<std::pair<std::string, int>>::iterator it =
    users.begin();
```

С auto:

```cpp
auto it = users.begin();
```

Гораздо удобнее.

### 3. auto выводит тип примерно как template deduction

Очень полезная связь:

```cpp
auto x = value;
```

по правилам вывода типа во многом похоже на:

```cpp
template <typename T>
void foo(T x);
```

Например:

```cpp
const int value = 42;

auto x = value;
```

Получим:

```cpp
int x;
```

`const` здесь не сохраняется.

Почему?

Потому что auto при таком выводе ведёт себя примерно как вывод параметра по значению.

### 4. const auto

Если хотим сохранить const:

```cpp
const int value = 42;

const auto x = value;
```

Теперь:

```text
x → const int
```

### 5. auto&

А если нужна ссылка:

```cpp
int x = 42;

auto& ref = x;
```

Получаем:

```text
ref → int&
```

Это особенно важно при работе с контейнерами.

```cpp
std::vector<int> numbers = {1, 2, 3};

for (auto& x : numbers) {
    x *= 2;
}
```

Здесь x — ссылка на настоящий элемент vector.

### 6. const auto&

Очень распространённый вариант:

```cpp
for (const auto& user : users) {
    std::cout << user.name;
}
```

Здесь:

```text
const → нельзя менять
&     → не копировать
auto  → вывести тип
```

То есть:

«Возьми элемент по константной ссылке, а тип выведи автоматически».

### 7. auto&&

Здесь появляется связь с perfect forwarding, который мы уже разбирали.

```cpp
auto&& x = something;
```

В зависимости от того, что такое something, auto&& может стать:

```text
lvalue → T&
rvalue → T&&
```

Например:

```cpp
int a = 10;

auto&& x = a;        // int&
auto&& y = 20;       // int&&
```

Это называется forwarding reference в соответствующем контексте.

### 8. Теперь decltype

`decltype` тоже выводит тип, но идея другая:

```cpp
decltype(expression)
```

означает:

«Какой тип имеет это выражение?»

Например:

```cpp
int x = 42;

decltype(x) y = 10;
```

Получаем:

```cpp
int y = 10;
```

### 9. Зачем нужен decltype?

Он особенно полезен, когда тип зависит от выражения.

Например:

```cpp
auto x = 42;

decltype(x) y = 100;
```

Если тип x изменится:

```cpp
auto x = 3.14;
```

то decltype(x) автоматически станет:

```cpp
double
```

### 10. Самая важная особенность decltype

Вот здесь часто ошибаются.

```cpp
int x = 42;

decltype(x) a;
decltype((x)) b;
```

На первый взгляд:

```cpp
decltype(x)
decltype((x))
```

кажется одним и тем же.

Но это разные вещи.

Получаем:

```text
decltype(x)   → int
decltype((x)) → int&
```

Почему?

Потому что у decltype есть специальные правила для выражений.

### 11. Правила decltype

Если выражение — это неквалифицированное имя переменной, функции, члена класса и т.п., то:

```cpp
decltype(x)
```

даёт объявленный тип x.

Например:

```cpp
const int x = 42;

decltype(x) y = 10;
```

Получим:

```text
const int
```

Но если это обычное выражение, decltype смотрит на его value category.

И тут вспоминаем:

```text
lvalue
xvalue
prvalue
```

Правила:

```text
expression → lvalue
    ↓
decltype(expression) = T&

expression → xvalue
    ↓
decltype(expression) = T&&

expression → prvalue
    ↓
decltype(expression) = T
```

### 12. Пример

```cpp
int x = 42;
x
```

`x` — lvalue.

Но `x` — специальный случай, потому что это имя переменной:

```cpp
decltype(x)
```

→ int.

```cpp
(x)
```

`(x)` — уже обычное выражение, которое является lvalue:

```cpp
decltype((x))
```

→ int&.

Поэтому:

```cpp
decltype(x) a = 10;      // int
decltype((x)) b = x;      // int&
```

### 13. Очень полезная таблица

Для:

```cpp
int x = 42;
```

получим:

| Выражение | Value category | decltype |
|-----------|----------------|----------|
| `x` | lvalue | int |
| `(x)` | lvalue | int& |
| `x + 1` | prvalue | int |
| `std::move(x)` | xvalue | int&& |

Последняя строка особенно важна:

```cpp
decltype(std::move(x))
```

→ int&&.

### 14. auto vs decltype

Теперь можно сравнить напрямую.

```cpp
int x = 42;

auto a = x;
decltype(x) b = x;
```

Оба:

```text
int
```

Но механизм разный.

**auto**

```cpp
auto a = expression;
```

→ выводит тип инициализатора, примерно как template deduction.

**decltype**

```cpp
decltype(expression)
```

→ определяет тип самого выражения по специальным правилам.

### 15. Главный пример различия

```cpp
int x = 42;

auto a = (x);
decltype((x)) b = x;
```

Получаем:

```text
a → int
b → int&
```

Почему?

`auto` здесь не сохраняет ссылочность выражения.

А `decltype((x))` учитывает, что `(x)` — lvalue.

### 16. auto и ссылки

Посмотри:

```cpp
int x = 42;

auto a = x;
auto& b = x;
```

Получаем:

```text
a → int
b → int&
```

То есть auto сам по себе не делает ссылку.

Если нужна ссылка:

```cpp
auto&
```

Если нужна const-ссылка:

```cpp
const auto&
```

### 17. decltype(auto)

А вот здесь оба механизма встречаются вместе.

C++11 добавил:

```cpp
decltype(auto)
```

Он означает:

«Выведи тип с использованием правил decltype».

Например:

```cpp
int x = 42;

decltype(auto) a = x;
decltype(auto) b = (x);
```

Получим:

```text
a → int
b → int&
```

Это отличается от:

```cpp
auto a = (x);
```

где будет:

```text
a → int
```

### 18. Почему decltype(auto) особенно важен для функций

Например:

```cpp
int x = 42;

decltype(auto) getValue() {
    return (x);
}
```

Возвращаемый тип:

```text
int&
```

Потому что:

```cpp
return (x);
```

— lvalue.

Если написать:

```cpp
auto getValue() {
    return (x);
}
```

получим:

```text
int
```

То есть ссылка потеряется.

### 19. Очень важная ловушка

Сравни:

```cpp
auto get() {
    return x;
}
```

и:

```cpp
decltype(auto) get() {
    return x;
}
```

Если x — обычная переменная int, оба дадут:

```text
int
```

Но:

```cpp
decltype(auto) get() {
    return (x);
}
```

даст:

```text
int&
```

Поэтому скобки здесь могут быть критически важны.

### 20. Связь с perfect forwarding

Мы уже видели:

```cpp
template <typename T>
void wrapper(T&& value) {
    foo(std::forward<T>(value));
}
```

decltype часто используется в generic code для получения типа выражения.

Например:

```cpp
decltype(value)
```

может сохранить информацию о ссылочности и value category в местах, где это нужно.

А ещё есть очень популярный паттерн:

```cpp
decltype(auto)
```

когда нужно вернуть результат другого выражения без потери его точного типа.

### 21. decltype с функцией

Можно получить тип функции:

```cpp
int foo(double);

decltype(foo)
```

Это будет тип функции:

```text
int(double)
```

А:

```cpp
decltype(&foo)
```

будет:

```text
int(*)(double)
```

То есть:

```text
foo
 ↓
тип функции

&foo
 ↓
указатель на функцию
```

### 22. auto в lambda

Мы уже видели:

```cpp
auto square = [](int x) {
    return x * x;
};
```

Здесь auto особенно полезен, потому что тип lambda уникальный и фактически безымянный.

Нельзя написать:

```cpp
SomeLambdaType square = ...; // такого обычного имени типа нет
```

Поэтому:

```cpp
auto square = ...
```

идеально подходит.

### 23. auto в structured bindings

Modern C++:

```cpp
std::pair<int, std::string> user{42, "Alice"};

auto [id, name] = user;
```

Получаем:

```text
id   → 42
name → "Alice"
```

Можно использовать:

```cpp
const auto& [id, name] = user;
```

Тогда элементы не копируются и не изменяются.

### 24. Как запомнить

Самая полезная ментальная модель:

```text
auto
 │
 └── «Какой тип мне создать для этой переменной?»

decltype
 │
 └── «Какой тип у этого выражения?»
```

И ещё:

```text
auto
 ↓
type deduction


decltype
 ↓
expression type
 + value category
```

### 🧠 Шпаргалка для собеседования

**auto**

```cpp
auto x = expr;
```

Выводит тип из инициализатора.

```cpp
auto x = 42;       // int
auto& x = value;   // T&
const auto& x = value;
```

**decltype**

```cpp
decltype(expr)
```

Получает тип выражения по специальным правилам.

```cpp
int x;

decltype(x)    // int
decltype((x))  // int&
```

Для value categories:

```text
lvalue  → T&
xvalue  → T&&
prvalue → T
```

Но: неквалифицированное имя (`decltype(x)`) — специальный случай и даёт объявленный тип x.

**decltype(auto)**

```cpp
decltype(auto) x = expr;
```

Использует правила decltype, а не обычные правила auto.

Особенно полезно:

```cpp
decltype(auto) get() {
    return (some_expression);
}
```

когда важно сохранить `T`, `T&` или `T&&`.

**Итоговая схема**

```text
                    Type deduction
                         │
                ┌────────┴────────┐
                ↓                 ↓
              auto             decltype
                │                 │
        «тип переменной»    «тип выражения»
                │                 │
                ↓                 ↓
        template deduction    value category
                                  │
                         ┌────────┼────────┐
                         ↓        ↓        ↓
                       lvalue   xvalue   prvalue
                         ↓        ↓        ↓
                        T&       T&&       T
```

Для собеседования главное не перепутать auto и decltype:

auto выводит тип переменной из initializer и обычно отбрасывает top-level const и references. decltype определяет тип выражения по специальным правилам и может сохранить &/&&. decltype(auto) использует именно правила decltype.









