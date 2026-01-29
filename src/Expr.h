#pragma once
#include <memory>
#include <string>
#include <variant>
#include "Token.h"
#include "ExprVisitor.h"

struct Expr {
    virtual ~Expr() = default;
    virtual std::string accept(ExprVisitor<std::string>& visitor) const = 0;
};

struct Binary : Expr {
    std::shared_ptr<Expr> left;
    Token op;
    std::shared_ptr<Expr> right;

    Binary(std::shared_ptr<Expr> l, Token o, std::shared_ptr<Expr> r)
        : left(l), op(o), right(r) {}

    std::string accept(ExprVisitor<std::string>& visitor) const override {
        return visitor.visitBinary(*this);
    }
};

struct Unary : Expr {
    Token op;
    std::shared_ptr<Expr> right;

    Unary(Token o, std::shared_ptr<Expr> r)
        : op(o), right(r) {}

    std::string accept(ExprVisitor<std::string>& visitor) const override {
        return visitor.visitUnary(*this);
    }
};

struct Literal : Expr {
    Value value;
    Literal(Value v) : value(std::move(v)) {}
    std::string accept(ExprVisitor<std::string>& visitor) const override {
        return visitor.visitLiteral(*this);
    }

    std::string toString() const {
        if (std::holds_alternative<std::monostate>(value)) return "nil";
        if (std::holds_alternative<double>(value))
            return std::to_string(std::get<double>(value));
        if (std::holds_alternative<std::string>(value))
            return std::get<std::string>(value);
        if (std::holds_alternative<bool>(value))
            return std::get<bool>(value) ? "true" : "false";
        return "";
    }
};

struct Grouping : Expr {
    std::shared_ptr<Expr> expression;

    Grouping(std::shared_ptr<Expr> expr) : expression(expr) {}

    std::string accept(ExprVisitor<std::string>& visitor) const override {
        return visitor.visitGrouping(*this);
    }
};
