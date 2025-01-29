#pragma once


#include <memory>
#include <string>
#include <iostream>
#include "expression.hpp"

class Statement {
public:
    virtual ~Statement() = default;
    virtual void execute() = 0;
    virtual std::string toString() const = 0;
};

class AssigmentStatement : public Statement {
private:
    std::unique_ptr<Expression> expression;
    std::string variable;

public:
    AssigmentStatement(std::string variable, std::unique_ptr<Expression> expression)
        : variable(variable), expression(std::move(expression)) {}

    void execute() override {
        double result = expression.get()->eval();
        Variables::set(variable, result);
    }

    std::string toString() const override {
        return variable + " = " + expression.get()->toString();
    }
};

class PrintStatement : public Statement {
private:
    std::unique_ptr<Expression> expression;

public:
    PrintStatement(std::unique_ptr<Expression> expression) : expression(std::move(expression)) {}

    void execute() override {
        std::cout << expression.get()->eval();
    }

    std::string toString() const override {
        return "print " + expression.get()->toString();
    }




};