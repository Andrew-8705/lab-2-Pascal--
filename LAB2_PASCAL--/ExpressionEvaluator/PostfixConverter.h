#pragma once

#include <vector>
#include "../Base/Token.h"
#include <stack>
#include <map>

using namespace std;

class PostfixConverter {
public:
    static map<string, int> priority;

    static vector<Token> Convert(const vector<Token>& expr) {
        stack<Token> st;
        vector<Token> out;
        Token stItem;
        for (const Token& item : expr) {
            if (item.type == TokenType::LEFT_PAREN) st.push(item);
            else if (item.type == TokenType::RIGHT_PAREN) {
                stItem = st.top(); st.pop();
                while (stItem.type != TokenType::LEFT_PAREN) {
                    out.push_back(stItem);
                    stItem = st.top(); st.pop();
                }
            }
            else if (priority.count(item.value)) {
                while (!st.empty()) {
                    stItem = st.top(); st.pop();
                    if (priority[item.value] <= priority[stItem.value])
                        out.push_back(stItem);
                    else {
                        st.push(stItem);
                        break;
                    }
                }
                st.push(item);
            }
            else {
                out.push_back(item);
            }
        }
        while (!st.empty()) {
            stItem = st.top(); st.pop();
            out.push_back(stItem);
        }
        return out;
    }
};

map<string, int> PostfixConverter::priority = {
    {"+", 1}, {"-", 1}, {"*", 2}, {"/", 2}, {"div", 2}, {"mod", 2}
};