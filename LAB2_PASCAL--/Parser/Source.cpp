#include <iostream>
#include "Parser.h"
#include "../Tokenizer/Tokenizer.h"
#include <vector>

using namespace std;

int main()
{
	const std::string code = "if counter = 10.5; while (i < 5) { result = result + i; } else output = \"hello\";";
	Tokenizer tok(code);
	std::vector<Token> tokens = tok.tokenize();
	Parser parser(tokens);
}
