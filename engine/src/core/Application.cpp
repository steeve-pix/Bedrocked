#include "bedrocked/core/Application.hpp"

#include "bedrocked/assets/Image.hpp"
#include "bedrocked/core/Logger.hpp"
#include "bedrocked/gameplay/Hotbar.hpp"
#include "bedrocked/gameplay/Player.hpp"
#include "bedrocked/input/Key.hpp"
#include "bedrocked/input/MouseButton.hpp"
#include "bedrocked/math/Matrix4.hpp"
#include "bedrocked/math/Vector3.hpp"
#include "bedrocked/rendering/Camera.hpp"
#include "bedrocked/rendering/ChunkRenderer.hpp"
#include "bedrocked/rendering/Mesh.hpp"
#include "bedrocked/rendering/opengl/ShaderProgram.hpp"
#include "bedrocked/rendering/opengl/Texture2D.hpp"
#include "bedrocked/ui/HotbarUI.hpp"
#include "bedrocked/ui/ImGuiLayer.hpp"
#include "bedrocked/world/World.hpp"
#include "bedrocked/world/block/BlockPosition.hpp"
#include "bedrocked/world/block/BlockRaycast.hpp"
#include "bedrocked/world/block/BlockType.hpp"
#include "bedrocked/world/chunk/ChunkManager.hpp"

#include <cstdint>
#include <iostream>
#include <iterator>
#include <optional>
#include <string_view>

#include <glad/glad.h>

namespace bedrocked {
    namespace {
        constexpr float kPi = 3.14159265358979323846F;

        constexpr float kFieldOfView =
                60.0F * kPi / 180.0F;

        constexpr float kNearClipPlane = 0.1F;
        constexpr float kFarClipPlane = 100.0F;

        constexpr float kMouseSensitivity = 0.002F;
        constexpr float kPlayerEyeHeight = 1.62F;
        constexpr float kPlayerSpeed = 4.0F;
        constexpr float kJumpVelocity = 8.0F;
        constexpr float kInteractionReach = 5.0F;

        constexpr double kStatisticsInterval = 10.0;

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

        constexpr std::string_view kOutlineVertexShaderSource = R"(
            #version 330 core

            layout(location = 0) in vec3 position;
            layout(location = 1) in vec3 color;

            uniform mat4 model;
            uniform mat4 view;
            uniform mat4 projection;

            out vec3 vertexColor;

            void main()
            {
                gl_Position =
                    projection * view * model * vec4(position, 1.0);

                vertexColor = color;
            }
        )";

        constexpr std::string_view kOutlineFragmentShaderSource = R"(
            #version 330 core

            in vec3 vertexColor;

            out vec4 fragmentColor;

