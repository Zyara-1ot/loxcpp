#pragma once

struct IfStmt;
struct ExpressionStmt;
struct PrintStmt;
struct VarStmt;
struct BlockStmt;
struct StmtVisitor {
    virtual void visitExpressionStmt(const ExpressionStmt&) = 0;
    virtual void visitPrintStmt(const PrintStmt&) = 0;
    virtual void visitVarStmt(const VarStmt&) = 0;
    virtual void visitBlockStmt(const BlockStmt&) = 0;
    virtual void visitIfStmt(const IfStmt&) = 0;

};