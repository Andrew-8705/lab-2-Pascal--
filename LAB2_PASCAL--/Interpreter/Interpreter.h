#pragma once

#include <iostream>
#include "../Base/Node.h"
#include <variant>
#include <string>
#include <unordered_map>

using namespace std;

class Interpreter {
private:
	Node* astRoot;
	unordered_map<string, variant<int, double, string>> variables;
	unordered_map<string, variant<int, double, string>> constants;

	variant<int, double, string> evaluateExpression(Node* node) {
		switch (node->type) {
			case Node::NodeType::IDENTIFIER:
				if (variables.count(node->name))
					return variables[node->name];
				else if (constants.count(node->name))
					return constants[node->name];
				else
					throw runtime_error("Undefined identifier: " + node->name);
			case Node::NodeType::INTEGER_LITERAL:
				return get<int>(node->value);
			case Node::NodeType::DOUBLE_LITERAL:
				return get<double>(node->value);
			case Node::NodeType::STRING_LITERAL:
				return get<std::string>(node->value);
				// TODO: Реализация других типов выражений (бинарные операции и тд)
			default:
				throw runtime_error("Unknown expression type");
		}
	}

	void executeStatement(Node* node) {
		switch (node->type) {
			case Node::NodeType::CONST_SECTION:
			case Node::NodeType::VAR_SECTION:
			case Node::NodeType::BEGIN_SECTION:
			case Node::NodeType::PROGRAM_STATEMENT:
			case Node::NodeType::BLOCK_STATEMENT: {
				for (Node* child : node->children) {
					executeStatement(child);
				}
				break;
			}

			case Node::NodeType::CONST_DECLARATION: {
				for (Node* child : node->children) {
					string name = child->name;
					string type = child->name;
					variant<int, double, string> value = evaluateExpression(node->children[2]);
					if (constants.count(name))
						throw runtime_error("Constant already declared: " + name);
					constants[name] = value;
				}
				break;
			}
			
			case Node::NodeType::VARIABLE_DECLARATION: {

				std::string type = get<std::string>(node->children[1]->value);
				for (Node* identifierNode : node->children[0]->children) {
					if (variables.count(identifierNode->name)) {
						throw std::runtime_error("Variable already declared: " + identifierNode->name);
					}
					if (type == "integer") variables[identifierNode->name] = 0;
					else if (type == "double") variables[identifierNode->name] = 0.0;
					else if (type == "string") variables[identifierNode->name] = "";
					else throw runtime_error("Unknown type: " + type);
				}
				break;
			}
		}
	}

public:
	Interpreter(Node* root) : astRoot(root) {}

	void run() {
		executeStatement(astRoot);
	}

};