#include "bedrocked/world/World.hpp"
#include "bedrocked/world/generation/TreePlacement.hpp"


namespace {
    [[nodiscard]] constexpr int floorDivide(int value, int divisor) noexcept {
        int quotient = value / divisor;
        const int remainder = value % divisor;

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
        : m_seed(seed), m_terrainGenerator(seed) {
    }

    void World::generateTestWorld() {
        constexpr int minimumChunkX = -1;
        constexpr int maximumChunkX = 1;
        constexpr int minimumChunkZ = -1;
        constexpr int maximumChunkZ = 1;

        for (int chunkZ = minimumChunkZ; chunkZ <= maximumChunkZ; ++chunkZ) {
            for (int chunkX = minimumChunkX; chunkX <= maximumChunkX; ++chunkX) {
                const ChunkPosition position{.x = chunkX, .y = 0, .z = chunkZ};

                Chunk &chunk = m_chunkManager.createChunk(position);

                m_terrainGenerator.generate(chunk, position);
            }
        }

        const int minimumWorldX =
                minimumChunkX * static_cast<int>(Chunk::Width);

        constexpr int minimumWorldZ =
                -1 * static_cast<int>(Chunk::Depth);

        constexpr int maximumWorldZ =
                2 * static_cast<int>(Chunk::Depth) - 1;

        constexpr int maximumWorldX =
                2 * static_cast<int>(Chunk::Width) - 1;

        for (int worldZ = minimumWorldZ; worldZ <= maximumWorldZ; ++worldZ) {
            for (int worldX = 0;
                 worldX <= maximumWorldX;
                 ++worldX) {
                for (int worldY = 0;
                     worldY < static_cast<int>(Chunk::Height);
                     ++worldY) {
                    setBlockAtWorld(
                        worldX,
                        worldY,
                        worldZ,
                        BlockType::Air
                    );
                }
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
} // bedrocked
