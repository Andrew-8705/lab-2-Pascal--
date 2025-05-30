#include <iostream>
#include "../Base/Node.h"
#include "Parser.h"
#include <chrono>
#include <vector>
#include "../Base/Token.h"
#include "../Lexer/Lexer.h"

int main()
{
	try {
		string code = R"(program Example; 
						const 
							Pi: double = 3.1415926;
							a: integer = 1; 
							MaxCount: integer = 100; 
						var 
							num1, num2, num3: integer;
							num1: integer;
						begin
							num1 := a + b - c;
							num2 := 2;
							num3 := num1;
							Write("������� ������ ����� �����: ", num1, num2, REs);
							RESULT := Pi;
							Read(num3, num1);
							Write(RESULT);
							if (b mod 2) then
							begin
								Write("��������� = ", Res);
								if (a / b) then 
								begin
									a := b;
								end
								else 
								begin 
									b := a;
								end
							end
							else
							begin
								d := y;
								Read(Pi);
							end
						end.)";
		string code1 = R"(program Example;
                    const
                        Pi: double = 3.1415926;
                    var
                        num1, num2: integer;
                        Res, d: double;
                    begin
                        num1 := 5;
                        Write("Enter an even integer: ");
                        Read(num2);
                        Write("Enter a real number: ");
	                    Read(d);
                        if (b) then
                        begin
                            Res := num1;
                            Write("Result = ", Res);
                        end
                        else
                            Write("Incorrect input");
                    end.)";
		string code2 = R"(program Example;
                    const
                        Pi: double = 3.1415926;
						a: integer = 1; 
						MaxCount: integer = 100; 
						b: double = 3.1;
					var
                        num1, num2: integer;
                        Res: double;
					begin
						a := 5;
						b := 5 * a + c + b;
						c := a;
						Write("������� ������ ����� �����: ");
						Write("��������� = ", Res);
						Read(num2);
						Read(num3, num1);
						if (b + a) then
                        begin
                            Res := num1;
                            Write("Result = ", Res);
                        end
                        else
						begin
                            Write("Incorrect input");
							Read(num2);
						end
						a := res;
						Read(num2);
					end.)";
		string problem = R"(program TestWriteCommaEndError;
                                        begin
                                            Write("End", str + str, Lol);
                                        end.)";
		auto start_tokenize = std::chrono::high_resolution_clock::now();
		Lexer lexer(problem);
		std::vector<Token> tokens = lexer.tokenize();
		auto end_tokenize = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> tokenize_time = end_tokenize - start_tokenize;
		cout << "Time of tokenization: " << tokenize_time.count() << '\n';

		auto start_parse = std::chrono::high_resolution_clock::now();
		Parser parser(tokens);
		parser.parse();
		auto end_parse = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> parse_time = end_parse - start_parse;
		cout << "Time of parsing: " << parse_time.count() << '\n';
	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
	}
}
