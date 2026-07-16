#include "bedrocked/gameplay/BlockInteractor.hpp"

#include "bedrocked/world/World.hpp"
#include "bedrocked/world/block/BlockRaycast.hpp"
#include "bedrocked/gameplay/BlockInteractor.hpp"

#include "bedrocked/gameplay/Player.hpp"
#include "bedrocked/rendering/ChunkRenderer.hpp"
#include "bedrocked/world/World.hpp"
#include "bedrocked/world/block/BlockRaycast.hpp"
#include "bedrocked/world/chunk/ChunkManager.hpp"

namespace bedrocked {
    BlockInteractor::BlockInteractor(float reach) noexcept
        : m_reach{reach} {
    }

    void BlockInteractor::updateTarget(const World &world, Vector3 rayOrigin, Vector3 rayDirection) noexcept {
        const auto hit = raycastBlocks(
            world,
            rayOrigin,
            rayDirection,
            m_reach
        );

        if (!hit.has_value()) {
            m_targetedBlock.reset();
            m_adjacentBlock.reset();
            return;
        }

        m_targetedBlock = hit->block;
        m_adjacentBlock = hit->adjacentBlock;
    }

    const std::optional<BlockPosition> &
    BlockInteractor::targetedBlock() const noexcept {
        return m_targetedBlock;
    }

    void BlockInteractor::destroyTargetedBlock(World &world, ChunkManager &chunkManager, ChunkRenderer &chunkRenderer) {
        if (!m_targetedBlock.has_value()) {
            return;
        }

        const BlockPosition block =
                m_targetedBlock.value();

        world.setBlockAtWorld(
            block.x,
            block.y,
            block.z,
            BlockType::Air
        );

        chunkRenderer.rebuildAroundBlock(
            chunkManager,
            block
        );

        m_targetedBlock.reset();
        m_adjacentBlock.reset();
    }

    void BlockInteractor::placeBlock(World &world, ChunkManager &chunkManager, ChunkRenderer &chunkRenderer,
                                     const Player &player, BlockType type) {
        if (!m_adjacentBlock.has_value()) {
            return;
        }

        const BlockPosition block =
                m_adjacentBlock.value();

        if (world.blockAtWorld(
                block.x,
                block.y,
                block.z) != BlockType::Air) {
            return;
        }

        if (player.intersectsBlock(block)) {
            return;
        }

        world.setBlockAtWorld(
            block.x,
            block.y,
            block.z,
            type
        );

        chunkRenderer.rebuildAroundBlock(
            chunkManager,
            block
        );
    }
} // namespace bedrocked
