#include <iostream>
#include "Error.h"

bool hadError = false;

void error(int line, const std::string& message) {
    std::cerr << "[line " << line << "] Error: " << message << "\n";
    hadError = true;
}
