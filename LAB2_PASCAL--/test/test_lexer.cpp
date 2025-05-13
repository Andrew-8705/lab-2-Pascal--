#include "pch.h"
#include "../Base/Token.h"
#include "../Lexer/Lexer.h"
#include <vector>
#include <string>
#include <iostream>

using namespace std;

bool CompareTokens(const vector<Token>& expected, const vector<Token>& actual) {
    cout << expected.size() << ' ' << actual.size() << '\n';
    if (expected.size() != actual.size()) return false;
    for (size_t i = 0; i < expected.size(); ++i) {
        if (expected[i].type != actual[i].type ||
            expected[i].value != actual[i].value ||
            expected[i].line != actual[i].line ||
            expected[i].column != actual[i].column) {
            cout << "IIIII: " << i << '\n';
            return false;
        }
    }
    return true;
}

TEST(LexerTest, can_tokenize_empty_input) {
    Lexer lexer("");
    EXPECT_TRUE(lexer.tokenize().empty());
}

TEST(LexerTest, can_tokenize_keywords) {
    string source = "program const var begin end if then else mod div integer double string Write Read";
    Lexer lexer(source);
    vector<Token> expectedTokens = {
        {TokenType::KEYWORD_PROGRAM, "program", 1, 1},
        {TokenType::KEYWORD_CONST, "const", 1, 9},
        {TokenType::KEYWORD_VAR, "var", 1, 15},
        {TokenType::KEYWORD_BEGIN, "begin", 1, 19},
        {TokenType::KEYWORD_END, "end", 1, 25},
        {TokenType::KEYWORD_IF, "if", 1, 29},
        {TokenType::KEYWORD_THEN, "then", 1, 32},
        {TokenType::KEYWORD_ELSE, "else", 1, 37},
        {TokenType::KEYWORD_MOD, "mod", 1, 42},
        {TokenType::KEYWORD_DIV, "div", 1, 46},
        {TokenType::KEYWORD_INTEGER, "integer", 1, 50},
        {TokenType::KEYWORD_DOUBLE, "double", 1, 58},
        {TokenType::KEYWORD_STRING, "string", 1, 65},
        {TokenType::KEYWORD_WRITE, "Write", 1, 72},
        {TokenType::KEYWORD_READ, "Read", 1, 78}
    };
    EXPECT_TRUE(CompareTokens(expectedTokens, lexer.tokenize()));
}

TEST(LexerTest, can_tokenize_identifiers) {
    string source = "variableName another_one _underscore var123";
    Lexer lexer(source);
    vector<Token> expectedTokens = {
        {TokenType::IDENTIFIER, "variableName", 1, 1},
        {TokenType::IDENTIFIER, "another_one", 1, 14},
        {TokenType::IDENTIFIER, "_underscore", 1, 26},
        {TokenType::IDENTIFIER, "var123", 1, 38}
    };
    EXPECT_TRUE(CompareTokens(expectedTokens, lexer.tokenize()));
}

TEST(LexerTest, can_tokenize_integer_literals) {
    string source = "123 0 9876543210";
    Lexer lexer(source);
    vector<Token> expectedTokens = {
        {TokenType::INTEGER_LITERAL, "123", 1, 1},
        {TokenType::INTEGER_LITERAL, "0", 1, 5},
        {TokenType::INTEGER_LITERAL, "9876543210", 1, 7}
    };
    EXPECT_TRUE(CompareTokens(expectedTokens, lexer.tokenize()));
}

TEST(LexerTest, can_tokenize_double_literals) {
    string source = "123.45 0.0 3.14159";
    Lexer lexer(source);
    vector<Token> expectedTokens = {
        {TokenType::DOUBLE_LITERAL, "123.45", 1, 1},
        {TokenType::DOUBLE_LITERAL, "0.0", 1, 8},
        {TokenType::DOUBLE_LITERAL, "3.14159", 1, 12}
    };
    EXPECT_TRUE(CompareTokens(expectedTokens, lexer.tokenize()));
}

TEST(LexerTest, can_tokenize_string_literals) {
    string source = "\"hello\" \"world\" \" with spaces  \"";
    Lexer lexer(source);
    vector<Token> expectedTokens = {
        {TokenType::STRING_LITERAL, "hello", 1, 1},
        {TokenType::STRING_LITERAL, "world", 1, 9},
        {TokenType::STRING_LITERAL, " with spaces  ", 1, 17}
    };
    EXPECT_TRUE(CompareTokens(expectedTokens, lexer.tokenize()));
}

TEST(LexerTest, can_tokenize_operators) {
    string source = "+ - * / :=";
    Lexer lexer(source);
    vector<Token> expectedTokens = {
        {TokenType::PLUS, "+", 1, 1},
        {TokenType::MINUS, "-", 1, 3},
        {TokenType::MULTIPLY, "*", 1, 5},
        {TokenType::DIVIDE, "/", 1, 7},
        {TokenType::ASSIGN, ":=", 1, 9},
    };
    EXPECT_TRUE(CompareTokens(expectedTokens, lexer.tokenize()));
}

TEST(LexerTest, can_tokenize_comparison_operators) {
    string source = "= <> > < >= <=";
    Lexer lexer(source);
    vector<Token> expectedTokens = {
        {TokenType::EQUAL, "=", 1, 1},
        {TokenType::NON_EQUAL, "<>", 1, 3},
        {TokenType::GREATER, ">", 1, 6},
        {TokenType::LESS, "<", 1, 8},
        {TokenType::GREATER_OR_EQUAL, ">=", 1, 10},
        {TokenType::LESS_OR_EQUAL, "<=", 1, 13}
    };
    ASSERT_TRUE(CompareTokens(expectedTokens, lexer.tokenize()));
}

TEST(LexerTest, can_tokenize_separators) {
    string source = "; , ( )";
    Lexer lexer(source);
    vector<Token> expectedTokens = {
        {TokenType::SEMICOLON, ";", 1, 1},
        {TokenType::COMMA, ",", 1, 3},
        {TokenType::LEFT_PAREN, "(", 1, 5},
        {TokenType::RIGHT_PAREN, ")", 1, 7}
    };
    EXPECT_TRUE(CompareTokens(expectedTokens, lexer.tokenize()));
}
