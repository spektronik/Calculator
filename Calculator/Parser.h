#pragma once
#include <string>
#include <memory>
#include <functional>
#include "PluginAPI.h"

class Parser {
public:
    using Resolver = std::function<IPlugin* (const std::string&)>;

    Parser(std::string input, Resolver resolver);
    double Parse();

private:
    std::string src_;
    size_t pos_ = 0;
    Resolver resolver_;

    void SkipSpaces();
    char Peek();
    char Get();
    void Expect(char c);

    double ParseExpression();
    double ParseTerm();
    double ParseFactor();
    double ParseUnary();
    double ParsePower();
    double ParsePrimary();
};