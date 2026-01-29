#include "AstPrinter.h"
#include "Expr.h"
#include <variant>
#include <string>

std::string AstPrinter::print(const std::shared_ptr<Expr>& expr) {
    return expr->accept(*this);
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
    return expr.toString();
}

std::string AstPrinter::visitGrouping(const Grouping& expr) {
    return "(group " + expr.expression->accept(*this) + ")";
}
