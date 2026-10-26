#include "PluginManager.h"
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

PluginManager::~PluginManager() { UnloadAll(); }

void PluginManager::UnloadAll() {
    byName_.clear();
    for (auto& l : libs_) l.plugin.reset();
    for (auto& l : libs_) {
        if (l.handle) { FreeLibrary(l.handle); l.handle = nullptr; }
    }
    libs_.clear();
}

size_t PluginManager::LoadFromDirectory(const std::string& dir) {
    UnloadAll();

    std::error_code ec;
    if (!fs::exists(dir, ec) || !fs::is_directory(dir, ec)) {
        std::cerr << "[plugins] directory '" << dir << "' not found\n";
        return 0;
    }

    size_t loaded = 0;
    for (auto& entry : fs::directory_iterator(dir, ec)) {
        if (ec) break;
        if (!entry.is_regular_file()) continue;
        if (entry.path().extension() != ".dll") continue;

        const std::string path = entry.path().string();

        HMODULE h = LoadLibraryA(path.c_str());
        if (!h) {
            std::cerr << "[plugins] failed to load " << path
                << " (error " << GetLastError() << ")\n";
            continue;
        }

        auto fn = reinterpret_cast<CreatePluginFn>(
            reinterpret_cast<void*>(GetProcAddress(h, "CreatePlugin")));
        if (!fn) {
            std::cerr << "[plugins] " << path
                << " has no CreatePlugin export\n";
            FreeLibrary(h);
            continue;
        }

        IPlugin* raw = nullptr;
        try {
            raw = fn();
        }
        catch (const std::exception& e) {
            std::cerr << "[plugins] " << path << " CreatePlugin threw: "
                << e.what() << "\n";
            FreeLibrary(h);
            continue;
        }
        catch (...) {
            std::cerr << "[plugins] " << path << " CreatePlugin threw unknown\n";
            FreeLibrary(h);
            continue;
        }

        if (!raw) {
            std::cerr << "[plugins] " << path << " returned null plugin\n";
            FreeLibrary(h);
            continue;
        }

        std::unique_ptr<IPlugin> plugin(raw);

        const char* nm = nullptr;
        try { nm = plugin->Name(); }
        catch (...) { nm = nullptr; }
        if (!nm || !*nm) {
            std::cerr << "[plugins] " << path << " plugin has empty name\n";
            continue;
        }
        std::string name(nm);
        if (byName_.count(name)) {
            std::cerr << "[plugins] duplicate function '" << name
                << "' in " << path << ", ignored\n";
            continue;
        }

        Loaded L;
        L.handle = h;
        L.plugin = std::move(plugin);
        byName_[name] = L.plugin.get();
        libs_.push_back(std::move(L));
        ++loaded;
        std::cout << "[plugins] loaded '" << name << "' from "
            << entry.path().filename().string() << "\n";
    }
    return loaded;
}

IPlugin* PluginManager::Find(const std::string& name) const {
    auto it = byName_.find(name);
    return it == byName_.end() ? nullptr : it->second;
}

std::vector<std::string> PluginManager::ListFunctions() const {
    std::vector<std::string> r;
    r.reserve(byName_.size());
    for (auto& kv : byName_) r.push_back(kv.first);
    return r;
}