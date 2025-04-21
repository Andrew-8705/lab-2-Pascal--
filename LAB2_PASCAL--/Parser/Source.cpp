#include <iostream>
#include "Parser.h"
#include "../Tokenizer/Tokenizer.h"
#include "../Base/Node.h"
#include <vector>

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
		const std::string code1 = "if counter = 10.5; while (i < 5) { result = result + i; } else output = \"hello\";";
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
						end.)";
		// Message: string = 'Hello, World!'
		Tokenizer tok(code);
		std::vector<Token> tokens = tok.tokenize();
		Parser parser(tokens);
		Node* root = parser.parse();
		//Print(root);
	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
	}
}
