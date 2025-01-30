#pragma once

#include <memory>
#include <string>
#include <iostream>
#include "expression.hpp"
#include "value.hpp"

class Statement {
public:
    virtual ~Statement() = default;
    virtual void execute() = 0;
    virtual std::string toString() const = 0;
};

class AssigmentStatement : public Statement {
private:
    std::shared_ptr<Expression> expression;
    std::string variable;

public:
    AssigmentStatement(std::string variable, std::shared_ptr<Expression> expression)
        : variable(std::move(variable)), expression(std::move(expression)) {}

    void execute() override {
        // Получаем результат выражения
        std::shared_ptr<Value> result = expression->eval();
        // Присваиваем результат в переменную
        Variables::set(variable, result);
    }

    std::string toString() const override {
        return variable + " = " + expression->toString();
    }
};

class PrintStatement : public Statement {
private:
    std::shared_ptr<Expression> expression;

public:
    explicit PrintStatement(std::shared_ptr<Expression> expression) 
        : expression(std::move(expression)) {}

    void execute() override {
        // Печатаем строковое представление результата выражения
        std::cout << expression->eval()->asString();
    }

    std::string toString() const override {
        return "print " + expression->toString();
    }
};


class IfStatement : public Statement{
private:
    std::shared_ptr<Expression> expression;
    std::shared_ptr<Statement> ifStatement, elseStatement;
public:
    explicit IfStatement(std::shared_ptr<Expression> expression, std::shared_ptr<Statement> ifStatement, std::shared_ptr<Statement> elseStatement)
        : expression(std::move(expression)), ifStatement(std::move(ifStatement)), elseStatement(std::move(elseStatement)) {}

    void execute() override {
        double result = expression->eval()->asNumber();
        if (result != 0) {
            ifStatement->execute();
        } else if (elseStatement != nullptr) {
            elseStatement->execute();
        }
    }

    std::string toString() const override {
        std::string result;
        result = "if " + expression->toString() + " " + ifStatement->toString();
        if (elseStatement != nullptr) {
            result += "\nelse " + elseStatement->toString();
        }
        return result;
    }

};