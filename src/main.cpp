// FILE: src/main.cpp
#include "core/Engine.h"

#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    std::string description;

    if (argc >= 2) {
        description = argv[1];
    } else {
        std::cout << "================================\n"
                  << "   Text2Cinema Engine v1.0\n"
                  << "================================\n\n"
                  << "Describe your scene:\n> ";
        std::getline(std::cin, description);
        if (description.empty()) {
            description = "A glowing meteor falls from the sky "
                          "and explodes into sparks when it hits the ocean.";
            std::cout << "(using demo scene)\n";
        }
    }

    t2c::Engine engine;
    return engine.run(description) ? 0 : 1;
}