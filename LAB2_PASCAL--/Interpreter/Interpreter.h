#pragma once

#include <iostream>
#include "../Parser_list/Node.h"
#include <list>
#include <variant>
#include <string>
#include <unordered_map>
#include "../expression_evaluator/evaluator.h"

using namespace std;

class Interpreter {
private:
	list<list<Node*>> ast;
	unordered_map<string, variant<int, double, string>> variables; // ����� ���� ����� ������������ unordered_map, ���� ������������ SymbolTable �� Base 
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
				if (variables.find(varName) == variables.end()) 
				{
					throw runtime_error("Undeclared variable assignment: " + varName);
				}
				if (constants.count(varName)) 
				{
					throw runtime_error("Assignment to constant: " + varName);
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
						case TokenType::STRING_LITERAL:
						{
							if (!expression.empty())
								throw runtime_error("Invalid Write Arguments");
							if (expression.empty() && !last_comma)
								throw runtime_error("Invalid Write Arguments");
							record += token.value;
							last_comma = false;
							is_first_token = false;
							break;
						}
						case TokenType::COMMA:
						{
							if(is_first_token)
								throw runtime_error("Invalid Write Arguments");
							if (!expression.empty())
							{
								try
								{
									record += to_string(Evaluator::evaluate_numeric(expression, variables, constants));
								}
								catch (exception& exc)
								{
									try 
									{
										record += Evaluator::evaluate_string(expression, variables, constants);
									}
									catch (exception& exc)
									{
										throw runtime_error("Invalid Write Arguments" + string(exc.what()));
									}
								}
							}
							if (last_comma)
								throw runtime_error("Invalid Write Arguments");
							expression.clear();
							last_comma = true;
							is_first_token = false;
							break;
						}
						case TokenType::IDENTIFIER: case TokenType::INTEGER_LITERAL: case TokenType::DOUBLE_LITERAL: case TokenType::PLUS: case TokenType::MINUS: case TokenType::MULTIPLY: case TokenType::DIVIDE:
						{
							expression.push_back(token);
							last_comma = false;
							is_first_token = false;
							break;
						}
						default:
						{
							throw runtime_error("Invalid Write Arguments");
							break;
						}
					}
				}
				if(expression.empty() && last_comma)
					throw runtime_error("Invalid Write Arguments");
				else if (!expression.empty())
				{
					try
					{
						record += to_string(Evaluator::evaluate_numeric(expression, variables, constants));
					}
					catch (exception& exc)
					{
						try
						{
							record += Evaluator::evaluate_string(expression, variables, constants);
						}
						catch (exception& exc)
						{
							throw runtime_error("Invalid Write Arguments" + string(exc.what()));
						}
					}
				}
				cout << record << endl;
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
					cin >> input;
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
				for (auto& token : ifNode->condition)//������
				{
					if (token.type == TokenType::EQUAL || token.type == TokenType::NON_EQUAL)//NON_EQUAL ����� � ��� �������, �� �� ��������������
					{
						if (after_sign)//��� �����
							throw runtime_error("Invalid condition expression");
						sign = token;
						after_sign = true;
						continue;
					}
					after_sign ? right_expression.push_back(token) : left_expression.push_back(token);
				}
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
						throw runtime_error("Invalid consition expression: " + string(exc.what()));
					}
				}
				
				bool is_equal = visit([](auto&& left, auto&& right) -> bool //�� ������, ���� ����� ��������� �����, � ������ ������
				{
					using LeftType = decay_t<decltype(left)>;
					using RightType = decay_t<decltype(right)>;
					if constexpr (is_same_v<LeftType, RightType>) 
					{
						return left == right;
					}
					else 
					{
						return false;
					}
				}, leftExRes, rightExRes);

				if (sign.type == TokenType::UNKNOWN)
					throw runtime_error("Expected comparison operator");

				switch (sign.type)
				{
					case TokenType::EQUAL:
					{
						is_equal ? executeBlock(ifNode->thenStatement) : executeBlock(ifNode->elseStatement);
						break;
					}
					case TokenType::NON_EQUAL:
					{
						is_equal ? executeBlock(ifNode->elseStatement) : executeBlock(ifNode->thenStatement);
						break;
					}
					default:
					{
						throw runtime_error("How?");
						break;
					}
				}
				break;
			}
		}
	}

public:
	Interpreter(const list<list<Node*>>& parsedAst) : ast(parsedAst) {}

	void run() {
		if (!ast.empty()) {
			cout << "Program'" << static_cast<ProgramNode*>(ast.front().front())->programName << "' started" << '\n';
			auto it = next(ast.begin());

			while (it != ast.end()) {
				executeBlock(*it);
				it++;
			}
		}
	}

};