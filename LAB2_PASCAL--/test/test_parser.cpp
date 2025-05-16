#include "pch.h"
#include "../Base/Node.h"
#include "../Base/Token.h"
#include "../Lexer/Lexer.h"
#include "../Parser/Parser.h"
#include <optional>
#include <variant>

bool CompareVariant(const variant<int, double, string>& v1, const variant<int, double, string>& v2) {
    if (v1.index() != v2.index()) return false;
    if (v1.index() == 0) return get<int>(v1) == get<int>(v2);
    if (v1.index() == 1) return get<double>(v1) == get<double>(v2);
    if (v1.index() == 2) return get<string>(v1) == get<string>(v2);
    return true;
}

bool CompareNodes(Node* n1, Node* n2) {
    if (n1 == nullptr && n2 == nullptr) return true;
    if (n1 == nullptr || n2 == nullptr) return false;

    if (n1->type != n2->type) return false;

    switch (n1->type) {
    case Node::NodeType::PROGRAM_STATEMENT:
        return static_cast<ProgramNode*>(n1)->programName == static_cast<ProgramNode*>(n2)->programName;
    case Node::NodeType::CONST_SECTION:
    case Node::NodeType::VAR_SECTION:
    case Node::NodeType::BEGIN_SECTION:
        return true;
    case Node::NodeType::CONST_DECLARATION: {
        ConstDeclarationNode* c1 = static_cast<ConstDeclarationNode*>(n1);
        ConstDeclarationNode* c2 = static_cast<ConstDeclarationNode*>(n2);
        return c1->identifier == c2->identifier && c1->type == c2->type && CompareVariant(c1->value, c2->value);
    }
    case Node::NodeType::VARIABLE_DECLARATION: {
        VariableDeclarationNode* v1 = static_cast<VariableDeclarationNode*>(n1);
        VariableDeclarationNode* v2 = static_cast<VariableDeclarationNode*>(n2);
        if (v1->identifierList == nullptr && v2->identifierList == nullptr) return true;
        if (v1->identifierList == nullptr || v2->identifierList == nullptr) return false;
        return CompareNodes(v1->identifierList, v2->identifierList) && v1->type == v2->type;
    }
    case Node::NodeType::IDENTIFIER_LIST: {
        IdentifierListNode* l1 = static_cast<IdentifierListNode*>(n1);
        IdentifierListNode* l2 = static_cast<IdentifierListNode*>(n2);
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
        AssignmentStatementNode* a1 = static_cast<AssignmentStatementNode*>(n1);
        AssignmentStatementNode* a2 = static_cast<AssignmentStatementNode*>(n2);
        if (a1->variableName != a2->variableName || a1->expression.size() != a2->expression.size()) return false;
        for (size_t i = 0; i < a1->expression.size(); ++i) {
            if (a1->expression[i].type != a2->expression[i].type || a1->expression[i].value != a2->expression[i].value) return false;
        }
        return true;
    }
    case Node::NodeType::WRITE_STATEMENT: {
        WriteStatementNode* w1 = static_cast<WriteStatementNode*>(n1);
        WriteStatementNode* w2 = static_cast<WriteStatementNode*>(n2);
        if (w1->expression.size() != w2->expression.size()) return false;
        for (size_t i = 0; i < w1->expression.size(); ++i) {
            if (w1->expression[i].type != w2->expression[i].type || w1->expression[i].value != w2->expression[i].value) return false;
        }
        return true;
    }
    case Node::NodeType::READ_STATEMENT: {
        ReadStatementNode* r1 = static_cast<ReadStatementNode*>(n1);
        ReadStatementNode* r2 = static_cast<ReadStatementNode*>(n2);
        if (r1->identifierList == nullptr && r2->identifierList == nullptr) return true;
        if (r1->identifierList == nullptr || r2->identifierList == nullptr) return false;
        return CompareNodes(r1->identifierList, r2->identifierList);
    }
    case Node::NodeType::IF_STATEMENT: {
        IfStatementNode* i1 = static_cast<IfStatementNode*>(n1);
        IfStatementNode* i2 = static_cast<IfStatementNode*>(n2);
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

bool CompareAST(const list<list<Node*>>& ast1, const list<list<Node*>>& ast2) {
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
    list<list<Node*>> ast = parser.parse();
    list<list<Node*>> expectedAst = {
        { new ProgramNode("Empty") },
        { new BeginSectionNode() }
    };
    EXPECT_TRUE(CompareAST(ast, expectedAst));
    for (const auto& block : ast) for (Node* node : block) delete node;
    for (const auto& block : expectedAst) for (Node* node : block) delete node;
}

TEST(ParserTest, can_parse_all_types_in_const_declaration) {
    Parser parser(tokenize(R"(program ConstDecl; 
                              const 
                                PI : double = 3.14; 
                                Count : integer = 100;
                                message : string = "Hello";
                              begin end.)"));
    list<list<Node*>> ast = parser.parse();
    list<list<Node*>> expectedAst = {
        { new ProgramNode("ConstDecl") },
        { new ConstSectionNode(), new ConstDeclarationNode("PI", "double", 3.14), new ConstDeclarationNode("Count", "integer", 100), 
            new ConstDeclarationNode("message", "string", "Hello")
        },
        { new BeginSectionNode() }
    };
    EXPECT_TRUE(CompareAST(ast, expectedAst));
    for (const auto& block : ast) for (Node* node : block) delete node;
    for (const auto& block : expectedAst) for (Node* node : block) delete node;
}

TEST(ParserTest, can_parse_all_types_in_var_section) {
    Parser parser(tokenize(R"(program VarSection; 
                              var 
                                x, y : double;
                                count : integer;
                                message1, message2 : string; 
                              begin end.)"));
    list<list<Node*>> ast = parser.parse();
    IdentifierListNode* idList1 = new IdentifierListNode({ "x", "y" });
    IdentifierListNode* idList2 = new IdentifierListNode({ "count" });
    IdentifierListNode* idList3 = new IdentifierListNode({ "message1", "message2"});
    list<list<Node*>> expectedAst = {
        { new ProgramNode("VarSection") },
        { new VarSectionNode(), new VariableDeclarationNode(idList1, "double"), new VariableDeclarationNode(idList2, "integer"),
            new VariableDeclarationNode(idList3, "string") },
        { new BeginSectionNode() }
    };
    EXPECT_TRUE(CompareAST(ast, expectedAst));
    for (const auto& block : ast) for (Node* node : block) delete node;
    for (const auto& block : expectedAst) for (Node* node : block) delete node;
    delete idList1;
    delete idList2;
}

TEST(ParserTest, can_parse_assignment_statement) {
    Parser parser(tokenize(R"(program AssignStmt; 
                              begin 
                                counter := 10; 
                              end.)"));
    list<list<Node*>> ast = parser.parse();
    vector<Token> expression = { { TokenType::INTEGER_LITERAL, "10", 1, 18 } };
    list<list<Node*>> expectedAst = {
        { new ProgramNode("AssignStmt") },
        { new BeginSectionNode(), new AssignmentStatementNode("counter", expression) }
    };
    EXPECT_TRUE(CompareAST(ast, expectedAst));
    for (const auto& block : ast) for (Node* node : block) delete node;
    for (const auto& block : expectedAst) for (Node* node : block) delete node;
}

TEST(ParserTest, can_parse_write_statement_single_literal) {
    Parser parser(tokenize(R"(program WriteLit; 
                             begin 
                                Write("Hello"); 
                             end.)"));
    list<list<Node*>> ast = parser.parse();
    vector<Token> expression = { { TokenType::STRING_LITERAL, "Hello", 1, 19 } };
    list<list<Node*>> expectedAst = {
        { new ProgramNode("WriteLit") },
        { new BeginSectionNode(), new WriteStatementNode(expression) }
    };
    EXPECT_TRUE(CompareAST(ast, expectedAst));
    for (const auto& block : ast) for (Node* node : block) delete node;
    for (const auto& block : expectedAst) for (Node* node : block) delete node;
}

TEST(ParserTest, can_parse_read_statement_single_identifier) {
    Parser parser(tokenize(R"(program ReadId; 
                              var 
                                input: string; 
                              begin 
                                Read(input); 
                              end.)"));
    list<list<Node*>> ast = parser.parse();
    IdentifierListNode* idList = new IdentifierListNode({ "input" });
    list<list<Node*>> expectedAst = {
        { new ProgramNode("ReadId") },
        { new VarSectionNode(), new VariableDeclarationNode(idList, "string") },
        { new BeginSectionNode(), new ReadStatementNode(idList) }
    };
    EXPECT_TRUE(CompareAST(ast, expectedAst));
    for (const auto& block : ast) for (Node* node : block) delete node;
    for (const auto& block : expectedAst) for (Node* node : block) delete node;
    delete idList;
}

TEST(ParserTest, can_parse_if_statement_without_else) {
    Parser parser(tokenize(R"(program IfNoElse; 
                              begin 
                                if (flag) then 
                                    result := 1; 
                              end.)"));
    list<list<Node*>> ast = parser.parse();
    vector<Token> condition = { { TokenType::IDENTIFIER, "flag", 1, 15 } };
    list<Node*> thenBlock = { new AssignmentStatementNode("result", { { TokenType::INTEGER_LITERAL, "1", 1, 28 } }) };
    list<list<Node*>> expectedAst = {
        { new ProgramNode("IfNoElse") },
        { new BeginSectionNode(), new IfStatementNode(condition, thenBlock, nullopt) }
    };
    EXPECT_TRUE(CompareAST(ast, expectedAst));
    for (const auto& block : ast) for (Node* node : block) delete node;
    for (const auto& block : expectedAst) for (Node* node : block) delete node;
    for (Node* node : thenBlock) delete node;
}

TEST(ParserTest, can_parse_if_statement_with_else) {
    Parser parser(tokenize(R"(program IfElse; 
                              begin 
                                if (count > 0) then 
                                    value := 1;
                                else
                                    value := -1; 
                             end.)"));
    list<list<Node*>> ast = parser.parse();
    vector<Token> condition = { { TokenType::IDENTIFIER, "count", 1, 15 }, { TokenType::GREATER, ">", 1, 21 }, { TokenType::INTEGER_LITERAL, "0", 1, 23 } };
    list<Node*> thenBlock = { new AssignmentStatementNode("value", { { TokenType::INTEGER_LITERAL, "1", 1, 33 } }) };
    list<Node*> elseBlock = { new AssignmentStatementNode("value", { { TokenType::MINUS, "-", 1, 45 }, { TokenType::INTEGER_LITERAL, "1", 1, 46 } }) };
    list<list<Node*>> expectedAst = {
        { new ProgramNode("IfElse") },
        { new BeginSectionNode(), new IfStatementNode(condition, thenBlock, elseBlock) }
    };
    EXPECT_TRUE(CompareAST(ast, expectedAst));
    for (const auto& block : ast) for (Node* node : block) delete node;
    for (const auto& block : expectedAst) for (Node* node : block) delete node;
    for (Node* node : thenBlock) delete node;
    for (Node* node : elseBlock) delete node;
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

TEST(ParserTest, handles_syntax_error_write_starts_with_comma) {
    EXPECT_THROW({
        Parser  parser(tokenize(R"(program ErrorWriteCommaStartError;
                                   begin
                                     Write(, "Error");
                                   end.)"));
        parser.parse();
        }, runtime_error);
}

TEST(ParserTest, handles_syntax_error_write_consecutive_commas) {
    EXPECT_THROW({
        Parser parser(tokenize(R"(program ErrorWriteConsecutiveCommasError;
                                  begin
                                    Write("A", , "B");
                                  end.)"));
        parser.parse();
        }, runtime_error);
}

TEST(ParserTest, handles_syntax_error_write_ends_with_comma) {
    EXPECT_THROW({
        Parser parser(tokenize(R"(program ErrorWriteCommaEndError;
                                  begin
                                    Write("End", );
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

TEST(ParserTest, handles_empty_write_statement) {
    Parser parser(tokenize(R"(program EmptyWrite; 
                              begin 
                                Write(); 
                              end.)"));
    list<list<Node*>> ast = parser.parse();
    list<list<Node*>> expectedAst = {
        { new ProgramNode("EmptyWrite") },
        { new BeginSectionNode(), new WriteStatementNode({}) }
    };
    EXPECT_TRUE(CompareAST(ast, expectedAst));
    for (const auto& block : ast) for (Node* node : block) delete node;
    for (const auto& block : expectedAst) for (Node* node : block) delete node;
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
    list<list<Node*>> ast = parser.parse();
    list<list<Node*>> expectedAst = {
        { new ProgramNode("AssignId") },
        { new BeginSectionNode(), new AssignmentStatementNode("a", { { TokenType::IDENTIFIER, "b", 3, 33 } }) }
    };
    EXPECT_TRUE(CompareAST(ast, expectedAst));
    for (const auto& block : ast) for (Node* node : block) delete node;
    for (const auto& block : expectedAst) for (Node* node : block) delete node;
}

TEST(ParserTest, can_parse_write_statement_multiple_args) {
    Parser parser(tokenize(R"(program WriteMulti;
                              begin
                                Write("Hello", 10, 10.10, var1, var2);
                              end.)"));
    list<list<Node*>> ast = parser.parse();
    vector<Token> expr = {
        { TokenType::STRING_LITERAL, "Hello", 3, 17 },
        { TokenType::INTEGER_LITERAL, "10", 3, 26 },
        { TokenType::DOUBLE_LITERAL, "10.10", 3, 31 },
        { TokenType::IDENTIFIER, "var1", 3, 39 },
        { TokenType::IDENTIFIER, "var2", 3, 45 }
    };
    list<list<Node*>> expectedAst = {
        { new ProgramNode("WriteMulti") },
        { new BeginSectionNode(), new WriteStatementNode(expr) }
    };
    EXPECT_TRUE(CompareAST(ast, expectedAst));
    for (const auto& block : ast) for (Node* node : block) delete node;
    for (const auto& block : expectedAst) for (Node* node : block) delete node;
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
    list<list<Node*>> ast = parser.parse();
    IdentifierListNode* idList = new IdentifierListNode({ "a", "b", "c", "d", "e" });
    list<list<Node*>> expectedAst = {
        { new ProgramNode("ReadMulti") },
        { new VarSectionNode(),
          new VariableDeclarationNode(new IdentifierListNode({ "a", "b" }), "integer"),
          new VariableDeclarationNode(new IdentifierListNode({ "c" }), "double"),
          new VariableDeclarationNode(new IdentifierListNode({ "d", "e" }), "string")
        },
        { new BeginSectionNode(), new ReadStatementNode(idList) }
    };
    EXPECT_TRUE(CompareAST(ast, expectedAst));
    for (const auto& block : ast) for (Node* node : block) delete node;
    for (const auto& block : expectedAst) for (Node* node : block) delete node;
    delete idList;
}


TEST(ParserTest, can_parse_nested_if_without_else) {
    Parser parser(tokenize(R"(program NestedIfNoElse;
                              begin
                                if (a > 0) then
                                    if (b < 10) then
                                        result := 5;
                              end.)"));
    list<list<Node*>> ast = parser.parse();

    vector<Token> condition1 = { { TokenType::IDENTIFIER, "a", 1, 8 }, { TokenType::GREATER, ">", 1, 10 }, { TokenType::INTEGER_LITERAL, "0", 1, 12 } };
    vector<Token> condition2 = { { TokenType::IDENTIFIER, "b", 2, 12 }, { TokenType::LESS, "<", 2, 14 }, { TokenType::INTEGER_LITERAL, "10", 2, 16 } };
    list<Node*> thenBlockInner = { new AssignmentStatementNode("result", { { TokenType::INTEGER_LITERAL, "5", 3, 17 } }) };
    list<Node*> thenBlockOuter = { new IfStatementNode(condition2, thenBlockInner, nullopt) };

    list<list<Node*>> expectedAst = {
        { new ProgramNode("NestedIfNoElse") },
        { new BeginSectionNode(), new IfStatementNode(condition1, thenBlockOuter, nullopt) }
    };

    EXPECT_TRUE(CompareAST(ast, expectedAst));

    for (const auto& block : ast) for (Node* node : block) delete node;
    for (const auto& block : expectedAst) for (Node* node : block) delete node;
    for (Node* node : thenBlockInner) delete node;
    for (Node* node : thenBlockOuter) delete node;
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
    list<list<Node*>> ast = parser.parse();

    vector<Token> conditionOuter = { { TokenType::IDENTIFIER, "x", 1, 8 }, { TokenType::EQUAL, "=", 1, 10 }, { TokenType::INTEGER_LITERAL, "1", 1, 12 } };
    vector<Token> conditionInner = { { TokenType::IDENTIFIER, "y", 2, 12 }, { TokenType::NON_EQUAL, "<>", 2, 14 }, { TokenType::INTEGER_LITERAL, "2", 2, 17 } };
    list<Node*> thenBlockInner = { new AssignmentStatementNode("value", { { TokenType::IDENTIFIER, "true", 3, 17 } }) };
    list<Node*> elseBlockInner = { new AssignmentStatementNode("value", { { TokenType::IDENTIFIER, "false", 5, 17 } }) };
    list<Node*> thenBlockOuter = { new IfStatementNode(conditionInner, thenBlockInner, elseBlockInner) };
    list<Node*> elseBlockOuter = { new AssignmentStatementNode("error", { { TokenType::MINUS, "-", 7, 17 }, { TokenType::INTEGER_LITERAL, "1", 7, 18 } }) };

    list<list<Node*>> expectedAst = {
        { new ProgramNode("NestedIfWithElse") },
        { new BeginSectionNode(), new IfStatementNode(conditionOuter, thenBlockOuter, elseBlockOuter) }
    };

    EXPECT_TRUE(CompareAST(ast, expectedAst));

    for (const auto& block : ast) for (Node* node : block) delete node;
    for (const auto& block : expectedAst) for (Node* node : block) delete node;
    for (Node* node : thenBlockInner) delete node;
    for (Node* node : elseBlockInner) delete node;
    for (Node* node : thenBlockOuter) delete node;
    for (Node* node : elseBlockOuter) delete node;
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

