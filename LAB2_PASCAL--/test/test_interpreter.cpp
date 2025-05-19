#include "pch.h"
#include "../Interpreter/Interpreter.h"
#include "../Lexer/Lexer.h"
#include "../Parser/Parser.h"
#include <sstream>
#include <functional>

list<list<Node*>> parseCode(const string& code) {
    Lexer lexer(code);
    Parser parser(lexer.tokenize());
    return parser.parse();
}

stringstream captureCout(function<void()> func) {
    stringstream ss;
    streambuf* oldCout = cout.rdbuf();
    cout.rdbuf(ss.rdbuf());
    func();
    cout.rdbuf(oldCout);
    return ss;
}

void simulateCin(const string& input, function<void()> func) {
    stringstream ss(input);
    streambuf* oldCin = cin.rdbuf();
    cin.rdbuf(ss.rdbuf());
    func();
    cin.rdbuf(oldCin);
}

TEST(InterpreterTest, declare_and_use_constants) {
    list<list<Node*>> ast = parseCode(R"(program TestConst; 
                                        const 
                                            PI : double = 3.14; 
                                            COUNT : integer = 10; 
                                            MSG : string = "Hello"; 
                                        begin 
                                        end.)");
    Interpreter interpreter(ast);
    EXPECT_NO_THROW(interpreter.run());
}

TEST(InterpreterTest, redeclare_constant_throws_error) {
    list<list<Node*>> ast = parseCode(R"(program TestConstRedecl; 
                                        const 
                                            PI : double = 3.14; 
                                            PI : integer = 10; 
                                        begin end.)");
    Interpreter interpreter(ast);
    EXPECT_THROW(interpreter.run(), runtime_error);
}

TEST(InterpreterTest, declare_variables) {
    list<list<Node*>> ast = parseCode(R"(program TestVar; 
                                        var 
                                            x, y : integer; 
                                            z : double; 
                                            s1, s2 : string; 
                                        begin 
                                        end.)");
    Interpreter interpreter(ast);
    EXPECT_NO_THROW(interpreter.run());
}

TEST(InterpreterTest, redeclare_variable_throws_error) {
    list<list<Node*>> ast = parseCode(R"(program TestVarRedecl; 
                                        var 
                                            x : integer;    
                                            x : double; 
                                        begin 
                                        end.)");
    Interpreter interpreter(ast);
    EXPECT_THROW(interpreter.run(), runtime_error);
}

TEST(InterpreterTest, declare_variable_same_name_as_constant_throws_error) {
    list<list<Node*>> ast = parseCode(R"(program TestVarConstConflict; 
                                        const 
                                            VALUE : integer = 5; 
                                        var  
                                            VALUE : double; 
                                        begin 
                                        end.)");
    Interpreter interpreter(ast);
    EXPECT_THROW(interpreter.run(), runtime_error);
}

TEST(InterpreterTest, assign_literal_to_variable) {
    list<list<Node*>> ast = parseCode(R"(program TestAssignLit; 
                                        var 
                                            count : integer; 
                                            pi : double; 
                                            message : string; 
                                        begin 
                                            count := 10; 
                                            pi := 3.14; 
                                             
                                        end.)");
    Interpreter interpreter(ast);
    EXPECT_NO_THROW(interpreter.run());
}

TEST(InterpreterTest, assign_variable_to_variable) {
    list<list<Node*>> ast = parseCode(R"(program TestAssignVar; 
                                        var 
                                            a : integer; 
                                            b : integer; 
                                        begin 
                                            a := 5;
                                            b := a; 
                                        end.)");
    Interpreter interpreter(ast);
    EXPECT_NO_THROW(interpreter.run());
}

TEST(InterpreterTest, assign_to_constant_throws_error) {
    list<list<Node*>> ast = parseCode(R"(program TestAssignConstError;
                                        const
                                            VALUE : integer = 5;
                                        begin
                                            VALUE := 10;
                                        end.)");
    Interpreter interpreter(ast);
    EXPECT_THROW(interpreter.run(), runtime_error);
}

TEST(InterpreterTest, assign_to_undeclared_variable_throws_error) {
    list<list<Node*>> ast = parseCode(R"(program TestAssignUndeclaredError;
                                        begin
                                            x := 5;
                                        end.)");
    Interpreter interpreter(ast);
    EXPECT_THROW(interpreter.run(), runtime_error);
}

