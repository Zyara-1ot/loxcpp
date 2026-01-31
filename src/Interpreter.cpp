#include "Interpreter.h"
#include "TokenType.h"
#include <stdexcept>
#include <memory>
#include "RuntimeError.h"

void Interpreter::interpret(const std::shared_ptr<Expr>& expr) {
    try {
        Value value = expr->accept(*this);
        std::cout << stringify(value) << std::endl;
    } catch (const RuntimeError& error) {
    std::cerr << error.what() << std::endl;
}
}

Value Interpreter::visitUnary(const Unary& expr) {
    Value right = expr.right->accept(*this);

    if (expr.op.type == TokenType::MINUS) {
        checkNumberOperand(expr.op, right);
        return -std::get<double>(right);
    }

    if (expr.op.type == TokenType::BANG) {
        return !isTruthy(right);
    }

    return std::monostate{};
}


Value Interpreter::visitBinary(const Binary& expr) {
    Value left = expr.left->accept(*this);
    Value right = expr.right->accept(*this);

    switch (expr.op.type) {
        case TokenType::PLUS:
            if (std::holds_alternative<double>(left) &&
                std::holds_alternative<double>(right))
                return std::get<double>(left) + std::get<double>(right);

            if (std::holds_alternative<std::string>(left) &&
                std::holds_alternative<std::string>(right))
                return std::get<std::string>(left) + std::get<std::string>(right);

            throw RuntimeError(expr.op,
                "Operands must be two numbers or two strings.");

        case TokenType::MINUS:
            checkNumberOperands(expr.op, left, right);
            return std::get<double>(left) - std::get<double>(right);

        case TokenType::STAR:
            checkNumberOperands(expr.op, left, right);
            return std::get<double>(left) * std::get<double>(right);

        case TokenType::SLASH:
            checkNumberOperands(expr.op, left, right);
            return std::get<double>(left) / std::get<double>(right);

        default:
            return std::monostate{};
    }
}

Value Interpreter::visitLiteral(const Literal& expr) {
    return expr.value;
}

Value Interpreter::visitGrouping(const Grouping& expr) {
    return expr.expression->accept(*this);
}


void Interpreter::checkNumberOperand(const Token& op, const Value& value) {
    if (std::holds_alternative<double>(value)) return;
    throw RuntimeError(op, "Operand must be a number.");
}

void Interpreter::checkNumberOperands(const Token& op,
                                      const Value& left,
                                      const Value& right) {
    if (std::holds_alternative<double>(left) &&
        std::holds_alternative<double>(right)) return;
    throw RuntimeError(op, "Operands must be numbers.");
}

bool Interpreter::isTruthy(const Value& value) {
    if (std::holds_alternative<std::monostate>(value)) return false;
    if (std::holds_alternative<bool>(value)) return std::get<bool>(value);
    return true;
}

std::string Interpreter::stringify(const Value& value) {
    if (std::holds_alternative<std::monostate>(value)) return "nil";

    if (std::holds_alternative<double>(value)) {
        std::string text = std::to_string(std::get<double>(value));
        if (text.size() >= 7 && text.substr(text.size() - 7) == ".000000")
            text.erase(text.size() - 7);
        return text;
    }

    if (std::holds_alternative<bool>(value))
        return std::get<bool>(value) ? "true" : "false";

    return std::get<std::string>(value);
}
