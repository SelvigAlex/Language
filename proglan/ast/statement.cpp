#include "statement.hpp"
#include <iostream>
#include <memory>
#include "/home/alexs/reverse/compiler/lib/value.hpp"
#include "/home/alexs/reverse/compiler/lib/function.hpp"


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

EchoStatement::EchoStatement(std::shared_ptr<Expression> expression)
    : expression(std::move(expression)) {}

void EchoStatement::execute() {
    // Печатаем строковое представление результата выражения
    std::cout << expression->eval()->asString();
}

std::string EchoStatement::toString() const {
    return "echo " + expression->toString();
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
        try {
            statement->execute();
        } catch (BreakStatement* bs) {
            break;
        } catch (ContinueStatement* cn) {
            continue;
        }
    }
}

std::string WhileStatement::toString() const {
    return "while " + condition->toString() + " " + statement->toString();
}

void DoWhileStatement::execute() {
    do {
        try {
            statement->execute();
        } catch (BreakStatement* bs) {
            break;
        } catch (ContinueStatement* cn) {
            continue;
        }
    } while (condition->eval()->asNumber());
}

std::string DoWhileStatement::toString() const {
    return "do " + statement->toString() + " while " + condition->toString();
}


DoWhileStatement::DoWhileStatement(std::shared_ptr<Expression> condition, std::shared_ptr<Statement> statement)
    : condition(std::move(condition)), statement(std::move(statement)) {}




ForStatement::ForStatement(std::shared_ptr<Statement> initialization, std::shared_ptr<Expression> termination, std::shared_ptr<Statement> increment, std::shared_ptr<Statement> statement)
    : initialization(std::move(initialization)), termination(std::move(termination)), increment(std::move(increment)), statement(std::move(statement)) {}

void ForStatement::execute() {
    for (initialization->execute(); termination->eval()->asNumber(); increment->execute()) {
        try {
            statement->execute();
        } catch (BreakStatement* bs) {
            break;
        } catch (ContinueStatement* cn) {
            continue;
        }
    }
}


std::string ForStatement::toString() const {
    return "for " + initialization->toString() + "; " + termination->toString() + "; " + increment->toString() + statement->toString();
}

void BreakStatement::execute() {
    throw this;
}

std::string BreakStatement::toString() const {
    return "break";
}

void ContinueStatement::execute() {
    throw this;
}

std::string ContinueStatement::toString() const {
    return "continue";
}

FunctionStatement::FunctionStatement(std::shared_ptr<FunctionalExpression> function)
    : function(std::move(function)) {}

void FunctionStatement::execute() {
    function->eval();
}

std::string FunctionStatement::toString() const {
    return function->toString();
}

FunctionDefineStatement::FunctionDefineStatement(const std::string& name, const std::vector<std::string>& argNames, std::shared_ptr<Statement> body)
    : name(name), argNames(argNames), body(std::move(body)) {}

void FunctionDefineStatement::execute() {
    Functions::set(name, std::make_shared<UserDefineFunction>(argNames, body));
}

std::string FunctionDefineStatement::toString() const {
    std::string result;
    for (const auto& name : argNames) {
        result += name;
    }
    return "function (" + result + ")" + body->toString();
}

ReturnStatement::ReturnStatement(std::shared_ptr<Expression> expression)
    : expression(std::move(expression)) {}

void ReturnStatement::execute() {
    result = expression->eval();
    throw this;
}

std::shared_ptr<Value> ReturnStatement::getResult() {
    return result;
}

std::string ReturnStatement::toString() const {
    return "return";
}

ArrayAssigmentStatement::ArrayAssigmentStatement(std::shared_ptr<ArrayAccessExpression> array, std::shared_ptr<Expression> expression)
    : array(std::move(array)), expression(std::move(expression)) {}


//! compute array[1][2][3] = ...

void ArrayAssigmentStatement::execute() {
    array->getArray()->set(array->lastIndex(), expression->eval());
}

std::string ArrayAssigmentStatement::toString() const {
    std::string result;
    return array->toString() + " = " + expression->toString();
}




