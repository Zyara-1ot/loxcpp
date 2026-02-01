#pragma once
#include <string>
#include <memory>
#include "Expr.h"

class AstPrinter : public ExprVisitor<std::string> {
public:
   std::string print(const std::shared_ptr<Expr>& expr);

   std::string visitBinary(const Binary& expr) override;
  std::string visitUnary(const Unary& expr) override;
  std::string visitLiteral(const Literal& expr) override;
  std::string visitGrouping(const Grouping& expr) override;
};
