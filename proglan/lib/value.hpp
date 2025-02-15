#pragma once

#include <memory>
#include <string>
#include <vector>

// Абстрактный базовый класс Value, представляющий любое значение
class Value {
public:
    virtual ~Value() = default;               // Виртуальный деструктор для правильного удаления объектов производных классов
    virtual double asNumber() const = 0;      // Чисто виртуальная функция для получения числового значения
    virtual std::string asString() const = 0; // Чисто виртуальная функция для получения строкового значения
    virtual std::string toString() const = 0; // Получить строковое представление значения
};

// Класс для представления числовых значений
class NumberValue : public Value {
private:
    double value; // Числовое значение

public:
    explicit NumberValue(double value);        // Конструктор с числовым дробным значением
    explicit NumberValue(int value);          // Конструктор с числовым целым значением
    explicit NumberValue(bool value);          // Конструктор с булевым значением


    double asNumber() const override;          // Возвращает число
    std::string asString() const override;     // Возвращает строковое представление числа
    std::string toString() const override;     // Возвращает строковое представление числа
};

// Класс для представления строковых значений
class StringValue : public Value {
private:
    std::string value; // Строковое значение

public:
    explicit StringValue(std::string value);   // Конструктор с строкой

    double asNumber() const override;          // Преобразует строку в число
    std::string asString() const override;     // Возвращает строковое представление строки
    std::string toString() const override;     // Возвращает строковое представление строки
};


class ArrayValue : public Value {
private:
    std::vector<std::shared_ptr<Value>> elements;


public:
    explicit ArrayValue(size_t size);
    explicit ArrayValue(const std::vector<std::shared_ptr<Value>>& newElements);
    explicit ArrayValue(ArrayValue const &newArray);
    double asNumber() const override;          // Преобразует строку в число
    std::string asString() const override;     // Возвращает строковое представление строки
    std::string toString() const override;     // Возвращает строковое представление строки
    std::shared_ptr<Value> get(size_t index) const;
    void set(size_t index, std::shared_ptr<Value> value);
};