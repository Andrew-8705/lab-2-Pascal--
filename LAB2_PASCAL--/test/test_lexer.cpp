#include "pch.h"
#include "../Base/Token.h"
#include "../Lexer/Lexer.h"
#include <vector>
#include <string>
#include <iostream>

using namespace std;

bool CompareTokens(const vector<Token>& expected, const vector<Token>& actual) {
    //cout << expected.size() << ' ' << actual.size() << '\n';
    if (expected.size() != actual.size()) return false;
    for (size_t i = 0; i < expected.size(); ++i) {
        if (expected[i].type != actual[i].type ||
            expected[i].value != actual[i].value ||
            expected[i].line != actual[i].line ||
            expected[i].column != actual[i].column) {
            //cout << "IIIII: " << i << '\n';
            //cout << (int)expected[i].type << ' ' << (int)actual[i].type << '\n';
            //cout << expected[i].value << ' ' << actual[i].value << '\n';
            //cout << expected[i].line << ' ' << actual[i].line << '\n';
            //cout << expected[i].column << ' ' << actual[i].column << '\n';
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
        {TokenTypes::KEYWORD_PROGRAM, "program", 1, 1},
        {TokenTypes::KEYWORD_CONST, "const", 1, 9},
        {TokenTypes::KEYWORD_VAR, "var", 1, 15},
        {TokenTypes::KEYWORD_BEGIN, "begin", 1, 19},
        {TokenTypes::KEYWORD_END, "end", 1, 25},
        {TokenTypes::KEYWORD_IF, "if", 1, 29},
        {TokenTypes::KEYWORD_THEN, "then", 1, 32},
        {TokenTypes::KEYWORD_ELSE, "else", 1, 37},
        {TokenTypes::KEYWORD_MOD, "mod", 1, 42},
        {TokenTypes::KEYWORD_DIV, "div", 1, 46},
        {TokenTypes::KEYWORD_INTEGER, "integer", 1, 50},
        {TokenTypes::KEYWORD_DOUBLE, "double", 1, 58},
        {TokenTypes::KEYWORD_STRING, "string", 1, 65},
        {TokenTypes::KEYWORD_WRITE, "Write", 1, 72},
        {TokenTypes::KEYWORD_READ, "Read", 1, 78}
    };
    EXPECT_TRUE(CompareTokens(expectedTokens, lexer.tokenize()));
}

TEST(LexerTest, can_tokenize_identifiers) {
    string source = "variableName another_one _underscore var123";
    Lexer lexer(source);
    vector<Token> expectedTokens = {
        {TokenTypes::IDENTIFIER, "variableName", 1, 1},
        {TokenTypes::IDENTIFIER, "another_one", 1, 14},
        {TokenTypes::IDENTIFIER, "_underscore", 1, 26},
        {TokenTypes::IDENTIFIER, "var123", 1, 38}
    };
    EXPECT_TRUE(CompareTokens(expectedTokens, lexer.tokenize()));
}

TEST(LexerTest, can_tokenize_integer_literals) {
    string source = "123 0 9876543210";
    Lexer lexer(source);
    vector<Token> expectedTokens = {
        {TokenTypes::INTEGER_LITERAL, "123", 1, 1},
        {TokenTypes::INTEGER_LITERAL, "0", 1, 5},
        {TokenTypes::INTEGER_LITERAL, "9876543210", 1, 7}
    };
    EXPECT_TRUE(CompareTokens(expectedTokens, lexer.tokenize()));
}

TEST(LexerTest, can_tokenize_double_literals) {
    string source = "123.45 0.0 3.14159";
    Lexer lexer(source);
    vector<Token> expectedTokens = {
        {TokenTypes::DOUBLE_LITERAL, "123.45", 1, 1},
        {TokenTypes::DOUBLE_LITERAL, "0.0", 1, 8},
        {TokenTypes::DOUBLE_LITERAL, "3.14159", 1, 12}
    };
    EXPECT_TRUE(CompareTokens(expectedTokens, lexer.tokenize()));
}

TEST(LexerTest, can_tokenize_string_literals) {
    string source = "\"hello\" \"world\" \" with spaces  \"";
    Lexer lexer(source);
    vector<Token> expectedTokens = {
        {TokenTypes::STRING_LITERAL, "hello", 1, 1},
        {TokenTypes::STRING_LITERAL, "world", 1, 9},
        {TokenTypes::STRING_LITERAL, " with spaces  ", 1, 17}
    };
    EXPECT_TRUE(CompareTokens(expectedTokens, lexer.tokenize()));
}

TEST(LexerTest, can_tokenize_operators) {
    string source = "+ - * / :=";
    Lexer lexer(source);
    vector<Token> expectedTokens = {
        {TokenTypes::PLUS, "+", 1, 1},
        {TokenTypes::MINUS, "-", 1, 3},
        {TokenTypes::MULTIPLY, "*", 1, 5},
        {TokenTypes::DIVIDE, "/", 1, 7},
        {TokenTypes::ASSIGN, ":=", 1, 9},
    };
    EXPECT_TRUE(CompareTokens(expectedTokens, lexer.tokenize()));
}

TEST(LexerTest, can_tokenize_comparison_operators) {
    string source = "= <> > < >= <=";
    Lexer lexer(source);
    vector<Token> expectedTokens = {
        {TokenTypes::EQUAL, "=", 1, 1},
        {TokenTypes::NON_EQUAL, "<>", 1, 3},
        {TokenTypes::GREATER, ">", 1, 6},
        {TokenTypes::LESS, "<", 1, 8},
        {TokenTypes::GREATER_OR_EQUAL, ">=", 1, 10},
        {TokenTypes::LESS_OR_EQUAL, "<=", 1, 13}
    };
    EXPECT_TRUE(CompareTokens(expectedTokens, lexer.tokenize()));
}

TEST(LexerTest, can_tokenize_separators) {
    string source = "; , ( )";
    Lexer lexer(source);
    vector<Token> expectedTokens = {
        {TokenTypes::SEMICOLON, ";", 1, 1},
        {TokenTypes::COMMA, ",", 1, 3},
        {TokenTypes::LEFT_PAREN, "(", 1, 5},
        {TokenTypes::RIGHT_PAREN, ")", 1, 7}
    };
    EXPECT_TRUE(CompareTokens(expectedTokens, lexer.tokenize()));
}

TEST(LexerTest, can_ignore_whitespace) {
    string source = "  program\n\tvar \n begin";
    Lexer lexer(source);
    vector<Token> expectedTokens = {
        {TokenTypes::KEYWORD_PROGRAM, "program", 1, 3},
        {TokenTypes::KEYWORD_VAR, "var", 2, 2},
        {TokenTypes::KEYWORD_BEGIN, "begin", 3, 2}
    };
    EXPECT_TRUE(CompareTokens(expectedTokens, lexer.tokenize()));
}

TEST(LexerTest, can_tokenize_end_of_program) {
    string source = "end.";
    Lexer lexer(source);
    vector<Token> expectedTokens1 = {
        {TokenTypes::END_OF_PROGRAM, "end.", 1, 1}
    };
    EXPECT_TRUE(CompareTokens(expectedTokens1, lexer.tokenize()));

    source = "end .";
    lexer = Lexer(source);
    vector<Token> expectedTokens2 = {
        {TokenTypes::KEYWORD_END, "end", 1, 1},
        {TokenTypes::UNKNOWN, ".", 1, 5}
    };
    EXPECT_TRUE(CompareTokens(expectedTokens2, lexer.tokenize()));
}

TEST(LexerTest, can_tokenize_mixed_tokens) {
    string source = "program myProgram; var counter: integer; begin Write(\"hello\"); end.";
    Lexer lexer(source);
    vector<Token> expectedTokens = {
        {TokenTypes::KEYWORD_PROGRAM, "program", 1, 1},
        {TokenTypes::IDENTIFIER, "myProgram", 1, 9},
        {TokenTypes::SEMICOLON, ";", 1, 18},
        {TokenTypes::KEYWORD_VAR, "var", 1, 20},
        {TokenTypes::IDENTIFIER, "counter", 1, 24},
        {TokenTypes::COLON, ":", 1, 31},
        {TokenTypes::KEYWORD_INTEGER, "integer", 1, 33},
        {TokenTypes::SEMICOLON, ";", 1, 40},
        {TokenTypes::KEYWORD_BEGIN, "begin", 1, 42},
        {TokenTypes::KEYWORD_WRITE, "Write", 1, 48},
        {TokenTypes::LEFT_PAREN, "(", 1, 53},
        {TokenTypes::STRING_LITERAL, "hello", 1, 54},
        {TokenTypes::RIGHT_PAREN, ")", 1, 61},
        {TokenTypes::SEMICOLON, ";", 1, 62},
        {TokenTypes::END_OF_PROGRAM, "end.", 1, 64}
    };
    EXPECT_TRUE(CompareTokens(expectedTokens, lexer.tokenize()));
}

TEST(LexerTest, throws_error_for_unterminated_string) {
    Lexer lexer("\"hello");
    EXPECT_THROW(lexer.tokenize(), runtime_error);
}

TEST(LexerTest, throws_error_for_unknown_token) {
    Lexer lexer("#");
    ASSERT_THROW(lexer.tokenize(), runtime_error);
}

