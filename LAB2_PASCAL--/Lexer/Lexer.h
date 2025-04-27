#pragma once

#include <vector>
#include "../Base/Token.h"
#include <string>
#include <cctype>
#include <variant>
#include <optional>
#include <iostream>
#include <stdexcept>

using namespace std;

enum class LexerState {
    START,
    IDENTIFIER,
    INTEGER,
    DOUBLE,
    STRING,
    ASSIGN // cостояние после ':'
};

class Lexer {
private:
    string sourceCode;
    int curPos = 0;
    int curLine = 1;
    int curColumn = 0;

    string currentLexeme;
    LexerState state = LexerState::START;
    int tokenStartColumn;

    vector<string> keywords = { "program", "const", "var", "begin", "end", "if", "then", "else", "mod", "div", "integer", "double", "Write", "Read" };

    TokenType getKeywordType(const string& lexeme) {
        if (lexeme == "program") return TokenType::KEYWORD_PROGRAM;
        if (lexeme == "const") return TokenType::KEYWORD_CONST;
        if (lexeme == "var") return TokenType::KEYWORD_VAR;
        if (lexeme == "begin") return TokenType::KEYWORD_BEGIN;
        if (lexeme == "end") return TokenType::KEYWORD_END;
        if (lexeme == "if") return TokenType::KEYWORD_IF;
        if (lexeme == "then") return TokenType::KEYWORD_THEN;
        if (lexeme == "else") return TokenType::KEYWORD_ELSE;
        if (lexeme == "mod") return TokenType::KEYWORD_MOD;
        if (lexeme == "div") return TokenType::KEYWORD_DIV;
        if (lexeme == "integer") return TokenType::KEYWORD_INTEGER;
        if (lexeme == "double") return TokenType::KEYWORD_DOUBLE;
        if (lexeme == "Write") return TokenType::KEYWORD_WRITE;
        if (lexeme == "Read") return TokenType::KEYWORD_READ;
        return TokenType::UNKNOWN;
    }

    Token createToken(TokenType type, const string& value) const {
        return Token(type, value, curLine, tokenStartColumn);
    }

    optional<Token> nextToken() {
        while (curPos < sourceCode.length()) {
            char currentChar = sourceCode[curPos];

            switch (state) {
            case LexerState::START:
                currentLexeme.clear();
                tokenStartColumn = curColumn + 1;

                if (isspace(currentChar)) {
                    if (currentChar == '\n') {
                        curLine++;
                        curColumn = 0;
                    }
                    else {
                        curColumn++;
                    }
                    curPos++;
                }
                else if (isalpha(currentChar) || currentChar == '_') {
                    state = LexerState::IDENTIFIER;
                    currentLexeme += currentChar;
                    curPos++;
                    curColumn++;
                }
                else if (isdigit(currentChar)) {
                    state = LexerState::INTEGER;
                    currentLexeme += currentChar;
                    curPos++;
                    curColumn++;
                }
                else if (currentChar == '"') {
                    state = LexerState::STRING;
                    currentLexeme += currentChar;
                    curPos++;
                    curColumn++;
                }
                else if (currentChar == ':') {
                    state = LexerState::ASSIGN;
                    currentLexeme += currentChar;
                    curPos++;
                    curColumn++;
                }
                else {
                    currentLexeme += currentChar;
                    curPos++;
                    curColumn++;
                    switch (currentChar) {
                    case '+': return createToken(TokenType::PLUS, currentLexeme);
                    case '-': return createToken(TokenType::MINUS, currentLexeme);
                    case '*': return createToken(TokenType::MULTIPLY, currentLexeme);
                    case '/': return createToken(TokenType::DIVIDE, currentLexeme);
                    case '=': return createToken(TokenType::EQUAL, currentLexeme);
                    case ';': return createToken(TokenType::SEMICOLON, currentLexeme);
                    case ',': return createToken(TokenType::COMMA, currentLexeme);
                    case '(': return createToken(TokenType::LEFT_PAREN, currentLexeme);
                    case ')': return createToken(TokenType::RIGHT_PAREN, currentLexeme);
                    case '.': return createToken(TokenType::UNKNOWN, currentLexeme); // Точка сама по себе
                    default:
                        throw runtime_error("Error: Unknown token '" + currentLexeme + "' at line " + to_string(curLine) + ", column " + to_string(tokenStartColumn));
                    }
                }
                break;

            case LexerState::IDENTIFIER:
                if (isalnum(currentChar) || currentChar == '_') {
                    currentLexeme += currentChar;
                    curPos++;
                    curColumn++;
                }
                else {
                    state = LexerState::START;
                    if (currentLexeme == "end" && curPos < sourceCode.length() && sourceCode[curPos] == '.') {
                        currentLexeme += sourceCode[curPos];
                        curPos++;
                        curColumn++;
                        return createToken(TokenType::END_OF_PROGRAM, currentLexeme);
                    }
                    else {
                        TokenType type = getKeywordType(currentLexeme);
                        if (type == TokenType::UNKNOWN) {
                            type = TokenType::IDENTIFIER;
                        }
                        return createToken(type, currentLexeme);
                    }
                }
                break;

            case LexerState::INTEGER:
                if (isdigit(currentChar)) {
                    currentLexeme += currentChar;
                    curPos++;
                    curColumn++;
                }
                else if (currentChar == '.') {
                    state = LexerState::DOUBLE;
                    currentLexeme += currentChar;
                    curPos++;
                    curColumn++;
                }
                else {
                    state = LexerState::START;
                    return createToken(TokenType::INTEGER_LITERAL, currentLexeme);
                }
                break;

            case LexerState::DOUBLE:
                if (isdigit(currentChar)) {
                    currentLexeme += currentChar;
                    curPos++;
                    curColumn++;
                }
                else {
                    state = LexerState::START;
                    return createToken(TokenType::DOUBLE_LITERAL, currentLexeme);
                }
                break;

            case LexerState::STRING:
                currentLexeme += currentChar;
                curPos++;
                curColumn++;
                if (currentChar == '"') {
                    state = LexerState::START;
                    return createToken(TokenType::STRING_LITERAL, currentLexeme);
                }
                if (curPos >= sourceCode.length()) {
                    throw runtime_error("Error: Unterminated string literal at line " + to_string(curLine) + ", column " + to_string(tokenStartColumn));
                }
                break;

            case LexerState::ASSIGN:
                currentLexeme += currentChar;
                curPos++;
                curColumn++;
                state = LexerState::START;
                if (currentChar == '=') {
                    return createToken(TokenType::ASSIGN, currentLexeme);
                }
                else {
                    return createToken(TokenType::COLON, currentLexeme.substr(0, 1)); // Вернуть только ':'
                }
            }
        }
        return nullopt; // Достигнут конец файла
    }

public:
    Lexer(const string& source) : sourceCode(source) {}

    vector<Token> tokenize() {
        vector<Token> tokens;
        optional<Token> token;
        while ((token = nextToken()).has_value()) {
            tokens.push_back(token.value());
        }
        return tokens;
    }
};