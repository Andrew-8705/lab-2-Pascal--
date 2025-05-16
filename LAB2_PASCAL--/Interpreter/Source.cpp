#include "../Interpreter/Interpreter.h"
#include "../Lexer/Lexer.h"
#include "../Parser/Parser.h"
#include <iostream>
#include <string>
#include <vector>
#include <variant>


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
							Write("¬ведите чЄтное целое число: ", num1, num2, REs);
							RESULT := Pi;
							Read(num3, num1);
							Write(RESULT);
							if (b mod 2) then
							begin
								Write("–езультат = ", Res);
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
						num3: integer;
                        Res: double;
					begin
						num1 := 5.0;
						num2 := 5 * a + b;
						num1 := a;
						Write("Enter an even integer: ");
						Write("Result = ", Res);
						Read(num2);
						Read(num3, num1);
						if (b + a = 0) then
                        begin
                            Res := num1;
                            Write("Result = ", Res);
                        end
                        else
						begin
                            Write("Incorrect input");
							Read(num2);
						end
						Res := a;
						Read(num2);
					end.)";

		string sample = R"(program Example;
						const
							Pi: double = 3.1415926;
						var
							num1, num2: integer;
							Res, d: double;
						begin
							num1 := 5;
							Write("Enter an", " even integer: ", num1);
							Read(num2);
							Write("Enter a real number: ");
							Read(d);
							if (num2 mod 2 = 0) then
							begin
								Res := (num1 - num2 * 5 div 2) / (d * 2);
								Write("Result = ", Res);
							end
							else
							begin
								Write("Incorrect input");
							end
						end.)";
		string sample2 = R"(program Example;
						var
							str: string;
						begin
							str := "borow";
							Write("Here is: ", str);
						end.)";
		string sample3 = R"(program Example;
						var
							num1: integer;
							num2: integer;
						begin
							num1 := 5;
							num2 := 10;
							if (num1 <= num2) then
							begin
								Write("then");
							end
							else
							begin
								Write("else");
							end
						end.)";
		string sample4 = R"( program Example;
						begin
							if (1 <> 2) then
								if (2 <= 3) then 
									if (5 > 4) then
										if (10 = 10) then
											if (15 >= 14) then
												if (16 >= 16) then
													Write("YES");
						end.
						)";
		string problem = R"(program TestReadMulti;
                                        var
                                            a : integer;
                                            b : double;
                                            c : string;
                                        begin
                                            Read(a, b, c);
                                            Write("Entered: ", a, b, c);
                                        end.)";
		Lexer lexer(problem);
		const vector<Token> tokens = lexer.tokenize();
		for (auto& x : tokens) cout << x.value << '\n';
		Parser parser(tokens);
		list<list<Node*>>& ast = parser.parse();
		Interpreter inter(ast);
		inter.run();
	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
	}
}