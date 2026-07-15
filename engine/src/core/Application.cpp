#include "bedrocked/core/Application.hpp"

#include "bedrocked/assets/Image.hpp"
#include "bedrocked/core/Logger.hpp"
#include "bedrocked/gameplay/Player.hpp"
#include "bedrocked/input/Key.hpp"
#include "bedrocked/math/Matrix4.hpp"
#include "bedrocked/math/Vector3.hpp"
#include "bedrocked/rendering/Camera.hpp"
#include "bedrocked/rendering/Mesh.hpp"
#include "bedrocked/rendering/opengl/ShaderProgram.hpp"
#include "bedrocked/rendering/opengl/Texture2D.hpp"
#include "bedrocked/world/World.hpp"
#include "bedrocked/world/chunk/ChunkManager.hpp"
#include "bedrocked/world/chunk/ChunkMesher.hpp"
#include "bedrocked/world/chunk/ChunkTransforms.hpp"

#include <iostream>
#include <memory>
#include <string_view>
#include <vector>

#include <glad/glad.h>

#include "bedrocked/world/block/BlockPosition.hpp"
#include "bedrocked/world/block/BlockRaycast.hpp"
#include "bedrocked/world/chunk/ChunkCoordinates.hpp"

#include <array>

namespace bedrocked {
    namespace {
        // RENDERING CONFIG
        constexpr float kPi = 3.14159265358979323846F;
        constexpr float kFieldOfView = 60.0f * kPi / 180.0f;

        constexpr float kNearClipPlane = 0.1f;
        constexpr float kFarClipPlane = 100.0f;

        constexpr float kMouseSensitivity = 0.002f;
        constexpr float kPlayerEyeHeight = 1.62f;

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

        struct RenderChunk {
            ChunkPosition position;
            Matrix4 model;
            std::unique_ptr<Mesh> mesh;
        };
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

        /*
         * Generate the world before building the GPU meshes.
         */
        World world{1234U};
        world.generateTestWorld();

        ChunkManager &chunkManager = world.chunks();

        const std::vector<ChunkPosition> positions =
                chunkManager.positions();

        /*
         * Every chunk has its own mesh and model matrix.
         *
         * There is deliberately no visual scene offset anymore.
         * Rendering and physics now use the same world coordinates.
         */
        std::vector<RenderChunk> renderChunks;
        renderChunks.reserve(positions.size());

