#pragma once

#include <string>

using namespace std;



enum class TokenType {
    IDENTIFIER,       // ����� ����������
    INTEGER_LITERAL,  // ���������� ���� int
    DOUBLE_LITERAL,   // ���������� ���� double
    STRING_LITERAL,   // ���������� ���� string
    PLUS,             // '+'
    MINUS,            // '-'
    MULTIPLY,         // '*'
    DIVIDE,           // '/'
    ASSIGN,           // ':='
    EQUAL,            // '='
    NON_EQUAL,        // '<>'
    LESS,             // '<' <- TODO
    GREATER,          // '>' <- TODO
    LESS_OR_EQUAL,    // '<=' <- TODO
    GREATER_OR_EQUAL, // '>=' <- TODO
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
    UNKNOWN           // ����������� ���
};

struct Token {
    TokenType type;
    string value;
    int line;
    int column;
    Token(TokenType tt = TokenType::UNKNOWN, string val = "", int l = -1, int c = -1) : type(tt), value(val), line(l), column(c) {}
};