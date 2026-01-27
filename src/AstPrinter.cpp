#include "AstPrinter.h"
#include "Expr.h"
#include <variant>
#include <string>

static std::string parenthesize(
    const std::string& name,
    const std::shared_ptr<Expr>& left,
    const std::shared_ptr<Expr>& right,
    ExprVisitor& visitor
) {
    return "(" + name + " " +
           left->accept(visitor) + " " +
           right->accept(visitor) + ")";
}


std::string AstPrinter::visitBinary(const Binary& expr) {
    return "(" + expr.op.lexeme + " " +
           expr.left->accept(*this) + " " +
           expr.right->accept(*this) + ")";
}

std::string AstPrinter::visitUnary(const Unary& expr) {
    return "(" + expr.op.lexeme + " " +
           expr.right->accept(*this) + ")";
}

std::string AstPrinter::visitLiteral(const Literal& expr) {
    if (std::holds_alternative<double>(expr.value)) {
        return std::to_string(std::get<double>(expr.value));
    }
    else if (std::holds_alternative<std::string>(expr.value)) {
        return std::get<std::string>(expr.value);
    }
    else if (std::holds_alternative<bool>(expr.value)) {
        return std::get<bool>(expr.value) ? "true" : "false";
    }
    return "nil";
}

std::string AstPrinter::visitGrouping(const Grouping& expr) {
    return "(group " + expr.expression->accept(*this) + ")";
}
