#pragma once

#include <string>

using namespace std;

enum class TokenType {
    IDENTIFIER,
    INTEGER_LITERAL,
    DOUBLE_LITERAL,
    STRING_LITERAL,
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    ASSIGN,
    EQUAL,
    NON_EQUAL,
    LESS,//TODO
    GREATER,// TODO
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
    KEYWORD_INTEGER,
    KEYWORD_DOUBLE,
    KEYWORD_STRING,
    END_OF_PROGRAM,
    UNKNOWN,
    END_OF_FILE
};

struct Token {
    TokenType type;
    string value;
    int line;
    int column;
    Token(TokenType tt = TokenType::UNKNOWN, string val = "", int l = -1, int c = -1) : type(tt), value(val), line(l), column(c) {}
};