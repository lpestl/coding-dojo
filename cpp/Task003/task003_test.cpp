#define BOOST_TEST_MODULE ValidParenthesesTest
#include <boost/test/unit_test.hpp>

#include <string>

#include "task003.h"

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
