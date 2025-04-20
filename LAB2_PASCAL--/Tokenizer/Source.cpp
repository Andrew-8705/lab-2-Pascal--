#include "Tokenizer.h"
#include <iostream>
#include <string>
#include <regex>
#include <vector>
#include <variant>

using namespace std;


int main() {
#include <string>

    string code = R"(program Example;
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
                        if (b mod 2 = 0) then
                        begin
                            Res := (num1 - num2 * 5 div 2) / (d * 2);
                            Write("Result = ", Res);
                        end
                        else
                            Write("Incorrect input");
                    end.)";
    Tokenizer tok(code);
    std::vector<Token> tokens = tok.tokenize();
    return 0;
}