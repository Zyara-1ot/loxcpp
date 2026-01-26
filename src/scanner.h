#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "Token.h"

class Scanner {
public:
    explicit Scanner(const std::string& source);
    std::vector<Token> scanTokens();

private:
    const std::string source;
    std::vector<Token> tokens;
    int start = 0;
    int current = 0;
    int line = 1;

    char advance();
    bool match(char expected);
    char peek() const;
    char peekNext() const;
    bool isAtEnd() const;

    void scanToken();
    void string();
    void number();
    void identifier();

    void addToken(TokenType type);
    void addToken(TokenType type, Literal literal);

    static const std::unordered_map<std::string, TokenType> keywords;
};