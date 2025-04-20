#pragma once

#include <vector>
#include <optional>
#include <stdexcept>
#include <exception>
#include "../Base/Token.h"
#include "../Base/Node.h"

using namespace std;

class Parser {
private:
    vector<Token> tokens;
    int curTokenPos;

    Token peek() const {
        if (curTokenPos < tokens.size()) {
            return tokens[curTokenPos];
        }
        return Token(TokenType::UNKNOWN, "", -1, -1);
    }

    optional<Token> match(vector<TokenType> tts) {
        if (curTokenPos < tokens.size()) {
            Token curToken = tokens[curTokenPos];
            for (TokenType tt : tts) {
                if (curToken.type == tt) {
                    curTokenPos++;
                    return curToken;
                }
            }
        }
        return nullopt;
    }

    Token require(vector<TokenType> tts, const string& errorMessage) {
        optional<Token> tk = match(tts);
        if (tk.has_value())
            return tk.value();
        cout << "UUUPPPSPS" << '\n';
        throw std::runtime_error("Expected: " + errorMessage);
    }

    Node* parseProgram() {
        require({ TokenType::KEYWORD_PROGRAM }, "'program'");
        Token programName = require({ TokenType::IDENTIFIER }, "program name");
        Node* programNode = new Node(Node::NodeType::PROGRAM_STATEMENT, programName.value);
        require({ TokenType::SEMICOLON }, "';'");
        programNode->children.push_back(parseBlock());
        require({ TokenType::END_OF_PROGRAM }, "'end.'");
        return programNode;
    }

    Node* parseBlock() {
        Node* blockNode = new Node(Node::NodeType::BLOCK_STATEMENT);
        if (peek().type == TokenType::KEYWORD_CONST) {
            blockNode->children.push_back(parseConstDeclarations());
        }
        /*if (peek().type == TokenType::KEYWORD_VAR) {
            blockNode->children.push_back(parseVarDeclarations());
        }
        blockNode->children.push_back(parseBeginStatement());*/
        return blockNode;
    }

    Node* parseConstDeclarations() {
        Node* constSectionNode = new Node(Node::NodeType::CONST_DECLARATION);
        require({ TokenType::KEYWORD_CONST }, "'const'");
        while (peek().type == TokenType::IDENTIFIER) {
            constSectionNode->children.push_back(parseConstDeclaration());
        }
        return constSectionNode;
    }

    Node* parseConstDeclaration() {
        Token identifier = require({ TokenType::IDENTIFIER }, "constant identifier");
        Node* constDeclNode = new Node(Node::NodeType::CONST_DECLARATION, identifier.value);
        require({ TokenType::COLON }, "':'");
        Node* typeNode = parseTypeSpecifier();
        require({ TokenType::EQUAL }, "'='");
        Node* literalNode = parseLiteral();
        require({ TokenType::SEMICOLON }, "';'");
        constDeclNode->children.push_back(new Node(Node::NodeType::IDENTIFIER, identifier.value)); // Store name again for clarity
        constDeclNode->children.push_back(typeNode);
        constDeclNode->children.push_back(literalNode);
        return constDeclNode;
    }

    Node* parseTypeSpecifier() {
        if (match({ TokenType::KEYWORD_INTEGER })) {
            return new Node(Node::NodeType::INTEGER_LITERAL, "integer");
        }
        else if (match({ TokenType::KEYWORD_DOUBLE })) {
            return new Node(Node::NodeType::DOUBLE_LITERAL, "double");
        }
        else if (match({ TokenType::KEYWORD_STRING })) {
            return new Node(Node::NodeType::STRING_LITERAL, "string");
        }
            
        throw std::runtime_error("Syntax Error: Expected 'integer' or 'double' but got " + peek().value);
    }

    Node* parseLiteral() {
        if (auto intToken = match({ TokenType::INTEGER_LITERAL })) {
            return new Node(Node::NodeType::INTEGER_LITERAL, "", std::stoi(intToken->value));
        }
        else if (auto doubleToken = match({ TokenType::DOUBLE_LITERAL })) {
            return new Node(Node::NodeType::DOUBLE_LITERAL, "", std::stod(doubleToken->value));
        }
        else if (auto stringToken = match({ TokenType::STRING_LITERAL })) {
            std::string value = stringToken->value.substr(1, stringToken->value.length() - 2);
            return new Node(Node::NodeType::STRING_LITERAL, "", value);
        }
        else {
            throw std::runtime_error("Syntax Error: Expected a literal or identifier but got " + peek().value);
        }
    }

public:
    Parser(const vector<Token>& tkns) : tokens(tkns) {};

    Node* parse() {
        return parseProgram();
    }

};
