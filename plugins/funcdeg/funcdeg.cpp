#include "PluginAPI.h"
#include <cmath>
#include <vector>

namespace {
    class DegPlugin final : public IPlugin {
    public:
        const char* Name() const override { return "deg"; }
        int Arity() const override { return 2; }
        double Call(const std::vector<double>& a) override {
            if (a.size() != 2) throw PluginError{ "deg expects 2 arguments" };
            return std::pow(a[0], a[1]);
        }
    };
}

PLUGIN_EXPORT IPlugin* CreatePlugin() { return new DegPlugin(); }