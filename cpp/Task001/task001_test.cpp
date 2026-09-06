#define BOOST_TEST_MODULE TwoSumTest
#include <boost/test/unit_test.hpp>

#include <algorithm>
#include <vector>

#include "task001.h"

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
