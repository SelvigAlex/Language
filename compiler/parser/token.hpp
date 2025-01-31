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
    LBRACE, // {
    RBRACE, // }
    LT,     // <
    LTEQ,   // <=
    GT,     // >
    GTEQ,   // <
    AMP,    // &
    AMPAMP, // &&
    BAR,    // |
    BARBAR, // ||
    SEMICOLON, // ;

    //! keyword
    PRINT,
    IF,
    ELSE,
    WHILE,
    FOR,

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