TEST(InterpreterTest, write_string_literal) {
    list<list<Node*>> ast = parseCode(R"(program TestWriteStr;
                                        begin
                                            Write("Hello World");
                                        end.)");
    Interpreter interpreter(ast);
    stringstream ss = captureCout([&]() { interpreter.run(); });
    EXPECT_EQ(ss.str(), "Hello World\n");
}

TEST(InterpreterTest, write_integer_literal) {
    list<list<Node*>> ast = parseCode(R"(program TestWriteInt;
                                        begin
                                            Write(123);
                                        end.)");
    Interpreter interpreter(ast);
    stringstream ss = captureCout([&]() { interpreter.run(); });
    EXPECT_EQ(ss.str(), "123\n");
}

TEST(InterpreterTest, write_double_literal) {
    list<list<Node*>> ast = parseCode(R"(program TestWriteDouble;
                                        begin
                                            Write(3.14159);
                                        end.)");
    Interpreter interpreter(ast);
    stringstream ss = captureCout([&]() { interpreter.run(); });
    EXPECT_EQ(ss.str(), "3.14159\n");
}

TEST(InterpreterTest, write_variable) {
    list<list<Node*>> ast = parseCode(R"(program TestWriteVar;
                                        var
                                            msg : string;
                                        begin
                                            msg := "Goodbye";
                                            Write(msg);
                                        end.)");
    Interpreter interpreter(ast);
    stringstream ss = captureCout([&]() { interpreter.run(); });
    EXPECT_EQ(ss.str(), "Goodbye\n");
}

TEST(InterpreterTest, write_multiple_arguments) {
    list<list<Node*>> ast = parseCode(R"(program TestWriteMulti;
                                        var
                                            num : integer;
                                            pi : double;
                                        begin
                                            num := 42;
                                            pi := 3.14;
                                            Write("Answer: ", num, " PI: ", pi);
                                        end.)");
    Interpreter interpreter(ast);
    stringstream ss = captureCout([&]() { interpreter.run(); });
    EXPECT_EQ(ss.str(), "Answer: 42 PI: 3.14\n");
}

TEST(InterpreterTest, write_undeclared_variable_in_expression_throws_error) {
    list<list<Node*>> ast = parseCode(R"(program TestWriteUndeclaredVarError;
                                        begin
                                            Write(unknown);
                                        end.)");
    Interpreter interpreter(ast);
    EXPECT_THROW(interpreter.run(), runtime_error);
}

TEST(InterpreterTest, read_integer_variable) {
    list<list<Node*>> ast = parseCode(R"(program TestReadInt;
                                        var
                                            num : integer;
                                        begin
                                            Read(num);
                                            Write("You entered: ", num);
                                        end.)");
    Interpreter interpreter(ast);
    simulateCin("123\n", [&]() {
        stringstream ss = captureCout([&]() { interpreter.run(); });
        EXPECT_EQ(ss.str(), "You entered: 123\n");
        });
}

TEST(InterpreterTest, read_double_variable) {
    list<list<Node*>> ast = parseCode(R"(program TestReadDouble;
                                        var
                                            pi : double;
                                        begin
                                            Read(pi);
                                            Write("You entered: ", pi);
                                        end.)");
    Interpreter interpreter(ast);
    simulateCin("3.14\n", [&]() {
        stringstream ss = captureCout([&]() { interpreter.run(); });
        EXPECT_EQ(ss.str(), "You entered: 3.14\n");
        });
}

TEST(InterpreterTest, read_string_variable) {
    list<list<Node*>> ast = parseCode(R"(program TestReadString;
                                        var
                                            msg : string;
                                        begin
                                            Read(msg);
                                            Write("You entered: ", msg);
                                        end.)");
    Interpreter interpreter(ast);
    simulateCin("Hello World\n", [&]() {
        stringstream ss = captureCout([&]() { interpreter.run(); });
        EXPECT_EQ(ss.str(), "You entered: Hello World\n");
        });
}

TEST(InterpreterTest, read_multiple_variables) {
    list<list<Node*>> ast = parseCode(R"(program TestReadMulti;
                                        var
                                            a : integer;
                                            b : double;
                                            c : string;
                                        begin
                                            Read(a, b, c);
                                            Write("Entered: ", a, " ", b, " ", c);
                                        end.)");
    Interpreter interpreter(ast);
    simulateCin("10\n2.71\nTest\n", [&]() {
        stringstream ss = captureCout([&]() { interpreter.run(); });
        EXPECT_EQ(ss.str(), "Entered: 10 2.71 Test\n");
        });
}

