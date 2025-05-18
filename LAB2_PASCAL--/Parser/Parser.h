#pragma once

#include <iostream>
#include <vector>
#include <optional>
#include <stdexcept>
#include <exception>
#include "../Base/Token.h"
#include "../Base/Node.h"
#include <list>
#include <set>

using namespace std;

class Parser {
private:
    vector<Token> tokens;
    list<list<Node*>> ast;
    int curTokenPos = 0;
    list<Node*>* currentBlock = nullptr;

    Token peek() const {
        if (curTokenPos < tokens.size()) {
            return tokens[curTokenPos];
        }
        return Token(TokenType::UNKNOWN, "", -1, -1);
    }

    TokenType getNextTokenType() const {
        if (curTokenPos + 1 < tokens.size()) {
            return tokens[curTokenPos + 1].type;
        }
        return TokenType::UNKNOWN;
    }

    Token pass() {
        Token current = peek();
        if (curTokenPos < tokens.size()) {
            curTokenPos++;
        }
        return current;
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
        throw runtime_error("Expected: " + errorMessage);
    }

    void addNode(Node* node) {
        if (currentBlock) {
            currentBlock->push_back(node);
        }
        else {
            ast.push_back({ node });
            currentBlock = &ast.back();
        }
    }

    void startNewBlock() {
        ast.push_back({});
        currentBlock = &ast.back();
    }

    void parseProgram() {
        require({ TokenType::KEYWORD_PROGRAM }, "'program'");
        Token programName = require({ TokenType::IDENTIFIER }, "'program name'");
        addNode(new ProgramNode(programName.value));
        require({ TokenType::SEMICOLON }, "';'");

        if (peek().type == TokenType::KEYWORD_CONST) {
            startNewBlock();
            addNode(new ConstSectionNode());
            parseConstDeclarations();
        }

        if (peek().type == TokenType::KEYWORD_VAR) {
            startNewBlock();
            addNode(new VarSectionNode());
            parseVarDeclarations();
        }

        startNewBlock();
        addNode(new BeginSectionNode());
        parseBeginStatement();

        require({ TokenType::END_OF_PROGRAM }, "'end.'");
    }

    void parseConstDeclarations() {
        require({ TokenType::KEYWORD_CONST }, "'const'");
        while (peek().type == TokenType::IDENTIFIER) {
            parseConstDeclaration();
        }

        if (peek().type != TokenType::KEYWORD_VAR && peek().type != TokenType::KEYWORD_BEGIN) {
            throw runtime_error("Syntax Error: Expected 'var' or 'begin' after constant declarations, but got " + peek().value);
        }
    }

    void parseConstDeclaration() {
        Token identifier = require({ TokenType::IDENTIFIER }, "constant identifier");
        ConstDeclarationNode* constDeclNode = new ConstDeclarationNode(identifier.value);
        require({ TokenType::COLON }, "':'");
        constDeclNode->type = parseTypeSpecifier();
        require({ TokenType::EQUAL }, "'='");
        constDeclNode->value = parseLiteral();
        currentBlock->push_back(constDeclNode);
        require({ TokenType::SEMICOLON }, "';'");
    }

    void parseVarDeclarations() {
        require({ TokenType::KEYWORD_VAR }, "'var'");
        while (peek().type == TokenType::IDENTIFIER) {
            parseVarDeclaration();
        }

        if (peek().type != TokenType::KEYWORD_BEGIN) {
            throw runtime_error("Syntax Error: Expected 'begin' after variable declarations, but got " + peek().value);
        }
    }

    void parseVarDeclaration() {
        VariableDeclarationNode* varDeclNode = new VariableDeclarationNode();
        varDeclNode->identifierList = parseIdentifierList();
        require({ TokenType::COLON }, "':'");
        varDeclNode->type = parseTypeSpecifier();
        currentBlock->push_back(varDeclNode);
        require({ TokenType::SEMICOLON }, "';'");
    }

    IdentifierListNode* parseIdentifierList() {
        IdentifierListNode* idListNode = new IdentifierListNode();
        Token identifier = require({ TokenType::IDENTIFIER }, "identifier");
        idListNode->identifiers.push_back(identifier.value);
        while (match({ TokenType::COMMA })) {
            identifier = require({ TokenType::IDENTIFIER }, "identifier");
            idListNode->identifiers.push_back(identifier.value);
        }
        return idListNode;
    }
    string parseTypeSpecifier() {
        if (match({ TokenType::KEYWORD_INTEGER }))
            return "integer";
        else if (match({ TokenType::KEYWORD_DOUBLE }))
            return "double";
        else if (match({ TokenType::KEYWORD_STRING }))
            return "string";
        throw runtime_error("Syntax Error: Expected 'integer', 'double' or 'string' but got " + peek().value);
    }

    variant<int, double, string> parseLiteral() {
        if (auto intToken = match({ TokenType::INTEGER_LITERAL })) {
            return stoi(intToken->value);
        }
        else if (auto doubleToken = match({ TokenType::DOUBLE_LITERAL })) {
            return stod(doubleToken->value);
        }
        else if (auto stringToken = match({ TokenType::STRING_LITERAL })) {
            string value = stringToken->value;
            return value;
        }
        else {
            throw runtime_error("Syntax Error: Expected a literal but got " + peek().value);
        }
    }

    void parseBeginStatement() {
        require({ TokenType::KEYWORD_BEGIN }, "'begin'");
        while (peek().type != TokenType::END_OF_PROGRAM && peek().type != TokenType::KEYWORD_END) {
            parseStatement();
        }
        if (peek().type != TokenType::END_OF_PROGRAM) {
            require({ TokenType::KEYWORD_END }, "'end'");
        }
    }

