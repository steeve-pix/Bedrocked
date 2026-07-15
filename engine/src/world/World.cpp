#include "bedrocked/world/World.hpp"
#include "bedrocked/world/generation/TreePlacement.hpp"


namespace {
    [[nodiscard]] constexpr int floorDivide(int value, int divisor) noexcept {
        int quotient = value / divisor;
        const int remainder = value % divisor;

        // C++ integer division truncates toward zero.
        if (remainder != 0 && ((remainder < 0) != (divisor < 0))) {
            --quotient;
        }

        return quotient;
    }

    [[nodiscard]] constexpr int positiveModulo(int value, int divisor) noexcept {
        const int remainder = value % divisor;

        return remainder < 0 ? remainder + divisor : remainder;
    }
} // namespace

namespace bedrocked {
    World::World(std::uint32_t seed) noexcept
        : m_seed{seed}, m_terrainGenerator{seed} {
    }

    void World::generateTestWorld() {
        constexpr int minimumChunkX = -1;
        constexpr int maximumChunkX = 1;
        constexpr int minimumChunkZ = -1;
        constexpr int maximumChunkZ = 1;

        /*
         * Phase 1:
         * Create every chunk before generating structures.
         *
         * This is important because trees near chunk boundaries may place
         * leaves or trunk blocks inside neighboring chunks.
         */
        for (int chunkZ = minimumChunkZ; chunkZ <= maximumChunkZ; ++chunkZ) {
            for (int chunkX = minimumChunkX; chunkX <= maximumChunkX; ++chunkX) {
                const ChunkPosition position{.x = chunkX, .y = 0, .z = chunkZ};

                Chunk &chunk = m_chunkManager.createChunk(position);

                m_terrainGenerator.generate(chunk, position);
            }
        }

        /*
         * Phase 2:
         * Place deterministic trees after all terrain chunks exist.
         */
        const int minimumWorldX =
                minimumChunkX * static_cast<int>(Chunk::Width);

        const int maximumWorldX =
                (maximumChunkX + 1) * static_cast<int>(Chunk::Width) - 1;

        const int minimumWorldZ =
                minimumChunkZ * static_cast<int>(Chunk::Depth);

        const int maximumWorldZ =
                (maximumChunkZ + 1) * static_cast<int>(Chunk::Depth) - 1;

        constexpr int treeMargin = 2;
        constexpr int trunkHeight = 4;
        constexpr int canopyHeight = 2;

        for (int worldZ = minimumWorldZ + treeMargin; worldZ <= maximumWorldZ - treeMargin; ++worldZ) {
            for (int worldX = minimumWorldX + treeMargin; worldX <= maximumWorldX - treeMargin; ++worldX) {
                if (!shouldPlaceTree(worldX, worldZ, m_seed)) {
                    continue;
                }

                const int surfaceHeight =
                        m_terrainGenerator.surfaceHeightAt(worldX, worldZ);

                // Skip trees that would exceed its vertical limits.
                if (surfaceHeight + trunkHeight + canopyHeight >= static_cast<int>(Chunk::Height)) {
                    continue;
                }

                // Generate the trunk above the terrain surface.
                for (int offsetY = 1; offsetY <= trunkHeight; ++offsetY) {
                    setBlockAtWorld(worldX, surfaceHeight + offsetY, worldZ, BlockType::Wood);
                }

                const int trunkTopY =
                        surfaceHeight + trunkHeight;

                // Two 3×3 leaf layers around the upper trunk.
                for (int leafY = trunkTopY - 1; leafY <= trunkTopY; ++leafY) {
                    for (int offsetZ = -1; offsetZ <= 1; ++offsetZ) {
                        for (int offsetX = -1; offsetX <= 1; ++offsetX) {
                            // Preserve the wooden trunk in the center.
                            if (offsetX == 0 && offsetZ == 0) {
                                continue;
                            }

                            setBlockAtWorld(worldX + offsetX, leafY, worldZ + offsetZ, BlockType::Leaves
                            );
                        }
                    }
                }

                // Add a small crown above the canopy.
                setBlockAtWorld(worldX, trunkTopY + 1, worldZ, BlockType::Leaves);
            }
        }
    }

    ChunkManager &World::chunks() noexcept {
        return m_chunkManager;
    }

    const ChunkManager &World::chunks() const noexcept {
        return m_chunkManager;
    }

    void World::setBlockAtWorld(int worldX, int worldY, int worldZ, BlockType type) noexcept {
        constexpr int chunkWidth =
                static_cast<int>(Chunk::Width);

        constexpr int chunkHeight =
                static_cast<int>(Chunk::Height);

        constexpr int chunkDepth =
                static_cast<int>(Chunk::Depth);

        const ChunkPosition chunkPosition{
            .x = floorDivide(worldX, chunkWidth),
            .y = floorDivide(worldY, chunkHeight),
            .z = floorDivide(worldZ, chunkDepth)
        };

        Chunk *chunk =
                m_chunkManager.chunkAt(chunkPosition);

        // The requested world coordinate belongs to an unloaded chunk.
        if (chunk == nullptr) {
            return;
        }

        const int localX =
                positiveModulo(worldX, chunkWidth);

        const int localY =
                positiveModulo(worldY, chunkHeight);

        const int localZ =
                positiveModulo(worldZ, chunkDepth);

        chunk->setBlock(localX, localY, localZ, type);
    }

    BlockType World::blockAtWorld(int worldX, int worldY, int worldZ) const noexcept {
        constexpr int chunkWidth =
                static_cast<int>(Chunk::Width);

        constexpr int chunkHeight =
                static_cast<int>(Chunk::Height);

        constexpr int chunkDepth =
                static_cast<int>(Chunk::Depth);

        const ChunkPosition chunkPosition{
            .x = floorDivide(worldX, chunkWidth),
            .y = floorDivide(worldY, chunkHeight),
            .z = floorDivide(worldZ, chunkDepth)
        };

        const Chunk *chunk =
                m_chunkManager.chunkAt(chunkPosition);

        // Unloaded world space currently behaves as air.
        if (chunk == nullptr) {
            return BlockType::Air;
        }

        const int localX =
                positiveModulo(worldX, chunkWidth);

        const int localY =
                positiveModulo(worldY, chunkHeight);

        const int localZ =
                positiveModulo(worldZ, chunkDepth);

        return chunk->block(
            localX,
            localY,
            localZ
        );
    }
} // namespace bedrocked
