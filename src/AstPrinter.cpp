#include "AstPrinter.h"
#include "Expr.h"
#include "Value.h"
#include <string>

std::string AstPrinter::print(const std::shared_ptr<Expr>& expr) {
    return expr->acceptPrinter(*this);
}

std::string AstPrinter::visitBinary(const Binary& expr) {
    return "(" + expr.op.lexeme + " " +
           expr.left->acceptPrinter(*this) + " " +
           expr.right->acceptPrinter(*this) + ")";
}

std::string AstPrinter::visitUnary(const Unary& expr) {
    return "(" + expr.op.lexeme + " " +
           expr.right->acceptPrinter(*this) + ")";
}

std::string AstPrinter::visitLiteral(const Literal& expr) {
    if (std::holds_alternative<std::monostate>(expr.value)) return "nil";
    if (std::holds_alternative<double>(expr.value)) return std::to_string(std::get<double>(expr.value));
    if (std::holds_alternative<std::string>(expr.value)) return std::get<std::string>(expr.value);
    if (std::holds_alternative<bool>(expr.value)) return std::get<bool>(expr.value) ? "true" : "false";
    return "nil";
}

std::string AstPrinter::visitGrouping(const Grouping& expr) {
    return "(group " + expr.expression->acceptPrinter(*this) + ")";
}
