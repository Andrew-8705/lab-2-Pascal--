#pragma once

#include "../Base/Token.h"
#include <stack>

using namespace std;

class PostfixCalculator {
public:
    static double Calculate(const vector<Token>& expr, const map<string, double>& operands) {
        stack<double> st;
        double leftOp, rightOp;

    }
};
