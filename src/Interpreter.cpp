#include "Interpreter.h"
#include "TokenType.h"
#include <stdexcept>
#include <memory>

Value Interpreter::interpret(const std::shared_ptr<Expr>& expr) {
    return evaluate(expr);
}

Value Interpreter::evaluate(const std::shared_ptr<Expr>& expr) {

    if (auto lit = std::dynamic_pointer_cast<Literal>(expr)) {
        return lit->value;
    }

    if (auto grp = std::dynamic_pointer_cast<Grouping>(expr)) {
        return evaluate(grp->expression);
    }

    if (auto un = std::dynamic_pointer_cast<Unary>(expr)) {
        Value right = evaluate(un->right);

        if (un->op.type == TokenType::MINUS) {
            return -std::get<double>(right);
        }

        if (un->op.type == TokenType::BANG) {
            return !std::get<bool>(right);
        }
    }

    if (auto bin = std::dynamic_pointer_cast<Binary>(expr)) {
        Value left = evaluate(bin->left);
        Value right = evaluate(bin->right);

        switch (bin->op.type) {
            case TokenType::PLUS:
                return std::get<double>(left) + std::get<double>(right);
            case TokenType::MINUS:
                return std::get<double>(left) - std::get<double>(right);
            case TokenType::STAR:
                return std::get<double>(left) * std::get<double>(right);
            case TokenType::SLASH:
                return std::get<double>(left) / std::get<double>(right);
            case TokenType::EQUAL_EQUAL:
                return left == right;
            case TokenType::BANG_EQUAL:
                return left != right;
            default:
                break;
        }
    }

    throw std::runtime_error("Invalid expression.");
}
