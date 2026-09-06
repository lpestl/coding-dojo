#pragma once

// Узел односвязного списка.
struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x = 0, ListNode* n = nullptr) : val(x), next(n) {}
};

// Разворачивает список и возвращает новую голову.
ListNode* reverseList(ListNode* head);
