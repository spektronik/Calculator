#pragma once
#include <windows.h>
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include "PluginAPI.h"

class PluginManager {
public:
    PluginManager() = default;
    ~PluginManager();
    PluginManager(const PluginManager&) = delete;
    PluginManager& operator=(const PluginManager&) = delete;

    size_t LoadFromDirectory(const std::string& dir);

    IPlugin* Find(const std::string& name) const;

    std::vector<std::string> ListFunctions() const;

private:
    struct Loaded {
        HMODULE handle = nullptr;
        std::unique_ptr<IPlugin> plugin;
    };
    std::vector<Loaded> libs_;
    std::unordered_map<std::string, IPlugin*> byName_;

    void UnloadAll();
};