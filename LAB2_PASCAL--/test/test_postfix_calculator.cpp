#include "pch.h"
#include "../ExpressionEvaluator/PostfixCalculator.h"
#include "../Base/Token.h"

using namespace std;

TEST(PostfixCalculatorTest, calculates_single_integer_literal) {
    vector<Token> postfix = { { TokenTypes::INTEGER_LITERAL, "10" } };
    map<string, double> operands;
    EXPECT_EQ(PostfixCalculator::Calculate(postfix, operands), 10.0);
}

TEST(PostfixCalculatorTest, calculates_single_double_literal) {
    vector<Token> postfix = { { TokenTypes::DOUBLE_LITERAL, "3.14" } };
    map<string, double> operands;
    EXPECT_EQ(PostfixCalculator::Calculate(postfix, operands), 3.14);
}

TEST(PostfixCalculatorTest, calculates_addition) {
    vector<Token> postfix = {
        { TokenTypes::INTEGER_LITERAL, "2" },
        { TokenTypes::INTEGER_LITERAL, "3" },
        { TokenTypes::PLUS, "+" }
    };
    map<string, double> operands;
    EXPECT_EQ(PostfixCalculator::Calculate(postfix, operands), 5.0);
}

TEST(PostfixCalculatorTest, calculates_subtraction) {
    vector<Token> postfix = {
        { TokenTypes::INTEGER_LITERAL, "5" },
        { TokenTypes::INTEGER_LITERAL, "2" },
        { TokenTypes::MINUS, "-" }
    };
    map<string, double> operands;
    EXPECT_EQ(PostfixCalculator::Calculate(postfix, operands), 3.0);
}

TEST(PostfixCalculatorTest, calculates_multiplication) {
    vector<Token> postfix = {
        { TokenTypes::INTEGER_LITERAL, "2" },
        { TokenTypes::INTEGER_LITERAL, "4" },
        { TokenTypes::MULTIPLY, "*" }
    };
    map<string, double> operands;
    EXPECT_EQ(PostfixCalculator::Calculate(postfix, operands), 8.0);
}

TEST(PostfixCalculatorTest, calculates_division) {
    vector<Token> postfix = {
        { TokenTypes::DOUBLE_LITERAL, "6.0" },
        { TokenTypes::DOUBLE_LITERAL, "2.0" },
        { TokenTypes::DIVIDE, "/" }
    };
    map<string, double> operands;
    EXPECT_EQ(PostfixCalculator::Calculate(postfix, operands), 3.0);
}

TEST(PostfixCalculatorTest, calculates_integer_division) {
    vector<Token> postfix = {
        { TokenTypes::INTEGER_LITERAL, "7" },
        { TokenTypes::INTEGER_LITERAL, "3" },
        { TokenTypes::KEYWORD_DIV, "div" }
    };
    map<string, double> operands;
    EXPECT_EQ(PostfixCalculator::Calculate(postfix, operands), 2.0);
}

TEST(PostfixCalculatorTest, calculates_modulo) {
    vector<Token> postfix = {
        { TokenTypes::INTEGER_LITERAL, "7" },
        { TokenTypes::INTEGER_LITERAL, "3" },
        { TokenTypes::KEYWORD_MOD, "mod" }
    };
    map<string, double> operands;
    EXPECT_EQ(PostfixCalculator::Calculate(postfix, operands), 1.0);
}

TEST(PostfixCalculatorTest, calculates_with_identifier) {
    vector<Token> postfix = {
        { TokenTypes::IDENTIFIER, "x" },
        { TokenTypes::INTEGER_LITERAL, "5" },
        { TokenTypes::PLUS, "+" }
    };
    map<string, double> operands = { {"x", 10.0} };
    EXPECT_EQ(PostfixCalculator::Calculate(postfix, operands), 15.0);
}

TEST(PostfixCalculatorTest, throws_error_undeclared_identifier) {
    vector<Token> postfix = {
        { TokenTypes::IDENTIFIER, "y" },
        { TokenTypes::INTEGER_LITERAL, "5" },
        { TokenTypes::PLUS, "+" }
    };
    map<string, double> operands = { {"x", 10.0} };
    EXPECT_THROW(PostfixCalculator::Calculate(postfix, operands), runtime_error);
}

