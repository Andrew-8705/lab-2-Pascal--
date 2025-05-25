#pragma once

#include <variant>
#include <string>
#include "../Base/Node.h"
#include "../Base/Token.h"
#include "Expression.h"
#include <unordered_map>
#include <map>

using namespace std;
class Evaluator
{
public:

	static double evaluate_numeric(vector<Token> expression, unordered_map<string, variant<int, double, string>> variables, unordered_map<string, variant<int, double, string>> constants)
	{
		Expression expr(expression);
		vector<string> vars = expr.GetOperands();
		map<string, double> values;

		auto get_double_value = [&](const string& name, const variant<int, double, string>& var) -> double //эта штука достает double из variant <...,double,....>
		{
			return visit([&](auto&& arg) -> double
			{	
				using T = decay_t<decltype(arg)>;
				if constexpr (is_same_v<T, int>)
				{
					return static_cast<double>(arg); // веро€тно приведение к double хуже чем хранение variant<int, double>
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

		for (const auto& var : vars) 
		{
			if (variables.find(var) != variables.end())
				values[var] = get_double_value(var, variables[var]);
			else if (constants.find(var) != constants.end())
				values[var] = get_double_value(var, constants[var]);
			else
				throw runtime_error("Undeclared identifier: " + var);
		}
		return expr.Calculate(values);
	}

	static string evaluate_string(const vector<Token>& expression, const unordered_map<string, variant<int, double, string>>& variables, const unordered_map<string, variant<int, double, string>>& constants)
	{
		bool last_sign = false;
		bool first_token = true;
		string result = "";
		for (const auto& token : expression)
		{
			switch (token.type)
			{
			case 
			
			
			
			
			TokenTypes::STRING_LITERAL:
			{
				if (!last_sign && !first_token) // два операнда подр€д
					throw runtime_error("Invalid string expression");
				result += token.value;
				first_token = false;
				last_sign = false;
				break;
			}
			case TokenTypes::IDENTIFIER:
			{
				try {
					if (!last_sign && !first_token) // два операнда подр€д
						throw runtime_error("Invalid string expression");
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
						throw runtime_error("Undeclared identifier: " + token.value);
					}
				}
				catch (const bad_variant_access& e) {
					throw runtime_error("Type error: Identifier '" + token.value + "' has unsuitable type");
				}
				catch (const exception& e) {
					throw runtime_error("Error: " + string(e.what()));
				}
				first_token = false;
				last_sign = false;
				break;
			}
			case TokenTypes::PLUS:
			{
				if (first_token)
					throw runtime_error("Invalid string expression: ");
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
		if (last_sign)
			throw runtime_error("Invalid string expression: ");
		return result;
	}
};


