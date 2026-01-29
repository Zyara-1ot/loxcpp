#pragma once
#include <memory>
#include "Expr.h"

class Interpreter {
public:
    Value interpret(const std::shared_ptr<Expr>& expr);

private:
    Value evaluate(const std::shared_ptr<Expr>& expr);
};