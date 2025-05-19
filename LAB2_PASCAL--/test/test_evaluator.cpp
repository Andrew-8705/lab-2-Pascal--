#include "pch.h"
#include "../ExpressionEvaluator/Evaluator.h"

//NUMERIC_PART

TEST(NUMERIC_EvaluatorTest, can_evaluate_simple_expression) 
{
	vector<Token> expression =
	{
		{TokenType::INTEGER_LITERAL,"7"},
		{TokenType::PLUS, "+"},
		{TokenType::INTEGER_LITERAL, "3"}
	};
	unordered_map<string, variant<int, double, string>> variables = {};
	unordered_map<string, variant<int, double, string>> constants = {};
	EXPECT_EQ(Evaluator::evaluate_numeric(expression, variables, constants),10);
}

TEST(NUMERIC_EvaluatorTest, subtraction) 
{
    vector<Token> expression = 
    {
        {TokenType::INTEGER_LITERAL, "10"},
        {TokenType::MINUS, "-"},
        {TokenType::INTEGER_LITERAL, "3"}
    };
    unordered_map<string, variant<int, double, string>> variables = {};
    unordered_map<string, variant<int, double, string>> constants = {};
    EXPECT_EQ(Evaluator::evaluate_numeric(expression, variables, constants), 7);
}

TEST(NUMERIC_EvaluatorTest, multiplication) 
{
    vector<Token> expression = 
    {
        {TokenType::INTEGER_LITERAL, "5"},
        {TokenType::MULTIPLY, "*"},
        {TokenType::INTEGER_LITERAL, "4"}
    };
    unordered_map<string, variant<int, double, string>> variables = {};
    unordered_map<string, variant<int, double, string>> constants = {};
    EXPECT_EQ(Evaluator::evaluate_numeric(expression, variables, constants), 20);
}

TEST(NUMERIC_EvaluatorTest, division) 
{
    vector<Token> expression = 
    {
        {TokenType::INTEGER_LITERAL, "20"},
        {TokenType::DIVIDE, "/"},
        {TokenType::INTEGER_LITERAL, "4"}
    };
    unordered_map<string, variant<int, double, string>> variables = {};
    unordered_map<string, variant<int, double, string>> constants = {};
    EXPECT_EQ(Evaluator::evaluate_numeric(expression, variables, constants), 5);
}

TEST(NUMERIC_EvaluatorTest, operator_precedence) 
{
    vector<Token> expression = 
    {
        {TokenType::INTEGER_LITERAL, "2"},
        {TokenType::PLUS, "+"},
        {TokenType::INTEGER_LITERAL, "3"},
        {TokenType::MULTIPLY, "*"},
        {TokenType::INTEGER_LITERAL, "4"}
    };
    unordered_map<string, variant<int, double, string>> variables = {};
    unordered_map<string, variant<int, double, string>> constants = {};
    EXPECT_EQ(Evaluator::evaluate_numeric(expression, variables, constants), 14); // 2 + (3 * 4)
}

TEST(NUMERIC_EvaluatorTest, parentheses) 
{
    vector<Token> expression = 
    {
        {TokenType::LEFT_PAREN, "("},
        {TokenType::INTEGER_LITERAL, "2"},
        {TokenType::PLUS, "+"},
        {TokenType::INTEGER_LITERAL, "3"},
        {TokenType::RIGHT_PAREN, ")"},
        {TokenType::MULTIPLY, "*"},
        {TokenType::INTEGER_LITERAL, "4"}
    };
    unordered_map<string, variant<int, double, string>> variables = {};
    unordered_map<string, variant<int, double, string>> constants = {};
    EXPECT_EQ(Evaluator::evaluate_numeric(expression, variables, constants), 20); // (2 + 3) * 4
}

