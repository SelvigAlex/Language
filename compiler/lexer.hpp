// #include "token.hpp"
// #include <vector>
// #include <string>
// #include <cctype>
// #include <algorithm>
// #include <stdexcept>
// #include <unordered_map>
// #include <unordered_set>
// #include <sstream>

// class lexer {
// private:
//     std::string code;
//     std::vector<token> tokens;
//     size_t position = 0;

//     const std::unordered_map<std::string, tokenType> OPERATORS = {
//             {"+", tokenType::PLUS},
//             {"-", tokenType::MINUS},
//             {"*", tokenType::STAR},
//             {"/", tokenType::SLASH},
//             {"(", tokenType::LPAREN},
//             {")", tokenType::RPAREN},
//             {"=", tokenType::EQ},
//             {"<", tokenType::LT},
//             {">", tokenType::GT},
            
//             {"!", tokenType::EXCL},
//             {"&", tokenType::AMP},
//             {"|", tokenType::BAR},
            
//             {"==", tokenType::EQEQ},
//             {"!=", tokenType::EXCLEQ},
//             {"<=", tokenType::LTEQ},
//             {">=", tokenType::GTEQ},
            
//             {"&&", tokenType::AMPAMP},
//             {"||", tokenType::BARBAR}
//     };

//     const std::unordered_set<char> OPERATOR_CHARS = {'+', '-', '*', '/', '(', ')', '=', '<', '>', '!', '&', '|'};
//     const std::unordered_set<char> WHITESPACE = {' ', '\t', '\n', '\r'};

//     char next() {
//         return (position < code.size()) ? code[position++] : '\0';
//     }

//     char peek(int offset = 0) const {
//         size_t pos = position + offset;
//         return (pos < code.size()) ? code[pos] : '\0';
//     }

//     void addToken(tokenType type, const std::string& lexeme = "") {
//         tokens.emplace_back(type, lexeme);
//     }

//     void tokenizeNumber() {
//         std::ostringstream buffer;
//         while (std::isdigit(peek())) {
//             buffer.put(next());
//         }

//         if (peek() == '.') {
//             buffer.put(next()); // consume the dot
//             while (std::isdigit(peek())) {
//                 buffer.put(next());
//             }
//         }

//         addToken(tokenType::NUMBER, buffer.str());
//     }

//     void tokenizeHexNumber() {
//         std::ostringstream buffer;
//         while (isxdigit(peek())) {
//             buffer.put(next());
//         }
//         addToken(tokenType::HEX_NUMBER, buffer.str());
//     }

//     bool isPrefixHexNumber() {
//         if (position < code.size() && code[position] == 'x') {
//             next();
//             return true;
//         }
//         return false;
//     }

//     void tokenizeWord() {
//         std::ostringstream buffer;
//         while (std::isalnum(peek()) || peek() == '_') {
//             buffer.put(next());
//         }

//         std::string word = buffer.str();
//         if (word == "print") {
//             addToken(tokenType::PRINT);
//         } else if (word == "if") {
//             addToken(tokenType::IF);
//         } else if (word == "else") {
//             addToken(tokenType::ELSE);
//         } else {
//             addToken(tokenType::WORD, word);
//         }
//     }

//     void tokenizeText() {
//         next(); // Skip opening quote
//         std::ostringstream buffer;
//         char current = peek();
//         while (current != '"') {
//             if (current == '\\') {
//                 next(); // Skip the backslash
//                 current = peek();
//                 if (current == 'n') {
//                     buffer.put('\n');
//                 } else if (current == 't') {
//                     buffer.put('\t');
//                 } else if (current == '"') {
//                     buffer.put('"');
//                 } else {
//                     buffer.put('\\');
//                 }
//             } else {
//                 buffer.put(next());
//             }
//             current = peek();
//         }
//         next(); // Skip closing quote
//         addToken(tokenType::TEXT, buffer.str());
//     }

//     void tokenizeOperator() {
//         char current = peek();
//         if (current == '/') {
//             if (peek(1) == '!') {
//                 next(); next();
//                 tokenizeComment();
//                 return;
//             } else if (peek(1) == '*') {
//                 next(); next();
//                 tokenizeMultilineComment();
//                 return;
//             }
//         }

//         std::ostringstream buffer;
//         while (true) {
//             std::string text = buffer.str() + peek();
//             if (OPERATORS.find(text) != OPERATORS.end()) {
//                 buffer.put(next());
//             } else {
//                 if (!buffer.str().empty()) {
//                     addToken(OPERATORS.at(buffer.str()));
//                     return;
//                 }
//                 next();
//             }
//         }
//     }

