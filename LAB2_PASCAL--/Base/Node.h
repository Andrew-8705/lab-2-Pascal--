#pragma once

#pragma once

#include <string>
#include <vector>
#include <variant>
#include <optional>
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
    ConstDeclarationNode(const string& id, const string& t, variant<int, double, string> v)
        : Node(NodeType::CONST_DECLARATION), identifier(id), type(t), value(v) {}
};

class IdentifierListNode : public Node {
public:
    list<string> identifiers;
    IdentifierListNode() : Node(NodeType::IDENTIFIER_LIST) {}
    IdentifierListNode(const list<string>& ids) : Node(NodeType::IDENTIFIER_LIST), identifiers(ids) {}
    IdentifierListNode(initializer_list<string> ids) : Node(NodeType::IDENTIFIER_LIST), identifiers(ids) {}
};

class VariableDeclarationNode : public Node {
public:
    Node* identifierList;
    string type;
    VariableDeclarationNode() : Node(NodeType::VARIABLE_DECLARATION) {}
    VariableDeclarationNode(IdentifierListNode* idList, const string& t)
        : Node(NodeType::VARIABLE_DECLARATION), identifierList(idList), type(t) {}
};

class AssignmentStatementNode : public Node {
public:
    string variableName;
    vector<Token> expression;
    AssignmentStatementNode(const string& name) : Node(NodeType::ASSIGNMENT_STATEMENT), variableName(name) {}
    AssignmentStatementNode(const string& name, const vector<Token>& expr)
        : Node(NodeType::ASSIGNMENT_STATEMENT), variableName(name), expression(expr) {}
};


class WriteStatementNode : public Node {
public:
    vector<Token> expression;
    WriteStatementNode() : Node(NodeType::WRITE_STATEMENT) {}
    WriteStatementNode(const vector<Token>& expr) : Node(NodeType::WRITE_STATEMENT), expression(expr) {}
    WriteStatementNode(initializer_list<Token> expr) : Node(NodeType::WRITE_STATEMENT), expression(expr) {}
};

class ReadStatementNode : public Node {
public:
    Node* identifierList;
    ReadStatementNode() : Node(NodeType::READ_STATEMENT) {}
    ReadStatementNode(IdentifierListNode* idList) : Node(NodeType::READ_STATEMENT), identifierList(idList) {}
};

class IfStatementNode : public Node {
public:
    vector<Token> condition;
    list<Node*> thenStatement;
    list<Node*> elseStatement;
    IfStatementNode() : Node(NodeType::IF_STATEMENT) {}
    IfStatementNode(const vector<Token>& cond, const list<Node*>& thenStmt, const optional<list<Node*>>& elseStmt)
        : Node(NodeType::IF_STATEMENT), condition(cond), thenStatement(thenStmt) {
        if (elseStmt.has_value()) {
            elseStatement = elseStmt.value();
        }
    }
};