#ifndef ENV_H
#define ENV_H

#include <unordered_map>
#include <memory>
#include <string>
#include "Token.h"
#include "Value.h"

class Environment {
public:
    Environment();
    Environment(std::shared_ptr<Environment> enclosing);
    void define(const std::string& name, const Value& value);
    Value get(const Token& name);
    void assign(const Token& name, const Value& value);
private:
    std::unordered_map<std::string, Value> values;
    std::shared_ptr<Environment> enclosing;
};

#endif