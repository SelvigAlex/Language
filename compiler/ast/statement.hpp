#pragma once

#include <memory>
#include <string>
#include "expression.hpp"
#include <vector>

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

class EchoStatement : public Statement {
private:
    std::shared_ptr<Expression> expression;

public:
    explicit EchoStatement(std::shared_ptr<Expression> expression);
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
public:
    ~BreakStatement() noexcept = default;
    void execute() override;
    std::string toString() const override;
};


class ContinueStatement : public Statement, std::exception{
public:
    ~ContinueStatement() noexcept = default;
    void execute() override;
    std::string toString() const override;
};


class ReturnStatement : public Statement, std::exception {
private:
    std::shared_ptr<Expression> expression;
    std::shared_ptr<Value> result;

public:
    explicit ReturnStatement(std::shared_ptr<Expression> expression);
    ~ReturnStatement() noexcept = default;
    void execute() override;
    std::shared_ptr<Value> getResult();
    std::string toString() const override;
};

class FunctionStatement : public Statement {
private:
    std::shared_ptr<FunctionalExpression> function;

public:
    explicit FunctionStatement(std::shared_ptr<FunctionalExpression> function);
    void execute() override;
    std::string toString() const override;
};


class FunctionDefine : public Statement {
private:
    std::string name;
    std::vector<std::string> argNames;
    std::shared_ptr<Statement> body;

public:
    explicit FunctionDefine(const std::string& name, const std::vector<std::string>& argNames, std::shared_ptr<Statement> body);
    void execute() override;
    std::string toString() const override;
};