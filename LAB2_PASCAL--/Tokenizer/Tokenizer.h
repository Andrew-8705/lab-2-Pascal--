#pragma once

#include <vector>
#include "../Base/Token.h"
#include <regex>
#include <optional>
#include <string>
#include <cctype>
#include <variant>
#include <iostream>

using namespace std;

class Tokenizer {
private:
    string SourceCode;
    int curPos = 0;
    int curLine = 1;
    int curColumn = 0;

    vector<pair<regex, TokenType>> tokenPatterns = {
        {regex(R"([a-zA-Z_][a-zA-Z0-9_]*)"), TokenType::IDENTIFIER},
        {regex(R"([0-9]+\.[0-9]*)"), TokenType::DOUBLE_LITERAL},
        {regex(R"([0-9]+)"), TokenType::INTEGER_LITERAL},
        {regex(R"(\+)"), TokenType::PLUS},
        {regex(R"(\-)"), TokenType::MINUS},
        {regex(R"(\*)"), TokenType::MULTIPLY},
        {regex(R"(/)"), TokenType::DIVIDE},
        {regex(R"(:=)"), TokenType::ASSIGN},
        {regex(R"(;)"), TokenType::SEMICOLON},
        {regex(R"(:)"), TokenType::COLON},
        {regex(R"(,)"), TokenType::COMMA},
        {regex(R"(\()"), TokenType::LEFT_PAREN},
        {regex(R"(\))"), TokenType::RIGHT_PAREN},
        {regex(R"(program\b)"), TokenType::KEYWORD_PROGRAM},
        {regex(R"(const\b)"), TokenType::KEYWORD_CONST},
        {regex(R"(var\b)"), TokenType::KEYWORD_VAR},
        {regex(R"(begin\b)"), TokenType::KEYWORD_BEGIN},
        {regex(R"(end\b)"), TokenType::KEYWORD_END},
        {regex(R"(if\b)"), TokenType::KEYWORD_IF},
        {regex(R"(then\b)"), TokenType::KEYWORD_THEN},
        {regex(R"(else\b)"), TokenType::KEYWORD_ELSE},
        {regex(R"(mod\b)"), TokenType::KEYWORD_MOD},
        {regex(R"(div\b)"), TokenType::KEYWORD_DIV},
        {regex(R"(Write\b)"), TokenType::KEYWORD_WRITE},
        {regex(R"(Read\b)"), TokenType::KEYWORD_READ},
        {regex(R"(\"[^"]*\")"), TokenType::STRING_LITERAL }
    };

    pair<optional<Token>, int> match() {
        if (curPos >= SourceCode.length())
            return { nullopt, 0 };

        if (isspace(SourceCode[curPos])) {
            int skipped = 0;
            while (curPos + skipped < SourceCode.length() && isspace(SourceCode[curPos + skipped])) {
                if (SourceCode[curPos + skipped] == '\n') {
                    curLine++;
                    curColumn = 0;
                }
                else {
                    curColumn++;
                }
                skipped++;
            }
            return { nullopt, skipped };
        }

        int startColumn = curColumn;
        string remaining = SourceCode.substr(curPos);

        for (const auto& pattern : tokenPatterns) {
            smatch match;
            if (regex_search(remaining, match, pattern.first)) {
                if (match.position() == 0) {
                    string matchedText = match.str();
                    TokenType type = pattern.second;
                    variant<string, double, int> value;
                    cout << matchedText << '\n';
                    return { Token(), matchedText.length() };
                }
            }
        }
        return { nullopt, 1 };
    }
public:
    Tokenizer(const string& source) : SourceCode(source) {}

    vector<Token> tokenize() {
        
        vector<Token> tokens;

        while (curPos < SourceCode.length()) {
            pair<optional<Token>, int> tk = match();
            if (tk.first.has_value()) {
                tokens.push_back(tk.first.value());
            }
            curPos += tk.second;
        }
        
        return tokens;
    }
};

