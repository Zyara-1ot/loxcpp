#pragma once
#include <string>
#include <variant>
#include "TokenType.h"
#include <iostream>


using Value = std::variant<std::monostate, double, std::string, bool>;

struct Token{
    TokenType type;
    std::string lexeme;
    Value literal;
    int line;

    Token(TokenType type, std::string lexeme, Value literal, int line)
        : type(type), lexeme(std::move(lexeme)),
        literal(std::move(literal)), line(line){}

    std::string toString() const {
        return lexeme;
    }
};

inline std::ostream& operator<<(std::ostream& os, const Token& token) {
    os << static_cast<int>(token.type) << " " << token.lexeme;

    if (!std::holds_alternative<std::monostate>(token.literal)) {
        os << " ";
        if (std::holds_alternative<double>(token.literal)) {
            os << std::get<double>(token.literal);
        } else {
            os << std::get<std::string>(token.literal);
        }
    }
    return os;
}