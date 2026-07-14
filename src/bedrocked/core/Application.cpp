#include "bedrocked/core/Application.hpp"
#include "bedrocked/input/Key.hpp"
#include "bedrocked/math/Matrix4.hpp"
#include "bedrocked/core/Logger.hpp"
#include "bedrocked/rendering/ShaderProgram.hpp"

#include <iterator>
#include <string_view>
#include <iostream>
#include <cstdint>

#include "bedrocked/rendering/Camera.hpp"


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
            uniform mat4 view;
            uniform mat4 projection;

            out vec3 vertexColor;

            void main()
            {
                gl_Position = projection * view * model * vec4(position, 1.0);
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
            // Front: z = 0.5
            {{-0.5F, -0.5F, 0.5F}, {1.0F, 0.0F, 0.0F}},
            {{0.5F, -0.5F, 0.5F}, {0.0F, 1.0F, 0.0F}},
            {{0.5F, 0.5F, 0.5F}, {0.0F, 0.0F, 1.0F}},
            {{-0.5F, 0.5F, 0.5F}, {1.0F, 1.0F, 0.0F}},

            // Back: z = -0.5
            {{-0.5F, -0.5F, -0.5F}, {1.0F, 0.0F, 1.0F}},
            {{0.5F, -0.5F, -0.5F}, {0.0F, 1.0F, 1.0F}},
            {{0.5F, 0.5F, -0.5F}, {1.0F, 1.0F, 1.0F}},
            {{-0.5F, 0.5F, -0.5F}, {0.3F, 0.3F, 0.3F}}
        };

        constexpr std::uint32_t indices[]{
            // Front
            0, 1, 2,
            0, 2, 3,

            // Back
            5, 4, 7,
            5, 7, 6,

            // Left
            4, 0, 3,
            4, 3, 7,

            // Right
            1, 5, 6,
            1, 6, 2,

            // Top
            3, 2, 6,
            3, 6, 7,

            // Bottom
            4, 5, 1,
            4, 1, 0
        };
        constexpr float pi = 3.14159265358979323846F;

        Mesh cube{vertices, std::size(vertices), indices, std::size(indices)};

        float rotationAngle{};

        Camera camera;

        camera.move(0.0F, 0.0F, 1.0F);

        while (!m_window.shouldClose()) {
            const double deltaTime = m_timer.tick();
            constexpr float rotationSpeed = 1.0F; // radians per second

            rotationAngle +=
                    rotationSpeed * static_cast<float>(deltaTime);

            const Matrix4 model =
                    Matrix4::translation(0.0F, 0.0F, -3.0F) *
                    Matrix4::rotationY(rotationAngle) *
                    Matrix4::rotationX(rotationAngle * 0.5F);

            m_window.pollEvents();

            constexpr float cameraSpeed = 2.0f;
            const float movement = cameraSpeed * static_cast<float>(deltaTime);

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

            if (m_window.isKeyDown(Key::W)) {
                camera.move(0.0f, 0.0f, -movement);
            }

            if (m_window.isKeyDown(Key::S)) {
                camera.move(0.0f, 0.0f, movement);
            }
            if (m_window.isKeyDown(Key::A)) {
                camera.move(-movement, 0.0f, 0.0f);
            }

            if (m_window.isKeyDown(Key::D)) {
                camera.move(movement, 0.0f, 0.0f);
            }

            m_renderer.clear();

            const Matrix4 view = camera.viewMatrix();

            shader.use();
            shader.setMat4("projection", projection.data());
            shader.setMat4("view", view.data());
            shader.setMat4("model", model.data());
            m_renderer.draw(cube);

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
