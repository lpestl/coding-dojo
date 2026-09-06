#define BOOST_TEST_MODULE RotatedSearchTest
#include <boost/test/unit_test.hpp>

#include <vector>

#include "task004.h"

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
