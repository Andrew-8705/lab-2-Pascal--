#pragma once

#include <string>

using namespace std;

enum class TokenType {
    IDENTIFIER,
    INTEGER_LITERAL,
    DOUBLE_LITERAL,
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    ASSIGN,
    SEMICOLON, // ';'
    COLON, // ':' 
    COMMA, // ','
    KEYWORD_PROGRAM,
    KEYWORD_CONST,
    KEYWORD_VAR,
    KEYWORD_BEGIN,
    KEYWORD_END,
    KEYWORD_IF,
    KEYWORD_THEN,
    KEYWORD_ELSE,
    KEYWORD_MOD,
    KEYWORD_DIV,
    KEYWORD_WRITE,
    KEYWORD_READ,
    LEFT_PAREN,
    RIGHT_PAREN,
    STRING_LITERAL,
    UNKNOWN
};

struct Token {
    TokenType type;
    string value;
    int line;
    int column;
    Token(TokenType tt = TokenType::UNKNOWN, string val = "", int l = -1, int c = -1) : type(tt), value(val), line(l), column(c) {}
};