TEST(NUMERIC_EvaluatorTest, nested_parentheses) 
{
    vector<Token> expression = 
    {
        {TokenType::LEFT_PAREN, "("},
        {TokenType::INTEGER_LITERAL, "1"},
        {TokenType::PLUS, "+"},
        {TokenType::LEFT_PAREN, "("},
        {TokenType::INTEGER_LITERAL, "2"},
        {TokenType::MULTIPLY, "*"},
        {TokenType::INTEGER_LITERAL, "3"},
        {TokenType::RIGHT_PAREN, ")"},
        {TokenType::RIGHT_PAREN, ")"},
        {TokenType::MULTIPLY, "*"},
        {TokenType::INTEGER_LITERAL, "4"}
    };
    unordered_map<string, variant<int, double, string>> variables = {};
    unordered_map<string, variant<int, double, string>> constants = {};
    EXPECT_EQ(Evaluator::evaluate_numeric(expression, variables, constants), 28); // (1 + (2 * 3)) * 4
}

TEST(NUMERIC_EvaluatorTest, negative_numbers) 
{
    vector<Token> expression = 
    {
        {TokenType::MINUS, "-"},
        {TokenType::INTEGER_LITERAL, "5"},
        {TokenType::PLUS, "+"},
        {TokenType::INTEGER_LITERAL, "3"}
    };
    unordered_map<string, variant<int, double, string>> variables = {};
    unordered_map<string, variant<int, double, string>> constants = {};
    EXPECT_EQ(Evaluator::evaluate_numeric(expression, variables, constants), -2);
}

TEST(NUMERIC_EvaluatorTest, negative_numbers_with_parentheses) 
{
    vector<Token> expression = 
    {
        {TokenType::LEFT_PAREN, "("},
        {TokenType::MINUS, "-"},
        {TokenType::INTEGER_LITERAL, "5"},
        {TokenType::RIGHT_PAREN, ")"},
        {TokenType::MULTIPLY, "*"},
        {TokenType::INTEGER_LITERAL, "2"}
    };
    unordered_map<string, variant<int, double, string>> variables = {};
    unordered_map<string, variant<int, double, string>> constants = {};
    EXPECT_EQ(Evaluator::evaluate_numeric(expression, variables, constants), -10);
}

TEST(NUMERIC_EvaluatorTest, division_by_zero_throws) 
{
    vector<Token> expression = 
    {
        {TokenType::INTEGER_LITERAL, "10"},
        {TokenType::DIVIDE, "/"},
        {TokenType::INTEGER_LITERAL, "0"}
    };
    unordered_map<string, variant<int, double, string>> variables = {};
    unordered_map<string, variant<int, double, string>> constants = {};
    ASSERT_THROW(Evaluator::evaluate_numeric(expression, variables, constants), runtime_error); 
}

TEST(NUMERIC_EvaluatorTest, simple_variable)
{
    vector<Token> expression = 
    {
        {TokenType::IDENTIFIER, "x"}
    };
    unordered_map<string, variant<int, double, string>> variables = { {"x", 10} };
    unordered_map<string, variant<int, double, string>> constants = {};
    EXPECT_EQ(Evaluator::evaluate_numeric(expression, variables, constants), 10);
}

TEST(NUMERIC_EvaluatorTest, simple_constant) 
{
    vector<Token> expression = 
    {
        {TokenType::IDENTIFIER, "PI"}
    };
    unordered_map<string, variant<int, double, string>> variables = {};
    unordered_map<string, variant<int, double, string>> constants = { {"PI", 3.14159} };
    EXPECT_TRUE(Evaluator::evaluate_numeric(expression, variables, constants), 3.14159);
}

TEST(NUMERIC_EvaluatorTest, variable_plus_constant) 
{
    vector<Token> expression = 
    {
        {TokenType::IDENTIFIER, "x"},
        {TokenType::PLUS, "+"},
        {TokenType::IDENTIFIER, "PI"}
    };
    unordered_map<string, variant<int, double, string>> variables = { {"x", 5} };
    unordered_map<string, variant<int, double, string>> constants = { {"PI", 3.14159} };
    EXPECT_TRUE(Evaluator::evaluate_numeric(expression, variables, constants), 8.14159);
}

