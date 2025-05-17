#include "pch.h"
#include "../ExpressionEvaluator/PostfixConverter.h"
#include "../ExpressionEvaluator/PostfixConverter.cpp"
#include "../Base/Token.h"
#include <vector>

using namespace std;

TEST(PostfixConverterTest, handles_single_integer_literal) {
    vector<Token> tokens = { { TokenType::INTEGER_LITERAL, "10" } };
    vector<Token> postfix = PostfixConverter::Convert(tokens);
    ASSERT_EQ(postfix.size(), 1);
    EXPECT_EQ(postfix[0].type, TokenType::INTEGER_LITERAL);
    EXPECT_EQ(postfix[0].value, "10");
}

TEST(PostfixConverterTest, handles_single_identifier) {
    vector<Token> tokens = { { TokenType::IDENTIFIER, "x"} };
    vector<Token> postfix = PostfixConverter::Convert(tokens);
    ASSERT_EQ(postfix.size(), 1);
    EXPECT_EQ(postfix[0].type, TokenType::IDENTIFIER);
    EXPECT_EQ(postfix[0].value, "x");
}

TEST(PostfixConverterTest, handles_addition) {
    vector<Token> tokens = {
        { TokenType::INTEGER_LITERAL, "2" },
        { TokenType::PLUS, "+" },
        { TokenType::INTEGER_LITERAL, "3" }
    };
    vector<Token> postfix = PostfixConverter::Convert(tokens);
    ASSERT_EQ(postfix.size(), 3);
    EXPECT_EQ(postfix[0].type, TokenType::INTEGER_LITERAL);
    EXPECT_EQ(postfix[0].value, "2");
    EXPECT_EQ(postfix[1].type, TokenType::INTEGER_LITERAL);
    EXPECT_EQ(postfix[1].value, "3");
    EXPECT_EQ(postfix[2].type, TokenType::PLUS);
    EXPECT_EQ(postfix[2].value, "+");
}

TEST(PostfixConverterTest, handles_multiplication_precedence) {
    vector<Token> tokens = {
        { TokenType::INTEGER_LITERAL, "2" },
        { TokenType::PLUS, "+" },
        { TokenType::INTEGER_LITERAL, "3" },
        { TokenType::MULTIPLY, "*" },
        { TokenType::INTEGER_LITERAL, "4" }
    };
    vector<Token> postfix = PostfixConverter::Convert(tokens);
    ASSERT_EQ(postfix.size(), 5);
    EXPECT_EQ(postfix[0].value, "2");
    EXPECT_EQ(postfix[1].value, "3");
    EXPECT_EQ(postfix[2].value, "4");
    EXPECT_EQ(postfix[3].value, "*");
    EXPECT_EQ(postfix[4].value, "+");
}

TEST(PostfixConverterTest, handles_parentheses) {
    vector<Token> tokens = {
        { TokenType::LEFT_PAREN, "(" },
        { TokenType::INTEGER_LITERAL, "2" },
        { TokenType::PLUS, "+" },
        { TokenType::INTEGER_LITERAL, "3" },
        { TokenType::RIGHT_PAREN, ")" },
        { TokenType::MULTIPLY, "*" },
        { TokenType::INTEGER_LITERAL, "4" }
    };
    vector<Token> postfix = PostfixConverter::Convert(tokens);
    ASSERT_EQ(postfix.size(), 5);
    EXPECT_EQ(postfix[0].value, "2");
    EXPECT_EQ(postfix[1].value, "3");
    EXPECT_EQ(postfix[2].value, "+");
    EXPECT_EQ(postfix[3].value, "4");
    EXPECT_EQ(postfix[4].value, "*");
}

TEST(PostfixConverterTest, handles_left_associativity) {
    vector<Token> tokens = {
        { TokenType::INTEGER_LITERAL, "10" },
        { TokenType::MINUS, "-" },
        { TokenType::INTEGER_LITERAL, "5" },
        { TokenType::MINUS, "-" },
        { TokenType::INTEGER_LITERAL, "2" }
    };
    vector<Token> postfix = PostfixConverter::Convert(tokens);
    ASSERT_EQ(postfix.size(), 5);
    EXPECT_EQ(postfix[0].value, "10");
    EXPECT_EQ(postfix[1].value, "5");
    EXPECT_EQ(postfix[2].value, "-");
    EXPECT_EQ(postfix[3].value, "2");
    EXPECT_EQ(postfix[4].value, "-");
}

TEST(PostfixConverterTest, handles_div_operator) {
    vector<Token> tokens = {
        { TokenType::INTEGER_LITERAL, "10" },
        { TokenType::KEYWORD_DIV, "div" },
        { TokenType::INTEGER_LITERAL, "3" }
    };
    vector<Token> postfix = PostfixConverter::Convert(tokens);
    ASSERT_EQ(postfix.size(), 3);
    EXPECT_EQ(postfix[2].value, "div");
}

TEST(PostfixConverterTest, handles_mod_operator) {
    vector<Token> tokens = {
        { TokenType::INTEGER_LITERAL, "10" },
        { TokenType::KEYWORD_MOD, "mod" },
        { TokenType::INTEGER_LITERAL, "3" }
    };
    vector<Token> postfix = PostfixConverter::Convert(tokens);
    ASSERT_EQ(postfix.size(), 3);
    EXPECT_EQ(postfix[2].value, "mod");
}

