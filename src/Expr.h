#pragma once
#include <memory>
#include "Value.h"
#include <string>
#include "Token.h"
#include "ExprVisitor.h"

struct Expr;
struct Binary;
struct Unary;
struct Literal;
struct Grouping;
struct Variable;

using Value = std::variant<std::monostate, double, std::string, bool>;

struct Expr {
    virtual ~Expr() = default;
    virtual Value accept(ExprVisitor<Value>& visitor) const = 0;
    virtual std::string acceptPrinter(ExprVisitor<std::string>& visitor) const = 0;
};

struct Binary : Expr {
    std::shared_ptr<Expr> left;
    Token op;
    std::shared_ptr<Expr> right;

    Binary(std::shared_ptr<Expr> l, Token o, std::shared_ptr<Expr> r)
        : left(l), op(o), right(r) {}

    Value accept(ExprVisitor<Value>& visitor) const override {
        return visitor.visitBinary(*this);
    }
    std::string acceptPrinter(ExprVisitor<std::string>& visitor) const override {
    return visitor.visitBinary(*this);
}
};

struct Unary : Expr {
    Token op;
    std::shared_ptr<Expr> right;

    Unary(Token o, std::shared_ptr<Expr> r)
        : op(o), right(r) {}

Value accept(ExprVisitor<Value>& visitor) const override {
        return visitor.visitUnary(*this);
    }
    std::string acceptPrinter(ExprVisitor<std::string>& visitor) const override {
    return visitor.visitUnary(*this);
}
};

struct Literal : Expr {
    Value value;

    Literal(Value v) : value(std::move(v)) {}

Value accept(ExprVisitor<Value>& visitor) const override {
        return visitor.visitLiteral(*this);
    }
    std::string acceptPrinter(ExprVisitor<std::string>& visitor) const override {
    return visitor.visitLiteral(*this);
}
};

struct Grouping : Expr {
    std::shared_ptr<Expr> expression;

    Grouping(std::shared_ptr<Expr> e) : expression(e) {}

Value accept(ExprVisitor<Value>& visitor) const override {
        return visitor.visitGrouping(*this);
    }
    std::string acceptPrinter(ExprVisitor<std::string>& visitor) const override {
    return visitor.visitGrouping(*this);
}
};

struct Variable : Expr {
    Token name;

    Variable(const Token& name) : name(name) {}

    Value accept(ExprVisitor<Value>& visitor) const override {
        return visitor.visitVariable(*this);
    }
    std::string acceptPrinter(ExprVisitor<std::string>& visitor) const override {
        return visitor.visitVariable(*this);
    }
};

struct Assign : Expr {
    Token name;
    std::shared_ptr<Expr> value;

    Assign(const Token& name, std::shared_ptr<Expr> value)
        : name(name), value(value) {}

    Value accept(ExprVisitor<Value>& visitor) const override {
        return visitor.visitAssign(*this);
    }
    std::string acceptPrinter(ExprVisitor<std::string>& visitor) const override {
    return visitor.visitAssign(*this);
}
};