TEST(NUMERIC_EvaluatorTest, missing_variable_throws) 
{
    vector<Token> expression = 
    {
        {TokenType::IDENTIFIER, "x"},
        {TokenType::PLUS, "+"},
        {TokenType::INTEGER_LITERAL, "3"}
    };
    unordered_map<string, variant<int, double, string>> variables = {}; 
    unordered_map<string, variant<int, double, string>> constants = {};
    ASSERT_THROW(Evaluator::evaluate_numeric(expression, variables, constants), runtime_error);
}

TEST(NUMERIC_EvaluatorTest, missing_constant_throws) 
{
    vector<Token> expression = 
    {
        {TokenType::IDENTIFIER, "E"},
        {TokenType::PLUS, "+"},
        {TokenType::INTEGER_LITERAL, "3"}
    };
    unordered_map<string, variant<int, double, string>> variables = {}; 
    unordered_map<string, variant<int, double, string>> constants = {};
    ASSERT_THROW(Evaluator::evaluate_numeric(expression, variables, constants), runtime_error);
}

TEST(NUMERIC_EvaluatorTest, can_handle_exponentials) 
{
    vector<Token> expression = 
    {
        {TokenType::INTEGER_LITERAL, "2"},
        {TokenType::MULTIPLY, "*"},
        {TokenType::INTEGER_LITERAL, "2"},
        {TokenType::MULTIPLY, "*"},
        {TokenType::INTEGER_LITERAL, "2"}
    };
    unordered_map<string, variant<int, double, string>> variables = {};
    unordered_map<string, variant<int, double, string>> constants = {};
    EXPECT_EQ(Evaluator::evaluate_numeric(expression, variables, constants), 8);
}
TEST(NUMERIC_EvaluatorTest, zero_division_complex) 
{
    vector<Token> expression = 
    {
        {TokenType::INTEGER_LITERAL, "5"},
        {TokenType::PLUS, "+"},
        {TokenType::LEFT_PAREN, "("},
        {TokenType::INTEGER_LITERAL, "3"},
        {TokenType::MINUS, "-"},
        {TokenType::INTEGER_LITERAL, "2"},
        {TokenType::RIGHT_PAREN, ")"},
        {TokenType::DIVIDE, "/"},
        {TokenType::INTEGER_LITERAL, "0"}
    };

    unordered_map<string, variant<int, double, string>> variables = {};
    unordered_map<string, variant<int, double, string>> constants = {};

    ASSERT_THROW(Evaluator::evaluate_numeric(expression, variables, constants), runtime_error);
}
TEST(NUMERIC_EvaluatorTest, multiple_operations) 
{
    vector<Token> expression = 
    {
        {TokenType::INTEGER_LITERAL, "10"},
        {TokenType::PLUS, "+"},
        {TokenType::INTEGER_LITERAL, "5"},
        {TokenType::MINUS, "-"},
        {TokenType::INTEGER_LITERAL, "3"},
        {TokenType::MULTIPLY, "*"},
        {TokenType::INTEGER_LITERAL, "2"},
        {TokenType::DIVIDE, "/"},
        {TokenType::INTEGER_LITERAL, "1"}
    };
    unordered_map<string, variant<int, double, string>> variables = {};
    unordered_map<string, variant<int, double, string>> constants = {};
    EXPECT_EQ(Evaluator::evaluate_numeric(expression, variables, constants), 9);
}

