#include <bits/stdc++.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "scanner.h"
#include "Error.h"
#include "parser.h"
#include "AstPrinter.h"

void run(const std::string& source){
    Scanner scanner(source);
    auto tokens = scanner.scanTokens();

    Parser parser(tokens);
    auto expr = parser.parse();

    if (!expr) return;

    AstPrinter printer;
    std::cout << printer.print(expr) << std::endl;


}

void report(int line, const std::string& where, const std::string& message){
    std::cerr << "[Line" << line << "[S] Error" << where << ":" << message << "/n";
    hadError = true;
}

void runFile(const std::string& path){
    std::ifstream file(path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    run(buffer.str());

    if(hadError){
        exit(65);
    }
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

int main(int argc, char* argv[]){
    if(argc > 2){
        std::cout << "usage: loxcpp [script]\n";
        return 64;
    } else if (argc == 2){
        runFile(argv[1]);
    } else{
        runPrompt();
    }
}