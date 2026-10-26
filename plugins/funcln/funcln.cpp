#include "PluginAPI.h"
#include <cmath>
#include <vector>

namespace {
    class LnPlugin final : public IPlugin {
    public:
        const char* Name() const override { return "ln"; }
        int Arity() const override { return 1; }
        double Call(const std::vector<double>& a) override {
            if (a.size() != 1) throw PluginError{ "ln expects 1 argument" };
            if (a[0] <= 0.0) throw PluginError{ "ln: argument must be > 0" };
            return std::log(a[0]);
        }
    };
}
PLUGIN_EXPORT IPlugin* CreatePlugin() { return new LnPlugin(); }