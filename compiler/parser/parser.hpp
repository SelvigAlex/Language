#pragma once

#include <vector>
#include <string>
#include <memory>
#include "/home/alexs/reverse/compiler/ast/expression.hpp"
#include "/home/alexs/reverse/compiler/parser/token.hpp"
#include "/home/alexs/reverse/compiler/ast/statement.hpp"

class parser {
private:
    static const token EOF_TOKEN;

    std::vector<token> tokens;
    size_t pos;

public:
    parser(const std::vector<token>& tokens);

    std::shared_ptr<Statement> parse();

private:
    std::shared_ptr<Expression> expression();
    std::shared_ptr<Expression> conditional();
    std::shared_ptr<Expression> logicalOr();
    std::shared_ptr<Expression> logicalAnd();
    std::shared_ptr<Expression> equality();
    std::shared_ptr<Statement> statement();
    std::shared_ptr<Statement> assigmentStatement();
    std::shared_ptr<Statement> ifElse();
    std::shared_ptr<Expression> additive();
    std::shared_ptr<Expression> multiplicative();
    std::shared_ptr<Expression> unary();
    std::shared_ptr<Expression> primary();
    std::shared_ptr<Statement> block();
    std::shared_ptr<Statement> statementOrBlock();
    std::shared_ptr<Statement> whileStatement();
    std::shared_ptr<Statement> forStatement();


    bool match(tokenType type);
    token consume(tokenType type);
    const token& get(int relativePosition);
    std::string toString(tokenType type) const;
};
