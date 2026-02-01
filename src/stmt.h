#pragma once

#include <memory>
#include "stmtVisitor.h"

struct Stmt {
    virtual ~Stmt() = default;
    virtual void accept(StmtVisitor& visitor) const = 0;
};

struct ExpressionStmt : Stmt {
    std::shared_ptr<Expr> expression;

    ExpressionStmt(std::shared_ptr<Expr> expr)
        : expression(std::move(expr)) {}

    void accept(StmtVisitor& visitor) const override {
        visitor.visitExpressionStmt(*this);
    }
};

struct PrintStmt : Stmt {
    std::shared_ptr<Expr> expression;

    PrintStmt(std::shared_ptr<Expr> expr)
        : expression(std::move(expr)) {}

    void accept(StmtVisitor& visitor) const override {
        visitor.visitPrintStmt(*this);
    }
};

struct VarStmt : Stmt {
    Token name;
    std::shared_ptr<Expr> initializer;

    VarStmt(Token name, std::shared_ptr<Expr> init)
        : name(std::move(name)), initializer(std::move(init)) {}

    void accept(StmtVisitor& visitor) const override {
        visitor.visitVarStmt(*this);
    }
};

struct BlockStmt : Stmt {
    std::vector<std::shared_ptr<Stmt>> statements;

    BlockStmt(const std::vector<std::shared_ptr<Stmt>>& statements)
        : statements(statements) {}

    void accept(StmtVisitor& visitor) const override {
        visitor.visitBlockStmt(*this);
    }
};

struct IfStmt : Stmt {
    std::shared_ptr<Expr> condition;
    std::shared_ptr<Stmt> thenBranch;
    std::shared_ptr<Stmt> elseBranch;

    IfStmt(
        std::shared_ptr<Expr> condition,
        std::shared_ptr<Stmt> thenBranch,
        std::shared_ptr<Stmt> elseBranch
    ) : condition(condition),
        thenBranch(thenBranch),
        elseBranch(elseBranch) {}

    void accept(StmtVisitor& visitor) const override {
        visitor.visitIfStmt(*this);
    }
};