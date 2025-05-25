#pragma once

#include <string>
#include <vector>
#include "../Base/Token.h"
#include "ExpressionValidator.h"
#include "PostfixConverter.h"
#include "PostfixCalculator.h"

using namespace std;

class Expression
{
private:
    vector<Token> infix;
    vector<Token> postfix;
    vector<string> operands;

public: 
    Expression(vector<Token>& expr) : infix(expr) {
        ExpressionValidator::Validate(expr);
        postfix = PostfixConverter::Convert(infix);
        
        for (Token tk : infix)
            if (tk.type == 
                
                
                
                
                
                
                
                TokenTypes::IDENTIFIER)
                operands.push_back(tk.value);
    }

    vector<Token> GetInfix() const { return infix; };
    vector<Token> GetPostfix() const { return postfix; };
    vector<string> GetOperands() const { return operands; };

    double Calculate(const map<string, double>& values) {
        return PostfixCalculator::Calculate(postfix, values);
    }
};

