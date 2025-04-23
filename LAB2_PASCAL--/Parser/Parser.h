#pragma once

#include <vector>
#include <optional>
#include <stdexcept>
#include <exception>
#include "../Base/Token.h"
#include "../Base/Node.h"
#include <list>

using namespace std;

class Parser {
private:
    list<list<Node>> ast;
    vector<Token> tokens;
    int curTokenPos;

    // функци€ котора€ позвол€ет просто посмотреть какого типа следующий токен
    Token peek() const {
        if (curTokenPos < tokens.size()) {
            return tokens[curTokenPos];
        }
        return Token(TokenType::UNKNOWN, "", -1, -1);
    }

    // функци€ простого перехода к следующему токену
    Token pass() {
        Token current = peek();
        if (curTokenPos < tokens.size()) {
            curTokenPos++;
        }
        return current;
    }

    // функци€ котора€ просматривает есть ли токен нужного дл€ нас типа токена
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

    // функци€ запроса котора€ смотрит есть ли у нас в векторе нужный токен
    Token require(vector<TokenType> tts, const string& errorMessage) { 
        optional<Token> tk = match(tts);
        if (tk.has_value())
            return tk.value();
        cout << "UUUPPPSPS" << '\n';
        throw std::runtime_error("Expected: " + errorMessage);
    }

    // начало парсинга
    Node* parseProgram() {
        require({ TokenType::KEYWORD_PROGRAM }, "'program'");
        Token programName = require({ TokenType::IDENTIFIER }, "program name");
        Node* programNode = new Node(Node::NodeType::PROGRAM_STATEMENT, programName.value);
        require({ TokenType::SEMICOLON }, "';'");
        programNode->children.push_back(parseBlock());
        require({ TokenType::END_OF_PROGRAM }, "'end.'");
        return programNode;
    }

    // парсим в отдельности блоки кода const/var/begin
    Node* parseBlock() {
        Node* blockNode = new Node(Node::NodeType::BLOCK_STATEMENT);
        if (peek().type == TokenType::KEYWORD_CONST) {
            blockNode->children.push_back(parseConstDeclarations());
        }
        if (peek().type == TokenType::KEYWORD_VAR) {
            blockNode->children.push_back(parseVarDeclarations());
        }
        blockNode->children.push_back(parseBeginStatement());
        return blockNode;
    }

    // парсинг блока const
    Node* parseConstDeclarations() {
        Node* constSectionNode = new Node(Node::NodeType::CONST_DECLARATION);
        require({ TokenType::KEYWORD_CONST }, "'const'");
        while (peek().type == TokenType::IDENTIFIER) {
            constSectionNode->children.push_back(parseConstDeclaration());
        }
        return constSectionNode;
    }

    // парсинг блока var
    Node* parseVarDeclarations() {
        Node* varSectionNode = new Node(Node::NodeType::VARIABLE_DECLARATION);
        require({ TokenType::KEYWORD_VAR }, "'var'");
        while (peek().type == TokenType::IDENTIFIER) {
            varSectionNode->children.push_back(parseVarDeclaration());
        }
        return varSectionNode;
    }

    // парсинг блока begin (основна€ программа + начало блока if/else)
    Node* parseBeginStatement() {
        require({ TokenType::KEYWORD_BEGIN }, "'begin'");
        Node* beginNode = new Node(Node::NodeType::BEGIN_STATEMENT);
        while (peek().type != TokenType::END_OF_PROGRAM && peek().type != TokenType::KEYWORD_END) {
            beginNode->children.push_back(parseStatement());
            if (peek().type == TokenType::SEMICOLON) {
                pass(); // пропускаем ';'
            }
        }
        return beginNode;
    }

    // парсим внутри блока begin, считаем что нельз€ оставл€ть пустой блок с основной программы
    Node* parseStatement() {
        if (peek().type == TokenType::IDENTIFIER) { // обрабатываем присваивание переменных
            return parseAssignmentStatement();
        }
        else if (peek().type == TokenType::KEYWORD_WRITE) { // обрабатываем функцию 'write'
            return parseWriteStatement();
        }
        else if (peek().type == TokenType::KEYWORD_READ) { // обрабатываем функцию 'read'
            return parseReadStatement();
        }
        else if (peek().type == TokenType::KEYWORD_IF) { // обрабатываем условные операторы
            return parseIfStatement();
        }
        else if (peek().type == TokenType::KEYWORD_BEGIN) { // обрабатываем начало условного оператора
            return parseBeginStatement();
        }
        else {
            throw std::runtime_error("Syntax Error: Unexpected token at start of statement: " + peek().value);
        }
    }

    Node* parseAssignmentStatement() {
        Token identifier = require({ TokenType::IDENTIFIER }, "variable identifier");
        Node* assignmentNode = new Node(Node::NodeType::ASSIGNMENT_STATEMENT, identifier.value);
        require({ TokenType::ASSIGN }, "':='");
        assignmentNode->children.push_back(parseExpression());
        require({ TokenType::SEMICOLON }, "';'");
        return assignmentNode;
    }

