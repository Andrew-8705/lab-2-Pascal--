#pragma once

#include <string>
#include <vector>
#include <variant>

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
        BINARY_OPERATION,
        UNARY_OPERATION,
        WRITE_STATEMENT,
        READ_STATEMENT
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