    void parseStatement() {
        if (peek().type == TokenType::IDENTIFIER) {
            parseAssignmentStatement();
        }
        else if (peek().type == TokenType::KEYWORD_WRITE) {
            parseWriteStatement();
        }
        else if (peek().type == TokenType::KEYWORD_READ) {
            parseReadStatement();
        }
        else if (peek().type == TokenType::KEYWORD_IF) {
            parseIfStatement();
        }
        else {
            throw runtime_error("Syntax Error: Unexpected token at start of statement: " + peek().value);
        }
    }

    void parseAssignmentStatement() {
        static const set<TokenType> unexpectedTokens = {
            TokenType::KEYWORD_BEGIN,
            TokenType::KEYWORD_WRITE,
            TokenType::KEYWORD_READ,
            TokenType::KEYWORD_IF,
            TokenType::KEYWORD_END,
            TokenType::END_OF_PROGRAM
        };

        Token identifier = require({ TokenType::IDENTIFIER }, "variable identifier");
        AssignmentStatementNode* assignNode = new AssignmentStatementNode(identifier.value);
        require({ TokenType::ASSIGN }, "':='");
        while (peek().type != TokenType::SEMICOLON) {
            if (unexpectedTokens.count(peek().type)) {
                throw runtime_error("Syntax Error: Expected ';' after assignment for variable '" + identifier.value + "' at line " + to_string(identifier.line) + ", column " + to_string(identifier.column));
            }
            if (peek().type == TokenType::IDENTIFIER && getNextTokenType() == TokenType::ASSIGN) {
                throw runtime_error("Syntax Error: Expected ';' after assignment for variable '" + identifier.value + "' at line " + to_string(identifier.line) + ", column " + to_string(identifier.column));
            }
            assignNode->expression.push_back(pass());
        }
        if (assignNode->expression.empty()) {
            throw runtime_error("Syntax Error: Expected an expression after ':=' for variable '" + identifier.value + "' at line " + to_string(identifier.line) + ", column " + to_string(identifier.column));
        }
        currentBlock->push_back(assignNode);
        require({ TokenType::SEMICOLON }, "';'");
    }

    void parseWriteStatement() {
        require({ TokenType::KEYWORD_WRITE }, "'Write'");
        WriteStatementNode* writeNode = new WriteStatementNode();
        require({ TokenType::LEFT_PAREN }, "'('");
        while (getNextTokenType() != TokenType::RIGHT_PAREN) {
            writeNode->expression.push_back(pass());
        }
        writeNode->expression.push_back(pass());
        require({ TokenType::RIGHT_PAREN }, "')'");
        require({ TokenType::SEMICOLON }, "';'");
        currentBlock->push_back(writeNode);
    }

    void parseReadStatement() {
        require({ TokenType::KEYWORD_READ }, "'Read'");
        ReadStatementNode* readNode = new ReadStatementNode();
        require({ TokenType::LEFT_PAREN }, "'('");
        readNode->identifierList = parseIdentifierList();
        require({ TokenType::RIGHT_PAREN }, "')'");
        require({ TokenType::SEMICOLON }, "';'");
        currentBlock->push_back(readNode);
    }

    void parseIfStatement() {
        require({ TokenType::KEYWORD_IF }, "'if'");
        IfStatementNode* ifNode = new IfStatementNode();
        require({ TokenType::LEFT_PAREN }, "'('");

        static const std::set<TokenType> unexpectedTokensCondition = {
            TokenType::KEYWORD_THEN,
            TokenType::KEYWORD_ELSE,
            TokenType::KEYWORD_BEGIN,
            TokenType::KEYWORD_WRITE,
            TokenType::KEYWORD_READ,
            TokenType::KEYWORD_IF,
            TokenType::KEYWORD_END,
            TokenType::END_OF_PROGRAM,
            TokenType::SEMICOLON // преждевременная точка с запятой
        };

        while (peek().type != TokenType::RIGHT_PAREN) {
            if (unexpectedTokensCondition.count(peek().type)) {
                throw runtime_error("Syntax Error: Expected ')' after 'if' condition at line " + to_string(peek().line) + ", column " + to_string(peek().column));
            }
            ifNode->condition.push_back(pass());
            if (curTokenPos >= tokens.size()) {
                throw runtime_error("Syntax Error: Unexpected end of input while parsing 'if' condition.");
            }
        }
        require({ TokenType::RIGHT_PAREN }, "')'");
        require({ TokenType::KEYWORD_THEN }, "'then'");
        ifNode->thenStatement = parseStatementBlock();
        if (match({ TokenType::KEYWORD_ELSE })) {
            ifNode->elseStatement = parseStatementBlock();
        }
        currentBlock->push_back(ifNode);
    }

    list<Node*> parseStatementBlock() {
        list<Node*> block;
        list<Node*>* previousBlock = currentBlock;
        currentBlock = &block;
        if (peek().type == TokenType::KEYWORD_BEGIN) {
            require({ TokenType::KEYWORD_BEGIN }, "'begin'");
            BeginSectionNode* beginNode = new BeginSectionNode();
            block.push_back(beginNode);
            while (peek().type != TokenType::KEYWORD_END) {
                parseStatement();
                if (peek().type == TokenType::SEMICOLON) {
                    pass();
                }
            }
            require({ TokenType::KEYWORD_END }, "'end'");
        }
        else {
            parseStatement(); // <- пропускает отсутствие ';' в конце строки, надо как-то пофиксить
        }
        currentBlock = previousBlock;
        return block;
    }

public:
    Parser(const vector<Token>& tkns) : tokens(tkns) {};

    list<list<Node*>>& parse() {
        parseProgram();
        return ast;
    }
};