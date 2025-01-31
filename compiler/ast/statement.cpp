#include "statement.hpp"
#include <iostream>
#include <memory>

AssigmentStatement::AssigmentStatement(std::string variable, std::shared_ptr<Expression> expression)
    : variable(std::move(variable)), expression(std::move(expression)) {}

void AssigmentStatement::execute() {
    // Получаем результат выражения
    std::shared_ptr<Value> result = expression->eval();
    // Присваиваем результат в переменную
    Variables::set(variable, result);
}

std::string AssigmentStatement::toString() const {
    return variable + " = " + expression->toString();
}

PrintStatement::PrintStatement(std::shared_ptr<Expression> expression)
    : expression(std::move(expression)) {}

void PrintStatement::execute() {
    // Печатаем строковое представление результата выражения
    std::cout << expression->eval()->asString();
}

std::string PrintStatement::toString() const {
    return "print " + expression->toString();
}

IfStatement::IfStatement(std::shared_ptr<Expression> expression, std::shared_ptr<Statement> ifStatement, std::shared_ptr<Statement> elseStatement)
    : expression(std::move(expression)), ifStatement(std::move(ifStatement)), elseStatement(std::move(elseStatement)) {}

void IfStatement::execute() {
    double result = expression->eval()->asNumber();
    if (result != 0) {
        ifStatement->execute();
    } else if (elseStatement != nullptr) {
        elseStatement->execute();
    }
}

std::string IfStatement::toString() const {
    std::string result;
    result = "if " + expression->toString() + " " + ifStatement->toString();
    if (elseStatement != nullptr) {
        result += "\nelse " + elseStatement->toString();
    }
    return result;
}

void BlockStatement::add(std::shared_ptr<Statement> statement) {
    statements.push_back(statement);
}

void BlockStatement::execute() {
    for (const auto& statement : statements) {
        statement->execute();
    }
}

std::string BlockStatement::toString() const {
    std::string result;
    for (const auto& statement : statements) {
        result.append(statement->toString() + '\n');
    }
    return result;
}

WhileStatement::WhileStatement(std::shared_ptr<Expression> condition, std::shared_ptr<Statement> statement)
    : condition(std::move(condition)), statement(std::move(statement)) {}


void WhileStatement::execute() {
    while (condition->eval()->asNumber()) {
        statement->execute();
    }
}

std::string WhileStatement::toString() const {
    return "while " + condition->toString() + " " + statement->toString();
}

ForStatement::ForStatement(std::shared_ptr<Statement> initialization, std::shared_ptr<Expression> termination, std::shared_ptr<Statement> increment, std::shared_ptr<Statement> statement) 
    : initialization(std::move(initialization)), termination(std::move(termination)), increment(std::move(increment)), statement(std::move(statement)) {}

void ForStatement::execute() {
    for (initialization->execute(); termination->eval()->asNumber(); increment->execute()) {
        statement->execute();
    }
}

std::string ForStatement::toString() const {
    return "for " + initialization->toString() + "; " + termination->toString() + "; " + increment->toString() + statement->toString();
}



