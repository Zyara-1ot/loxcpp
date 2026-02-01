#pragma once
#include <vector>
#include <memory>
#include "Expr.h"
#include "stmt.h"
#include "Expr.h"
#include "ExprVisitor.h"
#include "stmtVisitor.h"
#include "Env.h"
#include "Value.h"

using Value = std::variant<std::monostate, double, std::string, bool>;

class Interpreter :
    public ExprVisitor<Value>,
    public StmtVisitor {
        
public:
    void interpret(const std::vector<std::shared_ptr<Stmt>>& statements);
    Value visitBinary(const Binary&) override;
    Value visitUnary(const Unary&) override;
    Value visitLiteral(const Literal&) override;
    Value visitGrouping(const Grouping&) override;
    void visitExpressionStmt(const ExpressionStmt&) override;
    void visitPrintStmt(const PrintStmt&) override;
    void visitVarStmt(const VarStmt&) override;
    Value visitVariable(const Variable&) override;
    Value visitAssign(const Assign&) override;
    void visitBlockStmt(const BlockStmt&) override;
    Interpreter();
    void visitIfStmt(const IfStmt&) override;

private:

    std::shared_ptr<Environment> environment;
    Value evaluate(const std::shared_ptr<Expr>& expr);
    void execute(const std::shared_ptr<Stmt>& stmt);
    bool isEqual(const Value& left, const Value& right); 
    bool isTruthy(const Value& value);
    std::string stringify(const Value& value);
    void executeBlock(
        const std::vector<std::shared_ptr<Stmt>>& statements,
        std::shared_ptr<Environment> env
    );
    void checkNumberOperand(const Token& op, const Value& value);
    void checkNumberOperands(const Token& op,
    const Value& left,
    const Value& right);
};
