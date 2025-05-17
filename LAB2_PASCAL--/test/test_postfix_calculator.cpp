#include "pch.h"
#include "../ExpressionEvaluator/PostfixCalculator.h"
#include "../Base/Token.h"

using namespace std;

TEST(PostfixCalculatorTest, calculates_single_integer_literal) {
    vector<Token> postfix = { { TokenType::INTEGER_LITERAL, "10" } };
    map<string, double> operands;
    EXPECT_EQ(PostfixCalculator::Calculate(postfix, operands), 10.0);
}

TEST(PostfixCalculatorTest, calculates_single_double_literal) {
    vector<Token> postfix = { { TokenType::DOUBLE_LITERAL, "3.14" } };
    map<string, double> operands;
    EXPECT_EQ(PostfixCalculator::Calculate(postfix, operands), 3.14);
}

TEST(PostfixCalculatorTest, calculates_addition) {
    vector<Token> postfix = {
        { TokenType::INTEGER_LITERAL, "2" },
        { TokenType::INTEGER_LITERAL, "3" },
        { TokenType::PLUS, "+" }
    };
    map<string, double> operands;
    EXPECT_EQ(PostfixCalculator::Calculate(postfix, operands), 5.0);
}

TEST(PostfixCalculatorTest, calculates_subtraction) {
    vector<Token> postfix = {
        { TokenType::INTEGER_LITERAL, "5" },
        { TokenType::INTEGER_LITERAL, "2" },
        { TokenType::MINUS, "-" }
    };
    map<string, double> operands;
    EXPECT_EQ(PostfixCalculator::Calculate(postfix, operands), 3.0);
}

TEST(PostfixCalculatorTest, calculates_multiplication) {
    vector<Token> postfix = {
        { TokenType::INTEGER_LITERAL, "2" },
        { TokenType::INTEGER_LITERAL, "4" },
        { TokenType::MULTIPLY, "*" }
    };
    map<string, double> operands;
    EXPECT_EQ(PostfixCalculator::Calculate(postfix, operands), 8.0);
}

TEST(PostfixCalculatorTest, calculates_division) {
    vector<Token> postfix = {
        { TokenType::DOUBLE_LITERAL, "6.0" },
        { TokenType::DOUBLE_LITERAL, "2.0" },
        { TokenType::DIVIDE, "/" }
    };
    map<string, double> operands;
    EXPECT_EQ(PostfixCalculator::Calculate(postfix, operands), 3.0);
}

TEST(PostfixCalculatorTest, calculates_integer_division) {
    vector<Token> postfix = {
        { TokenType::INTEGER_LITERAL, "7" },
        { TokenType::INTEGER_LITERAL, "3" },
        { TokenType::KEYWORD_DIV, "div" }
    };
    map<string, double> operands;
    EXPECT_EQ(PostfixCalculator::Calculate(postfix, operands), 2.0);
}

TEST(PostfixCalculatorTest, calculates_modulo) {
    vector<Token> postfix = {
        { TokenType::INTEGER_LITERAL, "7" },
        { TokenType::INTEGER_LITERAL, "3" },
        { TokenType::KEYWORD_MOD, "mod" }
    };
    map<string, double> operands;
    EXPECT_EQ(PostfixCalculator::Calculate(postfix, operands), 1.0);
}

TEST(PostfixCalculatorTest, calculates_with_identifier) {
    vector<Token> postfix = {
        { TokenType::IDENTIFIER, "x" },
        { TokenType::INTEGER_LITERAL, "5" },
        { TokenType::PLUS, "+" }
    };
    map<string, double> operands = { {"x", 10.0} };
    EXPECT_EQ(PostfixCalculator::Calculate(postfix, operands), 15.0);
}

TEST(PostfixCalculatorTest, throws_error_undeclared_identifier) {
    vector<Token> postfix = {
        { TokenType::IDENTIFIER, "y" },
        { TokenType::INTEGER_LITERAL, "5" },
        { TokenType::PLUS, "+" }
    };
    map<string, double> operands = { {"x", 10.0} };
    EXPECT_THROW(PostfixCalculator::Calculate(postfix, operands), runtime_error);
}