TEST(NUMERIC_EvaluatorTest, handles_floating_point) 
{
    vector<Token> expression = 
    {
        {TokenType::INTEGER_LITERAL, "2.5"},
        {TokenType::PLUS, "+"},
        {TokenType::INTEGER_LITERAL, "3.5"}
    };

    unordered_map<string, variant<int, double, string>> variables = {};
    unordered_map<string, variant<int, double, string>> constants = {};
    EXPECT_EQ(Evaluator::evaluate_numeric(expression, variables, constants), 6);
}
TEST(NUMERIC_EvaluatorTest, long_chain) 
{
    vector<Token> expression = 
    {
        {TokenType::INTEGER_LITERAL, "1"},
        {TokenType::PLUS, "+"},
        {TokenType::INTEGER_LITERAL, "2"},
        {TokenType::PLUS, "+"},
        {TokenType::INTEGER_LITERAL, "3"},
        {TokenType::PLUS, "+"},
        {TokenType::INTEGER_LITERAL, "4"},
        {TokenType::PLUS, "+"},
        {TokenType::INTEGER_LITERAL, "5"}
    };

    unordered_map<string, variant<int, double, string>> variables = {};
    unordered_map<string, variant<int, double, string>> constants = {};
    EXPECT_EQ(Evaluator::evaluate_numeric(expression, variables, constants), 15);
}
TEST(NUMERIC_EvaluatorTest, mixed_variables_and_constants) 
{
    vector<Token> expression = 
    {
        {TokenType::IDENTIFIER, "x"},
        {TokenType::PLUS, "+"},
        {TokenType::IDENTIFIER, "PI"}
    };

    unordered_map<string, variant<int, double, string>> variables = { {"x", 10.0} };
    unordered_map<string, variant<int, double, string>> constants = { {"PI", 3.14} };
    EXPECT_TRUE(Evaluator::evaluate_numeric(expression, variables, constants), 13.14);
}
TEST(NUMERIC_EvaluatorTest, multiple_variables_multiplication) 
{
    vector<Token> expression = 
    {
        { TokenType::IDENTIFIER, "x" },
        { TokenType::MULTIPLY, "*" },
        { TokenType::IDENTIFIER, "y" }
    };

    unordered_map<string, variant<int, double, string>> variables = { {"x", 5.0}, {"y", 4.0} };
    unordered_map<string, variant<int, double, string>> constants = {};
    EXPECT_EQ(Evaluator::evaluate_numeric(expression, variables, constants), 20);
}

//STRING_PART

TEST(STRING_EvaluatorTest, can_evaluate_simple_expression)
{
    vector<Token> expression =
    {
        {TokenType:: STRING_LITERAL,"Hello, "},
        {TokenType::PLUS, "+"},
        {TokenType::STRING_LITERAL, "World!"}
    };
    unordered_map<string, variant<int, double, string>> variables = {};
    unordered_map<string, variant<int, double, string>> constants = {};
    EXPECT_EQ(Evaluator::evaluate_string(expression, variables, constants), "Hello, World!");
}

TEST(STRING_EvaluatorTest, can_evaluate_simple_expression_with_variables)
{
    vector<Token> expression =
    {
        {TokenType::STRING_LITERAL,"Hello, "},
        {TokenType::PLUS, "+"},
        {TokenType::IDENTIFIER, "str"}
    };
    unordered_map<string, variant<int, double, string>> variables = { {"str","World!"}};
    unordered_map<string, variant<int, double, string>> constants = {};
    EXPECT_EQ(Evaluator::evaluate_string(expression, variables, constants), "Hello, World!");
}

TEST(STRING_EvaluatorTest, two_variables) 
{
    vector<Token> expression = 
    {
        {TokenType::IDENTIFIER, "str1"},
        {TokenType::PLUS, "+"},
        {TokenType::IDENTIFIER, "str2"}
    };
    unordered_map<string, variant<int, double, string>> variables = { {"str1", "Hello, "}, {"str2", "World!"} };
    unordered_map<string, variant<int, double, string>> constants = {};
    EXPECT_EQ(Evaluator::evaluate_string(expression, variables, constants), "Hello, World!");
}

