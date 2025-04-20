#pragma once

#include <vector>
#include <optional>
#include <stdexcept>
#include <exception>
#include "../Base/Token.h"

using namespace std;

class Parser {
private:
    vector<Token> tokens;
    int curTokenPos;

    optional<Token> match(vector<TokenType> tts) {
        if (curTokenPos < tokens.size()) {
            Token curToken = tokens[curTokenPos];
            for (TokenType tt : tts) {
                if (curToken.type == tt)
                    curTokenPos++;
                    return curToken;
            }
        }
        return nullopt;
    }

    Token require(vector<TokenType> tts, const string& errorMessage) {
        optional<Token> tk = match(tts);
        if (tk.has_value())
            return tk.value();
        throw std::runtime_error("Expected: " + errorMessage);
    }

    void parseProgram() {
        require({ TokenType::KEYWORD_PROGRAM }, "'program'");
        // ...
    }
public:
    Parser(const vector<Token>& tkns) : tokens(tkns) {};

    void parse() {
        parseProgram();
    }

};
