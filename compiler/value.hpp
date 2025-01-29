#include <string>
#include <sstream>

class Value {
public:
    virtual ~Value() = default; // Виртуальный деструктор для правильного удаления объектов производных классов

    virtual double asNumber() const = 0; // Чисто виртуальная функция для получения числового значения
    virtual std::string asString() const = 0; // Чисто виртуальная функция для получения строкового значения
    virtual std::string toString() const = 0;
};


class NumberValue : public Value {
private:
    double value;

public:
    explicit NumberValue(double value) : value(value) {}

    double asNumber() const override {
        return value;
    }

    std::string asString() const override {
        return std::to_string(value);
    }

    std::string toString() const override {
        return asString();
    }
};


class StringValue : public Value {
private:
    std::string value;

public:
    explicit StringValue(const std::string& value) : value(value) {}

    double asNumber() const override {
        try {
            std::stringstream ss(value);
            double result;
            ss >> result;
            if (ss.fail()) {
                return 0.0;
            }
            return result;
        } catch (...) {
            return 0.0;
        }
    }

    std::string asString() const override {
        return value;
    }

    std::string toString() const override {
        return asString();
    }
};