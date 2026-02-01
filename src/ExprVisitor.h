#pragma once
struct Assign;
struct Binary;
struct Unary;
struct Literal;
struct Grouping;
struct Variable;

template<typename R>
struct ExprVisitor {
    virtual R visitBinary(const Binary&) = 0;
    virtual R visitUnary(const Unary&) = 0;
    virtual R visitLiteral(const Literal&) = 0;
    virtual R visitGrouping(const Grouping&) = 0;
    virtual R visitVariable(const Variable&) = 0;
    virtual R visitAssign(const Assign&) = 0;
};