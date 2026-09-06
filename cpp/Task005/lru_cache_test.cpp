#define BOOST_TEST_MODULE LRUCacheTest
#include <boost/test/unit_test.hpp>

#include "lru_cache.h"

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