TEST(STRING_EvaluatorTest, variable_and_constant) 
{
    vector<Token> expression = 
    {
        {TokenType::IDENTIFIER, "greeting"},
        {TokenType::PLUS, "+"},
        {TokenType::IDENTIFIER, "planet"}
    };
    unordered_map<string, variant<int, double, string>> variables = { {"greeting", "Hello, "} };
    unordered_map<string, variant<int, double, string>> constants = { {"planet", "World!"} };
    EXPECT_EQ(Evaluator::evaluate_string(expression, variables, constants), "Hello, World!");
}

TEST(STRING_EvaluatorTest, constant_and_variable) 
{
    vector<Token> expression = 
    {
        {TokenType::IDENTIFIER, "greeting"},
        {TokenType::PLUS, "+"},
        {TokenType::IDENTIFIER, "planet"}
    };
    unordered_map<string, variant<int, double, string>> variables = { {"planet", "World!"} };
    unordered_map<string, variant<int, double, string>> constants = { {"greeting", "Hello, "} };
    EXPECT_EQ(Evaluator::evaluate_string(expression, variables, constants), "Hello, World!");
}

TEST(STRING_EvaluatorTest, constant_and_literal) 
{
    vector<Token> expression = 
    {
        {TokenType::IDENTIFIER, "greeting"},
        {TokenType::PLUS, "+"},
        {TokenType::STRING_LITERAL, "World!"}
    };
    unordered_map<string, variant<int, double, string>> variables = {};
    unordered_map<string, variant<int, double, string>> constants = { {"greeting", "Hello, "} };
    EXPECT_EQ(Evaluator::evaluate_string(expression, variables, constants), "Hello, World!");
}

TEST(STRING_EvaluatorTest, literal_and_constant) 
{
    vector<Token> expression = 
    {
        {TokenType::STRING_LITERAL, "Hello, "},
        {TokenType::PLUS, "+"},
        {TokenType::IDENTIFIER, "planet"}
    };
    unordered_map<string, variant<int, double, string>> variables = {};
    unordered_map<string, variant<int, double, string>> constants = { {"planet", "World!"} };
    EXPECT_EQ(Evaluator::evaluate_string(expression, variables, constants), "Hello, World!");
}

TEST(STRING_EvaluatorTest, missing_variable_throws) 
{
    vector<Token> expression = 
    {
        {TokenType::STRING_LITERAL, "Hello, "},
        {TokenType::PLUS, "+"},
        {TokenType::IDENTIFIER, "missing"}
    };
    unordered_map<string, variant<int, double, string>> variables = {};
    unordered_map<string, variant<int, double, string>> constants = {};
    ASSERT_THROW(Evaluator::evaluate_string(expression, variables, constants), runtime_error);
}

TEST(STRING_EvaluatorTest, missing_constant_throws) 
{
    vector<Token> expression = {
        {TokenType::STRING_LITERAL, "Hello, "},
        {TokenType::PLUS, "+"},
        {TokenType::IDENTIFIER, "missing"}
    };
    unordered_map<string, variant<int, double, string>> variables = {};
    unordered_map<string, variant<int, double, string>> constants = {};
    ASSERT_THROW(Evaluator::evaluate_string(expression, variables, constants), runtime_error);
}

