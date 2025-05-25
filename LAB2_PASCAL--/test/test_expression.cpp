#include "pch.h"
#include "../ExpressionEvaluator/Expression.h"

TEST(ExpressionTest, can_create_expression) 
{
	vector<Token> expression =
	{
		{TokenTypes::INTEGER_LITERAL,"7"},
		{TokenTypes::PLUS, "+"},
		{TokenTypes::INTEGER_LITERAL, "3"}
	};
	ASSERT_NO_THROW(Expression tmp(expression));
}

TEST(ExpressionTest, can_perform_simple_expression)
{
	vector<Token> expression =
	{
		{TokenTypes::INTEGER_LITERAL,"7"},
		{TokenTypes::PLUS, "+"},
		{TokenTypes::INTEGER_LITERAL, "3"}
	};
	Expression tmp(expression);
	map <string, double> values = {};
	EXPECT_EQ(tmp.Calculate(values), 10);
}

TEST(ExpressionTest, handle_expression_without_operators)
{
	vector<Token> expression =
	{
		{TokenTypes::INTEGER_LITERAL,"7"}
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
		{TokenTypes::IDENTIFIER,"a"},
		{TokenTypes::PLUS, "+"},
		{TokenTypes::IDENTIFIER, "b"}
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
        {TokenTypes::INTEGER_LITERAL, "10"},
        {TokenTypes::MINUS, "-"},
        {TokenTypes::INTEGER_LITERAL, "3"}
    };
    Expression tmp(expression);
    map<string, double> values = {};
    EXPECT_EQ(tmp.Calculate(values), 7);
}

TEST(ExpressionTest, multiplication) 
{
    vector<Token> expression = 
    {
        {TokenTypes::INTEGER_LITERAL, "5"},
        {TokenTypes::MULTIPLY, "*"},
        {TokenTypes::INTEGER_LITERAL, "4"}
    };
    Expression tmp(expression);
    map<string, double> values = {};
    EXPECT_EQ(tmp.Calculate(values), 20);
}

TEST(ExpressionTest, division) 
{
    vector<Token> expression = 
    {
        {TokenTypes::INTEGER_LITERAL, "20"},
        {TokenTypes::DIVIDE, "/"},
        {TokenTypes::INTEGER_LITERAL, "4"}
    };
    Expression tmp(expression);
    map<string, double> values = {};
    EXPECT_EQ(tmp.Calculate(values), 5);
}

TEST(ExpressionTest, operator_precedence) 
{
    vector<Token> expression = 
    {
        {TokenTypes::INTEGER_LITERAL, "2"},
        {TokenTypes::PLUS, "+"},
        {TokenTypes::INTEGER_LITERAL, "3"},
        {TokenTypes::MULTIPLY, "*"},
        {TokenTypes::INTEGER_LITERAL, "4"}
    };
    Expression tmp(expression);
    map<string, double> values = {};
    EXPECT_EQ(tmp.Calculate(values), 14); 
}

TEST(ExpressionTest, parentheses) 
{
    vector<Token> expression = 
    {
        {TokenTypes::LEFT_PAREN, "("},
        {TokenTypes::INTEGER_LITERAL, "2"},
        {TokenTypes::PLUS, "+"},
        {TokenTypes::INTEGER_LITERAL, "3"},
        {TokenTypes::RIGHT_PAREN, ")"},
        {TokenTypes::MULTIPLY, "*"},
        {TokenTypes::INTEGER_LITERAL, "4"}
    };
    Expression tmp(expression);
    map<string, double> values = {};
    EXPECT_EQ(tmp.Calculate(values), 20); 
}

