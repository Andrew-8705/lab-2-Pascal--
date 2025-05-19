#include "pch.h"
#include "../ExpressionEvaluator/Expression.h"

TEST(ExpressionTest, can_create_expression) 
{
	vector<Token> expression =
	{
		{TokenType::INTEGER_LITERAL,"7"},
		{TokenType::PLUS, "+"},
		{TokenType::INTEGER_LITERAL, "3"}
	};
	ASSERT_NO_THROW(Expression tmp(expression));
}

TEST(ExpressionTest, can_perform_simple_expression)
{
	vector<Token> expression =
	{
		{TokenType::INTEGER_LITERAL,"7"},
		{TokenType::PLUS, "+"},
		{TokenType::INTEGER_LITERAL, "3"}
	};
	Expression tmp(expression);
	map <string, double> values = {};
	EXPECT_EQ(tmp.Calculate(values), 10);
}

TEST(ExpressionTest, handle_expression_without_operators)
{
	vector<Token> expression =
	{
		{TokenType::INTEGER_LITERAL,"7"}
	};
	Expression tmp(expression);
	map <string, double> values = {};
	double result;
	ASSERT_NO_THROW(result = tmp.Calculate(values));
	EXPECT_EQ(result, 7);
}

TEST(ExpressionTest, handle_empty_expression)
{
	vector<Token> expression = {};
	ASSERT_NO_THROW(Expression tmp(expression));
}

TEST(ExpressionTest, throws_when_perform_empty_expression)
{
	vector<Token> expression = {};
	Expression tmp(expression);
	map <string, double> values = {};
	double result;
	ASSERT_ANY_THROW(result = tmp.Calculate(values));
}

TEST(ExpressionTest, can_perform_simple_expression_with_variables)
{
	vector<Token> expression =
	{
		{TokenType::IDENTIFIER,"a"},
		{TokenType::PLUS, "+"},
		{TokenType::IDENTIFIER, "b"}
	};
	Expression tmp(expression);
	const map <string, double> values = 
	{
		{"a",7},
		{"b", 3}
	};
	EXPECT_EQ(tmp.Calculate(values), 10);
}

TEST(ExpressionTest, subtraction) 
{
    vector<Token> expression = 
    {
        {TokenType::INTEGER_LITERAL, "10"},
        {TokenType::MINUS, "-"},
        {TokenType::INTEGER_LITERAL, "3"}
    };
    Expression tmp(expression);
    map<string, double> values = {};
    EXPECT_EQ(tmp.Calculate(values), 7);
}

TEST(ExpressionTest, multiplication) 
{
    vector<Token> expression = 
    {
        {TokenType::INTEGER_LITERAL, "5"},
        {TokenType::MULTIPLY, "*"},
        {TokenType::INTEGER_LITERAL, "4"}
    };
    Expression tmp(expression);
    map<string, double> values = {};
    EXPECT_EQ(tmp.Calculate(values), 20);
}

TEST(ExpressionTest, division) 
{
    vector<Token> expression = 
    {
        {TokenType::INTEGER_LITERAL, "20"},
        {TokenType::DIVIDE, "/"},
        {TokenType::INTEGER_LITERAL, "4"}
    };
    Expression tmp(expression);
    map<string, double> values = {};
    EXPECT_EQ(tmp.Calculate(values), 5);
}

TEST(ExpressionTest, operator_precedence) 
{
    vector<Token> expression = 
    {
        {TokenType::INTEGER_LITERAL, "2"},
        {TokenType::PLUS, "+"},
        {TokenType::INTEGER_LITERAL, "3"},
        {TokenType::MULTIPLY, "*"},
        {TokenType::INTEGER_LITERAL, "4"}
    };
    Expression tmp(expression);
    map<string, double> values = {};
    EXPECT_EQ(tmp.Calculate(values), 14); 
}

TEST(ExpressionTest, parentheses) 
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
    Expression tmp(expression);
    map<string, double> values = {};
    EXPECT_EQ(tmp.Calculate(values), 20); 
}

TEST(ExpressionTest, nested_parentheses) 
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
    Expression tmp(expression);
    map<string, double> values = {};
    EXPECT_EQ(tmp.Calculate(values), 28);
}

TEST(ExpressionTest, negative_numbers) 
{
    vector<Token> expression = 
    {
        {TokenType::MINUS, "-"},
        {TokenType::INTEGER_LITERAL, "5"},
        {TokenType::PLUS, "+"},
        {TokenType::INTEGER_LITERAL, "3"}
    };
    Expression tmp(expression);
    map<string, double> values = {};
    EXPECT_EQ(tmp.Calculate(values), -2);
}

TEST(ExpressionTest, negative_numbers_with_parentheses) 
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
    Expression tmp(expression);
    map<string, double> values = {};
    EXPECT_EQ(tmp.Calculate(values), -10);
}

TEST(ExpressionTest, division_by_zero_throws) 
{
    vector<Token> expression = {
        {TokenType::INTEGER_LITERAL, "10"},
        {TokenType::DIVIDE, "/"},
        {TokenType::INTEGER_LITERAL, "0"}
    };
    Expression tmp(expression);
    map<string, double> values = {};
    ASSERT_THROW(tmp.Calculate(values), runtime_error); 
}

TEST(ExpressionTest, invalid_expression_throws_in_constructor_unbalanced_parentheses) 
{
    vector<Token> expression = 
    {
        {TokenType::LEFT_PAREN, "("},
        {TokenType::INTEGER_LITERAL, "3"}
    };
    ASSERT_THROW(Expression tmp(expression), runtime_error);
}

