#pragma once
#include "Timer.hpp"
#include "../platform/Window.hpp"
#include "../rendering/Renderer.hpp"

namespace bedrocked {
    class Application final {
    public:
        Application();

        [[nodiscard]] int run();

    private:
        Window m_window;
        Timer m_timer;
        Renderer m_renderer;
    };
} // namespace bedrocked