        for (const ChunkPosition position: positions) {
            Chunk *chunk = chunkManager.chunkAt(position);

            if (chunk == nullptr) {
                continue;
            }

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
                .position = position,
                .model = chunkModelMatrix(position),
                .mesh = std::move(mesh)
            });
        }

        /*
         * Player position represents the center of the player’s feet.
         * Starting at Y = 15 lets gravity drop the player onto terrain.
         */
        Player player{
            Vector3{
                .x = 0.5F,
                .y = 15.0F,
                .z = 0.5F
            }
        };

        Camera camera;

        m_renderer.setClearColor(0.1F, 0.2F, 0.3F, 1.0F);

        m_window.setCursorCaptured(true);

        CursorPosition previousCursor =
                m_window.cursorPosition();

        shader.use();
        shader.setInt("blockTexture", 0);

        double statisticsElapsedTime{};
        int loopCount{};

        std::optional<BlockPosition> previousTarget;
        bool previousLeftMouseDown{};

        const auto rebuildChunkMesh = [&chunkManager, &renderChunks](ChunkPosition position) {
            Chunk *chunk =
                    chunkManager.chunkAt(position);

            if (chunk == nullptr) {
                return;
            }

            const ChunkNeighbors neighbors =
                    chunkManager.neighborsOf(position);

            const ChunkMeshData meshData =
                    buildChunkMeshData(*chunk, neighbors);

            for (RenderChunk &renderChunk: renderChunks) {
                if (renderChunk.position != position) {
                    continue;
                }

                if (meshData.empty()) {
                    renderChunk.mesh.reset();
                    return;
                }

                renderChunk.mesh =
                        std::make_unique<Mesh>(
                            meshData.vertices.data(),
                            meshData.vertices.size(),
                            meshData.indices.data(),
                            meshData.indices.size()
                        );

                return;
            }
        };

        while (!m_window.shouldClose()) {
            const double deltaTime = m_timer.tick();
            const float deltaTimeSeconds =
                    static_cast<float>(deltaTime);

            m_window.pollEvents();

            const bool leftMouseDown =
                    m_window.isMouseButtonDown(MouseButton::Left);

            const bool leftMousePressed =
                    leftMouseDown && !previousLeftMouseDown;

            previousLeftMouseDown = leftMouseDown;

            if (leftMousePressed) {
                std::cout << "Left mouse clicked\n";
            }

            /*
             * Mouse input controls only camera orientation.
             * Camera position will come from the Player.
             */
            const CursorPosition currentCursor =
                    m_window.cursorPosition();

            const double mouseDeltaX =
                    currentCursor.x - previousCursor.x;

            const double mouseDeltaY =
                    previousCursor.y - currentCursor.y;

            previousCursor = currentCursor;

            camera.rotate(
                -static_cast<float>(mouseDeltaX) * kMouseSensitivity,
                static_cast<float>(mouseDeltaY) * kMouseSensitivity
            );

            // KEYBOARD INPUT
            // Close with if Escape key is pressed
            if (m_window.isKeyDown(Key::Escape)) {
                m_window.requestClose();
            }

            float forwardInput{};
            float rightInput{};
            bool previousJumpKeyDown{};

            if (m_window.isKeyDown(Key::W)) {
                forwardInput += 1.0f;
            }
            if (m_window.isKeyDown(Key::S)) {
                forwardInput -= 1.0F;
            }

            if (m_window.isKeyDown(Key::D)) {
                rightInput += 1.0F;
            }

            if (m_window.isKeyDown(Key::A)) {
                rightInput -= 1.0F;
            }

            constexpr float playerSpeed = 4.0F;
            player.move(forwardInput, rightInput, camera.yaw(), playerSpeed);

            const bool jumpKeyDown =
                    m_window.isKeyDown(Key::Space);

            const bool jumpKeyPressed =
                    jumpKeyDown && !previousJumpKeyDown;

            previousJumpKeyDown = jumpKeyDown;

            if (jumpKeyPressed) {
                constexpr float jumpVelocity = 8.0F;
                player.jump(jumpVelocity);
            }

            /*
             * Gravity and vertical ground collision.
             */
            player.update(deltaTimeSeconds, world);

            /*
             * Attach the camera to the player’s eye position.
             */
            const Vector3 &playerPosition =
                    player.position();

            const Vector3 rayOrigin{
                .x = playerPosition.x,
                .y = playerPosition.y + kPlayerEyeHeight,
                .z = playerPosition.z
            };

            constexpr float interactionReach = 5.0F;

            const auto raycastHit = raycastBlocks(
                world, rayOrigin,
                camera.forwardDirection(),
                interactionReach
            );

            if (raycastHit.has_value()) {
                const BlockPosition &targetedBlock =
                        raycastHit->block;

                if (!previousTarget.has_value() ||
                    previousTarget.value() != targetedBlock) {
                    std::cout
                            << "Targeted block: "
                            << targetedBlock.x << ", "
                            << targetedBlock.y << ", "
                            << targetedBlock.z
                            << " | distance: "
                            << raycastHit->distance
                            << '\n';

                    previousTarget = targetedBlock;
                }
            } else if (previousTarget.has_value()) {
                std::cout << "No block targeted\n";
                previousTarget.reset();
            }

            if (leftMousePressed && raycastHit.has_value()) {
                const BlockPosition destroyedBlock =
                        raycastHit->block;

                world.setBlockAtWorld(
                    destroyedBlock.x,
                    destroyedBlock.y,
                    destroyedBlock.z,
                    BlockType::Air
                );

                const ChunkPosition affectedChunk =
                        chunkPositionForBlock(destroyedBlock);

                constexpr std::array neighborOffsets{
                    ChunkPosition{0, 0, 0},
                    ChunkPosition{1, 0, 0},
                    ChunkPosition{-1, 0, 0},
                    ChunkPosition{0, 1, 0},
                    ChunkPosition{0, -1, 0},
                    ChunkPosition{0, 0, 1},
                    ChunkPosition{0, 0, -1}
                };

                for (const ChunkPosition offset: neighborOffsets) {
                    rebuildChunkMesh({
                        .x = affectedChunk.x + offset.x,
                        .y = affectedChunk.y + offset.y,
                        .z = affectedChunk.z + offset.z
                    });
                }

                rebuildChunkMesh(affectedChunk);
            }

            camera.setPosition(
                playerPosition.x,
                playerPosition.y + kPlayerEyeHeight,
                playerPosition.z
            );

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

            /*
             * Render the world.
             */
            m_renderer.clear();

            glActiveTexture(GL_TEXTURE0);
            blockTexture.bind();

            shader.use();
            shader.setMat4("projection", projection.data());
            shader.setMat4("view", view.data());

            for (const RenderChunk &renderChunk: renderChunks) {
                if (renderChunk.mesh == nullptr) {
                    continue;
                }
                shader.setMat4("model", renderChunk.model.data());

                m_renderer.draw(*renderChunk.mesh);
            }

            m_window.swapBuffers();

            /*
             * Performance statistics.
             */
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
                        << "Loop rate: "
                        << loopsPerSecond
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
