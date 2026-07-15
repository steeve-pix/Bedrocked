#include "bedrocked/world/World.hpp"

namespace bedrocked {
    void World::generateTestWorld() {
        Chunk &chunk = m_chunkManager.createChunk({0, 0, 0});

        for (int z{}; z < static_cast<int>(Chunk::Depth); ++z) {
            for (int x{}; x < static_cast<int>(Chunk::Width); ++x) {
                chunk.setBlock(x, 0, z, BlockType::Grass);
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
