#include "pch.h"
#include "../Base/Node.h"
#include "../Base/Token.h"
#include "../Lexer/Lexer.h"
#include "../Parser/Parser.h"
#include <optional>
#include <variant>
#include <memory>

using namespace std;

bool CompareVariant(const variant<int, double, string>& v1, const variant<int, double, string>& v2) {
    if (v1.index() != v2.index()) return false;
    if (v1.index() == 0) return get<int>(v1) == get<int>(v2);
    if (v1.index() == 1) return get<double>(v1) == get<double>(v2);
    if (v1.index() == 2) return get<string>(v1) == get<string>(v2);
    return true;
}

bool CompareNodes(shared_ptr<Node> n1, shared_ptr<Node> n2) {
    if (n1 == nullptr && n2 == nullptr) return true;
    if (n1 == nullptr || n2 == nullptr) return false;

    if (n1->type != n2->type) return false;

    switch (n1->type) {
    case Node::NodeType::PROGRAM_STATEMENT: {
        shared_ptr<ProgramNode> p1 = dynamic_pointer_cast<ProgramNode>(n1);
        shared_ptr<ProgramNode> p2 = dynamic_pointer_cast<ProgramNode>(n2);
        if (!p1 || !p2) return false;
        return p1->programName == p2->programName;
    }
    case Node::NodeType::CONST_SECTION:
    case Node::NodeType::VAR_SECTION:
    case Node::NodeType::BEGIN_SECTION:
        return true;
    case Node::NodeType::CONST_DECLARATION: {
        shared_ptr<ConstDeclarationNode> c1 = dynamic_pointer_cast<ConstDeclarationNode>(n1);
        shared_ptr<ConstDeclarationNode> c2 = dynamic_pointer_cast<ConstDeclarationNode>(n2);
        if (!c1 || !c2) return false;
        return c1->identifier == c2->identifier && c1->type == c2->type && CompareVariant(c1->value, c2->value);
    }
    case Node::NodeType::VARIABLE_DECLARATION: {
        shared_ptr<VariableDeclarationNode> v1 = dynamic_pointer_cast<VariableDeclarationNode>(n1);
        shared_ptr<VariableDeclarationNode> v2 = dynamic_pointer_cast<VariableDeclarationNode>(n2);
        if (!v1 || !v2) return false;
        if (!CompareNodes(v1->identifierList, v2->identifierList)) return false;
        return v1->type == v2->type;
    }
    case Node::NodeType::IDENTIFIER_LIST: {
        shared_ptr<IdentifierListNode> l1 = dynamic_pointer_cast<IdentifierListNode>(n1);
        shared_ptr<IdentifierListNode> l2 = dynamic_pointer_cast<IdentifierListNode>(n2);
        if (!l1 || !l2) return false;
        if (l1->identifiers.size() != l2->identifiers.size()) return false;
        auto it1 = l1->identifiers.begin();
        auto it2 = l2->identifiers.begin();
        while (it1 != l1->identifiers.end()) {
            if (*it1 != *it2) return false;
            ++it1;
            ++it2;
        }
        return true;
    }
    case Node::NodeType::ASSIGNMENT_STATEMENT: {
        shared_ptr<AssignmentStatementNode> a1 = dynamic_pointer_cast<AssignmentStatementNode>(n1);
        shared_ptr<AssignmentStatementNode> a2 = dynamic_pointer_cast<AssignmentStatementNode>(n2);
        if (!a1 || !a2) return false;
        if (a1->variableName != a2->variableName || a1->expression.size() != a2->expression.size()) return false;
        for (size_t i = 0; i < a1->expression.size(); ++i) {
            if (a1->expression[i].type != a2->expression[i].type || a1->expression[i].value != a2->expression[i].value) return false;
        }
        return true;
    }
    case Node::NodeType::WRITE_STATEMENT: {
        shared_ptr<WriteStatementNode> w1 = dynamic_pointer_cast<WriteStatementNode>(n1);
        shared_ptr<WriteStatementNode> w2 = dynamic_pointer_cast<WriteStatementNode>(n2);
        if (!w1 || !w2) return false;
        if (w1->expression.size() != w2->expression.size()) return false;
        for (size_t i = 0; i < w1->expression.size(); ++i) {
            if (w1->expression[i].type != w2->expression[i].type || w1->expression[i].value != w2->expression[i].value) return false;
        }
        return true;
    }
    case Node::NodeType::READ_STATEMENT: {
        shared_ptr<ReadStatementNode> r1 = dynamic_pointer_cast<ReadStatementNode>(n1);
        shared_ptr<ReadStatementNode> r2 = dynamic_pointer_cast<ReadStatementNode>(n2);
        if (!r1 || !r2) return false;
        return CompareNodes(r1->identifierList, r2->identifierList);
    }
    case Node::NodeType::IF_STATEMENT: {
        shared_ptr<IfStatementNode> i1 = dynamic_pointer_cast<IfStatementNode>(n1);
        shared_ptr<IfStatementNode> i2 = dynamic_pointer_cast<IfStatementNode>(n2);
        if (!i1 || !i2) return false;

        if (i1->condition.size() != i2->condition.size()) return false;
        for (size_t i = 0; i < i1->condition.size(); ++i) {
            if (i1->condition[i].type != i2->condition[i].type || i1->condition[i].value != i2->condition[i].value) return false;
        }

        if (i1->thenStatement.size() != i2->thenStatement.size()) return false;
        auto thenIt1 = i1->thenStatement.begin();
        auto thenIt2 = i2->thenStatement.begin();
        while (thenIt1 != i1->thenStatement.end()) {
            if (!CompareNodes(*thenIt1, *thenIt2)) return false;
            ++thenIt1;
            ++thenIt2;
        }

        if (i1->elseStatement.size() != i2->elseStatement.size()) return false;
        auto elseIt1 = i1->elseStatement.begin();
        auto elseIt2 = i2->elseStatement.begin();
        while (elseIt1 != i1->elseStatement.end()) {
            if (!CompareNodes(*elseIt1, *elseIt2)) return false;
            ++elseIt1;
            ++elseIt2;
        }
        return true;
    }
    default:
        return false;
    }
}

