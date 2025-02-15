#pragma once

#include <unordered_map>
#include <string>
#include "value.hpp"
#include <memory>


class Variables {
public:
    // Проверяет, существует ли переменная с данным ключом
    static bool isExists(const std::string& key);

    // Возвращает значение переменной по ключу, или 0, если переменная не существует
    static std::shared_ptr<Value> get(const std::string& key);

    // Устанавливает значение переменной
    static void set(const std::string& key, std::shared_ptr<Value> value);

    // Удаляет переменную
    static void remove(const std::string& key);

private:
    // Контейнер для хранения переменных
    static std::unordered_map<std::string, std::shared_ptr<Value>> variables;
};
