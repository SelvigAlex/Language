#include "variables.hpp"
#include <stdexcept>

// Инициализация статической переменной
std::unordered_map<std::string, std::shared_ptr<Value>> Variables::variables = {
    {"PI", std::make_shared<NumberValue>(3.14159265358979323846)},
    {"E", std::make_shared<NumberValue>(2.7182818284590452354)},
};

bool Variables::isExists(const std::string& key) {
    return variables.find(key) != variables.end();
}

std::shared_ptr<Value> Variables::get(const std::string& key) {
    auto it = variables.find(key);
    if (it != variables.end()) {
        return it->second;
    }
    // Выбрасываем исключение, если переменная не найдена
    throw std::runtime_error("Variable '" + key + "' not found");
}

void Variables::set(const std::string& key, std::shared_ptr<Value> value) {
    variables[key] = value;
}

void Variables::remove(const std::string& key) {
    variables.erase(key);
}
