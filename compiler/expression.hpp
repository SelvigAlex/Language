#pragma once

#include <memory>
#include <string>
#include <stdexcept>
#include "variables.hpp"
#include "value.hpp"

class Expression {
public:
    virtual ~Expression() = default; // Виртуальный деструктор
    virtual std::shared_ptr<Value> eval() const = 0; // Чисто виртуальная функция для вычисления значения
    virtual std::string toString() const = 0; // Чисто виртуальная функция для вывода выражения на экран
};

class NumberExpression : public Expression {
private:
    std::shared_ptr<NumberValue> value;

public:
    explicit NumberExpression(double value) 
        : value(std::make_shared<NumberValue>(value)) {}

    std::shared_ptr<Value> eval() const override {
        return value;
    }

    std::string toString() const override {
        return value->asString();
    }
};

class BinaryExpression : public Expression {
private:
    std::shared_ptr<Expression> expr1, expr2;
    char operation;

public:
    BinaryExpression(char operation, std::shared_ptr<Expression> expr1, std::shared_ptr<Expression> expr2)
        : operation(operation), expr1(std::move(expr1)), expr2(std::move(expr2)) {}

    std::shared_ptr<Value> eval() const override {
        double number1 = expr1->eval()->asNumber();
        double number2 = expr2->eval()->asNumber();
        
        // Проверка на деление на ноль
        if (operation == '/' && number2 == 0) {
            throw std::runtime_error("Division by zero");
        }

        switch (operation) {
            case '-': return std::make_shared<NumberValue>(number1 - number2);
            case '*': return std::make_shared<NumberValue>(number1 * number2);
            case '/': return std::make_shared<NumberValue>(number1 / number2);
            case '+':
            default:
                return std::make_shared<NumberValue>(number1 + number2);
        }
    }

    std::string toString() const override {
        return "(" + expr1->toString() + " " + operation + " " + expr2->toString() + ")";
    }
};

class UnaryExpression : public Expression {
private:
    std::shared_ptr<Expression> expr1; // Указатель на подвыражение
    char operation; // Операция (например, '+', '-')

public:
    UnaryExpression(char operation, std::shared_ptr<Expression> expr1)
        : operation(operation), expr1(std::move(expr1)) {}

    std::shared_ptr<Value> eval() const override {
        switch (operation) {
            case '-':
                return std::make_shared<NumberValue>(-expr1->eval()->asNumber()); // Отрицание
            case '+':
            default:
                return std::make_shared<NumberValue>(expr1->eval()->asNumber()); // Положительное значение
        }
    }

    std::string toString() const override {
        return std::string(1, operation) + " " + expr1->toString(); // Форматирование строки
    }
};

class VariableExpression : public Expression {
private:
    std::string name;

public:
    explicit VariableExpression(std::string name) : name(std::move(name)) {}

    std::shared_ptr<Value> eval() const override {
        if (!Variables::isExists(name)) {
            throw std::runtime_error("Constant '" + name + "' does not exist");
        }
        return Variables::get(name);
    }

    std::string toString() const override {
        return name; // Форматирование строки
    }
};
