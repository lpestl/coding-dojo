## 1. Tasks

1. [001 - Two Sum (Поиск пары элементов, дающих заданную сумму)](Task001/README.md)
1. [002 - Reverse Linked List (Разворот односвязного списка)](Task002/README.md)
1. [003 - Valid Parentheses (Проверка корректности скобочной последовательности)](Task003/README.md)
1. [004 - Search in Rotated Sorted Array (Поиск в циклически сдвинутом массиве)](Task004/README.md)
1. [005 - LRU Cache (Кэш с вытеснением давно не использовавшихся элементов)](Task005/README.md)

## 2. Questions

### 2.1 C++ basics

- [Чем `class` отличается от `struct`?](CppBasic.md#1-чем-class-отличается-от-struct)
- [Что такое `const`, `constexpr`, `consteval`?](CppBasic.md#2-что-такое-const-constexpr-consteval)
- [Что такое `static` в разных контекстах?](CppBasic.md#3-что-такое-static-в-разных-контекстах)
- [В чём разница между указателем и ссылкой?](CppBasic.md#4-в-чём-разница-между-указателем-и-ссылкой)
- [Что такое `nullptr` и чем он лучше `NULL`?](CppBasic.md#5-что-такое-nullptr-и-чем-он-лучше-null)
- [Что такое `lvalue`, `rvalue`, `xvalue`?](CppBasic.md#6-что-такое-lvalue-rvalue-xvalue)
- [Что такое `std::move` и что он реально делает?](CppBasic.md#8-почему-stdmove-не-перемещает-сам-по-себе)
- [Что такое perfect forwarding?](CppBasic.md#7-что-такое-perfect-forwarding)
- [Чем `std::unique_ptr` отличается от `std::shared_ptr`?](CppBasic.md#8-чем-stdunique_ptr-отличается-от-stdshared_ptr)
- [Когда возникает dangling pointer/reference?](CppBasic.md#9-когда-возникает-dangling-pointerreference)
- [Что такое RAII?](CppBasic.md#10-что-такое-raii)
- [Что такое UB (undefined behavior)? Приведите примеры.](CppBasic.md#11-что-такое-ub-undefined-behavior-приведите-примеры)

### 2.2 ООП

- [Что такое наследование, полиморфизм, инкапсуляция?](OOP.md#1-что-такое-наследование-полиморфизм-инкапсуляция)
- [Зачем нужен виртуальный деструктор?](OOP.md#2-зачем-нужен-виртуальный-деструктор)
- [Чем `virtual` отличается от `override`?](OOP.md#3-чем-virtual-отличается-от-override)
- [Что такое pure virtual function?](OOP.md#4-что-такое-pure-virtual-function)
- [Можно ли вызвать виртуальную функцию из конструктора?](OOP.md#5-можно-ли-вызвать-виртуальную-функцию-из-конструктора)
- [Чем композиция отличается от наследования?](OOP.md#6-чем-композиция-отличается-от-наследования)
- [Что такое object slicing?](OOP.md#7-что-такое-object-slicing)

### 2.3 STL

- [Как устроены `std::vector`, `std::list`, `std::map`, `std::unordered_map`?](STL.md#1-как-устроены-stdvector-stdlist-stdmap-stdunordered_map)
- [Какова сложность основных операций?](STL.md#2-какова-сложность-основных-операций)
- [Когда происходит realloc у `vector`?](STL.md#3-когда-происходит-realloc-у-vector)
- [Что происходит с итераторами после `erase`/`insert`?](STL.md#4-что-происходит-с-итераторами-после-eraseinsert)
- [Чем `std::set` отличается от `std::unordered_set`?](STL.md#5-чем-stdset-отличается-от-stdunordered_set)
- [Как работают хеш-таблицы?](STL.md#6-как-работают-хеш-таблицы)

### 2.4 Modern C++

- Move semantics.
- Rule of 0/3/5.
- Lambda expressions.
- `auto`, `decltype`.
- `std::optional`, `std::variant`, `std::any`.
- Concepts.
- Templates.
- `constexpr`.
- Coroutines.
- Smart pointers.

### 2.5 Многопоточность

- `std::thread`, `std::jthread`.
- `mutex`, `lock_guard`, `unique_lock`.
- `condition_variable`.
- `atomic`.
- Race condition.
- Deadlock.
- Memory ordering.

## 3. Что особенно хорошо выучить перед C++ интервью

Если времени немного, приоритет можно расставить следующим образом.

### Очень важно

1. RAII.
2. `unique_ptr` / `shared_ptr` / `weak_ptr`.
3. Copy/move semantics.
4. Rule of 0/3/5.
5. `lvalue` / `rvalue` / `std::move`.
6. `virtual`, `override`, virtual destructor.
7. STL и алгоритмическая сложность.
8. Templates.
9. `const`, `constexpr`.
10. Exceptions и exception safety.
11. Multithreading.
12. Undefined behavior.

### Для Middle/Senior

- perfect forwarding;
- forwarding references;
- SFINAE;
- concepts;
- type traits;
- CRTP;
- template specialization;
- memory model;
- atomics;
- lock-free programming;
- ABI/vtable;
- cache locality;
- allocator'ы;
- concurrency;
- design patterns и SOLID.

### Как обычно выглядит глубокий вопрос

Очень частый формат собеседования — не просто:

> Что такое move semantics?

а цепочка вопросов:

```text
Что такое std::move?
        ↓
Какие бывают категории value?
        ↓
Что такое rvalue reference?
        ↓
Когда вызывается move constructor?
        ↓
Почему move constructor желательно делать noexcept?
        ↓
Что произойдёт с std::vector,
если move constructor элемента
может бросить исключение?
```

Именно такие цепочки стоит потренировать: они гораздо лучше показывают глубину понимания C++, чем заучивание определений.