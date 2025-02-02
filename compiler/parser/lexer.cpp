#include "lexer.hpp"
#include <stdexcept>

const std::unordered_map<std::string, tokenType> lexer::OPERATORS = {
    {"+", tokenType::PLUS},
    {"-", tokenType::MINUS},
    {"*", tokenType::STAR},
    {"/", tokenType::SLASH},
    {"(", tokenType::LPAREN},
    {")", tokenType::RPAREN},
    {"{", tokenType::LBRACE},
    {"}", tokenType::RBRACE},
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
    {"||", tokenType::BARBAR},
    {";", tokenType::SEMICOLON},

};

const std::string lexer::OPERATOR_CHARS = "+-*/(){}=<>!&|;";

char lexer::next() {
    return (position < code.size()) ? code[position++] : '\0';
}

char lexer::peek(int offset) const {
    size_t pos = position + offset;
    return (pos < code.size()) ? code[pos] : '\0';
}

void lexer::addToken(tokenType type, const std::string& lexeme) {
    tokens.emplace_back(type, lexeme);
}

void lexer::tokenizeNumber() {
    std::string buffer;
    while (true) {
        if (peek() == '.') {
            if (buffer.find('.') != std::string::npos) {
                throw std::runtime_error("Invalid float number");
            }
        } else if (!std::isdigit(peek())) {
            break;
        }
        buffer += next();
    }
    addToken(tokenType::NUMBER, buffer);
}

void lexer::tokenizeHexNumber() {
    std::string buffer;
    while (isxdigit(peek())) {
        buffer += next();
    }
    addToken(tokenType::HEX_NUMBER, buffer);
}

bool lexer::isPrefixHexNumber(size_t position) {
    if (position < code.size() && code[position] == 'x') {
        next();
        return true;
    }
    return false;
}

void lexer::tokenizeWord() {
    std::string buffer;
    while (true) {
        if (!std::isalnum(peek()) && (peek() != '_')) {
            break;
        }
        buffer += next();
    }
    if (buffer == "echo") {
        addToken(tokenType::ECHO);
    } else if (buffer == "if") {
        addToken(tokenType::IF);
    } else if (buffer == "else") {
        addToken(tokenType::ELSE);
    } else if (buffer == "while") {
        addToken(tokenType::WHILE);
    } else if (buffer == "for") {
        addToken(tokenType::FOR);
    } else if (buffer == "do") {
        addToken(tokenType::DO);
    } else if (buffer == "break") {
        addToken(tokenType::BREAK);
    } else if (buffer == "continue") {
        addToken(tokenType::CONTINUE);
    } else if (buffer == "function") {
        addToken(tokenType::FUNCTION);
    } else if (buffer == "return") {
        addToken(tokenType::RETURN);
    } 
    else {
        addToken(tokenType::WORD, buffer);
    }
}

void lexer::tokenizeText() {
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
        buffer += next();
        current = peek();
    }
    next(); // pass closing "
    addToken(tokenType::TEXT, buffer);
}

void lexer::tokenizeOperator() {
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

void lexer::tokenizeComment() {
    char current = peek(0);
    while (current != '\n' && current != '\r' && current != '\0') {
        current = next();
    }
}

void lexer::tokenizeMultilineComment() {
    while (true) {
        if (peek() == '\0') throw std::runtime_error("Missing close tag");
        if (peek() == '*' && peek(1) == '/') break;
        next();
    }
    next(); next(); // pass */
}

lexer::lexer(const std::string& code) : code(code) {}

std::vector<token> lexer::tokenize() {
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
