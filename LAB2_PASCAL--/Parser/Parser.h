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
#include <memory>

using namespace std;

class Parser {
private:
    vector<Token> tokens;
    list<list<shared_ptr<Node>>> ast;
    int curTokenPos = 0;
    list<shared_ptr<Node>>* currentBlock = nullptr;

    Token peek() const {
        if (curTokenPos < tokens.size()) {
            return tokens[curTokenPos];
        }
        return Token(TokenTypes::UNKNOWN, "", -1, -1);
    }

    TokenTypes getNextTokenTypes() const {
        if (curTokenPos + 1 < tokens.size()) {
            return tokens[curTokenPos + 1].type;
        }
        return TokenTypes::UNKNOWN;
    }

    Token pass() {
        Token current = peek();
        if (curTokenPos < tokens.size()) {
            curTokenPos++;
        }
        return current;
    }

    optional<Token> match(vector<TokenTypes> tts) {
        if (curTokenPos < tokens.size()) {
            Token curToken = tokens[curTokenPos];
            for (TokenTypes tt : tts) {
                if (curToken.type == tt) {
                    curTokenPos++;
                    return curToken;
                }
            }
        }
        return nullopt;
    }

    Token require(vector<TokenTypes> tts, const string& errorMessage) {
        optional<Token> tk = match(tts);
        if (tk.has_value())
            return tk.value();
        throw runtime_error("Expected: " + errorMessage);
    }

    void addNode(shared_ptr<Node> node) {
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
        require({ TokenTypes::KEYWORD_PROGRAM }, "'program'");
        Token programName = require({ TokenTypes::IDENTIFIER }, "'program name'");
        addNode(make_shared<ProgramNode>(programName.value));
        require({ TokenTypes::SEMICOLON }, "';'");

        if (peek().type == TokenTypes::KEYWORD_CONST) {
            startNewBlock();
            addNode(make_shared<ConstSectionNode>());
            parseConstDeclarations();
        }

        if (peek().type == TokenTypes::KEYWORD_VAR) {
            startNewBlock();
            addNode(make_shared<VarSectionNode>());
            parseVarDeclarations();
        }

        startNewBlock();
        addNode(make_shared<BeginSectionNode>());
        parseBeginStatement();

        require({ TokenTypes::END_OF_PROGRAM }, "'end.'");
    }

    void parseConstDeclarations() {
        require({ TokenTypes::KEYWORD_CONST }, "'const'");
        while (peek().type == TokenTypes::IDENTIFIER) {
            parseConstDeclaration();
        }

        if (peek().type != TokenTypes::KEYWORD_VAR && peek().type != TokenTypes::KEYWORD_BEGIN) {
            throw runtime_error("Syntax Error: Expected 'var' or 'begin' after constant declarations, but got " + peek().value);
        }
    }

    void parseConstDeclaration() {
        Token identifier = require({ TokenTypes::IDENTIFIER }, "constant identifier");
        auto constDeclNode = make_shared<ConstDeclarationNode>(identifier.value);
        require({ TokenTypes::COLON }, "':'");
        constDeclNode->type = parseTypeSpecifier();
        require({ TokenTypes::EQUAL }, "'='");
        constDeclNode->value = parseLiteral();
        currentBlock->push_back(constDeclNode);
        require({ TokenTypes::SEMICOLON }, "';'");
    }

    void parseVarDeclarations() {
        require({ TokenTypes::KEYWORD_VAR }, "'var'");
        while (peek().type == TokenTypes::IDENTIFIER) {
            parseVarDeclaration();
        }

        if (peek().type != TokenTypes::KEYWORD_BEGIN) {
            throw runtime_error("Syntax Error: Expected 'begin' after variable declarations, but got " + peek().value);
        }
    }

    void parseVarDeclaration() {
        auto varDeclNode = make_shared<VariableDeclarationNode>();
        varDeclNode->identifierList = parseIdentifierList();
        require({ TokenTypes::COLON }, "':'");
        varDeclNode->type = parseTypeSpecifier();
        currentBlock->push_back(varDeclNode);
        require({ TokenTypes::SEMICOLON }, "';'");
    }

