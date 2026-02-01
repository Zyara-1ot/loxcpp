#include "parser.h"
#include "TokenType.h"

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens) {}

std::shared_ptr<Expr> Parser::expression() {
    return assignment();
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

std::shared_ptr<Expr> Parser::assignment() {
    auto expr = equality();

    if (match({TokenType::EQUAL})) {
        Token equals = previous();
        auto value = assignment();

        if (auto var = std::dynamic_pointer_cast<Variable>(expr)) {
            return std::make_shared<Assign>(var->name, value);
        }

        throw std::runtime_error("Invalid assignment target.");
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
    if (match({TokenType::IDENTIFIER})) {
    return std::make_shared<Variable>(previous());
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

std::vector<std::shared_ptr<Stmt>> Parser::parse() {
    std::vector<std::shared_ptr<Stmt>> statements;
    while (!isAtEnd()) {
        statements.push_back(declaration());
    }
    return statements;
}

std::shared_ptr<Stmt> Parser::statement() {
    if (match({TokenType::PRINT})) return printStatement();
    if (match({TokenType::IF})) return ifStatement();
    if(match({TokenType::LEFT_BRACE})) return block();
    return expressionStatement();
}

std::shared_ptr<Stmt> Parser::printStatement() {
    auto value = expression();
    consume(TokenType::SEMICOLON, "Expect ';' after value.");
    return std::make_shared<PrintStmt>(value);
}

std::shared_ptr<Stmt> Parser::expressionStatement() {
    auto expr = expression();
    consume(TokenType::SEMICOLON, "Expect ';' after expression.");
    return std::make_shared<ExpressionStmt>(expr);
}

std::shared_ptr<Stmt> Parser::declaration() {
    if (match({TokenType::VAR})) return varDeclaration();
    return statement();
}

std::shared_ptr<Stmt> Parser::varDeclaration() {
    Token name = consume(TokenType::IDENTIFIER, "Expected variable name.");

    std::shared_ptr<Expr> initializer = nullptr;
    if (match({TokenType::EQUAL})) {
        initializer = expression();
    }

    consume(TokenType::SEMICOLON, "Expected ';' after variable declaration.");
    return std::make_shared<VarStmt>(name, initializer);
}

std::shared_ptr<Stmt> Parser::block() {
    std::vector<std::shared_ptr<Stmt>> statements;

    while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
        statements.push_back(statement());
    }

    consume(TokenType::RIGHT_BRACE, "Expect '}' after block.");
    return std::make_shared<BlockStmt>(statements);
}

std::shared_ptr<Stmt> Parser::ifStatement() {
    consume(TokenType::LEFT_PAREN, "Expect '(' after 'if'.");
    auto condition = expression();
    consume(TokenType::RIGHT_PAREN, "Expect ')' after condition.");

    auto thenBranch = statement();
    std::shared_ptr<Stmt> elseBranch = nullptr;

    if (match({TokenType::ELSE})) {
        elseBranch = statement();
    }
    return std::make_shared<IfStmt>(condition, thenBranch, elseBranch);
}