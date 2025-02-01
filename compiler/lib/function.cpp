#include "function.hpp"
#include <stdexcept>
#include <unordered_map>
#include <cmath>
#include <stdexcept>
#include <string>
#include <vector>
#include <iostream>


std::unordered_map<std::string, std::shared_ptr<Function>> Functions::functions = {
    {"sin", std::make_shared<FunctionSin>(FunctionSin())},
    {"cos", std::make_shared<FunctionCos>(FunctionCos())},
    {"println", std::make_shared<FunctionPrintLn>(FunctionPrintLn())},
    {"print", std::make_shared<FunctionPrint>(FunctionPrint())},

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
