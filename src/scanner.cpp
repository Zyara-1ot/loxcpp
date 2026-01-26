#include <bits/stdc++.h>
#include "scanner.h"
#include <unordered_map>
#include <iostream>
#include "Error.h"
#include <cctype>


const std::unordered_map<std::string, TokenType> Scanner::keywords = {
    {"and",    TokenType::AND},
    {"class",  TokenType::CLASS},
    {"else",   TokenType::ELSE},
    {"false",  TokenType::FALSE},
    {"for",    TokenType::FOR},
    {"fun",    TokenType::FUN},
    {"if",     TokenType::IF},
    {"nil",    TokenType::NIL},
    {"or",     TokenType::OR},
    {"print",  TokenType::PRINT},
    {"return", TokenType::RETURN},
    {"super",  TokenType::SUPER},
    {"this",   TokenType::THIS},
    {"true",   TokenType::TRUE},
    {"var",    TokenType::VAR},
    {"while",  TokenType::WHILE}
};

Scanner::Scanner(const std::string& source)
    : source(source) {}

std::vector<Token> Scanner::scanTokens() {
    while(!isAtEnd()){
        start = current;
        scanToken();
    }
    tokens.emplace_back(TokenType::EOF_TOKEN, "", std::monostate{}, line);
    return tokens;
}

bool Scanner::match(char expected) {
    if (isAtEnd()) return false;
    if (source[current] != expected) return false;

    current++;
    return true;
}


void Scanner::scanToken(){
    char c = advance();
    switch (c) {
        case '(' : addToken(TokenType::LEFT_PAREN); break;
        case ')' : addToken(TokenType::RIGHT_PAREN); break;
        case '{' : addToken(TokenType::LEFT_BRACE); break;
        case '}' : addToken(TokenType::RIGHT_BRACE); break;
        case ',' : addToken(TokenType::COMMA); break;
        case '.' : addToken(TokenType::DOT); break;
        case '-' : addToken(TokenType::MINUS); break;
        case '+' : addToken(TokenType::PLUS); break;
        case ';' : addToken(TokenType::SEMICOLON); break;
        case '*' : addToken(TokenType::STAR); break;
        
        case '!':
            addToken(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
            break;

        case '=':
            addToken(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
            break;

        case '<':
            addToken(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
            break;

        case '>':
            addToken(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
            break;

        case '/':
            if (match('/')) {
    
                while (peek() != '\n' && !isAtEnd()) advance();
            } else {
                addToken(TokenType::SLASH);
            }
            break;

        
        case ' ':
        case '\r':
        case '\t':

            break;

        case '\n':
            line++;
            break;

        case '"':
            string();
            break;



        default:
        if (std::isalpha(c) || c == '_') {
        identifier();
        } else if (std::isdigit(c)) {
            number();
        } else {
            error(line, "Unexpected character.");
        }
        break;

    }
}

char Scanner::advance(){
    current++;
    return source[current - 1];
}


void Scanner::addToken(TokenType type){
    addToken(type, std::monostate{});
}

void Scanner::addToken(TokenType type, Literal literal){
    std::string text = source.substr(start, current - start);
    tokens.emplace_back(type, text, literal, line);
}

char Scanner::peek() const {
    if(isAtEnd()) return '\0';
    return source[current];
}

void Scanner::string() {
    while (peek() != '"' && !isAtEnd()) {
        if (peek() == '\n') line++;
        advance();
    }


    if (isAtEnd()) {
        error(line, "Unterminated string.");
        return;
    }

    advance();

    std::string value = source.substr(start + 1, current - start - 2);

    addToken(TokenType::STRING, value);
}

char Scanner::peekNext() const {
    if (current + 1 >= source.length()) return '\0';
    return source[current + 1];
}


void Scanner::number() {
    while (std::isdigit(peek())) {
        advance();
    }

    if (peek() == '.' && std::isdigit(peekNext())) {
        advance();

        while (std::isdigit(peek())) {
            advance();
        }
    }

    double value = std::stod(
        source.substr(start, current - start)
    );

    addToken(TokenType::NUMBER, value);
}

void Scanner::identifier() {
    while (std::isalnum(peek()) || peek() == '_') {
        advance();
    }

    std::string text = source.substr(start, current - start);

    auto it = keywords.find(text);
    TokenType type = (it != keywords.end())
        ? it->second
        : TokenType::IDENTIFIER;

    addToken(type);
}

bool Scanner::isAtEnd() const {
    return current >= source.length();
}