    Node* parseWriteStatement() {
        require({ TokenType::KEYWORD_WRITE }, "'Write'");
        require({ TokenType::LEFT_PAREN }, "'('");
        Node* writeNode = new Node(Node::NodeType::WRITE_STATEMENT);
        if (peek().type != TokenType::RIGHT_PAREN) {
            writeNode->children.push_back(parseExpression());
            while (match({ TokenType::COMMA })) {
                writeNode->children.push_back(parseExpression());
            }
        }
        require({ TokenType::RIGHT_PAREN }, "')'");
        require({ TokenType::SEMICOLON }, "';'");
        return writeNode;
    }

    Node* parseReadStatement() {
        require({ TokenType::KEYWORD_READ }, "'Read'");
        require({ TokenType::LEFT_PAREN }, "'('");
        Node* readNode = new Node(Node::NodeType::READ_STATEMENT);
        Node* identifierListNode = parseIdentifierList();
        readNode->children.push_back(identifierListNode);
        require({ TokenType::RIGHT_PAREN }, "')'");
        require({ TokenType::SEMICOLON }, "';'");
        return readNode;
    }

    Node* parseIfStatement() {
        require({ TokenType::KEYWORD_IF }, "'if'");
        Node* ifNode = new Node(Node::NodeType::IF_STATEMENT);
        require({ TokenType::LEFT_PAREN }, "'('");
        ifNode->children.push_back(parseExpression()); // условное выражение
        require({ TokenType::RIGHT_PAREN }, "')'");
        require({ TokenType::KEYWORD_THEN }, "'then'");

        if (peek().type == TokenType::KEYWORD_BEGIN) { // ветка if
            ifNode->children.push_back(parseStatement());
            require({ TokenType::KEYWORD_END }, "'end'");
        }
        else {
            ifNode->children.push_back(parseStatement());
        }

        if (match({ TokenType::KEYWORD_ELSE })) { // ветка else
            if (peek().type == TokenType::KEYWORD_BEGIN) {
                ifNode->children.push_back(parseStatement());
                require({ TokenType::KEYWORD_END }, "'end'");
            }
            else {
                ifNode->children.push_back(parseStatement()); 
            }
        }
        return ifNode;
    }

    Node* parseExpression() {
        // пока считаем, что выражение - это просто литерал или идентификатор
        if (peek().type == TokenType::IDENTIFIER) {
            Token identifier = pass();
            return new Node(Node::NodeType::IDENTIFIER, identifier.value);
        }
        else {
            return parseLiteral();
        }
        // TODO: –еализовать разбор сложных выражений с операторами
    }

    // парсинг отдельной переменной объ€вленной в блоке const
    Node* parseConstDeclaration() {
        Token identifier = require({ TokenType::IDENTIFIER }, "constant identifier");
        Node* constDeclNode = new Node(Node::NodeType::CONST_DECLARATION, identifier.value);
        require({ TokenType::COLON }, "':'");
        Node* typeNode = parseTypeSpecifier();
        require({ TokenType::EQUAL }, "'='");
        Node* literalNode = parseLiteral();
        require({ TokenType::SEMICOLON }, "';'");
        constDeclNode->children.push_back(new Node(Node::NodeType::IDENTIFIER, identifier.value));
        constDeclNode->children.push_back(typeNode);
        constDeclNode->children.push_back(literalNode);
        return constDeclNode;
    }

    // парсинг отдельной переменной/набора переменных объ€вленный в блоке var
    Node* parseVarDeclaration() {
        Node* varDeclNode = new Node(Node::NodeType::VARIABLE_DECLARATION);
        Node* identifierListNode = parseIdentifierList();
        require({ TokenType::COLON }, "':'");
        Node* typeNode = parseTypeSpecifier();
        require({ TokenType::SEMICOLON }, "';'");
        varDeclNode->children.push_back(identifierListNode);
        varDeclNode->children.push_back(typeNode);
        return varDeclNode;
    }

    // парсинг перечислени€ переменных при объ€влении в блоке var
    Node* parseIdentifierList() {
        Node* identifierListNode = new Node(Node::NodeType::IDENTIFIER_LIST);
        Token identifier = require({ TokenType::IDENTIFIER }, "identifier");
        identifierListNode->children.push_back(new Node(Node::NodeType::IDENTIFIER, identifier.value));
        while (match({ TokenType::COMMA })) {
            identifier = require({ TokenType::IDENTIFIER }, "identifier");
            identifierListNode->children.push_back(new Node(Node::NodeType::IDENTIFIER, identifier.value));
        }
        return identifierListNode;
    }

    // парсинг типа переменной
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

    // парсинг значени€ переменной
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
