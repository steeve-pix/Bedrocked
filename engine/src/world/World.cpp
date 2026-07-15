#include "bedrocked/world/World.hpp"

namespace bedrocked {
    void World::generateTestWorld() {
        for (int chunkZ = -1; chunkZ <= 1; ++chunkZ) {
            for (int chunkX = -1; chunkX <= 1; ++chunkX) {
                Chunk &chunk = m_chunkManager.createChunk({.x = chunkX, .y = 0, .z = chunkZ});

                for (int z{}; z < static_cast<int>(Chunk::Depth); ++z) {
                    for (int x{}; x < static_cast<int>(Chunk::Width); ++x) {
                        chunk.setBlock(x, 0, z, BlockType::Grass);
                    }
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
} // bedrocked
