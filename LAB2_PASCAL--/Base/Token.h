#pragma once

#include <string>

using namespace std;



enum class TokenTypes {
    IDENTIFIER,       // имена переменных
    INTEGER_LITERAL,  // переменная типа int
    DOUBLE_LITERAL,   // переменная типа double
    STRING_LITERAL,   // переменная типа string
    PLUS,             // '+'
    MINUS,            // '-'
    MULTIPLY,         // '*'
    DIVIDE,           // '/'
    ASSIGN,           // ':='
    EQUAL,            // '='
    NON_EQUAL,        // '<>'
    LESS,             // '<'
    GREATER,          // '>'
    LESS_OR_EQUAL,    // '<='
    GREATER_OR_EQUAL, // '>='
    SEMICOLON,        // ';'
    COLON,            // ':' 
    COMMA,            // ','
    KEYWORD_PROGRAM,  // 'program'
    KEYWORD_CONST,    // 'const'
    KEYWORD_VAR,      // 'var'
    KEYWORD_BEGIN,    // 'begin'
    KEYWORD_END,      // 'end'
    KEYWORD_IF,       // 'if'
    KEYWORD_THEN,     // 'then'
    KEYWORD_ELSE,     // 'else'
    KEYWORD_MOD,      // 'mod'
    KEYWORD_DIV,      // 'div'
    KEYWORD_WRITE,    // 'Write'
    KEYWORD_READ,     // 'Read'
    LEFT_PAREN,       // '('
    RIGHT_PAREN,      // ')'
    KEYWORD_INTEGER,  // 'integer'
    KEYWORD_DOUBLE,   // 'double'
    KEYWORD_STRING,   // 'string'
    END_OF_PROGRAM,   // 'end.'
    UNKNOWN           // неизвестный тип
};

struct Token {
    TokenTypes type;
    string value;
    int line;
    int column;
    Token(TokenTypes tt = TokenTypes::UNKNOWN, string val = "", int l = -1, int c = -1) : type(tt), value(val), line(l), column(c) {}
};