TEST(ExpressionTest, nested_parentheses) 
{
    vector<Token> expression = 
    {
        {TokenTypes::LEFT_PAREN, "("},
        {TokenTypes::INTEGER_LITERAL, "1"},
        {TokenTypes::PLUS, "+"},
        {TokenTypes::LEFT_PAREN, "("},
        {TokenTypes::INTEGER_LITERAL, "2"},
        {TokenTypes::MULTIPLY, "*"},
        {TokenTypes::INTEGER_LITERAL, "3"},
        {TokenTypes::RIGHT_PAREN, ")"},
        {TokenTypes::RIGHT_PAREN, ")"},
        {TokenTypes::MULTIPLY, "*"},
        {TokenTypes::INTEGER_LITERAL, "4"}
    };
    Expression tmp(expression);
    map<string, double> values = {};
    EXPECT_EQ(tmp.Calculate(values), 28);
}

TEST(ExpressionTest, negative_numbers) 
{
    vector<Token> expression = 
    {
        {TokenTypes::MINUS, "-"},
        {TokenTypes::INTEGER_LITERAL, "5"},
        {TokenTypes::PLUS, "+"},
        {TokenTypes::INTEGER_LITERAL, "3"}
    };
    Expression tmp(expression);
    map<string, double> values = {};
    EXPECT_EQ(tmp.Calculate(values), -2);
}

TEST(ExpressionTest, negative_numbers_with_parentheses) 
{
    vector<Token> expression = 
    {
        {TokenTypes::LEFT_PAREN, "("},
        {TokenTypes::MINUS, "-"},
        {TokenTypes::INTEGER_LITERAL, "5"},
        {TokenTypes::RIGHT_PAREN, ")"},
        {TokenTypes::MULTIPLY, "*"},
        {TokenTypes::INTEGER_LITERAL, "2"}
    };
    Expression tmp(expression);
    map<string, double> values = {};
    EXPECT_EQ(tmp.Calculate(values), -10);
}

TEST(ExpressionTest, division_by_zero_throws) 
{
    vector<Token> expression = {
        {TokenTypes::INTEGER_LITERAL, "10"},
        {TokenTypes::DIVIDE, "/"},
        {TokenTypes::INTEGER_LITERAL, "0"}
    };
    Expression tmp(expression);
    map<string, double> values = {};
    ASSERT_THROW(tmp.Calculate(values), runtime_error); 
}

TEST(ExpressionTest, invalid_expression_throws_in_constructor_unbalanced_parentheses) 
{
    vector<Token> expression = 
    {
        {TokenTypes::LEFT_PAREN, "("},
        {TokenTypes::INTEGER_LITERAL, "3"}
    };
    ASSERT_THROW(Expression tmp(expression), runtime_error);
}

TEST(ExpressionTest, can_perform_expression_with_floating_point) 
{
    vector<Token> expression =
    {
        {TokenTypes::INTEGER_LITERAL,"7.5"},
        {TokenTypes::PLUS, "+"},
        {TokenTypes::INTEGER_LITERAL, "2.5"}
    };
    Expression tmp(expression);
    map <string, double> values = {};
    EXPECT_EQ(tmp.Calculate(values), 10);
}

TEST(ExpressionTest, can_perform_expression_with_floating_point_and_int) 
{
    vector<Token> expression =
    {
        {TokenTypes::INTEGER_LITERAL,"7.5"},
        {TokenTypes::PLUS, "+"},
        {TokenTypes::INTEGER_LITERAL, "2"}
    };
    Expression tmp(expression);
    map <string, double> values = {};
    EXPECT_EQ(tmp.Calculate(values), 9.5);
}
TEST(ExpressionTest, can_perform_expression_with_floating_point_minus) 
{
    vector<Token> expression =
    {
        {TokenTypes::INTEGER_LITERAL,"7.5"},
        {TokenTypes::MINUS, "-"},
        {TokenTypes::INTEGER_LITERAL, "2.5"}
    };
    Expression tmp(expression);
    map <string, double> values = {};
    EXPECT_EQ(tmp.Calculate(values), 5);
}

