#pragma once

#include "/home/alexs/reverse/compiler/lib/value.hpp"
#include "/home/alexs/reverse/compiler/lib/variables.hpp"
#include <memory>
#include <string>

class Expression {
public:
    virtual ~Expression() = default;                 // Виртуальный деструктор
    virtual std::shared_ptr<Value> eval() const = 0; // Чисто виртуальная функция для вычисления значения
    virtual std::string toString() const = 0;        // Чисто виртуальная функция для вывода выражения на экран
};

class ValueExpression : public Expression {
private:
    std::shared_ptr<Value> value;

public:
    explicit ValueExpression(double value);
    explicit ValueExpression(std::string value);

    std::shared_ptr<Value> eval() const override;
    std::string toString() const override;
};

class BinaryExpression : public Expression {
private:
    std::shared_ptr<Expression> expr1, expr2;
    char operation;

public:
    BinaryExpression(char operation, std::shared_ptr<Expression> expr1, std::shared_ptr<Expression> expr2);

    std::shared_ptr<Value> eval() const override;
    std::string toString() const override;
};

class ConditionalExpression : public Expression {
public:
    enum Operator {
        PLUS,
        MINUS,
        MULTIPLY,
        DIVIDE,

        EQUALS,
        NOT_EQUALS,

        LT,
        LTEQ,
        GT,
        GTEQ,

        AND,
        OR
    };

private:
    std::shared_ptr<Expression> expr1, expr2;
    Operator operation;

    std::unordered_map<Operator, std::string> operatorNames;

public:
    ConditionalExpression(Operator operation, std::shared_ptr<Expression> expr1, std::shared_ptr<Expression> expr2);

    std::shared_ptr<Value> eval() const override;
    std::string asString(Operator op);
    std::string toString() const override;
};

class UnaryExpression : public Expression {
private:
    std::shared_ptr<Expression> expr1;
    char operation;

public:
    UnaryExpression(char operation, std::shared_ptr<Expression> expr1);

    std::shared_ptr<Value> eval() const override;
    std::string toString() const override;
};

class VariableExpression : public Expression {
private:
    std::string name;

public:
    explicit VariableExpression(std::string name);

    std::shared_ptr<Value> eval() const override;
    std::string toString() const override;
};