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

enum class ExpressionType
{
	STRING,
	NUMERIC,
	INVALID
};

class Evaluator
{
	static  ExpressionType CheckStringNumberMixing(vector<Token> expression)
	{
		bool any_string = false;
		bool any_numeric = false;
		for (auto& token : expression)
		{
			switch (token.type)
			{
				case TokenType::DOUBLE_LITERAL: case TokenType::INTEGER_LITERAL:
				{
					any_numeric = true;
					break;
				}
				case TokenType::STRING_LITERAL:
				{
					any_string = true;
					break;
				}
			}
			if (any_numeric && any_string)
				return ExpressionType::INVALID;
		}
		return (any_numeric ? ExpressionType::NUMERIC : ExpressionType::STRING);
	}

	static string infix(vector<Token> expression)
	{
		string infix = "";
		for (auto& token : expression)
			infix += token.value;
		return infix;
	}

public:

	static double evaluate_numeric(vector<Token> expression, unordered_map<string, variant<int, double, string>> variables, unordered_map<string, variant<int, double, string>> constants)
	{
		if (expression.empty())
			return 0;
		string infixExpression = infix(expression);
		if (CheckStringNumberMixing(expression) == ExpressionType::INVALID || CheckStringNumberMixing(expression) == ExpressionType::STRING)
			throw runtime_error("String in numeric expression: " + infixExpression);
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

	static string evaluate_string(const vector<Token>& expression, const unordered_map<string, variant<int, double, string>>& variables, const unordered_map<string, variant<int, double, string>>& constants) 
	{
		if (expression.empty()) 
			return "";
		string inf = infix(expression);
		bool last_sign = false;
		bool first_token = true;
		string result = "";
		for (const auto& token : expression) 
		{
			switch (token.type) 
			{
				case TokenType::STRING_LITERAL: 
				{
					if (!last_sign && !first_token)//два операнда подряд
						throw runtime_error("Invalid string expression: " + inf);
					result += token.value; 
					first_token = false;
					last_sign = false;
					break;
				}
				case TokenType::IDENTIFIER: 
				{
					try {
						if (!last_sign && !first_token)//два операнда подряд
							throw runtime_error("Invalid string expression: " + inf);
						if (variables.find(token.value) != variables.end()) 
						{
							result += get<string>(variables.at(token.value)); 
						}
						else if (constants.find(token.value) != constants.end()) 
						{
							result += get<string>(constants.at(token.value)); 
						}
						else 
						{
							throw runtime_error("Undeclared identifier: " + token.value + " in: " + inf);
						}
					}
					catch (const bad_variant_access& e) {
						throw runtime_error("Type error: Identifier '" + token.value + "' is not a string in: " + inf);
					}
					catch (const exception& e) {
						throw runtime_error("Error: " + string(e.what()));
					}
					first_token = false;
					last_sign = false;
					break;
				}
				case TokenType::PLUS: 
				{
					if(first_token)
						throw runtime_error("Invalid string expression: " + inf);
					first_token = false;
					last_sign = true;
					break;
				}
				default: 
				{
					throw runtime_error("Unexpected token in string expression: " + token.value);
					break;
				}
			}
		}
		if(last_sign)
			throw runtime_error("Invalid string expression: " + inf);
		return result;
	}
};

#endif

