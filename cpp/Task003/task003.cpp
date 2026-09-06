#include "task003.h"

#include <stack>
#include <unordered_map>

bool isValid(const std::string& s) {
    std::stack<char> st;
    static const std::unordered_map<char, char> matching = {
        {')', '('},
        {']', '['},
        {'}', '{'}
    };

    // TODO: реализуйте проверку корректности

    return st.empty();
}
