# Task 005 — LRU Cache (Кэш с вытеснением давно не использовавшихся элементов)

## Условие

Спроектируйте структуру данных, реализующую кэш с политикой **LRU**
(Least Recently Used — «наиболее давно использованный»), поддерживающую операции
`get` и `put`.

* `get(key)` — возвращает значение по ключу, либо `-1`, если ключ отсутствует.
* `put(key, value)` — записывает значение по ключу. Если ключ уже существует —
  обновляет значение. Если кэш переполнен (превышена ёмкость `capacity`) —
  вытесняется элемент, к которому **дольше всего не обращались**.

Обе операции должны выполняться за **O(1)** в среднем.

### Пример

```
capacity = 2
put(1, 1)
put(2, 2)
get(1)      -> 1
put(3, 3)   // вытесняется ключ 2
get(2)      -> -1
put(4, 4)   // вытесняется ключ 1
get(1)      -> -1
get(3)      -> 3
get(4)      -> 4
```

## Требования

* Используйте комбинацию хеш-таблицы (`std::unordered_map`) и двусвязного списка
  (`std::list`) для достижения константного времени операций.
* Обе операции — **O(1)** в среднем.

## Стартовый шаблон (C++)

```cpp
#include <list>
#include <unordered_map>
#include <utility>

class LRUCache {
public:
    explicit LRUCache(int capacity) : capacity_(capacity) {}

    int get(int key) {
        // TODO: вернуть значение или -1; обновить порядок использования
        return -1;
    }

    void put(int key, int value) {
        // TODO: вставить/обновить значение; вытеснить LRU при переполнении
    }

private:
    int capacity_;
    // Пары (key, value) в порядке использования: front — самый недавний.
    std::list<std::pair<int, int>> items_;
    // key -> итератор на элемент в items_.
    std::unordered_map<int, std::list<std::pair<int, int>>::iterator> lookup_;
};
```

## Базовые тесты (Boost.Test)

```cpp
#define BOOST_TEST_MODULE LRUCacheTest
#include <boost/test/unit_test.hpp>

#include "lru_cache.h" // или определение LRUCache выше

BOOST_AUTO_TEST_CASE(example_flow) {
    LRUCache cache(2);
    cache.put(1, 1);
    cache.put(2, 2);
    BOOST_CHECK_EQUAL(cache.get(1), 1);   // ключ 1 становится недавно использованным
    cache.put(3, 3);                       // вытесняется ключ 2
    BOOST_CHECK_EQUAL(cache.get(2), -1);
    cache.put(4, 4);                       // вытесняется ключ 1
    BOOST_CHECK_EQUAL(cache.get(1), -1);
    BOOST_CHECK_EQUAL(cache.get(3), 3);
    BOOST_CHECK_EQUAL(cache.get(4), 4);
}

BOOST_AUTO_TEST_CASE(update_existing) {
    LRUCache cache(2);
    cache.put(1, 10);
    cache.put(1, 100);                     // обновление, не создаёт новый элемент
    BOOST_CHECK_EQUAL(cache.get(1), 100);
}

BOOST_AUTO_TEST_CASE(single_capacity) {
    LRUCache cache(1);
    cache.put(1, 1);
    cache.put(2, 2);
    BOOST_CHECK_EQUAL(cache.get(1), -1);
    BOOST_CHECK_EQUAL(cache.get(2), 2);
}
```