TEST(PostfixCalculatorTest, calculates_complex_expression) {
    vector<Token> postfix = {
        { TokenTypes::INTEGER_LITERAL, "2" },
        { TokenTypes::INTEGER_LITERAL, "3" },
        { TokenTypes::PLUS, "+" },
        { TokenTypes::INTEGER_LITERAL, "4" },
        { TokenTypes::MULTIPLY, "*" }
    };
    map<string, double> operands;
    EXPECT_EQ(PostfixCalculator::Calculate(postfix, operands), 20.0);
}

TEST(PostfixCalculatorTest, throws_error_division_by_zero) {
    vector<Token> postfix = {
        { TokenTypes::INTEGER_LITERAL, "5" },
        { TokenTypes::INTEGER_LITERAL, "0" },
        { TokenTypes::DIVIDE, "/" }
    };
    map<string, double> operands;
    EXPECT_THROW(PostfixCalculator::Calculate(postfix, operands), runtime_error);
}

TEST(PostfixCalculatorTest, throws_error_modulo_by_zero) {
    vector<Token> postfix = {
        { TokenTypes::INTEGER_LITERAL, "5" },
        { TokenTypes::INTEGER_LITERAL, "0" },
        { TokenTypes::KEYWORD_MOD, "mod" }
    };
    map<string, double> operands;
    EXPECT_THROW(PostfixCalculator::Calculate(postfix, operands), runtime_error);
}

TEST(PostfixCalculatorTest, throws_error_integer_division_by_zero) {
    vector<Token> postfix = {
        { TokenTypes::INTEGER_LITERAL, "5" },
        { TokenTypes::INTEGER_LITERAL, "0" },
        { TokenTypes::KEYWORD_DIV, "div" }
    };
    map<string, double> operands;
    EXPECT_THROW(PostfixCalculator::Calculate(postfix, operands), runtime_error);
}

TEST(PostfixCalculatorTest, throws_error_insufficient_operands_binary) {
    vector<Token> postfix = {
        { TokenTypes::INTEGER_LITERAL, "5" },
        { TokenTypes::PLUS, "+" }
    };
    map<string, double> operands;
    EXPECT_THROW(PostfixCalculator::Calculate(postfix, operands), runtime_error);
}

TEST(PostfixCalculatorTest, throws_error_too_many_operands) {
    vector<Token> postfix = {
        { TokenTypes::INTEGER_LITERAL, "5" },
        { TokenTypes::INTEGER_LITERAL, "3" }
    };
    map<string, double> operands;
    EXPECT_THROW(PostfixCalculator::Calculate(postfix, operands), runtime_error);
}

TEST(PostfixCalculatorTest, calculates_unary_minus_literal) {
    vector<Token> postfix = {
        { TokenTypes::INTEGER_LITERAL, "5" },
        { TokenTypes::MINUS, "_" }
    };
    map<string, double> operands;
    EXPECT_EQ(PostfixCalculator::Calculate(postfix, operands), -5.0);
}

TEST(PostfixCalculatorTest, calculates_unary_minus_identifier) {
    vector<Token> postfix = {
        { TokenTypes::IDENTIFIER, "y" },
        { TokenTypes::MINUS, "_" }
    };
    map<string, double> operands = { {"y", 2.5} };
    EXPECT_EQ(PostfixCalculator::Calculate(postfix, operands), -2.5);
}

TEST(PostfixCalculatorTest, calculates_expression_with_unary_minus) {
    vector<Token> postfix = {
        { TokenTypes::INTEGER_LITERAL, "3" },
        { TokenTypes::INTEGER_LITERAL, "4" },
        { TokenTypes::MINUS, "_" },
        { TokenTypes::PLUS, "+" }
    };
    map<string, double> operands;
    EXPECT_EQ(PostfixCalculator::Calculate(postfix, operands), -1.0);
}

TEST(PostfixCalculatorTest, throws_error_insufficient_operands_unary_minus) {
    vector<Token> postfix = {
        { TokenTypes::MINUS, "_" }
    };
    map<string, double> operands;
    EXPECT_THROW(PostfixCalculator::Calculate(postfix, operands), runtime_error);
}

TEST(PostfixCalculatorTest, calculates_complex_expression_with_unary_minus) {
    vector<Token> postfix = {
        { TokenTypes::INTEGER_LITERAL, "2" },
        { TokenTypes::INTEGER_LITERAL, "3" },
        { TokenTypes::PLUS, "+" },
        { TokenTypes::INTEGER_LITERAL, "4" },
        { TokenTypes::MINUS, "_" },
        { TokenTypes::MULTIPLY, "*" }
    };
    map<string, double> operands;
    EXPECT_EQ(PostfixCalculator::Calculate(postfix, operands), -20.0);
}