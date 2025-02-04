#include "function.hpp"
#include <memory>
#include <stdexcept>
#include <unordered_map>
#include <cmath>
#include <stdexcept>
#include <string>
#include <vector>
#include <iostream>




UserDefineFunction::UserDefineFunction(const std::vector<std::string>& argNames, std::shared_ptr<Statement> body)
    : argNames(argNames), body(std::move(body)) {}

std::shared_ptr<Value> UserDefineFunction::execute(const std::vector<std::shared_ptr<Value>>& args) const {
    double a = args.size() - args.size();
    try {
        body->execute();
        return std::make_shared<NumberValue>(a);
    } catch (ReturnStatement* rt) {
        return rt->getResult();
    }
}

size_t UserDefineFunction::getArgsCount() const {
    return argNames.size();
}

std::string UserDefineFunction::getAgrsName(size_t index) const {
    if (index > argNames.size()) {
        return "";
    }
    return argNames[index];
}


std::unordered_map<std::string, std::shared_ptr<Function>> Functions::functions = {
    {"sin", std::make_shared<FunctionSin>(FunctionSin())},
    {"cos", std::make_shared<FunctionCos>(FunctionCos())},
    {"println", std::make_shared<FunctionPrintLn>(FunctionPrintLn())},
    {"print", std::make_shared<FunctionPrint>(FunctionPrint())},
    {"createArray", std::make_shared<FunctionCreateArray>(FunctionCreateArray())},
};

std::shared_ptr<Function> Functions::get(const std::string& key) {
    auto it = functions.find(key);
    if (it != functions.end()) {
        return it->second;
    }
    // Выбрасываем исключение, если переменная не найдена
    throw std::runtime_error("Unknown function " + key);
}

bool Functions::isExists(const std::string& key) {
    return functions.find(key) != functions.end();
}

void Functions::set(const std::string& key, std::shared_ptr<Function> value) {
    functions[key] = value;
}

std::shared_ptr<Value> FunctionSin::execute(const std::vector<std::shared_ptr<Value>>& args) const {
    if (args.size() != 1) {
        throw std::runtime_error("One argument expected for sin");
    }
    return std::make_shared<NumberValue>(std::sin(args[0]->asNumber()));
}
// FunctionCos implementation
std::shared_ptr<Value> FunctionCos::execute(const std::vector<std::shared_ptr<Value>>& args) const {
    if (args.size() != 1) {
        throw std::runtime_error("One argument expected for cos");
    }
    return std::make_shared<NumberValue>(std::cos(args[0]->asNumber()));
}

// FunctionPrintLn implementation
std::shared_ptr<Value> FunctionPrintLn::execute(const std::vector<std::shared_ptr<Value>>& args) const {
    for (const auto& arg : args) {
        std::cout << arg->asString() << '\n';
    }
    return std::make_shared<NumberValue>(0.);
}

std::shared_ptr<Value> FunctionPrint::execute(const std::vector<std::shared_ptr<Value>>& args) const {
    for (const auto& arg : args) {
        std::cout << arg->asString() << ' ';
    }
    return std::make_shared<NumberValue>(0.);
}

std::shared_ptr<ArrayValue> FunctionCreateArray::createArray(const std::vector<std::shared_ptr<Value>>& args, size_t index) const {
    size_t size = (size_t)args[index]->asNumber();
    size_t last = args.size() - 1;
    std::shared_ptr<ArrayValue> array = std::make_shared<ArrayValue>(size);
    if (index == last) {
        for (size_t i = 0; i < size; i++) {
            array->set(i, std::make_shared<NumberValue>(0.));
        }
    } else if (index < last) {
        for (size_t i = 0; i < size; i++) {
            array->set(i, createArray(args, index + 1));
        }
    }
    return array;
}

std::shared_ptr<Value> FunctionCreateArray::execute(const std::vector<std::shared_ptr<Value>>& args) const {
    return createArray(args, 0);
}
