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
        if (expr.size() == 1 && expr[0].type == TokenType::STRING_LITERAL) return;
        for (Token tk : expr) {
            if (tk.type != TokenType::INTEGER_LITERAL && tk.type != TokenType::DOUBLE_LITERAL && 
                tk.type != TokenType::IDENTIFIER && !IsOperator(tk.value))
                throw runtime_error("Invalid character in expression");
        }
    }

    static void CheckBrackets(vector<Token>& expr) {
        stack<Token> brStack;
        for (Token tk : expr) {
            if (tk.type == TokenType::LEFT_PAREN) {
                brStack.push(tk);
            }
            else if (tk.type == TokenType::RIGHT_PAREN) {
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
    static bool IsOperator(std::string op) {
        if (op == "+") return true;
        else if (op == "-") return true;
        else if (op == "*") return true;
        else if (op == "/") return true;
        else if (op == "mod") return true;
        else if (op == "div") return true;
        else return false;
    }

    static void Validate(vector<Token>& expr) {
        CheckValidCharacters(expr);
        CheckBrackets(expr);
    }
};