#include "value.hpp"
#include <memory>
#include <vector>
#include <unordered_map>
#include "/home/alexs/reverse/compiler/ast/statement.hpp"

class Function {
public:
    virtual ~Function() = default;                 // Виртуальный деструктор
    virtual std::shared_ptr<Value> execute(const std::vector<std::shared_ptr<Value>>& args) const = 0; // Чисто виртуальная функция для вычисления значения
};

class UserDefineFunction : public Function {
private:
    std::vector<std::string> argNames;
    std::shared_ptr<Statement> body;


public:
    explicit UserDefineFunction(const std::vector<std::string>& argNames, std::shared_ptr<Statement> body);
    std::shared_ptr<Value> execute(const std::vector<std::shared_ptr<Value>>& args) const;
    size_t getArgsCount() const;
    std::string getAgrsName(size_t index) const;
};

class FunctionSin : public Function {
public:
    explicit FunctionSin() = default;
    std::shared_ptr<Value> execute(const std::vector<std::shared_ptr<Value>>& args) const override;
};

class FunctionCos : public Function {
public:
    explicit FunctionCos() = default;

    std::shared_ptr<Value> execute(const std::vector<std::shared_ptr<Value>>& args) const override;
};

class FunctionPrintLn : public Function {
public:
    explicit FunctionPrintLn() = default;
    std::shared_ptr<Value> execute(const std::vector<std::shared_ptr<Value>>& args) const override;
};

class FunctionPrint : public Function {
public:
    explicit FunctionPrint() = default;
    std::shared_ptr<Value> execute(const std::vector<std::shared_ptr<Value>>& args) const override;
};


class Functions {
public:
    // Проверяет, существует ли переменная с данным ключом
    static bool isExists(const std::string& key);
    // Возвращает значение переменной по ключу, или 0, если переменная не существует
    static std::shared_ptr<Function> get(const std::string& key);
    // Устанавливает значение переменной
    static void set(const std::string& key, std::shared_ptr<Function> value);

private:
    // Контейнер для хранения переменных
    static std::unordered_map<std::string, std::shared_ptr<Function>> functions;

    
};

