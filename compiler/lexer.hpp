#pragma once

#include "token.hpp"
#include <vector>
#include <string>
#include <cctype>
#include <algorithm>
#include <stdexcept>

class lexer {
private:
    std::string code;
    std::vector<token> tokens;
    size_t position = 0;

    const std::string OPERATOR_CHARS = "+-*/()=";
    const std::vector<tokenType> OPERATOR_TOKENS = {
        tokenType::PLUS, tokenType::MINUS, tokenType::STAR, tokenType::SLASH, tokenType::LPAREN, tokenType::RPAREN, tokenType::EQ
    };

    char next() {
        return (position < code.size()) ? code[position++] : '\0';
    }

    char peek(int offset = 0) const {
        size_t pos = position + offset;
        return (pos < code.size()) ? code[pos] : '\0';
    }

    void addToken(tokenType type, const std::string& lexeme = "") {
        tokens.emplace_back(type, lexeme);
    }

    void tokenizeNumber() {
        std::string buffer;
        while (true) {
            //buffer += next();
            if (peek() == '.') {
                if (buffer.find('.') != -1) {
                    throw std::runtime_error("Invalid float number");
                }
            } else if (!std::isdigit(peek())) {
                break;
            }
            buffer += next();
        }
        addToken(tokenType::NUMBER, buffer);
    }

    void tokenizeHexNumber() {
        std::string buffer;
        while (isxdigit(peek())) {
            buffer += next();
        }
        addToken(tokenType::HEX_NUMBER, buffer);
    }

    bool isPrefixHexNumber(size_t position) {
        if (position < code.size() && code[position] == 'x') {
            next();
            return true;
        }
        return false;
    }

    void tokenizeWord() {
        std::string buffer;
        while (true) {
            if (!std::isalnum(peek()) && (peek() != '_')) {
                break;
            }
            buffer += next();
        }
        if (buffer == "print") {
            addToken(tokenType::PRINT);
        } else {
            addToken(tokenType::WORD, buffer);
        }
        
    }

    void tokenizeOperator() {
        size_t pos = OPERATOR_CHARS.find(peek());
        if (pos != std::string::npos) {
            addToken(OPERATOR_TOKENS[pos]);
            next();
        }
    }

public:
    explicit lexer(const std::string& code) : code(code) {}

    std::vector<token> tokenize() {
        while (position < code.size()) {
            char currentSymbol = peek();
            if (currentSymbol == '0' && isPrefixHexNumber(position + 1)) {
                next();
                tokenizeHexNumber();
            } else if (std::isdigit(currentSymbol)) {
                tokenizeNumber();
            } else if (std::isalpha(currentSymbol)) {
                tokenizeWord();
            }
            else if (OPERATOR_CHARS.find(currentSymbol) != std::string::npos) {
                tokenizeOperator();
            } else {
                next(); // Пропускаем неизвестные символы
            }
        }
        return tokens;
    }
};
