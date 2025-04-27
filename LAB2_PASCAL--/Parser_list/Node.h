#pragma once

#pragma once

#include <string>
#include <vector>
#include <variant>
#include <list>
#include "../Base/Token.h"

using namespace std;

class Node {
public:
    enum class NodeType {
        PROGRAM_STATEMENT,
        CONST_DECLARATION,
        VARIABLE_DECLARATION,
        ASSIGNMENT_STATEMENT,
        IF_STATEMENT,
        IDENTIFIER,
        INTEGER_LITERAL,
        DOUBLE_LITERAL,
        STRING_LITERAL,
        BINARY_OPERATION,
        UNARY_OPERATION,
        WRITE_STATEMENT,
        READ_STATEMENT,
        BEGIN_STATEMENT,
        IDENTIFIER_LIST,
        CONST_SECTION,
        VAR_SECTION,
        BEGIN_SECTION
    };

    NodeType type;

    Node(NodeType _type) : type(_type) {}

    virtual ~Node() = default;
};

class ProgramNode : public Node {
public:
    string programName;
    ProgramNode(const string& name) : Node(NodeType::PROGRAM_STATEMENT), programName(name) {}
};

class ConstSectionNode : public Node {
public:
    ConstSectionNode() : Node(NodeType::CONST_SECTION) {}
};

class VarSectionNode : public Node {
public:
    VarSectionNode() : Node(NodeType::VAR_SECTION) {}
};

class BeginSectionNode : public Node {
public:
    BeginSectionNode() : Node(NodeType::BEGIN_SECTION) {}
};

class ConstDeclarationNode : public Node {
public:
    string identifier;
    string type;
    variant<int, double, string> value;
    ConstDeclarationNode(const string& id) : Node(NodeType::CONST_DECLARATION), identifier(id) {}
};

class VariableDeclarationNode : public Node {
public:
    Node* identifierList;
    string type;
    VariableDeclarationNode() : Node(NodeType::VARIABLE_DECLARATION) {}
};

class IdentifierListNode : public Node {
public:
    list<string> identifiers;
    IdentifierListNode() : Node(NodeType::IDENTIFIER_LIST) {}
};

class AssignmentStatementNode : public Node {
public:
    string variableName;
    vector<Token> expression;
    AssignmentStatementNode(const string& name) : Node(NodeType::ASSIGNMENT_STATEMENT), variableName(name) {}
};

class WriteStatementNode : public Node {
public:
    vector<Token> expression;
    WriteStatementNode() : Node(NodeType::WRITE_STATEMENT) {}
};

class ReadStatementNode : public Node {
public:
    Node* identifierList;
    ReadStatementNode() : Node(NodeType::READ_STATEMENT) {}
};

class IfStatementNode : public Node {
public:
    vector<Token> condition;
    list<Node*> thenStatement;
    list<Node*> elseStatement;
    IfStatementNode() : Node(NodeType::IF_STATEMENT) {}
};
