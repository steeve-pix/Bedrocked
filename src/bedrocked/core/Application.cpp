#include "bedrocked/core/Application.hpp"
#include "bedrocked/input/Key.hpp"
#include "bedrocked/core/Logger.hpp"
#include "bedrocked/rendering/ShaderProgram.hpp"

#include <string_view>
#include <iostream>

#include <glad/glad.h>

namespace bedrocked {
    Application::Application() : m_window(WindowConfig{1280, 720, "Bedrocked Engine"}) {
    }

    int Application::run() {
        Logger::info("Bedrocked starting...");
        double accumulator{};
        int frameCount{};

        constexpr std::string_view vertexSource = R"(
            #version 330 core

            layout(location = 0) in vec3 position;

            void main()
            {
                gl_Position = vec4(position, 1.0);
            }
        )";

        constexpr std::string_view fragmentSource = R"(
            #version 330 core

            out vec4 fragmentColor;

            void main()
            {
                fragmentColor = vec4(1.0, 0.5, 0.2, 1.0);
            }
        )";

        ShaderProgram shader(vertexSource, fragmentSource);
        shader.use();

        m_renderer.setClearColor(0.1F, 0.2F, 0.3F, 1.0F);

        constexpr float vertices[]{
            0.0f, 0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f
        };

        Mesh triangle{vertices, sizeof(vertices), 3};

        while (!m_window.shouldClose()) {
            const double deltaTime = m_timer.tick();

            m_window.pollEvents();

            if (m_window.isKeyDown(Key::Escape)) {
                m_window.requestClose();
            }

            m_renderer.clear();

            shader.use();
            m_renderer.draw(triangle);

            m_window.swapBuffers();

            accumulator += deltaTime;
            ++frameCount;
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
