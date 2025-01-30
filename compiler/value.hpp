#pragma once

#include <string>
#include <sstream>
#include <stdexcept>
#include <memory>

class Value {
public:
    virtual ~Value() = default;  // Виртуальный деструктор для правильного удаления объектов производных классов
    virtual double asNumber() const = 0;  // Чисто виртуальная функция для получения числового значения
    virtual std::string asString() const = 0;  // Чисто виртуальная функция для получения строкового значения
    virtual std::string toString() const = 0;  // Получить строковое представление значения
};

class NumberValue : public Value {
private:
    double value;  // Числовое значение
    int number;

public:
    explicit NumberValue(double value) : value(value) {}
    explicit NumberValue(bool value) {
        this->value = value ? 1 : 0;
    }

    double asNumber() const override {
        return value;  // Преобразуем в число и возвращаем
    }

    std::string asString() const override {
        return std::to_string(value);  // Строковое представление числа
    }

    std::string toString() const override {
        return asString();  // Используем `asString` для строкового представления
    }
};

class StringValue : public Value {
private:
    std::string value;  // Строковое значение

public:
    explicit StringValue(std::string value) : value(std::move(value)) {}

    double asNumber() const override {
        // Преобразуем строку в число, если возможно
        try {
            return std::stod(value);  // Преобразуем строку в число
        } catch (const std::invalid_argument&) {
            // Если не удалось преобразовать строку в число, возвращаем 0.0
            return 0.0;
        } catch (const std::out_of_range&) {
            // Если число слишком большое для представления, возвращаем 0.0
            return 0.0;
        }
    }

    std::string asString() const override {
        return value;  // Строковое представление
    }

    std::string toString() const override {
        return asString();  // Используем `asString` для строкового представления
    }
    
};
