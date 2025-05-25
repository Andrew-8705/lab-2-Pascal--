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
    START,            // стартовое состо€ние
    IDENTIFIER,       // обработка имЄн переменных
    INTEGER,          // обработка целых чисел
    DOUBLE,           // обработка чисел с плавающей точкой
    STRING,           // обработка строк
    ASSIGN,           // cосто€ние после ':'
    GREATER_THAN,     // состо€ние после '>'
    LESS_THAN         // состо€ние после '<'
};

class Lexer {
private:
    string sourceCode;
    int curPos;
    int curLine;
    int curColumn;

    string currentLexeme;
    LexerState state = LexerState::START;
    int tokenStartColumn;

    vector<string> keywords = { "program", "const", "var", "begin", "end", "if", "then", "else", "mod", "div", "integer", "double", "string", "Write", "Read" };

    TokenTypes getKeywordType(const string& lexeme) {
        if (lexeme == "program") return TokenTypes::KEYWORD_PROGRAM;
        if (lexeme == "const") return TokenTypes::KEYWORD_CONST;
        if (lexeme == "var") return TokenTypes::KEYWORD_VAR;
        if (lexeme == "begin") return TokenTypes::KEYWORD_BEGIN;
        if (lexeme == "end") return TokenTypes::KEYWORD_END;
        if (lexeme == "if") return TokenTypes::KEYWORD_IF;
        if (lexeme == "then") return TokenTypes::KEYWORD_THEN;
        if (lexeme == "else") return TokenTypes::KEYWORD_ELSE;
        if (lexeme == "mod") return TokenTypes::KEYWORD_MOD;
        if (lexeme == "div") return TokenTypes::KEYWORD_DIV;
        if (lexeme == "integer") return TokenTypes::KEYWORD_INTEGER;
        if (lexeme == "double") return TokenTypes::KEYWORD_DOUBLE;
        if (lexeme == "string") return TokenTypes::KEYWORD_STRING;
        if (lexeme == "Write") return TokenTypes::KEYWORD_WRITE;
        if (lexeme == "Read") return TokenTypes::KEYWORD_READ;
        return TokenTypes::UNKNOWN;
    }

    Token createToken(TokenTypes type, const string& value) const {
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
                    curPos++;
                    curColumn++;
                }
                else if (currentChar == ':') {
                    state = LexerState::ASSIGN;
                    currentLexeme += currentChar;
                    curPos++;
                    curColumn++;
                }
                else if (currentChar == '>') {
                    state = LexerState::GREATER_THAN;
                    currentLexeme += currentChar;
                    curPos++;
                    curColumn++;
                }
                else if (currentChar == '<') {
                    state = LexerState::LESS_THAN;
                    currentLexeme += currentChar;
                    curPos++;
                    curColumn++;
                }
                else {
                    currentLexeme += currentChar;
                    curPos++;
                    curColumn++;
                    switch (currentChar) {
                    case '+': return createToken(TokenTypes::PLUS, currentLexeme);
                    case '-': return createToken(TokenTypes::MINUS, currentLexeme);
                    case '*': return createToken(TokenTypes::MULTIPLY, currentLexeme);
                    case '/': return createToken(TokenTypes::DIVIDE, currentLexeme);
                    case '=': return createToken(TokenTypes::EQUAL, currentLexeme);
                    case ';': return createToken(TokenTypes::SEMICOLON, currentLexeme);
                    case ',': return createToken(TokenTypes::COMMA, currentLexeme);
                    case '(': return createToken(TokenTypes::LEFT_PAREN, currentLexeme);
                    case ')': return createToken(TokenTypes::RIGHT_PAREN, currentLexeme);
                    case '.': return createToken(TokenTypes::UNKNOWN, currentLexeme); // “очка сама по себе
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
                        return createToken(TokenTypes::END_OF_PROGRAM, currentLexeme);
                    }
                    else {
                        TokenTypes type = getKeywordType(currentLexeme);
                        if (type == TokenTypes::UNKNOWN) {
                            type = TokenTypes::IDENTIFIER;
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
                    return createToken(TokenTypes::INTEGER_LITERAL, currentLexeme);
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
                    return createToken(TokenTypes::DOUBLE_LITERAL, currentLexeme);
                }
                break;

            case LexerState::STRING:
                curPos++;
                curColumn++;
                if (currentChar == '"') {
                    state = LexerState::START;
                    return createToken(TokenTypes::STRING_LITERAL, currentLexeme);
                }
                currentLexeme += currentChar;
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
                    return createToken(TokenTypes::ASSIGN, currentLexeme);
                }
                else {
                    return createToken(TokenTypes::COLON, currentLexeme.substr(0, 1)); // ¬ернуть только ':'
                }

            case LexerState::GREATER_THAN:
                if (curPos < sourceCode.length() && sourceCode[curPos] == '=') {
                    currentLexeme += sourceCode[curPos];
                    curPos++;
                    curColumn++;
                    state = LexerState::START;
                    return createToken(TokenTypes::GREATER_OR_EQUAL, currentLexeme);
                }
                else {
                    state = LexerState::START;
                    return createToken(TokenTypes::GREATER, currentLexeme);
                }
                break;

            case LexerState::LESS_THAN:
                if (curPos < sourceCode.length() && sourceCode[curPos] == '=') {
                    currentLexeme += sourceCode[curPos];
                    curPos++;
                    curColumn++;
                    state = LexerState::START;
                    return createToken(TokenTypes::LESS_OR_EQUAL, currentLexeme);
                }
                else if (curPos < sourceCode.length() && sourceCode[curPos] == '>') {
                    currentLexeme += sourceCode[curPos];
                    curPos++;
                    curColumn++;
                    state = LexerState::START;
                    return createToken(TokenTypes::NON_EQUAL, currentLexeme);
                }
                else {
                    state = LexerState::START;
                    return createToken(TokenTypes::LESS, currentLexeme);
                }
                break;
            }
        }

        if (!currentLexeme.empty()) {
            switch (state) {
            case LexerState::IDENTIFIER: {
                TokenTypes type = getKeywordType(currentLexeme);
                if (type == TokenTypes::UNKNOWN) {
                    type = TokenTypes::IDENTIFIER;
                }
                state = LexerState::START;
                return createToken(type, currentLexeme);
            }
            case LexerState::INTEGER:
                state = LexerState::START;
                return createToken(TokenTypes::INTEGER_LITERAL, currentLexeme);
            case LexerState::DOUBLE:
                state = LexerState::START;
                return createToken(TokenTypes::DOUBLE_LITERAL, currentLexeme);
            case LexerState::STRING:
                throw runtime_error("Error: Unterminated string literal at line " + to_string(curLine) + ", column " + to_string(tokenStartColumn));
            case LexerState::ASSIGN:
                state = LexerState::START;
                return createToken(TokenTypes::COLON, currentLexeme.substr(0, 1)); // обработка ':' без '='
            default:
                // в состо€нии START ничего не должно оставатьс€
                break;
            }
        }
        return nullopt; // ƒостигнут конец файла
    }

public:
    Lexer(const string& source) : sourceCode(source), curPos(0), curLine(1), curColumn(0) {}

    vector<Token> tokenize() {
        vector<Token> tokens;
        optional<Token> token;
        curPos = 0;
        curLine = 1;
        curColumn = 0;
        while ((token = nextToken()).has_value()) {
            tokens.push_back(token.value());
        }
        return tokens;
    }
};