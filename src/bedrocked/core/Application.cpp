#include "bedrocked/core/Application.hpp"
#include "bedrocked/input/Key.hpp"
#include "bedrocked/math/Matrix4.hpp"
#include "bedrocked/core/Logger.hpp"
#include "bedrocked/rendering/ShaderProgram.hpp"
#include "bedrocked/rendering/Camera.hpp"
#include "bedrocked/rendering/Mesh.hpp"
#include "bedrocked/rendering/Vertex.hpp"

#include <iterator>
#include <string_view>
#include <iostream>
#include <cstdint>


namespace bedrocked {
    namespace {
        // RENDERING CONFIG
        constexpr float kPi = 3.14159265358979323846F;
        constexpr float kFieldOfView = 60.0f * kPi / 180.0f;

        constexpr float kNearClipPlane = 0.1f;
        constexpr float kFarClipPlane = 100.0f;

        constexpr float kRotationSpeed = 1.0f;
        constexpr float kCameraSpeed = 2.0f;
        constexpr float kMouseSensitivity = 0.002f;

        constexpr double kStatisticsInterval = 10.0;

        // SHADERS
        constexpr std::string_view kVertexShaderSource = R"(
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

        constexpr std::string_view kFragmentShaderSource = R"(
            #version 330 core

            in vec3 vertexColor;

            out vec4 fragmentColor;

            void main()
            {
                fragmentColor = vec4(vertexColor, 1.0);
            }
        )";

        // CUBE GEOMETRY
        constexpr Vertex kCubeVertices[]{
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

        constexpr std::uint32_t kCubeIndices[]{
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
    } // namespace

    Application::Application()
        : m_window(WindowConfig{1280, 720, "Bedrocked Engine"}) {
    }

    int Application::run() {
        Logger::info("Bedrocked starting...");

        ShaderProgram shader(kVertexShaderSource, kFragmentShaderSource);

        Mesh cube{kCubeVertices, std::size(kCubeVertices), kCubeIndices, std::size(kCubeIndices)};

        Camera camera;

        // Move the camera away from the world origin so the cube is visible
        camera.move(0.0F, 0.0F, 1.0F);

        m_renderer.setClearColor(0.1F, 0.2F, 0.3F, 1.0F);

        m_window.setCursorCaptured(true);

        CursorPosition previousCursor =
                m_window.cursorPosition();

        float rotationAngle{};

        double statisticsElapsedTime = 0.0;
        int loopCount = 0;

        while (!m_window.shouldClose()) {
            const double deltaTime = m_timer.tick();
            const float deltaTimeSeconds = static_cast<float>(deltaTime);

            m_window.pollEvents();

            // MOUSE INPUT
            const CursorPosition currentCursor =
                    m_window.cursorPosition();

            const double mouseDeltaX =
                    currentCursor.x - previousCursor.x;

            const double mouseDeltaY =
                    previousCursor.y - currentCursor.y;

            previousCursor = currentCursor;

            camera.rotate(
                static_cast<float>(mouseDeltaX) * kMouseSensitivity,
                static_cast<float>(mouseDeltaY) * kMouseSensitivity
            );

            // KEYBOARD INPUT
            // Close with if Escape key is pressed
            if (m_window.isKeyDown(Key::Escape)) {
                m_window.requestClose();
            }

            const float movementDistance =
                    kCameraSpeed * deltaTimeSeconds;

            if (m_window.isKeyDown(Key::W)) {
                camera.moveForward(movementDistance);
            }

            if (m_window.isKeyDown(Key::S)) {
                camera.moveForward(-movementDistance);
            }
            if (m_window.isKeyDown(Key::A)) {
                camera.moveRight(-movementDistance);
            }

            if (m_window.isKeyDown(Key::D)) {
                camera.moveRight(movementDistance);
            }

            if (m_window.isKeyDown(Key::Space)) {
                camera.move(0.0f, movementDistance, 0.0f);
            }

            if (m_window.isKeyDown(Key::LeftShift)) {
                camera.move(0.0f, -movementDistance, 0.0f);
            }

            // FRAMEBUFFER AND PROJECTION
            const FrameBufferSize framebufferSize =
                    m_window.framebufferSize();

            if (framebufferSize.width <= 0 ||
                framebufferSize.height <= 0) {
                continue;
            }

            m_renderer.setViewPort(framebufferSize.width,
                                   framebufferSize.height);

            const float aspectRatio =
                    static_cast<float>(framebufferSize.width) / static_cast<float>(framebufferSize.height);

            const Matrix4 projection = Matrix4::perspective(kFieldOfView, aspectRatio, kNearClipPlane, kFarClipPlane);


            // SCENE TRANSFORMS
            rotationAngle +=
                    kRotationSpeed * static_cast<float>(deltaTimeSeconds);

            const Matrix4 model =
                    Matrix4::translation(0.0F, 0.0F, -3.0F) *
                    Matrix4::rotationY(rotationAngle) *
                    Matrix4::rotationX(rotationAngle * 0.5F);

            const Matrix4 view = camera.viewMatrix();

            // RENDERING
            m_renderer.clear();

            shader.use();
            shader.setMat4("projection", projection.data());
            shader.setMat4("view", view.data());
            shader.setMat4("model", model.data());

            m_renderer.draw(cube);
            m_window.swapBuffers();

            // PERFORMANCE STATS
            statisticsElapsedTime += deltaTime;
            ++loopCount;

            if (statisticsElapsedTime >= kStatisticsInterval) {
                const double loopsPerSecond =
                        static_cast<double>(loopCount) /
                        statisticsElapsedTime;

                const double averageLoopTimeMilliseconds =
                        statisticsElapsedTime /
                        static_cast<double>(loopCount) *
                        1'000.0;
                std::cout
                        << "Loop rate: " << loopsPerSecond
                        << " iterations/s | Average loop time: "
                        << averageLoopTimeMilliseconds
                        << " ms\n";

                statisticsElapsedTime -= kStatisticsInterval;
                loopCount = 0;
            }
        }

        return 0;
    }
} // namespace bedrocked
