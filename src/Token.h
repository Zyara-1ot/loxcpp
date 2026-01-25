#pragma once
#include <string>
#include <variant>

#include <iostream>

enum class TokenType{
    LEFT_PAREN, RIGHT_PAREN,
    LEFT_BRACE, RIGHT_BRACE,
    COMMA, DOT, MINUS, PLUS,
    SEMICOLON, SLASH, STAR,
    BANG, BANG_EQUAL, 
    EQUAL, EQUAL_EQUAL,
    GREATER, GREATER_EQUAL,
    LESS, LESS_EQUAL,
    IDENTIFIER, STRING, NUMBER,
    AND, CLASS, ELSE, FALSE, 
    FUN, FOR, IF, NIL , OR,  
    PRINT, RETURN, VAR, WHILE,
    SUPER, THIS, TRUE, 
    EOF_TOKEN
};


using Literal = std::variant<std::monostate, double, std::string, bool>;

struct Token{
    TokenType type;
    std::string lexeme;
    Literal literal;
    int line;

    Token(TokenType type, std::string lexeme, Literal literal, int line)
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