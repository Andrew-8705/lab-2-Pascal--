#pragma once

#include <iostream>
#include "../Base/Node.h"
#include <list>
#include <variant>
#include <string>
#include <unordered_map>
#include "../ExpressionEvaluator/Evaluator.h"
#include <iomanip>
#include <sstream>

using namespace std;

const int pascal_precision = 14;

const string num_to_str(const double num)
{
	stringstream ss;
	ss << fixed << setprecision(pascal_precision) << num;
	string str = ss.str();
	size_t dot_pos = str.find('.');
	for (size_t i = str.size() - 1; i > dot_pos - 1; i--)
	{
		if (str[i] != '0' && i != dot_pos)
			break;
		str.erase(i);
	}
	return str;
}

class Interpreter {
private:
	list<list<Node*>> ast;
	unordered_map<string, variant<int, double, string>> variables;
	unordered_map<string, variant<int, double, string>> constants;

	void executeBlock(const list<Node*>& block) {
		for (Node* node : block)
			executeStatement(node);
	}

	void executeStatement(Node* node) {
		switch (node->type) {
			case Node::NodeType::CONST_DECLARATION: 
			{
				const ConstDeclarationNode* constNode = static_cast<const ConstDeclarationNode*>(node);
				if (constants.count(constNode->identifier))
					throw runtime_error("Constant already declared: " + constNode->identifier);
				constants[constNode->identifier] = constNode->value;
				break;
			}
			
			case Node::NodeType::VARIABLE_DECLARATION: 
			{
				const VariableDeclarationNode* varNode = static_cast<const VariableDeclarationNode*>(node);
				const IdentifierListNode* identListNode = static_cast<const IdentifierListNode*>(varNode->identifierList);
				for (const string& identifier : identListNode->identifiers) {
					if (constants.count(identifier)) {
						throw runtime_error("Redeclared constant name: " + identifier);
					}
					else if(variables.count(identifier)) {
						throw runtime_error("Variable already declared: " + identifier);
					}
					if (varNode->type == "integer") variables[identifier] = 0;
					else if (varNode->type == "double") variables[identifier] = 0.0;
					else if (varNode->type == "string") variables[identifier] = "";
					else throw runtime_error("Unknown type: " + varNode->type);
				}
				break;
			}

			case Node::NodeType::ASSIGNMENT_STATEMENT:
			{
				const AssignmentStatementNode* assignNode = static_cast<const AssignmentStatementNode*>(node);
				string varName = assignNode->variableName;

				if (constants.count(varName))
				{
					throw runtime_error("Assignment to constant: " + varName);
				}
				if (variables.find(varName) == variables.end()) 
				{
					throw runtime_error("Undeclared variable assignment: " + varName);
				}

				switch (variables[varName].index()){
					case 0: case 1:
					{
						try{
							double assign = Evaluator::evaluate_numeric(assignNode->expression, variables, constants);
							if (variables[varName].index() == 0)
								variables[varName] = static_cast<int>(assign);
							else
								variables[varName] = assign;
						}
						catch (exception& exc){
							throw runtime_error("Failed to assign " + varName + ": " + exc.what());
						}
						break;
					}
					case 2:
					{
						try {
							variables[varName] = Evaluator::evaluate_string(assignNode->expression, variables, constants);
						}
						catch (const exception& exc) {
							throw runtime_error("Failed to assign " + varName + ": " + exc.what());
						}
						break;
					}
				}
				break;
			}

			case Node::NodeType::WRITE_STATEMENT:
			{
				const WriteStatementNode* writeNode = static_cast<const WriteStatementNode*>(node);
				string record = "";
				bool last_comma = false;
				bool is_first_token = true;
				vector<Token> expression;
				for (auto& token : writeNode->expression)
				{
					switch (token.type)
					{
						case TokenType::COMMA:
						{
							if (is_first_token)
								throw runtime_error("Syntax Error in Write statement: The argument list cannot start with a comma");
							if (!expression.empty())
							{
								try
								{
									record += num_to_str(Evaluator::evaluate_numeric(expression, variables, constants));
								}
								catch (exception& exc)
								{
									try 
									{
										record += Evaluator::evaluate_string(expression, variables, constants);
									}
									catch (exception& exc)
									{
										throw runtime_error("Runtime Error in Write statement: Could not evaluate the expression: " + string(exc.what()));
									}
								}
							}
							if (last_comma)
								throw runtime_error("Syntax Error in Write statement: Multiple consecutive commas are not allowed");
							expression.clear();
							last_comma = true;
							is_first_token = false;
							break;
						}
						case TokenType::IDENTIFIER: 
						case TokenType::INTEGER_LITERAL: 
						case TokenType::DOUBLE_LITERAL: 
						case TokenType::PLUS: 
						case TokenType::MINUS: 
						case TokenType::MULTIPLY: 
						case TokenType::DIVIDE:
						case TokenType::KEYWORD_DIV:
						case TokenType::KEYWORD_MOD:
						case TokenType::STRING_LITERAL:
						{
							expression.push_back(token);
							last_comma = false;
							is_first_token = false;
							break;
						}
						default:
						{
							throw runtime_error("Syntax Error in Write statement: Unexpected token '" + token.value + "' in the argument list");
							break;
						}
					}
				}
				if (expression.empty() && last_comma)
					throw runtime_error("Syntax Error in Write statement: The argument list cannot end with a comma");
				else if (!expression.empty())
				{
					try
					{
						record += num_to_str(Evaluator::evaluate_numeric(expression, variables, constants));
					}
					catch (exception& exc)
					{
						try
						{
							record += Evaluator::evaluate_string(expression, variables, constants);
						}
						catch (exception& exc)
						{
							throw runtime_error("Runtime Error in Write statement: Could not evaluate the expression: " + string(exc.what()));
						}
					}
				}
				cout << record << '\n';
				break;
			}

			case Node::NodeType::READ_STATEMENT:
			{
				const ReadStatementNode* readNode = static_cast<const ReadStatementNode*>(node);
				const IdentifierListNode* readList = static_cast<const IdentifierListNode*>(readNode->identifierList);
				for (auto& listItem : readList->identifiers)
				{
					if (constants.find(listItem) != constants.end())
						throw runtime_error("Input to constant not allowed: " + listItem);
					if (variables.find(listItem) == variables.end())
						throw runtime_error("Undeclared variable: " + listItem);
					string input;
					getline(cin, input);
					try 
					{
						visit([&](auto&& arg) 
						{
							using VarType = decay_t<decltype(arg)>;

							if constexpr (is_same_v<VarType, int>) {
								variables[listItem] = stoi(input);
							}
							else if constexpr (is_same_v<VarType, double>) {
								variables[listItem] = stod(input);
							}
							else if constexpr (is_same_v<VarType, string>) {
								variables[listItem] = input;
							}
							else {
								throw runtime_error("Unsupported variable type for input: " + listItem);
							}
						}, variables[listItem]);
					}
					catch (const invalid_argument& e) {
						throw runtime_error("Invalid input for variable " + listItem + ": " + e.what());
					}
					catch (const out_of_range& e) {
						throw runtime_error("Input value out of range for variable " + listItem + ": " + e.what());
					}
				}
				break;
			}

			case Node::NodeType::IF_STATEMENT:
			{
				const IfStatementNode* ifNode = static_cast<const IfStatementNode*>(node);
				Token sign;
				vector<Token> left_expression;
				vector<Token> right_expression;
				bool after_sign = false;
				for (auto& token : ifNode->condition) // разбор
				{
					if (token.type == TokenType::EQUAL || token.type == TokenType::NON_EQUAL || token.type == TokenType::GREATER 
						|| token.type == TokenType::LESS || token.type == TokenType::GREATER_OR_EQUAL || token.type == TokenType::LESS_OR_EQUAL)
					{
						if (after_sign) // два знака
							throw runtime_error("Syntax Error in conditional expression: Multiple comparison operators ('=', '<>', '<', '<=', '>', '>=') found");
						sign = token;
						after_sign = true;
						continue;
					}
					after_sign ? right_expression.push_back(token) : left_expression.push_back(token);
				}
				if(!after_sign)
					throw runtime_error("Syntax Error in conditional expression: Must be comparison operator!");
				variant<double, string> leftExRes;
				variant<double, string> rightExRes;
				try
				{
					leftExRes = Evaluator::evaluate_numeric(left_expression, variables, constants);
					rightExRes = Evaluator::evaluate_numeric(right_expression, variables, constants);
				}
				catch (exception exc)
				{
					try
					{
						leftExRes = Evaluator::evaluate_string(left_expression, variables, constants);
						rightExRes = Evaluator::evaluate_string(right_expression, variables, constants);
					}
					catch (const exception& exc)
					{
						throw runtime_error("Invalid condition expression: " + string(exc.what()));
					}
				}
				
				bool compare = visit([&sign](auto&& left, auto&& right) -> bool // на случай, если левое выражение число, а правое строка
				{
					using LeftType = decay_t<decltype(left)>;
					using RightType = decay_t<decltype(right)>;
					if constexpr (is_same_v<LeftType, RightType>) 
					{
						switch (sign.type)
						{
							case TokenType::GREATER:
							{
								if constexpr (is_same_v<LeftType, string>)
									throw runtime_error("Cannot compare strings using >");
								return left > right;
								break;
							}
							case TokenType::GREATER_OR_EQUAL:
							{
								if constexpr (is_same_v<LeftType, string>)
									throw runtime_error("Cannot compare strings using >=");
								return left >= right;
								break;
							}
							case TokenType::LESS:
							{
								if constexpr (is_same_v<LeftType, string>)
									throw runtime_error("Cannot compare strings using <");
								return left < right;
								break;
							}
							case TokenType::LESS_OR_EQUAL:
							{
								if constexpr (is_same_v<LeftType, string>)
									throw runtime_error("Cannot compare strings using <=");
								return left <= right;
								break;
							}
							case TokenType::EQUAL:
							{
								return left == right;
								break;
							}
							case TokenType::NON_EQUAL:
							{
								return left != right;
								break;
							}
							default:
							{
								throw runtime_error("How?");
								break;
							}
						}
					}
					else 
					{
						throw runtime_error("Type mismatch error in conditional expression");
					}
				}, leftExRes, rightExRes);

				if (sign.type == TokenType::UNKNOWN)
					throw runtime_error("Expected comparison operator");

				compare ? executeBlock(ifNode->thenStatement) : executeBlock(ifNode->elseStatement);
				break;
			}
		}
	}

public:
	Interpreter(const list<list<Node*>>& parsedAst) : ast(parsedAst) {}

	void run() {
		if (!ast.empty()) {

			auto it = next(ast.begin());

			while (it != ast.end()) {
				executeBlock(*it);
				it++;
			}
;
		}
	}

};