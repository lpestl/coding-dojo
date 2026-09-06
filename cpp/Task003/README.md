# Task 003 — Valid Parentheses (Проверка корректности скобочной последовательности)

## Условие

Дана строка `s`, состоящая только из символов `'('`, `')'`, `'{'`, `'}'`, `'['` и
`']'`. Определите, является ли входная строка **корректной** скобочной
последовательностью.

Строка считается корректной, если:

1. Каждая открывающая скобка закрывается скобкой **того же типа**.
2. Скобки закрываются в **правильном порядке** (каждая открывающая скобка имеет
   соответствующую закрывающую, и пары не пересекаются).

### Пример

```
Вход:  "()"        -> true
Вход:  "()[]{}"    -> true
Вход:  "(]"        -> false
Вход:  "([)]"      -> false
Вход:  "{[]}"      -> true
```

## Требования

* Используйте стек (`std::stack`) для хранения открывающих скобок.
* Сложность по времени **O(n)**, по памяти **O(n)**.

## Стартовый шаблон (C++)

```cpp
#include <string>
#include <stack>
#include <unordered_map>

bool isValid(const std::string& s) {
    std::stack<char> st;
    static const std::unordered_map<char, char> matching = {
        {')', '('},
        {']', '['},
        {'}', '{'}
    };

    // TODO: реализуйте проверку корректности

    return st.empty();
}
```

## Базовые тесты (Boost.Test)

```cpp
#define BOOST_TEST_MODULE ValidParenthesesTest
#include <boost/test/unit_test.hpp>

#include <string>

bool isValid(const std::string& s);

BOOST_AUTO_TEST_CASE(valid_cases) {
    BOOST_CHECK(isValid(""));
    BOOST_CHECK(isValid("()"));
    BOOST_CHECK(isValid("()[]{}"));
    BOOST_CHECK(isValid("{[]}"));
    BOOST_CHECK(isValid("(((())))"));
}

BOOST_AUTO_TEST_CASE(invalid_cases) {
    BOOST_CHECK(!isValid("(]"));
    BOOST_CHECK(!isValid("([)]"));
    BOOST_CHECK(!isValid("("));
    BOOST_CHECK(!isValid("]"));
    BOOST_CHECK(!isValid("((("));
    BOOST_CHECK(!isValid(")("));
}
```
