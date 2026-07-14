#include "../../include/bedrocked/core/Logger.hpp"

#include <iostream>
#include <ostream>

namespace {
    [[nodiscard]] std::string_view levelName(bedrocked::LogLevel level) noexcept {
        switch (level) {
            case bedrocked::LogLevel::Info: return "INFO";
            case bedrocked::LogLevel::Warning: return "WARNING";
            case bedrocked::LogLevel::Error: return "ERROR";
        }
        return "UNKNOWN";
    }
} // namespace

namespace bedrocked {
    void Logger::log(LogLevel level, std::string_view message) {
        std::ostream &output =
                level == LogLevel::Info ? std::cout : std::cerr;
        output << "[" << levelName(level) << "] " << message << '\n';
    }
} // namespace bedrocked
