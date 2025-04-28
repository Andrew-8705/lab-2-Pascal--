#pragma once

#include "../Base/Token.h"
#include <stack>
#include <vector>
#include <map>

using namespace std;

class PostfixConverter
{
public:
	static vector<Token> Convert(const vector<Token>& expr);
	static map<string, int>	op_leftassociative;
	static map<string, int> op_rightassociative;
};