#pragma once

#include "/home/alexs/reverse/compiler/lib/value.hpp"
#include "/home/alexs/reverse/compiler/lib/variables.hpp"
#include <memory>
#include <string>
#include <vector>

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
    char operation;
    std::shared_ptr<Expression> expr1, expr2;

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
    Operator operation;
    std::shared_ptr<Expression> expr1, expr2;

    std::unordered_map<Operator, std::string> operatorNames;

public:
    ConditionalExpression(Operator operation, std::shared_ptr<Expression> expr1, std::shared_ptr<Expression> expr2);

    std::shared_ptr<Value> eval() const override;
    std::string asString(Operator op);
    std::string toString() const override;
};

class UnaryExpression : public Expression {
private:
    char operation;
    std::shared_ptr<Expression> expr1;

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

class FunctionalExpression : public Expression {
private:
    std::string name;
    std::vector<std::shared_ptr<Expression>> arguments;

public:
    explicit FunctionalExpression(std::string name, std::vector<std::shared_ptr<Expression>> arguments);
    explicit FunctionalExpression(std::string name);
    std::shared_ptr<Value> eval() const override;
    std::string toString() const override;
    void addArgument(std::shared_ptr<Expression> arg);
};

class ArrayAccessExpression : public Expression {
private:
    std::string variable;
    std::shared_ptr<Expression> index;
public:
    explicit ArrayAccessExpression(const std::string& variable, std::shared_ptr<Expression> index);
    std::shared_ptr<Value> eval() const override;
    std::string toString() const override;
};

class ArrayExpression : public Expression {
private:
    std::vector<std::shared_ptr<Expression>> elements;
public:
    explicit ArrayExpression(std::vector<std::shared_ptr<Expression>>& elements);
    std::shared_ptr<Value> eval() const override;
    std::string toString() const override;
};