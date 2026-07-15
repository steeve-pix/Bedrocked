#include "bedrocked/world/World.hpp"

#include "bedrocked/world/generation/TerrainHeight.hpp"

namespace bedrocked {
    void World::generateTestWorld() {
        for (int chunkZ = -1; chunkZ <= 1; ++chunkZ) {
            for (int chunkX = -1; chunkX <= 1; ++chunkX) {
                Chunk &chunk = m_chunkManager.createChunk({.x = chunkX, .y = 0, .z = chunkZ});

                for (int localZ{}; localZ < static_cast<int>(Chunk::Depth); ++localZ) {
                    for (int localX{}; localX < static_cast<int>(Chunk::Width); ++localX) {
                        const int worldX =
                                chunkX * static_cast<int>(Chunk::Width) + localX;

                        const int worldZ =
                                chunkZ * static_cast<int>(Chunk::Depth) + localZ;

                        const int surfaceHeight =
                                terrainHeight(worldX, worldZ);

                        for (int y{}; y <= surfaceHeight; ++y) {
                            BlockType blockType = BlockType::Stone;

                            if (y == surfaceHeight) {
                                blockType = BlockType::Grass;
                            } else if (y >= surfaceHeight - 3) {
                                blockType = BlockType::Dirt;
                            }

                            chunk.setBlock(localX, y, localZ, blockType);
                        }
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
