#pragma once

#include <vector>
#include <string>
#include <memory>
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

    std::vector<std::shared_ptr<Statement>> parse() {
        std::vector<std::shared_ptr<Statement>> result;
        while (!match(tokenType::EOF_)) {
            result.push_back(statement());
        }
        return result;
    }

private:
    std::shared_ptr<Expression> expression() {
        return logicalOr();
    }


    std::shared_ptr<Expression> conditional() {
        auto result = additive();

        while (true) {
            if (match(tokenType::LT)) {
                result = std::make_shared<ConditionalExpression>(ConditionalExpression::Operator::LT, std::move(result), additive());
            }
            else if (match(tokenType::LTEQ)) {
                result = std::make_shared<ConditionalExpression>(ConditionalExpression::Operator::LTEQ, std::move(result), additive());
            }
            else if (match(tokenType::GT)) {
                result = std::make_shared<ConditionalExpression>(ConditionalExpression::Operator::GT, std::move(result), additive());
            }
            else if (match(tokenType::GTEQ)) {
                result = std::make_shared<ConditionalExpression>(ConditionalExpression::Operator::GTEQ, std::move(result), additive());
            }
            else {
                break;
            }
        }
        return result;
    }

    std::shared_ptr<Expression> logicalOr() {
        std::shared_ptr<Expression> result = logicalAnd();
        while(true) {
            if (match(tokenType::BARBAR)) {
                result = std::make_shared<ConditionalExpression>(ConditionalExpression::Operator::OR, std::move(result), logicalAnd());
                continue;
            }
            break;
        }
        
        
        return result;
    }

    std::shared_ptr<Expression> logicalAnd() {
        std::shared_ptr<Expression> result = equality();
        while(true) {
            if (match(tokenType::AMPAMP)) {
                result = std::make_shared<ConditionalExpression>(ConditionalExpression::Operator::AND, std::move(result), equality());
                continue;
            }
            break;
        }
        return result;
    }

    std::shared_ptr<Expression> equality() {
        std::shared_ptr<Expression> result = conditional();
        if (match(tokenType::EQEQ)) {
            return std::make_shared<ConditionalExpression>(ConditionalExpression::Operator::EQUALS, std::move(result), conditional());
        }
        else if (match(tokenType::EXCLEQ)) {
            return std::make_shared<ConditionalExpression>(ConditionalExpression::Operator::NOT_EQUALS, std::move(result), conditional());
        }
        return result;

    }

    std::shared_ptr<Statement> statement() {
        if (match(tokenType::PRINT)) {
            return std::make_shared<PrintStatement>(expression());
        }
        if (match(tokenType::IF)) {
            return ifElse();
        }
        return assigmentStatement();
    }

    std::shared_ptr<Statement> assigmentStatement() {
        auto current = get(0);
        if (match(tokenType::WORD) && get(0).getTokenType() == tokenType::EQ) {
            std::string variable = current.getLexeme();
            consume(tokenType::EQ);
            return std::make_shared<AssigmentStatement>(variable, expression());
        }
        throw std::runtime_error("Invalid assignment statement: token " + current.getLexeme() + " doesn't match");
    }

    std::shared_ptr<Statement> ifElse() {
        std::shared_ptr<Expression> condition = expression();
        std::shared_ptr<Statement> ifStatement = statement();
        std::shared_ptr<Statement> elseStatement;

        if (match(tokenType::ELSE)) {
            elseStatement = statement();
        } else {
            elseStatement = nullptr;
        }

        return std::make_shared<IfStatement>(condition, ifStatement, elseStatement);
    }

   

    std::shared_ptr<Expression> additive() {
        auto result = multiplicative();

        while (true) {
            if (match(tokenType::PLUS)) {
                result = std::make_shared<BinaryExpression>('+', std::move(result), multiplicative());
            }
            else if (match(tokenType::MINUS)) {
                result = std::make_shared<BinaryExpression>('-', std::move(result), multiplicative());
            }
            else {
                break;
            }
        }
        return result;
    }

    std::shared_ptr<Expression> multiplicative() {
        auto result = unary();

        while (true) {
            if (match(tokenType::STAR)) {
                result = std::make_shared<BinaryExpression>('*', std::move(result), unary());
            }
            else if (match(tokenType::SLASH)) {
                result = std::make_shared<BinaryExpression>('/', std::move(result), unary());
            }
            else {
                break;
            }
        }

        return result;
    }

    std::shared_ptr<Expression> unary() {
        if (match(tokenType::MINUS)) {
            return std::make_shared<UnaryExpression>('-', primary());
        }
        if (match(tokenType::PLUS)) {
            return primary();
        }
        return primary();
    }

    std::shared_ptr<Expression> primary() {
        const auto& current = get(0);
        if (match(tokenType::NUMBER)) {
            return std::make_shared<ValueExpression>(std::stod(current.getLexeme()));
        }
        if (match(tokenType::HEX_NUMBER)) { 
            return std::make_shared<ValueExpression>(std::stoul(current.getLexeme(), nullptr, 16));
        }
        if (match(tokenType::WORD)) {
            return std::make_shared<VariableExpression>(current.getLexeme());
        }
        if (match(tokenType::TEXT)) {
            return std::make_shared<ValueExpression>(current.getLexeme());
        }
        if (match(tokenType::LPAREN)) {
            auto result = expression();
            consume(tokenType::RPAREN);
            return result;
        }

        throw std::runtime_error("Unexpected token: " + current.getLexeme());
    }

    bool match(tokenType type) {
        const auto& current = get(0);
        if (type != current.getTokenType()) return false;
        pos++;
        return true;
    }

    token consume(tokenType type) {
        const auto& current = get(0);
        if (type != current.getTokenType()) {
            throw std::runtime_error("Expected token " + toString(type) + ", but got " + current.getLexeme());
        }
        pos++;
        return current;
    }

    const token& get(int relativePosition) {
        size_t position = pos + relativePosition;
        if (position >= tokens.size()) return EOF_TOKEN;
        return tokens[position];
    }

    std::string toString(tokenType type) const {
        switch (type) {
            case tokenType::PLUS: return "PLUS";
            case tokenType::MINUS: return "MINUS";
            case tokenType::STAR: return "STAR";
            case tokenType::SLASH: return "SLASH";
            case tokenType::EQ: return "EQ";
            case tokenType::NUMBER: return "NUMBER";
            case tokenType::HEX_NUMBER: return "HEX_NUMBER";
            case tokenType::WORD: return "WORD";
            case tokenType::PRINT: return "PRINT";
            case tokenType::LPAREN: return "LPAREN";
            case tokenType::RPAREN: return "RPAREN";
            case tokenType::EOF_: return "EOF";
            default: return "UNKNOWN";
        }
    }
};

// Определение статической константы EOF_TOKEN
const token parser::EOF_TOKEN = token(tokenType::EOF_, "");