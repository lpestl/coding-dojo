#include "task004.h"

// Возвращает индекс target или -1, если элемент не найден.
int search(const std::vector<int>& nums, int target) {
    int left = 0;
    int right = static_cast<int>(nums.size()) - 1;

    while (left <= right) {
        if (nums[left] == target)
            return left;
        if (nums[right] == target)
            return right;
        int mid = (left + right) / 2;
        if (nums[mid] == target)
            return mid;

        if (target < nums[mid])
            if (target < nums[left])
                left = mid + 1;
            else
                right = mid - 1;
        else if (target >= nums[mid])
            if (target >= nums[right])
                right = mid - 1;
            else
                left = mid + 1;

    }

    return -1;
}
