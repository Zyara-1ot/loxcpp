#pragma once
#include <string>

struct Binary;
struct Unary;
struct Literal;
struct Grouping;

struct ExprVisitor {
    virtual  std::string visitBinary(const Binary& expr) = 0;
    virtual std::string visitUnary(const Unary& expr) = 0;
    virtual std::string visitLiteral(const Literal& expr) = 0;
    virtual std::string visitGrouping(const Grouping& expr) = 0;
    virtual ~ExprVisitor() = default;
};