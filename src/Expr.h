#pragma once
#include <memory>
#include <string>
#include <variant>
#include "Token.h"
#include "ExprVIsitor.h"


struct Expr {
    virtual std::string accept(ExprVisitor& visitor) const = 0;
    virtual ~Expr() = default;
};


struct Binary : Expr {
    std::shared_ptr<Expr> left;
    Token op;
    std::shared_ptr<Expr> right;

    Binary(std::shared_ptr<Expr> l, Token o, std::shared_ptr<Expr> r)
        : left(l), op(o), right(r) {}

    std::string accept(ExprVisitor& visitor) const override {
        return visitor.visitBinary(*this);
    }
};


struct Unary : Expr {
    Token op;
    std::shared_ptr<Expr> right;

    Unary(Token o, std::shared_ptr<Expr> r)
        : op(o), right(r) {}

    std::string accept(ExprVisitor& visitor) const override {
        return visitor.visitUnary(*this);
    }
};

struct Literal : Expr {
    std::variant<double, std::string, bool, std::nullptr_t> value;

    Literal(double v) : value(v) {}
    Literal(const std::string& v) : value(v) {}
    Literal(bool v) : value(v) {}
    Literal(std::nullptr_t) : value(nullptr) {}

    std::string accept(ExprVisitor& visitor) const override {
        return visitor.visitLiteral(*this);
    }
};

struct Grouping : Expr {
    std::shared_ptr<Expr> expression;

    Grouping(std::shared_ptr<Expr> e) : expression(e) {}

    std::string accept(ExprVisitor& visitor) const override {
        return visitor.visitGrouping(*this);
    }
};
