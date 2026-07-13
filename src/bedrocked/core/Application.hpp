#pragma once
#include "bedrocked/core/Timer.hpp"
#include "bedrocked/platform/Window.hpp"

namespace bedrocked {
    class Application final {
    public:
        Application();

        ~Application() = default;

        [[nodiscard]] int run();
    private:
        Window m_window;
        Timer m_timer;
    };
}