TEST(PostfixConverterTest, handles_unary_minus_literal) {
    vector<Token> tokens = {
        { TokenType::MINUS, "-" },
        { TokenType::INTEGER_LITERAL, "5" }
    };
    vector<Token> postfix = PostfixConverter::Convert(tokens);
    ASSERT_EQ(postfix.size(), 2);
    EXPECT_EQ(postfix[0].type, TokenType::INTEGER_LITERAL);
    EXPECT_EQ(postfix[0].value, "5");
    EXPECT_EQ(postfix[1].value, "_");
}

TEST(PostfixConverterTest, handles_unary_minus_identifier) {
    vector<Token> tokens = {
        { TokenType::MINUS, "-" },
        { TokenType::IDENTIFIER, "y" }
    };
    vector<Token> postfix = PostfixConverter::Convert(tokens);
    ASSERT_EQ(postfix.size(), 2);
    EXPECT_EQ(postfix[0].type, TokenType::IDENTIFIER);
    EXPECT_EQ(postfix[0].value, "y");
    EXPECT_EQ(postfix[1].value, "_");
}

TEST(PostfixConverterTest, handles_unary_minus_in_parentheses) {
    vector<Token> tokens = {
        { TokenType::LEFT_PAREN, "(" },
        { TokenType::MINUS, "-" },
        { TokenType::INTEGER_LITERAL, "2" },
        { TokenType::RIGHT_PAREN, ")" }
    };
    vector<Token> postfix = PostfixConverter::Convert(tokens);
    ASSERT_EQ(postfix.size(), 2);
    EXPECT_EQ(postfix[0].value, "2");
    EXPECT_EQ(postfix[1].value, "_");
}

TEST(PostfixConverterTest, handles_complex_expression_with_unary_minus_and_parentheses) {
    vector<Token> tokens = {
        { TokenType::INTEGER_LITERAL, "3" },
        { TokenType::MULTIPLY, "*" },
        { TokenType::LEFT_PAREN, "(" },
        { TokenType::MINUS, "-" },
        { TokenType::INTEGER_LITERAL, "4" },
        { TokenType::PLUS, "+" },
        { TokenType::INTEGER_LITERAL, "2" },
        { TokenType::RIGHT_PAREN, ")" }
    };
    vector<Token> postfix = PostfixConverter::Convert(tokens);
    ASSERT_EQ(postfix.size(), 6);
    EXPECT_EQ(postfix[0].value, "3");
    EXPECT_EQ(postfix[1].value, "4");
    EXPECT_EQ(postfix[2].value, "_");
    EXPECT_EQ(postfix[3].value, "2");
    EXPECT_EQ(postfix[4].value, "+");
    EXPECT_EQ(postfix[5].value, "*");
}

TEST(PostfixConverterTest, handles_division_precedence) {
    vector<Token> tokens = {
        { TokenType::INTEGER_LITERAL, "10" },
        { TokenType::MINUS, "-" },
        { TokenType::INTEGER_LITERAL, "6" },
        { TokenType::DIVIDE, "/" },
        { TokenType::INTEGER_LITERAL, "2" }
    };
    vector<Token> postfix = PostfixConverter::Convert(tokens);
    ASSERT_EQ(postfix.size(), 5);
    EXPECT_EQ(postfix[0].value, "10");
    EXPECT_EQ(postfix[1].value, "6");
    EXPECT_EQ(postfix[2].value, "2");
    EXPECT_EQ(postfix[3].value, "/");
    EXPECT_EQ(postfix[4].value, "-");
}

TEST(PostfixConverterTest, handles_parentheses_affecting_precedence) {
    vector<Token> tokens = {
        { TokenType::LEFT_PAREN, "(" },
        { TokenType::INTEGER_LITERAL, "10" },
        { TokenType::MINUS, "-" },
        { TokenType::INTEGER_LITERAL, "6" },
        { TokenType::RIGHT_PAREN, ")" },
        { TokenType::DIVIDE, "/" },
        { TokenType::INTEGER_LITERAL, "2" }
    };
    vector<Token> postfix = PostfixConverter::Convert(tokens);
    ASSERT_EQ(postfix.size(), 5);
    EXPECT_EQ(postfix[0].value, "10");
    EXPECT_EQ(postfix[1].value, "6");
    EXPECT_EQ(postfix[2].value, "-");
    EXPECT_EQ(postfix[3].value, "2");
    EXPECT_EQ(postfix[4].value, "/");
}

TEST(PostfixConverterTest, handles_multiple_parentheses) {
    vector<Token> tokens = {
        { TokenType::LEFT_PAREN, "(" },
        { TokenType::INTEGER_LITERAL, "1" },
        { TokenType::PLUS, "+" },
        { TokenType::LEFT_PAREN, "(" },
        { TokenType::INTEGER_LITERAL, "2" },
        { TokenType::MULTIPLY, "*" },
        { TokenType::INTEGER_LITERAL, "3" },
        { TokenType::RIGHT_PAREN, ")" }, 
        { TokenType::RIGHT_PAREN, ")" }
    };
    vector<Token> postfix = PostfixConverter::Convert(tokens);
    ASSERT_EQ(postfix.size(), 5);
    EXPECT_EQ(postfix[0].value, "1");
    EXPECT_EQ(postfix[1].value, "2");
    EXPECT_EQ(postfix[2].value, "3");
    EXPECT_EQ(postfix[3].value, "*");
    EXPECT_EQ(postfix[4].value, "+");
}