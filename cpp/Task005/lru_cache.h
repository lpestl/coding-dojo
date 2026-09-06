#pragma once

#include <list>
#include <unordered_map>
#include <utility>

class LRUCache {
public:
    explicit LRUCache(int capacity);

    int get(int key);

    void put(int key, int value);

private:
    int capacity_;
    // Пары (key, value) в порядке использования: front — самый недавний.
    std::list<std::pair<int, int>> items_;
    // key -> итератор на элемент в items_.
    std::unordered_map<int, std::list<std::pair<int, int>>::iterator> lookup_;
};
