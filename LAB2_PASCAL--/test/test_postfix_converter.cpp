#include "pch.h"
#include "../ExpressionEvaluator/PostfixConverter.h"
#include "../ExpressionEvaluator/PostfixConverter.cpp"
#include "../Base/Token.h"
#include <vector>

using namespace std;

TEST(PostfixConverterTest, handles_single_integer_literal) {
    vector<Token> tokens = { { TokenTypes::INTEGER_LITERAL, "10" } };
    vector<Token> postfix = PostfixConverter::Convert(tokens);
    ASSERT_EQ(postfix.size(), 1);
    EXPECT_EQ(postfix[0].type, TokenTypes::INTEGER_LITERAL);
    EXPECT_EQ(postfix[0].value, "10");
}

TEST(PostfixConverterTest, handles_single_identifier) {
    vector<Token> tokens = { { TokenTypes::IDENTIFIER, "x"} };
    vector<Token> postfix = PostfixConverter::Convert(tokens);
    ASSERT_EQ(postfix.size(), 1);
    EXPECT_EQ(postfix[0].type, TokenTypes::IDENTIFIER);
    EXPECT_EQ(postfix[0].value, "x");
}

TEST(PostfixConverterTest, handles_addition) {
    vector<Token> tokens = {
        { TokenTypes::INTEGER_LITERAL, "2" },
        { TokenTypes::PLUS, "+" },
        { TokenTypes::INTEGER_LITERAL, "3" }
    };
    vector<Token> postfix = PostfixConverter::Convert(tokens);
    ASSERT_EQ(postfix.size(), 3);
    EXPECT_EQ(postfix[0].type, TokenTypes::INTEGER_LITERAL);
    EXPECT_EQ(postfix[0].value, "2");
    EXPECT_EQ(postfix[1].type, TokenTypes::INTEGER_LITERAL);
    EXPECT_EQ(postfix[1].value, "3");
    EXPECT_EQ(postfix[2].type, TokenTypes::PLUS);
    EXPECT_EQ(postfix[2].value, "+");
}

TEST(PostfixConverterTest, handles_multiplication_precedence) {
    vector<Token> tokens = {
        { TokenTypes::INTEGER_LITERAL, "2" },
        { TokenTypes::PLUS, "+" },
        { TokenTypes::INTEGER_LITERAL, "3" },
        { TokenTypes::MULTIPLY, "*" },
        { TokenTypes::INTEGER_LITERAL, "4" }
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
        { TokenTypes::LEFT_PAREN, "(" },
        { TokenTypes::INTEGER_LITERAL, "2" },
        { TokenTypes::PLUS, "+" },
        { TokenTypes::INTEGER_LITERAL, "3" },
        { TokenTypes::RIGHT_PAREN, ")" },
        { TokenTypes::MULTIPLY, "*" },
        { TokenTypes::INTEGER_LITERAL, "4" }
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
        { TokenTypes::INTEGER_LITERAL, "10" },
        { TokenTypes::MINUS, "-" },
        { TokenTypes::INTEGER_LITERAL, "5" },
        { TokenTypes::MINUS, "-" },
        { TokenTypes::INTEGER_LITERAL, "2" }
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
        { TokenTypes::INTEGER_LITERAL, "10" },
        { TokenTypes::KEYWORD_DIV, "div" },
        { TokenTypes::INTEGER_LITERAL, "3" }
    };
    vector<Token> postfix = PostfixConverter::Convert(tokens);
    ASSERT_EQ(postfix.size(), 3);
    EXPECT_EQ(postfix[2].value, "div");
}

TEST(PostfixConverterTest, handles_mod_operator) {
    vector<Token> tokens = {
        { TokenTypes::INTEGER_LITERAL, "10" },
        { TokenTypes::KEYWORD_MOD, "mod" },
        { TokenTypes::INTEGER_LITERAL, "3" }
    };
    vector<Token> postfix = PostfixConverter::Convert(tokens);
    ASSERT_EQ(postfix.size(), 3);
    EXPECT_EQ(postfix[2].value, "mod");
}

