#include "Env.h"
#include <stdexcept>

Environment::Environment() : enclosing(nullptr) {}

Environment::Environment(std::shared_ptr<Environment> enclosing)
    : enclosing(enclosing) {}
void Environment::define(const std::string& name, const Value& value) {
    values[name] = value;
}

Value Environment::get(const Token& name) {
    if (values.find(name.lexeme) != values.end()) {
        return values[name.lexeme];
    }
    if (enclosing) return enclosing->get(name);
    throw std::runtime_error("Undefined variable '" + name.lexeme + "'.");
}

void Environment::assign(const Token& name, const Value& value) {
    if (values.find(name.lexeme) != values.end()) {
        values[name.lexeme] = value;
        return;
    }
    if (enclosing) {
        enclosing->assign(name, value);
        return;
    }
    throw std::runtime_error("Undefined variable '" + name.lexeme + "'.");
}