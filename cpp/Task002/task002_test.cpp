#define BOOST_TEST_MODULE ReverseListTest
#include <boost/test/unit_test.hpp>

#include <vector>

#include "task002.h"

// Собирает значения списка в вектор.
std::vector<int> toVector(ListNode* head) {
    std::vector<int> result;
    while (head) {
        result.push_back(head->val);
        head = head->next;
    }
    return result;
}

BOOST_AUTO_TEST_CASE(empty_list) {
    BOOST_CHECK(reverseList(nullptr) == nullptr);
}

BOOST_AUTO_TEST_CASE(single_node) {
    ListNode head(42);
    ListNode* result = reverseList(&head);
    BOOST_REQUIRE(result != nullptr);
    BOOST_CHECK_EQUAL(result->val, 42);
    BOOST_CHECK(result->next == nullptr);
}

BOOST_AUTO_TEST_CASE(multiple_nodes) {
    ListNode n5(5);
    ListNode n4(4, &n5);
    ListNode n3(3, &n4);
    ListNode n2(2, &n3);
    ListNode n1(1, &n2);

    ListNode* result = reverseList(&n1);
    std::vector<int> expected{5, 4, 3, 2, 1};
    BOOST_CHECK(toVector(result) == expected);
}