TEST(ExpressionTest, can_perform_expression_with_floating_point_multiply) 
{
    vector<Token> expression =
    {
        {TokenTypes::INTEGER_LITERAL,"7.5"},
        {TokenTypes::MULTIPLY, "*"},
        {TokenTypes::INTEGER_LITERAL, "2.0"}
    };
    Expression tmp(expression);
    map <string, double> values = {};
    EXPECT_EQ(tmp.Calculate(values), 15);
}
TEST(ExpressionTest, can_perform_expression_with_floating_point_division) 
{
    vector<Token> expression =
    {
        {TokenTypes::INTEGER_LITERAL,"7.5"},
        {TokenTypes::DIVIDE, "/"},
        {TokenTypes::INTEGER_LITERAL, "2.5"}
    };
    Expression tmp(expression);
    map <string, double> values = {};
    EXPECT_EQ(tmp.Calculate(values), 3);
}
TEST(ExpressionTest, can_perform_expression_with_floating_point_and_variables) 
{
    vector<Token> expression =
    {
        {TokenTypes::IDENTIFIER,"a"},
        {TokenTypes::PLUS, "+"},
        {TokenTypes::IDENTIFIER, "b"}
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
        {TokenTypes::IDENTIFIER, "a"},
        {TokenTypes::PLUS, "+"},
        {TokenTypes::INTEGER_LITERAL, "3"}
    };
    Expression tmp(expression);
    map<string, double> values = {}; 
    ASSERT_THROW(tmp.Calculate(values), runtime_error); 
}
TEST(ExpressionTest, multiple_operations) 
{
    vector<Token> expression = 
    {
        {TokenTypes::INTEGER_LITERAL, "10"},
        {TokenTypes::PLUS, "+"},
        {TokenTypes::INTEGER_LITERAL, "5"},
        {TokenTypes::MINUS, "-"},
        {TokenTypes::INTEGER_LITERAL, "3"},
        {TokenTypes::MULTIPLY, "*"},
        {TokenTypes::INTEGER_LITERAL, "2"},
        {TokenTypes::DIVIDE, "/"},
        {TokenTypes::INTEGER_LITERAL, "1"}
    };
    Expression tmp(expression);
    map<string, double> values = {};
    EXPECT_EQ(tmp.Calculate(values), 9);
}
TEST(ExpressionTest, unbalanced_parentheses_left) 
{
    vector<Token> expression = 
    {
        {TokenTypes::LEFT_PAREN, "("},
        {TokenTypes::LEFT_PAREN, "("},
        {TokenTypes::INTEGER_LITERAL, "5"},
        {TokenTypes::PLUS, "+"},
        {TokenTypes::INTEGER_LITERAL, "3"},
        {TokenTypes::RIGHT_PAREN, ")"}
    };

    ASSERT_THROW(Expression tmp(expression), runtime_error);
}

TEST(ExpressionTest, unbalanced_parentheses_right) 
{
    vector<Token> expression = 
    {
        {TokenTypes::INTEGER_LITERAL, "5"},
        {TokenTypes::PLUS, "+"},
        {TokenTypes::INTEGER_LITERAL, "3"},
        {TokenTypes::RIGHT_PAREN, ")"}
    };

    ASSERT_THROW(Expression tmp(expression), runtime_error);
}

TEST(ExpressionTest, variable_multiplication) 
{
    vector<Token> expression = 
    {
        {TokenTypes::IDENTIFIER, "a"},
        {TokenTypes::MULTIPLY, "*"},
        {TokenTypes::INTEGER_LITERAL, "2"}
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
        {TokenTypes::LEFT_PAREN, "("},
        {TokenTypes::IDENTIFIER, "a"},
        {TokenTypes::PLUS, "+"},
        {TokenTypes::IDENTIFIER, "b"},
        {TokenTypes::RIGHT_PAREN, ")"},
        {TokenTypes::MULTIPLY, "*"},
        {TokenTypes::INTEGER_LITERAL, "2"}
    };
    Expression tmp(expression);
    const map<string, double> values = 
    {
        {"a", 5},
        {"b", 3}
    };
    EXPECT_EQ(tmp.Calculate(values), 16);
}