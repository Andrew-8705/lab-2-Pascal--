#include "pch.h"
#include "../ExpressionEvaluator/ExpressionValidator.h"
#include "../Base/Token.h"
#include <vector>

using namespace std;

TEST(ExpressionValidatorTest, valid_expression_numeric) {
    vector<Token> tokens = {
        { TokenType::INTEGER_LITERAL, "10" },
        { TokenType::PLUS, "+" },
        { TokenType::DOUBLE_LITERAL, "3.14" }
    };
    EXPECT_NO_THROW(ExpressionValidator::Validate(tokens));
}

TEST(ExpressionValidatorTest, valid_expression_with_identifier) {
    vector<Token> tokens = {
        { TokenType::IDENTIFIER, "x" },
        { TokenType::MULTIPLY, "*" },
        { TokenType::INTEGER_LITERAL, "5" }
    };
    EXPECT_NO_THROW(ExpressionValidator::Validate(tokens));
}

TEST(ExpressionValidatorTest, valid_expression_with_parentheses) {
    vector<Token> tokens = {
        { TokenType::LEFT_PAREN, "(" },
        { TokenType::INTEGER_LITERAL, "2" },
        { TokenType::PLUS, "+" },
        { TokenType::INTEGER_LITERAL, "3" },
        { TokenType::RIGHT_PAREN, ")" },
        { TokenType::MULTIPLY, "*" },
        { TokenType::INTEGER_LITERAL, "4" }
    };
    EXPECT_NO_THROW(ExpressionValidator::Validate(tokens));
}

TEST(ExpressionValidatorTest, handles_valid_operator_mod) {
    vector<Token> tokens = {
        { TokenType::INTEGER_LITERAL, "10" },
        { TokenType::KEYWORD_MOD, "mod" },
        { TokenType::INTEGER_LITERAL, "3" }
    };
    EXPECT_NO_THROW(ExpressionValidator::Validate(tokens));
}

TEST(ExpressionValidatorTest, handles_valid_operator_div) {
    vector<Token> tokens = {
        { TokenType::INTEGER_LITERAL, "10" },
        { TokenType::KEYWORD_DIV, "div" },
        { TokenType::INTEGER_LITERAL, "3" }
    };
    EXPECT_NO_THROW(ExpressionValidator::Validate(tokens));
}

TEST(ExpressionValidatorTest, throws_error_invalid_character) {
    vector<Token> tokens = {
        { TokenType::INTEGER_LITERAL, "5" },
        { TokenType::UNKNOWN, "$" },
        { TokenType::INTEGER_LITERAL, "3" }
    };
    EXPECT_THROW(ExpressionValidator::Validate(tokens), runtime_error);
}

TEST(ExpressionValidatorTest, throws_error_unmatched_opening_parenthesis) {
    vector<Token> tokens = {
        { TokenType::LEFT_PAREN, "(" },
        { TokenType::INTEGER_LITERAL, "5" }
    };
    EXPECT_THROW(ExpressionValidator::Validate(tokens), runtime_error);
}

TEST(ExpressionValidatorTest, throws_error_unmatched_closing_parenthesis) {
    vector<Token> tokens = {
        { TokenType::INTEGER_LITERAL, "5" },
        { TokenType::RIGHT_PAREN, ")" }
    };
    EXPECT_THROW(ExpressionValidator::Validate(tokens), runtime_error);
}

TEST(ExpressionValidatorTest, throws_error_multiple_unmatched_opening_parentheses) {
    vector<Token> tokens = {
        { TokenType::LEFT_PAREN, "(" },
        { TokenType::INTEGER_LITERAL, "2" },
        { TokenType::LEFT_PAREN, "(" },
        { TokenType::INTEGER_LITERAL, "3" }
    };
    EXPECT_THROW(ExpressionValidator::Validate(tokens), runtime_error);
}

TEST(ExpressionValidatorTest, throws_error_multiple_unmatched_closing_parentheses) {
    vector<Token> tokens = {
        { TokenType::INTEGER_LITERAL, "2" },
        { TokenType::RIGHT_PAREN, ")" },
        { TokenType::INTEGER_LITERAL, "3" },
        { TokenType::RIGHT_PAREN, ")" }
    };
    EXPECT_THROW(ExpressionValidator::Validate(tokens), runtime_error);
}

TEST(ExpressionValidatorTest, valid_expression_with_unary_minus_identifier) {
    vector<Token> tokens = {
        { TokenType::MINUS, "-" },
        { TokenType::IDENTIFIER, "y" }
    };
    EXPECT_NO_THROW(ExpressionValidator::Validate(tokens));
}

TEST(ExpressionValidatorTest, valid_expression_with_unary_minus_literal) {
    vector<Token> tokens = {
        { TokenType::MINUS, "-" },
        { TokenType::INTEGER_LITERAL, "7" }
    };
    EXPECT_NO_THROW(ExpressionValidator::Validate(tokens));
}

TEST(ExpressionValidatorTest, valid_expression_starting_with_parentheses_and_unary_minus) {
    vector<Token> tokens = {
        { TokenType::LEFT_PAREN, "(" },
        { TokenType::MINUS, "-" },
        { TokenType::INTEGER_LITERAL, "5" },
        { TokenType::RIGHT_PAREN, ")" }
    };
    EXPECT_NO_THROW(ExpressionValidator::Validate(tokens));
}

TEST(ExpressionValidatorTest, valid_expression_with_operator_and_unary_minus) {
    vector<Token> tokens = {
        { TokenType::INTEGER_LITERAL, "3" },
        { TokenType::MULTIPLY, "*" },
        { TokenType::LEFT_PAREN, "(" },
        { TokenType::MINUS, "-" },
        { TokenType::DOUBLE_LITERAL, "2.0" },
        { TokenType::RIGHT_PAREN, ")" }
    };
    EXPECT_NO_THROW(ExpressionValidator::Validate(tokens));
}