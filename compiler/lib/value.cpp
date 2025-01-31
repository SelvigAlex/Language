#include "value.hpp"
#include <stdexcept>

// Реализация методов класса NumberValue

NumberValue::NumberValue(double value) : value(value) {}

NumberValue::NumberValue(bool value) {
    this->value = value ? 1 : 0;
}

double NumberValue::asNumber() const {
    return value; // Преобразуем в число и возвращаем
}

std::string NumberValue::asString() const {
    return std::to_string(value); // Строковое представление числа
}

std::string NumberValue::toString() const {
    return asString(); // Используем `asString` для строкового представления
}

// Реализация методов класса StringValue

StringValue::StringValue(std::string value) : value(std::move(value)) {}

double StringValue::asNumber() const {
    // Преобразуем строку в число, если возможно
    try {
        return std::stod(value); // Преобразуем строку в число
    } catch (const std::invalid_argument&) {
        // Если не удалось преобразовать строку в число, возвращаем 0.0
        return 0.0;
    } catch (const std::out_of_range&) {
        // Если число слишком большое для представления, возвращаем 0.0
        return 0.0;
    }
}

std::string StringValue::asString() const {
    return value; // Строковое представление
}

std::string StringValue::toString() const {
    return asString(); // Используем `asString` для строкового представления
}