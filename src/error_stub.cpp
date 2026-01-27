#include <iostream>
#include <string>

void error(int line, const std::string& message) {
    std::cerr << "[line " << line << "] Error: " << message << std::endl;
}
