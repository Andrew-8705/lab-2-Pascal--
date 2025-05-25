#include "pch.h"
#include "../ExpressionEvaluator/ExpressionValidator.h"
#include "../Base/Token.h"
#include <vector>

using namespace std;

TEST(ExpressionValidatorTest, valid_expression_numeric) {
    vector<Token> tokens = {
        { TokenTypes::INTEGER_LITERAL, "10" },
        { TokenTypes::PLUS, "+" },
        { TokenTypes::DOUBLE_LITERAL, "3.14" }
    };
    EXPECT_NO_THROW(ExpressionValidator::Validate(tokens));
}

TEST(ExpressionValidatorTest, valid_expression_with_identifier) {
    vector<Token> tokens = {
        { TokenTypes::IDENTIFIER, "x" },
        { TokenTypes::MULTIPLY, "*" },
        { TokenTypes::INTEGER_LITERAL, "5" }
    };
    EXPECT_NO_THROW(ExpressionValidator::Validate(tokens));
}

TEST(ExpressionValidatorTest, valid_expression_with_parentheses) {
    vector<Token> tokens = {
        { TokenTypes::LEFT_PAREN, "(" },
        { TokenTypes::INTEGER_LITERAL, "2" },
        { TokenTypes::PLUS, "+" },
        { TokenTypes::INTEGER_LITERAL, "3" },
        { TokenTypes::RIGHT_PAREN, ")" },
        { TokenTypes::MULTIPLY, "*" },
        { TokenTypes::INTEGER_LITERAL, "4" }
    };
    EXPECT_NO_THROW(ExpressionValidator::Validate(tokens));
}

TEST(ExpressionValidatorTest, handles_valid_operator_mod) {
    vector<Token> tokens = {
        { TokenTypes::INTEGER_LITERAL, "10" },
        { TokenTypes::KEYWORD_MOD, "mod" },
        { TokenTypes::INTEGER_LITERAL, "3" }
    };
    EXPECT_NO_THROW(ExpressionValidator::Validate(tokens));
}

TEST(ExpressionValidatorTest, handles_valid_operator_div) {
    vector<Token> tokens = {
        { TokenTypes::INTEGER_LITERAL, "10" },
        { TokenTypes::KEYWORD_DIV, "div" },
        { TokenTypes::INTEGER_LITERAL, "3" }
    };
    EXPECT_NO_THROW(ExpressionValidator::Validate(tokens));
}

TEST(ExpressionValidatorTest, throws_error_invalid_character) {
    vector<Token> tokens = {
        { TokenTypes::INTEGER_LITERAL, "5" },
        { TokenTypes::UNKNOWN, "$" },
        { TokenTypes::INTEGER_LITERAL, "3" }
    };
    EXPECT_THROW(ExpressionValidator::Validate(tokens), runtime_error);
}

TEST(ExpressionValidatorTest, throws_error_unmatched_opening_parenthesis) {
    vector<Token> tokens = {
        { TokenTypes::LEFT_PAREN, "(" },
        { TokenTypes::INTEGER_LITERAL, "5" }
    };
    EXPECT_THROW(ExpressionValidator::Validate(tokens), runtime_error);
}

TEST(ExpressionValidatorTest, throws_error_unmatched_closing_parenthesis) {
    vector<Token> tokens = {
        { TokenTypes::INTEGER_LITERAL, "5" },
        { TokenTypes::RIGHT_PAREN, ")" }
    };
    EXPECT_THROW(ExpressionValidator::Validate(tokens), runtime_error);
}

TEST(ExpressionValidatorTest, throws_error_multiple_unmatched_opening_parentheses) {
    vector<Token> tokens = {
        { TokenTypes::LEFT_PAREN, "(" },
        { TokenTypes::INTEGER_LITERAL, "2" },
        { TokenTypes::LEFT_PAREN, "(" },
        { TokenTypes::INTEGER_LITERAL, "3" }
    };
    EXPECT_THROW(ExpressionValidator::Validate(tokens), runtime_error);
}

TEST(ExpressionValidatorTest, throws_error_multiple_unmatched_closing_parentheses) {
    vector<Token> tokens = {
        { TokenTypes::INTEGER_LITERAL, "2" },
        { TokenTypes::RIGHT_PAREN, ")" },
        { TokenTypes::INTEGER_LITERAL, "3" },
        { TokenTypes::RIGHT_PAREN, ")" }
    };
    EXPECT_THROW(ExpressionValidator::Validate(tokens), runtime_error);
}

TEST(ExpressionValidatorTest, valid_expression_with_unary_minus_identifier) {
    vector<Token> tokens = {
        { TokenTypes::MINUS, "-" },
        { TokenTypes::IDENTIFIER, "y" }
    };
    EXPECT_NO_THROW(ExpressionValidator::Validate(tokens));
}

TEST(ExpressionValidatorTest, valid_expression_with_unary_minus_literal) {
    vector<Token> tokens = {
        { TokenTypes::MINUS, "-" },
        { TokenTypes::INTEGER_LITERAL, "7" }
    };
    EXPECT_NO_THROW(ExpressionValidator::Validate(tokens));
}

TEST(ExpressionValidatorTest, valid_expression_starting_with_parentheses_and_unary_minus) {
    vector<Token> tokens = {
        { TokenTypes::LEFT_PAREN, "(" },
        { TokenTypes::MINUS, "-" },
        { TokenTypes::INTEGER_LITERAL, "5" },
        { TokenTypes::RIGHT_PAREN, ")" }
    };
    EXPECT_NO_THROW(ExpressionValidator::Validate(tokens));
}

TEST(ExpressionValidatorTest, valid_expression_with_operator_and_unary_minus) {
    vector<Token> tokens = {
        { TokenTypes::INTEGER_LITERAL, "3" },
        { TokenTypes::MULTIPLY, "*" },
        { TokenTypes::LEFT_PAREN, "(" },
        { TokenTypes::MINUS, "-" },
        { TokenTypes::DOUBLE_LITERAL, "2.0" },
        { TokenTypes::RIGHT_PAREN, ")" }
    };
    EXPECT_NO_THROW(ExpressionValidator::Validate(tokens));
}