TEST(STRING_EvaluatorTest, invalid_operation_throws) 
{
    vector<Token> expression = 
    {
        {TokenType::STRING_LITERAL, "Hello"},
        {TokenType::MINUS, "-"},
        {TokenType::STRING_LITERAL, "World"}
    };
    unordered_map<string, variant<int, double, string>> variables = {};
    unordered_map<string, variant<int, double, string>> constants = {};
    ASSERT_THROW(Evaluator::evaluate_string(expression, variables, constants), runtime_error);
}
TEST(STRING_EvaluatorTest, empty_string_literal) 
{
    vector<Token> expression = 
    {
        {TokenType::STRING_LITERAL, ""},
        {TokenType::PLUS, "+"},
        {TokenType::STRING_LITERAL, "World"}
    };
    unordered_map<string, variant<int, double, string>> variables = {};
    unordered_map<string, variant<int, double, string>> constants = {};
    EXPECT_EQ(Evaluator::evaluate_string(expression, variables, constants), "World");
}
TEST(STRING_EvaluatorTest, empty_string_variable) 
{
    vector<Token> expression = 
    {
        {TokenType::STRING_LITERAL, "Hello"},
        {TokenType::PLUS, "+"},
        {TokenType::IDENTIFIER, "empty"}
    };
    unordered_map<string, variant<int, double, string>> variables = { {"empty", ""} };
    unordered_map<string, variant<int, double, string>> constants = {};
    EXPECT_EQ(Evaluator::evaluate_string(expression, variables, constants), "Hello");
}
TEST(STRING_EvaluatorTest, empty_string_concat_empty_string) 
{
    vector<Token> expression = 
    {
        {TokenType::STRING_LITERAL, ""},
        {TokenType::PLUS, "+"},
        {TokenType::STRING_LITERAL, ""}
    };
    unordered_map<string, variant<int, double, string>> variables = {};
    unordered_map<string, variant<int, double, string>> constants = {};
    EXPECT_EQ(Evaluator::evaluate_string(expression, variables, constants), "");
}
TEST(STRING_EvaluatorTest, variable_at_start) 
{
    vector<Token> expression = 
    {
        {TokenType::IDENTIFIER, "greeting"},
        {TokenType::PLUS, "+"},
        {TokenType::STRING_LITERAL, " World"}
    };
    unordered_map<string, variant<int, double, string>> variables = { {"greeting", "Hello"} };
    unordered_map<string, variant<int, double, string>> constants = {};
    EXPECT_EQ(Evaluator::evaluate_string(expression, variables, constants), "Hello World");
}
TEST(STRING_EvaluatorTest, constants_at_start) 
{
    vector<Token> expression = 
    {
        {TokenType::IDENTIFIER, "x"},
        {TokenType::PLUS, "+"},
        {TokenType::STRING_LITERAL, " World"}
    };
    unordered_map<string, variant<int, double, string>> variables = {};
    unordered_map<string, variant<int, double, string>> constants = { {"x", "Hello"} };
    EXPECT_EQ(Evaluator::evaluate_string(expression, variables, constants), "Hello World");
}
TEST(STRING_EvaluatorTest, constants_and_variables) 
{
    vector<Token> expression = 
    {
        {TokenType::IDENTIFIER, "x"},
        {TokenType::PLUS, "+"},
        {TokenType::IDENTIFIER, "y"}
    };
    unordered_map<string, variant<int, double, string>> variables = { {"y", " World"} };
    unordered_map<string, variant<int, double, string>> constants = { {"x", "Hello"} };
    EXPECT_EQ(Evaluator::evaluate_string(expression, variables, constants), "Hello World");
}
TEST(STRING_EvaluatorTest, no_operations) 
{
    vector<Token> expression = 
    {
        {TokenType::STRING_LITERAL, "Hello"}
    };
    unordered_map<string, variant<int, double, string>> variables = {};
    unordered_map<string, variant<int, double, string>> constants = {};
    EXPECT_EQ(Evaluator::evaluate_string(expression, variables, constants), "Hello");
}

TEST(STRING_EvaluatorTest, no_operations_variable) 
{
    vector<Token> expression = 
    {
        {TokenType::IDENTIFIER, "greeting"}
    };
    unordered_map<string, variant<int, double, string>> variables = { {"greeting", "Hello"} };
    unordered_map<string, variant<int, double, string>> constants = {};
    EXPECT_EQ(Evaluator::evaluate_string(expression, variables, constants), "Hello");
}