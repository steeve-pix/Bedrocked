#pragma once
#include <chrono>

namespace bedrocked {
    class Timer {
    public:
        Timer();

        [[nodiscard]] double tick() noexcept;

    private:
        std::chrono::steady_clock::time_point m_previousTime;
    };
}
