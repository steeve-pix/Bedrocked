#include "bedrocked/core/Application.hpp"
#include "bedrocked/input/Key.hpp"
#include "bedrocked/math/Matrix4.hpp"
#include "bedrocked/core/Logger.hpp"
#include "bedrocked/rendering/ShaderProgram.hpp"

#include <iterator>
#include <string_view>
#include <iostream>
#include <cstdint>


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
            layout(location = 1) in vec3 color;

            uniform mat4 model;
            uniform mat4 projection;

            out vec3 vertexColor;

            void main()
            {
                gl_Position = projection * model * vec4(position, 1.0);
                vertexColor = color;
            }
        )";

        constexpr std::string_view fragmentSource = R"(
            #version 330 core

            in vec3 vertexColor;

            out vec4 fragmentColor;

            void main()
            {
                fragmentColor = vec4(vertexColor, 1.0);
            }
        )";

        ShaderProgram shader(vertexSource, fragmentSource);
        shader.use();

        m_renderer.setClearColor(0.1F, 0.2F, 0.3F, 1.0F);

        constexpr Vertex vertices[]{
            {
                .position = {-0.5F, 0.5F, 0.0F},
                .color = {1.0F, 0.0F, 0.0F}
            },
            {
                .position = {-0.5F, -0.5F, 0.0F},
                .color = {0.0F, 1.0F, 0.0F}
            },
            {
                .position = {0.5F, -0.5F, 0.0F},
                .color = {0.0F, 0.0F, 1.0F}
            },
            {
                .position = {0.5F, 0.5F, 0.0F},
                .color = {1.0F, 1.0F, 0.0F}
            }
        };

        constexpr std::uint32_t indices[]{
            0, 1, 2,
            0, 2, 3
        };
        constexpr float pi = 3.14159265358979323846F;

        const Matrix4 translation =
                Matrix4::translation(0.0F, 0.0F, -2.0F);

        const Matrix4 rotation =
                Matrix4::rotationZ(pi / 4.0F);

        const Matrix4 scaling =
                Matrix4::scale(0.5F, 0.5F, 1.0F);

        const Matrix4 model =
                translation * rotation * scaling;

        Mesh square{vertices, std::size(vertices), indices, std::size(indices)};

        const Matrix4 nearModel = Matrix4::translation(-0.15f, 0.0f, -2.0f) * Matrix4::rotationZ(pi / 4.0f) *
                                  Matrix4::scale(0.5f, 0.5f, 0.5f);
        const Matrix4 farModel = Matrix4::translation(0.15f, 0.0f, -3.0f) * Matrix4::rotationZ(pi / 4.0f) *
                                 Matrix4::scale(0.7f, 0.7f, 0.7f);

        while (!m_window.shouldClose()) {
            const double deltaTime = m_timer.tick();

            m_window.pollEvents();

            const FrameBufferSize framebufferSize = m_window.framebufferSize();
            if (framebufferSize.width <= 0 || framebufferSize.height <= 0) {
                continue;
            }
            m_renderer.setViewPort(framebufferSize.width, framebufferSize.height);

            const float aspectRatio =
                    static_cast<float>(framebufferSize.width) / static_cast<float>(framebufferSize.height);

            constexpr float fieldOfView = 60.0f * pi / 180.0f;
            const Matrix4 projection = Matrix4::perspective(fieldOfView, aspectRatio, 0.1F, 100.0F);;


            // Close with if Escape key is pressed
            if (m_window.isKeyDown(Key::Escape)) {
                m_window.requestClose();
            }

            m_renderer.clear();

            shader.use();
            shader.setMat4("projection", projection.data());
            shader.setMat4("model", nearModel.data());
            m_renderer.draw(square);

            shader.setMat4("model", farModel.data());
            m_renderer.draw(square);

            m_window.swapBuffers();

            accumulator += deltaTime;
            ++frameCount;
            if (accumulator >= 10.0) {
                std::cout << "Loop rate: " << frameCount
                        << " iterations/s | Avg loop time: "
                        << (accumulator / frameCount) * 1'000.0
                        << " ms\n";
                accumulator -= 10.0;
                frameCount = 0;
            }
        }
        return 0;
    }
}
