#pragma once
#include <stdexcept>
#include <string>

class CalcError : public std::runtime_error {
public:
    explicit CalcError(const std::string& msg) : std::runtime_error(msg) {}
};