bool CompareAST(const list<list<shared_ptr<Node>>>& ast1, const list<list<shared_ptr<Node>>>& ast2) {
    if (ast1.size() != ast2.size()) return false;
    auto it1 = ast1.begin();
    auto it2 = ast2.begin();
    while (it1 != ast1.end()) {
        if (it1->size() != it2->size()) return false;
        auto blockIt1 = it1->begin();
        auto blockIt2 = it2->begin();
        while (blockIt1 != it1->end()) {
            if (!CompareNodes(*blockIt1, *blockIt2)) return false;
            ++blockIt1;
            ++blockIt2;
        }
        ++it1;
        ++it2;
    }
    return true;
}

vector<Token> tokenize(const string& source) {
    Lexer lexer(source);
    return lexer.tokenize();
}

TEST(ParserTest, can_parse_empty_program) {
    Parser parser(tokenize(R"(program Empty;
                                  begin
                                  end.)"));
    auto ast = parser.parse();
    list<list<shared_ptr<Node>>> expectedAst = {
        { make_shared<ProgramNode>("Empty") },
        { make_shared<BeginSectionNode>() }
    };
    EXPECT_TRUE(CompareAST(ast, expectedAst));
}

TEST(ParserTest, can_parse_all_types_in_const_declaration) {
    Parser parser(tokenize(R"(program ConstDecl;
                                  const
                                    PI : double = 3.14;
                                    Count : integer = 100;
                                    message : string = "Hello";
                                  begin end.)"));
    auto ast = parser.parse();
    list<list<shared_ptr<Node>>> expectedAst = {
        { make_shared<ProgramNode>("ConstDecl") },
        { make_shared<ConstSectionNode>(),
          make_shared<ConstDeclarationNode>("PI", "double", 3.14),
          make_shared<ConstDeclarationNode>("Count", "integer", 100),
          make_shared<ConstDeclarationNode>("message", "string", "Hello")
        },
        { make_shared<BeginSectionNode>() }
    };
    EXPECT_TRUE(CompareAST(ast, expectedAst));
}

TEST(ParserTest, can_parse_all_types_in_var_section) {
    Parser parser(tokenize(R"(program VarSection;
                                  var
                                    x, y : double;
                                    count : integer;
                                    message1, message2 : string;
                                  begin end.)"));
    auto ast = parser.parse();
    list<list<shared_ptr<Node>>> expectedAst = {
        { make_shared<ProgramNode>("VarSection") },
        { make_shared<VarSectionNode>(),
          make_shared<VariableDeclarationNode>(make_shared<IdentifierListNode>(initializer_list<string>{ "x", "y" }), "double"),
          make_shared<VariableDeclarationNode>(make_shared<IdentifierListNode>(initializer_list<string>{ "count" }), "integer"),
          make_shared<VariableDeclarationNode>(make_shared<IdentifierListNode>(initializer_list<string>{ "message1", "message2"}), "string")
        },
        { make_shared<BeginSectionNode>() }
    };
    EXPECT_TRUE(CompareAST(ast, expectedAst));
}

TEST(ParserTest, can_parse_assignment_statement) {
    Parser parser(tokenize(R"(program AssignStmt;
                                  begin
                                    counter := 10;
                                  end.)"));
    auto ast = parser.parse();
    vector<Token> expression = { { TokenTypes::INTEGER_LITERAL, "10", 3, 31 } };
    list<list<shared_ptr<Node>>> expectedAst = {
        { make_shared<ProgramNode>("AssignStmt") },
        { make_shared<BeginSectionNode>(), make_shared<AssignmentStatementNode>("counter", expression) }
    };
    EXPECT_TRUE(CompareAST(ast, expectedAst));
}

TEST(ParserTest, can_parse_write_statement_single_literal) {
    Parser parser(tokenize(R"(program WriteLit;
                                 begin
                                   Write("Hello");
                                 end.)"));
    auto ast = parser.parse();
    vector<Token> expression = { { TokenTypes::STRING_LITERAL, "Hello", 3, 26 } };
    list<list<shared_ptr<Node>>> expectedAst = {
        { make_shared<ProgramNode>("WriteLit") },
        { make_shared<BeginSectionNode>(), make_shared<WriteStatementNode>(expression) }
    };
    EXPECT_TRUE(CompareAST(ast, expectedAst));
}

TEST(ParserTest, can_parse_read_statement_single_identifier) {
    Parser parser(tokenize(R"(program ReadId;
                                  var
                                    input: string;
                                  begin
                                    Read(input);
                                  end.)"));
    auto ast = parser.parse();
    shared_ptr<IdentifierListNode> idList = make_shared<IdentifierListNode>(initializer_list<string>{ "input" });
    list<list<shared_ptr<Node>>> expectedAst = {
        { make_shared<ProgramNode>("ReadId") },
        { make_shared<VarSectionNode>(), make_shared<VariableDeclarationNode>(idList, "string") },
        { make_shared<BeginSectionNode>(), make_shared<ReadStatementNode>(idList) }
    };
    EXPECT_TRUE(CompareAST(ast, expectedAst));
}

TEST(ParserTest, can_parse_if_statement_without_else) {
    Parser parser(tokenize(R"(program IfNoElse;
                                  begin
                                    if (flag) then
                                        result := 1;
                                  end.)"));
    auto ast = parser.parse();
    vector<Token> condition = { { TokenTypes::IDENTIFIER, "flag", 3, 30 } };
    list<shared_ptr<Node>> thenBlock = { make_shared<AssignmentStatementNode>("result", initializer_list<Token>{ { TokenTypes::INTEGER_LITERAL, "1", 4, 38 } }) };
    list<list<shared_ptr<Node>>> expectedAst = {
        { make_shared<ProgramNode>("IfNoElse") },
        { make_shared<BeginSectionNode>(), make_shared<IfStatementNode>(condition, thenBlock, nullopt) }
    };
    EXPECT_TRUE(CompareAST(ast, expectedAst));
}

TEST(ParserTest, can_parse_if_statement_with_else) {
    Parser parser(tokenize(R"(program IfElse;
                                  begin
                                    if (count > 0) then
                                        value := 1;
                                    else
                                        value := -1;
                                  end.)"));
    auto ast = parser.parse();
    vector<Token> condition = { { TokenTypes::IDENTIFIER, "count", 3, 29 }, { TokenTypes::GREATER, ">", 3, 35 }, { TokenTypes::INTEGER_LITERAL, "0", 3, 37 } };
    list<shared_ptr<Node>> thenBlock = { make_shared<AssignmentStatementNode>("value", initializer_list<Token>{ { TokenTypes::INTEGER_LITERAL, "1", 4, 39 } }) };
    list<shared_ptr<Node>> elseBlock = { make_shared<AssignmentStatementNode>("value", initializer_list<Token>{ { TokenTypes::MINUS, "-", 6, 39 }, { TokenTypes::INTEGER_LITERAL, "1", 6, 40 } }) };
    list<list<shared_ptr<Node>>> expectedAst = {
        { make_shared<ProgramNode>("IfElse") },
        { make_shared<BeginSectionNode>(), make_shared<IfStatementNode>(condition, thenBlock, elseBlock) }
    };
    EXPECT_TRUE(CompareAST(ast, expectedAst));
}

TEST(ParserTest, handles_syntax_error_missing_semicolon_program_header) {
    EXPECT_THROW({
        Parser parser(tokenize(R"(program ErrorNoSemicolon
                                  begin
                                  end.)"));
        parser.parse();
        }, runtime_error);
}

TEST(ParserTest, handles_syntax_error_missing_semicolon_const_declaration) {
    EXPECT_THROW({
        Parser parser(tokenize(R"(program ErrorConstNoSemicolon;
                                  const
                                    PI : double = 3.14
                                  begin
                                  end.)"));
        parser.parse();
        }, runtime_error);
}

TEST(ParserTest, handles_syntax_error_missing_colon_const_declaration) {
    EXPECT_THROW({
        Parser parser(tokenize(R"(program ErrorConstNoColon;
                                  const
                                    PI double = 3.14;
                                  begin
                                  end.)"));
        parser.parse();
        }, runtime_error);
}

TEST(ParserTest, handles_syntax_error_missing_equal_const_declaration) {
    EXPECT_THROW({
        Parser parser(tokenize(R"(program ErrorConstNoEqual;
                                  const
                                    PI : double 3.14;
                                  begin
                                  end.)"));
        parser.parse();
        }, runtime_error);
}

TEST(ParserTest, handles_syntax_error_missing_semicolon_var_declaration) {
    EXPECT_THROW({
        Parser parser(tokenize(R"(program ErrorVarNoSemicolon;
                                  var
                                    value : integer
                                  begin
                                  end.)"));
        parser.parse();
        }, runtime_error);
}

TEST(ParserTest, handles_syntax_error_missing_colon_var_declaration) {
    EXPECT_THROW({
        Parser parser(tokenize(R"(program ErrorVarNoColon;
                                  var
                                    value integer;
                                  begin
                                  end.)"));
        parser.parse();
        }, runtime_error);
}

TEST(ParserTest, handles_syntax_error_missing_semicolon_assignment) {
    EXPECT_THROW({
        Parser parser(tokenize(R"(program ErrorAss;
                                  begin
                                    counter := a
                                  end.)"));
        parser.parse();
        }, runtime_error);
}

TEST(ParserTest, handles_syntax_error_missing_left_paren_write) {
    EXPECT_THROW({
        Parser parser(tokenize(R"(program ErrorWriteNoLParen;
                                  begin
                                    Write value);
                                  end.)"));
        parser.parse();
        }, runtime_error);
}

TEST(ParserTest, handles_syntax_error_missing_right_paren_write) {
    EXPECT_THROW({
        Parser parser(tokenize(R"(program ErrorWriteNoRParen;
                                  begin
                                    Write(value;
                                  end.)"));
        parser.parse();
        }, runtime_error);
}

TEST(ParserTest, handles_syntax_error_missing_left_paren_read) {
    EXPECT_THROW({
        Parser parser(tokenize(R"(program ErrorReadNoLParen;
                                  begin
                                    Read value);
                                  end.)"));
        parser.parse();
        }, runtime_error);
}

TEST(ParserTest, handles_syntax_error_missing_right_paren_read) {
    EXPECT_THROW({
        Parser parser(tokenize(R"(program ErrorReadNoRParen;
                                  begin
                                    Read(value;
                                  end.)"));
        parser.parse();
        }, runtime_error);
}

TEST(ParserTest, handles_syntax_error_missing_then_if) {
    EXPECT_THROW({
        Parser parser(tokenize(R"(program ErrorIfNoThen;
                                  begin
                                    if (x > 0)
                                        y := 1;
                                  end.)"));
        parser.parse();
        }, runtime_error);
}

TEST(ParserTest, handles_syntax_error_missing_right_paren_if_condition) {
    EXPECT_THROW({
        Parser parser(tokenize(R"(program ErrorIfNoRParen;
                                  begin
                                    if (x > 0 then
                                        y := 1;
                                  end.)"));
        parser.parse();
        }, runtime_error);
}

TEST(ParserTest, handles_syntax_error_unexpected_token_after_var) {
    EXPECT_THROW({
        Parser parser(tokenize(R"(program ErrorUnexpectedAfterVar;
                                  var
                                  begin
                                    x : integer;
                                  end.)"));
        parser.parse();
        }, runtime_error);
}

TEST(ParserTest, handles_syntax_error_missing_end_dot) {
    EXPECT_THROW({
        Parser parser(tokenize(R"(program ErrorMissingEndDot;
                                  begin
                                  end)"));
        parser.parse();
        }, runtime_error);
}

TEST(ParserTest, handles_syntax_error_empty_write_statement) {
    EXPECT_THROW({
        Parser parser(tokenize(R"(program EmptyWrite;
                                  begin
                                    Write();
                                  end.)"));
        parser.parse();
        }, runtime_error);
}

TEST(ParserTest, handles_empty_read_statement_error) {
    EXPECT_THROW({
        Parser parser(tokenize(R"(program EmptyRead;
                                  begin
                                    Read();
                                  end.)"));
        parser.parse();
        }, runtime_error);
}

TEST(ParserTest, can_parse_assignment_with_identifier) {
    Parser parser(tokenize(R"(program AssignId;
                                  begin
                                    a := b;
                                  end.)"));
    auto ast = parser.parse();
    list<list<shared_ptr<Node>>> expectedAst = {
        { make_shared<ProgramNode>("AssignId") },
        { make_shared<BeginSectionNode>(), make_shared<AssignmentStatementNode>("a", initializer_list<Token>{ { TokenTypes::IDENTIFIER, "b", 3, 31 } }) }
    };
    EXPECT_TRUE(CompareAST(ast, expectedAst));
}

TEST(ParserTest, can_parse_write_statement_multiple_args_with_commas) {
    Parser parser(tokenize(R"(program WriteMulti;
                                  begin
                                    Write("Hello", 10, 10.10, var1, var2);
                                  end.)"));
    auto ast = parser.parse();
    vector<Token> expr = {
        { TokenTypes::STRING_LITERAL, "Hello", 3, 26 },
        { TokenTypes::COMMA, ",", 3, 33 },
        { TokenTypes::INTEGER_LITERAL, "10", 3, 35 },
        { TokenTypes::COMMA, ",", 3, 37 },
        { TokenTypes::DOUBLE_LITERAL, "10.10", 3, 39 },
        { TokenTypes::COMMA, ",", 3, 45 },
        { TokenTypes::IDENTIFIER, "var1", 3, 47 },
        { TokenTypes::COMMA, ",", 3, 51 },
        { TokenTypes::IDENTIFIER, "var2", 3, 53 }
    };
    list<list<shared_ptr<Node>>> expectedAst = {
        { make_shared<ProgramNode>("WriteMulti") },
        { make_shared<BeginSectionNode>(), make_shared<WriteStatementNode>(expr) }
    };
    EXPECT_TRUE(CompareAST(ast, expectedAst));
}

TEST(ParserTest, can_parse_read_statement_multiple_ids) {
    Parser parser(tokenize(R"(program ReadMulti;
                                  var
                                    a, b: integer;
                                    c : double;
                                    d, e : string;
                                  begin
                                    Read(a, b, c, d, e);
                                  end.)"));
    auto ast = parser.parse();
    shared_ptr<IdentifierListNode> readIdList = make_shared<IdentifierListNode>(initializer_list<string>{ "a", "b", "c", "d", "e" });
    list<list<shared_ptr<Node>>> expectedAst = {
        { make_shared<ProgramNode>("ReadMulti") },
        { make_shared<VarSectionNode>(),
          make_shared<VariableDeclarationNode>(make_shared<IdentifierListNode>(initializer_list<string>{ "a", "b" }), "integer"),
          make_shared<VariableDeclarationNode>(make_shared<IdentifierListNode>(initializer_list<string>{ "c" }), "double"),
          make_shared<VariableDeclarationNode>(make_shared<IdentifierListNode>(initializer_list<string>{ "d", "e" }), "string")
        },
        { make_shared<BeginSectionNode>(), make_shared<ReadStatementNode>(readIdList) }
    };
    EXPECT_TRUE(CompareAST(ast, expectedAst));
}


TEST(ParserTest, can_parse_nested_if_without_else) {
    Parser parser(tokenize(R"(program NestedIfNoElse;
                                  begin
                                    if (a > 0) then
                                        if (b < 10) then
                                            result := 5;
                                  end.)"));
    auto ast = parser.parse();

    vector<Token> condition1 = { { TokenTypes::IDENTIFIER, "a", 3, 30 }, 
                                    { TokenTypes::GREATER, ">", 3, 32 }, 
                                    { TokenTypes::INTEGER_LITERAL, "0", 3, 34 } };
    vector<Token> condition2 = { { TokenTypes::IDENTIFIER, "b", 4, 38 }, 
                                    { TokenTypes::LESS, "<", 4, 40 }, 
                                    { TokenTypes::INTEGER_LITERAL, "10", 4, 42 } };
    list<shared_ptr<Node>> thenBlockInner = { make_shared<AssignmentStatementNode>("result", initializer_list<Token>{ { TokenTypes::INTEGER_LITERAL, "5", 5, 49 } }) };
    list<shared_ptr<Node>> thenBlockOuter = { make_shared<IfStatementNode>(condition2, thenBlockInner, nullopt) };

    list<list<shared_ptr<Node>>> expectedAst = {
        { make_shared<ProgramNode>("NestedIfNoElse") },
        { make_shared<BeginSectionNode>(), make_shared<IfStatementNode>(condition1, thenBlockOuter, nullopt) }
    };

    EXPECT_TRUE(CompareAST(ast, expectedAst));
}

TEST(ParserTest, can_parse_nested_if_with_else) {
    Parser parser(tokenize(R"(program NestedIfWithElse;
                                  begin
                                    if (x = 1) then
                                        if (y <> 2) then
                                            value := true;
                                        else
                                            value := false;
                                    else
                                        error := -1;
                                  end.)"));
    auto ast = parser.parse();

    vector<Token> conditionOuter = { { TokenTypes::IDENTIFIER, "x", 3, 29 }, 
                                        { TokenTypes::EQUAL, "=", 3, 31 }, 
                                        { TokenTypes::INTEGER_LITERAL, "1", 3, 33 } };
    vector<Token> conditionInner = { { TokenTypes::IDENTIFIER, "y", 4, 38 }, 
                                        { TokenTypes::NON_EQUAL, "<>", 4, 40 }, 
                                        { TokenTypes::INTEGER_LITERAL, "2", 4, 43 } };
    list<shared_ptr<Node>> thenBlockInner = { make_shared<AssignmentStatementNode>("value", initializer_list<Token>{ { TokenTypes::IDENTIFIER, "true", 5, 49 } }) };
    list<shared_ptr<Node>> elseBlockInner = { make_shared<AssignmentStatementNode>("value", initializer_list<Token>{ { TokenTypes::IDENTIFIER, "false", 7, 49 } }) };
    list<shared_ptr<Node>> thenBlockOuter = { make_shared<IfStatementNode>(conditionInner, thenBlockInner, elseBlockInner) };
    list<shared_ptr<Node>> elseBlockOuter = { make_shared<AssignmentStatementNode>("error", initializer_list<Token>{ { TokenTypes::MINUS, "-", 9, 49 }, { TokenTypes::INTEGER_LITERAL, "1", 9, 50 } }) };

    list<list<shared_ptr<Node>>> expectedAst = {
        { make_shared<ProgramNode>("NestedIfWithElse") },
        { make_shared<BeginSectionNode>(), make_shared<IfStatementNode>(conditionOuter, thenBlockOuter, elseBlockOuter) }
    };

    EXPECT_TRUE(CompareAST(ast, expectedAst));
}

TEST(ParserTest, handles_syntax_error_missing_semicolon_between_statements) {
    EXPECT_THROW({
        Parser parser(tokenize(R"(program MissingSemicolon;
                                  begin
                                    a := 1
                                    b := 2;
                                  end.)"));
        parser.parse();
        }, runtime_error);
}
