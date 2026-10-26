#pragma once
#include <string>
#include <vector>

#ifdef _WIN32
#define PLUGIN_EXPORT extern "C" __declspec(dllexport)
#else
#define PLUGIN_EXPORT extern "C" __attribute__((visibility("default")))
#endif

struct PluginError {
    const char* message;
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