#include <bits/stdc++.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "scanner.h"
#include "Error.h"
#include "parser.h"
#include "Interpreter.h"
#include "stmt.h"

Interpreter interpreter;

void run(const std::string& source) {
    Scanner scanner(source);
    auto tokens = scanner.scanTokens();

    Parser parser(tokens);
    auto statements = parser.parse(); 

    interpreter.interpret(statements); 
}
void report(int line, const std::string& where, const std::string& message){
    std::cerr << "[Line" << line << "[S] Error" << where << ":" << message << "/n";
    hadError = true;
}
void runFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Could not open file: " << path << std::endl;
        return;
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    buffer << "\n";   
    run(buffer.str());
}
void runPrompt(){
    std::string line;
    while(true){
        std::cout << ">";
        if(!std::getline(std::cin, line)) break;
        run(line);
        hadError = false;
    }
}
int main(int argc, char* argv[]) {
    if (argc > 2) {
        std::cerr << "Usage: lox [script]\n";
        return 1;
    }
    if (argc == 2) {
        runFile(argv[1]);  
    } else {
        runPrompt();
    }

    return 0;
}