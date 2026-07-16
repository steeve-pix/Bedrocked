#pragma once

#include "bedrocked/world/block/BlockType.hpp"
#include "bedrocked/world/block/BlockPosition.hpp"

#include <optional>

namespace bedrocked {
    class Camera;
    class ChunkManager;
    class ChunkRenderer;
    class Player;
    class World;
    struct BlockPosition;
    struct Vector3;

    class BlockInteractor final {
    public:
        explicit BlockInteractor(float reach) noexcept;

        void updateTarget(
            const World &world,
            Vector3 rayOrigin,
            Vector3 rayDirection) noexcept;

        void destroyTargetedBlock(
            World &world,
            ChunkManager &chunkManager,
            ChunkRenderer &chunkRenderer);

        void placeBlock(
            World &world,
            ChunkManager &chunkManager,
            ChunkRenderer &chunkRenderer,
            const Player &player,
            BlockType type);

        [[nodiscard]] const std::optional<BlockPosition> &
        targetedBlock() const noexcept;

    private:
        float m_reach;
        std::optional<BlockPosition> m_targetedBlock;
        std::optional<BlockPosition> m_adjacentBlock;
    };
} // namespace bedrocked
