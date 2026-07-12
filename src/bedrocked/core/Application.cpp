#include "bedrocked/core/Application.hpp"

namespace bedrocked {
    Application::Application() : m_window(WindowConfig{1280, 720, "Bedrocked Engine"}) {
    }

    int Application::run() {
        while (!m_window.shouldClose()) {
            m_window.pollEvents();
        }
        return 0;
    }
}
