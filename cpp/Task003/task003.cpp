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

    for (char ch: s) {
        if (!st.empty()) {
            if (matching.find(ch) != matching.end()) {
                if (matching.at(ch) == st.top()) {
                    st.pop();
                    continue;
                }
            }
        }
        st.push(ch);
    }

    return st.empty();
}
