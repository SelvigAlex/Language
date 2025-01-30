#pragma once

#include <string>
#include <unordered_map>

enum class tokenType {
    NUMBER,    // 123
    HEX_NUMBER, // 0x123
    WORD,      // constant
    TEXT,       // string
    


    PLUS,   // +
    MINUS,  // -
    STAR,   // *
    SLASH,  // /
    EQ,     // =
    EQEQ,   // ==
    EXCL,   // !
    EXCLEQ,  // !=
    LPAREN, // (
    RPAREN, // )
    LT,     // <
    LTEQ,   // <=
    GT,     // >
    GTEQ,   // <
    AMP,    // &
    AMPAMP, // &&
    BAR,    // |
    BARBAR, // ||


    //! keyword
    PRINT,   
    IF,
    ELSE,



    EOF_     // end of file
};

class token {
private:
    tokenType type;
    std::string lexeme;

    // Использование unordered_map для улучшения производительности поиска
    static const std::unordered_map<tokenType, std::string> tokenToString;

public:
    explicit token(tokenType type, const std::string& lexeme) : type(type), lexeme(lexeme) {}

    void setLexeme(const std::string& lexeme) { this->lexeme = lexeme; }
    void setTokenType(tokenType type) { this->type = type; }

    const std::string& getLexeme() const { return lexeme; }
    tokenType getTokenType() const { return type; }

    std::string toString() const {
        auto it = tokenToString.find(type);
        return (it != tokenToString.end()) ? it->second : "UNKNOWN";
    }
};

// Инициализация статической переменной с использованием unordered_map
const std::unordered_map<tokenType, std::string> token::tokenToString = {
    {tokenType::PLUS, "OPERATOR +"},
    {tokenType::MINUS, "OPERATOR -"},
    {tokenType::STAR, "OPERATOR *"},
    {tokenType::SLASH, "OPERATOR /"},
    {tokenType::EQ, "OPERATOR ="},
    {tokenType::NUMBER, "NUMBER"},
    {tokenType::HEX_NUMBER, "HEX_NUMBER"},
    {tokenType::LPAREN, "OPERATOR ("},
    {tokenType::RPAREN, "OPERATOR )"},
    {tokenType::WORD, "WORD"},
    {tokenType::PRINT, "KEYWORD PRINT"},
    {tokenType::TEXT, "TEXT"},
    {tokenType::IF, "IF"},
    {tokenType::ELSE, "ELSE"},
    {tokenType::LT, "OPERATOR <"},
    {tokenType::GT, "OPERATOR >"},
    {tokenType::EQEQ, "OPERATOR =="},
    {tokenType::LTEQ, "OPERATOR <="},
    {tokenType::GTEQ, "OPERATOR >="},
    {tokenType::EXCL, "OPERATOR !"},
    {tokenType::EXCLEQ, "OPERATOR !="},
    {tokenType::AMP, "OPERATOR &"},
    {tokenType::AMPAMP, "OPERATOR &&"},
    {tokenType::BAR, "OPERATOR |"},
    {tokenType::BARBAR, "OPERATOR ||"},
};
