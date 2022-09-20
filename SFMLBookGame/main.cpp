#include "pch.h"
#include "ProcessArchitecture/Application.h"

int main() {
    try {
        Application app;
        app.run();
        return 0;
    }
    catch(std::exception& e) {
        std::cerr << '\n' << e.what() << '\n';
    }
}
