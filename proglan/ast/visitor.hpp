#include "statement.hpp"


class Visitor {
public:
    virtual ~Visitor() = default;                 // Виртуальный деструктор
    virtual void visit(Statement st) = 0;
    virtual void visit(FunctionDefineStatement st) = 0;
    //virtual void visit() = 0;
};