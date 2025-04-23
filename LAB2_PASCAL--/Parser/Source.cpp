#include <iostream>
#include "Parser.h"
#include "../Tokenizer/Tokenizer.h"
#include "../Base/Node.h"
#include <vector>
#include <chrono>

using namespace std;

void Print(Node* node) {
	cout << node->name << ": ";
	for (Node* c : node->children) {
		cout << c->name << ' ';
	}
	cout << '\n';
	for (Node* c : node->children) {
		Print(c);
	}
}
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
						begin
							num1 := 1;
							num2 := 2;
							num3 := num1;
							Write("¬ведите чЄтное целое число: ", num1, num2, REs);
							RESULT := Pi;
							Read(num3, num1);
							Write(RESULT);
							if (b) then
							begin
								Write("–езультат = ", Res);
								if (a) then 
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
		// Message: string = 'Hello, World!'
		auto start_tokenize = std::chrono::high_resolution_clock::now();
		Tokenizer tok(code1);
		std::vector<Token> tokens = tok.tokenize();
		auto end_tokenize = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> tokenize_time = end_tokenize - start_tokenize;
		cout << "Time of tokenization: " <<  tokenize_time.count() << '\n';

		auto start_parse = std::chrono::high_resolution_clock::now();
		Parser parser(tokens);
		Node* root = parser.parse();
		auto end_parse = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> parse_time = end_parse - start_parse;
		cout << "Time of parsing: " << parse_time.count() << '\n';
		//Print(root);
	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
	}
}