            void main()
            {
                fragmentColor = vec4(vertexColor, 1.0);
            }
        )";

        constexpr Vertex kOutlineVertices[]{
            {{-0.5F, -0.5F, 0.5F}, {1.0F, 1.0F, 0.0F}, {0.0F, 0.0F}},
            {{0.5F, -0.5F, 0.5F}, {1.0F, 1.0F, 0.0F}, {0.0F, 0.0F}},
            {{0.5F, 0.5F, 0.5F}, {1.0F, 1.0F, 0.0F}, {0.0F, 0.0F}},
            {{-0.5F, 0.5F, 0.5F}, {1.0F, 1.0F, 0.0F}, {0.0F, 0.0F}},

            {{-0.5F, -0.5F, -0.5F}, {1.0F, 1.0F, 0.0F}, {0.0F, 0.0F}},
            {{0.5F, -0.5F, -0.5F}, {1.0F, 1.0F, 0.0F}, {0.0F, 0.0F}},
            {{0.5F, 0.5F, -0.5F}, {1.0F, 1.0F, 0.0F}, {0.0F, 0.0F}},
            {{-0.5F, 0.5F, -0.5F}, {1.0F, 1.0F, 0.0F}, {0.0F, 0.0F}}
        };

        constexpr std::uint32_t kOutlineIndices[]{
            // Front
            0, 1,
            1, 2,
            2, 3,
            3, 0,

            // Back
            4, 5,
            5, 6,
            6, 7,
            7, 4,

            // Connections
            0, 4,
            1, 5,
            2, 6,
            3, 7
        };
    } // namespace

    Application::Application()
        : m_window{WindowConfig{1280, 720, "Bedrocked Engine"}} {
    }

    int Application::run() {
        Logger::info("Bedrocked starting...");

        ShaderProgram worldShader{
            kVertexShaderSource,
            kFragmentShaderSource
        };

        ShaderProgram outlineShader{
            kOutlineVertexShaderSource,
            kOutlineFragmentShaderSource
        };

        Image blockImage{
            "assets/textures/block_atlas.png"
        };

        Texture2D blockTexture{
            blockImage.width(),
            blockImage.height(),
            blockImage.pixels()
        };

        World world{1234U};
        world.generateTestWorld();

        ChunkManager &chunkManager =
                world.chunks();

        ChunkRenderer chunkRenderer;
        chunkRenderer.build(chunkManager);

        Mesh selectionOutline{
            kOutlineVertices, std::size(kOutlineVertices), kOutlineIndices, std::size(kOutlineIndices)
        };

        Player player{
            Vector3{
                .x = 0.5F,
                .y = 15.0F,
                .z = 0.5F
            }
        };

        Camera camera;
        Hotbar hotbar;
        HotbarUI hotbarUI;

        ImGuiLayer imguiLayer{m_window.nativeHandle()};

        m_renderer.setClearColor(0.1F, 0.2F, 0.3F, 1.0F);

        m_window.setCursorCaptured(true);

        CursorPosition previousCursor =
                m_window.cursorPosition();

        worldShader.use();
        worldShader.setInt("blockTexture", 0);

        std::optional<BlockPosition> previousTarget;

        bool previousLeftMouseDown{};
        bool previousRightMouseDown{};
        bool previousJumpKeyDown{};

        double statisticsElapsedTime{};
        int loopCount{};

        while (!m_window.shouldClose()) {
            const double deltaTime =
                    m_timer.tick();

            const float deltaTimeSeconds =
                    static_cast<float>(deltaTime);

            m_window.pollEvents();

            /*
             * Mouse-button edge detection.
             */
            const bool leftMouseDown =
                    m_window.isMouseButtonDown(MouseButton::Left);

            const bool leftMousePressed =
                    leftMouseDown && !previousLeftMouseDown;

            previousLeftMouseDown = leftMouseDown;

            const bool rightMouseDown =
                    m_window.isMouseButtonDown(MouseButton::Right);

            const bool rightMousePressed =
                    rightMouseDown && !previousRightMouseDown;

            previousRightMouseDown = rightMouseDown;

            /*
             * Camera orientation.
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

            if (m_window.isKeyDown(Key::Escape)) {
                m_window.requestClose();
            }

            /*
             * Hotbar selection.
             */
            if (m_window.isKeyDown(Key::Digit1)) {
                hotbar.select(0);
            }

            if (m_window.isKeyDown(Key::Digit2)) {
                hotbar.select(1);
            }

            if (m_window.isKeyDown(Key::Digit3)) {
                hotbar.select(2);
            }

            if (m_window.isKeyDown(Key::Digit4)) {
                hotbar.select(3);
            }

            if (m_window.isKeyDown(Key::Digit5)) {
                hotbar.select(4);
            }

            /*
             * Player movement input.
             */
            float forwardInput{};
            float rightInput{};

            if (m_window.isKeyDown(Key::W)) {
                forwardInput += 1.0F;
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

            player.move(
                forwardInput,
                rightInput,
                camera.yaw(),
                kPlayerSpeed
            );

            /*
             * Jump press-edge detection.
             */
            const bool jumpKeyDown =
                    m_window.isKeyDown(Key::Space);

            const bool jumpKeyPressed =
                    jumpKeyDown && !previousJumpKeyDown;

            previousJumpKeyDown = jumpKeyDown;

            if (jumpKeyPressed) {
                player.jump(kJumpVelocity);
            }

            player.update(deltaTimeSeconds, world);

            const Vector3 &playerPosition =
                    player.position();

            camera.setPosition(playerPosition.x, playerPosition.y + kPlayerEyeHeight, playerPosition.z);

            /*
             * Block targeting.
             */
            const Vector3 rayOrigin{
                .x = playerPosition.x,
                .y = playerPosition.y + kPlayerEyeHeight,
                .z = playerPosition.z
            };

            const auto raycastHit = raycastBlocks(world, rayOrigin, camera.forwardDirection(), kInteractionReach);

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

            /*
             * Destroy targeted block.
             */
            if (leftMousePressed &&
                raycastHit.has_value()) {
                const BlockPosition destroyedBlock =
                        raycastHit->block;

                world.setBlockAtWorld(
                    destroyedBlock.x,
                    destroyedBlock.y,
                    destroyedBlock.z,
                    BlockType::Air
                );

                chunkRenderer.rebuildAroundBlock(chunkManager, destroyedBlock);
            }

            /*
             * Place a block in the adjacent empty cell.
             */
            if (rightMousePressed && raycastHit.has_value()) {
                const BlockPosition placementPosition =
                        raycastHit->adjacentBlock;

                const BlockType existingBlock =
                        world.blockAtWorld(
                            placementPosition.x,
                            placementPosition.y,
                            placementPosition.z
                        );

                if (existingBlock == BlockType::Air &&
                    !player.intersectsBlock(placementPosition)) {
                    world.setBlockAtWorld(
                        placementPosition.x,
                        placementPosition.y,
                        placementPosition.z,
                        hotbar.selectedBlock()
                    );

                    chunkRenderer.rebuildAroundBlock(chunkManager, placementPosition);
                }
            }

            /*
             * Do not begin an ImGui frame if rendering will be skipped.
             */
            const FrameBufferSize framebufferSize =
                    m_window.framebufferSize();

            if (framebufferSize.width <= 0 ||
                framebufferSize.height <= 0) {
                continue;
            }

            imguiLayer.beginFrame();
            m_renderer.setViewPort(framebufferSize.width, framebufferSize.height);

            const float aspectRatio =
                    static_cast<float>(framebufferSize.width) / static_cast<float>(framebufferSize.height);

            const Matrix4 projection = Matrix4::perspective(kFieldOfView, aspectRatio, kNearClipPlane, kFarClipPlane);

            const Matrix4 view = camera.viewMatrix();

            /*
             * Render the voxel world.
             */
            m_renderer.clear();

            glActiveTexture(GL_TEXTURE0);
            blockTexture.bind();

            worldShader.use();

            worldShader.setMat4("projection", projection.data());

            worldShader.setMat4("view", view.data());

            chunkRenderer.draw(m_renderer, worldShader);

            /*
             * Render the targeted-block outline.
             */
            if (raycastHit.has_value()) {
                const BlockPosition &target =
                        raycastHit->block;

                constexpr float outlineScale = 1.002F;

                const Matrix4 outlineModel =
                        Matrix4::translation(
                            static_cast<float>(target.x) + 0.5F,
                            static_cast<float>(target.y) + 0.5F,
                            static_cast<float>(target.z) + 0.5F
                        ) *
                        Matrix4::scale(
                            outlineScale,
                            outlineScale,
                            outlineScale
                        );

                outlineShader.use();

                outlineShader.setMat4("projection", projection.data());

                outlineShader.setMat4("view", view.data());

                outlineShader.setMat4("model", outlineModel.data());

                m_renderer.drawLines(selectionOutline);
            }

            /*
             * Render UI after the 3D scene.
             */
            hotbarUI.draw(hotbar);
            imguiLayer.endFrame();

            m_window.swapBuffers();

            /*
             * Performance statistics.
             */
            statisticsElapsedTime += deltaTime;

            ++loopCount;

            if (statisticsElapsedTime >=
                kStatisticsInterval) {
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