TEST(InterpreterTest, read_to_constant_throws_error) {
    list<list<Node*>> ast = parseCode(R"(program TestReadConstError;
                                        const
                                            VALUE : integer = 5;
                                        begin
                                            Read(VALUE);
                                        end.)");
    Interpreter interpreter(ast);
    simulateCin("10\n", [&]() {
        EXPECT_THROW(interpreter.run(), runtime_error);
        });
}

TEST(InterpreterTest, read_undeclared_variable_throws_error) {
    list<list<Node*>> ast = parseCode(R"(program TestReadUndeclaredError;
                                        begin
                                            Read(unknown);
                                        end.)");
    Interpreter interpreter(ast);
    simulateCin("test\n", [&]() {
        EXPECT_THROW(interpreter.run(), runtime_error);
        });
}

TEST(InterpreterTest, read_invalid_integer_input_throws_error) {
    list<list<Node*>> ast = parseCode(R"(program TestReadInvalidIntError;
                                        var
                                            num : integer;
                                        begin
                                            Read(num);
                                        end.)");
    Interpreter interpreter(ast);
    simulateCin("abc\n", [&]() {
        EXPECT_THROW(interpreter.run(), runtime_error);
        });
}

TEST(InterpreterTest, if_statement_numeric_true) {
    list<list<Node*>> ast = parseCode(R"(program TestIfTrue;
                                        var
                                            x : integer;
                                        begin
                                            x := 5;
                                            if (x > 0) then
                                                Write("Positive");
                                        end.)");
    Interpreter interpreter(ast);
    stringstream ss = captureCout([&]() { interpreter.run(); });
    EXPECT_EQ(ss.str(), "Positive\n");
}

TEST(InterpreterTest, if_statement_numeric_false_no_else) {
    list<list<Node*>> ast = parseCode(R"(program TestIfFalseNoElse;
                                        var
                                            x : integer;
                                        begin
                                            x := -1;
                                            if (x > 0) then
                                                Write("Positive");
                                        end.)");
    Interpreter interpreter(ast);
    stringstream ss = captureCout([&]() { interpreter.run(); });
    EXPECT_EQ(ss.str().find("Positive"), string::npos);
}

TEST(InterpreterTest, if_statement_numeric_false_with_else) {
    list<list<Node*>> ast = parseCode(R"(program TestIfFalseElse;
                                        var
                                            x : integer;
                                        begin
                                            x := -1;
                                            if (x > 0) then
                                                Write("Positive");
                                            else
                                                Write("Not positive");
                                        end.)");
    Interpreter interpreter(ast);
    stringstream ss = captureCout([&]() { interpreter.run(); });
    EXPECT_EQ(ss.str(), "Not positive\n");
}

TEST(InterpreterTest, if_statement_string_equal_true) {
    list<list<Node*>> ast = parseCode(R"(program TestIfStrTrue;
                                        var
                                            msg : string;
                                        begin
                                            msg := "yes";
                                            if (msg = "yes") then
                                                Write("Affirmative");
                                        end.)");
    Interpreter interpreter(ast);
    stringstream ss = captureCout([&]() { interpreter.run(); });
    EXPECT_EQ(ss.str(), "Affirmative\n");
}

TEST(InterpreterTest, if_statement_string_equal_false_with_else) {
    list<list<Node*>> ast = parseCode(R"(program TestIfStrFalseElse;
                                        var
                                            msg : string;
                                        begin
                                            msg := "no";
                                            if (msg = "yes") then
                                                Write("Affirmative");
                                            else
                                                Write("Negative");
                                        end.)");
    Interpreter interpreter(ast);
    stringstream ss = captureCout([&]() { interpreter.run(); });
    EXPECT_EQ(ss.str(), "Negative\n");
}

TEST(InterpreterTest, if_statement_missing_comparison_operator_throws_error) {
    list<list<Node*>> ast = parseCode(R"(program TestIfNoOpError;
                                        var
                                            x : integer;
                                        begin
                                            x := 5;
                                            if (x 0) then
                                                Write("Error");
                                        end.)");
    Interpreter interpreter(ast);
    EXPECT_THROW(interpreter.run(), runtime_error);
}

TEST(InterpreterTest, if_statement_type_mismatch_throws_error) {
    list<list<Node*>> ast = parseCode(R"(program TestIfTypeError; 
                                        var 
                                            num : integer; 
                                            msg : string; 
                                        begin 
                                            num := 5; 
                                            if (num = msg) then 
                                                Write("Error"); 
                                        end.)");
    Interpreter interpreter(ast);
    EXPECT_THROW(interpreter.run(), runtime_error);
}
