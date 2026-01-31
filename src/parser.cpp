#include "parser.h"

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens) {}

std::shared_ptr<Expr> Parser::expression() {
    return equality();
}
std::shared_ptr<Expr> Parser::equality(){
    auto expr = comparison();
    while(match({TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL})){
        Token op = previous();
        auto right = comparison();
        expr = std::make_shared<Binary>(expr, op, right);
    }
    return expr;
}
bool Parser::match(std::initializer_list<TokenType> types) {
    for (TokenType type : types) {
        if (check(type)) {
            advance();
            return true;
        }
    }
    return false;
}
bool Parser::check(TokenType type) const{
    if(isAtEnd()) return false;
    return peek().type == type;
}
Token Parser::advance(){
    if(!isAtEnd()) current++;
    return previous();
}
Token Parser::peek() const{
    return tokens[current];
}
Token Parser::previous() const{
    return tokens[current -1];
}
bool Parser::isAtEnd() const {
    return peek().type == TokenType::EOF_TOKEN;
}
std::shared_ptr<Expr> Parser::comparison(){
    auto expr =  term();

    while(match({TokenType::GREATER,
    TokenType::GREATER_EQUAL,
    TokenType::LESS,
    TokenType::LESS_EQUAL})){
    Token op = previous();
    auto right = term();
    expr = std::make_shared<Binary>(expr, op, right);
}
return expr;
}
std::shared_ptr<Expr> Parser::term(){
    auto expr = factor();

    while(match({TokenType::PLUS, TokenType::MINUS})){
        Token op = previous();
        auto right = factor();
        expr = std::make_shared<Binary>(expr, op, right);
    }
    return expr;
}
std::shared_ptr<Expr> Parser::factor(){
    auto expr = unary();

    while (match({TokenType::STAR, TokenType::SLASH})){
        Token op = previous();
        auto right = unary();
        expr = std::make_shared<Binary>(expr, op, right);
    }
    return expr;
}
std::shared_ptr<Expr> Parser::unary(){
    if(match({TokenType::BANG, TokenType::MINUS})){
        Token op = previous();
        auto right = unary();
        return std::make_shared<Unary>(op, right);
    }
    return primary();
}
std::shared_ptr<Expr> Parser::primary(){
    if(match({TokenType::FALSE})) return std::make_shared<Literal>(false);
    if(match({TokenType::TRUE})) return std::make_shared<Literal>(true);
    if(match({TokenType::NIL})) return std::make_shared<Literal>(nullptr);
    if(match({TokenType::NUMBER, TokenType::STRING})){
        return std::make_shared<Literal>(previous().literal);
    }
    if(match({TokenType::LEFT_PAREN})){
        auto expr = expression();
        consume(TokenType::RIGHT_PAREN, "Expect ')' after expression.");
        return std::make_shared<Grouping>(expr);
    }
    throw std::runtime_error("Expected expression.");
}
Token Parser::consume(TokenType type, const std::string& message) {
    if (check(type)) return advance();
    throw error(peek(), message);
}

ParseError Parser::error(const Token& token, const std::string& message) {
    std::cerr << "[line " << token.line << "] Error at '" << token.lexeme << "': " << message << "\n";
    return ParseError();
}

std::shared_ptr<Expr> Parser::parse() {
    try {
        return expression();
    } catch (ParseError&) {
        return nullptr;
    }
}
