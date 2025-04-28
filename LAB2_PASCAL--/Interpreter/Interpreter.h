#pragma once

#include <iostream>
#include "../Parser_list/Node.h"
#include <list>
#include <variant>
#include <string>
#include <unordered_map>

using namespace std;

class Interpreter {
private:
	list<list<Node*>> ast;
	unordered_map<string, variant<int, double, string>> variables; // можно либо сразу использовать unordered_map, либо использовать SymbolTable из Base 
	unordered_map<string, variant<int, double, string>> constants;

	void executeBlock(const list<Node*>& block) {
		for (Node* node : block)
			executeStatement(node);
	}

	void executeStatement(Node* node) {
		switch (node->type) {
			case Node::NodeType::CONST_DECLARATION: {
				const ConstDeclarationNode* constNode = static_cast<const ConstDeclarationNode*>(node);
				if (constants.count(constNode->identifier))
					throw runtime_error("Constant already declared: " + constNode->identifier);
				constants[constNode->identifier] = constNode->value;
				break;
			}
			
			case Node::NodeType::VARIABLE_DECLARATION: {
				const VariableDeclarationNode* varNode = static_cast<const VariableDeclarationNode*>(node);
				const IdentifierListNode* identListNode = static_cast<const IdentifierListNode*>(varNode->identifierList);
				for (const string& identifier : identListNode->identifiers) {
					if (variables.count(identifier)) {
						throw runtime_error("Variable already declared: " + identifier);
					}
					if (varNode->type == "integer") variables[identifier] = 0;
					else if (varNode->type == "double") variables[identifier] = 0.0;
					else if (varNode->type == "string") variables[identifier] = "";
					else throw runtime_error("Unknown type: " + varNode->type);
				}
				break;
			}
			
			// реализовать обработку остальных типов Node

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