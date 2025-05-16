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
        bool expectOper = true;

        for (const Token& item : expr) {
            if (item.type == TokenType::LEFT_PAREN) {
                st.push(item); 
                expectOper = true;
            }
            else if (item.type == TokenType::RIGHT_PAREN) {
                stItem = st.top(); st.pop();
                while (stItem.type != TokenType::LEFT_PAREN) {
                    out.push_back(stItem);
                    stItem = st.top(); st.pop();
                }
                expectOper = false;
            }
            else if (priority.count(item.value)) {
                if (item.value == "-" && expectOper) {
                    Token unaryMinusToken = { TokenType::MINUS, "_", item.line, item.column };
                    st.push(unaryMinusToken);
                }
                else {
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
                expectOper = true;
            }
            else {
                out.push_back(item);
                expectOper = false;
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
    {"+", 1}, {"-", 1}, {"*", 2}, {"/", 2}, {"div", 2}, {"mod", 2}, {"_", 3}
};