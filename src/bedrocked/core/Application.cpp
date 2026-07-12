#include "bedrocked/core/Application.hpp"
#include "bedrocked/input/Key.hpp"

namespace bedrocked {
    Application::Application() : m_window(WindowConfig{1280, 720, "Bedrocked Engine"}) {
    }

    int Application::run() {
        while (!m_window.shouldClose()) {
            m_window.pollEvents();
            if (m_window.isKeyDown(Key::Escape))
                m_window.requestClose();
        }
        return 0;
    }
}
