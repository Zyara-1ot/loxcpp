#include <iostream>
#include <vector>
#include <cassert>
#include "scanner.h"

void expectTokenTypes(
    const std::string& source,
    const std::vector<TokenType>& expected
) {
    Scanner scanner(source);
    auto tokens = scanner.scanTokens();

    assert(tokens.size() == expected.size());

    for (size_t i = 0; i < expected.size(); i++) {
        assert(tokens[i].type == expected[i]);
    }
}

int main() {
    expectTokenTypes("()", {
        TokenType::LEFT_PAREN,
        TokenType::RIGHT_PAREN,
        TokenType::EOF_TOKEN
    });
    expectTokenTypes("var x = 10;", {
        TokenType::VAR,
        TokenType::IDENTIFIER,
        TokenType::EQUAL,
        TokenType::NUMBER,
        TokenType::SEMICOLON,
        TokenType::EOF_TOKEN
    });
    std::cout << "All scanner tests passed \n";
}
