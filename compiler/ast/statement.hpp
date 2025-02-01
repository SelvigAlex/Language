#pragma once

#include <memory>
#include <string>
#include "expression.hpp"
#include <vector>
#include "/home/alexs/reverse/compiler/lib/value.hpp"

class Statement {
public:
    virtual ~Statement() = default;
    virtual void execute() = 0;
    virtual std::string toString() const = 0;
};

class AssigmentStatement : public Statement {
private:
    std::string variable;
    std::shared_ptr<Expression> expression;

public:
    explicit AssigmentStatement(std::string variable, std::shared_ptr<Expression> expression);
    void execute() override;
    std::string toString() const override;
};

class PrintStatement : public Statement {
private:
    std::shared_ptr<Expression> expression;

public:
    explicit PrintStatement(std::shared_ptr<Expression> expression);
    void execute() override;
    std::string toString() const override;
};

class IfStatement : public Statement {
private:
    std::shared_ptr<Expression> expression;
    std::shared_ptr<Statement> ifStatement, elseStatement;

public:
    explicit IfStatement(std::shared_ptr<Expression> expression, std::shared_ptr<Statement> ifStatement, std::shared_ptr<Statement> elseStatement);
    void execute() override;
    std::string toString() const override;
};

class BlockStatement : public Statement {
private:
    std::vector<std::shared_ptr<Statement>> statements;

public:
    explicit BlockStatement() = default;
    void add(std::shared_ptr<Statement> statement);
    void execute() override;
    std::string toString() const override;
};


class WhileStatement : public Statement {
private:
    std::shared_ptr<Expression> condition;
    std::shared_ptr<Statement> statement;
public:
    explicit WhileStatement(std::shared_ptr<Expression> condition, std::shared_ptr<Statement> statement);
    void execute() override;
    std::string toString() const override;
};


class DoWhileStatement : public Statement {
private:
    std::shared_ptr<Expression> condition;
    std::shared_ptr<Statement> statement;
public:
    explicit DoWhileStatement(std::shared_ptr<Expression> condition, std::shared_ptr<Statement> statement);
    void execute() override;
    std::string toString() const override;
};

class ForStatement : public Statement {
private:
    std::shared_ptr<Statement> initialization;
    std::shared_ptr<Expression> termination;
    std::shared_ptr<Statement> increment;
    std::shared_ptr<Statement> statement;
public:
    explicit ForStatement(std::shared_ptr<Statement> initialization, std::shared_ptr<Expression> termination, std::shared_ptr<Statement> increment, std::shared_ptr<Statement> statement);
    void execute() override;
    std::string toString() const override;
};


class BreakStatement : public Statement, std::exception{
private:

public:
    ~BreakStatement() noexcept = default;
    void execute() override;
    std::string toString() const override;
};


class ContinueStatement : public Statement, std::exception{
private:

public:
    ~ContinueStatement() noexcept = default;
    void execute() override;
    std::string toString() const override;
};
