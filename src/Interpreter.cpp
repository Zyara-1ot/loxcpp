#include <iostream>
#include "Interpreter.h"
#include "TokenType.h"
#include <stdexcept>
#include <memory>
#include <vector>
#include "RuntimeError.h"

//void Interpreter::interpret(const std::shared_ptr<Expr>& expr) {
  //  try {
    //    Value value = expr->accept(*this);
    //    std::cout << stringify(value) << std::endl;
    //} catch (const RuntimeError& error) {
    //std::cerr << error.what() << std::endl;
//}
//}

void Interpreter::interpret(const std::vector<std::shared_ptr<Stmt>>& statements) {
    for (const auto& stmt : statements) {
    execute(stmt);
}
}

void Interpreter::execute(const std::shared_ptr<Stmt>& stmt) {
    stmt->accept(*this);
}

Value Interpreter::evaluate(const std::shared_ptr<Expr>& expr) {
    return expr->accept(*this);
}
void Interpreter::visitExpressionStmt(const ExpressionStmt& stmt) {
    evaluate(stmt.expression);
}

void Interpreter::visitPrintStmt(const PrintStmt& stmt) {
    Value value = evaluate(stmt.expression);
    std::cout << stringify(value) << std::endl;
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

        case TokenType::GREATER:
            checkNumberOperands(expr.op, left, right);
            return std::get<double>(left) > std::get<double>(right);

        case TokenType::GREATER_EQUAL:
            checkNumberOperands(expr.op, left, right);
            return std::get<double>(left) >= std::get<double>(right);

        case TokenType::LESS:
            checkNumberOperands(expr.op, left, right);
            return std::get<double>(left) < std::get<double>(right);

        case TokenType::LESS_EQUAL:
            checkNumberOperands(expr.op, left, right);
            return std::get<double>(left) <= std::get<double>(right);

        case TokenType::EQUAL_EQUAL:
            return isEqual(left, right);

        case TokenType::BANG_EQUAL:
            return !isEqual(left, right);

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

bool Interpreter::isEqual(const Value& left, const Value& right) {
    if (std::holds_alternative<std::monostate>(left) && 
        std::holds_alternative<std::monostate>(right)) return true;
    if (std::holds_alternative<std::monostate>(left)) return false;
    
    if (left.index() != right.index()) return false;
    
    if (std::holds_alternative<double>(left))
        return std::get<double>(left) == std::get<double>(right);
    if (std::holds_alternative<bool>(left))
        return std::get<bool>(left) == std::get<bool>(right);
    if (std::holds_alternative<std::string>(left))
        return std::get<std::string>(left) == std::get<std::string>(right);
    
    return false;
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

void Interpreter::visitVarStmt(const VarStmt& stmt) {
    Value value = std::monostate{};
    if (stmt.initializer) {
        value = evaluate(stmt.initializer);
    }
    environment->define(stmt.name.lexeme, value);
}

Value Interpreter::visitVariable(const Variable& expr) {
    return environment->get(expr.name);
}

Value Interpreter::visitAssign(const Assign& expr) {
    Value value = evaluate(expr.value);
    environment->assign(expr.name, value);
    return value;
}

void Interpreter::executeBlock(
    const std::vector<std::shared_ptr<Stmt>>& statements,
    std::shared_ptr<Environment> env
) {
    auto previous = environment;
    try {
        environment = env;
        for (const auto& stmt : statements) {
            execute(stmt);
        }
    } catch (...) {
        environment = previous;
        throw;
    }
    environment = previous;
}

void Interpreter::visitBlockStmt(const BlockStmt& stmt) {
    executeBlock(
        stmt.statements,
        std::make_shared<Environment>(environment)
    );
}

Interpreter::Interpreter() {
    environment = std::make_shared<Environment>();
}

void Interpreter::visitIfStmt(const IfStmt& stmt){
    auto conditionValue = evaluate(stmt.condition);
    bool truthiness = isTruthy(conditionValue);
    
    if(truthiness){
        execute(stmt.thenBranch);
    } else if(stmt.elseBranch){
        execute(stmt.elseBranch);
    }
}

