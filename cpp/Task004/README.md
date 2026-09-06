# Task 004 — Search in Rotated Sorted Array (Поиск в циклически сдвинутом массиве)

## Условие

Дан отсортированный по возрастанию массив **различных** целых чисел `nums`, который
был **циклически сдвинут** на неизвестное число позиций вправо. Например,
`[0, 1, 2, 4, 5, 6, 7]` после сдвига может превратиться в
`[4, 5, 6, 7, 0, 1, 2]`.

Дано целевое число `target`. Верните индекс `target` в массиве, либо `-1`, если
элемент отсутствует.

### Пример

```
Вход:  nums = [4,5,6,7,0,1,2], target = 0   -> 4
Вход:  nums = [4,5,6,7,0,1,2], target = 3   -> -1
Вход:  nums = [1],            target = 0    -> -1
```

## Требования

* Реализуйте решение за **O(log n)** по времени (модифицированный бинарный поиск).
* Используйте только **O(1)** дополнительной памяти.

## Стартовый шаблон (C++)

```cpp
#include <vector>

// Возвращает индекс target или -1, если элемент не найден.
int search(const std::vector<int>& nums, int target) {
    int left = 0;
    int right = static_cast<int>(nums.size()) - 1;

    // TODO: реализуйте модифицированный бинарный поиск

    return -1;
}
```

## Базовые тесты (Boost.Test)

```cpp
#define BOOST_TEST_MODULE RotatedSearchTest
#include <boost/test/unit_test.hpp>

#include <vector>

int search(const std::vector<int>& nums, int target);

BOOST_AUTO_TEST_CASE(found_in_rotated) {
    BOOST_CHECK_EQUAL(search({4, 5, 6, 7, 0, 1, 2}, 0), 4);
    BOOST_CHECK_EQUAL(search({4, 5, 6, 7, 0, 1, 2}, 4), 0);
    BOOST_CHECK_EQUAL(search({4, 5, 6, 7, 0, 1, 2}, 2), 6);
}

BOOST_AUTO_TEST_CASE(not_found) {
    BOOST_CHECK_EQUAL(search({4, 5, 6, 7, 0, 1, 2}, 3), -1);
    BOOST_CHECK_EQUAL(search({1}, 0), -1);
}

BOOST_AUTO_TEST_CASE(not_rotated) {
    BOOST_CHECK_EQUAL(search({0, 1, 2, 3, 4, 5}, 3), 3);
    BOOST_CHECK_EQUAL(search({0, 1, 2, 3, 4, 5}, 6), -1);
}
```
