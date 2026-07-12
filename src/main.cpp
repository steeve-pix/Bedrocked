#include <iostream>

#include "bedrocked/core/Application.hpp"

int main() {
    try {
        bedrocked::Application application;
        return application.run();
    } catch (const std::exception &error) {
        std::cerr << "Fatal error: " << error.what() << '\n';
        return EXIT_FAILURE;
    }
}
