#include "task001.h"
#include "iostream"

#include <unordered_map>

// Возвращает пару индексов элементов, сумма которых равна target.
std::vector<int> twoSum(const std::vector<int>& nums, int target) {
    std::unordered_map<int, int> seen; // значение -> индекс

    for (int i = 0; i < nums.size(); ++i) {
        auto it = seen.find(target - nums[i]);
        if (it != seen.end()) {
            return {it->second, i};
        }
        seen.insert({nums[i], i});
    }

    return {};
}
