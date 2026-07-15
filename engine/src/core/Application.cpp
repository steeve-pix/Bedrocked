#include "bedrocked/core/Application.hpp"

#include "bedrocked/assets/Image.hpp"
#include "bedrocked/core/Logger.hpp"
#include "bedrocked/input/Key.hpp"
#include "bedrocked/math/Matrix4.hpp"
#include "bedrocked/rendering/Camera.hpp"
#include "bedrocked/rendering/Mesh.hpp"
#include "bedrocked/rendering/opengl/ShaderProgram.hpp"
#include "bedrocked/rendering/opengl/Texture2D.hpp"
#include "bedrocked/world/chunk/Chunk.hpp"
#include "bedrocked/world/chunk/ChunkMesher.hpp"
#include "bedrocked/world/chunk/ChunkNeighbors.hpp"
#include "bedrocked/world/chunk/ChunkPosition.hpp"
#include "bedrocked/world/chunk/ChunkTransforms.hpp"
#include "bedrocked/world/World.hpp"
#include "bedrocked/world/chunk/ChunkManager.hpp"

#include <cassert>
#include <iostream>
#include <string_view>
#include <memory>
#include <vector>

#include <glad/glad.h>

namespace bedrocked {
    namespace {
        // RENDERING CONFIG
        constexpr float kPi = 3.14159265358979323846F;
        constexpr float kFieldOfView = 60.0f * kPi / 180.0f;

        constexpr float kNearClipPlane = 0.1f;
        constexpr float kFarClipPlane = 100.0f;

        constexpr float kCameraSpeed = 2.0f;
        constexpr float kMouseSensitivity = 0.002f;

        constexpr double kStatisticsInterval = 10.0;

        // SHADERS
        constexpr std::string_view kVertexShaderSource = R"(
            #version 330 core

            layout(location = 0) in vec3 position;
            layout(location = 1) in vec3 color;
            layout(location = 2) in vec2 textureCoordinates;

            uniform mat4 model;
            uniform mat4 view;
            uniform mat4 projection;

            out vec3 vertexColor;
            out vec2 vertexTextureCoordinates;

            void main()
            {
                gl_Position =
                    projection * view * model * vec4(position, 1.0);

                vertexColor = color;
                vertexTextureCoordinates = textureCoordinates;
            }
        )";

        constexpr std::string_view kFragmentShaderSource = R"(
            #version 330 core

            in vec3 vertexColor;
            in vec2 vertexTextureCoordinates;

            uniform sampler2D blockTexture;

            out vec4 fragmentColor;

            void main()
            {
                fragmentColor =
                    texture(blockTexture, vertexTextureCoordinates);
            }
        )";
    } // namespace

    Application::Application()
        : m_window{WindowConfig{1280, 720, "Bedrocked Engine"}} {
    }

    int Application::run() {
        Logger::info("Bedrocked starting...");

        ShaderProgram shader{
            kVertexShaderSource,
            kFragmentShaderSource
        };

        Image blockImage{"assets/textures/block_atlas.png"};

        Texture2D blockTexture{
            blockImage.width(),
            blockImage.height(),
            blockImage.pixels()
        };

        World world;
        world.generateTestWorld();

        ChunkManager &chunkManager = world.chunks();

        const std::vector<ChunkPosition> positions =
                chunkManager.positions();

        assert(positions.size() == 9);

        struct RenderChunk {
            Matrix4 model;
            std::unique_ptr<Mesh> mesh;
        };

        std::vector<RenderChunk> renderChunks;
        renderChunks.reserve(positions.size());

        const Matrix4 sceneOffset =
                Matrix4::translation(
                    -8.0F,
                    -2.0F,
                    -40.0F
                );

        for (const ChunkPosition position: positions) {
            Chunk *chunk = chunkManager.chunkAt(position);

            assert(chunk != nullptr);

            const ChunkNeighbors neighbors =
                    chunkManager.neighborsOf(position);

            const ChunkMeshData meshData =
                    buildChunkMeshData(*chunk, neighbors);

            if (meshData.empty()) {
                continue;
            }

            auto mesh = std::make_unique<Mesh>(
                meshData.vertices.data(),
                meshData.vertices.size(),
                meshData.indices.data(),
                meshData.indices.size()
            );

            renderChunks.push_back({
                .model =
                sceneOffset *
                chunkModelMatrix(position),

                .mesh = std::move(mesh)
            });
        }

        Camera camera;

        m_renderer.setClearColor(0.1F, 0.2F, 0.3F, 1.0F);

        m_window.setCursorCaptured(true);

        CursorPosition previousCursor =
                m_window.cursorPosition();

        shader.use();
        shader.setInt("blockTexture", 0);

        double statisticsElapsedTime{};
        int loopCount{};

        while (!m_window.shouldClose()) {
            const double deltaTime = m_timer.tick();
            const float deltaTimeSeconds =
                    static_cast<float>(deltaTime);

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

            const Matrix4 view = camera.viewMatrix();

            // RENDERING
            m_renderer.clear();

            glActiveTexture(GL_TEXTURE0);
            blockTexture.bind();

            shader.use();
            shader.setMat4("projection", projection.data());
            shader.setMat4("view", view.data());

            for (const RenderChunk &renderChunk: renderChunks) {
                shader.setMat4("model", renderChunk.model.data());

                m_renderer.draw(*renderChunk.mesh);
            }

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

                statisticsElapsedTime = 0.0;
                loopCount = 0;
            }
        }

        return 0;
    }
} // namespace bedrocked
