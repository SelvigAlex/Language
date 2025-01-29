#pragma once

#include <memory>
#include <string>
#include "variables.hpp"
#include "value.hpp"

class Expression {
public:
    virtual ~Expression() = default; // Виртуальный деструктор
    virtual double eval() const = 0; // Чисто виртуальная функция для вычисления значения
    virtual std::string toString() const = 0; // Чисто виртуальная функция для вывода выражения на экран
};

class NumberExpression : public Expression {
private:
    double value;

public:
    explicit NumberExpression(double value) : value(value) {}

    double eval() const override {
        return value;
    }

    std::string toString() const override {
        return std::to_string(value);
    }
};

class BinaryExpression : public Expression {
private:
    std::unique_ptr<Expression> expr1, expr2;
    char operation;

public:
    BinaryExpression(char operation, std::unique_ptr<Expression> expr1, std::unique_ptr<Expression> expr2)
        : operation(operation), expr1(std::move(expr1)), expr2(std::move(expr2)) {}

    double eval() const override {
        switch (operation) {
            case '-': return expr1->eval() - expr2->eval();
            case '*': return expr1->eval() * expr2->eval();
            case '/': return expr1->eval() / expr2->eval();
            case '+':
            default:
                return expr1->eval() + expr2->eval();
        }
    }

    std::string toString() const override {
        return "(" + expr1->toString() + " " + operation + " " + expr2->toString() + ")";
    }
};

class UnaryExpression : public Expression {
private:
    std::unique_ptr<Expression> expr1; // Указатель на подвыражение
    char operation; // Операция (например, '+', '-')

public:
    UnaryExpression(char operation, std::unique_ptr<Expression> expr1)
        : operation(operation), expr1(std::move(expr1)) {}

    double eval() const override {
        switch (operation) {
            case '-':
                return -expr1->eval(); // Отрицание
            case '+':
            default:
                return expr1->eval(); // Положительное значение
        }
    }
    std::string toString() const override {
        return std::string(1, operation) + " " + expr1->toString(); // Форматирование строки
    }
};


class VariableExpression :public Expression {
private:
    std::string name;


public:
    VariableExpression(std::string name) : name(name) {}

    double eval() const override {
        if (!Variables::isExists(name)) throw std::runtime_error("Constant does not exist");
        return Variables::get(name);
    }

    std::string toString() const override {
        return std::to_string(Variables::get(name)); // Форматирование строки
    }
};