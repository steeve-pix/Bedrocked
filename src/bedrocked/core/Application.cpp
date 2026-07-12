#include "Application.hpp"

#include <iostream>

namespace bedrocked {
    Application::Application() : m_window(WindowConfig{1280, 720, "Bedrocked Engine"}) {
    }

    void Application::run() {
        while (!m_window.ShouldClose()) {
            m_window.pollEvents();
        }
    }
}
