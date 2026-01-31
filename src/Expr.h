#pragma once
#include <memory>
#include <variant>
#include <string>
#include "Token.h"

using Value = std::variant<std::monostate, double, std::string, bool>;

struct Binary;
struct Unary;
struct Literal;
struct Grouping;

template<typename R>
struct ExprVisitor {
    virtual R visitBinary(const Binary&) = 0;
    virtual R visitUnary(const Unary&) = 0;
    virtual R visitLiteral(const Literal&) = 0;
    virtual R visitGrouping(const Grouping&) = 0;
    virtual ~ExprVisitor() = default;
};

struct Expr {
    virtual ~Expr() = default;

    template<typename R>
    R accept(ExprVisitor<R>& visitor) const {
        return acceptImpl(visitor);
    }

private:
    virtual Value acceptImpl(ExprVisitor<Value>&) const = 0;
    virtual std::string acceptImpl(ExprVisitor<std::string>&) const = 0;
};

struct Binary : Expr {
    std::shared_ptr<Expr> left;
    Token op;
    std::shared_ptr<Expr> right;

    Binary(std::shared_ptr<Expr> l, Token o, std::shared_ptr<Expr> r)
        : left(l), op(o), right(r) {}

private:
    Value acceptImpl(ExprVisitor<Value>& v) const override {
        return v.visitBinary(*this);
    }
    std::string acceptImpl(ExprVisitor<std::string>& v) const override {
        return v.visitBinary(*this);
    }
};

struct Unary : Expr {
    Token op;
    std::shared_ptr<Expr> right;

    Unary(Token o, std::shared_ptr<Expr> r)
        : op(o), right(r) {}

private:
    Value acceptImpl(ExprVisitor<Value>& v) const override {
        return v.visitUnary(*this);
    }
    std::string acceptImpl(ExprVisitor<std::string>& v) const override {
        return v.visitUnary(*this);
    }
};

struct Literal : Expr {
    Value value;

    Literal(Value v) : value(std::move(v)) {}

private:
    Value acceptImpl(ExprVisitor<Value>& v) const override {
        return v.visitLiteral(*this);
    }
    std::string acceptImpl(ExprVisitor<std::string>& v) const override {
        return v.visitLiteral(*this);
    }
};

struct Grouping : Expr {
    std::shared_ptr<Expr> expression;

    Grouping(std::shared_ptr<Expr> e) : expression(e) {}

private:
    Value acceptImpl(ExprVisitor<Value>& v) const override {
        return v.visitGrouping(*this);
    }
    std::string acceptImpl(ExprVisitor<std::string>& v) const override {
        return v.visitGrouping(*this);
    }
};
