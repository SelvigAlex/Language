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
    PERCENT, // %
    EQ,     // =
    EQEQ,   // ==
    EXCL,   // !
    EXCLEQ,  // !=
    LPAREN, // (
    RPAREN, // )
    LBRACE, // {
    RBRACE, // }
    LBRACKET, // [
    RBRACKET, // ]
    LT,     // <
    LTEQ,   // <=
    GT,     // >
    GTEQ,   // <
    AMP,    // &
    AMPAMP, // &&
    BAR,    // |
    BARBAR, // ||
    SEMICOLON, // ;
    COMMA, // ,

    //! keyword
    ECHO,
    IF,
    ELSE,
    WHILE,
    FOR,
    DO,
    BREAK,
    CONTINUE,
    FUNCTION,
    RETURN,


    EOF_     // end of file
};

class token {
private:
    tokenType type;
    std::string lexeme;

    // Использование unordered_map для улучшения производительности поиска
    static const std::unordered_map<tokenType, std::string> tokenToString;

public:
    explicit token(tokenType type, const std::string& lexeme);

    void setLexeme(const std::string& lexeme);
    void setTokenType(tokenType type);

    const std::string& getLexeme() const;
    tokenType getTokenType() const;

    std::string toString() const;
};