    shared_ptr<IdentifierListNode> parseIdentifierList() {
        auto idListNode = make_shared<IdentifierListNode>();
        Token identifier = require({ TokenTypes::IDENTIFIER }, "identifier");
        idListNode->identifiers.push_back(identifier.value);
        while (match({ TokenTypes::COMMA })) {
            identifier = require({ TokenTypes::IDENTIFIER }, "identifier");
            idListNode->identifiers.push_back(identifier.value);
        }
        return idListNode;
    }
    string parseTypeSpecifier() {
        if (match({ TokenTypes::KEYWORD_INTEGER }))
            return "integer";
        else if (match({ TokenTypes::KEYWORD_DOUBLE }))
            return "double";
        else if (match({ TokenTypes::KEYWORD_STRING }))
            return "string";
        throw runtime_error("Syntax Error: Expected 'integer', 'double' or 'string' but got " + peek().value);
    }

    variant<int, double, string> parseLiteral() {
        if (auto intToken = match({ TokenTypes::INTEGER_LITERAL })) {
            return stoi(intToken->value);
        }
        else if (auto doubleToken = match({ TokenTypes::DOUBLE_LITERAL })) {
            return stod(doubleToken->value);
        }
        else if (auto stringToken = match({ TokenTypes::STRING_LITERAL })) {
            string value = stringToken->value;
            return value;
        }
        else {
            throw runtime_error("Syntax Error: Expected a literal but got " + peek().value);
        }
    }

    void parseBeginStatement() {
        require({ TokenTypes::KEYWORD_BEGIN }, "'begin'");
        while (peek().type != TokenTypes::END_OF_PROGRAM && peek().type != TokenTypes::KEYWORD_END) {
            parseStatement();
        }
        if (peek().type != TokenTypes::END_OF_PROGRAM) {
            require({ TokenTypes::KEYWORD_END }, "'end'");
        }
    }

    void parseStatement() {
        if (peek().type == TokenTypes::IDENTIFIER) {
            parseAssignmentStatement();
        }
        else if (peek().type == TokenTypes::KEYWORD_WRITE) {
            parseWriteStatement();
        }
        else if (peek().type == TokenTypes::KEYWORD_READ) {
            parseReadStatement();
        }
        else if (peek().type == TokenTypes::KEYWORD_IF) {
            parseIfStatement();
        }
        else {
            throw runtime_error("Syntax Error: Unexpected token at start of statement: " + peek().value);
        }
    }

    void parseAssignmentStatement() {
        static const set<TokenTypes> unexpectedTokens = {
            TokenTypes::KEYWORD_BEGIN,
            TokenTypes::KEYWORD_WRITE,
            TokenTypes::KEYWORD_READ,
            TokenTypes::KEYWORD_IF,
            TokenTypes::KEYWORD_END,
            TokenTypes::END_OF_PROGRAM
        };

        Token identifier = require({ TokenTypes::IDENTIFIER }, "variable identifier");
        auto assignNode = make_shared<AssignmentStatementNode>(identifier.value);
        require({ TokenTypes::ASSIGN }, "':='");
        while (peek().type != TokenTypes::SEMICOLON) {
            if (unexpectedTokens.count(peek().type)) {
                throw runtime_error("Syntax Error: Expected ';' after assignment for variable '" + identifier.value + "' at line " + to_string(identifier.line) + ", column " + to_string(identifier.column));
            }
            if (peek().type == TokenTypes::IDENTIFIER && getNextTokenTypes() == TokenTypes::ASSIGN) {
                throw runtime_error("Syntax Error: Expected ';' after assignment for variable '" + identifier.value + "' at line " + to_string(identifier.line) + ", column " + to_string(identifier.column));
            }
            assignNode->expression.push_back(pass());
        }
        if (assignNode->expression.empty()) {
            throw runtime_error("Syntax Error: Expected an expression after ':=' for variable '" + identifier.value + "' at line " + to_string(identifier.line) + ", column " + to_string(identifier.column));
        }
        currentBlock->push_back(assignNode);
        require({ TokenTypes::SEMICOLON }, "';'");
    }

