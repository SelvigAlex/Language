#pragma once

#include <vector>
#include <string>
#include "expression.hpp"
#include "token.hpp"
#include "statement.hpp"

class parser {
private:
    static const token EOF_TOKEN;

    std::vector<token> tokens;
    size_t pos;

public:
    parser(const std::vector<token>& tokens) : tokens(tokens), pos(0) {}

    std::vector<std::unique_ptr<Statement>> parse() {
        std::vector<std::unique_ptr<Statement>> result;
        while (!match(tokenType::EOF_)) {
            result.push_back(statement());
        }
        return result;
    }

private:
    std::unique_ptr<Expression> expression() {
        return additive();
    }

    std::unique_ptr<Statement> statement() {
        if (match(tokenType::PRINT)) {
            return std::make_unique<PrintStatement>(expression());
        }
        return assigmentStatement();
    }



    std::unique_ptr<Statement> assigmentStatement() {
        token current = get(0);
        if (match(tokenType::WORD) && get(0).getTokenType() == tokenType::EQ) {
            std::string variable = current.getLexeme();
            consume(tokenType::EQ);
            return std::make_unique<AssigmentStatement>(variable, expression());
        }
        throw std::runtime_error("Unknown operator");
    }

    std::unique_ptr<Expression> additive() {
        auto result = multiplicative();

        while (true) {
            if (match(tokenType::PLUS)) {
                result = std::make_unique<BinaryExpression>('+', std::move(result), multiplicative());
                continue;
            }
            if (match(tokenType::MINUS)) {
                result = std::make_unique<BinaryExpression>('-', std::move(result), multiplicative());
                continue;
            }
            break;
        }

        return result;
    }

    std::unique_ptr<Expression> multiplicative() {
        auto result = unary();

        while (true) {
            if (match(tokenType::STAR)) {
                result = std::make_unique<BinaryExpression>('*', std::move(result), unary());
                continue;
            }
            if (match(tokenType::SLASH)) {
                result = std::make_unique<BinaryExpression>('/', std::move(result), unary());
                continue;
            }
            break;
        }

        return result;
    }

    std::unique_ptr<Expression> unary() {
        if (match(tokenType::MINUS)) {
            return std::make_unique<UnaryExpression>('-', primary());
        }
        if (match(tokenType::PLUS)) {
            return primary();
        }
        return primary();
    }

    std::unique_ptr<Expression> primary() {
        const token& current = get(0);
        if (match(tokenType::NUMBER)) {
            return std::make_unique<NumberExpression>(std::stod(current.getLexeme()));
        }
        if (match(tokenType::HEX_NUMBER)) { 
            return std::make_unique<NumberExpression>(std::stoul(current.getLexeme(), nullptr, 16));
        }
        if (match(tokenType::WORD)) {
            return std::make_unique<VariableExpression>(current.getLexeme());
        }
        if (match(tokenType::LPAREN)) {
            auto result = expression();
            match(tokenType::RPAREN);
            return result;
        }
        
        throw std::runtime_error("Unknown expression");
    }

    bool match(tokenType type) {
        const token& current = get(0);
        if (type != current.getTokenType()) return false;
        pos++;
        return true;
    }

    token consume(tokenType type) {
        const token& current = get(0);
        if (type != current.getTokenType()) throw std::runtime_error("Token " + current.getLexeme() + "doesn't match");
        pos++;
        return current;
    }

    const token& get(int relativePosition) {
        size_t position = pos + relativePosition;
        if (position >= tokens.size()) return EOF_TOKEN;
        return tokens[position];
    }
};

// Определение статической константы EOF_TOKEN
const token parser::EOF_TOKEN = token(tokenType::EOF_, "");
