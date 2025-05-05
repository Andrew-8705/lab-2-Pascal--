#pragma once

#include "../Base/Token.h"
#include "ExpressionValidator.h"
#include <vector>
#include <string>
#include <map>
#include <stdexcept>
#include <stack>
#include <cmath>

using namespace std;

class PostfixCalculator {
public:
    static double Calculate(const vector<Token>& expr, const map<string, double>& operands) {
        stack<double> st;
        double leftOp, rightOp;

        for (const Token& tk : expr) {
            string lexem = tk.value;
            if (ExpressionValidator::IsOperator(lexem)) {
                if (st.size() < 2)
                    throw runtime_error("Invalid postfix expression: insufficient operands for operator " + lexem);

                rightOp = st.top(); st.pop();
                leftOp = st.top(); st.pop();

                if (lexem == "+")
                    st.push(leftOp + rightOp);
                else if (lexem == "-")
                    st.push(leftOp - rightOp);
                else if (lexem == "*")
                    st.push(leftOp * rightOp);
                else if (lexem == "/") {
                    if (rightOp == 0.0) throw runtime_error("Division by zero");
                    st.push(leftOp / rightOp);
                }
                else if (lexem == "mod") {
                    if (rightOp == 0.0) {
                        throw runtime_error("Modulo by zero");
                    }
                    st.push(fmod(leftOp, rightOp));
                }
                else if (lexem == "div") {
                    if (rightOp == 0.0) {
                        throw runtime_error("Integer division by zero");
                    }
                    st.push(floor(leftOp / rightOp));
                }
            }
            else if (tk.type == TokenType::IDENTIFIER) {
                if (operands.count(lexem)) {
                    st.push(operands.at(lexem));
                }
                else {
                    throw runtime_error("Operand value not found for identifier: " + lexem);
                }
            }
            else if (tk.type == TokenType::INTEGER_LITERAL) {
                st.push(stod(lexem));
            }
            else if (tk.type == TokenType::DOUBLE_LITERAL) {
                st.push(stod(lexem));
            }
            else
                throw runtime_error("Unexpected token type in postfix expression: " + to_string(static_cast<int>(tk.type)));
        }
        if (st.size() != 1) {
            throw runtime_error("Invalid postfix expression: too many operands or too few operators");
        }
        return st.top();
    }
};

// TODO: добавить поддержку унарного минуса