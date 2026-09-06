# Task 001 — Two Sum (Поиск пары элементов, дающих заданную сумму)

## Условие

Дан массив целых чисел `nums` и целевое число `target`. Найдите **индексы** двух
различных элементов массива, сумма которых равна `target`.

Каждый набор входных данных гарантированно имеет **ровно одно** решение, и один и
тот же элемент нельзя использовать дважды. Индексы можно вернуть в любом порядке.

### Пример

```
Вход:  nums = [2, 7, 11, 15], target = 9
Выход: [0, 1]  (так как nums[0] + nums[1] == 9)
```

```
Вход:  nums = [3, 2, 4], target = 6
Выход: [1, 2]
```

## Требования

* Реализуйте функцию `twoSum` со сложностью по времени **O(n)**.
* Используйте хеш-таблицу (`std::unordered_map`) для хранения уже просмотренных
  значений и их индексов.

## Стартовый шаблон (C++)

```cpp
#include <vector>
#include <unordered_map>

// Возвращает пару индексов элементов, сумма которых равна target.
std::vector<int> twoSum(const std::vector<int>& nums, int target) {
    std::unordered_map<int, int> seen; // значение -> индекс

    // TODO: реализуйте поиск за O(n)

    return {};
}
```

## Базовые тесты (Boost.Test)

```cpp
#define BOOST_TEST_MODULE TwoSumTest
#include <boost/test/unit_test.hpp>

#include <vector>
#include <algorithm>

std::vector<int> twoSum(const std::vector<int>& nums, int target);

BOOST_AUTO_TEST_CASE(basic_cases) {
    BOOST_CHECK(twoSum({2, 7, 11, 15}, 9) == std::vector<int>({0, 1}));
    BOOST_CHECK(twoSum({3, 2, 4}, 6) == std::vector<int>({1, 2}));
    BOOST_CHECK(twoSum({3, 3}, 6) == std::vector<int>({0, 1}));
}

BOOST_AUTO_TEST_CASE(order_does_not_matter) {
    // Индексы могут быть возвращены в любом порядке.
    auto result = twoSum({2, 7, 11, 15}, 9);
    BOOST_REQUIRE_EQUAL(result.size(), 2u);
    std::sort(result.begin(), result.end());
    BOOST_CHECK_EQUAL(result[0], 0);
    BOOST_CHECK_EQUAL(result[1], 1);
}

BOOST_AUTO_TEST_CASE(negative_numbers) {
    auto result = twoSum({-3, 4, 3, 90}, 0);
    BOOST_REQUIRE_EQUAL(result.size(), 2u);
    std::sort(result.begin(), result.end());
    BOOST_CHECK_EQUAL(result[0], 0);
    BOOST_CHECK_EQUAL(result[1], 2);
}
```
