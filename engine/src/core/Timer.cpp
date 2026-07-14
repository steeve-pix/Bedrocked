#include "../../include/bedrocked/core/Timer.hpp"

namespace bedrocked {
    Timer::Timer() : m_previousTime(std::chrono::steady_clock::now()) {
    }

    double Timer::tick() noexcept {
        const auto currentTime = std::chrono::steady_clock::now();
        const std::chrono::duration<double> elapsedTime = currentTime - m_previousTime;
        m_previousTime = currentTime;

        return elapsedTime.count();
    }
} // namespace bedrocked
