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
    Parser parser(tokenize("program Empty; begin end."));
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
    ASSERT_TRUE(CompareAST(ast, expectedAst));
    for (const auto& block : ast) for (Node* node : block) delete node;
    for (const auto& block : expectedAst) for (Node* node : block) delete node;
    delete idList1;
    delete idList2;
}