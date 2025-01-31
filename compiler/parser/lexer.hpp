#pragma once

#include "/home/alexs/reverse/compiler/parser/token.hpp"
#include <vector>
#include <string>
#include <cctype>

class lexer {
private:
    std::string code;
    std::vector<token> tokens;
    size_t position = 0;

    static const std::unordered_map<std::string, tokenType> OPERATORS;
    static const std::string OPERATOR_CHARS;

    char next();
    char peek(int offset = 0) const;
    void addToken(tokenType type, const std::string& lexeme = "");
    
    void tokenizeNumber();
    void tokenizeHexNumber();
    bool isPrefixHexNumber(size_t position);
    void tokenizeWord();
    void tokenizeText();
    void tokenizeOperator();
    void tokenizeComment();
    void tokenizeMultilineComment();

public:
    explicit lexer(const std::string& code);
    std::vector<token> tokenize();
};
