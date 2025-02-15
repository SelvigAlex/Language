#include "value.hpp"
#include <memory>
#include <stdexcept>
#include <vector>
#include <string>

// Реализация методов класса NumberValue

NumberValue::NumberValue(double value) : value(value) {}

NumberValue::NumberValue(int value) : value(value) {}

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

ArrayValue::ArrayValue(size_t size) {
    elements.resize(size);
}

ArrayValue::ArrayValue(const std::vector<std::shared_ptr<Value>>& newElements) {
    this->elements = newElements;
}

ArrayValue::ArrayValue(ArrayValue const& newArray) {
    this->elements = newArray.elements;
}

double ArrayValue::asNumber() const {
    throw std::runtime_error("Cannot cast array to number");
}

std::string ArrayValue::asString() const {
    std::string result = "[ ";
    for (const auto& elem : elements) {
        result += elem->toString() + " ";
    }
    return result + "]";
}

std::string ArrayValue::toString() const {
    return asString();
}

std::shared_ptr<Value> ArrayValue::get(size_t index) const {
    if (index >= elements.size()) throw std::runtime_error("Out-of-range array");
    return elements[index];
}

void ArrayValue::set(size_t index, std::shared_ptr<Value> value) {
    if (index >= elements.size()) throw std::runtime_error("Out-of-range array");
    elements[index] = value;
}
