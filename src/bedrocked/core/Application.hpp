#pragma once
#include "bedrocked/platform/Window.hpp"

namespace bedrocked {
    class Application final {
    public:
        Application();

        ~Application() = default;

        [[nodiscard]] int run();
    private:
        Window m_window;
    };
}
