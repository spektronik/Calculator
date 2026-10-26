#include <iostream>
#include <string>
#include <windows.h>
#include "PluginManager.h"
#include "Parser.h"
#include "Exceptions.h"

int main(int argc, char** argv) {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
    try {
        PluginManager pm;
        size_t n = pm.LoadFromDirectory("./plugins");
        if (n == 0) {
            std::cerr << "[calc] warning: no plugins loaded, "
                "only arithmetic is available\n";
        }
        else {
            std::cout << "[calc] loaded " << n << " plugin(s): ";
            for (auto& s : pm.ListFunctions()) std::cout << s << ' ';
            std::cout << "\n";
        }

        std::string line;
        while (true) {
            std::cout << "> " << std::flush;
            if (!std::getline(std::cin, line)) break;
            if (line == "quit" || line == "exit") break;
            if (line.empty()) continue;

            try {
                Parser p(line, [&pm](const std::string& name) { return pm.Find(name); });
                double v = p.Parse();
                std::cout << v << "\n";
            }
            catch (const CalcError& e) {
                std::cerr << "error: " << e.what() << "\n";
            }
            catch (const std::exception& e) {
                std::cerr << "fatal: " << e.what() << "\n";
            }
        }
    }
    catch (const std::exception& e) {
        std::cerr << "fatal: " << e.what() << "\n";
        return 1;
    }
    return 0;
}