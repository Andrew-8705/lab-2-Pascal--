#pragma once

#include <string>
#include <vector>
#include <variant>

using namespace std;

class Node {
public:
    enum class NodeType {
        PROGRAM_STATEMENT,
        BLOCK_STATEMENT, 
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
    string name;
    variant<string, double, int> value;
    vector<Node*> children;

    Node(NodeType type, string name = "", variant<string, double, int> value = {})
        : type(type), name(name), value(value) {}

    virtual ~Node() {
        for (Node* child : children)
            delete child;
    }
};

