#include "PluginAPI.h"
#include <cmath>
#include <vector>

namespace {
    constexpr double PI = 3.14159265358979323846;

    class SinPlugin final : public IPlugin {
    public:
        const char* Name() const override { return "sin"; }
        int Arity() const override { return 1; }
        double Call(const std::vector<double>& a) override {
            if (a.size() != 1) throw PluginError{ "sin expects 1 argument" };
            return std::sin(a[0] * PI / 180.0);
        }
    };
}

PLUGIN_EXPORT IPlugin* CreatePlugin() { return new SinPlugin(); }