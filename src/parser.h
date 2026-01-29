#pragma once
#include <vector>
#include <memory>
#include "Token.h"
#include "Expr.h"
#include <stdexcept>

class ParseError : public std::runtime_error {
public:
    ParseError() : std::runtime_error("Parse error") {}
};

class Parser{
public:
    Parser(const std::vector<Token>& tokens);
    std::shared_ptr<Expr> parse();

private:
    const std::vector<Token>& tokens;
    int current = 0;

    std::shared_ptr<Expr> expression();
    std::shared_ptr<Expr> equality();
    std::shared_ptr<Expr> comparison();
    std::shared_ptr<Expr> term();
    std::shared_ptr<Expr> factor();
    std::shared_ptr<Expr> unary();
    std::shared_ptr<Expr> primary();

    bool match(std::initializer_list<TokenType> types);
    bool check(TokenType type) const;
    Token advance();
    bool isAtEnd() const;
    Token peek() const;
    Token previous() const;

    Token consume(TokenType type, const std::string& message);
    ParseError error(const Token& token, const std::string& message);

};



