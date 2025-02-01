#include "expression.hpp"
#include <memory>
#include <stdexcept>
#include "/home/alexs/reverse/compiler/lib/variables.hpp"
#include "/home/alexs/reverse/compiler/lib/function.hpp"

ValueExpression::ValueExpression(double value)
    : value(std::make_shared<NumberValue>(value)) {}

ValueExpression::ValueExpression(std::string value)
    : value(std::make_shared<StringValue>(value)) {}

std::shared_ptr<Value> ValueExpression::eval() const {
    return value;
}

std::string ValueExpression::toString() const {
    return value->asString();
}

BinaryExpression::BinaryExpression(char operation, std::shared_ptr<Expression> expr1, std::shared_ptr<Expression> expr2)
    : operation(operation), expr1(std::move(expr1)), expr2(std::move(expr2)) {}

std::shared_ptr<Value> BinaryExpression::eval() const {
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
    case '-':
        return std::make_shared<NumberValue>(number1 - number2);
    case '*':
        return std::make_shared<NumberValue>(number1 * number2);
    case '/':
        return std::make_shared<NumberValue>(number1 / number2);
    case '+':
    default:
        return std::make_shared<NumberValue>(number1 + number2);
    }
}

std::string BinaryExpression::toString() const {
    return "(" + expr1->toString() + " " + operation + " " + expr2->toString() + ")";
}

ConditionalExpression::ConditionalExpression(Operator operation, std::shared_ptr<Expression> expr1, std::shared_ptr<Expression> expr2)
    : operation(operation), expr1(std::move(expr1)), expr2(std::move(expr2)) {
    operatorNames = {
        {Operator::PLUS, "+"},
        {Operator::MINUS, "-"},
        {Operator::MULTIPLY, "*"},
        {Operator::DIVIDE, "/"},
        {Operator::EQUALS, "=="},
        {Operator::NOT_EQUALS, "!="},
        {Operator::LT, "<"},
        {Operator::LTEQ, "<="},
        {Operator::GT, ">"},
        {Operator::GTEQ, ">="},
        {Operator::AND, "&&"},
        {Operator::OR, "||"} };
}

std::shared_ptr<Value> ConditionalExpression::eval() const {
    std::shared_ptr<Value> value1 = expr1->eval();
    std::shared_ptr<Value> value2 = expr2->eval();

    double number1, number2;
    if (dynamic_cast<StringValue*>(value1.get())) {
        number1 = value1->asString().compare(value2->asString());
        number2 = 0;
    } else {
        number1 = value1->asNumber();
        number2 = value2->asNumber();
    }

    bool result;
    switch (operation) {
    case LT:
        result = number1 < number2;
        break;
    case LTEQ:
        result = number1 <= number2;
        break;
    case GT:
        result = number1 > number2;
        break;
    case GTEQ:
        result = number1 >= number2;
        break;
    case NOT_EQUALS:
        result = number1 != number2;
        break;

    case AND:
        result = (number1 != 0) && (number2 != 0);
        break;
    case OR:
        result = (number1 != 0) || (number2 != 0);
        break;

    case EQUALS:
    default:
        result = number1 == number2;
        break;
    }
    return std::make_shared<NumberValue>(result);
}

std::string ConditionalExpression::asString(Operator op) {
    return operatorNames[op];
}

std::string ConditionalExpression::toString() const {
    return "(" + expr1->toString() + " " + expr2->toString() + ")";
}

UnaryExpression::UnaryExpression(char operation, std::shared_ptr<Expression> expr1)
    : operation(operation), expr1(std::move(expr1)) {}

std::shared_ptr<Value> UnaryExpression::eval() const {
    switch (operation) {
    case '-':
        return std::make_shared<NumberValue>(-expr1->eval()->asNumber()); // Отрицание
    case '+':
    default:
        return std::make_shared<NumberValue>(expr1->eval()->asNumber()); // Положительное значение
    }
}

std::string UnaryExpression::toString() const {
    return std::string(1, operation) + " " + expr1->toString(); // Форматирование строки
}

VariableExpression::VariableExpression(std::string name) : name(std::move(name)) {}

std::shared_ptr<Value> VariableExpression::eval() const {
    if (!Variables::isExists(name)) {
        throw std::runtime_error("Constant '" + name + "' does not exist");
    }
    return Variables::get(name);
}

std::string VariableExpression::toString() const {
    return name; // Форматирование строки
}

FunctionalExpression::FunctionalExpression(std::string name, std::vector<std::shared_ptr<Expression>> arguments) 
    : name(name), arguments(std::move(arguments)) {}


FunctionalExpression::FunctionalExpression(std::string name) 
    : name(name), arguments(std::vector<std::shared_ptr<Expression>>()) {}


std::shared_ptr<Value> FunctionalExpression::eval() const {
    std::vector<std::shared_ptr<Value>> values(arguments.size());
    for (size_t i = 0; i < arguments.size(); ++i) {
        values[i] = arguments[i]->eval();
    } 
    //return Functions::get(name)->execute(values);
    return Functions::get(name)->execute(values);
}

std::string FunctionalExpression::toString() const {
    std::string result;
    for (const auto& arg : arguments) {
        result += arg->toString() + ", ";
    }
    return name +  "(" + result + ")"; 
}

void FunctionalExpression::addArgument(std::shared_ptr<Expression> arg) {
    arguments.push_back(arg);
}
