#include "bedrocked/core/Application.hpp"
#include "bedrocked/core/Logger.hpp"

#include <cstdlib>
#include <exception>


int main() {
    try {
        bedrocked::Application application;
        return application.run();
    } catch (const std::exception &error) {
        bedrocked::Logger::error(error.what());
        return EXIT_FAILURE;
    }
}
