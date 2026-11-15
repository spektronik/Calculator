#pragma once
#include <string>
#include <vector>

#ifdef _WIN32
#define PLUGIN_EXPORT extern "C" __declspec(dllexport)
#else
#define PLUGIN_EXPORT extern "C" __attribute__((visibility("default")))
#endif

class PluginError : public std::exception {
private:
    std::string msg_;
public:
    explicit PluginError(const std::string& msg) : msg_(msg) {}
    const char* what() const noexcept override { return msg_.c_str(); }
};

class IPlugin {
public:
    virtual ~IPlugin() = default;

    virtual const char* Name() const = 0;

    virtual int Arity() const = 0;

    virtual double Call(const std::vector<double>& args) = 0;
};

typedef IPlugin* (*CreatePluginFn)();

PLUGIN_EXPORT IPlugin* CreatePlugin();