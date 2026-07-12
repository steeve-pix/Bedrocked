#include "bedrocked/core/Application.hpp"
#include "bedrocked/input/Key.hpp"
#include "bedrocked/core/Logger.hpp"

#include <iostream>

#include <glad/glad.h>

namespace bedrocked {
    Application::Application() : m_window(WindowConfig{1280, 720, "Bedrocked Engine"}) {
    }

    int Application::run() {
        Logger::info("Bedrocked starting...");
        double accumulator{};
        int frameCount{};

        while (!m_window.shouldClose()) {
            const double deltaTime = m_timer.tick();

            m_window.pollEvents();

            if (m_window.isKeyDown(Key::Escape)) {
                m_window.requestClose();
            }

            glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            m_window.swapBuffers();

            accumulator += deltaTime;
            frameCount++;
            if (accumulator >= 1.0) {
                std::cout << "Loop rate: " << frameCount
                        << " iterations/s | Avg loop time: "
                        << (accumulator / frameCount) * 1'000.0
                        << " ms\n";
                accumulator -= 1.0;
                frameCount = 0;
            }
        }
        return 0;
    }
}
