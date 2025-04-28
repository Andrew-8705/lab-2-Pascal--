#pragma once

#include "../Base/Token.h"
#include <stack>
#include <unordered_map>
#include <variant>

using namespace std;
using Value = std::variant<int, double, std::string>;

class PostfixCalculator {
public:
    static variant<int, double, bool> Calculate(const vector<Token>& expr, 
		const unordered_map<string, Value>& variables, 
		const unordered_map<string, Value>& constants);
};