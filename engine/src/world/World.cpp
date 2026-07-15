#include "bedrocked/world/World.hpp"
#include "bedrocked/world/generation/TerrainHeight.hpp"


namespace bedrocked {
    World::World(std::uint32_t seed) noexcept
        : m_terrainGenerator(seed) {
    }

    void World::generateTestWorld() {
        for (int chunkZ = -1; chunkZ <= 1; ++chunkZ) {
            for (int chunkX = -1; chunkX <= 1; ++chunkX) {
                const ChunkPosition position{.x = chunkX, .y = 0, .z = chunkZ};

                Chunk &chunk = m_chunkManager.createChunk(position);

                m_terrainGenerator.generate(chunk, position);
            }
        }
    }

    ChunkManager &World::chunks() noexcept {
        return m_chunkManager;
    }

    const ChunkManager &World::chunks() const noexcept {
        return m_chunkManager;
    }
} // bedrocked
