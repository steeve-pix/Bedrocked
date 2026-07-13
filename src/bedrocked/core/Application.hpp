#pragma once
#include "bedrocked/core/Timer.hpp"
#include "bedrocked/platform/Window.hpp"
#include "bedrocked/rendering/Renderer.hpp"

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
}
