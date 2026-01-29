#pragma once

template<typename R>
struct ExprVisitor {
    virtual R visitBinary(const struct Binary&) = 0;
    virtual R visitUnary(const struct Unary&) = 0;
    virtual R visitLiteral(const struct Literal&) = 0;
    virtual R visitGrouping(const struct Grouping&) = 0;
    virtual ~ExprVisitor() = default;
};