TEST(ExpressionTest, can_perform_expression_with_floating_point) 
{
    vector<Token> expression =
    {
        {TokenType::INTEGER_LITERAL,"7.5"},
        {TokenType::PLUS, "+"},
        {TokenType::INTEGER_LITERAL, "2.5"}
    };
    Expression tmp(expression);
    map <string, double> values = {};
    EXPECT_EQ(tmp.Calculate(values), 10);
}

TEST(ExpressionTest, can_perform_expression_with_floating_point_and_int) 
{
    vector<Token> expression =
    {
        {TokenType::INTEGER_LITERAL,"7.5"},
        {TokenType::PLUS, "+"},
        {TokenType::INTEGER_LITERAL, "2"}
    };
    Expression tmp(expression);
    map <string, double> values = {};
    EXPECT_EQ(tmp.Calculate(values), 9.5);
}
TEST(ExpressionTest, can_perform_expression_with_floating_point_minus) 
{
    vector<Token> expression =
    {
        {TokenType::INTEGER_LITERAL,"7.5"},
        {TokenType::MINUS, "-"},
        {TokenType::INTEGER_LITERAL, "2.5"}
    };
    Expression tmp(expression);
    map <string, double> values = {};
    EXPECT_EQ(tmp.Calculate(values), 5);
}

TEST(ExpressionTest, can_perform_expression_with_floating_point_multiply) 
{
    vector<Token> expression =
    {
        {TokenType::INTEGER_LITERAL,"7.5"},
        {TokenType::MULTIPLY, "*"},
        {TokenType::INTEGER_LITERAL, "2.0"}
    };
    Expression tmp(expression);
    map <string, double> values = {};
    EXPECT_EQ(tmp.Calculate(values), 15);
}
TEST(ExpressionTest, can_perform_expression_with_floating_point_division) 
{
    vector<Token> expression =
    {
        {TokenType::INTEGER_LITERAL,"7.5"},
        {TokenType::DIVIDE, "/"},
        {TokenType::INTEGER_LITERAL, "2.5"}
    };
    Expression tmp(expression);
    map <string, double> values = {};
    EXPECT_EQ(tmp.Calculate(values), 3);
}
TEST(ExpressionTest, can_perform_expression_with_floating_point_and_variables) 
{
    vector<Token> expression =
    {
        {TokenType::IDENTIFIER,"a"},
        {TokenType::PLUS, "+"},
        {TokenType::IDENTIFIER, "b"}
    };
    Expression tmp(expression);
    const map <string, double> values =
    {
        {"a",7.5},
        {"b", 2.5}
    };
    EXPECT_EQ(tmp.Calculate(values), 10);
}
TEST(ExpressionTest, missing_variable_throws) 
{
    vector<Token> expression = {
        {TokenType::IDENTIFIER, "a"},
        {TokenType::PLUS, "+"},
        {TokenType::INTEGER_LITERAL, "3"}
    };
    Expression tmp(expression);
    map<string, double> values = {}; 
    ASSERT_THROW(tmp.Calculate(values), runtime_error); 
}
TEST(ExpressionTest, multiple_operations) 
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
    Expression tmp(expression);
    map<string, double> values = {};
    EXPECT_EQ(tmp.Calculate(values), 9);
}
TEST(ExpressionTest, unbalanced_parentheses_left) 
{
    vector<Token> expression = 
    {
        {TokenType::LEFT_PAREN, "("},
        {TokenType::LEFT_PAREN, "("},
        {TokenType::INTEGER_LITERAL, "5"},
        {TokenType::PLUS, "+"},
        {TokenType::INTEGER_LITERAL, "3"},
        {TokenType::RIGHT_PAREN, ")"}
    };

    ASSERT_THROW(Expression tmp(expression), runtime_error);
}

TEST(ExpressionTest, unbalanced_parentheses_right) 
{
    vector<Token> expression = 
    {
        {TokenType::INTEGER_LITERAL, "5"},
        {TokenType::PLUS, "+"},
        {TokenType::INTEGER_LITERAL, "3"},
        {TokenType::RIGHT_PAREN, ")"}
    };

    ASSERT_THROW(Expression tmp(expression), runtime_error);
}

TEST(ExpressionTest, variable_multiplication) 
{
    vector<Token> expression = 
    {
        {TokenType::IDENTIFIER, "a"},
        {TokenType::MULTIPLY, "*"},
        {TokenType::INTEGER_LITERAL, "2"}
    };
    Expression tmp(expression);
    const map<string, double> values = 
    {
        {"a", 5}
    };
    EXPECT_EQ(tmp.Calculate(values), 10);
}

TEST(ExpressionTest, complex_expression_with_variables_and_parentheses) 
{
    vector<Token> expression = 
    {
        {TokenType::LEFT_PAREN, "("},
        {TokenType::IDENTIFIER, "a"},
        {TokenType::PLUS, "+"},
        {TokenType::IDENTIFIER, "b"},
        {TokenType::RIGHT_PAREN, ")"},
        {TokenType::MULTIPLY, "*"},
        {TokenType::INTEGER_LITERAL, "2"}
    };
    Expression tmp(expression);
    const map<string, double> values = 
    {
        {"a", 5},
        {"b", 3}
    };
    EXPECT_EQ(tmp.Calculate(values), 16);
}