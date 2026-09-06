#include "lru_cache.h"

LRUCache::LRUCache(int capacity) : capacity_(capacity) {}

int LRUCache::get(int key) {
    auto it = lookup_.find(key);
    if (it != lookup_.end()) {
        items_.splice(items_.begin(), items_, it->second);
        return items_.front().second;
    }
    return -1;
}

void LRUCache::put(int key, int value) {
    //items_.push_front({key, value});
    auto it = lookup_.find(key);
    if (it == lookup_.end()) {
        if (lookup_.size() == capacity_) {
            lookup_.erase(items_.back().first);
            items_.pop_back();
        }
        items_.push_front({key, value});
        auto it = items_.begin();
        lookup_.emplace(key, it);
    }
    else {
        items_.splice(items_.begin(), items_, it->second);
        items_.front().second = value;
        lookup_[key] = items_.begin();
    }
}
