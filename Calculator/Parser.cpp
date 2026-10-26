#include "Parser.h"
#include "Exceptions.h"
#include <cctype>
#include <cmath>
#include <sstream>

Parser::Parser(std::string input, Resolver r)
    : src_(std::move(input)), resolver_(std::move(r)) {}

void Parser::SkipSpaces() {
    while (pos_ < src_.size() && std::isspace((unsigned char)src_[pos_])) ++pos_;
}
char Parser::Peek() { SkipSpaces(); return pos_ < src_.size() ? src_[pos_] : '\0'; }
char Parser::Get() { SkipSpaces(); return pos_ < src_.size() ? src_[pos_++] : '\0'; }
void Parser::Expect(char c) {
    if (Get() != c) { std::ostringstream o; o << "expected '" << c << "' at " << pos_; throw CalcError(o.str()); }
}

double Parser::Parse() {
    double v = ParseExpression();
    SkipSpaces();
    if (pos_ != src_.size()) throw CalcError("unexpected trailing input");
    return v;
}

double Parser::ParseExpression() {
    double v = ParseTerm();
    while (true) {
        char c = Peek();
        if (c == '+') { Get(); v += ParseTerm(); }
        else if (c == '-') { Get(); v -= ParseTerm(); }
        else break;
    }
    return v;
}

double Parser::ParseTerm() {
    double v = ParseUnary();
    while (true) {
        char c = Peek();
        if (c == '*') { Get(); v *= ParseUnary(); }
        else if (c == '/') {
            Get();
            double d = ParseUnary();
            if (d == 0.0) throw CalcError("division by zero");
            v /= d;
        }
        else break;
    }
    return v;
}

double Parser::ParseUnary() {
    if (Peek() == '-') { Get(); return -ParseUnary(); }
    if (Peek() == '+') { Get(); return ParseUnary(); }
    return ParsePower();
}

double Parser::ParsePower() {
    double base = ParsePrimary();
    if (Peek() == '^') {
        Get();
        double exp = ParseUnary();

        IPlugin* p = resolver_ ? resolver_("deg") : nullptr;
        if (!p) throw CalcError("plugin 'deg' not loaded, cannot use '^'");

        try {
            return p->Call({ base, exp });
        }
        catch (const PluginError& e) {
            throw CalcError(std::string("plugin error in deg: ") + e.message);
        }
    }
    return base;
}

double Parser::ParsePrimary() {
    char c = Peek();

    if (c == '(') {
        Get();
        double v = ParseExpression();
        Expect(')');
        return v;
    }

    if (std::isdigit((unsigned char)c) || c == '.') {
        size_t start = pos_;
        while (pos_ < src_.size() &&
            (std::isdigit((unsigned char)src_[pos_]) || src_[pos_] == '.')) ++pos_;
        try { return std::stod(src_.substr(start, pos_ - start)); }
        catch (...) { throw CalcError("bad number"); }
    }

    if (std::isalpha((unsigned char)c) || c == '_') {
        size_t start = pos_;
        while (pos_ < src_.size() &&
            (std::isalnum((unsigned char)src_[pos_]) || src_[pos_] == '_')) ++pos_;
        std::string name = src_.substr(start, pos_ - start);
        Expect('(');
        std::vector<double> args;
        if (Peek() != ')') {
            args.push_back(ParseExpression());
            while (Peek() == ',') { Get(); args.push_back(ParseExpression()); }
        }
        Expect(')');

        IPlugin* p = resolver_ ? resolver_(name) : nullptr;
        if (!p) throw CalcError("unknown function: " + name);
        if ((int)args.size() != p->Arity())
            throw CalcError("wrong arity for " + name);
        try {
            return p->Call(args);
        }
        catch (const PluginError& e) {
            throw CalcError(std::string("plugin error in ") + name + ": " + (e.message ? e.message : ""));
        }
        catch (const std::exception& e) {
            throw CalcError(std::string("plugin exception in ") + name + ": " + e.what());
        }
        catch (...) {
            throw CalcError("plugin threw unknown exception in " + name);
        }
    }

    throw CalcError("unexpected symbol");
}