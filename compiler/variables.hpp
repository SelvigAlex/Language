#pragma once
#include <iostream>
#include <unordered_map>
#include <string>
#include "value.hpp"

class Variables {
public:
    // Проверяет, существует ли переменная с данным ключом
    static bool isExists(const std::string& key) {
        return variables.find(key) != variables.end();
    }

    // Возвращает значение переменной по ключу, или 0, если переменная не существует
    static std::shared_ptr<Value> get(const std::string& key) {
        auto it = variables.find(key);
        if (it != variables.end()) {
            return it->second;
        }
        // Выбрасываем исключение, если переменная не найдена
        throw std::runtime_error("Variable '" + key + "' not found");
    }

    // Устанавливает значение переменной
    static void set(const std::string& key, std::shared_ptr<Value> value) {
        variables[key] = value;
    }

    // Удаляет переменную
    static void remove(const std::string& key) {
        variables.erase(key);
    }

    // //Инициализация переменных по умолчанию
    // static void initializeDefaults() {
    //     variables.insert({"PI", std::make_shared<NumberValue>(3.14159)});
    //     set("E", std::make_shared<NumberValue>(2.71828));
    // }

private:
    // Контейнер для хранения переменных
    static std::unordered_map<std::string, std::shared_ptr<Value>> variables;
};

// Инициализация статической переменной
std::unordered_map<std::string, std::shared_ptr<Value>> Variables::variables = {
    {"PI", std::make_shared<NumberValue>(3.14159265358979323846)},
    {"E", std::make_shared<NumberValue>(2.7182818284590452354)},
};