TEST(PostfixCalculatorTest, calculates_complex_expression) {
    vector<Token> postfix = {
        { TokenType::INTEGER_LITERAL, "2" },
        { TokenType::INTEGER_LITERAL, "3" },
        { TokenType::PLUS, "+" },
        { TokenType::INTEGER_LITERAL, "4" },
        { TokenType::MULTIPLY, "*" }
    };
    map<string, double> operands;
    EXPECT_EQ(PostfixCalculator::Calculate(postfix, operands), 20.0);
}

TEST(PostfixCalculatorTest, throws_error_division_by_zero) {
    vector<Token> postfix = {
        { TokenType::INTEGER_LITERAL, "5" },
        { TokenType::INTEGER_LITERAL, "0" },
        { TokenType::DIVIDE, "/" }
    };
    map<string, double> operands;
    EXPECT_THROW(PostfixCalculator::Calculate(postfix, operands), runtime_error);
}

TEST(PostfixCalculatorTest, throws_error_modulo_by_zero) {
    vector<Token> postfix = {
        { TokenType::INTEGER_LITERAL, "5" },
        { TokenType::INTEGER_LITERAL, "0" },
        { TokenType::KEYWORD_MOD, "mod" }
    };
    map<string, double> operands;
    EXPECT_THROW(PostfixCalculator::Calculate(postfix, operands), runtime_error);
}

TEST(PostfixCalculatorTest, throws_error_integer_division_by_zero) {
    vector<Token> postfix = {
        { TokenType::INTEGER_LITERAL, "5" },
        { TokenType::INTEGER_LITERAL, "0" },
        { TokenType::KEYWORD_DIV, "div" }
    };
    map<string, double> operands;
    EXPECT_THROW(PostfixCalculator::Calculate(postfix, operands), runtime_error);
}

TEST(PostfixCalculatorTest, throws_error_insufficient_operands_binary) {
    vector<Token> postfix = {
        { TokenType::INTEGER_LITERAL, "5" },
        { TokenType::PLUS, "+" }
    };
    map<string, double> operands;
    EXPECT_THROW(PostfixCalculator::Calculate(postfix, operands), runtime_error);
}

TEST(PostfixCalculatorTest, throws_error_too_many_operands) {
    vector<Token> postfix = {
        { TokenType::INTEGER_LITERAL, "5" },
        { TokenType::INTEGER_LITERAL, "3" }
    };
    map<string, double> operands;
    EXPECT_THROW(PostfixCalculator::Calculate(postfix, operands), runtime_error);
}

TEST(PostfixCalculatorTest, calculates_unary_minus_literal) {
    vector<Token> postfix = {
        { TokenType::INTEGER_LITERAL, "5" },
        { TokenType::MINUS, "_" }
    };
    map<string, double> operands;
    EXPECT_EQ(PostfixCalculator::Calculate(postfix, operands), -5.0);
}

TEST(PostfixCalculatorTest, calculates_unary_minus_identifier) {
    vector<Token> postfix = {
        { TokenType::IDENTIFIER, "y" },
        { TokenType::MINUS, "_" }
    };
    map<string, double> operands = { {"y", 2.5} };
    EXPECT_EQ(PostfixCalculator::Calculate(postfix, operands), -2.5);
}

TEST(PostfixCalculatorTest, calculates_expression_with_unary_minus) {
    vector<Token> postfix = {
        { TokenType::INTEGER_LITERAL, "3" },
        { TokenType::INTEGER_LITERAL, "4" },
        { TokenType::MINUS, "_" },
        { TokenType::PLUS, "+" }
    };
    map<string, double> operands;
    EXPECT_EQ(PostfixCalculator::Calculate(postfix, operands), -1.0);
}

TEST(PostfixCalculatorTest, throws_error_insufficient_operands_unary_minus) {
    vector<Token> postfix = {
        { TokenType::MINUS, "_" }
    };
    map<string, double> operands;
    EXPECT_THROW(PostfixCalculator::Calculate(postfix, operands), runtime_error);
}

TEST(PostfixCalculatorTest, calculates_complex_expression_with_unary_minus) {
    vector<Token> postfix = {
        { TokenType::INTEGER_LITERAL, "2" },
        { TokenType::INTEGER_LITERAL, "3" },
        { TokenType::PLUS, "+" },
        { TokenType::INTEGER_LITERAL, "4" },
        { TokenType::MINUS, "_" },
        { TokenType::MULTIPLY, "*" }
    };
    map<string, double> operands;
    EXPECT_EQ(PostfixCalculator::Calculate(postfix, operands), -20.0);
}