    void parseWriteStatement() {
        static const set<TokenTypes> unexpectedTokensInWrite = {
           TokenTypes::KEYWORD_THEN,
           TokenTypes::KEYWORD_ELSE,
           TokenTypes::KEYWORD_BEGIN,
           TokenTypes::KEYWORD_WRITE,
           TokenTypes::KEYWORD_READ,
           TokenTypes::KEYWORD_IF,
           TokenTypes::KEYWORD_END,
           TokenTypes::END_OF_PROGRAM,
           TokenTypes::SEMICOLON,
           TokenTypes::ASSIGN
        };

        require({ TokenTypes::KEYWORD_WRITE }, "'Write'");
        auto writeNode = make_shared<WriteStatementNode>();
        require({ TokenTypes::LEFT_PAREN }, "'('");
        while (getNextTokenTypes() != TokenTypes::RIGHT_PAREN) {
            if (unexpectedTokensInWrite.count(peek().type)) 
                throw runtime_error("Syntax Error in Write statement: Unexpected token '" + peek().value + "' within the argument list at line " 
                    + to_string(peek().line) + ", column " + to_string(peek().column) + ". Expected an expression or ')'.");
            writeNode->expression.push_back(pass());
        }
        writeNode->expression.push_back(pass());
        require({ TokenTypes::RIGHT_PAREN }, "')'");
        require({ TokenTypes::SEMICOLON }, "';'");
        currentBlock->push_back(writeNode);
    }

    void parseReadStatement() {
        require({ TokenTypes::KEYWORD_READ }, "'Read'");
        auto readNode = make_shared<ReadStatementNode>();
        require({ TokenTypes::LEFT_PAREN }, "'('");
        readNode->identifierList = parseIdentifierList();
        require({ TokenTypes::RIGHT_PAREN }, "')'");
        require({ TokenTypes::SEMICOLON }, "';'");
        currentBlock->push_back(readNode);
    }

    void parseIfStatement() {
        require({ TokenTypes::KEYWORD_IF }, "'if'");
        auto ifNode = make_shared<IfStatementNode>();
        require({ TokenTypes::LEFT_PAREN }, "'('");

        static const set<TokenTypes> unexpectedTokensCondition = {
            TokenTypes::KEYWORD_THEN,
            TokenTypes::KEYWORD_ELSE,
            TokenTypes::KEYWORD_BEGIN,
            TokenTypes::KEYWORD_WRITE,
            TokenTypes::KEYWORD_READ,
            TokenTypes::KEYWORD_IF,
            TokenTypes::KEYWORD_END,
            TokenTypes::END_OF_PROGRAM,
            TokenTypes::SEMICOLON // преждевременная точка с запятой
        };

        while (peek().type != TokenTypes::RIGHT_PAREN) {
            if (unexpectedTokensCondition.count(peek().type)) {
                throw runtime_error("Syntax Error: Expected ')' after 'if' condition at line " + to_string(peek().line) + ", column " + to_string(peek().column));
            }
            ifNode->condition.push_back(pass());
            if (curTokenPos >= tokens.size()) {
                throw runtime_error("Syntax Error: Unexpected end of input while parsing 'if' condition.");
            }
        }
        require({ TokenTypes::RIGHT_PAREN }, "')'");
        require({ TokenTypes::KEYWORD_THEN }, "'then'");
        ifNode->thenStatement = parseStatementBlock();
        if (match({ TokenTypes::KEYWORD_ELSE })) {
            ifNode->elseStatement = parseStatementBlock();
        }
        currentBlock->push_back(ifNode);
    }

    list<shared_ptr<Node>> parseStatementBlock() {
        list<shared_ptr<Node>> block;
        list<shared_ptr<Node>>* previousBlock = currentBlock;
        currentBlock = &block;
        if (peek().type == TokenTypes::KEYWORD_BEGIN) {
            require({ TokenTypes::KEYWORD_BEGIN }, "'begin'");
            auto beginNode = make_shared<BeginSectionNode>();
            block.push_back(beginNode);
            while (peek().type != TokenTypes::KEYWORD_END) {
                parseStatement();
                if (peek().type == TokenTypes::SEMICOLON) {
                    pass();
                }
            }
            require({ TokenTypes::KEYWORD_END }, "'end'");
        }
        else {
            parseStatement();
        }
        currentBlock = previousBlock;
        return block;
    }

public:
    Parser(const vector<Token>& tkns) : tokens(tkns) {};

    list<list<shared_ptr<Node>>>& parse() {
        parseProgram();
        return ast;
    }
};