TEST(PostfixConverterTest, handles_unary_minus_literal) {
    vector<Token> tokens = {
        { TokenTypes::MINUS, "-" },
        { TokenTypes::INTEGER_LITERAL, "5" }
    };
    vector<Token> postfix = PostfixConverter::Convert(tokens);
    ASSERT_EQ(postfix.size(), 2);
    EXPECT_EQ(postfix[0].type, TokenTypes::INTEGER_LITERAL);
    EXPECT_EQ(postfix[0].value, "5");
    EXPECT_EQ(postfix[1].value, "_");
}

TEST(PostfixConverterTest, handles_unary_minus_identifier) {
    vector<Token> tokens = {
        { TokenTypes::MINUS, "-" },
        { TokenTypes::IDENTIFIER, "y" }
    };
    vector<Token> postfix = PostfixConverter::Convert(tokens);
    ASSERT_EQ(postfix.size(), 2);
    EXPECT_EQ(postfix[0].type, TokenTypes::IDENTIFIER);
    EXPECT_EQ(postfix[0].value, "y");
    EXPECT_EQ(postfix[1].value, "_");
}

TEST(PostfixConverterTest, handles_unary_minus_in_parentheses) {
    vector<Token> tokens = {
        { TokenTypes::LEFT_PAREN, "(" },
        { TokenTypes::MINUS, "-" },
        { TokenTypes::INTEGER_LITERAL, "2" },
        { TokenTypes::RIGHT_PAREN, ")" }
    };
    vector<Token> postfix = PostfixConverter::Convert(tokens);
    ASSERT_EQ(postfix.size(), 2);
    EXPECT_EQ(postfix[0].value, "2");
    EXPECT_EQ(postfix[1].value, "_");
}

TEST(PostfixConverterTest, handles_complex_expression_with_unary_minus_and_parentheses) {
    vector<Token> tokens = {
        { TokenTypes::INTEGER_LITERAL, "3" },
        { TokenTypes::MULTIPLY, "*" },
        { TokenTypes::LEFT_PAREN, "(" },
        { TokenTypes::MINUS, "-" },
        { TokenTypes::INTEGER_LITERAL, "4" },
        { TokenTypes::PLUS, "+" },
        { TokenTypes::INTEGER_LITERAL, "2" },
        { TokenTypes::RIGHT_PAREN, ")" }
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
        { TokenTypes::INTEGER_LITERAL, "10" },
        { TokenTypes::MINUS, "-" },
        { TokenTypes::INTEGER_LITERAL, "6" },
        { TokenTypes::DIVIDE, "/" },
        { TokenTypes::INTEGER_LITERAL, "2" }
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
        { TokenTypes::LEFT_PAREN, "(" },
        { TokenTypes::INTEGER_LITERAL, "10" },
        { TokenTypes::MINUS, "-" },
        { TokenTypes::INTEGER_LITERAL, "6" },
        { TokenTypes::RIGHT_PAREN, ")" },
        { TokenTypes::DIVIDE, "/" },
        { TokenTypes::INTEGER_LITERAL, "2" }
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
        { TokenTypes::LEFT_PAREN, "(" },
        { TokenTypes::INTEGER_LITERAL, "1" },
        { TokenTypes::PLUS, "+" },
        { TokenTypes::LEFT_PAREN, "(" },
        { TokenTypes::INTEGER_LITERAL, "2" },
        { TokenTypes::MULTIPLY, "*" },
        { TokenTypes::INTEGER_LITERAL, "3" },
        { TokenTypes::RIGHT_PAREN, ")" }, 
        { TokenTypes::RIGHT_PAREN, ")" }
    };
    vector<Token> postfix = PostfixConverter::Convert(tokens);
    ASSERT_EQ(postfix.size(), 5);
    EXPECT_EQ(postfix[0].value, "1");
    EXPECT_EQ(postfix[1].value, "2");
    EXPECT_EQ(postfix[2].value, "3");
    EXPECT_EQ(postfix[3].value, "*");
    EXPECT_EQ(postfix[4].value, "+");
}