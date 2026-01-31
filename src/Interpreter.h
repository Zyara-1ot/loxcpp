#pragma once
#include "Expr.h"
#include "RuntimeError.h"

class Interpreter : public ExprVisitor<Value> {
public:
    void interpret(const std::shared_ptr<Expr>& expr);

    Value visitBinary(const Binary&) override;
    Value visitUnary(const Unary&) override;
    Value visitLiteral(const Literal&) override;
    Value visitGrouping(const Grouping&) override;

private:
    bool isTruthy(const Value&);
    std::string stringify(const Value&);

    void checkNumberOperand(const Token&, const Value&);
    void checkNumberOperands(const Token&, const Value&, const Value&);
};
