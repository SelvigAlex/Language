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

class ValueExpression : public Expression {
private:
    std::shared_ptr<Value> value;

public:
    explicit ValueExpression(double value) 
        : value(std::make_shared<NumberValue>(value)) {}

    explicit ValueExpression(std::string value) 
        : value(std::make_shared<StringValue>(value)) {}

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

    // std::shared_ptr<Value> eval() const override {
    //     std::shared_ptr<Value> value1 = expr1->eval();

    //     if (std::shared_ptr<StringValue> New = dynamic_cast<StringValue*>(value1)) {

    //     }

    //     double number1 = value1->asNumber();
    //     double number2 = expr2->eval()->asNumber();
        
    //     // Проверка на деление на ноль
    //     if (operation == '/' && number2 == 0) {
    //         throw std::runtime_error("Division by zero");
    //     }

    //     switch (operation) {
    //         case '-': return std::make_shared<NumberValue>(number1 - number2);
    //         case '*': return std::make_shared<NumberValue>(number1 * number2);
    //         case '/': return std::make_shared<NumberValue>(number1 / number2);
    //         case '+':
    //         default:
    //             return std::make_shared<NumberValue>(number1 + number2);
    //     }
    // }

    std::shared_ptr<Value> eval() const override {
        std::shared_ptr<Value> value1 = expr1->eval();
        std::shared_ptr<Value> value2 = expr2->eval();
        
        if (dynamic_cast<StringValue*>(value1.get())) {
            std::string string1 = value1->asString();
            if (operation == '*') {
                int iterations = static_cast<int>(value2->asNumber());
                std::string result;
                for (int i = 0; i < iterations; ++i) {
                    result += string1;
                }
                return std::make_shared<StringValue>(result);
            } else {
                return std::make_shared<StringValue>(string1 + value2->asString());
            }
        }

        double number1 = value1->asNumber();
        double number2 = value2->asNumber();

        switch (operation) {
            case '-': return std::make_shared<NumberValue>(number1 - number2);
            case '*': return std::make_shared<NumberValue>(number1 * number2);
            case '/': return std::make_shared<NumberValue>(number1 / number2);
            case '+':
            default: return std::make_shared<NumberValue>(number1 + number2);
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
            //case '++': префикс и постфикс
            //case '--':
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
