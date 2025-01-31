#include "token.hpp"

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
    {tokenType::LBRACE, "OPERATOR {"},
    {tokenType::RBRACE, "OPERATOR }"},
    {tokenType::SEMICOLON, "OPERATOR ;"},
    {tokenType::WORD, "WORD"},
    {tokenType::PRINT, "KEYWORD PRINT"},
    {tokenType::WHILE, "KEYWORD WHILE"},
    {tokenType::FOR, "KEYWORD FOR"},
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

token::token(tokenType type, const std::string& lexeme) : type(type), lexeme(lexeme) {}

void token::setLexeme(const std::string& lexeme) { this->lexeme = lexeme; }
void token::setTokenType(tokenType type) { this->type = type; }

const std::string& token::getLexeme() const { return lexeme; }
tokenType token::getTokenType() const { return type; }

std::string token::toString() const {
    auto it = tokenToString.find(type);
    return (it != tokenToString.end()) ? it->second : "UNKNOWN";
}
