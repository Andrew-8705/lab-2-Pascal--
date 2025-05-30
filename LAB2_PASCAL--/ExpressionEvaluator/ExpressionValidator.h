#pragma once

#include <vector>
#include <string>
#include <exception>
#include <stdexcept>
#include <stack>
#include "../Base/Token.h"

using namespace std;

class ExpressionValidator {
    static void CheckValidCharacters(vector<Token>& expr) {
        for (Token tk : expr) {
            if (tk.type != 
                TokenTypes::INTEGER_LITERAL && tk.type != TokenTypes::DOUBLE_LITERAL &&
                tk.type != TokenTypes::IDENTIFIER && !IsOperator(tk.value) && tk.type != TokenTypes::LEFT_PAREN &&
                tk.type != TokenTypes::RIGHT_PAREN)
                throw runtime_error("Invalid character in expression");
        }
    }

    static void CheckBrackets(vector<Token>& expr) {
        stack<Token> brStack;
        for (Token tk : expr) {
            if (tk.type == TokenTypes::LEFT_PAREN) {
                brStack.push(tk);
            }
            else if (tk.type == TokenTypes::RIGHT_PAREN) {
                if (brStack.empty()) {
                    throw runtime_error("Closing parenthesis is not matched");
                }
                brStack.pop();
            }
        }
        if (!brStack.empty()) {
            throw runtime_error("One or more opening parentheses are not paired");
        }
    }

public:
    static bool IsOperator(string op) {
        return op == "+" || op == "-" || op == "*" || op == "/" || op == "mod" || op == "div" || op == "_";
    }

    static void Validate(vector<Token>& expr) {
        CheckValidCharacters(expr);
        CheckBrackets(expr);
    }
};