//     void tokenizeComment() {
//         char current = peek();
//         while (current != '\0' && current != '\n') {
//             current = next();
//         }
//     }

//     void tokenizeMultilineComment() {
//         while (true) {
//             if (peek() == '\0') throw std::runtime_error("Missing close tag");
//             if (peek() == '*' && peek(1) == '/') break;
//             next();
//         }
//         next(); next(); // Skip closing 
//     }

// public:
//     explicit lexer(const std::string& code) : code(code) {}

//     std::vector<token> tokenize() {
//         while (position < code.size()) {
//             char currentSymbol = peek();

//             // Skip whitespaces faster
//             if (WHITESPACE.count(currentSymbol)) {
//                 next();
//                 continue;
//             }

//             if (currentSymbol == '0' && isPrefixHexNumber()) {
//                 tokenizeHexNumber();
//             } else if (std::isdigit(currentSymbol)) {
//                 tokenizeNumber();
//             } else if (std::isalpha(currentSymbol)) {
//                 tokenizeWord();
//             } else if (currentSymbol == '"') {
//                 tokenizeText();
//             } else if (OPERATOR_CHARS.count(currentSymbol)) {
//                 tokenizeOperator();
//             } else {
//                 next(); // Skip unknown characters
//             }
//         }
//         return tokens;
//     }
// };




#pragma once

#include "token.hpp"
#include <vector>
#include <string>
#include <cctype>
#include <algorithm>
#include <stdexcept>
#include <map>

class lexer {
private:
    std::string code;
    std::vector<token> tokens;
    size_t position = 0;

    const std::unordered_map<std::string, tokenType> OPERATORS = {
            {"+", tokenType::PLUS},
            {"-", tokenType::MINUS},
            {"*", tokenType::STAR},
            {"/", tokenType::SLASH},
            {"(", tokenType::LPAREN},
            {")", tokenType::RPAREN},
            {"=", tokenType::EQ},
            {"<", tokenType::LT},
            {">", tokenType::GT},
            
            {"!", tokenType::EXCL},
            {"&", tokenType::AMP},
            {"|", tokenType::BAR},
            
            {"==", tokenType::EQEQ},
            {"!=", tokenType::EXCLEQ},
            {"<=", tokenType::LTEQ},
            {">=", tokenType::GTEQ},
            
            {"&&", tokenType::AMPAMP},
            {"||", tokenType::BARBAR}
    };

    const std::string OPERATOR_CHARS = "+-*/()=<>!&|";

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
        } else if (buffer == "if") {
            addToken(tokenType::IF);
        } else if (buffer == "else") {
            addToken(tokenType::ELSE);
        } else {
            addToken(tokenType::WORD, buffer);
        }
    }

    void tokenizeText() {
        next(); // pass "
        std::string buffer;
        char current = peek();
        while (true) {
            if(current == '\\') {
                current = next();
                current = next();
                switch(current) {
                    case '"': current = next(); buffer += '"'; continue;
                    case 'n': current = next(); buffer += '\n'; continue;
                    case 't': current = next(); buffer += '\t'; continue;
                }
                buffer += '\\';
                continue;
            }
            if (current == '"') break;
            // current = next();
            // buffer += current;
            buffer += next();
            current = peek();
        }
        next(); // pass closing "
        addToken(tokenType::TEXT, buffer);
    }

    void tokenizeOperator() {
        // size_t pos = OPERATOR_CHARS.find(peek());
        // if (pos != std::string::npos) {
        //     addToken(OPERATOR_TOKENS[pos]);
        //     next();
        // }
        char current = peek();
        if (current == '/') {
            if (peek(1) == '!') {
                next();
                next();
                tokenizeComment();
                return;
            } else if (peek(1) == '*') {
                next();
                next();
                tokenizeMultilineComment();
                return;
            }
        }
        std::string buffer;
        while (true) {
            if (OPERATORS.find(buffer + peek()) == OPERATORS.end() && !buffer.empty()) {
                addToken(OPERATORS.at(buffer));
                return;
            }
            buffer.push_back(peek());
            next();
        }
    }

    // void tokenizeComment() {
    //     char current = peek();
    //     std::string str = "\r\n\0";
    //     while (str.find(current) != -1) {
    //         current = next();
    //     }
    // }

    void tokenizeComment() {
        char current = peek(0);
        while (current != '\n' && current != '\r' && current != '\0') {
            current = next();
        }
    }

    void tokenizeMultilineComment() {
        while (true) {
            if (peek() == '\0') throw std::runtime_error("Missing close tag");
            if (peek() == '*' && peek(1) == '/') break;
            next();
        }
        next(); next(); // pass */
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
            else if (currentSymbol == '"') {
                tokenizeText();
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