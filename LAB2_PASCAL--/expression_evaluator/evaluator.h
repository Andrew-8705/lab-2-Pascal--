#ifndef EVALUATOR_H
#define EVALUATOR_H

#include <variant>
#include <string>
#include "../Parser_list/Node.h"
#include "../Base/Token.h"
#include <unordered_map>
#include "postfix.h"
#include <map>
#include <type_traits>

using namespace std;

class Evaluator
{
	static bool CheckStringNumberMixing(vector<Token> expression)
	{
		bool isString = false;
		bool isNum = false;
		for (auto& token : expression)
		{
			if (token.type == TokenType::STRING_LITERAL)
				isString = true;
			else if (token.type == TokenType::DOUBLE_LITERAL || token.type == TokenType::INTEGER_LITERAL)
				isNum = true;
			if (isString && isNum)
				return true;
		}
		return false;
	}
	static string infix(vector<Token> expression)
	{
		string infix = "";
		for (auto& token : expression)
			infix += token.value;
		return infix;
	}
public:
	static double evaluate(vector<Token> expression, unordered_map<string, variant<int, double, string>> variables, unordered_map<string, variant<int, double, string>> constants)
	{
		string infixExpression = infix(expression);
		if (CheckStringNumberMixing(expression))
			throw runtime_error("String in expression: " + infixExpression);
		TArithmeticExpression expr(infixExpression);
		map<string, double> values;

		auto get_double_value = [&](const string& name, const variant<int, double, string>& var) -> double //эта штука достает double из variant <...,double,....>
		{
			return visit([&](auto&& arg) -> double 
			{
				using T = decay_t<decltype(arg)>;
				if constexpr (is_same_v<T, int>) 
				{
					return static_cast<double>(arg);
				}
				else if constexpr (is_same_v<T, double>) 
				{
					return arg;
				}
				else 
				{
					throw runtime_error("String variable in arithmetic expression: " + name);
				}
			}, var);
		};

		for (auto& token : expression)
		{
			if (token.type == TokenType::IDENTIFIER)
			{
				if (variables.find(token.value) != variables.end())//нашли в переменных
				{
					values[token.value] = get_double_value(token.value, variables[token.value]);
				}
				else if (constants.find(token.value) != constants.end())//нашли в константах
				{
					values[token.value] = get_double_value(token.value, constants[token.value]);
				}
				else
					throw runtime_error("Undeclared identifier: " + token.value + " in expression: " + infix(expression));
			}
		}
		return expr.Calculate(values);
	}
};

#endif

